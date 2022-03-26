/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_5A0D36FB903763FA__
#define __JUCE_HEADER_5A0D36FB903763FA__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
#include <juce_audio_utils/juce_audio_utils.h>

class Monique_Ui_Refresher;

class Monique_Ui_MainwindowPopup;
class Monique_Ui_MidiIO;
class Monique_Ui_MorphConfig;
class Monique_Ui_GlobalSettings;
class Monique_Ui_ENVPopup;
class Monique_Ui_MFOPopup;
class Monique_Ui_OptionPopup;
class Monique_Ui_AmpPainter;
class Monique_Ui_Playback;

class MoniqueSynthesiserVoice;
struct MoniqueSynthData;

class Monique_Ui_SegmentedMeter;
class MIDIControl;
class Monique_Ui_DualSlider;

struct ModulationSliderConfigBase;

class monique_ui_Overlay;
class monique_ui_Credits;

struct ENVData;
struct LFOData;
enum COLOUR_THEMES;

class Monique_Ui_Mainwindow;
struct CreditsPoper : public juce::Component
{
    Monique_Ui_Mainwindow *const parent;
    const bool force;
    void mouseEnter(const juce::MouseEvent &e_) override;
    void mouseExit(const juce::MouseEvent &e_) override;

  public:
    CreditsPoper(Monique_Ui_Mainwindow *parent_, bool force_ = false)
        : parent(parent_), force(force_)
    {
    }
};
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Mainwindow : public juce::AudioProcessorEditor,
                              public Monique_Ui_Refreshable,
                              public juce::AsyncUpdater,
                              public ParameterListener,
                              public juce::Button::Listener,
                              public juce::ComboBox::Listener,
                              public juce::Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_Mainwindow(Monique_Ui_Refresher *ui_refresher_);
    ~Monique_Ui_Mainwindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int last_refreshed_note;

    MoniqueAudioProcessor *audio_processor;
    MoniqueSynthesiserVoice *voice;

#ifdef JUCE_OPENGL
    OpenGLContext openGLContext;
#endif
    juce::StringArray getRenderingEngines() const;
    int getActiveRenderingEngine() const;
    void setRenderingEngine(int index);
    void setOpenGLRenderingEngine();

    std::unique_ptr<Monique_Ui_MainwindowPopup> popup;
    bool is_in_help_mode;
    std::unique_ptr<Monique_Ui_MidiIO> editor_midiio;
    std::unique_ptr<Monique_Ui_MorphConfig> editor_morph;
    std::unique_ptr<Monique_Ui_GlobalSettings> editor_global_settings;
    std::unique_ptr<Monique_Ui_ENVPopup> env_popup;
    std::unique_ptr<Monique_Ui_MFOPopup> mfo_popup;
    std::unique_ptr<Monique_Ui_OptionPopup> option_popup;
    std::unique_ptr<monique_ui_Credits> credits;
    std::unique_ptr<Monique_Ui_Playback> playback;
    Monique_Ui_AmpPainter *amp_painter;

    void refresh() noexcept override;
    void update_tooltip_handling(bool is_help_key_down_) noexcept;
    void show_current_voice_data();
    void show_current_poly_data();
    void handleAsyncUpdate() override;
    void show_programs_and_select(bool force);
    void toggle_modulation_slider_top_button(juce::Button *button_, bool by_force_) noexcept;
    void clear_record_buffer() noexcept;
    void stop_clear_chorus() noexcept;
    void update_slider_return_values() noexcept;
    void restore_slider_value(juce::Component *slider_,
                              int type_ /* 0 = user, 1 = factory, 2, program */) noexcept;
    void global_slider_settings_changed(juce::Component *parent_) noexcept;
    bool last_shift_state;
    void show_ctrl_state();
    Parameter *find_parameter_in_dual_sliders(const juce::Component *comp_) noexcept;
    Parameter *find_back_parameter_in_dual_sliders(const juce::Component *comp_) noexcept;
    void show_info_popup(juce::Component *comp_, MIDIControl *midi_conrtrol_,
                         bool force_turn_on_ = false);
    void close_all_subeditors();
    void open_env_popup(ENVData *const env_data_, Parameter *const sustain_,
                        juce::Button *const for_comp_, Monique_Ui_DualSlider *slider_,
                        bool has_negative_sustain_) noexcept;
    void open_env_popup(Monique_Ui_DualSlider *dual_slider_) noexcept;
    void open_mfo_popup(LFOData *const mfo_data_, juce::Button *const for_comp_,
                        Monique_Ui_DualSlider *slider_, COLOUR_THEMES theme_) noexcept;
    void open_mfo_popup(Monique_Ui_DualSlider *dual_slider_) noexcept;
    void open_option_popup(juce::Component *const for_comp_, BoolParameter *param_a_,
                           BoolParameter *param_b_, BoolParameter *param_arp_or_seq_,
                           juce::StringRef text_a_, juce::StringRef text_b_,
                           juce::StringRef tool_tip_a_, juce::StringRef tool_tip_b_) noexcept;
    bool handle_keep_env_open(ModulationSliderConfigBase *const caller_config_) noexcept;
    bool handle_keep_mfo_open(ModulationSliderConfigBase *const caller_config_) noexcept;
    void resize_subeditors();
    void open_midi_editor_if_closed() noexcept;
    int flash_counter;
    void flash_midi_editor_button() noexcept;

    float last_shuffle;
    juce::Array<Monique_Ui_DualSlider *> sequence_sliders;
    juce::Array<juce::TextButton *> sequence_buttons;
    juce::Array<juce::TextButton *> sequence_buttons_original_order;
    juce::CriticalSection resize_lock;
    void resize_sequence_buttons(bool force_ = false);
    void switch_finalizer_tab(bool fx_);
    juce::Array<Monique_Ui_DualSlider *> dual_sliders;
    void update_size();

    juce::Array<int> last_morpher_index;

    float original_w;
    float original_h;
    int last_bank;
    int last_programm;
    enum EDIT_TYPES
    {
        LOAD,
        REPLACE,
        CREATE,
        RENAME,
        REMOVE,
        NOT_SET
    };
    EDIT_TYPES program_edit_type;

    void mouseEnter(const juce::MouseEvent &event) override;

    std::unique_ptr<juce::Timer> clear_record_timer;
    std::unique_ptr<juce::Timer> button_flasher;

    void open_env_or_lfo_popup_by_midi(Parameter *param_) noexcept;
    int last_env_popup_open;
    int last_lfo_popup_open;
    int last_step_offset;
    int last_fine_offset;
    juce::Array<juce::Point<int>> original_slider_positions;

    void show_overlay() noexcept;
    void show_credits(bool force_) noexcept;
    void hide_credits() noexcept;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;
    bool keyPressed(const juce::KeyPress &key) override;
    bool keyStateChanged(const bool isKeyDown) override;
    void modifierKeysChanged(const juce::ModifierKeys &modifiers) override;

    // Binary resources:
    static const char *empty_png;
    static const int empty_pngSize;
    static const char *_01hintergrundalles_svg;
    static const int _01hintergrundalles_svgSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    std::unique_ptr<juce::ResizableCornerComponent> resizer;
    juce::ComponentBoundsConstrainer resizeLimits;
    std::unique_ptr<juce::TooltipWindow> tooltipWindow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> filter_type_bg_button_5;
    std::unique_ptr<juce::TextButton> filter_type_bg_button_4;
    std::unique_ptr<monique_ui_Overlay> overlay;
    std::unique_ptr<juce::Label> label_monique;
    std::unique_ptr<CreditsPoper> pop_credits;
    std::unique_ptr<juce::Label> label_fx_delay;
    std::unique_ptr<Monique_Ui_DualSlider> eq_7;
    std::unique_ptr<Monique_Ui_DualSlider> eq_6;
    std::unique_ptr<Monique_Ui_DualSlider> eq_5;
    std::unique_ptr<Monique_Ui_DualSlider> eq_4;
    std::unique_ptr<Monique_Ui_DualSlider> eq_3;
    std::unique_ptr<Monique_Ui_DualSlider> eq_2;
    std::unique_ptr<Monique_Ui_DualSlider> eq_1;
    std::unique_ptr<Monique_Ui_DualSlider> distortion;
    std::unique_ptr<Monique_Ui_DualSlider> chorus_modulation;
    std::unique_ptr<Monique_Ui_DualSlider> delay3;
    std::unique_ptr<Monique_Ui_DualSlider> delay2;
    std::unique_ptr<Monique_Ui_DualSlider> delay4;
    std::unique_ptr<Monique_Ui_DualSlider> reverb_room;
    std::unique_ptr<Monique_Ui_DualSlider> reverb_dry;
    std::unique_ptr<juce::Label> label_lfo_3;
    std::unique_ptr<juce::Label> label_lfo_2;
    std::unique_ptr<juce::Label> label_lfo_1;
    std::unique_ptr<Monique_Ui_DualSlider> lfo_1;
    std::unique_ptr<Monique_Ui_DualSlider> lfo_2;
    std::unique_ptr<Monique_Ui_DualSlider> lfo_3;
    std::unique_ptr<Monique_Ui_DualSlider> morpher_1;
    std::unique_ptr<Monique_Ui_DualSlider> morpher_2;
    std::unique_ptr<Monique_Ui_DualSlider> morpher_4;
    std::unique_ptr<Monique_Ui_DualSlider> morpher_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_13;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_12;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_11;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_6;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_7;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_8;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_input_1;
    std::unique_ptr<juce::TextButton> button_edit_lfo_1;
    std::unique_ptr<juce::TextButton> button_edit_lfo_2;
    std::unique_ptr<juce::TextButton> button_edit_lfo_3;
    std::unique_ptr<juce::TextButton> button_edit_input_env_3_3;
    std::unique_ptr<juce::TextButton> button_edit_input_env_3_2;
    std::unique_ptr<juce::TextButton> button_edit_input_env_3_1;
    std::unique_ptr<juce::TextButton> button_edit_input_env_2_1;
    std::unique_ptr<juce::TextButton> button_edit_input_env_2_2;
    std::unique_ptr<juce::TextButton> button_edit_input_env_2_3;
    std::unique_ptr<juce::TextButton> button_edit_input_env_1_3;
    std::unique_ptr<juce::TextButton> button_edit_input_env_1_2;
    std::unique_ptr<juce::TextButton> button_edit_input_env_1_1;
    std::unique_ptr<juce::TextButton> button_edit_mfo_4;
    std::unique_ptr<juce::TextButton> button_edit_mfo_3;
    std::unique_ptr<juce::TextButton> button_edit_mfo_2;
    std::unique_ptr<juce::TextButton> button_edit_mfo_1;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_1;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_2;
    std::unique_ptr<juce::TextButton> effect_finalizer_switch2;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_7;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_3;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_4;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_5;
    std::unique_ptr<juce::TextButton> button_edit_input_env_band_6;
    std::unique_ptr<juce::TextButton> filter_type_bg_button_3;
    std::unique_ptr<juce::TextButton> filter_type_2_3;
    std::unique_ptr<juce::TextButton> filter_type_bg_button_2;
    std::unique_ptr<juce::TextButton> filter_type_2_2;
    std::unique_ptr<juce::TextButton> filter_type_bg_button_1;
    std::unique_ptr<juce::TextButton> filter_type_2_1;
    std::unique_ptr<juce::TextButton> button_toggle_morph_buttons_1;
    std::unique_ptr<juce::TextButton> button_toggle_morph_buttons_2;
    std::unique_ptr<juce::TextButton> button_toggle_morph_buttons_3;
    std::unique_ptr<juce::TextButton> button_toggle_morph_buttons_4;
    std::unique_ptr<juce::Label> label_band_hz_5;
    std::unique_ptr<juce::Label> label_band_hz_6;
    std::unique_ptr<juce::Label> label_band_hz_4;
    std::unique_ptr<juce::Label> label_band_hz_1;
    std::unique_ptr<juce::Label> label_morph;
    std::unique_ptr<juce::Label> label_band_hz_7;
    std::unique_ptr<juce::Label> label_arpeggiator;
    std::unique_ptr<juce::TextButton> button_programm_replace;
    std::unique_ptr<juce::TextButton> button_programm_new;
    std::unique_ptr<Monique_Ui_DualSlider> bypass;
    std::unique_ptr<Monique_Ui_DualSlider> colour;
    std::unique_ptr<juce::Label> label_band_hz_2;
    std::unique_ptr<juce::Label> label_band_hz_3;
    std::unique_ptr<Monique_Ui_DualSlider> speed_multi;
    std::unique_ptr<Monique_Ui_DualSlider> osc_wave_3;
    std::unique_ptr<juce::MidiKeyboardComponent> keyboard;
    std::unique_ptr<Monique_Ui_DualSlider> glide2;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_16;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_15;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_14;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_13;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_12;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_11;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_10;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_9;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_8;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_7;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_6;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_5;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_4;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_3;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_2;
    std::unique_ptr<Monique_Ui_DualSlider> arp_step_1;
    std::unique_ptr<Monique_Ui_DualSlider> shuffle;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_4;
    std::unique_ptr<Monique_Ui_DualSlider> flt_decay_4;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_4;
    std::unique_ptr<Monique_Ui_DualSlider> flt_release_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_time_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_decay_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_release_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_time_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_decay_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_release_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_time_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_sustain_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_decay_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_1;
    std::unique_ptr<juce::TextButton> filter_type_3_1;
    std::unique_ptr<juce::TextButton> filter_type_3_2;
    std::unique_ptr<juce::TextButton> filter_type_3_3;
    std::unique_ptr<juce::TextButton> filter_type_5_1;
    std::unique_ptr<juce::TextButton> filter_type_5_2;
    std::unique_ptr<juce::TextButton> filter_type_5_3;
    std::unique_ptr<juce::TextButton> button_sequence_2;
    std::unique_ptr<juce::TextButton> button_sequence_3;
    std::unique_ptr<juce::TextButton> button_sequence_4;
    std::unique_ptr<juce::TextButton> button_sequence_5;
    std::unique_ptr<juce::TextButton> button_sequence_6;
    std::unique_ptr<juce::TextButton> button_sequence_7;
    std::unique_ptr<juce::TextButton> button_sequence_8;
    std::unique_ptr<juce::TextButton> button_sequence_9;
    std::unique_ptr<juce::TextButton> button_sequence_10;
    std::unique_ptr<juce::TextButton> button_sequence_11;
    std::unique_ptr<juce::TextButton> button_sequence_12;
    std::unique_ptr<juce::TextButton> button_sequence_13;
    std::unique_ptr<juce::TextButton> button_sequence_14;
    std::unique_ptr<juce::TextButton> button_sequence_15;
    std::unique_ptr<juce::TextButton> button_sequence_16;
    std::unique_ptr<juce::ComboBox> combo_programm;
    std::unique_ptr<juce::TextButton> button_programm_left;
    std::unique_ptr<juce::TextButton> button_programm_right;
    std::unique_ptr<juce::TextButton> button_open_oszi;
    std::unique_ptr<juce::TextButton> button_open_midi_io_settings;
    std::unique_ptr<juce::ComboBox> combo_bank;
    std::unique_ptr<juce::TextButton> button_programm_load;
    std::unique_ptr<Monique_Ui_DualSlider> osc_1;
    std::unique_ptr<Monique_Ui_DualSlider> osc_2;
    std::unique_ptr<Monique_Ui_DualSlider> osc_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_cutoff_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_cutoff_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_cutoff_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_distortion_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_resonance_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_resonance_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_resonance_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_volume_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_volume_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_volume_3;
    std::unique_ptr<Monique_Ui_DualSlider> adsr_lfo_mix;
    std::unique_ptr<Monique_Ui_DualSlider> lfo_opt_2;
    std::unique_ptr<Monique_Ui_DualSlider> lfo_opt_3;
    std::unique_ptr<juce::TextButton> button_sequence_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_release_4;
    std::unique_ptr<Monique_Ui_DualSlider> volume;
    std::unique_ptr<Monique_Ui_DualSlider> flt_distortion_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_distortion_3;
    std::unique_ptr<juce::TextButton> button_arp_speed_XNORM;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_5;
    std::unique_ptr<Monique_Ui_DualSlider> flt_attack_6;
    std::unique_ptr<Monique_Ui_DualSlider> osc_wave_1;
    std::unique_ptr<Monique_Ui_DualSlider> osc_wave_2;
    std::unique_ptr<juce::TextButton> button_programm_delete;
    std::unique_ptr<juce::TextButton> filter_type_6_1;
    std::unique_ptr<juce::TextButton> filter_type_6_2;
    std::unique_ptr<juce::TextButton> filter_type_6_3;
    std::unique_ptr<juce::TextButton> button_ctrl_toggle;
    std::unique_ptr<Monique_Ui_DualSlider> speed;
    std::unique_ptr<juce::TextButton> button_open_morph;
    std::unique_ptr<juce::TextButton> effect_finalizer_switch;
    std::unique_ptr<juce::TextButton> button_values_toggle;
    std::unique_ptr<Monique_Ui_DualSlider> octave_offset;
    std::unique_ptr<juce::Label> label_filter_inputs;
    std::unique_ptr<juce::Label> label_oscillators;
    std::unique_ptr<juce::Label> label_filter_envelope;
    std::unique_ptr<juce::Label> label_lfo;
    std::unique_ptr<juce::Label> label_filter_config;
    std::unique_ptr<juce::Label> label_filter_fx;
    std::unique_ptr<juce::Label> label_out;
    std::unique_ptr<juce::Label> label_amp_envelope;
    std::unique_ptr<juce::Label> label_glide;
    std::unique_ptr<juce::Label> label_speed;
    std::unique_ptr<juce::Label> label_tune;
    std::unique_ptr<juce::Label> label_fm;
    std::unique_ptr<Monique_Ui_SegmentedMeter> volume_master_meter;
    std::unique_ptr<juce::TextButton> button_open_config2;
    std::unique_ptr<juce::Label> label_mod_mix;
    std::unique_ptr<Monique_Ui_DualSlider> flt_pan_3;
    std::unique_ptr<Monique_Ui_DualSlider> flt_pan_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_pan_1;
    std::unique_ptr<juce::TextButton> button_reset_arp_tune;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_2_3;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_2_3;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_2_2;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_2_2;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_2_1;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_2_1;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_3_3;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_3_3;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_3_2;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_3_2;
    std::unique_ptr<juce::TextButton> button_show_active_input_r_3_1;
    std::unique_ptr<juce::TextButton> button_show_active_input_l_3_1;
    std::unique_ptr<juce::TextButton> button_programm_rename;
    std::unique_ptr<Monique_Ui_DualSlider> flt_shape_1;
    std::unique_ptr<Monique_Ui_DualSlider> flt_shape_2;
    std::unique_ptr<Monique_Ui_DualSlider> flt_shape_3;
    std::unique_ptr<juce::TextButton> button_programm_scratch;
    std::unique_ptr<Monique_Ui_DualSlider> flt_shape_4;
    std::unique_ptr<juce::Label> label_reverb;
    std::unique_ptr<juce::TextButton> button_open_playback;
    std::unique_ptr<juce::TextButton> button_preset_agro;
    std::unique_ptr<juce::TextButton> button_tracking_mode_hm;
    std::unique_ptr<juce::TextButton> button_preset_down;
    std::unique_ptr<juce::TextButton> button_tracking_mode_lf;
    std::unique_ptr<juce::TextButton> button_preset_rising;
    std::unique_ptr<juce::TextButton> button_tracking_mode_hf;
    std::unique_ptr<juce::TextButton> button_preset_soft;
    std::unique_ptr<juce::TextButton> button_tracking_mode_keep;
    std::unique_ptr<juce::Label> label_2;
    std::unique_ptr<juce::Label> label_24;
    std::unique_ptr<juce::Label> label_23;
    std::unique_ptr<juce::Label> label_22;
    std::unique_ptr<juce::Label> label_21;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_3;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_2;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_1;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_1;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_2;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_3;
    std::unique_ptr<juce::Label> label_13;
    std::unique_ptr<juce::Label> label_7;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_3;
    std::unique_ptr<juce::Slider> slider_osc_tracking_oct_3;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_3;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_3_1;
    std::unique_ptr<juce::TextButton> button_osc_tracking_3;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_3;
    std::unique_ptr<juce::Label> label_12;
    std::unique_ptr<juce::Label> label_6;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_1_1;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_2_1;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_1;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_2;
    std::unique_ptr<juce::Slider> slider_osc_tracking_oct_2;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_2;
    std::unique_ptr<juce::TextButton> button_osc_tracking_2;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_2;
    std::unique_ptr<juce::Label> label_5;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_1;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_1;
    std::unique_ptr<juce::TextButton> button_osc_tracking_1;
    std::unique_ptr<juce::Label> label_oscillators2;
    std::unique_ptr<juce::Label> label_sub_poly;
    std::unique_ptr<juce::Label> label_poly_desc_1;
    std::unique_ptr<juce::Label> label_poly_desc_2;
    std::unique_ptr<juce::Label> label_poly_desc_3;
    std::unique_ptr<juce::Label> label_poly_desc_4;
    std::unique_ptr<juce::Label> label_poly_desc_5;
    std::unique_ptr<juce::Label> label_poly_desc_6;
    std::unique_ptr<juce::Label> label_poly_desc_7;
    std::unique_ptr<juce::Label> label_poly_desc_8;
    std::unique_ptr<juce::Label> label_poly_desc_9;
    std::unique_ptr<juce::Label> label_poly_desc_10;
    std::unique_ptr<juce::Label> label_poly_desc_11;
    std::unique_ptr<juce::Label> label_poly_desc_12;
    std::unique_ptr<juce::Label> label_poly_desc_13;
    std::unique_ptr<juce::Label> label_poly_desc_14;
    std::unique_ptr<juce::Label> label_poly_desc_15;
    std::unique_ptr<juce::Label> label_poly_desc_16;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_1_2;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_1_3;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_2_2;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_2_3;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_3_2;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_3_3;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Mainwindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_5A0D36FB903763FA__
