/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
*/

#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING

#include "monique_ui_MainWindow.h"
#include "monique_ui_LookAndFeel.h"

#include "core/mono_AudioDeviceManager.h"

#include "monique_ui_MIDIIO.h"

void Monique_Ui_MidiIO::refresh() noexcept
{
    toggle_input_main_thru->setToggleState(_audio_device_manager->main_input_thru,
                                           dontSendNotification);
    toggle_input_main_cc->setToggleState(_audio_device_manager->use_main_input_as_cc,
                                         dontSendNotification);
    toggle_input_cc_thru->setToggleState(_audio_device_manager->cc_input_thru,
                                         dontSendNotification);

    int state_change_counter = _audio_device_manager->get_state_change_counter();
    if (state_change_counter != last_state_change)
    {
        last_state_change = state_change_counter;
        update_combo_boxed();
    }

    TURN_BUTTON_ON_OR_OFF(button_midi_learn, midi_control_handler->is_waiting_for_param() ? true
                                             : midi_control_handler->is_learning()        ? true
                                                                                          : false);
}
void Monique_Ui_MidiIO::update_combo_boxed()
{
    // INPUT
    {
        StringArray input_devices = _audio_device_manager->get_available_in_ports();

        // NOTE INPUT
        {
            String selected_item_text = combo_input_main->getText();
            combo_input_main->clear(dontSendNotification);
            combo_input_main->addItemList(input_devices, 1);
            combo_input_main->addItem(CLOSED_PORT, 1 + input_devices.size());

            // SELECT THE CURRENT ONE
            String selected_note_in_device(_audio_device_manager->get_selected_in_device(
                mono_AudioDeviceManager::INPUT_ID::NOTES));
            const mono_AudioDeviceManager::DEVICE_STATE state(
                _audio_device_manager->get_selected_in_device_state(
                    mono_AudioDeviceManager::INPUT_ID::NOTES));
            switch (state)
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN:
                combo_input_main->setColour(ComboBox::backgroundColourId,
                                            Colours::green.withAlpha(0.3f));
                combo_input_main->setSelectedItemIndex(
                    input_devices.indexOf(selected_note_in_device), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED:
                combo_input_main->setColour(ComboBox::backgroundColourId,
                                            Colours::orange.withAlpha(0.2f));
                combo_input_main->setSelectedItemIndex(input_devices.size(), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED:
                combo_input_main->setColour(ComboBox::backgroundColourId,
                                            Colours::red.withAlpha(0.5f));
                combo_input_main->setText("REMOVED: " + selected_note_in_device,
                                          dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR:
                combo_input_main->setColour(ComboBox::backgroundColourId,
                                            Colours::red.withAlpha(0.2f));
                combo_input_main->setText("ERROR: " + selected_note_in_device,
                                          dontSendNotification);
                break;
            }
        }

        // CC INPUT
        {
            String selected_item_text = combo_input_cc->getText();
            combo_input_cc->clear(dontSendNotification);
            combo_input_cc->addItemList(input_devices, 1);
            combo_input_cc->addItem(CLOSED_PORT, 1 + input_devices.size());

            // SELECT THE CURRENT ONE
            String selected_cc_in_device(_audio_device_manager->get_selected_in_device(
                mono_AudioDeviceManager::INPUT_ID::CC));
            const mono_AudioDeviceManager::DEVICE_STATE state(
                _audio_device_manager->get_selected_in_device_state(
                    mono_AudioDeviceManager::INPUT_ID::CC));
            switch (state)
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN:
                combo_input_cc->setColour(ComboBox::backgroundColourId,
                                          Colours::green.withAlpha(0.3f));
                combo_input_cc->setSelectedItemIndex(input_devices.indexOf(selected_cc_in_device),
                                                     dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED:
                combo_input_cc->setColour(ComboBox::backgroundColourId,
                                          Colours::orange.withAlpha(0.2f));
                combo_input_cc->setSelectedItemIndex(input_devices.size(), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED:
                combo_input_cc->setColour(ComboBox::backgroundColourId,
                                          Colours::red.withAlpha(0.5f));
                combo_input_cc->setText("REMOVED: " + selected_cc_in_device, dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR:
                combo_input_cc->setColour(ComboBox::backgroundColourId,
                                          Colours::red.withAlpha(0.2f));
                combo_input_cc->setText("ERROR: " + selected_cc_in_device, dontSendNotification);
                break;
            }
        }

        // INPUT CHANNEL
        int current_channel = _audio_device_manager->input_channel;
        combo_input_main_channel->clear(dontSendNotification);
        {
            combo_input_main_channel->addItem("OMNI", 1);
            for (int i = 0; i != 16; ++i)
            {
                combo_input_main_channel->addItem(String(i + 1), i + 2);
            }
            combo_input_main_channel->setSelectedItemIndex(current_channel, dontSendNotification);
        }
    }

    // OUTPUT
    {
        StringArray output_devices = _audio_device_manager->get_available_out_ports();

        // CC OUTPUT
        {
            String selected_item_text = combo_output_cc->getText();
            combo_output_cc->clear(dontSendNotification);
            combo_output_cc->addItemList(output_devices, 1);
            combo_output_cc->addItem(CLOSED_PORT, 1 + output_devices.size());

            // SELECT THE CURRENT ONE
            String selected_cc_out_device(_audio_device_manager->get_selected_out_device(
                mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK));
            const mono_AudioDeviceManager::DEVICE_STATE state(
                _audio_device_manager->get_selected_out_device_state(
                    mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK));
            switch (state)
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN:
                combo_output_cc->setColour(ComboBox::backgroundColourId,
                                           Colours::green.withAlpha(0.3f));
                combo_output_cc->setSelectedItemIndex(
                    output_devices.indexOf(selected_cc_out_device), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED:
                combo_output_cc->setColour(ComboBox::backgroundColourId,
                                           Colours::orange.withAlpha(0.2f));
                combo_output_cc->setSelectedItemIndex(output_devices.size(), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED:
                combo_output_cc->setColour(ComboBox::backgroundColourId,
                                           Colours::red.withAlpha(0.5f));
                combo_output_cc->setText("REMOVED: " + selected_cc_out_device,
                                         dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR:
                combo_output_cc->setColour(ComboBox::backgroundColourId,
                                           Colours::red.withAlpha(0.2f));
                combo_output_cc->setText("ERROR: " + selected_cc_out_device, dontSendNotification);
                break;
            }
        }

        // THRU OUTPUT
        {
            String selected_item_text = combo_output_thru->getText();
            combo_output_thru->clear(dontSendNotification);
            combo_output_thru->addItemList(output_devices, 1);
            combo_output_thru->addItem(CLOSED_PORT, 1 + output_devices.size());

            // SELECT THE CURRENT ONE
            String selected_thru_out_device(_audio_device_manager->get_selected_out_device(
                mono_AudioDeviceManager::OUTPUT_ID::THRU));
            const mono_AudioDeviceManager::DEVICE_STATE state(
                _audio_device_manager->get_selected_out_device_state(
                    mono_AudioDeviceManager::OUTPUT_ID::THRU));
            switch (state)
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN:
                combo_output_thru->setColour(ComboBox::backgroundColourId,
                                             Colours::green.withAlpha(0.3f));
                combo_output_thru->setSelectedItemIndex(
                    output_devices.indexOf(selected_thru_out_device), dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED:
                combo_output_thru->setColour(ComboBox::backgroundColourId,
                                             Colours::orange.withAlpha(0.2f));
                combo_output_thru->setSelectedItemIndex(output_devices.size(),
                                                        dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED:
                combo_output_thru->setColour(ComboBox::backgroundColourId,
                                             Colours::red.withAlpha(0.5f));
                combo_output_thru->setText("REMOVED: " + selected_thru_out_device,
                                           dontSendNotification);
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR:
                combo_output_thru->setColour(ComboBox::backgroundColourId,
                                             Colours::red.withAlpha(0.2f));
                combo_output_thru->setText("ERROR: " + selected_thru_out_device,
                                           dontSendNotification);
                break;
            }
        }
    }
}

//==============================================================================
Monique_Ui_MidiIO::Monique_Ui_MidiIO(Monique_Ui_Refresher *ui_refresher_,
                                     mono_AudioDeviceManager *const audio_device_manager_,
                                     Monique_Ui_Mainwindow *parent_)
    : Monique_Ui_Refreshable(ui_refresher_), _audio_device_manager(audio_device_manager_),
      original_w(1465), original_h(180)
{
    last_state_change = -1;
    parent = parent_;

    addAndMakeVisible(label_7 = new Label(String(), TRANS("CC")));
    label_7->setFont(Font(30.00f, Font::plain));
    label_7->setJustificationType(Justification::centred);
    label_7->setEditable(false, false, false);
    label_7->setColour(Label::textColourId, Colour(0xffff3b00));
    label_7->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_7->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(combo_input_main = new ComboBox("RECIEVE_MIDI_MAIN"));
    combo_input_main->setTooltip(
        TRANS("Select a MIDI device as input for notes and MIDI clock.\n"
              "\n"
              "To receive MIDI CC from this device enable the right CC checkbox.\n"));
    combo_input_main->setEditableText(false);
    combo_input_main->setJustificationType(Justification::centredLeft);
    combo_input_main->setTextWhenNothingSelected(String());
    combo_input_main->setTextWhenNoChoicesAvailable(String());
    combo_input_main->addListener(this);

    addAndMakeVisible(combo_input_main_channel = new ComboBox(String()));
    combo_input_main_channel->setTooltip(
        TRANS("Select a the MIDI channel there you like to listen to notes.\n"
              "(Kepp OMNI if you are not familiar with MIDI)"));
    combo_input_main_channel->setEditableText(false);
    combo_input_main_channel->setJustificationType(Justification::centredLeft);
    combo_input_main_channel->setTextWhenNothingSelected(TRANS("CH"));
    combo_input_main_channel->setTextWhenNoChoicesAvailable(TRANS("OMNI"));
    combo_input_main_channel->addListener(this);

    addAndMakeVisible(label_3 = new Label(String(), TRANS("INPUT (Notes, CC optional, Clock)")));
    label_3->setFont(Font(30.00f, Font::plain));
    label_3->setJustificationType(Justification::centredLeft);
    label_3->setEditable(false, false, false);
    label_3->setColour(Label::textColourId, Colour(0xffff3b00));
    label_3->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_3->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(toggle_input_main_thru = new ToggleButton(String()));
    toggle_input_main_thru->setTooltip(
        TRANS("Enable this to forward messages from the INPUT device to the THRU device."));
    toggle_input_main_thru->addListener(this);

    addAndMakeVisible(label_5 = new Label(String(), TRANS("THRU (OUT)")));
    label_5->setFont(Font(30.00f, Font::plain));
    label_5->setJustificationType(Justification::centredLeft);
    label_5->setEditable(false, false, false);
    label_5->setColour(Label::textColourId, Colour(0xffff3b00));
    label_5->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_5->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(combo_output_thru = new ComboBox("SEND_MIDI_THRU"));
    combo_output_thru->setTooltip(
        TRANS("Select a MIDI device there you like to forward incoming MIDI messages."));
    combo_output_thru->setEditableText(false);
    combo_output_thru->setJustificationType(Justification::centredLeft);
    combo_output_thru->setTextWhenNothingSelected(String());
    combo_output_thru->setTextWhenNoChoicesAvailable(String());
    combo_output_thru->addListener(this);

    addAndMakeVisible(label_6 =
                          new Label(String(), TRANS("Controller INPUT (CC, Notes for Buttons)")));
    label_6->setFont(Font(30.00f, Font::plain));
    label_6->setJustificationType(Justification::centredLeft);
    label_6->setEditable(false, false, false);
    label_6->setColour(Label::textColourId, Colour(0xffff3b00));
    label_6->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_6->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(combo_input_cc = new ComboBox("RECIEVE_CC"));
    combo_input_cc->setTooltip(TRANS("Select a MIDI device as input for CC and Note messages to "
                                     "control Monique e.g. by a hardware MIDI controller.\n"
                                     "\n"
                                     "See: MIDI TRAIN (right)"));
    combo_input_cc->setEditableText(false);
    combo_input_cc->setJustificationType(Justification::centredLeft);
    combo_input_cc->setTextWhenNothingSelected(String());
    combo_input_cc->setTextWhenNoChoicesAvailable(String());
    combo_input_cc->addListener(this);

    addAndMakeVisible(toggle_input_main_cc = new ToggleButton(String()));
    toggle_input_main_cc->setTooltip(
        TRANS("Enable this to receive also CC MIDI messages from the selected input device."));
    toggle_input_main_cc->addListener(this);

    addAndMakeVisible(toggle_input_cc_thru = new ToggleButton(String()));
    toggle_input_cc_thru->setTooltip(TRANS(
        "Enable this to forward messages from the Controller INPUT device to the THRU device."));
    toggle_input_cc_thru->addListener(this);

    addAndMakeVisible(label_9 = new Label(String(), TRANS("Controller Feedback (OUT)")));
    label_9->setFont(Font(30.00f, Font::plain));
    label_9->setJustificationType(Justification::centredLeft);
    label_9->setEditable(false, false, false);
    label_9->setColour(Label::textColourId, Colour(0xffff3b00));
    label_9->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_9->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(combo_output_cc = new ComboBox("SEND_MIDI_CC_FEEDBACK"));
    combo_output_cc->setTooltip(TRANS(
        "Select a MIDI device there you like to send the controller feedback messages. Normally "
        "this is the input of your MIDI controller which you have selected as Controller INPUT."));
    combo_output_cc->setEditableText(false);
    combo_output_cc->setJustificationType(Justification::centredLeft);
    combo_output_cc->setTextWhenNothingSelected(String());
    combo_output_cc->setTextWhenNoChoicesAvailable(String());
    combo_output_cc->addListener(this);

    addAndMakeVisible(slider_midi_pickup = new Slider("0"));
    slider_midi_pickup->setTooltip(TRANS(
        "Define the CC PICKUP in percent. \n"
        "\n"
        "Example:\n"
        "A listen sliders value is 50 (MIN:0, MAX:100).\n"
        "The current position of your MIDI controller slider is 0% or 0.\n"
        "The PICKUP offset is about 50% (middle).\n"
        "\n"
        "If you move the MIDI controller slider the slider on the user inderface does not change "
        "until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup->setRange(0, 1000, 1);
    slider_midi_pickup->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup->setColour(Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup->setColour(Slider::rotarySliderOutlineColourId, Colour(0xff161616));
    slider_midi_pickup->setColour(Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup->setColour(Slider::textBoxBackgroundColourId, Colour(0xff161616));
    slider_midi_pickup->addListener(this);

    addAndMakeVisible(label_2 = new Label(String(), TRANS("CC PICKUP OFFSET:")));
    label_2->setFont(Font(30.00f, Font::plain));
    label_2->setJustificationType(Justification::centredRight);
    label_2->setEditable(false, false, false);
    label_2->setColour(Label::textColourId, Colour(0xffff3b00));
    label_2->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_2->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(label_4 = new Label(String(), TRANS("THRU")));
    label_4->setFont(Font(30.00f, Font::plain));
    label_4->setJustificationType(Justification::centred);
    label_4->setEditable(false, false, false);
    label_4->setColour(Label::textColourId, Colour(0xffff3b00));
    label_4->setColour(TextEditor::textColourId, Colour(0xffff3b00));
    label_4->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(button_midi_learn = new TextButton(String()));
    button_midi_learn->setTooltip(
        TRANS("Enables the MIDI train/learn mode.\n"
              "\n"
              "Handling: enable MIDI train and select a slider or button on the main user "
              "interface. A little window pops up. Now you can move a slider on your MIDI "
              "controller (sender) to assign it to the element on the user interface (listener).\n"
              "\n"
              "Shortcut: CTRL + m"));
    button_midi_learn->setButtonText(TRANS("MAP"));
    button_midi_learn->addListener(this);
    button_midi_learn->setColour(TextButton::buttonColourId, Colours::black);
    button_midi_learn->setColour(TextButton::textColourOnId, Colour(0xffff3b00));
    button_midi_learn->setColour(TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible(close = new TextButton(String()));
    close->setTooltip(TRANS("Close MIDI settings."));
    close->setButtonText(TRANS("X"));
    close->addListener(this);
    close->setColour(TextButton::buttonColourId, Colours::red);
    close->setColour(TextButton::buttonOnColourId, Colours::red);
    close->setColour(TextButton::textColourOnId, Colours::black);
    close->setColour(TextButton::textColourOffId, Colours::black);

    // slider_midi_pickup->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE,
    // SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    setOpaque(true);
    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MIDI_THEME);
        if (Slider *slider = dynamic_cast<Slider *>(child))
        {
            slider->getProperties().set(VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER);
        }
    }
    toggle_input_main_thru->setOpaque(false);
    toggle_input_main_cc->setOpaque(false);
    toggle_input_cc_thru->setOpaque(false);

    slider_midi_pickup->setValue(synth_data->midi_pickup_offset * 1000, dontSendNotification);

    update_combo_boxed();

    has_grabbed_focus = false;
    /*

    setSize (1465, 180);

    */
}

Monique_Ui_MidiIO::~Monique_Ui_MidiIO()
{
    label_7 = nullptr;
    combo_input_main = nullptr;
    combo_input_main_channel = nullptr;
    label_3 = nullptr;
    toggle_input_main_thru = nullptr;
    label_5 = nullptr;
    combo_output_thru = nullptr;
    label_6 = nullptr;
    combo_input_cc = nullptr;
    toggle_input_main_cc = nullptr;
    toggle_input_cc_thru = nullptr;
    label_9 = nullptr;
    combo_output_cc = nullptr;
    slider_midi_pickup = nullptr;
    label_2 = nullptr;
    label_4 = nullptr;
    button_midi_learn = nullptr;
    close = nullptr;
}

//==============================================================================
void Monique_Ui_MidiIO::paint(Graphics &g)
{
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.fillAll(Colour(0xff050505));

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(10.0f, 0.0f, 1440.0f, 198.0f, 10.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(15.0f, 256.0f, 570.0f, 1.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(560.0f, 70.0f, 1.0f, 51.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(625.0f, 145.0f, 20.0f, 1.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(645.0f, 65.0f, 1.0f, 81.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(625.0f, 65.0f, 350.0f, 1.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(450.0f, 120.0f, 110.0f, 1.0f, 1.000f);

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(450.0f, 120.0f, 1.0f, 16.0f, 1.000f);
}

void Monique_Ui_MidiIO::resized()
{
    WIDTH_AND_HIGHT_FACTORS

    label_7->setBounds(545, 15, 35, 30);
    combo_input_main->setBounds(35, 50, 430, 30);
    combo_input_main_channel->setBounds(475, 50, 60, 30);
    label_3->setBounds(35, 15, 405, 30);
    toggle_input_main_thru->setBounds(595, 50, 60, 30);
    label_5->setBounds(975, 15, 430, 30);
    combo_output_thru->setBounds(975, 50, 465, 30);
    label_6->setBounds(35, 95, 405, 30);
    combo_input_cc->setBounds(35, 130, 430, 30);
    toggle_input_main_cc->setBounds(545, 50, 35, 35);
    toggle_input_cc_thru->setBounds(595, 130, 60, 30);
    label_9->setBounds(975, 95, 430, 30);
    combo_output_cc->setBounds(975, 130, 465, 30);
    slider_midi_pickup->setBounds(865, 100, 60, 60);
    label_2->setBounds(855 - 180, 115, 180, 30);
    label_4->setBounds(595, 15, 60, 30);
    button_midi_learn->setBounds(560 - 85, 130, 85, 30);
    close->setBounds(1420, 5, 25, 25);

    if (!has_grabbed_focus)
    {
        combo_input_main->grabKeyboardFocus();
        has_grabbed_focus = true;
    }
#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_MidiIO::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == combo_input_main)
    {
        _audio_device_manager->open_in_port(mono_AudioDeviceManager::INPUT_ID::NOTES,
                                            comboBoxThatHasChanged->getText());
    }
    else if (comboBoxThatHasChanged == combo_input_main_channel)
    {
        _audio_device_manager->input_channel = combo_input_main_channel->getSelectedItemIndex();
    }
    else if (comboBoxThatHasChanged == combo_output_thru)
    {
        _audio_device_manager->open_out_port(mono_AudioDeviceManager::OUTPUT_ID::THRU,
                                             comboBoxThatHasChanged->getText());
    }
    else if (comboBoxThatHasChanged == combo_input_cc)
    {
        _audio_device_manager->open_in_port(mono_AudioDeviceManager::INPUT_ID::CC,
                                            comboBoxThatHasChanged->getText());
    }
    else if (comboBoxThatHasChanged == combo_output_cc)
    {
        _audio_device_manager->open_out_port(mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK,
                                             comboBoxThatHasChanged->getText());
    }
}

void Monique_Ui_MidiIO::buttonClicked(Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggle_input_main_thru)
    {
        _audio_device_manager->main_input_thru = buttonThatWasClicked->getToggleState();
    }
    else if (buttonThatWasClicked == toggle_input_main_cc)
    {
        _audio_device_manager->use_main_input_as_cc = buttonThatWasClicked->getToggleState();
    }
    else if (buttonThatWasClicked == toggle_input_cc_thru)
    {
        _audio_device_manager->cc_input_thru = buttonThatWasClicked->getToggleState();
    }
    else if (buttonThatWasClicked == button_midi_learn)
    {
        bool was_lerning = IS_MIDI_LEARN;
        midi_control_handler->toggle_midi_learn();
        if (was_lerning)
            get_editor()->show_info_popup(nullptr, nullptr);
    }
    else if (buttonThatWasClicked == close)
    {
        parent->editor_midiio = nullptr;
        return;
    }
}

void Monique_Ui_MidiIO::sliderValueChanged(Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == slider_midi_pickup)
    {
        synth_data->midi_pickup_offset = slider_midi_pickup->getValue() / 1000;
    }
}

//==============================================================================
#endif
