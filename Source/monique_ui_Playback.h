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

#ifndef __JUCE_HEADER_65FB963CF91AD20E__
#define __JUCE_HEADER_65FB963CF91AD20E__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Playback : public Component,
                            public Monique_Ui_Refreshable,
                            public Button::Listener,
                            public Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_Playback(Monique_Ui_Refresher *ui_refresher_, Monique_Ui_Mainwindow *parent_);
    ~Monique_Ui_Playback();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Monique_Ui_Mainwindow *parent;

    void refresh() noexcept override;

    const float original_w;
    const float original_h;
    //[/UserMethods]

    void paint(Graphics &g) override;
    void resized() override;
    void buttonClicked(Button *buttonThatWasClicked) override;
    void sliderValueChanged(Slider *sliderThatWasMoved) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> button_preset_agro;
    std::unique_ptr<TextButton> button_preset_down;
    std::unique_ptr<TextButton> button_preset_rising;
    std::unique_ptr<TextButton> button_preset_soft;
    std::unique_ptr<TextButton> button_flt_out_triggering_3;
    std::unique_ptr<TextButton> button_flt_out_triggering_2;
    std::unique_ptr<TextButton> button_flt_out_triggering_1;
    std::unique_ptr<TextButton> button_flt_env_triggering_3;
    std::unique_ptr<TextButton> button_flt_env_triggering_2;
    std::unique_ptr<TextButton> button_flt_env_triggering_1;
    std::unique_ptr<TextButton> button_flt_input_triggering_3;
    std::unique_ptr<TextButton> button_flt_input_triggering_2;
    std::unique_ptr<TextButton> button_flt_input_triggering_1;
    std::unique_ptr<TextButton> filter_type_bg_button_1;
    std::unique_ptr<TextButton> button_tracking_mode_hm;
    std::unique_ptr<TextButton> button_tracking_mode_keep;
    std::unique_ptr<TextButton> button_tracking_mode_lf;
    std::unique_ptr<TextButton> button_tracking_mode_hf;
    std::unique_ptr<Slider> slider_cutoff_tracking_oct_3;
    std::unique_ptr<Slider> slider_cutoff_tracking_oct_2;
    std::unique_ptr<Slider> slider_cutoff_tracking_oct_1;
    std::unique_ptr<Slider> slider_osc_tracking_oct_3;
    std::unique_ptr<Slider> slider_osc_tracking_oct_2;
    std::unique_ptr<TextButton> button_cutoff_tracking_3;
    std::unique_ptr<TextButton> button_cutoff_tracking_2;
    std::unique_ptr<TextButton> button_cutoff_tracking_1;
    std::unique_ptr<TextButton> button_osc_tracking_3;
    std::unique_ptr<TextButton> button_osc_tracking_2;
    std::unique_ptr<TextButton> button_osc_tracking_1;
    std::unique_ptr<Label> label_2;
    std::unique_ptr<Label> label_4;
    std::unique_ptr<Label> label_5;
    std::unique_ptr<Label> label_6;
    std::unique_ptr<Label> label_7;
    std::unique_ptr<Label> label_8;
    std::unique_ptr<Label> label_9;
    std::unique_ptr<Label> label_10;
    std::unique_ptr<TextButton> button_osc_key_tracking_highest2;
    std::unique_ptr<Label> label_12;
    std::unique_ptr<Label> label_13;
    std::unique_ptr<Slider> slider_flt_out_sesitivity_1;
    std::unique_ptr<Slider> slider_flt_out_sesitivity_2;
    std::unique_ptr<Slider> slider_flt_out_sesitivity_3;
    std::unique_ptr<Label> label_21;
    std::unique_ptr<Label> label_22;
    std::unique_ptr<Label> label_23;
    std::unique_ptr<Label> label_24;
    std::unique_ptr<TextButton> close;
    std::unique_ptr<Label> label_3;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Playback)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_65FB963CF91AD20E__
