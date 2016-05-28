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
#include "App_h_includer.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Playback  : public Component,
                             public Monique_Ui_Refreshable,
                             public ButtonListener,
                             public SliderListener
{
public:
    //==============================================================================
    Monique_Ui_Playback (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow* parent_);
    ~Monique_Ui_Playback();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Monique_Ui_Mainwindow* parent;

    void refresh() noexcept override;

    const float original_w;
    const float original_h;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> button_preset_agro;
    ScopedPointer<TextButton> button_preset_down;
    ScopedPointer<TextButton> button_preset_rising;
    ScopedPointer<TextButton> button_preset_soft;
    ScopedPointer<TextButton> button_flt_out_triggering_3;
    ScopedPointer<TextButton> button_flt_out_triggering_2;
    ScopedPointer<TextButton> button_flt_out_triggering_1;
    ScopedPointer<TextButton> button_flt_env_triggering_3;
    ScopedPointer<TextButton> button_flt_env_triggering_2;
    ScopedPointer<TextButton> button_flt_env_triggering_1;
    ScopedPointer<TextButton> button_flt_input_triggering_3;
    ScopedPointer<TextButton> button_flt_input_triggering_2;
    ScopedPointer<TextButton> button_flt_input_triggering_1;
    ScopedPointer<TextButton> filter_type_bg_button_1;
    ScopedPointer<TextButton> button_tracking_mode_hm;
    ScopedPointer<TextButton> button_tracking_mode_keep;
    ScopedPointer<TextButton> button_tracking_mode_lf;
    ScopedPointer<TextButton> button_tracking_mode_hf;
    ScopedPointer<Slider> slider_cutoff_tracking_oct_3;
    ScopedPointer<Slider> slider_cutoff_tracking_oct_2;
    ScopedPointer<Slider> slider_cutoff_tracking_oct_1;
    ScopedPointer<Slider> slider_osc_tracking_oct_3;
    ScopedPointer<Slider> slider_osc_tracking_oct_2;
    ScopedPointer<TextButton> button_cutoff_tracking_3;
    ScopedPointer<TextButton> button_cutoff_tracking_2;
    ScopedPointer<TextButton> button_cutoff_tracking_1;
    ScopedPointer<TextButton> button_osc_tracking_3;
    ScopedPointer<TextButton> button_osc_tracking_2;
    ScopedPointer<TextButton> button_osc_tracking_1;
    ScopedPointer<Label> label_2;
    ScopedPointer<Label> label_4;
    ScopedPointer<Label> label_5;
    ScopedPointer<Label> label_6;
    ScopedPointer<Label> label_7;
    ScopedPointer<Label> label_8;
    ScopedPointer<Label> label_9;
    ScopedPointer<Label> label_10;
    ScopedPointer<TextButton> button_osc_key_tracking_highest2;
    ScopedPointer<Label> label_12;
    ScopedPointer<Label> label_13;
    ScopedPointer<Slider> slider_flt_out_sesitivity_1;
    ScopedPointer<Slider> slider_flt_out_sesitivity_2;
    ScopedPointer<Slider> slider_flt_out_sesitivity_3;
    ScopedPointer<Label> label_21;
    ScopedPointer<Label> label_22;
    ScopedPointer<Label> label_23;
    ScopedPointer<Label> label_24;
    ScopedPointer<TextButton> close;
    ScopedPointer<Label> label_3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_Playback)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_65FB963CF91AD20E__
