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

#ifndef __JUCE_HEADER_A95548A290FE7278__
#define __JUCE_HEADER_A95548A290FE7278__

#include "App.h"

//==============================================================================

class monique_ui_Credits : public juce::Component, public Monique_Ui_Refreshable
{
  public:
    //==============================================================================
    monique_ui_Credits(Monique_Ui_Refresher *ui_refresher_);
    ~monique_ui_Credits();

    //==============================================================================
    std::unique_ptr<juce::LookAndFeel> activation_look_and_feel;
    void mouseDown(const juce::MouseEvent &e_) override
    {
        setVisible(false);
        setAlwaysOnTop(false);
    }
    void refresh() noexcept override {}

    const float original_w;
    const float original_h;

    void paint(juce::Graphics &g) override;
    void resized() override;

    // Binary resources:
    static const char *monoplugs_png;
    static const int monoplugs_pngSize;

  private:
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
    std::unique_ptr<juce::Drawable> drawable1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(monique_ui_Credits)
};

#endif // __JUCE_HEADER_A95548A290FE7278__
