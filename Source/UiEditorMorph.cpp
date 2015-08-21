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
#include "UiLookAndFeel.h"
#include "SynthData.h"
#include "UiDragPad.h"
#include "UiEditorSynthLite.h"

//[/Headers]

#include "UiEditorMorph.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorMorph::refresh()
{
    // MORPH 1
    {
        const Array< int >& selections_1 = DATA( synth_data ).get_active_morph_selections( 0 );
        toggle_all_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ALL), dontSendNotification );
        toggle_main_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::MAIN), dontSendNotification );
        toggle_filters_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FILTERS), dontSendNotification );
        toggle_filter_1_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_1), dontSendNotification );
        toggle_filter_2_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_2), dontSendNotification );
        toggle_filter_3_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_3), dontSendNotification );
        toggle_oscs_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::OSCS), dontSendNotification );
        toggle_osc_1_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_1), dontSendNotification );
        toggle_osc_2_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_2), dontSendNotification );
        toggle_osc_3_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_3), dontSendNotification );
        toggle_fm_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FM), dontSendNotification );
        toggle_effects_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::FX), dontSendNotification );
        toggle_arp_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ARP), dontSendNotification );
        toggle_arp_notes_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE), dontSendNotification );
        toggle_arp_velocity_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY), dontSendNotification );
        toggle_arp_glide_shuffle_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE), dontSendNotification );
        toggle_arp_switchs_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS), dontSendNotification );
        toggle_eq_1->setToggleState( selections_1.contains(SynthData::MORPH_SELCTIONS_IDS::EQ), dontSendNotification );
    }

    // MORPH 2
    {
        const Array< int >& selections_2 = DATA( synth_data ).get_active_morph_selections( 1 );
        toggle_all_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ALL), dontSendNotification );
        toggle_main_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::MAIN), dontSendNotification );
        toggle_filters_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FILTERS), dontSendNotification );
        toggle_filter_1_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_1), dontSendNotification );
        toggle_filter_2_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_2), dontSendNotification );
        toggle_filter_3_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_3), dontSendNotification );
        toggle_oscs_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::OSCS), dontSendNotification );
        toggle_osc_1_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_1), dontSendNotification );
        toggle_osc_2_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_2), dontSendNotification );
        toggle_osc_3_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_3), dontSendNotification );
        toggle_fm_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FM), dontSendNotification );
        toggle_effects_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::FX), dontSendNotification );
        toggle_arp_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ARP), dontSendNotification );
        toggle_arp_notes_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE), dontSendNotification );
        toggle_arp_velocity_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY), dontSendNotification );
        toggle_arp_glide_shuffle_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE), dontSendNotification );
        toggle_arp_switchs_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS), dontSendNotification );
        toggle_eq_2->setToggleState( selections_2.contains(SynthData::MORPH_SELCTIONS_IDS::EQ), dontSendNotification );
    }

    // MORPH 3
    {
        const Array< int >& selections_3 = DATA( synth_data ).get_active_morph_selections( 2 );
        toggle_all_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ALL), dontSendNotification );
        toggle_main_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::MAIN), dontSendNotification );
        toggle_filters_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FILTERS), dontSendNotification );
        toggle_filter_1_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_1), dontSendNotification );
        toggle_filter_2_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_2), dontSendNotification );
        toggle_filter_3_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_3), dontSendNotification );
        toggle_oscs_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::OSCS), dontSendNotification );
        toggle_osc_1_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_1), dontSendNotification );
        toggle_osc_2_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_2), dontSendNotification );
        toggle_osc_3_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_3), dontSendNotification );
        toggle_fm_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FM), dontSendNotification );
        toggle_effects_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::FX), dontSendNotification );
        toggle_arp_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ARP), dontSendNotification );
        toggle_arp_notes_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE), dontSendNotification );
        toggle_arp_velocity_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY), dontSendNotification );
        toggle_arp_glide_shuffle_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE), dontSendNotification );
        toggle_arp_switchs_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS), dontSendNotification );
        toggle_eq_3->setToggleState( selections_3.contains(SynthData::MORPH_SELCTIONS_IDS::EQ), dontSendNotification );
    }

    // MORPH 4
    {
        const Array< int >& selections_4 = DATA( synth_data ).get_active_morph_selections( 3 );
        toggle_all_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ALL), dontSendNotification );
        toggle_main_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::MAIN), dontSendNotification );
        toggle_filters_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FILTERS), dontSendNotification );
        toggle_filter_1_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_1), dontSendNotification );
        toggle_filter_2_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_2), dontSendNotification );
        toggle_filter_3_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FILTER_3), dontSendNotification );
        toggle_oscs_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::OSCS), dontSendNotification );
        toggle_osc_1_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_1), dontSendNotification );
        toggle_osc_2_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_2), dontSendNotification );
        toggle_osc_3_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::OSC_3), dontSendNotification );
        toggle_fm_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FM), dontSendNotification );
        toggle_effects_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::FX), dontSendNotification );
        toggle_arp_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ARP), dontSendNotification );
        toggle_arp_notes_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE), dontSendNotification );
        toggle_arp_velocity_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY), dontSendNotification );
        toggle_arp_glide_shuffle_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE), dontSendNotification );
        toggle_arp_switchs_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS), dontSendNotification );
        toggle_eq_4->setToggleState( selections_4.contains(SynthData::MORPH_SELCTIONS_IDS::EQ), dontSendNotification );
    }
}
//[/MiscUserDefs]

//==============================================================================
UiEditorMorph::UiEditorMorph ()
    : original_w(350), original_h(720)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (hit_area_3 = new Component());

    addAndMakeVisible (hit_area_1 = new Component());

    addAndMakeVisible (hit_area_2 = new Component());

    addAndMakeVisible (hit_area_through = new Component());

    addAndMakeVisible (label_g_1 = new Label (String::empty,
                                              TRANS("MORPH 1")));
    label_g_1->setFont (Font (30.00f, Font::plain));
    label_g_1->setJustificationType (Justification::centred);
    label_g_1->setEditable (false, false, false);
    label_g_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_g_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_g_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_1 = new Label (String::empty,
                                            TRANS("ALL")));
    label_1->setFont (Font (30.00f, Font::plain));
    label_1->setJustificationType (Justification::centredRight);
    label_1->setEditable (false, false, false);
    label_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("FILTERS")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredRight);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_all_1 = new ToggleButton (String::empty));
    toggle_all_1->addListener (this);

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("F1")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredRight);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_filters_1 = new ToggleButton (String::empty));
    toggle_filters_1->addListener (this);

    addAndMakeVisible (toggle_filter_1_1 = new ToggleButton (String::empty));
    toggle_filter_1_1->addListener (this);

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("F2")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredRight);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_filter_2_1 = new ToggleButton (String::empty));
    toggle_filter_2_1->addListener (this);

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("F3")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredRight);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_filter_3_1 = new ToggleButton (String::empty));
    toggle_filter_3_1->addListener (this);

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("OSC\'s")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredRight);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("O1")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredRight);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_oscs_1 = new ToggleButton (String::empty));
    toggle_oscs_1->addListener (this);

    addAndMakeVisible (toggle_osc_1_1 = new ToggleButton (String::empty));
    toggle_osc_1_1->addListener (this);

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("O2")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredRight);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_osc_2_1 = new ToggleButton (String::empty));
    toggle_osc_2_1->addListener (this);

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("O3")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredRight);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_osc_3_1 = new ToggleButton (String::empty));
    toggle_osc_3_1->addListener (this);

    addAndMakeVisible (label_10 = new Label (String::empty,
                                             TRANS("FM")));
    label_10->setFont (Font (30.00f, Font::plain));
    label_10->setJustificationType (Justification::centredRight);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_fm_1 = new ToggleButton (String::empty));
    toggle_fm_1->addListener (this);

    addAndMakeVisible (label_11 = new Label (String::empty,
                                             TRANS("FX")));
    label_11->setFont (Font (30.00f, Font::plain));
    label_11->setJustificationType (Justification::centredRight);
    label_11->setEditable (false, false, false);
    label_11->setColour (Label::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_effects_1 = new ToggleButton (String::empty));
    toggle_effects_1->addListener (this);

    addAndMakeVisible (label_15 = new Label (String::empty,
                                             TRANS("ARP")));
    label_15->setFont (Font (30.00f, Font::plain));
    label_15->setJustificationType (Justification::centredRight);
    label_15->setEditable (false, false, false);
    label_15->setColour (Label::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_arp_1 = new ToggleButton (String::empty));
    toggle_arp_1->addListener (this);

    addAndMakeVisible (label_16 = new Label (String::empty,
                                             TRANS("TUNE")));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredRight);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_arp_notes_1 = new ToggleButton (String::empty));
    toggle_arp_notes_1->addListener (this);

    addAndMakeVisible (label_17 = new Label (String::empty,
                                             TRANS("VELOCITY")));
    label_17->setFont (Font (30.00f, Font::plain));
    label_17->setJustificationType (Justification::centredRight);
    label_17->setEditable (false, false, false);
    label_17->setColour (Label::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_arp_velocity_1 = new ToggleButton (String::empty));
    toggle_arp_velocity_1->addListener (this);

    addAndMakeVisible (label_18 = new Label (String::empty,
                                             TRANS("GLIDE/SHFL")));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredRight);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_arp_glide_shuffle_1 = new ToggleButton (String::empty));
    toggle_arp_glide_shuffle_1->addListener (this);

    addAndMakeVisible (label_20 = new Label (String::empty,
                                             TRANS("EQ")));
    label_20->setFont (Font (30.00f, Font::plain));
    label_20->setJustificationType (Justification::centredRight);
    label_20->setEditable (false, false, false);
    label_20->setColour (Label::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_eq_1 = new ToggleButton (String::empty));
    toggle_eq_1->addListener (this);

    addAndMakeVisible (label_g_2 = new Label (String::empty,
                                              TRANS("MORPH 2")));
    label_g_2->setFont (Font (30.00f, Font::plain));
    label_g_2->setJustificationType (Justification::centred);
    label_g_2->setEditable (false, false, false);
    label_g_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_g_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_g_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_g_3 = new Label (String::empty,
                                              TRANS("MORPH 3")));
    label_g_3->setFont (Font (30.00f, Font::plain));
    label_g_3->setJustificationType (Justification::centred);
    label_g_3->setEditable (false, false, false);
    label_g_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_g_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_g_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_g_4 = new Label (String::empty,
                                              TRANS("MORPH 4")));
    label_g_4->setFont (Font (30.00f, Font::plain));
    label_g_4->setJustificationType (Justification::centred);
    label_g_4->setEditable (false, false, false);
    label_g_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_g_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_g_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_morph_group_1 = new ComboBox (String::empty));
    combo_morph_group_1->setEditableText (true);
    combo_morph_group_1->setJustificationType (Justification::centredLeft);
    combo_morph_group_1->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_1->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_1->addListener (this);

    addAndMakeVisible (combo_morph_group_2 = new ComboBox (String::empty));
    combo_morph_group_2->setEditableText (true);
    combo_morph_group_2->setJustificationType (Justification::centredLeft);
    combo_morph_group_2->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_2->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_2->addListener (this);

    addAndMakeVisible (combo_morph_group_3 = new ComboBox (String::empty));
    combo_morph_group_3->setEditableText (true);
    combo_morph_group_3->setJustificationType (Justification::centredLeft);
    combo_morph_group_3->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_3->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_3->addListener (this);

    addAndMakeVisible (combo_morph_group_4 = new ComboBox (String::empty));
    combo_morph_group_4->setEditableText (true);
    combo_morph_group_4->setJustificationType (Justification::centredLeft);
    combo_morph_group_4->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_4->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_4->addListener (this);

    addAndMakeVisible (combo_morph_group_5 = new ComboBox (String::empty));
    combo_morph_group_5->setEditableText (true);
    combo_morph_group_5->setJustificationType (Justification::centredLeft);
    combo_morph_group_5->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_5->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_5->addListener (this);

    addAndMakeVisible (combo_morph_group_6 = new ComboBox (String::empty));
    combo_morph_group_6->setEditableText (true);
    combo_morph_group_6->setJustificationType (Justification::centredLeft);
    combo_morph_group_6->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_6->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_6->addListener (this);

    addAndMakeVisible (combo_morph_group_7 = new ComboBox (String::empty));
    combo_morph_group_7->setEditableText (true);
    combo_morph_group_7->setJustificationType (Justification::centredLeft);
    combo_morph_group_7->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_7->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_7->addListener (this);

    addAndMakeVisible (combo_morph_group_8 = new ComboBox (String::empty));
    combo_morph_group_8->setEditableText (true);
    combo_morph_group_8->setJustificationType (Justification::centredLeft);
    combo_morph_group_8->setTextWhenNothingSelected (TRANS("PRESET"));
    combo_morph_group_8->setTextWhenNoChoicesAvailable (TRANS("-"));
    combo_morph_group_8->addListener (this);

    addAndMakeVisible (label_19 = new Label (String::empty,
                                             TRANS("LOAD LEFT")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredLeft);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_21 = new Label (String::empty,
                                             TRANS("LOAD RIGHT")));
    label_21->setFont (Font (30.00f, Font::plain));
    label_21->setJustificationType (Justification::centredLeft);
    label_21->setEditable (false, false, false);
    label_21->setColour (Label::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_all_2 = new ToggleButton (String::empty));
    toggle_all_2->addListener (this);

    addAndMakeVisible (toggle_filters_2 = new ToggleButton (String::empty));
    toggle_filters_2->addListener (this);

    addAndMakeVisible (toggle_filter_1_2 = new ToggleButton (String::empty));
    toggle_filter_1_2->addListener (this);

    addAndMakeVisible (toggle_filter_2_2 = new ToggleButton (String::empty));
    toggle_filter_2_2->addListener (this);

    addAndMakeVisible (toggle_filter_3_2 = new ToggleButton (String::empty));
    toggle_filter_3_2->addListener (this);

    addAndMakeVisible (toggle_oscs_2 = new ToggleButton (String::empty));
    toggle_oscs_2->addListener (this);

    addAndMakeVisible (toggle_osc_1_2 = new ToggleButton (String::empty));
    toggle_osc_1_2->addListener (this);

    addAndMakeVisible (toggle_osc_2_2 = new ToggleButton (String::empty));
    toggle_osc_2_2->addListener (this);

    addAndMakeVisible (toggle_osc_3_2 = new ToggleButton (String::empty));
    toggle_osc_3_2->addListener (this);

    addAndMakeVisible (toggle_fm_2 = new ToggleButton (String::empty));
    toggle_fm_2->addListener (this);

    addAndMakeVisible (toggle_effects_2 = new ToggleButton (String::empty));
    toggle_effects_2->addListener (this);

    addAndMakeVisible (toggle_arp_2 = new ToggleButton (String::empty));
    toggle_arp_2->addListener (this);

    addAndMakeVisible (toggle_arp_notes_2 = new ToggleButton (String::empty));
    toggle_arp_notes_2->addListener (this);

    addAndMakeVisible (toggle_arp_velocity_2 = new ToggleButton (String::empty));
    toggle_arp_velocity_2->addListener (this);

    addAndMakeVisible (toggle_arp_glide_shuffle_2 = new ToggleButton (String::empty));
    toggle_arp_glide_shuffle_2->addListener (this);

    addAndMakeVisible (toggle_eq_2 = new ToggleButton (String::empty));
    toggle_eq_2->addListener (this);

    addAndMakeVisible (toggle_all_3 = new ToggleButton (String::empty));
    toggle_all_3->addListener (this);

    addAndMakeVisible (toggle_filters_3 = new ToggleButton (String::empty));
    toggle_filters_3->addListener (this);

    addAndMakeVisible (toggle_filter_1_3 = new ToggleButton (String::empty));
    toggle_filter_1_3->addListener (this);

    addAndMakeVisible (toggle_filter_2_3 = new ToggleButton (String::empty));
    toggle_filter_2_3->addListener (this);

    addAndMakeVisible (toggle_filter_3_3 = new ToggleButton (String::empty));
    toggle_filter_3_3->addListener (this);

    addAndMakeVisible (toggle_oscs_3 = new ToggleButton (String::empty));
    toggle_oscs_3->addListener (this);

    addAndMakeVisible (toggle_osc_1_3 = new ToggleButton (String::empty));
    toggle_osc_1_3->addListener (this);

    addAndMakeVisible (toggle_osc_2_3 = new ToggleButton (String::empty));
    toggle_osc_2_3->addListener (this);

    addAndMakeVisible (toggle_osc_3_3 = new ToggleButton (String::empty));
    toggle_osc_3_3->addListener (this);

    addAndMakeVisible (toggle_fm_3 = new ToggleButton (String::empty));
    toggle_fm_3->addListener (this);

    addAndMakeVisible (toggle_effects_3 = new ToggleButton (String::empty));
    toggle_effects_3->addListener (this);

    addAndMakeVisible (toggle_arp_3 = new ToggleButton (String::empty));
    toggle_arp_3->addListener (this);

    addAndMakeVisible (toggle_arp_notes_3 = new ToggleButton (String::empty));
    toggle_arp_notes_3->addListener (this);

    addAndMakeVisible (toggle_arp_velocity_3 = new ToggleButton (String::empty));
    toggle_arp_velocity_3->addListener (this);

    addAndMakeVisible (toggle_arp_glide_shuffle_3 = new ToggleButton (String::empty));
    toggle_arp_glide_shuffle_3->addListener (this);

    addAndMakeVisible (toggle_eq_3 = new ToggleButton (String::empty));
    toggle_eq_3->addListener (this);

    addAndMakeVisible (toggle_all_4 = new ToggleButton (String::empty));
    toggle_all_4->addListener (this);

    addAndMakeVisible (toggle_filters_4 = new ToggleButton (String::empty));
    toggle_filters_4->addListener (this);

    addAndMakeVisible (toggle_filter_1_4 = new ToggleButton (String::empty));
    toggle_filter_1_4->addListener (this);

    addAndMakeVisible (toggle_filter_2_4 = new ToggleButton (String::empty));
    toggle_filter_2_4->addListener (this);

    addAndMakeVisible (toggle_filter_3_4 = new ToggleButton (String::empty));
    toggle_filter_3_4->addListener (this);

    addAndMakeVisible (toggle_oscs_4 = new ToggleButton (String::empty));
    toggle_oscs_4->addListener (this);

    addAndMakeVisible (toggle_osc_1_4 = new ToggleButton (String::empty));
    toggle_osc_1_4->addListener (this);

    addAndMakeVisible (toggle_osc_2_4 = new ToggleButton (String::empty));
    toggle_osc_2_4->addListener (this);

    addAndMakeVisible (toggle_osc_3_4 = new ToggleButton (String::empty));
    toggle_osc_3_4->addListener (this);

    addAndMakeVisible (toggle_fm_4 = new ToggleButton (String::empty));
    toggle_fm_4->addListener (this);

    addAndMakeVisible (toggle_effects_4 = new ToggleButton (String::empty));
    toggle_effects_4->addListener (this);

    addAndMakeVisible (toggle_arp_4 = new ToggleButton (String::empty));
    toggle_arp_4->addListener (this);

    addAndMakeVisible (toggle_arp_notes_4 = new ToggleButton (String::empty));
    toggle_arp_notes_4->addListener (this);

    addAndMakeVisible (toggle_arp_velocity_4 = new ToggleButton (String::empty));
    toggle_arp_velocity_4->addListener (this);

    addAndMakeVisible (toggle_arp_glide_shuffle_4 = new ToggleButton (String::empty));
    toggle_arp_glide_shuffle_4->addListener (this);

    addAndMakeVisible (toggle_eq_4 = new ToggleButton (String::empty));
    toggle_eq_4->addListener (this);

    addAndMakeVisible (label_12 = new Label (String::empty,
                                             TRANS("MAIN")));
    label_12->setFont (Font (30.00f, Font::plain));
    label_12->setJustificationType (Justification::centredRight);
    label_12->setEditable (false, false, false);
    label_12->setColour (Label::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_main_1 = new ToggleButton (String::empty));
    toggle_main_1->addListener (this);

    addAndMakeVisible (toggle_main_2 = new ToggleButton (String::empty));
    toggle_main_2->addListener (this);

    addAndMakeVisible (toggle_main_3 = new ToggleButton (String::empty));
    toggle_main_3->addListener (this);

    addAndMakeVisible (toggle_main_4 = new ToggleButton (String::empty));
    toggle_main_4->addListener (this);

    addAndMakeVisible (label_13 = new Label (String::empty,
                                             TRANS("SET LEFT")));
    label_13->setFont (Font (30.00f, Font::plain));
    label_13->setJustificationType (Justification::centredLeft);
    label_13->setEditable (false, false, false);
    label_13->setColour (Label::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_left_1 = new TextButton (String::empty));
    button_set_left_1->setButtonText (TRANS("CURRENT"));
    button_set_left_1->addListener (this);
    button_set_left_1->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_left_2 = new TextButton (String::empty));
    button_set_left_2->setButtonText (TRANS("CURRENT"));
    button_set_left_2->addListener (this);
    button_set_left_2->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_left_3 = new TextButton (String::empty));
    button_set_left_3->setButtonText (TRANS("CURRENT"));
    button_set_left_3->addListener (this);
    button_set_left_3->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_left_4 = new TextButton (String::empty));
    button_set_left_4->setButtonText (TRANS("CURRENT"));
    button_set_left_4->addListener (this);
    button_set_left_4->setColour (TextButton::buttonColourId, Colours::black);
    button_set_left_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_left_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_14 = new Label (String::empty,
                                             TRANS("SET RIGHT")));
    label_14->setFont (Font (30.00f, Font::plain));
    label_14->setJustificationType (Justification::centredLeft);
    label_14->setEditable (false, false, false);
    label_14->setColour (Label::textColourId, Colour (0xffff3b00));
    label_14->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_set_right_1 = new TextButton (String::empty));
    button_set_right_1->setButtonText (TRANS("CURRENT"));
    button_set_right_1->addListener (this);
    button_set_right_1->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_right_2 = new TextButton (String::empty));
    button_set_right_2->setButtonText (TRANS("CURRENT"));
    button_set_right_2->addListener (this);
    button_set_right_2->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_right_3 = new TextButton (String::empty));
    button_set_right_3->setButtonText (TRANS("CURRENT"));
    button_set_right_3->addListener (this);
    button_set_right_3->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_right_4 = new TextButton (String::empty));
    button_set_right_4->setButtonText (TRANS("CURRENT"));
    button_set_right_4->addListener (this);
    button_set_right_4->setColour (TextButton::buttonColourId, Colours::black);
    button_set_right_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_right_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_set_all_from_current = new TextButton (String::empty));
    button_set_all_from_current->setButtonText (TRANS("SET ALL TO CURRENT"));
    button_set_all_from_current->addListener (this);
    button_set_all_from_current->setColour (TextButton::buttonColourId, Colours::black);
    button_set_all_from_current->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_set_all_from_current->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (track_pad = new UiDragPad());

    addAndMakeVisible (label_23 = new Label (String::empty,
                                             TRANS("SWITCHS")));
    label_23->setFont (Font (30.00f, Font::plain));
    label_23->setJustificationType (Justification::centredRight);
    label_23->setEditable (false, false, false);
    label_23->setColour (Label::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_arp_switchs_1 = new ToggleButton (String::empty));
    toggle_arp_switchs_1->addListener (this);

    addAndMakeVisible (toggle_arp_switchs_2 = new ToggleButton (String::empty));
    toggle_arp_switchs_2->addListener (this);

    addAndMakeVisible (toggle_arp_switchs_3 = new ToggleButton (String::empty));
    toggle_arp_switchs_3->addListener (this);

    addAndMakeVisible (toggle_arp_switchs_4 = new ToggleButton (String::empty));
    toggle_arp_switchs_4->addListener (this);

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setButtonText (TRANS("ESC X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]
    this->setInterceptsMouseClicks( false, true );
    hit_area_through->setInterceptsMouseClicks( false, false );

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

    Array< ComboBox* > morph_combos;
    morph_combos.add( combo_morph_group_1 );
    morph_combos.add( combo_morph_group_2 );
    morph_combos.add( combo_morph_group_3 );
    morph_combos.add( combo_morph_group_4 );
    morph_combos.add( combo_morph_group_5 );
    morph_combos.add( combo_morph_group_6 );
    morph_combos.add( combo_morph_group_7 );
    morph_combos.add( combo_morph_group_8 );
    for( int i = 0 ; i != morph_combos.size() ; ++i ) {
        ComboBox* box = morph_combos.getUnchecked(i);
        for( int bank_id = 0 ; bank_id != 4 ; ++bank_id )
        {
            String bank = bank_id == 0 ? "A" : bank_id == 1 ? "B" : bank_id == 2 ? "C" : "D";
            if( i > 3 )
                box->addSectionHeading(String("Load programm to LEFT from bank: ")+bank);
            else
                box->addSectionHeading(String("Load programm to RIGHT from bank: ")+bank);
            StringArray morph_program_names( DATA( synth_data ).get_programms( bank_id ) );

            int start_index = box->getNumItems()+1;
            box->addItemList( morph_program_names, start_index );

            if( i == 0 )
                bank_start_indexs.add( start_index );
        }

        box->setSelectedItemIndex(0,dontSendNotification);
    }

    for( int i = 0 ; i < getNumChildComponents() ; ++i ) {
        getChildComponent(i)->setPaintingIsUnclipped(true);
    }
    //[/UserPreSize]

    setSize (350, 720);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UiEditorMorph::~UiEditorMorph()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    hit_area_3 = nullptr;
    hit_area_1 = nullptr;
    hit_area_2 = nullptr;
    hit_area_through = nullptr;
    label_g_1 = nullptr;
    label_1 = nullptr;
    label_2 = nullptr;
    toggle_all_1 = nullptr;
    label_3 = nullptr;
    toggle_filters_1 = nullptr;
    toggle_filter_1_1 = nullptr;
    label_4 = nullptr;
    toggle_filter_2_1 = nullptr;
    label_5 = nullptr;
    toggle_filter_3_1 = nullptr;
    label_6 = nullptr;
    label_7 = nullptr;
    toggle_oscs_1 = nullptr;
    toggle_osc_1_1 = nullptr;
    label_8 = nullptr;
    toggle_osc_2_1 = nullptr;
    label_9 = nullptr;
    toggle_osc_3_1 = nullptr;
    label_10 = nullptr;
    toggle_fm_1 = nullptr;
    label_11 = nullptr;
    toggle_effects_1 = nullptr;
    label_15 = nullptr;
    toggle_arp_1 = nullptr;
    label_16 = nullptr;
    toggle_arp_notes_1 = nullptr;
    label_17 = nullptr;
    toggle_arp_velocity_1 = nullptr;
    label_18 = nullptr;
    toggle_arp_glide_shuffle_1 = nullptr;
    label_20 = nullptr;
    toggle_eq_1 = nullptr;
    label_g_2 = nullptr;
    label_g_3 = nullptr;
    label_g_4 = nullptr;
    combo_morph_group_1 = nullptr;
    combo_morph_group_2 = nullptr;
    combo_morph_group_3 = nullptr;
    combo_morph_group_4 = nullptr;
    combo_morph_group_5 = nullptr;
    combo_morph_group_6 = nullptr;
    combo_morph_group_7 = nullptr;
    combo_morph_group_8 = nullptr;
    label_19 = nullptr;
    label_21 = nullptr;
    toggle_all_2 = nullptr;
    toggle_filters_2 = nullptr;
    toggle_filter_1_2 = nullptr;
    toggle_filter_2_2 = nullptr;
    toggle_filter_3_2 = nullptr;
    toggle_oscs_2 = nullptr;
    toggle_osc_1_2 = nullptr;
    toggle_osc_2_2 = nullptr;
    toggle_osc_3_2 = nullptr;
    toggle_fm_2 = nullptr;
    toggle_effects_2 = nullptr;
    toggle_arp_2 = nullptr;
    toggle_arp_notes_2 = nullptr;
    toggle_arp_velocity_2 = nullptr;
    toggle_arp_glide_shuffle_2 = nullptr;
    toggle_eq_2 = nullptr;
    toggle_all_3 = nullptr;
    toggle_filters_3 = nullptr;
    toggle_filter_1_3 = nullptr;
    toggle_filter_2_3 = nullptr;
    toggle_filter_3_3 = nullptr;
    toggle_oscs_3 = nullptr;
    toggle_osc_1_3 = nullptr;
    toggle_osc_2_3 = nullptr;
    toggle_osc_3_3 = nullptr;
    toggle_fm_3 = nullptr;
    toggle_effects_3 = nullptr;
    toggle_arp_3 = nullptr;
    toggle_arp_notes_3 = nullptr;
    toggle_arp_velocity_3 = nullptr;
    toggle_arp_glide_shuffle_3 = nullptr;
    toggle_eq_3 = nullptr;
    toggle_all_4 = nullptr;
    toggle_filters_4 = nullptr;
    toggle_filter_1_4 = nullptr;
    toggle_filter_2_4 = nullptr;
    toggle_filter_3_4 = nullptr;
    toggle_oscs_4 = nullptr;
    toggle_osc_1_4 = nullptr;
    toggle_osc_2_4 = nullptr;
    toggle_osc_3_4 = nullptr;
    toggle_fm_4 = nullptr;
    toggle_effects_4 = nullptr;
    toggle_arp_4 = nullptr;
    toggle_arp_notes_4 = nullptr;
    toggle_arp_velocity_4 = nullptr;
    toggle_arp_glide_shuffle_4 = nullptr;
    toggle_eq_4 = nullptr;
    label_12 = nullptr;
    toggle_main_1 = nullptr;
    toggle_main_2 = nullptr;
    toggle_main_3 = nullptr;
    toggle_main_4 = nullptr;
    label_13 = nullptr;
    button_set_left_1 = nullptr;
    button_set_left_2 = nullptr;
    button_set_left_3 = nullptr;
    button_set_left_4 = nullptr;
    label_14 = nullptr;
    button_set_right_1 = nullptr;
    button_set_right_2 = nullptr;
    button_set_right_3 = nullptr;
    button_set_right_4 = nullptr;
    button_set_all_from_current = nullptr;
    track_pad = nullptr;
    label_23 = nullptr;
    toggle_arp_switchs_1 = nullptr;
    toggle_arp_switchs_2 = nullptr;
    toggle_arp_switchs_3 = nullptr;
    toggle_arp_switchs_4 = nullptr;
    close = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorMorph::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "UiDynamicSizeStart.h"
    //[/UserPrePaint]

    g.setColour (Colours::black);
    g.fillRect (0, 0, 350, 480);

    g.setColour (Colours::black);
    g.fillRect (50, 720 - 110, 300, 110);

    g.setColour (Colours::black);
    g.fillRect (0, 720 - 260, 80, 260);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 40.0f, 1.0f, 273.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 173.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 193.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 93.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 273.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 293.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 313.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 100.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 113.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 133.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 153.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 200.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 233.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 253.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 213.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (115.0f, 320.0f, 1.0f, 74.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 353.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 373.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 333.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (80.0f, 40.0f, 50.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 40.0f, 1.0f, 273.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 173.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 193.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 93.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 273.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 293.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 313.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 100.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 113.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 133.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 153.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 200.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 233.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 253.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 213.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (175.0f, 320.0f, 1.0f, 74.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 353.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 373.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 333.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (140.0f, 40.0f, 50.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 40.0f, 1.0f, 273.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 173.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 193.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 93.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 273.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 293.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 313.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 100.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 113.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 133.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 153.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 200.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 233.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 253.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 213.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (235.0f, 320.0f, 1.0f, 74.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 353.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 373.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 333.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (200.0f, 40.0f, 50.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 40.0f, 1.0f, 273.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 173.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 193.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 93.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 273.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 293.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 313.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 100.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 113.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 133.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 153.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 200.0f, 1.0f, 54.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 233.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 253.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 213.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (295.0f, 320.0f, 1.0f, 74.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 353.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 373.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 333.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 40.0f, 50.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (106.0f, 73.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (166.0f, 73.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (226.0f, 73.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (286.0f, 73.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (116.0f, 393.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (176.0f, 393.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (236.0f, 393.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (296.0f, 393.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath1, PathStrokeType (1.000f, PathStrokeType::mitered, PathStrokeType::rounded));

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath2, PathStrokeType (2.000f));

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void UiEditorMorph::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    hit_area_3->setBounds (80, 610, 270, 110);
    hit_area_1->setBounds (0, 0, 350, 480);
    hit_area_2->setBounds (0, 480, 80, 220);
    hit_area_through->setBounds (80, 480, 270, 130);
    label_g_1->setBounds (75, 10, 60, 30);
    label_1->setBounds (0, 70 - 30, 50, 30);
    label_2->setBounds (20, 210 - 30, 50, 30);
    toggle_all_1->setBounds (100, 45, 30, 20);
    label_3->setBounds (40, 230 - 30, 50, 30);
    toggle_filters_1->setBounds (110, 185, 30, 20);
    toggle_filter_1_1->setBounds (120, 205, 30, 20);
    label_4->setBounds (40, 250 - 30, 50, 30);
    toggle_filter_2_1->setBounds (120, 225, 30, 20);
    label_5->setBounds (40, 270 - 30, 50, 30);
    toggle_filter_3_1->setBounds (120, 245, 30, 20);
    label_6->setBounds (20, 110 - 30, 50, 30);
    label_7->setBounds (40, 130 - 30, 50, 30);
    toggle_oscs_1->setBounds (110, 85, 30, 20);
    toggle_osc_1_1->setBounds (120, 105, 30, 20);
    label_8->setBounds (40, 150 - 30, 50, 30);
    toggle_osc_2_1->setBounds (120, 125, 29, 20);
    label_9->setBounds (40, 170 - 30, 50, 30);
    toggle_osc_3_1->setBounds (120, 145, 30, 20);
    label_10->setBounds (20, 190 - 30, 50, 30);
    toggle_fm_1->setBounds (110, 165, 30, 20);
    label_11->setBounds (20, 310 - 30, 50, 30);
    toggle_effects_1->setBounds (110, 285, 30, 20);
    label_15->setBounds (20, 330 - 30, 50, 30);
    toggle_arp_1->setBounds (110, 305, 30, 20);
    label_16->setBounds (40, 350 - 30, 50, 30);
    toggle_arp_notes_1->setBounds (120, 325, 30, 20);
    label_17->setBounds (40, 370 - 30, 50, 30);
    toggle_arp_velocity_1->setBounds (120, 345, 30, 20);
    label_18->setBounds (40, 390 - 30, 50, 30);
    toggle_arp_glide_shuffle_1->setBounds (120, 365, 30, 20);
    label_20->setBounds (20, 290 - 30, 50, 30);
    toggle_eq_1->setBounds (110, 265, 33, 20);
    label_g_2->setBounds (135, 10, 60, 30);
    label_g_3->setBounds (195, 10, 60, 30);
    label_g_4->setBounds (255, 10, 60, 30);
    combo_morph_group_1->setBounds (150 - 60, 646 - 26, 60, 26);
    combo_morph_group_2->setBounds (210 - 60, 646 - 26, 60, 26);
    combo_morph_group_3->setBounds (270 - 60, 646 - 26, 60, 26);
    combo_morph_group_4->setBounds (330 - 60, 646 - 26, 60, 26);
    combo_morph_group_5->setBounds (150 - 60, 469 - 26, 60, 26);
    combo_morph_group_6->setBounds (210 - 60, 469 - 26, 60, 26);
    combo_morph_group_7->setBounds (270 - 60, 469 - 26, 60, 26);
    combo_morph_group_8->setBounds (330 - 60, 469 - 26, 60, 26);
    label_19->setBounds (20, 472 - 29, 70, 29);
    label_21->setBounds (20, 649 - 29, 70, 29);
    toggle_all_2->setBounds (160, 45, 30, 20);
    toggle_filters_2->setBounds (170, 185, 30, 20);
    toggle_filter_1_2->setBounds (180, 205, 30, 20);
    toggle_filter_2_2->setBounds (180, 225, 30, 20);
    toggle_filter_3_2->setBounds (180, 245, 30, 20);
    toggle_oscs_2->setBounds (170, 85, 30, 20);
    toggle_osc_1_2->setBounds (180, 105, 30, 20);
    toggle_osc_2_2->setBounds (180, 125, 29, 20);
    toggle_osc_3_2->setBounds (180, 145, 30, 20);
    toggle_fm_2->setBounds (170, 165, 30, 20);
    toggle_effects_2->setBounds (170, 285, 30, 20);
    toggle_arp_2->setBounds (170, 305, 30, 20);
    toggle_arp_notes_2->setBounds (180, 325, 30, 20);
    toggle_arp_velocity_2->setBounds (180, 345, 30, 20);
    toggle_arp_glide_shuffle_2->setBounds (180, 365, 30, 20);
    toggle_eq_2->setBounds (170, 265, 33, 20);
    toggle_all_3->setBounds (220, 45, 30, 20);
    toggle_filters_3->setBounds (230, 185, 30, 20);
    toggle_filter_1_3->setBounds (240, 205, 30, 20);
    toggle_filter_2_3->setBounds (240, 225, 30, 20);
    toggle_filter_3_3->setBounds (240, 245, 30, 20);
    toggle_oscs_3->setBounds (230, 85, 30, 20);
    toggle_osc_1_3->setBounds (240, 105, 30, 20);
    toggle_osc_2_3->setBounds (240, 125, 29, 20);
    toggle_osc_3_3->setBounds (240, 145, 30, 20);
    toggle_fm_3->setBounds (230, 165, 30, 20);
    toggle_effects_3->setBounds (230, 285, 30, 20);
    toggle_arp_3->setBounds (230, 305, 30, 20);
    toggle_arp_notes_3->setBounds (240, 325, 30, 20);
    toggle_arp_velocity_3->setBounds (240, 345, 30, 20);
    toggle_arp_glide_shuffle_3->setBounds (240, 365, 30, 20);
    toggle_eq_3->setBounds (230, 265, 33, 20);
    toggle_all_4->setBounds (280, 45, 30, 20);
    toggle_filters_4->setBounds (290, 185, 30, 20);
    toggle_filter_1_4->setBounds (300, 205, 30, 20);
    toggle_filter_2_4->setBounds (300, 225, 30, 20);
    toggle_filter_3_4->setBounds (300, 245, 30, 20);
    toggle_oscs_4->setBounds (290, 85, 30, 20);
    toggle_osc_1_4->setBounds (300, 105, 30, 20);
    toggle_osc_2_4->setBounds (300, 125, 29, 20);
    toggle_osc_3_4->setBounds (300, 145, 30, 20);
    toggle_fm_4->setBounds (290, 165, 30, 20);
    toggle_effects_4->setBounds (290, 285, 30, 20);
    toggle_arp_4->setBounds (290, 305, 30, 20);
    toggle_arp_notes_4->setBounds (300, 325, 30, 20);
    toggle_arp_velocity_4->setBounds (300, 345, 30, 20);
    toggle_arp_glide_shuffle_4->setBounds (300, 365, 30, 20);
    toggle_eq_4->setBounds (290, 265, 33, 20);
    label_12->setBounds (20, 90 - 30, 50, 30);
    toggle_main_1->setBounds (110, 65, 30, 20);
    toggle_main_2->setBounds (170, 65, 30, 20);
    toggle_main_3->setBounds (230, 65, 30, 20);
    toggle_main_4->setBounds (290, 65, 30, 20);
    label_13->setBounds (20, 410, 70, 30);
    button_set_left_1->setBounds (150 - 60, 413, 60, 27);
    button_set_left_2->setBounds (210 - 60, 440 - 27, 60, 27);
    button_set_left_3->setBounds (270 - 60, 440 - 27, 60, 27);
    button_set_left_4->setBounds (330 - 60, 440 - 27, 60, 27);
    label_14->setBounds (20, 646, 70, 30);
    button_set_right_1->setBounds (90, 649, 60, 27);
    button_set_right_2->setBounds (150, 649, 60, 27);
    button_set_right_3->setBounds (210, 649, 60, 27);
    button_set_right_4->setBounds (270, 649, 60, 27);
    button_set_all_from_current->setBounds (90, 680, 240, 27);
    track_pad->setBounds (5, 475, 70, 140);
    label_23->setBounds (40, 410 - 30, 50, 30);
    toggle_arp_switchs_1->setBounds (120, 385, 30, 20);
    toggle_arp_switchs_2->setBounds (180, 385, 30, 20);
    toggle_arp_switchs_3->setBounds (240, 385, 30, 20);
    toggle_arp_switchs_4->setBounds (300, 385, 30, 20);
    close->setBounds (345 - 45, 5, 45, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath (350.0f, 610.0f);
    internalPath1.lineTo (80.0f, 610.0f);
    internalPath1.lineTo (80.0f, 480.0f);
    internalPath1.lineTo (350.0f, 480.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath (350.0f, 610.0f);
    internalPath2.lineTo (350.0f, 720.0f);
    internalPath2.lineTo (0.0f, 720.0f);
    internalPath2.lineTo (0.0f, 0.0f);
    internalPath2.lineTo (350.0f, 0.0f);
    internalPath2.lineTo (350.0f, 480.0f);

    //[UserResized] Add your own custom resize handling here..
#include "UiDynamicSizeEnd.h"
    //[/UserResized]
}

void UiEditorMorph::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggle_all_1)
    {
        //[UserButtonCode_toggle_all_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ALL );
        //[/UserButtonCode_toggle_all_1]
    }
    else if (buttonThatWasClicked == toggle_filters_1)
    {
        //[UserButtonCode_toggle_filters_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FILTERS );
        //[/UserButtonCode_toggle_filters_1]
    }
    else if (buttonThatWasClicked == toggle_filter_1_1)
    {
        //[UserButtonCode_toggle_filter_1_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FILTER_1 );
        //[/UserButtonCode_toggle_filter_1_1]
    }
    else if (buttonThatWasClicked == toggle_filter_2_1)
    {
        //[UserButtonCode_toggle_filter_2_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FILTER_2 );
        //[/UserButtonCode_toggle_filter_2_1]
    }
    else if (buttonThatWasClicked == toggle_filter_3_1)
    {
        //[UserButtonCode_toggle_filter_3_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FILTER_3 );
        //[/UserButtonCode_toggle_filter_3_1]
    }
    else if (buttonThatWasClicked == toggle_oscs_1)
    {
        //[UserButtonCode_toggle_oscs_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::OSCS );
        //[/UserButtonCode_toggle_oscs_1]
    }
    else if (buttonThatWasClicked == toggle_osc_1_1)
    {
        //[UserButtonCode_toggle_osc_1_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::OSC_1 );
        //[/UserButtonCode_toggle_osc_1_1]
    }
    else if (buttonThatWasClicked == toggle_osc_2_1)
    {
        //[UserButtonCode_toggle_osc_2_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::OSC_2 );
        //[/UserButtonCode_toggle_osc_2_1]
    }
    else if (buttonThatWasClicked == toggle_osc_3_1)
    {
        //[UserButtonCode_toggle_osc_3_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::OSC_3 );
        //[/UserButtonCode_toggle_osc_3_1]
    }
    else if (buttonThatWasClicked == toggle_fm_1)
    {
        //[UserButtonCode_toggle_fm_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FM );
        //[/UserButtonCode_toggle_fm_1]
    }
    else if (buttonThatWasClicked == toggle_effects_1)
    {
        //[UserButtonCode_toggle_effects_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::FX );
        //[/UserButtonCode_toggle_effects_1]
    }
    else if (buttonThatWasClicked == toggle_arp_1)
    {
        //[UserButtonCode_toggle_arp_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ARP );
        //[/UserButtonCode_toggle_arp_1]
    }
    else if (buttonThatWasClicked == toggle_arp_notes_1)
    {
        //[UserButtonCode_toggle_arp_notes_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE );
        //[/UserButtonCode_toggle_arp_notes_1]
    }
    else if (buttonThatWasClicked == toggle_arp_velocity_1)
    {
        //[UserButtonCode_toggle_arp_velocity_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY );
        //[/UserButtonCode_toggle_arp_velocity_1]
    }
    else if (buttonThatWasClicked == toggle_arp_glide_shuffle_1)
    {
        //[UserButtonCode_toggle_arp_glide_shuffle_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE );
        //[/UserButtonCode_toggle_arp_glide_shuffle_1]
    }
    else if (buttonThatWasClicked == toggle_eq_1)
    {
        //[UserButtonCode_toggle_eq_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::EQ );
        //[/UserButtonCode_toggle_eq_1]
    }
    else if (buttonThatWasClicked == toggle_all_2)
    {
        //[UserButtonCode_toggle_all_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ALL );
        //[/UserButtonCode_toggle_all_2]
    }
    else if (buttonThatWasClicked == toggle_filters_2)
    {
        //[UserButtonCode_toggle_filters_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FILTERS );
        //[/UserButtonCode_toggle_filters_2]
    }
    else if (buttonThatWasClicked == toggle_filter_1_2)
    {
        //[UserButtonCode_toggle_filter_1_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FILTER_1 );
        //[/UserButtonCode_toggle_filter_1_2]
    }
    else if (buttonThatWasClicked == toggle_filter_2_2)
    {
        //[UserButtonCode_toggle_filter_2_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FILTER_2 );
        //[/UserButtonCode_toggle_filter_2_2]
    }
    else if (buttonThatWasClicked == toggle_filter_3_2)
    {
        //[UserButtonCode_toggle_filter_3_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FILTER_3 );
        //[/UserButtonCode_toggle_filter_3_2]
    }
    else if (buttonThatWasClicked == toggle_oscs_2)
    {
        //[UserButtonCode_toggle_oscs_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::OSCS );
        //[/UserButtonCode_toggle_oscs_2]
    }
    else if (buttonThatWasClicked == toggle_osc_1_2)
    {
        //[UserButtonCode_toggle_osc_1_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::OSC_1 );
        //[/UserButtonCode_toggle_osc_1_2]
    }
    else if (buttonThatWasClicked == toggle_osc_2_2)
    {
        //[UserButtonCode_toggle_osc_2_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::OSC_2 );
        //[/UserButtonCode_toggle_osc_2_2]
    }
    else if (buttonThatWasClicked == toggle_osc_3_2)
    {
        //[UserButtonCode_toggle_osc_3_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::OSC_3 );
        //[/UserButtonCode_toggle_osc_3_2]
    }
    else if (buttonThatWasClicked == toggle_fm_2)
    {
        //[UserButtonCode_toggle_fm_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FM );
        //[/UserButtonCode_toggle_fm_2]
    }
    else if (buttonThatWasClicked == toggle_effects_2)
    {
        //[UserButtonCode_toggle_effects_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::FX );
        //[/UserButtonCode_toggle_effects_2]
    }
    else if (buttonThatWasClicked == toggle_arp_2)
    {
        //[UserButtonCode_toggle_arp_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ARP );
        //[/UserButtonCode_toggle_arp_2]
    }
    else if (buttonThatWasClicked == toggle_arp_notes_2)
    {
        //[UserButtonCode_toggle_arp_notes_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE );
        //[/UserButtonCode_toggle_arp_notes_2]
    }
    else if (buttonThatWasClicked == toggle_arp_velocity_2)
    {
        //[UserButtonCode_toggle_arp_velocity_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY );
        //[/UserButtonCode_toggle_arp_velocity_2]
    }
    else if (buttonThatWasClicked == toggle_arp_glide_shuffle_2)
    {
        //[UserButtonCode_toggle_arp_glide_shuffle_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE );
        //[/UserButtonCode_toggle_arp_glide_shuffle_2]
    }
    else if (buttonThatWasClicked == toggle_eq_2)
    {
        //[UserButtonCode_toggle_eq_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::EQ );
        //[/UserButtonCode_toggle_eq_2]
    }
    else if (buttonThatWasClicked == toggle_all_3)
    {
        //[UserButtonCode_toggle_all_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ALL );
        //[/UserButtonCode_toggle_all_3]
    }
    else if (buttonThatWasClicked == toggle_filters_3)
    {
        //[UserButtonCode_toggle_filters_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FILTERS );
        //[/UserButtonCode_toggle_filters_3]
    }
    else if (buttonThatWasClicked == toggle_filter_1_3)
    {
        //[UserButtonCode_toggle_filter_1_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FILTER_1 );
        //[/UserButtonCode_toggle_filter_1_3]
    }
    else if (buttonThatWasClicked == toggle_filter_2_3)
    {
        //[UserButtonCode_toggle_filter_2_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FILTER_2 );
        //[/UserButtonCode_toggle_filter_2_3]
    }
    else if (buttonThatWasClicked == toggle_filter_3_3)
    {
        //[UserButtonCode_toggle_filter_3_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FILTER_3 );
        //[/UserButtonCode_toggle_filter_3_3]
    }
    else if (buttonThatWasClicked == toggle_oscs_3)
    {
        //[UserButtonCode_toggle_oscs_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::OSCS );
        //[/UserButtonCode_toggle_oscs_3]
    }
    else if (buttonThatWasClicked == toggle_osc_1_3)
    {
        //[UserButtonCode_toggle_osc_1_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::OSC_1 );
        //[/UserButtonCode_toggle_osc_1_3]
    }
    else if (buttonThatWasClicked == toggle_osc_2_3)
    {
        //[UserButtonCode_toggle_osc_2_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::OSC_2 );
        //[/UserButtonCode_toggle_osc_2_3]
    }
    else if (buttonThatWasClicked == toggle_osc_3_3)
    {
        //[UserButtonCode_toggle_osc_3_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::OSC_3 );
        //[/UserButtonCode_toggle_osc_3_3]
    }
    else if (buttonThatWasClicked == toggle_fm_3)
    {
        //[UserButtonCode_toggle_fm_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FM );
        //[/UserButtonCode_toggle_fm_3]
    }
    else if (buttonThatWasClicked == toggle_effects_3)
    {
        //[UserButtonCode_toggle_effects_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::FX );
        //[/UserButtonCode_toggle_effects_3]
    }
    else if (buttonThatWasClicked == toggle_arp_3)
    {
        //[UserButtonCode_toggle_arp_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ARP );
        //[/UserButtonCode_toggle_arp_3]
    }
    else if (buttonThatWasClicked == toggle_arp_notes_3)
    {
        //[UserButtonCode_toggle_arp_notes_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE );
        //[/UserButtonCode_toggle_arp_notes_3]
    }
    else if (buttonThatWasClicked == toggle_arp_velocity_3)
    {
        //[UserButtonCode_toggle_arp_velocity_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY );
        //[/UserButtonCode_toggle_arp_velocity_3]
    }
    else if (buttonThatWasClicked == toggle_arp_glide_shuffle_3)
    {
        //[UserButtonCode_toggle_arp_glide_shuffle_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE );
        //[/UserButtonCode_toggle_arp_glide_shuffle_3]
    }
    else if (buttonThatWasClicked == toggle_eq_3)
    {
        //[UserButtonCode_toggle_eq_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::EQ );
        //[/UserButtonCode_toggle_eq_3]
    }
    else if (buttonThatWasClicked == toggle_all_4)
    {
        //[UserButtonCode_toggle_all_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ALL );
        //[/UserButtonCode_toggle_all_4]
    }
    else if (buttonThatWasClicked == toggle_filters_4)
    {
        //[UserButtonCode_toggle_filters_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FILTERS );
        //[/UserButtonCode_toggle_filters_4]
    }
    else if (buttonThatWasClicked == toggle_filter_1_4)
    {
        //[UserButtonCode_toggle_filter_1_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FILTER_1 );
        //[/UserButtonCode_toggle_filter_1_4]
    }
    else if (buttonThatWasClicked == toggle_filter_2_4)
    {
        //[UserButtonCode_toggle_filter_2_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FILTER_2 );
        //[/UserButtonCode_toggle_filter_2_4]
    }
    else if (buttonThatWasClicked == toggle_filter_3_4)
    {
        //[UserButtonCode_toggle_filter_3_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FILTER_3 );
        //[/UserButtonCode_toggle_filter_3_4]
    }
    else if (buttonThatWasClicked == toggle_oscs_4)
    {
        //[UserButtonCode_toggle_oscs_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::OSCS );
        //[/UserButtonCode_toggle_oscs_4]
    }
    else if (buttonThatWasClicked == toggle_osc_1_4)
    {
        //[UserButtonCode_toggle_osc_1_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::OSC_1 );
        //[/UserButtonCode_toggle_osc_1_4]
    }
    else if (buttonThatWasClicked == toggle_osc_2_4)
    {
        //[UserButtonCode_toggle_osc_2_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::OSC_2 );
        //[/UserButtonCode_toggle_osc_2_4]
    }
    else if (buttonThatWasClicked == toggle_osc_3_4)
    {
        //[UserButtonCode_toggle_osc_3_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::OSC_3 );
        //[/UserButtonCode_toggle_osc_3_4]
    }
    else if (buttonThatWasClicked == toggle_fm_4)
    {
        //[UserButtonCode_toggle_fm_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FM );
        //[/UserButtonCode_toggle_fm_4]
    }
    else if (buttonThatWasClicked == toggle_effects_4)
    {
        //[UserButtonCode_toggle_effects_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::FX );
        //[/UserButtonCode_toggle_effects_4]
    }
    else if (buttonThatWasClicked == toggle_arp_4)
    {
        //[UserButtonCode_toggle_arp_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ARP );
        //[/UserButtonCode_toggle_arp_4]
    }
    else if (buttonThatWasClicked == toggle_arp_notes_4)
    {
        //[UserButtonCode_toggle_arp_notes_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ARP_TUNE );
        //[/UserButtonCode_toggle_arp_notes_4]
    }
    else if (buttonThatWasClicked == toggle_arp_velocity_4)
    {
        //[UserButtonCode_toggle_arp_velocity_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ARP_VELOCITY );
        //[/UserButtonCode_toggle_arp_velocity_4]
    }
    else if (buttonThatWasClicked == toggle_arp_glide_shuffle_4)
    {
        //[UserButtonCode_toggle_arp_glide_shuffle_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ARP_GLIDE_SHUFFLE );
        //[/UserButtonCode_toggle_arp_glide_shuffle_4]
    }
    else if (buttonThatWasClicked == toggle_eq_4)
    {
        //[UserButtonCode_toggle_eq_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::EQ );
        //[/UserButtonCode_toggle_eq_4]
    }
    else if (buttonThatWasClicked == toggle_main_1)
    {
        //[UserButtonCode_toggle_main_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::MAIN );
        //[/UserButtonCode_toggle_main_1]
    }
    else if (buttonThatWasClicked == toggle_main_2)
    {
        //[UserButtonCode_toggle_main_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::MAIN );
        //[/UserButtonCode_toggle_main_2]
    }
    else if (buttonThatWasClicked == toggle_main_3)
    {
        //[UserButtonCode_toggle_main_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::MAIN );
        //[/UserButtonCode_toggle_main_3]
    }
    else if (buttonThatWasClicked == toggle_main_4)
    {
        //[UserButtonCode_toggle_main_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::MAIN );
        //[/UserButtonCode_toggle_main_4]
    }
    else if (buttonThatWasClicked == button_set_left_1)
    {
        //[UserButtonCode_button_set_left_1] -- add your button handler code here..
        DATA( synth_data ).update_left_morph_source( 0 );
        //[/UserButtonCode_button_set_left_1]
    }
    else if (buttonThatWasClicked == button_set_left_2)
    {
        //[UserButtonCode_button_set_left_2] -- add your button handler code here..
        DATA( synth_data ).update_left_morph_source( 1 );
        //[/UserButtonCode_button_set_left_2]
    }
    else if (buttonThatWasClicked == button_set_left_3)
    {
        //[UserButtonCode_button_set_left_3] -- add your button handler code here..
        DATA( synth_data ).update_left_morph_source( 2 );
        //[/UserButtonCode_button_set_left_3]
    }
    else if (buttonThatWasClicked == button_set_left_4)
    {
        //[UserButtonCode_button_set_left_4] -- add your button handler code here..
        DATA( synth_data ).update_left_morph_source( 3 );
        //[/UserButtonCode_button_set_left_4]
    }
    else if (buttonThatWasClicked == button_set_right_1)
    {
        //[UserButtonCode_button_set_right_1] -- add your button handler code here..
        DATA( synth_data ).update_right_morph_source( 0 );
        //[/UserButtonCode_button_set_right_1]
    }
    else if (buttonThatWasClicked == button_set_right_2)
    {
        //[UserButtonCode_button_set_right_2] -- add your button handler code here..
        DATA( synth_data ).update_right_morph_source( 1 );
        //[/UserButtonCode_button_set_right_2]
    }
    else if (buttonThatWasClicked == button_set_right_3)
    {
        //[UserButtonCode_button_set_right_3] -- add your button handler code here..
        DATA( synth_data ).update_right_morph_source( 2 );
        //[/UserButtonCode_button_set_right_3]
    }
    else if (buttonThatWasClicked == button_set_right_4)
    {
        //[UserButtonCode_button_set_right_4] -- add your button handler code here..
        DATA( synth_data ).update_right_morph_source( 3 );
        //[/UserButtonCode_button_set_right_4]
    }
    else if (buttonThatWasClicked == button_set_all_from_current)
    {
        //[UserButtonCode_button_set_all_from_current] -- add your button handler code here..
        DATA( synth_data ).update_left_morph_source( 0 );
        DATA( synth_data ).update_left_morph_source( 1 );
        DATA( synth_data ).update_left_morph_source( 2 );
        DATA( synth_data ).update_left_morph_source( 3 );
        DATA( synth_data ).update_right_morph_source( 0 );
        DATA( synth_data ).update_right_morph_source( 1 );
        DATA( synth_data ).update_right_morph_source( 2 );
        DATA( synth_data ).update_right_morph_source( 3 );
        //[/UserButtonCode_button_set_all_from_current]
    }
    else if (buttonThatWasClicked == toggle_arp_switchs_1)
    {
        //[UserButtonCode_toggle_arp_switchs_1] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 0, SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS );
        //[/UserButtonCode_toggle_arp_switchs_1]
    }
    else if (buttonThatWasClicked == toggle_arp_switchs_2)
    {
        //[UserButtonCode_toggle_arp_switchs_2] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 1, SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS );
        //[/UserButtonCode_toggle_arp_switchs_2]
    }
    else if (buttonThatWasClicked == toggle_arp_switchs_3)
    {
        //[UserButtonCode_toggle_arp_switchs_3] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 2, SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS );
        //[/UserButtonCode_toggle_arp_switchs_3]
    }
    else if (buttonThatWasClicked == toggle_arp_switchs_4)
    {
        //[UserButtonCode_toggle_arp_switchs_4] -- add your button handler code here..
        DATA( synth_data ).activate_morph_selection( 3, SynthData::MORPH_SELCTIONS_IDS::ARP_SWITCHS );
        //[/UserButtonCode_toggle_arp_switchs_4]
    }
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        AppInstanceStore::getInstance()->editor->editor_morph = nullptr;
        return;
        //[/UserButtonCode_close]
    }

    //[UserbuttonClicked_Post]
    refresh();
    //[/UserbuttonClicked_Post]
}

void UiEditorMorph::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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
        DATA( synth_data ).try_to_load_programm_to_right_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_1]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_2)
    {
        //[UserComboBoxCode_combo_morph_group_2] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_right_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_2]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_3)
    {
        //[UserComboBoxCode_combo_morph_group_3] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_right_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_3]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_4)
    {
        //[UserComboBoxCode_combo_morph_group_4] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_right_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_4]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_5)
    {
        //[UserComboBoxCode_combo_morph_group_5] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_left_side( 0, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_5]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_6)
    {
        //[UserComboBoxCode_combo_morph_group_6] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_left_side( 1, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_6]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_7)
    {
        //[UserComboBoxCode_combo_morph_group_7] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_left_side( 2, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_7]
    }
    else if (comboBoxThatHasChanged == combo_morph_group_8)
    {
        //[UserComboBoxCode_combo_morph_group_8] -- add your combo box handling code here..
        DATA( synth_data ).try_to_load_programm_to_left_side( 3, bank, comboBoxThatHasChanged->getSelectedId()-bank_start_indexs[bank] );
        //[/UserComboBoxCode_combo_morph_group_8]
    }

    //[UsercomboBoxChanged_Post]
    refresh();
    //[/UsercomboBoxChanged_Post]
}

bool UiEditorMorph::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        AppInstanceStore::getInstance()->editor->editor_morph = nullptr;
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

<JUCER_COMPONENT documentType="Component" className="UiEditorMorph" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams=""
                 variableInitialisers="original_w(350), original_h(720)" snapPixels="10"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="350" initialHeight="720">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="0">
    <RECT pos="0 0 350 480" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="50 720r 300 110" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="0 720r 80 260" fill="solid: ff000000" hasStroke="0"/>
    <ROUNDRECT pos="106 40 1 273" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 173 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 193 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 93 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 273 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 293 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 313 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 100 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 113 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 133 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 153 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 200 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 233 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 253 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 213 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="115 320 1 74" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 353 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 373 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 333 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="80 40 50 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 40 1 273" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 173 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 193 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 93 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 273 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 293 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 313 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 100 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 113 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 133 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 153 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 200 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 233 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 253 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 213 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="175 320 1 74" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 353 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 373 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 333 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="140 40 50 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 40 1 273" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 173 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 193 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 93 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 273 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 293 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 313 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 100 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 113 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 133 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 153 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 200 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 233 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 253 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 213 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="235 320 1 74" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 353 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 373 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 333 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="200 40 50 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 40 1 273" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 173 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 193 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 93 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 273 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 293 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 313 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 100 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 113 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 133 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 153 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 200 1 54" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 233 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 253 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 213 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="295 320 1 74" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 353 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 373 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 333 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 40 50 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="106 73 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="166 73 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="226 73 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="286 73 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="116 393 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="176 393 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="236 393 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="296 393 5 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, rounded"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 350 610 l 80 610 l 80 480 l 350 480</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 350 610 l 350 720 l 0 720 l 0 0 l 350 0 l 350 480</PATH>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="1071c0eb324a9419" memberName="hit_area_3" virtualName=""
                    explicitFocusOrder="0" pos="80 610 270 110" class="Component"
                    params=""/>
  <GENERICCOMPONENT name="" id="fd4e4a690bbe6b50" memberName="hit_area_1" virtualName=""
                    explicitFocusOrder="0" pos="0 0 350 480" class="Component" params=""/>
  <GENERICCOMPONENT name="" id="7afb75fad7b0007b" memberName="hit_area_2" virtualName=""
                    explicitFocusOrder="0" pos="0 480 80 220" class="Component" params=""/>
  <GENERICCOMPONENT name="" id="e457cfe3632c73db" memberName="hit_area_through" virtualName=""
                    explicitFocusOrder="0" pos="80 480 270 130" class="Component"
                    params=""/>
  <LABEL name="" id="d152f9e6390795d1" memberName="label_g_1" virtualName=""
         explicitFocusOrder="0" pos="75 10 60 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="MORPH 1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="36"/>
  <LABEL name="" id="af53a5122473eec4" memberName="label_1" virtualName=""
         explicitFocusOrder="0" pos="0 70r 50 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="ALL" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="34"/>
  <LABEL name="" id="9e4ab2352c294adf" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="20 210r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="9b95f066f9f18093" memberName="toggle_all_1" virtualName=""
                explicitFocusOrder="0" pos="100 45 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="d17ed12f73d131d7" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="40 230r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="9392ecf44b9f62b" memberName="toggle_filters_1" virtualName=""
                explicitFocusOrder="0" pos="110 185 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="4626ae4fece4e4f6" memberName="toggle_filter_1_1"
                virtualName="" explicitFocusOrder="0" pos="120 205 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="6750057fece88fed" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="40 250r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="ff30b1e3bf54f5f6" memberName="toggle_filter_2_1"
                virtualName="" explicitFocusOrder="0" pos="120 225 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="18b91ba9880d5c7d" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="40 270r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="F3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="ba825f02465c19b0" memberName="toggle_filter_3_1"
                virtualName="" explicitFocusOrder="0" pos="120 245 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="154697bfaebfc2b7" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="20 110r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC's" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="ac27821e5e9cd0be" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="40 130r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="O1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="f68b7def9800d53f" memberName="toggle_oscs_1" virtualName=""
                explicitFocusOrder="0" pos="110 85 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="f8535600c3a61b96" memberName="toggle_osc_1_1" virtualName=""
                explicitFocusOrder="0" pos="120 105 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="8bdb7528cf149af2" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="40 150r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="O2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="cd30be3d51bb714b" memberName="toggle_osc_2_1" virtualName=""
                explicitFocusOrder="0" pos="120 125 29 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="e258c7b0b0ee099c" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="40 170r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="O3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="145267d9a05309c7" memberName="toggle_osc_3_1" virtualName=""
                explicitFocusOrder="0" pos="120 145 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="8e6080ed49e2dc06" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="20 190r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="bb8cf4abee20f4b3" memberName="toggle_fm_1" virtualName=""
                explicitFocusOrder="0" pos="110 165 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="d86362bbd67bc3b5" memberName="label_11" virtualName=""
         explicitFocusOrder="0" pos="20 310r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="b44917150c3713ff" memberName="toggle_effects_1" virtualName=""
                explicitFocusOrder="0" pos="110 285 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="51672d8c72865e76" memberName="label_15" virtualName=""
         explicitFocusOrder="0" pos="20 330r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="3fc97fb27d0a51e1" memberName="toggle_arp_1" virtualName=""
                explicitFocusOrder="0" pos="110 305 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="c675fa0402db4d9b" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="40 350r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="TUNE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="f0264c6396f60086" memberName="toggle_arp_notes_1"
                virtualName="" explicitFocusOrder="0" pos="120 325 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="5f97db588db97125" memberName="label_17" virtualName=""
         explicitFocusOrder="0" pos="40 370r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="VELOCITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="aa172b756bfd096a" memberName="toggle_arp_velocity_1"
                virtualName="" explicitFocusOrder="0" pos="120 345 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="7b0715741d39eef" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="40 390r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLIDE/SHFL" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="b6a78219f3bc40ca" memberName="toggle_arp_glide_shuffle_1"
                virtualName="" explicitFocusOrder="0" pos="120 365 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="3d0dfe15569e33da" memberName="label_20" virtualName=""
         explicitFocusOrder="0" pos="20 290r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="EQ" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="7d12eb2be46f4065" memberName="toggle_eq_1" virtualName=""
                explicitFocusOrder="0" pos="110 265 33 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="e6ddcb9af4c0bb16" memberName="label_g_2" virtualName=""
         explicitFocusOrder="0" pos="135 10 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH 2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="c53c11872a288b8b" memberName="label_g_3" virtualName=""
         explicitFocusOrder="0" pos="195 10 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH 3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="2e66144f7206d2a6" memberName="label_g_4" virtualName=""
         explicitFocusOrder="0" pos="255 10 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH 4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="" id="6b20aebfc73aac49" memberName="combo_morph_group_1"
            virtualName="" explicitFocusOrder="0" pos="150r 646r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="feb2297e12b64ba2" memberName="combo_morph_group_2"
            virtualName="" explicitFocusOrder="0" pos="210r 646r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="308668407147eb6a" memberName="combo_morph_group_3"
            virtualName="" explicitFocusOrder="0" pos="270r 646r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="6f35e1d484c7ec07" memberName="combo_morph_group_4"
            virtualName="" explicitFocusOrder="0" pos="330r 646r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="bc6993231c0c71b4" memberName="combo_morph_group_5"
            virtualName="" explicitFocusOrder="0" pos="150r 469r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="bd00414d21da9940" memberName="combo_morph_group_6"
            virtualName="" explicitFocusOrder="0" pos="210r 469r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="dc620a04b2f3c235" memberName="combo_morph_group_7"
            virtualName="" explicitFocusOrder="0" pos="270r 469r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <COMBOBOX name="" id="97e42362463feae7" memberName="combo_morph_group_8"
            virtualName="" explicitFocusOrder="0" pos="330r 469r 60 26" editable="1"
            layout="33" items="" textWhenNonSelected="PRESET" textWhenNoItems="-"/>
  <LABEL name="" id="2d5427059e8d821" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="20 472r 70 29" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LOAD LEFT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="7561a0dd3b71c4da" memberName="label_21" virtualName=""
         explicitFocusOrder="0" pos="20 649r 70 29" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LOAD RIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="8107e56b455fcb75" memberName="toggle_all_2" virtualName=""
                explicitFocusOrder="0" pos="160 45 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="35a9d279f5bfb639" memberName="toggle_filters_2" virtualName=""
                explicitFocusOrder="0" pos="170 185 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="e943174c58d52233" memberName="toggle_filter_1_2"
                virtualName="" explicitFocusOrder="0" pos="180 205 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="a6c40c38db96cf35" memberName="toggle_filter_2_2"
                virtualName="" explicitFocusOrder="0" pos="180 225 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="1ef764b152f30875" memberName="toggle_filter_3_2"
                virtualName="" explicitFocusOrder="0" pos="180 245 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="fff1e9624e57c0de" memberName="toggle_oscs_2" virtualName=""
                explicitFocusOrder="0" pos="170 85 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="bd6dc94bfdd2cb9" memberName="toggle_osc_1_2" virtualName=""
                explicitFocusOrder="0" pos="180 105 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="55dce6d6555b9228" memberName="toggle_osc_2_2" virtualName=""
                explicitFocusOrder="0" pos="180 125 29 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="f5fded3f717e6c09" memberName="toggle_osc_3_2" virtualName=""
                explicitFocusOrder="0" pos="180 145 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="71c087d00756c73b" memberName="toggle_fm_2" virtualName=""
                explicitFocusOrder="0" pos="170 165 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="dba545b6c9c4085c" memberName="toggle_effects_2" virtualName=""
                explicitFocusOrder="0" pos="170 285 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="f4b0015ac46012a8" memberName="toggle_arp_2" virtualName=""
                explicitFocusOrder="0" pos="170 305 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="35779498c9ebb9da" memberName="toggle_arp_notes_2"
                virtualName="" explicitFocusOrder="0" pos="180 325 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="29e9d8e4b2229e3b" memberName="toggle_arp_velocity_2"
                virtualName="" explicitFocusOrder="0" pos="180 345 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="29e361cb268c327f" memberName="toggle_arp_glide_shuffle_2"
                virtualName="" explicitFocusOrder="0" pos="180 365 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="78686e9d698e028d" memberName="toggle_eq_2" virtualName=""
                explicitFocusOrder="0" pos="170 265 33 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="9c3658d4d2d4e4d8" memberName="toggle_all_3" virtualName=""
                explicitFocusOrder="0" pos="220 45 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="d11aeef4257cd200" memberName="toggle_filters_3" virtualName=""
                explicitFocusOrder="0" pos="230 185 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="a701b79058f530de" memberName="toggle_filter_1_3"
                virtualName="" explicitFocusOrder="0" pos="240 205 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="1a7c63406554356" memberName="toggle_filter_2_3" virtualName=""
                explicitFocusOrder="0" pos="240 225 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="a4aed0c0cc6b530a" memberName="toggle_filter_3_3"
                virtualName="" explicitFocusOrder="0" pos="240 245 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="23b1e3d976dadb6d" memberName="toggle_oscs_3" virtualName=""
                explicitFocusOrder="0" pos="230 85 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="f5fbbbb7addc70d5" memberName="toggle_osc_1_3" virtualName=""
                explicitFocusOrder="0" pos="240 105 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="21d57c6b961636f6" memberName="toggle_osc_2_3" virtualName=""
                explicitFocusOrder="0" pos="240 125 29 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="75eecfd67b81436b" memberName="toggle_osc_3_3" virtualName=""
                explicitFocusOrder="0" pos="240 145 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="7109ccd800347d3f" memberName="toggle_fm_3" virtualName=""
                explicitFocusOrder="0" pos="230 165 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="a430ca8a4c91788d" memberName="toggle_effects_3" virtualName=""
                explicitFocusOrder="0" pos="230 285 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="caf02df7b124d987" memberName="toggle_arp_3" virtualName=""
                explicitFocusOrder="0" pos="230 305 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="718e7bcea1aa5e64" memberName="toggle_arp_notes_3"
                virtualName="" explicitFocusOrder="0" pos="240 325 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="81cd3abe8c827755" memberName="toggle_arp_velocity_3"
                virtualName="" explicitFocusOrder="0" pos="240 345 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="1e0c34c9e922933d" memberName="toggle_arp_glide_shuffle_3"
                virtualName="" explicitFocusOrder="0" pos="240 365 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="92ef84debf7c8926" memberName="toggle_eq_3" virtualName=""
                explicitFocusOrder="0" pos="230 265 33 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="1d2a46f2d2d663c7" memberName="toggle_all_4" virtualName=""
                explicitFocusOrder="0" pos="280 45 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="23a2481eb255612d" memberName="toggle_filters_4" virtualName=""
                explicitFocusOrder="0" pos="290 185 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="831ced9452fef933" memberName="toggle_filter_1_4"
                virtualName="" explicitFocusOrder="0" pos="300 205 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="7d88500a6745d7d4" memberName="toggle_filter_2_4"
                virtualName="" explicitFocusOrder="0" pos="300 225 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="6d22f012fdd1d538" memberName="toggle_filter_3_4"
                virtualName="" explicitFocusOrder="0" pos="300 245 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="11bfd55443391405" memberName="toggle_oscs_4" virtualName=""
                explicitFocusOrder="0" pos="290 85 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="d69c00611258c059" memberName="toggle_osc_1_4" virtualName=""
                explicitFocusOrder="0" pos="300 105 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="2ee7e6fdf0ccfac3" memberName="toggle_osc_2_4" virtualName=""
                explicitFocusOrder="0" pos="300 125 29 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="6372c651734a2cf6" memberName="toggle_osc_3_4" virtualName=""
                explicitFocusOrder="0" pos="300 145 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="d70fb7021eba211" memberName="toggle_fm_4" virtualName=""
                explicitFocusOrder="0" pos="290 165 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="66779a025a1ed2aa" memberName="toggle_effects_4" virtualName=""
                explicitFocusOrder="0" pos="290 285 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="fad27627e74f4a18" memberName="toggle_arp_4" virtualName=""
                explicitFocusOrder="0" pos="290 305 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="49f4531dbbd4f4e2" memberName="toggle_arp_notes_4"
                virtualName="" explicitFocusOrder="0" pos="300 325 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="7f12f3fb215b7347" memberName="toggle_arp_velocity_4"
                virtualName="" explicitFocusOrder="0" pos="300 345 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="74b099088f0308c3" memberName="toggle_arp_glide_shuffle_4"
                virtualName="" explicitFocusOrder="0" pos="300 365 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="c6a099f648f2befd" memberName="toggle_eq_4" virtualName=""
                explicitFocusOrder="0" pos="290 265 33 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="fa1a42600b93896" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="20 90r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="1743ef4b5e9f10e9" memberName="toggle_main_1" virtualName=""
                explicitFocusOrder="0" pos="110 65 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="dae71ebb000a7ced" memberName="toggle_main_2" virtualName=""
                explicitFocusOrder="0" pos="170 65 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="97ade35dcf036153" memberName="toggle_main_3" virtualName=""
                explicitFocusOrder="0" pos="230 65 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="ea6a3c1ec8a1e51c" memberName="toggle_main_4" virtualName=""
                explicitFocusOrder="0" pos="290 65 30 20" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="b58392013c355147" memberName="label_13" virtualName=""
         explicitFocusOrder="0" pos="20 410 70 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SET LEFT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="9a3c3dfa23389bbf" memberName="button_set_left_1"
              virtualName="" explicitFocusOrder="0" pos="150r 413 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c49e8ed08e43c50b" memberName="button_set_left_2"
              virtualName="" explicitFocusOrder="0" pos="210r 440r 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8097b16b582b5ab8" memberName="button_set_left_3"
              virtualName="" explicitFocusOrder="0" pos="270r 440r 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ebaf45ab7ef39acf" memberName="button_set_left_4"
              virtualName="" explicitFocusOrder="0" pos="330r 440r 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="7d20f93c64ce38b7" memberName="label_14" virtualName=""
         explicitFocusOrder="0" pos="20 646 70 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SET RIGHT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="d8906266e2ef4832" memberName="button_set_right_1"
              virtualName="" explicitFocusOrder="0" pos="90 649 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="edc15917a087fcef" memberName="button_set_right_2"
              virtualName="" explicitFocusOrder="0" pos="150 649 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="22e64fa9111b2ae3" memberName="button_set_right_3"
              virtualName="" explicitFocusOrder="0" pos="210 649 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ffe2586bcf362e2a" memberName="button_set_right_4"
              virtualName="" explicitFocusOrder="0" pos="270 649 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="273817f565f46996" memberName="button_set_all_from_current"
              virtualName="" explicitFocusOrder="0" pos="90 680 240 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="SET ALL TO CURRENT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="c1f9aa7ebccd3843" memberName="track_pad" virtualName="UiDragPad"
                    explicitFocusOrder="0" pos="5 475 70 140" class="Component" params=""/>
  <LABEL name="" id="e02c3237c9152618" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="40 410r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SWITCHS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TOGGLEBUTTON name="" id="eaf57d812828aa44" memberName="toggle_arp_switchs_1"
                virtualName="" explicitFocusOrder="0" pos="120 385 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="9e544513218ae01e" memberName="toggle_arp_switchs_2"
                virtualName="" explicitFocusOrder="0" pos="180 385 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="c83aa0b1bbc66815" memberName="toggle_arp_switchs_3"
                virtualName="" explicitFocusOrder="0" pos="240 385 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="c3d3beaacf9b4aaa" memberName="toggle_arp_switchs_4"
                virtualName="" explicitFocusOrder="0" pos="300 385 30 20" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="345r 5 45 20" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
