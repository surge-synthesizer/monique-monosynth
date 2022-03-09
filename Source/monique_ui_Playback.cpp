/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "monique_ui_LookAndFeel.h"
#include "monique_core_Datastructures.h"
//[/Headers]

#include "monique_ui_Playback.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_Playback::refresh() noexcept
{
    // KEYTRACK
    const bool is_key_0_down = synth_data->is_key_down(0);
    const bool is_key_1_down = synth_data->is_key_down(1);
    const bool is_key_2_down = synth_data->is_key_down(2);
#ifdef POLY
    // MODES
    TURN_BUTTON_ON_OR_OFF(button_tracking_mode_lf, synth_data->keytrack_osci_play_mode == 0);
    TURN_BUTTON_ON_OR_OFF(button_tracking_mode_hf, synth_data->keytrack_osci_play_mode == 1);
    TURN_BUTTON_ON_OR_OFF(button_tracking_mode_keep, synth_data->keytrack_osci_play_mode == 2);
    TURN_BUTTON_ON_OR_OFF(button_tracking_mode_hm, synth_data->keytrack_osci_play_mode == 3);

    // OSC TRACKING
    TURN_BUTTON_ON_OR_OFF(button_osc_tracking_1, synth_data->keytrack_osci[0]);
    if (button_osc_tracking_1->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[0] ? 0.7 + is_key_0_down * 0.3 : 0))
    {
        button_osc_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_osc_tracking_2, synth_data->keytrack_osci[1]);
    if (button_osc_tracking_2->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[1] ? 0.7 + is_key_1_down * 0.3 : 0))
    {
        button_osc_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_osc_tracking_3, synth_data->keytrack_osci[2]);
    if (button_osc_tracking_3->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[2] ? 0.7 + is_key_2_down * 0.3 : 0))
    {
        button_osc_tracking_3->repaint();
    }
    slider_osc_tracking_oct_2->setValue(synth_data->keytrack_osci_octave_offset[1].get_value(),
                                        dontSendNotification);
    slider_osc_tracking_oct_3->setValue(synth_data->keytrack_osci_octave_offset[2].get_value(),
                                        dontSendNotification);

    TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_1, synth_data->keytrack_cutoff[0]);
    if (button_cutoff_tracking_1->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[0] ? 0.7 + is_key_0_down * 0.3 : 0))
    {
        button_cutoff_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_2, synth_data->keytrack_cutoff[1]);
    if (button_cutoff_tracking_2->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[1] ? 0.7 + is_key_1_down * 0.3 : 0))
    {
        button_cutoff_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_3, synth_data->keytrack_cutoff[2]);
    if (button_cutoff_tracking_3->getProperties().set(
            VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[2] ? 0.7 + is_key_2_down * 0.3 : 0))
    {
        button_cutoff_tracking_3->repaint();
    }
    slider_cutoff_tracking_oct_1->setValue(synth_data->keytrack_cutoff_octave_offset[0].get_value(),
                                           dontSendNotification);
    slider_cutoff_tracking_oct_2->setValue(synth_data->keytrack_cutoff_octave_offset[1].get_value(),
                                           dontSendNotification);
    slider_cutoff_tracking_oct_3->setValue(synth_data->keytrack_cutoff_octave_offset[2].get_value(),
                                           dontSendNotification);

    // INPUT TRIG
    TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_1, synth_data->keytrack_filter_inputs[0]);
    if (button_flt_input_triggering_1->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_inputs[0] ? 0.7 + is_key_0_down * 0.3 : 0))
    {
        button_flt_input_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_2, synth_data->keytrack_filter_inputs[1]);
    if (button_flt_input_triggering_2->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_inputs[1] ? 0.7 + is_key_1_down * 0.3 : 0))
    {
        button_flt_input_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_3, synth_data->keytrack_filter_inputs[2]);
    if (button_flt_input_triggering_3->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_inputs[2] ? 0.7 + is_key_2_down * 0.3 : 0))
    {
        button_flt_input_triggering_3->repaint();
    }

    // ENV TRIG
    TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_1, synth_data->keytrack_filter_env[0]);
    if (button_flt_env_triggering_1->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_env[0] ? 0.7 + is_key_0_down * 0.3 : 0))
    {
        button_flt_env_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_2, synth_data->keytrack_filter_env[1]);
    if (button_flt_env_triggering_2->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_env[1] ? 0.7 + is_key_1_down * 0.3 : 0))
    {
        button_flt_env_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_3, synth_data->keytrack_filter_env[2]);
    if (button_flt_env_triggering_3->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_env[2] ? 0.7 + is_key_2_down * 0.3 : 0))
    {
        button_flt_env_triggering_3->repaint();
    }

    // OUTPUT TRIG
    TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_1, synth_data->keytrack_filter_volume[0]);
    if (button_flt_out_triggering_1->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_volume[0] ? 0.7 + is_key_0_down * 0.3 : 0))
    {
        button_flt_out_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_2, synth_data->keytrack_filter_volume[1]);
    if (button_flt_out_triggering_2->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_volume[1] ? 0.7 + is_key_1_down * 0.3 : 0))
    {
        button_flt_out_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_3, synth_data->keytrack_filter_volume[2]);
    if (button_flt_out_triggering_3->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->keytrack_filter_volume[2] ? 0.7 + is_key_2_down * 0.3 : 0))
    {
        button_flt_out_triggering_3->repaint();
    }
    slider_flt_out_sesitivity_1->setValue(synth_data->keytrack_filter_volume_offset[0].get_value(),
                                          dontSendNotification);
    slider_flt_out_sesitivity_2->setValue(synth_data->keytrack_filter_volume_offset[1].get_value(),
                                          dontSendNotification);
    slider_flt_out_sesitivity_3->setValue(synth_data->keytrack_filter_volume_offset[2].get_value(),
                                          dontSendNotification);

#endif

    /*
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_1, synth_data->keytrack_filter_env_1 );
    if( button_env_key_trigger_1->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_env_1 ? 0.7 + is_key_0_down*0.3 : 0 ) )
    {
        button_env_key_trigger_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_2, synth_data->keytrack_filter_env_2 );
    if( button_env_key_trigger_2->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_env_2 ? 0.7 + is_key_1_down*0.3 : 0 ) )
    {
        button_env_key_trigger_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_3, synth_data->keytrack_filter_env_3 );
    if( button_env_key_trigger_3->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_env_3 ? 0.7 + is_key_2_down*0.3 : 0 ) )
    {
        button_env_key_trigger_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_1, synth_data->keytrack_filter_volume_1 );
    if( button_flt_key_tracking_volume_1->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_volume_1 ? 0.7 + synth_data->get_tracking_env_state(0)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_2, synth_data->keytrack_filter_volume_2 );
    if( button_flt_key_tracking_volume_2->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_volume_2 ? 0.7 + synth_data->get_tracking_env_state(1)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_3, synth_data->keytrack_filter_volume_3 );
    if( button_flt_key_tracking_volume_3->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_volume_3 ? 0.7 + synth_data->get_tracking_env_state(2)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_1, synth_data->keytrack_filter_cutoff_1 );
    if( button_cutoff_key_tracking_1->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_cutoff_1 ? 0.7 + is_key_0_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_2, synth_data->keytrack_filter_cutoff_2 );
    if( button_cutoff_key_tracking_2->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_cutoff_2 ? 0.7 + is_key_1_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_3, synth_data->keytrack_filter_cutoff_3 );
    if( button_cutoff_key_tracking_3->getProperties().set( VAR_INDEX_BUTTON_AMP,
    synth_data->keytrack_filter_cutoff_3 ? 0.7 + is_key_2_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_osc_key_tracking2, synth_data->keytrack_filter_inputs );
    */

    // toggle_input_main_thru->setToggleState( _audio_device_manager->main_input_thru,
    // dontSendNotification ); TURN_BUTTON_ON_OR_OFF( button_midi_learn,
    // midi_control_handler->is_waiting_for_param() ? true : midi_control_handler->is_learning() ?
    // true : false );
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_Playback::Monique_Ui_Playback(Monique_Ui_Refresher *ui_refresher_,
                                         Monique_Ui_Mainwindow *parent_)
    : Monique_Ui_Refreshable(ui_refresher_), original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    parent = parent_;
    //[/Constructor_pre]

    button_preset_agro = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_agro);
    button_preset_agro->setButtonText(TRANS("AGRESSIVE"));
    button_preset_agro->addListener(this);
    button_preset_agro->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_agro->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_agro->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_agro->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_preset_down = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_down);
    button_preset_down->setButtonText(TRANS("DOWN"));
    button_preset_down->addListener(this);
    button_preset_down->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_down->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_down->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_down->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_preset_rising = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_rising);
    button_preset_rising->setButtonText(TRANS("RISING"));
    button_preset_rising->addListener(this);
    button_preset_rising->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_rising->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_rising->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_rising->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_preset_soft = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_soft);
    button_preset_soft->setButtonText(TRANS("SOFT"));
    button_preset_soft->addListener(this);
    button_preset_soft->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_soft->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_soft->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_soft->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_flt_out_triggering_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_3);
    button_flt_out_triggering_3->setButtonText(TRANS("3"));
    button_flt_out_triggering_3->addListener(this);
    button_flt_out_triggering_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_3->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xffff1111));
    button_flt_out_triggering_3->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_3->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_out_triggering_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_2);
    button_flt_out_triggering_2->setButtonText(TRANS("2"));
    button_flt_out_triggering_2->addListener(this);
    button_flt_out_triggering_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_2->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xffff1111));
    button_flt_out_triggering_2->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_2->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_out_triggering_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_1);
    button_flt_out_triggering_1->setButtonText(TRANS("FLT 1"));
    button_flt_out_triggering_1->addListener(this);
    button_flt_out_triggering_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_1->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_1->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_env_triggering_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_3);
    button_flt_env_triggering_3->setButtonText(TRANS("3"));
    button_flt_env_triggering_3->addListener(this);
    button_flt_env_triggering_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_3->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_3->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_3->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_env_triggering_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_2);
    button_flt_env_triggering_2->setButtonText(TRANS("2"));
    button_flt_env_triggering_2->addListener(this);
    button_flt_env_triggering_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_2->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_2->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_2->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_env_triggering_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_1);
    button_flt_env_triggering_1->setButtonText(TRANS("FLT 1"));
    button_flt_env_triggering_1->addListener(this);
    button_flt_env_triggering_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_1->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_1->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_1->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_input_triggering_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_3);
    button_flt_input_triggering_3->setButtonText(TRANS("3"));
    button_flt_input_triggering_3->addListener(this);
    button_flt_input_triggering_3->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_flt_input_triggering_3->setColour(juce::TextButton::buttonOnColourId,
                                             juce::Colour(0xff4444ff));
    button_flt_input_triggering_3->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_flt_input_triggering_3->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    button_flt_input_triggering_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_2);
    button_flt_input_triggering_2->setButtonText(TRANS("2"));
    button_flt_input_triggering_2->addListener(this);
    button_flt_input_triggering_2->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_flt_input_triggering_2->setColour(juce::TextButton::buttonOnColourId,
                                             juce::Colour(0xff4444ff));
    button_flt_input_triggering_2->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_flt_input_triggering_2->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    button_flt_input_triggering_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_1);
    button_flt_input_triggering_1->setButtonText(TRANS("FLT 1"));
    button_flt_input_triggering_1->addListener(this);
    button_flt_input_triggering_1->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_flt_input_triggering_1->setColour(juce::TextButton::buttonOnColourId,
                                             juce::Colour(0xff4444ff));
    button_flt_input_triggering_1->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_flt_input_triggering_1->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    filter_type_bg_button_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_1);
    filter_type_bg_button_1->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_tracking_mode_hm = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_tracking_mode_hm);
    button_tracking_mode_hm->setButtonText(TRANS("---"));
    button_tracking_mode_hm->setConnectedEdges(juce::Button::ConnectedOnTop);
    button_tracking_mode_hm->addListener(this);
    button_tracking_mode_hm->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_tracking_mode_hm->setColour(juce::TextButton::buttonOnColourId,
                                       juce::Colour(0xffff1111));
    button_tracking_mode_hm->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_tracking_mode_hm->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_tracking_mode_keep = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_tracking_mode_keep);
    button_tracking_mode_keep->setButtonText(TRANS("AS YOU PLAY"));
    button_tracking_mode_keep->setConnectedEdges(juce::Button::ConnectedOnBottom);
    button_tracking_mode_keep->addListener(this);
    button_tracking_mode_keep->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_tracking_mode_keep->setColour(juce::TextButton::buttonOnColourId,
                                         juce::Colour(0xffff1111));
    button_tracking_mode_keep->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_tracking_mode_keep->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_tracking_mode_lf = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_tracking_mode_lf);
    button_tracking_mode_lf->setButtonText(TRANS("HIGH to LOW"));
    button_tracking_mode_lf->setConnectedEdges(juce::Button::ConnectedOnTop |
                                               juce::Button::ConnectedOnBottom);
    button_tracking_mode_lf->addListener(this);
    button_tracking_mode_lf->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_tracking_mode_lf->setColour(juce::TextButton::buttonOnColourId,
                                       juce::Colour(0xffff1111));
    button_tracking_mode_lf->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_tracking_mode_lf->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_tracking_mode_hf = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_tracking_mode_hf);
    button_tracking_mode_hf->setButtonText(TRANS("LOW to HIGH"));
    button_tracking_mode_hf->setConnectedEdges(juce::Button::ConnectedOnTop |
                                               juce::Button::ConnectedOnBottom);
    button_tracking_mode_hf->addListener(this);
    button_tracking_mode_hf->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_tracking_mode_hf->setColour(juce::TextButton::buttonOnColourId,
                                       juce::Colour(0xffff1111));
    button_tracking_mode_hf->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_tracking_mode_hf->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    slider_cutoff_tracking_oct_3 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_cutoff_tracking_oct_3);
    slider_cutoff_tracking_oct_3->setRange(-4, 4, 1);
    slider_cutoff_tracking_oct_3->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_3->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_3->setColour(juce::Slider::rotarySliderFillColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_3->setColour(juce::Slider::rotarySliderOutlineColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_3->setColour(juce::Slider::textBoxTextColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_3->setColour(juce::Slider::textBoxBackgroundColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_3->addListener(this);

    slider_cutoff_tracking_oct_2 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_cutoff_tracking_oct_2);
    slider_cutoff_tracking_oct_2->setRange(-4, 4, 1);
    slider_cutoff_tracking_oct_2->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_2->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_2->setColour(juce::Slider::rotarySliderFillColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_2->setColour(juce::Slider::rotarySliderOutlineColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_2->setColour(juce::Slider::textBoxTextColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_2->setColour(juce::Slider::textBoxBackgroundColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_2->addListener(this);

    slider_cutoff_tracking_oct_1 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_cutoff_tracking_oct_1);
    slider_cutoff_tracking_oct_1->setRange(-4, 4, 1);
    slider_cutoff_tracking_oct_1->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_1->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_1->setColour(juce::Slider::rotarySliderFillColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_1->setColour(juce::Slider::rotarySliderOutlineColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_1->setColour(juce::Slider::textBoxTextColourId,
                                            juce::Colours::yellow);
    slider_cutoff_tracking_oct_1->setColour(juce::Slider::textBoxBackgroundColourId,
                                            juce::Colour(0xff161616));
    slider_cutoff_tracking_oct_1->addListener(this);

    slider_osc_tracking_oct_3 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_osc_tracking_oct_3);
    slider_osc_tracking_oct_3->setRange(-2, 2, 1);
    slider_osc_tracking_oct_3->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_osc_tracking_oct_3->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_osc_tracking_oct_3->setColour(juce::Slider::rotarySliderFillColourId,
                                         juce::Colours::yellow);
    slider_osc_tracking_oct_3->setColour(juce::Slider::rotarySliderOutlineColourId,
                                         juce::Colour(0xff161616));
    slider_osc_tracking_oct_3->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_osc_tracking_oct_3->setColour(juce::Slider::textBoxBackgroundColourId,
                                         juce::Colour(0xff161616));
    slider_osc_tracking_oct_3->addListener(this);

    slider_osc_tracking_oct_2 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_osc_tracking_oct_2);
    slider_osc_tracking_oct_2->setRange(-2, 2, 1);
    slider_osc_tracking_oct_2->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_osc_tracking_oct_2->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_osc_tracking_oct_2->setColour(juce::Slider::rotarySliderFillColourId,
                                         juce::Colours::yellow);
    slider_osc_tracking_oct_2->setColour(juce::Slider::rotarySliderOutlineColourId,
                                         juce::Colour(0xff161616));
    slider_osc_tracking_oct_2->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_osc_tracking_oct_2->setColour(juce::Slider::textBoxBackgroundColourId,
                                         juce::Colour(0xff161616));
    slider_osc_tracking_oct_2->addListener(this);

    button_cutoff_tracking_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_3);
    button_cutoff_tracking_3->setButtonText(TRANS("3"));
    button_cutoff_tracking_3->addListener(this);
    button_cutoff_tracking_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_3->setColour(juce::TextButton::buttonOnColourId,
                                        juce::Colour(0xffff1111));
    button_cutoff_tracking_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_cutoff_tracking_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_2);
    button_cutoff_tracking_2->setButtonText(TRANS("2"));
    button_cutoff_tracking_2->addListener(this);
    button_cutoff_tracking_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_2->setColour(juce::TextButton::buttonOnColourId,
                                        juce::Colour(0xffff1111));
    button_cutoff_tracking_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_cutoff_tracking_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_1);
    button_cutoff_tracking_1->setButtonText(TRANS("FLT 1"));
    button_cutoff_tracking_1->addListener(this);
    button_cutoff_tracking_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_osc_tracking_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_3);
    button_osc_tracking_3->setButtonText(TRANS("3"));
    button_osc_tracking_3->addListener(this);
    button_osc_tracking_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_3->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_osc_tracking_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_2);
    button_osc_tracking_2->setButtonText(TRANS("2"));
    button_osc_tracking_2->addListener(this);
    button_osc_tracking_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_2->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_osc_tracking_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_1);
    button_osc_tracking_1->setButtonText(TRANS("OSC 1"));
    button_osc_tracking_1->addListener(this);
    button_osc_tracking_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_1->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    label_2 = std::make_unique<juce::Label>(juce::String(), TRANS("OSC KEY-TRACKING"));
    addAndMakeVisible(*label_2);
    label_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_2->setJustificationType(juce::Justification::centredLeft);
    label_2->setEditable(false, false, false);
    label_2->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_4 = std::make_unique<juce::Label>(juce::String(), TRANS("CUTOFF KEY-TRACKING"));
    addAndMakeVisible(*label_4);
    label_4->setFont(juce::Font(30.00f, juce::Font::plain));
    label_4->setJustificationType(juce::Justification::centredLeft);
    label_4->setEditable(false, false, false);
    label_4->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_4->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_5 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_5);
    label_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_5->setJustificationType(juce::Justification::centredLeft);
    label_5->setEditable(false, false, false);
    label_5->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_6 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_6);
    label_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_6->setJustificationType(juce::Justification::centredLeft);
    label_6->setEditable(false, false, false);
    label_6->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_7 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_7);
    label_7->setFont(juce::Font(30.00f, juce::Font::plain));
    label_7->setJustificationType(juce::Justification::centredLeft);
    label_7->setEditable(false, false, false);
    label_7->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_8 = std::make_unique<juce::Label>(juce::String(), TRANS("FLT INPUT KEY-TRIG"));
    addAndMakeVisible(*label_8);
    label_8->setFont(juce::Font(30.00f, juce::Font::plain));
    label_8->setJustificationType(juce::Justification::centredLeft);
    label_8->setEditable(false, false, false);
    label_8->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_8->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_8->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_9 = std::make_unique<juce::Label>(juce::String(), TRANS("FLT ENV KEY-TRIG"));
    addAndMakeVisible(*label_9);
    label_9->setFont(juce::Font(30.00f, juce::Font::plain));
    label_9->setJustificationType(juce::Justification::centredLeft);
    label_9->setEditable(false, false, false);
    label_9->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_9->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_9->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_10 = std::make_unique<juce::Label>(juce::String(), TRANS("FLT OUTPUT KEY-TRIG"));
    addAndMakeVisible(*label_10);
    label_10->setFont(juce::Font(30.00f, juce::Font::plain));
    label_10->setJustificationType(juce::Justification::centredLeft);
    label_10->setEditable(false, false, false);
    label_10->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_10->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_10->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_osc_key_tracking_highest2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_key_tracking_highest2);
    button_osc_key_tracking_highest2->setButtonText(TRANS("LF"));
    button_osc_key_tracking_highest2->addListener(this);
    button_osc_key_tracking_highest2->setColour(juce::TextButton::buttonColourId,
                                                juce::Colours::black);
    button_osc_key_tracking_highest2->setColour(juce::TextButton::buttonOnColourId,
                                                juce::Colour(0xffff1111));
    button_osc_key_tracking_highest2->setColour(juce::TextButton::textColourOnId,
                                                juce::Colour(0xffff3b00));
    button_osc_key_tracking_highest2->setColour(juce::TextButton::textColourOffId,
                                                juce::Colours::yellow);

    label_12 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_12);
    label_12->setFont(juce::Font(30.00f, juce::Font::plain));
    label_12->setJustificationType(juce::Justification::centredLeft);
    label_12->setEditable(false, false, false);
    label_12->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_13 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_13);
    label_13->setFont(juce::Font(30.00f, juce::Font::plain));
    label_13->setJustificationType(juce::Justification::centredLeft);
    label_13->setEditable(false, false, false);
    label_13->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_13->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_13->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_flt_out_sesitivity_1 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_flt_out_sesitivity_1);
    slider_flt_out_sesitivity_1->setTooltip(TRANS("\n"));
    slider_flt_out_sesitivity_1->setRange(0, 1, 0.001);
    slider_flt_out_sesitivity_1->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_flt_out_sesitivity_1->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_flt_out_sesitivity_1->setColour(juce::Slider::rotarySliderFillColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_1->setColour(juce::Slider::rotarySliderOutlineColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_1->setColour(juce::Slider::textBoxTextColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_1->setColour(juce::Slider::textBoxBackgroundColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_1->addListener(this);

    slider_flt_out_sesitivity_2 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_flt_out_sesitivity_2);
    slider_flt_out_sesitivity_2->setRange(0, 1, 0.001);
    slider_flt_out_sesitivity_2->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_flt_out_sesitivity_2->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_flt_out_sesitivity_2->setColour(juce::Slider::rotarySliderFillColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_2->setColour(juce::Slider::rotarySliderOutlineColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_2->setColour(juce::Slider::textBoxTextColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_2->setColour(juce::Slider::textBoxBackgroundColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_2->addListener(this);

    slider_flt_out_sesitivity_3 = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_flt_out_sesitivity_3);
    slider_flt_out_sesitivity_3->setRange(0, 1, 0.001);
    slider_flt_out_sesitivity_3->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_flt_out_sesitivity_3->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_flt_out_sesitivity_3->setColour(juce::Slider::rotarySliderFillColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_3->setColour(juce::Slider::rotarySliderOutlineColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_3->setColour(juce::Slider::textBoxTextColourId,
                                           juce::Colours::yellow);
    slider_flt_out_sesitivity_3->setColour(juce::Slider::textBoxBackgroundColourId,
                                           juce::Colour(0xff161616));
    slider_flt_out_sesitivity_3->addListener(this);

    label_21 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_21);
    label_21->setFont(juce::Font(30.00f, juce::Font::plain));
    label_21->setJustificationType(juce::Justification::centredLeft);
    label_21->setEditable(false, false, false);
    label_21->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_21->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_21->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_22 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_22);
    label_22->setFont(juce::Font(30.00f, juce::Font::plain));
    label_22->setJustificationType(juce::Justification::centredLeft);
    label_22->setEditable(false, false, false);
    label_22->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_22->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_22->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_23 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_23);
    label_23->setFont(juce::Font(30.00f, juce::Font::plain));
    label_23->setJustificationType(juce::Justification::centredLeft);
    label_23->setEditable(false, false, false);
    label_23->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_23->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_23->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_24 = std::make_unique<juce::Label>(juce::String(), TRANS("NOTE ORDER"));
    addAndMakeVisible(*label_24);
    label_24->setFont(juce::Font(30.00f, juce::Font::plain));
    label_24->setJustificationType(juce::Justification::centredLeft);
    label_24->setEditable(false, false, false);
    label_24->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_24->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_24->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    close = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*close);
    close->setTooltip(TRANS("Close setup."));
    close->setButtonText(TRANS("X"));
    close->addListener(this);
    close->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    close->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    close->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    close->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    label_3 = std::make_unique<juce::Label>(juce::String(), TRANS("PRESETS"));
    addAndMakeVisible(*label_3);
    label_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_3->setJustificationType(juce::Justification::centredLeft);
    label_3->setEditable(false, false, false);
    label_3->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    //[UserPreSize]
    setOpaque(true);
    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        juce::Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MIDI_THEME);
        if (juce::Slider *slider = dynamic_cast<juce::Slider *>(child))
        {
            slider->getProperties().set(VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER);
        }
    }

    slider_osc_tracking_oct_2->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                   SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_osc_tracking_oct_3->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                   SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_cutoff_tracking_oct_1->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                      SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_cutoff_tracking_oct_2->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                      SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_cutoff_tracking_oct_3->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                      SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);

    button_tracking_mode_hm->setEnabled(false);
    // slider_midi_pickup->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE,
    // SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX ); toggle_input_main_thru->setOpaque(false);
    // slider_midi_pickup->setValue( synth_data->midi_pickup_offset*1000, dontSendNotification );

    /*
    //[/UserPreSize]

    setSize (1465, 180);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_Playback::~Monique_Ui_Playback()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    button_preset_agro = nullptr;
    button_preset_down = nullptr;
    button_preset_rising = nullptr;
    button_preset_soft = nullptr;
    button_flt_out_triggering_3 = nullptr;
    button_flt_out_triggering_2 = nullptr;
    button_flt_out_triggering_1 = nullptr;
    button_flt_env_triggering_3 = nullptr;
    button_flt_env_triggering_2 = nullptr;
    button_flt_env_triggering_1 = nullptr;
    button_flt_input_triggering_3 = nullptr;
    button_flt_input_triggering_2 = nullptr;
    button_flt_input_triggering_1 = nullptr;
    filter_type_bg_button_1 = nullptr;
    button_tracking_mode_hm = nullptr;
    button_tracking_mode_keep = nullptr;
    button_tracking_mode_lf = nullptr;
    button_tracking_mode_hf = nullptr;
    slider_cutoff_tracking_oct_3 = nullptr;
    slider_cutoff_tracking_oct_2 = nullptr;
    slider_cutoff_tracking_oct_1 = nullptr;
    slider_osc_tracking_oct_3 = nullptr;
    slider_osc_tracking_oct_2 = nullptr;
    button_cutoff_tracking_3 = nullptr;
    button_cutoff_tracking_2 = nullptr;
    button_cutoff_tracking_1 = nullptr;
    button_osc_tracking_3 = nullptr;
    button_osc_tracking_2 = nullptr;
    button_osc_tracking_1 = nullptr;
    label_2 = nullptr;
    label_4 = nullptr;
    label_5 = nullptr;
    label_6 = nullptr;
    label_7 = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    label_10 = nullptr;
    button_osc_key_tracking_highest2 = nullptr;
    label_12 = nullptr;
    label_13 = nullptr;
    slider_flt_out_sesitivity_1 = nullptr;
    slider_flt_out_sesitivity_2 = nullptr;
    slider_flt_out_sesitivity_3 = nullptr;
    label_21 = nullptr;
    label_22 = nullptr;
    label_23 = nullptr;
    label_24 = nullptr;
    close = nullptr;
    label_3 = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_Playback::paint(juce::Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPrePaint]

    g.fillAll(Colour(0xff050505));

    g.setColour(Colour(0xffff3b00));
    g.fillRoundedRectangle(15.0f, 256.0f, 570.0f, 1.0f, 1.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(181.0f, 0.0f, 409.0f, 250.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(10.0f, 0.0f, 169.0f, 250.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1011.0f, 0.0f, 209.0f, 250.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(591.0f, 0.0f, 209.0f, 250.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(801.0f, 0.0f, 209.0f, 250.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1221.0f, 0.0f, 229.0f, 250.0f, 10.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_Playback::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    button_preset_agro->setBounds(1240, 130, 90, 30);
    button_preset_down->setBounds(1240, 100, 90, 30);
    button_preset_rising->setBounds(1240, 70, 90, 30);
    button_preset_soft->setBounds(1240, 40, 90, 30);
    button_flt_out_triggering_3->setBounds(1145, 40, 60, 30);
    button_flt_out_triggering_2->setBounds(1085, 40, 60, 30);
    button_flt_out_triggering_1->setBounds(1025, 40, 60, 30);
    button_flt_env_triggering_3->setBounds(935, 40, 60, 30);
    button_flt_env_triggering_2->setBounds(875, 40, 60, 30);
    button_flt_env_triggering_1->setBounds(815, 40, 60, 30);
    button_flt_input_triggering_3->setBounds(725, 40, 60, 30);
    button_flt_input_triggering_2->setBounds(665, 40, 60, 30);
    button_flt_input_triggering_1->setBounds(605, 40, 60, 30);
    filter_type_bg_button_1->setBounds(25, 40, 140, 130);
    button_tracking_mode_hm->setBounds(25, 140, 140, 30);
    button_tracking_mode_keep->setBounds(25, 40, 140, 30);
    button_tracking_mode_lf->setBounds(25, 107, 140, 30);
    button_tracking_mode_hf->setBounds(25, 73, 140, 30);
    slider_cutoff_tracking_oct_3->setBounds(515, 80, 60, 56);
    slider_cutoff_tracking_oct_2->setBounds(455, 80, 60, 56);
    slider_cutoff_tracking_oct_1->setBounds(395, 80, 60, 56);
    slider_osc_tracking_oct_3->setBounds(315, 80, 60, 56);
    slider_osc_tracking_oct_2->setBounds(255, 80, 60, 56);
    button_cutoff_tracking_3->setBounds(515, 40, 60, 30);
    button_cutoff_tracking_2->setBounds(455, 40, 60, 30);
    button_cutoff_tracking_1->setBounds(395, 40, 60, 30);
    button_osc_tracking_3->setBounds(315, 40, 60, 30);
    button_osc_tracking_2->setBounds(255, 40, 60, 30);
    button_osc_tracking_1->setBounds(195, 40, 60, 30);
    label_2->setBounds(195, 0, 180, 30);
    label_4->setBounds(390, 0, 190, 30);
    label_5->setBounds(395, 140, 60, 30);
    label_6->setBounds(455, 140, 60, 30);
    label_7->setBounds(515, 140, 60, 30);
    label_8->setBounds(600, 0, 190, 30);
    label_9->setBounds(810, 0, 190, 30);
    label_10->setBounds(1020, 0, 190, 30);
    button_osc_key_tracking_highest2->setBounds(-622, 38, 60, 30);
    label_12->setBounds(255, 140, 60, 30);
    label_13->setBounds(315, 140, 60, 30);
    slider_flt_out_sesitivity_1->setBounds(1025, 80, 60, 56);
    slider_flt_out_sesitivity_2->setBounds(1085, 80, 60, 56);
    slider_flt_out_sesitivity_3->setBounds(1145, 80, 60, 56);
    label_21->setBounds(1025, 140, 60, 30);
    label_22->setBounds(1085, 140, 60, 30);
    label_23->setBounds(1145, 140, 60, 30);
    label_24->setBounds(20, 0, 150, 30);
    close->setBounds(1420, 5, 25, 25);
    label_3->setBounds(1260, 0, 150, 30);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_Playback::buttonClicked(juce::Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
#ifdef POLY
    //[/UserbuttonClicked_Pre]
    if (buttonThatWasClicked == button_preset_agro)
    {
        //[UserButtonCode_button_preset_agro] -- add your button handler code here..

        synth_data->keytrack_osci[0] = true;
        synth_data->keytrack_osci[1] = true;
        synth_data->keytrack_osci[2] = true;
        synth_data->keytrack_osci_octave_offset[0] = 0;
        synth_data->keytrack_osci_octave_offset[1] = 1;
        synth_data->keytrack_osci_octave_offset[2] = -2;
        synth_data->keytrack_cutoff[0] = true;
        synth_data->keytrack_cutoff[1] = true;
        synth_data->keytrack_cutoff[2] = true;
        synth_data->keytrack_cutoff_octave_offset[0] = 1;
        synth_data->keytrack_cutoff_octave_offset[1] = 3;
        synth_data->keytrack_cutoff_octave_offset[2] = 3;
        synth_data->keytrack_filter_inputs[0] = false;
        synth_data->keytrack_filter_inputs[1] = false;
        synth_data->keytrack_filter_inputs[2] = false;
        synth_data->keytrack_filter_env[0] = true;
        synth_data->keytrack_filter_env[1] = true;
        synth_data->keytrack_filter_env[2] = true;
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 1;
        //[/UserButtonCode_button_preset_agro]
    }
    else if (buttonThatWasClicked == button_preset_down)
    {
        //[UserButtonCode_button_preset_down] -- add your button handler code here..
        synth_data->keytrack_osci[0] = true;
        synth_data->keytrack_osci[1] = true;
        synth_data->keytrack_osci[2] = true;
        synth_data->keytrack_osci_octave_offset[0] = 0;
        synth_data->keytrack_osci_octave_offset[1] = 0;
        synth_data->keytrack_osci_octave_offset[2] = 1;
        synth_data->keytrack_cutoff[0] = true;
        synth_data->keytrack_cutoff[1] = true;
        synth_data->keytrack_cutoff[2] = true;
        synth_data->keytrack_cutoff_octave_offset[0] = 1;
        synth_data->keytrack_cutoff_octave_offset[1] = 2;
        synth_data->keytrack_cutoff_octave_offset[2] = 1;
        synth_data->keytrack_filter_inputs[0] = false;
        synth_data->keytrack_filter_inputs[1] = false;
        synth_data->keytrack_filter_inputs[2] = false;
        synth_data->keytrack_filter_env[0] = true;
        synth_data->keytrack_filter_env[1] = true;
        synth_data->keytrack_filter_env[2] = true;
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 0;
        //[/UserButtonCode_button_preset_down]
    }
    else if (buttonThatWasClicked == button_preset_rising)
    {
        //[UserButtonCode_button_preset_rising] -- add your button handler code here..
        synth_data->keytrack_osci[0] = true;
        synth_data->keytrack_osci[1] = true;
        synth_data->keytrack_osci[2] = true;
        synth_data->keytrack_osci_octave_offset[0] = 0;
        synth_data->keytrack_osci_octave_offset[1] = 1;
        synth_data->keytrack_osci_octave_offset[2] = 2;
        synth_data->keytrack_cutoff[0] = true;
        synth_data->keytrack_cutoff[1] = true;
        synth_data->keytrack_cutoff[2] = true;
        synth_data->keytrack_cutoff_octave_offset[0] = 0;
        synth_data->keytrack_cutoff_octave_offset[1] = 2;
        synth_data->keytrack_cutoff_octave_offset[2] = 1;
        synth_data->keytrack_filter_inputs[0] = false;
        synth_data->keytrack_filter_inputs[1] = false;
        synth_data->keytrack_filter_inputs[2] = false;
        synth_data->keytrack_filter_env[0] = true;
        synth_data->keytrack_filter_env[1] = true;
        synth_data->keytrack_filter_env[2] = true;
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 0;
        //[/UserButtonCode_button_preset_rising]
    }
    else if (buttonThatWasClicked == button_preset_soft)
    {
        //[UserButtonCode_button_preset_soft] -- add your button handler code here..
        synth_data->keytrack_osci[0] = true;
        synth_data->keytrack_osci[1] = true;
        synth_data->keytrack_osci[2] = true;
        synth_data->keytrack_osci_octave_offset[0] = 0;
        synth_data->keytrack_osci_octave_offset[1] = 2;
        synth_data->keytrack_osci_octave_offset[2] = 2;
        synth_data->keytrack_cutoff[0] = true;
        synth_data->keytrack_cutoff[1] = true;
        synth_data->keytrack_cutoff[2] = true;
        synth_data->keytrack_cutoff_octave_offset[0] = 2;
        synth_data->keytrack_cutoff_octave_offset[1] = 0;
        synth_data->keytrack_cutoff_octave_offset[2] = 1;
        synth_data->keytrack_filter_inputs[0] = false;
        synth_data->keytrack_filter_inputs[1] = false;
        synth_data->keytrack_filter_inputs[2] = false;
        synth_data->keytrack_filter_env[0] = true;
        synth_data->keytrack_filter_env[1] = true;
        synth_data->keytrack_filter_env[2] = true;
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 1;
        //[/UserButtonCode_button_preset_soft]
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_3)
    {
        //[UserButtonCode_button_flt_out_triggering_3] -- add your button handler code here..
        synth_data->keytrack_filter_volume[2] ^= true;
        //[/UserButtonCode_button_flt_out_triggering_3]
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_2)
    {
        //[UserButtonCode_button_flt_out_triggering_2] -- add your button handler code here..
        synth_data->keytrack_filter_volume[1] ^= true;
        //[/UserButtonCode_button_flt_out_triggering_2]
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_1)
    {
        //[UserButtonCode_button_flt_out_triggering_1] -- add your button handler code here..
        synth_data->keytrack_filter_volume[0] ^= true;
        //[/UserButtonCode_button_flt_out_triggering_1]
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_3)
    {
        //[UserButtonCode_button_flt_env_triggering_3] -- add your button handler code here..
        synth_data->keytrack_filter_env[2] ^= true;
        //[/UserButtonCode_button_flt_env_triggering_3]
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_2)
    {
        //[UserButtonCode_button_flt_env_triggering_2] -- add your button handler code here..
        synth_data->keytrack_filter_env[1] ^= true;
        //[/UserButtonCode_button_flt_env_triggering_2]
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_1)
    {
        //[UserButtonCode_button_flt_env_triggering_1] -- add your button handler code here..
        synth_data->keytrack_filter_env[0] ^= true;
        //[/UserButtonCode_button_flt_env_triggering_1]
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_3)
    {
        //[UserButtonCode_button_flt_input_triggering_3] -- add your button handler code here..
        synth_data->keytrack_filter_inputs[2] ^= true;
        //[/UserButtonCode_button_flt_input_triggering_3]
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_2)
    {
        //[UserButtonCode_button_flt_input_triggering_2] -- add your button handler code here..
        synth_data->keytrack_filter_inputs[1] ^= true;
        //[/UserButtonCode_button_flt_input_triggering_2]
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_1)
    {
        //[UserButtonCode_button_flt_input_triggering_1] -- add your button handler code here..
        synth_data->keytrack_filter_inputs[0] ^= true;
        //[/UserButtonCode_button_flt_input_triggering_1]
    }
    else if (buttonThatWasClicked == button_tracking_mode_hm)
    {
        //[UserButtonCode_button_tracking_mode_hm] -- add your button handler code here..
        synth_data->keytrack_osci_play_mode = 3;
        //[/UserButtonCode_button_tracking_mode_hm]
    }
    else if (buttonThatWasClicked == button_tracking_mode_keep)
    {
        //[UserButtonCode_button_tracking_mode_keep] -- add your button handler code here..
        synth_data->keytrack_osci_play_mode = 2;
        //[/UserButtonCode_button_tracking_mode_keep]
    }
    else if (buttonThatWasClicked == button_tracking_mode_lf)
    {
        //[UserButtonCode_button_tracking_mode_lf] -- add your button handler code here..
        synth_data->keytrack_osci_play_mode = 0;
        //[/UserButtonCode_button_tracking_mode_lf]
    }
    else if (buttonThatWasClicked == button_tracking_mode_hf)
    {
        //[UserButtonCode_button_tracking_mode_hf] -- add your button handler code here..
        synth_data->keytrack_osci_play_mode = 1;
        //[/UserButtonCode_button_tracking_mode_hf]
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_3)
    {
        //[UserButtonCode_button_cutoff_tracking_3] -- add your button handler code here..
        synth_data->keytrack_cutoff[2] ^= true;
        //[/UserButtonCode_button_cutoff_tracking_3]
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_2)
    {
        //[UserButtonCode_button_cutoff_tracking_2] -- add your button handler code here..
        synth_data->keytrack_cutoff[1] ^= true;
        //[/UserButtonCode_button_cutoff_tracking_2]
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_1)
    {
        //[UserButtonCode_button_cutoff_tracking_1] -- add your button handler code here..
        synth_data->keytrack_cutoff[0] ^= true;
        //[/UserButtonCode_button_cutoff_tracking_1]
    }
    else if (buttonThatWasClicked == button_osc_tracking_3)
    {
        //[UserButtonCode_button_osc_tracking_3] -- add your button handler code here..
        synth_data->keytrack_osci[2] ^= true;
        //[/UserButtonCode_button_osc_tracking_3]
    }
    else if (buttonThatWasClicked == button_osc_tracking_2)
    {
        //[UserButtonCode_button_osc_tracking_2] -- add your button handler code here..
        synth_data->keytrack_osci[1] ^= true;
        //[/UserButtonCode_button_osc_tracking_2]
    }
    else if (buttonThatWasClicked == button_osc_tracking_1)
    {
        //[UserButtonCode_button_osc_tracking_1] -- add your button handler code here..
        synth_data->keytrack_osci[0] ^= true;
        //[/UserButtonCode_button_osc_tracking_1]
    }
    else if (buttonThatWasClicked == button_osc_key_tracking_highest2)
    {
        //[UserButtonCode_button_osc_key_tracking_highest2] -- add your button handler code here..
        //[/UserButtonCode_button_osc_key_tracking_highest2]
    }
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        //[/UserButtonCode_close]
    }

    //[UserbuttonClicked_Post]
#endif
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_Playback::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
#ifdef POLY
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_cutoff_tracking_oct_3)
    {
        //[UserSliderCode_slider_cutoff_tracking_oct_3] -- add your slider handling code here..
        synth_data->keytrack_cutoff_octave_offset[2] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_cutoff_tracking_oct_3]
    }
    else if (sliderThatWasMoved == slider_cutoff_tracking_oct_2)
    {
        //[UserSliderCode_slider_cutoff_tracking_oct_2] -- add your slider handling code here..
        synth_data->keytrack_cutoff_octave_offset[1] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_cutoff_tracking_oct_2]
    }
    else if (sliderThatWasMoved == slider_cutoff_tracking_oct_1)
    {
        //[UserSliderCode_slider_cutoff_tracking_oct_1] -- add your slider handling code here..
        synth_data->keytrack_cutoff_octave_offset[0] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_cutoff_tracking_oct_1]
    }
    else if (sliderThatWasMoved == slider_osc_tracking_oct_3)
    {
        //[UserSliderCode_slider_osc_tracking_oct_3] -- add your slider handling code here..
        synth_data->keytrack_osci_octave_offset[2] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_osc_tracking_oct_3]
    }
    else if (sliderThatWasMoved == slider_osc_tracking_oct_2)
    {
        //[UserSliderCode_slider_osc_tracking_oct_2] -- add your slider handling code here..
        synth_data->keytrack_osci_octave_offset[1] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_osc_tracking_oct_2]
    }
    else if (sliderThatWasMoved == slider_flt_out_sesitivity_1)
    {
        //[UserSliderCode_slider_flt_out_sesitivity_1] -- add your slider handling code here..
        synth_data->keytrack_filter_volume_offset[0] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_flt_out_sesitivity_1]
    }
    else if (sliderThatWasMoved == slider_flt_out_sesitivity_2)
    {
        //[UserSliderCode_slider_flt_out_sesitivity_2] -- add your slider handling code here..
        synth_data->keytrack_filter_volume_offset[1] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_flt_out_sesitivity_2]
    }
    else if (sliderThatWasMoved == slider_flt_out_sesitivity_3)
    {
        //[UserSliderCode_slider_flt_out_sesitivity_3] -- add your slider handling code here..
        synth_data->keytrack_filter_volume_offset[2] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_flt_out_sesitivity_3]
    }

    //[UsersliderValueChanged_Post]
#endif
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_Playback" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow* parent_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;original_w(1465), &#10;original_h(180)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="15 256 570 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="181 0 409 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="10 0 169 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1011 0 209 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="591 0 209 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="801 0 209 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1221 0 229 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
  </BACKGROUND>
  <TEXTBUTTON name="" id="15e881164d19f29b" memberName="button_preset_agro"
              virtualName="" explicitFocusOrder="0" pos="1240 130 90 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="AGRESSIVE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="af6069a413186503" memberName="button_preset_down"
              virtualName="" explicitFocusOrder="0" pos="1240 100 90 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="DOWN"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="879dba86b68841e7" memberName="button_preset_rising"
              virtualName="" explicitFocusOrder="0" pos="1240 70 90 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="RISING"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c79f13786e76a48b" memberName="button_preset_soft"
              virtualName="" explicitFocusOrder="0" pos="1240 40 90 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="SOFT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="98903a957525eccb" memberName="button_flt_out_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="1145 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="40d5725e65405de1" memberName="button_flt_out_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="1085 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="58d6c148c398eb2c" memberName="button_flt_out_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="1025 40 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="44d00292596c1d28" memberName="button_flt_env_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="935 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2378bbfb84f1ab97" memberName="button_flt_env_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="875 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="3c2bd1d338499dd" memberName="button_flt_env_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="815 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6be8f0e439149b1c" memberName="button_flt_input_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="725 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b408d83bfb8e45f6" memberName="button_flt_input_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="665 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="dc48f03c31f28f31" memberName="button_flt_input_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="605 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="40809d639d290176" memberName="filter_type_bg_button_1"
              virtualName="" explicitFocusOrder="0" pos="25 40 140 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fd80a890cf07a64" memberName="button_tracking_mode_hm"
              virtualName="" explicitFocusOrder="0" pos="25 140 140 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="---"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ccd73db87cfab50b" memberName="button_tracking_mode_keep"
              virtualName="" explicitFocusOrder="0" pos="25 40 140 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="AS YOU PLAY"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f4c678f3f0e9f6d5" memberName="button_tracking_mode_lf"
              virtualName="" explicitFocusOrder="0" pos="25 107 140 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="HIGH to LOW"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f463c263e3c8841d" memberName="button_tracking_mode_hf"
              virtualName="" explicitFocusOrder="0" pos="25 73 140 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LOW to HIGH"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="0" id="af7cd66c376dd8af" memberName="slider_cutoff_tracking_oct_3"
          virtualName="Slider" explicitFocusOrder="0" pos="515 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="eaeb1952f52d40f1" memberName="slider_cutoff_tracking_oct_2"
          virtualName="Slider" explicitFocusOrder="0" pos="455 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_cutoff_tracking_oct_1"
          virtualName="Slider" explicitFocusOrder="0" pos="395 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="766b2badd17965f3" memberName="slider_osc_tracking_oct_3"
          virtualName="Slider" explicitFocusOrder="0" pos="315 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-2" max="2" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="b2d956b7def64e52" memberName="slider_osc_tracking_oct_2"
          virtualName="Slider" explicitFocusOrder="0" pos="255 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-2" max="2" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="6a3e54bcd6cd51b" memberName="button_cutoff_tracking_3"
              virtualName="" explicitFocusOrder="0" pos="515 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9aa0dc1b0f793710" memberName="button_cutoff_tracking_2"
              virtualName="" explicitFocusOrder="0" pos="455 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d11a466dd34a172" memberName="button_cutoff_tracking_1"
              virtualName="" explicitFocusOrder="0" pos="395 40 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2194df5e065a174a" memberName="button_osc_tracking_3"
              virtualName="" explicitFocusOrder="0" pos="315 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="702de62630ddb999" memberName="button_osc_tracking_2"
              virtualName="" explicitFocusOrder="0" pos="255 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7a3b545ab65b9c5c" memberName="button_osc_tracking_1"
              virtualName="" explicitFocusOrder="0" pos="195 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="OSC 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="59fe62ccfd0a3d95" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="195 0 180 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC KEY-TRACKING"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="fb7b01265de6cb01" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="390 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CUTOFF KEY-TRACKING"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="83516a58e629f75e" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="395 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="f66dfa6aa4e89192" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="455 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1a3d269596c5fd4b" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="515 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="ec3600d0ff1f41b7" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="600 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT INPUT KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="4e728fb8882f8240" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="810 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT ENV KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="c9659e86997ec2bc" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="1020 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT OUTPUT KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="30ce1d751bea6be8" memberName="button_osc_key_tracking_highest2"
              virtualName="" explicitFocusOrder="0" pos="-622 38 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LF"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="47badf2084c5bcb1" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="255 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="76dfa6e205a722e0" memberName="label_13" virtualName=""
         explicitFocusOrder="0" pos="315 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="1a1d7f6c2ca843ef" memberName="slider_flt_out_sesitivity_1"
          virtualName="Slider" explicitFocusOrder="0" pos="1025 80 60 56"
          tooltip="&#10;" rotarysliderfill="ffffff00" rotaryslideroutline="ff161616"
          textboxtext="ffffff00" textboxbkgd="ff161616" min="0" max="1"
          int="0.0010000000000000000208" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="7fe963dd2889e887" memberName="slider_flt_out_sesitivity_2"
          virtualName="Slider" explicitFocusOrder="0" pos="1085 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.0010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="821606ba1a813d43" memberName="slider_flt_out_sesitivity_3"
          virtualName="Slider" explicitFocusOrder="0" pos="1145 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.0010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="83734337ebb799ae" memberName="label_21" virtualName=""
         explicitFocusOrder="0" pos="1025 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="47a727c85872ccd8" memberName="label_22" virtualName=""
         explicitFocusOrder="0" pos="1085 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1aaee1ac83e64c9c" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="1145 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="b53da3238c135f98" memberName="label_24" virtualName=""
         explicitFocusOrder="0" pos="20 0 150 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="NOTE ORDER" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="b6a23ff465364b08" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="1420 5 25 25" tooltip="Close setup."
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="b602d449dc6b32a3" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="1260 0 150 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="PRESETS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
