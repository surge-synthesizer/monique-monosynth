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

#ifndef __JUCE_HEADER_896740BF6EEE43F0__
#define __JUCE_HEADER_896740BF6EEE43F0__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_GlobalSettings  : public Component,
                                   public Monique_Ui_Refreshable,
                                   public ComboBoxListener,
                                   public ButtonListener,
                                   public SliderListener
{
public:
    //==============================================================================
    Monique_Ui_GlobalSettings ();
    ~Monique_Ui_GlobalSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void refresh() noexcept override;

    const float original_w;
    const float original_h;

private:
    //==============================================================================
    // COLOURS
    class CPUUsageSmoother
    {
        enum { BUFFER_SIZE = 34 };

        int pos;
        float buffer[BUFFER_SIZE];
        float sum;

    public:
        inline void add( float val_ ) noexcept
        {
            sum-=buffer[pos];
            buffer[pos] = val_;
            sum+=val_;

            ++pos;
            if( pos == BUFFER_SIZE )
                pos = 0;
        }
        inline int get_average() const noexcept
        {
            return round0(sum / BUFFER_SIZE);
        }
        inline int add_and_get_average(float val_) noexcept
        {
            add( val_ );
            return round0(sum / BUFFER_SIZE);
        }

        CPUUsageSmoother() : pos(0), sum(0) {
            for( int i = 0 ; i != BUFFER_SIZE ; ++i )
                buffer[i] = 0;
        }
    } cpu_usage_smoother;

    Colour* current_edited_colour;
    Colour last_repainted_colour;
    void open_colour_selector( Colour& colour_to_edit_ );

    //==============================================================================
    // AUDIO
    void update_audio_devices();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);

    // Binary resources:
    static const char* vst_logo_100x_png;
    static const int vst_logo_100x_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_9;
    ScopedPointer<Label> label_2;
    ScopedPointer<Label> label_4;
    ScopedPointer<ComboBox> combo_multicore_cpus;
    ScopedPointer<Label> label_cpu_usage;
    ScopedPointer<ComboBox> combo_block_size;
    ScopedPointer<Label> label_10;
    ScopedPointer<ComboBox> combo_audio_driver;
    ScopedPointer<ComboBox> combo_audio_device;
    ScopedPointer<Label> label_7;
    ScopedPointer<Label> label_8;
    ScopedPointer<ComboBox> combo_sample_rate;
    ScopedPointer<ColourSelector> colour_selector;
    ScopedPointer<Label> label_colour_4;
    ScopedPointer<TextButton> button_current_editor_color;
    ScopedPointer<Label> label_colour_7;
    ScopedPointer<Label> label_colour_9;
    ScopedPointer<Label> label_colour_6;
    ScopedPointer<Label> label_colour_5;
    ScopedPointer<Slider> slider_morph_motor_time;
    ScopedPointer<Slider> slider_glide_time;
    ScopedPointer<Label> label13;
    ScopedPointer<Label> label7;
    ScopedPointer<Label> label_16;
    ScopedPointer<ToggleButton> toggle_animate_input_env;
    ScopedPointer<Label> label_18;
    ScopedPointer<ToggleButton> toggle_show_tooltips;
    ScopedPointer<Label> label_ui_headline_1;
    ScopedPointer<Label> label_ui_headline_2;
    ScopedPointer<Label> label_ui_headline_4;
    ScopedPointer<Slider> slider_sensitivity;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label_3;
    ScopedPointer<Label> label_5;
    ScopedPointer<ToggleButton> toggle_slider_linear;
    ScopedPointer<ToggleButton> toggle_slider_rotary;
    ScopedPointer<Label> label_ui_headline_5;
    ScopedPointer<Label> label_colour_1;
    ScopedPointer<Label> label_colour_8;
    ScopedPointer<Label> label_colour_2;
    ScopedPointer<TextButton> button_colour_bg;
    ScopedPointer<TextButton> button_colour_border;
    ScopedPointer<TextButton> button_colour_buttons_on;
    ScopedPointer<TextButton> button_colour_slider_1;
    ScopedPointer<TextButton> button_colour_slider_2;
    ScopedPointer<TextButton> button_colour_slider_mod;
    ScopedPointer<Label> label_colour_3;
    ScopedPointer<TextButton> button_colour_signal_lines;
    ScopedPointer<TextButton> button_colour_buttons_off;
    ScopedPointer<TextButton> button_colour_labels;
    ScopedPointer<Label> label_ui_headline_6;
    ScopedPointer<TextButton> button_preset_1;
    ScopedPointer<TextButton> button_preset_2;
    ScopedPointer<TextButton> button_preset_3;
    ScopedPointer<Label> label_6;
    ScopedPointer<ToggleButton> toggle_one_pedal;
    ScopedPointer<Label> label_ui_headline_3;
    ScopedPointer<Label> label_ui_headline_7;
    ScopedPointer<ImageButton> image_vst;
    ScopedPointer<Label> label_ui_headline_9;
    ScopedPointer<HyperlinkButton> link_to_monoplugs;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_GlobalSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_896740BF6EEE43F0__
