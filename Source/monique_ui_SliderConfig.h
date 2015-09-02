/*
  ==============================================================================

    UiEditorSynthLiteConfig.h
    Created: 6 May 2015 9:03:35am
    Author:  monotomy

  ==============================================================================
*/

#ifndef UIEDITORSYNTHLITECONFIG_H_INCLUDED
#define UIEDITORSYNTHLITECONFIG_H_INCLUDED

#include "monique_core_Datastructures.h"
#include "monique_ui_ModulationSlider.h"
#include "monique_ui_LookAndFeel.h"

// TODO replace the pointers to the base by references to the real params

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class WAVESlConfig : public ModulationSliderConfigBase
{
    Parameter*const wave;
    Parameter*const fm_amount;
    BoolParameter*const top_parameter;

    const String top_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return false;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return wave;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    Parameter* get_back_parameter_base() const noexcept override
    {
        return fm_amount;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return top_parameter;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return top_text;
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "WAVE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "FM";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( fm_amount->midi_control->get_ctrl_mode() )
            return String( round0(fm_amount->get_value()*100) );
        else
            return String( wave->get_value() );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( fm_amount->midi_control->get_ctrl_mode() )
            return "%";
        else
            return "wav";
    }

public:
    WAVESlConfig( int id_ )
        :
        wave( &(GET_DATA(osc_datas[id_]).wave) ),
        fm_amount( &(GET_DATA(osc_datas[id_]).fm_amount) ),
        top_parameter( id_ == 0 ? &(GET_DATA(osc_datas[id_]).mod_off) : &(GET_DATA(osc_datas[id_]).sync) ),
        top_text( id_ == 0 ? "MOD OFF" : "SYNC" )
    {}

    JUCE_LEAK_DETECTOR (WAVESlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class OSCSlConfig : public ModulationSliderConfigBase
{
    ModulatedParameter*const octave;
    BoolParameter*const is_lfo_modulated;

    const String top_text;
    const String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return octave;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    Parameter* get_back_parameter_base() const noexcept override
    {
        return octave;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return is_lfo_modulated;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return top_text;
    }
    float get_top_button_amp() const noexcept override
    {
        return get_last_modulation_amount(octave)*(1.0f/48);
    }

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        const float value = octave->get_value();
        if( octave->midi_control->get_ctrl_mode() )
            return String( round0(octave->get_modulation_amount() * 100) );
        else
        {
            if( value < 10 and value > -10 )
                return String(round001(value));
            else
                return String(round01(value));
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( octave->midi_control->get_ctrl_mode() )
            return "%";
        else
            return "#";
    }

public:
    OSCSlConfig( int id_ )
        :
        octave( &(GET_DATA(osc_datas[id_]).octave) ),
        is_lfo_modulated( &(GET_DATA(osc_datas[id_]).is_lfo_modulated ) ),
        top_text( String("LFO ") + String(id_+1) ),
        bottom_text( String("OSC ") + String(id_+1) )
    {}

    JUCE_LEAK_DETECTOR (OSCSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FMFreqSlConfig : public ModulationSliderConfigBase
{
    Parameter*const fm_multi;
    Parameter*const fm_swing;
    BoolParameter*const sync;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return false;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return fm_multi;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return fm_swing;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return sync;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "SYNC -";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "FM FREQ";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "FM SWING";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FMFreqSlConfig()
        :
        fm_multi( &(GET_DATA(osc_datas[0]).fm_multi) ),
        fm_swing( &(GET_DATA(osc_datas[0]).fm_swing) ),
        sync( &(GET_DATA(osc_datas[0]).sync) )
    {}

    JUCE_LEAK_DETECTOR (FMFreqSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FMAmountSlConfig : public ModulationSliderConfigBase
{
    Parameter*const puls_width;
    Parameter*const osc_switch;
    BoolParameter*const fm_wave;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return puls_width;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return osc_switch;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return fm_wave;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "- SHOT";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "OSC PLUS";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "OSC SWITCH";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( puls_width->midi_control->get_ctrl_mode() )
            return String( osc_switch->get_value() );
        else
            return String( puls_width->get_value() );
    }
    /*
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FMAmountSlConfig()
        :
        puls_width( &(GET_DATA(osc_datas[0]).puls_width) ),
        osc_switch( &(GET_DATA(osc_datas[0]).osc_switch) ),
        fm_wave( &(GET_DATA(osc_datas[0]).fm_wave) )
    {}

    JUCE_LEAK_DETECTOR (FMAmountSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class InputSlConfig : public ModulationSliderConfigBase
{
    const int filter_id;
    const int input_id;

    Parameter*const input_sustain;
    Parameter*const state;
    BoolParameter*const input_hold;

    const String bottom_text;
    const String input_text;

    SynthData*const synth_data;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return input_sustain;
    }
    int get_override_front_min_value() const noexcept override
    {
        if( filter_id == 0 )
            return 0;
        else
            return DONT_OVERRIDE_SLIDER_VALUE;
    }
    /*
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return state;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return input_hold;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "FIX IN";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! synth_data->animate_input_env )
            value = NO_TOP_BUTTON_AMP;
        else if( not bool(input_hold->get_value()) )
            value = mono_ParameterOwnerStore::getInstance()->get_flt_input_env_amp(filter_id,input_id);

        return value;
    }

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "ADR";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( state->midi_control->get_ctrl_mode() )
            return String( round001(state->get_value()) );
        else
            return String( round01(input_sustain->get_value()*100)  );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( state->midi_control->get_ctrl_mode() )
            return "X";
        else if( input_sustain->get_value() >= 0 )
        {
            if( filter_id == 1 )
                return "F1";
            else if( filter_id == 2 )
                return "F2";
        }

        return input_text;
    }

public:
    InputSlConfig( int filter_id_, int input_id_ )
        :
        filter_id( filter_id_ ),
        input_id( input_id_ ),
        input_sustain( &(GET_DATA(filter_datas[filter_id_]).input_sustains[input_id_]) ),
        state( &(GET_DATA(filter_datas[filter_id_]).input_env_datas[input_id_]->state ) ),
        input_hold( &(GET_DATA(filter_datas[filter_id_]).input_holds[input_id_]) ),

        bottom_text( String("OSC ") + String(input_id_+1) ),
        input_text( String("O") + String(input_id_+1) ),

        synth_data( GET_DATA_PTR( synth_data ) )
    {}

    JUCE_LEAK_DETECTOR (InputSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class GForceSlConfig : public ModulationSliderConfigBase
{
    Parameter*const distortion;
    BoolParameter*const modulate_distortion;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return false;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return distortion;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return distortion;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_distortion;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MODUL";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "DESTROY";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    GForceSlConfig( int id_ )
        :
        distortion( &(GET_DATA(filter_datas[id_]).distortion) ),
        modulate_distortion( &(GET_DATA(filter_datas[id_]).modulate_distortion) )
    {}

    JUCE_LEAK_DETECTOR (GForceSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FAttackSlConfig : public ModulationSliderConfigBase
{
    Parameter*const attack;
    Parameter*const max_attack_time;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return attack;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_attack_time;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "ATTACK";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MAX T(ms)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value = attack->get_value() * max_attack_time->get_value();
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }

public:
    FAttackSlConfig( int id_ )
        :
        attack( &(GET_DATA(filter_datas[id_]).env_data->attack) ),
        max_attack_time( &(GET_DATA(filter_datas[id_]).env_data->max_attack_time) )
    {}
    FAttackSlConfig()
        :
        attack(  &(GET_DATA(synth_data).env_data->attack) ),
        max_attack_time(  &(GET_DATA(synth_data).env_data->max_attack_time) )
    {}

    JUCE_LEAK_DETECTOR (FAttackSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FDecaySlConfig : public ModulationSliderConfigBase
{
    Parameter*const decay;
    Parameter*const max_decay_time;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return decay;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_decay_time;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "DECAY";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MAX T(ms)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value = decay->get_value() * max_decay_time->get_value();
        if( value < 0 )
            return "OFF";
        else if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }

public:
    FDecaySlConfig( int id_ )
        :
        decay( &(GET_DATA(filter_datas[id_]).env_data->decay) ),
        max_decay_time( &(GET_DATA(filter_datas[id_]).env_data->max_decay_time) )
    {}
    FDecaySlConfig()
        :
        decay( &(GET_DATA(synth_data).env_data->decay) ),
        max_decay_time( &(GET_DATA(synth_data).env_data->max_decay_time) )
    {}

    JUCE_LEAK_DETECTOR (FDecaySlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FSustainSlConfig : public ModulationSliderConfigBase
{
    Parameter*const sustain;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return sustain;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_decay_time;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "SUSTAIN";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MAX T(ms)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value = sustain->get_value() * 100;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "%";
    }

public:
    FSustainSlConfig( int id_ ) : sustain( &(GET_DATA(filter_datas[id_]).env_data->sustain) ) {}
    FSustainSlConfig() : sustain( &(GET_DATA(synth_data).env_data->sustain) ) {}

    JUCE_LEAK_DETECTOR (FSustainSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FSustainTimeSlConfig : public ModulationSliderConfigBase
{
    Parameter*const sustain_time;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return sustain_time;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_decay_time;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "SUS TIME";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MAX T(ms)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value = sustain_time->get_value() * 10000;
        if( value < 100 )
            return String(round01(value));
        else if( value == 10000 )
            return String("unltd");
        else
            return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( sustain_time->get_value() == 1 )
            return "";
        else
            return "ms";
    }

public:
    FSustainTimeSlConfig( int id_ ) : sustain_time( &(GET_DATA(filter_datas[id_]).env_data->sustain_time) ) {}
    FSustainTimeSlConfig() : sustain_time( &(GET_DATA(synth_data).env_data->sustain_time) ) {}

    JUCE_LEAK_DETECTOR (FSustainTimeSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FReleaseSlConfig : public ModulationSliderConfigBase
{
    Parameter*const release;
    Parameter*const max_release_time;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return release;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_release_time;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "RELEASE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MAX T(ms)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value = release->get_value() * max_release_time->get_value();
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }

public:
    FReleaseSlConfig( int id_ )
        :
        release( &(GET_DATA(filter_datas[id_]).env_data->release) ),
        max_release_time( &(GET_DATA(filter_datas[id_]).env_data->max_release_time) )
    {}

    FReleaseSlConfig()
        :
        release( &(GET_DATA(synth_data).env_data->release) ),
        max_release_time( &(GET_DATA(synth_data).env_data->max_release_time) )
    {}

    JUCE_LEAK_DETECTOR (FReleaseSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class EnvLfoSlConfig : public ModulationSliderConfigBase
{
    Parameter*const adsr_lfo_mix;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return adsr_lfo_mix;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return UNDEFINED_SLIDER_STYLE;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return nullptr;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "MOD MIX";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    EnvLfoSlConfig( int id_ )
        :
        adsr_lfo_mix( &(GET_DATA(filter_datas[id_]).adsr_lfo_mix) )
    {}

    JUCE_LEAK_DETECTOR (EnvLfoSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class LFOSlConfig : public ModulationSliderConfigBase
{
    Parameter*const speed;
    String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return speed;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return UNDEFINED_SLIDER_STYLE;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return nullptr;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        const float speed_ = speed->get_value();
        if( speed_ <= 6 )
        {
            if( speed_ == 0 )
                return "16/1";
            else if( speed_ == 1 )
                return "12/1";
            else if( speed_ == 2 )
                return "8/1";
            else if( speed_ == 3 )
                return "4/1";
            else if( speed_ == 4 )
                return "3/1";
            else if( speed_ == 5 )
                return "2/1";
            else
                return "1/1";
        }
        else if( speed_ <= 17 )
            if( speed_ == 7 )
                return "3/4";
            else if( speed_ == 8 )
                return "1/2";
            else if( speed_ == 9 )
                return "1/3";
            else if( speed_ == 10 )
                return "1/4";
            else if( speed_ == 11 )
                return "1/8";
            else if( speed_ == 12 )
                return "1/12";
            else if( speed_ == 13 )
                return "1/16";
            else if( speed_ == 14 )
                return "1/24";
            else if( speed_ == 15 )
                return "1/32";
            else if( speed_ == 16 )
                return "1/64";
            else
                return "1/128";
        else
        {
            return MidiMessage::getMidiNoteName(frequencyToMidi(midiToFrequency(33+speed_-18)),true,true,0);
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( speed->get_value() <= 17 )
            return "th";
        else
            return "#";
    }

public:
    LFOSlConfig( int id_ )
        :
        speed( &(GET_DATA(lfo_datas[id_]).speed) ),
        bottom_text( "LFO " + String(id_+1) )
    {}

    JUCE_LEAK_DETECTOR (LFOSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FCutoffSLConfig : public ModulationSliderConfigBase
{
    Parameter*const cutoff;
    BoolParameter*const modulate_cutoff;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return cutoff;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return cutoff;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_cutoff;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MODUL";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "CUTOFF";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FCutoffSLConfig( int id_ )
        :
        cutoff( &(GET_DATA(filter_datas[id_]).cutoff) ),
        modulate_cutoff( &(GET_DATA(filter_datas[id_]).modulate_cutoff) )
    {}

    JUCE_LEAK_DETECTOR (FCutoffSLConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FResonanceSLConfig : public ModulationSliderConfigBase
{
    Parameter*const resonance;
    BoolParameter*const modulate_resonance;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return resonance;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return resonance;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_resonance;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MODUL";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "RESO";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FResonanceSLConfig( int id_ )
        :
        resonance( &(GET_DATA(filter_datas[id_]).resonance) ),
        modulate_resonance( &(GET_DATA(filter_datas[id_]).modulate_resonance) )
    {}

    JUCE_LEAK_DETECTOR (FResonanceSLConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FGainSLConfig : public ModulationSliderConfigBase
{
    Parameter*const gain;
    BoolParameter*const modulate_gain;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return gain;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return gain;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_gain;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MODUL";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "GAIN";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FGainSLConfig( int id_ )
        :
        gain( &(GET_DATA(filter_datas[id_]).gain) ),
        modulate_gain( &(GET_DATA(filter_datas[id_]).modulate_gain) )
    {}

    JUCE_LEAK_DETECTOR (FGainSLConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FVolumeSlConfig : public ModulationSliderConfigBase
{
    Parameter*const volume;
    BoolParameter*const modulate_volume;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return volume;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return volume;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_volume;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MODUL";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "VOLUME";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MOD (%)";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FVolumeSlConfig( int id_ )
        :
        volume( &(GET_DATA(filter_datas[id_]).output) ),
        modulate_volume( &(GET_DATA(filter_datas[id_]).modulate_output) )
    {}

    JUCE_LEAK_DETECTOR (FVolumeSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class BPMSlConfig : public ModulationSliderConfigBase
{
    Parameter*const speed;
    BoolParameter*const sync;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return speed;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return speed;
    }
    */

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return sync;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "SYNC";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return String(speed->get_value())+String(" BPM");
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    */
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        return String(speed->get_value());
    }
    /*
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    BPMSlConfig()
        :
        speed( &(GET_DATA(synth_data).speed) ),
        sync( &(GET_DATA(synth_data).sync) )
    {}

    JUCE_LEAK_DETECTOR (BPMSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class SpeedMultiSlConfig : public ModulationSliderConfigBase
{
    Parameter*const speed_multi;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return speed_multi;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return speed_multi;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "x1";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return ArpSequencerData::speed_multi_to_text( speed_multi->get_value() );
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    */
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return true;
    }

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    SpeedMultiSlConfig()
        :
        speed_multi( &(GET_DATA(arp_data).speed_multi) )
    {}

    JUCE_LEAK_DETECTOR (SpeedMultiSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class OctaveOffsetSlConfig : public ModulationSliderConfigBase
{
    Parameter*const octave_offset;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return octave_offset;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return speed_multi;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "x1";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    /*
    StringRef get_bottom_button_text() const noexcept override
    {
        return speed_multi_to_text( speed_multi->get_scaled_value() );
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return true;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        switch( int(octave_offset->get_value()) )
        {
        case 0 :
            return "+/-";
        case 1 :
            return "+1";
        case 2 :
            return "+2";
        case -1 :
            return "-1";
        default :
            return "-2";
        }
    }
    /*
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    OctaveOffsetSlConfig()
        :
        octave_offset( &(GET_DATA(synth_data).octave_offset) )
    {}

    JUCE_LEAK_DETECTOR (OctaveOffsetSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FCompressorSlConfig : public ModulationSliderConfigBase
{
    const int id;

    Parameter*const compressor;
    Parameter*const clipping;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return compressor;
    }
    int get_override_front_min_value() const noexcept override
    {
        if( id == 0 )
            return 0;
        else
            return DONT_OVERRIDE_SLIDER_VALUE;
    }
    /*
    int get_override_front_max_value() const noexcept override
    {
        if( id == 0 )
            return 0;
        else
            return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "BOOST";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "CLIPP";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FCompressorSlConfig( int id_ )
        :
        id( id_ ),
        compressor( &(GET_DATA(filter_datas[id]).compressor) ),
        clipping( &(GET_DATA(filter_datas[id]).output_clipping) )
    {}

    JUCE_LEAK_DETECTOR (FCompressorSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class FColourSlConfig : public ModulationSliderConfigBase
{
    Parameter*const shape;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return shape;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "SHAPE";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    FColourSlConfig()
        :
        shape( &(GET_DATA(synth_data).resonance) )
    {}

    JUCE_LEAK_DETECTOR (FColourSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class RRoomSlConfig : public ModulationSliderConfigBase
{
    Parameter*const room;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return room;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "ROOM";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    RRoomSlConfig()
        :
        room( &(GET_DATA(reverb_data).room) )
    {}

    JUCE_LEAK_DETECTOR (RRoomSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class RWidthSlConfig : public ModulationSliderConfigBase
{
    Parameter*const width;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return width;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "WIDTH";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    RWidthSlConfig()
        :
        width( &(GET_DATA(reverb_data).width) )
    {}

    JUCE_LEAK_DETECTOR (RWidthSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class RDrySlConfig : public ModulationSliderConfigBase
{
    Parameter*const dry_wet_mix;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return dry_wet_mix;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "WET|DRY";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    RDrySlConfig()
        :
        dry_wet_mix( &(GET_DATA(reverb_data).dry_wet_mix) )
    {}

    JUCE_LEAK_DETECTOR (RDrySlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class DelaySlConfig : public ModulationSliderConfigBase
{
    Parameter*const delay;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return delay;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "DELAY";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    DelaySlConfig()
        :
        delay( &(GET_DATA(synth_data).delay) )
    {}

    JUCE_LEAK_DETECTOR (DelaySlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class BypassConfig : public ModulationSliderConfigBase
{
    Parameter*const effect_bypass;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return effect_bypass;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "MIX";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    BypassConfig()
        :
        effect_bypass( &(GET_DATA(synth_data).effect_bypass) )
    {}

    JUCE_LEAK_DETECTOR (BypassConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class VolumeConfig : public ModulationSliderConfigBase
{
    Parameter*const volume;
    Parameter*const final_compression;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return volume;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return final_compression;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "VOLUME";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "CLIPP";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    VolumeConfig()
        :
        volume( &(GET_DATA(synth_data).volume) ),
        final_compression( &(GET_DATA(synth_data).final_compression) )
    {}

    JUCE_LEAK_DETECTOR (VolumeConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class CModSlConfig : public ModulationSliderConfigBase
{
    Parameter*const modulation;
    Parameter*const state;
    BoolParameter*const hold_modulation;

    SynthData*const synth_data;
    ChorusData*const chorus_data;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return modulation;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return state;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return hold_modulation;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "FIX";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! synth_data->animate_eq_env )
            value = NO_TOP_BUTTON_AMP;
        else if( ! bool(hold_modulation->get_value())  )
        {
            value = mono_ParameterOwnerStore::getInstance()->get_chorus_modulation_env_amp();
        }

        return value;
    }

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "CHORS";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "ADR";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( modulation->midi_control->get_ctrl_mode() )
            return String( round001(state->get_value()) );
        else
            return String( round01(chorus_data->modulation*100)  );
    }
    /*
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    CModSlConfig()
        :
        modulation( &(GET_DATA(chorus_data).modulation) ),
        state( &(GET_DATA(chorus_data).modulation_env_data->state) ),
        hold_modulation( &(GET_DATA(chorus_data).hold_modulation) ),

        synth_data( GET_DATA_PTR( synth_data ) ),
        chorus_data( GET_DATA_PTR( chorus_data ) )
    {}

    JUCE_LEAK_DETECTOR (CModSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class GlideConfig : public ModulationSliderConfigBase
{
    Parameter*const glide;
    Parameter*const velocity_glide_time;
    BoolParameter*const connect;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return glide;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return velocity_glide_time;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return connect;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "BIND";
    }
    /*
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "NOTE-G";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "VELO-G";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    GlideConfig()
        :
        glide( &(GET_DATA(synth_data).glide) ),
        velocity_glide_time( &(GET_DATA(synth_data).velocity_glide_time) ),
        connect( &(GET_DATA(arp_data).connect) )
    {}

    JUCE_LEAK_DETECTOR (GlideConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class ShuffleConfig : public ModulationSliderConfigBase
{
    Parameter*const shuffle;
    BoolParameter*const is_on;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return shuffle;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return is_on;
    }
    */

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return is_on;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "ARP";
    }
    float get_top_button_amp() const noexcept override
    {
        return is_on->get_value() ? 1 : 0;
    }

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "SHUFL";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    ShuffleConfig()
        :
        shuffle( &(GET_DATA(arp_data).shuffle) ),
        is_on( &(GET_DATA(arp_data).is_on) )
    {}

    JUCE_LEAK_DETECTOR (ShuffleConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class EQSlConfig : public ModulationSliderConfigBase
{
    const int id;

    Parameter*const velocity;
    BoolParameter*const hold;
    Parameter*const state;

    String bottom_text;

    SynthData*const synth_data;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return velocity;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return state;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return hold;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "FIX";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! synth_data->animate_eq_env )
            value = NO_TOP_BUTTON_AMP;
        else if( ! bool(hold->get_value()) ) {
            value = mono_ParameterOwnerStore::getInstance()->get_band_env_amp(id);
        }

        return value;
    }

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "ADR";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    EQSlConfig( int id_ )
        :
        id( id_ ),
        velocity( &(GET_DATA(eq_data).velocity[id_]) ),
        hold( &(GET_DATA(eq_data).hold[id]) ),
        state( &(GET_DATA(eq_data).env_datas[id_]->state) ),

        synth_data( GET_DATA_PTR( synth_data ) )
    {
        const float frequency_low_pass = (62.5f/2) * pow(2,id_+1);
        const float frequency_high_pass = frequency_low_pass / 2.0f;
        bottom_text = String( frequency_high_pass ) + String("Hz");
    }

    JUCE_LEAK_DETECTOR (EQSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class ArpStepSlConfig : public ModulationSliderConfigBase
{
    Parameter*const tune;
    Parameter*const velocity;

    const String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return tune;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return velocity;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "VELOCITY";
    }
    /*
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( tune->midi_control->get_ctrl_mode() )
            return String( round01(velocity->get_value()*100) );
        else
            return String( tune->get_value() );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( tune->midi_control->get_ctrl_mode() )
            return "%";
        else
            return "#";
    }

public:
    ArpStepSlConfig( int id_ )
        :
        tune( &(GET_DATA(arp_data).tune[id_]) ),
        velocity( &(GET_DATA(arp_data).velocity[id_]) ),
        bottom_text( id_ == 0 ? String(("STEP " + String(id_+1))) : String(id_) )
    {}

    JUCE_LEAK_DETECTOR (ArpStepSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class MorphSLConfig : public ModulationSliderConfigBase
{
    Parameter*const morhp_state;
    const String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return morhp_state;
    }
    /*
    int get_override_front_min_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    int get_override_front_max_value() const noexcept override
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    */

    //==============================================================================
    // BACK SLIDER
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return clipping;
    }
    */

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return nullptr;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "";
    }
    float get_top_button_amp() const noexcept override
    {
        return NO_TOP_BUTTON_AMP;
    }
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "MIX";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "";
    }
    bool get_is_bottom_button_text_dynamic() const noexcept override
    {
        return false;
    }
    */

    //==============================================================================
    // CENTER LABEL
    /*
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    String get_center_value() const noexcept override
    {
        return "";
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }
    */

public:
    MorphSLConfig(int id_)
        :
        morhp_state( &(GET_DATA(synth_data).morhp_states[id_]) ),
        bottom_text( String( "TOGGL" ) + String(id_+1) )
    {}

    JUCE_LEAK_DETECTOR (MorphSLConfig)
};

/*
        if (sliderThatWasMoved == sl_morph_1)
    {
        //[UserSliderCode_sl_morph_1] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
        &synth_data->morhp_states[0],
        sliderThatWasMoved
        )
        else
    {
        synth_data->morph( 0, sl_morph_1->getValue()/1000, true );
    }
        show_info_popup( sliderThatWasMoved, synth_data->morhp_states[0].midi_control );
        //[/UserSliderCode_sl_morph_1]
    }
        else if (sliderThatWasMoved == sl_morph_2)
    {
        //[UserSliderCode_sl_morph_2] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
        &synth_data->morhp_states[1],
        sliderThatWasMoved
        )
        else
    {
        synth_data->morph( 1, sl_morph_2->getValue()/1000, true );
    }
        show_info_popup( sliderThatWasMoved, synth_data->morhp_states[1].midi_control );
        //[/UserSliderCode_sl_morph_2]
    }
        else if (sliderThatWasMoved == sl_morph_3)
    {
        //[UserSliderCode_sl_morph_3] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
        &synth_data->morhp_states[2],
        sliderThatWasMoved
        )
        else
    {
        synth_data->morph( 2, sl_morph_3->getValue()/1000, true );
    }
        show_info_popup( sliderThatWasMoved, synth_data->morhp_states[2].midi_control );
        //[/UserSliderCode_sl_morph_3]
    }
        else if (sliderThatWasMoved == sl_morph_4)
    {
        //[UserSliderCode_sl_morph_4] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
        &synth_data->morhp_states[3],
        sliderThatWasMoved
        )
        else
    {
        synth_data->morph( 3, sl_morph_4->getValue()/1000, true );
    }
        show_info_popup( sliderThatWasMoved, synth_data->morhp_states[3].midi_control );
        //[/UserSliderCode_sl_morph_4]
    }
        else if (sliderThatWasMoved == sl_morhp_mix)
    {
        //[UserSliderCode_sl_morhp_mix] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
        &synth_data->linear_morhp_state,
        sliderThatWasMoved
        )
        else
    {
        synth_data->linear_morhp_state = sl_morhp_mix->getValue()/1000;
    }
        show_info_popup( sliderThatWasMoved, synth_data->linear_morhp_state.midi_control );
        //[/UserSliderCode_sl_morhp_mix]
    }
        */

#endif  // UIEDITORSYNTHLITECONFIG_H_INCLUDED
