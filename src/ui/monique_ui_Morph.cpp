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

#include "monique_ui_Morph.h"

#include "monique_ui_DragPad.h"
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "core/monique_core_Datastructures.h"

#include <memory>

void Monique_Ui_MorphConfig::refresh() noexcept
{
    {
        int motor_time = synth_data->morph_motor_time.get_value();
        slider_morph_motor_time->setValue(motor_time, juce::dontSendNotification);
        if (motor_time > 999)
        {
            if (slider_morph_motor_time->getProperties().set(
                    VAR_INDEX_VALUE_TO_SHOW, juce::String(round01(float(motor_time) / 1000)) +
                                                 juce::String("@") + juce::String("s")))
            {
                slider_morph_motor_time->repaint();
            }
        }
        else
        {
            if (slider_morph_motor_time->getProperties().set(
                    VAR_INDEX_VALUE_TO_SHOW,
                    juce::String(motor_time) + juce::String("@") + juce::String("ms")))
            {
                slider_morph_motor_time->repaint();
            }
        }
    }

    if (synth_data->get_current_program() != last_program)
    {
        last_program = synth_data->get_current_program();
        refresh_combos();
    }
}

void Monique_Ui_MorphConfig::refresh_combos() noexcept
{
    for (int i = 0; i != morph_combos.size(); ++i)
    {
        juce::ComboBox *box = morph_combos.getUnchecked(i);

        box->clear(juce::dontSendNotification);
        for (int bank_id = 0; bank_id != 4; ++bank_id)
        {
            juce::String bank = bank_id == 0    ? "A"
                                : bank_id == 1  ? "B"
                                : bank_id == 2  ? "C"
                                : bank_id == 3  ? "D"
                                : bank_id == 4  ? "E"
                                : bank_id == 5  ? "F"
                                : bank_id == 6  ? "G"
                                : bank_id == 7  ? "H"
                                : bank_id == 8  ? "I"
                                : bank_id == 9  ? "J"
                                : bank_id == 10 ? "K"
                                : bank_id == 11 ? "L"
                                : bank_id == 12 ? "M"
                                : bank_id == 13 ? "N"
                                : bank_id == 14 ? "O"
                                : bank_id == 15 ? "P"
                                : bank_id == 16 ? "Q"
                                : bank_id == 17 ? "R"
                                : bank_id == 18 ? "S"
                                : bank_id == 19 ? "T"
                                : bank_id == 20 ? "U"
                                : bank_id == 21 ? "V"
                                : bank_id == 22 ? "W"
                                : bank_id == 23 ? "X"
                                : bank_id == 24 ? "Y"
                                                : "Z";

            if (i > 3)
            {
                box->addSectionHeading(juce::String("Load preset to LEFT from bank: ") + bank);
            }
            else
            {
                box->addSectionHeading(juce::String("Load preset to RIGHT from bank: ") + bank);
            }
            juce::StringArray morph_program_names(synth_data->get_programms(bank_id));

            int start_index = box->getNumItems() + 1 + 1;
            box->addItemList(morph_program_names, start_index);

            if (i == 0)
            {
                bank_start_indexs.add(start_index);
            }
        }

        box->setText(synth_data->get_morph_source_name(i), juce::dontSendNotification);
    }
}

//==============================================================================
Monique_Ui_MorphConfig::Monique_Ui_MorphConfig(Monique_Ui_Refresher *const ui_refresher_,
                                               UiLookAndFeel *const look_and_feel_)
    : Monique_Ui_Refreshable(ui_refresher_), original_w(1465), original_h(180)
{
    last_program = -1;

    label_31 = std::make_unique<juce::Label>(juce::String(), TRANS("FILTER (R)"));
    addAndMakeVisible(*label_31);
    label_31->setFont(juce::Font(30.00f, juce::Font::plain));
    label_31->setJustificationType(juce::Justification::centredRight);
    label_31->setEditable(false, false, false);
    label_31->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_31->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_31->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_32 = std::make_unique<juce::Label>(juce::String(), TRANS("FX (R)"));
    addAndMakeVisible(*label_32);
    label_32->setFont(juce::Font(30.00f, juce::Font::plain));
    label_32->setJustificationType(juce::Justification::centredRight);
    label_32->setEditable(false, false, false);
    label_32->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_32->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_32->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_33 = std::make_unique<juce::Label>(juce::String(), TRANS("ARP (R)"));
    addAndMakeVisible(*label_33);
    label_33->setFont(juce::Font(30.00f, juce::Font::plain));
    label_33->setJustificationType(juce::Justification::centredLeft);
    label_33->setEditable(false, false, false);
    label_33->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_33->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_33->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_30 = std::make_unique<juce::Label>(juce::String(), TRANS("OSC (R)"));
    addAndMakeVisible(*label_30);
    label_30->setFont(juce::Font(30.00f, juce::Font::plain));
    label_30->setJustificationType(juce::Justification::centredLeft);
    label_30->setEditable(false, false, false);
    label_30->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_30->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_30->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_34 = std::make_unique<juce::Label>(juce::String(), TRANS("ALL (L)"));
    addAndMakeVisible(*label_34);
    label_34->setFont(juce::Font(30.00f, juce::Font::plain));
    label_34->setJustificationType(juce::Justification::centred);
    label_34->setEditable(false, false, false);
    label_34->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_34->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_34->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    combo_morph_group_1 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_1);
    combo_morph_group_1->setTooltip(
        TRANS("Load a program to the RIGHT side of the OSC morph group.\n"
              "\n"
              "Use the OSC morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_1->setEditableText(false);
    combo_morph_group_1->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_1->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_1->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_1->addListener(this);

    combo_morph_group_2 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_2);
    combo_morph_group_2->setTooltip(
        TRANS("Load a program to the RIGHT side of the FILTER morph group.\n"
              "\n"
              "Use the FLT morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_2->setEditableText(false);
    combo_morph_group_2->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_2->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_2->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_2->addListener(this);

    combo_morph_group_3 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_3);
    combo_morph_group_3->setTooltip(
        TRANS("Load a program to the RIGHT side of the FX morph group.\n"
              "\n"
              "Use the FX morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_3->setEditableText(false);
    combo_morph_group_3->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_3->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_3->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_3->addListener(this);

    combo_morph_group_4 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_4);
    combo_morph_group_4->setTooltip(
        TRANS("Load a program to the RIGHT side of the ARPEGGIATOR morph group.\n"
              "\n"
              "Use the ARP morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_4->setEditableText(false);
    combo_morph_group_4->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_4->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_4->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_4->addListener(this);

    combo_morph_group_5 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_5);
    combo_morph_group_5->setTooltip(
        TRANS("Load a program to the LEFT side of the OSC morph group.\n"
              "\n"
              "Use the OSC morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_5->setEditableText(false);
    combo_morph_group_5->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_5->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_5->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_5->addListener(this);

    combo_morph_group_6 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_6);
    combo_morph_group_6->setTooltip(
        TRANS("Load a program to the LEFT side of the FILTER morph group.\n"
              "\n"
              "Use the FLT morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_6->setEditableText(false);
    combo_morph_group_6->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_6->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_6->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_6->addListener(this);

    combo_morph_group_7 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_7);
    combo_morph_group_7->setTooltip(TRANS("Load a program to the LEFT side of the FX morph group.\n"
                                          "\n"
                                          "Use the FX morph slider on the main user interface to "
                                          "morph between the LEFT and RIGHT side. "));
    combo_morph_group_7->setEditableText(false);
    combo_morph_group_7->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_7->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_7->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_7->addListener(this);

    combo_morph_group_8 = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_morph_group_8);
    combo_morph_group_8->setTooltip(
        TRANS("Load a program to the LEFT side of the ARPEGGIATOR morph group.\n"
              "\n"
              "Use the ARP morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    combo_morph_group_8->setEditableText(false);
    combo_morph_group_8->setJustificationType(juce::Justification::centredLeft);
    combo_morph_group_8->setTextWhenNothingSelected(TRANS("PRESET"));
    combo_morph_group_8->setTextWhenNoChoicesAvailable(TRANS("-"));
    combo_morph_group_8->addListener(this);

    label_19 = std::make_unique<juce::Label>(juce::String(), TRANS("LEFT"));
    addAndMakeVisible(*label_19);
    label_19->setFont(juce::Font(30.00f, juce::Font::plain));
    label_19->setJustificationType(juce::Justification::centredRight);
    label_19->setEditable(false, false, false);
    label_19->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_19->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_19->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_set_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_1);
    button_set_1->setTooltip(
        TRANS("Set the current state as the LEFT and RIGHT side of the OSC morph group.\n"
              "\n"
              "Use the OSC morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_1->setButtonText(TRANS("SET TO CURRENT"));
    button_set_1->addListener(this);
    button_set_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_1->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    label_ui_headline_1 = std::make_unique<juce::Label>(juce::String(), TRANS("OSC MORPH (OSC)"));
    addAndMakeVisible(*label_ui_headline_1);
    label_ui_headline_1->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_1->setJustificationType(juce::Justification::centred);
    label_ui_headline_1->setEditable(false, false, false);
    label_ui_headline_1->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_1->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_1->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_ui_headline_2 =
        std::make_unique<juce::Label>(juce::String(), TRANS("FILTER MORPH (FLT)"));
    addAndMakeVisible(*label_ui_headline_2);
    label_ui_headline_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_2->setJustificationType(juce::Justification::centred);
    label_ui_headline_2->setEditable(false, false, false);
    label_ui_headline_2->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_ui_headline_3 = std::make_unique<juce::Label>(juce::String(), TRANS("FX/EQ MORPH (FX)"));
    addAndMakeVisible(*label_ui_headline_3);
    label_ui_headline_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_3->setJustificationType(juce::Justification::centred);
    label_ui_headline_3->setEditable(false, false, false);
    label_ui_headline_3->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_ui_headline_4 = std::make_unique<juce::Label>(juce::String(), TRANS("ARP MORPH (ARP)"));
    addAndMakeVisible(*label_ui_headline_4);
    label_ui_headline_4->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_4->setJustificationType(juce::Justification::centred);
    label_ui_headline_4->setEditable(false, false, false);
    label_ui_headline_4->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_4->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_4 = std::make_unique<juce::Label>(juce::String(), TRANS("RIGHT"));
    addAndMakeVisible(*label_4);
    label_4->setFont(juce::Font(30.00f, juce::Font::plain));
    label_4->setJustificationType(juce::Justification::centredRight);
    label_4->setEditable(false, false, false);
    label_4->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_4->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_ui_headline_5 =
        std::make_unique<juce::Label>(juce::String(), TRANS("MORPH PAD (DRAG IT!)"));
    addAndMakeVisible(*label_ui_headline_5);
    label_ui_headline_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_5->setJustificationType(juce::Justification::centred);
    label_ui_headline_5->setEditable(false, false, false);
    label_ui_headline_5->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_35 = std::make_unique<juce::Label>(juce::String(), TRANS("DOUBLE CLICK"));
    addAndMakeVisible(*label_35);
    label_35->setFont(juce::Font(30.00f, juce::Font::plain));
    label_35->setJustificationType(juce::Justification::centred);
    label_35->setEditable(false, false, false);
    label_35->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_35->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_35->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_2 = std::make_unique<juce::Label>(juce::String(), TRANS("L&R"));
    addAndMakeVisible(*label_2);
    label_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_2->setJustificationType(juce::Justification::centredRight);
    label_2->setEditable(false, false, false);
    label_2->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_set_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_2);
    button_set_2->setTooltip(
        TRANS("Set the current state as the LEFT and RIGHT side of the FLT morph group.\n"
              "\n"
              "Use the FLT morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_2->setButtonText(TRANS("SET TO CURRENT"));
    button_set_2->addListener(this);
    button_set_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_2->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    label_3 = std::make_unique<juce::Label>(juce::String(), TRANS("LEFT"));
    addAndMakeVisible(*label_3);
    label_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_3->setJustificationType(juce::Justification::centredRight);
    label_3->setEditable(false, false, false);
    label_3->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_5 = std::make_unique<juce::Label>(juce::String(), TRANS("RIGHT"));
    addAndMakeVisible(*label_5);
    label_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_5->setJustificationType(juce::Justification::centredRight);
    label_5->setEditable(false, false, false);
    label_5->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_6 = std::make_unique<juce::Label>(juce::String(), TRANS("L&R"));
    addAndMakeVisible(*label_6);
    label_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_6->setJustificationType(juce::Justification::centredRight);
    label_6->setEditable(false, false, false);
    label_6->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_set_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_3);
    button_set_3->setTooltip(
        TRANS("Set the current state as the LEFT and RIGHT side of the FX morph group.\n"
              "\n"
              "Use the FX morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_3->setButtonText(TRANS("SET TO CURRENT"));
    button_set_3->addListener(this);
    button_set_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_3->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    label_7 = std::make_unique<juce::Label>(juce::String(), TRANS("LEFT"));
    addAndMakeVisible(*label_7);
    label_7->setFont(juce::Font(30.00f, juce::Font::plain));
    label_7->setJustificationType(juce::Justification::centredRight);
    label_7->setEditable(false, false, false);
    label_7->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_8 = std::make_unique<juce::Label>(juce::String(), TRANS("RIGHT"));
    addAndMakeVisible(*label_8);
    label_8->setFont(juce::Font(30.00f, juce::Font::plain));
    label_8->setJustificationType(juce::Justification::centredRight);
    label_8->setEditable(false, false, false);
    label_8->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_8->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_8->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_9 = std::make_unique<juce::Label>(juce::String(), TRANS("L&R"));
    addAndMakeVisible(*label_9);
    label_9->setFont(juce::Font(30.00f, juce::Font::plain));
    label_9->setJustificationType(juce::Justification::centredRight);
    label_9->setEditable(false, false, false);
    label_9->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_9->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_9->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_set_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_4);
    button_set_4->setTooltip(
        TRANS("Set the current state as the LEFT and RIGHT side of the ARP morph group.\n"
              "\n"
              "Use the ARP morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_4->setButtonText(TRANS("SET TO CURRENT"));
    button_set_4->addListener(this);
    button_set_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_4->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    label_10 = std::make_unique<juce::Label>(juce::String(), TRANS("LEFT"));
    addAndMakeVisible(*label_10);
    label_10->setFont(juce::Font(30.00f, juce::Font::plain));
    label_10->setJustificationType(juce::Justification::centredRight);
    label_10->setEditable(false, false, false);
    label_10->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_10->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_10->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_11 = std::make_unique<juce::Label>(juce::String(), TRANS("RIGHT"));
    addAndMakeVisible(*label_11);
    label_11->setFont(juce::Font(30.00f, juce::Font::plain));
    label_11->setJustificationType(juce::Justification::centredRight);
    label_11->setEditable(false, false, false);
    label_11->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_11->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_11->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_12 = std::make_unique<juce::Label>(juce::String(), TRANS("L&R"));
    addAndMakeVisible(*label_12);
    label_12->setFont(juce::Font(30.00f, juce::Font::plain));
    label_12->setJustificationType(juce::Justification::centredRight);
    label_12->setEditable(false, false, false);
    label_12->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_morph_motor_time = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_morph_motor_time);
    slider_morph_motor_time->setTooltip(
        TRANS("Define the MORPH MOTOR TIME.\n"
              "\n"
              "Morph motor time is the speed of the morph sliders if you morph via the morph pad "
              "or the horizontal slider below the morph sliders"));
    slider_morph_motor_time->setRange(1, 20000, 1);
    slider_morph_motor_time->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_morph_motor_time->setColour(juce::Slider::rotarySliderFillColourId,
                                       juce::Colours::yellow);
    slider_morph_motor_time->setColour(juce::Slider::rotarySliderOutlineColourId,
                                       juce::Colour(0xff161616));
    slider_morph_motor_time->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_morph_motor_time->setColour(juce::Slider::textBoxBackgroundColourId,
                                       juce::Colour(0xff161616));
    slider_morph_motor_time->addListener(this);

    label_ui_headline_6 = std::make_unique<juce::Label>(juce::String(), TRANS("SMOOTH"));
    addAndMakeVisible(*label_ui_headline_6);
    label_ui_headline_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_ui_headline_6->setJustificationType(juce::Justification::centred);
    label_ui_headline_6->setEditable(false, false, false);
    label_ui_headline_6->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_ui_headline_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_ui_headline_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_set_r_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_r_1);
    button_set_r_1->setTooltip(
        TRANS("Set the current state as the RIGHT side of the OSC morph group.\n"
              "\n"
              "Use the OSC morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_r_1->setButtonText(TRANS("SET"));
    button_set_r_1->addListener(this);
    button_set_r_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_r_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_r_1->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_l_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_l_1);
    button_set_l_1->setTooltip(
        TRANS("Set the current state as the LEFT side of the OSC morph group.\n"
              "\n"
              "Use the OSC morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_l_1->setButtonText(TRANS("SET"));
    button_set_l_1->addListener(this);
    button_set_l_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_l_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_l_1->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_r_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_r_2);
    button_set_r_2->setTooltip(
        TRANS("Set the current state as the RIGHT side of the FILTER morph group.\n"
              "\n"
              "Use the FLT morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_r_2->setButtonText(TRANS("SET"));
    button_set_r_2->addListener(this);
    button_set_r_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_r_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_r_2->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_l_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_l_2);
    button_set_l_2->setTooltip(
        TRANS("Set the current state as the LEFT side of the FILTER morph group.\n"
              "\n"
              "Use the FLT morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_l_2->setButtonText(TRANS("SET"));
    button_set_l_2->addListener(this);
    button_set_l_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_l_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_l_2->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_r_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_r_3);
    button_set_r_3->setTooltip(
        TRANS("Set the current state as the RIGHT side of the FX morph group.\n"
              "\n"
              "Use the FX morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_r_3->setButtonText(TRANS("SET"));
    button_set_r_3->addListener(this);
    button_set_r_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_r_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_r_3->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_l_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_l_3);
    button_set_l_3->setTooltip(
        TRANS("Set the current state as the LEFT side of the FX morph group.\n"
              "\n"
              "Use the FX morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_l_3->setButtonText(TRANS("SET"));
    button_set_l_3->addListener(this);
    button_set_l_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_l_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_l_3->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_r_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_r_4);
    button_set_r_4->setTooltip(
        TRANS("Set the current state as the RIGHT side of the ARP morph group.\n"
              "\n"
              "Use the ARP morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_r_4->setButtonText(TRANS("SET"));
    button_set_r_4->addListener(this);
    button_set_r_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_r_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_r_4->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    button_set_l_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_set_l_4);
    button_set_l_4->setTooltip(
        TRANS("Set the current state as the LEFT side of the ARP morph group.\n"
              "\n"
              "Use the ARP morph slider on the main user interface to morph between the LEFT and "
              "RIGHT side. "));
    button_set_l_4->setButtonText(TRANS("SET"));
    button_set_l_4->addListener(this);
    button_set_l_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_set_l_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_set_l_4->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffff11));

    drag_pad = std::make_unique<Monique_Ui_DragPad>(ui_refresher);
    addAndMakeVisible(*drag_pad);

    morph_combos.add(combo_morph_group_1.get());
    morph_combos.add(combo_morph_group_2.get());
    morph_combos.add(combo_morph_group_3.get());
    morph_combos.add(combo_morph_group_4.get());
    morph_combos.add(combo_morph_group_5.get());
    morph_combos.add(combo_morph_group_6.get());
    morph_combos.add(combo_morph_group_7.get());
    morph_combos.add(combo_morph_group_8.get());
    refresh_combos();

    this->setWantsKeyboardFocus(false);
    this->setOpaque(true);
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        juce::Component *comp = getChildComponent(i);
        comp->setOpaque(true);
    }
    drag_pad->setOpaque(false);
    label_ui_headline_5->setOpaque(false);
    label_30->setOpaque(false);
    label_31->setOpaque(false);
    label_34->setOpaque(false);
    label_35->setOpaque(false);
    label_33->setOpaque(false);
    label_32->setOpaque(false);
    label_ui_headline_6->setOpaque(false);
    label_ui_headline_1->setOpaque(false);
    label_ui_headline_2->setOpaque(false);
    label_ui_headline_4->setOpaque(false);
    label_ui_headline_3->setOpaque(false);

    label_ui_headline_5->getProperties().set(VAR_INDEX_COLOUR_THEME, MORPH_THEME);
    label_34->getProperties().set(VAR_INDEX_COLOUR_THEME, MORPH_THEME);
    label_35->getProperties().set(VAR_INDEX_COLOUR_THEME, MORPH_THEME);
    slider_morph_motor_time->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                 SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_morph_motor_time->getProperties().set(VAR_INDEX_COLOUR_THEME, MORPH_THEME);
    label_ui_headline_6->getProperties().set(VAR_INDEX_COLOUR_THEME, MORPH_THEME);

    label_ui_headline_1->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    combo_morph_group_5->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    combo_morph_group_1->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    button_set_1->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    label_19->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    label_4->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    label_2->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    label_30->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    button_set_l_1->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);
    button_set_r_1->getProperties().set(VAR_INDEX_COLOUR_THEME, OSC_THEME);

    label_ui_headline_2->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    combo_morph_group_6->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    combo_morph_group_2->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    button_set_2->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    label_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    label_5->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    label_6->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    label_31->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    button_set_l_2->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);
    button_set_r_2->getProperties().set(VAR_INDEX_COLOUR_THEME, FILTER_THEME);

    label_ui_headline_4->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    combo_morph_group_8->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    combo_morph_group_4->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    button_set_4->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    label_10->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    label_11->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    label_12->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    label_32->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    button_set_l_4->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);
    button_set_r_4->getProperties().set(VAR_INDEX_COLOUR_THEME, ARP_THEME);

    label_ui_headline_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    combo_morph_group_7->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    combo_morph_group_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    button_set_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    label_7->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    label_8->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    label_9->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    label_33->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    button_set_l_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    button_set_r_3->getProperties().set(VAR_INDEX_COLOUR_THEME, FX_THEME);
    /*

    setSize (1465, 720);

    */
}

Monique_Ui_MorphConfig::~Monique_Ui_MorphConfig()
{
    label_31 = nullptr;
    label_32 = nullptr;
    label_33 = nullptr;
    label_30 = nullptr;
    label_34 = nullptr;
    combo_morph_group_1 = nullptr;
    combo_morph_group_2 = nullptr;
    combo_morph_group_3 = nullptr;
    combo_morph_group_4 = nullptr;
    combo_morph_group_5 = nullptr;
    combo_morph_group_6 = nullptr;
    combo_morph_group_7 = nullptr;
    combo_morph_group_8 = nullptr;
    label_19 = nullptr;
    button_set_1 = nullptr;
    label_ui_headline_1 = nullptr;
    label_ui_headline_2 = nullptr;
    label_ui_headline_3 = nullptr;
    label_ui_headline_4 = nullptr;
    label_4 = nullptr;
    label_ui_headline_5 = nullptr;
    label_35 = nullptr;
    label_2 = nullptr;
    button_set_2 = nullptr;
    label_3 = nullptr;
    label_5 = nullptr;
    label_6 = nullptr;
    button_set_3 = nullptr;
    label_7 = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    button_set_4 = nullptr;
    label_10 = nullptr;
    label_11 = nullptr;
    label_12 = nullptr;
    slider_morph_motor_time = nullptr;
    label_ui_headline_6 = nullptr;
    button_set_r_1 = nullptr;
    button_set_l_1 = nullptr;
    button_set_r_2 = nullptr;
    button_set_l_2 = nullptr;
    button_set_r_3 = nullptr;
    button_set_l_3 = nullptr;
    button_set_r_4 = nullptr;
    button_set_l_4 = nullptr;
    drag_pad = nullptr;
}

//==============================================================================
void Monique_Ui_MorphConfig::paint(juce::Graphics &g)
{
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.fillAll(Colour(0xff050505));

    g.setColour(Colour(0xffffffee));
    g.fillRoundedRectangle(1051.0f, 0.0f, 308.0f, 198.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xffff11ff), 1050.0f, 180.0f, Colour(0x00000000),
                                     1170.0f, 120.0f, true));
    g.fillRoundedRectangle(1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 1360.0f, 180.0f, Colour(0x00000000),
                                     1250.0f, 110.0f, true));
    g.fillRoundedRectangle(1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xffff1111), 1050.0f, 0.0f, Colour(0x00000000), 1160.0f,
                                     60.0f, true));
    g.fillRoundedRectangle(1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 1360.0f, 0.0f, Colour(0x00000000), 1240.0f,
                                     60.0f, true));
    g.fillRoundedRectangle(1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(271.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour(Colour(0xffff11ff));
    g.fillRoundedRectangle(791.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(531.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour(Colour(0xffff1111));
    g.fillRoundedRectangle(10.0f, 0.0f, 259.0f, 198.0f, 10.000f);

    g.setColour(Colour(0xffffffee));
    g.fillRoundedRectangle(1361.0f, 0.0f, 90.0f, 198.0f, 10.000f);
}

void Monique_Ui_MorphConfig::resized()
{
    WIDTH_AND_HIGHT_FACTORS

    label_31->setBounds(1270, 30, 80, 30);
    label_32->setBounds(1270, 130, 80, 30);
    label_33->setBounds(1070, 130, 80, 30);
    label_30->setBounds(1070, 30, 80, 30);
    label_34->setBounds(1050 + 309.0 / 2 - (80.0 / 2), 0 + 180.0 / 2 + -13 - (30.0 / 2), 80, 30);
    combo_morph_group_1->setBounds(70, 90, 130, 30);
    combo_morph_group_2->setBounds(330, 90, 130, 30);
    combo_morph_group_3->setBounds(850, 90, 130, 30);
    combo_morph_group_4->setBounds(590, 90, 130, 30);
    combo_morph_group_5->setBounds(70, 50, 130, 30);
    combo_morph_group_6->setBounds(330, 50, 130, 30);
    combo_morph_group_7->setBounds(850, 50, 130, 30);
    combo_morph_group_8->setBounds(590, 50, 130, 30);
    label_19->setBounds(20, 50, 50, 30);
    button_set_1->setBounds(70, 130, 180, 30);
    label_ui_headline_1->setBounds(10, 0, 260, 30);
    label_ui_headline_2->setBounds(270, 0, 260, 30);
    label_ui_headline_3->setBounds(790, 0, 260, 30);
    label_ui_headline_4->setBounds(530, 0, 260, 30);
    label_4->setBounds(20, 90, 50, 30);
    label_ui_headline_5->setBounds(1050 + 309.0 / 2 - (320.0 / 2), 0, 320, 30);
    label_35->setBounds(1050 + 309.0 / 2 - (130.0 / 2), 0 + 180.0 / 2 + 15 - (30.0 / 2), 130, 30);
    label_2->setBounds(20, 130, 50, 30);
    button_set_2->setBounds(330, 130, 180, 30);
    label_3->setBounds(280, 50, 50, 30);
    label_5->setBounds(280, 90, 50, 30);
    label_6->setBounds(280, 130, 50, 30);
    button_set_3->setBounds(850, 130, 180, 30);
    label_7->setBounds(800, 50, 50, 30);
    label_8->setBounds(800, 90, 50, 30);
    label_9->setBounds(800, 130, 50, 30);
    button_set_4->setBounds(590, 130, 180, 30);
    label_10->setBounds(540, 50, 50, 30);
    label_11->setBounds(540, 90, 50, 30);
    label_12->setBounds(540, 130, 50, 30);
    slider_morph_motor_time->setBounds(1370, 60, 70, 60);
    label_ui_headline_6->setBounds(1050 + 311, 0, 89, 30);
    button_set_r_1->setBounds(200, 90, 50, 30);
    button_set_l_1->setBounds(200, 50, 50, 30);
    button_set_r_2->setBounds(460, 90, 50, 30);
    button_set_l_2->setBounds(460, 50, 50, 30);
    button_set_r_3->setBounds(980, 90, 50, 30);
    button_set_l_3->setBounds(980, 50, 50, 30);
    button_set_r_4->setBounds(720, 90, 50, 30);
    button_set_l_4->setBounds(720, 50, 50, 30);
    drag_pad->setBounds(1050, 0, 309, 180);

#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_MorphConfig::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    int bank;
    if (comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[1])
        bank = 0;
    else if (comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[2])
        bank = 1;
    else if (comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[3])
        bank = 2;
    else
        bank = 3;

    if (comboBoxThatHasChanged == combo_morph_group_1.get())
    {
        synth_data->try_to_load_programm_to_right_side(
            0, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_2.get())
    {
        synth_data->try_to_load_programm_to_right_side(
            1, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_3.get())
    {
        synth_data->try_to_load_programm_to_right_side(
            2, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_4.get())
    {
        synth_data->try_to_load_programm_to_right_side(
            3, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_5.get())
    {
        synth_data->try_to_load_programm_to_left_side(
            0, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_6.get())
    {
        synth_data->try_to_load_programm_to_left_side(
            1, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_7.get())
    {
        synth_data->try_to_load_programm_to_left_side(
            2, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
    else if (comboBoxThatHasChanged == combo_morph_group_8.get())
    {
        synth_data->try_to_load_programm_to_left_side(
            3, bank, comboBoxThatHasChanged->getSelectedId() - bank_start_indexs[bank]);
    }
}

void Monique_Ui_MorphConfig::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == button_set_1.get())
    {
        synth_data->set_morph_source_data_from_current(0, LEFT, false);
        synth_data->set_morph_source_data_from_current(0, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_2.get())
    {
        synth_data->set_morph_source_data_from_current(1, LEFT, false);
        synth_data->set_morph_source_data_from_current(1, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_3.get())
    {
        synth_data->set_morph_source_data_from_current(2, LEFT, false);
        synth_data->set_morph_source_data_from_current(2, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_4.get())
    {
        synth_data->set_morph_source_data_from_current(3, LEFT, false);
        synth_data->set_morph_source_data_from_current(3, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_r_1.get())
    {
        synth_data->set_morph_source_data_from_current(0, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_l_1.get())
    {
        synth_data->set_morph_source_data_from_current(0, LEFT, true);
    }
    else if (buttonThatWasClicked == button_set_r_2.get())
    {
        synth_data->set_morph_source_data_from_current(1, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_l_2.get())
    {
        synth_data->set_morph_source_data_from_current(1, LEFT, true);
    }
    else if (buttonThatWasClicked == button_set_r_3.get())
    {
        synth_data->set_morph_source_data_from_current(2, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_l_3.get())
    {
        synth_data->set_morph_source_data_from_current(2, LEFT, true);
    }
    else if (buttonThatWasClicked == button_set_r_4.get())
    {
        synth_data->set_morph_source_data_from_current(3, RIGHT, true);
    }
    else if (buttonThatWasClicked == button_set_l_4.get())
    {
        synth_data->set_morph_source_data_from_current(3, LEFT, true);
    }

    refresh_combos();
}

void Monique_Ui_MorphConfig::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{

    if (sliderThatWasMoved == slider_morph_motor_time.get())
    {
        synth_data->morph_motor_time = sliderThatWasMoved->getValue();
    }
}
