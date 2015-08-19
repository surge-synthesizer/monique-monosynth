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
#include "UiEditorSynthLite.h"

#include "SynthData.h"
//[/Headers]

#include "UiEditorSettings.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorSettings::refresh() {
    stop_current_painting = true;
    repaint_curve = true;

    slider_attack_1->setValue( DATA( env_preset_def ).attack_1*1000, dontSendNotification );
    slider_attack_2->setValue( DATA( env_preset_def ).attack_2*1000, dontSendNotification );
    slider_attack_3->setValue( DATA( env_preset_def ).attack_3*1000, dontSendNotification );
    slider_attack_4->setValue( DATA( env_preset_def ).attack_4*1000, dontSendNotification );
    label_attack_1->setText(String(slider_attack_1->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_attack_2->setText(String(slider_attack_2->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_attack_3->setText(String(slider_attack_3->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_attack_4->setText(String(slider_attack_4->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);

    slider_decay_1->setValue( DATA( env_preset_def ).decay_1*1000, dontSendNotification );
    slider_decay_2->setValue( DATA( env_preset_def ).decay_2*1000, dontSendNotification );
    slider_decay_3->setValue( DATA( env_preset_def ).decay_3*1000, dontSendNotification );
    slider_decay_4->setValue( DATA( env_preset_def ).decay_4*1000, dontSendNotification );
    label_decay_1->setText(String(slider_decay_1->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_decay_2->setText(String(slider_decay_2->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_decay_3->setText(String(slider_decay_3->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);
    label_decay_4->setText(String(slider_decay_4->getValue()*DATA( env_preset_def ).max_release_time)+String("ms"), dontSendNotification);

    slider_sustain_time_1->setValue( DATA( env_preset_def ).sustain_time_1*1000, dontSendNotification );
    slider_sustain_time_2->setValue( DATA( env_preset_def ).sustain_time_2*1000, dontSendNotification );
    slider_sustain_time_3->setValue( DATA( env_preset_def ).sustain_time_3*1000, dontSendNotification );
    slider_sustain_time_4->setValue( DATA( env_preset_def ).sustain_time_4*1000, dontSendNotification );
    label_sustain_time_1->setText(String(slider_sustain_time_1->getValue()), dontSendNotification);
    label_sustain_time_2->setText(String(slider_sustain_time_2->getValue()), dontSendNotification);
    label_sustain_time_3->setText(String(slider_sustain_time_3->getValue()), dontSendNotification);
    label_sustain_time_4->setText(String(slider_sustain_time_4->getValue()), dontSendNotification);

    slider_release_1->setValue( DATA( env_preset_def ).release_1*1000, dontSendNotification );
    slider_release_2->setValue( DATA( env_preset_def ).release_2*1000, dontSendNotification );
    slider_release_3->setValue( DATA( env_preset_def ).release_3*1000, dontSendNotification );
    slider_release_4->setValue( DATA( env_preset_def ).release_4*1000, dontSendNotification );
    label_release_1->setText(String(round01(slider_release_1->getValue()*DATA( env_preset_def ).max_release_time))+String("ms"), dontSendNotification);
    label_release_2->setText(String(round01(slider_release_2->getValue()*DATA( env_preset_def ).max_release_time))+String("ms"), dontSendNotification);
    label_release_3->setText(String(round01(slider_release_3->getValue()*DATA( env_preset_def ).max_release_time))+String("ms"), dontSendNotification);
    label_release_4->setText(String(round01(slider_release_4->getValue()*DATA( env_preset_def ).max_release_time))+String("ms"), dontSendNotification);

    slider_test_attack->setValue( ENVPresetData::get_attack_at(DATA( env_preset_def ),slider_test->getValue()) *1000, dontSendNotification );
    slider_test_decay->setValue( ENVPresetData::get_decay_at(DATA( env_preset_def ),slider_test->getValue()) *1000, dontSendNotification );
    slider_test_suatain_time->setValue( ENVPresetData::get_sustain_time_at(DATA( env_preset_def ),slider_test->getValue()) *1000, dontSendNotification );
    slider_test_release->setValue( ENVPresetData::get_release_at(DATA( env_preset_def ),slider_test->getValue()) *1000, dontSendNotification );

    slider_max_adr_attack->setValue( DATA( env_preset_def ).max_attack_time*1000, dontSendNotification );
    slider_max_adr_decay->setValue( DATA( env_preset_def ).max_decay_time*1000, dontSendNotification );
    slider_max_adr_release->setValue( DATA( env_preset_def ).max_release_time*1000, dontSendNotification );

    slider_morph_motor_time->setValue( DATA( synth_data ).morph_motor_time, dontSendNotification );
    slider_glide_time->setValue( DATA( synth_data ).glide_motor_time, dontSendNotification );

    toggle_animate_input_env->setToggleState( DATA( synth_data ).animate_input_env ,dontSendNotification );
    toggle_animate_eq_env->setToggleState( DATA( synth_data ).animate_eq_env ,dontSendNotification );
    toggle_output_cc_mute4->setToggleState( DATA( synth_data ).animate_modulations ,dontSendNotification );

    repaint();
}
//[/MiscUserDefs]

//==============================================================================
UiEditorSettings::UiEditorSettings ()
    : original_w(650), original_h(590)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label_21 = new Label (String::empty,
                                             TRANS("SUS TIME")));
    label_21->setFont (Font (15.00f, Font::plain));
    label_21->setJustificationType (Justification::centred);
    label_21->setEditable (false, false, false);
    label_21->setColour (Label::textColourId, Colours::yellow);
    label_21->setColour (TextEditor::textColourId, Colours::black);
    label_21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_22 = new Label (String::empty,
                                             TRANS("SUS TIME")));
    label_22->setFont (Font (15.00f, Font::plain));
    label_22->setJustificationType (Justification::centred);
    label_22->setEditable (false, false, false);
    label_22->setColour (Label::textColourId, Colours::yellow);
    label_22->setColour (TextEditor::textColourId, Colours::black);
    label_22->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (plotter = new Component());

    addAndMakeVisible (label_1 = new Label (String::empty,
                                            TRANS("ATTACK")));
    label_1->setFont (Font (15.00f, Font::plain));
    label_1->setJustificationType (Justification::centred);
    label_1->setEditable (false, false, false);
    label_1->setColour (Label::textColourId, Colours::yellow);
    label_1->setColour (TextEditor::textColourId, Colours::black);
    label_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_1 = new Slider ("0"));
    slider_attack_1->setRange (0, 1000, 0.01);
    slider_attack_1->setSliderStyle (Slider::LinearVertical);
    slider_attack_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_1->addListener (this);

    addAndMakeVisible (label_ui_headline4 = new Label (String::empty,
                                                       TRANS("GLOBAL ADR PRESET VALUES")));
    label_ui_headline4->setFont (Font (30.00f, Font::plain));
    label_ui_headline4->setJustificationType (Justification::centredLeft);
    label_ui_headline4->setEditable (false, false, false);
    label_ui_headline4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("0\n")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::yellow);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("DECAY")));
    label_2->setFont (Font (15.00f, Font::plain));
    label_2->setJustificationType (Justification::centred);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colours::yellow);
    label_2->setColour (TextEditor::textColourId, Colours::black);
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_1 = new Slider ("0"));
    slider_decay_1->setRange (0, 1000, 0.01);
    slider_decay_1->setSliderStyle (Slider::LinearVertical);
    slider_decay_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_1->addListener (this);

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("RELEASE")));
    label_3->setFont (Font (15.00f, Font::plain));
    label_3->setJustificationType (Justification::centred);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colours::yellow);
    label_3->setColour (TextEditor::textColourId, Colours::black);
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_1 = new Slider ("0"));
    slider_release_1->setRange (0, 1000, 0.01);
    slider_release_1->setSliderStyle (Slider::LinearVertical);
    slider_release_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_1->addListener (this);

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("ATTACK")));
    label_4->setFont (Font (15.00f, Font::plain));
    label_4->setJustificationType (Justification::centred);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colours::yellow);
    label_4->setColour (TextEditor::textColourId, Colours::black);
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_2 = new Slider ("0"));
    slider_attack_2->setRange (0, 1000, 0.01);
    slider_attack_2->setSliderStyle (Slider::LinearVertical);
    slider_attack_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_2->addListener (this);

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("DECAY")));
    label_5->setFont (Font (15.00f, Font::plain));
    label_5->setJustificationType (Justification::centred);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colours::yellow);
    label_5->setColour (TextEditor::textColourId, Colours::black);
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_2 = new Slider ("0"));
    slider_decay_2->setRange (0, 1000, 0.01);
    slider_decay_2->setSliderStyle (Slider::LinearVertical);
    slider_decay_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_2->addListener (this);

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("RELEASE")));
    label_6->setFont (Font (15.00f, Font::plain));
    label_6->setJustificationType (Justification::centred);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colours::yellow);
    label_6->setColour (TextEditor::textColourId, Colours::black);
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_2 = new Slider ("0"));
    slider_release_2->setRange (0, 1000, 0.01);
    slider_release_2->setSliderStyle (Slider::LinearVertical);
    slider_release_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_2->addListener (this);

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("ATTACK")));
    label_7->setFont (Font (15.00f, Font::plain));
    label_7->setJustificationType (Justification::centred);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colours::yellow);
    label_7->setColour (TextEditor::textColourId, Colours::black);
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_3 = new Slider ("0"));
    slider_attack_3->setRange (0, 1000, 0.01);
    slider_attack_3->setSliderStyle (Slider::LinearVertical);
    slider_attack_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_3->addListener (this);

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("DECAY")));
    label_8->setFont (Font (15.00f, Font::plain));
    label_8->setJustificationType (Justification::centred);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colours::yellow);
    label_8->setColour (TextEditor::textColourId, Colours::black);
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_3 = new Slider ("0"));
    slider_decay_3->setRange (0, 1000, 0.01);
    slider_decay_3->setSliderStyle (Slider::LinearVertical);
    slider_decay_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_3->addListener (this);

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("RELEASE")));
    label_9->setFont (Font (15.00f, Font::plain));
    label_9->setJustificationType (Justification::centred);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colours::yellow);
    label_9->setColour (TextEditor::textColourId, Colours::black);
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_3 = new Slider ("0"));
    slider_release_3->setRange (0, 1000, 0.01);
    slider_release_3->setSliderStyle (Slider::LinearVertical);
    slider_release_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_3->addListener (this);

    addAndMakeVisible (label_10 = new Label (String::empty,
                                             TRANS("ATTACK")));
    label_10->setFont (Font (15.00f, Font::plain));
    label_10->setJustificationType (Justification::centred);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colours::yellow);
    label_10->setColour (TextEditor::textColourId, Colours::black);
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack_4 = new Slider ("0"));
    slider_attack_4->setRange (0, 1000, 0.01);
    slider_attack_4->setSliderStyle (Slider::LinearVertical);
    slider_attack_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack_4->addListener (this);

    addAndMakeVisible (label_11 = new Label (String::empty,
                                             TRANS("DECAY")));
    label_11->setFont (Font (15.00f, Font::plain));
    label_11->setJustificationType (Justification::centred);
    label_11->setEditable (false, false, false);
    label_11->setColour (Label::textColourId, Colours::yellow);
    label_11->setColour (TextEditor::textColourId, Colours::black);
    label_11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay_4 = new Slider ("0"));
    slider_decay_4->setRange (0, 1000, 0.01);
    slider_decay_4->setSliderStyle (Slider::LinearVertical);
    slider_decay_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay_4->addListener (this);

    addAndMakeVisible (label_12 = new Label (String::empty,
                                             TRANS("RELEASE")));
    label_12->setFont (Font (15.00f, Font::plain));
    label_12->setJustificationType (Justification::centred);
    label_12->setEditable (false, false, false);
    label_12->setColour (Label::textColourId, Colours::yellow);
    label_12->setColour (TextEditor::textColourId, Colours::black);
    label_12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release_4 = new Slider ("0"));
    slider_release_4->setRange (0, 1000, 0.01);
    slider_release_4->setSliderStyle (Slider::LinearVertical);
    slider_release_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release_4->addListener (this);

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("TEST")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (Label::textColourId, Colours::yellow);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test = new Slider ("0"));
    slider_test->setRange (0, 3, 0.001);
    slider_test->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_test->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_test->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test->addListener (this);

    addAndMakeVisible (label_13 = new Label (String::empty,
                                             TRANS("ATTACK")));
    label_13->setFont (Font (15.00f, Font::plain));
    label_13->setJustificationType (Justification::centred);
    label_13->setEditable (false, false, false);
    label_13->setColour (Label::textColourId, Colours::yellow);
    label_13->setColour (TextEditor::textColourId, Colours::black);
    label_13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test_attack = new Slider ("0"));
    slider_test_attack->setRange (0, 1000, 0.01);
    slider_test_attack->setSliderStyle (Slider::LinearVertical);
    slider_test_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_test_attack->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test_attack->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test_attack->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test_attack->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test_attack->addListener (this);

    addAndMakeVisible (label_14 = new Label (String::empty,
                                             TRANS("DECAY")));
    label_14->setFont (Font (15.00f, Font::plain));
    label_14->setJustificationType (Justification::centred);
    label_14->setEditable (false, false, false);
    label_14->setColour (Label::textColourId, Colours::yellow);
    label_14->setColour (TextEditor::textColourId, Colours::black);
    label_14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test_decay = new Slider ("0"));
    slider_test_decay->setRange (0, 1000, 0.01);
    slider_test_decay->setSliderStyle (Slider::LinearVertical);
    slider_test_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_test_decay->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test_decay->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test_decay->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test_decay->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test_decay->addListener (this);

    addAndMakeVisible (label_15 = new Label (String::empty,
                                             TRANS("RELEASE")));
    label_15->setFont (Font (15.00f, Font::plain));
    label_15->setJustificationType (Justification::centred);
    label_15->setEditable (false, false, false);
    label_15->setColour (Label::textColourId, Colours::yellow);
    label_15->setColour (TextEditor::textColourId, Colours::black);
    label_15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test_release = new Slider ("0"));
    slider_test_release->setRange (0, 1000, 0.01);
    slider_test_release->setSliderStyle (Slider::LinearVertical);
    slider_test_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_test_release->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test_release->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test_release->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test_release->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test_release->addListener (this);

    addAndMakeVisible (slider_max_adr_attack = new Slider ("0"));
    slider_max_adr_attack->setRange (100, 20000, 1);
    slider_max_adr_attack->setSliderStyle (Slider::LinearVertical);
    slider_max_adr_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_max_adr_attack->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_max_adr_attack->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_max_adr_attack->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_max_adr_attack->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_max_adr_attack->addListener (this);

    addAndMakeVisible (slider_max_adr_decay = new Slider ("0"));
    slider_max_adr_decay->setRange (100, 20000, 1);
    slider_max_adr_decay->setSliderStyle (Slider::LinearVertical);
    slider_max_adr_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_max_adr_decay->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_max_adr_decay->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_max_adr_decay->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_max_adr_decay->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_max_adr_decay->addListener (this);

    addAndMakeVisible (slider_max_adr_release = new Slider ("0"));
    slider_max_adr_release->setRange (100, 20000, 1);
    slider_max_adr_release->setSliderStyle (Slider::LinearVertical);
    slider_max_adr_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_max_adr_release->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_max_adr_release->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_max_adr_release->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_max_adr_release->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_max_adr_release->addListener (this);

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("1")));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (Label::textColourId, Colours::yellow);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("2")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::yellow);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("3")));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colours::yellow);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline2 = new Label (String::empty,
                                                       TRANS("GLOBAL")));
    label_ui_headline2->setFont (Font (30.00f, Font::plain));
    label_ui_headline2->setJustificationType (Justification::centredLeft);
    label_ui_headline2->setEditable (false, false, false);
    label_ui_headline2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_morph_motor_time = new Slider ("0"));
    slider_morph_motor_time->setRange (1, 20000, 1);
    slider_morph_motor_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_morph_motor_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_morph_motor_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_morph_motor_time->addListener (this);

    addAndMakeVisible (label10 = new Label ("new label",
                                            TRANS("MAX ATT T(ms)")));
    label10->setFont (Font (15.00f, Font::plain));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (Label::textColourId, Colours::yellow);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_glide_time = new Slider ("0"));
    slider_glide_time->setRange (1, 20000, 1);
    slider_glide_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_glide_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_glide_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_glide_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_glide_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_glide_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_glide_time->addListener (this);

    addAndMakeVisible (label11 = new Label ("new label",
                                            TRANS("MAX DEC T(ms)")));
    label11->setFont (Font (15.00f, Font::plain));
    label11->setJustificationType (Justification::centred);
    label11->setEditable (false, false, false);
    label11->setColour (Label::textColourId, Colours::yellow);
    label11->setColour (TextEditor::textColourId, Colours::black);
    label11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label12 = new Label ("new label",
                                            TRANS("MAX REL T(ms)")));
    label12->setFont (Font (15.00f, Font::plain));
    label12->setJustificationType (Justification::centred);
    label12->setEditable (false, false, false);
    label12->setColour (Label::textColourId, Colours::yellow);
    label12->setColour (TextEditor::textColourId, Colours::black);
    label12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label13 = new Label ("new label",
                                            TRANS("MORPH MOTOR TIME")));
    label13->setFont (Font (15.00f, Font::plain));
    label13->setJustificationType (Justification::centred);
    label13->setEditable (false, false, false);
    label13->setColour (Label::textColourId, Colours::yellow);
    label13->setColour (TextEditor::textColourId, Colours::black);
    label13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setButtonText (TRANS("ESC X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (true);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (false);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setColour (TextEditor::textColourId, Colours::aliceblue);
    textEditor->setColour (TextEditor::backgroundColourId, Colours::black);
    textEditor->setText (TRANS("On the left side you can define the presets for single slider ADR curves. With the test slider you can check the mixed results of the 4 presets.\n"
    "Note: this are global settings and will by morphed by the \"main\" option."));

    addAndMakeVisible (label7 = new Label ("new label",
                                           TRANS("GLIDE TIME")));
    label7->setFont (Font (15.00f, Font::plain));
    label7->setJustificationType (Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (Label::textColourId, Colours::yellow);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_17 = new Label (String::empty,
                                             TRANS("EXAMPLE: ATTACK (ms) = (ATT/1000) * (MAX-ATT-TIME)")));
    label_17->setFont (Font (15.00f, Font::plain));
    label_17->setJustificationType (Justification::centred);
    label_17->setEditable (false, false, false);
    label_17->setColour (Label::textColourId, Colours::yellow);
    label_17->setColour (TextEditor::textColourId, Colours::black);
    label_17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_16 = new Label (String::empty,
                                             TRANS("ANIMATE INPUT ENV")));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_input_env = new ToggleButton (String::empty));
    toggle_animate_input_env->addListener (this);

    addAndMakeVisible (label_18 = new Label (String::empty,
                                             TRANS("ANIMATE EQ ENV")));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredLeft);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_eq_env = new ToggleButton (String::empty));
    toggle_animate_eq_env->addListener (this);

    addAndMakeVisible (label_19 = new Label (String::empty,
                                             TRANS("ANIMATE MODULATIONS")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredLeft);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_output_cc_mute4 = new ToggleButton (String::empty));
    toggle_output_cc_mute4->addListener (this);

    addAndMakeVisible (slider_sustain_time_1 = new Slider ("0"));
    slider_sustain_time_1->setRange (0, 1000, 0.01);
    slider_sustain_time_1->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_1->addListener (this);

    addAndMakeVisible (slider_sustain_time_2 = new Slider ("0"));
    slider_sustain_time_2->setRange (0, 1000, 0.01);
    slider_sustain_time_2->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_2->addListener (this);

    addAndMakeVisible (slider_sustain_time_3 = new Slider ("0"));
    slider_sustain_time_3->setRange (0, 1000, 0.01);
    slider_sustain_time_3->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_3->addListener (this);

    addAndMakeVisible (slider_sustain_time_4 = new Slider ("0"));
    slider_sustain_time_4->setRange (0, 1000, 0.01);
    slider_sustain_time_4->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time_4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time_4->addListener (this);

    addAndMakeVisible (label_20 = new Label (String::empty,
                                             TRANS("SUS TIME")));
    label_20->setFont (Font (15.00f, Font::plain));
    label_20->setJustificationType (Justification::centred);
    label_20->setEditable (false, false, false);
    label_20->setColour (Label::textColourId, Colours::yellow);
    label_20->setColour (TextEditor::textColourId, Colours::black);
    label_20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_23 = new Label (String::empty,
                                             TRANS("SUS TIME")));
    label_23->setFont (Font (15.00f, Font::plain));
    label_23->setJustificationType (Justification::centred);
    label_23->setEditable (false, false, false);
    label_23->setColour (Label::textColourId, Colours::yellow);
    label_23->setColour (TextEditor::textColourId, Colours::black);
    label_23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_test_suatain_time = new Slider ("0"));
    slider_test_suatain_time->setRange (0, 1000, 0.01);
    slider_test_suatain_time->setSliderStyle (Slider::LinearVertical);
    slider_test_suatain_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_test_suatain_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_test_suatain_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_test_suatain_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_test_suatain_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_test_suatain_time->addListener (this);

    addAndMakeVisible (label_24 = new Label (String::empty,
                                             TRANS("SUS TIME")));
    label_24->setFont (Font (15.00f, Font::plain));
    label_24->setJustificationType (Justification::centred);
    label_24->setEditable (false, false, false);
    label_24->setColour (Label::textColourId, Colours::yellow);
    label_24->setColour (TextEditor::textColourId, Colours::black);
    label_24->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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

    addAndMakeVisible (label8 = new Label ("new label",
                                           TRANS("ENV SHAPE")));
    label8->setFont (Font (15.00f, Font::plain));
    label8->setJustificationType (Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (Label::textColourId, Colours::yellow);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    for( int i = 0 ; i != getNumChildComponents() ; ++i ) {
        //getChildComponent(i)->setOpaque(true);
        if( Slider* slider = dynamic_cast<Slider*>(getChildComponent(i)) ) {
            SET_SLIDER_STYLE( slider, VALUE_SLIDER );
        }
    }

    slider_test_attack->setEnabled(false);
    slider_test_decay->setEnabled(false);
    slider_test_release->setEnabled(false);
    slider_test_suatain_time->setEnabled(false);

    refresh();
    repaint_curve = true;
    stop_current_painting = false;

    //[/UserPreSize]

    setSize (650, 590);


    //[Constructor] You can add your own custom stuff here..
    slider_env_shape->setValue(DATA(synth_data).curve_shape,sendNotification);
    refresh();
    //[/Constructor]
}

UiEditorSettings::~UiEditorSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_21 = nullptr;
    label_22 = nullptr;
    plotter = nullptr;
    label_1 = nullptr;
    slider_attack_1 = nullptr;
    label_ui_headline4 = nullptr;
    label2 = nullptr;
    label_2 = nullptr;
    slider_decay_1 = nullptr;
    label_3 = nullptr;
    slider_release_1 = nullptr;
    label_4 = nullptr;
    slider_attack_2 = nullptr;
    label_5 = nullptr;
    slider_decay_2 = nullptr;
    label_6 = nullptr;
    slider_release_2 = nullptr;
    label_7 = nullptr;
    slider_attack_3 = nullptr;
    label_8 = nullptr;
    slider_decay_3 = nullptr;
    label_9 = nullptr;
    slider_release_3 = nullptr;
    label_10 = nullptr;
    slider_attack_4 = nullptr;
    label_11 = nullptr;
    slider_decay_4 = nullptr;
    label_12 = nullptr;
    slider_release_4 = nullptr;
    label6 = nullptr;
    slider_test = nullptr;
    label_13 = nullptr;
    slider_test_attack = nullptr;
    label_14 = nullptr;
    slider_test_decay = nullptr;
    label_15 = nullptr;
    slider_test_release = nullptr;
    slider_max_adr_attack = nullptr;
    slider_max_adr_decay = nullptr;
    slider_max_adr_release = nullptr;
    label5 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label_ui_headline2 = nullptr;
    slider_morph_motor_time = nullptr;
    label10 = nullptr;
    slider_glide_time = nullptr;
    label11 = nullptr;
    label12 = nullptr;
    label13 = nullptr;
    close = nullptr;
    textEditor = nullptr;
    label7 = nullptr;
    label_17 = nullptr;
    label_16 = nullptr;
    toggle_animate_input_env = nullptr;
    label_18 = nullptr;
    toggle_animate_eq_env = nullptr;
    label_19 = nullptr;
    toggle_output_cc_mute4 = nullptr;
    slider_sustain_time_1 = nullptr;
    slider_sustain_time_2 = nullptr;
    slider_sustain_time_3 = nullptr;
    slider_sustain_time_4 = nullptr;
    label_20 = nullptr;
    label_23 = nullptr;
    slider_test_suatain_time = nullptr;
    label_24 = nullptr;
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
    label8 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorSettings::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "UiDynamicSizeStart.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff000033));

    g.setColour (Colour (0xffff3b00));
    g.drawRect (0, 0, 650, 590, 2);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 85.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath1, PathStrokeType (1.000f, PathStrokeType::mitered, PathStrokeType::rounded));

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath2, PathStrokeType (1.000f, PathStrokeType::mitered, PathStrokeType::rounded));

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath3, PathStrokeType (1.000f, PathStrokeType::mitered, PathStrokeType::rounded));

    g.setColour (Colour (0xffff3b00));
    g.strokePath (internalPath4, PathStrokeType (1.000f, PathStrokeType::mitered, PathStrokeType::rounded));

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 185.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 285.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 385.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (400.0f, 167.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (462.0f, 167.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (420.0f, 167.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (480.0f, 167.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (582.0f, 167.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (600.0f, 167.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 60.0f, 1.0f, 50.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 160.0f, 1.0f, 50.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 260.0f, 1.0f, 50.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (260.0f, 360.0f, 1.0f, 50.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    if( repaint_curve )
    {
        repaint_curve = false;
        stop_current_painting = false;

        curve.clearQuick();
        curve.ensureStorageAllocated( 50000 );

        int sustain_start = -1;
        int sustain_end = -1;
        mono_ParameterOwnerStore::get_full_adsr( slider_test->getValue(), curve, sustain_start, sustain_end );

        int plotter_x = plotter->getX();
        int plotter_y = plotter->getY();
        int plotter_width = plotter->getWidth();
        int plotter_hight = plotter->getHeight();
        const int curve_size = curve.size();
        const int sustain_size = curve.size()/5;
        float scale_w = float(plotter_width)/(curve.size()+sustain_size);

        int last_x = -1;
        int last_y = -1;
        int count_sustain = -1;
        Colour col;
        for( int i = 0 ; i != curve_size+sustain_size ; ++i )
        {
            if(stop_current_painting)
                return;

            float value;
            if( i >= sustain_start && count_sustain < sustain_size )
            {
	        value = 0.5f;
                col = Colours::orange;
                count_sustain++;
            }
            else
            {
                col = Colours::yellow;
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
                    g.fillRect (last_x, last_y, 1, (1.0f-value)*plotter_hight);
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
    //[/UserPreResize]

    label_21->setBounds (200 - 60, 336 - 26, 60, 26);
    label_22->setBounds (200 - 60, 236 - 26, 60, 26);
    plotter->setBounds (320, 310, 310, 100);
    label_1->setBounds (80 - 60, 136 - 26, 60, 26);
    slider_attack_1->setBounds (20, 110 - 50, 60, 50);
    label_ui_headline4->setBounds (20, 10, 490, 30);
    label2->setBounds (270, 70, 20, 26);
    label_2->setBounds (140 - 60, 136 - 26, 60, 26);
    slider_decay_1->setBounds (140 - 60, 110 - 50, 60, 50);
    label_3->setBounds (260 - 60, 136 - 26, 60, 26);
    slider_release_1->setBounds (260 - 60, 110 - 50, 60, 50);
    label_4->setBounds (80 - 60, 236 - 26, 60, 26);
    slider_attack_2->setBounds (80 - 60, 210 - 50, 60, 50);
    label_5->setBounds (140 - 60, 236 - 26, 60, 26);
    slider_decay_2->setBounds (140 - 60, 210 - 50, 60, 50);
    label_6->setBounds (260 - 60, 236 - 26, 60, 26);
    slider_release_2->setBounds (260 - 60, 210 - 50, 60, 50);
    label_7->setBounds (80 - 60, 336 - 26, 60, 26);
    slider_attack_3->setBounds (80 - 60, 310 - 50, 60, 50);
    label_8->setBounds (140 - 60, 336 - 26, 60, 26);
    slider_decay_3->setBounds (140 - 60, 310 - 50, 60, 50);
    label_9->setBounds (260 - 60, 336 - 26, 60, 26);
    slider_release_3->setBounds (260 - 60, 310 - 50, 60, 50);
    label_10->setBounds (80 - 60, 436 - 26, 60, 26);
    slider_attack_4->setBounds (80 - 60, 410 - 50, 60, 50);
    label_11->setBounds (140 - 60, 436 - 26, 60, 26);
    slider_decay_4->setBounds (140 - 60, 410 - 50, 60, 50);
    label_12->setBounds (260 - 60, 436 - 26, 60, 26);
    slider_release_4->setBounds (260 - 60, 410 - 50, 60, 50);
    label6->setBounds (330, 250, 60, 26);
    slider_test->setBounds (390 - 60, 250 - 70, 60, 70);
    label_13->setBounds (450 - 60, 276 - 26, 60, 26);
    slider_test_attack->setBounds (450 - 60, 250 - 70, 60, 70);
    label_14->setBounds (510 - 60, 276 - 26, 60, 26);
    slider_test_decay->setBounds (510 - 60, 250 - 70, 60, 70);
    label_15->setBounds (630 - 60, 276 - 26, 60, 26);
    slider_test_release->setBounds (630 - 60, 250 - 70, 60, 70);
    slider_max_adr_attack->setBounds (450 - 60, 140 - 70, 60, 70);
    slider_max_adr_decay->setBounds (510 - 60, 140 - 70, 60, 70);
    slider_max_adr_release->setBounds (630 - 60, 140 - 70, 60, 70);
    label5->setBounds (260, 160, 20, 26);
    label3->setBounds (260, 290, 20, 26);
    label4->setBounds (270, 370, 20, 26);
    label_ui_headline2->setBounds (20, 450, 490, 30);
    slider_morph_motor_time->setBounds (80 - 60, 550 - 70, 60, 70);
    label10->setBounds (390, 140, 60, 26);
    slider_glide_time->setBounds (150 - 60, 550 - 70, 60, 70);
    label11->setBounds (450, 140, 60, 26);
    label12->setBounds (570, 140, 60, 26);
    label13->setBounds (20, 550, 60, 26);
    close->setBounds (getWidth() - 5 - 45, 5, 45, 20);
    textEditor->setBounds (320, 410, 310, 70);
    label7->setBounds (90, 550, 60, 26);
    label_17->setBounds (570 - 250, 66 - 26, 250, 26);
    label_16->setBounds (480 - 160, 510 - 30, 160, 30);
    toggle_animate_input_env->setBounds (280, 480, 30, 30);
    label_18->setBounds (480 - 160, 540 - 30, 160, 30);
    toggle_animate_eq_env->setBounds (280, 510, 30, 30);
    label_19->setBounds (480 - 160, 570 - 30, 160, 30);
    toggle_output_cc_mute4->setBounds (280, 540, 30, 30);
    slider_sustain_time_1->setBounds (200 - 60, 110 - 50, 60, 50);
    slider_sustain_time_2->setBounds (200 - 60, 210 - 50, 60, 50);
    slider_sustain_time_3->setBounds (200 - 60, 310 - 50, 60, 50);
    slider_sustain_time_4->setBounds (200 - 60, 410 - 50, 60, 50);
    label_20->setBounds (200 - 60, 436 - 26, 60, 26);
    label_23->setBounds (200 - 60, 136 - 26, 60, 26);
    slider_test_suatain_time->setBounds (570 - 60, 250 - 70, 60, 70);
    label_24->setBounds (570 - 60, 276 - 26, 60, 26);
    label_attack_1->setBounds (80 - 60, 56 - 26, 60, 26);
    label_decay_1->setBounds (140 - 60, 56 - 26, 60, 26);
    label_sustain_time_1->setBounds (200 - 60, 56 - 26, 60, 26);
    label_release_1->setBounds (260 - 60, 56 - 26, 60, 26);
    label_attack_2->setBounds (80 - 60, 156 - 26, 60, 26);
    label_decay_2->setBounds (140 - 60, 156 - 26, 60, 26);
    label_sustain_time_2->setBounds (200 - 60, 156 - 26, 60, 26);
    label_release_2->setBounds (260 - 60, 156 - 26, 60, 26);
    label_attack_3->setBounds (80 - 60, 256 - 26, 60, 26);
    label_decay_3->setBounds (140 - 60, 256 - 26, 60, 26);
    label_sustain_time_3->setBounds (200 - 60, 256 - 26, 60, 26);
    label_release_3->setBounds (260 - 60, 256 - 26, 60, 26);
    label_attack_4->setBounds (80 - 60, 356 - 26, 60, 26);
    label_decay_4->setBounds (140 - 60, 356 - 26, 60, 26);
    label_sustain_time_4->setBounds (200 - 60, 356 - 26, 60, 26);
    label_release_4->setBounds (260 - 60, 356 - 26, 60, 26);
    slider_env_shape->setBounds (230 - 60, 550 - 70, 60, 70);
    label8->setBounds (170, 550, 60, 26);
    internalPath1.clear();
    internalPath1.startNewSubPath (270.0f, 85.0f);
    internalPath1.lineTo (300.0f, 200.0f);
    internalPath1.lineTo (320.0f, 200.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath (270.0f, 185.0f);
    internalPath2.lineTo (300.0f, 210.0f);
    internalPath2.lineTo (320.0f, 210.0f);

    internalPath3.clear();
    internalPath3.startNewSubPath (270.0f, 285.0f);
    internalPath3.lineTo (300.0f, 220.0f);
    internalPath3.lineTo (320.0f, 220.0f);

    internalPath4.clear();
    internalPath4.startNewSubPath (270.0f, 385.0f);
    internalPath4.lineTo (300.0f, 230.0f);
    internalPath4.lineTo (320.0f, 230.0f);

    //[UserResized] Add your own custom resize handling here..
#include "UiDynamicSizeEnd.h"
    //[/UserResized]
}

void UiEditorSettings::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_attack_1)
    {
        //[UserSliderCode_slider_attack_1] -- add your slider handling code here..
        DATA( env_preset_def ).attack_1 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_attack_1]
    }
    else if (sliderThatWasMoved == slider_decay_1)
    {
        //[UserSliderCode_slider_decay_1] -- add your slider handling code here..
        DATA( env_preset_def ).decay_1 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_decay_1]
    }
    else if (sliderThatWasMoved == slider_release_1)
    {
        //[UserSliderCode_slider_release_1] -- add your slider handling code here..
        DATA( env_preset_def ).release_1 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_release_1]
    }
    else if (sliderThatWasMoved == slider_attack_2)
    {
        //[UserSliderCode_slider_attack_2] -- add your slider handling code here..
        DATA( env_preset_def ).attack_2 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_attack_2]
    }
    else if (sliderThatWasMoved == slider_decay_2)
    {
        //[UserSliderCode_slider_decay_2] -- add your slider handling code here..
        DATA( env_preset_def ).decay_2 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_decay_2]
    }
    else if (sliderThatWasMoved == slider_release_2)
    {
        //[UserSliderCode_slider_release_2] -- add your slider handling code here..
        DATA( env_preset_def ).release_2 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_release_2]
    }
    else if (sliderThatWasMoved == slider_attack_3)
    {
        //[UserSliderCode_slider_attack_3] -- add your slider handling code here..
        DATA( env_preset_def ).attack_3 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_attack_3]
    }
    else if (sliderThatWasMoved == slider_decay_3)
    {
        //[UserSliderCode_slider_decay_3] -- add your slider handling code here..
        DATA( env_preset_def ).decay_3 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_decay_3]
    }
    else if (sliderThatWasMoved == slider_release_3)
    {
        //[UserSliderCode_slider_release_3] -- add your slider handling code here..
        DATA( env_preset_def ).release_3 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_release_3]
    }
    else if (sliderThatWasMoved == slider_attack_4)
    {
        //[UserSliderCode_slider_attack_4] -- add your slider handling code here..
        DATA( env_preset_def ).attack_4 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_attack_4]
    }
    else if (sliderThatWasMoved == slider_decay_4)
    {
        //[UserSliderCode_slider_decay_4] -- add your slider handling code here..
        DATA( env_preset_def ).decay_4 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_decay_4]
    }
    else if (sliderThatWasMoved == slider_release_4)
    {
        //[UserSliderCode_slider_release_4] -- add your slider handling code here..
        DATA( env_preset_def ).release_4 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_release_4]
    }
    else if (sliderThatWasMoved == slider_test)
    {
        //[UserSliderCode_slider_test] -- add your slider handling code here..
        refresh();
        //[/UserSliderCode_slider_test]
    }
    else if (sliderThatWasMoved == slider_test_attack)
    {
        //[UserSliderCode_slider_test_attack] -- add your slider handling code here..
        //[/UserSliderCode_slider_test_attack]
    }
    else if (sliderThatWasMoved == slider_test_decay)
    {
        //[UserSliderCode_slider_test_decay] -- add your slider handling code here..
        //[/UserSliderCode_slider_test_decay]
    }
    else if (sliderThatWasMoved == slider_test_release)
    {
        //[UserSliderCode_slider_test_release] -- add your slider handling code here..
        //[/UserSliderCode_slider_test_release]
    }
    else if (sliderThatWasMoved == slider_max_adr_attack)
    {
        //[UserSliderCode_slider_max_adr_attack] -- add your slider handling code here..
        DATA( env_preset_def ).max_attack_time = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_max_adr_attack]
    }
    else if (sliderThatWasMoved == slider_max_adr_decay)
    {
        //[UserSliderCode_slider_max_adr_decay] -- add your slider handling code here..
        DATA( env_preset_def ).max_decay_time = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_max_adr_decay]
    }
    else if (sliderThatWasMoved == slider_max_adr_release)
    {
        //[UserSliderCode_slider_max_adr_release] -- add your slider handling code here..
        DATA( env_preset_def ).max_release_time = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_max_adr_release]
    }
    else if (sliderThatWasMoved == slider_morph_motor_time)
    {
        //[UserSliderCode_slider_morph_motor_time] -- add your slider handling code here..
        DATA( synth_data ).morph_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_morph_motor_time]
    }
    else if (sliderThatWasMoved == slider_glide_time)
    {
        //[UserSliderCode_slider_glide_time] -- add your slider handling code here..
        DATA( synth_data ).glide_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_glide_time]
    }
    else if (sliderThatWasMoved == slider_sustain_time_1)
    {
        //[UserSliderCode_slider_sustain_time_1] -- add your slider handling code here..
        DATA( env_preset_def ).sustain_time_1 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_sustain_time_1]
    }
    else if (sliderThatWasMoved == slider_sustain_time_2)
    {
        //[UserSliderCode_slider_sustain_time_2] -- add your slider handling code here..
        DATA( env_preset_def ).sustain_time_2 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_sustain_time_2]
    }
    else if (sliderThatWasMoved == slider_sustain_time_3)
    {
        //[UserSliderCode_slider_sustain_time_3] -- add your slider handling code here..
        DATA( env_preset_def ).sustain_time_3 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_sustain_time_3]
    }
    else if (sliderThatWasMoved == slider_sustain_time_4)
    {
        //[UserSliderCode_slider_sustain_time_4] -- add your slider handling code here..
        DATA( env_preset_def ).sustain_time_4 = sliderThatWasMoved->getValue()/1000;
        refresh();
        //[/UserSliderCode_slider_sustain_time_4]
    }
    else if (sliderThatWasMoved == slider_test_suatain_time)
    {
        //[UserSliderCode_slider_test_suatain_time] -- add your slider handling code here..
        //[/UserSliderCode_slider_test_suatain_time]
    }
    else if (sliderThatWasMoved == slider_env_shape)
    {
        //[UserSliderCode_slider_env_shape] -- add your slider handling code here..
        DATA(synth_data).curve_shape = slider_env_shape->getValue();
	refresh();
        //[/UserSliderCode_slider_env_shape]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void UiEditorSettings::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        AppInstanceStore::getInstance()->editor->editor_settings = nullptr;
        return;
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env)
    {
        //[UserButtonCode_toggle_animate_input_env] -- add your button handler code here..
        DATA( synth_data ).animate_input_env = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_input_env]
    }
    else if (buttonThatWasClicked == toggle_animate_eq_env)
    {
        //[UserButtonCode_toggle_animate_eq_env] -- add your button handler code here..
        DATA( synth_data ).animate_eq_env = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_eq_env]
    }
    else if (buttonThatWasClicked == toggle_output_cc_mute4)
    {
        //[UserButtonCode_toggle_output_cc_mute4] -- add your button handler code here..
        DATA( synth_data ).animate_modulations = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_output_cc_mute4]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

bool UiEditorSettings::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        AppInstanceStore::getInstance()->editor->editor_settings = nullptr;
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

<JUCER_COMPONENT documentType="Component" className="UiEditorSettings" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers="original_w(650), original_h(590)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="650" initialHeight="590">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff000033">
    <RECT pos="0 0 650 590" fill="solid: 0" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="260 85 10 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, rounded"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 270 85 l 300 200 l 320 200</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, rounded"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 270 185 l 300 210 l 320 210</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, rounded"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 270 285 l 300 220 l 320 220</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, rounded"
          strokeColour="solid: ffff3b00" nonZeroWinding="1">s 270 385 l 300 230 l 320 230</PATH>
    <ROUNDRECT pos="260 185 10 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 285 10 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 385 10 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="400 167 40 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="462 167 40 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="420 167 1 5" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="480 167 1 5" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="582 167 40 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="600 167 1 5" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 60 1 50" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 160 1 50" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 260 1 50" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="260 360 1 50" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="63733ef294c01fe3" memberName="label_21" virtualName=""
         explicitFocusOrder="0" pos="200r 336r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="b86afc1f9a82c57b" memberName="label_22" virtualName=""
         explicitFocusOrder="0" pos="200r 236r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c88b6f548bad2da7" memberName="plotter" virtualName=""
                    explicitFocusOrder="0" pos="320 310 310 100" class="Component"
                    params=""/>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label_1" virtualName=""
         explicitFocusOrder="0" pos="80r 136r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_attack_1" virtualName="Slider"
          explicitFocusOrder="0" pos="20 110r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="e7f4af47148b8b38" memberName="label_ui_headline4"
         virtualName="" explicitFocusOrder="0" pos="20 10 490 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLOBAL ADR PRESET VALUES"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e861ccb6ec177afa" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="270 70 20 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="0&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="5269c763f2d5a37b" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="140r 136r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b62502f225e4fe3a" memberName="slider_decay_1" virtualName="Slider"
          explicitFocusOrder="0" pos="140r 110r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="d001c80859e5b7cb" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="260r 136r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e8c91a0aabc505e" memberName="slider_release_1" virtualName="Slider"
          explicitFocusOrder="0" pos="260r 110r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="652f8c698bf6e2c7" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="80r 236r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="d64a1c27fe0baec4" memberName="slider_attack_2" virtualName="Slider"
          explicitFocusOrder="0" pos="80r 210r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="29cbca18263366d5" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="140r 236r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="917d80ecff72d938" memberName="slider_decay_2" virtualName="Slider"
          explicitFocusOrder="0" pos="140r 210r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="7ef526dd8315ebcb" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="260r 236r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="f4a67e7ca9df04d3" memberName="slider_release_2"
          virtualName="Slider" explicitFocusOrder="0" pos="260r 210r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="69b28ea11658a763" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="80r 336r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e0e4215f40e7cc1b" memberName="slider_attack_3" virtualName="Slider"
          explicitFocusOrder="0" pos="80r 310r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="c285c1141eaa0118" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="140r 336r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="c9794aadac72be2b" memberName="slider_decay_3" virtualName="Slider"
          explicitFocusOrder="0" pos="140r 310r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="f5e6e332ab87bc22" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="260r 336r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="1cc6a6994a1ac6d6" memberName="slider_release_3"
          virtualName="Slider" explicitFocusOrder="0" pos="260r 310r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ac7391e000b56ff6" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="80r 436r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="8e9f61467e1ec899" memberName="slider_attack_4" virtualName="Slider"
          explicitFocusOrder="0" pos="80r 410r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="117c28f4bd17a85e" memberName="label_11" virtualName=""
         explicitFocusOrder="0" pos="140r 436r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="2c342433ae30cbcc" memberName="slider_decay_4" virtualName="Slider"
          explicitFocusOrder="0" pos="140r 410r 60 50" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="228028476280a34e" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="260r 436r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="da498e3b67b4f708" memberName="slider_release_4"
          virtualName="Slider" explicitFocusOrder="0" pos="260r 410r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="c4e98860bf594960" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="330 250 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="TEST" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="11e6ab83bd6d05d" memberName="slider_test" virtualName="Slider"
          explicitFocusOrder="0" pos="390r 250r 60 70" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="3" int="0.0010000000000000000208" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="b4a8467037809de6" memberName="label_13" virtualName=""
         explicitFocusOrder="0" pos="450r 276r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b3a495ad0b81fd2c" memberName="slider_test_attack"
          virtualName="Slider" explicitFocusOrder="0" pos="450r 250r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="e084983e1dcdf834" memberName="label_14" virtualName=""
         explicitFocusOrder="0" pos="510r 276r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="fac0e1c06f8a39b0" memberName="slider_test_decay"
          virtualName="Slider" explicitFocusOrder="0" pos="510r 250r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="cfacf744887adf0" memberName="label_15" virtualName=""
         explicitFocusOrder="0" pos="630r 276r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="aed6567ad376d73d" memberName="slider_test_release"
          virtualName="Slider" explicitFocusOrder="0" pos="630r 250r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="a006df15a71ff5d1" memberName="slider_max_adr_attack"
          virtualName="Slider" explicitFocusOrder="0" pos="450r 140r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="100" max="20000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="6d2e354993098bc4" memberName="slider_max_adr_decay"
          virtualName="Slider" explicitFocusOrder="0" pos="510r 140r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="100" max="20000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="fe766d25b223b503" memberName="slider_max_adr_release"
          virtualName="Slider" explicitFocusOrder="0" pos="630r 140r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="100" max="20000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="5b872ff6cb0af317" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="260 160 20 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="49f0b442d69b7a21" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="260 290 20 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="5c9b5905a436c83d" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="270 370 20 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="db0b469687f2859" memberName="label_ui_headline2"
         virtualName="" explicitFocusOrder="0" pos="20 450 490 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLOBAL" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="30eb597ef479a74e" memberName="slider_morph_motor_time"
          virtualName="Slider" explicitFocusOrder="0" pos="80r 550r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="b473b01a82cc9210" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="390 140 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="MAX ATT T(ms)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="c244467fb56dffee" memberName="slider_glide_time"
          virtualName="Slider" explicitFocusOrder="0" pos="150r 550r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="d0c9c1b7ff22e03" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="450 140 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="MAX DEC T(ms)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="a82df65802aa4467" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="570 140 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="MAX REL T(ms)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="796db125cec623bd" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="20 550 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="MORPH MOTOR TIME"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="5Rr 5 45 20" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="44485d10c8ab48b1" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="320 410 310 70" textcol="fff0f8ff"
              bkgcol="ff000000" initialText="On the left side you can define the presets for single slider ADR curves. With the test slider you can check the mixed results of the 4 presets.&#10;Note: this are global settings and will by morphed by the &quot;main&quot; option."
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
  <LABEL name="new label" id="2c68301961d63e82" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="90 550 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="GLIDE TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="c9897e7f7500c724" memberName="label_17" virtualName=""
         explicitFocusOrder="0" pos="570r 66r 250 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="EXAMPLE: ATTACK (ms) = (ATT/1000) * (MAX-ATT-TIME)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="dfd8e91824767f78" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="480r 510r 160 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ANIMATE INPUT ENV"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="f342323c5495d0a1" memberName="toggle_animate_input_env"
                virtualName="" explicitFocusOrder="0" pos="280 480 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="55ed48882dea6ac8" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="480r 540r 160 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ANIMATE EQ ENV"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="80003915f6558086" memberName="toggle_animate_eq_env"
                virtualName="" explicitFocusOrder="0" pos="280 510 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="48465e518b2a9587" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="480r 570r 160 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ANIMATE MODULATIONS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="13a54c23c88819cb" memberName="toggle_output_cc_mute4"
                virtualName="" explicitFocusOrder="0" pos="280 540 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="0" id="76a391a494643c63" memberName="slider_sustain_time_1"
          virtualName="Slider" explicitFocusOrder="0" pos="200r 110r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="17f02dd31261d734" memberName="slider_sustain_time_2"
          virtualName="Slider" explicitFocusOrder="0" pos="200r 210r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="8d1895289bf4e022" memberName="slider_sustain_time_3"
          virtualName="Slider" explicitFocusOrder="0" pos="200r 310r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="7ded7da4555fca64" memberName="slider_sustain_time_4"
          virtualName="Slider" explicitFocusOrder="0" pos="200r 410r 60 50"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ebbaa43d5c1e571e" memberName="label_20" virtualName=""
         explicitFocusOrder="0" pos="200r 436r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="ffcf23120599c6e5" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="200r 136r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="52d330db793ec7b5" memberName="slider_test_suatain_time"
          virtualName="Slider" explicitFocusOrder="0" pos="570r 250r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="0.010000000000000000208"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ee3b8151aa34102d" memberName="label_24" virtualName=""
         explicitFocusOrder="0" pos="570r 276r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a09cec04c5ae6b58" memberName="label_attack_1" virtualName=""
         explicitFocusOrder="0" pos="80r 56r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1b295ca55294b0a2" memberName="label_decay_1" virtualName=""
         explicitFocusOrder="0" pos="140r 56r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e1944df446a5aea6" memberName="label_sustain_time_1"
         virtualName="" explicitFocusOrder="0" pos="200r 56r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="72cc727ebebbec15" memberName="label_release_1"
         virtualName="" explicitFocusOrder="0" pos="260r 56r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="24934f1f91aef173" memberName="label_attack_2" virtualName=""
         explicitFocusOrder="0" pos="80r 156r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="53fe0e195c25619a" memberName="label_decay_2" virtualName=""
         explicitFocusOrder="0" pos="140r 156r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e688900429f36aad" memberName="label_sustain_time_2"
         virtualName="" explicitFocusOrder="0" pos="200r 156r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e22cc8a807ae92d7" memberName="label_release_2"
         virtualName="" explicitFocusOrder="0" pos="260r 156r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="440ee7d337aeafa1" memberName="label_attack_3" virtualName=""
         explicitFocusOrder="0" pos="80r 256r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1be8567e87efb1a9" memberName="label_decay_3" virtualName=""
         explicitFocusOrder="0" pos="140r 256r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="b99c3b638343c553" memberName="label_sustain_time_3"
         virtualName="" explicitFocusOrder="0" pos="200r 256r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="f793a360d861539a" memberName="label_release_3"
         virtualName="" explicitFocusOrder="0" pos="260r 256r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="4fb02c88d8b7c30c" memberName="label_attack_4" virtualName=""
         explicitFocusOrder="0" pos="80r 356r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="70378c7b2ac364b3" memberName="label_decay_4" virtualName=""
         explicitFocusOrder="0" pos="140r 356r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a090d14d65d3041b" memberName="label_sustain_time_4"
         virtualName="" explicitFocusOrder="0" pos="200r 356r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="2b1562db42f4565a" memberName="label_release_4"
         virtualName="" explicitFocusOrder="0" pos="260r 356r 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e7a1c7c979888f2f" memberName="slider_env_shape"
          virtualName="Slider" explicitFocusOrder="0" pos="230r 550r 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ad65d35c7b51c7ea" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="170 550 60 26" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ENV SHAPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
