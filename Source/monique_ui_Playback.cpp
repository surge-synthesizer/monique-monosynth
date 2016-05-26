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

    // PLAYBACK
    TURN_BUTTON_ON_OR_OFF( button_playback_last, synth_data->play_mode == 0 );
    TURN_BUTTON_ON_OR_OFF( button_playback_first, synth_data->play_mode == 1 );
    TURN_BUTTON_ON_OR_OFF( button_playback_high, synth_data->play_mode == 2 );
    TURN_BUTTON_ON_OR_OFF( button_playback_low, synth_data->play_mode == 3 );

    // MODES
    TURN_BUTTON_ON_OR_OFF( button_tracking_mode_lf, synth_data->keytrack_osci_play_mode == 0 );
    TURN_BUTTON_ON_OR_OFF( button_tracking_mode_hf, synth_data->keytrack_osci_play_mode == 1 );
    TURN_BUTTON_ON_OR_OFF( button_tracking_mode_lm, synth_data->keytrack_osci_play_mode == 2 );
    TURN_BUTTON_ON_OR_OFF( button_tracking_mode_hm, synth_data->keytrack_osci_play_mode == 3 );

    // OSC TRACKING
    TURN_BUTTON_ON_OR_OFF( button_osc_tracking_1, synth_data->keytrack_osci[0] );
    if( button_osc_tracking_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[0] ? 0.7+is_key_0_down*0.3 : 0 ) )
    {
        button_osc_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_osc_tracking_2, synth_data->keytrack_osci[1] );
    if( button_osc_tracking_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[1] ? 0.7+is_key_1_down*0.3 : 0 ) )
    {
        button_osc_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_osc_tracking_3, synth_data->keytrack_osci[2] );
    if( button_osc_tracking_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_osci[2] ? 0.7+is_key_2_down*0.3 : 0 ) )
    {
        button_osc_tracking_3->repaint();
    }
    slider_osc_tracking_oct_2->setValue( synth_data->keytrack_osci_octave_offset[1].get_value(), dontSendNotification );
    slider_osc_tracking_oct_3->setValue( synth_data->keytrack_osci_octave_offset[2].get_value(), dontSendNotification );
    slider_osc_tracking_oct_4->setValue( synth_data->keytrack_osci_octave_offset[3].get_value(), dontSendNotification );

    TURN_BUTTON_ON_OR_OFF( button_cutoff_tracking_1, synth_data->keytrack_cutoff[0] );
    if( button_cutoff_tracking_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[0] ? 0.7+is_key_0_down*0.3 : 0 ) )
    {
        button_cutoff_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_tracking_2, synth_data->keytrack_cutoff[1] );
    if( button_cutoff_tracking_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[1] ? 0.7+is_key_1_down*0.3 : 0 ) )
    {
        button_cutoff_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_tracking_3, synth_data->keytrack_cutoff[2] );
    if( button_cutoff_tracking_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_cutoff[2] ? 0.7+is_key_2_down*0.3 : 0 ) )
    {
        button_cutoff_tracking_3->repaint();
    }
    slider_cutoff_tracking_oct_1->setValue( synth_data->keytrack_cutoff_octave_offset[0].get_value(), dontSendNotification );
    slider_cutoff_tracking_oct_2->setValue( synth_data->keytrack_cutoff_octave_offset[1].get_value(), dontSendNotification );
    slider_cutoff_tracking_oct_3->setValue( synth_data->keytrack_cutoff_octave_offset[2].get_value(), dontSendNotification );

    // INPUT TRIG
    TURN_BUTTON_ON_OR_OFF( button_flt_input_triggering_1, synth_data->keytrack_filter_inputs[0] );
    if( button_flt_input_triggering_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_inputs[0] ? 0.7+is_key_0_down*0.3 : 0 ) )
    {
        button_flt_input_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_input_triggering_2, synth_data->keytrack_filter_inputs[1] );
    if( button_flt_input_triggering_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_inputs[1] ? 0.7+is_key_1_down*0.3 : 0 ) )
    {
        button_flt_input_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_input_triggering_3, synth_data->keytrack_filter_inputs[2] );
    if( button_flt_input_triggering_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_inputs[2] ? 0.7+is_key_2_down*0.3 : 0 ) )
    {
        button_flt_input_triggering_3->repaint();
    }

    // ENV TRIG
    TURN_BUTTON_ON_OR_OFF( button_flt_env_triggering_1, synth_data->keytrack_filter_env[0] );
    if( button_flt_env_triggering_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env[0] ? 0.7+is_key_0_down*0.3 : 0 ) )
    {
        button_flt_env_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_env_triggering_2, synth_data->keytrack_filter_env[1] );
    if( button_flt_env_triggering_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env[1] ? 0.7+is_key_1_down*0.3 : 0 ) )
    {
        button_flt_env_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_env_triggering_3, synth_data->keytrack_filter_env[2] );
    if( button_flt_env_triggering_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env[2] ? 0.7+is_key_2_down*0.3 : 0 ) )
    {
        button_flt_env_triggering_3->repaint();
    }

    // OUTPUT TRIG
    TURN_BUTTON_ON_OR_OFF( button_flt_out_triggering_1, synth_data->keytrack_filter_volume[0] );
    if( button_flt_out_triggering_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume[0] ? 0.7+is_key_0_down*0.3 : 0 ) )
    {
        button_flt_out_triggering_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_out_triggering_2, synth_data->keytrack_filter_volume[1] );
    if( button_flt_out_triggering_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume[1] ? 0.7+is_key_1_down*0.3 : 0 ) )
    {
        button_flt_out_triggering_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_out_triggering_3, synth_data->keytrack_filter_volume[2] );
    if( button_flt_out_triggering_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume[2] ? 0.7+is_key_2_down*0.3 : 0 ) )
    {
        button_flt_out_triggering_3->repaint();
    }


    /*
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_1, synth_data->keytrack_filter_env_1 );
    if( button_env_key_trigger_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env_1 ? 0.7 + is_key_0_down*0.3 : 0 ) )
    {
        button_env_key_trigger_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_2, synth_data->keytrack_filter_env_2 );
    if( button_env_key_trigger_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env_2 ? 0.7 + is_key_1_down*0.3 : 0 ) )
    {
        button_env_key_trigger_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_env_key_trigger_3, synth_data->keytrack_filter_env_3 );
    if( button_env_key_trigger_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_env_3 ? 0.7 + is_key_2_down*0.3 : 0 ) )
    {
        button_env_key_trigger_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_1, synth_data->keytrack_filter_volume_1 );
    if( button_flt_key_tracking_volume_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume_1 ? 0.7 + synth_data->get_tracking_env_state(0)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_2, synth_data->keytrack_filter_volume_2 );
    if( button_flt_key_tracking_volume_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume_2 ? 0.7 + synth_data->get_tracking_env_state(1)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_3, synth_data->keytrack_filter_volume_3 );
    if( button_flt_key_tracking_volume_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_volume_3 ? 0.7 + synth_data->get_tracking_env_state(2)*0.3 : 0 ) )
    {
        button_flt_key_tracking_volume_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_1, synth_data->keytrack_filter_cutoff_1 );
    if( button_cutoff_key_tracking_1->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_cutoff_1 ? 0.7 + is_key_0_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_1->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_2, synth_data->keytrack_filter_cutoff_2 );
    if( button_cutoff_key_tracking_2->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_cutoff_2 ? 0.7 + is_key_1_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_2->repaint();
    }
    TURN_BUTTON_ON_OR_OFF( button_cutoff_key_tracking_3, synth_data->keytrack_filter_cutoff_3 );
    if( button_cutoff_key_tracking_3->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->keytrack_filter_cutoff_3 ? 0.7 + is_key_2_down*0.3 : 0 ) )
    {
        button_cutoff_key_tracking_3->repaint();
    }

    TURN_BUTTON_ON_OR_OFF( button_osc_key_tracking2, synth_data->keytrack_filter_inputs );
    */




    //toggle_input_main_thru->setToggleState( _audio_device_manager->main_input_thru, dontSendNotification );
    //TURN_BUTTON_ON_OR_OFF( button_midi_learn, midi_control_handler->is_waiting_for_param() ? true : midi_control_handler->is_learning() ? true : false );
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_Playback::Monique_Ui_Playback (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow* parent_)
    : Monique_Ui_Refreshable(ui_refresher_),
      original_w(1465),
      original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    parent = parent_;
    //[/Constructor_pre]

    addAndMakeVisible (filter_type_bg_button_2 = new TextButton (String()));
    filter_type_bg_button_2->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_bg_button_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_bg_button_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_playback_low = new TextButton (String()));
    button_playback_low->setButtonText (TRANS("REPLACE LOW"));
    button_playback_low->setConnectedEdges (Button::ConnectedOnTop);
    button_playback_low->addListener (this);
    button_playback_low->setColour (TextButton::buttonColourId, Colours::black);
    button_playback_low->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_playback_low->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_playback_low->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_playback_high = new TextButton (String()));
    button_playback_high->setButtonText (TRANS("REPLACE HIGH"));
    button_playback_high->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_playback_high->addListener (this);
    button_playback_high->setColour (TextButton::buttonColourId, Colours::black);
    button_playback_high->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_playback_high->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_playback_high->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_playback_first = new TextButton (String()));
    button_playback_first->setButtonText (TRANS("FIFO"));
    button_playback_first->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_playback_first->addListener (this);
    button_playback_first->setColour (TextButton::buttonColourId, Colours::black);
    button_playback_first->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_playback_first->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_playback_first->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_playback_last = new TextButton (String()));
    button_playback_last->setButtonText (TRANS("LIFO"));
    button_playback_last->setConnectedEdges (Button::ConnectedOnBottom);
    button_playback_last->addListener (this);
    button_playback_last->setColour (TextButton::buttonColourId, Colours::black);
    button_playback_last->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_playback_last->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_playback_last->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_out_triggering_3 = new TextButton (String()));
    button_flt_out_triggering_3->setButtonText (TRANS("3"));
    button_flt_out_triggering_3->addListener (this);
    button_flt_out_triggering_3->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_out_triggering_3->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_flt_out_triggering_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_out_triggering_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_out_triggering_2 = new TextButton (String()));
    button_flt_out_triggering_2->setButtonText (TRANS("2"));
    button_flt_out_triggering_2->addListener (this);
    button_flt_out_triggering_2->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_out_triggering_2->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_flt_out_triggering_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_out_triggering_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_out_triggering_1 = new TextButton (String()));
    button_flt_out_triggering_1->setButtonText (TRANS("FLT 1"));
    button_flt_out_triggering_1->addListener (this);
    button_flt_out_triggering_1->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_out_triggering_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_out_triggering_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_env_triggering_3 = new TextButton (String()));
    button_flt_env_triggering_3->setButtonText (TRANS("3"));
    button_flt_env_triggering_3->addListener (this);
    button_flt_env_triggering_3->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_env_triggering_3->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_env_triggering_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_env_triggering_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_env_triggering_2 = new TextButton (String()));
    button_flt_env_triggering_2->setButtonText (TRANS("2"));
    button_flt_env_triggering_2->addListener (this);
    button_flt_env_triggering_2->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_env_triggering_2->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_env_triggering_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_env_triggering_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_env_triggering_1 = new TextButton (String()));
    button_flt_env_triggering_1->setButtonText (TRANS("FLT 1"));
    button_flt_env_triggering_1->addListener (this);
    button_flt_env_triggering_1->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_env_triggering_1->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_env_triggering_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_env_triggering_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_input_triggering_3 = new TextButton (String()));
    button_flt_input_triggering_3->setButtonText (TRANS("3"));
    button_flt_input_triggering_3->addListener (this);
    button_flt_input_triggering_3->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_input_triggering_3->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_input_triggering_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_input_triggering_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_input_triggering_2 = new TextButton (String()));
    button_flt_input_triggering_2->setButtonText (TRANS("2"));
    button_flt_input_triggering_2->addListener (this);
    button_flt_input_triggering_2->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_input_triggering_2->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_input_triggering_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_input_triggering_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_flt_input_triggering_1 = new TextButton (String()));
    button_flt_input_triggering_1->setButtonText (TRANS("FLT 1"));
    button_flt_input_triggering_1->addListener (this);
    button_flt_input_triggering_1->setColour (TextButton::buttonColourId, Colours::black);
    button_flt_input_triggering_1->setColour (TextButton::buttonOnColourId, Colour (0xff4444ff));
    button_flt_input_triggering_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_flt_input_triggering_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_bg_button_1 = new TextButton (String()));
    filter_type_bg_button_1->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_bg_button_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_bg_button_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_tracking_mode_hm = new TextButton (String()));
    button_tracking_mode_hm->setButtonText (TRANS("HM"));
    button_tracking_mode_hm->setConnectedEdges (Button::ConnectedOnTop);
    button_tracking_mode_hm->addListener (this);
    button_tracking_mode_hm->setColour (TextButton::buttonColourId, Colours::black);
    button_tracking_mode_hm->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_tracking_mode_hm->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_tracking_mode_hm->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_tracking_mode_lm = new TextButton (String()));
    button_tracking_mode_lm->setButtonText (TRANS("LM"));
    button_tracking_mode_lm->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_tracking_mode_lm->addListener (this);
    button_tracking_mode_lm->setColour (TextButton::buttonColourId, Colours::black);
    button_tracking_mode_lm->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_tracking_mode_lm->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_tracking_mode_lm->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_tracking_mode_lf = new TextButton (String()));
    button_tracking_mode_lf->setButtonText (TRANS("LF"));
    button_tracking_mode_lf->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_tracking_mode_lf->addListener (this);
    button_tracking_mode_lf->setColour (TextButton::buttonColourId, Colours::black);
    button_tracking_mode_lf->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_tracking_mode_lf->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_tracking_mode_lf->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_tracking_mode_hf = new TextButton (String()));
    button_tracking_mode_hf->setButtonText (TRANS("HF"));
    button_tracking_mode_hf->setConnectedEdges (Button::ConnectedOnBottom);
    button_tracking_mode_hf->addListener (this);
    button_tracking_mode_hf->setColour (TextButton::buttonColourId, Colours::black);
    button_tracking_mode_hf->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_tracking_mode_hf->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_tracking_mode_hf->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (slider_cutoff_tracking_oct_3 = new Slider ("0"));
    slider_cutoff_tracking_oct_3->setRange (-4, 4, 1);
    slider_cutoff_tracking_oct_3->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_cutoff_tracking_oct_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_cutoff_tracking_oct_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_3->addListener (this);

    addAndMakeVisible (slider_cutoff_tracking_oct_2 = new Slider ("0"));
    slider_cutoff_tracking_oct_2->setRange (-4, 4, 1);
    slider_cutoff_tracking_oct_2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_cutoff_tracking_oct_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_cutoff_tracking_oct_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_2->addListener (this);

    addAndMakeVisible (slider_cutoff_tracking_oct_1 = new Slider ("0"));
    slider_cutoff_tracking_oct_1->setRange (-4, 4, 1);
    slider_cutoff_tracking_oct_1->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_cutoff_tracking_oct_1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_cutoff_tracking_oct_1->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_cutoff_tracking_oct_1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_1->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_cutoff_tracking_oct_1->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_cutoff_tracking_oct_1->addListener (this);

    addAndMakeVisible (slider_osc_tracking_oct_4 = new Slider ("0"));
    slider_osc_tracking_oct_4->setRange (-2, 2, 1);
    slider_osc_tracking_oct_4->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_osc_tracking_oct_4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_osc_tracking_oct_4->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_osc_tracking_oct_4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_osc_tracking_oct_4->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_osc_tracking_oct_4->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_osc_tracking_oct_4->addListener (this);

    addAndMakeVisible (slider_osc_tracking_oct_3 = new Slider ("0"));
    slider_osc_tracking_oct_3->setRange (-2, 2, 1);
    slider_osc_tracking_oct_3->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_osc_tracking_oct_3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_osc_tracking_oct_3->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_osc_tracking_oct_3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_osc_tracking_oct_3->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_osc_tracking_oct_3->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_osc_tracking_oct_3->addListener (this);

    addAndMakeVisible (slider_osc_tracking_oct_2 = new Slider ("0"));
    slider_osc_tracking_oct_2->setRange (-2, 2, 1);
    slider_osc_tracking_oct_2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_osc_tracking_oct_2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_osc_tracking_oct_2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_osc_tracking_oct_2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_osc_tracking_oct_2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_osc_tracking_oct_2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_osc_tracking_oct_2->addListener (this);

    addAndMakeVisible (button_cutoff_tracking_3 = new TextButton (String()));
    button_cutoff_tracking_3->setButtonText (TRANS("3"));
    button_cutoff_tracking_3->addListener (this);
    button_cutoff_tracking_3->setColour (TextButton::buttonColourId, Colours::black);
    button_cutoff_tracking_3->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_cutoff_tracking_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_cutoff_tracking_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_cutoff_tracking_2 = new TextButton (String()));
    button_cutoff_tracking_2->setButtonText (TRANS("2"));
    button_cutoff_tracking_2->addListener (this);
    button_cutoff_tracking_2->setColour (TextButton::buttonColourId, Colours::black);
    button_cutoff_tracking_2->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_cutoff_tracking_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_cutoff_tracking_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_cutoff_tracking_1 = new TextButton (String()));
    button_cutoff_tracking_1->setButtonText (TRANS("FLT 1"));
    button_cutoff_tracking_1->addListener (this);
    button_cutoff_tracking_1->setColour (TextButton::buttonColourId, Colours::black);
    button_cutoff_tracking_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_cutoff_tracking_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_osc_tracking_4 = new TextButton (String()));
    button_osc_tracking_4->setButtonText (TRANS("FM"));
    button_osc_tracking_4->addListener (this);
    button_osc_tracking_4->setColour (TextButton::buttonColourId, Colours::black);
    button_osc_tracking_4->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_osc_tracking_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_osc_tracking_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_osc_tracking_3 = new TextButton (String()));
    button_osc_tracking_3->setButtonText (TRANS("3"));
    button_osc_tracking_3->addListener (this);
    button_osc_tracking_3->setColour (TextButton::buttonColourId, Colours::black);
    button_osc_tracking_3->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_osc_tracking_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_osc_tracking_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_osc_tracking_2 = new TextButton (String()));
    button_osc_tracking_2->setButtonText (TRANS("2"));
    button_osc_tracking_2->addListener (this);
    button_osc_tracking_2->setColour (TextButton::buttonColourId, Colours::black);
    button_osc_tracking_2->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_osc_tracking_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_osc_tracking_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_osc_tracking_1 = new TextButton (String()));
    button_osc_tracking_1->setButtonText (TRANS("OSC 1"));
    button_osc_tracking_1->addListener (this);
    button_osc_tracking_1->setColour (TextButton::buttonColourId, Colours::black);
    button_osc_tracking_1->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_osc_tracking_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_osc_tracking_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (close = new TextButton (String()));
    close->setTooltip (TRANS("Close MIDI settings."));
    close->setButtonText (TRANS("X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (label_2 = new Label (String(),
                                            TRANS("OSC KEY-TRACKING")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredLeft);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String(),
                                            TRANS("CUTOFF KEY-TRACKING")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredLeft);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String(),
                                            TRANS("OCT")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_6 = new Label (String(),
                                            TRANS("OCT")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_7 = new Label (String(),
                                            TRANS("OCT")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredLeft);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_3 = new Label (String(),
                                            TRANS("MODE\n")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_8 = new Label (String(),
                                            TRANS("FLT INPUT KEY-TRIG")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredLeft);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String(),
                                            TRANS("FLT ENV KEY-TRIG")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredLeft);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_10 = new Label (String(),
                                             TRANS("FLT OUTPUT KEY-TRIG")));
    label_10->setFont (Font (30.00f, Font::plain));
    label_10->setJustificationType (Justification::centredLeft);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_osc_key_tracking_highest2 = new TextButton (String()));
    button_osc_key_tracking_highest2->setButtonText (TRANS("LF"));
    button_osc_key_tracking_highest2->addListener (this);
    button_osc_key_tracking_highest2->setColour (TextButton::buttonColourId, Colours::black);
    button_osc_key_tracking_highest2->setColour (TextButton::buttonOnColourId, Colour (0xffff1111));
    button_osc_key_tracking_highest2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_osc_key_tracking_highest2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_12 = new Label (String(),
                                             TRANS("OCT")));
    label_12->setFont (Font (30.00f, Font::plain));
    label_12->setJustificationType (Justification::centredLeft);
    label_12->setEditable (false, false, false);
    label_12->setColour (Label::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_13 = new Label (String(),
                                             TRANS("OCT")));
    label_13->setFont (Font (30.00f, Font::plain));
    label_13->setJustificationType (Justification::centredLeft);
    label_13->setEditable (false, false, false);
    label_13->setColour (Label::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_14 = new Label (String(),
                                             TRANS("OCT")));
    label_14->setFont (Font (30.00f, Font::plain));
    label_14->setJustificationType (Justification::centredLeft);
    label_14->setEditable (false, false, false);
    label_14->setColour (Label::textColourId, Colour (0xffff3b00));
    label_14->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_midi_pickup8 = new Slider ("0"));
    slider_midi_pickup8->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup8->setRange (0, 1000, 1);
    slider_midi_pickup8->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup8->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup8->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup8->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup8->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup8->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup8->addListener (this);

    addAndMakeVisible (slider_midi_pickup9 = new Slider ("0"));
    slider_midi_pickup9->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup9->setRange (0, 1000, 1);
    slider_midi_pickup9->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup9->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup9->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup9->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup9->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup9->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup9->addListener (this);

    addAndMakeVisible (slider_midi_pickup10 = new Slider ("0"));
    slider_midi_pickup10->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup10->setRange (0, 1000, 1);
    slider_midi_pickup10->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup10->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup10->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup10->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup10->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup10->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup10->addListener (this);

    addAndMakeVisible (label_15 = new Label (String(),
                                             TRANS("CIVITY")));
    label_15->setFont (Font (30.00f, Font::plain));
    label_15->setJustificationType (Justification::centredLeft);
    label_15->setEditable (false, false, false);
    label_15->setColour (Label::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_16 = new Label (String(),
                                             TRANS("CIVITY")));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_17 = new Label (String(),
                                             TRANS("CIVITY")));
    label_17->setFont (Font (30.00f, Font::plain));
    label_17->setJustificationType (Justification::centredLeft);
    label_17->setEditable (false, false, false);
    label_17->setColour (Label::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_midi_pickup11 = new Slider ("0"));
    slider_midi_pickup11->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup11->setRange (0, 1000, 1);
    slider_midi_pickup11->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup11->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup11->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup11->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup11->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup11->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup11->addListener (this);

    addAndMakeVisible (slider_midi_pickup12 = new Slider ("0"));
    slider_midi_pickup12->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup12->setRange (0, 1000, 1);
    slider_midi_pickup12->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup12->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup12->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup12->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup12->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup12->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup12->addListener (this);

    addAndMakeVisible (slider_midi_pickup13 = new Slider ("0"));
    slider_midi_pickup13->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup13->setRange (0, 1000, 1);
    slider_midi_pickup13->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup13->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup13->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup13->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup13->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup13->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup13->addListener (this);

    addAndMakeVisible (label_18 = new Label (String(),
                                             TRANS("CIVITY")));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredLeft);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_19 = new Label (String(),
                                             TRANS("CIVITY")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredLeft);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_20 = new Label (String(),
                                             TRANS("CIVITY")));
    label_20->setFont (Font (30.00f, Font::plain));
    label_20->setJustificationType (Justification::centredLeft);
    label_20->setEditable (false, false, false);
    label_20->setColour (Label::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_midi_pickup14 = new Slider ("0"));
    slider_midi_pickup14->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup14->setRange (0, 1000, 1);
    slider_midi_pickup14->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup14->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup14->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup14->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup14->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup14->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup14->addListener (this);

    addAndMakeVisible (slider_midi_pickup15 = new Slider ("0"));
    slider_midi_pickup15->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup15->setRange (0, 1000, 1);
    slider_midi_pickup15->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup15->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup15->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup15->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup15->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup15->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup15->addListener (this);

    addAndMakeVisible (slider_midi_pickup16 = new Slider ("0"));
    slider_midi_pickup16->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup16->setRange (0, 1000, 1);
    slider_midi_pickup16->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup16->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup16->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup16->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup16->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup16->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup16->addListener (this);

    addAndMakeVisible (label_21 = new Label (String(),
                                             TRANS("CIVITY")));
    label_21->setFont (Font (30.00f, Font::plain));
    label_21->setJustificationType (Justification::centredLeft);
    label_21->setEditable (false, false, false);
    label_21->setColour (Label::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_22 = new Label (String(),
                                             TRANS("CIVITY")));
    label_22->setFont (Font (30.00f, Font::plain));
    label_22->setJustificationType (Justification::centredLeft);
    label_22->setEditable (false, false, false);
    label_22->setColour (Label::textColourId, Colour (0xffff3b00));
    label_22->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_22->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_23 = new Label (String(),
                                             TRANS("CIVITY")));
    label_23->setFont (Font (30.00f, Font::plain));
    label_23->setJustificationType (Justification::centredLeft);
    label_23->setEditable (false, false, false);
    label_23->setColour (Label::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_24 = new Label (String(),
                                             TRANS("PLAY MODES")));
    label_24->setFont (Font (30.00f, Font::plain));
    label_24->setJustificationType (Justification::centredLeft);
    label_24->setEditable (false, false, false);
    label_24->setColour (Label::textColourId, Colour (0xffff3b00));
    label_24->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_24->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    setOpaque(true);
    for( int i = 0 ; i != getNumChildComponents() ; ++i )
    {
        Component*child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MIDI_THEME );
        if( Slider* slider = dynamic_cast<Slider*>(child) )
        {
            slider->getProperties().set( VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER );
        }
    }

    slider_osc_tracking_oct_2->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_osc_tracking_oct_3->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_osc_tracking_oct_4->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_cutoff_tracking_oct_1->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_cutoff_tracking_oct_2->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_cutoff_tracking_oct_3->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );

    //slider_midi_pickup->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    //toggle_input_main_thru->setOpaque(false);
    //slider_midi_pickup->setValue( synth_data->midi_pickup_offset*1000, dontSendNotification );

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

    filter_type_bg_button_2 = nullptr;
    button_playback_low = nullptr;
    button_playback_high = nullptr;
    button_playback_first = nullptr;
    button_playback_last = nullptr;
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
    button_tracking_mode_lm = nullptr;
    button_tracking_mode_lf = nullptr;
    button_tracking_mode_hf = nullptr;
    slider_cutoff_tracking_oct_3 = nullptr;
    slider_cutoff_tracking_oct_2 = nullptr;
    slider_cutoff_tracking_oct_1 = nullptr;
    slider_osc_tracking_oct_4 = nullptr;
    slider_osc_tracking_oct_3 = nullptr;
    slider_osc_tracking_oct_2 = nullptr;
    button_cutoff_tracking_3 = nullptr;
    button_cutoff_tracking_2 = nullptr;
    button_cutoff_tracking_1 = nullptr;
    button_osc_tracking_4 = nullptr;
    button_osc_tracking_3 = nullptr;
    button_osc_tracking_2 = nullptr;
    button_osc_tracking_1 = nullptr;
    close = nullptr;
    label_2 = nullptr;
    label_4 = nullptr;
    label_5 = nullptr;
    label_6 = nullptr;
    label_7 = nullptr;
    label_3 = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    label_10 = nullptr;
    button_osc_key_tracking_highest2 = nullptr;
    label_12 = nullptr;
    label_13 = nullptr;
    label_14 = nullptr;
    slider_midi_pickup8 = nullptr;
    slider_midi_pickup9 = nullptr;
    slider_midi_pickup10 = nullptr;
    label_15 = nullptr;
    label_16 = nullptr;
    label_17 = nullptr;
    slider_midi_pickup11 = nullptr;
    slider_midi_pickup12 = nullptr;
    slider_midi_pickup13 = nullptr;
    label_18 = nullptr;
    label_19 = nullptr;
    label_20 = nullptr;
    slider_midi_pickup14 = nullptr;
    slider_midi_pickup15 = nullptr;
    slider_midi_pickup16 = nullptr;
    label_21 = nullptr;
    label_22 = nullptr;
    label_23 = nullptr;
    label_24 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_Playback::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (15.0f, 256.0f, 570.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (181.0f, 0.0f, 539.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, 0.0f, 169.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (1141.0f, 0.0f, 310.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (721.0f, 0.0f, 209.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (931.0f, 0.0f, 209.0f, 250.0f, 10.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_Playback::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    filter_type_bg_button_2->setBounds (30, 40, 130, 130);
    button_playback_low->setBounds (30, 140, 130, 30);
    button_playback_high->setBounds (30, 107, 130, 30);
    button_playback_first->setBounds (30, 73, 130, 30);
    button_playback_last->setBounds (30, 40, 130, 30);
    button_flt_out_triggering_3->setBounds (1275, 40, 60, 30);
    button_flt_out_triggering_2->setBounds (1215, 40, 60, 30);
    button_flt_out_triggering_1->setBounds (1155, 40, 60, 30);
    button_flt_env_triggering_3->setBounds (1065, 40, 60, 30);
    button_flt_env_triggering_2->setBounds (1005, 40, 60, 30);
    button_flt_env_triggering_1->setBounds (945, 40, 60, 30);
    button_flt_input_triggering_3->setBounds (855, 40, 60, 30);
    button_flt_input_triggering_2->setBounds (795, 40, 60, 30);
    button_flt_input_triggering_1->setBounds (735, 40, 60, 30);
    filter_type_bg_button_1->setBounds (195, 40, 60, 130);
    button_tracking_mode_hm->setBounds (195, 140, 60, 30);
    button_tracking_mode_lm->setBounds (195, 107, 60, 30);
    button_tracking_mode_lf->setBounds (195, 73, 60, 30);
    button_tracking_mode_hf->setBounds (195, 40, 60, 30);
    slider_cutoff_tracking_oct_3->setBounds (645, 80, 60, 56);
    slider_cutoff_tracking_oct_2->setBounds (585, 80, 60, 56);
    slider_cutoff_tracking_oct_1->setBounds (525, 80, 60, 56);
    slider_osc_tracking_oct_4->setBounds (445, 80, 60, 56);
    slider_osc_tracking_oct_3->setBounds (385, 80, 60, 56);
    slider_osc_tracking_oct_2->setBounds (325, 80, 60, 56);
    button_cutoff_tracking_3->setBounds (645, 40, 60, 30);
    button_cutoff_tracking_2->setBounds (585, 40, 60, 30);
    button_cutoff_tracking_1->setBounds (525, 40, 60, 30);
    button_osc_tracking_4->setBounds (445, 40, 60, 30);
    button_osc_tracking_3->setBounds (385, 40, 60, 30);
    button_osc_tracking_2->setBounds (325, 40, 60, 30);
    button_osc_tracking_1->setBounds (265, 40, 60, 30);
    close->setBounds (1420, 5, 25, 25);
    label_2->setBounds (265, 0, 240, 30);
    label_4->setBounds (520, 0, 190, 30);
    label_5->setBounds (525, 140, 60, 30);
    label_6->setBounds (585, 140, 60, 30);
    label_7->setBounds (645, 140, 60, 30);
    label_3->setBounds (190, 0, 70, 30);
    label_8->setBounds (730, 0, 190, 30);
    label_9->setBounds (940, 0, 190, 30);
    label_10->setBounds (1150, 0, 190, 30);
    button_osc_key_tracking_highest2->setBounds (-622, 38, 60, 30);
    label_12->setBounds (325, 140, 60, 30);
    label_13->setBounds (385, 140, 60, 30);
    label_14->setBounds (445, 140, 60, 30);
    slider_midi_pickup8->setBounds (735, 80, 60, 56);
    slider_midi_pickup9->setBounds (795, 80, 60, 56);
    slider_midi_pickup10->setBounds (855, 80, 60, 56);
    label_15->setBounds (735, 140, 60, 30);
    label_16->setBounds (795, 140, 60, 30);
    label_17->setBounds (855, 140, 60, 30);
    slider_midi_pickup11->setBounds (945, 80, 60, 56);
    slider_midi_pickup12->setBounds (1005, 80, 60, 56);
    slider_midi_pickup13->setBounds (1065, 80, 60, 56);
    label_18->setBounds (945, 140, 60, 30);
    label_19->setBounds (1005, 140, 60, 30);
    label_20->setBounds (1065, 140, 60, 30);
    slider_midi_pickup14->setBounds (1155, 80, 60, 56);
    slider_midi_pickup15->setBounds (1215, 80, 60, 56);
    slider_midi_pickup16->setBounds (1275, 80, 60, 56);
    label_21->setBounds (1155, 140, 60, 30);
    label_22->setBounds (1215, 140, 60, 30);
    label_23->setBounds (1275, 140, 60, 30);
    label_24->setBounds (20, 0, 150, 30);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_Playback::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_playback_low)
    {
        //[UserButtonCode_button_playback_low] -- add your button handler code here..
        synth_data->play_mode = 3;
        //[/UserButtonCode_button_playback_low]
    }
    else if (buttonThatWasClicked == button_playback_high)
    {
        //[UserButtonCode_button_playback_high] -- add your button handler code here..
        synth_data->play_mode = 2;
        //[/UserButtonCode_button_playback_high]
    }
    else if (buttonThatWasClicked == button_playback_first)
    {
        //[UserButtonCode_button_playback_first] -- add your button handler code here..
        synth_data->play_mode = 1;
        //[/UserButtonCode_button_playback_first]
    }
    else if (buttonThatWasClicked == button_playback_last)
    {
        //[UserButtonCode_button_playback_last] -- add your button handler code here..
        synth_data->play_mode = 0;
        //[/UserButtonCode_button_playback_last]
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
    else if (buttonThatWasClicked == button_tracking_mode_lm)
    {
        //[UserButtonCode_button_tracking_mode_lm] -- add your button handler code here..
        synth_data->keytrack_osci_play_mode = 2;
        //[/UserButtonCode_button_tracking_mode_lm]
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
    else if (buttonThatWasClicked == button_osc_tracking_4)
    {
        //[UserButtonCode_button_osc_tracking_4] -- add your button handler code here..
        synth_data->keytrack_osci[3] ^= true;
        //[/UserButtonCode_button_osc_tracking_4]
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
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == button_osc_key_tracking_highest2)
    {
        //[UserButtonCode_button_osc_key_tracking_highest2] -- add your button handler code here..
        //[/UserButtonCode_button_osc_key_tracking_highest2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_Playback::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
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
    else if (sliderThatWasMoved == slider_osc_tracking_oct_4)
    {
        //[UserSliderCode_slider_osc_tracking_oct_4] -- add your slider handling code here..
        synth_data->keytrack_osci_octave_offset[3] = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_osc_tracking_oct_4]
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
    else if (sliderThatWasMoved == slider_midi_pickup8)
    {
        //[UserSliderCode_slider_midi_pickup8] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup8]
    }
    else if (sliderThatWasMoved == slider_midi_pickup9)
    {
        //[UserSliderCode_slider_midi_pickup9] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup9]
    }
    else if (sliderThatWasMoved == slider_midi_pickup10)
    {
        //[UserSliderCode_slider_midi_pickup10] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup10]
    }
    else if (sliderThatWasMoved == slider_midi_pickup11)
    {
        //[UserSliderCode_slider_midi_pickup11] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup11]
    }
    else if (sliderThatWasMoved == slider_midi_pickup12)
    {
        //[UserSliderCode_slider_midi_pickup12] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup12]
    }
    else if (sliderThatWasMoved == slider_midi_pickup13)
    {
        //[UserSliderCode_slider_midi_pickup13] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup13]
    }
    else if (sliderThatWasMoved == slider_midi_pickup14)
    {
        //[UserSliderCode_slider_midi_pickup14] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup14]
    }
    else if (sliderThatWasMoved == slider_midi_pickup15)
    {
        //[UserSliderCode_slider_midi_pickup15] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup15]
    }
    else if (sliderThatWasMoved == slider_midi_pickup16)
    {
        //[UserSliderCode_slider_midi_pickup16] -- add your slider handling code here..
        //[/UserSliderCode_slider_midi_pickup16]
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_Playback" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow* parent_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;original_w(1465), &#10;original_h(180)"
                 snapPixels="5" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="15 256 570 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="181 0 539 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="10 0 169 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1141 0 310 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="721 0 209 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="931 0 209 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
  </BACKGROUND>
  <TEXTBUTTON name="" id="6ffb804a65970830" memberName="filter_type_bg_button_2"
              virtualName="" explicitFocusOrder="0" pos="30 40 130 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="66aa09006ca0dc04" memberName="button_playback_low"
              virtualName="" explicitFocusOrder="0" pos="30 140 130 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="REPLACE LOW"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="19270265aa971673" memberName="button_playback_high"
              virtualName="" explicitFocusOrder="0" pos="30 107 130 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="REPLACE HIGH"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9a963a96fab38846" memberName="button_playback_first"
              virtualName="" explicitFocusOrder="0" pos="30 73 130 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="FIFO"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fcc4de6ce6cd0d47" memberName="button_playback_last"
              virtualName="" explicitFocusOrder="0" pos="30 40 130 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LIFO"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="98903a957525eccb" memberName="button_flt_out_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="1275 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="40d5725e65405de1" memberName="button_flt_out_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="1215 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="58d6c148c398eb2c" memberName="button_flt_out_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="1155 40 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="44d00292596c1d28" memberName="button_flt_env_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="1065 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2378bbfb84f1ab97" memberName="button_flt_env_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="1005 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="3c2bd1d338499dd" memberName="button_flt_env_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="945 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6be8f0e439149b1c" memberName="button_flt_input_triggering_3"
              virtualName="" explicitFocusOrder="0" pos="855 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b408d83bfb8e45f6" memberName="button_flt_input_triggering_2"
              virtualName="" explicitFocusOrder="0" pos="795 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="dc48f03c31f28f31" memberName="button_flt_input_triggering_1"
              virtualName="" explicitFocusOrder="0" pos="735 40 60 30" bgColOff="ff000000"
              bgColOn="ff4444ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="40809d639d290176" memberName="filter_type_bg_button_1"
              virtualName="" explicitFocusOrder="0" pos="195 40 60 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fd80a890cf07a64" memberName="button_tracking_mode_hm"
              virtualName="" explicitFocusOrder="0" pos="195 140 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="HM"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ccd73db87cfab50b" memberName="button_tracking_mode_lm"
              virtualName="" explicitFocusOrder="0" pos="195 107 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LM"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f4c678f3f0e9f6d5" memberName="button_tracking_mode_lf"
              virtualName="" explicitFocusOrder="0" pos="195 73 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LF"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f463c263e3c8841d" memberName="button_tracking_mode_hf"
              virtualName="" explicitFocusOrder="0" pos="195 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="HF"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="0" id="af7cd66c376dd8af" memberName="slider_cutoff_tracking_oct_3"
          virtualName="Slider" explicitFocusOrder="0" pos="645 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="eaeb1952f52d40f1" memberName="slider_cutoff_tracking_oct_2"
          virtualName="Slider" explicitFocusOrder="0" pos="585 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_cutoff_tracking_oct_1"
          virtualName="Slider" explicitFocusOrder="0" pos="525 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-4" max="4" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="3d6cbeb9598716ae" memberName="slider_osc_tracking_oct_4"
          virtualName="Slider" explicitFocusOrder="0" pos="445 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-2" max="2" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="766b2badd17965f3" memberName="slider_osc_tracking_oct_3"
          virtualName="Slider" explicitFocusOrder="0" pos="385 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-2" max="2" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="b2d956b7def64e52" memberName="slider_osc_tracking_oct_2"
          virtualName="Slider" explicitFocusOrder="0" pos="325 80 60 56"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="-2" max="2" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="6a3e54bcd6cd51b" memberName="button_cutoff_tracking_3"
              virtualName="" explicitFocusOrder="0" pos="645 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9aa0dc1b0f793710" memberName="button_cutoff_tracking_2"
              virtualName="" explicitFocusOrder="0" pos="585 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d11a466dd34a172" memberName="button_cutoff_tracking_1"
              virtualName="" explicitFocusOrder="0" pos="525 40 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8d305367c3021538" memberName="button_osc_tracking_4"
              virtualName="" explicitFocusOrder="0" pos="445 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="FM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2194df5e065a174a" memberName="button_osc_tracking_3"
              virtualName="" explicitFocusOrder="0" pos="385 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="702de62630ddb999" memberName="button_osc_tracking_2"
              virtualName="" explicitFocusOrder="0" pos="325 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7a3b545ab65b9c5c" memberName="button_osc_tracking_1"
              virtualName="" explicitFocusOrder="0" pos="265 40 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="OSC 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b6a23ff465364b08" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="1420 5 25 25" tooltip="Close MIDI settings."
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="59fe62ccfd0a3d95" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="265 0 240 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC KEY-TRACKING"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="fb7b01265de6cb01" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="520 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CUTOFF KEY-TRACKING"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="83516a58e629f75e" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="525 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="f66dfa6aa4e89192" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="585 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1a3d269596c5fd4b" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="645 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="ad0fa09eeb91d953" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="190 0 70 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="MODE&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="33"/>
  <LABEL name="" id="ec3600d0ff1f41b7" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="730 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT INPUT KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="4e728fb8882f8240" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="940 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT ENV KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="c9659e86997ec2bc" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="1150 0 190 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT OUTPUT KEY-TRIG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="30ce1d751bea6be8" memberName="button_osc_key_tracking_highest2"
              virtualName="" explicitFocusOrder="0" pos="-622 38 60 30" bgColOff="ff000000"
              bgColOn="ffff1111" textCol="ffff3b00" textColOn="ffffff00" buttonText="LF"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="47badf2084c5bcb1" memberName="label_12" virtualName=""
         explicitFocusOrder="0" pos="325 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="76dfa6e205a722e0" memberName="label_13" virtualName=""
         explicitFocusOrder="0" pos="385 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="fa6cb20d0fcb31d4" memberName="label_14" virtualName=""
         explicitFocusOrder="0" pos="445 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OCT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="67ee7cffd3beff61" memberName="slider_midi_pickup8"
          virtualName="Slider" explicitFocusOrder="0" pos="735 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="581f1669eb5feb67" memberName="slider_midi_pickup9"
          virtualName="Slider" explicitFocusOrder="0" pos="795 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="94c3bbc1e201810e" memberName="slider_midi_pickup10"
          virtualName="Slider" explicitFocusOrder="0" pos="855 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="c40622ad9e625e0c" memberName="label_15" virtualName=""
         explicitFocusOrder="0" pos="735 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="5f27f53125ce5bbe" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="795 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="da394749244c9a80" memberName="label_17" virtualName=""
         explicitFocusOrder="0" pos="855 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="204da1ca701610e1" memberName="slider_midi_pickup11"
          virtualName="Slider" explicitFocusOrder="0" pos="945 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="9a68e3c50e07cf7f" memberName="slider_midi_pickup12"
          virtualName="Slider" explicitFocusOrder="0" pos="1005 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="e6fa08119a3fa0b6" memberName="slider_midi_pickup13"
          virtualName="Slider" explicitFocusOrder="0" pos="1065 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="81daa22c59f3b11c" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="945 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="c9839b26de95b8eb" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="1005 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="4a331aaa8624fcfe" memberName="label_20" virtualName=""
         explicitFocusOrder="0" pos="1065 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="1a1d7f6c2ca843ef" memberName="slider_midi_pickup14"
          virtualName="Slider" explicitFocusOrder="0" pos="1155 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="7fe963dd2889e887" memberName="slider_midi_pickup15"
          virtualName="Slider" explicitFocusOrder="0" pos="1215 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="821606ba1a813d43" memberName="slider_midi_pickup16"
          virtualName="Slider" explicitFocusOrder="0" pos="1275 80 60 56"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="83734337ebb799ae" memberName="label_21" virtualName=""
         explicitFocusOrder="0" pos="1155 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="47a727c85872ccd8" memberName="label_22" virtualName=""
         explicitFocusOrder="0" pos="1215 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1aaee1ac83e64c9c" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="1275 140 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="b53da3238c135f98" memberName="label_24" virtualName=""
         explicitFocusOrder="0" pos="20 0 150 30" textCol="ffff3b00" edTextCol="ffff3b00"
         edBkgCol="0" labelText="PLAY MODES" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="30"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
