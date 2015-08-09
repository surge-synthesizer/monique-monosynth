/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_D0A211EF20981AF4__
#define __JUCE_HEADER_D0A211EF20981AF4__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
#include "SynthData.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorSettings  : public Component,
                          public SliderListener,
                          public ButtonListener
{
public:
    //==============================================================================
    UiEditorSettings ();
    ~UiEditorSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void refresh();
    bool repaint_curve;
    bool stop_current_painting;
    Array< float > curve;

    const float original_w;
    const float original_h;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
    bool keyPressed (const KeyPress& key);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_21;
    ScopedPointer<Label> label_22;
    ScopedPointer<Component> plotter;
    ScopedPointer<Label> label_1;
    ScopedPointer<Slider> slider_attack_1;
    ScopedPointer<Label> label_ui_headline4;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label_2;
    ScopedPointer<Slider> slider_decay_1;
    ScopedPointer<Label> label_3;
    ScopedPointer<Slider> slider_release_1;
    ScopedPointer<Label> label_4;
    ScopedPointer<Slider> slider_attack_2;
    ScopedPointer<Label> label_5;
    ScopedPointer<Slider> slider_decay_2;
    ScopedPointer<Label> label_6;
    ScopedPointer<Slider> slider_release_2;
    ScopedPointer<Label> label_7;
    ScopedPointer<Slider> slider_attack_3;
    ScopedPointer<Label> label_8;
    ScopedPointer<Slider> slider_decay_3;
    ScopedPointer<Label> label_9;
    ScopedPointer<Slider> slider_release_3;
    ScopedPointer<Label> label_10;
    ScopedPointer<Slider> slider_attack_4;
    ScopedPointer<Label> label_11;
    ScopedPointer<Slider> slider_decay_4;
    ScopedPointer<Label> label_12;
    ScopedPointer<Slider> slider_release_4;
    ScopedPointer<Label> label6;
    ScopedPointer<Slider> slider_test;
    ScopedPointer<Label> label_13;
    ScopedPointer<Slider> slider_test_attack;
    ScopedPointer<Label> label_14;
    ScopedPointer<Slider> slider_test_decay;
    ScopedPointer<Label> label_15;
    ScopedPointer<Slider> slider_test_release;
    ScopedPointer<Slider> slider_max_adr_attack;
    ScopedPointer<Slider> slider_max_adr_decay;
    ScopedPointer<Slider> slider_max_adr_release;
    ScopedPointer<Label> label5;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> label4;
    ScopedPointer<Label> label_ui_headline2;
    ScopedPointer<Slider> slider_morph_motor_time;
    ScopedPointer<Label> label10;
    ScopedPointer<Slider> slider_glide_time;
    ScopedPointer<Label> label11;
    ScopedPointer<Label> label12;
    ScopedPointer<Label> label13;
    ScopedPointer<TextButton> close;
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<Label> label7;
    ScopedPointer<Label> label_17;
    ScopedPointer<Label> label_16;
    ScopedPointer<ToggleButton> toggle_animate_input_env;
    ScopedPointer<Label> label_18;
    ScopedPointer<ToggleButton> toggle_animate_eq_env;
    ScopedPointer<Label> label_19;
    ScopedPointer<ToggleButton> toggle_output_cc_mute4;
    ScopedPointer<Slider> slider_sustain_time_1;
    ScopedPointer<Slider> slider_sustain_time_2;
    ScopedPointer<Slider> slider_sustain_time_3;
    ScopedPointer<Slider> slider_sustain_time_4;
    ScopedPointer<Label> label_20;
    ScopedPointer<Label> label_23;
    ScopedPointer<Slider> slider_test_suatain_time;
    ScopedPointer<Label> label_24;
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
    ScopedPointer<Label> label8;
    Path internalPath1;
    Path internalPath2;
    Path internalPath3;
    Path internalPath4;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_D0A211EF20981AF4__
