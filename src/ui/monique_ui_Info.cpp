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

#include "monique_ui_Info.h"

#include "version.h"
#include <string>

//==============================================================================
Monique_Ui_Info::Monique_Ui_Info(UiLookAndFeel *look_and_feel_)
    : original_w(1465), original_h(180), look_and_feel(look_and_feel_)
{
#if MAC
    std::string platform = "macOS";
#elif WINDOWS
    std::string platform = "Windows";
#elif LINUX
    std::string platform = "Linux";
#else
    std::string platform = "GLaDOS, Orac or Skynet";
#endif
    info.push_back({std::string(Monique::Build::FullVersionStr) + " on " + platform});
    info.push_back({std::string(Monique::Build::BuildDate) + " at " + Monique::Build::BuildTime +
                    " using " + Monique::Build::BuildCompiler});
    setOpaque(true);

    copy_vers_info = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*copy_vers_info);
    copy_vers_info->setTooltip(TRANS("Copy Build Info"));
    copy_vers_info->setButtonText(TRANS("COPY"));
    copy_vers_info->addListener(this);
    copy_vers_info->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
}

Monique_Ui_Info::~Monique_Ui_Info() { copy_vers_info = nullptr; }

void Monique_Ui_Info::buttonClicked(juce::Button *buttonThatWasClicked)
{
    std::ostringstream oss;
    for (auto i : info)
    {
        oss << i.value << "\n";
    }
    if (buttonThatWasClicked == copy_vers_info.get())
    {
        juce::SystemClipboard::copyTextToClipboard(oss.str());
    }
}

void Monique_Ui_Info::paint(juce::Graphics &g)
{
    ComponentColours &colours(look_and_feel->colours);

    g.setColour(juce::Colour(0xff000a12));
    g.fillRoundedRectangle(3.0f, 3.0f, static_cast<float>(getWidth() - 6),
                           static_cast<float>(getHeight() - 6), 1.000f);

    g.setColour(juce::Colours::aqua);
    g.drawRoundedRectangle(3.0f, 3.0f, static_cast<float>(getWidth() - 6),
                           static_cast<float>(getHeight() - 6), 1.000f, 1.000f);

    auto r = getLocalBounds().withTrimmedTop(getLocalBounds().getHeight() - 64).translated(0, -10);

    for (auto q : info)
    {
        g.setColour(juce::Colours::lightgrey);
        g.setFont(14);
        g.drawText(q.value, r, juce::Justification::centred);
        r = r.translated(0, 20);
    }
}

void Monique_Ui_Info::resized() { copy_vers_info->setBounds(10, 10, 60, 30); }
