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
#include "PluginProcessor.h"
#include "Synth.h"

#include "UiLookAndFeel.h"
#include "UiEditorSynthLitePopup.h"

#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_AmpPainter.h"
#include "monique_ui_ModulationSlider.h"
#include "monique_ui_SliderConfig.h"
#include "monique_ui_MIDIIO.h"
#include "monique_ui_ADRDesigner.h"
#include "monique_ui_GlobalSettings.h"
#include "monique_ui_Morph.h"
//[/Headers]

#include "UiEditorSynthLite.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


//==============================================================================
//==============================================================================

void UiEditorSynthLite::refresh() noexcept {
    show_current_voice_data();
    resize_sequence_buttons();
    show_programs_and_select();
    show_ctrl_state();
}
void UiEditorSynthLite::show_programs_and_select() {
    const int current_bank = synth_data->get_current_bank();
    const int current_programm = synth_data->get_current_program();
    if( current_bank != last_bank || current_programm != last_programm )
    {
        last_bank = current_bank;
        last_programm = current_programm;

        combo_bank->clear(dontSendNotification);
        combo_bank->addItemList(synth_data->get_banks(),1);
        combo_bank->setSelectedId(synth_data->get_current_bank()+1,dontSendNotification);

        combo_programm->clear(dontSendNotification);
        combo_programm->addItemList(synth_data->get_current_bank_programms(),1);
        combo_programm->setSelectedItemIndex(synth_data->get_current_program(),dontSendNotification);
    }
}
void UiEditorSynthLite::show_ctrl_state() {
    if( last_ctrl_mode != synth_data->ctrl )
    {
        last_ctrl_mode = synth_data->ctrl;
        for( int i = 0 ; i < getNumChildComponents() ; ++i )
        {
            if( mono_ModulationSlider* mod_slider = dynamic_cast< mono_ModulationSlider* >( getChildComponent(i) ) )
            {
                if( mod_slider->is_in_ctrl_view() != synth_data->ctrl )
                {
                    mod_slider->set_ctrl_view_mode(synth_data->ctrl);
                    mod_slider->show_view_mode();
                }
            }
        }
    }
}
void UiEditorSynthLite::show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_ ) {
    popup = nullptr;
    if( MIDIControlHandler::getInstance()->is_learning() && midi_conrtrol_ )
    {
        addAndMakeVisible( popup = new UiEditorSynthLitePopup(this,midi_conrtrol_) );
        popup->set_element_to_show( comp_ );
    }
}

#define STANDARD_MULT 1000.0f
void UiEditorSynthLite::show_current_voice_data() {
    ComponentColours colours = UiLookAndFeel::getInstance()->colours;
    Colour button_on = colours.button_on_colour;
    Colour button_off = colours.button_off_colour;

    // FILTER 1
    uint8 f_type = synth_data->filter_datas[0]->filter_type;
    filter_type_6_1->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_1->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_1_1->setColour( TextButton::buttonColourId, f_type == LPF_2_PASS || f_type == HIGH_2_PASS || f_type == MOOG_AND_LPF ? button_on : button_off );
    filter_type_3_1->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_1->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // FILTER 2
    f_type = synth_data->filter_datas[1]->filter_type;
    filter_type_6_2->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_2->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_1_2->setColour( TextButton::buttonColourId, f_type == LPF_2_PASS || f_type == HIGH_2_PASS || f_type == MOOG_AND_LPF ? button_on : button_off );
    filter_type_3_2->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_2->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // FILTER 3
    f_type = synth_data->filter_datas[2]->filter_type;
    filter_type_6_3->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_3->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_1_3->setColour( TextButton::buttonColourId, f_type == LPF_2_PASS || f_type == HIGH_2_PASS || f_type == MOOG_AND_LPF ? button_on : button_off );
    filter_type_3_3->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_3->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // SEQUENCE
    MONOVoice* voice = reinterpret_cast< MONOVoice* >( _app_instance_store->audio_processor->synth.getVoice(0) );
    button_sequence_1->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[0] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(0) ) : button_off );
    button_sequence_2->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[1] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(1) ) : button_off );
    button_sequence_3->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[2] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(2) ) : button_off );
    button_sequence_4->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[3] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(3) ) : button_off );
    button_sequence_5->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[4] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(4) ) : button_off );
    button_sequence_6->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[5] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(5) ) : button_off );
    button_sequence_7->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[6] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(6) ) : button_off );
    button_sequence_8->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[7] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(7) ) : button_off );
    button_sequence_9->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[8] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(8) ) : button_off );
    button_sequence_10->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[9] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(9) ) : button_off );
    button_sequence_11->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[10] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(10) ) : button_off );
    button_sequence_12->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[11] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(11) ) : button_off );
    button_sequence_13->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[12] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(12) ) : button_off );
    button_sequence_14->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[13] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(13) ) : button_off );
    button_sequence_15->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[14] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(14) ) : button_off );
    button_sequence_16->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->step[15] ? button_on.darker( 1.0f-voice->get_arp_sequence_amp(15) ) : button_off );

    button_arp_speed_XNORM->setColour( TextButton::buttonColourId, synth_data->arp_sequencer_data->speed_multi == 0 ? button_on : button_off );

    // MORPHERS
    {
        sl_morhp_mix->setValue( synth_data->linear_morhp_state*1000, dontSendNotification );
    }

    // MIDI LEARN
    button_midi_learn->setColour( TextButton::buttonColourId, MIDIControlHandler::getInstance()->is_waiting_for_param() ? button_on : MIDIControlHandler::getInstance()->is_learning() ? Colours::red : button_off );
    button_ctrl_toggle->setColour( TextButton::buttonColourId, synth_data->ctrl ? Colours::red : button_off );

    // EDITORS
    button_open_config->setColour( TextButton::buttonColourId, editor_settings ? Colours::lightblue : button_off );
    button_open_midi_io_settings->setColour( TextButton::buttonColourId, editor_midiio ? Colours::lightblue : button_off );
    button_open_morph->setColour( TextButton::buttonColourId, editor_morph ? Colours::lightblue:button_off );
    button_open_oszi->setColour( TextButton::buttonColourId, AppInstanceStore::getInstance()->get_amp_painter_unsave() ? Colours::lightblue : button_off );
    button_open_config2->setColour( TextButton::buttonColourId, editor_global_settings ? Colours::lightblue:button_off );

    button_values_toggle->setColour( TextButton::buttonColourId, UiLookAndFeel::getInstance()->show_values_always ? Colours::lightblue : button_off );

    if( popup )
        popup->refresh();
}

void UiEditorSynthLite::resize_sequence_buttons() {
    const float width_factor = 1.0f/original_w*getWidth();
    const float height_factor = 1.0f/original_h*getHeight();
    float shuffle = synth_data->arp_sequencer_data->shuffle * 0.8f * 60.0f * width_factor;
    if( shuffle != last_shuffle )
    {
        last_shuffle = shuffle;
        for( int i = 0 ; i != sequence_buttons.size() ; ++i )
        {
            TextButton* button = sequence_buttons[i];
            if( i % 4 == 0 )
            {
                button->setSize( 60.0f*width_factor+shuffle, 20.0f * height_factor );
            }
            else if( i % 2 == 0  )
                button->setSize( 60.0f*width_factor+shuffle, 20.0f * height_factor );
            else
                button->setBounds(
                    float(sequence_buttons[i-1]->getX()+60.0f*width_factor+shuffle),
                    float(button->getY()),
                    60.0f*width_factor-shuffle,
                    20.0f * height_factor );
        }
    }
}

void UiEditorSynthLite::switch_finalizer_tab() {
    //reverb
    bool state_switch = eq_1->isVisible();
    reverb_room->setVisible( state_switch );
    colour->setVisible( state_switch );
    reverb_dry->setVisible( state_switch );
    reverb_width->setVisible( state_switch );
    delay2->setVisible( state_switch );
    chorus_modulation->setVisible( state_switch );
    bypass->setVisible( state_switch );
    label_effect_hider->setVisible( !state_switch );
    label_reverb->setVisible( state_switch );
    label_shape->setVisible( state_switch );
    label_delay->setVisible( state_switch );
    label_chorus->setVisible( state_switch );
    label_fx_mix->setVisible( state_switch );

    //eg
    label_eq->setVisible( !state_switch );
    eq_1->setVisible( !state_switch );
    eq_2->setVisible( !state_switch );
    eq_3->setVisible( !state_switch );
    eq_4->setVisible( !state_switch );
    eq_5->setVisible( !state_switch );
    eq_6->setVisible( !state_switch );
    eq_7->setVisible( !state_switch );
    eq_8->setVisible( !state_switch );
    eq_9->setVisible( !state_switch );

    !state_switch ? effect_finalizer_switch->setButtonText ("E Q") : effect_finalizer_switch->setButtonText ("F X");
}
void UiEditorSynthLite::sliderClicked (Slider*s_)
{
    if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() )
        sliderValueChanged(s_);
}
//[/MiscUserDefs]

//==============================================================================
UiEditorSynthLite::UiEditorSynthLite ()
    : AudioProcessorEditor(AppInstanceStore::getInstance()->audio_processor),_app_instance_store(AppInstanceStore::getInstance()),original_w(1465), original_h(1235)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (speed_multi = new mono_ModulationSlider (new SpeedMultiSlConfig()));

    addAndMakeVisible (morpher_4 = new mono_ModulationSlider (new MorphSLConfig(3)));

    addAndMakeVisible (morpher_3 = new mono_ModulationSlider (new MorphSLConfig(2)));

    addAndMakeVisible (morpher_2 = new mono_ModulationSlider (new MorphSLConfig(1)));

    addAndMakeVisible (morpher_1 = new mono_ModulationSlider (new MorphSLConfig(0)));

    addAndMakeVisible (label_effect_hider = new Label (String::empty,
                                                       String::empty));
    label_effect_hider->setFont (Font (30.00f, Font::plain));
    label_effect_hider->setJustificationType (Justification::centredLeft);
    label_effect_hider->setEditable (false, false, false);
    label_effect_hider->setColour (Label::textColourId, Colour (0xffff3b00));
    label_effect_hider->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_effect_hider->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (eq_9 = new mono_ModulationSlider (new EQSlConfig(8)));

    addAndMakeVisible (eq_8 = new mono_ModulationSlider (new EQSlConfig(7)));

    addAndMakeVisible (eq_7 = new mono_ModulationSlider (new EQSlConfig(6)));

    addAndMakeVisible (eq_6 = new mono_ModulationSlider (new EQSlConfig(5)));

    addAndMakeVisible (eq_5 = new mono_ModulationSlider (new EQSlConfig(4)));

    addAndMakeVisible (eq_4 = new mono_ModulationSlider (new EQSlConfig(3)));

    addAndMakeVisible (eq_3 = new mono_ModulationSlider (new EQSlConfig(2)));

    addAndMakeVisible (eq_2 = new mono_ModulationSlider (new EQSlConfig(1)));

    addAndMakeVisible (eq_1 = new mono_ModulationSlider (new EQSlConfig(0)));

    addAndMakeVisible (bypass = new mono_ModulationSlider (new BypassConfig()));

    addAndMakeVisible (chorus_modulation = new mono_ModulationSlider (new CModSlConfig()));

    addAndMakeVisible (reverb_dry = new mono_ModulationSlider (new RDrySlConfig()));

    addAndMakeVisible (reverb_room = new mono_ModulationSlider (new RRoomSlConfig()));

    addAndMakeVisible (osc_wave_3 = new mono_ModulationSlider (new WAVESlConfig(2)));

    addAndMakeVisible (keyboard = new MidiKeyboardComponent (*_app_instance_store->audio_processor, MidiKeyboardComponent::horizontalKeyboard));

    addAndMakeVisible (glide2 = new mono_ModulationSlider (new GlideConfig()));

    addAndMakeVisible (arp_step_16 = new mono_ModulationSlider (new ArpStepSlConfig(15)));

    addAndMakeVisible (arp_step_15 = new mono_ModulationSlider (new ArpStepSlConfig(14)));

    addAndMakeVisible (arp_step_14 = new mono_ModulationSlider (new ArpStepSlConfig(13)));

    addAndMakeVisible (arp_step_13 = new mono_ModulationSlider (new ArpStepSlConfig(12)));

    addAndMakeVisible (arp_step_12 = new mono_ModulationSlider (new ArpStepSlConfig(11)));

    addAndMakeVisible (arp_step_11 = new mono_ModulationSlider (new ArpStepSlConfig(10)));

    addAndMakeVisible (arp_step_10 = new mono_ModulationSlider (new ArpStepSlConfig(9)));

    addAndMakeVisible (arp_step_9 = new mono_ModulationSlider (new ArpStepSlConfig(8)));

    addAndMakeVisible (arp_step_8 = new mono_ModulationSlider (new ArpStepSlConfig(7)));

    addAndMakeVisible (arp_step_7 = new mono_ModulationSlider (new ArpStepSlConfig(6)));

    addAndMakeVisible (arp_step_6 = new mono_ModulationSlider (new ArpStepSlConfig(5)));

    addAndMakeVisible (arp_step_5 = new mono_ModulationSlider (new ArpStepSlConfig(4)));

    addAndMakeVisible (arp_step_4 = new mono_ModulationSlider (new ArpStepSlConfig(3)));

    addAndMakeVisible (arp_step_3 = new mono_ModulationSlider (new ArpStepSlConfig(2)));

    addAndMakeVisible (arp_step_2 = new mono_ModulationSlider (new ArpStepSlConfig(1)));

    addAndMakeVisible (arp_step_1 = new mono_ModulationSlider (new ArpStepSlConfig(0)));

    addAndMakeVisible (shuffle = new mono_ModulationSlider (new ShuffleConfig()));

    addAndMakeVisible (flt_sustain_4 = new mono_ModulationSlider (new FSustainSlConfig(MAIN_ENV)));

    addAndMakeVisible (flt_decay_4 = new mono_ModulationSlider (new FDecaySlConfig(MAIN_ENV)));

    addAndMakeVisible (flt_attack_4 = new mono_ModulationSlider (new FAttackSlConfig(MAIN_ENV)));

    addAndMakeVisible (flt_release_3 = new mono_ModulationSlider (new FReleaseSlConfig(2)));

    addAndMakeVisible (flt_sustain_time_3 = new mono_ModulationSlider (new FSustainTimeSlConfig(2)));

    addAndMakeVisible (flt_sustain_3 = new mono_ModulationSlider (new FSustainSlConfig(2)));

    addAndMakeVisible (flt_decay_3 = new mono_ModulationSlider (new FDecaySlConfig(2)));

    addAndMakeVisible (flt_attack_3 = new mono_ModulationSlider (new FAttackSlConfig(2)));

    addAndMakeVisible (flt_release_2 = new mono_ModulationSlider (new FReleaseSlConfig(1)));

    addAndMakeVisible (flt_sustain_time_2 = new mono_ModulationSlider (new FSustainTimeSlConfig(1)));

    addAndMakeVisible (flt_sustain_2 = new mono_ModulationSlider (new FSustainSlConfig(1)));

    addAndMakeVisible (flt_decay_2 = new mono_ModulationSlider (new FDecaySlConfig(1)));

    addAndMakeVisible (flt_attack_2 = new mono_ModulationSlider (new FAttackSlConfig(1)));

    addAndMakeVisible (flt_release_1 = new mono_ModulationSlider (new FReleaseSlConfig(0)));

    addAndMakeVisible (flt_sustain_time_1 = new mono_ModulationSlider (new FSustainTimeSlConfig(0)));

    addAndMakeVisible (flt_sustain_1 = new mono_ModulationSlider (new FSustainSlConfig(0)));

    addAndMakeVisible (flt_decay_1 = new mono_ModulationSlider (new FDecaySlConfig(0)));

    addAndMakeVisible (flt_attack_1 = new mono_ModulationSlider (new FAttackSlConfig(0)));

    addAndMakeVisible (label_monolisa = new Label (String::empty,
                                                   TRANS("M O N I Q U E")));
    label_monolisa->setFont (Font (30.00f, Font::plain));
    label_monolisa->setJustificationType (Justification::centredRight);
    label_monolisa->setEditable (false, false, false);
    label_monolisa->setColour (Label::textColourId, Colour (0xffff3b00));
    label_monolisa->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_monolisa->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filter_type_1_1 = new TextButton ("VOICE 1"));
    filter_type_1_1->setButtonText (TRANS("- 2PASS"));
    filter_type_1_1->addListener (this);
    filter_type_1_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_1_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_1_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_1 = new TextButton ("VOICE 1"));
    filter_type_2_1->setButtonText (TRANS("HP"));
    filter_type_2_1->addListener (this);
    filter_type_2_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_1 = new TextButton ("VOICE 1"));
    filter_type_3_1->setButtonText (TRANS("BAND"));
    filter_type_3_1->addListener (this);
    filter_type_3_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_1_2 = new TextButton ("VOICE 1"));
    filter_type_1_2->setButtonText (TRANS("- 2PASS"));
    filter_type_1_2->addListener (this);
    filter_type_1_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_1_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_1_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_2 = new TextButton ("VOICE 1"));
    filter_type_2_2->setButtonText (TRANS("HP"));
    filter_type_2_2->addListener (this);
    filter_type_2_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_2 = new TextButton ("VOICE 1"));
    filter_type_3_2->setButtonText (TRANS("BAND"));
    filter_type_3_2->addListener (this);
    filter_type_3_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_1_3 = new TextButton ("VOICE 1"));
    filter_type_1_3->setButtonText (TRANS("- 2PASS"));
    filter_type_1_3->addListener (this);
    filter_type_1_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_1_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_1_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_3 = new TextButton ("VOICE 1"));
    filter_type_2_3->setButtonText (TRANS("HP"));
    filter_type_2_3->addListener (this);
    filter_type_2_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_3 = new TextButton ("VOICE 1"));
    filter_type_3_3->setButtonText (TRANS("BAND"));
    filter_type_3_3->addListener (this);
    filter_type_3_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_1 = new TextButton ("VOICE 1"));
    filter_type_5_1->setButtonText (TRANS("PASS"));
    filter_type_5_1->addListener (this);
    filter_type_5_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_2 = new TextButton ("VOICE 1"));
    filter_type_5_2->setButtonText (TRANS("PASS"));
    filter_type_5_2->addListener (this);
    filter_type_5_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_3 = new TextButton ("VOICE 1"));
    filter_type_5_3->setButtonText (TRANS("PASS"));
    filter_type_5_3->addListener (this);
    filter_type_5_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_2 = new TextButton (String::empty));
    button_sequence_2->addListener (this);
    button_sequence_2->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_3 = new TextButton (String::empty));
    button_sequence_3->addListener (this);
    button_sequence_3->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_4 = new TextButton (String::empty));
    button_sequence_4->addListener (this);
    button_sequence_4->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_5 = new TextButton (String::empty));
    button_sequence_5->addListener (this);
    button_sequence_5->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_5->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_5->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_6 = new TextButton (String::empty));
    button_sequence_6->addListener (this);
    button_sequence_6->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_6->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_6->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_7 = new TextButton (String::empty));
    button_sequence_7->addListener (this);
    button_sequence_7->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_7->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_7->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_8 = new TextButton (String::empty));
    button_sequence_8->addListener (this);
    button_sequence_8->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_8->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_8->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_9 = new TextButton (String::empty));
    button_sequence_9->addListener (this);
    button_sequence_9->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_9->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_9->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_10 = new TextButton (String::empty));
    button_sequence_10->addListener (this);
    button_sequence_10->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_10->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_10->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_11 = new TextButton (String::empty));
    button_sequence_11->addListener (this);
    button_sequence_11->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_11->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_11->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_12 = new TextButton (String::empty));
    button_sequence_12->addListener (this);
    button_sequence_12->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_12->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_12->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_13 = new TextButton (String::empty));
    button_sequence_13->addListener (this);
    button_sequence_13->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_13->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_13->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_14 = new TextButton (String::empty));
    button_sequence_14->addListener (this);
    button_sequence_14->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_14->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_14->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_15 = new TextButton (String::empty));
    button_sequence_15->addListener (this);
    button_sequence_15->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_15->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_15->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_16 = new TextButton (String::empty));
    button_sequence_16->addListener (this);
    button_sequence_16->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_16->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_16->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (combo_programm = new ComboBox (String::empty));
    combo_programm->setEditableText (true);
    combo_programm->setJustificationType (Justification::centredLeft);
    combo_programm->setTextWhenNothingSelected (TRANS("SELECT PROGRAM"));
    combo_programm->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_programm->addListener (this);

    addAndMakeVisible (button_programm_left = new TextButton (String::empty));
    button_programm_left->setButtonText (TRANS("<"));
    button_programm_left->addListener (this);
    button_programm_left->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_left->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_left->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_right = new TextButton (String::empty));
    button_programm_right->setButtonText (TRANS(">"));
    button_programm_right->addListener (this);
    button_programm_right->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_right->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_right->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_replace = new TextButton (String::empty));
    button_programm_replace->setButtonText (TRANS("REPLACE"));
    button_programm_replace->addListener (this);
    button_programm_replace->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_replace->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_replace->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_new = new TextButton (String::empty));
    button_programm_new->setButtonText (TRANS("NEW"));
    button_programm_new->addListener (this);
    button_programm_new->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_new->setColour (TextButton::textColourOnId, Colour (0xffbcff00));
    button_programm_new->setColour (TextButton::textColourOffId, Colour (0xffd0ff00));

    addAndMakeVisible (button_open_oszi = new TextButton (String::empty));
    button_open_oszi->setButtonText (TRANS("OSCI"));
    button_open_oszi->addListener (this);
    button_open_oszi->setColour (TextButton::buttonColourId, Colours::black);
    button_open_oszi->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_oszi->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_open_midi_io_settings = new TextButton (String::empty));
    button_open_midi_io_settings->setButtonText (TRANS("MIDI IO"));
    button_open_midi_io_settings->addListener (this);
    button_open_midi_io_settings->setColour (TextButton::buttonColourId, Colours::black);
    button_open_midi_io_settings->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_midi_io_settings->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (combo_bank = new ComboBox (String::empty));
    combo_bank->setEditableText (false);
    combo_bank->setJustificationType (Justification::centredLeft);
    combo_bank->setTextWhenNothingSelected (String::empty);
    combo_bank->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_bank->addListener (this);

    addAndMakeVisible (button_programm_load = new TextButton (String::empty));
    button_programm_load->setButtonText (TRANS("LOAD"));
    button_programm_load->addListener (this);
    button_programm_load->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_load->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_load->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (osc_1 = new mono_ModulationSlider (new OSCSlConfig(0)));

    addAndMakeVisible (osc_2 = new mono_ModulationSlider (new OSCSlConfig(1)));

    addAndMakeVisible (osc_3 = new mono_ModulationSlider (new OSCSlConfig(2)));

    addAndMakeVisible (lfo_1 = new mono_ModulationSlider (new LFOSlConfig(0)));

    addAndMakeVisible (flt_cutoff_1 = new mono_ModulationSlider (new FCutoffSLConfig(0)));

    addAndMakeVisible (lfo_2 = new mono_ModulationSlider (new LFOSlConfig(1)));

    addAndMakeVisible (lfo_3 = new mono_ModulationSlider (new LFOSlConfig(2)));

    addAndMakeVisible (flt_cutoff_2 = new mono_ModulationSlider (new FCutoffSLConfig(1)));

    addAndMakeVisible (flt_cutoff_3 = new mono_ModulationSlider (new FCutoffSLConfig(2)));

    addAndMakeVisible (flt_input_1 = new mono_ModulationSlider (new InputSlConfig(0,0)));

    addAndMakeVisible (flt_input_2 = new mono_ModulationSlider (new InputSlConfig(0,1)));

    addAndMakeVisible (flt_input_3 = new mono_ModulationSlider (new InputSlConfig(0,2)));

    addAndMakeVisible (flt_compressor_1 = new mono_ModulationSlider (new FCompressorSlConfig(0)));

    addAndMakeVisible (flt_distortion_1 = new mono_ModulationSlider (new GForceSlConfig(0)));

    addAndMakeVisible (flt_input_6 = new mono_ModulationSlider (new InputSlConfig(1,0)));

    addAndMakeVisible (flt_input_7 = new mono_ModulationSlider (new InputSlConfig(1,1)));

    addAndMakeVisible (flt_input_8 = new mono_ModulationSlider (new InputSlConfig(1,2)));

    addAndMakeVisible (flt_compressor_2 = new mono_ModulationSlider (new FCompressorSlConfig(1)));

    addAndMakeVisible (flt_input_11 = new mono_ModulationSlider (new InputSlConfig(2,0)));

    addAndMakeVisible (flt_input_12 = new mono_ModulationSlider (new InputSlConfig(2,1)));

    addAndMakeVisible (flt_input_13 = new mono_ModulationSlider (new InputSlConfig(2,2)));

    addAndMakeVisible (flt_compressor_3 = new mono_ModulationSlider (new FCompressorSlConfig(2)));

    addAndMakeVisible (flt_resonance_1 = new mono_ModulationSlider (new FResonanceSLConfig(0)));

    addAndMakeVisible (flt_gain_1 = new mono_ModulationSlider (new FGainSLConfig(0)));

    addAndMakeVisible (flt_resonance_2 = new mono_ModulationSlider (new FResonanceSLConfig(1)));

    addAndMakeVisible (flt_gain_2 = new mono_ModulationSlider (new FGainSLConfig(1)));

    addAndMakeVisible (flt_resonance_3 = new mono_ModulationSlider (new FResonanceSLConfig(2)));

    addAndMakeVisible (flt_gain_3 = new mono_ModulationSlider (new FGainSLConfig(2)));

    addAndMakeVisible (flt_volume_1 = new mono_ModulationSlider (new FVolumeSlConfig(0)));

    addAndMakeVisible (flt_volume_2 = new mono_ModulationSlider (new FVolumeSlConfig(1)));

    addAndMakeVisible (flt_volume_3 = new mono_ModulationSlider (new FVolumeSlConfig(2)));

    addAndMakeVisible (adsr_lfo_mix = new mono_ModulationSlider (new EnvLfoSlConfig(0)));

    addAndMakeVisible (lfo_opt_2 = new mono_ModulationSlider (new EnvLfoSlConfig(1)));

    addAndMakeVisible (lfo_opt_3 = new mono_ModulationSlider (new EnvLfoSlConfig(2)));

    addAndMakeVisible (button_sequence_1 = new TextButton (String::empty));
    button_sequence_1->addListener (this);
    button_sequence_1->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_release_4 = new mono_ModulationSlider (new FReleaseSlConfig(MAIN_ENV)));

    addAndMakeVisible (delay2 = new mono_ModulationSlider (new DelaySlConfig()));

    addAndMakeVisible (volume = new mono_ModulationSlider (new VolumeConfig()));

    addAndMakeVisible (flt_distortion_2 = new mono_ModulationSlider (new GForceSlConfig(1)));

    addAndMakeVisible (flt_distortion_3 = new mono_ModulationSlider (new GForceSlConfig(2)));

    addAndMakeVisible (button_arp_speed_XNORM = new TextButton (String::empty));
    button_arp_speed_XNORM->setButtonText (TRANS("x1"));
    button_arp_speed_XNORM->addListener (this);
    button_arp_speed_XNORM->setColour (TextButton::buttonColourId, Colours::black);
    button_arp_speed_XNORM->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_arp_speed_XNORM->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_attack_5 = new mono_ModulationSlider (new FMFreqSlConfig()));

    addAndMakeVisible (flt_attack_6 = new mono_ModulationSlider (new FMAmountSlConfig()));

    addAndMakeVisible (osc_wave_1 = new mono_ModulationSlider (new WAVESlConfig(0)));

    addAndMakeVisible (osc_wave_2 = new mono_ModulationSlider (new WAVESlConfig(1)));

    addAndMakeVisible (sl_morhp_mix = new Slider ("new slider"));
    sl_morhp_mix->setRange (0, 3000, 0.01);
    sl_morhp_mix->setSliderStyle (Slider::LinearHorizontal);
    sl_morhp_mix->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_morhp_mix->addListener (this);

    addAndMakeVisible (button_programm_delete = new TextButton (String::empty));
    button_programm_delete->setButtonText (TRANS("DELETE"));
    button_programm_delete->addListener (this);
    button_programm_delete->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_delete->setColour (TextButton::textColourOnId, Colours::red);
    button_programm_delete->setColour (TextButton::textColourOffId, Colour (0xffff7900));

    addAndMakeVisible (button_open_config = new TextButton (String::empty));
    button_open_config->setButtonText (TRANS("ADR"));
    button_open_config->addListener (this);
    button_open_config->setColour (TextButton::buttonColourId, Colours::black);
    button_open_config->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_config->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_1 = new TextButton ("VOICE 1"));
    filter_type_6_1->setButtonText (TRANS("LP"));
    filter_type_6_1->addListener (this);
    filter_type_6_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_2 = new TextButton ("VOICE 1"));
    filter_type_6_2->setButtonText (TRANS("LP"));
    filter_type_6_2->addListener (this);
    filter_type_6_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_3 = new TextButton ("VOICE 1"));
    filter_type_6_3->setButtonText (TRANS("LP"));
    filter_type_6_3->addListener (this);
    filter_type_6_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_midi_learn = new TextButton (String::empty));
    button_midi_learn->setButtonText (TRANS("LEARN"));
    button_midi_learn->addListener (this);
    button_midi_learn->setColour (TextButton::buttonColourId, Colours::black);
    button_midi_learn->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_midi_learn->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_ctrl_toggle = new TextButton (String::empty));
    button_ctrl_toggle->setButtonText (TRANS("SHIFT"));
    button_ctrl_toggle->addListener (this);
    button_ctrl_toggle->setColour (TextButton::buttonColourId, Colours::black);
    button_ctrl_toggle->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_ctrl_toggle->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (colour = new mono_ModulationSlider (new FColourSlConfig()));

    addAndMakeVisible (speed = new mono_ModulationSlider (new BPMSlConfig()));

    addAndMakeVisible (button_open_morph = new TextButton (String::empty));
    button_open_morph->setButtonText (TRANS("EDIT"));
    button_open_morph->addListener (this);
    button_open_morph->setColour (TextButton::buttonColourId, Colours::black);
    button_open_morph->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_morph->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (effect_finalizer_switch = new TextButton (String::empty));
    effect_finalizer_switch->setButtonText (TRANS("F X"));
    effect_finalizer_switch->addListener (this);
    effect_finalizer_switch->setColour (TextButton::buttonColourId, Colours::black);
    effect_finalizer_switch->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    effect_finalizer_switch->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_ui_headline2 = new Label (String::empty,
                                                       TRANS("OSC")));
    label_ui_headline2->setFont (Font (30.00f, Font::plain));
    label_ui_headline2->setJustificationType (Justification::centred);
    label_ui_headline2->setEditable (false, false, false);
    label_ui_headline2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline3 = new Label (String::empty,
                                                       TRANS("FLT")));
    label_ui_headline3->setFont (Font (30.00f, Font::plain));
    label_ui_headline3->setJustificationType (Justification::centred);
    label_ui_headline3->setEditable (false, false, false);
    label_ui_headline3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline5 = new Label (String::empty,
                                                       TRANS("FX")));
    label_ui_headline5->setFont (Font (30.00f, Font::plain));
    label_ui_headline5->setJustificationType (Justification::centred);
    label_ui_headline5->setEditable (false, false, false);
    label_ui_headline5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline6 = new Label (String::empty,
                                                       TRANS("ARP")));
    label_ui_headline6->setFont (Font (30.00f, Font::plain));
    label_ui_headline6->setJustificationType (Justification::centred);
    label_ui_headline6->setEditable (false, false, false);
    label_ui_headline6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_ui_headline6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_values_toggle = new TextButton (String::empty));
    button_values_toggle->setButtonText (TRANS("VAL"));
    button_values_toggle->addListener (this);
    button_values_toggle->setColour (TextButton::buttonColourId, Colours::black);
    button_values_toggle->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_values_toggle->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (reverb_width = new mono_ModulationSlider (new RWidthSlConfig()));

    addAndMakeVisible (octave_offset = new mono_ModulationSlider (new OctaveOffsetSlConfig()));

    addAndMakeVisible (label_ui_headline4 = new Label ("DL",
                                                       TRANS("FILTER INPUTS")));
    label_ui_headline4->setFont (Font (30.00f, Font::plain));
    label_ui_headline4->setJustificationType (Justification::centred);
    label_ui_headline4->setEditable (false, false, false);
    label_ui_headline4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline7 = new Label ("DL",
                                                       TRANS("OSCILLATORS")));
    label_ui_headline7->setFont (Font (30.00f, Font::plain));
    label_ui_headline7->setJustificationType (Justification::centred);
    label_ui_headline7->setEditable (false, false, false);
    label_ui_headline7->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline8 = new Label ("DL",
                                                       TRANS("FILTER ENVELOPE")));
    label_ui_headline8->setFont (Font (30.00f, Font::plain));
    label_ui_headline8->setJustificationType (Justification::centred);
    label_ui_headline8->setEditable (false, false, false);
    label_ui_headline8->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline9 = new Label ("DL",
                                                       TRANS("LFO")));
    label_ui_headline9->setFont (Font (30.00f, Font::plain));
    label_ui_headline9->setJustificationType (Justification::centred);
    label_ui_headline9->setEditable (false, false, false);
    label_ui_headline9->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline10 = new Label ("DL",
                                                        TRANS("FILTER CONFIGURATION")));
    label_ui_headline10->setFont (Font (30.00f, Font::plain));
    label_ui_headline10->setJustificationType (Justification::centred);
    label_ui_headline10->setEditable (false, false, false);
    label_ui_headline10->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline11 = new Label ("DL",
                                                        TRANS("FILTER FX")));
    label_ui_headline11->setFont (Font (30.00f, Font::plain));
    label_ui_headline11->setJustificationType (Justification::centred);
    label_ui_headline11->setEditable (false, false, false);
    label_ui_headline11->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline11->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline12 = new Label ("DL",
                                                        TRANS("OUTPUT")));
    label_ui_headline12->setFont (Font (30.00f, Font::plain));
    label_ui_headline12->setJustificationType (Justification::centred);
    label_ui_headline12->setEditable (false, false, false);
    label_ui_headline12->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline12->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline13 = new Label ("DL",
                                                        TRANS("MAIN ENVELOPE")));
    label_ui_headline13->setFont (Font (30.00f, Font::plain));
    label_ui_headline13->setJustificationType (Justification::centred);
    label_ui_headline13->setEditable (false, false, false);
    label_ui_headline13->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline13->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_shape = new Label ("DL",
                                                TRANS("SHAPE")));
    label_shape->setFont (Font (30.00f, Font::plain));
    label_shape->setJustificationType (Justification::centred);
    label_shape->setEditable (false, false, false);
    label_shape->setColour (Label::textColourId, Colour (0xff1111ff));
    label_shape->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_shape->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_reverb = new Label ("DL",
                                                 TRANS("REVERB")));
    label_reverb->setFont (Font (30.00f, Font::plain));
    label_reverb->setJustificationType (Justification::centred);
    label_reverb->setEditable (false, false, false);
    label_reverb->setColour (Label::textColourId, Colour (0xff1111ff));
    label_reverb->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_reverb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_delay = new Label ("DL",
                                                TRANS("DELAY")));
    label_delay->setFont (Font (30.00f, Font::plain));
    label_delay->setJustificationType (Justification::centred);
    label_delay->setEditable (false, false, false);
    label_delay->setColour (Label::textColourId, Colour (0xff1111ff));
    label_delay->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_delay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_chorus = new Label ("DL",
                                                 TRANS("CHORUS")));
    label_chorus->setFont (Font (30.00f, Font::plain));
    label_chorus->setJustificationType (Justification::centred);
    label_chorus->setEditable (false, false, false);
    label_chorus->setColour (Label::textColourId, Colour (0xff1111ff));
    label_chorus->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_chorus->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_fx_mix = new Label ("DL",
                                                 TRANS("FX MIX")));
    label_fx_mix->setFont (Font (30.00f, Font::plain));
    label_fx_mix->setJustificationType (Justification::centred);
    label_fx_mix->setEditable (false, false, false);
    label_fx_mix->setColour (Label::textColourId, Colour (0xff1111ff));
    label_fx_mix->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_fx_mix->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline19 = new Label ("DL",
                                                        TRANS("MORPH SECTION")));
    label_ui_headline19->setFont (Font (30.00f, Font::plain));
    label_ui_headline19->setJustificationType (Justification::centred);
    label_ui_headline19->setEditable (false, false, false);
    label_ui_headline19->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline20 = new Label ("DL",
                                                        TRANS("ARPEGGIATOR")));
    label_ui_headline20->setFont (Font (30.00f, Font::plain));
    label_ui_headline20->setJustificationType (Justification::centred);
    label_ui_headline20->setEditable (false, false, false);
    label_ui_headline20->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline20->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline21 = new Label ("DL",
                                                        TRANS("GLIDE")));
    label_ui_headline21->setFont (Font (30.00f, Font::plain));
    label_ui_headline21->setJustificationType (Justification::centred);
    label_ui_headline21->setEditable (false, false, false);
    label_ui_headline21->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline21->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline22 = new Label ("DL",
                                                        TRANS("SPEED")));
    label_ui_headline22->setFont (Font (30.00f, Font::plain));
    label_ui_headline22->setJustificationType (Justification::centred);
    label_ui_headline22->setEditable (false, false, false);
    label_ui_headline22->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline22->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline22->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline23 = new Label ("DL",
                                                        TRANS("MASTER")));
    label_ui_headline23->setFont (Font (30.00f, Font::plain));
    label_ui_headline23->setJustificationType (Justification::centred);
    label_ui_headline23->setEditable (false, false, false);
    label_ui_headline23->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline23->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline24 = new Label ("DL",
                                                        TRANS("OCTAVE")));
    label_ui_headline24->setFont (Font (30.00f, Font::plain));
    label_ui_headline24->setJustificationType (Justification::centred);
    label_ui_headline24->setEditable (false, false, false);
    label_ui_headline24->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline24->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline24->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline25 = new Label ("DL",
                                                        TRANS("FM")));
    label_ui_headline25->setFont (Font (30.00f, Font::plain));
    label_ui_headline25->setJustificationType (Justification::centred);
    label_ui_headline25->setEditable (false, false, false);
    label_ui_headline25->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline25->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline25->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (volume_master_meter = new SegmentedMeter());

    addAndMakeVisible (label_eq = new Label ("DL",
                                             TRANS("EQUALIZER")));
    label_eq->setFont (Font (30.00f, Font::plain));
    label_eq->setJustificationType (Justification::centred);
    label_eq->setEditable (false, false, false);
    label_eq->setColour (Label::textColourId, Colour (0xff1111ff));
    label_eq->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_eq->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_open_config2 = new TextButton (String::empty));
    button_open_config2->setButtonText (TRANS("CFG"));
    button_open_config2->addListener (this);
    button_open_config2->setColour (TextButton::buttonColourId, Colours::black);
    button_open_config2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_config2->setColour (TextButton::textColourOffId, Colours::yellow);


    //[UserPreSize]
    SET_SLIDER_STYLE(sl_morhp_mix,VALUE_SLIDER);

    last_bank = -1;
    last_programm = -1;
    is_in_help_mode = false;

    _app_instance_store->audio_processor->peak_meter = volume_master_meter;

    voice = reinterpret_cast< MONOVoice* >( _app_instance_store->audio_processor->synth.getVoice(0) );
    synth_data = _app_instance_store->audio_processor->synth_data.get();
    last_ctrl_mode = synth_data->ctrl = false;

    last_shuffle = -1;
    sequence_buttons.add( button_sequence_1 );
    sequence_buttons.add( button_sequence_2 );
    sequence_buttons.add( button_sequence_3 );
    sequence_buttons.add( button_sequence_4 );
    sequence_buttons.add( button_sequence_5 );
    sequence_buttons.add( button_sequence_6 );
    sequence_buttons.add( button_sequence_7 );
    sequence_buttons.add( button_sequence_8 );
    sequence_buttons.add( button_sequence_9 );
    sequence_buttons.add( button_sequence_10 );
    sequence_buttons.add( button_sequence_11 );
    sequence_buttons.add( button_sequence_12 );
    sequence_buttons.add( button_sequence_13 );
    sequence_buttons.add( button_sequence_14 );
    sequence_buttons.add( button_sequence_15 );
    sequence_buttons.add( button_sequence_16 );

    for( int i = 0 ; i != getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setOpaque(true);
        getChildComponent(i)->setRepaintsOnMouseActivity(false);
    }
    label_monolisa->setOpaque(false);

    switch_finalizer_tab();

    ComponentColours colours = UiLookAndFeel::getInstance()->colours;
    Colour button_off = colours.button_off_colour;

    button_programm_new->setColour(TextButton::buttonColourId, button_off);
    button_programm_replace->setColour(TextButton::buttonColourId, button_off);
    button_programm_delete->setColour(TextButton::buttonColourId, button_off);
    button_programm_load->setColour(TextButton::buttonColourId, button_off);
    button_programm_left->setColour(TextButton::buttonColourId, button_off);
    button_programm_right->setColour(TextButton::buttonColourId, button_off);
    button_open_config->setColour(TextButton::buttonColourId, button_off);
    button_open_oszi->setColour(TextButton::buttonColourId, button_off);
    button_open_midi_io_settings->setColour(TextButton::buttonColourId, button_off);
    effect_finalizer_switch->setColour(TextButton::buttonColourId, button_off);


    setOpaque(true);

    combo_programm->setEditableText(false);

    if( false )
    {
    //[/UserPreSize]

    setSize (1465, 1235);


    //[Constructor] You can add your own custom stuff here..
    }
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
#ifdef IS_PLUGIN
    //resizeLimits.setSizeLimits (1440.0f*0.7f, 1080.0f*0.7f, 1440*10, 1080*10);
    //resizeLimits.setFixedAspectRatio( 1440.0f/1080.0f );
#else
    resizer->setVisible(false);
#endif
    //setSize (1440.0f*0.8f, 1080.0f*0.8f);
    //setSize (1440.0f*0.95f, 1080.0f*0.95f);
    //setSize (1280, 900);
    //setBounds(0,0,1430.0f, 1080.0f);
    //setSize (1430.0f, 1080.0f); // INTROJUCER SIZE

    setSize(original_w*0.8f, original_h*0.8f);
    //setSize (1430.0f, 1080.0f); // INTROJUCER SIZE

    keyboard->setLowestVisibleKey(50);
    keyboard->setAvailableRange( 0, 127 );
    keyboard->setKeyWidth(45);

    //setVisible(true);
    AppInstanceStore::getInstance()->editor = this;
    mono_UiRefresher::getInstance()->startTimer( UI_REFRESH_RATE );

    //UiLookAndFeel::getInstance()->colours.edit();
    //[/Constructor]
}

UiEditorSynthLite::~UiEditorSynthLite()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    mono_UiRefresher::getInstance()->stopTimer();
    mono_UiRefresher::getInstance()->remove_all();
    Thread::sleep(100);

    AppInstanceStore::getInstance()->editor = nullptr;
    Thread::sleep(500); // to be sure we are no more in a update run

    if( mono_AmpPainter* amp_painter = AppInstanceStore::getInstance()->get_amp_painter_unsave() )
    {
        removeChildComponent( amp_painter );
        AppInstanceStore::getInstance()->kill_amp_painter();
    }

    _app_instance_store->audio_processor->peak_meter = nullptr;
    //[/Destructor_pre]

    speed_multi = nullptr;
    morpher_4 = nullptr;
    morpher_3 = nullptr;
    morpher_2 = nullptr;
    morpher_1 = nullptr;
    label_effect_hider = nullptr;
    eq_9 = nullptr;
    eq_8 = nullptr;
    eq_7 = nullptr;
    eq_6 = nullptr;
    eq_5 = nullptr;
    eq_4 = nullptr;
    eq_3 = nullptr;
    eq_2 = nullptr;
    eq_1 = nullptr;
    bypass = nullptr;
    chorus_modulation = nullptr;
    reverb_dry = nullptr;
    reverb_room = nullptr;
    osc_wave_3 = nullptr;
    keyboard = nullptr;
    glide2 = nullptr;
    arp_step_16 = nullptr;
    arp_step_15 = nullptr;
    arp_step_14 = nullptr;
    arp_step_13 = nullptr;
    arp_step_12 = nullptr;
    arp_step_11 = nullptr;
    arp_step_10 = nullptr;
    arp_step_9 = nullptr;
    arp_step_8 = nullptr;
    arp_step_7 = nullptr;
    arp_step_6 = nullptr;
    arp_step_5 = nullptr;
    arp_step_4 = nullptr;
    arp_step_3 = nullptr;
    arp_step_2 = nullptr;
    arp_step_1 = nullptr;
    shuffle = nullptr;
    flt_sustain_4 = nullptr;
    flt_decay_4 = nullptr;
    flt_attack_4 = nullptr;
    flt_release_3 = nullptr;
    flt_sustain_time_3 = nullptr;
    flt_sustain_3 = nullptr;
    flt_decay_3 = nullptr;
    flt_attack_3 = nullptr;
    flt_release_2 = nullptr;
    flt_sustain_time_2 = nullptr;
    flt_sustain_2 = nullptr;
    flt_decay_2 = nullptr;
    flt_attack_2 = nullptr;
    flt_release_1 = nullptr;
    flt_sustain_time_1 = nullptr;
    flt_sustain_1 = nullptr;
    flt_decay_1 = nullptr;
    flt_attack_1 = nullptr;
    label_monolisa = nullptr;
    filter_type_1_1 = nullptr;
    filter_type_2_1 = nullptr;
    filter_type_3_1 = nullptr;
    filter_type_1_2 = nullptr;
    filter_type_2_2 = nullptr;
    filter_type_3_2 = nullptr;
    filter_type_1_3 = nullptr;
    filter_type_2_3 = nullptr;
    filter_type_3_3 = nullptr;
    filter_type_5_1 = nullptr;
    filter_type_5_2 = nullptr;
    filter_type_5_3 = nullptr;
    button_sequence_2 = nullptr;
    button_sequence_3 = nullptr;
    button_sequence_4 = nullptr;
    button_sequence_5 = nullptr;
    button_sequence_6 = nullptr;
    button_sequence_7 = nullptr;
    button_sequence_8 = nullptr;
    button_sequence_9 = nullptr;
    button_sequence_10 = nullptr;
    button_sequence_11 = nullptr;
    button_sequence_12 = nullptr;
    button_sequence_13 = nullptr;
    button_sequence_14 = nullptr;
    button_sequence_15 = nullptr;
    button_sequence_16 = nullptr;
    combo_programm = nullptr;
    button_programm_left = nullptr;
    button_programm_right = nullptr;
    button_programm_replace = nullptr;
    button_programm_new = nullptr;
    button_open_oszi = nullptr;
    button_open_midi_io_settings = nullptr;
    combo_bank = nullptr;
    button_programm_load = nullptr;
    osc_1 = nullptr;
    osc_2 = nullptr;
    osc_3 = nullptr;
    lfo_1 = nullptr;
    flt_cutoff_1 = nullptr;
    lfo_2 = nullptr;
    lfo_3 = nullptr;
    flt_cutoff_2 = nullptr;
    flt_cutoff_3 = nullptr;
    flt_input_1 = nullptr;
    flt_input_2 = nullptr;
    flt_input_3 = nullptr;
    flt_compressor_1 = nullptr;
    flt_distortion_1 = nullptr;
    flt_input_6 = nullptr;
    flt_input_7 = nullptr;
    flt_input_8 = nullptr;
    flt_compressor_2 = nullptr;
    flt_input_11 = nullptr;
    flt_input_12 = nullptr;
    flt_input_13 = nullptr;
    flt_compressor_3 = nullptr;
    flt_resonance_1 = nullptr;
    flt_gain_1 = nullptr;
    flt_resonance_2 = nullptr;
    flt_gain_2 = nullptr;
    flt_resonance_3 = nullptr;
    flt_gain_3 = nullptr;
    flt_volume_1 = nullptr;
    flt_volume_2 = nullptr;
    flt_volume_3 = nullptr;
    adsr_lfo_mix = nullptr;
    lfo_opt_2 = nullptr;
    lfo_opt_3 = nullptr;
    button_sequence_1 = nullptr;
    flt_release_4 = nullptr;
    delay2 = nullptr;
    volume = nullptr;
    flt_distortion_2 = nullptr;
    flt_distortion_3 = nullptr;
    button_arp_speed_XNORM = nullptr;
    flt_attack_5 = nullptr;
    flt_attack_6 = nullptr;
    osc_wave_1 = nullptr;
    osc_wave_2 = nullptr;
    sl_morhp_mix = nullptr;
    button_programm_delete = nullptr;
    button_open_config = nullptr;
    filter_type_6_1 = nullptr;
    filter_type_6_2 = nullptr;
    filter_type_6_3 = nullptr;
    button_midi_learn = nullptr;
    button_ctrl_toggle = nullptr;
    colour = nullptr;
    speed = nullptr;
    button_open_morph = nullptr;
    effect_finalizer_switch = nullptr;
    label_ui_headline2 = nullptr;
    label_ui_headline3 = nullptr;
    label_ui_headline5 = nullptr;
    label_ui_headline6 = nullptr;
    button_values_toggle = nullptr;
    reverb_width = nullptr;
    octave_offset = nullptr;
    label_ui_headline4 = nullptr;
    label_ui_headline7 = nullptr;
    label_ui_headline8 = nullptr;
    label_ui_headline9 = nullptr;
    label_ui_headline10 = nullptr;
    label_ui_headline11 = nullptr;
    label_ui_headline12 = nullptr;
    label_ui_headline13 = nullptr;
    label_shape = nullptr;
    label_reverb = nullptr;
    label_delay = nullptr;
    label_chorus = nullptr;
    label_fx_mix = nullptr;
    label_ui_headline19 = nullptr;
    label_ui_headline20 = nullptr;
    label_ui_headline21 = nullptr;
    label_ui_headline22 = nullptr;
    label_ui_headline23 = nullptr;
    label_ui_headline24 = nullptr;
    label_ui_headline25 = nullptr;
    volume_master_meter = nullptr;
    label_eq = nullptr;
    button_open_config2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorSynthLite::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.fillAll (Colour (0xff161616));
#include "UiDynamicSizeStart.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (480.0f, 595.0f, 604.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (480.0f, 595.0f, 604.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (10.0f, 795.0f, 1445.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (10.0f, 795.0f, 1445.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 20.0f, 1260.0f, 195.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 20.0f, 1260.0f, 195.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (10.0f, 20.0f, 170.0f, 754.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (10.0f, 20.0f, 170.0f, 754.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 44, 25, 3);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 49, 25, 3);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (170.0f, 50.0f, 181.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 39, 25, 3);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 395.0f, 1260.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 395.0f, 1260.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 215.0f, 1260.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 215.0f, 1260.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 595.0f, 284.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 595.0f, 284.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 60.0f, 1.0f, 541.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 50.0f, 201.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 600.0f, 76.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 830.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (90.0f, 830.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 55.0f, 280.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 605.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 600.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (855.0f, 55.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (775.0f, 50.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (615.0f, 760.0f, 421.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (970.0f, 755.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (615.0f, 755.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1035.0f, 755.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1015.0f, 755.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 45.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (875.0f, 50.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 50.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 45.0f, 621.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 50.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 635.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 630.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1260.0f, 720.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1265.0f, 715.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 50.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1045.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1105.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1165.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1255.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (300.0f, 200.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (240.0f, 200.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (360.0f, 200.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1385.0f, 195.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 195.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1315.0f, 200.0f, 91.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 415.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 410.0f, 76.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 410.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 240.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 60.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 185.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 365.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 420.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 545.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 125.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 75.0f, 1.0f, 51.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 100.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 75.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 55.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 40.0f, 1.0f, 16.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 45.0f, 1.0f, 185.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (170.0f, 50.0f, 1.0f, 360.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 235.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 230.0f, 71.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 230.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 45, 3, 10);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 45.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 45.0f, 126.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 50.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 40, 3, 16);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 40.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 40.0f, 136.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (259, 205, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (319, 205, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (379, 205, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 195, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 195, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (349, 195, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 195.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 195.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 195.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (380.0f, 200.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (320.0f, 200.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (260.0f, 200.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1085.0f, 595.0f, 269.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (1085.0f, 595.0f, 269.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1355.0f, 595.0f, 100.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (1355.0f, 595.0f, 100.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 230.0f, 201.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 235.0f, 280.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (855.0f, 235.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (775.0f, 230.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 225.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (875.0f, 230.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 230.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 225.0f, 621.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 230.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 230.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1045.0f, 225.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1105.0f, 225.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1165.0f, 225.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 225.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1255.0f, 225.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1385.0f, 375.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 375.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1315.0f, 380.0f, 91.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 305.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 255.0f, 1.0f, 51.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 280.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 255.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (1314, 205, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1315.0f, 200.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 410.0f, 201.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 415.0f, 280.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (855.0f, 415.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (775.0f, 410.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 405.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (875.0f, 410.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 410.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (785.0f, 405.0f, 621.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (795.0f, 410.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (575.0f, 410.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1045.0f, 405.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1105.0f, 405.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1165.0f, 405.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 405.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1255.0f, 405.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 485.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 435.0f, 1.0f, 51.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 460.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (940.0f, 435.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (1314, 385, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1315.0f, 380.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (300.0f, 380.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (240.0f, 380.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (360.0f, 380.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (259, 385, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (319, 385, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (379, 385, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 375, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 375, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (349, 375, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 375.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 375.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 375.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (380.0f, 380.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (320.0f, 380.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (260.0f, 380.0f, 1.0f, 35.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorSynthLite::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    speed_multi->setBounds (1355 - 60, 950 - 130, 60, 130);
    morpher_4->setBounds (1345 - 60, 620, 60, 130);
    morpher_3->setBounds (1285 - 60, 620, 60, 130);
    morpher_2->setBounds (1225 - 60, 620, 60, 130);
    morpher_1->setBounds (1165 - 60, 620, 60, 130);
    label_effect_hider->setBounds (1065 - 540, 758 - 15, 540, 15);
    eq_9->setBounds (1065 - 60, 750 - 130, 60, 130);
    eq_8->setBounds (1005 - 60, 750 - 130, 60, 130);
    eq_7->setBounds (945 - 60, 750 - 130, 60, 130);
    eq_6->setBounds (885 - 60, 750 - 130, 60, 130);
    eq_5->setBounds (825 - 60, 750 - 130, 60, 130);
    eq_4->setBounds (765 - 60, 750 - 130, 60, 130);
    eq_3->setBounds (705 - 60, 750 - 130, 60, 130);
    eq_2->setBounds (645 - 60, 750 - 130, 60, 130);
    eq_1->setBounds (585 - 60, 750 - 130, 60, 130);
    bypass->setBounds (1065 - 60, 750 - 130, 60, 130);
    chorus_modulation->setBounds (975 - 60, 750 - 130, 60, 130);
    reverb_dry->setBounds (795 - 60, 750 - 130, 60, 130);
    reverb_room->setBounds (675 - 60, 750 - 130, 60, 130);
    osc_wave_3->setBounds (90 - 60, 550 - 130, 60, 130);
    keyboard->setBounds (0, 1055, 1465, 180);
    glide2->setBounds (100, 820, 60, 130);
    arp_step_16->setBounds (1205 - 60, 950 - 130, 60, 130);
    arp_step_15->setBounds (1145 - 60, 950 - 130, 60, 130);
    arp_step_14->setBounds (1085 - 60, 950 - 130, 60, 130);
    arp_step_13->setBounds (965, 820, 60, 130);
    arp_step_12->setBounds (955 - 60, 950 - 130, 60, 130);
    arp_step_11->setBounds (895 - 60, 950 - 130, 60, 130);
    arp_step_10->setBounds (835 - 60, 950 - 130, 60, 130);
    arp_step_9->setBounds (775 - 60, 950 - 130, 60, 130);
    arp_step_8->setBounds (705 - 60, 950 - 130, 60, 130);
    arp_step_7->setBounds (645 - 60, 950 - 130, 60, 130);
    arp_step_6->setBounds (585 - 60, 950 - 130, 60, 130);
    arp_step_5->setBounds (525 - 60, 950 - 130, 60, 130);
    arp_step_4->setBounds (455 - 60, 950 - 130, 60, 130);
    arp_step_3->setBounds (395 - 60, 950 - 130, 60, 130);
    arp_step_2->setBounds (335 - 60, 950 - 130, 60, 130);
    arp_step_1->setBounds (275 - 60, 950 - 130, 60, 130);
    shuffle->setBounds (30, 820, 60, 130);
    flt_sustain_4->setBounds (395 - 60, 750 - 130, 60, 130);
    flt_decay_4->setBounds (335 - 60, 750 - 130, 60, 130);
    flt_attack_4->setBounds (275 - 60, 750 - 130, 60, 130);
    flt_release_3->setBounds (725 - 60, 550 - 130, 60, 130);
    flt_sustain_time_3->setBounds (665 - 60, 550 - 130, 60, 130);
    flt_sustain_3->setBounds (605 - 60, 550 - 130, 60, 130);
    flt_decay_3->setBounds (545 - 60, 550 - 130, 60, 130);
    flt_attack_3->setBounds (485 - 60, 550 - 130, 60, 130);
    flt_release_2->setBounds (725 - 60, 370 - 130, 60, 130);
    flt_sustain_time_2->setBounds (665 - 60, 370 - 130, 60, 130);
    flt_sustain_2->setBounds (605 - 60, 370 - 130, 60, 130);
    flt_decay_2->setBounds (545 - 60, 370 - 130, 60, 130);
    flt_attack_2->setBounds (485 - 60, 370 - 130, 60, 130);
    flt_release_1->setBounds (725 - 60, 190 - 130, 60, 130);
    flt_sustain_time_1->setBounds (665 - 60, 190 - 130, 60, 130);
    flt_sustain_1->setBounds (605 - 60, 190 - 130, 60, 130);
    flt_decay_1->setBounds (545 - 60, 190 - 130, 60, 130);
    flt_attack_1->setBounds (425, 190 - 130, 60, 130);
    label_monolisa->setBounds (1440 - 205, 1055 - 85, 205, 85);
    filter_type_1_1->setBounds (1005 - 60, 112, 60, 24);
    filter_type_2_1->setBounds (1005 - 60, 87, 60, 25);
    filter_type_3_1->setBounds (1005 - 60, 136, 60, 25);
    filter_type_1_2->setBounds (1005 - 60, 292, 60, 24);
    filter_type_2_2->setBounds (1005 - 60, 267, 60, 25);
    filter_type_3_2->setBounds (1005 - 60, 341 - 25, 60, 25);
    filter_type_1_3->setBounds (1005 - 60, 496 - 24, 60, 24);
    filter_type_2_3->setBounds (1005 - 60, 447, 60, 25);
    filter_type_3_3->setBounds (1005 - 60, 496, 60, 25);
    filter_type_5_1->setBounds (1005 - 60, 161, 60, 27);
    filter_type_5_2->setBounds (1005 - 60, 341, 60, 27);
    filter_type_5_3->setBounds (1005 - 60, 520, 60, 27);
    button_sequence_2->setBounds (335 - 60, 840 - 20, 60, 20);
    button_sequence_3->setBounds (395 - 60, 840 - 20, 60, 20);
    button_sequence_4->setBounds (455 - 60, 840 - 20, 60, 20);
    button_sequence_5->setBounds (525 - 60, 840 - 20, 60, 20);
    button_sequence_6->setBounds (585 - 60, 840 - 20, 60, 20);
    button_sequence_7->setBounds (645 - 60, 840 - 20, 60, 20);
    button_sequence_8->setBounds (705 - 60, 840 - 20, 60, 20);
    button_sequence_9->setBounds (775 - 60, 840 - 20, 60, 20);
    button_sequence_10->setBounds (835 - 60, 840 - 20, 60, 20);
    button_sequence_11->setBounds (895 - 60, 840 - 20, 60, 20);
    button_sequence_12->setBounds (955 - 60, 840 - 20, 60, 20);
    button_sequence_13->setBounds (1025 - 60, 840 - 20, 60, 20);
    button_sequence_14->setBounds (1085 - 60, 840 - 20, 60, 20);
    button_sequence_15->setBounds (1145 - 60, 840 - 20, 60, 20);
    button_sequence_16->setBounds (1205 - 60, 840 - 20, 60, 20);
    combo_programm->setBounds (645 - 310, 1030 - 30, 310, 30);
    button_programm_left->setBounds (275 - 60, 1030 - 30, 60, 30);
    button_programm_right->setBounds (705 - 60, 1030 - 30, 60, 30);
    button_programm_replace->setBounds (835 - 60, 1030 - 30, 60, 30);
    button_programm_new->setBounds (775 - 60, 1030 - 30, 60, 30);
    button_open_oszi->setBounds (965, 1000, 60, 30);
    button_open_midi_io_settings->setBounds (1205 - 60, 1030 - 30, 60, 30);
    combo_bank->setBounds (335 - 60, 1030 - 30, 60, 30);
    button_programm_load->setBounds (955 - 60, 1030 - 30, 60, 30);
    osc_1->setBounds (160 - 60, 190 - 130, 60, 130);
    osc_2->setBounds (160 - 60, 370 - 130, 60, 130);
    osc_3->setBounds (160 - 60, 550 - 130, 60, 130);
    lfo_1->setBounds (905 - 60, 190 - 130, 60, 130);
    flt_cutoff_1->setBounds (1075 - 60, 190 - 130, 60, 130);
    lfo_2->setBounds (905 - 60, 370 - 130, 60, 130);
    lfo_3->setBounds (905 - 60, 550 - 130, 60, 130);
    flt_cutoff_2->setBounds (1075 - 60, 370 - 130, 60, 130);
    flt_cutoff_3->setBounds (1075 - 60, 550 - 130, 60, 130);
    flt_input_1->setBounds (215, 60, 60, 130);
    flt_input_2->setBounds (335 - 60, 190 - 130, 60, 130);
    flt_input_3->setBounds (395 - 60, 190 - 130, 60, 130);
    flt_compressor_1->setBounds (1345 - 60, 190 - 130, 60, 130);
    flt_distortion_1->setBounds (1285 - 60, 190 - 130, 60, 130);
    flt_input_6->setBounds (275 - 60, 370 - 130, 60, 130);
    flt_input_7->setBounds (335 - 60, 370 - 130, 60, 130);
    flt_input_8->setBounds (395 - 60, 370 - 130, 60, 130);
    flt_compressor_2->setBounds (1345 - 60, 370 - 130, 60, 130);
    flt_input_11->setBounds (275 - 60, 550 - 130, 60, 130);
    flt_input_12->setBounds (335 - 60, 550 - 130, 60, 130);
    flt_input_13->setBounds (395 - 60, 550 - 130, 60, 130);
    flt_compressor_3->setBounds (1345 - 60, 550 - 130, 60, 130);
    flt_resonance_1->setBounds (1135 - 60, 190 - 130, 60, 130);
    flt_gain_1->setBounds (1195 - 60, 190 - 130, 60, 130);
    flt_resonance_2->setBounds (1135 - 60, 370 - 130, 60, 130);
    flt_gain_2->setBounds (1195 - 60, 370 - 130, 60, 130);
    flt_resonance_3->setBounds (1135 - 60, 550 - 130, 60, 130);
    flt_gain_3->setBounds (1195 - 60, 550 - 130, 60, 130);
    flt_volume_1->setBounds (1435 - 60, 190 - 130, 60, 130);
    flt_volume_2->setBounds (1435 - 60, 370 - 130, 60, 130);
    flt_volume_3->setBounds (1435 - 60, 550 - 130, 60, 130);
    adsr_lfo_mix->setBounds (815 - 60, 190 - 130, 60, 130);
    lfo_opt_2->setBounds (815 - 60, 370 - 130, 60, 130);
    lfo_opt_3->setBounds (815 - 60, 550 - 130, 60, 130);
    button_sequence_1->setBounds (275 - 60, 840 - 20, 60, 20);
    flt_release_4->setBounds (455 - 60, 750 - 130, 60, 130);
    delay2->setBounds (885 - 60, 750 - 130, 60, 130);
    volume->setBounds (1435 - 60, 750 - 130, 60, 130);
    flt_distortion_2->setBounds (1285 - 60, 370 - 130, 60, 130);
    flt_distortion_3->setBounds (1285 - 60, 550 - 130, 60, 130);
    button_arp_speed_XNORM->setBounds (1295, 820, 60, 27);
    flt_attack_5->setBounds (90 - 60, 755 - 130, 60, 130);
    flt_attack_6->setBounds (160 - 60, 755 - 130, 60, 130);
    osc_wave_1->setBounds (30, 60, 60, 130);
    osc_wave_2->setBounds (90 - 60, 370 - 130, 60, 130);
    sl_morhp_mix->setBounds (1165, 715, 180, 33);
    button_programm_delete->setBounds (895 - 60, 1030 - 30, 60, 30);
    button_open_config->setBounds (1085 - 60, 1030 - 30, 60, 30);
    filter_type_6_1->setBounds (1005 - 60, 60, 60, 27);
    filter_type_6_2->setBounds (1005 - 60, 240, 60, 27);
    filter_type_6_3->setBounds (1005 - 60, 420, 60, 27);
    button_midi_learn->setBounds (1205 - 60, 1000 - 30, 60, 30);
    button_ctrl_toggle->setBounds (100, 1000, 60, 30);
    colour->setBounds (585 - 60, 750 - 130, 60, 130);
    speed->setBounds (1285 - 60, 950 - 130, 60, 130);
    button_open_morph->setBounds (1165 - 60, 715, 60, 33);
    effect_finalizer_switch->setBounds (520 - 25, 620, 25, 130);
    label_ui_headline2->setBounds (1105, 616, 60, 35);
    label_ui_headline3->setBounds (1165, 616, 60, 35);
    label_ui_headline5->setBounds (1225, 616, 60, 35);
    label_ui_headline6->setBounds (1285, 616, 60, 35);
    button_values_toggle->setBounds (30, 1000, 60, 30);
    reverb_width->setBounds (735 - 60, 750 - 130, 60, 130);
    octave_offset->setBounds (1435 - 60, 950 - 130, 60, 130);
    label_ui_headline4->setBounds (245, 5, 120, 35);
    label_ui_headline7->setBounds (40, 5, 110, 35);
    label_ui_headline8->setBounds (510, 5, 130, 35);
    label_ui_headline9->setBounds (850, 5, 50, 35);
    label_ui_headline10->setBounds (975, 5, 190, 35);
    label_ui_headline11->setBounds (1230, 5, 110, 35);
    label_ui_headline12->setBounds (1375, 5, 60, 35);
    label_ui_headline13->setBounds (270, 580, 130, 35);
    label_shape->setBounds (525, 580, 60, 35);
    label_reverb->setBounds (665, 580, 80, 35);
    label_delay->setBounds (825, 580, 60, 35);
    label_chorus->setBounds (910, 580, 70, 35);
    label_fx_mix->setBounds (1005, 580, 60, 35);
    label_ui_headline19->setBounds (1135, 580, 180, 35);
    label_ui_headline20->setBounds (615, 780, 190, 35);
    label_ui_headline21->setBounds (100, 780, 60, 35);
    label_ui_headline22->setBounds (1260, 780, 60, 35);
    label_ui_headline23->setBounds (1375, 580, 60, 35);
    label_ui_headline24->setBounds (1375, 780, 60, 35);
    label_ui_headline25->setBounds (35, 563, 120, 35);
    volume_master_meter->setBounds (1375, 620, 60, 27);
    label_eq->setBounds (740, 580, 90, 35);
    button_open_config2->setBounds (1145 - 60, 1030 - 30, 60, 30);
    //[UserResized] Add your own custom resize handling here..
    if( resizer )
        resizer->setBounds (original_w - 16, original_h - 16, 16, 16);
#include "UiDynamicSizeEnd.h"

    if( mono_AmpPainter* amp_painter = AppInstanceStore::getInstance()->get_amp_painter_unsave() )
        amp_painter->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
    //[/UserResized]
}

void UiEditorSynthLite::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == filter_type_1_1)
    {
        //[UserButtonCode_filter_type_1_1] -- add your button handler code here..
        int flt_id = 0;
        Array< Component* > buttons;
        buttons.add( filter_type_1_1 );
        buttons.add( filter_type_2_1 );
        buttons.add( filter_type_3_1 );
        //buttons.add( filter_bypass_1 );
        buttons.add( filter_type_5_1 );
        buttons.add( filter_type_6_1 );
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttons
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF;
            }
            else if( current_filter_type == HIGH_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = HPF;
            }
            else if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_1_1]
    }
    else if (buttonThatWasClicked == filter_type_2_1)
    {
        //[UserButtonCode_filter_type_2_1] -- add your button handler code here..
        int flt_id = 0;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = HPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_2_1]
    }
    else if (buttonThatWasClicked == filter_type_3_1)
    {
        //[UserButtonCode_filter_type_3_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[0]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[0]->filter_type = BPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[0]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_3_1]
    }
    else if (buttonThatWasClicked == filter_type_1_2)
    {
        //[UserButtonCode_filter_type_1_2] -- add your button handler code here..
        int flt_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF;
            }
            else if( current_filter_type == HIGH_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = HPF;
            }
            else if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_1_2]
    }
    else if (buttonThatWasClicked == filter_type_2_2)
    {
        //[UserButtonCode_filter_type_2_2] -- add your button handler code here..
        int flt_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[1]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = HPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[1]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_2_2]
    }
    else if (buttonThatWasClicked == filter_type_3_2)
    {
        //[UserButtonCode_filter_type_3_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[1]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[1]->filter_type = BPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[1]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_3_2]
    }
    else if (buttonThatWasClicked == filter_type_1_3)
    {
        //[UserButtonCode_filter_type_1_3] -- add your button handler code here..
        int flt_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF;
            }
            else if( current_filter_type == HIGH_2_PASS ) {
                synth_data->filter_datas[flt_id]->filter_type = HPF;
            }
            else if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_1_3]
    }
    else if (buttonThatWasClicked == filter_type_2_3)
    {
        //[UserButtonCode_filter_type_2_3] -- add your button handler code here..
        int flt_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == HPF ) {
                synth_data->filter_datas[flt_id]->filter_type = HIGH_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = HPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_2_3]
    }
    else if (buttonThatWasClicked == filter_type_3_3)
    {
        //[UserButtonCode_filter_type_3_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[2]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[2]->filter_type = BPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[2]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_3_3]
    }
    else if (buttonThatWasClicked == filter_type_5_1)
    {
        //[UserButtonCode_filter_type_5_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[0]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[0]->filter_type = PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[0]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_5_1]
    }
    else if (buttonThatWasClicked == filter_type_5_2)
    {
        //[UserButtonCode_filter_type_5_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[1]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[1]->filter_type = PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[1]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_5_2]
    }
    else if (buttonThatWasClicked == filter_type_5_3)
    {
        //[UserButtonCode_filter_type_5_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[2]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            synth_data->filter_datas[2]->filter_type = PASS;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[2]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_5_3]
    }
    else if (buttonThatWasClicked == button_sequence_2)
    {
        //[UserButtonCode_button_sequence_2] -- add your button handler code here..
        int step_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_2]
    }
    else if (buttonThatWasClicked == button_sequence_3)
    {
        //[UserButtonCode_button_sequence_3] -- add your button handler code here..
        int step_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_3]
    }
    else if (buttonThatWasClicked == button_sequence_4)
    {
        //[UserButtonCode_button_sequence_4] -- add your button handler code here..
        int step_id = 3;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_4]
    }
    else if (buttonThatWasClicked == button_sequence_5)
    {
        //[UserButtonCode_button_sequence_5] -- add your button handler code here..
        int step_id = 4;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_5]
    }
    else if (buttonThatWasClicked == button_sequence_6)
    {
        //[UserButtonCode_button_sequence_6] -- add your button handler code here..
        int step_id = 5;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_6]
    }
    else if (buttonThatWasClicked == button_sequence_7)
    {
        //[UserButtonCode_button_sequence_7] -- add your button handler code here..
        int step_id = 6;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_7]
    }
    else if (buttonThatWasClicked == button_sequence_8)
    {
        //[UserButtonCode_button_sequence_8] -- add your button handler code here..
        int step_id = 7;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_8]
    }
    else if (buttonThatWasClicked == button_sequence_9)
    {
        //[UserButtonCode_button_sequence_9] -- add your button handler code here..
        int step_id = 8;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_9]
    }
    else if (buttonThatWasClicked == button_sequence_10)
    {
        //[UserButtonCode_button_sequence_10] -- add your button handler code here..
        int step_id = 9;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_10]
    }
    else if (buttonThatWasClicked == button_sequence_11)
    {
        //[UserButtonCode_button_sequence_11] -- add your button handler code here..
        int step_id = 10;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_11]
    }
    else if (buttonThatWasClicked == button_sequence_12)
    {
        //[UserButtonCode_button_sequence_12] -- add your button handler code here..
        int step_id = 11;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_12]
    }
    else if (buttonThatWasClicked == button_sequence_13)
    {
        //[UserButtonCode_button_sequence_13] -- add your button handler code here..
        int step_id = 12;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_13]
    }
    else if (buttonThatWasClicked == button_sequence_14)
    {
        //[UserButtonCode_button_sequence_14] -- add your button handler code here..
        int step_id = 13;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_14]
    }
    else if (buttonThatWasClicked == button_sequence_15)
    {
        //[UserButtonCode_button_sequence_15] -- add your button handler code here..
        int step_id = 14;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_15]
    }
    else if (buttonThatWasClicked == button_sequence_16)
    {
        //[UserButtonCode_button_sequence_16] -- add your button handler code here..
        int step_id = 15;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[step_id],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[step_id]  ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[step_id].midi_control );
        //[/UserButtonCode_button_sequence_16]
    }
    else if (buttonThatWasClicked == button_programm_left)
    {
        //[UserButtonCode_button_programm_left] -- add your button handler code here..
        synth_data->load_prev();
        show_programs_and_select();
        //[/UserButtonCode_button_programm_left]
    }
    else if (buttonThatWasClicked == button_programm_right)
    {
        //[UserButtonCode_button_programm_right] -- add your button handler code here..
        synth_data->load_next();
        show_programs_and_select();
        //[/UserButtonCode_button_programm_right]
    }
    else if (buttonThatWasClicked == button_programm_replace)
    {
        //[UserButtonCode_button_programm_replace] -- add your button handler code here..
        synth_data->replace();
        show_programs_and_select();
        //[/UserButtonCode_button_programm_replace]
    }
    else if (buttonThatWasClicked == button_programm_new)
    {
        //[UserButtonCode_button_programm_new] -- add your button handler code here..
        combo_programm->setEditableText(true);
        combo_programm->showEditor();
        //[/UserButtonCode_button_programm_new]
    }
    else if (buttonThatWasClicked == button_open_oszi)
    {
        //[UserButtonCode_button_open_oszi] -- add your button handler code here..
        if( mono_AmpPainter* amp_painter = AppInstanceStore::getInstance()->get_amp_painter_unsave() )
        {
            removeChildComponent( amp_painter );
            AppInstanceStore::getInstance()->kill_amp_painter();
        }
        else if( mono_AmpPainter* amp_painter = AppInstanceStore::getInstance()->get_create_amp_painter() )
        {
            editor_midiio = nullptr;
            editor_morph = nullptr;
            editor_settings = nullptr;
            editor_global_settings = nullptr;
            popup = nullptr;

            addAndMakeVisible( amp_painter );
            amp_painter->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
        }
        //[/UserButtonCode_button_open_oszi]
    }
    else if (buttonThatWasClicked == button_open_midi_io_settings)
    {
        //[UserButtonCode_button_open_midi_io_settings] -- add your button handler code here..
        if( editor_midiio )
            editor_midiio = nullptr;
        else
        {
            close_all_subeditors();

            addAndMakeVisible( editor_midiio = new UiEditorMIDIIO( _app_instance_store->audio_processor ) );
            editor_midiio->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
        }
        //[/UserButtonCode_button_open_midi_io_settings]
    }
    else if (buttonThatWasClicked == button_programm_load)
    {
        //[UserButtonCode_button_programm_load] -- add your button handler code here..
        synth_data->load();
        show_programs_and_select();
        //[/UserButtonCode_button_programm_load]
    }
    else if (buttonThatWasClicked == button_sequence_1)
    {
        //[UserButtonCode_button_sequence_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->step[0],
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->step[0] ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->step[0].midi_control );
        //[/UserButtonCode_button_sequence_1]
    }
    else if (buttonThatWasClicked == button_arp_speed_XNORM)
    {
        //[UserButtonCode_button_arp_speed_XNORM] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->arp_sequencer_data->speed_multi,
            buttonThatWasClicked
        )
        else
        {
            synth_data->arp_sequencer_data->speed_multi = 0;
        }
        show_info_popup( buttonThatWasClicked, synth_data->arp_sequencer_data->speed_multi.midi_control );
        //[/UserButtonCode_button_arp_speed_XNORM]
    }
    else if (buttonThatWasClicked == button_programm_delete)
    {
        //[UserButtonCode_button_programm_delete] -- add your button handler code here..
        if( synth_data->remove() ) {
            last_programm = -1;
            show_programs_and_select();
        }
        else
            show_programs_and_select();
        //[/UserButtonCode_button_programm_delete]
    }
    else if (buttonThatWasClicked == button_open_config)
    {
        //[UserButtonCode_button_open_config] -- add your button handler code here..
        if( ! editor_settings )
        {
            close_all_subeditors();

            addAndMakeVisible( editor_settings = new UiEditorSettings() );
            editor_settings->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
        }
        else
            editor_settings = nullptr;
        //[/UserButtonCode_button_open_config]
    }
    else if (buttonThatWasClicked == filter_type_6_1)
    {
        //[UserButtonCode_filter_type_6_1] -- add your button handler code here..
        int flt_id = 0;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_6_1]
    }
    else if (buttonThatWasClicked == filter_type_6_2)
    {
        //[UserButtonCode_filter_type_6_2] -- add your button handler code here..
        int flt_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_6_2]
    }
    else if (buttonThatWasClicked == filter_type_6_3)
    {
        //[UserButtonCode_filter_type_6_3] -- add your button handler code here..
        int flt_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->filter_datas[flt_id]->filter_type,
            buttonThatWasClicked
        )
        else
        {
            uint8 current_filter_type = synth_data->filter_datas[flt_id]->filter_type;
            if( current_filter_type == LPF ) {
                synth_data->filter_datas[flt_id]->filter_type = LPF_2_PASS;
            }
            else
                synth_data->filter_datas[flt_id]->filter_type = LPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_6_3]
    }
    else if (buttonThatWasClicked == button_midi_learn)
    {
        //[UserButtonCode_button_midi_learn] -- add your button handler code here..
        popup = nullptr;
        MIDIControlHandler::getInstance()->toggle_midi_learn();
        //[/UserButtonCode_button_midi_learn]
    }
    else if (buttonThatWasClicked == button_ctrl_toggle)
    {
        //[UserButtonCode_button_ctrl_toggle] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->ctrl,
            buttonThatWasClicked
        )
        else
        {
            synth_data->ctrl ^= true;
            show_ctrl_state();
        }
        show_info_popup( buttonThatWasClicked, synth_data->ctrl.midi_control );
        //[/UserButtonCode_button_ctrl_toggle]
    }
    else if (buttonThatWasClicked == button_open_morph)
    {
        //[UserButtonCode_button_open_morph] -- add your button handler code here..
        if( ! editor_morph )
        {
	    close_all_subeditors();

            addAndMakeVisible( editor_morph = new UiEditorMorph() );
            editor_morph->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
        }
        else
            editor_morph = nullptr;
        //[/UserButtonCode_button_open_morph]
    }
    else if (buttonThatWasClicked == effect_finalizer_switch)
    {
        //[UserButtonCode_effect_finalizer_switch] -- add your button handler code here..
        switch_finalizer_tab();
        //[/UserButtonCode_effect_finalizer_switch]
    }
    else if (buttonThatWasClicked == button_values_toggle)
    {
        //[UserButtonCode_button_values_toggle] -- add your button handler code here..
        UiLookAndFeel::getInstance()->show_values_always ^= true;
        //[/UserButtonCode_button_values_toggle]
    }
    else if (buttonThatWasClicked == button_open_config2)
    {
        //[UserButtonCode_button_open_config2] -- add your button handler code here..
        if( ! editor_global_settings )
        {
            close_all_subeditors();

            addAndMakeVisible( editor_global_settings = new UiEditorGlobalSettings() );
            editor_global_settings->setBounds( keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight() );
        }
        else
            editor_global_settings = nullptr;
        //[/UserButtonCode_button_open_config2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void UiEditorSynthLite::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_programm)
    {
        //[UserComboBoxCode_combo_programm] -- add your combo box handling code here..
        combo_programm->setEditableText(false);

        String new_name = combo_programm->getText();
        String old_name = combo_programm->getItemText(combo_programm->getSelectedItemIndex());
        if( old_name != new_name && combo_programm->getSelectedItemIndex() == -1 )
        {
            synth_data->create_new();
            synth_data->rename( new_name );
            show_programs_and_select();
        }
        else
        {
            synth_data->set_current_program( combo_programm->getSelectedItemIndex() );
            synth_data->load();
        }
        //[/UserComboBoxCode_combo_programm]
    }
    else if (comboBoxThatHasChanged == combo_bank)
    {
        //[UserComboBoxCode_combo_bank] -- add your combo box handling code here..
        synth_data->set_current_bank( combo_bank->getSelectedItemIndex() );
        show_programs_and_select();
        //[/UserComboBoxCode_combo_bank]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void UiEditorSynthLite::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_morhp_mix)
    {
        //[UserSliderCode_sl_morhp_mix] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->linear_morhp_state,
            sliderThatWasMoved
        )
        else
        {
            synth_data->linear_morhp_state = sl_morhp_mix->getValue()/1000;
        }
        show_info_popup( sliderThatWasMoved, synth_data->linear_morhp_state.midi_control );
        //[/UserSliderCode_sl_morhp_mix]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

bool UiEditorSynthLite::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        MIDIControlHandler::getInstance()->clear();

        synth_data->ctrl = false;

        close_all_subeditors();
        success = true;
    }
    return success;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

void UiEditorSynthLite::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    if( ! combo_programm->isTextEditable() ) {
        synth_data->ctrl = modifiers.isShiftDown();
        show_ctrl_state();
    }
    //else
    //    Component::modifierKeysChanged( modifiers );
    //[/UserCode_modifierKeysChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void UiEditorSynthLite::close_all_subeditors() {
    editor_midiio = nullptr;
    editor_morph = nullptr;
    editor_settings = nullptr;
    editor_global_settings = nullptr;
    popup = nullptr;

    if( mono_AmpPainter* amp_painter = AppInstanceStore::getInstance()->get_amp_painter_unsave() )
    {
        removeChildComponent( amp_painter );
        AppInstanceStore::getInstance()->kill_amp_painter();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiEditorSynthLite" componentName=""
                 parentClasses="public AudioProcessorEditor, public mono_UiRefreshable"
                 constructorParams="" variableInitialisers="AudioProcessorEditor(AppInstanceStore::getInstance()-&gt;audio_processor),_app_instance_store(AppInstanceStore::getInstance()),original_w(1465), original_h(1235)"
                 snapPixels="5" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="1235">
  <METHODS>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="480 595 604 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="10 795 1445 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 20 1260 195" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="10 20 170 754" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <RECT pos="175 44 25 3" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="175 49 25 3" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="170 50 181 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="175 39 25 3" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="195 395 1260 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 215 1260 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 595 284 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="20 60 1 541" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="575 50 201 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 600 76 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 830 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="90 830 10 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 55 280 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 605 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 600 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="855 55 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="775 50 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="615 760 421 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="970 755 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="615 755 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1035 755 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1015 755 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 45 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="875 50 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 50 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 45 621 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 50 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 635 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 630 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1260 720 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1265 715 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="575 50 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1045 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1105 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1165 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1255 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="300 200 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="240 200 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="360 200 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1385 195 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 195 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1315 200 91 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 415 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 410 76 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 410 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 240 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 60 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 185 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 365 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 420 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 545 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 125 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 75 1 51" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 100 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 75 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 55 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 40 1 16" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 45 1 185" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="170 50 1 360" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 235 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 230 71 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 230 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="289 45 3 10" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="290 45 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 45 126 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="350 50 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="229 40 3 16" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="230 40 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 40 136 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="259 205 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="319 205 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="379 205 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="229 195 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="289 195 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="349 195 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="350 195 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="290 195 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="230 195 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="380 200 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="320 200 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="260 200 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1085 595 269 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="1355 595 100 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="575 230 201 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 235 280 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="855 235 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="775 230 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 225 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="875 230 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 230 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 225 621 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 230 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="575 230 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1045 225 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1105 225 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1165 225 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 225 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1255 225 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1385 375 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 375 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1315 380 91 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 305 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 255 1 51" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 280 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 255 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="1314 205 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="1315 200 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="575 410 201 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 415 280 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="855 415 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="775 410 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 405 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="875 410 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 410 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="785 405 621 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="795 410 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="575 410 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1045 405 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1105 405 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1165 405 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 405 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1255 405 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 485 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 435 1 51" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 460 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="940 435 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="1314 385 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="1315 380 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="300 380 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="240 380 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="360 380 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="259 385 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="319 385 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="379 385 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="229 375 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="289 375 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="349 375 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="350 375 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="290 375 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="230 375 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="380 380 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="320 380 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="260 380 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="8916123bb68766dc" memberName="speed_multi" virtualName=""
                    explicitFocusOrder="0" pos="1355r 950r 60 130" class="mono_ModulationSlider"
                    params="new SpeedMultiSlConfig()"/>
  <GENERICCOMPONENT name="" id="f1f5ea6816f11113" memberName="morpher_4" virtualName=""
                    explicitFocusOrder="0" pos="1345r 620 60 130" class="mono_ModulationSlider"
                    params="new MorphSLConfig(3)"/>
  <GENERICCOMPONENT name="" id="6319f13308da05dc" memberName="morpher_3" virtualName=""
                    explicitFocusOrder="0" pos="1285r 620 60 130" class="mono_ModulationSlider"
                    params="new MorphSLConfig(2)"/>
  <GENERICCOMPONENT name="" id="d7bed13dc76b014a" memberName="morpher_2" virtualName=""
                    explicitFocusOrder="0" pos="1225r 620 60 130" class="mono_ModulationSlider"
                    params="new MorphSLConfig(1)"/>
  <GENERICCOMPONENT name="" id="ab7bfe937e5ada83" memberName="morpher_1" virtualName=""
                    explicitFocusOrder="0" pos="1165r 620 60 130" class="mono_ModulationSlider"
                    params="new MorphSLConfig(0)"/>
  <LABEL name="" id="4a610cd12c392ab8" memberName="label_effect_hider"
         virtualName="" explicitFocusOrder="0" pos="1065r 758r 540 15"
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText=""
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="" id="6250362aea841eea" memberName="eq_9" virtualName=""
                    explicitFocusOrder="0" pos="1065r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(8)"/>
  <GENERICCOMPONENT name="" id="26ebe414133b55e7" memberName="eq_8" virtualName=""
                    explicitFocusOrder="0" pos="1005r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(7)"/>
  <GENERICCOMPONENT name="" id="12a573d837478d38" memberName="eq_7" virtualName=""
                    explicitFocusOrder="0" pos="945r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(6)"/>
  <GENERICCOMPONENT name="" id="2b128fb147c2823c" memberName="eq_6" virtualName=""
                    explicitFocusOrder="0" pos="885r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(5)"/>
  <GENERICCOMPONENT name="" id="8a0f89a0c0f219b8" memberName="eq_5" virtualName=""
                    explicitFocusOrder="0" pos="825r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(4)"/>
  <GENERICCOMPONENT name="" id="3b0e3a8ef55d061a" memberName="eq_4" virtualName=""
                    explicitFocusOrder="0" pos="765r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(3)"/>
  <GENERICCOMPONENT name="" id="1dbf561cd93cbd59" memberName="eq_3" virtualName=""
                    explicitFocusOrder="0" pos="705r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="30a759af59bc090b" memberName="eq_2" virtualName=""
                    explicitFocusOrder="0" pos="645r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="5d07e2bb48e90cc6" memberName="eq_1" virtualName=""
                    explicitFocusOrder="0" pos="585r 750r 60 130" class="mono_ModulationSlider"
                    params="new EQSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="83c667b94dd3ef45" memberName="bypass" virtualName=""
                    explicitFocusOrder="0" pos="1065r 750r 60 130" class="mono_ModulationSlider"
                    params="new BypassConfig()"/>
  <GENERICCOMPONENT name="" id="9378cae1ce589256" memberName="chorus_modulation"
                    virtualName="" explicitFocusOrder="0" pos="975r 750r 60 130"
                    class="mono_ModulationSlider" params="new CModSlConfig()"/>
  <GENERICCOMPONENT name="" id="9d2507984890a079" memberName="reverb_dry" virtualName=""
                    explicitFocusOrder="0" pos="795r 750r 60 130" class="mono_ModulationSlider"
                    params="new RDrySlConfig()"/>
  <GENERICCOMPONENT name="" id="19311f1c6e549e68" memberName="reverb_room" virtualName=""
                    explicitFocusOrder="0" pos="675r 750r 60 130" class="mono_ModulationSlider"
                    params="new RRoomSlConfig()"/>
  <GENERICCOMPONENT name="" id="92e86ca444a56d1e" memberName="osc_wave_3" virtualName=""
                    explicitFocusOrder="0" pos="90r 550r 60 130" class="mono_ModulationSlider"
                    params="new WAVESlConfig(2)"/>
  <GENERICCOMPONENT name="" id="a8343a0b5df2dc06" memberName="keyboard" virtualName="MidiKeyboardComponent"
                    explicitFocusOrder="0" pos="0 1055 1465 180" class="Component"
                    params="*_app_instance_store-&gt;audio_processor, MidiKeyboardComponent::horizontalKeyboard"/>
  <GENERICCOMPONENT name="" id="35003b6b21577713" memberName="glide2" virtualName=""
                    explicitFocusOrder="0" pos="100 820 60 130" class="mono_ModulationSlider"
                    params="new GlideConfig()"/>
  <GENERICCOMPONENT name="" id="d8ef93ac038fadca" memberName="arp_step_16" virtualName=""
                    explicitFocusOrder="0" pos="1205r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(15)"/>
  <GENERICCOMPONENT name="" id="7761deb0276debbd" memberName="arp_step_15" virtualName=""
                    explicitFocusOrder="0" pos="1145r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(14)"/>
  <GENERICCOMPONENT name="" id="20a9ed6504a039e2" memberName="arp_step_14" virtualName=""
                    explicitFocusOrder="0" pos="1085r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(13)"/>
  <GENERICCOMPONENT name="" id="791739ade4aee5df" memberName="arp_step_13" virtualName=""
                    explicitFocusOrder="0" pos="965 820 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(12)"/>
  <GENERICCOMPONENT name="" id="1e3ef8bba1be4b28" memberName="arp_step_12" virtualName=""
                    explicitFocusOrder="0" pos="955r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(11)"/>
  <GENERICCOMPONENT name="" id="fe823ea88a7a2471" memberName="arp_step_11" virtualName=""
                    explicitFocusOrder="0" pos="895r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(10)"/>
  <GENERICCOMPONENT name="" id="ee7d6057133dde55" memberName="arp_step_10" virtualName=""
                    explicitFocusOrder="0" pos="835r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(9)"/>
  <GENERICCOMPONENT name="" id="b4852f8bf0385747" memberName="arp_step_9" virtualName=""
                    explicitFocusOrder="0" pos="775r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(8)"/>
  <GENERICCOMPONENT name="" id="fd84ed45f47ab8b9" memberName="arp_step_8" virtualName=""
                    explicitFocusOrder="0" pos="705r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(7)"/>
  <GENERICCOMPONENT name="" id="cf5a0e63bd7f558a" memberName="arp_step_7" virtualName=""
                    explicitFocusOrder="0" pos="645r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(6)"/>
  <GENERICCOMPONENT name="" id="31712e752afeb9b5" memberName="arp_step_6" virtualName=""
                    explicitFocusOrder="0" pos="585r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(5)"/>
  <GENERICCOMPONENT name="" id="a729cce2b51e5737" memberName="arp_step_5" virtualName=""
                    explicitFocusOrder="0" pos="525r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(4)"/>
  <GENERICCOMPONENT name="" id="4ea4b03b58657c40" memberName="arp_step_4" virtualName=""
                    explicitFocusOrder="0" pos="455r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(3)"/>
  <GENERICCOMPONENT name="" id="b45b0bde6cb27e9d" memberName="arp_step_3" virtualName=""
                    explicitFocusOrder="0" pos="395r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="be72c3cee3e34864" memberName="arp_step_2" virtualName=""
                    explicitFocusOrder="0" pos="335r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="6665063ca7bdff41" memberName="arp_step_1" virtualName=""
                    explicitFocusOrder="0" pos="275r 950r 60 130" class="mono_ModulationSlider"
                    params="new ArpStepSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="bb9c66366ce375c" memberName="shuffle" virtualName=""
                    explicitFocusOrder="0" pos="30 820 60 130" class="mono_ModulationSlider"
                    params="new ShuffleConfig()"/>
  <GENERICCOMPONENT name="" id="6e5608d47c1be7c4" memberName="flt_sustain_4" virtualName=""
                    explicitFocusOrder="0" pos="395r 750r 60 130" class="mono_ModulationSlider"
                    params="new FSustainSlConfig(MAIN_ENV)"/>
  <GENERICCOMPONENT name="" id="8386fe429fe8a2e6" memberName="flt_decay_4" virtualName=""
                    explicitFocusOrder="0" pos="335r 750r 60 130" class="mono_ModulationSlider"
                    params="new FDecaySlConfig(MAIN_ENV)"/>
  <GENERICCOMPONENT name="" id="bb503e115ddb6edb" memberName="flt_attack_4" virtualName=""
                    explicitFocusOrder="0" pos="275r 750r 60 130" class="mono_ModulationSlider"
                    params="new FAttackSlConfig(MAIN_ENV)"/>
  <GENERICCOMPONENT name="" id="ca4537ccb809ca96" memberName="flt_release_3" virtualName=""
                    explicitFocusOrder="0" pos="725r 550r 60 130" class="mono_ModulationSlider"
                    params="new FReleaseSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="a60fcd747c533a26" memberName="flt_sustain_time_3"
                    virtualName="" explicitFocusOrder="0" pos="665r 550r 60 130"
                    class="mono_ModulationSlider" params="new FSustainTimeSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="78d4de9e0ffe3029" memberName="flt_sustain_3" virtualName=""
                    explicitFocusOrder="0" pos="605r 550r 60 130" class="mono_ModulationSlider"
                    params="new FSustainSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="e8b49d00205726e6" memberName="flt_decay_3" virtualName=""
                    explicitFocusOrder="0" pos="545r 550r 60 130" class="mono_ModulationSlider"
                    params="new FDecaySlConfig(2)"/>
  <GENERICCOMPONENT name="" id="638e13e96c94deb1" memberName="flt_attack_3" virtualName=""
                    explicitFocusOrder="0" pos="485r 550r 60 130" class="mono_ModulationSlider"
                    params="new FAttackSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="4e206df1142d5f1d" memberName="flt_release_2" virtualName=""
                    explicitFocusOrder="0" pos="725r 370r 60 130" class="mono_ModulationSlider"
                    params="new FReleaseSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="b2e468ddbdcb0be8" memberName="flt_sustain_time_2"
                    virtualName="" explicitFocusOrder="0" pos="665r 370r 60 130"
                    class="mono_ModulationSlider" params="new FSustainTimeSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="aa6aa381eebdd61" memberName="flt_sustain_2" virtualName=""
                    explicitFocusOrder="0" pos="605r 370r 60 130" class="mono_ModulationSlider"
                    params="new FSustainSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="140fc1e77383e0f9" memberName="flt_decay_2" virtualName=""
                    explicitFocusOrder="0" pos="545r 370r 60 130" class="mono_ModulationSlider"
                    params="new FDecaySlConfig(1)"/>
  <GENERICCOMPONENT name="" id="8a6f21a1f4a86dd" memberName="flt_attack_2" virtualName=""
                    explicitFocusOrder="0" pos="485r 370r 60 130" class="mono_ModulationSlider"
                    params="new FAttackSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="b17b21322ed6df73" memberName="flt_release_1" virtualName=""
                    explicitFocusOrder="0" pos="725r 190r 60 130" class="mono_ModulationSlider"
                    params="new FReleaseSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="1460533da718423d" memberName="flt_sustain_time_1"
                    virtualName="" explicitFocusOrder="0" pos="665r 190r 60 130"
                    class="mono_ModulationSlider" params="new FSustainTimeSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="16f83a5a025850d0" memberName="flt_sustain_1" virtualName=""
                    explicitFocusOrder="0" pos="605r 190r 60 130" class="mono_ModulationSlider"
                    params="new FSustainSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="dc67a284425c81d9" memberName="flt_decay_1" virtualName=""
                    explicitFocusOrder="0" pos="545r 190r 60 130" class="mono_ModulationSlider"
                    params="new FDecaySlConfig(0)"/>
  <GENERICCOMPONENT name="" id="1a64935d9407f5bb" memberName="flt_attack_1" virtualName=""
                    explicitFocusOrder="0" pos="425 190r 60 130" class="mono_ModulationSlider"
                    params="new FAttackSlConfig(0)"/>
  <LABEL name="" id="39e8fb50cf1d668d" memberName="label_monolisa" virtualName=""
         explicitFocusOrder="0" pos="1440r 1055r 205 85" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="M O N I Q U E" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="VOICE 1" id="77e66f63bda9d507" memberName="filter_type_1_1"
              virtualName="" explicitFocusOrder="0" pos="1005r 112 60 24" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="- 2PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="ab74fcbdb09aa48f" memberName="filter_type_2_1"
              virtualName="" explicitFocusOrder="0" pos="1005r 87 60 25" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="HP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="cf1d3ce65d7cdcdc" memberName="filter_type_3_1"
              virtualName="" explicitFocusOrder="0" pos="1005r 136 60 25" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c1383d06f6f9a2a4" memberName="filter_type_1_2"
              virtualName="" explicitFocusOrder="0" pos="1005r 292 60 24" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="- 2PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="121af38bc5254d57" memberName="filter_type_2_2"
              virtualName="" explicitFocusOrder="0" pos="1005r 267 60 25" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="HP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="43c53216b803243b" memberName="filter_type_3_2"
              virtualName="" explicitFocusOrder="0" pos="1005r 341r 60 25"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="8ff86dc73f1f9885" memberName="filter_type_1_3"
              virtualName="" explicitFocusOrder="0" pos="1005r 496r 60 24"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="- 2PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="e53892e80132f60c" memberName="filter_type_2_3"
              virtualName="" explicitFocusOrder="0" pos="1005r 447 60 25" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="HP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="f44bd17c008d0db3" memberName="filter_type_3_3"
              virtualName="" explicitFocusOrder="0" pos="1005r 496 60 25" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c1a5cea82178d7f1" memberName="filter_type_5_1"
              virtualName="" explicitFocusOrder="0" pos="1005r 161 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="375699baffcdc070" memberName="filter_type_5_2"
              virtualName="" explicitFocusOrder="0" pos="1005r 341 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="74deee6f861b7bf5" memberName="filter_type_5_3"
              virtualName="" explicitFocusOrder="0" pos="1005r 520 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7a60e9dcf8b32a0a" memberName="button_sequence_2"
              virtualName="" explicitFocusOrder="0" pos="335r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="17704b0ee658c01b" memberName="button_sequence_3"
              virtualName="" explicitFocusOrder="0" pos="395r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="70bcd1e56b41c2c6" memberName="button_sequence_4"
              virtualName="" explicitFocusOrder="0" pos="455r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e835074126c3a82d" memberName="button_sequence_5"
              virtualName="" explicitFocusOrder="0" pos="525r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7ebcf311504b804b" memberName="button_sequence_6"
              virtualName="" explicitFocusOrder="0" pos="585r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7964742ab1d9d236" memberName="button_sequence_7"
              virtualName="" explicitFocusOrder="0" pos="645r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="290ac159e50889a3" memberName="button_sequence_8"
              virtualName="" explicitFocusOrder="0" pos="705r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="444c07bec0f97ff4" memberName="button_sequence_9"
              virtualName="" explicitFocusOrder="0" pos="775r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d8ad414b935f59d8" memberName="button_sequence_10"
              virtualName="" explicitFocusOrder="0" pos="835r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="af5f2477751cef2c" memberName="button_sequence_11"
              virtualName="" explicitFocusOrder="0" pos="895r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d3687d6b28982234" memberName="button_sequence_12"
              virtualName="" explicitFocusOrder="0" pos="955r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="804115318ad213c1" memberName="button_sequence_13"
              virtualName="" explicitFocusOrder="0" pos="1025r 840r 60 20"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="13d758647984d5d5" memberName="button_sequence_14"
              virtualName="" explicitFocusOrder="0" pos="1085r 840r 60 20"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8cbd35271097248c" memberName="button_sequence_15"
              virtualName="" explicitFocusOrder="0" pos="1145r 840r 60 20"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2370645873984939" memberName="button_sequence_16"
              virtualName="" explicitFocusOrder="0" pos="1205r 840r 60 20"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="7c9b1844748d88e" memberName="combo_programm" virtualName=""
            explicitFocusOrder="0" pos="645r 1030r 310 30" editable="1" layout="33"
            items="" textWhenNonSelected="SELECT PROGRAM" textWhenNoItems="EMPTY BANK"/>
  <TEXTBUTTON name="" id="dd0cd965aaddf5ba" memberName="button_programm_left"
              virtualName="" explicitFocusOrder="0" pos="275r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&lt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6ccb0337451b3a26" memberName="button_programm_right"
              virtualName="" explicitFocusOrder="0" pos="705r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&gt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b91a29c51d2d93f1" memberName="button_programm_replace"
              virtualName="" explicitFocusOrder="0" pos="835r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="REPLACE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9481aeac211cafc0" memberName="button_programm_new"
              virtualName="" explicitFocusOrder="0" pos="775r 1030r 60 30"
              bgColOff="ff000000" textCol="ffbcff00" textColOn="ffd0ff00" buttonText="NEW"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e5ff1639c5671984" memberName="button_open_oszi" virtualName=""
              explicitFocusOrder="0" pos="965 1000 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="OSCI" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="57c6c986fc98dac" memberName="button_open_midi_io_settings"
              virtualName="" explicitFocusOrder="0" pos="1205r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="MIDI IO"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_bank" virtualName=""
            explicitFocusOrder="0" pos="335r 1030r 60 30" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="" id="aa7c44443637097c" memberName="button_programm_load"
              virtualName="" explicitFocusOrder="0" pos="955r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LOAD"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="e8e2f9e6488018da" memberName="osc_1" virtualName=""
                    explicitFocusOrder="0" pos="160r 190r 60 130" class="mono_ModulationSlider"
                    params="new OSCSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="308060a72bcb3066" memberName="osc_2" virtualName=""
                    explicitFocusOrder="0" pos="160r 370r 60 130" class="mono_ModulationSlider"
                    params="new OSCSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="17d8341f811bcb5a" memberName="osc_3" virtualName=""
                    explicitFocusOrder="0" pos="160r 550r 60 130" class="mono_ModulationSlider"
                    params="new OSCSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="2a31f2713e80bed3" memberName="lfo_1" virtualName=""
                    explicitFocusOrder="0" pos="905r 190r 60 130" class="mono_ModulationSlider"
                    params="new LFOSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="ffb8076636239778" memberName="flt_cutoff_1" virtualName=""
                    explicitFocusOrder="0" pos="1075r 190r 60 130" class="mono_ModulationSlider"
                    params="new FCutoffSLConfig(0)"/>
  <GENERICCOMPONENT name="" id="e36ec1f3ea5f1edf" memberName="lfo_2" virtualName=""
                    explicitFocusOrder="0" pos="905r 370r 60 130" class="mono_ModulationSlider"
                    params="new LFOSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="10b142e0e3bd1edf" memberName="lfo_3" virtualName=""
                    explicitFocusOrder="0" pos="905r 550r 60 130" class="mono_ModulationSlider"
                    params="new LFOSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="9eb8d35cf54eee3" memberName="flt_cutoff_2" virtualName=""
                    explicitFocusOrder="0" pos="1075r 370r 60 130" class="mono_ModulationSlider"
                    params="new FCutoffSLConfig(1)"/>
  <GENERICCOMPONENT name="" id="d7143931caaf1976" memberName="flt_cutoff_3" virtualName=""
                    explicitFocusOrder="0" pos="1075r 550r 60 130" class="mono_ModulationSlider"
                    params="new FCutoffSLConfig(2)"/>
  <GENERICCOMPONENT name="" id="ecbcc81adebe9850" memberName="flt_input_1" virtualName=""
                    explicitFocusOrder="0" pos="215 60 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(0,0)"/>
  <GENERICCOMPONENT name="" id="6af45f57190e5260" memberName="flt_input_2" virtualName=""
                    explicitFocusOrder="0" pos="335r 190r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(0,1)"/>
  <GENERICCOMPONENT name="" id="9abcdbe824977dbc" memberName="flt_input_3" virtualName=""
                    explicitFocusOrder="0" pos="395r 190r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(0,2)"/>
  <GENERICCOMPONENT name="" id="2bcfc71597ef5fbd" memberName="flt_compressor_1" virtualName=""
                    explicitFocusOrder="0" pos="1345r 190r 60 130" class="mono_ModulationSlider"
                    params="new FCompressorSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="16470f25818b13ce" memberName="flt_distortion_1" virtualName=""
                    explicitFocusOrder="0" pos="1285r 190r 60 130" class="mono_ModulationSlider"
                    params="new GForceSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="cc59ad897708e932" memberName="flt_input_6" virtualName=""
                    explicitFocusOrder="0" pos="275r 370r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(1,0)"/>
  <GENERICCOMPONENT name="" id="30402f9a5bf56bfb" memberName="flt_input_7" virtualName=""
                    explicitFocusOrder="0" pos="335r 370r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(1,1)"/>
  <GENERICCOMPONENT name="" id="e54fd10f87874627" memberName="flt_input_8" virtualName=""
                    explicitFocusOrder="0" pos="395r 370r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(1,2)"/>
  <GENERICCOMPONENT name="" id="f8269875dd70ecfa" memberName="flt_compressor_2" virtualName=""
                    explicitFocusOrder="0" pos="1345r 370r 60 130" class="mono_ModulationSlider"
                    params="new FCompressorSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="f3d6d4daa7867cda" memberName="flt_input_11" virtualName=""
                    explicitFocusOrder="0" pos="275r 550r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(2,0)"/>
  <GENERICCOMPONENT name="" id="7371ee7afd1877b4" memberName="flt_input_12" virtualName=""
                    explicitFocusOrder="0" pos="335r 550r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(2,1)"/>
  <GENERICCOMPONENT name="" id="d2e2be5869047a2e" memberName="flt_input_13" virtualName=""
                    explicitFocusOrder="0" pos="395r 550r 60 130" class="mono_ModulationSlider"
                    params="new InputSlConfig(2,2)"/>
  <GENERICCOMPONENT name="" id="3dab756c3e18a7de" memberName="flt_compressor_3" virtualName=""
                    explicitFocusOrder="0" pos="1345r 550r 60 130" class="mono_ModulationSlider"
                    params="new FCompressorSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="31da059865f2567b" memberName="flt_resonance_1" virtualName=""
                    explicitFocusOrder="0" pos="1135r 190r 60 130" class="mono_ModulationSlider"
                    params="new FResonanceSLConfig(0)"/>
  <GENERICCOMPONENT name="" id="8e9c871f56bec21b" memberName="flt_gain_1" virtualName=""
                    explicitFocusOrder="0" pos="1195r 190r 60 130" class="mono_ModulationSlider"
                    params="new FGainSLConfig(0)"/>
  <GENERICCOMPONENT name="" id="75550ba5bb7ce4e0" memberName="flt_resonance_2" virtualName=""
                    explicitFocusOrder="0" pos="1135r 370r 60 130" class="mono_ModulationSlider"
                    params="new FResonanceSLConfig(1)"/>
  <GENERICCOMPONENT name="" id="577a04755f6e3eca" memberName="flt_gain_2" virtualName=""
                    explicitFocusOrder="0" pos="1195r 370r 60 130" class="mono_ModulationSlider"
                    params="new FGainSLConfig(1)"/>
  <GENERICCOMPONENT name="" id="aa2b2c2864221426" memberName="flt_resonance_3" virtualName=""
                    explicitFocusOrder="0" pos="1135r 550r 60 130" class="mono_ModulationSlider"
                    params="new FResonanceSLConfig(2)"/>
  <GENERICCOMPONENT name="" id="5d7a48dcb59f2814" memberName="flt_gain_3" virtualName=""
                    explicitFocusOrder="0" pos="1195r 550r 60 130" class="mono_ModulationSlider"
                    params="new FGainSLConfig(2)"/>
  <GENERICCOMPONENT name="" id="ba71384f051dd23" memberName="flt_volume_1" virtualName=""
                    explicitFocusOrder="0" pos="1435r 190r 60 130" class="mono_ModulationSlider"
                    params="new FVolumeSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="32dd3f586d1d81eb" memberName="flt_volume_2" virtualName=""
                    explicitFocusOrder="0" pos="1435r 370r 60 130" class="mono_ModulationSlider"
                    params="new FVolumeSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="18f72cc654c99917" memberName="flt_volume_3" virtualName=""
                    explicitFocusOrder="0" pos="1435r 550r 60 130" class="mono_ModulationSlider"
                    params="new FVolumeSlConfig(2)"/>
  <GENERICCOMPONENT name="" id="68cebc996c492894" memberName="adsr_lfo_mix" virtualName=""
                    explicitFocusOrder="0" pos="815r 190r 60 130" class="mono_ModulationSlider"
                    params="new EnvLfoSlConfig(0)"/>
  <GENERICCOMPONENT name="" id="944e7d4439e86773" memberName="lfo_opt_2" virtualName=""
                    explicitFocusOrder="0" pos="815r 370r 60 130" class="mono_ModulationSlider"
                    params="new EnvLfoSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="2d0d9d7f81f143" memberName="lfo_opt_3" virtualName=""
                    explicitFocusOrder="0" pos="815r 550r 60 130" class="mono_ModulationSlider"
                    params="new EnvLfoSlConfig(2)"/>
  <TEXTBUTTON name="" id="9669ee100bf4ee95" memberName="button_sequence_1"
              virtualName="" explicitFocusOrder="0" pos="275r 840r 60 20" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="3eaa1962698c14dc" memberName="flt_release_4" virtualName=""
                    explicitFocusOrder="0" pos="455r 750r 60 130" class="mono_ModulationSlider"
                    params="new FReleaseSlConfig(MAIN_ENV)"/>
  <GENERICCOMPONENT name="" id="49d3d717347ff877" memberName="delay2" virtualName=""
                    explicitFocusOrder="0" pos="885r 750r 60 130" class="mono_ModulationSlider"
                    params="new DelaySlConfig()"/>
  <GENERICCOMPONENT name="" id="94c6b03ecc4d4642" memberName="volume" virtualName=""
                    explicitFocusOrder="0" pos="1435r 750r 60 130" class="mono_ModulationSlider"
                    params="new VolumeConfig()"/>
  <GENERICCOMPONENT name="" id="9771b840efca92c2" memberName="flt_distortion_2" virtualName=""
                    explicitFocusOrder="0" pos="1285r 370r 60 130" class="mono_ModulationSlider"
                    params="new GForceSlConfig(1)"/>
  <GENERICCOMPONENT name="" id="766d923ef01630c7" memberName="flt_distortion_3" virtualName=""
                    explicitFocusOrder="0" pos="1285r 550r 60 130" class="mono_ModulationSlider"
                    params="new GForceSlConfig(2)"/>
  <TEXTBUTTON name="" id="28379674f941d830" memberName="button_arp_speed_XNORM"
              virtualName="" explicitFocusOrder="0" pos="1295 820 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="x1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="9f8319dda0065826" memberName="flt_attack_5" virtualName=""
                    explicitFocusOrder="0" pos="90r 755r 60 130" class="mono_ModulationSlider"
                    params="new FMFreqSlConfig()"/>
  <GENERICCOMPONENT name="" id="53fd0bab31e1ce" memberName="flt_attack_6" virtualName=""
                    explicitFocusOrder="0" pos="160r 755r 60 130" class="mono_ModulationSlider"
                    params="new FMAmountSlConfig()"/>
  <GENERICCOMPONENT name="" id="7abd69d58b16456c" memberName="osc_wave_1" virtualName=""
                    explicitFocusOrder="0" pos="30 60 60 130" class="mono_ModulationSlider"
                    params="new WAVESlConfig(0)"/>
  <GENERICCOMPONENT name="" id="289652ee3553683c" memberName="osc_wave_2" virtualName=""
                    explicitFocusOrder="0" pos="90r 370r 60 130" class="mono_ModulationSlider"
                    params="new WAVESlConfig(1)"/>
  <SLIDER name="new slider" id="20de89a2be986cc1" memberName="sl_morhp_mix"
          virtualName="" explicitFocusOrder="0" pos="1165 715 180 33" min="0"
          max="3000" int="0.010000000000000000208" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="87db63f1017ff04b" memberName="button_programm_delete"
              virtualName="" explicitFocusOrder="0" pos="895r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff0000" textColOn="ffff7900" buttonText="DELETE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="120408211886469e" memberName="button_open_config"
              virtualName="" explicitFocusOrder="0" pos="1085r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="ADR"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="bcfd327216c64e93" memberName="filter_type_6_1"
              virtualName="" explicitFocusOrder="0" pos="1005r 60 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="LP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="78dd95fdf1cece7e" memberName="filter_type_6_2"
              virtualName="" explicitFocusOrder="0" pos="1005r 240 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="LP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c642f9acf4b813ef" memberName="filter_type_6_3"
              virtualName="" explicitFocusOrder="0" pos="1005r 420 60 27" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="LP" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="53f01906b113ce41" memberName="button_midi_learn"
              virtualName="" explicitFocusOrder="0" pos="1205r 1000r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LEARN"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4d29473e06fd562f" memberName="button_ctrl_toggle"
              virtualName="" explicitFocusOrder="0" pos="100 1000 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="SHIFT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="1f9f546ceacaa4b2" memberName="colour" virtualName=""
                    explicitFocusOrder="0" pos="585r 750r 60 130" class="mono_ModulationSlider"
                    params="new FColourSlConfig()"/>
  <GENERICCOMPONENT name="" id="ca562cfd2b6999c4" memberName="speed" virtualName=""
                    explicitFocusOrder="0" pos="1285r 950r 60 130" class="mono_ModulationSlider"
                    params="new BPMSlConfig()"/>
  <TEXTBUTTON name="" id="8f0b48518cbff149" memberName="button_open_morph"
              virtualName="" explicitFocusOrder="0" pos="1165r 715 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f57674183a67085" memberName="effect_finalizer_switch"
              virtualName="" explicitFocusOrder="0" pos="520r 620 25 130" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="F X" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="af640d06672c6a96" memberName="label_ui_headline2"
         virtualName="" explicitFocusOrder="0" pos="1105 616 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="6cf55c2697d84492" memberName="label_ui_headline3"
         virtualName="" explicitFocusOrder="0" pos="1165 616 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="31349ce13448848e" memberName="label_ui_headline5"
         virtualName="" explicitFocusOrder="0" pos="1225 616 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="1e86fc07c2fe9e40" memberName="label_ui_headline6"
         virtualName="" explicitFocusOrder="0" pos="1285 616 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="8b8fa534e67fede0" memberName="button_values_toggle"
              virtualName="" explicitFocusOrder="0" pos="30 1000 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="VAL" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="1e7a797188cff129" memberName="reverb_width" virtualName=""
                    explicitFocusOrder="0" pos="735r 750r 60 130" class="mono_ModulationSlider"
                    params="new RWidthSlConfig()"/>
  <GENERICCOMPONENT name="" id="6c9f41765f0f3e8a" memberName="octave_offset" virtualName=""
                    explicitFocusOrder="0" pos="1435r 950r 60 130" class="mono_ModulationSlider"
                    params="new OctaveOffsetSlConfig()"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline4"
         virtualName="" explicitFocusOrder="0" pos="245 5 120 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER INPUTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="706628ef52338a3" memberName="label_ui_headline7"
         virtualName="" explicitFocusOrder="0" pos="40 5 110 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSCILLATORS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="40822f39512f59ee" memberName="label_ui_headline8"
         virtualName="" explicitFocusOrder="0" pos="510 5 130 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER ENVELOPE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="4a296c8c1b36d5b5" memberName="label_ui_headline9"
         virtualName="" explicitFocusOrder="0" pos="850 5 50 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="8dfe8598a2227d6" memberName="label_ui_headline10"
         virtualName="" explicitFocusOrder="0" pos="975 5 190 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER CONFIGURATION"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="624597a6e0fd4f43" memberName="label_ui_headline11"
         virtualName="" explicitFocusOrder="0" pos="1230 5 110 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="d111f7d6b78091bd" memberName="label_ui_headline12"
         virtualName="" explicitFocusOrder="0" pos="1375 5 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OUTPUT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="cc8c514c76739c41" memberName="label_ui_headline13"
         virtualName="" explicitFocusOrder="0" pos="270 580 130 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MAIN ENVELOPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="c10833d7ab234ea1" memberName="label_shape" virtualName=""
         explicitFocusOrder="0" pos="525 580 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SHAPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="bfabeb8da43f3421" memberName="label_reverb" virtualName=""
         explicitFocusOrder="0" pos="665 580 80 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="REVERB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="7a6e3d75d5abc4de" memberName="label_delay" virtualName=""
         explicitFocusOrder="0" pos="825 580 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DELAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="a4b6cc975d476069" memberName="label_chorus" virtualName=""
         explicitFocusOrder="0" pos="910 580 70 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CHORUS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="fcb9d931dfbdfa5d" memberName="label_fx_mix" virtualName=""
         explicitFocusOrder="0" pos="1005 580 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX MIX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="660b9ead77642f4f" memberName="label_ui_headline19"
         virtualName="" explicitFocusOrder="0" pos="1135 580 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH SECTION" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="c72f149b5e4ef3a1" memberName="label_ui_headline20"
         virtualName="" explicitFocusOrder="0" pos="615 780 190 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARPEGGIATOR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="365d9d273db6db3a" memberName="label_ui_headline21"
         virtualName="" explicitFocusOrder="0" pos="100 780 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLIDE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="9b0c123898785ff8" memberName="label_ui_headline22"
         virtualName="" explicitFocusOrder="0" pos="1260 780 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="3dc587f677ecc27f" memberName="label_ui_headline23"
         virtualName="" explicitFocusOrder="0" pos="1375 580 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MASTER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="de3bbc3ac23fc36a" memberName="label_ui_headline24"
         virtualName="" explicitFocusOrder="0" pos="1375 780 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCTAVE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ecf3af4d030b7b19" memberName="label_ui_headline25"
         virtualName="" explicitFocusOrder="0" pos="35 563 120 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c54e3e2b543626c5" memberName="volume_master_meter"
                    virtualName="SegmentedMeter" explicitFocusOrder="0" pos="1375 620 60 27"
                    class="Component" params=""/>
  <LABEL name="DL" id="ad887653d405d154" memberName="label_eq" virtualName=""
         explicitFocusOrder="0" pos="740 580 90 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="EQUALIZER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="30ecdded1d4d2622" memberName="button_open_config2"
              virtualName="" explicitFocusOrder="0" pos="1145r 1030r 60 30"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="CFG"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
