/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
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
class monique_ui_Overlay : public juce::Component
{
  public:
    //==============================================================================
    monique_ui_Overlay();
    ~monique_ui_Overlay();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void mouseDown(const juce::MouseEvent &e_) override
    {
        setVisible(false);
        setAlwaysOnTop(false);
    }
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;

    // Binary resources:
    static const char *monique_overlay_svg;
    static const int monique_overlay_svgSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Drawable> drawable1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(monique_ui_Overlay)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_519A36E9C6142AC8__
