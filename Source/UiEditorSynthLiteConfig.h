/*
  ==============================================================================

    UiEditorSynthLiteConfig.h
    Created: 6 May 2015 9:03:35am
    Author:  monotomy

  ==============================================================================
*/

#ifndef UIEDITORSYNTHLITECONFIG_H_INCLUDED
#define UIEDITORSYNTHLITECONFIG_H_INCLUDED

#include "mono_ModulationSlider.h"
#include "SynthData.h"

// TODO store references to the most uses objects

//==============================================================================
//==============================================================================
//==============================================================================
struct WAVESlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "WAVE";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[id]->wave);
    }

    StringRef get_botton_button_switch_text() const override {
        return "FM";
    }

    StringRef get_top_button_text() const override {
        if( id == 0 )
            return "MOD OFF";
        else
            return "SYNC";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        if( id == 0 )
            return SYNTH_PARAM(osc_datas[id]->mod_off);
        else
            return SYNTH_PARAM(osc_datas[id]->sync);
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[id]->fm_amount);
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        if( DATA(osc_datas[id]).fm_amount.midi_control->get_ctrl_mode() )
            return String( round0(DATA(osc_datas[id]).fm_amount*100) );
        else
            return String( DATA(osc_datas[id]).wave );
    }
    StringRef get_top_suffix() const override {
        if( DATA(osc_datas[id]).fm_amount.midi_control->get_ctrl_mode() )
            return "%";
        else
            return "wav";
    }
    WAVESlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (WAVESlConfig)
};

//==============================================================================
struct OSCSlConfig : public ModulationSliderConfigBase {
    const int id;

    String bottom_text;
    StringRef get_bottom_button_text() const override {
        return bottom_text;
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[id]->octave);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    String top_text;
    StringRef get_top_button_text() const override {
        return top_text;
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[id]->is_lfo_modulated);
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        const float octave = DATA(osc_datas[id]).octave;
        if( DATA(osc_datas[id]).octave.midi_control->get_ctrl_mode() )
            return String( round0(DATA(osc_datas[id]).octave.get_modulation_amount() * 100) );
        else
        {
            if( octave < 10 and octave > -10 )
                return String(round001(octave));
            else
                return String(round01(octave));
        }
    }
    StringRef get_top_suffix() const override {
        if( DATA(osc_datas[id]).octave.midi_control->get_ctrl_mode() )
            return "%";
        else
            return "#";
    }
    OSCSlConfig( int id_ )
        :
        id( id_ ),
        bottom_text( "OSC " + String(id+1) ),
        top_text( "LFO " + String(id+1) )
    {}

    JUCE_LEAK_DETECTOR (OSCSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FMFreqSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "FM FREQ";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->fm_multi);
    }

    StringRef get_botton_button_switch_text() const override {
        return "FM SWING";
    }

    StringRef get_top_button_text() const override {
        return "SYNC -";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->sync);
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->fm_swing);
    }

    FMFreqSlConfig() {}

    JUCE_LEAK_DETECTOR (FMFreqSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FMAmountSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "OSC PLUS";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->puls_width);
    }

    StringRef get_botton_button_switch_text() const override {
        return "OSC SWITCH";
    }

    StringRef get_top_button_text() const override {
        return "- SHOT";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->fm_wave);
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(osc_datas[0]->osc_switch);
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        if( DATA(osc_datas[0]).puls_width.midi_control->get_ctrl_mode() )
            return String( DATA(osc_datas[0]).osc_switch );
        else
            return String( DATA(osc_datas[0]).puls_width );
    }

    FMAmountSlConfig() {}

    JUCE_LEAK_DETECTOR (FMAmountSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct InputSlConfig : public ModulationSliderConfigBase {
    const int filter_id;
    const int input_id;

    String bottom_text;
    StringRef get_bottom_button_text() const override {
        return bottom_text;
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[filter_id]->input_sustains[input_id]);
    }

    String top_text;
    StringRef get_botton_button_switch_text() const override {
        return top_text;
    }

    StringRef get_top_button_text() const override {
        return "FIX IN";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[filter_id]->input_holds[input_id]);
    }
    float get_top_button_amp() const override {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! DATA( synth_data ).animate_input_env )
            value = NO_TOP_BUTTON_AMP;
        else if( ! DATA(filter_datas[filter_id]).input_holds[input_id] )
            value = mono_ParameterOwnerStore::getInstance()->get_flt_input_env_amp(filter_id,input_id);

        return value;
    }

    float get_override_min_value() const override {
        if( filter_id == 0 )
            return 0;
        else
            return DONT_OVERRIDE_SLIDER_VALUE;
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_input_env_datas[input_id + SUM_INPUTS_PER_FILTER*filter_id]->state);
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        if( DATA(filter_input_env_datas[input_id + SUM_INPUTS_PER_FILTER*filter_id]).state.midi_control->get_ctrl_mode() )
            return String( round001(DATA(filter_input_env_datas[input_id + SUM_INPUTS_PER_FILTER*filter_id]).state) );
        else
            return String( round01(DATA(filter_datas[filter_id]).input_sustains[input_id]*100)  );
    }
    StringRef get_top_suffix() const override {
        if( DATA(filter_input_env_datas[input_id + SUM_INPUTS_PER_FILTER*filter_id]).state.midi_control->get_ctrl_mode() )
            return "X";
        else if( DATA(filter_datas[filter_id]).input_sustains[input_id] >= 0 )
        {
            if( filter_id == 1 )
                return "F1";
            else if( filter_id == 2 )
                return "F2";
            else
                return "O";
        }
        else
            return "O";
    }

    InputSlConfig( int filter_id_, int input_id_ )
        :
        filter_id( filter_id_ ),
        input_id( input_id_ ),
        bottom_text( "OSC " + String(input_id+1) ),
        top_text( "ADR" )
    {}

    JUCE_LEAK_DETECTOR (InputSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct GForceSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "DESTROY";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->distortion);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    StringRef get_top_button_text() const override {
        return "MODUL";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->modulate_distortion);
    }

    GForceSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (GForceSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FAttackSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "ATTACK";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->attack);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_attack_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).attack * DATA(env_datas[id]).max_attack_time * 1000;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    FAttackSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FAttackSlConfig)
};
//==============================================================================
struct FDecaySlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "DECAY";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->decay);
    }
    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_decay_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).decay * DATA(env_datas[id]).max_decay_time * 1000;
        if( value < 0 )
            return "OFF";
        else if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    FDecaySlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FDecaySlConfig)
};
//==============================================================================
struct FSustainSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "SUSTAIN";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->sustain);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).sustain * 100;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "%";
    }
    FSustainSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FSustainSlConfig)
};
//==============================================================================
struct FSustainTimeSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "SUS TIME";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->sustain_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).sustain_time * 8.0f * 1000;
        if( value < 100 )
            return String(round01(value));
        else if( value == 8000 )
            return String("unltd");
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        if( DATA(env_datas[id]).sustain_time == 1 )
            return "";
        else
            return "ms";
    }
    FSustainTimeSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FSustainTimeSlConfig)
};
//==============================================================================
struct FReleaseSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "RELEASE";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->release);
    }
    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_release_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).release * DATA(env_datas[id]).max_release_time * 1000;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    FReleaseSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FReleaseSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct EnvLfoSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "MOD MIX";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->adsr_lfo_mix);
    }

    EnvLfoSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (EnvLfoSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct LFOSlConfig : public ModulationSliderConfigBase {
    const int id;

    String bottom_text;
    StringRef get_bottom_button_text() const override {
        return bottom_text;
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(lfo_datas[id]->speed);
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        const float speed = DATA(lfo_datas[id]).speed;
        if( speed <= 6 )
        {
            if( speed == 0 )
                return "16/1";
            else if( speed == 1 )
                return "12/1";
            else if( speed == 2 )
                return "8/1";
            else if( speed == 3 )
                return "4/1";
            else if( speed == 4 )
                return "3/1";
            else if( speed == 5 )
                return "2/1";
            else
                return "1/1";
        }
        else if( speed <= 17 )
            if( speed == 7 )
                return "3/4";
            else if( speed == 8 )
                return "1/2";
            else if( speed == 9 )
                return "1/3";
            else if( speed == 10 )
                return "1/4";
            else if( speed == 11 )
                return "1/8";
            else if( speed == 12 )
                return "1/12";
            else if( speed == 13 )
                return "1/16";
            else if( speed == 14 )
                return "1/24";
            else if( speed == 15 )
                return "1/32";
            else if( speed == 16 )
                return "1/64";
            else
                return "1/128";
        else
        {
            return MidiMessage::getMidiNoteName(frequencyToMidi(midiToFrequency(33+speed-18)),true,true,0);
        }
    }
    StringRef get_top_suffix() const override {
        if( DATA(lfo_datas[id]).speed <= 17 )
            return "th";
        else
            return "#";
    }
    LFOSlConfig( int id_ )
        :
        id( id_ ),
        bottom_text( "LFO " + String(id+1) )
    {}

    JUCE_LEAK_DETECTOR (LFOSlConfig)
};

//==============================================================================
struct FCutoffSLConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "CUTOFF";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->cutoff);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    StringRef get_top_button_text() const override {
        return "MODUL";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->modulate_cutoff);
    }

    FCutoffSLConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FCutoffSLConfig)
};
//==============================================================================
struct FResonanceSLConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "RESO";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->resonance);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    StringRef get_top_button_text() const override {
        return "MODUL";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->modulate_resonance);
    }

    FResonanceSLConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FResonanceSLConfig)
};
//==============================================================================
struct FGainSLConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "GAIN";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->gain);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    StringRef get_top_button_text() const override {
        return "MODUL";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->modulate_gain);
    }

    FGainSLConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FGainSLConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FVolumeSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "VOLUME";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->output);
    }

    StringRef get_botton_button_switch_text() const override {
        return "MOD (%)";
    }

    StringRef get_top_button_text() const override {
        return "MODUL";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->modulate_output);
    }

    FVolumeSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FVolumeSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct BPMSlConfig : public ModulationSliderConfigBase {
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->speed);
    }
    bool get_is_bottom_button_text_dynamic() const override {
        return true;
    }
    StringRef get_bottom_button_text() const override {
        return String(DATA(synth_data).speed)+String(" BPM");
    }
    StringRef get_top_button_text() const override {
        return "SYNC";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->sync);
    }

    BPMSlConfig() {}

    JUCE_LEAK_DETECTOR (BPMSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct SpeedMultiSlConfig : public ModulationSliderConfigBase {
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(arp_data->speed_multi);
    }
    bool get_is_bottom_button_text_dynamic() const override {
        return true;
    }
    StringRef get_bottom_button_text() const override {
        return speed_multi_to_text( DATA( arp_data ).speed_multi );
    }
    StringRef get_top_button_text() const override {
        return "x1";
    }

    SpeedMultiSlConfig() {}

    JUCE_LEAK_DETECTOR (SpeedMultiSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FCompressorSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        if( id == 0 )
            return "PEAK-BOOST";
        else
            return "COMP|BOOST";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->compressor);
    }

    float get_override_min_value() const override {
        if( id == 0 )
            return 0;
        else
            return DONT_OVERRIDE_SLIDER_VALUE;
    }

    StringRef get_botton_button_switch_text() const override {
        return "CLIPP";
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(filter_datas[id]->output_clipping);
    }

    FCompressorSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (FCompressorSlConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
struct AttackSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "ATTACK";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->attack);
    }
    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_attack_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).attack * DATA(env_datas[id]).max_attack_time * 1000;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    AttackSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (AttackSlConfig)
};
//==============================================================================
struct DecaySlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "DECAY";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->decay);
    }
    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_decay_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).decay * DATA(env_datas[id]).max_decay_time * 1000;
        if( value < 0 )
            return "OFF";
        else if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    DecaySlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (DecaySlConfig)
};
//==============================================================================
struct SustainSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "SUSTAIN";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->sustain);
    }

    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).sustain * 100;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "%";
    }
    SustainSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (SustainSlConfig)
};
//==============================================================================
struct ReleaseSlConfig : public ModulationSliderConfigBase {
    const int id;

    StringRef get_bottom_button_text() const override {
        return "RELEASE";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->release);
    }
    StringRef get_botton_button_switch_text() const override {
        return "MAX T(ms)";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(env_datas[id]->max_release_time);
    }
    bool get_is_linear() const {
        return true;
    }
    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        float value = DATA(env_datas[id]).release * DATA(env_datas[id]).max_release_time * 1000;
        if( value < 100 )
            return String(round01(value));
        else
            return String(round0(value));
    }
    StringRef get_top_suffix() const override {
        return "ms";
    }
    ReleaseSlConfig( int id_ )
        :
        id( id_ )
    {}

    JUCE_LEAK_DETECTOR (ReleaseSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct RRoomSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "ROOM";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(reverb_data->room);
    }
    JUCE_LEAK_DETECTOR (RRoomSlConfig)
};
//==============================================================================
struct RWidthSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "WIDTH";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(reverb_data->width);
    }
    JUCE_LEAK_DETECTOR (RWidthSlConfig)
};
//==============================================================================
struct RDrySlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "WET|DRY";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(reverb_data->dry_wet_mix);
    }

    JUCE_LEAK_DETECTOR (RDrySlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct DelaySlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "DELAY";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->delay);
    }

    JUCE_LEAK_DETECTOR (DelaySlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct CModSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "CHORS";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(chorus_data->modulation);
    }
    StringRef get_botton_button_switch_text() const override {
        return "ADR";
    }
    StringRef get_top_button_text() const override {
        return "FIX";
    }
    /*
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
    return SYNTH_PARAM(chorus_data->shine);
    }
    */
    float get_top_button_amp() const override
    {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! DATA( synth_data ).animate_eq_env )
            value = NO_TOP_BUTTON_AMP;
        else if( ! DATA(chorus_data).hold_modulation )
        {
            value = mono_ParameterOwnerStore::getInstance()->get_chorus_modulation_env_amp();
        }

        return value;
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(chorus_data->hold_modulation);
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        if( DATA(chorus_data).modulation.midi_control->get_ctrl_mode() )
            return String( round001(DATA(chorus_data).modulation_env_data->state) );
        else
            return String( round01(DATA(chorus_data).modulation*100)  );
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(chorus_data->modulation_env_data->state);
    }

    JUCE_LEAK_DETECTOR (CModSlConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct BypassConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "MIX";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->effect_bypass);
    }

    JUCE_LEAK_DETECTOR (BypassConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct VolumeConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "VOLUME";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->volume);
    }

    StringRef get_botton_button_switch_text() const override {
        return "CLIPP";
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->final_compression);
    }

    JUCE_LEAK_DETECTOR (VolumeConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct GlideConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "N GLIDE";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->glide);
    }
    StringRef get_botton_button_switch_text() const override {
        return "V GLIDE";
    }

    StringRef get_top_button_text() const override {
        return "BIND";
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->arp_sequencer_data->connect);
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->velocity_glide_time);
    }

    JUCE_LEAK_DETECTOR (GlideConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ShuffleConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "SHUFL";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(arp_data->shuffle);
    }

    StringRef get_top_button_text() const override {
        return "ARP";
    }
    float get_top_button_amp() const override {
        return DATA(arp_data).is_on ? 1 : 0 ;
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(arp_data->is_on);
    }

    JUCE_LEAK_DETECTOR (ShuffleConfig)
};
//==============================================================================
//==============================================================================
//==============================================================================
struct FColourSlConfig : public ModulationSliderConfigBase {
    StringRef get_bottom_button_text() const override {
        return "SHAPE";
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->resonance);
    }
    JUCE_LEAK_DETECTOR (ShuffleConfig)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
struct EQSlConfig : public ModulationSliderConfigBase {
    const int id;

    String bottom_text;
    StringRef get_bottom_button_text() const override {
        return bottom_text;
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(eq_data->velocity[id]);
    }

    StringRef get_top_button_text() const override {
        return "FIX";
    }
    float get_top_button_amp() const override {
        float value = FIXED_TOP_BUTTON_COLOUR;
        if( ! DATA( synth_data ).animate_eq_env )
            value = NO_TOP_BUTTON_AMP;
        else if( ! DATA(eq_data).hold[id] ) {
            value = mono_ParameterOwnerStore::getInstance()->get_band_env_amp(id);
        }

        return value;
    }
    mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const override {
        return SYNTH_PARAM(eq_data->hold[id]);
    }

    StringRef get_botton_button_switch_text() const override {
        return "ADR";
    }
    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(eq_data->env_datas[id]->state);
    }

    EQSlConfig( int id_ ) : id( id_ )
    {
        const float frequency_low_pass = (62.5/2) * pow(2,id+1);
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
struct ArpStepSlConfig : public ModulationSliderConfigBase {
    const int id;

    String bottom_text;
    StringRef get_bottom_button_text() const override {
        if( id == 0 )
            return bottom_text;
        else
            return String(id+1);
    }
    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(arp_data->tune[id]);
    }

    mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const override {
        return SYNTH_PARAM(arp_data->velocity[id]);
    }

    StringRef get_botton_button_switch_text() const override {
        return "VELOCITY";
    }

    ModulationSliderConfigBase::SHOW_TYPES show_slider_value_on_top_on_change() const override {
        return SHOW_OWN_VALUE;
    }
    String get_top_value() const override {
        if( DATA(arp_data).tune[id].midi_control->get_ctrl_mode() )
            return String( round01(DATA(arp_data).velocity[id]*100) );
        else
            return String( DATA(arp_data).tune[id] );
    }
    StringRef get_top_suffix() const override {
        if( DATA(arp_data).tune[id].midi_control->get_ctrl_mode() )
            return "%";
        else
            return "#";
    }

    ArpStepSlConfig( int id_ )
        :
        id( id_ ),
        bottom_text( "STEP " + String(id+1) )
    {}

    JUCE_LEAK_DETECTOR (ArpStepSlConfig)
};


struct MorphSLConfig : public ModulationSliderConfigBase {
    const int id;

    String bottom_text;

    mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const override {
        return SYNTH_PARAM(synth_data->morhp_states[id]);
    }
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
    MorphSLConfig( int id_ ) : id( id_ )
    {
        bottom_text = String( "TOGGL" ) + String(id_+1);
    }

    JUCE_LEAK_DETECTOR (MorphSLConfig)
};

#endif  // UIEDITORSYNTHLITECONFIG_H_INCLUDED



