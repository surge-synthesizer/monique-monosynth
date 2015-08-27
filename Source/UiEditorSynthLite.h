/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_C931A4A92150166E__
#define __JUCE_HEADER_C931A4A92150166E__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class AppInstanceStore;
class UiEditorSynthLitePopup;
class UiEditorMIDIIO;
class UiEditorMorph;
class UiEditorSettings;
class UiEditorGlobalSettings;
class MONOVoice;
class SynthData;
class SegmentedMeter;
class MIDIControl;
class mono_ModulationSlider;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorSynthLite  : public AudioProcessorEditor,
                           public mono_UiRefreshable,
                           public ButtonListener,
                           public ComboBoxListener,
                           public SliderListener
{
public:
    //==============================================================================
    UiEditorSynthLite ();
    ~UiEditorSynthLite();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    AppInstanceStore*const _app_instance_store;
    MONOVoice*voice;
    SynthData* synth_data;

    ScopedPointer<UiEditorSynthLitePopup> popup;
    bool last_ctrl_mode;
    bool is_in_help_mode;
    ScopedPointer<UiEditorMIDIIO> editor_midiio;
    ScopedPointer<UiEditorMorph> editor_morph;
    ScopedPointer<UiEditorSettings> editor_settings;
    ScopedPointer<UiEditorGlobalSettings> editor_global_settings;

    void refresh() noexcept override;
    void show_current_voice_data();
    void show_programs_and_select();
    void show_ctrl_state();
    void show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_ );
    void close_all_subeditors();
    void resize_subeditors();

    float last_shuffle;
    Array< TextButton* > sequence_buttons;
    void resize_sequence_buttons();
    void switch_finalizer_tab();

    Array<int> last_morpher_index;

    const float original_w;
    const float original_h;
    int last_bank;
    int last_programm;
    void sliderClicked (Slider*s_) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    bool keyPressed (const KeyPress& key);
    void modifierKeysChanged (const ModifierKeys& modifiers);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<mono_ModulationSlider> speed_multi;
    ScopedPointer<mono_ModulationSlider> morpher_4;
    ScopedPointer<mono_ModulationSlider> morpher_3;
    ScopedPointer<mono_ModulationSlider> morpher_2;
    ScopedPointer<mono_ModulationSlider> morpher_1;
    ScopedPointer<Label> label_effect_hider;
    ScopedPointer<mono_ModulationSlider> eq_9;
    ScopedPointer<mono_ModulationSlider> eq_8;
    ScopedPointer<mono_ModulationSlider> eq_7;
    ScopedPointer<mono_ModulationSlider> eq_6;
    ScopedPointer<mono_ModulationSlider> eq_5;
    ScopedPointer<mono_ModulationSlider> eq_4;
    ScopedPointer<mono_ModulationSlider> eq_3;
    ScopedPointer<mono_ModulationSlider> eq_2;
    ScopedPointer<mono_ModulationSlider> eq_1;
    ScopedPointer<mono_ModulationSlider> bypass;
    ScopedPointer<mono_ModulationSlider> chorus_modulation;
    ScopedPointer<mono_ModulationSlider> reverb_dry;
    ScopedPointer<mono_ModulationSlider> reverb_room;
    ScopedPointer<mono_ModulationSlider> osc_wave_3;
    ScopedPointer<MidiKeyboardComponent> keyboard;
    ScopedPointer<mono_ModulationSlider> glide2;
    ScopedPointer<mono_ModulationSlider> arp_step_16;
    ScopedPointer<mono_ModulationSlider> arp_step_15;
    ScopedPointer<mono_ModulationSlider> arp_step_14;
    ScopedPointer<mono_ModulationSlider> arp_step_13;
    ScopedPointer<mono_ModulationSlider> arp_step_12;
    ScopedPointer<mono_ModulationSlider> arp_step_11;
    ScopedPointer<mono_ModulationSlider> arp_step_10;
    ScopedPointer<mono_ModulationSlider> arp_step_9;
    ScopedPointer<mono_ModulationSlider> arp_step_8;
    ScopedPointer<mono_ModulationSlider> arp_step_7;
    ScopedPointer<mono_ModulationSlider> arp_step_6;
    ScopedPointer<mono_ModulationSlider> arp_step_5;
    ScopedPointer<mono_ModulationSlider> arp_step_4;
    ScopedPointer<mono_ModulationSlider> arp_step_3;
    ScopedPointer<mono_ModulationSlider> arp_step_2;
    ScopedPointer<mono_ModulationSlider> arp_step_1;
    ScopedPointer<mono_ModulationSlider> shuffle;
    ScopedPointer<mono_ModulationSlider> flt_sustain_4;
    ScopedPointer<mono_ModulationSlider> flt_decay_4;
    ScopedPointer<mono_ModulationSlider> flt_attack_4;
    ScopedPointer<mono_ModulationSlider> flt_release_3;
    ScopedPointer<mono_ModulationSlider> flt_sustain_time_3;
    ScopedPointer<mono_ModulationSlider> flt_sustain_3;
    ScopedPointer<mono_ModulationSlider> flt_decay_3;
    ScopedPointer<mono_ModulationSlider> flt_attack_3;
    ScopedPointer<mono_ModulationSlider> flt_release_2;
    ScopedPointer<mono_ModulationSlider> flt_sustain_time_2;
    ScopedPointer<mono_ModulationSlider> flt_sustain_2;
    ScopedPointer<mono_ModulationSlider> flt_decay_2;
    ScopedPointer<mono_ModulationSlider> flt_attack_2;
    ScopedPointer<mono_ModulationSlider> flt_release_1;
    ScopedPointer<mono_ModulationSlider> flt_sustain_time_1;
    ScopedPointer<mono_ModulationSlider> flt_sustain_1;
    ScopedPointer<mono_ModulationSlider> flt_decay_1;
    ScopedPointer<mono_ModulationSlider> flt_attack_1;
    ScopedPointer<Label> label_monolisa;
    ScopedPointer<TextButton> filter_type_1_1;
    ScopedPointer<TextButton> filter_type_2_1;
    ScopedPointer<TextButton> filter_type_3_1;
    ScopedPointer<TextButton> filter_type_1_2;
    ScopedPointer<TextButton> filter_type_2_2;
    ScopedPointer<TextButton> filter_type_3_2;
    ScopedPointer<TextButton> filter_type_1_3;
    ScopedPointer<TextButton> filter_type_2_3;
    ScopedPointer<TextButton> filter_type_3_3;
    ScopedPointer<TextButton> filter_type_5_1;
    ScopedPointer<TextButton> filter_type_5_2;
    ScopedPointer<TextButton> filter_type_5_3;
    ScopedPointer<TextButton> button_sequence_2;
    ScopedPointer<TextButton> button_sequence_3;
    ScopedPointer<TextButton> button_sequence_4;
    ScopedPointer<TextButton> button_sequence_5;
    ScopedPointer<TextButton> button_sequence_6;
    ScopedPointer<TextButton> button_sequence_7;
    ScopedPointer<TextButton> button_sequence_8;
    ScopedPointer<TextButton> button_sequence_9;
    ScopedPointer<TextButton> button_sequence_10;
    ScopedPointer<TextButton> button_sequence_11;
    ScopedPointer<TextButton> button_sequence_12;
    ScopedPointer<TextButton> button_sequence_13;
    ScopedPointer<TextButton> button_sequence_14;
    ScopedPointer<TextButton> button_sequence_15;
    ScopedPointer<TextButton> button_sequence_16;
    ScopedPointer<ComboBox> combo_programm;
    ScopedPointer<TextButton> button_programm_left;
    ScopedPointer<TextButton> button_programm_right;
    ScopedPointer<TextButton> button_programm_replace;
    ScopedPointer<TextButton> button_programm_new;
    ScopedPointer<TextButton> button_open_oszi;
    ScopedPointer<TextButton> button_open_midi_io_settings;
    ScopedPointer<ComboBox> combo_bank;
    ScopedPointer<TextButton> button_programm_load;
    ScopedPointer<mono_ModulationSlider> osc_1;
    ScopedPointer<mono_ModulationSlider> osc_2;
    ScopedPointer<mono_ModulationSlider> osc_3;
    ScopedPointer<mono_ModulationSlider> lfo_1;
    ScopedPointer<mono_ModulationSlider> flt_cutoff_1;
    ScopedPointer<mono_ModulationSlider> lfo_2;
    ScopedPointer<mono_ModulationSlider> lfo_3;
    ScopedPointer<mono_ModulationSlider> flt_cutoff_2;
    ScopedPointer<mono_ModulationSlider> flt_cutoff_3;
    ScopedPointer<mono_ModulationSlider> flt_input_1;
    ScopedPointer<mono_ModulationSlider> flt_input_2;
    ScopedPointer<mono_ModulationSlider> flt_input_3;
    ScopedPointer<mono_ModulationSlider> flt_compressor_1;
    ScopedPointer<mono_ModulationSlider> flt_distortion_1;
    ScopedPointer<mono_ModulationSlider> flt_input_6;
    ScopedPointer<mono_ModulationSlider> flt_input_7;
    ScopedPointer<mono_ModulationSlider> flt_input_8;
    ScopedPointer<mono_ModulationSlider> flt_compressor_2;
    ScopedPointer<mono_ModulationSlider> flt_input_11;
    ScopedPointer<mono_ModulationSlider> flt_input_12;
    ScopedPointer<mono_ModulationSlider> flt_input_13;
    ScopedPointer<mono_ModulationSlider> flt_compressor_3;
    ScopedPointer<mono_ModulationSlider> flt_resonance_1;
    ScopedPointer<mono_ModulationSlider> flt_gain_1;
    ScopedPointer<mono_ModulationSlider> flt_resonance_2;
    ScopedPointer<mono_ModulationSlider> flt_gain_2;
    ScopedPointer<mono_ModulationSlider> flt_resonance_3;
    ScopedPointer<mono_ModulationSlider> flt_gain_3;
    ScopedPointer<mono_ModulationSlider> flt_volume_1;
    ScopedPointer<mono_ModulationSlider> flt_volume_2;
    ScopedPointer<mono_ModulationSlider> flt_volume_3;
    ScopedPointer<mono_ModulationSlider> adsr_lfo_mix;
    ScopedPointer<mono_ModulationSlider> lfo_opt_2;
    ScopedPointer<mono_ModulationSlider> lfo_opt_3;
    ScopedPointer<TextButton> button_sequence_1;
    ScopedPointer<mono_ModulationSlider> flt_release_4;
    ScopedPointer<mono_ModulationSlider> delay2;
    ScopedPointer<mono_ModulationSlider> volume;
    ScopedPointer<mono_ModulationSlider> flt_distortion_2;
    ScopedPointer<mono_ModulationSlider> flt_distortion_3;
    ScopedPointer<TextButton> button_arp_speed_XNORM;
    ScopedPointer<mono_ModulationSlider> flt_attack_5;
    ScopedPointer<mono_ModulationSlider> flt_attack_6;
    ScopedPointer<mono_ModulationSlider> osc_wave_1;
    ScopedPointer<mono_ModulationSlider> osc_wave_2;
    ScopedPointer<Slider> sl_morhp_mix;
    ScopedPointer<TextButton> button_programm_delete;
    ScopedPointer<TextButton> button_open_config;
    ScopedPointer<TextButton> filter_type_6_1;
    ScopedPointer<TextButton> filter_type_6_2;
    ScopedPointer<TextButton> filter_type_6_3;
    ScopedPointer<TextButton> button_midi_learn;
    ScopedPointer<TextButton> button_ctrl_toggle;
    ScopedPointer<mono_ModulationSlider> colour;
    ScopedPointer<mono_ModulationSlider> speed;
    ScopedPointer<TextButton> button_open_morph;
    ScopedPointer<TextButton> effect_finalizer_switch;
    ScopedPointer<Label> label_ui_headline2;
    ScopedPointer<Label> label_ui_headline3;
    ScopedPointer<Label> label_ui_headline5;
    ScopedPointer<Label> label_ui_headline6;
    ScopedPointer<TextButton> button_values_toggle;
    ScopedPointer<mono_ModulationSlider> reverb_width;
    ScopedPointer<mono_ModulationSlider> octave_offset;
    ScopedPointer<Label> label_ui_headline4;
    ScopedPointer<Label> label_ui_headline7;
    ScopedPointer<Label> label_ui_headline8;
    ScopedPointer<Label> label_ui_headline9;
    ScopedPointer<Label> label_ui_headline10;
    ScopedPointer<Label> label_ui_headline11;
    ScopedPointer<Label> label_ui_headline12;
    ScopedPointer<Label> label_ui_headline13;
    ScopedPointer<Label> label_shape;
    ScopedPointer<Label> label_reverb;
    ScopedPointer<Label> label_delay;
    ScopedPointer<Label> label_chorus;
    ScopedPointer<Label> label_fx_mix;
    ScopedPointer<Label> label_ui_headline19;
    ScopedPointer<Label> label_ui_headline20;
    ScopedPointer<Label> label_ui_headline21;
    ScopedPointer<Label> label_ui_headline22;
    ScopedPointer<Label> label_ui_headline23;
    ScopedPointer<Label> label_ui_headline24;
    ScopedPointer<Label> label_ui_headline25;
    ScopedPointer<SegmentedMeter> volume_master_meter;
    ScopedPointer<Label> label_eq;
    ScopedPointer<TextButton> button_open_config2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorSynthLite)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_C931A4A92150166E__
