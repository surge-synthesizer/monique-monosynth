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

#ifndef __JUCE_HEADER_F6D557DA58F99AC__
#define __JUCE_HEADER_F6D557DA58F99AC__

//[Headers]     -- You can add your own extra header files here --

#include "App.h"
#ifdef DEMO
#include "UIEditorToolbar.h"

class AppInstanceStore;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorDemo  : public UiEditor,
                      public ButtonListener
{
public:
    //==============================================================================
    UiEditorDemo (AppInstanceStore* const app_instance_store_);
    ~UiEditorDemo();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    AppInstanceStore*const _app_instance_store;
    void refresh_ui();
    String last_colour_theme;
private:
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* demobg_png2;
    static const int demobg_png2Size;
    static const char* democontinuehover_png;
    static const int democontinuehover_pngSize;
    static const char* democontinue_png;
    static const int democontinue_pngSize;
    static const char* demogoprohover_png;
    static const int demogoprohover_pngSize;
    static const char* demogopro_png;
    static const int demogopro_pngSize;
    static const char* demorestarthover_png;
    static const int demorestarthover_pngSize;
    static const char* demorestart_png;
    static const int demorestart_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> continue_;
    ScopedPointer<TextButton> restart;
    ScopedPointer<TextButton> buy;
    ScopedPointer<HyperlinkButton> video_1;
    ScopedPointer<HyperlinkButton> video_2;
    ScopedPointer<HyperlinkButton> video_3;
    ScopedPointer<HyperlinkButton> video_4;
    ScopedPointer<HyperlinkButton> video_5;
    ScopedPointer<HyperlinkButton> video_6;
    ScopedPointer<HyperlinkButton> video_7;
    ScopedPointer<HyperlinkButton> video_8;
    ScopedPointer<HyperlinkButton> video_9;
    ScopedPointer<HyperlinkButton> video_10;
    ScopedPointer<HyperlinkButton> video_11;
    ScopedPointer<HyperlinkButton> video_12;
    ScopedPointer<HyperlinkButton> video_13;
    ScopedPointer<HyperlinkButton> video_14;
    ScopedPointer<Label> label;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> label4;
    ScopedPointer<Label> label5;
    ScopedPointer<Label> label6;
    ScopedPointer<Label> label7;
    ScopedPointer<Label> label8;
    ScopedPointer<Label> label9;
    ScopedPointer<Label> label10;
    ScopedPointer<HyperlinkButton> video_15;
    ScopedPointer<HyperlinkButton> video_16;
    ScopedPointer<HyperlinkButton> video_18;
    ScopedPointer<HyperlinkButton> video_17;
    Image cachedImage_demobg_png2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorDemo)
};

//[EndFile] You can add extra defines here...
#endif // DEMO
//[/EndFile]

#endif   // __JUCE_HEADER_F6D557DA58F99AC__
