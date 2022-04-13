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

#include "App.h"

//==============================================================================

class monique_ui_Overlay : public juce::Component
{
  public:
    //==============================================================================
    monique_ui_Overlay();
    ~monique_ui_Overlay();

    //==============================================================================
    void mouseDown(const juce::MouseEvent &e_) override
    {
        setVisible(false);
        setAlwaysOnTop(false);
    }

    void paint(juce::Graphics &g) override;
    void resized() override;

    // Binary resources:
    static const char *monique_overlay_svg;
    static const int monique_overlay_svgSize;

  private:
    //==============================================================================
    std::unique_ptr<juce::Drawable> drawable1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(monique_ui_Overlay)
};

#endif // __JUCE_HEADER_519A36E9C6142AC8__
