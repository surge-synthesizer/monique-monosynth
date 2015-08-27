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

#ifndef __JUCE_HEADER_BE41B261562DCF2__
#define __JUCE_HEADER_BE41B261562DCF2__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class UiEditorSynthLite;
class MIDIControl;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorSynthLitePopup  : public Component,
                                public ComboBoxListener,
                                public ButtonListener
{
public:
    //==============================================================================
    UiEditorSynthLitePopup (UiEditorSynthLite*const parent_, MIDIControl* midi_control_);
    ~UiEditorSynthLitePopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Component* related_to_comp;
    void set_element_to_show(Component*const);
    void update_positions();
    void refresh();

    const float original_w;
    const float original_h;

private:
    UiEditorSynthLite*const parent;
    MIDIControl* _midi_control;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> combo_midi_listen_type;
    ScopedPointer<ComboBox> combo_midi_number;
    ScopedPointer<TextButton> close;
    ScopedPointer<ComboBox> combo_midi_channel;
    Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorSynthLitePopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_BE41B261562DCF2__
