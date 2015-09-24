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
void Monique_Ui_MorphConfig::refresh_combos() noexcept
{
    for( int i = 0 ; i != morph_combos.size() ; ++i )
    {
        ComboBox* box = morph_combos.getUnchecked(i);
	
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
            StringArray morph_program_names( GET_DATA( synth_data ).get_programms( bank_id ) );

            int start_index = box->getNumItems()+1+1;
            box->addItemList( morph_program_names, start_index );

            if( i == 0 )
            {
                bank_start_indexs.add( start_index );
            }
        }

	box->setText(  GET_DATA( synth_data ).get_morph_source_name(i), dontSendNotification );
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_MorphConfig::Monique_Ui_MorphConfig ()
    : original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
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
            TRANS("LEFT PRESET")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredRight);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_left_1 = new TextButton (String::empty));
    button_set_left_1->setTooltip (TRANS("Set the current state as the LEFT side of the OSC morph group.\n"
                                         "\n"
                                         "Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_left_1->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_left_1->addListener (this);
    button_set_left_1->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_1->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_left_2 = new TextButton (String::empty));
    button_set_left_2->setTooltip (TRANS("Set the current state as the LEFT side of the FILTER morph group.\n"
                                         "\n"
                                         "Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_left_2->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_left_2->addListener (this);
    button_set_left_2->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_2->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_left_3 = new TextButton (String::empty));
    button_set_left_3->setTooltip (TRANS("Set the current state as the LEFT side of the FX morph group.\n"
                                         "\n"
                                         "Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_left_3->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_left_3->addListener (this);
    button_set_left_3->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_3->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_left_4 = new TextButton (String::empty));
    button_set_left_4->setTooltip (TRANS("Set the current state as the LEFT side of the ARPEGGIATOR morph group.\n"
                                         "\n"
                                         "Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_left_4->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_left_4->addListener (this);
    button_set_left_4->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_4->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_right_1 = new TextButton (String::empty));
    button_set_right_1->setTooltip (TRANS("Set the current state as the RIGHT side of the OSC morph group.\n"
                                          "\n"
                                          "Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_right_1->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_right_1->addListener (this);
    button_set_right_1->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_1->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_right_2 = new TextButton (String::empty));
    button_set_right_2->setTooltip (TRANS("Set the current state as the RIGHT side of the FILTER morph group.\n"
                                          "\n"
                                          "Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_right_2->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_right_2->addListener (this);
    button_set_right_2->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_2->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_right_3 = new TextButton (String::empty));
    button_set_right_3->setTooltip (TRANS("Set the current state as the RIGHT side of the FX morph group.\n"
                                          "\n"
                                          "Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_right_3->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_right_3->addListener (this);
    button_set_right_3->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_3->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_right_4 = new TextButton (String::empty));
    button_set_right_4->setTooltip (TRANS("Set the current state as the RIGHT side of the ARPEGGIATOR morph group.\n"
                                          "\n"
                                          "Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "));
    button_set_right_4->setButtonText (TRANS("SET FROM CURRENT"));
    button_set_right_4->addListener (this);
    button_set_right_4->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_4->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (button_set_all_from_current = new TextButton (String::empty));
    button_set_all_from_current->setButtonText (TRANS("ALL"));
    button_set_all_from_current->addListener (this);
    button_set_all_from_current->setColour (TextButton::buttonColourId, Colours::black);
    button_set_all_from_current->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_all_from_current->setColour (TextButton::textColourOffId, Colour (0xffffff11));

    addAndMakeVisible (label_ui_headline_1 = new Label ("DL",
            TRANS("OSC MORPH (GROUP 1)")));
    label_ui_headline_1->setFont (Font (30.00f, Font::plain));
    label_ui_headline_1->setJustificationType (Justification::centred);
    label_ui_headline_1->setEditable (false, false, false);
    label_ui_headline_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_2 = new Label ("DL",
            TRANS("FILTER MORPH (GROUP 2)")));
    label_ui_headline_2->setFont (Font (30.00f, Font::plain));
    label_ui_headline_2->setJustificationType (Justification::centred);
    label_ui_headline_2->setEditable (false, false, false);
    label_ui_headline_2->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_3 = new Label ("DL",
            TRANS("FX MORPH (GROUP 3)")));
    label_ui_headline_3->setFont (Font (30.00f, Font::plain));
    label_ui_headline_3->setJustificationType (Justification::centred);
    label_ui_headline_3->setEditable (false, false, false);
    label_ui_headline_3->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_4 = new Label ("DL",
            TRANS("ARP MORPH (GROUP 4)")));
    label_ui_headline_4->setFont (Font (30.00f, Font::plain));
    label_ui_headline_4->setJustificationType (Justification::centred);
    label_ui_headline_4->setEditable (false, false, false);
    label_ui_headline_4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("RIGHT PRESET")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredRight);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("LEFT PRESET")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredRight);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_25 = new Label (String::empty,
            TRANS("RIGHT PRESET")));
    label_25->setFont (Font (30.00f, Font::plain));
    label_25->setJustificationType (Justification::centredRight);
    label_25->setEditable (false, false, false);
    label_25->setColour (Label::textColourId, Colour (0xffff3b00));
    label_25->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_25->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_26 = new Label (String::empty,
            TRANS("LEFT PRESET")));
    label_26->setFont (Font (30.00f, Font::plain));
    label_26->setJustificationType (Justification::centredRight);
    label_26->setEditable (false, false, false);
    label_26->setColour (Label::textColourId, Colour (0xffff3b00));
    label_26->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_26->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_27 = new Label (String::empty,
            TRANS("RIGHT PRESET")));
    label_27->setFont (Font (30.00f, Font::plain));
    label_27->setJustificationType (Justification::centredRight);
    label_27->setEditable (false, false, false);
    label_27->setColour (Label::textColourId, Colour (0xffff3b00));
    label_27->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_27->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_28 = new Label (String::empty,
            TRANS("LEFT PRESET")));
    label_28->setFont (Font (30.00f, Font::plain));
    label_28->setJustificationType (Justification::centredRight);
    label_28->setEditable (false, false, false);
    label_28->setColour (Label::textColourId, Colour (0xffff3b00));
    label_28->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_28->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_29 = new Label (String::empty,
            TRANS("RIGHT PRESET")));
    label_29->setFont (Font (30.00f, Font::plain));
    label_29->setJustificationType (Justification::centredRight);
    label_29->setEditable (false, false, false);
    label_29->setColour (Label::textColourId, Colour (0xffff3b00));
    label_29->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_29->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_5 = new Label ("DL",
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

    addAndMakeVisible (drag_pad = new Monique_Ui_DragPad());


    //[UserPreSize]
    Colour button_off = UiLookAndFeel::getInstance()->colours.button_off_colour;
    button_set_left_1->setColour( TextButton::buttonColourId, button_off );
    button_set_left_2->setColour( TextButton::buttonColourId, button_off );
    button_set_left_3->setColour( TextButton::buttonColourId, button_off );
    button_set_left_4->setColour( TextButton::buttonColourId, button_off );
    button_set_right_1->setColour( TextButton::buttonColourId, button_off );
    button_set_right_2->setColour( TextButton::buttonColourId, button_off );
    button_set_right_3->setColour( TextButton::buttonColourId, button_off );
    button_set_right_4->setColour( TextButton::buttonColourId, button_off );
    button_set_all_from_current->setColour( TextButton::buttonColourId, button_off );

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
        comp->setWantsKeyboardFocus(false);
        comp->setOpaque(true);
    }
    drag_pad->setOpaque(false);
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
    button_set_left_1 = nullptr;
    button_set_left_2 = nullptr;
    button_set_left_3 = nullptr;
    button_set_left_4 = nullptr;
    button_set_right_1 = nullptr;
    button_set_right_2 = nullptr;
    button_set_right_3 = nullptr;
    button_set_right_4 = nullptr;
    button_set_all_from_current = nullptr;
    label_ui_headline_1 = nullptr;
    label_ui_headline_2 = nullptr;
    label_ui_headline_3 = nullptr;
    label_ui_headline_4 = nullptr;
    label_4 = nullptr;
    label_5 = nullptr;
    label_25 = nullptr;
    label_26 = nullptr;
    label_27 = nullptr;
    label_28 = nullptr;
    label_29 = nullptr;
    label_ui_headline_5 = nullptr;
    label_35 = nullptr;
    drag_pad = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_MorphConfig::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (10.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (270.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (530.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (790.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (1050.0f, 10.0f, 405.0f, 150.0f, 6.000f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_MorphConfig::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_31->setBounds (1365, 20, 80, 30);
    label_32->setBounds (1365, 120, 80, 30);
    label_33->setBounds (1060, 120, 80, 30);
    label_30->setBounds (1060, 20, 80, 30);
    label_34->setBounds (1050 + 405 / 2 - (80 / 2), 10 + 150 / 2 - (30 / 2), 80, 30);
    combo_morph_group_1->setBounds (100, 90, 150, 30);
    combo_morph_group_2->setBounds (360, 90, 150, 30);
    combo_morph_group_3->setBounds (620, 90, 150, 30);
    combo_morph_group_4->setBounds (880, 90, 150, 30);
    combo_morph_group_5->setBounds (100, 30, 150, 30);
    combo_morph_group_6->setBounds (360, 30, 150, 30);
    combo_morph_group_7->setBounds (620, 30, 150, 30);
    combo_morph_group_8->setBounds (880, 30, 150, 30);
    label_19->setBounds (20, 30, 80, 30);
    button_set_left_1->setBounds (100, 60, 150, 25);
    button_set_left_2->setBounds (360, 60, 150, 25);
    button_set_left_3->setBounds (620, 60, 150, 25);
    button_set_left_4->setBounds (880, 60, 150, 25);
    button_set_right_1->setBounds (100, 120, 150, 25);
    button_set_right_2->setBounds (360, 120, 150, 25);
    button_set_right_3->setBounds (620, 120, 150, 25);
    button_set_right_4->setBounds (880, 120, 150, 25);
    button_set_all_from_current->setBounds (900, 200, 50, 120);
    label_ui_headline_1->setBounds (50, -4, 180, 35);
    label_ui_headline_2->setBounds (310, -4, 180, 35);
    label_ui_headline_3->setBounds (570, -4, 180, 35);
    label_ui_headline_4->setBounds (830, -4, 180, 35);
    label_4->setBounds (20, 90, 80, 30);
    label_5->setBounds (280, 30, 80, 30);
    label_25->setBounds (280, 90, 80, 30);
    label_26->setBounds (540, 30, 80, 30);
    label_27->setBounds (540, 90, 80, 30);
    label_28->setBounds (800, 30, 80, 30);
    label_29->setBounds (800, 90, 80, 30);
    label_ui_headline_5->setBounds (1050 + 405 / 2 - (180 / 2), -4, 180, 35);
    label_35->setBounds (1050 + 405 / 2 - (130 / 2), 10 + 150 / 2 + 25 - (30 / 2), 130, 30);
    drag_pad->setBounds (1050, 10, 405, 150);
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
        GET_DATA( synth_data ).try_to_load_programm_to_right_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_1]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_2)
    {
        //[UserComboBoxCode_combo_morph_group_2] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_right_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_2]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_3)
    {
        //[UserComboBoxCode_combo_morph_group_3] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_right_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_3]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_4)
    {
        //[UserComboBoxCode_combo_morph_group_4] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_right_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_4]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_5)
    {
        //[UserComboBoxCode_combo_morph_group_5] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_left_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_5]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_6)
    {
        //[UserComboBoxCode_combo_morph_group_6] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_left_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_6]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_7)
    {
        //[UserComboBoxCode_combo_morph_group_7] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_left_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_7]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_8)
    {
        //[UserComboBoxCode_combo_morph_group_8] -- add your combo box handling code here..
        GET_DATA( synth_data ).try_to_load_programm_to_left_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_8]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_MorphConfig::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_set_left_1)
    {
        //[UserButtonCode_button_set_left_1] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 0, LEFT );
        //[/UserButtonCode_button_set_left_1]
    }
    else if (buttonThatWasClicked == button_set_left_2)
    {
        //[UserButtonCode_button_set_left_2] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 1, LEFT );
        //[/UserButtonCode_button_set_left_2]
    }
    else if (buttonThatWasClicked == button_set_left_3)
    {
        //[UserButtonCode_button_set_left_3] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 2, LEFT );
        //[/UserButtonCode_button_set_left_3]
    }
    else if (buttonThatWasClicked == button_set_left_4)
    {
        //[UserButtonCode_button_set_left_4] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 3, LEFT );
        //[/UserButtonCode_button_set_left_4]
    }
    else if (buttonThatWasClicked == button_set_right_1)
    {
        //[UserButtonCode_button_set_right_1] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 0, RIGHT );
        //[/UserButtonCode_button_set_right_1]
    }
    else if (buttonThatWasClicked == button_set_right_2)
    {
        //[UserButtonCode_button_set_right_2] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 1, RIGHT );
        //[/UserButtonCode_button_set_right_2]
    }
    else if (buttonThatWasClicked == button_set_right_3)
    {
        //[UserButtonCode_button_set_right_3] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 2, RIGHT );
        //[/UserButtonCode_button_set_right_3]
    }
    else if (buttonThatWasClicked == button_set_right_4)
    {
        //[UserButtonCode_button_set_right_4] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 3, RIGHT );
        //[/UserButtonCode_button_set_right_4]
    }
    else if (buttonThatWasClicked == button_set_all_from_current)
    {
        //[UserButtonCode_button_set_all_from_current] -- add your button handler code here..
        GET_DATA( synth_data ).set_morph_source_data_from_current( 0, LEFT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 1, LEFT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 2, LEFT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 3, LEFT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 0, RIGHT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 1, RIGHT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 2, RIGHT );
        GET_DATA( synth_data ).set_morph_source_data_from_current( 3, RIGHT );
        //[/UserButtonCode_button_set_all_from_current]
    }

    //[UserbuttonClicked_Post]
    refresh_combos();
    //[/UserbuttonClicked_Post]
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
                 parentClasses="public Component" constructorParams="" variableInitialisers="original_w(1465), original_h(180)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="720">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="10 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="270 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="530 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="790 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="1050 10 405 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
  </BACKGROUND>
  <LABEL name="" id="cd3ee10dff65146e" memberName="label_31" virtualName=""
         explicitFocusOrder="0" pos="1365 20 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="da0d8b83db1722b2" memberName="label_32" virtualName=""
         explicitFocusOrder="0" pos="1365 120 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="143c47240065ecd" memberName="label_33" virtualName=""
         explicitFocusOrder="0" pos="1060 120 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d382fc2bef399418" memberName="label_30" virtualName=""
         explicitFocusOrder="0" pos="1060 20 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC (R)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="447056352a2f62e6" memberName="label_34" virtualName=""
         explicitFocusOrder="0" pos="0Cc 0Cc 80 30" posRelativeX="c1f9aa7ebccd3843"
         posRelativeY="c1f9aa7ebccd3843" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="ALL (L)" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="36"/>
  <COMBOBOX name="" id="6b20aebfc73aac49" memberName="combo_morph_group_1"
            virtualName="" explicitFocusOrder="0" pos="100 90 150 30" tooltip="Load a program to the RIGHT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="feb2297e12b64ba2" memberName="combo_morph_group_2"
            virtualName="" explicitFocusOrder="0" pos="360 90 150 30" tooltip="Load a program to the RIGHT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="308668407147eb6a" memberName="combo_morph_group_3"
            virtualName="" explicitFocusOrder="0" pos="620 90 150 30" tooltip="Load a program to the RIGHT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="6f35e1d484c7ec07" memberName="combo_morph_group_4"
            virtualName="" explicitFocusOrder="0" pos="880 90 150 30" tooltip="Load a program to the RIGHT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="bc6993231c0c71b4" memberName="combo_morph_group_5"
            virtualName="" explicitFocusOrder="0" pos="100 30 150 30" tooltip="Load a program to the LEFT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="bd00414d21da9940" memberName="combo_morph_group_6"
            virtualName="" explicitFocusOrder="0" pos="360 30 150 30" tooltip="Load a program to the LEFT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="dc620a04b2f3c235" memberName="combo_morph_group_7"
            virtualName="" explicitFocusOrder="0" pos="620 30 150 30" tooltip="Load a program to the LEFT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <COMBOBOX name="" id="97e42362463feae7" memberName="combo_morph_group_8"
            virtualName="" explicitFocusOrder="0" pos="880 30 150 30" tooltip="Load a program to the LEFT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
            editable="0" layout="33" items="" textWhenNonSelected="PRESET"
            textWhenNoItems="-"/>
  <LABEL name="" id="2d5427059e8d821" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="20 30 80 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="LEFT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="" id="9a3c3dfa23389bbf" memberName="button_set_left_1"
              virtualName="" explicitFocusOrder="0" pos="100 60 150 25" tooltip="Set the current state as the LEFT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c49e8ed08e43c50b" memberName="button_set_left_2"
              virtualName="" explicitFocusOrder="0" pos="360 60 150 25" tooltip="Set the current state as the LEFT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8097b16b582b5ab8" memberName="button_set_left_3"
              virtualName="" explicitFocusOrder="0" pos="620 60 150 25" tooltip="Set the current state as the LEFT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ebaf45ab7ef39acf" memberName="button_set_left_4"
              virtualName="" explicitFocusOrder="0" pos="880 60 150 25" tooltip="Set the current state as the LEFT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d8906266e2ef4832" memberName="button_set_right_1"
              virtualName="" explicitFocusOrder="0" pos="100 120 150 25" tooltip="Set the current state as the RIGHT side of the OSC morph group.&#10;&#10;Use the OSC morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="edc15917a087fcef" memberName="button_set_right_2"
              virtualName="" explicitFocusOrder="0" pos="360 120 150 25" tooltip="Set the current state as the RIGHT side of the FILTER morph group.&#10;&#10;Use the FLT morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="22e64fa9111b2ae3" memberName="button_set_right_3"
              virtualName="" explicitFocusOrder="0" pos="620 120 150 25" tooltip="Set the current state as the RIGHT side of the FX morph group.&#10;&#10;Use the FX morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ffe2586bcf362e2a" memberName="button_set_right_4"
              virtualName="" explicitFocusOrder="0" pos="880 120 150 25" tooltip="Set the current state as the RIGHT side of the ARPEGGIATOR morph group.&#10;&#10;Use the ARP morph slider on the main user interface to morph between the LEFT and RIGHT side. "
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff11" buttonText="SET FROM CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="273817f565f46996" memberName="button_set_all_from_current"
              virtualName="" explicitFocusOrder="0" pos="900 200 50 120" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff11" buttonText="ALL" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline_1"
         virtualName="" explicitFocusOrder="0" pos="50 -4 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC MORPH (GROUP 1)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="693cc31199788cce" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="310 -4 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER MORPH (GROUP 2)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="f367122df639fda8" memberName="label_ui_headline_3"
         virtualName="" explicitFocusOrder="0" pos="570 -4 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX MORPH (GROUP 3)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="43e970e3cf227751" memberName="label_ui_headline_4"
         virtualName="" explicitFocusOrder="0" pos="830 -4 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP MORPH (GROUP 4)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e7f40caf971a23eb" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="20 90 80 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="RIGHT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="6f6721901c0803b5" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="280 30 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="8ef4024616e54904" memberName="label_25" virtualName=""
         explicitFocusOrder="0" pos="280 90 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="4e147a3a01f0dfb0" memberName="label_26" virtualName=""
         explicitFocusOrder="0" pos="540 30 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="cb04eccf05589130" memberName="label_27" virtualName=""
         explicitFocusOrder="0" pos="540 90 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="12e13283610b9ac0" memberName="label_28" virtualName=""
         explicitFocusOrder="0" pos="800 30 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LEFT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="bda2f409189f3ffa" memberName="label_29" virtualName=""
         explicitFocusOrder="0" pos="800 90 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RIGHT PRESET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="DL" id="53d7da04fb78032a" memberName="label_ui_headline_5"
         virtualName="" explicitFocusOrder="0" pos="0Cc -4 180 35" posRelativeX="c1f9aa7ebccd3843"
         textCol="ff1111ff" edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH PAD (DRAG IT!)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e73285ddadbdaa78" memberName="label_35" virtualName=""
         explicitFocusOrder="0" pos="0Cc 25Cc 130 30" posRelativeX="c1f9aa7ebccd3843"
         posRelativeY="c1f9aa7ebccd3843" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="(DOUBLE CLICK)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c1f9aa7ebccd3843" memberName="drag_pad" virtualName="Monique_Ui_DragPad"
                    explicitFocusOrder="0" pos="1050 10 405 150" class="Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
