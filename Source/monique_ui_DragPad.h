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

#ifndef __JUCE_HEADER_A9EA3B45608EED62__
#define __JUCE_HEADER_A9EA3B45608EED62__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class DragPad;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_DragPad : public juce::Component
{
  public:
    //==============================================================================
    Monique_Ui_DragPad(Monique_Ui_Refresher *ui_refresher_);
    ~Monique_Ui_DragPad();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;

    Monique_Ui_Refresher *const ui_refresher;
    UiLookAndFeel *look_and_feel;

    void set_left_to_right_states(float left2right_state_, float top2bottom_state_);

  private:
    //[/UserMethods]

    void paint(juce::Graphics &g);
    void resized();

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    float left2right_state;
    float top2bottom_state;
    juce::Point<int> current_position;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<DragPad> track_area;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_DragPad)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_A9EA3B45608EED62__
