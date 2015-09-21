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


//==============================================================================
//==============================================================================
//==============================================================================
#define LINE_DOTTED  "-------------------------------------------------------------------------\n"
#define LINE_STRIPE  "_________________________________________________________________________\n"

//==============================================================================
//==============================================================================
//==============================================================================
#define GERNERAL_SIGNAL_FLOW         "\n\n" \
                                     LINE_STRIPE \
				     "\n" \
                                     "GENERAL SIGNAL FLOW:\n" \
                                     "--------------------\n" \
                                     "OSC's                -> FILTER 1 INPUT-> FILTER 1 FX-> FILTER 1 OUT-> MIX\n" \
                                     "OSC's or FILTER 1 OUT-> FILTER 2 INPUT-> ..........................-> MIX\n" \
                                     "OSC's or FILTER 2 OUT-> FILTER 3 INPUT-> ..........................-> MIX\n" \
                                     "MIX -> EQ -> FX -> YOUR EARS!"

//==============================================================================
//==============================================================================
//==============================================================================
#define GERNERAL_SLIDER_INFORMATION  "\n\n" \
                                     LINE_STRIPE \
				     "\n" \
                                     "GENERAL SLIDER/DIAL INFO:\n" \
                                     "-------------------------\n" \
                                     "To edit the value in velocity mode: press CTRL & drag the slider.\n" \
                                     "Double click the slider to return to the factory default value.\n" \
				     "To set the current state as return value: click the right mouse button and select 'set current value as new return'."

//==============================================================================
//==============================================================================
//==============================================================================
#define TOP_SLIDER_DESCIPTION( text ) \
StringRef get_tootip_front() const noexcept override \
{ \
    return \
    text "\n" \
    GERNERAL_SLIDER_INFORMATION \
    GERNERAL_SIGNAL_FLOW; \
}

//==============================================================================
//==============================================================================
//==============================================================================
#define BACK_SLIDER_DESCRIPTION( text ) \
StringRef get_tootip_back() const noexcept override \
{ \
  return \
  text "\n" \
  GERNERAL_SLIDER_INFORMATION \
  GERNERAL_SIGNAL_FLOW; \
}

//==============================================================================
//==============================================================================
//==============================================================================
#define GERNERAL_TOP_BUTTON_INFORMATION "\n\n" \
					LINE_STRIPE \
					"\n" \
					"GENERAL TOP BUTTON INFO:\n" \
					"------------------------\n" \
					"Buttons on top of sliders/dials turns usually modulations for the front slider on or off." \
					" The modulations are usually controlled by the back slider."

#define TOP_BUTTON_DESCRIPTION( text ) \
StringRef get_tootip_top() const noexcept override \
{ \
  return \
  text "\n" \
  GERNERAL_TOP_BUTTON_INFORMATION; \
}

//==============================================================================
//==============================================================================
//==============================================================================
#define GERNERAL_SWITCH_BUTTON_INFORMATION  "\n\n" \
                                     LINE_STRIPE \
				     "\n" \
                                     "GENERAL BOTTOM BUTTON INFO:\n" \
                                     "---------------------------\n" \
                                     "Global shortcut: SHIFT\n" \
                                     "Buttons below sliders/dials switch always from front slider to back slider and vice versa.\n"

//==============================================================================
//==============================================================================
//==============================================================================
#define GENERAL_MOD_EXAMPLE "\n\n" \
				     LINE_STRIPE \
				     "\n" \
				     "GENERAL MODULATION EXAMPLE:\n" \
				     "---------------------------\n" \
				     "Defined CUTOFF is 50 (MIN:0, MAX:100).\n" \
				     "Defined CUTOFF-MODULATION is plus 50%.\n" \
				     "Current ADSR-LFO-MIX amplitude is 50% of the maximum.\n" \
				     "The outcome CUTOFF will be 62.5.\n" \
				     "\n" \
				     "FORMULA:\n" \
				     "--------\n" \
				     "OUTCOME = CUTOFF + (MAX-CUTOFF) * CUTOFF-MODULATION * ADSR-LFO-MIX\n" \
				     "62.5    = 50     + (100-50)     * 50%               * 50%" \
 
//==============================================================================
//==============================================================================
//==============================================================================
#define BOTTOM_BUTTON_SLIDERS( front_name, back_name ) \
StringRef get_tootip_bottom() const noexcept override \
{ \
  return \
  "Switch between '" front_name "' (front) and '" back_name "' (back) sliders.\n" \
  GERNERAL_SWITCH_BUTTON_INFORMATION; \
}

#define BOTTOM_BUTTON_DIALS( front_name, back_name ) \
StringRef get_tootip_bottom() const noexcept override \
{ \
  return \
  "Switch between '" front_name "' (front) and '" back_name "' (back) dials.\n" \
  GERNERAL_SWITCH_BUTTON_INFORMATION; \
}








#define SHORTCUT_SHIFT "\nNote: .\nNote: Buttons below sliders/dials are always switchs from front slider to back slider and vice versa."

#define SHORTCUT_CTRL  "\nNote: Global shortcut: CTRL."

#define EXAMPLE_START  "\nExample:\n----------\n"




#define NO_MOD_EFFECT "(Has no effect if X-MOD is turned off)\n"

#define VALUES_EDITABLE_ON_TOP "\nNote: to edit the value via keyboard click on the top label."





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
    const String bottom_text;

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
        return bottom_text;
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the wave form of this oscillator.\n"
        "(Sine (LEFT), Square, Saw, Noise (RIGHT))"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "OSC 1: Turns oscillator modulations on and off (OSC PULSE, OSC SWITCH and FM SWING).\n"
        "(See FM section: O-PULSE, O-SWITCH and FM SWING dials)\n"
        "\n"
        "OSC 2 & 3: Turns sync to OSC 1 on or off.\n"
        "\n"
        "If SYNC is enabled the oscillator waits for the next cycle of OSC 1 to start its next own cycle.\n"
        "If SYNC is disabled this oscillator is absolutely independent."
    )
    BOTTOM_BUTTON_DIALS
    (
        "WAVE FORM",
        "FM MASS"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the mass of the FM oscillator which will be added to this oscillator.\n"
        "\n"
        "OSC 1: will be also added if O-MOD is turned off."
    )

public:
    WAVESlConfig( int id_ )
        :
        wave( &(GET_DATA(osc_datas[id_]).wave) ),
        fm_amount( &(GET_DATA(osc_datas[id_]).fm_amount) ),
        top_parameter( id_ == 0 ? &(GET_DATA(osc_datas[id_]).o_mod) : &(GET_DATA(osc_datas[id_]).sync) ),
        top_text( id_ == 0 ? "O-MOD" : "SYNC" ),
        bottom_text( "WAV-" + String(id_+1) )
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
        if( synth_data->animate_envs )
        {
            if( bool(is_lfo_modulated->get_value()) )
            {
                value = get_last_modulation_amount(octave);
            }
            else
                value = NO_TOP_BUTTON_AMP;
        }
        else
        {
            value = NO_TOP_BUTTON_AMP;
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
        return "MOD %";
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
        {
            return String( round0(octave->get_modulation_amount() * 100) );
        }
        else
        {
            if( value == 0 )
                return String(0);
            else if( value < 10 and value > -10 )
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
    TOP_SLIDER_DESCIPTION
    (
        "Detune this oscillator in semitones.\n"
        "\n"
        "OSC 1: Affected: FM TUNE.\n"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns tune modulation by LFO on and off.\n"
        "(OSC 1 will be modulated by LFO 1 (See: FILTER 1), OSC 2 by LFO 2... .)"
    )
    BOTTOM_BUTTON_DIALS
    (
        "OSC TUNE",
        "LFO TUNE-MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the power of the tune modulation by the LFO amplitude relative to the defined tune (front).\n"
        "\n"
        "Values greater than 0 increase the current tune by the current LFO amplitude. Values less than 0 decrease the tune by the current LFO amplitude.\n"
        "\n"
        "\n"
        "EXAMPLE:\n"
        "--------\n"
        "Defined tune is 12 (front) (maximum 24).\n"
        "Defined tune-modulation power is plus 50% (this).\n"
        "Current LFO amplitude is 100% of the maximum.\n"
        "The outcome tune will be 18.\n"
        "\n"
        "FORMULA:\n"
        "--------\n"
        "OUTCOME = TUNE + (MAX-TUNE) * MOD-POWER * CURRENT AMPLITUDE\n"
        "18      = 12   + (24-12)    * 50%       * 100%"
    )

public:
    OSCSlConfig( int id_ )
        :
        octave( &(GET_DATA(osc_datas[id_]).octave) ),
        is_lfo_modulated( &(GET_DATA(osc_datas[id_]).is_lfo_modulated ) ),
        top_text( String("L-MOD") ),
        bottom_text( String("TUNE-") + String(id_+1) ),

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
        return "F-TUNE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "F-SWING";
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
        if( fm_multi->midi_control->get_ctrl_mode() )
        {
            const float swing = fm_swing->get_value();
            if( swing == 0 )
            {
                return "OFF";
            }
            else
            {
                return String( swing );
            }
        }
        else
        {
            return String( fm_multi->get_value() );
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( fm_multi->midi_control->get_ctrl_mode() )
            return "th";
        else
            return "";
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION
    (
        "Detune the FM oscillator relative, upwards to the tune of OSC 1.\n"
        "(Zero = same as TUNE of OSC 1)\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if the FM MASS is greater than 0"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns sync to OSC 1 on or off.\n"
        "\n"
        "If SYNC is enabled the FM oscillator waits for the next cycle of OSC 1 to start its next own cycles (as many as fit into one OSC 1 cycle).\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if its FM MASS is greater than 0\n"
        "\n"
        "See: FM SHOT (next button on the right)"
    )
    BOTTOM_BUTTON_DIALS
    (
        "FM TUNE",
        "FM SWING"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the FM SWING amount.\n"
        "\n"
        "Values greater than 0 switch the FM oscillator phase smoothly (greater values = faster).\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if its FM MASS is greater than 0"
    )
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
        return "O-PLUSE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "O-SWCH";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define pulses for each oscillator.\n"
        "\n"
        "Affected: OSC 1 (if O-MOD is turned on), OSC 2 & 3\n"
        "\n"
        LINE_DOTTED
        "\n"
        "Values greater than 0 insert cycle breaks with the duration of one cycle of the affected oscillator.\n"
        "\n"
        "EXAMPLES:\n"
        "---------\n"
        "PULSE = 0 =   ‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| \n"
        "               |_| |_| |_| |_| |_| |_| |_| |_| |_| |_\n"
        "\n"
        "PULSE = 1 =   ‾|  ___|‾|  ___|‾|  ___|‾|  ___|‾|  ___\n"
        "               |_|     |_|     |_|     |_|     |_|   \n"
        "\n"
        "PULSE = 2 =   ‾|  _______|‾|  _______|‾|  _______|‾| \n"
        "               |_|         |_|         |_|         |_\n"
        "\n"
        LINE_DOTTED
        "\n"
        "Values less than 0 change the cycle duration. One cycle becomes longer, the next one shorter.\n"
        "\n"
        "EXAMPLES:\n"
        "---------\n"
        "PULSE =   0 = ‾‾‾‾‾‾|      |‾‾‾‾‾‾|      |‾‾‾‾‾‾|    \n"
        "                    |______|      |______|      |____\n"
        "\n"
        "PULSE =  -1 = ‾‾‾‾‾‾‾|       |‾‾‾‾‾|     |‾‾‾‾‾‾‾|   \n"
        "                     |_______|     |_____|       |___\n"
        "\n"
        "PULSE = -12 = ‾‾‾‾‾‾‾‾‾‾‾|           |‾| |‾‾‾‾‾‾‾‾‾‾‾\n"
        "                         |___________| |_|           "
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns FM SHOT for each oscillator on and off.\n"
        "\n"
        "If FM SHOT is enabled only one cycle of the FM oscillator will be added to the modulated oscillator and then the FM OSC waits for a new OSC 1 cycle to start its next own cycle."
    )
    BOTTOM_BUTTON_DIALS
    (
        "OSC PULSE",
        "OSC SWITCH"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define phase switches for each oscillator.\n"
        "\n"
        "Affected: OSC 1 (if O-MOD is turned on), OSC 2 & 3\n"
        "\n"
        LINE_DOTTED
        "\n"
        "Values greater than 0 switches the phase after the defined number of cycles.\n"
        "\n"
        "EXAMPLES:\n"
        "---------\n"
        "SWITCH = 0 =  ‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| |‾| \n"
        "               |_| |_| |_| |_| |_| |_| |_| |_| |_| |_\n"
        "\n"
        "SWITCH = 1 =  ‾|   |‾|‾|   |‾|‾|   |‾|‾|   |‾|‾|   |‾\n"
        "               |_|_|   |_|_|   |_|_|   |_|_|   |_|_| \n"
        "\n"
        "\n"
        "SWITCH = 3 =  ‾|   |‾| |‾| |‾|‾| |‾| |‾|   |‾| |‾| |‾\n"
        "               |_|_| |_| |_|   |_| |_| |_|_| |_| |_| "
    )

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
        return "ENV";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = NO_TOP_BUTTON_AMP;
        const bool is_on = not bool(input_hold->get_value());
        if( synth_data->animate_envs )
        {
            if( is_on )
            {
                value = mono_ParameterOwnerStore::getInstance()->voice->get_flt_input_env_amp(filter_id,input_id);
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
        }
        else
        {
            if( is_on )
            {
                value = TOP_BUTTON_IS_ON;
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
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
        return "MO-ENV";
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
            return String( round001(state->get_value()+1) );
        else
            return String( round01(input_sustain->get_value()*100)  );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( state->midi_control->get_ctrl_mode() )
            return "MO";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the OSC input gain into this filter.\n"
        "(Will be the sustain level if ENV is turned on)\n"
        "\n"
        "FILTER 2 & 3:\n"
        "On the left side of the dial (minus) the original OSC signal will be used as input.\n"
        "On the right side (plus) the filtered OSC signal through the filter before will be used as input."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns morphable ENV modulations for the OSC input gain on or off.\n"
        "\n"
        "If ENV is enabled the input dial defines the sustain level for the morphable envelope (MORPH-ENV dial (back dial)).\n"
        "If ENV is disabled the input dial defines input gain and the morphable envelope will be ignored."
    )
    BOTTOM_BUTTON_DIALS
    (
        "INPUT/SUSTAIN",
        "MORPH ENV"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define an envelope to control the OSC input gain into this FILTER.\n"
        "(Has no effect if ENV is turned off)\n"
        "\n"
        "The dial defines an envelope out of the 4 morphable envelope presets (which you can define with the ENV preset editor (ENV button at the bottom of the main user interface)).\n"
        "The input dial (front dial) will be the sustain level for this envelope."
    )

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
        return "X-MOD";
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
        return "MOD %";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the filter distortion power.\n"
        "(Also processed if 'PASS' as filter type is selected)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns filter distortion modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "DISTORTION",
        "DISTORTION MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the filter distortion modulation power relative to the defined distortion (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

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
    TOP_SLIDER_DESCIPTION
    (
        is_main_adsr

        ?

        "Define the main envelope attack time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        :

        "Define the filter envelope attack time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP
    )
    BOTTOM_BUTTON_SLIDERS
    (
        "ATTACK",
        "MAX ATTACK"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the the maximum, adjustable attack time for the front slider."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        is_main_adsr

        ?

        "Define the main envelope decay time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        :

        "Define the filter envelope decay time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP
    )
    BOTTOM_BUTTON_SLIDERS
    (
        "DECAY",
        "MAX DECAY"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the the maximum, adjustable decay time for the front slider."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        is_main_adsr

        ?

        "Define the main envelope sustain level.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        :

        "Define the filter envelope sustain level.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP
    )

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
    TOP_SLIDER_DESCIPTION
    (
        is_main_adsr

        ?

        "Define the main envelope sustain time in ms.\n"
        "\n"
        "If the slider is set to max, the sustain time is unlimited (until note off).\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        :

        "Define the filter envelope sustain time in ms.\n"
        "\n"
        "If the slider is set to max, the sustain time is unlimited (until note off).\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP
    )

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
    TOP_SLIDER_DESCIPTION
    (
        is_main_adsr

        ?

        "Define the main envelope release time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        :

        "Define the filter envelope release time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP
    )
    BOTTOM_BUTTON_SLIDERS
    (
        "RELEASE",
        "MAX RELEASE"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the the maximum, adjustable release time for the front slider."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the mix of filter envelope & LFO which will be used as modulator (X-MOD)).\n"
        "\n"
        "If the slider is on the left side only the filter envelope amplitude will be used as modulation signal.\n"
        "On the right side: only the LFO amplitude will be uesd as modulation signal.\n"
        "In the middle: 50% of the envelope and 50% of the LFO will be used as modulator.\n"
        "\n"
        "Possible targets: CUTOFF, RESONANCE, GAIN, DESTROY, FILTER VOLUME"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the LFO speed.\n"
        "\n"
        "If the LFO speed is slow (note durations) the LFO is always synced to the current speed (BPM).\n"
        "If the LFO speed is fast (note values) the LFO is not synced to anything.\n"
        "\n"
        "Possible targets: OSC, MOD-MIX -> CUTOFF, RESONANCE, GAIN, DESTROY, FILTER VOLUME"
    )

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
        return "X-MOD";
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
        return "MOD %";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the filter cutoff frequency.\n"
        "(Has no effect if 'PASS' as filter type is selected)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns cutoff modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "CUTOFF",
        "CUTOFF MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the cutoff modulation power relative to the defined cutoff (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

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
        return "MOD %";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the filter resonance (boost frequencys at the edge of the cutoff).\n"
        "(Has no effect if 'PASS' as filter type is selected)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns resonance modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "RESONANCE",
        "RESONANCE MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the resonance modulation power relative to the defined resonance (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

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
        return "X-MOD";
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
        return "MOD %";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the filter gain (boost the frequencys below the cutoff (LOW PASS), above the cutoff (HIGH PASS), close to the cutoff (BAND PASS) ).\n"
        "(Has no effect if 'PASS' as filter type is selected)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns gain modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "GAIN",
        "GAIN MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the gain modulation power relative to the defined gain (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

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
        return "X-MOD";
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
        return "MOD %";
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
    TOP_SLIDER_DESCIPTION
    (
        "Define the filter output volume.\n"
        "\n"
        "Output target: EQ bank\n"
        "Affected: BOOST of the next filter (except FILTER 3, there is no FILTER 4)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns filter volume modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "FILTER VOLUME",
        "FILTER VOLUME MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the filter volume modulation power relative to the defined filter volume (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define Moniques internal speed in BPM.\n"
        "(PLUGIN: has no effect if SYNC is turned on)\n"
        "(STANALONE: has no effect if SYNC is turned on and a MIDI clock is continuously received)\n"
        "\n"
        "Affected: ARPEGGIATOR, LFO's (if slow) (OSC's and FM if L-MOD is turned on)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns SPEED SYNC on or off.\n"
        "\n"
        "If disabled you can define your own speed ignoring the host speed (plugin) or the MIDI-Clock (standalone)."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the speed multiplier/divisor.\n"
        "\n"
        "Affected: ARPEGGIATOR, LFO's (if slow) (OSC's and FM if L-MOD is turned on)\n"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the octave offset related to the current root note (e.g. pressed on the keyboard).\n"
        "\n"
        "Affected: OSC's, FM"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the peak boost power of the filter output.\n"
        "\n"
        "FILTER 2 & 3: boosts or limits the filter ouput by peaks in the output of the filter before."
    )
    BOTTOM_BUTTON_DIALS
    (
        "OUTPUT PEAK BOOST",
        "OUTPUT SOFT CLIPPING"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the soft clipping amount of the filter output.\n"
        "\n"
        "Values greater than 0 clipps the output softly and minimizes very large peaks much more than smaller ones.\n"
        "Note: that samples after the soft clipper will be clipped hard if they are still larger as the possible output volume (this will have a distortion effect (may be wanted, may not))."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the shape amount of the FX input (after EQ bank).\n"
        "\n"
        "Shaping makes some details in the sound more 'visible'."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the room of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the width of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the wet/dry mix of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the delay time of the delay effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the mix of FX output and FX input.\n"
        "\n"
        "Values greater than 0 add more and more of the FX section output to the not FX processed signal (EQ output).\n"
        "(If the value is zero the FX section is bypassed)\n"
        "\n"
        "Affected:     REVERB, DELAY, CHORUS\n"
        "NOT Affected: SHAPE"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the final output volume."
    )
    BOTTOM_BUTTON_DIALS
    (
        "MASTER VOLUME",
        "SOFT CLIPPING"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the SOFT CLIPPING amount of the final output.\n"
        "\n"
        "Values greater than 0 clipps the output softly and minimizes very large peaks much more than smaller ones.\n"
        "Note: that samples after the soft clipper will be clipped hard if they are still larger as the possible output volume (this will have a distortion effect (may be wanted, may not))."
    )

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
        return "ENV";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = NO_TOP_BUTTON_AMP;
        const bool is_on = not bool(hold_modulation->get_value());
        if( synth_data->animate_envs )
        {
            if( is_on )
            {
                value = mono_ParameterOwnerStore::getInstance()->voice->get_chorus_modulation_env_amp();
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
        }
        else
        {
            if( is_on )
            {
                value = TOP_BUTTON_IS_ON;
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
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
        return "MO-ENV";
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
            return String( round001(state->get_value()+1) );
        else
            return String( round01(chorus_data->modulation*100)  );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( modulation->midi_control->get_ctrl_mode() )
            return "MO";
        else
            return "";
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION
    (
        "Define the chorus amount.\n"
        "(Will be the sustain level if ENV is turned on)\n"
        "(Has no effect if FX MIX is set to zero)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns morphable ENV modulations for the chorus amount on or off.\n"
        "\n"
        "If ENV is enabled the chorus dial defines the sustain level for the morphable envelope (MORPH-ENV dial (back dial)).\n"
        "If ENV is disabled the chorus dial defines just the chorus amount and the morphable envelope will be ignored."
    )
    BOTTOM_BUTTON_DIALS
    (
        "CHORUS AMOUNT",
        "MORPH ENV"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define an envelope to control the chorus amount.\n"
        "(Has no effect if ENV is turned off)\n"
        "\n"
        "The dial defines an envelope out of the 4 morphable envelope presets (which you can define with the ENV preset editor (ENV button at the bottom of the main user interface)).\n"
        "The chorus dial (front dial) will be the sustain level for this envelope."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the note glide time.\n"
        "\n"
        "Values greater than 0 will glide more and more smoothly from the last note tune to the current one."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Binds the steps of the arpeggiator.\n"
        "(Has no effect if the ARP is turned off)\n"
        "\n"
        "If enabled all steps without an empty step between will be handled as one step (no envelope retrigger)."
    )
    BOTTOM_BUTTON_DIALS
    (
        "NOTE GLIDE",
        "VELOCITY GLIDE"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the velocity glide time.\n"
        "\n"
        "Values greater than 0 will glide more and more smoothly from the last velocity to the current one.\n"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the shuffle of the arpeggiator steps in note durations.\n"
        "(Has no effect if ARP is turned off)\n"
        "\n"
        "Values greater than 0 decrease the duration of each second step and increase the step before."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns the arpeggiator on or off.\n"
        "\n"
        "If enabled notes will automatically triggered at the position of the enabled arpeggiator steps."
    )

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
        return "ENV";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = NO_TOP_BUTTON_AMP;
        const bool is_on = not bool(hold->get_value());
        if( synth_data->animate_envs )
        {
            if( is_on )
            {
                value = mono_ParameterOwnerStore::getInstance()->voice->get_band_env_amp(id);
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
        }
        else
        {
            if( is_on )
            {
                value = TOP_BUTTON_IS_ON;
            }
            else
            {
                value = TOP_BUTTON_IS_OFF;
            }
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
        return "MO-ENV";
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
        if( velocity->midi_control->get_ctrl_mode() )
            return String( round001(state->get_value()+1) );
        else
            return String( round01(velocity->get_value()*100)  );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( velocity->midi_control->get_ctrl_mode() )
            return "MO";
        else
            return "";
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION
    (
        "Define the band boost amount for this frequency (bottom button caption).\n"
        "(Will be the sustain level if ENV is turned on)\n"
        "\n"
        "Values greater than 0 will boost this band and values less than 0 will reduce it."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns morphable ENV modulations for the band boost on or off.\n"
        "\n"
        "If ENV is enabled the band boost dial defines the sustain level for the morphable envelope (MORPH-ENV dial (back dial)).\n"
        "If ENV is disabled the band boost dial defines just the band boost amount and the morphable envelope will be ignored."
    )
    BOTTOM_BUTTON_DIALS
    (
        "BAND BOOST",
        "MORPH ENV"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define an envelope to control the band boost amount.\n"
        "(Has no effect if ENV is turned off)\n"
        "\n"
        "The dial defines an envelope out of the 4 morphable envelope presets (which you can define with the ENV preset editor (ENV button at the bottom of the main user interface)).\n"
        "The band boost dial (front dial) will be the sustain level for this envelope."
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Define the note offset for this step in semitones.\n"
        "(Has no effect if the arpeggiator (ARP) is turned off)\n"
        "\n"
        "See: NOTE-G (NOTE GLIDE) (left)"
    )
    BOTTOM_BUTTON_DIALS
    (
        "NOTE OFFSET",
        "VELOCITY"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the velocity amount for this step.\n"
        "(Has no effect if the arpeggiator (ARP) is turned off)\n"
        "\n"
        "See: VELO-G (VELOCITY GLIDE) (left)\n"
    )

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
    TOP_SLIDER_DESCIPTION
    (
        "Morph between two programs, one the LEFT side and one on the RIGHT side.\n"
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
        "With the morph editor (EDIT) you can load each of your programs to the left or right side of each morph group."
    )

public:
    MorphSLConfig(int id_)
        :
        morhp_state( &(GET_DATA(synth_data).morhp_states[id_]) ),
        bottom_text( String( "TOGGL" ) + String(id_+1) )
    {}

    JUCE_LEAK_DETECTOR (MorphSLConfig)
};

#endif  // Monique_Ui_MainwindowCONFIG_H_INCLUDED




