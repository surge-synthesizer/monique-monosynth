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
#include "monique_core_Processor.h"
#include "monique_core_Synth.h"

#include "monique_ui_LookAndFeel.h"
#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_AmpPainter.h"
#include "monique_ui_ModulationSlider.h"
#include "monique_ui_SliderConfig.h"
#include "monique_ui_MIDIIO.h"
#include "monique_ui_GlobalSettings.h"
#include "monique_ui_Morph.h"
#include "monique_ui_MidiLearnPopup.h"
#include "monique_ui_ENVPopup.h"
#include "monique_ui_OptionPopup.h"
//[/Headers]

#include "monique_ui_MainWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


//==============================================================================
//==============================================================================

void Monique_Ui_Mainwindow::refresh() noexcept
{
    show_current_voice_data();
    resize_sequence_buttons();
    show_programs_and_select(false);
    show_ctrl_state();
}
void Monique_Ui_Mainwindow::update_tooltip_handling( bool is_help_key_down_ ) noexcept
{
    // TOOLTIP TIME
    if( synth_data->show_tooltips )
    {
        if( tooltipWindow == nullptr )
        {
            tooltipWindow = new TooltipWindow( this, is_help_key_down_ ? 30 : 1000 );
        }
        else if( is_help_key_down_ )
        {
            tooltipWindow->setMillisecondsBeforeTipAppears(30);
        }
        else if( not is_help_key_down_ )
        {
            tooltipWindow->setMillisecondsBeforeTipAppears(1000);
        }
    }
    else
    {
        if( is_help_key_down_ && tooltipWindow == nullptr )
        {
            tooltipWindow = new TooltipWindow( this, 30 );
        }
        else if( not is_help_key_down_ and tooltipWindow )
        {
            tooltipWindow = nullptr;
        }
    }
}
void Monique_Ui_Mainwindow::handleAsyncUpdate()
{
    show_programs_and_select(true);
}
void Monique_Ui_Mainwindow::show_programs_and_select(bool force)
{
    const int current_bank = synth_data->get_current_bank();
    const int current_programm = synth_data->get_current_program();
    if( force || current_bank != last_bank || current_programm != last_programm )
    {
        last_bank = current_bank;
        last_programm = current_programm;

        combo_bank->clear(dontSendNotification);
        combo_bank->addItemList(synth_data->get_banks(),1);
        combo_bank->setSelectedId(synth_data->get_current_bank()+1,dontSendNotification);

        combo_programm->clear(dontSendNotification);
        combo_programm->addItemList(synth_data->get_current_bank_programms(),1);
        // int programm_id = synth_data->get_current_program();
        /*
        if( programm_id >= 0 )
               {
                   combo_programm->setSelectedItemIndex(programm_id,dontSendNotification);
               }
               else
               {
          */
        combo_programm->setText(synth_data->alternative_program_name,dontSendNotification);
        //}
    }
}
void Monique_Ui_Mainwindow::show_ctrl_state()
{
    if( last_ctrl_mode != synth_data->ctrl )
    {
        last_ctrl_mode = synth_data->ctrl;
        for( int i = 0 ; i < getNumChildComponents() ; ++i )
        {
            if( Monique_Ui_DualSlider* mod_slider = dynamic_cast< Monique_Ui_DualSlider* >( getChildComponent(i) ) )
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
void Monique_Ui_Mainwindow::show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_ )
{
    popup = nullptr;
    if( midi_control_handler->is_learning() && midi_conrtrol_ )
    {
        addAndMakeVisible( popup = new Monique_Ui_MainwindowPopup(ui_refresher,this,midi_conrtrol_) );
        popup->set_element_to_show( comp_ );
	resize_subeditors();
    }
}
void Monique_Ui_Mainwindow::show_current_voice_data()
{
    keyboard->setRootNote( 60+synth_data->note_offset.get_value()-24 );

    ComponentColours& colours = look_and_feel->colours;
    Colour button_on = colours.button_on_colour;
    Colour button_off = colours.button_off_colour;

    // FILTER 1
    int f_type = synth_data->filter_datas[0]->filter_type;
    filter_type_6_1->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_1->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_3_1->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_1->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // FILTER 2
    f_type = synth_data->filter_datas[1]->filter_type;
    filter_type_6_2->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_2->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_3_2->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_2->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // FILTER 3
    f_type = synth_data->filter_datas[2]->filter_type;
    filter_type_6_3->setColour( TextButton::buttonColourId, f_type == LPF || f_type == LPF_2_PASS ? button_on : button_off );
    filter_type_2_3->setColour( TextButton::buttonColourId, f_type == HPF || f_type == HIGH_2_PASS ? button_on : button_off );
    filter_type_3_3->setColour( TextButton::buttonColourId, f_type == BPF ? button_on : button_off );
    filter_type_5_3->setColour( TextButton::buttonColourId, f_type == PASS ? button_on : button_off );

    // SEQUENCE
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

    button_ctrl_toggle->setColour( TextButton::buttonColourId, synth_data->ctrl ? Colours::red : button_off );

    // EDITORS
#ifdef IS_STANDALONE
    if( flash_counter > 0 )
    {
        flash_counter--;
        button_open_midi_io_settings->setColour( TextButton::buttonColourId, Colours::orange.darker() );
    }
    else
    {
        button_open_midi_io_settings->setColour( TextButton::buttonColourId, editor_midiio ? Colours::lightblue : button_off );
    }
#else
    button_open_midi_io_settings->setColour( TextButton::buttonColourId, midi_control_handler->is_waiting_for_param() ? Colours::red : midi_control_handler->is_learning() ? Colours::red : colours.button_off_colour );
#endif
    button_open_morph->setColour( TextButton::buttonColourId, editor_morph ? Colours::lightblue:button_off );
    button_open_oszi->setColour( TextButton::buttonColourId, amp_painter ? Colours::lightblue : button_off );
    button_open_config2->setColour( TextButton::buttonColourId, editor_global_settings ? Colours::lightblue:button_off );

    button_values_toggle->setColour( TextButton::buttonColourId, look_and_feel->show_values_always ? Colours::lightblue : button_off );

    // ACTIVE FILTER INPUTS
    {
        const FilterData*const filter_1_data = synth_data->filter_datas[0];
        const FilterData*const filter_2_data = synth_data->filter_datas[1];
        const FilterData*const filter_3_data = synth_data->filter_datas[2];

        bool last_filter_1_1_sustain = filter_1_data->input_sustains[0] > 0 ? 1 : 0;
        bool last_filter_1_2_sustain = filter_1_data->input_sustains[1] > 0 ? 1 : 0;
        bool last_filter_1_3_sustain = filter_1_data->input_sustains[2] > 0 ? 1 : 0;

        bool last_filter_2_1_sustain = filter_2_data->input_sustains[0] < 0 ? true : (filter_2_data->input_sustains[0] > 0) and last_filter_1_1_sustain;
        bool last_filter_2_2_sustain = filter_2_data->input_sustains[1] < 0 ? true : (filter_2_data->input_sustains[1] > 0) and last_filter_1_2_sustain;
        bool last_filter_2_3_sustain = filter_2_data->input_sustains[2] < 0 ? true : (filter_2_data->input_sustains[2] > 0) and last_filter_1_3_sustain;

        // FILTER 2
        {
            float filter_2_1_sustain = filter_2_data->input_sustains[0];
            float filter_2_2_sustain = filter_2_data->input_sustains[1];
            float filter_2_3_sustain = filter_2_data->input_sustains[2];

            // FILTER 2 INPUT 1
            if( filter_2_1_sustain > 0 )
            {
                button_show_active_input_l_2_1->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_1_1_sustain )
                {
                    button_show_active_input_r_2_1->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_2_1->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_2_1_sustain < 0 )
            {
                button_show_active_input_l_2_1->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_2_1->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_2_1->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_2_1->setColour( TextButton::buttonColourId, button_off );
            }

            // FILTER 2 INPUT 2
            if( filter_2_2_sustain > 0 )
            {
                button_show_active_input_l_2_2->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_1_2_sustain )
                {
                    button_show_active_input_r_2_2->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_2_2->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_2_2_sustain < 0 )
            {
                button_show_active_input_l_2_2->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_2_2->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_2_2->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_2_2->setColour( TextButton::buttonColourId, button_off );
            }

            // FILTER 2 INPUT 3
            if( filter_2_3_sustain > 0 )
            {
                button_show_active_input_l_2_3->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_1_3_sustain )
                {
                    button_show_active_input_r_2_3->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_2_3->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_2_3_sustain < 0 )
            {
                button_show_active_input_l_2_3->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_2_3->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_2_3->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_2_3->setColour( TextButton::buttonColourId, button_off );
            }
        }

        // FILTER 3
        {
            float filter_3_1_sustain = filter_3_data->input_sustains[0];
            float filter_3_2_sustain = filter_3_data->input_sustains[1];
            float filter_3_3_sustain = filter_3_data->input_sustains[2];

            // FILTER 3 INPUT 1
            if( filter_3_1_sustain > 0 )
            {
                button_show_active_input_l_3_1->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_2_1_sustain )
                {
                    button_show_active_input_r_3_1->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_3_1->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_3_1_sustain < 0 )
            {
                button_show_active_input_l_3_1->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_3_1->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_3_1->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_3_1->setColour( TextButton::buttonColourId, button_off );
            }

            // FILTER 3 INPUT 2
            if( filter_3_2_sustain > 0  )
            {
                button_show_active_input_l_3_2->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_2_2_sustain )
                {
                    button_show_active_input_r_3_2->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_3_2->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_3_2_sustain < 0 )
            {
                button_show_active_input_l_3_2->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_3_2->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_3_2->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_3_2->setColour( TextButton::buttonColourId, button_off );
            }

            // FILTER 3 INPUT 3
            if( filter_3_3_sustain > 0 )
            {
                button_show_active_input_l_3_3->setColour( TextButton::buttonColourId, button_off );
                if( last_filter_2_3_sustain )
                {
                    button_show_active_input_r_3_3->setColour( TextButton::buttonColourId, button_on );
                }
                else
                {
                    button_show_active_input_r_3_3->setColour( TextButton::buttonColourId, Colours::red );
                }
            }
            else if( filter_3_3_sustain < 0 )
            {
                button_show_active_input_l_3_3->setColour( TextButton::buttonColourId, button_on );
                button_show_active_input_r_3_3->setColour( TextButton::buttonColourId, button_off );
            }
            else
            {
                button_show_active_input_l_3_3->setColour( TextButton::buttonColourId, button_off );
                button_show_active_input_r_3_3->setColour( TextButton::buttonColourId, button_off );
            }
        }
    }
}

void Monique_Ui_Mainwindow::resize_sequence_buttons()
{
    const float width_factor = 1.0f/original_w*getWidth();
    const float height_factor = 1.0f/original_h*getHeight();
    float shuffle = mono_floor(60.0 * width_factor * ArpSequencerData::shuffle_to_value( synth_data->arp_sequencer_data->shuffle ));
    if( shuffle != last_shuffle )
    {
        const int use_shuffle = mono_floor(60.0*width_factor+shuffle);
        last_shuffle = shuffle;
        for( int i = 0 ; i != sequence_buttons.size() ; ++i )
        {
            TextButton* button = sequence_buttons[i];
            if( i % 4 == 0 )
            {
                button->setSize( use_shuffle, 20.0f * height_factor );
            }
            else if( i % 2 == 0  )
            {
                button->setSize( use_shuffle, 20.0f * height_factor );
            }
            else
            {
                button->setBounds
                (
                    float(sequence_buttons[i-1]->getX()+use_shuffle),
                    float(button->getY()),
                    60.0f*width_factor-shuffle,
                    20.0f * height_factor
                );
            }
        }
    }
}
void Monique_Ui_Mainwindow::switch_finalizer_tab()
{
    //reverb
    bool state_switch = eq_1->isVisible();
    reverb_room->setVisible( state_switch );
    reverb_dry->setVisible( state_switch );
    reverb_width->setVisible( state_switch );
    delay2->setVisible( state_switch );
    chorus_modulation->setVisible( state_switch );
    bypass->setVisible( state_switch );
    label_reverb->setVisible( state_switch );
    label_shape->setVisible( state_switch );
    label_delay->setVisible( state_switch );
    label_chorus->setVisible( state_switch );
    label_fx_mix->setVisible( state_switch );
    button_edit_env_chorus->setVisible( state_switch );
    distortion->setVisible( state_switch );

    //eg
    label_effect_hider->setVisible( !state_switch );
    label_eq->setVisible( !state_switch );
    colour->setVisible( !state_switch );
    eq_1->setVisible( !state_switch );
    eq_2->setVisible( !state_switch );
    eq_3->setVisible( !state_switch );
    eq_4->setVisible( !state_switch );
    eq_5->setVisible( !state_switch );
    eq_6->setVisible( !state_switch );
    eq_7->setVisible( !state_switch );
    label_band_hz_1->setVisible( !state_switch );
    label_band_hz_2->setVisible( !state_switch );
    label_band_hz_3->setVisible( !state_switch );
    label_band_hz_4->setVisible( !state_switch );
    label_band_hz_5->setVisible( !state_switch );
    label_band_hz_6->setVisible( !state_switch );
    label_band_hz_7->setVisible( !state_switch );
    button_edit_input_env_band_1->setVisible( !state_switch );
    button_edit_input_env_band_2->setVisible( !state_switch );
    button_edit_input_env_band_3->setVisible( !state_switch );
    button_edit_input_env_band_4->setVisible( !state_switch );
    button_edit_input_env_band_5->setVisible( !state_switch );
    button_edit_input_env_band_6->setVisible( !state_switch );
    button_edit_input_env_band_7->setVisible( !state_switch );

    !state_switch ? effect_finalizer_switch->setButtonText ("E Q") : effect_finalizer_switch->setButtonText ("F X");
}
static inline void update_slider_handling_( MoniqueSynthData*synth_data_, Component*parent_ )
{
    const bool is_in_rotary_mode = synth_data_->sliders_in_rotary_mode;
    const int sensitivity = synth_data_->sliders_sensitivity;
    for( int i = 0 ; i != parent_->getNumChildComponents() ; ++i )
    {
        if( Slider*const slider = dynamic_cast< Slider* >( parent_->getChildComponent(i) ) )
        {
            if( slider->getSliderStyle() == Slider::Rotary or slider->getSliderStyle() == Slider::RotaryHorizontalVerticalDrag)
            {
                if( is_in_rotary_mode )
                {
                    slider->setSliderStyle (Slider::Rotary);
                }
                else
                {
                    slider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
                }
                slider->setMouseDragSensitivity( sensitivity );
            }
        }
        else
        {
            // RECURSIVE
            update_slider_handling_( synth_data_, parent_->getChildComponent(i) );
        }
    }
}

void Monique_Ui_Mainwindow::update_slider_handling()
{
    update_slider_handling_( synth_data, this );
}
/*
void Monique_Ui_Mainwindow::update_slider_return_values()
{
    for( int i = 0 ; i != dual_sliders.size() ; ++i )
    {
        Monique_Ui_DualSlider*slider = dual_sliders.getUnchecked(i);
        slider->update_return_values();
    }

}
*/
void Monique_Ui_Mainwindow::update_size()
{
    float ui_scale_factor = synth_data->ui_scale_factor;
    const Desktop::Displays::Display& main_display( Desktop::getInstance().getDisplays().getMainDisplay() );
    const int main_display_h = main_display.userArea.getHeight();
    const int main_display_w = main_display.userArea.getWidth();
    int use_height = original_h*ui_scale_factor;
    int use_width = original_w*ui_scale_factor;

    if( main_display_h < use_height )
    {
        use_height = main_display_h * 0.9;
        float new_scale = 1.0f/original_h*use_height;
        use_width = original_w*new_scale;
    }
    if( main_display_w < use_width )
    {
        use_width = main_display_w * 0.9;
        float new_scale = 1.0f/original_w*use_width;
        use_height = original_h*new_scale;
    }

    setSize(use_width,use_height);
}
void Monique_Ui_Mainwindow::sliderClicked (Slider*s_)
{
    if( midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning() )
        sliderValueChanged(s_);
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_Mainwindow::Monique_Ui_Mainwindow (Monique_Ui_Refresher*ui_refresher_)
    : Monique_Ui_Refreshable(ui_refresher_),
      AudioProcessorEditor(ui_refresher_->audio_processor),
      original_w(1465), original_h(1235)
{
    //[Constructor_pre] You can add your own custom stuff here..
    audio_processor = reinterpret_cast< MoniqueAudioProcessor* >( &processor );
    amp_painter = nullptr;
    is_ctrl_down = false;
    flash_counter = 0;
    program_edit_type = NOT_SET;
    ui_refresher_->editor = this;
    //[/Constructor_pre]

    addAndMakeVisible (distortion = new Monique_Ui_DualSlider (ui_refresher,
                                                               new FXDistortionSlConfig(synth_data)));

    addAndMakeVisible (reverb_room = new Monique_Ui_DualSlider (ui_refresher,
                                                                new RRoomSlConfig(synth_data)));

    addAndMakeVisible (reverb_width = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new RWidthSlConfig(synth_data)));

    addAndMakeVisible (reverb_dry = new Monique_Ui_DualSlider (ui_refresher,
                                                               new RDrySlConfig(synth_data)));

    addAndMakeVisible (bypass = new Monique_Ui_DualSlider (ui_refresher,
                                                           new BypassConfig(synth_data)));

    addAndMakeVisible (eq_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,0)));

    addAndMakeVisible (eq_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,2)));

    addAndMakeVisible (eq_4 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,3)));

    addAndMakeVisible (eq_5 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,4)));

    addAndMakeVisible (eq_6 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,5)));

    addAndMakeVisible (eq_7 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,6)));

    addAndMakeVisible (colour = new Monique_Ui_DualSlider (ui_refresher,
                                                           new FColourSlConfig(synth_data)));

    addAndMakeVisible (delay2 = new Monique_Ui_DualSlider (ui_refresher,
                                                           new DelaySlConfig(synth_data)));

    addAndMakeVisible (chorus_modulation = new Monique_Ui_DualSlider (ui_refresher,
                                                                      new CModSlConfig(synth_data)));

    addAndMakeVisible (eq_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                         new EQSlConfig(synth_data,1)));

    addAndMakeVisible (label_effect_hider = new Label (String::empty,
                                                       String::empty));
    label_effect_hider->setFont (Font (30.00f, Font::plain));
    label_effect_hider->setJustificationType (Justification::centredLeft);
    label_effect_hider->setEditable (false, false, false);
    label_effect_hider->setColour (Label::textColourId, Colour (0xffff3b00));
    label_effect_hider->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_effect_hider->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_2 = new Label ("DL",
                                                    TRANS("160Hz")));
    label_band_hz_2->setFont (Font (30.00f, Font::plain));
    label_band_hz_2->setJustificationType (Justification::centred);
    label_band_hz_2->setEditable (false, false, false);
    label_band_hz_2->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_3 = new Label ("DL",
                                                    TRANS("300Hz")));
    label_band_hz_3->setFont (Font (30.00f, Font::plain));
    label_band_hz_3->setJustificationType (Justification::centred);
    label_band_hz_3->setEditable (false, false, false);
    label_band_hz_3->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_4 = new Label ("DL",
                                                    TRANS("600Hz")));
    label_band_hz_4->setFont (Font (30.00f, Font::plain));
    label_band_hz_4->setJustificationType (Justification::centred);
    label_band_hz_4->setEditable (false, false, false);
    label_band_hz_4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_5 = new Label ("DL",
                                                    TRANS("1.2kHz")));
    label_band_hz_5->setFont (Font (30.00f, Font::plain));
    label_band_hz_5->setJustificationType (Justification::centred);
    label_band_hz_5->setEditable (false, false, false);
    label_band_hz_5->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_6 = new Label ("DL",
                                                    TRANS("2.5kHz")));
    label_band_hz_6->setFont (Font (30.00f, Font::plain));
    label_band_hz_6->setJustificationType (Justification::centred);
    label_band_hz_6->setEditable (false, false, false);
    label_band_hz_6->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_7 = new Label ("DL",
                                                    TRANS(">2.5kHz")));
    label_band_hz_7->setFont (Font (30.00f, Font::plain));
    label_band_hz_7->setJustificationType (Justification::centred);
    label_band_hz_7->setEditable (false, false, false);
    label_band_hz_7->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_1 = new Label ("DL",
                                                    TRANS("<80Hz")));
    label_band_hz_1->setFont (Font (30.00f, Font::plain));
    label_band_hz_1->setJustificationType (Justification::centred);
    label_band_hz_1->setEditable (false, false, false);
    label_band_hz_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_band_hz_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (speed_multi = new Monique_Ui_DualSlider (ui_refresher,
                                                                new SpeedMultiSlConfig(synth_data)));

    addAndMakeVisible (morpher_4 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new MorphSLConfig(synth_data,3)));

    addAndMakeVisible (morpher_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new MorphSLConfig(synth_data,2)));

    addAndMakeVisible (morpher_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new MorphSLConfig(synth_data,1)));

    addAndMakeVisible (morpher_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new MorphSLConfig(synth_data,0)));

    addAndMakeVisible (osc_wave_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new WAVESlConfig(synth_data,2)));

    addAndMakeVisible (keyboard = new MidiKeyboardComponent (*reinterpret_cast< MoniqueAudioProcessor* >( &processor ), MidiKeyboardComponent::horizontalKeyboard));

    addAndMakeVisible (glide2 = new Monique_Ui_DualSlider (ui_refresher,
                                                           new GlideConfig(synth_data)));

    addAndMakeVisible (arp_step_16 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,15)));

    addAndMakeVisible (arp_step_15 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,14)));

    addAndMakeVisible (arp_step_14 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,13)));

    addAndMakeVisible (arp_step_13 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,12)));

    addAndMakeVisible (arp_step_12 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,11)));

    addAndMakeVisible (arp_step_11 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,10)));

    addAndMakeVisible (arp_step_10 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new ArpStepSlConfig(synth_data,9)));

    addAndMakeVisible (arp_step_9 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,8)));

    addAndMakeVisible (arp_step_8 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,7)));

    addAndMakeVisible (arp_step_7 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,6)));

    addAndMakeVisible (arp_step_6 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,5)));

    addAndMakeVisible (arp_step_5 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,4)));

    addAndMakeVisible (arp_step_4 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,3)));

    addAndMakeVisible (arp_step_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,2)));

    addAndMakeVisible (arp_step_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,1)));

    addAndMakeVisible (arp_step_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new ArpStepSlConfig(synth_data,0)));

    addAndMakeVisible (shuffle = new Monique_Ui_DualSlider (ui_refresher,
                                                            new ShuffleConfig(synth_data)));

    addAndMakeVisible (flt_sustain_4 = new Monique_Ui_DualSlider (ui_refresher, new FSustainSlConfig(synth_data)));

    addAndMakeVisible (flt_decay_4 = new Monique_Ui_DualSlider (ui_refresher, new FDecaySlConfig(synth_data)
                                                                ));

    addAndMakeVisible (flt_attack_4 = new Monique_Ui_DualSlider (ui_refresher, new FAttackSlConfig(synth_data)));

    addAndMakeVisible (flt_release_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FReleaseSlConfig(synth_data,2)));

    addAndMakeVisible (flt_sustain_time_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                       new FSustainTimeSlConfig(synth_data,2)));

    addAndMakeVisible (flt_sustain_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FSustainSlConfig(synth_data,2)));

    addAndMakeVisible (flt_decay_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FDecaySlConfig(synth_data,2)));

    addAndMakeVisible (flt_attack_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FAttackSlConfig(synth_data,2)));

    addAndMakeVisible (flt_release_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FReleaseSlConfig(synth_data,1)));

    addAndMakeVisible (flt_sustain_time_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                       new FSustainTimeSlConfig(synth_data,1)));

    addAndMakeVisible (flt_sustain_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FSustainSlConfig(synth_data,1)));

    addAndMakeVisible (flt_decay_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FDecaySlConfig(synth_data,1)));

    addAndMakeVisible (flt_attack_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FAttackSlConfig(synth_data,1)));

    addAndMakeVisible (flt_release_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FReleaseSlConfig(synth_data,0)));

    addAndMakeVisible (flt_sustain_time_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                       new FSustainTimeSlConfig(synth_data,0)));

    addAndMakeVisible (flt_sustain_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new FSustainSlConfig(synth_data,0)));

    addAndMakeVisible (flt_decay_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FDecaySlConfig(synth_data,0)));

    addAndMakeVisible (flt_attack_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FAttackSlConfig(synth_data,0)));

    addAndMakeVisible (label_monolisa = new Label (String::empty,
                                                   TRANS("M O N I Q U E")));
    label_monolisa->setFont (Font (30.00f, Font::plain));
    label_monolisa->setJustificationType (Justification::centred);
    label_monolisa->setEditable (false, false, false);
    label_monolisa->setColour (Label::textColourId, Colour (0xffff3b00));
    label_monolisa->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_monolisa->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filter_type_2_1 = new TextButton ("VOICE 1"));
    filter_type_2_1->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_1->setButtonText (TRANS("HP"));
    filter_type_2_1->addListener (this);
    filter_type_2_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_1 = new TextButton ("VOICE 1"));
    filter_type_3_1->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_1->setButtonText (TRANS("BAND"));
    filter_type_3_1->addListener (this);
    filter_type_3_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_2 = new TextButton ("VOICE 1"));
    filter_type_2_2->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_2->setButtonText (TRANS("HP"));
    filter_type_2_2->addListener (this);
    filter_type_2_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_2 = new TextButton ("VOICE 1"));
    filter_type_3_2->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_2->setButtonText (TRANS("BAND"));
    filter_type_3_2->addListener (this);
    filter_type_3_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_3 = new TextButton ("VOICE 1"));
    filter_type_2_3->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_3->setButtonText (TRANS("HP"));
    filter_type_2_3->addListener (this);
    filter_type_2_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_3 = new TextButton ("VOICE 1"));
    filter_type_3_3->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_3->setButtonText (TRANS("BAND"));
    filter_type_3_3->addListener (this);
    filter_type_3_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_1 = new TextButton ("VOICE 1"));
    filter_type_5_1->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_1->setButtonText (TRANS("PASS"));
    filter_type_5_1->addListener (this);
    filter_type_5_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_2 = new TextButton ("VOICE 1"));
    filter_type_5_2->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_2->setButtonText (TRANS("PASS"));
    filter_type_5_2->addListener (this);
    filter_type_5_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_3 = new TextButton ("VOICE 1"));
    filter_type_5_3->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_3->setButtonText (TRANS("PASS"));
    filter_type_5_3->addListener (this);
    filter_type_5_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_2 = new TextButton (String::empty));
    button_sequence_2->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_2->addListener (this);
    button_sequence_2->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_3 = new TextButton (String::empty));
    button_sequence_3->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_3->addListener (this);
    button_sequence_3->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_4 = new TextButton (String::empty));
    button_sequence_4->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_4->addListener (this);
    button_sequence_4->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_5 = new TextButton (String::empty));
    button_sequence_5->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_5->addListener (this);
    button_sequence_5->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_5->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_5->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_6 = new TextButton (String::empty));
    button_sequence_6->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_6->addListener (this);
    button_sequence_6->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_6->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_6->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_7 = new TextButton (String::empty));
    button_sequence_7->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_7->addListener (this);
    button_sequence_7->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_7->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_7->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_8 = new TextButton (String::empty));
    button_sequence_8->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_8->addListener (this);
    button_sequence_8->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_8->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_8->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_9 = new TextButton (String::empty));
    button_sequence_9->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_9->addListener (this);
    button_sequence_9->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_9->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_9->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_10 = new TextButton (String::empty));
    button_sequence_10->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_10->addListener (this);
    button_sequence_10->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_10->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_10->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_11 = new TextButton (String::empty));
    button_sequence_11->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_11->addListener (this);
    button_sequence_11->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_11->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_11->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_12 = new TextButton (String::empty));
    button_sequence_12->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_12->addListener (this);
    button_sequence_12->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_12->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_12->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_13 = new TextButton (String::empty));
    button_sequence_13->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_13->addListener (this);
    button_sequence_13->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_13->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_13->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_14 = new TextButton (String::empty));
    button_sequence_14->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_14->addListener (this);
    button_sequence_14->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_14->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_14->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_15 = new TextButton (String::empty));
    button_sequence_15->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_15->addListener (this);
    button_sequence_15->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_15->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_15->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_sequence_16 = new TextButton (String::empty));
    button_sequence_16->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_16->addListener (this);
    button_sequence_16->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_16->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_16->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (combo_programm = new ComboBox (String::empty));
    combo_programm->setTooltip (TRANS("Select and load a program of the selected bank (one box left)."));
    combo_programm->setEditableText (true);
    combo_programm->setJustificationType (Justification::centredLeft);
    combo_programm->setTextWhenNothingSelected (TRANS("DEFAULT"));
    combo_programm->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_programm->addListener (this);

    addAndMakeVisible (button_programm_left = new TextButton (String::empty));
    button_programm_left->setTooltip (TRANS("Load the program before of the selected bank (right of this button)."));
    button_programm_left->setButtonText (TRANS("<"));
    button_programm_left->addListener (this);
    button_programm_left->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_left->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_left->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_right = new TextButton (String::empty));
    button_programm_right->setTooltip (TRANS("Load the next program of the selected bank."));
    button_programm_right->setButtonText (TRANS(">"));
    button_programm_right->addListener (this);
    button_programm_right->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_right->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_right->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_replace = new TextButton (String::empty));
    button_programm_replace->setTooltip (TRANS("Replaces the selected program."));
    button_programm_replace->setButtonText (TRANS("REPLACE"));
    button_programm_replace->addListener (this);
    button_programm_replace->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_replace->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_replace->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_new = new TextButton (String::empty));
    button_programm_new->setTooltip (TRANS("Create a new program from the current state."));
    button_programm_new->setButtonText (TRANS("SAVE AS"));
    button_programm_new->addListener (this);
    button_programm_new->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_new->setColour (TextButton::textColourOnId, Colour (0xffbcff00));
    button_programm_new->setColour (TextButton::textColourOffId, Colour (0xffd0ff00));

    addAndMakeVisible (button_open_oszi = new TextButton (String::empty));
    button_open_oszi->setTooltip (TRANS("Open/Close the oscilloscope.\n"
    "\n"
    "Note: press ESC to close editors."));
    button_open_oszi->setButtonText (TRANS("OSCI"));
    button_open_oszi->addListener (this);
    button_open_oszi->setColour (TextButton::buttonColourId, Colours::black);
    button_open_oszi->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_oszi->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_open_midi_io_settings = new TextButton (String::empty));
    button_open_midi_io_settings->setTooltip (TRANS("Open/Close the MIDI settings.\n"
    "\n"
    "Note: press ESC to close editors."));
    button_open_midi_io_settings->setButtonText (TRANS("MIDI"));
    button_open_midi_io_settings->addListener (this);
    button_open_midi_io_settings->setColour (TextButton::buttonColourId, Colours::black);
    button_open_midi_io_settings->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_midi_io_settings->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (combo_bank = new ComboBox (String::empty));
    combo_bank->setTooltip (TRANS("Select the current program bank."));
    combo_bank->setEditableText (false);
    combo_bank->setJustificationType (Justification::centredLeft);
    combo_bank->setTextWhenNothingSelected (String::empty);
    combo_bank->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_bank->addListener (this);

    addAndMakeVisible (button_programm_load = new TextButton (String::empty));
    button_programm_load->setTooltip (TRANS("Load the selected program."));
    button_programm_load->setButtonText (TRANS("LOAD"));
    button_programm_load->addListener (this);
    button_programm_load->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_load->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_load->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (osc_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new OSCSlConfig(synth_data,0)));

    addAndMakeVisible (osc_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new OSCSlConfig(synth_data,1)));

    addAndMakeVisible (osc_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new OSCSlConfig(synth_data,2)));

    addAndMakeVisible (lfo_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new LFOSlConfig(synth_data,0)));

    addAndMakeVisible (flt_cutoff_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FCutoffSLConfig(synth_data,0)));

    addAndMakeVisible (lfo_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new LFOSlConfig(synth_data,1)));

    addAndMakeVisible (lfo_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                          new LFOSlConfig(synth_data,2)));

    addAndMakeVisible (flt_cutoff_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FCutoffSLConfig(synth_data,1)));

    addAndMakeVisible (flt_cutoff_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FCutoffSLConfig(synth_data,2)));

    addAndMakeVisible (flt_input_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,0,0)));

    addAndMakeVisible (flt_input_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,0,1)));

    addAndMakeVisible (flt_input_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,0,2)));

    addAndMakeVisible (flt_distortion_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                     new GForceSlConfig(synth_data,0)));

    addAndMakeVisible (flt_input_6 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,1,0)));

    addAndMakeVisible (flt_input_7 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,1,1)));

    addAndMakeVisible (flt_input_8 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new InputSlConfig(synth_data,1,2)));

    addAndMakeVisible (flt_input_11 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new InputSlConfig(synth_data,2,0)));

    addAndMakeVisible (flt_input_12 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new InputSlConfig(synth_data,2,1)));

    addAndMakeVisible (flt_input_13 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new InputSlConfig(synth_data,2,2)));

    addAndMakeVisible (flt_resonance_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                    new FResonanceSLConfig(synth_data,0)));

    addAndMakeVisible (flt_resonance_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                    new FResonanceSLConfig(synth_data,1)));

    addAndMakeVisible (flt_resonance_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                    new FResonanceSLConfig(synth_data,2)));

    addAndMakeVisible (flt_volume_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FVolumeSlConfig(synth_data,0)));

    addAndMakeVisible (flt_volume_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FVolumeSlConfig(synth_data,1)));

    addAndMakeVisible (flt_volume_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FVolumeSlConfig(synth_data,2)));

    addAndMakeVisible (adsr_lfo_mix = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new EnvLfoSlConfig(synth_data,0)));

    addAndMakeVisible (lfo_opt_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new EnvLfoSlConfig(synth_data,1)));

    addAndMakeVisible (lfo_opt_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new EnvLfoSlConfig(synth_data,2)));

    addAndMakeVisible (button_sequence_1 = new TextButton (String::empty));
    button_sequence_1->setTooltip (TRANS("Turns this step on or off.\n"
    "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_1->addListener (this);
    button_sequence_1->setColour (TextButton::buttonColourId, Colours::black);
    button_sequence_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_sequence_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_release_4 = new Monique_Ui_DualSlider (ui_refresher, new FReleaseSlConfig(synth_data)));

    addAndMakeVisible (volume = new Monique_Ui_DualSlider (ui_refresher,
                                                           new VolumeConfig(synth_data)));

    addAndMakeVisible (flt_distortion_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                     new GForceSlConfig(synth_data,1)));

    addAndMakeVisible (flt_distortion_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                     new GForceSlConfig(synth_data,2)));

    addAndMakeVisible (button_arp_speed_XNORM = new TextButton (String::empty));
    button_arp_speed_XNORM->setTooltip (TRANS("Shortcut to set the speed multiplier back to 1x (in sync)."));
    button_arp_speed_XNORM->setButtonText (TRANS("x1"));
    button_arp_speed_XNORM->addListener (this);
    button_arp_speed_XNORM->setColour (TextButton::buttonColourId, Colours::black);
    button_arp_speed_XNORM->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_arp_speed_XNORM->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_attack_5 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FMFreqSlConfig(synth_data)));

    addAndMakeVisible (flt_attack_6 = new Monique_Ui_DualSlider (ui_refresher,
                                                                 new FMAmountSlConfig(synth_data)));

    addAndMakeVisible (osc_wave_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new WAVESlConfig(synth_data,0)));

    addAndMakeVisible (osc_wave_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                               new WAVESlConfig(synth_data,1)));

    addAndMakeVisible (sl_morhp_mix = new Slider ("new slider"));
    sl_morhp_mix->setTooltip (TRANS("Morph between the morph sliders. \n"
    "\n"
    "Take a look at the MORPH MOTOR time in the SETUP to control the speed of this morph."));
    sl_morhp_mix->setRange (0, 3000, 0.01);
    sl_morhp_mix->setSliderStyle (Slider::LinearHorizontal);
    sl_morhp_mix->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_morhp_mix->addListener (this);

    addAndMakeVisible (button_programm_delete = new TextButton (String::empty));
    button_programm_delete->setTooltip (TRANS("Delete the selected program."));
    button_programm_delete->setButtonText (TRANS("DELETE"));
    button_programm_delete->addListener (this);
    button_programm_delete->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_delete->setColour (TextButton::textColourOnId, Colours::red);
    button_programm_delete->setColour (TextButton::textColourOffId, Colour (0xffff7900));

    addAndMakeVisible (filter_type_6_1 = new TextButton ("VOICE 1"));
    filter_type_6_1->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_6_1->setButtonText (TRANS("LP"));
    filter_type_6_1->addListener (this);
    filter_type_6_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_2 = new TextButton ("VOICE 1"));
    filter_type_6_2->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_6_2->setButtonText (TRANS("LP"));
    filter_type_6_2->addListener (this);
    filter_type_6_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_3 = new TextButton ("VOICE 1"));
    filter_type_6_3->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_6_3->setButtonText (TRANS("LP"));
    filter_type_6_3->addListener (this);
    filter_type_6_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_ctrl_toggle = new TextButton (String::empty));
    button_ctrl_toggle->setTooltip (TRANS("Turns the SHIFT mode on or off.\n"
    "\n"
    "The shift mode moves all back sliders to front and front sliders to back."));
    button_ctrl_toggle->setButtonText (TRANS("SHIFT"));
    button_ctrl_toggle->addListener (this);
    button_ctrl_toggle->setColour (TextButton::buttonColourId, Colours::black);
    button_ctrl_toggle->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_ctrl_toggle->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (speed = new Monique_Ui_DualSlider (ui_refresher,
                                                          new BPMSlConfig(synth_data)));

    addAndMakeVisible (button_open_morph = new TextButton (String::empty));
    button_open_morph->setTooltip (TRANS("Open/Close the morph editor.\n"
    "\n"
    "Note: press ESC to close editors."));
    button_open_morph->setButtonText (TRANS("EDIT"));
    button_open_morph->addListener (this);
    button_open_morph->setColour (TextButton::buttonColourId, Colours::black);
    button_open_morph->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_morph->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (effect_finalizer_switch = new TextButton (String::empty));
    effect_finalizer_switch->setTooltip (TRANS("Switch between FX and EQ bank section."));
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
    button_values_toggle->setTooltip (TRANS("Turns the CTRL mode on or off.\n"
    "\n"
    "In CTRL mode are all values visble.\n"
    "\n"
    "Hold down CTRL/CMD on your keyboard and drag a slider to control it in velocity mode.\n"
    "\n"
    "Hold down CTRL/CMD on your keyboard and press + or - to resize the user interface. Press F11 to toggle fullscreen mode."));
    button_values_toggle->setButtonText (TRANS("CTRL"));
    button_values_toggle->addListener (this);
    button_values_toggle->setColour (TextButton::buttonColourId, Colours::black);
    button_values_toggle->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_values_toggle->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (octave_offset = new Monique_Ui_DualSlider (ui_refresher,
                                                                  new OctaveOffsetSlConfig(synth_data)));

    addAndMakeVisible (label_ui_headline4 = new Label ("DL",
                                                       TRANS("FILTER INPUTS")));
    label_ui_headline4->setFont (Font (30.00f, Font::plain));
    label_ui_headline4->setJustificationType (Justification::centred);
    label_ui_headline4->setEditable (false, false, false);
    label_ui_headline4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline7 = new Label ("DL",
                                                       TRANS("OSCILLATORS (O)")));
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
                                                       TRANS("LFO (L)")));
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
                                                TRANS("DISTORTION")));
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
                                                        TRANS("MORPH (MO) SECTION")));
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
                                                        TRANS("TUNE")));
    label_ui_headline24->setFont (Font (30.00f, Font::plain));
    label_ui_headline24->setJustificationType (Justification::centred);
    label_ui_headline24->setEditable (false, false, false);
    label_ui_headline24->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline24->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline24->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline25 = new Label ("DL",
                                                        TRANS("FM (F)")));
    label_ui_headline25->setFont (Font (30.00f, Font::plain));
    label_ui_headline25->setJustificationType (Justification::centred);
    label_ui_headline25->setEditable (false, false, false);
    label_ui_headline25->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline25->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline25->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (volume_master_meter = new Monique_Ui_SegmentedMeter (ui_refresher));

    addAndMakeVisible (label_eq = new Label ("DL",
                                             TRANS("EQUALIZER")));
    label_eq->setFont (Font (30.00f, Font::plain));
    label_eq->setJustificationType (Justification::centred);
    label_eq->setEditable (false, false, false);
    label_eq->setColour (Label::textColourId, Colour (0xff1111ff));
    label_eq->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_eq->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_open_config2 = new TextButton (String::empty));
    button_open_config2->setTooltip (TRANS("Open/Close the setup.\n"
    "\n"
    "Note: press ESC to close editors."));
    button_open_config2->setButtonText (TRANS("SETUP"));
    button_open_config2->addListener (this);
    button_open_config2->setColour (TextButton::buttonColourId, Colours::black);
    button_open_config2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_config2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_ui_headline14 = new Label ("DL",
                                                        TRANS("MOD MIX (X)")));
    label_ui_headline14->setFont (Font (30.00f, Font::plain));
    label_ui_headline14->setJustificationType (Justification::centred);
    label_ui_headline14->setEditable (false, false, false);
    label_ui_headline14->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline14->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_edit_input_env_1_1 = new TextButton (String::empty));
    button_edit_input_env_1_1->setButtonText (TRANS("EDIT"));
    button_edit_input_env_1_1->addListener (this);
    button_edit_input_env_1_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_1_2 = new TextButton (String::empty));
    button_edit_input_env_1_2->setButtonText (TRANS("EDIT"));
    button_edit_input_env_1_2->addListener (this);
    button_edit_input_env_1_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_1_3 = new TextButton (String::empty));
    button_edit_input_env_1_3->setButtonText (TRANS("EDIT"));
    button_edit_input_env_1_3->addListener (this);
    button_edit_input_env_1_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_1 = new TextButton (String::empty));
    button_edit_input_env_2_1->setButtonText (TRANS("EDIT"));
    button_edit_input_env_2_1->addListener (this);
    button_edit_input_env_2_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_2 = new TextButton (String::empty));
    button_edit_input_env_2_2->setButtonText (TRANS("EDIT"));
    button_edit_input_env_2_2->addListener (this);
    button_edit_input_env_2_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_3 = new TextButton (String::empty));
    button_edit_input_env_2_3->setButtonText (TRANS("EDIT"));
    button_edit_input_env_2_3->addListener (this);
    button_edit_input_env_2_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_1 = new TextButton (String::empty));
    button_edit_input_env_3_1->setButtonText (TRANS("EDIT"));
    button_edit_input_env_3_1->addListener (this);
    button_edit_input_env_3_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_2 = new TextButton (String::empty));
    button_edit_input_env_3_2->setButtonText (TRANS("EDIT"));
    button_edit_input_env_3_2->addListener (this);
    button_edit_input_env_3_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_3 = new TextButton (String::empty));
    button_edit_input_env_3_3->setButtonText (TRANS("EDIT"));
    button_edit_input_env_3_3->addListener (this);
    button_edit_input_env_3_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_1 = new TextButton (String::empty));
    button_edit_input_env_band_1->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_1->addListener (this);
    button_edit_input_env_band_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_2 = new TextButton (String::empty));
    button_edit_input_env_band_2->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_2->addListener (this);
    button_edit_input_env_band_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_3 = new TextButton (String::empty));
    button_edit_input_env_band_3->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_3->addListener (this);
    button_edit_input_env_band_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_4 = new TextButton (String::empty));
    button_edit_input_env_band_4->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_4->addListener (this);
    button_edit_input_env_band_4->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_5 = new TextButton (String::empty));
    button_edit_input_env_band_5->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_5->addListener (this);
    button_edit_input_env_band_5->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_5->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_5->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_6 = new TextButton (String::empty));
    button_edit_input_env_band_6->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_6->addListener (this);
    button_edit_input_env_band_6->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_6->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_6->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_7 = new TextButton (String::empty));
    button_edit_input_env_band_7->setButtonText (TRANS("EDIT"));
    button_edit_input_env_band_7->addListener (this);
    button_edit_input_env_band_7->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_7->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_7->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_env_chorus = new TextButton (String::empty));
    button_edit_env_chorus->setButtonText (TRANS("EDIT"));
    button_edit_env_chorus->addListener (this);
    button_edit_env_chorus->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_env_chorus->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_env_chorus->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_pan_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new FPanSlConfig(synth_data,2)));

    addAndMakeVisible (flt_pan_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new FPanSlConfig(synth_data,1)));

    addAndMakeVisible (flt_pan_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                              new FPanSlConfig(synth_data,0)));

    addAndMakeVisible (button_reset_arp_tune = new TextButton (String::empty));
    button_reset_arp_tune->setTooltip (TRANS("Resets the arpeggiator to the defined program note.\n"
    "(Triggers a note which is defineable by the note dial (back dial))"));
    button_reset_arp_tune->setButtonText (TRANS("RESET"));
    button_reset_arp_tune->addListener (this);
    button_reset_arp_tune->setColour (TextButton::buttonColourId, Colours::black);
    button_reset_arp_tune->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_reset_arp_tune->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_2_3 = new TextButton (String::empty));
    button_show_active_input_r_2_3->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_2_3 = new TextButton (String::empty));
    button_show_active_input_l_2_3->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_2_2 = new TextButton (String::empty));
    button_show_active_input_r_2_2->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_2_2 = new TextButton (String::empty));
    button_show_active_input_l_2_2->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_2_1 = new TextButton (String::empty));
    button_show_active_input_r_2_1->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_2_1 = new TextButton (String::empty));
    button_show_active_input_l_2_1->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_3_3 = new TextButton (String::empty));
    button_show_active_input_r_3_3->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_3_3 = new TextButton (String::empty));
    button_show_active_input_l_3_3->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_3_2 = new TextButton (String::empty));
    button_show_active_input_r_3_2->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_3_2 = new TextButton (String::empty));
    button_show_active_input_l_3_2->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_r_3_1 = new TextButton (String::empty));
    button_show_active_input_r_3_1->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_r_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_r_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_show_active_input_l_3_1 = new TextButton (String::empty));
    button_show_active_input_l_3_1->setColour (TextButton::buttonColourId, Colours::black);
    button_show_active_input_l_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_show_active_input_l_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_rename = new TextButton (String::empty));
    button_programm_rename->setTooltip (TRANS("Rename the selected program."));
    button_programm_rename->setButtonText (TRANS("RENAME"));
    button_programm_rename->addListener (this);
    button_programm_rename->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_rename->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_rename->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (flt_shape_1 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FShapeSlConfig(synth_data,0)));

    addAndMakeVisible (flt_shape_2 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FShapeSlConfig(synth_data,1)));

    addAndMakeVisible (flt_shape_3 = new Monique_Ui_DualSlider (ui_refresher,
                                                                new FShapeSlConfig(synth_data,2)));


    //[UserPreSize]
#ifdef IS_PLUGIN
    button_open_midi_io_settings->setTooltip
    (
        "Enables the MIDI train/learn mode.\n"
        "\n"
        "Handling: enable MIDI train and select a slider or button on the main user interface. A little window pops up. Now you can move a slider on your MIDI controller (sender) to assign it to the element on the user interface (listener).\n"
        "\n"
        "Shortcut: CTRL + m"
    );
    button_open_midi_io_settings->setButtonText (TRANS("TRAIN"));
#endif


    SET_SLIDER_STYLE(sl_morhp_mix,VALUE_SLIDER);

    last_bank = -1;
    last_programm = -1;
    is_in_help_mode = false;

    voice = audio_processor->voice;
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

    for( int i = 0 ;  i != getNumChildComponents() ; ++i )
    {
        Component* comp( getChildComponent(i) );
        if( comp->GET_LABEL_STYLE() != "I" ) // DISCRIPTION LABELS
        {
            comp->setOpaque(true);
        }
        else
        {
            comp->setVisible(false);
        }

        /*
            if( Button* button = dynamic_cast< Button* >( comp ) )
            {
                button->setTooltip("test");
            }
            */
        if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( comp ) )
        {
            dual_sliders.add(slider);
        }
    }
    label_monolisa->setOpaque(false);

    switch_finalizer_tab();

    ComponentColours& colours = look_and_feel->colours;
    Colour button_off = colours.button_off_colour;

    button_programm_new->setColour(TextButton::buttonColourId, button_off);
    button_programm_replace->setColour(TextButton::buttonColourId, button_off);
    button_programm_delete->setColour(TextButton::buttonColourId, button_off);
    button_programm_load->setColour(TextButton::buttonColourId, button_off);
    button_programm_left->setColour(TextButton::buttonColourId, button_off);
    button_programm_right->setColour(TextButton::buttonColourId, button_off);
    button_programm_rename->setColour(TextButton::buttonColourId, button_off);
    button_open_oszi->setColour(TextButton::buttonColourId, button_off);
    button_open_midi_io_settings->setColour(TextButton::buttonColourId, button_off);
    effect_finalizer_switch->setColour(TextButton::buttonColourId, button_off);


    button_edit_input_env_1_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_1_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_1_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_4->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_5->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_6->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_7->setColour( TextButton::buttonColourId, button_off );
    button_edit_env_chorus->setColour( TextButton::buttonColourId, button_off );
    button_reset_arp_tune->setColour( TextButton::buttonColourId, button_off );

    button_show_active_input_r_2_3->setEnabled(false);
    button_show_active_input_l_2_3->setEnabled(false);
    button_show_active_input_r_2_2->setEnabled(false);
    button_show_active_input_l_2_2->setEnabled(false);
    button_show_active_input_r_2_1->setEnabled(false);
    button_show_active_input_l_2_1->setEnabled(false);
    button_show_active_input_r_3_3->setEnabled(false);
    button_show_active_input_l_3_3->setEnabled(false);
    button_show_active_input_r_3_2->setEnabled(false);
    button_show_active_input_l_3_2->setEnabled(false);
    button_show_active_input_r_3_1->setEnabled(false);
    button_show_active_input_l_3_1->setEnabled(false);


    setOpaque(true);

    combo_programm->setEditableText(false);
    update_slider_handling();
    update_tooltip_handling(false);
    //update_slider_return_values();

    /*
    //[/UserPreSize]

    setSize (1465, 1235);


    //[Constructor] You can add your own custom stuff here..
    */
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
#ifdef IS_PLUGIN
    //resizeLimits.setSizeLimits (1440.0f*0.7f, 1080.0f*0.7f, 1440*10, 1080*10);
    //resizeLimits.setFixedAspectRatio( 1440.0f/1080.0f );
#else
    resizer->setVisible(false);
#endif


    //setVisible(true);
    audio_processor->set_peak_meter( volume_master_meter );
    ui_refresher->startTimer( UI_REFRESH_RATE );

    update_size();
    keyboard->setLowestVisibleKey(24);
    keyboard->setAvailableRange( 12, 60 + 24 );
    keyboard->setKeyWidth(45);

    if( synth_data->is_osci_open.get_value() )
        buttonClicked( button_open_oszi );

    // resizer->setTooltip( "Global shortcut: CTRL + PLUS or CTRL + MINUS" );
    //look_and_feel->colours.edit();
    //[/Constructor]
}

Monique_Ui_Mainwindow::~Monique_Ui_Mainwindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    ui_refresher->editor = nullptr;
    if( amp_painter )
    {
        audio_processor->amp_painter = nullptr;
        delete amp_painter;
        amp_painter = nullptr;
    }

    ui_refresher->stopTimer();
    ui_refresher->remove_all();

    env_popup = nullptr;
    option_popup = nullptr;
    editor_midiio = nullptr;
    editor_morph = nullptr;
    editor_global_settings = nullptr;
    popup = nullptr;

    audio_processor->clear_preak_meter();
    //[/Destructor_pre]

    distortion = nullptr;
    reverb_room = nullptr;
    reverb_width = nullptr;
    reverb_dry = nullptr;
    bypass = nullptr;
    eq_1 = nullptr;
    eq_3 = nullptr;
    eq_4 = nullptr;
    eq_5 = nullptr;
    eq_6 = nullptr;
    eq_7 = nullptr;
    colour = nullptr;
    delay2 = nullptr;
    chorus_modulation = nullptr;
    eq_2 = nullptr;
    label_effect_hider = nullptr;
    label_band_hz_2 = nullptr;
    label_band_hz_3 = nullptr;
    label_band_hz_4 = nullptr;
    label_band_hz_5 = nullptr;
    label_band_hz_6 = nullptr;
    label_band_hz_7 = nullptr;
    label_band_hz_1 = nullptr;
    speed_multi = nullptr;
    morpher_4 = nullptr;
    morpher_3 = nullptr;
    morpher_2 = nullptr;
    morpher_1 = nullptr;
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
    filter_type_2_1 = nullptr;
    filter_type_3_1 = nullptr;
    filter_type_2_2 = nullptr;
    filter_type_3_2 = nullptr;
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
    flt_distortion_1 = nullptr;
    flt_input_6 = nullptr;
    flt_input_7 = nullptr;
    flt_input_8 = nullptr;
    flt_input_11 = nullptr;
    flt_input_12 = nullptr;
    flt_input_13 = nullptr;
    flt_resonance_1 = nullptr;
    flt_resonance_2 = nullptr;
    flt_resonance_3 = nullptr;
    flt_volume_1 = nullptr;
    flt_volume_2 = nullptr;
    flt_volume_3 = nullptr;
    adsr_lfo_mix = nullptr;
    lfo_opt_2 = nullptr;
    lfo_opt_3 = nullptr;
    button_sequence_1 = nullptr;
    flt_release_4 = nullptr;
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
    filter_type_6_1 = nullptr;
    filter_type_6_2 = nullptr;
    filter_type_6_3 = nullptr;
    button_ctrl_toggle = nullptr;
    speed = nullptr;
    button_open_morph = nullptr;
    effect_finalizer_switch = nullptr;
    label_ui_headline2 = nullptr;
    label_ui_headline3 = nullptr;
    label_ui_headline5 = nullptr;
    label_ui_headline6 = nullptr;
    button_values_toggle = nullptr;
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
    label_ui_headline14 = nullptr;
    button_edit_input_env_1_1 = nullptr;
    button_edit_input_env_1_2 = nullptr;
    button_edit_input_env_1_3 = nullptr;
    button_edit_input_env_2_1 = nullptr;
    button_edit_input_env_2_2 = nullptr;
    button_edit_input_env_2_3 = nullptr;
    button_edit_input_env_3_1 = nullptr;
    button_edit_input_env_3_2 = nullptr;
    button_edit_input_env_3_3 = nullptr;
    button_edit_input_env_band_1 = nullptr;
    button_edit_input_env_band_2 = nullptr;
    button_edit_input_env_band_3 = nullptr;
    button_edit_input_env_band_4 = nullptr;
    button_edit_input_env_band_5 = nullptr;
    button_edit_input_env_band_6 = nullptr;
    button_edit_input_env_band_7 = nullptr;
    button_edit_env_chorus = nullptr;
    flt_pan_3 = nullptr;
    flt_pan_2 = nullptr;
    flt_pan_1 = nullptr;
    button_reset_arp_tune = nullptr;
    button_show_active_input_r_2_3 = nullptr;
    button_show_active_input_l_2_3 = nullptr;
    button_show_active_input_r_2_2 = nullptr;
    button_show_active_input_l_2_2 = nullptr;
    button_show_active_input_r_2_1 = nullptr;
    button_show_active_input_l_2_1 = nullptr;
    button_show_active_input_r_3_3 = nullptr;
    button_show_active_input_l_3_3 = nullptr;
    button_show_active_input_r_3_2 = nullptr;
    button_show_active_input_l_3_2 = nullptr;
    button_show_active_input_r_3_1 = nullptr;
    button_show_active_input_l_3_1 = nullptr;
    button_programm_rename = nullptr;
    flt_shape_1 = nullptr;
    flt_shape_2 = nullptr;
    flt_shape_3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_Mainwindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.fillAll (Colour (0xff161616));
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (480.0f, 645.0f, 605.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (480.0f, 645.0f, 605.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (10.0f, 845.0f, 1445.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (10.0f, 845.0f, 1445.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 70.0f, 1260.0f, 195.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 70.0f, 1260.0f, 195.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (10.0f, 70.0f, 170.0f, 754.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (10.0f, 70.0f, 170.0f, 754.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 94, 25, 3);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 99, 25, 3);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (170.0f, 100.0f, 181.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (175, 89, 25, 3);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 445.0f, 1260.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 445.0f, 1260.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 265.0f, 1260.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 265.0f, 1260.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (195.0f, 645.0f, 285.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (195.0f, 645.0f, 285.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 110.0f, 1.0f, 541.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 100.0f, 226.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 650.0f, 76.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 880.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (90.0f, 880.0f, 10.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 105.0f, 340.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 655.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 650.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (910.0f, 105.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (830.0f, 100.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (615.0f, 810.0f, 421.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (970.0f, 805.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (615.0f, 805.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1035.0f, 805.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1015.0f, 805.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 95.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (930.0f, 100.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 100.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 95.0f, 566.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 100.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1260.0f, 770.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1265.0f, 765.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 100.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1095.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1155.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1245.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (300.0f, 250.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (240.0f, 250.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (360.0f, 250.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 465.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 460.0f, 76.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 460.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 290.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 110.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 235.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 415.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 470.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (20.0f, 595.0f, 5.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 105.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 90.0f, 1.0f, 16.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 95.0f, 1.0f, 185.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (170.0f, 100.0f, 1.0f, 360.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (40.0f, 285.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 280.0f, 71.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 280.0f, 1.0f, 6.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 95, 3, 10);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (165.0f, 95.0f, 126.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 100.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 90, 3, 16);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 90.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (95.0f, 90.0f, 136.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (259, 255, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (319, 255, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (379, 255, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 245, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 245, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (349, 245, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 245.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 245.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 245.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (380.0f, 250.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (320.0f, 250.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (260.0f, 250.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1085.0f, 645.0f, 270.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (1085.0f, 645.0f, 270.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1355.0f, 645.0f, 100.0f, 180.0f, 6.000f);

    g.setColour (Colour (0xff1111ff));
    g.drawRoundedRectangle (1355.0f, 645.0f, 100.0f, 180.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 280.0f, 226.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 285.0f, 340.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (910.0f, 285.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (830.0f, 280.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 275.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (930.0f, 280.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 280.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 275.0f, 566.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 280.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 280.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1095.0f, 275.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1155.0f, 275.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 275.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1245.0f, 275.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 460.0f, 226.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (435.0f, 465.0f, 340.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (910.0f, 465.0f, 40.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (830.0f, 460.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 455.0f, 1.0f, 15.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (930.0f, 460.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 460.0f, 81.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (840.0f, 455.0f, 566.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (850.0f, 460.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (605.0f, 460.0f, 1.0f, 5.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1095.0f, 455.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1155.0f, 455.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1405.0f, 455.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1245.0f, 455.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (300.0f, 430.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (240.0f, 430.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (360.0f, 430.0f, 25.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xff050505));
    g.fillRect (259, 435, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (319, 435, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (379, 435, 3, 20);

    g.setColour (Colour (0xff050505));
    g.fillRect (229, 425, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (289, 425, 3, 30);

    g.setColour (Colour (0xff050505));
    g.fillRect (349, 425, 3, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (350.0f, 425.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (290.0f, 425.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (230.0f, 425.0f, 1.0f, 40.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (380.0f, 430.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (320.0f, 430.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (260.0f, 430.0f, 1.0f, 35.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1305.0f, 275.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1305.0f, 95.0f, 1.0f, 10.0f, 1.000f);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (1305.0f, 455.0f, 1.0f, 10.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_Mainwindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    distortion->setBounds (585 - 60, 800 - 130, 60, 130);
    reverb_room->setBounds (855 - 60, 800 - 130, 60, 130);
    reverb_width->setBounds (915 - 60, 800 - 130, 60, 130);
    reverb_dry->setBounds (975 - 60, 800 - 130, 60, 130);
    bypass->setBounds (1065 - 60, 800 - 130, 60, 130);
    eq_1->setBounds (585 - 60, 800 - 130, 60, 130);
    eq_3->setBounds (720 - 60, 800 - 130, 60, 130);
    eq_4->setBounds (780 - 60, 800 - 130, 60, 130);
    eq_5->setBounds (850 - 60, 800 - 130, 60, 130);
    eq_6->setBounds (910 - 60, 800 - 130, 60, 130);
    eq_7->setBounds (980 - 60, 800 - 130, 60, 130);
    colour->setBounds (1065 - 60, 800 - 130, 60, 130);
    delay2->setBounds (765 - 60, 800 - 130, 60, 130);
    chorus_modulation->setBounds (615, 800 - 130, 60, 130);
    eq_2->setBounds (645 - 60, 800 - 130, 60, 130);
    label_effect_hider->setBounds (1065 - 540, 815 - 15, 540, 15);
    label_band_hz_2->setBounds (590, 795, 50, 30);
    label_band_hz_3->setBounds (665, 795, 50, 30);
    label_band_hz_4->setBounds (725, 795, 50, 30);
    label_band_hz_5->setBounds (795, 795, 50, 30);
    label_band_hz_6->setBounds (855, 795, 50, 30);
    label_band_hz_7->setBounds (925, 795, 50, 30);
    label_band_hz_1->setBounds (530, 795, 50, 30);
    speed_multi->setBounds (1355 - 60, 1000 - 130, 60, 130);
    morpher_4->setBounds (1345 - 60, 670, 60, 130);
    morpher_3->setBounds (1285 - 60, 670, 60, 130);
    morpher_2->setBounds (1225 - 60, 670, 60, 130);
    morpher_1->setBounds (1165 - 60, 670, 60, 130);
    osc_wave_3->setBounds (90 - 60, 600 - 130, 60, 130);
    keyboard->setBounds (0, 1055, 1465, 180);
    glide2->setBounds (100, 870, 60, 130);
    arp_step_16->setBounds (1205 - 60, 1000 - 130, 60, 130);
    arp_step_15->setBounds (1145 - 60, 1000 - 130, 60, 130);
    arp_step_14->setBounds (1085 - 60, 1000 - 130, 60, 130);
    arp_step_13->setBounds (965, 870, 60, 130);
    arp_step_12->setBounds (955 - 60, 1000 - 130, 60, 130);
    arp_step_11->setBounds (895 - 60, 1000 - 130, 60, 130);
    arp_step_10->setBounds (835 - 60, 1000 - 130, 60, 130);
    arp_step_9->setBounds (775 - 60, 1000 - 130, 60, 130);
    arp_step_8->setBounds (705 - 60, 1000 - 130, 60, 130);
    arp_step_7->setBounds (645 - 60, 1000 - 130, 60, 130);
    arp_step_6->setBounds (585 - 60, 1000 - 130, 60, 130);
    arp_step_5->setBounds (525 - 60, 1000 - 130, 60, 130);
    arp_step_4->setBounds (455 - 60, 1000 - 130, 60, 130);
    arp_step_3->setBounds (395 - 60, 1000 - 130, 60, 130);
    arp_step_2->setBounds (335 - 60, 1000 - 130, 60, 130);
    arp_step_1->setBounds (275 - 60, 1000 - 130, 60, 130);
    shuffle->setBounds (30, 870, 60, 130);
    flt_sustain_4->setBounds (395 - 60, 800 - 130, 60, 130);
    flt_decay_4->setBounds (335 - 60, 800 - 130, 60, 130);
    flt_attack_4->setBounds (275 - 60, 800 - 130, 60, 130);
    flt_release_3->setBounds (725 - 60, 600 - 130, 60, 130);
    flt_sustain_time_3->setBounds (665 - 60, 600 - 130, 60, 130);
    flt_sustain_3->setBounds (605 - 60, 600 - 130, 60, 130);
    flt_decay_3->setBounds (545 - 60, 600 - 130, 60, 130);
    flt_attack_3->setBounds (485 - 60, 600 - 130, 60, 130);
    flt_release_2->setBounds (725 - 60, 420 - 130, 60, 130);
    flt_sustain_time_2->setBounds (665 - 60, 420 - 130, 60, 130);
    flt_sustain_2->setBounds (605 - 60, 420 - 130, 60, 130);
    flt_decay_2->setBounds (545 - 60, 420 - 130, 60, 130);
    flt_attack_2->setBounds (485 - 60, 420 - 130, 60, 130);
    flt_release_1->setBounds (725 - 60, 240 - 130, 60, 130);
    flt_sustain_time_1->setBounds (665 - 60, 240 - 130, 60, 130);
    flt_sustain_1->setBounds (605 - 60, 240 - 130, 60, 130);
    flt_decay_1->setBounds (545 - 60, 240 - 130, 60, 130);
    flt_attack_1->setBounds (425, 240 - 130, 60, 130);
    label_monolisa->setBounds (1220 - 190, 50 - 50, 190, 50);
    filter_type_2_1->setBounds (1055 - 60, 140, 60, 30);
    filter_type_3_1->setBounds (1055 - 60, 170, 60, 30);
    filter_type_2_2->setBounds (1055 - 60, 320, 60, 30);
    filter_type_3_2->setBounds (1055 - 60, 377 - 30, 60, 30);
    filter_type_2_3->setBounds (1055 - 60, 500, 60, 30);
    filter_type_3_3->setBounds (1055 - 60, 530, 60, 30);
    filter_type_5_1->setBounds (1055 - 60, 205, 60, 33);
    filter_type_5_2->setBounds (1055 - 60, 385, 60, 33);
    filter_type_5_3->setBounds (1055 - 60, 565, 60, 33);
    button_sequence_2->setBounds (335 - 60, 890 - 20, 60, 20);
    button_sequence_3->setBounds (395 - 60, 890 - 20, 60, 20);
    button_sequence_4->setBounds (455 - 60, 890 - 20, 60, 20);
    button_sequence_5->setBounds (525 - 60, 890 - 20, 60, 20);
    button_sequence_6->setBounds (585 - 60, 890 - 20, 60, 20);
    button_sequence_7->setBounds (645 - 60, 890 - 20, 60, 20);
    button_sequence_8->setBounds (705 - 60, 890 - 20, 60, 20);
    button_sequence_9->setBounds (775 - 60, 890 - 20, 60, 20);
    button_sequence_10->setBounds (835 - 60, 890 - 20, 60, 20);
    button_sequence_11->setBounds (895 - 60, 890 - 20, 60, 20);
    button_sequence_12->setBounds (955 - 60, 890 - 20, 60, 20);
    button_sequence_13->setBounds (1025 - 60, 890 - 20, 60, 20);
    button_sequence_14->setBounds (1085 - 60, 890 - 20, 60, 20);
    button_sequence_15->setBounds (1145 - 60, 890 - 20, 60, 20);
    button_sequence_16->setBounds (1205 - 60, 890 - 20, 60, 20);
    combo_programm->setBounds (335, 10, 315, 27);
    button_programm_left->setBounds (215, 10, 60, 27);
    button_programm_right->setBounds (710 - 60, 10, 60, 27);
    button_programm_replace->setBounds (910 - 60, 10, 60, 27);
    button_programm_new->setBounds (850 - 60, 10, 60, 27);
    button_open_oszi->setBounds (1375, 10, 60, 30);
    button_open_midi_io_settings->setBounds (1280 - 60, 40 - 30, 60, 30);
    combo_bank->setBounds (275, 10, 60, 27);
    button_programm_load->setBounds (770 - 60, 10, 60, 27);
    osc_1->setBounds (100, 240 - 130, 60, 130);
    osc_2->setBounds (160 - 60, 420 - 130, 60, 130);
    osc_3->setBounds (160 - 60, 600 - 130, 60, 130);
    lfo_1->setBounds (960 - 60, 240 - 130, 60, 130);
    flt_cutoff_1->setBounds (1125 - 60, 240 - 130, 60, 130);
    lfo_2->setBounds (960 - 60, 420 - 130, 60, 130);
    lfo_3->setBounds (960 - 60, 600 - 130, 60, 130);
    flt_cutoff_2->setBounds (1125 - 60, 420 - 130, 60, 130);
    flt_cutoff_3->setBounds (1125 - 60, 600 - 130, 60, 130);
    flt_input_1->setBounds (215, 110, 60, 130);
    flt_input_2->setBounds (335 - 60, 240 - 130, 60, 130);
    flt_input_3->setBounds (395 - 60, 240 - 130, 60, 130);
    flt_distortion_1->setBounds (1275 - 60, 240 - 130, 60, 130);
    flt_input_6->setBounds (275 - 60, 420 - 130, 60, 130);
    flt_input_7->setBounds (335 - 60, 420 - 130, 60, 130);
    flt_input_8->setBounds (395 - 60, 420 - 130, 60, 130);
    flt_input_11->setBounds (275 - 60, 600 - 130, 60, 130);
    flt_input_12->setBounds (335 - 60, 600 - 130, 60, 130);
    flt_input_13->setBounds (395 - 60, 600 - 130, 60, 130);
    flt_resonance_1->setBounds (1185 - 60, 240 - 130, 60, 130);
    flt_resonance_2->setBounds (1185 - 60, 420 - 130, 60, 130);
    flt_resonance_3->setBounds (1185 - 60, 600 - 130, 60, 130);
    flt_volume_1->setBounds (1375, 110, 60, 130);
    flt_volume_2->setBounds (1435 - 60, 420 - 130, 60, 130);
    flt_volume_3->setBounds (1435 - 60, 600 - 130, 60, 130);
    adsr_lfo_mix->setBounds (870 - 60, 240 - 130, 60, 130);
    lfo_opt_2->setBounds (870 - 60, 420 - 130, 60, 130);
    lfo_opt_3->setBounds (870 - 60, 600 - 130, 60, 130);
    button_sequence_1->setBounds (275 - 60, 890 - 20, 60, 20);
    flt_release_4->setBounds (455 - 60, 800 - 130, 60, 130);
    volume->setBounds (1435 - 60, 800 - 130, 60, 130);
    flt_distortion_2->setBounds (1275 - 60, 420 - 130, 60, 130);
    flt_distortion_3->setBounds (1275 - 60, 600 - 130, 60, 130);
    button_arp_speed_XNORM->setBounds (1295, 870, 60, 27);
    flt_attack_5->setBounds (90 - 60, 805 - 130, 60, 130);
    flt_attack_6->setBounds (160 - 60, 805 - 130, 60, 130);
    osc_wave_1->setBounds (30, 110, 60, 130);
    osc_wave_2->setBounds (90 - 60, 420 - 130, 60, 130);
    sl_morhp_mix->setBounds (1165, 765, 180, 33);
    button_programm_delete->setBounds (1030 - 60, 10, 60, 27);
    filter_type_6_1->setBounds (1055 - 60, 110, 60, 30);
    filter_type_6_2->setBounds (1055 - 60, 290, 60, 30);
    filter_type_6_3->setBounds (1055 - 60, 470, 60, 30);
    button_ctrl_toggle->setBounds (100, 10, 60, 30);
    speed->setBounds (1285 - 60, 1000 - 130, 60, 130);
    button_open_morph->setBounds (1165 - 60, 765, 60, 33);
    effect_finalizer_switch->setBounds (520 - 25, 670, 25, 130);
    label_ui_headline2->setBounds (1105, 666, 60, 35);
    label_ui_headline3->setBounds (1165, 666, 60, 35);
    label_ui_headline5->setBounds (1225, 666, 60, 35);
    label_ui_headline6->setBounds (1285, 666, 60, 35);
    button_values_toggle->setBounds (30, 10, 60, 30);
    octave_offset->setBounds (1435 - 60, 870, 60, 130);
    label_ui_headline4->setBounds (245, 55, 120, 35);
    label_ui_headline7->setBounds (40, 55, 110, 35);
    label_ui_headline8->setBounds (510, 55, 130, 35);
    label_ui_headline9->setBounds (850, 55, 50, 35);
    label_ui_headline10->setBounds (975, 55, 190, 35);
    label_ui_headline11->setBounds (1220, 55, 110, 35);
    label_ui_headline12->setBounds (1375, 55, 60, 35);
    label_ui_headline13->setBounds (270, 630, 130, 35);
    label_shape->setBounds (525, 630, 60, 35);
    label_reverb->setBounds (840, 630, 80, 35);
    label_delay->setBounds (705, 630, 60, 35);
    label_chorus->setBounds (610, 630, 70, 35);
    label_fx_mix->setBounds (1005, 630, 60, 35);
    label_ui_headline19->setBounds (1135, 630, 180, 35);
    label_ui_headline20->setBounds (615, 830, 190, 35);
    label_ui_headline21->setBounds (100, 830, 60, 35);
    label_ui_headline22->setBounds (1260, 830, 60, 35);
    label_ui_headline23->setBounds (1375, 630, 60, 35);
    label_ui_headline24->setBounds (1375, 830, 60, 35);
    label_ui_headline25->setBounds (35, 613, 120, 35);
    volume_master_meter->setBounds (1375, 670, 60, 27);
    label_eq->setBounds (740, 630, 90, 35);
    button_open_config2->setBounds (1335 - 60, 40 - 30, 60, 30);
    label_ui_headline14->setBounds (745, 55, 80, 35);
    button_edit_input_env_1_1->setBounds (215, 205, 60, 33);
    button_edit_input_env_1_2->setBounds (275, 205, 60, 33);
    button_edit_input_env_1_3->setBounds (335, 205, 60, 33);
    button_edit_input_env_2_1->setBounds (215, 385, 60, 33);
    button_edit_input_env_2_2->setBounds (275, 385, 60, 33);
    button_edit_input_env_2_3->setBounds (335, 385, 60, 33);
    button_edit_input_env_3_1->setBounds (215, 565, 60, 33);
    button_edit_input_env_3_2->setBounds (275, 565, 60, 33);
    button_edit_input_env_3_3->setBounds (335, 565, 60, 33);
    button_edit_input_env_band_1->setBounds (525, 765, 60, 33);
    button_edit_input_env_band_2->setBounds (585, 765, 60, 33);
    button_edit_input_env_band_3->setBounds (660, 765, 60, 33);
    button_edit_input_env_band_4->setBounds (720, 765, 60, 33);
    button_edit_input_env_band_5->setBounds (790, 765, 60, 33);
    button_edit_input_env_band_6->setBounds (850, 765, 60, 33);
    button_edit_input_env_band_7->setBounds (920, 765, 60, 33);
    button_edit_env_chorus->setBounds (615, 765, 60, 33);
    flt_pan_3->setBounds (1335 - 60, 600 - 130, 60, 130);
    flt_pan_2->setBounds (1335 - 60, 420 - 130, 60, 130);
    flt_pan_1->setBounds (1335 - 60, 240 - 130, 60, 130);
    button_reset_arp_tune->setBounds (1435 - 60, 870, 60, 27);
    button_show_active_input_r_2_3->setBounds (376, 279, 10, 10);
    button_show_active_input_l_2_3->setBounds (346, 279, 10, 10);
    button_show_active_input_r_2_2->setBounds (316, 279, 10, 10);
    button_show_active_input_l_2_2->setBounds (286, 279, 10, 10);
    button_show_active_input_r_2_1->setBounds (256, 279, 10, 10);
    button_show_active_input_l_2_1->setBounds (226, 279, 10, 10);
    button_show_active_input_r_3_3->setBounds (376, 459, 10, 10);
    button_show_active_input_l_3_3->setBounds (346, 459, 10, 10);
    button_show_active_input_r_3_2->setBounds (316, 459, 10, 10);
    button_show_active_input_l_3_2->setBounds (286, 459, 10, 10);
    button_show_active_input_r_3_1->setBounds (256, 459, 10, 10);
    button_show_active_input_l_3_1->setBounds (225, 459, 10, 10);
    button_programm_rename->setBounds (970 - 60, 10, 60, 27);
    flt_shape_1->setBounds (785 - 60, 240 - 130, 60, 130);
    flt_shape_2->setBounds (785 - 60, 420 - 130, 60, 130);
    flt_shape_3->setBounds (785 - 60, 600 - 130, 60, 130);
    //[UserResized] Add your own custom resize handling here..
    if( resizer )
        resizer->setBounds (original_w - 16, original_h - 16, 16, 16);

    resize_subeditors();

#include "mono_ui_includeHacks_END.h"

    synth_data->ui_scale_factor = getHeight()/original_h;
    //[/UserResized]
}

void Monique_Ui_Mainwindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == filter_type_2_1)
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
        program_edit_type = EDIT_TYPES::LOAD;
        synth_data->ask_and_save_if_changed();
        synth_data->load_prev();
        show_programs_and_select(true);
        //[/UserButtonCode_button_programm_left]
    }
    else if (buttonThatWasClicked == button_programm_right)
    {
        //[UserButtonCode_button_programm_right] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::LOAD;
        synth_data->ask_and_save_if_changed();
        synth_data->load_next();
        show_programs_and_select(true);
        //[/UserButtonCode_button_programm_right]
    }
    else if (buttonThatWasClicked == button_programm_replace)
    {
        //[UserButtonCode_button_programm_replace] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::REPLACE;
        synth_data->replace();
        show_programs_and_select(true);
        //[/UserButtonCode_button_programm_replace]
    }
    else if (buttonThatWasClicked == button_programm_new)
    {
        //[UserButtonCode_button_programm_new] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::CREATE;
        combo_programm->setEditableText(true);
        combo_programm->showEditor();
        //[/UserButtonCode_button_programm_new]
    }
    else if (buttonThatWasClicked == button_open_oszi)
    {
        //[UserButtonCode_button_open_oszi] -- add your button handler code here..
        if( amp_painter )
        {
            audio_processor->amp_painter = nullptr;
            removeChildComponent( amp_painter );
            delete amp_painter;
            amp_painter = nullptr;

            synth_data->is_osci_open = false;
        }
        else
        {
            editor_midiio = nullptr;
            editor_morph = nullptr;
            editor_global_settings = nullptr;
            popup = nullptr;

            amp_painter = new Monique_Ui_AmpPainter(synth_data,look_and_feel);

            addAndMakeVisible( amp_painter );
            resize_subeditors();


            audio_processor->amp_painter = amp_painter;

            synth_data->is_osci_open = true;
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
            open_midi_editor_if_closed();
        }
        //[/UserButtonCode_button_open_midi_io_settings]
    }
    else if (buttonThatWasClicked == button_programm_load)
    {
        //[UserButtonCode_button_programm_load] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::LOAD;
        synth_data->load();
        show_programs_and_select(true);
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
        program_edit_type = EDIT_TYPES::REMOVE;
        if( synth_data->remove() )
        {
            show_programs_and_select(true);
        }
        //[/UserButtonCode_button_programm_delete]
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
            synth_data->filter_datas[flt_id]->filter_type = LPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_6_3]
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

            addAndMakeVisible( editor_morph = new Monique_Ui_MorphConfig( ui_refresher, look_and_feel ) );
            resize_subeditors();
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
        look_and_feel->show_values_always ^= true;
        //[/UserButtonCode_button_values_toggle]
    }
    else if (buttonThatWasClicked == button_open_config2)
    {
        //[UserButtonCode_button_open_config2] -- add your button handler code here..
        if( ! editor_global_settings )
        {
            close_all_subeditors();

            addAndMakeVisible( editor_global_settings = new Monique_Ui_GlobalSettings(ui_refresher) );
            resize_subeditors();
        }
        else
            editor_global_settings = nullptr;
        //[/UserButtonCode_button_open_config2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_1)
    {
        //[UserButtonCode_button_edit_input_env_1_1] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[0]->input_envs[0], &synth_data->filter_datas[0]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_1, false );
        //[/UserButtonCode_button_edit_input_env_1_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_2)
    {
        //[UserButtonCode_button_edit_input_env_1_2] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[0]->input_envs[1], &synth_data->filter_datas[0]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_2, false );
        //[/UserButtonCode_button_edit_input_env_1_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_3)
    {
        //[UserButtonCode_button_edit_input_env_1_3] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[0]->input_envs[2], &synth_data->filter_datas[0]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_3, false );
        //[/UserButtonCode_button_edit_input_env_1_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_1)
    {
        //[UserButtonCode_button_edit_input_env_2_1] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[1]->input_envs[0], &synth_data->filter_datas[1]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_6, true );
        //[/UserButtonCode_button_edit_input_env_2_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_2)
    {
        //[UserButtonCode_button_edit_input_env_2_2] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[1]->input_envs[1], &synth_data->filter_datas[1]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_7, true );
        //[/UserButtonCode_button_edit_input_env_2_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_3)
    {
        //[UserButtonCode_button_edit_input_env_2_3] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[1]->input_envs[2], &synth_data->filter_datas[1]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_8, true );
        //[/UserButtonCode_button_edit_input_env_1_3]
        //[/UserButtonCode_button_edit_input_env_2_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_1)
    {
        //[UserButtonCode_button_edit_input_env_3_1] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[2]->input_envs[0], &synth_data->filter_datas[2]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_11, true );
        //[/UserButtonCode_button_edit_input_env_3_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_2)
    {
        //[UserButtonCode_button_edit_input_env_3_2] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[2]->input_envs[1], &synth_data->filter_datas[2]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_12, true );
        //[/UserButtonCode_button_edit_input_env_3_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_3)
    {
        //[UserButtonCode_button_edit_input_env_3_3] -- add your button handler code here..
        open_env_popup( synth_data->filter_datas[2]->input_envs[2], &synth_data->filter_datas[2]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_13, true );
        //[/UserButtonCode_button_edit_input_env_3_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_1)
    {
        //[UserButtonCode_button_edit_input_env_band_1] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[0], &synth_data->eq_data->envs[0]->sustain, buttonThatWasClicked, eq_1, true );
        //[/UserButtonCode_button_edit_input_env_band_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_2)
    {
        //[UserButtonCode_button_edit_input_env_band_2] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[1], &synth_data->eq_data->envs[1]->sustain, buttonThatWasClicked, eq_2, true );
        //[/UserButtonCode_button_edit_input_env_band_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_3)
    {
        //[UserButtonCode_button_edit_input_env_band_3] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[2], &synth_data->eq_data->envs[2]->sustain, buttonThatWasClicked, eq_3, true );
        //[/UserButtonCode_button_edit_input_env_band_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_4)
    {
        //[UserButtonCode_button_edit_input_env_band_4] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[3], &synth_data->eq_data->envs[3]->sustain, buttonThatWasClicked, eq_4, true );
        //[/UserButtonCode_button_edit_input_env_band_4]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_5)
    {
        //[UserButtonCode_button_edit_input_env_band_5] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[4], &synth_data->eq_data->envs[4]->sustain, buttonThatWasClicked, eq_5, true );
        //[/UserButtonCode_button_edit_input_env_band_5]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_6)
    {
        //[UserButtonCode_button_edit_input_env_band_6] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[5], &synth_data->eq_data->envs[5]->sustain, buttonThatWasClicked, eq_6, true );
        //[/UserButtonCode_button_edit_input_env_band_6]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_7)
    {
        //[UserButtonCode_button_edit_input_env_band_7] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[6], &synth_data->eq_data->envs[6]->sustain, buttonThatWasClicked, eq_7, true );
        //[/UserButtonCode_button_edit_input_env_band_7]
    }
    else if (buttonThatWasClicked == button_edit_env_chorus)
    {
        //[UserButtonCode_button_edit_env_chorus] -- add your button handler code here..
        open_env_popup( synth_data->chorus_data->env_data, &synth_data->chorus_data->env_data->sustain, buttonThatWasClicked, chorus_modulation, false );
        //[/UserButtonCode_button_edit_env_chorus]
    }
    else if (buttonThatWasClicked == button_reset_arp_tune)
    {
        //[UserButtonCode_button_reset_arp_tune] -- add your button handler code here..
        audio_processor->noteOn( 1, 60+synth_data->note_offset.get_value()-24, 1.0f );
        audio_processor->noteOff( 1, 60+synth_data->note_offset.get_value()-24 );
        //[/UserButtonCode_button_reset_arp_tune]
    }
    else if (buttonThatWasClicked == button_programm_rename)
    {
        //[UserButtonCode_button_programm_rename] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::RENAME;
        combo_programm->setEditableText(true);
        combo_programm->showEditor();
        //[/UserButtonCode_button_programm_rename]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_Mainwindow::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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
            if( program_edit_type == CREATE )
            {
                synth_data->create_new(new_name);
            }
            else if( program_edit_type = RENAME )
            {
                synth_data->rename(new_name);
            }
            show_programs_and_select(true);
        }
        else
        {
            String program_to_load = combo_programm->getText();
            synth_data->ask_and_save_if_changed();
            int new_index = synth_data->get_current_bank_programms().indexOf(program_to_load);
            synth_data->set_current_program( new_index );
            synth_data->load();
        }
        program_edit_type = NOT_SET;
        //[/UserComboBoxCode_combo_programm]
    }
    else if (comboBoxThatHasChanged == combo_bank)
    {
        //[UserComboBoxCode_combo_bank] -- add your combo box handling code here..
        synth_data->set_current_bank( combo_bank->getSelectedItemIndex() );
        show_programs_and_select(true);
        //[/UserComboBoxCode_combo_bank]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_Mainwindow::sliderValueChanged (Slider* sliderThatWasMoved)
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

bool Monique_Ui_Mainwindow::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        midi_control_handler->clear();

        synth_data->ctrl = false;

        if( env_popup )
        {
            open_env_popup( nullptr, nullptr, nullptr, nullptr, false );
        }
        else
        {
            close_all_subeditors();
        }

        success = true;
    }
    else if( key.getTextDescription() == "ctrl + +")
    {
        synth_data->ui_scale_factor = synth_data->ui_scale_factor + 0.1;
        update_size();
        success = true;
    }
    else if( key.getTextDescription() == "ctrl + -" )
    {
        synth_data->ui_scale_factor = synth_data->ui_scale_factor - 0.1;
        update_size();
        success = true;
    }
    else if( key.getTextDescription() == "ctrl + H" )
    {
        update_tooltip_handling(true);
        success = true;
    }
    else if( key.getTextDescription() == "ctrl + M" )
    {
        midi_control_handler->toggle_midi_learn();
        const Desktop& desktop = Desktop::getInstance();
        bool found = false;
        for (int i = 0; i != desktop.getNumMouseSources() ; ++i )
        {
            const MouseInputSource* const mi = desktop.getMouseSource(i);
            Component* const c = mi->getComponentUnderMouse();
            if( c )
            {
                if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( c ) )
                {
                    {
                        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
                        (
                            slider->get_front_parameter(),
                            c
                        )
                    }
                    show_info_popup(c,slider->get_front_parameter()->midi_control);
                    found = true;
                    break;
                }
                else
                {
                    Component*parent = c->getParentComponent();
                    if( parent )
                    {
                        do
                        {
                            if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( parent ) )
                            {
                                {
                                    IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
                                    (
                                        slider->get_front_parameter(),
                                        c
                                    )
                                }

                                show_info_popup(c,slider->get_front_parameter()->midi_control);
                                found = true;
                                break;
                            }
                        }
                        while( parent = parent->getParentComponent() );
                    }
                }
            }

            if( found )
            {
                break;
            }
        }

        success = true;
    }
#ifdef IS_PLUGIN
    else if( key.getTextDescription() == "F11" )
    {
        if (this->getPeer() != nullptr)
        {
            if( Desktop::getInstance().getKioskModeComponent() )
            {
                Desktop::getInstance().setKioskModeComponent (nullptr);
            }
            else
            {
                Desktop::getInstance().setKioskModeComponent (this);
            }
            success = true;
        }
    }
#endif

    return success;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool Monique_Ui_Mainwindow::keyStateChanged (const bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}

void Monique_Ui_Mainwindow::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    is_ctrl_down = modifiers.isCtrlDown();
    if( not combo_programm->isTextEditable() )
    {
        if( not is_ctrl_down )
        {
            update_tooltip_handling(false);
        }

        const bool is_shift_down = modifiers.isShiftDown();

        synth_data->ctrl = is_shift_down;
        show_ctrl_state();

        look_and_feel->show_values_always = is_ctrl_down;
    }

    //else
    //    Component::modifierKeysChanged( modifiers );
    //[/UserCode_modifierKeysChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Monique_Ui_Mainwindow::close_all_subeditors()
{
    editor_midiio = nullptr;
    editor_morph = nullptr;
    editor_global_settings = nullptr;
    popup = nullptr;
    option_popup = nullptr;

    if( amp_painter )
    {
        audio_processor->amp_painter = nullptr;
        delete amp_painter;
        amp_painter = nullptr;
    }
}
void Monique_Ui_Mainwindow::open_env_popup( ENVData*const env_data_,
        Parameter*const sustain_,
        Button*const for_comp_,
        Monique_Ui_DualSlider*slider_,
        bool has_negative_sustain_ ) noexcept
{
    ComponentColours& colours = look_and_feel->colours;
    Colour button_off = colours.button_off_colour;
    Colour button_on = colours.button_on_colour;
    button_edit_input_env_1_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_1_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_1_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_2_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_3_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_1->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_2->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_3->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_4->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_5->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_6->setColour( TextButton::buttonColourId, button_off );
    button_edit_input_env_band_7->setColour( TextButton::buttonColourId, button_off );
    button_edit_env_chorus->setColour( TextButton::buttonColourId, button_off );

    if( ! env_popup  )
    {
        if( env_data_ )
        {
            Array<Component*> comps_to_observe;
            if( popup ) // IGNORE POPUP
            {
                removeChildComponent(popup);
            }
            if( option_popup ) // IGNORE POPUP
            {
                removeChildComponent(option_popup);
            }
            for( int i = 0 ;  i != getNumChildComponents() ; ++i )
            {
                comps_to_observe.add( getChildComponent(i) );
            }
            comps_to_observe.removeFirstMatchingValue(for_comp_);
            comps_to_observe.add(this);
            if( popup )
            {
                addChildComponent(popup);
            }
            if( option_popup )
            {
                addChildComponent(option_popup);
            }

            addAndMakeVisible( env_popup = new Monique_Ui_ENVPopup( ui_refresher, this, env_data_, sustain_, for_comp_->getX() < getWidth()/2, has_negative_sustain_ ) );
            env_popup->set_element_to_show(for_comp_,slider_);
            env_popup->set_clickable_components( comps_to_observe );
            for_comp_->setColour( TextButton::buttonColourId, button_on );
            resize_subeditors();
        }
    }
    else
    {
        if( env_popup->is_open_for() == env_data_ )
        {
            env_popup = nullptr;
        }
        else if( env_data_ == nullptr )
        {
            env_popup = nullptr;
        }
        else
        {
            env_popup = nullptr;
            open_env_popup( env_data_, sustain_, for_comp_, slider_, has_negative_sustain_ );
        }
    }
}
void Monique_Ui_Mainwindow::open_env_popup( Monique_Ui_DualSlider*dual_slider_ ) noexcept
{
    if( dual_slider_ == flt_input_1 )
    {
        buttonClicked(button_edit_input_env_1_1);
    }
    else if( dual_slider_ == flt_input_2 )
    {
        buttonClicked(button_edit_input_env_1_2);
    }
    else if( dual_slider_ == flt_input_3 )
    {
        buttonClicked(button_edit_input_env_1_3);
    }
    else if( dual_slider_ == flt_input_6 )
    {
        buttonClicked(button_edit_input_env_2_1);
    }
    else if( dual_slider_ == flt_input_7 )
    {
        buttonClicked(button_edit_input_env_2_2);
    }
    else if( dual_slider_ == flt_input_8 )
    {
        buttonClicked(button_edit_input_env_2_3);
    }
    else if( dual_slider_ == flt_input_11 )
    {
        buttonClicked(button_edit_input_env_3_1);
    }
    else if( dual_slider_ == flt_input_12 )
    {
        buttonClicked(button_edit_input_env_3_2);
    }
    else if( dual_slider_ == flt_input_13 )
    {
        buttonClicked(button_edit_input_env_3_3);
    }
    else if( dual_slider_ == chorus_modulation )
    {
        buttonClicked(button_edit_env_chorus);
    }
    else if( dual_slider_ == eq_1 )
    {
        buttonClicked(button_edit_input_env_band_1);
    }
    else if( dual_slider_ == eq_2 )
    {
        buttonClicked(button_edit_input_env_band_2);
    }
    else if( dual_slider_ == eq_3 )
    {
        buttonClicked(button_edit_input_env_band_3);
    }
    else if( dual_slider_ == eq_4 )
    {
        buttonClicked(button_edit_input_env_band_4);
    }
    else if( dual_slider_ == eq_5 )
    {
        buttonClicked(button_edit_input_env_band_5);
    }
    else if( dual_slider_ == eq_6 )
    {
        buttonClicked(button_edit_input_env_band_6);
    }
    else if( dual_slider_ == eq_7 )
    {
        buttonClicked(button_edit_input_env_band_7);
    }
}
void Monique_Ui_Mainwindow::open_option_popup(
    Component*const for_comp_,
    BoolParameter*param_a_, BoolParameter*param_b_,
    StringRef text_a_, StringRef text_b_,
    StringRef tool_tip_a_, StringRef tool_tip_b_ ) noexcept
{
    if( option_popup )
    {
        if( Desktop::getInstance().getMainMouseSource().getComponentUnderMouse() != option_popup )
        {
            option_popup = nullptr;
            if( for_comp_ and param_a_ and param_b_ )
            {
                open_option_popup( for_comp_, param_a_, param_b_, text_a_, text_b_, tool_tip_a_, tool_tip_b_ );
            }
        }
    }
    else
    {
        addAndMakeVisible( option_popup = new Monique_Ui_OptionPopup( ui_refresher, this, param_a_, param_b_ ) );
        option_popup->set_element_to_show(for_comp_);
	option_popup->set_infos(text_a_, text_b_, tool_tip_a_, tool_tip_b_);
        resize_subeditors();
    }
}

bool Monique_Ui_Mainwindow::handle_keep_env_open( ModulationSliderConfigBase*const caller_config_ ) noexcept
{
    bool keeps_open = false;
    if( env_popup )
    {
        if( not caller_config_->action_keep_env_pop_open_for( env_popup->is_open_for() ) )
        {
            if( synth_data->auto_close_env_popup )
            {
                open_env_popup(nullptr,nullptr,nullptr,nullptr,false);
            }
        }
        else
        {
            keeps_open = true;
        }
    }

    return keeps_open;
}

void Monique_Ui_Mainwindow::resize_subeditors()
{
#ifdef IS_STANDALONE
    if( editor_midiio )
    {
        editor_midiio->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight());
    }
#endif
    if( editor_morph )
    {
        editor_morph->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight());
    }
    if( editor_global_settings )
    {
        editor_global_settings->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight());
    }
    if( popup )
    {
        popup->setSize( popup->original_w*(1.0f/original_w*getWidth()), popup->original_h*(1.0f/original_h*getHeight() ) );
        popup->update_positions();
    }
    if( env_popup )
    {
        env_popup->setSize( env_popup->original_w*(1.0f/original_w*getWidth()), env_popup->original_h*(1.0f/original_h*getHeight() ) );
        env_popup->update_positions();
    }
    if( option_popup )
    {
        option_popup->setSize( option_popup->original_w*(1.0f/original_w*getWidth()), option_popup->original_h*(1.0f/original_h*getHeight() ) );
        option_popup->update_positions();
    }

    if( amp_painter )
    {
        amp_painter->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight());
    }

    resizer->toFront(true);
}

void Monique_Ui_Mainwindow::open_midi_editor_if_closed() noexcept
{
#ifdef IS_STANDALONE
    if( not editor_midiio )
    {
        close_all_subeditors();

        addAndMakeVisible( editor_midiio = new Monique_Ui_MidiIO( ui_refresher, audio_processor ) );
        resize_subeditors();
    }
#else
    midi_control_handler->toggle_midi_learn();
    show_info_popup(nullptr,nullptr);
#endif
}
void Monique_Ui_Mainwindow::open_setup_editor_if_closed() noexcept
{
    if( not editor_global_settings )
    {
        close_all_subeditors();

        addAndMakeVisible( editor_global_settings = new Monique_Ui_GlobalSettings(ui_refresher) );
        resize_subeditors();
    }
}

void Monique_Ui_Mainwindow::flash_midi_editor_button() noexcept
{
    flash_counter = 30;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_Mainwindow" componentName=""
                 parentClasses="public AudioProcessorEditor, public Monique_Ui_Refreshable, public AsyncUpdater"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_" variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;AudioProcessorEditor(ui_refresher_-&gt;audio_processor),&#10;original_w(1465), original_h(1235)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="1235">
  <METHODS>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="keyStateChanged (const bool isKeyDown)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="480 645 605 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="10 845 1445 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 70 1260 195" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="10 70 170 754" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <RECT pos="175 94 25 3" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="175 99 25 3" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="170 100 181 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="175 89 25 3" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="195 445 1260 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 265 1260 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="195 645 285 180" cornerSize="6" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="20 110 1 541" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="605 100 226 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 650 76 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 880 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="90 880 10 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 105 340 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 655 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 650 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="910 105 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="830 100 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="615 810 421 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="970 805 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="615 805 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1035 805 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1015 805 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 95 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="930 100 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 100 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 95 566 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 100 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1260 770 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1265 765 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="605 100 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1095 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1155 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1245 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="300 250 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="240 250 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="360 250 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 465 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 460 76 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 460 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 290 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 110 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 235 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 415 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 470 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 595 5 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 105 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 90 1 16" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 95 1 185" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="170 100 1 360" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="40 285 110 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 280 71 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 280 1 6" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="289 95 3 10" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="290 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="165 95 126 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="350 100 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="229 90 3 16" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="230 90 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="95 90 136 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="259 255 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="319 255 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="379 255 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="229 245 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="289 245 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="349 245 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="350 245 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="290 245 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="230 245 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="380 250 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="320 250 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="260 250 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1085 645 270 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="1355 645 100 180" cornerSize="6" fill="solid: ff050505"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff1111ff"/>
    <ROUNDRECT pos="605 280 226 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 285 340 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="910 285 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="830 280 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 275 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="930 280 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 280 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 275 566 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 280 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="605 280 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1095 275 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1155 275 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 275 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1245 275 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="605 460 226 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="435 465 340 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="910 465 40 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="830 460 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 455 1 15" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="930 460 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 460 81 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="840 455 566 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="850 460 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="605 460 1 5" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1095 455 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1155 455 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1405 455 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1245 455 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="300 430 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="240 430 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="360 430 25 1" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <RECT pos="259 435 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="319 435 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="379 435 3 20" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="229 425 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="289 425 3 30" fill="solid: ff050505" hasStroke="0"/>
    <RECT pos="349 425 3 30" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="350 425 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="290 425 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="230 425 1 40" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="380 430 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="320 430 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="260 430 1 35" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1305 275 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1305 95 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="1305 455 1 10" cornerSize="1" fill="solid: ff11ffff" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="b482d3e604966296" memberName="distortion" virtualName=""
                    explicitFocusOrder="0" pos="585r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FXDistortionSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="19311f1c6e549e68" memberName="reverb_room" virtualName=""
                    explicitFocusOrder="0" pos="855r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new RRoomSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="1e7a797188cff129" memberName="reverb_width" virtualName=""
                    explicitFocusOrder="0" pos="915r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new RWidthSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9d2507984890a079" memberName="reverb_dry" virtualName=""
                    explicitFocusOrder="0" pos="975r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new RDrySlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="83c667b94dd3ef45" memberName="bypass" virtualName=""
                    explicitFocusOrder="0" pos="1065r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new BypassConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="5d07e2bb48e90cc6" memberName="eq_1" virtualName=""
                    explicitFocusOrder="0" pos="585r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="1dbf561cd93cbd59" memberName="eq_3" virtualName=""
                    explicitFocusOrder="0" pos="720r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="3b0e3a8ef55d061a" memberName="eq_4" virtualName=""
                    explicitFocusOrder="0" pos="780r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="8a0f89a0c0f219b8" memberName="eq_5" virtualName=""
                    explicitFocusOrder="0" pos="850r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,4)"/>
  <GENERICCOMPONENT name="" id="2b128fb147c2823c" memberName="eq_6" virtualName=""
                    explicitFocusOrder="0" pos="910r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,5)"/>
  <GENERICCOMPONENT name="" id="12a573d837478d38" memberName="eq_7" virtualName=""
                    explicitFocusOrder="0" pos="980r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,6)"/>
  <GENERICCOMPONENT name="" id="1f9f546ceacaa4b2" memberName="colour" virtualName=""
                    explicitFocusOrder="0" pos="1065r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FColourSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="49d3d717347ff877" memberName="delay2" virtualName=""
                    explicitFocusOrder="0" pos="765r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new DelaySlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9378cae1ce589256" memberName="chorus_modulation"
                    virtualName="" explicitFocusOrder="0" pos="615 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new CModSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="30a759af59bc090b" memberName="eq_2" virtualName=""
                    explicitFocusOrder="0" pos="645r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,1)"/>
  <LABEL name="" id="4a610cd12c392ab8" memberName="label_effect_hider"
         virtualName="" explicitFocusOrder="0" pos="1065r 815r 540 15"
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText=""
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="DL" id="4c9a611da59481e8" memberName="label_band_hz_2"
         virtualName="" explicitFocusOrder="0" pos="590 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="160Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="9e42519baddf946e" memberName="label_band_hz_3"
         virtualName="" explicitFocusOrder="0" pos="665 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="300Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ec2bcb9f1c216890" memberName="label_band_hz_4"
         virtualName="" explicitFocusOrder="0" pos="725 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="600Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="89a933c800933b8f" memberName="label_band_hz_5"
         virtualName="" explicitFocusOrder="0" pos="795 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="1.2kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="8a859696dca2f1f0" memberName="label_band_hz_6"
         virtualName="" explicitFocusOrder="0" pos="855 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="2.5kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ceab7b618b72215f" memberName="label_band_hz_7"
         virtualName="" explicitFocusOrder="0" pos="925 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="&gt;2.5kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="b8619ebb44316d58" memberName="label_band_hz_1"
         virtualName="" explicitFocusOrder="0" pos="530 795 50 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="&lt;80Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="8916123bb68766dc" memberName="speed_multi" virtualName=""
                    explicitFocusOrder="0" pos="1355r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new SpeedMultiSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="f1f5ea6816f11113" memberName="morpher_4" virtualName=""
                    explicitFocusOrder="0" pos="1345r 670 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="6319f13308da05dc" memberName="morpher_3" virtualName=""
                    explicitFocusOrder="0" pos="1285r 670 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="d7bed13dc76b014a" memberName="morpher_2" virtualName=""
                    explicitFocusOrder="0" pos="1225r 670 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="ab7bfe937e5ada83" memberName="morpher_1" virtualName=""
                    explicitFocusOrder="0" pos="1165r 670 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="92e86ca444a56d1e" memberName="osc_wave_3" virtualName=""
                    explicitFocusOrder="0" pos="90r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="a8343a0b5df2dc06" memberName="keyboard" virtualName="MidiKeyboardComponent"
                    explicitFocusOrder="0" pos="0 1055 1465 180" class="Component"
                    params="*reinterpret_cast&lt; MoniqueAudioProcessor* &gt;( &amp;processor ), MidiKeyboardComponent::horizontalKeyboard"/>
  <GENERICCOMPONENT name="" id="35003b6b21577713" memberName="glide2" virtualName=""
                    explicitFocusOrder="0" pos="100 870 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GlideConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="d8ef93ac038fadca" memberName="arp_step_16" virtualName=""
                    explicitFocusOrder="0" pos="1205r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,15)"/>
  <GENERICCOMPONENT name="" id="7761deb0276debbd" memberName="arp_step_15" virtualName=""
                    explicitFocusOrder="0" pos="1145r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,14)"/>
  <GENERICCOMPONENT name="" id="20a9ed6504a039e2" memberName="arp_step_14" virtualName=""
                    explicitFocusOrder="0" pos="1085r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,13)"/>
  <GENERICCOMPONENT name="" id="791739ade4aee5df" memberName="arp_step_13" virtualName=""
                    explicitFocusOrder="0" pos="965 870 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,12)"/>
  <GENERICCOMPONENT name="" id="1e3ef8bba1be4b28" memberName="arp_step_12" virtualName=""
                    explicitFocusOrder="0" pos="955r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,11)"/>
  <GENERICCOMPONENT name="" id="fe823ea88a7a2471" memberName="arp_step_11" virtualName=""
                    explicitFocusOrder="0" pos="895r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,10)"/>
  <GENERICCOMPONENT name="" id="ee7d6057133dde55" memberName="arp_step_10" virtualName=""
                    explicitFocusOrder="0" pos="835r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,9)"/>
  <GENERICCOMPONENT name="" id="b4852f8bf0385747" memberName="arp_step_9" virtualName=""
                    explicitFocusOrder="0" pos="775r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,8)"/>
  <GENERICCOMPONENT name="" id="fd84ed45f47ab8b9" memberName="arp_step_8" virtualName=""
                    explicitFocusOrder="0" pos="705r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,7)"/>
  <GENERICCOMPONENT name="" id="cf5a0e63bd7f558a" memberName="arp_step_7" virtualName=""
                    explicitFocusOrder="0" pos="645r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,6)"/>
  <GENERICCOMPONENT name="" id="31712e752afeb9b5" memberName="arp_step_6" virtualName=""
                    explicitFocusOrder="0" pos="585r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,5)"/>
  <GENERICCOMPONENT name="" id="a729cce2b51e5737" memberName="arp_step_5" virtualName=""
                    explicitFocusOrder="0" pos="525r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,4)"/>
  <GENERICCOMPONENT name="" id="4ea4b03b58657c40" memberName="arp_step_4" virtualName=""
                    explicitFocusOrder="0" pos="455r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="b45b0bde6cb27e9d" memberName="arp_step_3" virtualName=""
                    explicitFocusOrder="0" pos="395r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="be72c3cee3e34864" memberName="arp_step_2" virtualName=""
                    explicitFocusOrder="0" pos="335r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="6665063ca7bdff41" memberName="arp_step_1" virtualName=""
                    explicitFocusOrder="0" pos="275r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="bb9c66366ce375c" memberName="shuffle" virtualName=""
                    explicitFocusOrder="0" pos="30 870 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ShuffleConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="6e5608d47c1be7c4" memberName="flt_sustain_4" virtualName=""
                    explicitFocusOrder="0" pos="395r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FSustainSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="8386fe429fe8a2e6" memberName="flt_decay_4" virtualName=""
                    explicitFocusOrder="0" pos="335r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FDecaySlConfig(synth_data)&#10;"/>
  <GENERICCOMPONENT name="" id="bb503e115ddb6edb" memberName="flt_attack_4" virtualName=""
                    explicitFocusOrder="0" pos="275r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FAttackSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="ca4537ccb809ca96" memberName="flt_release_3" virtualName=""
                    explicitFocusOrder="0" pos="725r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="a60fcd747c533a26" memberName="flt_sustain_time_3"
                    virtualName="" explicitFocusOrder="0" pos="665r 600r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="78d4de9e0ffe3029" memberName="flt_sustain_3" virtualName=""
                    explicitFocusOrder="0" pos="605r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="e8b49d00205726e6" memberName="flt_decay_3" virtualName=""
                    explicitFocusOrder="0" pos="545r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="638e13e96c94deb1" memberName="flt_attack_3" virtualName=""
                    explicitFocusOrder="0" pos="485r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FAttackSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="4e206df1142d5f1d" memberName="flt_release_2" virtualName=""
                    explicitFocusOrder="0" pos="725r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="b2e468ddbdcb0be8" memberName="flt_sustain_time_2"
                    virtualName="" explicitFocusOrder="0" pos="665r 420r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="aa6aa381eebdd61" memberName="flt_sustain_2" virtualName=""
                    explicitFocusOrder="0" pos="605r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="140fc1e77383e0f9" memberName="flt_decay_2" virtualName=""
                    explicitFocusOrder="0" pos="545r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="8a6f21a1f4a86dd" memberName="flt_attack_2" virtualName=""
                    explicitFocusOrder="0" pos="485r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FAttackSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="b17b21322ed6df73" memberName="flt_release_1" virtualName=""
                    explicitFocusOrder="0" pos="725r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="1460533da718423d" memberName="flt_sustain_time_1"
                    virtualName="" explicitFocusOrder="0" pos="665r 240r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="16f83a5a025850d0" memberName="flt_sustain_1" virtualName=""
                    explicitFocusOrder="0" pos="605r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="dc67a284425c81d9" memberName="flt_decay_1" virtualName=""
                    explicitFocusOrder="0" pos="545r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="1a64935d9407f5bb" memberName="flt_attack_1" virtualName=""
                    explicitFocusOrder="0" pos="425 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher,&#10;new FAttackSlConfig(synth_data,0)"/>
  <LABEL name="" id="39e8fb50cf1d668d" memberName="label_monolisa" virtualName=""
         explicitFocusOrder="0" pos="1220r 50r 190 50" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="M O N I Q U E" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="VOICE 1" id="ab74fcbdb09aa48f" memberName="filter_type_2_1"
              virtualName="" explicitFocusOrder="0" pos="1055r 140 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="cf1d3ce65d7cdcdc" memberName="filter_type_3_1"
              virtualName="" explicitFocusOrder="0" pos="1055r 170 60 30" tooltip="Set the filter type to BAND PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="121af38bc5254d57" memberName="filter_type_2_2"
              virtualName="" explicitFocusOrder="0" pos="1055r 320 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="43c53216b803243b" memberName="filter_type_3_2"
              virtualName="" explicitFocusOrder="0" pos="1055r 377r 60 30"
              tooltip="Set the filter type to BAND PASS." bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="e53892e80132f60c" memberName="filter_type_2_3"
              virtualName="" explicitFocusOrder="0" pos="1055r 500 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="f44bd17c008d0db3" memberName="filter_type_3_3"
              virtualName="" explicitFocusOrder="0" pos="1055r 530 60 30" tooltip="Set the filter type to BAND PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c1a5cea82178d7f1" memberName="filter_type_5_1"
              virtualName="" explicitFocusOrder="0" pos="1055r 205 60 33" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="375699baffcdc070" memberName="filter_type_5_2"
              virtualName="" explicitFocusOrder="0" pos="1055r 385 60 33" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="74deee6f861b7bf5" memberName="filter_type_5_3"
              virtualName="" explicitFocusOrder="0" pos="1055r 565 60 33" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7a60e9dcf8b32a0a" memberName="button_sequence_2"
              virtualName="" explicitFocusOrder="0" pos="335r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="17704b0ee658c01b" memberName="button_sequence_3"
              virtualName="" explicitFocusOrder="0" pos="395r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="70bcd1e56b41c2c6" memberName="button_sequence_4"
              virtualName="" explicitFocusOrder="0" pos="455r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e835074126c3a82d" memberName="button_sequence_5"
              virtualName="" explicitFocusOrder="0" pos="525r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7ebcf311504b804b" memberName="button_sequence_6"
              virtualName="" explicitFocusOrder="0" pos="585r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7964742ab1d9d236" memberName="button_sequence_7"
              virtualName="" explicitFocusOrder="0" pos="645r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="290ac159e50889a3" memberName="button_sequence_8"
              virtualName="" explicitFocusOrder="0" pos="705r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="444c07bec0f97ff4" memberName="button_sequence_9"
              virtualName="" explicitFocusOrder="0" pos="775r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d8ad414b935f59d8" memberName="button_sequence_10"
              virtualName="" explicitFocusOrder="0" pos="835r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="af5f2477751cef2c" memberName="button_sequence_11"
              virtualName="" explicitFocusOrder="0" pos="895r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d3687d6b28982234" memberName="button_sequence_12"
              virtualName="" explicitFocusOrder="0" pos="955r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="804115318ad213c1" memberName="button_sequence_13"
              virtualName="" explicitFocusOrder="0" pos="1025r 890r 60 20"
              tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="13d758647984d5d5" memberName="button_sequence_14"
              virtualName="" explicitFocusOrder="0" pos="1085r 890r 60 20"
              tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8cbd35271097248c" memberName="button_sequence_15"
              virtualName="" explicitFocusOrder="0" pos="1145r 890r 60 20"
              tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2370645873984939" memberName="button_sequence_16"
              virtualName="" explicitFocusOrder="0" pos="1205r 890r 60 20"
              tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="7c9b1844748d88e" memberName="combo_programm" virtualName=""
            explicitFocusOrder="0" pos="335 10 315 27" tooltip="Select and load a program of the selected bank (one box left)."
            editable="1" layout="33" items="" textWhenNonSelected="DEFAULT"
            textWhenNoItems="EMPTY BANK"/>
  <TEXTBUTTON name="" id="dd0cd965aaddf5ba" memberName="button_programm_left"
              virtualName="" explicitFocusOrder="0" pos="215 10 60 27" tooltip="Load the program before of the selected bank (right of this button)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&lt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6ccb0337451b3a26" memberName="button_programm_right"
              virtualName="" explicitFocusOrder="0" pos="710r 10 60 27" tooltip="Load the next program of the selected bank."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&gt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b91a29c51d2d93f1" memberName="button_programm_replace"
              virtualName="" explicitFocusOrder="0" pos="910r 10 60 27" tooltip="Replaces the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="REPLACE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9481aeac211cafc0" memberName="button_programm_new"
              virtualName="" explicitFocusOrder="0" pos="850r 10 60 27" tooltip="Create a new program from the current state."
              bgColOff="ff000000" textCol="ffbcff00" textColOn="ffd0ff00" buttonText="SAVE AS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e5ff1639c5671984" memberName="button_open_oszi" virtualName=""
              explicitFocusOrder="0" pos="1375 10 60 30" tooltip="Open/Close the oscilloscope.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="OSCI"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="57c6c986fc98dac" memberName="button_open_midi_io_settings"
              virtualName="" explicitFocusOrder="0" pos="1280r 40r 60 30" tooltip="Open/Close the MIDI settings.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="MIDI"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_bank" virtualName=""
            explicitFocusOrder="0" pos="275 10 60 27" tooltip="Select the current program bank."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="" id="aa7c44443637097c" memberName="button_programm_load"
              virtualName="" explicitFocusOrder="0" pos="770r 10 60 27" tooltip="Load the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LOAD"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="e8e2f9e6488018da" memberName="osc_1" virtualName=""
                    explicitFocusOrder="0" pos="100 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="308060a72bcb3066" memberName="osc_2" virtualName=""
                    explicitFocusOrder="0" pos="160r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="17d8341f811bcb5a" memberName="osc_3" virtualName=""
                    explicitFocusOrder="0" pos="160r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="2a31f2713e80bed3" memberName="lfo_1" virtualName=""
                    explicitFocusOrder="0" pos="960r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="ffb8076636239778" memberName="flt_cutoff_1" virtualName=""
                    explicitFocusOrder="0" pos="1125r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="e36ec1f3ea5f1edf" memberName="lfo_2" virtualName=""
                    explicitFocusOrder="0" pos="960r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="10b142e0e3bd1edf" memberName="lfo_3" virtualName=""
                    explicitFocusOrder="0" pos="960r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="9eb8d35cf54eee3" memberName="flt_cutoff_2" virtualName=""
                    explicitFocusOrder="0" pos="1125r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="d7143931caaf1976" memberName="flt_cutoff_3" virtualName=""
                    explicitFocusOrder="0" pos="1125r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="ecbcc81adebe9850" memberName="flt_input_1" virtualName=""
                    explicitFocusOrder="0" pos="215 110 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,0)"/>
  <GENERICCOMPONENT name="" id="6af45f57190e5260" memberName="flt_input_2" virtualName=""
                    explicitFocusOrder="0" pos="335r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,1)"/>
  <GENERICCOMPONENT name="" id="9abcdbe824977dbc" memberName="flt_input_3" virtualName=""
                    explicitFocusOrder="0" pos="395r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,2)"/>
  <GENERICCOMPONENT name="" id="16470f25818b13ce" memberName="flt_distortion_1" virtualName=""
                    explicitFocusOrder="0" pos="1275r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="cc59ad897708e932" memberName="flt_input_6" virtualName=""
                    explicitFocusOrder="0" pos="275r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,0)"/>
  <GENERICCOMPONENT name="" id="30402f9a5bf56bfb" memberName="flt_input_7" virtualName=""
                    explicitFocusOrder="0" pos="335r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,1)"/>
  <GENERICCOMPONENT name="" id="e54fd10f87874627" memberName="flt_input_8" virtualName=""
                    explicitFocusOrder="0" pos="395r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,2)"/>
  <GENERICCOMPONENT name="" id="f3d6d4daa7867cda" memberName="flt_input_11" virtualName=""
                    explicitFocusOrder="0" pos="275r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,0)"/>
  <GENERICCOMPONENT name="" id="7371ee7afd1877b4" memberName="flt_input_12" virtualName=""
                    explicitFocusOrder="0" pos="335r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,1)"/>
  <GENERICCOMPONENT name="" id="d2e2be5869047a2e" memberName="flt_input_13" virtualName=""
                    explicitFocusOrder="0" pos="395r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,2)"/>
  <GENERICCOMPONENT name="" id="31da059865f2567b" memberName="flt_resonance_1" virtualName=""
                    explicitFocusOrder="0" pos="1185r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="75550ba5bb7ce4e0" memberName="flt_resonance_2" virtualName=""
                    explicitFocusOrder="0" pos="1185r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="aa2b2c2864221426" memberName="flt_resonance_3" virtualName=""
                    explicitFocusOrder="0" pos="1185r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="ba71384f051dd23" memberName="flt_volume_1" virtualName=""
                    explicitFocusOrder="0" pos="1375 110 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="32dd3f586d1d81eb" memberName="flt_volume_2" virtualName=""
                    explicitFocusOrder="0" pos="1435r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="18f72cc654c99917" memberName="flt_volume_3" virtualName=""
                    explicitFocusOrder="0" pos="1435r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="68cebc996c492894" memberName="adsr_lfo_mix" virtualName=""
                    explicitFocusOrder="0" pos="870r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="944e7d4439e86773" memberName="lfo_opt_2" virtualName=""
                    explicitFocusOrder="0" pos="870r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="2d0d9d7f81f143" memberName="lfo_opt_3" virtualName=""
                    explicitFocusOrder="0" pos="870r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,2)"/>
  <TEXTBUTTON name="" id="9669ee100bf4ee95" memberName="button_sequence_1"
              virtualName="" explicitFocusOrder="0" pos="275r 890r 60 20" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="3eaa1962698c14dc" memberName="flt_release_4" virtualName=""
                    explicitFocusOrder="0" pos="455r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FReleaseSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="94c6b03ecc4d4642" memberName="volume" virtualName=""
                    explicitFocusOrder="0" pos="1435r 800r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new VolumeConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9771b840efca92c2" memberName="flt_distortion_2" virtualName=""
                    explicitFocusOrder="0" pos="1275r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="766d923ef01630c7" memberName="flt_distortion_3" virtualName=""
                    explicitFocusOrder="0" pos="1275r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,2)"/>
  <TEXTBUTTON name="" id="28379674f941d830" memberName="button_arp_speed_XNORM"
              virtualName="" explicitFocusOrder="0" pos="1295 870 60 27" tooltip="Shortcut to set the speed multiplier back to 1x (in sync)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="x1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="9f8319dda0065826" memberName="flt_attack_5" virtualName=""
                    explicitFocusOrder="0" pos="90r 805r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FMFreqSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="53fd0bab31e1ce" memberName="flt_attack_6" virtualName=""
                    explicitFocusOrder="0" pos="160r 805r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FMAmountSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="7abd69d58b16456c" memberName="osc_wave_1" virtualName=""
                    explicitFocusOrder="0" pos="30 110 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="289652ee3553683c" memberName="osc_wave_2" virtualName=""
                    explicitFocusOrder="0" pos="90r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,1)"/>
  <SLIDER name="new slider" id="20de89a2be986cc1" memberName="sl_morhp_mix"
          virtualName="" explicitFocusOrder="0" pos="1165 765 180 33" tooltip="Morph between the morph sliders. &#10;&#10;Take a look at the MORPH MOTOR time in the SETUP to control the speed of this morph."
          min="0" max="3000" int="0.010000000000000000208" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="87db63f1017ff04b" memberName="button_programm_delete"
              virtualName="" explicitFocusOrder="0" pos="1030r 10 60 27" tooltip="Delete the selected program."
              bgColOff="ff000000" textCol="ffff0000" textColOn="ffff7900" buttonText="DELETE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="bcfd327216c64e93" memberName="filter_type_6_1"
              virtualName="" explicitFocusOrder="0" pos="1055r 110 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="78dd95fdf1cece7e" memberName="filter_type_6_2"
              virtualName="" explicitFocusOrder="0" pos="1055r 290 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c642f9acf4b813ef" memberName="filter_type_6_3"
              virtualName="" explicitFocusOrder="0" pos="1055r 470 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4d29473e06fd562f" memberName="button_ctrl_toggle"
              virtualName="" explicitFocusOrder="0" pos="100 10 60 30" tooltip="Turns the SHIFT mode on or off.&#10;&#10;The shift mode moves all back sliders to front and front sliders to back."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="SHIFT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="ca562cfd2b6999c4" memberName="speed" virtualName=""
                    explicitFocusOrder="0" pos="1285r 1000r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new BPMSlConfig(synth_data)"/>
  <TEXTBUTTON name="" id="8f0b48518cbff149" memberName="button_open_morph"
              virtualName="" explicitFocusOrder="0" pos="1165r 765 60 33" tooltip="Open/Close the morph editor.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f57674183a67085" memberName="effect_finalizer_switch"
              virtualName="" explicitFocusOrder="0" pos="520r 670 25 130" tooltip="Switch between FX and EQ bank section."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="F X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="af640d06672c6a96" memberName="label_ui_headline2"
         virtualName="" explicitFocusOrder="0" pos="1105 666 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="6cf55c2697d84492" memberName="label_ui_headline3"
         virtualName="" explicitFocusOrder="0" pos="1165 666 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FLT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="31349ce13448848e" memberName="label_ui_headline5"
         virtualName="" explicitFocusOrder="0" pos="1225 666 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="1e86fc07c2fe9e40" memberName="label_ui_headline6"
         virtualName="" explicitFocusOrder="0" pos="1285 666 60 35" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARP" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="8b8fa534e67fede0" memberName="button_values_toggle"
              virtualName="" explicitFocusOrder="0" pos="30 10 60 30" tooltip="Turns the CTRL mode on or off.&#10;&#10;In CTRL mode are all values visble.&#10;&#10;Hold down CTRL/CMD on your keyboard and drag a slider to control it in velocity mode.&#10;&#10;Hold down CTRL/CMD on your keyboard and press + or - to resize the user interface. Press F11 to toggle fullscreen mode."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="CTRL"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="6c9f41765f0f3e8a" memberName="octave_offset" virtualName=""
                    explicitFocusOrder="0" pos="1435r 870 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OctaveOffsetSlConfig(synth_data)"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline4"
         virtualName="" explicitFocusOrder="0" pos="245 55 120 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER INPUTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="706628ef52338a3" memberName="label_ui_headline7"
         virtualName="" explicitFocusOrder="0" pos="40 55 110 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSCILLATORS (O)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="40822f39512f59ee" memberName="label_ui_headline8"
         virtualName="" explicitFocusOrder="0" pos="510 55 130 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER ENVELOPE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="4a296c8c1b36d5b5" memberName="label_ui_headline9"
         virtualName="" explicitFocusOrder="0" pos="850 55 50 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO (L)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="8dfe8598a2227d6" memberName="label_ui_headline10"
         virtualName="" explicitFocusOrder="0" pos="975 55 190 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER CONFIGURATION"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="624597a6e0fd4f43" memberName="label_ui_headline11"
         virtualName="" explicitFocusOrder="0" pos="1220 55 110 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="d111f7d6b78091bd" memberName="label_ui_headline12"
         virtualName="" explicitFocusOrder="0" pos="1375 55 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OUTPUT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="cc8c514c76739c41" memberName="label_ui_headline13"
         virtualName="" explicitFocusOrder="0" pos="270 630 130 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MAIN ENVELOPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="c10833d7ab234ea1" memberName="label_shape" virtualName=""
         explicitFocusOrder="0" pos="525 630 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DISTORTION" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="bfabeb8da43f3421" memberName="label_reverb" virtualName=""
         explicitFocusOrder="0" pos="840 630 80 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="REVERB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="7a6e3d75d5abc4de" memberName="label_delay" virtualName=""
         explicitFocusOrder="0" pos="705 630 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DELAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="a4b6cc975d476069" memberName="label_chorus" virtualName=""
         explicitFocusOrder="0" pos="610 630 70 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CHORUS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="fcb9d931dfbdfa5d" memberName="label_fx_mix" virtualName=""
         explicitFocusOrder="0" pos="1005 630 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FX MIX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="660b9ead77642f4f" memberName="label_ui_headline19"
         virtualName="" explicitFocusOrder="0" pos="1135 630 180 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH (MO) SECTION"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="c72f149b5e4ef3a1" memberName="label_ui_headline20"
         virtualName="" explicitFocusOrder="0" pos="615 830 190 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARPEGGIATOR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="365d9d273db6db3a" memberName="label_ui_headline21"
         virtualName="" explicitFocusOrder="0" pos="100 830 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLIDE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="9b0c123898785ff8" memberName="label_ui_headline22"
         virtualName="" explicitFocusOrder="0" pos="1260 830 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="3dc587f677ecc27f" memberName="label_ui_headline23"
         virtualName="" explicitFocusOrder="0" pos="1375 630 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MASTER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="de3bbc3ac23fc36a" memberName="label_ui_headline24"
         virtualName="" explicitFocusOrder="0" pos="1375 830 60 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="TUNE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ecf3af4d030b7b19" memberName="label_ui_headline25"
         virtualName="" explicitFocusOrder="0" pos="35 613 120 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FM (F)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c54e3e2b543626c5" memberName="volume_master_meter"
                    virtualName="Monique_Ui_SegmentedMeter" explicitFocusOrder="0"
                    pos="1375 670 60 27" class="Component" params="ui_refresher"/>
  <LABEL name="DL" id="ad887653d405d154" memberName="label_eq" virtualName=""
         explicitFocusOrder="0" pos="740 630 90 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="EQUALIZER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="30ecdded1d4d2622" memberName="button_open_config2"
              virtualName="" explicitFocusOrder="0" pos="1335r 40r 60 30" tooltip="Open/Close the setup.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="SETUP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="DL" id="657dea65d9f85585" memberName="label_ui_headline14"
         virtualName="" explicitFocusOrder="0" pos="745 55 80 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MOD MIX (X)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="b915f30bfb983dc7" memberName="button_edit_input_env_1_1"
              virtualName="" explicitFocusOrder="0" pos="215 205 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="cfad8aa44907e8cf" memberName="button_edit_input_env_1_2"
              virtualName="" explicitFocusOrder="0" pos="275 205 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f7dd0a8e5d005547" memberName="button_edit_input_env_1_3"
              virtualName="" explicitFocusOrder="0" pos="335 205 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6c31c6d826c76bfd" memberName="button_edit_input_env_2_1"
              virtualName="" explicitFocusOrder="0" pos="215 385 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="283cd78e11cc7a6e" memberName="button_edit_input_env_2_2"
              virtualName="" explicitFocusOrder="0" pos="275 385 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="47db5d103cbca51f" memberName="button_edit_input_env_2_3"
              virtualName="" explicitFocusOrder="0" pos="335 385 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d4a552f9f250c90" memberName="button_edit_input_env_3_1"
              virtualName="" explicitFocusOrder="0" pos="215 565 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d93900dcb94156f" memberName="button_edit_input_env_3_2"
              virtualName="" explicitFocusOrder="0" pos="275 565 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4ed4547628245141" memberName="button_edit_input_env_3_3"
              virtualName="" explicitFocusOrder="0" pos="335 565 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="972bb72707bb206b" memberName="button_edit_input_env_band_1"
              virtualName="" explicitFocusOrder="0" pos="525 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e4790e6c91bf6fec" memberName="button_edit_input_env_band_2"
              virtualName="" explicitFocusOrder="0" pos="585 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c5b44a8882d48b9" memberName="button_edit_input_env_band_3"
              virtualName="" explicitFocusOrder="0" pos="660 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="55e4c7770dd6c6a5" memberName="button_edit_input_env_band_4"
              virtualName="" explicitFocusOrder="0" pos="720 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="325021dee5961ad1" memberName="button_edit_input_env_band_5"
              virtualName="" explicitFocusOrder="0" pos="790 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7d42e1a02a42a873" memberName="button_edit_input_env_band_6"
              virtualName="" explicitFocusOrder="0" pos="850 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ac8fdd08954a9d8" memberName="button_edit_input_env_band_7"
              virtualName="" explicitFocusOrder="0" pos="920 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="88bbe8f41160cb4a" memberName="button_edit_env_chorus"
              virtualName="" explicitFocusOrder="0" pos="615 765 60 33" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="fa465a4afae26fc7" memberName="flt_pan_3" virtualName=""
                    explicitFocusOrder="0" pos="1335r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="6397e9617b7dcaf9" memberName="flt_pan_2" virtualName=""
                    explicitFocusOrder="0" pos="1335r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="e272cc245f5b87a1" memberName="flt_pan_1" virtualName=""
                    explicitFocusOrder="0" pos="1335r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,0)"/>
  <TEXTBUTTON name="" id="db06e124f1fcbf4d" memberName="button_reset_arp_tune"
              virtualName="" explicitFocusOrder="0" pos="1435r 870 60 27" tooltip="Resets the arpeggiator to the defined program note.&#10;(Triggers a note which is defineable by the note dial (back dial))"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="RESET"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bcf2554ab289ccda" memberName="button_show_active_input_r_2_3"
              virtualName="" explicitFocusOrder="0" pos="376 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e67decdf6ffaf352" memberName="button_show_active_input_l_2_3"
              virtualName="" explicitFocusOrder="0" pos="346 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="327a90a8c4ed5aa9" memberName="button_show_active_input_r_2_2"
              virtualName="" explicitFocusOrder="0" pos="316 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6d0f49fc4b55f79e" memberName="button_show_active_input_l_2_2"
              virtualName="" explicitFocusOrder="0" pos="286 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8be6e66ece9ca751" memberName="button_show_active_input_r_2_1"
              virtualName="" explicitFocusOrder="0" pos="256 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="69acf26b6e8a591e" memberName="button_show_active_input_l_2_1"
              virtualName="" explicitFocusOrder="0" pos="226 279 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c04afcf396fb5173" memberName="button_show_active_input_r_3_3"
              virtualName="" explicitFocusOrder="0" pos="376 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="73aa09a176276d09" memberName="button_show_active_input_l_3_3"
              virtualName="" explicitFocusOrder="0" pos="346 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bf707b726ad7a93b" memberName="button_show_active_input_r_3_2"
              virtualName="" explicitFocusOrder="0" pos="316 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c895ade2dcdf51aa" memberName="button_show_active_input_l_3_2"
              virtualName="" explicitFocusOrder="0" pos="286 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="a84a66eefd216154" memberName="button_show_active_input_r_3_1"
              virtualName="" explicitFocusOrder="0" pos="256 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="aab3f66b90ddf6" memberName="button_show_active_input_l_3_1"
              virtualName="" explicitFocusOrder="0" pos="225 459 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="5e29190ea95f8441" memberName="button_programm_rename"
              virtualName="" explicitFocusOrder="0" pos="970r 10 60 27" tooltip="Rename the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="RENAME"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="f27a090801db5056" memberName="flt_shape_1" virtualName=""
                    explicitFocusOrder="0" pos="785r 240r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="8420242133122df9" memberName="flt_shape_2" virtualName=""
                    explicitFocusOrder="0" pos="785r 420r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="fda8401083c9b835" memberName="flt_shape_3" virtualName=""
                    explicitFocusOrder="0" pos="785r 600r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,2)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: empty_png, 159, "../Images/empty.png"
static const unsigned char resource_Monique_Ui_Mainwindow_empty_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,1,0,0,0,1,8,4,0,0,0,181,28,12,2,0,0,0,2,98,75,71,68,0,0,170,141,35,50,0,0,
0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,223,9,18,16,13,44,67,236,72,225,0,0,0,25,116,69,88,116,67,111,109,109,101,110,116,0,67,114,101,97,116,101,100,32,119,105,116,
104,32,71,73,77,80,87,129,14,23,0,0,0,11,73,68,65,84,8,215,99,248,255,31,0,3,0,1,255,100,93,114,245,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* Monique_Ui_Mainwindow::empty_png = (const char*) resource_Monique_Ui_Mainwindow_empty_png;
const int Monique_Ui_Mainwindow::empty_pngSize = 159;


//[EndFile] You can add extra defines here...
//[/EndFile]
