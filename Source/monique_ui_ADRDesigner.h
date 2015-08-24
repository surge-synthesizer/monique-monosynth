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

#ifndef __JUCE_HEADER_8EB35BC3F04D7C4__
#define __JUCE_HEADER_8EB35BC3F04D7C4__

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
class UiEditorSettings  : public Component,
                          public mono_UiRefreshable,
                          public SliderListener
{
public:
    //==============================================================================
    UiEditorSettings ();
    ~UiEditorSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;
    
private:
    int repaint_counter;
    int last_repaint_counter;
    
    void refresh() noexcept override;
    Array< float > curve;

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_ui_headline_4;
    ScopedPointer<Label> label_ui_headline_3;
    ScopedPointer<Label> label_ui_headline_2;
    ScopedPointer<Label> label_ui_headline_1;
    ScopedPointer<Label> label_sus_3;
    ScopedPointer<Label> label_sus_2;
    ScopedPointer<Component> plotter;
    ScopedPointer<Label> label_att_1;
    ScopedPointer<Slider> slider_attack_1;
    ScopedPointer<Label> label_dec_1;
    ScopedPointer<Slider> slider_decay_1;
    ScopedPointer<Label> label_rel_1;
    ScopedPointer<Slider> slider_release_1;
    ScopedPointer<Label> label_att_2;
    ScopedPointer<Slider> slider_attack_2;
    ScopedPointer<Label> label_dec_2;
    ScopedPointer<Slider> slider_decay_2;
    ScopedPointer<Label> label_rel_2;
    ScopedPointer<Slider> slider_release_2;
    ScopedPointer<Label> label_att_3;
    ScopedPointer<Slider> slider_attack_3;
    ScopedPointer<Label> label_dec_3;
    ScopedPointer<Slider> slider_decay_3;
    ScopedPointer<Label> label_rel_3;
    ScopedPointer<Slider> slider_release_3;
    ScopedPointer<Label> label_att_4;
    ScopedPointer<Slider> slider_attack_4;
    ScopedPointer<Label> label_dec_4;
    ScopedPointer<Slider> slider_decay_4;
    ScopedPointer<Label> label_rel_4;
    ScopedPointer<Slider> slider_release_4;
    ScopedPointer<Label> label6;
    ScopedPointer<Slider> slider_test;
    ScopedPointer<Slider> slider_sustain_time_1;
    ScopedPointer<Slider> slider_sustain_time_2;
    ScopedPointer<Slider> slider_sustain_time_3;
    ScopedPointer<Slider> slider_sustain_time_4;
    ScopedPointer<Label> label_sus_4;
    ScopedPointer<Label> label_sus_1;
    ScopedPointer<Label> label_attack_1;
    ScopedPointer<Label> label_decay_1;
    ScopedPointer<Label> label_sustain_time_1;
    ScopedPointer<Label> label_release_1;
    ScopedPointer<Label> label_attack_2;
    ScopedPointer<Label> label_decay_2;
    ScopedPointer<Label> label_sustain_time_2;
    ScopedPointer<Label> label_release_2;
    ScopedPointer<Label> label_attack_3;
    ScopedPointer<Label> label_decay_3;
    ScopedPointer<Label> label_sustain_time_3;
    ScopedPointer<Label> label_release_3;
    ScopedPointer<Label> label_attack_4;
    ScopedPointer<Label> label_decay_4;
    ScopedPointer<Label> label_sustain_time_4;
    ScopedPointer<Label> label_release_4;
    ScopedPointer<Slider> slider_env_shape;
    ScopedPointer<Label> label_shape;
    ScopedPointer<Label> label_test_value;
    ScopedPointer<Label> label_ui_headline_5;
    ScopedPointer<Label> label3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8EB35BC3F04D7C4__
