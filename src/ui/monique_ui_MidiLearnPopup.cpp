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

#include "monique_ui_MidiLearnPopup.h"

#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "core/monique_core_Processor.h"

void Monique_Ui_MainwindowPopup::refresh() noexcept
{
    if (_midi_control)
    {
        if (!combo_midi_number->isMouseOverOrDragging())
        {
            combo_midi_number->setSelectedItemIndex(_midi_control->get_midi_number(),
                                                    juce::dontSendNotification);
        }
    }
    else
    {
        combo_midi_number->setEnabled(false);
    }
}

void Monique_Ui_MainwindowPopup::set_element_to_show(const juce::Component *const comp_)
{
    related_to_comp = comp_;
    int x = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getX();
    int y = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getY();
    setTopLeftPosition(x + comp_->getWidth() / 2 - getWidth() / 2, y + comp_->getHeight());

    COLOUR_THEMES theme = static_cast<COLOUR_THEMES>(int(
        comp_->getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME)));
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        juce::Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, theme);
    }
    this->getProperties().set(VAR_INDEX_COLOUR_THEME, theme);
}
void Monique_Ui_MainwindowPopup::update_positions()
{
    if (related_to_comp)
    {
        set_element_to_show(related_to_comp);
    }
}

//==============================================================================
Monique_Ui_MainwindowPopup::Monique_Ui_MainwindowPopup(Monique_Ui_Refresher *ui_refresher_,
                                                       Monique_Ui_Mainwindow *const parent_,
                                                       MIDIControl *midi_control_)
    : Monique_Ui_Refreshable(ui_refresher_),
      juce::DropShadower(
          juce::DropShadow(juce::Colours::black.withAlpha(0.8f), 10, juce::Point<int>(10, 10))),
      original_w(80), original_h(95), parent(parent_), _midi_control(midi_control_)
{
    setOwner(this);

    combo_midi_number = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_midi_number);
    combo_midi_number->setTooltip(TRANS("Select the MIDI controller number manually."));
    combo_midi_number->setEditableText(false);
    combo_midi_number->setJustificationType(juce::Justification::centredLeft);
    combo_midi_number->setTextWhenNothingSelected(TRANS("NR"));
    combo_midi_number->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    combo_midi_number->addListener(this);

    close = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*close);
    close->setButtonText(TRANS("ESC X"));
    close->addListener(this);
    close->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    close->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    close->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    close->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    related_to_comp = nullptr;

    close->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);

    int i = 0;
    combo_midi_number->addItem(juce::String("PW"), i + 1);
    i++;
    for (; i != 128; ++i)
    {
        combo_midi_number->addItem(juce::String(i + 1) + juce::String("-C"), i + 1);
    }
    for (int i = 0; i != 128; ++i)
    {
        combo_midi_number->addItem(juce::String(i + 1) + juce::String("-N"), i + 1 + 128);
    }

    /*

    setSize (80, 95);

    */
}

Monique_Ui_MainwindowPopup::~Monique_Ui_MainwindowPopup()
{

    combo_midi_number = nullptr;
    close = nullptr;
}

//==============================================================================
void Monique_Ui_MainwindowPopup::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRect(getWidth() - 10, getHeight() - 10, 10, 10);
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    COLOUR_THEMES theme_id = static_cast<COLOUR_THEMES>(int(
        this->getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME)));
    g.setColour(colours.get_theme(theme_id).area_colour);
    g.fillRoundedRectangle(1.0f, 10.0f, 78.0f, 84.0f, 10.000f);

    g.setColour(colours.get_theme(theme_id).value_slider_track_colour);
    g.drawRoundedRectangle(1.0f, 10.0f, 78.0f, 84.0f, 10.000f, 1.000f);

    g.fillPath(internalPath1);

    /*

    g.setColour (Colour (0xbaffffff));
    g.fillRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    */
}

void Monique_Ui_MainwindowPopup::resized()
{
    WIDTH_AND_HIGHT_FACTORS

    combo_midi_number->setBounds(10, 20, 60, 30);
    close->setBounds(10, 55, 60, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath(40.0f, 0.0f);
    internalPath1.lineTo(50.0f, 10.0f);
    internalPath1.lineTo(30.0f, 10.0f);
    internalPath1.closeSubPath();

#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_MainwindowPopup::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == combo_midi_number.get())
    {
        if (_midi_control)
        {
            _midi_control->train(comboBoxThatHasChanged->getSelectedItemIndex(),
                                 _midi_control->get_is_ctrl_version_of_name(),
                                 synth_data->audio_processor);
        }
    }
}

void Monique_Ui_MainwindowPopup::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == close.get())
    {
        midi_control_handler->clear();
        parent->popup = nullptr;
        return;
    }
}
