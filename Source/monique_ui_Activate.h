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

#ifndef __JUCE_HEADER_FCD4CD5F67BB3EE2__
#define __JUCE_HEADER_FCD4CD5F67BB3EE2__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
class Monique_Ui_Mainwindow;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class monique_ui_Activate  : public Component,
                             public DropShadower,
                             public FileDragAndDropTarget,
                             public ButtonListener
{
public:
    //==============================================================================
    monique_ui_Activate (Monique_Ui_Mainwindow*parent_);
    ~monique_ui_Activate();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    ScopedPointer< LookAndFeel > activation_look_and_feel;
    Monique_Ui_Mainwindow*parent;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    struct OverlayComp;
    struct OfflineActivation;
    friend struct OverlayComp;
    friend struct OfflineActivation;
    Component::SafePointer<Component> overlay;
    OfflineActivation* overlayOffline;
    ScopedPointer<BubbleMessageComponent> bubble;

    void close() noexcept;
    String get_state() noexcept;

    void showBubbleMessage (const String&, Component&);

    bool isInterestedInFileDrag (const StringArray& files) override;
    void fileDragEnter (const StringArray& files, int x, int y) override;
    void fileDragExit (const StringArray& files ) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> offline_text;
    ScopedPointer<Label> label_offline_header;
    ScopedPointer<TextButton> button_activate_online;
    ScopedPointer<Label> label_header;
    ScopedPointer<Label> label_sub_header;
    ScopedPointer<Label> label5;
    ScopedPointer<TextButton> button_activate_offline;
    ScopedPointer<TextButton> button_buy;
    ScopedPointer<TextButton> button_evaluate;
    ScopedPointer<Label> label2;
    ScopedPointer<TextEditor> text_pw;
    ScopedPointer<TextEditor> text_mail;
    ScopedPointer<TextButton> button_export;
    ScopedPointer<TextButton> button_import;
    ScopedPointer<TextButton> button_cancel;
    ScopedPointer<Label> label_header2;
    ScopedPointer<Label> label_sub_header2;
    ScopedPointer<Label> label_sub_header3;
    ScopedPointer<HyperlinkButton> hyperlinkButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (monique_ui_Activate)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_FCD4CD5F67BB3EE2__
