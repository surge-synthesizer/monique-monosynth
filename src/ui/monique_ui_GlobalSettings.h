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
#include <juce_gui_extra/juce_gui_extra.h>

struct SectionTheme;
class Monique_Ui_Mainwindow;
struct CreditsPoper;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_GlobalSettings : public juce::Component,
                                  public Monique_Ui_Refreshable,
                                  public juce::ComboBox::Listener,
                                  public juce::Button::Listener,
                                  public juce::Label::Listener
{
  public:
    //==============================================================================
    Monique_Ui_GlobalSettings(Monique_Ui_Refresher *ui_refresher_, Monique_Ui_Mainwindow *parent_);
    ~Monique_Ui_GlobalSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    SectionTheme *editable_theme;
    Monique_Ui_Mainwindow *parent;

    bool block_colour_update;
    void refresh() noexcept override;

    const float original_w;
    const float original_h;

    std::uint32_t colour_clipboard;

  private:
    //==============================================================================
    // COLOURS
    class CPUUsageSmoother
    {
        enum
        {
            BUFFER_SIZE = 34
        };

        int pos;
        float buffer[BUFFER_SIZE];
        float sum;

      public:
        inline void add(float val_) noexcept
        {
            sum -= buffer[pos];
            buffer[pos] = val_;
            sum += val_;

            ++pos;
            if (pos == BUFFER_SIZE)
                pos = 0;
        }
        inline int get_average() const noexcept { return round0(sum / BUFFER_SIZE); }
        inline int add_and_get_average(float val_) noexcept
        {
            add(val_);
            return round0(sum / BUFFER_SIZE);
        }

        CPUUsageSmoother() : pos(0), sum(0)
        {
            for (int i = 0; i != BUFFER_SIZE; ++i)
                buffer[i] = 0;
        }
    } cpu_usage_smoother;

    bool force_repaint;
    juce::Colour *current_edited_colour;
    juce::Colour last_repainted_colour;
    COLOUR_CODES current_colour;
    void open_colour_selector(COLOUR_CODES code_);

    //==============================================================================
    void update_colour_presets();
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void labelTextChanged(juce::Label *labelThatHasChanged) override;

    // Binary resources:
    static const char *vst_logo_100x_png;
    static const int vst_logo_100x_pngSize;
    static const char *au_logo_100x_png;
    static const int au_logo_100x_pngSize;
    static const char *aax_logo_100x_png;
    static const int aax_logo_100x_pngSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label_ui_headline_9;
    std::unique_ptr<juce::Label> label_ui_headline_3;
    std::unique_ptr<juce::Label> label_ui_headline_7;
    std::unique_ptr<juce::ImageButton> image_vst;
    std::unique_ptr<juce::TextButton> button_colour_bg;
    std::unique_ptr<juce::TextButton> button_colour_background;
    std::unique_ptr<juce::Label> label_buttons__;
    std::unique_ptr<juce::Label> label_slider__;
    std::unique_ptr<juce::Label> label_section__;
    std::unique_ptr<juce::Label> label_2;
    std::unique_ptr<juce::Label> label_cpu_usage;
    std::unique_ptr<juce::Label> label_16;
    std::unique_ptr<juce::ToggleButton> toggle_animate_input_env;
    std::unique_ptr<juce::Label> label_18;
    std::unique_ptr<juce::ToggleButton> toggle_show_tooltips;
    std::unique_ptr<juce::Label> label_ui_headline_2;
    std::unique_ptr<juce::TextButton> button_colour_buttons_on;
    std::unique_ptr<juce::TextButton> button_colour_slider_1;
    std::unique_ptr<juce::TextButton> button_colour_slider_2;
    std::unique_ptr<juce::TextButton> button_colour_slider_mod;
    std::unique_ptr<juce::TextButton> button_colour_buttons_font_on;
    std::unique_ptr<juce::TextButton> button_colour_labels;
    std::unique_ptr<juce::Label> label_ui_headline_6;
    std::unique_ptr<juce::TextButton> button_replace_preset;
    std::unique_ptr<juce::TextButton> button_save_as_preset;
    std::unique_ptr<juce::Label> label_6;
    std::unique_ptr<juce::ToggleButton> toggle_animate_sliders;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_1;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_2;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_3;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_4;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_5;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_6;
    std::unique_ptr<juce::TextButton> button_colour_bg_svg_7;
    std::unique_ptr<juce::TextButton> copy;
    std::unique_ptr<juce::TextButton> past;
    std::unique_ptr<juce::TextButton> button_colour_buttons_off;
    std::unique_ptr<juce::TextButton> button_colour_buttons_font_off;
    std::unique_ptr<juce::TextButton> button_colour_slider_bg;
    std::unique_ptr<juce::TextButton> button_remove_preset;
    std::unique_ptr<juce::Label> label_11;
    std::unique_ptr<juce::ToggleButton> toggle_for_all;
    std::unique_ptr<juce::TextButton> copy2;
    std::unique_ptr<juce::TextButton> past2;
    std::unique_ptr<juce::TextButton> button_colour_slider_disabled;
    std::unique_ptr<juce::Label> label_section;
    std::unique_ptr<juce::Label> label_colour;
    std::unique_ptr<juce::ColourSelector> colour_selector;
    std::unique_ptr<juce::Label> label_colour2;
    std::unique_ptr<juce::Label> label_section2;
    std::unique_ptr<juce::ComboBox> combo_theme;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::TextButton> button_colour_oszi_1;
    std::unique_ptr<juce::TextButton> button_colour_oszi_2;
    std::unique_ptr<juce::TextButton> button_colour_oszi_3;
    std::unique_ptr<juce::TextButton> selected_section_marker;
    std::unique_ptr<juce::TextButton> selected_element_marker;
    std::unique_ptr<CreditsPoper> credits_poper;
    std::unique_ptr<juce::TextButton> close;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_GlobalSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_896740BF6EEE43F0__
