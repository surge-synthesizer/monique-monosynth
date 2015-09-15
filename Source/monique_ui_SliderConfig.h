/*
  ==============================================================================

    Monique_Ui_MainwindowConfig.h
    Created: 6 May 2015 9:03:35am
    Author:  monotomy

  ==============================================================================
*/

#ifndef Monique_Ui_MainwindowCONFIG_H_INCLUDED
#define Monique_Ui_MainwindowCONFIG_H_INCLUDED

#include "monique_core_Datastructures.h"
#include "monique_ui_ModulationSlider.h"
#include "monique_ui_LookAndFeel.h"

// TODO replace the pointers to the base by references to the real params

#define SHORTCUT_SHIFT "\nNote: Global shortcut: SHIFT."

#define SHORTCUT_CTRL  "\nNote: Global shortcut: CTRL."

#define EXAMPLE_START  "\nExample:\n----------\n"

#define LINE__  "--------------------\n"

#define GENERAL_MOD_EXAMPLE \
  "\n" \
  "General Modulation Example:\n" \
  "----------------------------------------------------------------------\n" \
  "CUTOFF                   = 50 (MIN:0, MAX:100)\n" \
  "CUTOFF-MODULATION        = +50%\n" \
  "Current ADSR-LFO-MIX amp = 50%\n" \
  "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n" \
  "Result CUTOFF            = 75\n" \
  "======================================================================\n" \
  "Formula: CUTOFF + (MAX-CUTOFF) * CUTOFF-MODULATION * ADSR-LFO-MIX"

#define NO_MOD_EFFECT "(Has no effect if MODUL is turned off)\n"

#define VALUES_EDITABLE_ON_TOP "\nNote: to edit the value via keyboard click on the top label."


#define GERNERAL_SIGNAL_FLOW         "\n" \
				     "\n" \
                                     "General signal flow:\n" \
                                     "--------------------\n" \
                                     "OSC's                -> FILTER 1 INPUT-> FILTER 1 FX-> FILTER 1 OUT-> MIX\n" \
                                     "OSC's or FILTER 1 OUT-> FILTER 2 INPUT-> ..........................-> MIX\n" \
                                     "OSC's or FILTER 2 OUT-> FILTER 3 INPUT-> ..........................-> MIX\n" \
                                     "MIX -> EQ -> FX -> YOUR EARS!"
#define GERNERAL_SLIDER_INFORMATION  "\n\n" \
                                     "_________________________________________________________________________\n" \
				     "\n" \
                                     "General slider info:\n" \
                                     "--------------------\n" \
                                     "To edit the value in velocity mode: press CTRL & drag the slider.\n" \
                                     "Double click the slider to return to the factory default value.\n" \
				     "To set the current state as return value: click the right mouse button and select 'set current value as new return'." \
				     GERNERAL_SIGNAL_FLOW

// VIRTUALISIERUNG: OSCI, TOP BUTTON

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Change the WAVE of this oscillator. \n"
               "Sine (LEFT), Square, Saw, Noise (RIGHT)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "OSC 1: Turns FM modulations on and off (except FM amount, FM amount is always controlled by the back slider).\n"
               "\n"
               "OSC 2 & 3: Turns sync to OSC 1 on and off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between FM amount and WAVE form sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Add an amount of the FM oscillator to this oscillator.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
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

    MoniqueSynthData*const synth_data;

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
        float value;
        if( synth_data->animate_modulations )
        {
            if( bool(is_lfo_modulated->get_value()) )
            {
                value = get_last_modulation_amount(octave)*(1.0f/48);
            }
            else
                value = NO_TOP_BUTTON_AMP;
        }
        else
        {
            if( bool(is_lfo_modulated->get_value()) )
            {
                value = 0.999f;
            }
            else
                value = 0;
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Detune this oscillator to the currently running note in semitones.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns TUNE modulation by LFO on and off.\n"
               "(OSC 1 uses LFO 1 (from FILTER 1), OSC 2 uses LFO 2... ."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between TUNE and LFO modulation range sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the LFO modulation range which will be added to the current TUNE.\n"
               "\n"
               "A modulation amount larger 0 will increase the TUNE. Values smaller 0 decrease.\n"

               EXAMPLE_START
               "TUNE = 0, MODULATION = +100%\n"
               "Current LFO amp = 50%\n"
               "Result TUNE = 12\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

public:
    OSCSlConfig( int id_ )
        :
        octave( &(GET_DATA(osc_datas[id_]).octave) ),
        is_lfo_modulated( &(GET_DATA(osc_datas[id_]).is_lfo_modulated ) ),
        top_text( String("LFO ") + String(id_+1) ),
        bottom_text( String("OSC ") + String(id_+1) ),

        synth_data( GET_DATA_PTR( synth_data ) )
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Set the frequency of the FM oscillator.\n"
               "Zero = OSC 1 frequency.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns sync to OSC 1 on and off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between FM FREQUENCY and FM SWING sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the FM SWING amount. Values larger zero switch the FM oscillator phase smoothly faster and faster.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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
        return "OSC PLUSE";
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define pulses for each oscillator.\n"
               "(Can by turned off for OSC 1 by enabling 'MOD OFF')\n"
               "\n"
               LINE__
               "\n"
               "Values larger zero insert cycle breaks of the duration of one OSC cycle.\n"
               "Examples:\n"
               "PULSE = 0 = --- _|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_ ---\n"
               "\n"
               "PULSE = 1 = --- _|‾|_____|‾|_____|‾|_____|‾|_____|‾|_____ ---\n"
               "\n"
               "PULSE = 2 = --- _|‾|_________|‾|_________|‾|_________|‾|_ ---\n"
               "\n"
               LINE__
               "\n"
               "Values smaller zero change the cycle duration. One becomes longer, the next one shorter.\n"
               "Examples:\n"
               "PULSE =   0 = --- _|‾‾‾‾‾‾|______|‾‾‾‾‾‾|______|‾‾‾‾‾‾|__ ---\n"
               "\n"
               "PULSE =  -1 = --- _|‾‾‾‾‾|_______|‾‾‾‾‾|_______|‾‾‾‾‾|___ ---\n"
               "\n"
               "PULSE = -12 = --- _|‾|___________|‾|___________|‾|_______ ---\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns FM SHOT for each oscillator on and off.\n"
               "(Can by turned off for OSC 1 by enabling 'MOD OFF')\n"
               "\n"
               "If FM SHOT is enabled only ONE cycle of the FM oscillator will be added to the OSC cylce and than it waits for a new cycle of the modulated OSC."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between OSC SWITCH and OSC PULSE sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define phase switches for each oscillator.\n"
               "(Can by turned off for OSC 1 by enabling 'MOD OFF')\n"
               "\n"
               LINE__
               "\n"
               "Values larger zero switches the phase after the defined number of cycles.\n"
               "Examples:\n"
               "PULSE = 0 = --- _|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_ ---\n"
               "\n"
               "PULSE = 1 = --- _|‾'‾|_,_|‾'‾|_,_|‾'‾|_,_|‾'‾|_,_|‾'‾|_,_ ---\n"
               "\n"
               "PULSE = 3 = --- _|‾|_|‾|_|‾|_,_|‾|_|‾|_|‾'‾|_|‾|_|‾|_|‾|_ ---\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    MoniqueSynthData*const synth_data;

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
            value = mono_ParameterOwnerStore::getInstance()->voice->get_flt_input_env_amp(filter_id,input_id);

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the input amount for an OSC into this FILTER.\n"
               "(Will be the sustain level if 'FIX IN' is turned off)\n"
               "\n"
               "FILTER 2 & 3:\n"
               "On the left side of the slider (minus) the raw OSC is used as input into the FILTER.\n"
               "If the slider is on the right side (plus) the filtered result of the OSC through the FILTER before is used as input.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns 'FIX IN' on and off.\n"
               "\n"
               "If 'FIX IN' is enabled the INPUT slider just defines the input amount.\n"
               "If disabled the INPUT slider amount will be the sustain level of an ADR curve you can choose with the ADR slider (back slider)."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between INPUT/SUSTAIN and ADR PRESET sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set an ADR curve to control the input amount for an OSC into this FILTER.\n"
               "(Will have no effect if 'FIX IN' is enabled)\n"
               "\n"
               "The slider position morphs between 4 ADR presets which you can define with the ADR preset editor (ADR button at the bottom of the main user interface).\n"
               "The INPUT slider amount will be the sustain level for your curve.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Set the amount of the filter DISTORTION.\n"
               "(Also processed if 'PASS' as filter type is selected)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns DISTORTION MODULATION by ADSR-LFO-MIX on or off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between DISTORTION and DISTORTION MODULATION sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the DISTORTION MODULATION amount.\n"
               NO_MOD_EFFECT
               GENERAL_MOD_EXAMPLE
               "\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    const bool is_main_adsr;

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
        return "RANGE";
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
        if( not attack->midi_control->get_ctrl_mode() )
        {
            float value = MIN_ENV_TIMES + attack->get_value() * max_attack_time->get_value();
            if( value < 100 )
                return String(round01(value));
            else
                return String(round0(value));
        }
        else
        {
            return String(max_attack_time->get_value());
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_-MIN_ENV_TIMES)/max_attack_time->get_value();
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        if( is_main_adsr )
        {
            return "Set the ATTACK time in ms.\n"
                   "\n"
                   "Controls the OUTPUT VOLUME (processed after EQ bank and before FX).\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
        else
        {
            return "Set the ATTACK time in ms.\n"
                   "\n"
                   "Possible targets: MOD-MIX\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between ATTACK and MAX ATTACK sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the maximum ATTACK time for the front slider.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

public:
    FAttackSlConfig( int id_ )
        :
        attack( &(GET_DATA(filter_datas[id_]).env_data->attack) ),
        max_attack_time( &(GET_DATA(filter_datas[id_]).env_data->max_attack_time) ),
        is_main_adsr(false)
    {}
    FAttackSlConfig()
        :
        attack(  &(GET_DATA(synth_data).env_data->attack) ),
        max_attack_time(  &(GET_DATA(synth_data).env_data->max_attack_time) ),
        is_main_adsr(true)
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

    const bool is_main_adsr;

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
        return "RANGE";
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
        if( not decay->midi_control->get_ctrl_mode() )
        {
            float value = decay->get_value() * max_decay_time->get_value();
            if( value < 0 )
                return "OFF";
            else if( value < 100 )
                return String(round01(value));
            else
                return String(round0(value));
        }
        else
        {
            return String(max_decay_time->get_value());
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_)/max_decay_time->get_value();
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        if( is_main_adsr )
        {
            return "Set the DECAY time in ms.\n"
                   "\n"
                   "Controls the OUTPUT VOLUME (processed after EQ bank and before FX).\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
        else
        {
            return "Set the DECAY time in ms.\n"
                   "\n"
                   "Possible targets: MOD-MIX\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between DECAY and MAX DECAY sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the maximum DECAY time for the front slider.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

public:
    FDecaySlConfig( int id_ )
        :
        decay( &(GET_DATA(filter_datas[id_]).env_data->decay) ),
        max_decay_time( &(GET_DATA(filter_datas[id_]).env_data->max_decay_time) ),
        is_main_adsr(false)
    {}
    FDecaySlConfig()
        :
        decay( &(GET_DATA(synth_data).env_data->decay) ),
        max_decay_time( &(GET_DATA(synth_data).env_data->max_decay_time) ),
        is_main_adsr(true)
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

    const bool is_main_adsr;

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
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
    return "RANGE";
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
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return entered_value_/100;
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        if( is_main_adsr )
        {
            return "Set the SUSTAIN level in percent.\n"
                   "\n"
                   "Controls the OUTPUT VOLUME (processed after EQ bank and before FX).\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
        else
        {
            return "Set the SUSTAIN level in percent.\n"
                   "\n"
                   "Possible targets: MOD-MIX\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
    {
    return "";
    }
    */

public:
    FSustainSlConfig( int id_ ) : sustain( &(GET_DATA(filter_datas[id_]).env_data->sustain) ), is_main_adsr(false) {}
    FSustainSlConfig() : sustain( &(GET_DATA(synth_data).env_data->sustain) ), is_main_adsr(true) {}

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

    const bool is_main_adsr;

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
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
    return "MAX T(ms)";
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
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return entered_value_/10000;
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        if( is_main_adsr )
        {
            return "Set the SUSTAIN time in ms.\n"
                   "\n"
                   "If the slider is set to max, the sustain time is unlimited (until note off).\n"
                   "\n"
                   "Controls the OUTPUT VOLUME (processed after EQ bank and before FX).\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
        else
        {
            return "Set the SUSTAIN time in ms.\n"
                   "\n"
                   "If the slider is set to max, the sustain time is unlimited (until note off)\n."
                   "\n"
                   "Possible targets: MOD-MIX\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
    {
    return "";
    }
    */

public:
    FSustainTimeSlConfig( int id_ ) : sustain_time( &(GET_DATA(filter_datas[id_]).env_data->sustain_time) ), is_main_adsr(false) {}
    FSustainTimeSlConfig() : sustain_time( &(GET_DATA(synth_data).env_data->sustain_time) ), is_main_adsr(true) {}

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

    const bool is_main_adsr;

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
        return "RANGE";
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
        if( not release->midi_control->get_ctrl_mode() )
        {
            float value = MIN_ENV_TIMES + release->get_value() * max_release_time->get_value();
            if( value < 100 )
                return String(round01(value));
            else
                return String(round0(value));
        }
        else
        {
            return String(max_release_time->get_value());
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_-MIN_ENV_TIMES)/max_release_time->get_value();
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        if( is_main_adsr )
        {
            return "Set the RELEASE time in ms.\n"
                   "\n"
                   "Controls the OUTPUT VOLUME (processed after EQ bank and before FX).\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
        else
        {
            return "Set the RELEASE time in ms.\n"
                   "\n"
                   "Possible targets: MOD-MIX\n"
                   "\n"
                   VALUES_EDITABLE_ON_TOP
                   GERNERAL_SLIDER_INFORMATION

                   ;
        }
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between RELEASE and MAX RELEASE sliders.\n"
               SHORTCUT_SHIFT

               ;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the maximum RELEASE time for the front slider.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

public:
    FReleaseSlConfig( int id_ )
        :
        release( &(GET_DATA(filter_datas[id_]).env_data->release) ),
        max_release_time( &(GET_DATA(filter_datas[id_]).env_data->max_release_time) ),
        is_main_adsr(false)
    {}

    FReleaseSlConfig()
        :
        release( &(GET_DATA(synth_data).env_data->release) ),
        max_release_time( &(GET_DATA(synth_data).env_data->max_release_time) ),
        is_main_adsr(true)
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the used MODULATION MIX of ADSR & LFO.\n"
               "\n"
               "If the slider is on the left side only the ADSR amp will be used as modulation signal.\n"
               "On the right side only the LFO amp will be uesd.\n"
               "In the middle 50% of the ADSR and 50% of the LFO will be used as modulator.\n"
               "\n"
               "Possible targets: CUTOFF, RESONANCE, GAIN, DESTROY, VOLUME\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
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
    return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
    return sync;
    }
    StringRef get_top_button_text() const noexcept override
    {
    return "IN-SYNC";
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
        {
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
        }
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Set the LFO speed.\n"
               "\n"
               "If the LFO speed is slow (note durations) the LFO is always synced to the current BPM.\n"
               "If the LFO speed is fast (note values) the LFO is not synced to anything.\n"
               "\n"
               "Possible targets: OSC, MODULATION-MIX\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
    {
    return "";
    }
    */

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the filter CUTOFF (filter frequency) in percent.\n"
               "(Has no effect if 'PASS' as filter type is selected)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns CUTOFF MODULATION by ADSR-LFO-MIX (MOD-MIX) on or off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between CUTOFF and CUTOFF MODULATION sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the CUTOFF MODULATION amount.\n"
               NO_MOD_EFFECT
               GENERAL_MOD_EXAMPLE
               "\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the filter RESONANCE (boost frequencys at the edge of the filter CUTOFF) in percent.\n"
               "(Has no effect if 'PASS' as filter type is selected)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns RESONANCE MODULATION by ADSR-LFO-MIX (MOD-MIX) on or off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between RESONANCE and RESONANCE MODULATION sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the RESONANCE MODULATION amount.\n"
               NO_MOD_EFFECT
               GENERAL_MOD_EXAMPLE
               "\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the filter GAIN (boost the filter frequencys) in percent.\n"
               "(Has no effect if 'PASS' as filter type is selected)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns GAIN MODULATION by ADSR-LFO-MIX (MOD-MIX) on or off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between GAIN and GAIN MODULATION sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the GAIN MODULATION amount.\n"
               NO_MOD_EFFECT
               GENERAL_MOD_EXAMPLE
               "\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the filter OUTPUT VOLUME in percent.\n"
               "\n"
               "Target: EQ bank, BOOST (of the next filter)\n"

               GERNERAL_SLIDER_INFORMATION
               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns VOLUME MODULATION by ADSR-LFO-MIX (MOD-MIX) on or off."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between VOLUME and VOLUME MODULATION sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set the VOLUME MODULATION amount.\n"
               NO_MOD_EFFECT
               GENERAL_MOD_EXAMPLE
               "\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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
    RuntimeInfo*const runtime_info;

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
        String value( round01(runtime_info->bpm) );
#ifdef IS_STANDALONE
        if( runtime_info->is_extern_synced )
        {
            value += String(" EXT");
        }
        else
        {
            value += String(" BPM");
        }
#else
        value += String(" BPM");
#endif

        return value;
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
        return String(round01(speed->get_value()));
    }
    /*
    StringRef get_center_suffix() const noexcept override
    {
    return "";
    }
    */

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the internal speed of Monique in BPM.\n"
               "(PLUGIN: has no effect if SYNC is turned on)\n"
               "(STANALONE: has no effect if SYNC is turned on and a MIDI clock is continuously received)\n"
               "\n"
               "Affected: ARPEGGIATOR, LFO (if slow) (OSC's and FM if LFO MOD is turned on)\n"

               GERNERAL_SLIDER_INFORMATION
               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns SPEED SYNC on or off.\n"
               "\n"
               "If disabled you can define your own speed ignoring the host speed (plugin) or the MIDI-Clock (standalone)."
               ;
    }
    /*
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
    {
    return "";
    }
    */

public:
    BPMSlConfig()
        :
        speed( &(GET_DATA(synth_data).speed) ),
        sync( &(GET_DATA(synth_data).sync) ),
        runtime_info( &(GET_DATA(runtime_info)) )
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
    Parameter*const speed;

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
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        return String(round0(speed->get_value()*ArpSequencerData::speed_multi_to_value(speed_multi->get_value())));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "BPM";
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the SPEED MULTIPLIER/DIVISOR.\n"
               "\n"
               "Affected: ARPEGGIATOR, LFO (if slow) (OSC's and FM if LFO MOD is turned on)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
    {
    return "";
    }
    */

public:
    SpeedMultiSlConfig()
        :
        speed_multi( &(GET_DATA(arp_data).speed_multi) ),
        speed( &(GET_DATA(synth_data).speed) )
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the OCTAVE OFFSET related to current root note (e.g. pressed on keyboard).\n"
               "\n"
               "Affected: OSC's, FM"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
    return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the PEAK BOOST amount of the filter output.\n"
               "\n"
               "FILTER 2 & 3: boosts or limits the filter ouput by peaks in the output of the filter before."
               " The boost attack & release time become more agressive with an larger value."

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between PEAK BOOST and SOFT CLIPPING sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define the SOFT CLIPPING amount of the filter output.\n"
               "\n"
               "Values larger zero clipps the output softly and minimizes very large peaks much more than smaller ones.\n"
               "Note that samples after the soft clipper will be clipped hard if they are still larger as the possible output volume (this will have a distortion effect (may be wanted, may not))."

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the SHAPE amount of the FX input (after EQ bank).\n"
               "\n"
               "Shaping makes some details in the sound more 'visible'."

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between PEAK BOOST and SOFT CLIPPING sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define the SOFT CLIPPING amount of the filter output.\n"
               "\n"
               "Values larger zero clipps the output softly and minimizes very large peaks much more than smaller ones.\n"
               "Note that samples after the soft clipper will be clipped hard if they are still larger as the possible output volume (this will have a distortion effect (may be wanted, may not))."

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the ROOM of the reverb effect.\n"
               "(Has no effect if FX MIX is set to zero)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the WIDTH of the reverb effect.\n"
               "(Has no effect if FX MIX is set to zero)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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
        return "DRY|WET";
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the WET/DRY amount of the reverb effect.\n"
               "(Has no effect if FX MIX is set to zero)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the DELAY time of the delay effect.\n"
               "(Has no effect if FX MIX is set to zero)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the amount of the FX input and output mix.\n"
               "\n"
               "Values larger zero add the FX output to the FX input (EQ output).\n(If the value is zero the FX section is bypassed)\n"
               "\n"
               "Affected:     REVERB, DELAY, CHORUS\n"
               "NOT Affected: SHAPE\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the final output volume.\n"

               GERNERAL_SLIDER_INFORMATION

               ;


    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return ""
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between MASTER VOLUME and SOFT CLIPPING sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define the SOFT CLIPPING amount of the final output.\n"
               "\n"
               "Values larger zero clipps the output softly and minimizes very large peaks much more than smaller ones.\n"
               "Note that samples after the soft clipper will be clipped hard if they are still larger as the possible output volume (this will have a distortion effect (may be wanted, may not))."

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    MoniqueSynthData*const synth_data;
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
            value = mono_ParameterOwnerStore::getInstance()->voice->get_chorus_modulation_env_amp();
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the CHORUS amount in percent.\n"
               "(Will be the sustain level if 'FIX' is turned off)\n"
               "(Has no effect if FX MIX is set to zero)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns 'FIX' on and off.\n"
               "\n"
               "If 'FIX IN' is enabled the CHORUS slider just defines the CHORUS amount.\n"
               "If disabled the CHORUS slider amount will be the sustain level of an ADR curve you can choose with the ADR slider (back slider)."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between CHORUS amount and CHORUS ADR sliders.\n"

               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set an ADR curve to control the CHORUS amount.\n"
               "(Has no effect if FIX is enabled)\n"
               "\n"
               "The slider position morphs between 4 ADR presets which you can define with the ADR preset editor (ADR button at the bottom of the main user interface).\n"
               "The CHORUS slider amount will be the sustain level for your curve.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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
    IntParameter*const velocity_glide_time;
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
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        float value;
        if( glide->midi_control->get_ctrl_mode() )
        {
            value = velocity_glide_time->get_value();
        }
        else
        {
            const float sr = RuntimeNotifyer::getInstanceWithoutCreating()->get_sample_rate();
            value = samplesToMsFast(glide->get_value()*sr/2,sr);
        }
        return String(round0(value));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the NOTE GLIDE time in ms.\n"
               "(Has no effect if the ARPEGGIATOR is turned off)\n"
               "\n"
               "Values larger zero will glide more and more smoothly from the last note to the current one.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Binds the steps of the ARPEGGIATOR.\n"
               "(Has no effect if the ARPEGGIATOR is turned off)\n"
               "\n"
               "If enabled all steps without an empty step between will be handled as one note (no envelope retrigger)."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between NOTE GLIDE time and VELOCITY GLIDE sliders.\n"

               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define the VELOCITY GLIDE time in ms.\n"
               "\n"
               "Values larger zero will glide more and more smoothly from the last velocity to the current one.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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
    IntParameter*const shuffle;
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
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        return ArpSequencerData::shuffle_to_text( shuffle->get_value() ).text;
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "th";
    }

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the SHUFFLE amount of the ARPEGGIATOR STEPS in note durations.\n"
               "(Has no effect if the ARPEGGIATOR is turned off)\n"
               "\n"
               "Values larger zero makes the duration of each second ARPEGGIATOR step shorter and the step before longer.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns the ARPEGGIATOR on or off.\n"
               "\n"
               "If enabled notes will automatically triggered at the position of the enabled ARPEGGIATOR STEPS."

               ;
    }
    /*
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

    MoniqueSynthData*const synth_data;

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
            value = mono_ParameterOwnerStore::getInstance()->voice->get_band_env_amp(id);
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the BAND BOOST amount for this frequency.\n"
               "(Will be the sustain level if 'FIX' is turned off)\n"
               "\n"

               "Values larger zero will boost this band. Values smaller zero will reduce this band"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    StringRef get_tootip_top() const noexcept override
    {
        return "Turns 'FIX' on and off.\n"
               "\n"
               "If 'FIX' is enabled the BAND BOOST slider just defines the boost amount.\n"
               "If disabled the BAND BOOST slider amount will be the sustain level of an ADR curve you can choose with the ADR slider (back slider)."

               ;
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between BAND BOOST and ADR PRESET sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Set an ADR curve to control the band boost amount for this frequency.\n"
               "(Will have no effect if 'FIX' is enabled)\n"
               "\n"
               "The slider position morphs between 4 ADR presets which you can define with the ADR preset editor (ADR button at the bottom of the main user interface).\n"
               "The BAND BOOST slider amount will be the sustain level for your curve.\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }

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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Define the NOTE OFFSET for this step in semitones.\n"
               "(Has no effect if the ARPEGGIATOR is turned off)\n"
               "\n"
               "See: NOTE-G (NOTE GLIDE) (left)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "SEE MAINWINDOW";
    }
    */
    StringRef get_tootip_bottom() const noexcept override
    {
        return "Switch between NOTE OFFSET and VELOCITY OFFSET sliders.\n"
               SHORTCUT_SHIFT;
    }
    StringRef get_tootip_back() const noexcept override
    {
        return "Define the VELOCITY amount for this step.\n"
               "(Has no effect if the ARPEGGIATOR is turned off)\n"
               "\n"
               "See: VELO-G (VELOCITY GLIDE) (left)\n"

               GERNERAL_SLIDER_INFORMATION

               ;
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

    //==============================================================================
    // TOOLTIP
    StringRef get_tootip_front() const noexcept override
    {
        return "Morph between two programs, one the LEFT side and one on the RIGHT side.\n"
               "\n"
	       "\n"
               "OSC Morph Group:\n"
	       "Morphs: OSC's, FM\n"
	       "Except: OSC SWITCH, OSC PULSE, BUTTONS\n"
	       "\n"
               "FLT Morph Group:\n"
	       "Morphs: FILTERS\n"
	       "Except: FILTER TYPE, BUTTONS\n"
	       "\n"
	       "FX Morph Group:\n"
	       "Morphs: EQ BANK, SHAPE, REVERB, DELAY, CHORUS, FX-MIX, MASTER VOLUME, ADR PRESETS\n"
	       "Except: BUTTONS\n"
	       "\n"
	       "ARP Morph Group:\n"
	       "Morphs: (ARPEGGIATOR), SHUFFLE, NOTE-G, VELO-G, STEP NOTE, STEP VELOCITY\n"
	       "Except: BUTTONS\n"
	       "\n"
	       "\n"
	       "With the MORPH EDITOR you can load each of your available programs to the left or right side of each morph group."
               GERNERAL_SLIDER_INFORMATION

               ;
    }
    /*
    StringRef get_tootip_top() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_bottom() const noexcept override
    {
        return "";
    }
    StringRef get_tootip_back() const noexcept override
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

#endif  // Monique_Ui_MainwindowCONFIG_H_INCLUDED

