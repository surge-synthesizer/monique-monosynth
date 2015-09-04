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
#include "monique_core_Datastructures.h"
//[/Headers]

#include "monique_ui_ADRDesigner.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorSettings::refresh() noexcept
{
    float sample_rate = RuntimeNotifyer::getInstance()->get_sample_rate();

    slider_attack_1->setValue( env_preset_def->attack_1*10000, dontSendNotification );
    slider_attack_2->setValue( env_preset_def->attack_2*10000, dontSendNotification );
    slider_attack_3->setValue( env_preset_def->attack_3*10000, dontSendNotification );
    slider_attack_4->setValue( env_preset_def->attack_4*10000, dontSendNotification );
    label_attack_1->setText(String( MIN_ENV_TIMES + env_preset_def->attack_1*10000 )+String("ms"), dontSendNotification);
    label_attack_2->setText(String( MIN_ENV_TIMES + env_preset_def->attack_2*10000 )+String("ms"), dontSendNotification);
    label_attack_3->setText(String( MIN_ENV_TIMES + env_preset_def->attack_3*10000 )+String("ms"), dontSendNotification);
    label_attack_4->setText(String( MIN_ENV_TIMES + env_preset_def->attack_4*10000 )+String("ms"), dontSendNotification);

    slider_decay_1->setValue( env_preset_def->decay_1*10000, dontSendNotification );
    slider_decay_2->setValue( env_preset_def->decay_2*10000, dontSendNotification );
    slider_decay_3->setValue( env_preset_def->decay_3*10000, dontSendNotification );
    slider_decay_4->setValue( env_preset_def->decay_4*10000, dontSendNotification );
    label_decay_1->setText(String( env_preset_def->decay_1*10000 )+String("ms"), dontSendNotification);
    label_decay_2->setText(String( env_preset_def->decay_2*10000 )+String("ms"), dontSendNotification);
    label_decay_3->setText(String( env_preset_def->decay_3*10000 )+String("ms"), dontSendNotification);
    label_decay_4->setText(String( env_preset_def->decay_4*10000 )+String("ms"), dontSendNotification);

    slider_sustain_time_1->setValue( env_preset_def->sustain_time_1*10000, dontSendNotification );
    slider_sustain_time_2->setValue( env_preset_def->sustain_time_2*10000, dontSendNotification );
    slider_sustain_time_3->setValue( env_preset_def->sustain_time_3*10000, dontSendNotification );
    slider_sustain_time_4->setValue( env_preset_def->sustain_time_4*10000, dontSendNotification );
    label_sustain_time_1->setText(String( env_preset_def->sustain_time_1*10000 ), dontSendNotification);
    label_sustain_time_2->setText(String( env_preset_def->sustain_time_2*10000 ), dontSendNotification);
    label_sustain_time_3->setText(String( env_preset_def->sustain_time_3*10000 ), dontSendNotification);
    label_sustain_time_4->setText(String( env_preset_def->sustain_time_4*10000 ), dontSendNotification);

    slider_release_1->setValue( env_preset_def->release_1*10000, dontSendNotification );
    slider_release_2->setValue( env_preset_def->release_2*10000, dontSendNotification );
    slider_release_3->setValue( env_preset_def->release_3*10000, dontSendNotification );
    slider_release_4->setValue( env_preset_def->release_4*10000, dontSendNotification );
    label_release_1->setText(String( MIN_ENV_TIMES + env_preset_def->release_1*10000 )+String("ms"), dontSendNotification);
    label_release_2->setText(String( MIN_ENV_TIMES + env_preset_def->release_2*10000 )+String("ms"), dontSendNotification);
    label_release_3->setText(String( MIN_ENV_TIMES + env_preset_def->release_3*10000 )+String("ms"), dontSendNotification);
    label_release_4->setText(String( MIN_ENV_TIMES + env_preset_def->release_4*10000 )+String("ms"), dontSendNotification);

    SynthData* synth_data( GET_DATA_PTR( synth_data ) );
    slider_force_to_zero->setValue( synth_data->force_envs_to_zero, dontSendNotification );
    slider_env_shape->setValue( synth_data->curve_shape,sendNotification);

    if( last_repaint_counter != repaint_counter )
    {
        last_repaint_counter = repaint_counter;
        repaint(plotter->getBounds());
    }
}
//[/MiscUserDefs]

//==============================================================================
UiEditorSettings::UiEditorSettings ()
    : original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    env_preset_def = GET_DATA_PTR( env_preset_def );
    //[/Constructor_pre]

    addAndMakeVisible (label_ui_headline_4 = new Label ("DL",
                                                        TRANS("GROUP 4 (MIX: 3 to 4)")));
    label_ui_headline_4->setFont (Font (30.00f, Font::plain));
    label_ui_headline_4->setJustificationType (Justification::centred);
    label_ui_headline_4->setEditable (false, false, false);
    label_ui_headline_4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_3 = new Label ("DL",
                                                        TRANS("GROUP 3 (MIX: 2 to 4)")));
    label_ui_headline_3->setFont (Font (30.00f, Font::plain));
    label_ui_headline_3->setJustificationType (Justification::centred);
    label_ui_headline_3->setEditable (false, false, false);
    label_ui_headline_3->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_2 = new Label ("DL",
                                                        TRANS("GROUP 2 (MIX: 1 to 3)")));
    label_ui_headline_2->setFont (Font (30.00f, Font::plain));
    label_ui_headline_2->setJustificationType (Justification::centred);
    label_ui_headline_2->setEditable (false, false, false);
    label_ui_headline_2->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_1 = new Label ("DL",
                                                        TRANS("ADSR PRESET GROUP 1 (MIX: 1 to 2)")));
    label_ui_headline_1->setFont (Font (30.00f, Font::plain));
    label_ui_headline_1->setJustificationType (Justification::centred);
    label_ui_headline_1->setEditable (false, false, false);
    label_ui_headline_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sus_3 = new Label (String::empty,
                                                TRANS("SUS TIME")));
    label_sus_3->setFont (Font (15.00f, Font::plain));
    label_sus_3->setJustificationType (Justification::centred);
    label_sus_3->setEditable (false, false, false);
    label_sus_3->setColour (Label::textColourId, Colours::yellow);
    label_sus_3->setColour (TextEditor::textColourId, Colours::black);
    label_sus_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sus_2 = new Label (String::empty,
                                                TRANS("SUS TIME")));
    label_sus_2->setFont (Font (15.00f, Font::plain));
    label_sus_2->setJustificationType (Justification::centred);
    label_sus_2->setEditable (false, false, false);
    label_sus_2->setColour (Label::textColourId, Colours::yellow);
    label_sus_2->setColour (TextEditor::textColourId, Colours::black);
    label_sus_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (plotter = new Component());

    addAndMakeVisible (label_att_1 = new Label (String::empty,
                                                TRANS("ATTACK")));
    label_att_1->setFont (Font (15.00f, Font::plain));
    label_att_1->setJustificationType (Justification::centred);
    label_att_1->setEditable (false, false, false);
    label_att_1->setColour (Label::textColourId, Colours::yellow);
    label_att_1->setColour (TextEditor::textColourId, Colours::black);
    label_att_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_1 = new Slider ("0"));
    slider_attack_1->setRange (0, 10000, 1);
    slider_attack_1->setSliderStyle (Slider::LinearVertical);
    slider_attack_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_1->addListener (this);

    addAndMakeVisible (label_dec_1 = new Label (String::empty,
                                                TRANS("DECAY")));
    label_dec_1->setFont (Font (15.00f, Font::plain));
    label_dec_1->setJustificationType (Justification::centred);
    label_dec_1->setEditable (false, false, false);
    label_dec_1->setColour (Label::textColourId, Colours::yellow);
    label_dec_1->setColour (TextEditor::textColourId, Colours::black);
    label_dec_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_1 = new Slider ("0"));
    slider_decay_1->setRange (0, 10000, 1);
    slider_decay_1->setSliderStyle (Slider::LinearVertical);
    slider_decay_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_1->addListener (this);

    addAndMakeVisible (label_rel_1 = new Label (String::empty,
                                                TRANS("RELEASE")));
    label_rel_1->setFont (Font (15.00f, Font::plain));
    label_rel_1->setJustificationType (Justification::centred);
    label_rel_1->setEditable (false, false, false);
    label_rel_1->setColour (Label::textColourId, Colours::yellow);
    label_rel_1->setColour (TextEditor::textColourId, Colours::black);
    label_rel_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_1 = new Slider ("0"));
    slider_release_1->setRange (0, 10000, 1);
    slider_release_1->setSliderStyle (Slider::LinearVertical);
    slider_release_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_1->addListener (this);

    addAndMakeVisible (label_att_2 = new Label (String::empty,
                                                TRANS("ATTACK")));
    label_att_2->setFont (Font (15.00f, Font::plain));
    label_att_2->setJustificationType (Justification::centred);
    label_att_2->setEditable (false, false, false);
    label_att_2->setColour (Label::textColourId, Colours::yellow);
    label_att_2->setColour (TextEditor::textColourId, Colours::black);
    label_att_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_2 = new Slider ("0"));
    slider_attack_2->setRange (0, 10000, 1);
    slider_attack_2->setSliderStyle (Slider::LinearVertical);
    slider_attack_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_2->addListener (this);

    addAndMakeVisible (label_dec_2 = new Label (String::empty,
                                                TRANS("DECAY")));
    label_dec_2->setFont (Font (15.00f, Font::plain));
    label_dec_2->setJustificationType (Justification::centred);
    label_dec_2->setEditable (false, false, false);
    label_dec_2->setColour (Label::textColourId, Colours::yellow);
    label_dec_2->setColour (TextEditor::textColourId, Colours::black);
    label_dec_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_2 = new Slider ("0"));
    slider_decay_2->setRange (0, 10000, 1);
    slider_decay_2->setSliderStyle (Slider::LinearVertical);
    slider_decay_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_2->addListener (this);

    addAndMakeVisible (label_rel_2 = new Label (String::empty,
                                                TRANS("RELEASE")));
    label_rel_2->setFont (Font (15.00f, Font::plain));
    label_rel_2->setJustificationType (Justification::centred);
    label_rel_2->setEditable (false, false, false);
    label_rel_2->setColour (Label::textColourId, Colours::yellow);
    label_rel_2->setColour (TextEditor::textColourId, Colours::black);
    label_rel_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_2 = new Slider ("0"));
    slider_release_2->setRange (0, 10000, 1);
    slider_release_2->setSliderStyle (Slider::LinearVertical);
    slider_release_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_2->addListener (this);

    addAndMakeVisible (label_att_3 = new Label (String::empty,
                                                TRANS("ATTACK")));
    label_att_3->setFont (Font (15.00f, Font::plain));
    label_att_3->setJustificationType (Justification::centred);
    label_att_3->setEditable (false, false, false);
    label_att_3->setColour (Label::textColourId, Colours::yellow);
    label_att_3->setColour (TextEditor::textColourId, Colours::black);
    label_att_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_3 = new Slider ("0"));
    slider_attack_3->setRange (0, 10000, 1);
    slider_attack_3->setSliderStyle (Slider::LinearVertical);
    slider_attack_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_3->addListener (this);

    addAndMakeVisible (label_dec_3 = new Label (String::empty,
                                                TRANS("DECAY")));
    label_dec_3->setFont (Font (15.00f, Font::plain));
    label_dec_3->setJustificationType (Justification::centred);
    label_dec_3->setEditable (false, false, false);
    label_dec_3->setColour (Label::textColourId, Colours::yellow);
    label_dec_3->setColour (TextEditor::textColourId, Colours::black);
    label_dec_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_3 = new Slider ("0"));
    slider_decay_3->setRange (0, 10000, 1);
    slider_decay_3->setSliderStyle (Slider::LinearVertical);
    slider_decay_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_3->addListener (this);

    addAndMakeVisible (label_rel_3 = new Label (String::empty,
                                                TRANS("RELEASE")));
    label_rel_3->setFont (Font (15.00f, Font::plain));
    label_rel_3->setJustificationType (Justification::centred);
    label_rel_3->setEditable (false, false, false);
    label_rel_3->setColour (Label::textColourId, Colours::yellow);
    label_rel_3->setColour (TextEditor::textColourId, Colours::black);
    label_rel_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_3 = new Slider ("0"));
    slider_release_3->setRange (0, 10000, 1);
    slider_release_3->setSliderStyle (Slider::LinearVertical);
    slider_release_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_3->addListener (this);

    addAndMakeVisible (label_att_4 = new Label (String::empty,
                                                TRANS("ATTACK")));
    label_att_4->setFont (Font (15.00f, Font::plain));
    label_att_4->setJustificationType (Justification::centred);
    label_att_4->setEditable (false, false, false);
    label_att_4->setColour (Label::textColourId, Colours::yellow);
    label_att_4->setColour (TextEditor::textColourId, Colours::black);
    label_att_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_4 = new Slider ("0"));
    slider_attack_4->setRange (0, 10000, 1);
    slider_attack_4->setSliderStyle (Slider::LinearVertical);
    slider_attack_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_4->addListener (this);

    addAndMakeVisible (label_dec_4 = new Label (String::empty,
                                                TRANS("DECAY")));
    label_dec_4->setFont (Font (15.00f, Font::plain));
    label_dec_4->setJustificationType (Justification::centred);
    label_dec_4->setEditable (false, false, false);
    label_dec_4->setColour (Label::textColourId, Colours::yellow);
    label_dec_4->setColour (TextEditor::textColourId, Colours::black);
    label_dec_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_4 = new Slider ("0"));
    slider_decay_4->setRange (0, 10000, 1);
    slider_decay_4->setSliderStyle (Slider::LinearVertical);
    slider_decay_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_4->addListener (this);

    addAndMakeVisible (label_rel_4 = new Label (String::empty,
                                                TRANS("RELEASE")));
    label_rel_4->setFont (Font (15.00f, Font::plain));
    label_rel_4->setJustificationType (Justification::centred);
    label_rel_4->setEditable (false, false, false);
    label_rel_4->setColour (Label::textColourId, Colours::yellow);
    label_rel_4->setColour (TextEditor::textColourId, Colours::black);
    label_rel_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_4 = new Slider ("0"));
    slider_release_4->setRange (0, 10000, 1);
    slider_release_4->setSliderStyle (Slider::LinearVertical);
    slider_release_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_4->addListener (this);

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("TEST MIX:")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centredRight);
    label6->setEditable (false, false, false);
    label6->setColour (Label::textColourId, Colours::yellow);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test = new Slider ("0"));
    slider_test->setRange (0, 3, 0.001);
    slider_test->setSliderStyle (Slider::LinearHorizontal);
    slider_test->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_test->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test->addListener (this);

    addAndMakeVisible (slider_sustain_time_1 = new Slider ("0"));
    slider_sustain_time_1->setRange (0, 10000, 1);
    slider_sustain_time_1->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_1->addListener (this);

    addAndMakeVisible (slider_sustain_time_2 = new Slider ("0"));
    slider_sustain_time_2->setRange (0, 10000, 1);
    slider_sustain_time_2->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_2->addListener (this);

    addAndMakeVisible (slider_sustain_time_3 = new Slider ("0"));
    slider_sustain_time_3->setRange (0, 10000, 1);
    slider_sustain_time_3->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_3->addListener (this);

    addAndMakeVisible (slider_sustain_time_4 = new Slider ("0"));
    slider_sustain_time_4->setRange (0, 10000, 1);
    slider_sustain_time_4->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_4->addListener (this);

    addAndMakeVisible (label_sus_4 = new Label (String::empty,
                                                TRANS("SUS TIME")));
    label_sus_4->setFont (Font (15.00f, Font::plain));
    label_sus_4->setJustificationType (Justification::centred);
    label_sus_4->setEditable (false, false, false);
    label_sus_4->setColour (Label::textColourId, Colours::yellow);
    label_sus_4->setColour (TextEditor::textColourId, Colours::black);
    label_sus_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sus_1 = new Label (String::empty,
                                                TRANS("SUS TIME")));
    label_sus_1->setFont (Font (15.00f, Font::plain));
    label_sus_1->setJustificationType (Justification::centred);
    label_sus_1->setEditable (false, false, false);
    label_sus_1->setColour (Label::textColourId, Colours::yellow);
    label_sus_1->setColour (TextEditor::textColourId, Colours::black);
    label_sus_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_attack_1 = new Label ("VL",
                                                   TRANS("x\n")));
    label_attack_1->setFont (Font (15.00f, Font::plain));
    label_attack_1->setJustificationType (Justification::centred);
    label_attack_1->setEditable (false, false, false);
    label_attack_1->setColour (Label::textColourId, Colours::yellow);
    label_attack_1->setColour (TextEditor::textColourId, Colours::black);
    label_attack_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_decay_1 = new Label ("VL",
                                                  TRANS("x\n")));
    label_decay_1->setFont (Font (15.00f, Font::plain));
    label_decay_1->setJustificationType (Justification::centred);
    label_decay_1->setEditable (false, false, false);
    label_decay_1->setColour (Label::textColourId, Colours::yellow);
    label_decay_1->setColour (TextEditor::textColourId, Colours::black);
    label_decay_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain_time_1 = new Label ("VL",
                                                         TRANS("x\n")));
    label_sustain_time_1->setFont (Font (15.00f, Font::plain));
    label_sustain_time_1->setJustificationType (Justification::centred);
    label_sustain_time_1->setEditable (false, false, false);
    label_sustain_time_1->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time_1->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_release_1 = new Label ("VL",
                                                    TRANS("x\n")));
    label_release_1->setFont (Font (15.00f, Font::plain));
    label_release_1->setJustificationType (Justification::centred);
    label_release_1->setEditable (false, false, false);
    label_release_1->setColour (Label::textColourId, Colours::yellow);
    label_release_1->setColour (TextEditor::textColourId, Colours::black);
    label_release_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_attack_2 = new Label ("VL",
                                                   TRANS("x\n")));
    label_attack_2->setFont (Font (15.00f, Font::plain));
    label_attack_2->setJustificationType (Justification::centred);
    label_attack_2->setEditable (false, false, false);
    label_attack_2->setColour (Label::textColourId, Colours::yellow);
    label_attack_2->setColour (TextEditor::textColourId, Colours::black);
    label_attack_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_decay_2 = new Label ("VL",
                                                  TRANS("x\n")));
    label_decay_2->setFont (Font (15.00f, Font::plain));
    label_decay_2->setJustificationType (Justification::centred);
    label_decay_2->setEditable (false, false, false);
    label_decay_2->setColour (Label::textColourId, Colours::yellow);
    label_decay_2->setColour (TextEditor::textColourId, Colours::black);
    label_decay_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain_time_2 = new Label ("VL",
                                                         TRANS("x\n")));
    label_sustain_time_2->setFont (Font (15.00f, Font::plain));
    label_sustain_time_2->setJustificationType (Justification::centred);
    label_sustain_time_2->setEditable (false, false, false);
    label_sustain_time_2->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time_2->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_release_2 = new Label ("VL",
                                                    TRANS("x\n")));
    label_release_2->setFont (Font (15.00f, Font::plain));
    label_release_2->setJustificationType (Justification::centred);
    label_release_2->setEditable (false, false, false);
    label_release_2->setColour (Label::textColourId, Colours::yellow);
    label_release_2->setColour (TextEditor::textColourId, Colours::black);
    label_release_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_attack_3 = new Label ("VL",
                                                   TRANS("x\n")));
    label_attack_3->setFont (Font (15.00f, Font::plain));
    label_attack_3->setJustificationType (Justification::centred);
    label_attack_3->setEditable (false, false, false);
    label_attack_3->setColour (Label::textColourId, Colours::yellow);
    label_attack_3->setColour (TextEditor::textColourId, Colours::black);
    label_attack_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_decay_3 = new Label ("VL",
                                                  TRANS("x\n")));
    label_decay_3->setFont (Font (15.00f, Font::plain));
    label_decay_3->setJustificationType (Justification::centred);
    label_decay_3->setEditable (false, false, false);
    label_decay_3->setColour (Label::textColourId, Colours::yellow);
    label_decay_3->setColour (TextEditor::textColourId, Colours::black);
    label_decay_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain_time_3 = new Label ("VL",
                                                         TRANS("x\n")));
    label_sustain_time_3->setFont (Font (15.00f, Font::plain));
    label_sustain_time_3->setJustificationType (Justification::centred);
    label_sustain_time_3->setEditable (false, false, false);
    label_sustain_time_3->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time_3->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_release_3 = new Label ("VL",
                                                    TRANS("x\n")));
    label_release_3->setFont (Font (15.00f, Font::plain));
    label_release_3->setJustificationType (Justification::centred);
    label_release_3->setEditable (false, false, false);
    label_release_3->setColour (Label::textColourId, Colours::yellow);
    label_release_3->setColour (TextEditor::textColourId, Colours::black);
    label_release_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_attack_4 = new Label ("VL",
                                                   TRANS("x\n")));
    label_attack_4->setFont (Font (15.00f, Font::plain));
    label_attack_4->setJustificationType (Justification::centred);
    label_attack_4->setEditable (false, false, false);
    label_attack_4->setColour (Label::textColourId, Colours::yellow);
    label_attack_4->setColour (TextEditor::textColourId, Colours::black);
    label_attack_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_decay_4 = new Label ("VL",
                                                  TRANS("x\n")));
    label_decay_4->setFont (Font (15.00f, Font::plain));
    label_decay_4->setJustificationType (Justification::centred);
    label_decay_4->setEditable (false, false, false);
    label_decay_4->setColour (Label::textColourId, Colours::yellow);
    label_decay_4->setColour (TextEditor::textColourId, Colours::black);
    label_decay_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain_time_4 = new Label ("VL",
                                                         TRANS("x\n")));
    label_sustain_time_4->setFont (Font (15.00f, Font::plain));
    label_sustain_time_4->setJustificationType (Justification::centred);
    label_sustain_time_4->setEditable (false, false, false);
    label_sustain_time_4->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time_4->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_release_4 = new Label ("VL",
                                                    TRANS("x\n")));
    label_release_4->setFont (Font (15.00f, Font::plain));
    label_release_4->setJustificationType (Justification::centred);
    label_release_4->setEditable (false, false, false);
    label_release_4->setColour (Label::textColourId, Colours::yellow);
    label_release_4->setColour (TextEditor::textColourId, Colours::black);
    label_release_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_env_shape = new Slider ("0"));
    slider_env_shape->setRange (0, 1, 0.01);
    slider_env_shape->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_env_shape->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_env_shape->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_env_shape->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_env_shape->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_env_shape->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_env_shape->addListener (this);

    addAndMakeVisible (label_shape = new Label ("new label",
                                                TRANS("SHAPE")));
    label_shape->setFont (Font (15.00f, Font::plain));
    label_shape->setJustificationType (Justification::centred);
    label_shape->setEditable (false, false, false);
    label_shape->setColour (Label::textColourId, Colours::yellow);
    label_shape->setColour (TextEditor::textColourId, Colours::black);
    label_shape->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_test_value = new Label (String::empty,
                                                     TRANS("x\n")));
    label_test_value->setFont (Font (15.00f, Font::plain));
    label_test_value->setJustificationType (Justification::centredLeft);
    label_test_value->setEditable (false, false, false);
    label_test_value->setColour (Label::textColourId, Colours::yellow);
    label_test_value->setColour (TextEditor::textColourId, Colours::black);
    label_test_value->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_5 = new Label ("DL",
                                                        TRANS("123ms")));
    label_ui_headline_5->setFont (Font (30.00f, Font::plain));
    label_ui_headline_5->setJustificationType (Justification::centred);
    label_ui_headline_5->setEditable (false, false, false);
    label_ui_headline_5->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("(GLOBAL)")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::yellow);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_force_to_zero = new Slider ("0"));
    slider_force_to_zero->setRange (0, 1, 0.01);
    slider_force_to_zero->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_force_to_zero->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_force_to_zero->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_force_to_zero->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_force_to_zero->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_force_to_zero->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_force_to_zero->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("(GLOBAL)")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::yellow);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_force_z = new Label ("new label",
                                                  TRANS("FORCE 0")));
    label_force_z->setFont (Font (15.00f, Font::plain));
    label_force_z->setJustificationType (Justification::centred);
    label_force_z->setEditable (false, false, false);
    label_force_z->setColour (Label::textColourId, Colours::yellow);
    label_force_z->setColour (TextEditor::textColourId, Colours::black);
    label_force_z->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    last_repaint_counter = -99;
    repaint_counter = 0;

    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setRepaintsOnMouseActivity(false);
        getChildComponent(i)->setWantsKeyboardFocus(false);

        if( Slider* slider = dynamic_cast<Slider*>(getChildComponent(i)) ) {
            SET_SLIDER_STYLE( slider, VALUE_SLIDER );
            slider->setOpaque(true);
        }
    }
    setOpaque(true);
    slider_test->setOpaque(false);
    label_shape->setOpaque(true);
    label_att_1->setOpaque(true);
    label_att_2->setOpaque(true);
    label_att_3->setOpaque(true);
    label_att_4->setOpaque(true);
    label_dec_1->setOpaque(true);
    label_dec_2->setOpaque(true);
    label_dec_3->setOpaque(true);
    label_dec_4->setOpaque(true);
    label_sus_1->setOpaque(true);
    label_sus_2->setOpaque(true);
    label_sus_3->setOpaque(true);
    label_sus_4->setOpaque(true);
    label_rel_1->setOpaque(true);
    label_rel_2->setOpaque(true);
    label_rel_3->setOpaque(true);
    label_rel_4->setOpaque(true);
    label_ui_headline_1->setOpaque(true);
    label_ui_headline_2->setOpaque(true);
    label_ui_headline_3->setOpaque(true);
    label_ui_headline_4->setOpaque(true);

    slider_env_shape->setValue(GET_DATA(synth_data).curve_shape,sendNotification);
    refresh();

    /*
    //[/UserPreSize]

    setSize (1465, 180);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

UiEditorSettings::~UiEditorSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_ui_headline_4 = nullptr;
    label_ui_headline_3 = nullptr;
    label_ui_headline_2 = nullptr;
    label_ui_headline_1 = nullptr;
    label_sus_3 = nullptr;
    label_sus_2 = nullptr;
    plotter = nullptr;
    label_att_1 = nullptr;
    slider_attack_1 = nullptr;
    label_dec_1 = nullptr;
    slider_decay_1 = nullptr;
    label_rel_1 = nullptr;
    slider_release_1 = nullptr;
    label_att_2 = nullptr;
    slider_attack_2 = nullptr;
    label_dec_2 = nullptr;
    slider_decay_2 = nullptr;
    label_rel_2 = nullptr;
    slider_release_2 = nullptr;
    label_att_3 = nullptr;
    slider_attack_3 = nullptr;
    label_dec_3 = nullptr;
    slider_decay_3 = nullptr;
    label_rel_3 = nullptr;
    slider_release_3 = nullptr;
    label_att_4 = nullptr;
    slider_attack_4 = nullptr;
    label_dec_4 = nullptr;
    slider_decay_4 = nullptr;
    label_rel_4 = nullptr;
    slider_release_4 = nullptr;
    label6 = nullptr;
    slider_test = nullptr;
    slider_sustain_time_1 = nullptr;
    slider_sustain_time_2 = nullptr;
    slider_sustain_time_3 = nullptr;
    slider_sustain_time_4 = nullptr;
    label_sus_4 = nullptr;
    label_sus_1 = nullptr;
    label_attack_1 = nullptr;
    label_decay_1 = nullptr;
    label_sustain_time_1 = nullptr;
    label_release_1 = nullptr;
    label_attack_2 = nullptr;
    label_decay_2 = nullptr;
    label_sustain_time_2 = nullptr;
    label_release_2 = nullptr;
    label_attack_3 = nullptr;
    label_decay_3 = nullptr;
    label_sustain_time_3 = nullptr;
    label_release_3 = nullptr;
    label_attack_4 = nullptr;
    label_decay_4 = nullptr;
    label_sustain_time_4 = nullptr;
    label_release_4 = nullptr;
    slider_env_shape = nullptr;
    label_shape = nullptr;
    label_test_value = nullptr;
    label_ui_headline_5 = nullptr;
    label3 = nullptr;
    slider_force_to_zero = nullptr;
    label2 = nullptr;
    label_force_z = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorSettings::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0x66000000));
    g.fillRoundedRectangle (10.0f, 10.0f, 260.0f, 135.0f, 6.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (10.0f, 10.0f, 260.0f, 135.0f, 6.000f, 1.000f);

    g.setColour (Colour (0x66000000));
    g.fillRoundedRectangle (270.0f, 10.0f, 260.0f, 135.0f, 6.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (270.0f, 10.0f, 260.0f, 135.0f, 6.000f, 1.000f);

    g.setColour (Colour (0x66000000));
    g.fillRoundedRectangle (530.0f, 10.0f, 260.0f, 135.0f, 6.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (530.0f, 10.0f, 260.0f, 135.0f, 6.000f, 1.000f);

    g.setColour (Colour (0x66000000));
    g.fillRoundedRectangle (790.0f, 10.0f, 260.0f, 135.0f, 6.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (790.0f, 10.0f, 260.0f, 135.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (140, 145, 1, 11);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (400, 145, 1, 11);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (660, 145, 1, 11);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (920, 145, 1, 11);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (140, 155, 930, 1);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (1070, 65, 1, 91);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (1070, 65, 5, 1);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (1140, 65, 10, 1);

    g.setColour (Colour (0xffff3b00));
    g.fillRect (1215, 65, 5, 1);

    //[UserPaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_END.h"
    {
        curve.clearQuick();
        curve.ensureStorageAllocated( 50000 );

        int sustain_start = -1;
        int sustain_end = -1;
        mono_ParameterOwnerStore::get_full_adsr( slider_test->getValue(), curve, sustain_start, sustain_end );

        int plotter_x = plotter->getX();
        int plotter_y = plotter->getY();
        int plotter_width = plotter->getWidth();
        int plotter_hight = plotter->getHeight();

        {
            Colour colour = Colour(0xff222222 );
            g.setGradientFill (ColourGradient (colour.darker (0.3f), 0.0f, 0.0f, Colour (0xff050505), 0.0f, plotter_hight, false));
            //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f, height, false));
            g.fillRoundedRectangle (plotter_x, plotter_y, plotter_width, plotter_hight, 3);

            g.setColour (colour.darker (0.6f));
            g.drawRoundedRectangle (plotter_x, plotter_y, plotter_width, plotter_hight, 3, 1);
        }

        const int curve_size = curve.size();
        const int sustain_size = curve.size()/5;
        float scale_w = float(plotter_width)/(curve.size()+sustain_size);

        int last_x = -1;
        int last_y = -1;
        int count_sustain = -1;
        Colour col;
        for( int i = 0 ; i != curve_size+sustain_size ; ++i )
        {
            float value;
            if( i >= sustain_start && count_sustain < sustain_size )
            {
                value = 0.5f;
                col = Colours::orange;
                count_sustain++;
            }
            else
            {
                col = UiLookAndFeel::getInstance()->colours.slider_track_colour,
                value = 1.0f-curve[count_sustain == sustain_size ? i-sustain_size : i];
            }

            const int x = scale_w*i+plotter_x;
            const int y = value*plotter_hight+plotter_y;
            if( last_x != x || last_y != y )
            {
                last_y = y;
                g.setColour (col);
                g.fillRect (last_x, last_y, 1, 1);

                if( last_x != x )
                {
                    g.setColour (col.withAlpha(0.1f));
                    g.fillRect (last_x, last_y, 1, int((1.0f-value)*plotter_hight));
                }
                last_x = x;
            }
        }
    }
    //[/UserPaint]
}

void UiEditorSettings::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPreResize]

    label_ui_headline_4->setBounds (850, -4, 140, 35);
    label_ui_headline_3->setBounds (590, -4, 140, 35);
    label_ui_headline_2->setBounds (330, -4, 140, 35);
    label_ui_headline_1->setBounds (50, -4, 180, 35);
    label_sus_3->setBounds (660, 105, 60, 33);
    label_sus_2->setBounds (400, 105, 60, 33);
    plotter->setBounds (1220, 15, 235, 135);
    label_att_1->setBounds (20, 105, 60, 33);
    slider_attack_1->setBounds (20, 55, 60, 50);
    label_dec_1->setBounds (80, 105, 60, 33);
    slider_decay_1->setBounds (80, 55, 60, 50);
    label_rel_1->setBounds (200, 105, 60, 33);
    slider_release_1->setBounds (200, 55, 60, 50);
    label_att_2->setBounds (280, 105, 60, 33);
    slider_attack_2->setBounds (280, 55, 60, 50);
    label_dec_2->setBounds (340, 105, 60, 33);
    slider_decay_2->setBounds (400 - 60, 105 - 50, 60, 50);
    label_rel_2->setBounds (460, 105, 60, 33);
    slider_release_2->setBounds (520 - 60, 105 - 50, 60, 50);
    label_att_3->setBounds (540, 105, 60, 33);
    slider_attack_3->setBounds (600 - 60, 105 - 50, 60, 50);
    label_dec_3->setBounds (600, 105, 60, 33);
    slider_decay_3->setBounds (660 - 60, 105 - 50, 60, 50);
    label_rel_3->setBounds (720, 105, 60, 33);
    slider_release_3->setBounds (780 - 60, 105 - 50, 60, 50);
    label_att_4->setBounds (800, 105, 60, 33);
    slider_attack_4->setBounds (860 - 60, 105 - 50, 60, 50);
    label_dec_4->setBounds (860, 105, 60, 33);
    slider_decay_4->setBounds (920 - 60, 105 - 50, 60, 50);
    label_rel_4->setBounds (980, 105, 60, 33);
    slider_release_4->setBounds (1040 - 60, 105 - 50, 60, 50);
    label6->setBounds (20, 150, 60, 33);
    slider_test->setBounds (135, 185 - 40, 792, 40);
    slider_sustain_time_1->setBounds (140, 55, 60, 50);
    slider_sustain_time_2->setBounds (460 - 60, 105 - 50, 60, 50);
    slider_sustain_time_3->setBounds (720 - 60, 105 - 50, 60, 50);
    slider_sustain_time_4->setBounds (980 - 60, 105 - 50, 60, 50);
    label_sus_4->setBounds (920, 105, 60, 33);
    label_sus_1->setBounds (140, 105, 60, 33);
    label_attack_1->setBounds (20, 17, 60, 33);
    label_decay_1->setBounds (80, 17, 60, 33);
    label_sustain_time_1->setBounds (140, 17, 60, 33);
    label_release_1->setBounds (200, 17, 60, 33);
    label_attack_2->setBounds (280, 17, 60, 33);
    label_decay_2->setBounds (340, 17, 60, 33);
    label_sustain_time_2->setBounds (400, 17, 60, 33);
    label_release_2->setBounds (460, 17, 60, 33);
    label_attack_3->setBounds (540, 17, 60, 33);
    label_decay_3->setBounds (600, 17, 60, 33);
    label_sustain_time_3->setBounds (660, 17, 60, 33);
    label_release_3->setBounds (720, 17, 60, 33);
    label_attack_4->setBounds (800, 17, 60, 33);
    label_decay_4->setBounds (860, 17, 60, 33);
    label_sustain_time_4->setBounds (920, 17, 60, 33);
    label_release_4->setBounds (980, 17, 60, 33);
    slider_env_shape->setBounds (1140 - 60, 105 - 70, 60, 70);
    label_shape->setBounds (1080, 105, 60, 33);
    label_test_value->setBounds (80, 150, 50, 33);
    label_ui_headline_5->setBounds (1330, 20, 60, 33);
    label3->setBounds (1080, 125, 60, 33);
    slider_force_to_zero->setBounds (1150, 105 - 70, 60, 70);
    label2->setBounds (1150, 125, 60, 33);
    label_force_z->setBounds (1150, 105, 60, 33);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void UiEditorSettings::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_attack_1)
    {
        //[UserSliderCode_slider_attack_1] -- add your slider handling code here..
        env_preset_def->attack_1 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_attack_1]
    }
    else if (sliderThatWasMoved == slider_decay_1)
    {
        //[UserSliderCode_slider_decay_1] -- add your slider handling code here..
        env_preset_def->decay_1 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_decay_1]
    }
    else if (sliderThatWasMoved == slider_release_1)
    {
        //[UserSliderCode_slider_release_1] -- add your slider handling code here..
        env_preset_def->release_1 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_release_1]
    }
    else if (sliderThatWasMoved == slider_attack_2)
    {
        //[UserSliderCode_slider_attack_2] -- add your slider handling code here..
        env_preset_def->attack_2 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_attack_2]
    }
    else if (sliderThatWasMoved == slider_decay_2)
    {
        //[UserSliderCode_slider_decay_2] -- add your slider handling code here..
        env_preset_def->decay_2 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_decay_2]
    }
    else if (sliderThatWasMoved == slider_release_2)
    {
        //[UserSliderCode_slider_release_2] -- add your slider handling code here..
        env_preset_def->release_2 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_release_2]
    }
    else if (sliderThatWasMoved == slider_attack_3)
    {
        //[UserSliderCode_slider_attack_3] -- add your slider handling code here..
        env_preset_def->attack_3 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_attack_3]
    }
    else if (sliderThatWasMoved == slider_decay_3)
    {
        //[UserSliderCode_slider_decay_3] -- add your slider handling code here..
        env_preset_def->decay_3 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_decay_3]
    }
    else if (sliderThatWasMoved == slider_release_3)
    {
        //[UserSliderCode_slider_release_3] -- add your slider handling code here..
        env_preset_def->release_3 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_release_3]
    }
    else if (sliderThatWasMoved == slider_attack_4)
    {
        //[UserSliderCode_slider_attack_4] -- add your slider handling code here..
        env_preset_def->attack_4 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_attack_4]
    }
    else if (sliderThatWasMoved == slider_decay_4)
    {
        //[UserSliderCode_slider_decay_4] -- add your slider handling code here..
        env_preset_def->decay_4 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_decay_4]
    }
    else if (sliderThatWasMoved == slider_release_4)
    {
        //[UserSliderCode_slider_release_4] -- add your slider handling code here..
        env_preset_def->release_4 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_release_4]
    }
    else if (sliderThatWasMoved == slider_test)
    {
        //[UserSliderCode_slider_test] -- add your slider handling code here..
        label_test_value->setText(String(slider_test->getValue()+1),dontSendNotification);
        //[/UserSliderCode_slider_test]
    }
    else if (sliderThatWasMoved == slider_sustain_time_1)
    {
        //[UserSliderCode_slider_sustain_time_1] -- add your slider handling code here..
        env_preset_def->sustain_time_1 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_sustain_time_1]
    }
    else if (sliderThatWasMoved == slider_sustain_time_2)
    {
        //[UserSliderCode_slider_sustain_time_2] -- add your slider handling code here..
        env_preset_def->sustain_time_2 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_sustain_time_2]
    }
    else if (sliderThatWasMoved == slider_sustain_time_3)
    {
        //[UserSliderCode_slider_sustain_time_3] -- add your slider handling code here..
        env_preset_def->sustain_time_3 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_sustain_time_3]
    }
    else if (sliderThatWasMoved == slider_sustain_time_4)
    {
        //[UserSliderCode_slider_sustain_time_4] -- add your slider handling code here..
        env_preset_def->sustain_time_4 = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_sustain_time_4]
    }
    else if (sliderThatWasMoved == slider_env_shape)
    {
        //[UserSliderCode_slider_env_shape] -- add your slider handling code here..
        GET_DATA(synth_data).curve_shape = slider_env_shape->getValue();
        //[/UserSliderCode_slider_env_shape]
    }
    else if (sliderThatWasMoved == slider_force_to_zero)
    {
        //[UserSliderCode_slider_force_to_zero] -- add your slider handling code here..
        GET_DATA(synth_data).force_envs_to_zero = slider_force_to_zero->getValue();
        //[/UserSliderCode_slider_force_to_zero]
    }

    //[UsersliderValueChanged_Post]
    repaint_counter++;
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

<JUCER_COMPONENT documentType="Component" className="UiEditorSettings" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams=""
                 variableInitialisers="original_w(1465), original_h(180)" snapPixels="10"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="10 10 260 135" cornerSize="6" fill="solid: 66000000" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="270 10 260 135" cornerSize="6" fill="solid: 66000000" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="530 10 260 135" cornerSize="6" fill="solid: 66000000" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="790 10 260 135" cornerSize="6" fill="solid: 66000000" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <RECT pos="140 145 1 11" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="400 145 1 11" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="660 145 1 11" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="920 145 1 11" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="140 155 930 1" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="1070 65 1 91" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="1070 65 5 1" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="1140 65 10 1" fill="solid: ffff3b00" hasStroke="0"/>
    <RECT pos="1215 65 5 1" fill="solid: ffff3b00" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="DL" id="d447f2eac1344e8b" memberName="label_ui_headline_4"
         virtualName="" explicitFocusOrder="0" pos="850 -4 140 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GROUP 4 (MIX: 3 to 4)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="523ac0b70e351f43" memberName="label_ui_headline_3"
         virtualName="" explicitFocusOrder="0" pos="590 -4 140 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GROUP 3 (MIX: 2 to 4)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="9571679d902f0b2a" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="330 -4 140 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GROUP 2 (MIX: 1 to 3)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline_1"
         virtualName="" explicitFocusOrder="0" pos="50 -4 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ADSR PRESET GROUP 1 (MIX: 1 to 2)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="63733ef294c01fe3" memberName="label_sus_3" virtualName=""
         explicitFocusOrder="0" pos="660 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="b86afc1f9a82c57b" memberName="label_sus_2" virtualName=""
         explicitFocusOrder="0" pos="400 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c88b6f548bad2da7" memberName="plotter" virtualName=""
                    explicitFocusOrder="0" pos="1220 15 235 135" class="Component"
                    params=""/>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label_att_1" virtualName=""
         explicitFocusOrder="0" pos="20 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_attack_1" virtualName="Slider"
          explicitFocusOrder="0" pos="20 55 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="5269c763f2d5a37b" memberName="label_dec_1" virtualName=""
         explicitFocusOrder="0" pos="80 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b62502f225e4fe3a" memberName="slider_decay_1" virtualName="Slider"
          explicitFocusOrder="0" pos="80 55 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="d001c80859e5b7cb" memberName="label_rel_1" virtualName=""
         explicitFocusOrder="0" pos="200 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e8c91a0aabc505e" memberName="slider_release_1" virtualName="Slider"
          explicitFocusOrder="0" pos="200 55 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="652f8c698bf6e2c7" memberName="label_att_2" virtualName=""
         explicitFocusOrder="0" pos="280 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="d64a1c27fe0baec4" memberName="slider_attack_2" virtualName="Slider"
          explicitFocusOrder="0" pos="280 55 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="29cbca18263366d5" memberName="label_dec_2" virtualName=""
         explicitFocusOrder="0" pos="340 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="917d80ecff72d938" memberName="slider_decay_2" virtualName="Slider"
          explicitFocusOrder="0" pos="400r 105r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="7ef526dd8315ebcb" memberName="label_rel_2" virtualName=""
         explicitFocusOrder="0" pos="460 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="f4a67e7ca9df04d3" memberName="slider_release_2"
          virtualName="Slider" explicitFocusOrder="0" pos="520r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="69b28ea11658a763" memberName="label_att_3" virtualName=""
         explicitFocusOrder="0" pos="540 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e0e4215f40e7cc1b" memberName="slider_attack_3" virtualName="Slider"
          explicitFocusOrder="0" pos="600r 105r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="c285c1141eaa0118" memberName="label_dec_3" virtualName=""
         explicitFocusOrder="0" pos="600 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="c9794aadac72be2b" memberName="slider_decay_3" virtualName="Slider"
          explicitFocusOrder="0" pos="660r 105r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="f5e6e332ab87bc22" memberName="label_rel_3" virtualName=""
         explicitFocusOrder="0" pos="720 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="1cc6a6994a1ac6d6" memberName="slider_release_3"
          virtualName="Slider" explicitFocusOrder="0" pos="780r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ac7391e000b56ff6" memberName="label_att_4" virtualName=""
         explicitFocusOrder="0" pos="800 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="8e9f61467e1ec899" memberName="slider_attack_4" virtualName="Slider"
          explicitFocusOrder="0" pos="860r 105r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="117c28f4bd17a85e" memberName="label_dec_4" virtualName=""
         explicitFocusOrder="0" pos="860 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="2c342433ae30cbcc" memberName="slider_decay_4" virtualName="Slider"
          explicitFocusOrder="0" pos="920r 105r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="10000" int="1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="228028476280a34e" memberName="label_rel_4" virtualName=""
         explicitFocusOrder="0" pos="980 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="da498e3b67b4f708" memberName="slider_release_4"
          virtualName="Slider" explicitFocusOrder="0" pos="1040r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="c4e98860bf594960" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="20 150 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="TEST MIX:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="34"/>
  <SLIDER name="0" id="11e6ab83bd6d05d" memberName="slider_test" virtualName="Slider"
          explicitFocusOrder="0" pos="135 185r 792 40" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="3" int="0.0010000000000000000208" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="76a391a494643c63" memberName="slider_sustain_time_1"
          virtualName="Slider" explicitFocusOrder="0" pos="140 55 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="17f02dd31261d734" memberName="slider_sustain_time_2"
          virtualName="Slider" explicitFocusOrder="0" pos="460r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="8d1895289bf4e022" memberName="slider_sustain_time_3"
          virtualName="Slider" explicitFocusOrder="0" pos="720r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="7ded7da4555fca64" memberName="slider_sustain_time_4"
          virtualName="Slider" explicitFocusOrder="0" pos="980r 105r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ebbaa43d5c1e571e" memberName="label_sus_4" virtualName=""
         explicitFocusOrder="0" pos="920 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="ffcf23120599c6e5" memberName="label_sus_1" virtualName=""
         explicitFocusOrder="0" pos="140 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a09cec04c5ae6b58" memberName="label_attack_1" virtualName=""
         explicitFocusOrder="0" pos="20 17 60 33" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1b295ca55294b0a2" memberName="label_decay_1" virtualName=""
         explicitFocusOrder="0" pos="80 17 60 33" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e1944df446a5aea6" memberName="label_sustain_time_1"
         virtualName="" explicitFocusOrder="0" pos="140 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="72cc727ebebbec15" memberName="label_release_1"
         virtualName="" explicitFocusOrder="0" pos="200 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="24934f1f91aef173" memberName="label_attack_2" virtualName=""
         explicitFocusOrder="0" pos="280 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="53fe0e195c25619a" memberName="label_decay_2" virtualName=""
         explicitFocusOrder="0" pos="340 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e688900429f36aad" memberName="label_sustain_time_2"
         virtualName="" explicitFocusOrder="0" pos="400 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e22cc8a807ae92d7" memberName="label_release_2"
         virtualName="" explicitFocusOrder="0" pos="460 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="440ee7d337aeafa1" memberName="label_attack_3" virtualName=""
         explicitFocusOrder="0" pos="540 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1be8567e87efb1a9" memberName="label_decay_3" virtualName=""
         explicitFocusOrder="0" pos="600 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="b99c3b638343c553" memberName="label_sustain_time_3"
         virtualName="" explicitFocusOrder="0" pos="660 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="f793a360d861539a" memberName="label_release_3"
         virtualName="" explicitFocusOrder="0" pos="720 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="4fb02c88d8b7c30c" memberName="label_attack_4" virtualName=""
         explicitFocusOrder="0" pos="800 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="70378c7b2ac364b3" memberName="label_decay_4" virtualName=""
         explicitFocusOrder="0" pos="860 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a090d14d65d3041b" memberName="label_sustain_time_4"
         virtualName="" explicitFocusOrder="0" pos="920 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="2b1562db42f4565a" memberName="label_release_4"
         virtualName="" explicitFocusOrder="0" pos="980 17 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e7a1c7c979888f2f" memberName="slider_env_shape"
          virtualName="Slider" explicitFocusOrder="0" pos="1140r 105r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ad65d35c7b51c7ea" memberName="label_shape"
         virtualName="" explicitFocusOrder="0" pos="1080 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SHAPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="9e8f8c2aecf0c625" memberName="label_test_value" virtualName=""
         explicitFocusOrder="0" pos="80 150 50 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="DL" id="4b4915adb0dcf9ee" memberName="label_ui_headline_5"
         virtualName="" explicitFocusOrder="0" pos="1330 20 60 33" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="123ms" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="6a35bf0fb43137d1" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="1080 125 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="(GLOBAL)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="241b05fe5f21f4d6" memberName="slider_force_to_zero"
          virtualName="Slider" explicitFocusOrder="0" pos="1150 105r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="5f324f181175362d" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="1150 125 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="(GLOBAL)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="67e9e147e209996c" memberName="label_force_z"
         virtualName="" explicitFocusOrder="0" pos="1150 105 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="FORCE 0" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
