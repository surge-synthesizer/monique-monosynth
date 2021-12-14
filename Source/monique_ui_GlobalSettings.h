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

#ifndef __JUCE_HEADER_896740BF6EEE43F0__
#define __JUCE_HEADER_896740BF6EEE43F0__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class SectionTheme;
class Monique_Ui_Mainwindow;
class CreditsPoper;
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
								   public ComboBox::Listener,
								   public Button::Listener,
								   public Label::Listener
{
public:
    //==============================================================================
    Monique_Ui_GlobalSettings (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*parent_ );
    ~Monique_Ui_GlobalSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    SectionTheme* editable_theme;
    Monique_Ui_Mainwindow* parent;

    bool block_colour_update;
    void refresh() noexcept override;

    const float original_w;
    const float original_h;

    uint32 colour_clipboard;

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

    bool force_repaint;
    Colour* current_edited_colour;
    Colour last_repainted_colour;
    COLOUR_CODES current_colour;
    void open_colour_selector( COLOUR_CODES code_ );

    //==============================================================================
    void update_colour_presets();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void labelTextChanged (Label* labelThatHasChanged) override;

    // Binary resources:
    static const char* vst_logo_100x_png;
    static const int vst_logo_100x_pngSize;
    static const char* au_logo_100x_png;
    static const int au_logo_100x_pngSize;
    static const char* aax_logo_100x_png;
    static const int aax_logo_100x_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_ui_headline_9;
    ScopedPointer<Label> label_ui_headline_3;
    ScopedPointer<Label> label_ui_headline_7;
    ScopedPointer<ImageButton> image_vst;
    ScopedPointer<TextButton> button_colour_bg;
    ScopedPointer<TextButton> button_colour_background;
    ScopedPointer<Label> label_buttons__;
    ScopedPointer<Label> label_slider__;
    ScopedPointer<Label> label_section__;
    ScopedPointer<Label> label_2;
    ScopedPointer<Label> label_cpu_usage;
    ScopedPointer<Label> label_16;
    ScopedPointer<ToggleButton> toggle_animate_input_env;
    ScopedPointer<Label> label_18;
    ScopedPointer<ToggleButton> toggle_show_tooltips;
    ScopedPointer<Label> label_ui_headline_2;
    ScopedPointer<TextButton> button_colour_buttons_on;
    ScopedPointer<TextButton> button_colour_slider_1;
    ScopedPointer<TextButton> button_colour_slider_2;
    ScopedPointer<TextButton> button_colour_slider_mod;
    ScopedPointer<TextButton> button_colour_buttons_font_on;
    ScopedPointer<TextButton> button_colour_labels;
    ScopedPointer<Label> label_ui_headline_6;
    ScopedPointer<TextButton> button_replace_preset;
    ScopedPointer<TextButton> button_save_as_preset;
    ScopedPointer<Label> label_6;
    ScopedPointer<ToggleButton> toggle_animate_sliders;
    ScopedPointer<TextButton> button_colour_bg_svg_1;
    ScopedPointer<TextButton> button_colour_bg_svg_2;
    ScopedPointer<TextButton> button_colour_bg_svg_3;
    ScopedPointer<TextButton> button_colour_bg_svg_4;
    ScopedPointer<TextButton> button_colour_bg_svg_5;
    ScopedPointer<TextButton> button_colour_bg_svg_6;
    ScopedPointer<TextButton> button_colour_bg_svg_7;
    ScopedPointer<TextButton> copy;
    ScopedPointer<TextButton> past;
    ScopedPointer<TextButton> button_colour_buttons_off;
    ScopedPointer<TextButton> button_colour_buttons_font_off;
    ScopedPointer<TextButton> button_colour_slider_bg;
    ScopedPointer<TextButton> button_remove_preset;
    ScopedPointer<Label> label_11;
    ScopedPointer<ToggleButton> toggle_for_all;
    ScopedPointer<TextButton> copy2;
    ScopedPointer<TextButton> past2;
    ScopedPointer<TextButton> button_colour_slider_disabled;
    ScopedPointer<Label> label_section;
    ScopedPointer<Label> label_colour;
    ScopedPointer<ColourSelector> colour_selector;
    ScopedPointer<Label> label_colour2;
    ScopedPointer<Label> label_section2;
    ScopedPointer<ComboBox> combo_theme;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> button_colour_oszi_1;
    ScopedPointer<TextButton> button_colour_oszi_2;
    ScopedPointer<TextButton> button_colour_oszi_3;
    ScopedPointer<TextButton> selected_section_marker;
    ScopedPointer<TextButton> selected_element_marker;
    ScopedPointer<CreditsPoper> credits_poper;
    ScopedPointer<TextButton> close;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_GlobalSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_896740BF6EEE43F0__
