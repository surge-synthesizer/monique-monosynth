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

#include "monique_ui_OptionPopup.h"

#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "core/monique_core_Processor.h"

void Monique_Ui_OptionPopup::refresh() noexcept
{
    SectionTheme &theme = look_and_feel->colours.get_theme(COLOUR_THEMES::POPUP_THEME);
    juce::Colour button_off = theme.button_off_colour;
    juce::Colour button_on = theme.button_on_colour;

    TURN_BUTTON_ON_OR_OFF(button_option_a, param_a->get_value());
    TURN_BUTTON_ON_OR_OFF(button_option_b, param_b->get_value());
}

void Monique_Ui_OptionPopup::set_element_to_show(juce::Component *const comp_)
{
    related_to_comp = comp_;
    int x = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getX();
    int y = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getY();
    setTopLeftPosition(x + comp_->getWidth(), y - getHeight() / 2 + comp_->getHeight() / 2);
}
void Monique_Ui_OptionPopup::update_positions()
{
    if (related_to_comp)
    {
        set_element_to_show(related_to_comp);
    }
}
void Monique_Ui_OptionPopup::set_infos(juce::StringRef text_a, juce::StringRef text_b,
                                       juce::StringRef tool_a, juce::StringRef tool_b)
{
    button_option_a->setButtonText(text_a.text);
    button_option_b->setButtonText(text_b.text);
    button_option_a->setTooltip(tool_a.text);
    button_option_b->setTooltip(tool_b.text);
}

//==============================================================================
Monique_Ui_OptionPopup::Monique_Ui_OptionPopup(Monique_Ui_Refresher *ui_refresher_,
                                               Monique_Ui_Mainwindow *const parent_,
                                               BoolParameter *param_a_, BoolParameter *param_b_)
    : Monique_Ui_Refreshable(ui_refresher_),
      juce::DropShadower(
          juce::DropShadow(juce::Colours::black.withAlpha(0.8f), 10, juce::Point<int>(10, 10))),
      param_a(param_a_), param_b(param_b_), original_w(140), original_h(90), parent(parent_)
{
    setOwner(this);

    button_option_a = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_option_a);
    button_option_a->setButtonText(TRANS("x"));
    button_option_a->addListener(this);
    button_option_a->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_option_a->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_option_a->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_option_b = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_option_b);
    button_option_b->setButtonText(TRANS("x"));
    button_option_b->addListener(this);
    button_option_b->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_option_b->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_option_b->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    related_to_comp = nullptr;
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        juce::Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
    }
    /*

    setSize (140, 90);

    */
}

Monique_Ui_OptionPopup::~Monique_Ui_OptionPopup()
{
    button_option_a = nullptr;
    button_option_b = nullptr;
}

//==============================================================================
void Monique_Ui_OptionPopup::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRect(getWidth() - 10, getHeight() - 10, 10, 10);
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.setColour(look_and_feel->colours.get_theme(COLOUR_THEMES::ARP_THEME).area_colour);
    g.fillRoundedRectangle(10.0f, 0.0f, 129.0f, 89.0f, 10.000f);

    g.setColour(
        look_and_feel->colours.get_theme(COLOUR_THEMES::ARP_THEME).value_slider_track_colour);
    g.drawRoundedRectangle(10.0f, 0.0f, 129.0f, 89.0f, 10.000f, 1.000f);

    g.fillPath(internalPath1);

    /*

    g.setColour (Colour (0xbaffffff));
    g.fillRoundedRectangle (10.0f, 0.0f, 129.0f, 89.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (10.0f, 0.0f, 129.0f, 89.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    */
}

void Monique_Ui_OptionPopup::resized()
{
    WIDTH_AND_HIGHT_FACTORS

    button_option_a->setBounds(20, 10, 110, 30);
    button_option_b->setBounds(20, 50, 110, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath(0.0f, 45.0f);
    internalPath1.lineTo(10.0f, 35.0f);
    internalPath1.lineTo(10.0f, 55.0f);
    internalPath1.closeSubPath();

#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_OptionPopup::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == button_option_a.get())
    {
        *param_a ^= true;
        param_b->set_value(false);
    }
    else if (buttonThatWasClicked == button_option_b.get())
    {
        *param_b ^= true;
        param_a->set_value(false);
    }
}

void Monique_Ui_OptionPopup::mouseExit(const juce::MouseEvent &event)
{
    parent->open_option_popup(nullptr, nullptr, nullptr, nullptr, "", "", "", "");
}
