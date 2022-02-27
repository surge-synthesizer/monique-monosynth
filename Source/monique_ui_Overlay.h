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

#ifndef __JUCE_HEADER_519A36E9C6142AC8__
#define __JUCE_HEADER_519A36E9C6142AC8__

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
class monique_ui_Overlay : public Component
{
  public:
    //==============================================================================
    monique_ui_Overlay();
    ~monique_ui_Overlay();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void mouseDown(const MouseEvent &e_) override
    {
        setVisible(false);
        setAlwaysOnTop(false);
    }
    //[/UserMethods]

    void paint(Graphics &g) override;
    void resized() override;

    // Binary resources:
    static const char *monique_overlay_svg;
    static const int monique_overlay_svgSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Drawable> drawable1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(monique_ui_Overlay)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_519A36E9C6142AC8__
