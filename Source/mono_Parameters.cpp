/*
  ==============================================================================

    mono_Parameters.cpp
    Created: 24 Apr 2015 9:37:51am
    Author:  monotomy

  ==============================================================================
*/

#include "mono_Parameters.h"
#include "UiLookAndFeel.h"
#include "SynthData.h"
#include "PluginProcessor.h"

// ==============================================================================
juce_ImplementSingleton (MIDIControlHandler)

NOINLINE MIDIControl::MIDIControl(mono_ParameterCompatibilityBase*const owner_): is_in_ctrl_mode(false), owner(owner_) {
    midi_number = -1;
    listen_type = NOT_SET;
    channel = -1;
    is_ctrl_version_of_name = "";
}
NOINLINE MIDIControl::~MIDIControl() {}

void MIDIControl::clear()
{
    stop_listen_for_feedback();
    send_clear_feedback_only();

    midi_number = -1;
    listen_type = NOT_SET;
    channel = -1;
    is_ctrl_version_of_name = "";
}

bool MIDIControl::is_listen_to( MidiMessage& message_ ) const noexcept {
    bool is = false;
    if( message_.isController() && listen_type == CC ) {
        if( message_.getControllerNumber() == midi_number ) {
            is = true;
        }
    }
    else if( message_.isNoteOn() && listen_type == NOTE ) {
        if( message_.getNoteNumber() == midi_number ) {
            is = true;
        }
    }

    return is;
}
bool MIDIControl::read_from_if_you_listen( const MidiMessage& input_message_ ) noexcept {
    bool success = false;
    const float pickup = DATA( synth_data ).midi_pickup_offset;
    if( input_message_.isController() && listen_type == CC ) {
        if( midi_number == input_message_.getControllerNumber() ) {
            if( input_message_.getChannel() == channel ) {
                float value = 1.0f/127.0f*input_message_.getControllerValue();
                if( owner->get_type() == IS_BOOL )
                {
                    if( value > 0.5 )
                        owner->set_scaled_value(true);
                    else
                        owner->set_scaled_value(false);
                    success = true;
                }
                else
                {
                    if( is_in_ctrl_mode )
                    {
                        if( is_ctrl_version_of_name != "" )
                        {
                            float current_value = owner->get_float_percent_value();
                            // PICKUP
                            if( current_value + pickup > value && current_value - pickup < value ) {
                                owner->set_float_percent_value( value );
                                success = true;
                            }
                        }
                        else if( owner->has_modulation() )
                        {
                            float current_modulation = owner->get_modulation_amount();
                            float new_modulation = value*2.0f - 1.0f;
                            // PICKUP
                            if( current_modulation + pickup > new_modulation && current_modulation - pickup < new_modulation ) {
                                owner->set_modulation_amount( new_modulation );
                                success = true;
                            }
                        }
                    }
                    else
                    {
                        if( is_ctrl_version_of_name == "" ) {
                            float current_value = owner->get_float_percent_value();
                            // PICKUP
                            if( current_value + pickup > value && current_value - pickup < value ) {
                                owner->set_float_percent_value( value );
                                success = true;
                            }
                        }
                    }
                }
            }
        }
    }
    else if( input_message_.isNoteOn() && listen_type == NOTE ) {
        if( midi_number == input_message_.getNoteNumber() ) {
            if( input_message_.getChannel() == channel )
            {
                if( owner->get_type() == IS_BOOL )
                {
                    owner->toggle_value();
                    success = true;
                }
                else
                {
                    if( is_in_ctrl_mode )
                    {
                        if( is_ctrl_version_of_name != "" )
                        {
                            owner->toggle_value();
                            success = true;
                        }
                        else if( owner->has_modulation() )
                        {
                            float current_modulation = owner->get_modulation_amount();
                            if( current_modulation == -1 )
                                owner->set_modulation_amount( 1 );
                            else
                                owner->set_modulation_amount( -1 );

                            success = true;
                        }
                    }
                    else
                    {
                        if( is_ctrl_version_of_name == "" )
                        {
                            owner->toggle_value();
                            success = true;
                        }
                    }
                }
            }
        }
    }

    return success;
}
bool MIDIControl::train( const MidiMessage& input_message_, mono_ParameterCompatibilityBase*const is_ctrl_version_of_ ) noexcept
{
    send_clear_feedback_only();

    bool success = false;
    if( input_message_.isController() )
    {
        listen_type = CC;
        channel = input_message_.getChannel();
        midi_number = input_message_.getControllerNumber();
        if( is_ctrl_version_of_ )
            is_ctrl_version_of_name = is_ctrl_version_of_->get_name();
        else
            is_ctrl_version_of_name = "";

        success = true;
    }
    else if( input_message_.isNoteOn() )
    {
        listen_type = NOTE;
        channel = input_message_.getChannel();
        midi_number = input_message_.getNoteNumber();
        if( is_ctrl_version_of_ )
            is_ctrl_version_of_name = is_ctrl_version_of_->get_name();
        else
            is_ctrl_version_of_name = "";

        success = true;
    }

    if( success )
    {
        send_feedback_only();
        start_listen_for_feedback();
    }
    else
    {
        stop_listen_for_feedback();
    }

    return success;
}
bool MIDIControl::train( int listen_type_, int8 midi_number_, int channel_, String is_ctrl_version_of_name_ ) noexcept
{
    send_clear_feedback_only();

    listen_type = listen_type_;
    midi_number = midi_number_;
    channel = channel_;
    is_ctrl_version_of_name = is_ctrl_version_of_name_;

    if( is_valid_trained() )
    {
        send_feedback_only();
        start_listen_for_feedback();
    }
    else
    {
        stop_listen_for_feedback();
    }
    return true;
}

bool MIDIControl::is_valid_trained() const noexcept {
    bool success = true;
    if( listen_type == NOT_SET )
        success = false;
    else if( midi_number == -1 )
        success = false;
    else if( channel < 1 )
        success = false;

    return success;
}

void MIDIControl::start_listen_for_feedback() noexcept {
    switch( owner->get_type() )
    {
    case IS_FLOAT :
        reinterpret_cast< mono_ParameterBase< float >* >( owner )->register_listener( this );
        break;
    case IS_BOOL :
        reinterpret_cast< mono_ParameterBase< bool >* >( owner )->register_listener( this );
        break;
    case IS_INT :
        reinterpret_cast< mono_ParameterBase< int >* >( owner )->register_listener( this );
        break;
    }
}
void MIDIControl::stop_listen_for_feedback() noexcept {
    switch( owner->get_type() )
    {
    case IS_FLOAT :
        reinterpret_cast< mono_ParameterBase< float >* >( owner )->remove_listener( this );
        break;
    case IS_BOOL :
        reinterpret_cast< mono_ParameterBase< bool >* >( owner )->remove_listener( this );
        break;
    case IS_INT :
        reinterpret_cast< mono_ParameterBase< int >* >( owner )->remove_listener( this );
        break;
    }
}

void MIDIControl::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    bool is_ctrl_version_of = is_ctrl_version_of_name != "";
    bool do_send = ( ! is_ctrl_version_of && ! is_in_ctrl_mode) || ( is_ctrl_version_of && is_in_ctrl_mode );
    if( do_send )
        send_standard_feedback();
}
void MIDIControl::parameter_modulation_value_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    if( is_in_ctrl_mode )
        send_modulation_feedback();
}
void MIDIControl::parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void MIDIControl::parameter_value_changed( mono_ParameterBase< bool >* param_ ) noexcept
{
    bool is_ctrl_version_of = is_ctrl_version_of_name != "";
    bool do_send = ( ! is_ctrl_version_of && ! is_in_ctrl_mode) || ( (is_ctrl_version_of && is_in_ctrl_mode) || DATA( synth_data ).ctrl.get_base() == param_ );
    if( do_send )
        send_standard_feedback();
}
void MIDIControl::parameter_value_on_load_changed( mono_ParameterBase< bool >* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void MIDIControl::parameter_value_changed( mono_ParameterBase< int >* param_ ) noexcept
{
    bool is_ctrl_version_of = is_ctrl_version_of_name != "";
    bool do_send = ( ! is_ctrl_version_of && ! is_in_ctrl_mode) || ( is_ctrl_version_of && is_in_ctrl_mode );
    if( do_send )
        send_standard_feedback();
}
void MIDIControl::parameter_value_on_load_changed( mono_ParameterBase< int >* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void MIDIControl::generate_feedback_message( MidiMessage& message_ ) const noexcept {
    if( listen_type == CC )
        message_ = MidiMessage::controllerEvent( channel, midi_number, 127.0f*owner->get_float_percent_value() );
    else
        message_ = MidiMessage::noteOn( channel, midi_number, owner->get_float_percent_value() );
}
void MIDIControl::generate_modulation_feedback_message( MidiMessage& message_ ) const noexcept {
    if( listen_type == CC )
        message_ = MidiMessage::controllerEvent( channel, midi_number, 127.0f*(owner->get_modulation_amount()*0.5f + 1.0f) );
    else
        message_ = MidiMessage::noteOn( channel, midi_number, (owner->get_modulation_amount()*0.5f + 1.0f) );
}
void MIDIControl::send_feedback_only() const noexcept {
    if( is_valid_trained() )
    {
        bool is_ctrl_version_of = is_ctrl_version_of_name != "";
        if( ! is_ctrl_version_of && ! is_in_ctrl_mode )
        {
            send_standard_feedback();
        }
        else if( is_ctrl_version_of && is_in_ctrl_mode )
        {
            send_standard_feedback();
        }
        else if( is_in_ctrl_mode && owner->has_modulation() )
        {
            send_modulation_feedback();
        }
    }
}
void MIDIControl::send_clear_feedback_only() const noexcept {
    if( is_valid_trained() )
    {
        MidiMessage fb_message;
        if( listen_type == CC )
            fb_message = MidiMessage::controllerEvent( channel, midi_number, 0 );
        else
            fb_message = MidiMessage::noteOn( channel, midi_number, 0.0f );

        AppInstanceStore::getInstance()->audio_processor->send_feedback_message( fb_message );
    }
}

void MIDIControl::set_ctrl_mode( bool mode_ ) noexcept {
    is_in_ctrl_mode = mode_;

    send_feedback_only();
}

void MIDIControl::send_standard_feedback() const noexcept {
    MidiMessage fb_message;
    generate_feedback_message( fb_message );
    AppInstanceStore::getInstance()->audio_processor->send_feedback_message( fb_message );
}
void MIDIControl::send_modulation_feedback() const noexcept {
    MidiMessage fb_message;
    generate_modulation_feedback_message( fb_message );
    AppInstanceStore::getInstance()->audio_processor->send_feedback_message( fb_message );
}
// ==============================================================================
MIDIControlHandler::MIDIControlHandler() {
    clear();
}

void MIDIControlHandler::toggle_midi_learn() {
    bool tmp_activated = is_activated_and_waiting_for_param;
    bool tmp_learning = learning_param;
    clear();
    is_activated_and_waiting_for_param = !(tmp_activated || tmp_learning);
}
bool MIDIControlHandler::is_waiting_for_param() const {
    return is_activated_and_waiting_for_param;
}
void MIDIControlHandler::set_learn_param(mono_ParameterCompatibilityBase* param_) {
    clear();

    learning_param = param_;
}
void MIDIControlHandler::set_learn_width_ctrl_param( mono_ParameterCompatibilityBase* param_, mono_ParameterCompatibilityBase* ctrl_param_, Component* comp_ ) {
    clear();

    learning_param = param_;
    learning_ctrl_param = ctrl_param_;

    learning_comps.add( comp_ );
    SET_COMPONENT_TO_MIDI_LEARN( comp_ )
}
void MIDIControlHandler::set_learn_param(mono_ParameterCompatibilityBase* param_, Component* comp_ ) {
    clear();

    learning_param = param_;

    learning_comps.add( comp_ );
    SET_COMPONENT_TO_MIDI_LEARN( comp_ )
}
void MIDIControlHandler::set_learn_param(mono_ParameterCompatibilityBase* param_, Array< Component* > comps_ ) {
    clear();

    learning_param = param_;

    learning_comps = comps_;
    for( int i = 0 ; i != learning_comps.size() ; ++i )
        SET_COMPONENT_TO_MIDI_LEARN( learning_comps[i] )
    }
mono_ParameterCompatibilityBase* MIDIControlHandler::is_learning() const {
    return learning_param;
}
bool MIDIControlHandler::handle_incoming_message( const MidiMessage& input_message_ ) {
    bool success = false;
    if( learning_param )
    {
        if( learning_param->midi_control->train(input_message_,nullptr) )
            success = true;

        if( learning_ctrl_param )
            learning_ctrl_param->midi_control->train(input_message_,learning_param);

        clear();
        is_activated_and_waiting_for_param = true;
    }

    return success;
}
void MIDIControlHandler::clear() {
    learning_param = nullptr;
    learning_ctrl_param = nullptr;
    is_activated_and_waiting_for_param = false;

    for( int i = 0 ; i != learning_comps.size() ; ++i )
        UNSET_COMPONENT_MIDI_LEARN( learning_comps[i] )
        learning_comps.clearQuick();
}

// ==============================================================================
NOINLINE mono_ParameterCompatibilityBase::mono_ParameterCompatibilityBase() noexcept :
midi_control( new MIDIControl( this ) ) {}
NOINLINE mono_ParameterCompatibilityBase::~mono_ParameterCompatibilityBase() noexcept {}

NOINLINE float mono_ParameterCompatibilityBase::get_modulation_amount() const noexcept {
    return HAS_NO_MODULATION;
};

NOINLINE float mono_ParameterCompatibilityBase::get_last_modulation_amount() const noexcept {
    return 0;
}

// ==============================================================================
void ChangeParamOverTime::timerCallback() {
    _sum_callbacks--;

    if( _sum_callbacks > 0 ) {
        change();
    }
    else
    {
        _param.set_scaled( _target_value );
        forceStopAndKill();
        return;
    }
}
void ChangeParamOverTime::change() {
    float new_value = _param + _value_delta;
    if( new_value > _param.get_max() )
        new_value = _param.get_max();
    else if( new_value < _param.get_min() )
        new_value = _param.get_min();

    _param.set_scaled( new_value );
}

ChangeParamOverTime::ChangeParamOverTime(mono_ParameterBase< float >& param_, float target_value_, int time_in_ms_)
    : _param( param_ ),
      _sum_callbacks( time_in_ms_/PARAM_CHANGE_INTERVAL_IN_MS ),
      _target_value( target_value_ ),
      _value_delta( (target_value_-param_)/_sum_callbacks )
{
    if( param_.timeChanger != nullptr ) {
        param_.timeChanger->forceStopAndKill();
    }
    param_.timeChanger = this;

    change();
    startTimer( PARAM_CHANGE_INTERVAL_IN_MS );
}
void ChangeParamOverTime::forceStopAndKill()
{
    stopTimer();
    if( _param.timeChanger == this )
        _param.timeChanger = nullptr;

    delete this;
    return;
}




