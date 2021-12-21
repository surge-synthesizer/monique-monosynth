/*
  ==============================================================================

    mono_Parameters.cpp
    Created: 24 Apr 2015 9:37:51am
    Author:  monotomy

  ==============================================================================
*/

#include "monique_core_Parameters.h"
#include "monique_core_Datastructures.h"

#include "monique_core_Processor.h"

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
COLD ParameterListener::ParameterListener() noexcept {};
COLD ParameterListener::~ParameterListener() noexcept {};

//==============================================================================
//==============================================================================
//==============================================================================
COLD ParameterInfo::ParameterInfo(TYPES_DEF type_,

                                  const float min_value_, const float max_value_,
                                  const float init_value_, const float init_modulation_amount_,
                                  const int num_steps_, const String &name_,
                                  const String &short_name_) noexcept
    : type(type_),

      min_value(min_value_), max_value(max_value_), init_value(init_value_),
      factory_default_value(0), program_on_load_value(0),

      init_modulation_amount(init_modulation_amount_), factory_default_modulation_amount(0),
      program_on_load_modulation_amount(0),

      num_steps(num_steps_),

      name(name_), short_name(short_name_), parameter_host_id(-1), is_inverted(false)
{
}

COLD ParameterInfo::~ParameterInfo() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
COLD ParameterRuntimeInfo::ParameterRuntimeInfo() noexcept
    : my_smoother(nullptr), smoothing_is_enabled(true), current_modulation_amount(0),
      current_value_state(HAS_NO_VALUE_STATE), current_modulation_state(HAS_NO_VALUE_STATE),
      timeChanger(nullptr)
{
}

COLD ParameterRuntimeInfo::~ParameterRuntimeInfo() noexcept
{
    if (timeChanger)
    {
        timeChanger->forceStopAndKill();
        timeChanger = nullptr;
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD Parameter::Parameter(const float min_value_, const float max_value_, const float init_value_,
                          const int num_steps_, const String &name_, const String &short_name_,
                          const float init_modulation_amount_, TYPES_DEF type_) noexcept
    : value(init_value_), modulation_amount(init_modulation_amount_),
      info(new ParameterInfo(type_, min_value_, max_value_, init_value_, init_modulation_amount_,
                             num_steps_, name_, short_name_)),
      runtime_info(new ParameterRuntimeInfo()),

      midi_control(new MIDIControl(this)),

      ignore_listener(nullptr)
{
    always_value_listeners.minimiseStorageOverheads();
    value_listeners.minimiseStorageOverheads();
}

COLD Parameter::~Parameter() noexcept
{
    delete midi_control;
    delete runtime_info;
    delete info;
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD BoolParameter::BoolParameter(const bool init_value_, const String &name_,
                                  const String &short_name_) noexcept
    : Parameter(MIN_MAX(false, true), float(init_value_), 1, name_, short_name_,

                HAS_NO_MODULATION, IS_BOOL)
{
}

COLD BoolParameter::~BoolParameter() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
COLD IntParameter::IntParameter(const int min_value_, const int max_value_, const int init_value_,
                                const String &name_, const String &short_name_) noexcept
    : Parameter(MIN_MAX(min_value_, max_value_), float(init_value_), max_value_ - min_value_, name_,
                short_name_,

                HAS_NO_MODULATION, IS_INT)
{
}
COLD IntParameter::~IntParameter() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
COLD ModulatedParameter::ModulatedParameter(const float min_value_, const float max_value_,
                                            const float init_value_, const int num_steps_,
                                            const String &name_, const String &short_name_,
                                            const float init_modulation_amount_) noexcept
    : Parameter(min_value_, max_value_, init_value_, num_steps_, name_, short_name_,
                init_modulation_amount_, IS_FLOAT)
{
}

COLD ModulatedParameter::~ModulatedParameter() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
COLD ArrayOfParameters::ArrayOfParameters(
    const int num_parameters_,

    const float min_value_, const float max_value_, const float init_value_, const int num_steps_,

    const String &owner_class_name_, const String &short_owner_class_name_, const int owner_id_,

    const String &param_name_, const String &param_name_short_,

    bool create_human_id_) noexcept
    : size(num_parameters_)
{
    parameters = new Parameter *[size];
    for (int i = 0; i != size; ++i)
    {
        parameters[i] = new Parameter(
            MIN_MAX(min_value_, max_value_), init_value_, num_steps_,
            generate_param_name(owner_class_name_, owner_id_, param_name_, i),
            create_human_id_
                ? generate_short_human_name(short_owner_class_name_, owner_id_, param_name_short_,
                                            i)
                : generate_short_human_name(short_owner_class_name_, param_name_short_, i));
    }
}
COLD ArrayOfParameters::~ArrayOfParameters() noexcept
{
    for (int i = size - 1; i > -1; --i)
    {
        delete parameters[i];
    }
}
//==============================================================================
//==============================================================================
//==============================================================================
COLD ArrayOfBoolParameters::ArrayOfBoolParameters(const int num_parameters_,

                                                  const bool init_value_,

                                                  const String &owner_class_name_,
                                                  const String &short_owner_class_name_,
                                                  const int owner_id_,

                                                  const String &param_name_,
                                                  const String &param_name_short_,

                                                  bool create_human_id_) noexcept
    : size(num_parameters_)
{
    parameters = new BoolParameter *[size];
    for (int i = 0; i != size; ++i)
    {
        parameters[i] = new BoolParameter(
            init_value_, generate_param_name(owner_class_name_, owner_id_, param_name_, i),
            create_human_id_
                ? generate_short_human_name(short_owner_class_name_, owner_id_, param_name_short_,
                                            i)
                : generate_short_human_name(short_owner_class_name_, param_name_short_, i));
    }
}
COLD ArrayOfBoolParameters::~ArrayOfBoolParameters() noexcept
{
    for (int i = size - 1; i > -1; --i)
    {
        delete parameters[i];
    }
}
//==============================================================================
//==============================================================================
//==============================================================================
COLD ArrayOfIntParameters::ArrayOfIntParameters(
    const int num_parameters_,

    const int min_value_, const int max_value_, const int init_value_,

    const String &owner_class_name_, const String &short_owner_class_name_, const int owner_id_,

    const String &param_name_, const String &param_name_short_,

    bool create_human_id_) noexcept
    : size(num_parameters_)
{
    parameters = new IntParameter *[size];
    for (int i = 0; i != size; ++i)
    {
        parameters[i] = new IntParameter(
            MIN_MAX(min_value_, max_value_), init_value_,
            generate_param_name(owner_class_name_, owner_id_, param_name_, i),
            create_human_id_
                ? generate_short_human_name(short_owner_class_name_, owner_id_, param_name_short_,
                                            i)
                : generate_short_human_name(short_owner_class_name_, param_name_short_, i));
    }
}
COLD ArrayOfIntParameters::~ArrayOfIntParameters() noexcept
{
    for (int i = size - 1; i > -1; --i)
    {
        delete parameters[i];
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
ChangeParamOverTime::ChangeParamOverTime(Parameter &param_, float target_value_,
                                         int time_in_ms_) noexcept
    : param(param_),

      sum_callbacks(time_in_ms_ / PARAM_CHANGE_INTERVAL_IN_MS),

      current_value(param_), target_value(target_value_),
      value_delta((target_value_ - current_value) / sum_callbacks),

      min(param_.get_info().min_value), max(param_.get_info().max_value)
{
    ChangeParamOverTime *current_time_changer = param_.get_runtime_info().timeChanger;
    if (current_time_changer != nullptr)
    {
        current_time_changer->forceStopAndKill();
    }
    param_.get_runtime_info().timeChanger = this;

    change();
    startTimer(PARAM_CHANGE_INTERVAL_IN_MS);
}
ChangeParamOverTime::~ChangeParamOverTime() noexcept {}

void ChangeParamOverTime::timerCallback()
{
    sum_callbacks--;

    if (sum_callbacks > 0)
    {
        change();
    }
    else
    {
        param.set_value_by_automation(target_value);
        forceStopAndKill();
        return;
    }
}
inline void ChangeParamOverTime::change() noexcept
{
    current_value += value_delta;
    if (current_value > max)
    {
        current_value = max;
    }
    else if (current_value < min)
    {
        current_value = min;
    }

    param.set_value_by_automation(current_value);
}

//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================

// ==============================================================================
MIDIControl::MIDIControl(Parameter *const owner_)
    : is_in_ctrl_mode(false), owner(owner_), audio_processor(nullptr)
{
    midi_number = -1;
    is_ctrl_version_of_name = "";
}
MIDIControl::~MIDIControl() {}

void MIDIControl::clear()
{
    audio_processor->midi_control_handler->remove_trained(this);
    stop_listen_for_feedback();
    send_clear_feedback_only();

    audio_processor = nullptr;
    midi_number = -1;
    is_ctrl_version_of_name = "";
}

bool MIDIControl::read_from_if_you_listen(int controller_number_, int controller_value_,
                                          float pickup_offset_) noexcept
{
    bool success = false;
    {
        if (midi_number == controller_number_ and controller_number_ < 128)
        {
            float value;
            if (controller_number_ == PITCHWHEEL_CC) // PitchWheel
            {
                value = 1.0f / float(0x3fff) * controller_value_;
            }
            else
            {
                value = 1.0f / 127.0f * controller_value_;
            }

            if (controller_value_ == 63)
            {
                value = 0.5;
            }
            if (controller_value_ == 62)
            {
                value = 1.0f / 125.66f * controller_value_;
            }

            if (type_of(owner) == IS_BOOL)
            {
                if (value > 0.5)
                {
                    owner->set_value(true);
                }
                else
                {
                    owner->set_value(false);
                }

                success = true;
            }
            else
            {
                if (is_in_ctrl_mode)
                {
                    if (is_ctrl_version_of_name != "")
                    {
                        if (MIDIControl *midi_control =
                                audio_processor->midi_control_handler->get_trained(
                                    is_ctrl_version_of_name))
                        {
                            float current_value = get_percent_value(midi_control->owner);
                            // PICKUP
                            if (current_value + pickup_offset_ >= value &&
                                current_value - pickup_offset_ <= value)
                            {
                                set_percent_value(midi_control->owner, value);
                                success = true;
                            }
                        }
                    }
                    else if (has_modulation(owner))
                    {
                        float current_modulation = owner->get_modulation_amount();
                        float new_modulation = value * 2.0f - 1.0f;
                        // PICKUP
                        if (current_modulation + pickup_offset_ > new_modulation &&
                            current_modulation - pickup_offset_ < new_modulation)
                        {
                            owner->set_modulation_amount(new_modulation);
                            success = true;
                        }
                    }
                }
                else
                {
                    if (is_ctrl_version_of_name == "")
                    {
                        float current_value = get_percent_value(owner);
                        // PICKUP
                        if (current_value + pickup_offset_ >= value &&
                            current_value - pickup_offset_ <= value)
                        {
                            set_percent_value(owner, value);
                            success = true;
                        }
                    }
                }
            }
        }
        else if (midi_number == controller_number_)
        {
            if (type_of(owner) == IS_BOOL)
            {
                owner->set_value(not owner->get_value());
                success = true;
            }
        }
    }

    if (success)
    {
        send_standard_feedback();
    }

    return success;
}
bool MIDIControl::train(int controller_number_, Parameter *const is_ctrl_version_of_,
                        MoniqueAudioProcessor *audio_processor_) noexcept
{
    send_clear_feedback_only();
    audio_processor = audio_processor_;

    bool success = false;
    {
        midi_number = controller_number_;
        if (is_ctrl_version_of_)
        {
            is_ctrl_version_of_name = is_ctrl_version_of_->get_info().name;
        }
        else
        {
            is_ctrl_version_of_name = "";
        }

        success = true;
    }

    if (success)
    {
        audio_processor->midi_control_handler->add_trained(this);
        send_feedback_only();
        start_listen_for_feedback();
    }
    else
    {
        stop_listen_for_feedback();
        audio_processor->midi_control_handler->remove_trained(this);
    }

    return success;
}
bool MIDIControl::train(int controller_number_, String is_ctrl_version_of_name_,
                        MoniqueAudioProcessor *audio_processor_) noexcept
{
    send_clear_feedback_only();
    audio_processor = audio_processor_;

    midi_number = controller_number_;
    is_ctrl_version_of_name = is_ctrl_version_of_name_;

    if (is_valid_trained())
    {
        audio_processor->midi_control_handler->add_trained(this);
        send_feedback_only();
        start_listen_for_feedback();
    }
    else
    {
        stop_listen_for_feedback();
        audio_processor->midi_control_handler->remove_trained(this);
    }
    return true;
}

void MIDIControl::start_listen_for_feedback() noexcept { owner->register_listener(this); }
void MIDIControl::stop_listen_for_feedback() noexcept { owner->remove_listener(this); }

void MIDIControl::parameter_value_changed(Parameter *param_) noexcept
{
    const bool is_ctrl_version_of = is_ctrl_version_of_name != "";
    if (type_of(param_) == IS_BOOL)
    {
        bool do_send =
            (!is_ctrl_version_of && !is_in_ctrl_mode) || ((is_ctrl_version_of && is_in_ctrl_mode));
        //|| &(GET_DATA( synth_data ).ctrl) == param_ );
        if (do_send)
        {
            send_standard_feedback();
        }
    }
    {
        bool do_send =
            (!is_ctrl_version_of && !is_in_ctrl_mode) || (is_ctrl_version_of && is_in_ctrl_mode);
        if (do_send)
        {
            send_standard_feedback();
        }
    }
}
void MIDIControl::parameter_value_changed_by_automation(Parameter *param_) noexcept {}
void MIDIControl::parameter_value_on_load_changed(Parameter *param_) noexcept
{
    // parameter_value_changed( param_ );
}
void MIDIControl::parameter_modulation_value_changed(Parameter *param_) noexcept
{
    if (is_in_ctrl_mode)
    {
        send_modulation_feedback();
    }
}

void MIDIControl::send_feedback_only() const noexcept
{
    if (is_valid_trained())
    {
        bool is_ctrl_version_of = is_ctrl_version_of_name != "";
        if (!is_ctrl_version_of && !is_in_ctrl_mode)
        {
            send_standard_feedback();
        }
        else if (is_ctrl_version_of && is_in_ctrl_mode)
        {
            send_standard_feedback();
        }
        else if (is_in_ctrl_mode && has_modulation(owner))
        {
            send_modulation_feedback();
        }
    }
}
void MIDIControl::send_clear_feedback_only() const noexcept
{
    if (is_valid_trained())
    {
        audio_processor->clear_feedback_message(midi_number);
    }
}

void MIDIControl::set_ctrl_mode(bool mode_) noexcept
{
    is_in_ctrl_mode = mode_;

    send_feedback_only();
}

inline void MIDIControl::send_standard_feedback() const noexcept
{
    if (is_valid_trained())
    {
        if (type_of(owner) == IS_BOOL)
        {
            if (owner->get_info().is_inverted)
            {
                audio_processor->send_feedback_message(midi_number, owner->get_value() ? 0 : 127);
            }
            else
            {
                audio_processor->send_feedback_message(midi_number, owner->get_value() * 127);
            }
        }
        else
        {
            audio_processor->send_feedback_message(midi_number,
                                                   std::floor(127.0f * get_percent_value(owner)));
        }
    }
}
inline void MIDIControl::send_modulation_feedback() const noexcept
{
    if (is_valid_trained())
    {
        audio_processor->send_feedback_message(
            midi_number, std::floor(127.0f * (owner->get_modulation_amount() * 0.5f + 1.0f)));
    }
}
// ==============================================================================
// ==============================================================================
// ==============================================================================
// ==============================================================================
COLD MIDIControlHandler::MIDIControlHandler(UiLookAndFeel *look_and_feel_,
                                            MoniqueAudioProcessor *const audio_processor_) noexcept
    : ui_look_and_feel(look_and_feel_), audio_processor(audio_processor_)
{
    clear();
}

COLD MIDIControlHandler::~MIDIControlHandler() noexcept { clear(); }

// ==============================================================================
void MIDIControlHandler::toggle_midi_learn() noexcept
{
    bool tmp_activated = is_activated_and_waiting_for_param;
    bool tmp_learning = learning_param;
    clear();
    is_activated_and_waiting_for_param = !(tmp_activated || tmp_learning);
}

void MIDIControlHandler::set_learn_param(Parameter *param_) noexcept
{
    clear();

    learning_param = param_;
}
void MIDIControlHandler::set_learn_width_ctrl_param(Parameter *param_, Parameter *ctrl_param_,
                                                    Component *comp_) noexcept
{
    clear();

    learning_param = param_;
    learning_ctrl_param = ctrl_param_;

    learning_comps.add(comp_);
    SET_COMPONENT_TO_MIDI_LEARN(comp_, ui_look_and_feel)
}
void MIDIControlHandler::set_learn_param(Parameter *param_, Component *comp_) noexcept
{
    clear();

    learning_param = param_;
    learning_ctrl_param = nullptr;

    learning_comps.add(comp_);
    SET_COMPONENT_TO_MIDI_LEARN(comp_, ui_look_and_feel)
}
void MIDIControlHandler::set_learn_param(Parameter *param_, Array<Component *> comps_) noexcept
{
    clear();

    learning_param = param_;

    learning_comps = comps_;
    for (int i = 0; i != learning_comps.size(); ++i)
    {
        SET_COMPONENT_TO_MIDI_LEARN(learning_comps[i], ui_look_and_feel);
    }
}
Parameter *MIDIControlHandler::is_learning() const noexcept { return learning_param; }
bool MIDIControlHandler::handle_incoming_message(int controller_number_) noexcept
{
    bool success = false;
    if (learning_param)
    {
        if (learning_param->midi_control->train(controller_number_, nullptr, audio_processor))
        {
            success = true;
        }
        if (learning_ctrl_param)
        {
            learning_ctrl_param->midi_control->train(controller_number_, learning_param,
                                                     audio_processor);
        }

        clear();
        is_activated_and_waiting_for_param = true;
    }

    return success;
}
void MIDIControlHandler::clear() noexcept
{
    learning_param = nullptr;
    learning_ctrl_param = nullptr;
    is_activated_and_waiting_for_param = false;

    for (int i = 0; i != learning_comps.size(); ++i)
    {
        UNSET_COMPONENT_MIDI_LEARN(learning_comps[i], ui_look_and_feel)
    }
    learning_comps.clearQuick();
}
