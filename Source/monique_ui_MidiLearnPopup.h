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

#ifndef __JUCE_HEADER_4AEB729D0E305906__
#define __JUCE_HEADER_4AEB729D0E305906__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

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
class Monique_Ui_MainwindowPopup : public Component,
                                   public Monique_Ui_Refreshable,
                                   public DropShadower,
                                   public ComboBox::Listener,
                                   public Button::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MainwindowPopup(Monique_Ui_Refresher *ui_refresher_,
                               Monique_Ui_Mainwindow *const parent_, MIDIControl *midi_control_);
    ~Monique_Ui_MainwindowPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const Component *related_to_comp;
    void set_element_to_show(const Component *const);
    void update_positions();
    void refresh() noexcept override;

    const float original_w;
    const float original_h;

  private:
    Monique_Ui_Mainwindow *const parent;
    MIDIControl *_midi_control;
    //[/UserMethods]

    void paint(Graphics &g) override;
    void resized() override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> combo_midi_number;
    std::unique_ptr<TextButton> close;
    Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MainwindowPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_4AEB729D0E305906__
