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
                                     "OSC's                -> FILTER 1 INPUT->FILTER PROCESS->FILTER 1 OUT->MIX\n" \
                                     "OSC's or FILTER 1 OUT-> FILTER 2 INPUT-> ...........................->MIX\n" \
                                     "OSC's or FILTER 2 OUT-> FILTER 3 INPUT-> ...........................->MIX\n" \
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
#define TOP_SLIDER_DESCIPTION_2_CASE( text_1_1, text_2_1, boolean ) \
StringRef get_tootip_front() const noexcept override \
{ \
  return \
  boolean \
  ? \
  text_1_1 "\n" GERNERAL_SLIDER_INFORMATION GERNERAL_SIGNAL_FLOW \
  : \
  text_2_1 "\n" GERNERAL_SLIDER_INFORMATION GERNERAL_SIGNAL_FLOW; \
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

#define BACK_SLIDER_DESCRIPTION_2_CASE( text_1_1, text_2_1, boolean ) \
StringRef get_tootip_back() const noexcept override \
{ \
  return \
  boolean \
  ? \
  text_1_1 "\n" GERNERAL_SLIDER_INFORMATION GERNERAL_SIGNAL_FLOW \
  : \
  text_2_1 "\n" GERNERAL_SLIDER_INFORMATION GERNERAL_SIGNAL_FLOW; \
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
#define TOP_BUTTON_DESCRIPTION_2_CASE( text_1_1, text_2_1, boolean ) \
StringRef get_tootip_top() const noexcept override \
{ \
  return \
  boolean \
  ? \
  text_1_1 "\n" GERNERAL_TOP_BUTTON_INFORMATION \
  : \
  text_2_1 "\n" GERNERAL_TOP_BUTTON_INFORMATION; \
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
                                     "Buttons below sliders/dials switch always from front slider to back slider and vice versa."

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
  "Switch between '" front_name "' (front) and '" back_name "' (back) slider.\n" \
  GERNERAL_SWITCH_BUTTON_INFORMATION; \
}
#define BOTTOM_BUTTON_SLIDERS_2_CASE( front_name_1_1, front_name_1_2, back_name_2_1, back_name_2_2, boolean ) \
StringRef get_tootip_bottom() const noexcept override \
{ \
  return \
  boolean \
  ? \
  "Switch between '" front_name_1_1 "' (front) and '" back_name_2_1 "' (back) slider.\n" GERNERAL_SWITCH_BUTTON_INFORMATION \
  : \
  "Switch between '" front_name_1_2 "' (front) and '" back_name_2_2 "' (back) slider.\n" GERNERAL_SWITCH_BUTTON_INFORMATION; \
}

#define BOTTOM_BUTTON_DIALS( front_name, back_name ) \
StringRef get_tootip_bottom() const noexcept override \
{ \
  return \
  "Switch between '" front_name "' (front) and '" back_name "' (back) dial.\n" \
  GERNERAL_SWITCH_BUTTON_INFORMATION; \
}
#define BOTTOM_BUTTON_DIALS_2_CASE( front_name_1_1, front_name_1_2, back_name_2_1, back_name_2_2, boolean ) \
StringRef get_tootip_bottom() const noexcept override \
{ \
  return \
  boolean \
  ? \
  "Switch between '" front_name_1_1 "' (front) and '" back_name_2_1 "' (back) dial.\n" GERNERAL_SWITCH_BUTTON_INFORMATION \
  : \
  "Switch between '" front_name_1_2 "' (front) and '" back_name_2_2 "' (back) dial.\n" GERNERAL_SWITCH_BUTTON_INFORMATION; \
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
    const int id;

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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::OSC_THEME;
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
    virtual bool get_override_front_screw_value() const noexcept
    {
        return true;
    }

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
        if( fm_amount->midi_control->get_ctrl_mode() )
            return SHOW_OWN_VALUE;
        else
            return SHOW_OWN_VALUE_ALWAYS;
    }
    String get_center_value() const noexcept override
    {
        if( fm_amount->midi_control->get_ctrl_mode() )
            return String( auto_round(fm_amount->get_value()*100) );
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
    TOP_BUTTON_DESCRIPTION_2_CASE
    (
        "Turns Key Sync for all oscillators on or off."
        "\n"
        "If Key Sync is enabled all oscillators will be hard reseted to a new cycle at each note on.\n"
        "\n",

        "OSC 2 & 3: Turns sync to OSC 1 on or off.\n"
        "\n"
        "If SYNC is enabled the oscillator waits for the next cycle of OSC 1 to start its next own cycle.\n"
        "If SYNC is disabled this oscillator is absolutely independent.\n"
        "\n"
        "Note: the oscillator will be auto-synced to OSC 1 at a defined tune of exactly 0 and +/-24.",

        id == 0
    )
    BOTTOM_BUTTON_DIALS
    (
        "WAVE FORM",
        "FM MASS"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the mass of the FM oscillator which will be added to this oscillator."
    )

public:
    WAVESlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        ModulationSliderConfigBase(),
        id(id_),
        wave( &synth_data_->osc_datas[id_]->wave ),
        fm_amount( &synth_data_->osc_datas[id_]->fm_amount ),
        top_parameter( &synth_data_->osc_datas[id_]->sync ),
        top_text( id_ == 0 ? "K-SNC" : "SYNC" ),
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
    const int id;

    ModulatedParameter*const front_param;
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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::OSC_THEME;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return front_param;
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
    virtual bool get_override_front_screw_value() const noexcept
    {
        return id > 0;
    }

    //==============================================================================
    // BACK SLIDER
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return MODULATION_SLIDER;
    }
    Parameter* get_back_parameter_base() const noexcept override
    {
        return front_param;
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
                value = get_last_modulation_amount(front_param);
            }
            else
            {
                value = NO_TOP_BUTTON_AMP;
            }
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
        const float value = front_param->get_value();
        if( front_param->midi_control->get_ctrl_mode() )
        {
            return String( auto_round(front_param->get_modulation_amount() * 100) );
        }
        else
        {
            return String(auto_round(value));
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( front_param->midi_control->get_ctrl_mode() )
            return "%";
        else
            return "#";
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Moves the phase of OSC 1.",

        "Detune this oscillator in semitones to OSC 1.\n"
        "(OSC 1 runs with the frequency of the last triggered note.)",

        id == 0
    )
    TOP_BUTTON_DESCRIPTION_2_CASE
    (
        "Turns phase modulation by LFO 1 (See: FILTER 1) on or off.",

        "Turns tune modulation by LFO on or off.\n"
        "(OSC 2 will be modulated by LFO 2 (See: FILTER 2), OSC 3 by LFO 3... .)",

        id == 0
    )
    BOTTOM_BUTTON_DIALS_2_CASE
    (
        "OSC 1 PHASE", "LFO PHASE-MODULATION POWER",

        "OSC TUNE", "LFO TUNE-MODULATION POWER",

        id == 0
    )
    BACK_SLIDER_DESCRIPTION_2_CASE
    (
        "Define the power of the phase modulation by the LFO 1 amplitude relative to the defined phase (front).\n"
        "\n"
        "Values greater than 0 increase the current phase by the current LFO 1 amplitude. Values less than 0 decrease the phase by the current LFO 1 amplitude.\n"
        "\n"
        "\n"
        "EXAMPLE:\n"
        "--------\n"
        "Defined phase is 0.5 (front) (maximum 1).\n"
        "Defined phase-modulation power is plus 50% (this).\n"
        "Current LFO amplitude is 100% of the maximum.\n"
        "The outcome phase will be 0.75.\n"
        "\n"
        "FORMULA:\n"
        "--------\n"
        "OUTCOME = PHASE + (MAX-PHASE) * MOD-POWER * CURRENT AMPLITUDE\n"
        "0.75      = 0.5 + (1-0.5)     * 50%       * 100%"

        ,

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

        ,

        id == 0
    )

public:
    OSCSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        ModulationSliderConfigBase(),
        id(id_),
        front_param( id == 0 ? &synth_data_->fm_osc_data->master_shift : &synth_data_->osc_datas[id_]->tune ),
        is_lfo_modulated( &synth_data_->osc_datas[id_]->is_lfo_modulated ),
        top_text( String("L-MOD") ),
        bottom_text( id_ == 0 ? String("PHASE") : String("TUNE") ),

        synth_data( synth_data_ )
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
    Parameter*const fm_freq;
    BoolParameter*const sync;
    Parameter*const fm_shape;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return false;
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FM_THEME;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return fm_freq;
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
        return fm_shape;
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
        return "TUNE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "SHAPE";
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
        if( not fm_freq->midi_control->get_ctrl_mode() )
        {
            return String( auto_round(fm_freq->get_value()*6 +2.01) );
        }
        else
        {
            return String( auto_round(fm_shape->get_value()*100) );
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( not fm_freq->midi_control->get_ctrl_mode() )
        {
            return "x";
        }
        else
        {
            return "%";
        }
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION
    (
        "Detune the FM oscillator relative, upwards to the tune of OSC 1.\n"
        "(1 = same frequency as OSC 1)\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if the FM MASS is greater than 0."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns sync to OSC 1 on or off.\n"
        "\n"
        "If SYNC is enabled the FM oscillator waits for the next cycle of OSC 1 to start its next own cycles (as many as fit into one cycle of OSC 1).\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if its FM MASS is greater than 0."
    )
    BOTTOM_BUTTON_DIALS
    (
        "FM-TUNE", "FM-SHAPE"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the shape of the FM oscillator\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if its FM MASS is greater than 0."
    )

public:
    FMFreqSlConfig(MoniqueSynthData*const synth_data_)
        :
        ModulationSliderConfigBase(),
        fm_freq( &synth_data_->fm_osc_data->fm_freq ),
        sync( &synth_data_->fm_osc_data->sync ),
        fm_shape( &synth_data_->fm_osc_data->fm_shape )
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
    Parameter*const fm_swing;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FM_THEME;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return fm_swing;
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
        return fm_phaser;
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
        return shot;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "- SHOT";
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
        return "SWING";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "ROTATE";
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
        return String( auto_round(fm_swing->get_value()*5) );
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "hz";
    }

    //==============================================================================
    // TOOLTIP
    TOP_BUTTON_DESCRIPTION
    (
        "Turns FM SHOT on or off.\n"
        "\n"
        "If FM SHOT is enabled only one cycle of the FM oscillator will be added to the modulated oscillator and then the FM OSC waits for a new OSC 1 cycle to start its next own cycle."
    )
    TOP_SLIDER_DESCIPTION
    (
        "Define the FM SWING amount.\n"
        "\n"
        "Values greater than 0 switch the FM oscillator phase smoothly (greater values = faster).\n"
        "\n"
        "Affected: OSC 1, 2 & 3 if its FM MASS is greater than 0."
    )
public:
    FMAmountSlConfig(MoniqueSynthData*const synth_data_)
        :
        ModulationSliderConfigBase(),
        fm_swing( &synth_data_->fm_osc_data->fm_swing )
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
    bool action_keep_env_pop_open_for( const ENVData*const env_ ) const noexcept override
    {
        bool success = false;
        if
        (
            env_ == synth_data->filter_datas[0]->input_envs[0] or env_ == synth_data->filter_datas[0]->input_envs[1] or env_ == synth_data->filter_datas[0]->input_envs[2]
            or env_ == synth_data->filter_datas[1]->input_envs[0] or env_ == synth_data->filter_datas[1]->input_envs[1] or env_ == synth_data->filter_datas[1]->input_envs[2]
            or env_ == synth_data->filter_datas[2]->input_envs[0] or env_ == synth_data->filter_datas[2]->input_envs[1] or env_ == synth_data->filter_datas[2]->input_envs[2]
        )
        {
            success = true;
        }

        return success;
    }
    bool use_click_through_hack() const noexcept override
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
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return nullptr;
    }
    */

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
                value = get_last_modulation_amount(input_sustain);
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
    /*
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MO-ENV";
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
        return String( auto_round(input_sustain->get_value()*100)  );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( input_sustain->get_value() >= 0 )
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
        "(Will be a fixed gain level if ENV is turned off)\n"
        "\n"
        "FILTER 2 & 3:\n"
        "On the left side of the dial (minus) the original OSC signal will be used as input. "
        "On the right side (plus) the filtered OSC signal through the filter before will be used as input.\n"
        "\n"
        "A red highlighted LED on top marks an inactive input. This can happen if you try to use filtered signal as input (right dial positions) and the input of the same branch of the filter before is 0."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns modulation by an envelope for this input on or off.\n"
        "\n"
        "If ENV is disabled the input dial defines a fixed gain level and the envelope will be ignored."
    )

public:
    InputSlConfig( MoniqueSynthData*const synth_data_, int filter_id_, int input_id_ )
        :
        filter_id( filter_id_ ),
        input_id( input_id_ ),
        input_sustain( &synth_data_->filter_datas[filter_id_]->input_sustains[input_id_] ),
        input_hold( &synth_data_->filter_datas[filter_id_]->input_holds[input_id_] ),

        bottom_text( String("OSC ") + String(input_id_+1) ),
        input_text( String("O") + String(input_id_+1) ),

        synth_data( synth_data_ )
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
        return "DIST";
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
    GForceSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        distortion( &synth_data_->filter_datas[id_]->distortion ),
        modulate_distortion( &synth_data_->filter_datas[id_]->modulate_distortion )
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

    const bool is_main_adsr;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
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
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_attack_time;
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
        return "ATT";
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
        return String( auto_round( MIN_ENV_TIMES + attack->get_value() * MAX_ENV_TIMES ) );
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_-MIN_ENV_TIMES)/max_attack_time->get_value();
    }
    */

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Define the amp envelope attack time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        "Define the filter envelope attack time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, PAN, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        is_main_adsr
    )

public:
    FAttackSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        attack( &synth_data_->filter_datas[id_]->env_data->attack ),
        is_main_adsr(false)
    {}
    FAttackSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        attack( &synth_data_->env_data->attack ),
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

    const bool is_main_adsr;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
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
        return "DEC";
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
        float value = decay->get_value();
        if( value > 0 )
        {
            return String( auto_round( value * (MAX_ENV_TIMES+MIN_ENV_TIMES) ));
        }
        else
        {
            return "OFF";
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( decay->get_value() > 0 )
        {
            return "ms";
        }
        else
        {
            return "";
        }
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_)/max_decay_time->get_value();
    }
    */

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Define the amp envelope decay time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        "Define the filter envelope decay time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, PAN, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        is_main_adsr
    )

public:
    FDecaySlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        decay( &synth_data_->filter_datas[id_]->env_data->decay ),
        is_main_adsr(false)
    {}
    FDecaySlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        decay( &synth_data_->env_data->decay ),
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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
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
    int get_override_front_min_value() const noexcept override
    {
        return 0;
    }
    /*
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
        return "SUS";
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
        return String( auto_round( sustain->get_value() * 100 ) );
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "%";
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return entered_value_/100;
    }
    */

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Define the main envelope sustain level.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        "Define the filter envelope sustain level.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, PAN, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        is_main_adsr
    )

public:
    FSustainSlConfig( MoniqueSynthData*const synth_data_, int id_ ) : sustain( &synth_data_->filter_datas[id_]->env_data->sustain ), is_main_adsr(false) {}
    FSustainSlConfig( MoniqueSynthData*const synth_data_ ) : ModulationSliderConfigBase(), sustain( &synth_data_->env_data->sustain ), is_main_adsr(true) {}

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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
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
        return "HOLD";
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
        float value = sustain_time->get_value() * 5000;
        if( value < 5000 )
        {
            return String(auto_round(value));
        }
        else
        {
            return String("UNLTD");
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( sustain_time->get_value() == 1 )
            return "";
        else
            return "ms";
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return entered_value_/10000;
    }
    */

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Define the main envelope sustain time in ms.\n"
        "\n"
        "If the slider is set to max, the sustain time is unlimited (until note off).\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        "Define the filter envelope sustain time in ms.\n"
        "\n"
        "If the slider is set to max, the sustain time is unlimited (until note off).\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, PAN, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        is_main_adsr
    )

public:
    FSustainTimeSlConfig( MoniqueSynthData*const synth_data_, int id_ ) : sustain_time( &synth_data_->filter_datas[id_]->env_data->sustain_time ), is_main_adsr(false) {}
    FSustainTimeSlConfig( MoniqueSynthData*const synth_data_ ) : ModulationSliderConfigBase(), sustain_time( &synth_data_->env_data->sustain_time ), is_main_adsr(true) {}

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

    const bool is_main_adsr;

    //==============================================================================
    // BASIC SLIDER TYPE
    bool get_is_linear() const noexcept override
    {
        return true;
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
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
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return max_release_time;
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
        return "REL";
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
        return String( auto_round( MIN_ENV_TIMES + release->get_value() * MAX_ENV_TIMES ) );
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return (entered_value_-MIN_ENV_TIMES)/max_release_time->get_value();
    }
    */

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        "Define the main envelope release time in ms.\n"
        "\n"
        "Controls the output gain (processed after EQ bank and before FX).\n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        "Define the filter envelope release time in ms.\n"
        "\n"
        "Possible targets: MOD-MIX -> CUTOFF, RESONANCE, GAIN, DISTORTION, PAN, FILTER VOLUME \n"
        "\n"
        VALUES_EDITABLE_ON_TOP

        ,

        is_main_adsr
    )

public:
    FReleaseSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        release( &synth_data_->filter_datas[id_]->env_data->release ),
        is_main_adsr(false)
    {}

    FReleaseSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        release( &synth_data_->env_data->release ),
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
class FShapeSlConfig : public ModulationSliderConfigBase
{
    Parameter*const shape;

    const bool is_main_adsr;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
        return true;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        if( is_main_adsr )
        {
            return COLOUR_THEMES::FX_THEME;
        }
        else
        {
            return COLOUR_THEMES::FILTER_THEME;
        }
    }

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
        return "SHAPE";
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
        return String(auto_round( shape->get_value() * 100 ));
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "/\\";
    }
    /*
    float get_label_edit_value( float entered_value_ ) const noexcept override
    {
        return entered_value_/10000;
    }
    */

    //==============================================================================
    // TOOLTIP
    // TODO
    TOP_SLIDER_DESCIPTION_2_CASE
    (
        ""

        ,

        ""

        ,

        is_main_adsr
    )

public:
    FShapeSlConfig( MoniqueSynthData*const synth_data_, int id_ ) : shape( &synth_data_->filter_datas[id_]->env_data->shape ), is_main_adsr(false) {}
    FShapeSlConfig( MoniqueSynthData*const synth_data_ ) : ModulationSliderConfigBase(), shape( &synth_data_->env_data->shape ), is_main_adsr(true) {}

    JUCE_LEAK_DETECTOR (FShapeSlConfig)
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

    MoniqueSynthData*const synth_data;

    bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
    {
        return true;
    }
    bool action_keep_env_pop_open_for( const LFOData*const lfo_data_ ) const noexcept override
    {
        return lfo_data_ == synth_data->lfo_datas[0] or lfo_data_ == synth_data->lfo_datas[1] or lfo_data_ == synth_data->lfo_datas[2];
    }

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
    /*
    StringRef get_bottom_button_text() const noexcept override
    {
        return "";
    }
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
        "Possible targets: CUTOFF, RESONANCE, GAIN, DESTROY, PAN, FILTER VOLUME"
    )

public:
    EnvLfoSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        adsr_lfo_mix( &synth_data_->filter_datas[id_]->adsr_lfo_mix ),
        synth_data( synth_data_ )
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
    MoniqueSynthData*const synth_data;

    String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
    {
        return true;
    }
    bool action_keep_env_pop_open_for( const LFOData*const lfo_data_ ) const noexcept override
    {
        return lfo_data_ == synth_data->lfo_datas[0] or lfo_data_ == synth_data->lfo_datas[1] or lfo_data_ == synth_data->lfo_datas[2];
    }

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
    */
    StringRef get_top_button_text() const noexcept override
    {
        return bottom_text;
    }
    /*
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
        return bottom_text;
    }
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
        return get_lfo_speed_multi_as_text( speed->get_value(), synth_data->runtime_info, synth_data->runtime_notifyer->get_sample_rate() );
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( speed->get_value() <= 17 )
        {
            if( is_integer(speed->get_value()) )
            {
                return "th";
            }
            else
            {
                return "hz";
            }
        }
        else
        {
            return "#";
        }
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
        "Possible targets: OSC, MOD-MIX -> CUTOFF, RESONANCE, GAIN, DESTROY, PAN, FILTER VOLUME"
    )

public:
    LFOSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        speed( &synth_data_->lfo_datas[id_]->speed ),
        synth_data( synth_data_ ),

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
    IntParameter*const filter_type;

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
    SHOW_TYPES show_slider_value_on_top_on_change() const noexcept override
    {
        return SHOW_OWN_VALUE;
    }
    String get_center_value() const noexcept override
    {
        if( not cutoff->midi_control->get_ctrl_mode() )
        {
            /*
                if( filter_type->get_value() == LPF )
                {
                    return String( auto_round( MAX_CUTOFF * cutoff->get_value() + MIN_CUTOFF ) );
                }
                else
                {
                    return String( auto_round( MAX_CUTOFF * cutoff->get_value() + MIN_CUTOFF ) );
                }
                */
            return String( auto_round( get_cutoff(cutoff->get_value()) ) );
        }
        else
        {
            return String( auto_round( cutoff->get_modulation_amount()*100 ) );
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( not cutoff->midi_control->get_ctrl_mode() )
        {
            return "Hz";
        }
        else
        {
            return "%";
        }
    }

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
    FCutoffSLConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        cutoff( &synth_data_->filter_datas[id_]->cutoff ),
        modulate_cutoff( &synth_data_->filter_datas[id_]->modulate_cutoff ),
        filter_type( &synth_data_->filter_datas[id_]->filter_type )
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
    FResonanceSLConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        resonance( &synth_data_->filter_datas[id_]->resonance ),
        modulate_resonance( &synth_data_->filter_datas[id_]->modulate_resonance )
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
class FPanSlConfig : public ModulationSliderConfigBase
{
    Parameter*const pan;
    BoolParameter*const modulate_pan;

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
        return pan;
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
        return pan;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return modulate_pan;
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
        return "PAN";
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
        "Define the filter panorama (left and right).\n"
        "(Also processed if 'PASS' as filter type is selected)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns filter panorama modulation by ENVELOPE-LFO-MIX (MOD-MIX) on or off."
    )
    BOTTOM_BUTTON_DIALS
    (
        "FILTER PAN",
        "FILTER PAN MODULATION POWER"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the filter pan modulation power relative to the defined filter pan (front).\n"
        NO_MOD_EFFECT
        GENERAL_MOD_EXAMPLE
    )

public:
    FPanSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        pan( &synth_data_->filter_datas[id_]->pan ),
        modulate_pan( &synth_data_->filter_datas[id_]->modulate_pan )
    {}

    JUCE_LEAK_DETECTOR (FPanSlConfig)
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
        return "VOL";
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
        "(Also processed if 'PASS' as filter type is selected)"
        "\n"
        "Output target: EQ bank"
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
    FVolumeSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        volume( &synth_data_->filter_datas[id_]->output ),
        modulate_volume( &synth_data_->filter_datas[id_]->modulate_output )
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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

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
        float bpm( runtime_info->bpm );
#ifdef IS_STANDALONE
        if( runtime_info->is_extern_synced )
        {
            return String(auto_round(bpm)) + String(" E");
        }
        else
        {
            return String(round01(bpm)) + String(" I");
        }
#else
        return String(round01(bpm)) + String(" BPM");
#endif
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
        "Affected: ARPEGGIATOR, LFO's if slow (speed is defined in note durations)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns SPEED SYNC on or off.\n"
        "\n"
        "If disabled you can define your own speed ignoring the host speed (plugin) or the MIDI-Clock (standalone)."
    )

public:
    BPMSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        speed( &synth_data_->speed ),
        sync( &synth_data_->sync ),
        runtime_info( synth_data_->runtime_info )
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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

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
        return String(auto_round(speed->get_value()*ArpSequencerData::speed_multi_to_value(speed_multi->get_value())));
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
    SpeedMultiSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        speed_multi( &synth_data_->arp_sequencer_data->speed_multi ),
        speed( &synth_data_->speed )
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
    IntParameter*const octave_offset;
    IntParameter*const note_offset;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

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
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return note_offset;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return osc_retune;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "O-TUNE";
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
        return "OCT";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "P-NOTE";
    }
    /*
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
        if( not octave_offset->midi_control->get_ctrl_mode() )
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
        else
        {
            return MidiMessage::getMidiNoteName( 60+note_offset->get_value(), true, false, 0 );
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
    BOTTOM_BUTTON_DIALS
    (
        "OCTAVE OFFSET",
        "PROGRAM NOTE"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the program note to easily get back to the right tune of the program by pushing the reset button."
    )

public:
    OctaveOffsetSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        octave_offset( &synth_data_->octave_offset ),
        note_offset( &synth_data_->note_offset )
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
class FXDistortionSlConfig : public ModulationSliderConfigBase
{
    Parameter*const distortion;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }
    /*
    bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
    {
        return true;
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
        return "DIST";
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
        "Define the power of distortion effects before and after the EQ bank."
    )

public:
    FXDistortionSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        distortion( &synth_data_->distortion )
    {}

    JUCE_LEAK_DETECTOR (FXDistortionSlConfig)
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
    Parameter*const bypass;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }

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
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return bypass;
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
        return "RESO";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MIX";
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
        "Define the resonance of all band filters."
    )
    BOTTOM_BUTTON_DIALS
    (
        "BAND RESONANCE",
        "EQ MIX"
    )
    BACK_SLIDER_DESCRIPTION
    (
        "Define the mix of EQ output and EQ input.\n"
        "\n"
        "Values greater than 0 add more and more of the EQ bank output to the not EQ processed signal (FILTER MIX output).\n"
        "(If the value is zero the EQ is bypassed)"
    )

public:
    FColourSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        shape( &synth_data_->shape ),
        bypass( &synth_data_->eq_data->bypass )
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
    Parameter*const width;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }

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
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return width;
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
        return "ROOM";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "WIDTH";
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
    // TODO
    TOP_SLIDER_DESCIPTION
    (
        "Define the room of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

    /*
    TOP_SLIDER_DESCIPTION
    (
        "Define the width of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )
    */
public:
    RRoomSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        room( &synth_data_->reverb_data->room ),
        width( &synth_data_->reverb_data->width )
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
class RDrySlConfig : public ModulationSliderConfigBase
{
    Parameter*const dry_wet_mix;
    Parameter*const pan;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }

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
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return pan;
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
        return "WET";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "PAN";
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
        if( dry_wet_mix->midi_control->get_ctrl_mode() )
        {
            return String(auto_round(pan->get_value()*100));
        }
        else
        {
            return String(auto_round(dry_wet_mix->get_value()*100));
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( dry_wet_mix->midi_control->get_ctrl_mode() )
        {
            if( pan->get_value() > 0 )
                return "R";
            else
                return "L";
        }
        else
        {
            return "";
        }
    }

    //==============================================================================
    // TOOLTIP
    // TODO
    TOP_SLIDER_DESCIPTION
    (
        "Define the wet/dry mix of the reverb effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

public:
    RDrySlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        dry_wet_mix( &synth_data_->reverb_data->dry_wet_mix ),
        pan( &synth_data_->reverb_data->pan )
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
    Parameter*const pan;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }
    bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
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
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return pan;
    }

    //==============================================================================
    // TOP BUTTON
    /*
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
    return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
    return delay_record;
    }
    StringRef get_top_button_text() const noexcept override
    {
    return "REC";
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
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "PAN";
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
        if( delay->midi_control->get_ctrl_mode() )
        {
            return String(auto_round(pan->get_value()*100));
        }
        else
        {
            return String(auto_round(delay->get_value()*100));
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( delay->midi_control->get_ctrl_mode() )
        {
            if( pan->get_value() > 0 )
                return "R";
            else
                return "L";
        }
        else
        {
            return "";
        }
    }

    //==============================================================================
    // TOOLTIP
    // TODO
    // TODO RECORD
    TOP_SLIDER_DESCIPTION
    (
        "Define the delay time of the delay effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

public:
    DelaySlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        delay( &synth_data_->delay ),
        pan( &synth_data_->delay_pan )
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
class DelayReflexSlConfig : public ModulationSliderConfigBase
{
    Parameter*const delay_reflexion;

    RuntimeNotifyer*const runtime_notifyer;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }
        bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
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
        return delay_reflexion;
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
        return delay_record_size;
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
        return delay_record;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "FILL";
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
        return "RFLEX";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "SIZE";
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
        return delay_to_text(delay_reflexion->get_value(), runtime_notifyer->get_sample_rate());
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "ms";
    }

    //==============================================================================
    // TOOLTIP
    // TODO
    TOP_SLIDER_DESCIPTION
    (
        "Define the delay time of the delay effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

public:
    DelayReflexSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        delay_reflexion( &synth_data_->delay_refexion ),

        runtime_notifyer( synth_data_->runtime_notifyer )
    {}

    JUCE_LEAK_DETECTOR (DelayReflexSlConfig)
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
class DelayRecordSlConfig : public ModulationSliderConfigBase
{
    Parameter*const record_release;
    IntParameter*const record_size;
    BoolParameter*const delay_record;

    RuntimeNotifyer*const runtime_notifyer;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }

    //==============================================================================
    // FRONT SLIDER
    SLIDER_STYLES get_front_slider_style() const noexcept override
    {
        return VALUE_SLIDER;
    }
    Parameter* get_front_parameter_base() const noexcept override
    {
        return record_release;
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
        return record_size;
    }

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_ON_OFF;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return delay_record;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "FILL";
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
        return "REL";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "SIZE";
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
        if( record_release->midi_control->get_ctrl_mode() )
        {
            return delay_to_text(record_size->get_value(), runtime_notifyer->get_sample_rate());
        }
        else
        {
            return String(auto_round( record_release->get_value()*100 ));
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        return "";
    }

    //==============================================================================
    // TOOLTIP
    // TODO
    TOP_SLIDER_DESCIPTION
    (
        "Define the delay time of the delay effect.\n"
        "(Has no effect if FX MIX is set to zero)"
    )

public:
    DelayRecordSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        record_size( &synth_data_->delay_record_size ),
        record_release( &synth_data_->delay_record_release ),
        delay_record( &synth_data_->delay_record ),

        runtime_notifyer( synth_data_->runtime_notifyer )
    {}

    JUCE_LEAK_DETECTOR (DelayRecordSlConfig)
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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }

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
        "NOT Affected: DESTROY"
    )

public:
    BypassConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        effect_bypass( &synth_data_->effect_bypass )
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

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::MASTER_THEME;
    }

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
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return final_clipping;
    }

    //==============================================================================
    // TOP BUTTON
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
        return "VOL";
    }
    /*
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "FORCE";
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
        "Define the final output volume."
    )

public:
    VolumeConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        volume( &synth_data_->volume )
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
    Parameter*const pan;

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
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }    
    bool is_opaque() const noexcept override
    {
        return false;
    }
    bool use_click_through_hack() const noexcept override
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
        return pan;
    }

    //==============================================================================
    // TOP BUTTON
    /*
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
                value = synth_data->voice->get_chorus_modulation_env_amp();
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
    */

    //==============================================================================
    // BOTTOM BUTTON
    StringRef get_bottom_button_text() const noexcept override
    {
        return "CHOR";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "PAN";
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
        {
            return String( auto_round(pan->get_value()*100)  );
        }
        else
        {
            return String( auto_round(modulation->get_value()*100)  );
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( modulation->midi_control->get_ctrl_mode() )
        {
            if( pan->get_value() > 0 )
                return "R";
            else
                return "L";
        }
        else
        {
            return "";
        }
    }

    //==============================================================================
    // TOOLTIP
    // TODO
    TOP_SLIDER_DESCIPTION
    (
        "Define the chorus amount.\n"
        "(Will be a fixed amount if ENV is turned off)\n"
        "(Has no effect if FX MIX is set to zero)"
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns modulation by an envelope for the chorus amount on or off.\n"
        "(Has no effect if ENV is turned off)\n"
        "\n"
        "If ENV is disabled the chorus dial defines a fixed amount level and the envelope will be ignored."
    )

public:
    CModSlConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        modulation( &synth_data_->chorus_data->modulation ),
        pan( &synth_data_->chorus_data->pan ),

        synth_data( synth_data_ ),
        chorus_data( synth_data_->chorus_data )
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

    RuntimeNotifyer*const runtime_notifyer;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

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
        return "NOTE";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "VELO";
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
            const float sr = runtime_notifyer->get_sample_rate();
            value = samplesToMsFast(glide->get_value()*sr/2,sr);
        }
        return String(auto_round(value));
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
        "If enabled all steps without an empty step between will be handled as one step (no envelope retrigger).\n"
        "\n"
        "Sustain pedal can bind and hold notes (AMP ENV).\n"
        "Sostenuto pedal can bind and hold the filter ENV's.\n"
        "(If you only have a sustain pedal you can enable bind pedals in the setup.)\n"
        "\n"
        "Note: MIDI CC 64 = sustain pedal.\n"
        "Note: MIDI CC 66 = sostenuto pedal."
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
    GlideConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        glide( &synth_data_->glide ),
        velocity_glide_time( &synth_data_->velocity_glide_time ),
        connect( &synth_data_->arp_sequencer_data->connect ),

        runtime_notifyer( synth_data_->runtime_notifyer )
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
    MoniqueSynthData*const synth_data;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

    //==============================================================================
    // OPTION POPUP
    BoolParameter* get_top_button_option_param_a() noexcept override
    {
        return &synth_data->keep_arp_always_on;
    }
    BoolParameter* get_top_button_option_param_b() noexcept override
    {
        return &synth_data->keep_arp_always_off;
    }
    StringRef get_top_button_option_param_a_text() const noexcept override
    {
        return "FORCED ON";
    }
    StringRef get_top_button_option_param_b_text() const noexcept override
    {
        return "FORCED OFF";
    }
    StringRef get_top_button_option_param_a_tool_tip() const noexcept override
    {
        return "Keeps the arpeggiator always on.\n"
               "\n"
               "If enabled the defined ARP ON state will be ignored and the arpeggiator is always ON by force!\n"
               "If you save a program and this option is turned on the program will be stored with ARP ON, else it uses the current ARP ON state.\n"
               "\n"
               "IMPORTANT NOTES:\n"
               "****************\n"
               "This is a runtime parameter and is always turned off at application start.\n"
               "If you change the ARP ON state on the main user interface will turning off this this option.\n"
               ;
    }
    StringRef get_top_button_option_param_b_tool_tip() const noexcept override
    {
        return "Keeps the arpeggiator always off.\n"
               "\n"
               "If enabled the defined ARP ON state will be ignored and the arpeggiator is always OFF by force!\n"
               "If you save a program and this option is turned on the program will be stored with ARP OFF, else it uses the current ARP ON state.\n"
               "\n"
               "IMPORTANT NOTES:\n"
               "****************\n"
               "This is a runtime parameter and is always turned off at application start.\n"
               "If you change the ARP ON state on the main user interface will turning off this this option.\n"
               ;
    }

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
    ShuffleConfig( MoniqueSynthData*const synth_data_ )
        :
        ModulationSliderConfigBase(),
        shuffle( &synth_data_->arp_sequencer_data->shuffle ),
        is_on( &synth_data_->arp_sequencer_data->is_on ),

        synth_data( synth_data_ )
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
    bool action_keep_env_pop_open_for( const ENVData*const env_ ) const noexcept override
    {
        bool success = false;
        EQData*eq_data = synth_data->eq_data;
        if( env_ == eq_data->envs[0]
                or env_ == eq_data->envs[1]
                or env_ == eq_data->envs[2]
                or env_ == eq_data->envs[3]
                or env_ == eq_data->envs[4]
                or env_ == eq_data->envs[5]
                or env_ == eq_data->envs[6] )
        {
            success = true;
        }

        return success;
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::FX_THEME;
    }
    bool use_click_through_hack() const noexcept override
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
    /*
    SLIDER_STYLES get_back_slider_style() const noexcept override
    {
        return VALUE_SLIDER_2;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    Parameter* get_back_parameter_base() const noexcept override
    {
        return state;
    }
    */

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
                value = get_last_modulation_amount( velocity );
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
    /*
    StringRef get_bottom_button_text() const noexcept override
    {
        return bottom_text;
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "MO-ENV";
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
        return String( auto_round(velocity->get_value()*100)  );
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
        "Define the band boost amount for this frequency (bottom caption).\n"
        "(Has no effect if EQ MIX is set to zero)\n"
        "(Will be a fixed gain level if ENV is turned off)\n"
        "\n"
        "Values greater than 0 will boost this band and values less than 0 will reduce it."
    )
    TOP_BUTTON_DESCRIPTION
    (
        "Turns modulation by an envelope for this band boost on or off.\n"
        "\n"
        "If ENV is disabled the band boost dial defines a fixed boost level and the envelope will be ignored."
    )

public:
    EQSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        ModulationSliderConfigBase(),
        id( id_ ),
        velocity( &synth_data_->eq_data->velocity[id_] ),
        hold( &synth_data_->eq_data->hold[id_] ),

        synth_data( synth_data_ )
    {
        const float frequency_high_pass = get_high_pass_band_frequency( id_ );
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
#include "monique_core_Synth.h"
class ArpStepSlConfig : public ModulationSliderConfigBase
{
    Parameter*const octave_offset;
    Parameter*const tune;
    Parameter*const velocity;
    MoniqueSynthesiserVoice*const voice;

    const String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::ARP_THEME;
    }

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
        return "VELO";
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
        {
            return String( auto_round(velocity->get_value()*100) );
        }
        else
        {
            return MidiMessage::getMidiNoteName(voice->get_current_note() + tune->get_value() +octave_offset->get_value()*12 + 12,true,true,0);
        }
    }
    StringRef get_center_suffix() const noexcept override
    {
        if( tune->midi_control->get_ctrl_mode() )
            return "%";
        else
            return "";
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
//todo chencken
public:
    ArpStepSlConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        ModulationSliderConfigBase(),
        octave_offset( &synth_data_->octave_offset ),
        tune( &synth_data_->arp_sequencer_data->tune[id_] ),
        velocity( &synth_data_->arp_sequencer_data->velocity[id_] ),
        voice( synth_data_->voice ),
        bottom_text( id_ == 0 ? String(("STEP " + String(id_+1))) : String(id_+1) )
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
    const int id;

    Parameter*const morhp_state;
    BoolParameter*const is_morph_modulated;

    MoniqueSynthData*const synth_data;

    const String bottom_text;

    //==============================================================================
    // BASIC SLIDER TYPE
    /*
    bool get_is_linear() const noexcept override
    {
    return false;
    }
    */
    bool action_keep_env_pop_open_for( const LFOData*const lfo_data_ ) const noexcept override
    {
        return lfo_data_ == synth_data->mfo_datas[0] or lfo_data_ == synth_data->mfo_datas[1] or lfo_data_ == synth_data->mfo_datas[2] or lfo_data_ == synth_data->mfo_datas[3];
    }
    COLOUR_THEMES get_colour_theme() const noexcept override
    {
        return COLOUR_THEMES::MORPH_THEME;
    }
    bool use_click_through_hack() const noexcept override
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
        return morhp_automation_power;
    }
    */

    //==============================================================================
    // TOP BUTTON
    TOP_BUTTON_TYPE get_top_button_type() const noexcept override
    {
        return TOP_BUTTON_IS_MODULATOR;
    }
    BoolParameter* get_top_button_parameter_base() const noexcept override
    {
        return is_morph_modulated;
    }
    StringRef get_top_button_text() const noexcept override
    {
        return "MFO";
    }
    float get_top_button_amp() const noexcept override
    {
        float value = NO_TOP_BUTTON_AMP;
        const bool is_on = bool(is_morph_modulated->get_value());
        if( synth_data->animate_envs )
        {
            if( is_on )
            {
                value = synth_data->voice->get_mfo_amp(id);
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
    /*
    StringRef get_bottom_button_text() const noexcept override
    {
        return "MORPH";
    }
    StringRef get_bottom_button_switch_text() const noexcept override
    {
        return "LFO";
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
        return String(auto_round( morhp_state->get_value()*100 ));
    }
    StringRef get_center_suffix() const noexcept override
    {
        const float state = morhp_state->get_value();
        if( state <= 0.5 )
        {
            return String("L");
        }
        else
        {
            return String("R");
        }
    }

    //==============================================================================
    // TOOLTIP
    TOP_SLIDER_DESCIPTION
    (
        "Morph between two programs, one the LEFT side and one on the RIGHT side.\n"
        "\n"
        "With the morph editor (EDIT) you can load any available Monique-program to the left or right side of each morph group.\n"
        "\n"
        "How stay the morph programs in sync with my changes?\n"
        "Any changes at a morphable paramter (e.g. cutoff) will update the left or right morph program. "
        "The update depends on the morph slider position. "
        "If the slider is more left (less than 50) the left program will be updated with first priority, on the right the right program.\n"
        "\n"
        "RECOMMENDATION: if you like to keep 100% control over the left and right programs keep the morph slider absolutely left or right during changing morphable parameters (e.g. cutoff). Because this will route changes only to the left or right side.\n"
        "\n"
        "\n"
        "MORPH OVERVIEW:\n"
        "---------------\n"
        "OSC Morph Group:\n"
        "Morphs: OSC's, FM\n"
        "Except: BUTTONS\n"
        "\n"
        "FLT Morph Group:\n"
        "Morphs: FILTERS\n"
        "Except: FILTER TYPE, BUTTONS\n"
        "\n"
        "FX Morph Group:\n"
        "Morphs: AMP ENVELOPE, EQ BANK, ALL FX, FX-MIX, MASTER VOLUME\n"
        "Except: BUTTONS\n"
        "\n"
        "ARP Morph Group:\n"
        "Morphs: (ARPEGGIATOR), SHUFFLE, NOTE-G, VELO-G, STEP NOTE, STEP VELOCITY\n"
        "Except: BUTTONS\n"
    )

public:
    MorphSLConfig( MoniqueSynthData*const synth_data_, int id_ )
        :
        ModulationSliderConfigBase(),
        id(id_),
        morhp_state( &synth_data_->morhp_states[id_] ),
        is_morph_modulated( &synth_data_->is_morph_modulated[id_] ),
        synth_data( synth_data_ ),
        bottom_text( String( "TOGGL" ) + String(id_+1) )
    {}

    JUCE_LEAK_DETECTOR (MorphSLConfig)
};

#endif  // Monique_Ui_MainwindowCONFIG_H_INCLUDED












