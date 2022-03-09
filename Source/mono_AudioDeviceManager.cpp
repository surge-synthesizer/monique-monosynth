#include "mono_AudioDeviceManager.h"

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
#include "monique_ui_MainWindow.h"
#include "monique_ui_MIDIIO.h"

//==============================================================================
//==============================================================================
//==============================================================================
static inline void show_feedback(MoniqueSynthData *synth_data_) noexcept
{
    juce::Array<Parameter *> &parameters = synth_data_->get_all_parameters();
    for (int i = 0; i != parameters.size(); ++i)
    {
        parameters.getUnchecked(i)->midi_control->send_feedback_only();
    }
}
void mono_AudioDeviceManager::clear_feedback() noexcept
{
    juce::Array<Parameter *> &parameters = get_synth_data()->get_all_parameters();
    for (int i = 0; i != parameters.size(); ++i)
    {
        parameters.getUnchecked(i)->midi_control->send_clear_feedback_only();
    }
}
void mono_AudioDeviceManager::clear_feedback_and_shutdown() noexcept
{
    open_state_checker.force_quit = true;
    open_state_checker.stopTimer();

    clear_feedback();
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD mono_AudioDeviceManager::AdvancedMidiInputCallback::AdvancedMidiInputCallback(
    mono_AudioDeviceManager *manager_) noexcept
    : manager(manager_), device_name(CLOSED_PORT)
{
}
COLD mono_AudioDeviceManager::AdvancedMidiInputCallback::~AdvancedMidiInputCallback() noexcept {}
COLD mono_AudioDeviceManager::MidiInputCallback_CC::MidiInputCallback_CC(
    mono_AudioDeviceManager *manager_) noexcept
    : AdvancedMidiInputCallback(manager_)
{
}
COLD mono_AudioDeviceManager::MidiInputCallback_CC::~MidiInputCallback_CC() noexcept {}
COLD mono_AudioDeviceManager::MidiInputCallback_NOTES::MidiInputCallback_NOTES(
    mono_AudioDeviceManager *manager_) noexcept
    : AdvancedMidiInputCallback(manager_)
{
}
COLD mono_AudioDeviceManager::MidiInputCallback_NOTES::~MidiInputCallback_NOTES() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
COLD mono_AudioDeviceManager::mono_AudioDeviceManager(
    RuntimeNotifyer *const runtime_notifyer_) noexcept
    : RuntimeListener(runtime_notifyer_), runtime_notifyer(runtime_notifyer_),

      main_input_thru(false, generate_param_name("MIDI", 1, "main_thru"),
                      generate_short_human_name("MIDI_main_thru")),
      cc_input_thru(false, generate_param_name("MIDI", 1, "cc_thru"),
                    generate_short_human_name("MIDI_cc_thru")),
      use_main_input_as_cc(true, generate_param_name("MIDI", 1, "cc_via_main"),
                           generate_short_human_name("MIDI_cc_via_main")),
      input_channel(MIN_MAX(0, 16),
                    0, // OMNI
                    generate_param_name("MIDI", 1, "input_channel"),
                    generate_short_human_name("MIDI_input_channel")),

      cc_input_callback(new MidiInputCallback_CC(this)),
      note_input_callback(new MidiInputCallback_NOTES(this)), note_input_state(CLOSED),
      cc_input_state(CLOSED),

      midi_thru_output(nullptr), midi_feedback_output(nullptr), midi_thru_output_state(CLOSED),
      midi_feedback_output_state(CLOSED),

      open_state_checker(this), state_change_counter(0),

      restored_all_devices(true), its_your_first_time(true),

      restored_audio_devices(true), init_first_time_audio_device(true)
{
    sample_rate_or_block_changed();
}

COLD mono_AudioDeviceManager::~mono_AudioDeviceManager() noexcept
{
    juce::StringArray devices = get_available_in_ports();
    for (int i = 0; i != devices.size(); ++i)
    {
        removeMidiInputCallback(devices[i], cc_input_callback);
        removeMidiInputCallback(devices[i], note_input_callback);
    }

    delete cc_input_callback;
    delete note_input_callback;

    if (midi_thru_output)
    {
        delete midi_thru_output;
        midi_thru_output = nullptr;
    }
    if (midi_feedback_output)
    {
        delete midi_feedback_output;
        midi_feedback_output = nullptr;
    }
}

//==============================================================================
COLD void mono_AudioDeviceManager::sample_rate_or_block_changed() noexcept
{
    cc_feedback_collector.reset(sample_rate);
    thru_collector.reset(sample_rate);
    cc_input_collector.reset(sample_rate);
    note_input_collector.reset(sample_rate);
    sync_input_collector.reset(sample_rate);
}

//==============================================================================
COLD bool mono_AudioDeviceManager::save_to(juce::XmlElement *xml_) const noexcept
{
    juce::XmlElement *audio_device_setup(AudioDeviceManager::createStateXml().release());

    bool success = false;
    if (audio_device_setup and xml_)
    {
        // AUDIO
        xml_->addChildElement(audio_device_setup);

        // INPUT
        xml_->setAttribute("noteInputDeviceName", note_input_callback->get_device_name());
        xml_->setAttribute("ccInputDeviceName", cc_input_callback->get_device_name());

        // OUTPU
        xml_->setAttribute("thruOutputDeviceName", midi_thru_name);
        xml_->setAttribute("feedbackOutputDeviceName", midi_feedback_name);

        // PARAMS
        write_parameter_to_file(*xml_, &main_input_thru);
        write_parameter_to_file(*xml_, &cc_input_thru);
        write_parameter_to_file(*xml_, &use_main_input_as_cc);
        write_parameter_to_file(*xml_, &input_channel);

        success = true;
    }
    return success;
}
COLD juce::String mono_AudioDeviceManager::read_from(const juce::XmlElement *xml_) noexcept
{
    juce::String error("ERROR: CAN'T RESTORE DEVICES ON LOAD");
    if (xml_)
    {
        its_your_first_time = false;

        // AUDIO
        {
            audio_device_init_backup = std::make_unique<juce::XmlElement>(*xml_);
            error = restore_audio_device(false);
        }

        // INPUT
        {
            juce::String note_device = xml_->getStringAttribute("noteInputDeviceName", "");
            if (note_device != "")
            {
                open_in_port(INPUT_ID::NOTES, note_device);
                if (note_device != CLOSED_PORT)
                {
                    if (not is_selected_in_device_open(INPUT_ID::NOTES))
                    {
                        restored_all_devices = false;
                    }
                }
            }
        }
        {
            juce::String cc_device = xml_->getStringAttribute("ccInputDeviceName", "");
            if (cc_device != "")
            {
                open_in_port(INPUT_ID::CC, cc_device);
                if (cc_device != CLOSED_PORT)
                {
                    if (not is_selected_in_device_open(INPUT_ID::CC))
                    {
                        restored_all_devices = false;
                    }
                }
            }
        }

        // OUTPU
        {
            juce::String thru_device = xml_->getStringAttribute("thruOutputDeviceName", "");
            if (thru_device != "")
            {
                open_out_port(OUTPUT_ID::THRU, thru_device);
                if (thru_device != CLOSED_PORT)
                {
                    if (not is_selected_out_device_open(OUTPUT_ID::THRU))
                    {
                        restored_all_devices = false;
                    }
                }
            }
        }
        {
            juce::String feedback_device = xml_->getStringAttribute("feedbackOutputDeviceName", "");
            if (feedback_device != "")
            {
                open_out_port(OUTPUT_ID::FEEDBACK, feedback_device);
                if (feedback_device != CLOSED_PORT)
                {
                    if (not is_selected_out_device_open(OUTPUT_ID::FEEDBACK))
                    {
                        restored_all_devices = false;
                    }
                }
            }
        }

        // PARAMS
        read_parameter_from_file(*xml_, &main_input_thru);
        read_parameter_from_file(*xml_, &cc_input_thru);
        read_parameter_from_file(*xml_, &use_main_input_as_cc);
        read_parameter_from_file(*xml_, &input_channel);
    }

    state_change_counter++;

    return error;
}
COLD juce::String
mono_AudioDeviceManager::restore_audio_device(bool try_to_open_an_alternativ_) noexcept
{
    juce::String error;
    const juce::OwnedArray<juce::AudioIODeviceType> &types = getAvailableDeviceTypes();
    error = AudioDeviceManager::initialise(
        0, 2, audio_device_init_backup->getChildByName("DEVICESETUP"), try_to_open_an_alternativ_);
    juce::AudioIODevice *active_device = getCurrentAudioDevice();
    if (error != "" or not active_device)
    {
        restored_audio_devices = false;
        if (error == "")
        {
            error = "Error open device.";
        }
    }
    else
    {
        restored_audio_devices = true;

        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        getAudioDeviceSetup(current_device_setup);

        runtime_notifyer->set_block_size(current_device_setup.bufferSize);
        runtime_notifyer->set_sample_rate(current_device_setup.sampleRate);
    }

    return error;
}
COLD juce::String mono_AudioDeviceManager::read_defaults() noexcept
{
    juce::String error("ERROR: CAN'T OPEN ANY DEFAULT DEVICE.");
    // AUDIO
    {
        const juce::OwnedArray<juce::AudioIODeviceType> &types = getAvailableDeviceTypes();
        for (int i = 0; i != types.size(); ++i)
        {
            juce::AudioIODeviceType *type = types.getUnchecked(i);
            setCurrentAudioDeviceType(type->getTypeName(), false);
            type->scanForDevices();
            error = AudioDeviceManager::initialise(0, 2, nullptr, false);
            juce::AudioIODevice *active_device = getCurrentAudioDevice();
            if (error == "" and active_device)
            {
                {
                    static bool fix_oss_port_issue = false;
                    jassert(fix_oss_port_issue);
                    fix_oss_port_issue = true;
                }
                /*
                 AudioDeviceManager::updateXml();

                 AudioDeviceManager::AudioDeviceSetup current_device_setup;
                 getAudioDeviceSetup( current_device_setup );

                 runtime_notifyer->set_block_size( current_device_setup.bufferSize );
                 runtime_notifyer->set_sample_rate( current_device_setup.sampleRate );
 */
                break;
            }
            else
            {
                if (error == "")
                {
                    error = "Error open device.";
                }
                init_first_time_audio_device = false;
            }
        }
    }

    state_change_counter++;

    return error;
}

COLD void mono_AudioDeviceManager::save() const noexcept
{
    juce::File folder = GET_ROOT_FOLDER();
    folder = juce::File(folder.getFullPathName() + PROJECT_FOLDER);
    if (folder.createDirectory())
    {
        juce::File device_file(
            juce::File(folder.getFullPathName() + juce::String("/") + "devices.mcfg"));

        juce::XmlElement xml("DEVICES-1.0");
        if (save_to(&xml))
        {
            xml.writeTo(device_file, {});
        }
    }
}
COLD juce::String mono_AudioDeviceManager::read() noexcept
{
#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init audio" << std::endl;
#endif

    juce::File folder = GET_ROOT_FOLDER();
    juce::File device_file =
        juce::File(folder.getFullPathName() + PROJECT_FOLDER + juce::String("devices.mcfg"));

    juce::String error;
    if (auto xml = juce::XmlDocument(device_file).getDocumentElement().release())
    {
        if (xml->hasTagName("DEVICES-1.0"))
        {
            error = read_from(xml);
        }
        else
        {
            error = "WRONG DEVICE FILE VERSION";
        }
    }
    else
    {
        error = read_defaults();
    }

    state_change_counter++;

    return error;
}

//==============================================================================
//==============================================================================
//==============================================================================
void mono_AudioDeviceManager::AdvancedMidiInputCallback::set_device_name(
    const juce::String &name_) noexcept
{
    device_name = name_;
}
const juce::String &
mono_AudioDeviceManager::AdvancedMidiInputCallback::get_device_name() const noexcept
{
    return device_name;
}

//==============================================================================
void mono_AudioDeviceManager::MidiInputCallback_CC::handleIncomingMidiMessage(
    juce::MidiInput *, const juce::MidiMessage &message)
{
    manager->collect_incoming_midi_messages(INPUT_ID::CC, message);
}

//==============================================================================
void mono_AudioDeviceManager::MidiInputCallback_NOTES::handleIncomingMidiMessage(
    juce::MidiInput *, const juce::MidiMessage &message)
{
    manager->collect_incoming_midi_messages(INPUT_ID::NOTES, message);
}

//==============================================================================
//==============================================================================
//==============================================================================
void mono_AudioDeviceManager::collect_incoming_midi_messages(
    mono_AudioDeviceManager::INPUT_ID input_id_, const juce::MidiMessage &midi_message_) noexcept
{
    switch (input_id_)
    {
    case INPUT_ID::CC:
    {
        if (midi_message_.isPitchWheel())
        {
            juce::MidiMessage message =
                juce::MidiMessage::pitchWheel(1, midi_message_.getPitchWheelValue());
            message.setTimeStamp(midi_message_.getTimeStamp());
            cc_input_collector.addMessageToQueue(message);
        }
        else if (midi_message_.isController())
        {
            juce::MidiMessage message = juce::MidiMessage::controllerEvent(
                1, midi_message_.getControllerNumber(), midi_message_.getControllerValue());
            message.setTimeStamp(midi_message_.getTimeStamp());
            cc_input_collector.addMessageToQueue(message);
        }
        else if (midi_message_.isNoteOn())
        {
            juce::MidiMessage message =
                juce::MidiMessage::controllerEvent(2, midi_message_.getNoteNumber(), 0);
            message.setTimeStamp(midi_message_.getTimeStamp());
            cc_input_collector.addMessageToQueue(message);
        }

        // THRU
        if (cc_input_thru)
        {
            thru_collector.addMessageToQueue(midi_message_);
        }
    }
    break;
    case INPUT_ID::NOTES:
    {
        if (not midi_message_.isPitchWheel())
        {
            if (midi_message_.isMidiClock())
            {
                sync_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isMidiStart())
            {
                sync_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isMidiStop())
            {
                sync_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isMidiContinue())
            {
                sync_input_collector.addMessageToQueue(midi_message_);
            }
        }

        if (input_channel == 0 or
            midi_message_.getChannel() == input_channel.get_value()) // 0 = OMNI
        {
            if (midi_message_.isPitchWheel())
            {
                juce::MidiMessage message =
                    juce::MidiMessage::pitchWheel(1, midi_message_.getPitchWheelValue());
                message.setTimeStamp(midi_message_.getTimeStamp());
            }
            else if (midi_message_.isController())
            {
                juce::MidiMessage message = juce::MidiMessage::controllerEvent(
                    1, midi_message_.getControllerNumber(), midi_message_.getControllerValue());
                message.setTimeStamp(midi_message_.getTimeStamp());
            }
            // ELSE IF!!!!
            if (midi_message_.isNoteOnOrOff())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isAllNotesOff())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isProgramChange())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isSustainPedalOn())
            {
                note_input_collector.addMessageToQueue(midi_message_);
                // BIND PEDALS OPTION
                /*
                        if( get_synth_data()->bind_sustain_and_sostenuto_pedal )
                        {
                    MidiMessage message = MidiMessage::controllerEvent( 1, 66,
                   midi_message_.getControllerValue() ); message.setTimeStamp(
                   midi_message_.getTimeStamp() ); note_input_collector.addMessageToQueue( message
                   );
                        }
                        */
            }
            else if (midi_message_.isSustainPedalOff())
            {
                note_input_collector.addMessageToQueue(midi_message_);
                // BIND PEDALS OPTION
                /*
                        if( get_synth_data()->bind_sustain_and_sostenuto_pedal )
                        {
                    MidiMessage message = MidiMessage::controllerEvent( 1, 66,
                   midi_message_.getControllerValue() ); message.setTimeStamp(
                   midi_message_.getTimeStamp() ); note_input_collector.addMessageToQueue( message
                   );
                        }
                        */
            }
            else if (midi_message_.isSostenutoPedalOn())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isSostenutoPedalOff())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isSoftPedalOn())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isSoftPedalOff())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isPitchWheel())
            {
                note_input_collector.addMessageToQueue(midi_message_);
            }
            else if (midi_message_.isController())
            {
                if (use_main_input_as_cc)
                {
                    cc_input_collector.addMessageToQueue(midi_message_);
                }
            }
        }

        // THRU
        if (main_input_thru)
        {
            thru_collector.addMessageToQueue(midi_message_);
        }
    }
    break;
    }
}

//==============================================================================
mono_AudioDeviceManager::AdvancedMidiInputCallback *
mono_AudioDeviceManager::get_input_device_callback(
    mono_AudioDeviceManager::INPUT_ID input_id_) const noexcept
{
    switch (input_id_)
    {
    case INPUT_ID::CC:
        return cc_input_callback;
    case INPUT_ID::NOTES:
        return note_input_callback;
    }
}

//==============================================================================
juce::StringArray mono_AudioDeviceManager::get_available_in_ports() const noexcept
{
    return juce::MidiInput::getDevices();
}
void mono_AudioDeviceManager::open_in_port(mono_AudioDeviceManager::INPUT_ID input_id_,
                                           const juce::String &device_name_) noexcept
{
    close_in_port(input_id_);

    AdvancedMidiInputCallback *midi_callback(get_input_device_callback(input_id_));
    if (device_name_ != CLOSED_PORT)
    {
        setMidiInputEnabled(device_name_, true);
        addMidiInputCallback(device_name_, midi_callback);

        switch (input_id_)
        {
        case INPUT_ID::CC:
        {
            cc_input_state = isMidiInputEnabled(device_name_) ? OPEN : ERROR;
        }
        break;
        case INPUT_ID::NOTES:
        {
            note_input_state = isMidiInputEnabled(device_name_) ? OPEN : ERROR;
        }
        break;
        }
    }
    else
    {
        switch (input_id_)
        {
        case INPUT_ID::CC:
        {
            cc_input_state = CLOSED;
        }
        break;
        case INPUT_ID::NOTES:
        {
            note_input_state = CLOSED;
        }
        break;
        }
    }

    midi_callback->set_device_name(device_name_);

    state_change_counter++;
}
void mono_AudioDeviceManager::close_in_port(mono_AudioDeviceManager::INPUT_ID input_id_) noexcept
{
    AdvancedMidiInputCallback *const midi_callback(get_input_device_callback(input_id_));
    removeMidiInputCallback(midi_callback->get_device_name(), midi_callback);
    setMidiInputEnabled(midi_callback->get_device_name(), false);

    midi_callback->set_device_name(CLOSED_PORT);

    switch (input_id_)
    {
    case INPUT_ID::CC:
    {
        cc_input_state = CLOSED;
    }
    break;
    case INPUT_ID::NOTES:
    {
        note_input_state = CLOSED;
    }
    break;
    }

    state_change_counter++;
}
juce::String mono_AudioDeviceManager::get_selected_in_device(
    mono_AudioDeviceManager::INPUT_ID input_id_) const noexcept
{
    return get_input_device_callback(input_id_)->get_device_name();
}
COLD bool mono_AudioDeviceManager::is_selected_in_device_open(
    mono_AudioDeviceManager::INPUT_ID input_id_) const noexcept
{
    switch (input_id_)
    {
    case INPUT_ID::CC:
    {
        return isMidiInputEnabled(cc_input_callback->get_device_name()) and cc_input_state == OPEN;
    }
    case INPUT_ID::NOTES:
    {
        return isMidiInputEnabled(note_input_callback->get_device_name()) and
               note_input_state == OPEN;
    }
    }

    return false;
}
COLD mono_AudioDeviceManager::DEVICE_STATE
mono_AudioDeviceManager::get_selected_in_device_state(INPUT_ID input_id_) const noexcept
{
    switch (input_id_)
    {
    case INPUT_ID::CC:
    {
        return cc_input_state;
    }
    case INPUT_ID::NOTES:
    {
        return note_input_state;
    }
    }

    return DEVICE_STATE::ERROR;
}

//==============================================================================
void mono_AudioDeviceManager::send_thru_messages(int num_samples_) noexcept
{
    juce::MidiBuffer midi_messages;
    thru_collector.removeNextBlockOfMessages(midi_messages, num_samples_);
    if (midi_thru_output)
    {
        midi_thru_output->sendBlockOfMessages(
            midi_messages, juce::Time::getMillisecondCounterHiRes(), sample_rate);
    }
}
void mono_AudioDeviceManager::send_feedback_messages(int num_samples_) noexcept
{
    juce::MidiBuffer midi_messages;
    cc_feedback_collector.removeNextBlockOfMessages(midi_messages, num_samples_);
    if (midi_feedback_output)
    {
        midi_feedback_output->sendBlockOfMessages(
            midi_messages, juce::Time::getMillisecondCounterHiRes(), sample_rate);
    }
}

//==============================================================================
juce::MidiOutput *mono_AudioDeviceManager::get_output_device(
    mono_AudioDeviceManager::OUTPUT_ID output_id_) const noexcept
{
    switch (output_id_)
    {
    case OUTPUT_ID::THRU:
    {
        return midi_thru_output;
    }
    case OUTPUT_ID::FEEDBACK:
    {
        return midi_feedback_output;
    }
    }
}
juce::StringArray mono_AudioDeviceManager::get_available_out_ports() const noexcept
{
    return juce::MidiOutput::getDevices();
}
//==============================================================================
//==============================================================================
//==============================================================================
void mono_AudioDeviceManager::close_out_port(OUTPUT_ID output_id_) noexcept
{
    switch (output_id_)
    {
    case OUTPUT_ID::FEEDBACK:
    {
        if (midi_feedback_output)
        {
            clear_feedback();

            midi_feedback_output->stopBackgroundThread();
            delete midi_feedback_output;
            midi_feedback_output = nullptr;
        }
        midi_feedback_name = "";
        midi_feedback_output_state = CLOSED;
        break;
    }
    case OUTPUT_ID::THRU:
    {
        if (midi_thru_output)
        {
            midi_thru_output->stopBackgroundThread();
            delete midi_thru_output;
            midi_thru_output = nullptr;
        }
        midi_thru_name = "";
        midi_thru_output_state = CLOSED;
        break;
    }
    }

    state_change_counter++;
}
bool mono_AudioDeviceManager::open_out_port(mono_AudioDeviceManager::OUTPUT_ID output_id_,
                                            const juce::String &device_name_) noexcept
{
    // CLOSE
    close_out_port(output_id_);

    // OPEN
    juce::MidiOutput *output =
        juce::MidiOutput::openDevice(get_available_out_ports().indexOf(device_name_)).release();
    {
        switch (output_id_)
        {
        case OUTPUT_ID::FEEDBACK:
        {
            midi_feedback_output = output;
            midi_feedback_name = device_name_;

            if (output)
            {
                output->startBackgroundThread();
                show_feedback(get_synth_data());
                midi_feedback_output_state = OPEN;
            }
            else if (device_name_ == CLOSED_PORT)
            {
                midi_feedback_output_state = CLOSED;
            }
            else
            {
                midi_feedback_output_state = ERROR;
            }

            break;
        }
        case OUTPUT_ID::THRU:
        {
            midi_thru_output = output;
            midi_thru_name = device_name_;

            if (output)
            {
                output->startBackgroundThread();
                midi_thru_output_state = OPEN;
            }
            else if (device_name_ == CLOSED_PORT)
            {
                midi_thru_output_state = CLOSED;
            }
            else
            {
                midi_thru_output_state = ERROR;
            }
            break;
        }
        }
    }

    state_change_counter++;

    return output;
}
juce::String mono_AudioDeviceManager::get_selected_out_device(OUTPUT_ID output_id_) const noexcept
{
    switch (output_id_)
    {
    case OUTPUT_ID::FEEDBACK:
    {
        return midi_feedback_name;
    }
    default: // OUTPUT_ID::THRU :
    {
        return midi_thru_name;
    }
    }
}
COLD bool mono_AudioDeviceManager::is_selected_out_device_open(OUTPUT_ID output_id_) const noexcept
{
    switch (output_id_)
    {
    case OUTPUT_ID::FEEDBACK:
    {
        return midi_feedback_output and midi_feedback_output_state == OPEN;
    }
    case OUTPUT_ID::THRU:
    {
        return midi_thru_output and midi_thru_output_state == OPEN;
    }
    }

    return false;
}
mono_AudioDeviceManager::DEVICE_STATE
mono_AudioDeviceManager::get_selected_out_device_state(OUTPUT_ID output_id_) const noexcept
{
    switch (output_id_)
    {
    case OUTPUT_ID::FEEDBACK:
    {
        return midi_feedback_output_state;
    }
    case OUTPUT_ID::THRU:
    {
        return midi_thru_output_state;
    }
    }

    return DEVICE_STATE::ERROR;
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD mono_AudioDeviceManager::OpenStateChecker::OpenStateChecker(
    mono_AudioDeviceManager *const manager_) noexcept
    : manager(manager_),

      force_quit(false),

      ignore_note_input(false), ignore_cc_input(false), ignore_thru_output(false),
      ignore_cc_output(false),

      was_open_note_input(false), was_open_cc_input(false), was_open_thru_output(false),
      was_open_cc_output(false),

      connection_lost_note_input(false), connection_lost_cc_input(false),
      connection_lost_thru_output(false), connection_lost_cc_output(false)
{
    startTimer(1000);
}
COLD mono_AudioDeviceManager::OpenStateChecker::~OpenStateChecker() noexcept {}

COLD void mono_AudioDeviceManager::OpenStateChecker::timerCallback()
{
    stopTimer();

    Monique_Ui_Mainwindow *const editor = manager->get_editor();

    juce::StringArray in_devices = manager->get_available_in_ports();
    if (last_in_devices != in_devices)
    {
        last_in_devices = in_devices;
        manager->state_change_counter++;

        if (editor)
        {
            editor->flash_midi_editor_button();
        }

        if (manager->its_your_first_time)
        {
            manager->its_your_first_time = false;

            bool success =
#ifndef IS_MOBILE
                false;
            {
                static bool fix_oss_port_issue = false;
                jassert(fix_oss_port_issue);
                fix_oss_port_issue = true;
            }
            /*
            AlertWindow::showOkCancelBox
                           (
                               AlertWindow::AlertIconType::QuestionIcon,
                               "MIDI DEVICES DETECTED.",
                               "Monique found a MIDI input device. May be your piano keyboard.\n"
                               "Do you like to connect it?\n"
                               "\n"
                               "(a keyboard you can select at 'INPUT (Notes...)').",
                               "YES", "NO"
                           );*/
#else
                false;
#endif
            if (force_quit)
            {
                return;
            }
            if (editor and success)
            {
                editor->open_midi_editor_if_closed();
            }
        }
    }

    // NOTES IN
    {
        juce::String selected_note_in_device(manager->get_selected_in_device(INPUT_ID::NOTES));
        if (selected_note_in_device != CLOSED_PORT)
        {
            if (selected_note_in_device != CLOSED_PORT)
            {
                const bool is_open(manager->is_selected_in_device_open(INPUT_ID::NOTES));
                const bool is_connected(in_devices.contains(selected_note_in_device));
                if ((not is_open and is_connected) or (connection_lost_note_input and is_connected))
                {
                    if (not ignore_note_input)
                    {
                        if (editor)
                        {
                            editor->open_midi_editor_if_closed();
                        }
                        {
                            static bool fix_oss_port_issue = false;
                            jassert(fix_oss_port_issue);
                            fix_oss_port_issue = true;
                        }
                        bool success = false; /*AlertWindow::showOkCancelBox
                                       (
                                           AlertWindow::AlertIconType::QuestionIcon,
                                           "MIDI IN DEVICE CONNECTED.",
                                           "Monique found a previously connected MIDI NOTE IN
                                       port.\n" "Do you like to reactivate that port: " +
                                       selected_note_in_device + "?", "YES","NO"
                                       );*/
                        if (force_quit)
                        {
                            return;
                        }
                        if (success)
                        {
                            manager->open_in_port(INPUT_ID::NOTES, selected_note_in_device);
                        }
                        ignore_note_input = not success;

                        was_open_note_input = manager->is_selected_in_device_open(INPUT_ID::NOTES);
                        connection_lost_note_input = not was_open_note_input;

                        manager->state_change_counter++;
                    }
                }
                else if (is_open and is_connected)
                {
                    was_open_note_input = true;
                    connection_lost_note_input = false;
                    ignore_note_input = false;
                }
                else if (was_open_note_input and not is_connected)
                {
                    manager->note_input_state = REMOVED;
                    was_open_note_input = false;
                    connection_lost_note_input = true;
                    juce::AlertWindow::showMessageBoxAsync(
                        juce::AlertWindow::AlertIconType::WarningIcon, "MIDI IN DEVICE REMOVED.",
                        "Monique lost the MIDI NOTE IN connection to: " + selected_note_in_device +
                            ".\n"
                            "Please reconnect the device or select another one.");
                    if (force_quit)
                    {
                        return;
                    }
                    if (editor)
                    {
                        editor->open_midi_editor_if_closed();
                    }

                    manager->state_change_counter++;
                }
            }
        }
    }

    // CC IN
    {
        juce::String selected_cc_in_device(manager->get_selected_in_device(INPUT_ID::CC));
        if (selected_cc_in_device != CLOSED_PORT)
        {
            if (selected_cc_in_device != CLOSED_PORT)
            {
                const bool is_open(manager->is_selected_in_device_open(INPUT_ID::CC));
                const bool is_connected(in_devices.contains(selected_cc_in_device));
                if ((not is_open and is_connected) or (connection_lost_cc_input and is_connected))
                {
                    if (not ignore_cc_input)
                    {
                        if (editor)
                        {
                            editor->open_midi_editor_if_closed();
                        }
                        {
                            static bool fix_oss_port_issue = false;
                            jassert(fix_oss_port_issue);
                            fix_oss_port_issue = true;
                        }
                        bool success = false; /* AlertWindow::showOkCancelBox
                                       (
                                           AlertWindow::AlertIconType::QuestionIcon,
                                           "MIDI IN DEVICE CONNECTED.",
                                           "Monique found a previously connected MIDI CC IN port.\n"
                                           "Do you like to reactivate that port: " +
                                       selected_cc_in_device + "?", "YES", "NO"
                                       ); */
                        if (force_quit)
                        {
                            return;
                        }
                        if (success)
                        {
                            manager->open_in_port(INPUT_ID::CC, selected_cc_in_device);
                        }
                        ignore_cc_input = not success;

                        was_open_cc_input = manager->is_selected_in_device_open(INPUT_ID::CC);
                        connection_lost_cc_input = not was_open_cc_input;

                        manager->state_change_counter++;
                    }
                }
                else if (is_open and is_connected)
                {
                    was_open_cc_input = true;
                    connection_lost_cc_input = false;
                    ignore_cc_input = false;
                }
                else if (was_open_cc_input and not is_connected)
                {
                    manager->cc_input_state = REMOVED;
                    was_open_cc_input = false;
                    connection_lost_cc_input = true;
                    {
                        static bool fix_oss_port_issue = false;
                        jassert(fix_oss_port_issue);
                        fix_oss_port_issue = true;
                    }
                    /*AlertWindow::showMessageBoxAsync
                    (
                        AlertWindow::AlertIconType::WarningIcon,
                        "MIDI IN DEVICE REMOVED.",
                        "Monique lost the MIDI CC IN connection to: " + selected_cc_in_device +
                    ".\n" "Please reconnect the device or select another one."
                    );*/
                    if (force_quit)
                    {
                        return;
                    }
                    if (editor)
                    {
                        editor->open_midi_editor_if_closed();
                    }

                    manager->state_change_counter++;
                }
            }
        }
    }

    juce::StringArray out_devices = manager->get_available_out_ports();
    if (last_out_devices != out_devices)
    {
        last_out_devices = out_devices;
        manager->state_change_counter++;

        if (editor)
        {
            editor->flash_midi_editor_button();
        }
    }

    // THRU OUT
    {
        juce::String selected_thru_out_device(manager->get_selected_out_device(OUTPUT_ID::THRU));
        if (selected_thru_out_device != CLOSED_PORT)
        {
            const bool is_open(manager->is_selected_out_device_open(OUTPUT_ID::THRU));
            const bool is_connected(out_devices.contains(selected_thru_out_device));
            if ((not is_open and is_connected) or (connection_lost_thru_output and is_connected))
            {
                if (not ignore_thru_output)
                {
                    if (editor)
                    {
                        editor->open_midi_editor_if_closed();
                    }
                    {
                        static bool fix_oss_port_issue = false;
                        jassert(fix_oss_port_issue);
                        fix_oss_port_issue = true;
                    }
                    bool success = false; /*AlertWindow::showOkCancelBox
                                   (
                                       AlertWindow::AlertIconType::QuestionIcon,
                                       "MIDI OUT DEVICE CONNECTED.",
                                       "Monique found a previously connected MIDI THRU OUT port.\n"
                                       "Do you like to reactivate that port: " +
                                   selected_thru_out_device + "?", "YES", "NO"
                                   );*/
                    if (force_quit)
                    {
                        return;
                    }
                    if (success)
                    {
                        manager->open_out_port(OUTPUT_ID::THRU, selected_thru_out_device);
                    }
                    ignore_thru_output = not success;

                    was_open_thru_output = manager->is_selected_out_device_open(OUTPUT_ID::THRU);
                    connection_lost_thru_output = not was_open_thru_output;

                    manager->state_change_counter++;
                }
            }
            else if (is_open and is_connected)
            {
                was_open_thru_output = true;
                connection_lost_thru_output = false;
                ignore_thru_output = false;
            }
            else if (was_open_thru_output and not is_connected)
            {
                manager->midi_thru_output_state = REMOVED;
                was_open_thru_output = false;
                connection_lost_thru_output = true;
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::AlertIconType::WarningIcon, "MIDI OUT DEVICE REMOVED.",
                    "Monique lost the MIDI THRU OUT connection to: " + selected_thru_out_device +
                        ".\n"
                        "Please reconnect the device or select another one.");
                if (force_quit)
                {
                    return;
                }
                if (editor)
                {
                    editor->open_midi_editor_if_closed();
                }

                manager->state_change_counter++;
            }
        }
    }

    // CC OUT
    {
        juce::String selected_feedback_out_device(
            manager->get_selected_out_device(OUTPUT_ID::FEEDBACK));
        if (selected_feedback_out_device != CLOSED_PORT)
        {
            const bool is_open(manager->is_selected_out_device_open(OUTPUT_ID::FEEDBACK));
            const bool is_connected(out_devices.contains(selected_feedback_out_device));
            if ((not is_open and is_connected) or (connection_lost_cc_output and is_connected))
            {
                if (not ignore_cc_output)
                {
                    if (editor)
                    {
                        editor->open_midi_editor_if_closed();
                    }
                    {
                        static bool fix_oss_port_issue = false;
                        jassert(fix_oss_port_issue);
                        fix_oss_port_issue = true;
                    }
                    bool success = false; /* AlertWindow::showOkCancelBox
                                   (
                                       AlertWindow::AlertIconType::QuestionIcon,
                                       "MIDI OUT DEVICE CONNECTED.",
                                       "Monique found a previously connected MIDI FEEDBACK OUT
                                   port.\n" "Do you like to reactivate that port: " +
                                   selected_feedback_out_device + "?", "YES", "NO"
                                   ); */
                    if (success)
                    {
                        manager->open_out_port(OUTPUT_ID::FEEDBACK, selected_feedback_out_device);
                    }
                    if (force_quit)
                    {
                        return;
                    }
                    ignore_cc_output = not success;

                    was_open_cc_output = manager->is_selected_out_device_open(OUTPUT_ID::FEEDBACK);
                    connection_lost_cc_output = not was_open_cc_output;

                    manager->state_change_counter++;
                }
            }
            else if (is_open and is_connected)
            {
                was_open_cc_output = true;
                connection_lost_cc_output = false;
                ignore_cc_output = false;
            }
            else if (was_open_cc_output and not is_connected)
            {
                manager->midi_feedback_output_state = REMOVED;
                was_open_cc_output = false;
                connection_lost_cc_output = true;
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::AlertIconType::WarningIcon, "MIDI OUT DEVICE REMOVED.",
                    "Monique lost the MIDI FEEDBACK OUT connection to: " +
                        selected_feedback_out_device +
                        ".\n"
                        "Please reconnect the device or select another one.");
                if (force_quit)
                {
                    return;
                }
                if (editor)
                {
                    editor->open_midi_editor_if_closed();
                }

                manager->state_change_counter++;
            }
        }
    }

    // RESTART IF NOT ALL IGNORED
    if (not ignore_note_input or not ignore_note_input or not ignore_note_input or
        not ignore_note_input)
    {
        startTimer(1000);
    }
}

#else
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
COLD mono_AudioDeviceManager::mono_AudioDeviceManager(
    RuntimeNotifyer *const runtime_notifyer_) noexcept
    : RuntimeListener(runtime_notifyer_), runtime_notifyer(runtime_notifyer_)
{
    sample_rate_or_block_changed();
}

COLD mono_AudioDeviceManager::~mono_AudioDeviceManager() noexcept {}

//==============================================================================
COLD void mono_AudioDeviceManager::sample_rate_or_block_changed() noexcept
{
    cc_feedback_collector.reset(sample_rate);
}

COLD void mono_AudioDeviceManager::clear_feedback_and_shutdown() noexcept
{
    juce::Array<Parameter *> &parameters = get_synth_data()->get_all_parameters();
    for (int i = 0; i != parameters.size(); ++i)
    {
        parameters.getUnchecked(i)->midi_control->send_clear_feedback_only();
    }
}
#endif
