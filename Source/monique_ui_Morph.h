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

#ifndef __JUCE_HEADER_65BEB9C97C372EA3__
#define __JUCE_HEADER_65BEB9C97C372EA3__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class UiDragPad;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorMorph  : public Component,
                       public ComboBoxListener,
                       public ButtonListener
{
public:
    //==============================================================================
    UiEditorMorph ();
    ~UiEditorMorph();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;

private:
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Array< int > bank_start_indexs;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_31;
    ScopedPointer<Label> label_32;
    ScopedPointer<Label> label_33;
    ScopedPointer<Label> label_30;
    ScopedPointer<Label> label_34;
    ScopedPointer<ComboBox> combo_morph_group_1;
    ScopedPointer<ComboBox> combo_morph_group_2;
    ScopedPointer<ComboBox> combo_morph_group_3;
    ScopedPointer<ComboBox> combo_morph_group_4;
    ScopedPointer<ComboBox> combo_morph_group_5;
    ScopedPointer<ComboBox> combo_morph_group_6;
    ScopedPointer<ComboBox> combo_morph_group_7;
    ScopedPointer<ComboBox> combo_morph_group_8;
    ScopedPointer<Label> label_19;
    ScopedPointer<TextButton> button_set_left_1;
    ScopedPointer<TextButton> button_set_left_2;
    ScopedPointer<TextButton> button_set_left_3;
    ScopedPointer<TextButton> button_set_left_4;
    ScopedPointer<TextButton> button_set_right_1;
    ScopedPointer<TextButton> button_set_right_2;
    ScopedPointer<TextButton> button_set_right_3;
    ScopedPointer<TextButton> button_set_right_4;
    ScopedPointer<TextButton> button_set_all_from_current;
    ScopedPointer<Label> label_ui_headline_1;
    ScopedPointer<Label> label_ui_headline_2;
    ScopedPointer<Label> label_ui_headline_3;
    ScopedPointer<Label> label_ui_headline_4;
    ScopedPointer<Label> label_4;
    ScopedPointer<Label> label_5;
    ScopedPointer<Label> label_25;
    ScopedPointer<Label> label_26;
    ScopedPointer<Label> label_27;
    ScopedPointer<Label> label_28;
    ScopedPointer<Label> label_29;
    ScopedPointer<Label> label_ui_headline_5;
    ScopedPointer<Label> label_35;
    ScopedPointer<UiDragPad> drag_pad;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorMorph)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_65BEB9C97C372EA3__
