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

#ifndef __JUCE_HEADER_3437163A265DCA8E__
#define __JUCE_HEADER_3437163A265DCA8E__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class mono_AudioDeviceManager;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MidiIO  : public Component,
                           public Monique_Ui_Refreshable,
                           public ComboBoxListener,
                           public ButtonListener,
                           public SliderListener
{
public:
    //==============================================================================
    Monique_Ui_MidiIO (mono_AudioDeviceManager*const audio_device_manager_);
    ~Monique_Ui_MidiIO();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int last_state_change;
    void refresh() noexcept override;
    void update_combo_boxed();

    const float original_w;
    const float original_h;

    bool has_grabbed_focus;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    mono_AudioDeviceManager* _audio_device_manager;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_7;
    ScopedPointer<ComboBox> combo_input_main;
    ScopedPointer<ComboBox> combo_input_main_channel;
    ScopedPointer<Label> label_3;
    ScopedPointer<ToggleButton> toggle_input_main_thru;
    ScopedPointer<Label> label_5;
    ScopedPointer<ComboBox> combo_output_thru;
    ScopedPointer<Label> label_6;
    ScopedPointer<ComboBox> combo_input_cc;
    ScopedPointer<ToggleButton> toggle_input_main_cc;
    ScopedPointer<ToggleButton> toggle_input_cc_thru;
    ScopedPointer<Label> label_9;
    ScopedPointer<ComboBox> combo_output_cc;
    ScopedPointer<Slider> slider_midi_pickup;
    ScopedPointer<Label> label_2;
    ScopedPointer<Label> label_4;
    ScopedPointer<TextButton> button_midi_learn;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_MidiIO)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_3437163A265DCA8E__
