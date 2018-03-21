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

#ifndef __JUCE_HEADER_4BD3D86F1072C2BE__
#define __JUCE_HEADER_4BD3D86F1072C2BE__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class Monique_Ui_Mainwindow;
class MIDIControl;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_OptionPopup  : 
	public Component,
	public Monique_Ui_Refreshable,
	public DropShadower,
	public Button::Listener
{
public:
    //==============================================================================
    Monique_Ui_OptionPopup (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, BoolParameter* param_a_, BoolParameter* param_b_);
    ~Monique_Ui_OptionPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Component* related_to_comp;
    BoolParameter*const param_a;
    BoolParameter*const param_b;
    void set_element_to_show(Component*const);
    void update_positions();
    void refresh() noexcept override;

    void set_infos( StringRef text_a, StringRef text_b, StringRef tool_a, StringRef tool_b );

    const float original_w;
    const float original_h;

private:
    Monique_Ui_Mainwindow*const parent;
    void mouseExit (const MouseEvent& event) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> button_option_a;
    ScopedPointer<TextButton> button_option_b;
    Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_OptionPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_4BD3D86F1072C2BE__
