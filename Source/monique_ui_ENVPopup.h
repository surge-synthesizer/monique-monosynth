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

#ifndef __JUCE_HEADER_685271DE805B685A__
#define __JUCE_HEADER_685271DE805B685A__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class Monique_Ui_Mainwindow;
class Monique_Ui_DualSlider;
struct ENVData;

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_ENVPopup : public juce::Component,
                            public Monique_Ui_Refreshable,
                            public juce::DropShadower,
                            public juce::Timer,
                            public ParameterListener,
                            public juce::Slider::Listener,
                            public juce::Label::Listener,
                            public juce::Button::Listener
{
  public:
    //==============================================================================
    Monique_Ui_ENVPopup(Monique_Ui_Refresher *ui_refresher_, Monique_Ui_Mainwindow *const parent_,
                        ENVData *const env_data_, Parameter *const sustain_, bool left_,
                        bool has_negative_sustain_);
    ~Monique_Ui_ENVPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    juce::Component *related_to_comp;
    Monique_Ui_DualSlider *owner_slider;
    void set_element_to_show(juce::Component *const, Monique_Ui_DualSlider *owner_);
    void update_positions();
    void refresh() noexcept override;
    void timerCallback() override;
    int callbacks;

    COLOUR_THEMES theme;

    const ENVData *const is_open_for() const noexcept { return env_data; }
    void set_clickable_components(juce::Array<juce::Component *> &comps_) noexcept;

    const float original_w;
    const float original_h;

  private:
    bool is_repainting;
    const bool left;
    juce::Array<float> curve;
    Monique_Ui_Mainwindow *const parent;
    ENVData *const env_data;
    Parameter *const sustain;
    juce::Array<juce::Component *> observed_comps;

    float last_attack;
    float last_sustain;
    float last_decay;
    float last_release;
    float last_shape;
    float sustain_time;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;
    void mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &) override;
    void mouseMagnify(const juce::MouseEvent &event, float) override;

    void parameter_value_changed(Parameter *param_) noexcept override;
    void sliderClicked(juce::Slider *s_) /*override */;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;
    void labelTextChanged(juce::Label *labelThatHasChanged) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label_attack_bottom;
    std::unique_ptr<juce::Slider> slider_attack;
    std::unique_ptr<juce::Label> label_decay_bottom;
    std::unique_ptr<juce::Slider> slider_decay;
    std::unique_ptr<juce::Label> label_release_bottom;
    std::unique_ptr<juce::Slider> slider_release;
    std::unique_ptr<juce::Slider> slider_sustain_time;
    std::unique_ptr<juce::Label> label_sustain_time_bottom;
    std::unique_ptr<juce::Label> label_attack;
    std::unique_ptr<juce::Label> label_decay;
    std::unique_ptr<juce::Label> label_sustain_time;
    std::unique_ptr<juce::Label> label_release;
    std::unique_ptr<juce::Slider> slider_sustain;
    std::unique_ptr<juce::Label> label_sustain_bottom;
    std::unique_ptr<juce::Label> label_sustain;
    std::unique_ptr<juce::Component> plotter;
    std::unique_ptr<juce::Slider> slider_env_shape;
    std::unique_ptr<juce::Label> label_shape;
    std::unique_ptr<juce::TextButton> close;
    std::unique_ptr<juce::TextButton> keep;
    std::unique_ptr<juce::TextButton> auto_close;
    std::unique_ptr<juce::TextButton> copy;
    std::unique_ptr<juce::TextButton> past;
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_ENVPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_685271DE805B685A__
