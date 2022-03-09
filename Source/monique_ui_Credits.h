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

#ifndef __JUCE_HEADER_A95548A290FE7278__
#define __JUCE_HEADER_A95548A290FE7278__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class monique_ui_Credits : public juce::Component, public Monique_Ui_Refreshable
{
  public:
    //==============================================================================
    monique_ui_Credits(Monique_Ui_Refresher *ui_refresher_);
    ~monique_ui_Credits();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    std::unique_ptr<juce::LookAndFeel> activation_look_and_feel;
    void mouseDown(const juce::MouseEvent &e_) override
    {
        setVisible(false);
        setAlwaysOnTop(false);
    }
    void refresh() noexcept override {}

    const float original_w;
    const float original_h;

    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;

    // Binary resources:
    static const char *monoplugs_png;
    static const int monoplugs_pngSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label_sub_header9;
    std::unique_ptr<juce::Label> label_sub_header2;
    std::unique_ptr<juce::Label> label_sub_header3;
    std::unique_ptr<juce::Label> label_sub_header4;
    std::unique_ptr<juce::Label> label_sub_header5;
    std::unique_ptr<juce::Label> label_sub_header6;
    std::unique_ptr<juce::Label> label_sub_header7;
    std::unique_ptr<juce::Label> label_sub_header10;
    std::unique_ptr<juce::Label> label_sub_header11;
    std::unique_ptr<juce::Label> label_sub_header12;
    std::unique_ptr<juce::Label> label_sub_header13;
    std::unique_ptr<juce::Label> label_sub_header8;
    std::unique_ptr<juce::Label> label_sub_header16;
    std::unique_ptr<juce::Label> label_sub_header17;
    std::unique_ptr<juce::Label> label_sub_header14;
    std::unique_ptr<juce::Label> label_sub_header15;
    std::unique_ptr<juce::Label> label_sub_header18;
    std::unique_ptr<juce::Label> label_sub_header19;
    std::unique_ptr<juce::Label> label_sub_header20;
    std::unique_ptr<juce::HyperlinkButton> link_to_monoplugs;
    std::unique_ptr<juce::Drawable> drawable1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(monique_ui_Credits)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_A95548A290FE7278__
