/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
*/

#ifndef __JUCE_HEADER_B12ED69DCBAC8838__
#define __JUCE_HEADER_B12ED69DCBAC8838__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class Monique_Ui_Mainwindow;
class Monique_Ui_DualSlider;
struct LFOData;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MFOPopup : public juce::Component,
                            public Monique_Ui_Refreshable,
                            public juce::DropShadower,
                            public juce::Timer,
                            public ParameterListener,
                            public juce::Slider::Listener,
                            public juce::Button::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MFOPopup(Monique_Ui_Refresher *ui_refresher_, Monique_Ui_Mainwindow *const parent_,
                        LFOData *const mfo_data_, COLOUR_THEMES theme_);
    ~Monique_Ui_MFOPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    COLOUR_THEMES theme;
    bool is_repainting;

    juce::Component *related_to_comp;
    Monique_Ui_DualSlider *owner_slider;
    void set_element_to_show(juce::Component *const, Monique_Ui_DualSlider *owner_);
    void update_positions();
    void refresh() noexcept override;
    void timerCallback() override;
    int callbacks;

    const LFOData *const is_open_for() const noexcept { return mfo_data; }
    void set_clickable_components(juce::Array<juce::Component *> &comps_) noexcept;

    const float original_w;
    const float original_h;

  private:
    float last_wave;
    float last_speed;
    float last_offset;

    juce::Array<float> curve;
    Monique_Ui_Mainwindow *const parent;
    LFOData *const mfo_data;
    juce::Array<juce::Component *> observed_comps;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;
    void mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &) override;
    void mouseMagnify(const juce::MouseEvent &event, float) override;

    void parameter_value_changed(Parameter *param_) noexcept override;
    void sliderClicked(juce::Slider *s_) /*override*/;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> slider_wave;
    std::unique_ptr<juce::Label> label_shape2;
    std::unique_ptr<juce::Label> label_shape;
    std::unique_ptr<juce::Slider> slider_speed;
    std::unique_ptr<juce::Component> plotter;
    std::unique_ptr<juce::TextButton> close;
    std::unique_ptr<juce::TextButton> keep;
    std::unique_ptr<juce::TextButton> auto_close;
    std::unique_ptr<juce::TextButton> copy;
    std::unique_ptr<juce::TextButton> past;
    std::unique_ptr<juce::Label> label_shape3;
    std::unique_ptr<juce::Slider> slider_offset;
    std::unique_ptr<juce::TextButton> mfo_minus;
    std::unique_ptr<juce::TextButton> mfo_plus;
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MFOPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_B12ED69DCBAC8838__
