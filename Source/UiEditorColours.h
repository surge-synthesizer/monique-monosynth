/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_1EE5576E0584BFD0__
#define __JUCE_HEADER_1EE5576E0584BFD0__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"
class ComponentColours;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorColourStyler  : public Component,
    public Timer,
    public ButtonListener
{
public:
    //==============================================================================
    UiEditorColourStyler (ComponentColours* const colours_);
    ~UiEditorColourStyler();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    ComponentColours*const _colours;
    Colour* editable_colour;

    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ColourSelector> colour_selector;
    ScopedPointer<TextButton> col_1;
    ScopedPointer<TextButton> style_1;
    ScopedPointer<TextButton> col_2;
    ScopedPointer<TextButton> col_3;
    ScopedPointer<TextButton> col_4;
    ScopedPointer<TextButton> col_5;
    ScopedPointer<TextButton> col_6;
    ScopedPointer<TextButton> col_7;
    ScopedPointer<TextButton> col_8;
    ScopedPointer<TextButton> col_9;
    ScopedPointer<TextButton> col_10;
    ScopedPointer<TextButton> style_2;
    ScopedPointer<TextButton> style_3;
    ScopedPointer<TextButton> style_4;
    ScopedPointer<TextButton> style_5;
    ScopedPointer<TextButton> col_11;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorColourStyler)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1EE5576E0584BFD0__
