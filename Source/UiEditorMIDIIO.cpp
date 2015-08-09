/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "UiEditorSynthLite.h"
#include "mono_AudioDeviceManager.h"
#include "UiLookAndFeel.h"
//[/Headers]

#include "UiEditorMIDIIO.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorMIDIIO::refresh()
{
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        if( ComboBox*combo_box = dynamic_cast< ComboBox*>( getChildComponent(i) ) )
        {
            // PORTS
            if( combo_box->getTextWhenNoChoicesAvailable() == "NO DEVICE CONNECTED" )
            {
                combo_box->clear( dontSendNotification );
                combo_box->addItemList( _audio_device_manager->get_available_ports( combo_box->getName() ), 2 );
                combo_box->setText( _audio_device_manager->get_selected_device_name( combo_box->getName() ), dontSendNotification );
                if( _audio_device_manager->is_port_open( combo_box->getName() ) ) {
                    // TODO change colour
                }
            }

            // CHANNELS
            else if( combo_box->getTextWhenNoChoicesAvailable() == "OMNI" )
            {
                combo_box->clear( dontSendNotification );
                for( int i = 0+1 ; i != 16+1 ; ++i ) {
                    combo_box->addItem( String( i ) , i );
                }
            }

            // TYPES
        }
    }
}
//[/MiscUserDefs]

//==============================================================================
UiEditorMIDIIO::UiEditorMIDIIO (mono_AudioDeviceManager*const audio_device_manager_)
    : _audio_device_manager(audio_device_manager_), original_w(1040), original_h(500)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("RECEIVE CC:")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredLeft);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_main = new ComboBox ("RECIEVE_MIDI_MAIN"));
    combo_input_main->setEditableText (false);
    combo_input_main->setJustificationType (Justification::centredLeft);
    combo_input_main->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_input_main->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_input_main->addListener (this);

    addAndMakeVisible (combo_input_main_channel = new ComboBox (String::empty));
    combo_input_main_channel->setEditableText (false);
    combo_input_main_channel->setJustificationType (Justification::centredLeft);
    combo_input_main_channel->setTextWhenNothingSelected (TRANS("CH"));
    combo_input_main_channel->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_input_main_channel->addListener (this);

    addAndMakeVisible (label_1 = new Label (String::empty,
                                            TRANS("PORT")));
    label_1->setFont (Font (30.00f, Font::plain));
    label_1->setJustificationType (Justification::centred);
    label_1->setEditable (false, false, false);
    label_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("INPUT:")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_input_main_thru = new ToggleButton (String::empty));
    toggle_input_main_thru->addListener (this);

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredLeft);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_thru = new ComboBox ("SEND_MIDI_THRU"));
    combo_output_thru->setEditableText (false);
    combo_output_thru->setJustificationType (Justification::centredLeft);
    combo_output_thru->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_thru->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_thru->addListener (this);

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("CC IN:")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_cc = new ComboBox ("RECIEVE_CC"));
    combo_input_cc->setEditableText (false);
    combo_input_cc->setJustificationType (Justification::centredLeft);
    combo_input_cc->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_input_cc->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_input_cc->addListener (this);

    addAndMakeVisible (toggle_input_main_cc = new ToggleButton (String::empty));
    toggle_input_main_cc->addListener (this);

    addAndMakeVisible (toggle_input_cc_thru = new ToggleButton (String::empty));
    toggle_input_cc_thru->addListener (this);

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredLeft);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("CC FB (OUT):")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredLeft);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_cc = new ComboBox ("SEND_MIDI_CC_FEEDBACK"));
    combo_output_cc->setEditableText (false);
    combo_output_cc->setJustificationType (Justification::centredLeft);
    combo_output_cc->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_cc->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_cc->addListener (this);

    addAndMakeVisible (toggle_output_cc_mute = new ToggleButton (String::empty));
    toggle_output_cc_mute->addListener (this);

    addAndMakeVisible (toggle_output_thru_mute = new ToggleButton (String::empty));
    toggle_output_thru_mute->addListener (this);

    addAndMakeVisible (label_11 = new Label (String::empty,
                                             TRANS("LFO 1:")));
    label_11->setFont (Font (30.00f, Font::plain));
    label_11->setJustificationType (Justification::centredLeft);
    label_11->setEditable (false, false, false);
    label_11->setColour (Label::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_lfo_1 = new ComboBox ("SEND_MIDI_LFO_1"));
    combo_output_lfo_1->setEditableText (false);
    combo_output_lfo_1->setJustificationType (Justification::centredLeft);
    combo_output_lfo_1->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_lfo_1->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_lfo_1->addListener (this);

    addAndMakeVisible (combo_output_lfo_channel_1 = new ComboBox (String::empty));
    combo_output_lfo_channel_1->setEditableText (false);
    combo_output_lfo_channel_1->setJustificationType (Justification::centredLeft);
    combo_output_lfo_channel_1->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_lfo_channel_1->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_lfo_channel_1->addListener (this);

    addAndMakeVisible (label_12 = new Label (String::empty,
                                             TRANS("CC-NR")));
    label_12->setFont (Font (30.00f, Font::plain));
    label_12->setJustificationType (Justification::centred);
    label_12->setEditable (false, false, false);
    label_12->setColour (Label::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_output_lfo_mute_1 = new ToggleButton (String::empty));
    toggle_output_lfo_mute_1->addListener (this);

    addAndMakeVisible (combo_output_lfo_number_1 = new ComboBox (String::empty));
    combo_output_lfo_number_1->setEditableText (false);
    combo_output_lfo_number_1->setJustificationType (Justification::centredLeft);
    combo_output_lfo_number_1->setTextWhenNothingSelected (TRANS("T"));
    combo_output_lfo_number_1->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_lfo_number_1->addListener (this);

    addAndMakeVisible (label_15 = new Label (String::empty,
                                             TRANS("LFO 2:")));
    label_15->setFont (Font (30.00f, Font::plain));
    label_15->setJustificationType (Justification::centredLeft);
    label_15->setEditable (false, false, false);
    label_15->setColour (Label::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_lfo_2 = new ComboBox ("SEND_MIDI_LFO_2"));
    combo_output_lfo_2->setEditableText (false);
    combo_output_lfo_2->setJustificationType (Justification::centredLeft);
    combo_output_lfo_2->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_lfo_2->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_lfo_2->addListener (this);

    addAndMakeVisible (combo_output_lfo_channel_2 = new ComboBox (String::empty));
    combo_output_lfo_channel_2->setEditableText (false);
    combo_output_lfo_channel_2->setJustificationType (Justification::centredLeft);
    combo_output_lfo_channel_2->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_lfo_channel_2->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_lfo_channel_2->addListener (this);

    addAndMakeVisible (toggle_output_lfo_mute_2 = new ToggleButton (String::empty));
    toggle_output_lfo_mute_2->addListener (this);

    addAndMakeVisible (combo_output_lfo_number_2 = new ComboBox (String::empty));
    combo_output_lfo_number_2->setEditableText (false);
    combo_output_lfo_number_2->setJustificationType (Justification::centredLeft);
    combo_output_lfo_number_2->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_lfo_number_2->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_lfo_number_2->addListener (this);

    addAndMakeVisible (label_16 = new Label (String::empty,
                                             TRANS("LFO 3:")));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_lfo_3 = new ComboBox ("SEND_MIDI_LFO_3"));
    combo_output_lfo_3->setEditableText (false);
    combo_output_lfo_3->setJustificationType (Justification::centredLeft);
    combo_output_lfo_3->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_lfo_3->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_lfo_3->addListener (this);

    addAndMakeVisible (combo_output_lfo_channel_3 = new ComboBox (String::empty));
    combo_output_lfo_channel_3->setEditableText (false);
    combo_output_lfo_channel_3->setJustificationType (Justification::centredLeft);
    combo_output_lfo_channel_3->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_lfo_channel_3->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_lfo_channel_3->addListener (this);

    addAndMakeVisible (toggle_output_lfo_mute_3 = new ToggleButton (String::empty));
    toggle_output_lfo_mute_3->addListener (this);

    addAndMakeVisible (combo_output_lfo_number_3 = new ComboBox (String::empty));
    combo_output_lfo_number_3->setEditableText (false);
    combo_output_lfo_number_3->setJustificationType (Justification::centredLeft);
    combo_output_lfo_number_3->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_lfo_number_3->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_lfo_number_3->addListener (this);

    addAndMakeVisible (toggle_output_lfo_mute_5 = new ToggleButton (String::empty));
    toggle_output_lfo_mute_5->addListener (this);

    addAndMakeVisible (label_19 = new Label (String::empty,
                                             TRANS("F-ADSR 1:")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredLeft);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_adsr_1 = new ComboBox ("SEND_MIDI_F_ADSR_1"));
    combo_output_adsr_1->setEditableText (false);
    combo_output_adsr_1->setJustificationType (Justification::centredLeft);
    combo_output_adsr_1->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_adsr_1->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_adsr_1->addListener (this);

    addAndMakeVisible (combo_output_adsr_channel_1 = new ComboBox (String::empty));
    combo_output_adsr_channel_1->setEditableText (false);
    combo_output_adsr_channel_1->setJustificationType (Justification::centredLeft);
    combo_output_adsr_channel_1->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_adsr_channel_1->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_adsr_channel_1->addListener (this);

    addAndMakeVisible (toggle_output_adsr_mute_1 = new ToggleButton (String::empty));
    toggle_output_adsr_mute_1->addListener (this);

    addAndMakeVisible (combo_output_adsr_number_1 = new ComboBox (String::empty));
    combo_output_adsr_number_1->setEditableText (false);
    combo_output_adsr_number_1->setJustificationType (Justification::centredLeft);
    combo_output_adsr_number_1->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_adsr_number_1->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_adsr_number_1->addListener (this);

    addAndMakeVisible (label_20 = new Label (String::empty,
                                             TRANS("F-ADSR 2:")));
    label_20->setFont (Font (30.00f, Font::plain));
    label_20->setJustificationType (Justification::centredLeft);
    label_20->setEditable (false, false, false);
    label_20->setColour (Label::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_adsr_2 = new ComboBox ("SEND_MIDI_F_ADSR_2"));
    combo_output_adsr_2->setEditableText (false);
    combo_output_adsr_2->setJustificationType (Justification::centredLeft);
    combo_output_adsr_2->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_adsr_2->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_adsr_2->addListener (this);

    addAndMakeVisible (combo_output_adsr_channel_2 = new ComboBox (String::empty));
    combo_output_adsr_channel_2->setEditableText (false);
    combo_output_adsr_channel_2->setJustificationType (Justification::centredLeft);
    combo_output_adsr_channel_2->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_adsr_channel_2->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_adsr_channel_2->addListener (this);

    addAndMakeVisible (toggle_output_adsr_mute_2 = new ToggleButton (String::empty));
    toggle_output_adsr_mute_2->addListener (this);

    addAndMakeVisible (combo_output_adsr_number_2 = new ComboBox (String::empty));
    combo_output_adsr_number_2->setEditableText (false);
    combo_output_adsr_number_2->setJustificationType (Justification::centredLeft);
    combo_output_adsr_number_2->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_adsr_number_2->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_adsr_number_2->addListener (this);

    addAndMakeVisible (label_21 = new Label (String::empty,
                                             TRANS("F-ADSR 2:")));
    label_21->setFont (Font (30.00f, Font::plain));
    label_21->setJustificationType (Justification::centredLeft);
    label_21->setEditable (false, false, false);
    label_21->setColour (Label::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_adsr_3 = new ComboBox ("SEND_MIDI_F_ADSR_3"));
    combo_output_adsr_3->setEditableText (false);
    combo_output_adsr_3->setJustificationType (Justification::centredLeft);
    combo_output_adsr_3->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_adsr_3->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_adsr_3->addListener (this);

    addAndMakeVisible (combo_output_adsr_channel_3 = new ComboBox (String::empty));
    combo_output_adsr_channel_3->setEditableText (false);
    combo_output_adsr_channel_3->setJustificationType (Justification::centredLeft);
    combo_output_adsr_channel_3->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_adsr_channel_3->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_adsr_channel_3->addListener (this);

    addAndMakeVisible (toggle_output_adsr_mute_3 = new ToggleButton (String::empty));
    toggle_output_adsr_mute_3->addListener (this);

    addAndMakeVisible (combo_output_adsr_number_3 = new ComboBox (String::empty));
    combo_output_adsr_number_3->setEditableText (false);
    combo_output_adsr_number_3->setJustificationType (Justification::centredLeft);
    combo_output_adsr_number_3->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_adsr_number_3->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_adsr_number_3->addListener (this);

    addAndMakeVisible (label_22 = new Label (String::empty,
                                             TRANS("ADSR:")));
    label_22->setFont (Font (30.00f, Font::plain));
    label_22->setJustificationType (Justification::centredLeft);
    label_22->setEditable (false, false, false);
    label_22->setColour (Label::textColourId, Colour (0xffff3b00));
    label_22->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_22->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_adsr_4 = new ComboBox ("SEND_MIDI_ADSR"));
    combo_output_adsr_4->setEditableText (false);
    combo_output_adsr_4->setJustificationType (Justification::centredLeft);
    combo_output_adsr_4->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_adsr_4->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_adsr_4->addListener (this);

    addAndMakeVisible (combo_output_adsr_channel_4 = new ComboBox (String::empty));
    combo_output_adsr_channel_4->setEditableText (false);
    combo_output_adsr_channel_4->setJustificationType (Justification::centredLeft);
    combo_output_adsr_channel_4->setTextWhenNothingSelected (TRANS("CH"));
    combo_output_adsr_channel_4->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_output_adsr_channel_4->addListener (this);

    addAndMakeVisible (toggle_output_adsr_mute_4 = new ToggleButton (String::empty));
    toggle_output_adsr_mute_4->addListener (this);

    addAndMakeVisible (combo_output_adsr_number_4 = new ComboBox (String::empty));
    combo_output_adsr_number_4->setEditableText (false);
    combo_output_adsr_number_4->setJustificationType (Justification::centredLeft);
    combo_output_adsr_number_4->setTextWhenNothingSelected (TRANS("NR"));
    combo_output_adsr_number_4->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_output_adsr_number_4->addListener (this);

    addAndMakeVisible (label_23 = new Label (String::empty,
                                             TRANS("CLOCK:")));
    label_23->setFont (Font (30.00f, Font::plain));
    label_23->setJustificationType (Justification::centredLeft);
    label_23->setEditable (false, false, false);
    label_23->setColour (Label::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_clock = new ComboBox ("SEND_MIDI_CLOCK"));
    combo_output_clock->setEditableText (false);
    combo_output_clock->setJustificationType (Justification::centredLeft);
    combo_output_clock->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_clock->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_clock->addListener (this);

    addAndMakeVisible (toggle_output_clock_mute = new ToggleButton (String::empty));
    toggle_output_clock_mute->addListener (this);

    addAndMakeVisible (label_32 = new Label (String::empty,
                                             TRANS("INPUT:")));
    label_32->setFont (Font (30.00f, Font::plain));
    label_32->setJustificationType (Justification::centredLeft);
    label_32->setEditable (false, false, false);
    label_32->setColour (Label::textColourId, Colour (0xffff3b00));
    label_32->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_32->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_33 = new Label (String::empty,
                                             TRANS("OUTPUT:")));
    label_33->setFont (Font (30.00f, Font::plain));
    label_33->setJustificationType (Justification::centredLeft);
    label_33->setEditable (false, false, false);
    label_33->setColour (Label::textColourId, Colour (0xffff3b00));
    label_33->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_33->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_34 = new Label (String::empty,
                                             TRANS("PORT")));
    label_34->setFont (Font (30.00f, Font::plain));
    label_34->setJustificationType (Justification::centred);
    label_34->setEditable (false, false, false);
    label_34->setColour (Label::textColourId, Colour (0xffff3b00));
    label_34->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_34->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_35 = new Label (String::empty,
                                             TRANS("CH")));
    label_35->setFont (Font (30.00f, Font::plain));
    label_35->setJustificationType (Justification::centred);
    label_35->setEditable (false, false, false);
    label_35->setColour (Label::textColourId, Colour (0xffff3b00));
    label_35->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_35->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_13 = new Label (String::empty,
                                             TRANS("MUTE")));
    label_13->setFont (Font (30.00f, Font::plain));
    label_13->setJustificationType (Justification::centredLeft);
    label_13->setEditable (false, false, false);
    label_13->setColour (Label::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setButtonText (TRANS("ESC X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (label_38 = new Label (String::empty,
                                             TRANS("CC INPUT PICKUP OFFSET")));
    label_38->setFont (Font (15.00f, Font::plain));
    label_38->setJustificationType (Justification::centred);
    label_38->setEditable (false, false, false);
    label_38->setColour (Label::textColourId, Colours::yellow);
    label_38->setColour (TextEditor::textColourId, Colours::black);
    label_38->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_midi_pickup = new Slider ("0"));
    slider_midi_pickup->setRange (0, 1000, 1);
    slider_midi_pickup->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_midi_pickup->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup->addListener (this);

    addAndMakeVisible (button_midi_learn = new TextButton (String::empty));
    button_midi_learn->setButtonText (TRANS("REFRESH"));
    button_midi_learn->addListener (this);
    button_midi_learn->setColour (TextButton::buttonColourId, Colours::black);
    button_midi_learn->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_midi_learn->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_17 = new Label (String::empty,
                                             TRANS("MUTE")));
    label_17->setFont (Font (30.00f, Font::plain));
    label_17->setJustificationType (Justification::centredLeft);
    label_17->setEditable (false, false, false);
    label_17->setColour (Label::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    slider_midi_pickup->setValue( DATA(synth_data).midi_pickup_offset*1000, dontSendNotification );
    refresh();
    //[/UserPreSize]

    setSize (1040, 500);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UiEditorMIDIIO::~UiEditorMIDIIO()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_7 = nullptr;
    combo_input_main = nullptr;
    combo_input_main_channel = nullptr;
    label_1 = nullptr;
    label_3 = nullptr;
    toggle_input_main_thru = nullptr;
    label_4 = nullptr;
    label_5 = nullptr;
    combo_output_thru = nullptr;
    label_6 = nullptr;
    combo_input_cc = nullptr;
    toggle_input_main_cc = nullptr;
    toggle_input_cc_thru = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    combo_output_cc = nullptr;
    toggle_output_cc_mute = nullptr;
    toggle_output_thru_mute = nullptr;
    label_11 = nullptr;
    combo_output_lfo_1 = nullptr;
    combo_output_lfo_channel_1 = nullptr;
    label_12 = nullptr;
    toggle_output_lfo_mute_1 = nullptr;
    combo_output_lfo_number_1 = nullptr;
    label_15 = nullptr;
    combo_output_lfo_2 = nullptr;
    combo_output_lfo_channel_2 = nullptr;
    toggle_output_lfo_mute_2 = nullptr;
    combo_output_lfo_number_2 = nullptr;
    label_16 = nullptr;
    combo_output_lfo_3 = nullptr;
    combo_output_lfo_channel_3 = nullptr;
    toggle_output_lfo_mute_3 = nullptr;
    combo_output_lfo_number_3 = nullptr;
    toggle_output_lfo_mute_5 = nullptr;
    label_19 = nullptr;
    combo_output_adsr_1 = nullptr;
    combo_output_adsr_channel_1 = nullptr;
    toggle_output_adsr_mute_1 = nullptr;
    combo_output_adsr_number_1 = nullptr;
    label_20 = nullptr;
    combo_output_adsr_2 = nullptr;
    combo_output_adsr_channel_2 = nullptr;
    toggle_output_adsr_mute_2 = nullptr;
    combo_output_adsr_number_2 = nullptr;
    label_21 = nullptr;
    combo_output_adsr_3 = nullptr;
    combo_output_adsr_channel_3 = nullptr;
    toggle_output_adsr_mute_3 = nullptr;
    combo_output_adsr_number_3 = nullptr;
    label_22 = nullptr;
    combo_output_adsr_4 = nullptr;
    combo_output_adsr_channel_4 = nullptr;
    toggle_output_adsr_mute_4 = nullptr;
    combo_output_adsr_number_4 = nullptr;
    label_23 = nullptr;
    combo_output_clock = nullptr;
    toggle_output_clock_mute = nullptr;
    label_32 = nullptr;
    label_33 = nullptr;
    label_34 = nullptr;
    label_35 = nullptr;
    label_13 = nullptr;
    close = nullptr;
    label_38 = nullptr;
    slider_midi_pickup = nullptr;
    button_midi_learn = nullptr;
    label_17 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorMIDIIO::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "UiDynamicSizeStart.h"
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour (0xffff3b00));
    g.drawRect (0, 0, 1040, 500, 2);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (20.0f, 118.0f, 570.0f, 4.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (20.0f, 54.0f, 1000.0f, 2.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (610.0f, 305.0f, 410.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (610.0f, 158.0f, 410.0f, 4.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorMIDIIO::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_7->setBounds (530 - 90, 100 - 30, 90, 30);
    combo_input_main->setBounds (240 - 130, 100 - 30, 130, 30);
    combo_input_main_channel->setBounds (310 - 60, 100 - 30, 60, 30);
    label_1->setBounds (240 - 130, 50 - 30, 130, 30);
    label_3->setBounds (20, 100 - 30, 80, 30);
    toggle_input_main_thru->setBounds (400, 70, 30, 30);
    label_4->setBounds (390 - 60, 100 - 30, 60, 30);
    label_5->setBounds (620, 100 - 30, 80, 30);
    combo_output_thru->setBounds (840 - 130, 100 - 30, 130, 30);
    label_6->setBounds (100 - 80, 170 - 30, 80, 30);
    combo_input_cc->setBounds (240 - 130, 170 - 30, 130, 30);
    toggle_input_main_cc->setBounds (540, 70, 30, 30);
    toggle_input_cc_thru->setBounds (400, 140, 30, 30);
    label_8->setBounds (390 - 60, 170 - 30, 60, 30);
    label_9->setBounds (100 - 80, 210 - 30, 80, 30);
    combo_output_cc->setBounds (240 - 130, 210 - 30, 130, 30);
    toggle_output_cc_mute->setBounds (540, 180, 30, 30);
    toggle_output_thru_mute->setBounds (990, 70, 30, 30);
    label_11->setBounds (700 - 80, 210 - 30, 80, 30);
    combo_output_lfo_1->setBounds (840 - 130, 210 - 30, 130, 30);
    combo_output_lfo_channel_1->setBounds (910 - 60, 210 - 30, 60, 30);
    label_12->setBounds (980 - 60, 50 - 30, 60, 30);
    toggle_output_lfo_mute_1->setBounds (990, 180, 30, 30);
    combo_output_lfo_number_1->setBounds (980 - 60, 210 - 30, 60, 30);
    label_15->setBounds (700 - 80, 250 - 30, 80, 30);
    combo_output_lfo_2->setBounds (840 - 130, 250 - 30, 130, 30);
    combo_output_lfo_channel_2->setBounds (910 - 60, 250 - 30, 60, 30);
    toggle_output_lfo_mute_2->setBounds (990, 220, 30, 30);
    combo_output_lfo_number_2->setBounds (980 - 60, 250 - 30, 60, 30);
    label_16->setBounds (700 - 80, 290 - 30, 80, 30);
    combo_output_lfo_3->setBounds (840 - 130, 290 - 30, 130, 30);
    combo_output_lfo_channel_3->setBounds (910 - 60, 290 - 30, 60, 30);
    toggle_output_lfo_mute_3->setBounds (990, 260, 30, 30);
    combo_output_lfo_number_3->setBounds (980 - 60, 290 - 30, 60, 30);
    toggle_output_lfo_mute_5->setBounds (990, 180, 30, 30);
    label_19->setBounds (700 - 80, 350 - 30, 80, 30);
    combo_output_adsr_1->setBounds (840 - 130, 350 - 30, 130, 30);
    combo_output_adsr_channel_1->setBounds (910 - 60, 350 - 30, 60, 30);
    toggle_output_adsr_mute_1->setBounds (990, 321, 30, 30);
    combo_output_adsr_number_1->setBounds (980 - 60, 351 - 30, 60, 30);
    label_20->setBounds (700 - 80, 390 - 30, 80, 30);
    combo_output_adsr_2->setBounds (840 - 130, 390 - 30, 130, 30);
    combo_output_adsr_channel_2->setBounds (910 - 60, 390 - 30, 60, 30);
    toggle_output_adsr_mute_2->setBounds (990, 360, 30, 30);
    combo_output_adsr_number_2->setBounds (980 - 60, 390 - 30, 60, 30);
    label_21->setBounds (700 - 80, 430 - 30, 80, 30);
    combo_output_adsr_3->setBounds (840 - 130, 430 - 30, 130, 30);
    combo_output_adsr_channel_3->setBounds (910 - 60, 430 - 30, 60, 30);
    toggle_output_adsr_mute_3->setBounds (990, 400, 30, 30);
    combo_output_adsr_number_3->setBounds (980 - 60, 430 - 30, 60, 30);
    label_22->setBounds (700 - 80, 470 - 30, 80, 30);
    combo_output_adsr_4->setBounds (840 - 130, 470 - 30, 130, 30);
    combo_output_adsr_channel_4->setBounds (910 - 60, 470 - 30, 60, 30);
    toggle_output_adsr_mute_4->setBounds (990, 440, 30, 30);
    combo_output_adsr_number_4->setBounds (980 - 60, 470 - 30, 60, 30);
    label_23->setBounds (700 - 80, 140 - 30, 80, 30);
    combo_output_clock->setBounds (840 - 130, 140 - 30, 130, 30);
    toggle_output_clock_mute->setBounds (990, 110, 30, 30);
    label_32->setBounds (150 - 130, 50 - 30, 130, 30);
    label_33->setBounds (750 - 130, 50 - 30, 130, 30);
    label_34->setBounds (840 - 130, 50 - 30, 130, 30);
    label_35->setBounds (910 - 60, 50 - 30, 60, 30);
    label_13->setBounds (1040 - 50, 50 - 30, 50, 30);
    close->setBounds (1035 - 45, 5, 45, 20);
    label_38->setBounds (120 - 100, 330 - 30, 100, 30);
    slider_midi_pickup->setBounds (40, 300 - 70, 60, 70);
    button_midi_learn->setBounds (310 - 60, 210 - 30, 60, 30);
    label_17->setBounds (530 - 50, 210 - 30, 50, 30);
    //[UserResized] Add your own custom resize handling here..
#include "UiDynamicSizeEnd.h"
    //[/UserResized]
}

void UiEditorMIDIIO::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_input_main)
    {
        //[UserComboBoxCode_combo_input_main] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_main]
    }
    else if (comboBoxThatHasChanged == combo_input_main_channel)
    {
        //[UserComboBoxCode_combo_input_main_channel] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_input_main_channel]
    }
    else if (comboBoxThatHasChanged == combo_output_thru)
    {
        //[UserComboBoxCode_combo_output_thru] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_thru]
    }
    else if (comboBoxThatHasChanged == combo_input_cc)
    {
        //[UserComboBoxCode_combo_input_cc] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_cc]
    }
    else if (comboBoxThatHasChanged == combo_output_cc)
    {
        //[UserComboBoxCode_combo_output_cc] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_cc]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_1)
    {
        //[UserComboBoxCode_combo_output_lfo_1] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_lfo_1]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_channel_1)
    {
        //[UserComboBoxCode_combo_output_lfo_channel_1] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_channel_1]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_number_1)
    {
        //[UserComboBoxCode_combo_output_lfo_number_1] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_number_1]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_2)
    {
        //[UserComboBoxCode_combo_output_lfo_2] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_lfo_2]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_channel_2)
    {
        //[UserComboBoxCode_combo_output_lfo_channel_2] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_channel_2]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_number_2)
    {
        //[UserComboBoxCode_combo_output_lfo_number_2] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_number_2]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_3)
    {
        //[UserComboBoxCode_combo_output_lfo_3] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_lfo_3]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_channel_3)
    {
        //[UserComboBoxCode_combo_output_lfo_channel_3] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_channel_3]
    }
    else if (comboBoxThatHasChanged == combo_output_lfo_number_3)
    {
        //[UserComboBoxCode_combo_output_lfo_number_3] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_lfo_number_3]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_1)
    {
        //[UserComboBoxCode_combo_output_adsr_1] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_adsr_1]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_channel_1)
    {
        //[UserComboBoxCode_combo_output_adsr_channel_1] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_channel_1]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_number_1)
    {
        //[UserComboBoxCode_combo_output_adsr_number_1] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_number_1]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_2)
    {
        //[UserComboBoxCode_combo_output_adsr_2] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_adsr_2]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_channel_2)
    {
        //[UserComboBoxCode_combo_output_adsr_channel_2] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_channel_2]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_number_2)
    {
        //[UserComboBoxCode_combo_output_adsr_number_2] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_number_2]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_3)
    {
        //[UserComboBoxCode_combo_output_adsr_3] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_adsr_3]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_channel_3)
    {
        //[UserComboBoxCode_combo_output_adsr_channel_3] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_channel_3]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_number_3)
    {
        //[UserComboBoxCode_combo_output_adsr_number_3] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_number_3]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_4)
    {
        //[UserComboBoxCode_combo_output_adsr_4] -- add your combo box handling code here..

        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_adsr_4]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_channel_4)
    {
        //[UserComboBoxCode_combo_output_adsr_channel_4] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_channel_4]
    }
    else if (comboBoxThatHasChanged == combo_output_adsr_number_4)
    {
        //[UserComboBoxCode_combo_output_adsr_number_4] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_output_adsr_number_4]
    }
    else if (comboBoxThatHasChanged == combo_output_clock)
    {
        //[UserComboBoxCode_combo_output_clock] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_clock]
    }

    //[UsercomboBoxChanged_Post]
    refresh();
    //[/UsercomboBoxChanged_Post]
}

void UiEditorMIDIIO::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggle_input_main_thru)
    {
        //[UserButtonCode_toggle_input_main_thru] -- add your button handler code here..
        //[/UserButtonCode_toggle_input_main_thru]
    }
    else if (buttonThatWasClicked == toggle_input_main_cc)
    {
        //[UserButtonCode_toggle_input_main_cc] -- add your button handler code here..
        _audio_device_manager->use_main_input_as_cc = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_input_main_cc]
    }
    else if (buttonThatWasClicked == toggle_input_cc_thru)
    {
        //[UserButtonCode_toggle_input_cc_thru] -- add your button handler code here..
        _audio_device_manager->main_input_thru = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_input_cc_thru]
    }
    else if (buttonThatWasClicked == toggle_output_cc_mute)
    {
        //[UserButtonCode_toggle_output_cc_mute] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_cc_mute]
    }
    else if (buttonThatWasClicked == toggle_output_thru_mute)
    {
        //[UserButtonCode_toggle_output_thru_mute] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_thru_mute]
    }
    else if (buttonThatWasClicked == toggle_output_lfo_mute_1)
    {
        //[UserButtonCode_toggle_output_lfo_mute_1] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_lfo_mute_1]
    }
    else if (buttonThatWasClicked == toggle_output_lfo_mute_2)
    {
        //[UserButtonCode_toggle_output_lfo_mute_2] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_lfo_mute_2]
    }
    else if (buttonThatWasClicked == toggle_output_lfo_mute_3)
    {
        //[UserButtonCode_toggle_output_lfo_mute_3] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_lfo_mute_3]
    }
    else if (buttonThatWasClicked == toggle_output_lfo_mute_5)
    {
        //[UserButtonCode_toggle_output_lfo_mute_5] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_lfo_mute_5]
    }
    else if (buttonThatWasClicked == toggle_output_adsr_mute_1)
    {
        //[UserButtonCode_toggle_output_adsr_mute_1] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_adsr_mute_1]
    }
    else if (buttonThatWasClicked == toggle_output_adsr_mute_2)
    {
        //[UserButtonCode_toggle_output_adsr_mute_2] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_adsr_mute_2]
    }
    else if (buttonThatWasClicked == toggle_output_adsr_mute_3)
    {
        //[UserButtonCode_toggle_output_adsr_mute_3] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_adsr_mute_3]
    }
    else if (buttonThatWasClicked == toggle_output_adsr_mute_4)
    {
        //[UserButtonCode_toggle_output_adsr_mute_4] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_adsr_mute_4]
    }
    else if (buttonThatWasClicked == toggle_output_clock_mute)
    {
        //[UserButtonCode_toggle_output_clock_mute] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_clock_mute]
    }
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        AppInstanceStore::getInstance()->editor->editor_midiio = nullptr;
        return;
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == button_midi_learn)
    {
        //[UserButtonCode_button_midi_learn] -- add your button handler code here..
        //[/UserButtonCode_button_midi_learn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void UiEditorMIDIIO::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_midi_pickup)
    {
        //[UserSliderCode_slider_midi_pickup] -- add your slider handling code here..
        DATA(synth_data).midi_pickup_offset = slider_midi_pickup->getValue()/1000;
        //[/UserSliderCode_slider_midi_pickup]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

bool UiEditorMIDIIO::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        AppInstanceStore::getInstance()->editor->editor_midiio = nullptr;
        return true;
    }
    return success;
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiEditorMIDIIO" componentName=""
                 parentClasses="public Component" constructorParams="mono_AudioDeviceManager*const audio_device_manager_"
                 variableInitialisers="_audio_device_manager(audio_device_manager_), original_w(1040), original_h(500)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1040" initialHeight="500">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="0 0 1040 500" fill="solid: 161616" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="20 118 570 4" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="20 54 1000 2" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="610 305 410 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="610 158 410 4" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="cc90d2b25e08ea4d" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="530r 100r 90 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RECEIVE CC:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="RECIEVE_MIDI_MAIN" id="7c9b1844748d88e" memberName="combo_input_main"
            virtualName="" explicitFocusOrder="0" pos="240r 100r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="f28f9452a84a6616" memberName="combo_input_main_channel"
            virtualName="" explicitFocusOrder="0" pos="310r 100r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <LABEL name="" id="d152f9e6390795d1" memberName="label_1" virtualName=""
         explicitFocusOrder="0" pos="240r 50r 130 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="PORT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="af53a5122473eec4" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="20 100r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="INPUT:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="de81426eb5b7f19d" memberName="toggle_input_main_thru"
                virtualName="" explicitFocusOrder="0" pos="400 70 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="a1c517f047c587f1" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="390r 100r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1b0bd4421c8d9acd" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="620 100r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_THRU" id="8d7dd9d502564afb" memberName="combo_output_thru"
            virtualName="" explicitFocusOrder="0" pos="840r 100r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <LABEL name="" id="9e4ab2352c294adf" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="100r 170r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC IN:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="RECIEVE_CC" id="db50823c1df0e85" memberName="combo_input_cc"
            virtualName="" explicitFocusOrder="0" pos="240r 170r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TOGGLEBUTTON name="" id="9b95f066f9f18093" memberName="toggle_input_main_cc"
                virtualName="" explicitFocusOrder="0" pos="540 70 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="c29d5dd5da0a9644" memberName="toggle_input_cc_thru"
                virtualName="" explicitFocusOrder="0" pos="400 140 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="299bb88252c83194" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="390r 170r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d17ed12f73d131d7" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="100r 210r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC FB (OUT):" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_CC_FEEDBACK" id="997c13a17c6bb37" memberName="combo_output_cc"
            virtualName="" explicitFocusOrder="0" pos="240r 210r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TOGGLEBUTTON name="" id="baa1942f449ba064" memberName="toggle_output_cc_mute"
                virtualName="" explicitFocusOrder="0" pos="540 180 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="91aa4ab6d0e7ab" memberName="toggle_output_thru_mute"
                virtualName="" explicitFocusOrder="0" pos="990 70 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="5b0bb462754cb9a0" memberName="label_11" virtualName=""
         explicitFocusOrder="0" pos="700r 210r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 1:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_LFO_1" id="48b3bc5333b137be" memberName="combo_output_lfo_1"
            virtualName="" explicitFocusOrder="0" pos="840r 210r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="a57ea37a93f88515" memberName="combo_output_lfo_channel_1"
            virtualName="" explicitFocusOrder="0" pos="910r 210r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <LABEL name="" id="b6839897960749c8" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="980r 50r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC-NR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="" id="d046bf082c11845b" memberName="toggle_output_lfo_mute_1"
                virtualName="" explicitFocusOrder="0" pos="990 180 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="aab861147afc05da" memberName="combo_output_lfo_number_1"
            virtualName="" explicitFocusOrder="0" pos="980r 210r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="T" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="670333e40d39bb4b" memberName="label_15" virtualName=""
         explicitFocusOrder="0" pos="700r 250r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_LFO_2" id="c58762e1803460b2" memberName="combo_output_lfo_2"
            virtualName="" explicitFocusOrder="0" pos="840r 250r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="c5742dec71cf7177" memberName="combo_output_lfo_channel_2"
            virtualName="" explicitFocusOrder="0" pos="910r 250r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="7a9dd498428cd6e1" memberName="toggle_output_lfo_mute_2"
                virtualName="" explicitFocusOrder="0" pos="990 220 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="2ff0439f9928fe13" memberName="combo_output_lfo_number_2"
            virtualName="" explicitFocusOrder="0" pos="980r 250r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="c9b8ea70aa4df7c7" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="700r 290r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 3:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_LFO_3" id="45b5aeee7f3eafff" memberName="combo_output_lfo_3"
            virtualName="" explicitFocusOrder="0" pos="840r 290r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="d974ba72119127b5" memberName="combo_output_lfo_channel_3"
            virtualName="" explicitFocusOrder="0" pos="910r 290r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="f38914c5226efbb6" memberName="toggle_output_lfo_mute_3"
                virtualName="" explicitFocusOrder="0" pos="990 260 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="86802ee082763c44" memberName="combo_output_lfo_number_3"
            virtualName="" explicitFocusOrder="0" pos="980r 290r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <TOGGLEBUTTON name="" id="8e19083d33f0b281" memberName="toggle_output_lfo_mute_5"
                virtualName="" explicitFocusOrder="0" pos="990 180 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="35958851e3aad329" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="700r 350r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F-ADSR 1:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_F_ADSR_1" id="5ba4642c2a690b2f" memberName="combo_output_adsr_1"
            virtualName="" explicitFocusOrder="0" pos="840r 350r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="a2625ec01f960d1d" memberName="combo_output_adsr_channel_1"
            virtualName="" explicitFocusOrder="0" pos="910r 350r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="dec1bb8bdeaccead" memberName="toggle_output_adsr_mute_1"
                virtualName="" explicitFocusOrder="0" pos="990 321 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="576c1064e2438be2" memberName="combo_output_adsr_number_1"
            virtualName="" explicitFocusOrder="0" pos="980r 351r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="47b5df99101b343c" memberName="label_20" virtualName=""
         explicitFocusOrder="0" pos="700r 390r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F-ADSR 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_F_ADSR_2" id="1d446cff6cd26165" memberName="combo_output_adsr_2"
            virtualName="" explicitFocusOrder="0" pos="840r 390r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="e1b131a968f27f22" memberName="combo_output_adsr_channel_2"
            virtualName="" explicitFocusOrder="0" pos="910r 390r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="6dfb71a7166e095d" memberName="toggle_output_adsr_mute_2"
                virtualName="" explicitFocusOrder="0" pos="990 360 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="d830be179ff43278" memberName="combo_output_adsr_number_2"
            virtualName="" explicitFocusOrder="0" pos="980r 390r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="278082e173c85311" memberName="label_21" virtualName=""
         explicitFocusOrder="0" pos="700r 430r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F-ADSR 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_F_ADSR_3" id="a5f6da0528b6c6aa" memberName="combo_output_adsr_3"
            virtualName="" explicitFocusOrder="0" pos="840r 430r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="fadb1ff81bace49d" memberName="combo_output_adsr_channel_3"
            virtualName="" explicitFocusOrder="0" pos="910r 430r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="c8567781a691ee4d" memberName="toggle_output_adsr_mute_3"
                virtualName="" explicitFocusOrder="0" pos="990 400 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="c6c25f12e963527d" memberName="combo_output_adsr_number_3"
            virtualName="" explicitFocusOrder="0" pos="980r 430r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="e779770111575bd1" memberName="label_22" virtualName=""
         explicitFocusOrder="0" pos="700r 470r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ADSR:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_ADSR" id="f223104badc610dd" memberName="combo_output_adsr_4"
            virtualName="" explicitFocusOrder="0" pos="840r 470r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="73ecef8bc067454f" memberName="combo_output_adsr_channel_4"
            virtualName="" explicitFocusOrder="0" pos="910r 470r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <TOGGLEBUTTON name="" id="883128e4efc73ab" memberName="toggle_output_adsr_mute_4"
                virtualName="" explicitFocusOrder="0" pos="990 440 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="" id="d7127c5f859b3321" memberName="combo_output_adsr_number_4"
            virtualName="" explicitFocusOrder="0" pos="980r 470r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="EMPTY BANK"/>
  <LABEL name="" id="617ff70d12af87f6" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="700r 140r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CLOCK:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_CLOCK" id="82904a7966365c28" memberName="combo_output_clock"
            virtualName="" explicitFocusOrder="0" pos="840r 140r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TOGGLEBUTTON name="" id="742ea04632aaec4f" memberName="toggle_output_clock_mute"
                virtualName="" explicitFocusOrder="0" pos="990 110 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="4a36eb691d867ee7" memberName="label_32" virtualName=""
         explicitFocusOrder="0" pos="150r 50r 130 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="INPUT:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="af0ae8bf62a45732" memberName="label_33" virtualName=""
         explicitFocusOrder="0" pos="750r 50r 130 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OUTPUT:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="cd4678f9cdb1bfa1" memberName="label_34" virtualName=""
         explicitFocusOrder="0" pos="840r 50r 130 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="PORT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="5c5a95d4039eeae6" memberName="label_35" virtualName=""
         explicitFocusOrder="0" pos="910r 50r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CH" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="b0142f2a67f79820" memberName="label_13" virtualName=""
         explicitFocusOrder="0" pos="1040r 50r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MUTE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="1035r 5 45 20" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label_38" virtualName=""
         explicitFocusOrder="0" pos="120r 330r 100 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="CC INPUT PICKUP OFFSET"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_midi_pickup"
          virtualName="Slider" explicitFocusOrder="0" pos="40 300r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="53f01906b113ce41" memberName="button_midi_learn"
              virtualName="" explicitFocusOrder="0" pos="310r 210r 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="REFRESH"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="833d176af8d85d12" memberName="label_17" virtualName=""
         explicitFocusOrder="0" pos="530r 210r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MUTE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
