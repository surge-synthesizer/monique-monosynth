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

#ifndef __MONIQUE_UI_INFO__
#define __MONIQUE_UI_INFO__

#include "monique_ui_LookAndFeel.h"
#include <memory>

class Monique_Ui_Info : public juce::Button::Listener, public juce::Component
{
  public:
    //==============================================================================
    Monique_Ui_Info(UiLookAndFeel *look_and_feel_);
    ~Monique_Ui_Info();

    //==============================================================================
    const float original_w;
    const float original_h;
    std::string host, wrapper;
    struct AboutInfo
    {
        std::string value;
    };
    std::vector<AboutInfo> info;
    std::unique_ptr<juce::TextButton> copy_vers_info;

    void resized() override;

    void paint(juce::Graphics &g) override;

    void buttonClicked(juce::Button *button) override;

    juce::Colour fillColour{juce::Colour(0, 0, 0)};

  private:
    //==============================================================================
    UiLookAndFeel *const look_and_feel;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Info);
};

#endif // __MONIQUE_UI_INFO__
