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

#ifndef __JUCE_HEADER_5A0D36FB903763FA__
#define __JUCE_HEADER_5A0D36FB903763FA__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class AppInstanceStore;

class Monique_Ui_MainwindowPopup;
class Monique_Ui_MidiIO;
class Monique_Ui_MorphConfig;
class Monique_Ui_GlobalSettings;
class Monique_Ui_ENVPopup;

class MoniqueSynthesiserVoice;
class MoniqueSynthData;

class Monique_Ui_SegmentedMeter;
class MIDIControl;
class Monique_Ui_DualSlider;

class ENVData;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Mainwindow  : public AudioProcessorEditor,
                               public Monique_Ui_Refreshable,
                               public ButtonListener,
                               public ComboBoxListener,
                               public SliderListener
{
public:
    //==============================================================================
    Monique_Ui_Mainwindow ();
    ~Monique_Ui_Mainwindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    AppInstanceStore*const _app_instance_store;
    MoniqueSynthesiserVoice*voice;
    MoniqueSynthData* synth_data;

    ScopedPointer<Monique_Ui_MainwindowPopup> popup;
    bool last_ctrl_mode;
    bool is_in_help_mode;
    ScopedPointer<Monique_Ui_MidiIO> editor_midiio;
    ScopedPointer<Monique_Ui_MorphConfig> editor_morph;
    ScopedPointer<Monique_Ui_GlobalSettings> editor_global_settings;
    ScopedPointer<Monique_Ui_ENVPopup> env_popup;

    void refresh() noexcept override;
    void update_tooltip_handling( bool is_help_key_down_ ) noexcept;
    void show_current_voice_data();
    void show_programs_and_select();
    void show_ctrl_state();
    void show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_ );
    void close_all_subeditors();
    void open_env_popup( ENVData*const env_data_, Parameter*const sustain_, Button*const for_comp_, bool has_negative_sustain_ ) noexcept;
    void resize_subeditors();
    void open_midi_editor_if_closed() noexcept;
    void open_setup_editor_if_closed() noexcept;
    int flash_counter;
    void flash_midi_editor_button() noexcept;

    float last_shuffle;
    Array< TextButton* > sequence_buttons;
    void resize_sequence_buttons();
    void switch_finalizer_tab();
    void update_slider_handling();
    Array< Monique_Ui_DualSlider* > dual_sliders;
    //void update_slider_return_values();
    void update_size();

    Array<int> last_morpher_index;

    const float original_w;
    const float original_h;
    int last_bank;
    int last_programm;
    void sliderClicked (Slider*s_) override;

    bool is_ctrl_down;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    bool keyPressed (const KeyPress& key);
    bool keyStateChanged (const bool isKeyDown);
    void modifierKeysChanged (const ModifierKeys& modifiers);

    // Binary resources:
    static const char* empty_png;
    static const int empty_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    ScopedPointer< TooltipWindow > tooltipWindow;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Monique_Ui_DualSlider> distortion;
    ScopedPointer<Monique_Ui_DualSlider> reverb_dry;
    ScopedPointer<Monique_Ui_DualSlider> reverb_width;
    ScopedPointer<Monique_Ui_DualSlider> reverb_room;
    ScopedPointer<Monique_Ui_DualSlider> delay2;
    ScopedPointer<Monique_Ui_DualSlider> chorus_modulation;
    ScopedPointer<Monique_Ui_DualSlider> eq_2;
    ScopedPointer<Monique_Ui_DualSlider> eq_3;
    ScopedPointer<Monique_Ui_DualSlider> eq_4;
    ScopedPointer<Monique_Ui_DualSlider> eq_7;
    ScopedPointer<Monique_Ui_DualSlider> eq_6;
    ScopedPointer<Monique_Ui_DualSlider> eq_5;
    ScopedPointer<Monique_Ui_DualSlider> eq_1;
    ScopedPointer<Monique_Ui_DualSlider> bypass;
    ScopedPointer<Monique_Ui_DualSlider> colour;
    ScopedPointer<Label> label_effect_hider;
    ScopedPointer<Label> label_band_hz_2;
    ScopedPointer<Label> label_band_hz_3;
    ScopedPointer<Label> label_band_hz_4;
    ScopedPointer<Label> label_band_hz_5;
    ScopedPointer<Label> label_band_hz_6;
    ScopedPointer<Label> label_band_hz_7;
    ScopedPointer<Label> label_band_hz_1;
    ScopedPointer<Monique_Ui_DualSlider> speed_multi;
    ScopedPointer<Monique_Ui_DualSlider> morpher_4;
    ScopedPointer<Monique_Ui_DualSlider> morpher_3;
    ScopedPointer<Monique_Ui_DualSlider> morpher_2;
    ScopedPointer<Monique_Ui_DualSlider> morpher_1;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_3;
    ScopedPointer<MidiKeyboardComponent> keyboard;
    ScopedPointer<Monique_Ui_DualSlider> glide2;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_16;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_15;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_14;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_13;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_12;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_11;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_10;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_9;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_8;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_7;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_6;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_5;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_4;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_3;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_2;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_1;
    ScopedPointer<Monique_Ui_DualSlider> shuffle;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_1;
    ScopedPointer<Label> label_monolisa;
    ScopedPointer<TextButton> filter_type_2_1;
    ScopedPointer<TextButton> filter_type_3_1;
    ScopedPointer<TextButton> filter_type_2_2;
    ScopedPointer<TextButton> filter_type_3_2;
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
    ScopedPointer<Monique_Ui_DualSlider> osc_1;
    ScopedPointer<Monique_Ui_DualSlider> osc_2;
    ScopedPointer<Monique_Ui_DualSlider> osc_3;
    ScopedPointer<Monique_Ui_DualSlider> lfo_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_1;
    ScopedPointer<Monique_Ui_DualSlider> lfo_2;
    ScopedPointer<Monique_Ui_DualSlider> lfo_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_6;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_7;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_8;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_11;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_12;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_13;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_gain_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_gain_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_gain_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_3;
    ScopedPointer<Monique_Ui_DualSlider> adsr_lfo_mix;
    ScopedPointer<Monique_Ui_DualSlider> lfo_opt_2;
    ScopedPointer<Monique_Ui_DualSlider> lfo_opt_3;
    ScopedPointer<TextButton> button_sequence_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_4;
    ScopedPointer<Monique_Ui_DualSlider> volume;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_3;
    ScopedPointer<TextButton> button_arp_speed_XNORM;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_5;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_6;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_1;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_2;
    ScopedPointer<Slider> sl_morhp_mix;
    ScopedPointer<TextButton> button_programm_delete;
    ScopedPointer<TextButton> filter_type_6_1;
    ScopedPointer<TextButton> filter_type_6_2;
    ScopedPointer<TextButton> filter_type_6_3;
    ScopedPointer<TextButton> button_ctrl_toggle;
    ScopedPointer<Monique_Ui_DualSlider> speed;
    ScopedPointer<TextButton> button_open_morph;
    ScopedPointer<TextButton> effect_finalizer_switch;
    ScopedPointer<Label> label_ui_headline2;
    ScopedPointer<Label> label_ui_headline3;
    ScopedPointer<Label> label_ui_headline5;
    ScopedPointer<Label> label_ui_headline6;
    ScopedPointer<TextButton> button_values_toggle;
    ScopedPointer<Monique_Ui_DualSlider> octave_offset;
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
    ScopedPointer<Monique_Ui_SegmentedMeter> volume_master_meter;
    ScopedPointer<Label> label_eq;
    ScopedPointer<TextButton> button_open_config2;
    ScopedPointer<Label> label_ui_headline14;
    ScopedPointer<TextButton> button_edit_input_env_1_1;
    ScopedPointer<TextButton> button_edit_input_env_1_2;
    ScopedPointer<TextButton> button_edit_input_env_1_3;
    ScopedPointer<TextButton> button_edit_input_env_2_1;
    ScopedPointer<TextButton> button_edit_input_env_2_2;
    ScopedPointer<TextButton> button_edit_input_env_2_3;
    ScopedPointer<TextButton> button_edit_input_env_3_1;
    ScopedPointer<TextButton> button_edit_input_env_3_2;
    ScopedPointer<TextButton> button_edit_input_env_3_3;
    ScopedPointer<TextButton> button_edit_input_env_band_1;
    ScopedPointer<TextButton> button_edit_input_env_band_2;
    ScopedPointer<TextButton> button_edit_input_env_band_3;
    ScopedPointer<TextButton> button_edit_input_env_band_4;
    ScopedPointer<TextButton> button_edit_input_env_band_5;
    ScopedPointer<TextButton> button_edit_input_env_band_6;
    ScopedPointer<TextButton> button_edit_input_env_band_7;
    ScopedPointer<TextButton> button_edit_env_chorus;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_1;
    ScopedPointer<TextButton> button_reset_arp_tune;
    ScopedPointer<TextButton> button_show_active_input_r_2_3;
    ScopedPointer<TextButton> button_show_active_input_l_2_3;
    ScopedPointer<TextButton> button_show_active_input_r_2_2;
    ScopedPointer<TextButton> button_show_active_input_l_2_2;
    ScopedPointer<TextButton> button_show_active_input_r_2_1;
    ScopedPointer<TextButton> button_show_active_input_l_2_1;
    ScopedPointer<TextButton> button_show_active_input_r_3_3;
    ScopedPointer<TextButton> button_show_active_input_l_3_3;
    ScopedPointer<TextButton> button_show_active_input_r_3_2;
    ScopedPointer<TextButton> button_show_active_input_l_3_2;
    ScopedPointer<TextButton> button_show_active_input_r_3_1;
    ScopedPointer<TextButton> button_show_active_input_l_3_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_Mainwindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_5A0D36FB903763FA__
