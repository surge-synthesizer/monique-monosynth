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

//[Headers] You can add your own extra header files here...
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_DragPad.h"
#include "monique_ui_MainWindow.h"

#include "monique_core_Datastructures.h"
//[/Headers]

#include "monique_ui_Morph.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_MorphConfig::refresh() noexcept
{
    {
        int motor_time = synth_data->morph_motor_time.get_value();
        slider_morph_motor_time->setValue( motor_time, dontSendNotification );
        if( motor_time > 999 )
        {
            if( slider_morph_motor_time->getProperties().set( VAR_INDEX_VALUE_TO_SHOW, String( round01( float(motor_time) / 1000 ) ) + String("@") + String("s") ) )
            {
                slider_morph_motor_time->repaint();
            }
        }
        else
        {
            if( slider_morph_motor_time->getProperties().set( VAR_INDEX_VALUE_TO_SHOW, String(motor_time) + String("@") + String("ms") ) )
            {
                slider_morph_motor_time->repaint();
            }
        }
    }

    if( synth_data->get_current_program() != last_program )
    {
        last_program = synth_data->get_current_program();
        refresh_combos();
    }
}

void Monique_Ui_MorphConfig::refresh_combos() noexcept
{
    for( int i = 0 ; i != morph_combos.size() ; ++i )
    {
        ComboBox* box = morph_combos.getUnchecked(i);

        box->clear(dontSendNotification);
        for( int bank_id = 0 ; bank_id != 4 ; ++bank_id )
        {
            String bank = bank_id == 0 ? "A" : bank_id == 1 ? "B" : bank_id == 2 ? "C" : "D";
            if( i > 3 )
            {
                box->addSectionHeading(String("Load preset to LEFT from bank: ")+bank);
            }
            else
            {
                box->addSectionHeading(String("Load preset to RIGHT from bank: ")+bank);
            }
            StringArray morph_program_names( synth_data->get_programms( bank_id ) );

            int start_index = box->getNumItems()+1+1;
            box->addItemList( morph_program_names, start_index );

            if( i == 0 )
            {
                bank_start_indexs.add( start_index );
            }
        }

        box->setText(  synth_data->get_morph_source_name(i), dontSendNotification );
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_MorphConfig::Monique_Ui_MorphConfig (Monique_Ui_Refresher*const ui_refresher_, UiLookAndFeel*const look_and_feel_)
    : Monique_Ui_Refreshable(ui_refresher_),
      original_w(1465),
      original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    last_program = -1;
    //[/Constructor_pre]

    addAndMakeVisible (label_31 = new Label (String::empty,
                                             TRANS("FILTER (R)")));
    label_31->setFont (Font (30.00f, Font::plain));
    label_31->setJustificationType (Justification::centredRight);
    label_31->setEditable (false, false, false);
    label_31->setColour (Label::textColourId, Colour (0xffff3b00));
    label_31->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_31->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_32 = new Label (String::empty,
                                             TRANS("ARP (R)")));
    label_32->setFont (Font (30.00f, Font::plain));
    label_32->setJustificationType (Justification::centredRight);
    label_32->setEditable (false, false, false);
    label_32->setColour (Label::textColourId, Colour (0xffff3b00));
    label_32->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_32->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_33 = new Label (String::empty,
                                             TRANS("FX (R)")));
    label_33->setFont (Font (30.00f, Font::plain));
    label_33->setJustificationType (Justification::centredLeft);
    label_33->setEditable (false, false, false);
    label_33->setColour (Label::textColourId, Colour (0xffff3b00));
    label_33->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_33->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_30 = new Label (String::empty,
                                             TRANS("OSC (R)")));
    label_30->setFont (Font (30.00f, Font::plain));
    label_30->setJustificationType (Justification::centredLeft);
    label_30->setEditable (false, false, false);
    label_30->setColour (Label::textColourId, Colour (0xffff3b00));
    label_30->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_30->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_34 = new Label (String::empty,
                                             TRANS("ALL (L)")));
    label_34->setFont (Font (30.00f, Font::plain));
    label_34->setJustificationType (Justification::centred);
    label_34->setEditable (false, false, false);
    label_34->setColour (Label::textColourId, Colour (0xffff3b00));
    label_34->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_34->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_morph_group_1 = new ComboBox (String::empty));
    combo_morph_group_1->setTooltip (TRANS("Load a program to the RIGHT side of the OSC morph group.\n"
    "\n"
    "Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_1->setEditableText (false);
    combo_morph_group_1->setJustificationType (Justification::centredLeft);
    combo_morph_group_1->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_1->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_1->addListener (this);

    addAndMakeVisible (combo_morph_group_2 = new ComboBox (String::empty));
    combo_morph_group_2->setTooltip (TRANS("Load a program to the RIGHT side of the FILTER morph group.\n"
    "\n"
    "Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_2->setEditableText (false);
    combo_morph_group_2->setJustificationType (Justification::centredLeft);
    combo_morph_group_2->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_2->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_2->addListener (this);

    addAndMakeVisible (combo_morph_group_3 = new ComboBox (String::empty));
    combo_morph_group_3->setTooltip (TRANS("Load a program to the RIGHT side of the FX morph group.\n"
    "\n"
    "Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_3->setEditableText (false);
    combo_morph_group_3->setJustificationType (Justification::centredLeft);
    combo_morph_group_3->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_3->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_3->addListener (this);

    addAndMakeVisible (combo_morph_group_4 = new ComboBox (String::empty));
    combo_morph_group_4->setTooltip (TRANS("Load a program to the RIGHT side of the ARPEGGIATOR morph group.\n"
    "\n"
    "Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_4->setEditableText (false);
    combo_morph_group_4->setJustificationType (Justification::centredLeft);
    combo_morph_group_4->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_4->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_4->addListener (this);

    addAndMakeVisible (combo_morph_group_5 = new ComboBox (String::empty));
    combo_morph_group_5->setTooltip (TRANS("Load a program to the LEFT side of the OSC morph group.\n"
    "\n"
    "Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_5->setEditableText (false);
    combo_morph_group_5->setJustificationType (Justification::centredLeft);
    combo_morph_group_5->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_5->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_5->addListener (this);

    addAndMakeVisible (combo_morph_group_6 = new ComboBox (String::empty));
    combo_morph_group_6->setTooltip (TRANS("Load a program to the LEFT side of the FILTER morph group.\n"
    "\n"
    "Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_6->setEditableText (false);
    combo_morph_group_6->setJustificationType (Justification::centredLeft);
    combo_morph_group_6->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_6->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_6->addListener (this);

    addAndMakeVisible (combo_morph_group_7 = new ComboBox (String::empty));
    combo_morph_group_7->setTooltip (TRANS("Load a program to the LEFT side of the FX morph group.\n"
    "\n"
    "Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_7->setEditableText (false);
    combo_morph_group_7->setJustificationType (Justification::centredLeft);
    combo_morph_group_7->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_7->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_7->addListener (this);

    addAndMakeVisible (combo_morph_group_8 = new ComboBox (String::empty));
    combo_morph_group_8->setTooltip (TRANS("Load a program to the LEFT side of the ARPEGGIATOR morph group.\n"
    "\n"
    "Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    combo_morph_group_8->setEditableText (false);
    combo_morph_group_8->setJustificationType (Justification::centredLeft);
    combo_morph_group_8->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_8->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_8->addListener (this);

    addAndMakeVisible (label_19 = new Label (String::empty,
                                             TRANS("LEFT")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredRight);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_1 = new TextButton (String::empty));
    button_set_1->setTooltip (TRANS("Set the current state as the LEFT and RIGHT side of the OSC morph group.\n"
    "\n"
    "Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_1->setButtonText (TRANS("SET TO CURRENT"));
    button_set_1->addListener (this);
    button_set_1->setColour (TextButton::buttonColourId, Colours::black);
    button_set_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_1->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (label_ui_headline_1 = new Label (String::empty,
                                                        TRANS("OSC MORPH (OSC)")));
    label_ui_headline_1->setFont (Font (30.00f, Font::plain));
    label_ui_headline_1->setJustificationType (Justification::centred);
    label_ui_headline_1->setEditable (false, false, false);
    label_ui_headline_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_2 = new Label (String::empty,
                                                        TRANS("FILTER MORPH (FLT)")));
    label_ui_headline_2->setFont (Font (30.00f, Font::plain));
    label_ui_headline_2->setJustificationType (Justification::centred);
    label_ui_headline_2->setEditable (false, false, false);
    label_ui_headline_2->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_3 = new Label (String::empty,
                                                        TRANS("FX/EQ MORPH (FX)")));
    label_ui_headline_3->setFont (Font (30.00f, Font::plain));
    label_ui_headline_3->setJustificationType (Justification::centred);
    label_ui_headline_3->setEditable (false, false, false);
    label_ui_headline_3->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_4 = new Label (String::empty,
                                                        TRANS("ARP MORPH (ARP)")));
    label_ui_headline_4->setFont (Font (30.00f, Font::plain));
    label_ui_headline_4->setJustificationType (Justification::centred);
    label_ui_headline_4->setEditable (false, false, false);
    label_ui_headline_4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("RIGHT")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredRight);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_5 = new Label (String::empty,
                                                        TRANS("MORPH PAD (DRAG IT!)")));
    label_ui_headline_5->setFont (Font (30.00f, Font::plain));
    label_ui_headline_5->setJustificationType (Justification::centred);
    label_ui_headline_5->setEditable (false, false, false);
    label_ui_headline_5->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_35 = new Label (String::empty,
                                             TRANS("(DOUBLE CLICK)")));
    label_35->setFont (Font (30.00f, Font::plain));
    label_35->setJustificationType (Justification::centred);
    label_35->setEditable (false, false, false);
    label_35->setColour (Label::textColourId, Colour (0xffff3b00));
    label_35->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_35->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("L&R")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredRight);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_2 = new TextButton (String::empty));
    button_set_2->setTooltip (TRANS("Set the current state as the LEFT and RIGHT side of the FLT morph group.\n"
    "\n"
    "Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_2->setButtonText (TRANS("SET TO CURRENT"));
    button_set_2->addListener (this);
    button_set_2->setColour (TextButton::buttonColourId, Colours::black);
    button_set_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_2->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("LEFT")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredRight);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("RIGHT")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredRight);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("L&R")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredRight);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_3 = new TextButton (String::empty));
    button_set_3->setTooltip (TRANS("Set the current state as the LEFT and RIGHT side of the FX morph group.\n"
    "\n"
    "Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_3->setButtonText (TRANS("SET TO CURRENT"));
    button_set_3->addListener (this);
    button_set_3->setColour (TextButton::buttonColourId, Colours::black);
    button_set_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_3->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("LEFT")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredRight);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("RIGHT")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredRight);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("L&R")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredRight);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_4 = new TextButton (String::empty));
    button_set_4->setTooltip (TRANS("Set the current state as the LEFT and RIGHT side of the ARP morph group.\n"
    "\n"
    "Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_4->setButtonText (TRANS("SET TO CURRENT"));
    button_set_4->addListener (this);
    button_set_4->setColour (TextButton::buttonColourId, Colours::black);
    button_set_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_4->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (label_10 = new Label (String::empty,
                                             TRANS("LEFT")));
    label_10->setFont (Font (30.00f, Font::plain));
    label_10->setJustificationType (Justification::centredRight);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_11 = new Label (String::empty,
                                             TRANS("RIGHT")));
    label_11->setFont (Font (30.00f, Font::plain));
    label_11->setJustificationType (Justification::centredRight);
    label_11->setEditable (false, false, false);
    label_11->setColour (Label::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_12 = new Label (String::empty,
                                             TRANS("L&R")));
    label_12->setFont (Font (30.00f, Font::plain));
    label_12->setJustificationType (Justification::centredRight);
    label_12->setEditable (false, false, false);
    label_12->setColour (Label::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_morph_motor_time = new Slider ("0"));
    slider_morph_motor_time->setTooltip (TRANS("Define the MORPH MOTOR TIME.\n"
    "\n"
    "Morph motor time is the speed of the morph sliders if you morph via the morph pad or the horizontal slider below the morph sliders"));
    slider_morph_motor_time->setRange (1, 20000, 1);
    slider_morph_motor_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_morph_motor_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_morph_motor_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_morph_motor_time->addListener (this);

    addAndMakeVisible (label_ui_headline_6 = new Label (String::empty,
                                                        TRANS("SMOOTH")));
    label_ui_headline_6->setFont (Font (30.00f, Font::plain));
    label_ui_headline_6->setJustificationType (Justification::centred);
    label_ui_headline_6->setEditable (false, false, false);
    label_ui_headline_6->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (drag_pad = new Monique_Ui_DragPad (ui_refresher));


    //[UserPreSize]
    morph_combos.add( combo_morph_group_1 );
    morph_combos.add( combo_morph_group_2 );
    morph_combos.add( combo_morph_group_3 );
    morph_combos.add( combo_morph_group_4 );
    morph_combos.add( combo_morph_group_5 );
    morph_combos.add( combo_morph_group_6 );
    morph_combos.add( combo_morph_group_7 );
    morph_combos.add( combo_morph_group_8 );
    refresh_combos();

    this->setWantsKeyboardFocus(false);
    this->setOpaque(true);
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        Component* comp = getChildComponent(i);
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

    label_ui_headline_5->getProperties().set( VAR_INDEX_COLOUR_THEME, MORPH_THEME );
    label_34->getProperties().set( VAR_INDEX_COLOUR_THEME, MORPH_THEME );
    label_35->getProperties().set( VAR_INDEX_COLOUR_THEME, MORPH_THEME );
    slider_morph_motor_time->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_morph_motor_time->getProperties().set( VAR_INDEX_COLOUR_THEME, MORPH_THEME );
    label_ui_headline_6->getProperties().set( VAR_INDEX_COLOUR_THEME, MORPH_THEME );

    label_ui_headline_1->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    combo_morph_group_5->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    combo_morph_group_1->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    button_set_1->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    label_19->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    label_4->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    label_2->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );
    label_30->getProperties().set( VAR_INDEX_COLOUR_THEME, OSC_THEME );

    label_ui_headline_2->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    combo_morph_group_6->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    combo_morph_group_2->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    button_set_2->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    label_3->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    label_5->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    label_6->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );
    label_31->getProperties().set( VAR_INDEX_COLOUR_THEME, FILTER_THEME );

    label_ui_headline_4->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    combo_morph_group_8->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    combo_morph_group_4->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    button_set_4->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    label_10->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    label_11->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    label_12->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );
    label_32->getProperties().set( VAR_INDEX_COLOUR_THEME, ARP_THEME );

    label_ui_headline_3->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    combo_morph_group_7->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    combo_morph_group_3->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    button_set_3->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    label_7->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    label_8->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    label_9->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    label_33->getProperties().set( VAR_INDEX_COLOUR_THEME, FX_THEME );
    /*
    //[/UserPreSize]

    setSize (1465, 720);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_MorphConfig::~Monique_Ui_MorphConfig()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

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
    drag_pad = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_MorphConfig::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffffffee));
    g.fillRoundedRectangle (1051.0f, 0.0f, 308.0f, 198.0f, 10.000f);

    g.setGradientFill (ColourGradient (Colour (0xffff11ff),
                                       1050.0f, 180.0f,
                                       Colour (0x00000000),
                                       1170.0f, 120.0f,
                                       true));
    g.fillRoundedRectangle (1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill (ColourGradient (Colour (0xffffff11),
                                       1360.0f, 180.0f,
                                       Colour (0x00000000),
                                       1250.0f, 110.0f,
                                       true));
    g.fillRoundedRectangle (1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill (ColourGradient (Colour (0xffff1111),
                                       1050.0f, 0.0f,
                                       Colour (0x00000000),
                                       1160.0f, 60.0f,
                                       true));
    g.fillRoundedRectangle (1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1360.0f, 0.0f,
                                       Colour (0x00000000),
                                       1240.0f, 60.0f,
                                       true));
    g.fillRoundedRectangle (1051.0f, 0.0f, 308.0f, 200.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (271.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xffff11ff));
    g.fillRoundedRectangle (791.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (531.0f, 0.0f, 258.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xffff1111));
    g.fillRoundedRectangle (10.0f, 0.0f, 259.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xffffffee));
    g.fillRoundedRectangle (1361.0f, 0.0f, 90.0f, 198.0f, 10.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_MorphConfig::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    label_31->setBounds (1270, 30, 80, 30);
    label_32->setBounds (1270, 130, 80, 30);
    label_33->setBounds (1070, 130, 80, 30);
    label_30->setBounds (1070, 30, 80, 30);
    label_34->setBounds (1050 + 309 / 2 - (80 / 2), 0 + 180 / 2 + -13 - (30 / 2), 80, 30);
    combo_morph_group_1->setBounds (70, 90, 180, 30);
    combo_morph_group_2->setBounds (330, 90, 180, 30);
    combo_morph_group_3->setBounds (850, 90, 180, 30);
    combo_morph_group_4->setBounds (590, 90, 180, 30);
    combo_morph_group_5->setBounds (70, 50, 180, 30);
    combo_morph_group_6->setBounds (330, 50, 180, 30);
    combo_morph_group_7->setBounds (850, 50, 180, 30);
    combo_morph_group_8->setBounds (590, 50, 180, 30);
    label_19->setBounds (20, 50, 50, 30);
    button_set_1->setBounds (70, 130, 180, 30);
    label_ui_headline_1->setBounds (10, 0, 260, 30);
    label_ui_headline_2->setBounds (270, 0, 260, 30);
    label_ui_headline_3->setBounds (790, 0, 260, 30);
    label_ui_headline_4->setBounds (530, 0, 260, 30);
    label_4->setBounds (20, 90, 50, 30);
    label_ui_headline_5->setBounds (1050 + 309 / 2 - (320 / 2), 0, 320, 30);
    label_35->setBounds (1050 + 309 / 2 - (130 / 2), 0 + 180 / 2 + 15 - (30 / 2), 130, 30);
    label_2->setBounds (20, 130, 50, 30);
    button_set_2->setBounds (330, 130, 180, 30);
    label_3->setBounds (280, 50, 50, 30);
    label_5->setBounds (280, 90, 50, 30);
    label_6->setBounds (280, 130, 50, 30);
    button_set_3->setBounds (850, 130, 180, 30);
    label_7->setBounds (800, 50, 50, 30);
    label_8->setBounds (800, 90, 50, 30);
    label_9->setBounds (800, 130, 50, 30);
    button_set_4->setBounds (590, 130, 180, 30);
    label_10->setBounds (540, 50, 50, 30);
    label_11->setBounds (540, 90, 50, 30);
    label_12->setBounds (540, 130, 50, 30);
    slider_morph_motor_time->setBounds (1370, 60, 70, 60);
    label_ui_headline_6->setBounds (1050 + 311, 0, 89, 30);
    drag_pad->setBounds (1050, 0, 309, 180);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_MorphConfig::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    int bank;
    if( comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[1] )
        bank = 0;
    else if( comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[2] )
        bank = 1;
    else if( comboBoxThatHasChanged->getSelectedId() < bank_start_indexs[3] )
        bank = 2;
    else
        bank = 3;
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_morph_group_1)
    {
        //[UserComboBoxCode_combo_morph_group_1] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_right_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_1]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_2)
    {
        //[UserComboBoxCode_combo_morph_group_2] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_right_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_2]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_3)
    {
        //[UserComboBoxCode_combo_morph_group_3] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_right_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_3]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_4)
    {
        //[UserComboBoxCode_combo_morph_group_4] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_right_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_4]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_5)
    {
        //[UserComboBoxCode_combo_morph_group_5] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_left_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_5]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_6)
    {
        //[UserComboBoxCode_combo_morph_group_6] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_left_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_6]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_7)
    {
        //[UserComboBoxCode_combo_morph_group_7] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_left_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_7]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_8)
    {
        //[UserComboBoxCode_combo_morph_group_8] -- add your combo box handling code here..
        synth_data->try_to_load_programm_to_left_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_8]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_MorphConfig::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_set_1)
    {
        //[UserButtonCode_button_set_1] -- add your button handler code here..
        synth_data->set_morph_source_data_from_current( 0, LEFT, false );
        synth_data->set_morph_source_data_from_current( 0, RIGHT, true );
        //[/UserButtonCode_button_set_1]
    }
    else if (buttonThatWasClicked == button_set_2)
    {
        //[UserButtonCode_button_set_2] -- add your button handler code here..
        synth_data->set_morph_source_data_from_current( 1, LEFT, false );
        synth_data->set_morph_source_data_from_current( 1, RIGHT, true );
        //[/UserButtonCode_button_set_2]
    }
    else if (buttonThatWasClicked == button_set_3)
    {
        //[UserButtonCode_button_set_3] -- add your button handler code here..
        synth_data->set_morph_source_data_from_current( 2, LEFT, false );
        synth_data->set_morph_source_data_from_current( 2, RIGHT, true );
        //[/UserButtonCode_button_set_3]
    }
    else if (buttonThatWasClicked == button_set_4)
    {
        //[UserButtonCode_button_set_4] -- add your button handler code here..
        synth_data->set_morph_source_data_from_current( 3, LEFT, false );
        synth_data->set_morph_source_data_from_current( 3, RIGHT, true );
        //[/UserButtonCode_button_set_4]
    }

    //[UserbuttonClicked_Post]
    refresh_combos();
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_MorphConfig::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_morph_motor_time)
    {
        //[UserSliderCode_slider_morph_motor_time] -- add your slider handling code here..
        synth_data->morph_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_morph_motor_time]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_MorphConfig" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*const ui_refresher_, UiLookAndFeel*const look_and_feel_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;original_w(1465), &#10;original_h(180)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="720">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="1051 0 308 198" cornerSize="10" fill="solid: ffffffee" hasStroke="0"/>
    <ROUNDRECT pos="1051 0 308 200" cornerSize="10" fill=" radial: 1050 180, 1170 120, 0=ffff11ff, 1=0"
               hasStroke="0"/>
    <ROUNDRECT pos="1051 0 308 200" cornerSize="10" fill=" radial: 1360 180, 1250 110, 0=ffffff11, 1=0"
               hasStroke="0"/>
    <ROUNDRECT pos="1051 0 308 200" cornerSize="10" fill=" radial: 1050 0, 1160 60, 0=ffff1111, 1=0"
               hasStroke="0"/>
    <ROUNDRECT pos="1051 0 308 200" cornerSize="10" fill=" radial: 1360 0, 1240 60, 0=ff1111ff, 1=0"
               hasStroke="0"/>
    <ROUNDRECT pos="271 0 258 198" cornerSize="10" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="791 0 258 198" cornerSize="10" fill="solid: ffff11ff" hasStroke="0"/>
    <ROUNDRECT pos="531 0 258 198" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="10 0 259 198" cornerSize="10" fill="solid: ffff1111" hasStroke="0"/>
    <ROUNDRECT pos="1361 0 90 198" cornerSize="10" fill="solid: ffffffee" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="cd3ee10dff65146e" memberName="label_31" virtualName=""
         explicitFocusOrder="0" pos="1270 30 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="da0d8b83db1722b2" memberName="label_32" virtualName=""
         explicitFocusOrder="0" pos="1270 130 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="143c47240065ecd" memberName="label_33" virtualName=""
         explicitFocusOrder="0" pos="1070 130 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d382fc2bef399418" memberName="label_30" virtualName=""
         explicitFocusOrder="0" pos="1070 30 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="447056352a2f62e6" memberName="label_34" virtualName=""
         explicitFocusOrder="0" pos="0Cc -13Cc 80 30" posRelativeX="c1f9aa7ebccd3843"
         posRelativeY="c1f9aa7ebccd3843" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="ALL (L)" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="36"/>
  <COMBOBOX name="" id="6b20aebfc73aac49" memberName="combo_morph_group_1"
            virtualName="" explicitFocusOrder="0" pos="70 90 180 30" tooltip="Load a program to the RIGHT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="feb2297e12b64ba2" memberName="combo_morph_group_2"
            virtualName="" explicitFocusOrder="0" pos="330 90 180 30" tooltip="Load a program to the RIGHT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="308668407147eb6a" memberName="combo_morph_group_3"
            virtualName="" explicitFocusOrder="0" pos="850 90 180 30" tooltip="Load a program to the RIGHT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="6f35e1d484c7ec07" memberName="combo_morph_group_4"
            virtualName="" explicitFocusOrder="0" pos="590 90 180 30" tooltip="Load a program to the RIGHT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="bc6993231c0c71b4" memberName="combo_morph_group_5"
            virtualName="" explicitFocusOrder="0" pos="70 50 180 30" tooltip="Load a program to the LEFT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="bd00414d21da9940" memberName="combo_morph_group_6"
            virtualName="" explicitFocusOrder="0" pos="330 50 180 30" tooltip="Load a program to the LEFT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="dc620a04b2f3c235" memberName="combo_morph_group_7"
            virtualName="" explicitFocusOrder="0" pos="850 50 180 30" tooltip="Load a program to the LEFT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="97e42362463feae7" memberName="combo_morph_group_8"
            virtualName="" explicitFocusOrder="0" pos="590 50 180 30" tooltip="Load a program to the LEFT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <LABEL name="" id="2d5427059e8d821" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="20 50 50 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="LEFT" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="" id="d8906266e2ef4832" memberName="button_set_1" virtualName=""
              explicitFocusOrder="0" pos="70 130 180 30" tooltip="Set the current state as the LEFT and RIGHT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET TO CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="b59f286362d58d43" memberName="label_ui_headline_1"
         virtualName="" explicitFocusOrder="0" pos="10 0 260 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC MORPH (OSC)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="693cc31199788cce" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="270 0 260 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER MORPH (FLT)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="f367122df639fda8" memberName="label_ui_headline_3"
         virtualName="" explicitFocusOrder="0" pos="790 0 260 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX/EQ MORPH (FX)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="43e970e3cf227751" memberName="label_ui_headline_4"
         virtualName="" explicitFocusOrder="0" pos="530 0 260 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP MORPH (ARP)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e7f40caf971a23eb" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="20 90 50 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="RIGHT" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="34"/>
  <LABEL name="" id="53d7da04fb78032a" memberName="label_ui_headline_5"
         virtualName="" explicitFocusOrder="0" pos="0Cc 0 320 30" posRelativeX="c1f9aa7ebccd3843"
         textCol="ff1111ff" edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH PAD (DRAG IT!)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e73285ddadbdaa78" memberName="label_35" virtualName=""
         explicitFocusOrder="0" pos="0Cc 15Cc 130 30" posRelativeX="c1f9aa7ebccd3843"
         posRelativeY="c1f9aa7ebccd3843" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="(DOUBLE CLICK)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="1bbfcd7b95bbf419" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="20 130 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="L&amp;R" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="" id="675a5fd17fb0d02f" memberName="button_set_2" virtualName=""
              explicitFocusOrder="0" pos="330 130 180 30" tooltip="Set the current state as the LEFT and RIGHT side of the FLT morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET TO CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="455b79aac9991718" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="280 50 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="cc65e5b8bd2e839c" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="280 90 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="690291096626b3fb" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="280 130 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="L&amp;R" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="" id="f14e9e820285f4d9" memberName="button_set_3" virtualName=""
              explicitFocusOrder="0" pos="850 130 180 30" tooltip="Set the current state as the LEFT and RIGHT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET TO CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="7d4dc33eb1a70605" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="800 50 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="9bc5565527b0f21c" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="800 90 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="3f40246ec502a912" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="800 130 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="L&amp;R" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="" id="da57b2e3f2f76af2" memberName="button_set_4" virtualName=""
              explicitFocusOrder="0" pos="590 130 180 30" tooltip="Set the current state as the LEFT and RIGHT side of the ARP morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET TO CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="6e1e76eb8c001ce9" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="540 50 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="73f2398305a5f70f" memberName="label_11" virtualName=""
         explicitFocusOrder="0" pos="540 90 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="c86f0fc9f5141d39" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="540 130 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="L&amp;R" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <SLIDER name="0" id="30eb597ef479a74e" memberName="slider_morph_motor_time"
          virtualName="Slider" explicitFocusOrder="0" pos="1370 60 70 60"
          tooltip="Define the MORPH MOTOR TIME.&#10;&#10;Morph motor time is the speed of the morph sliders if you morph via the morph pad or the horizontal slider below the morph sliders"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="be24663ea3b6bd4" memberName="label_ui_headline_6"
         virtualName="" explicitFocusOrder="0" pos="311 0 89 30" posRelativeX="c1f9aa7ebccd3843"
         textCol="ff1111ff" edTextCol="ffff3b00" edBkgCol="0" labelText="SMOOTH"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c1f9aa7ebccd3843" memberName="drag_pad" virtualName="Monique_Ui_DragPad"
                    explicitFocusOrder="0" pos="1050 0 309 180" class="Component"
                    params="ui_refresher"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
