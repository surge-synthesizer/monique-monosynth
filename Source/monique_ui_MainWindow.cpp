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
#include "monique_ui_MFOPopup.h"
#include "monique_ui_OptionPopup.h"

#include "monique_ui_Activate.h"
//[/Headers]

#include "monique_ui_MainWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#ifdef IS_PLUGIN
class Monique_Ui_MidiIO {};
#endif

//==============================================================================
//==============================================================================
class ButtonFlasher : public Timer, AsyncUpdater
{
    Monique_Ui_Mainwindow*const parent;
    Button*const button;
    int counter;
    const bool button_state;
    const bool success;
    const int num_flashs;

    void timerCallback() override
    {
        if( ++counter >= num_flashs*2 )
        {
            stopTimer();
            triggerAsyncUpdate();
            return;
        }
        else
        {
            if( success )
            {
                button->setColour( TextButton::buttonColourId, Colours::lightgreen );
            }
            else
            {
                button->setColour( TextButton::buttonColourId, Colours::red );
            }
            button->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, counter%2 );
            button->repaint();
        }
    }
    void handleAsyncUpdate() override
    {
        parent->button_flasher = nullptr;
    }

public:
ButtonFlasher( Monique_Ui_Mainwindow*parent_, Button*button_, bool success_, int num_flashs_ = 2 ) noexcept :
    parent(parent_), button(button_), counter(0), button_state( IS_BUTTON_ON( button ) ), success( success_ ), num_flashs( num_flashs_ )
    {
        button->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        timerCallback();
        startTimer(400);
    }
    ~ButtonFlasher() noexcept
    {
        TURN_BUTTON_ON_OR_OFF( button, button_state );
        button->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false );
        button->repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonFlasher)
};

void Monique_Ui_Mainwindow::refresh() noexcept
{
    if( last_env_popup_open != synth_data->midi_env_popup.get_value() )
    {
        last_env_popup_open = synth_data->midi_env_popup;
        open_env_or_lfo_popup_by_midi( &synth_data->midi_env_popup );
    }
    else if( last_lfo_popup_open != synth_data->midi_lfo_popup.get_value() )
    {
        last_lfo_popup_open = synth_data->midi_lfo_popup;
        open_env_or_lfo_popup_by_midi( &synth_data->midi_lfo_popup );
    }

    if( not combo_programm->hasKeyboardFocus (false) and combo_programm->isTextEditable() )
    {
        combo_programm->setEditableText(false);
    }

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
            tooltipWindow = new TooltipWindow( nullptr, is_help_key_down_ ? 30 : 1000 );
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
            tooltipWindow = new TooltipWindow( nullptr, 20 );
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
    update_slider_return_values();
}
void Monique_Ui_Mainwindow::update_slider_return_values() noexcept
{
    for( int i = 0 ; i != dual_sliders.size() ; ++i )
    {
        Monique_Ui_DualSlider*slider = dual_sliders.getUnchecked(i);
        slider->update_return_values();
    }
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
void Monique_Ui_Mainwindow::global_slider_settings_changed(Component*parent_) noexcept
{

    const bool rotary_sliders_are_in_rotary_mode = synth_data->sliders_in_rotary_mode;
    const int rotary_sensitivity = synth_data->sliders_sensitivity;
    const int linear_sensitivity = synth_data->sliders_linear_sensitivity;
    for( int i = 0 ; i < parent_->getNumChildComponents() ; ++i )
    {
        /*
        if( Button* button = dynamic_cast< Button* >( parent_->getChildComponent(i) ) )
        {
            if( slider->getSliderStyle() == Slider::Rotary )
            {
                slider->setSliderStyle ( rotary_sliders_are_in_rotary_mode ? Slider::Rotary : Slider::RotaryHorizontalVerticalDrag );
                slider->setMouseDragSensitivity( rotary_sensitivity );
            }
            else
            {
                slider->setMouseDragSensitivity( linear_sensitivity );
            }
        }
        */
        if( Slider* slider = dynamic_cast< Slider* >( parent_->getChildComponent(i) ) )
        {
            Slider::SliderStyle style = slider->getSliderStyle();
            if( style == Slider::LinearVertical or style == Slider::Slider::LinearHorizontal )
            {
                slider->setMouseDragSensitivity( linear_sensitivity );
                slider->setVelocityBasedMode( synth_data->is_linear_sliders_velocity_mode );
                slider->setVelocityModeParameters( float(linear_sensitivity)/1000, 1, 0, false );
            }
            else
            {
slider->setSliderStyle ( rotary_sliders_are_in_rotary_mode ? synth_data->is_rotary_sliders_velocity_mode ? Slider::RotaryHorizontalVerticalDrag : Slider::Rotary : Slider::RotaryHorizontalVerticalDrag );
                slider->setMouseDragSensitivity( rotary_sensitivity );
                slider->setVelocityBasedMode( synth_data->is_rotary_sliders_velocity_mode );
            }
        }
        else if( Monique_Ui_DualSlider* mod_slider = dynamic_cast< Monique_Ui_DualSlider* >( parent_->getChildComponent(i) ) )
        {
            for( int n = 0 ; n < mod_slider->getNumChildComponents() ; ++n )
            {
                if( Slider* slider = dynamic_cast< Slider* >( mod_slider->getChildComponent(n) ) )
                {
                    Slider::SliderStyle style = slider->getSliderStyle();
                    if( style == Slider::LinearVertical or style == Slider::Slider::LinearHorizontal )
                    {
                        slider->setMouseDragSensitivity( linear_sensitivity );
                        slider->setVelocityBasedMode( synth_data->is_linear_sliders_velocity_mode );
                        slider->setVelocityModeParameters( float(linear_sensitivity)/1000, 1, 0, false );
                    }
                    else
                    {
slider->setSliderStyle ( rotary_sliders_are_in_rotary_mode ? synth_data->is_rotary_sliders_velocity_mode ? Slider::RotaryHorizontalVerticalDrag : Slider::Rotary : Slider::RotaryHorizontalVerticalDrag );
                        slider->setMouseDragSensitivity( rotary_sensitivity );
                        slider->setVelocityBasedMode( synth_data->is_rotary_sliders_velocity_mode );
                    }
                }
            }
        }
    }
}

void Monique_Ui_Mainwindow::show_ctrl_state()
{
    if( last_ctrl_mode != synth_data->ctrl )
    {
        ui_refresher->pause();
        last_ctrl_mode = synth_data->ctrl;
        MessageManagerLock mmLock;
        for( int i = 0 ; i < dual_sliders.size() ; ++i )
        {
            if( Monique_Ui_DualSlider* mod_slider = dual_sliders.getUnchecked(i) )
            {
                if( mod_slider->is_in_ctrl_view() != last_ctrl_mode )
                {
                    mod_slider->set_ctrl_view_mode(last_ctrl_mode);
                    mod_slider->show_view_mode();
                }
            }
        }
        ui_refresher->go_on();
    }
}
Parameter* Monique_Ui_Mainwindow::find_parameter_in_dual_sliders( const Component* comp_ ) noexcept
{
    if( const Slider*slider = dynamic_cast< const Slider* >( comp_ ) )
    {
        for( int i = 0 ; i < dual_sliders.size() ; ++i )
        {
            Monique_Ui_DualSlider*dual_slider = dual_sliders.getUnchecked(i);
            if( Parameter*param = dual_slider->get_parameter( slider ) )
            {
                return param;
            }
        }
    }

    return nullptr;
}
Parameter* Monique_Ui_Mainwindow::find_back_parameter_in_dual_sliders( const Component* comp_ ) noexcept
{
    if( const Slider*slider = dynamic_cast< const Slider* >( comp_ ) )
    {
        for( int i = 0 ; i < dual_sliders.size() ; ++i )
        {
            Monique_Ui_DualSlider*dual_slider = dual_sliders.getUnchecked(i);
            if( dual_slider->get_parameter( slider ) )
            {
                return dual_slider->get_back_parameter();
            }
        }
    }

    return nullptr;
}

void Monique_Ui_Mainwindow::show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_, bool force_turn_on_ )
{
    popup = nullptr;
    if( force_turn_on_ )
    {
        if( not midi_control_handler->is_learning() or not midi_control_handler->is_waiting_for_param() )
        {
            midi_control_handler->toggle_midi_learn();
        }

        if( not midi_conrtrol_ )
        {
            if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( comp_ ) )
            {
                slider->slider_value->triggerClick();
                return;
            }
            else if( Slider* slider = dynamic_cast< Slider* >( comp_ ) )
            {
                slider->triggerClick();
                return;
            }
            else
            {
                Component*parent = comp_->getParentComponent();
                if( parent )
                {
                    do
                    {
                        if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( parent ) )
                        {
                            slider->slider_value->triggerClick();

                            return;
                        }
                    }
                    while( (parent = parent->getParentComponent()) );
                }
            }
        }
    }

    if( midi_control_handler->is_learning() && midi_conrtrol_ )
    {
        addChildComponent( popup = new Monique_Ui_MainwindowPopup(ui_refresher,this,midi_conrtrol_) );
        popup->set_element_to_show( comp_ );
        resize_subeditors();
        popup->setVisible(true);
    }
    else
    {
        midi_control_handler->clear();
    }
}
void Monique_Ui_Mainwindow::show_current_voice_data()
{
    keyboard->setRootNote( 60+synth_data->note_offset.get_value()-24 );

    ComponentColours& colours = look_and_feel->colours;

    // FILTER 1
    int f_type = synth_data->filter_datas[0]->filter_type;
    TURN_BUTTON_ON_OR_OFF( filter_type_6_1, f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_2_1, f_type == HPF || f_type == HIGH_2_PASS )
    TURN_BUTTON_ON_OR_OFF( filter_type_3_1, f_type == BPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_5_1, f_type == PASS || f_type == UNKNOWN )

    // FILTER 2
    f_type = synth_data->filter_datas[1]->filter_type;
    TURN_BUTTON_ON_OR_OFF( filter_type_6_2, f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_2_2, f_type == HPF || f_type == HIGH_2_PASS )
    TURN_BUTTON_ON_OR_OFF( filter_type_3_2, f_type == BPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_5_2, f_type == PASS || f_type == UNKNOWN )

    // FILTER 3
    f_type = synth_data->filter_datas[2]->filter_type;
    TURN_BUTTON_ON_OR_OFF( filter_type_6_3, f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_2_3, f_type == HPF || f_type == HIGH_2_PASS )
    TURN_BUTTON_ON_OR_OFF( filter_type_3_3, f_type == BPF )
    TURN_BUTTON_ON_OR_OFF( filter_type_5_3, f_type == PASS || f_type == UNKNOWN )

#define UPDATE_SEQUENCE_BUTTON( button, id ) \
    if( button->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->arp_sequencer_data->step[id] ? voice->get_arp_sequence_amp(id)*0.4+0.6 : voice->get_arp_sequence_amp(id)>0 ? 0.3f : 0) ){ button->repaint(); }

    UPDATE_SEQUENCE_BUTTON( button_sequence_1, 0 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_2, 1 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_3, 2 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_4, 3 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_5, 4 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_6, 5 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_7, 6 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_8, 7 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_9, 8 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_10, 9 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_11, 10 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_12, 11 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_13, 12 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_14, 13 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_15, 14 );
    UPDATE_SEQUENCE_BUTTON( button_sequence_16, 15 );

    TURN_BUTTON_ON_OR_OFF( button_arp_speed_XNORM, synth_data->arp_sequencer_data->speed_multi.get_value() == 0 )

    // MORPHERS
    TURN_BUTTON_ON_OR_OFF( button_open_morph, editor_morph )
    bool state = synth_data->get_morph_switch_state(0);
    TURN_BUTTON_ON_OR_OFF( button_toggle_morph_buttons_1, state )
    if( state == LEFT )
    {
        button_toggle_morph_buttons_1->setButtonText( "OSC-L" );
    }
    else
    {
        button_toggle_morph_buttons_1->setButtonText( "OSC-R" );
    }

    state = synth_data->get_morph_switch_state(1);
    TURN_BUTTON_ON_OR_OFF( button_toggle_morph_buttons_2, state )
    if( state == LEFT )
    {
        button_toggle_morph_buttons_2->setButtonText( "FLT-L" );
    }
    else
    {
        button_toggle_morph_buttons_2->setButtonText( "FLT-R" );
    }

    state = synth_data->get_morph_switch_state(3);
    TURN_BUTTON_ON_OR_OFF( button_toggle_morph_buttons_3, state )
    if( state == LEFT )
    {
        button_toggle_morph_buttons_3->setButtonText( "ARP-L" );
    }
    else
    {
        button_toggle_morph_buttons_3->setButtonText( "ARP-R" );
    }

    state = synth_data->get_morph_switch_state(2);
    TURN_BUTTON_ON_OR_OFF( button_toggle_morph_buttons_4, state )
    if( state == LEFT )
    {
        button_toggle_morph_buttons_4->setButtonText( "FX-L" );
    }
    else
    {
        button_toggle_morph_buttons_4->setButtonText( "FX-R" );
    }



    // EDITORS
#ifdef IS_STANDALONE
    if( flash_counter > 0 )
    {
        flash_counter--;
        if( button_open_midi_io_settings->getProperties().set( VAR_INDEX_BUTTON_AMP, VALUE_SLIDER_COLOUR ) ) {
            button_open_midi_io_settings->repaint();
        }
    }
    else
    {
        if( button_open_midi_io_settings->getProperties().set( VAR_INDEX_BUTTON_AMP, editor_midiio.get() ) ) {
            button_open_midi_io_settings->repaint();
        }
    }
#else
    if( button_open_midi_io_settings->getProperties().set( VAR_INDEX_BUTTON_AMP, midi_control_handler->is_waiting_for_param() ? FORCE_RED : midi_control_handler->is_learning() ? FORCE_RED : TURN_OFF ) ) {
        button_open_midi_io_settings->repaint();
    }
#endif
    if( button_open_oszi->getProperties().set( VAR_INDEX_BUTTON_AMP, amp_painter ? VALUE_SLIDER_COLOUR : TURN_OFF ) ) {
        button_open_oszi->repaint();
    }
    if( button_open_config2->getProperties().set( VAR_INDEX_BUTTON_AMP, editor_global_settings.get() ? VALUE_SLIDER_2_COLOUR : TURN_OFF ) ) {
        button_open_config2->repaint();
    }
    if( button_ctrl_toggle->getProperties().set( VAR_INDEX_BUTTON_AMP, synth_data->ctrl ? MOD_SLIDER_COLOUR : TURN_OFF ) ) {
        button_ctrl_toggle->repaint();
    }
    if( button_values_toggle->getProperties().set( VAR_INDEX_BUTTON_AMP, look_and_feel->show_values_always ? TURN_ON : TURN_OFF ) ) {
        button_values_toggle->repaint();
    }

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
                if( button_show_active_input_l_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_1->repaint();
                }
                if( last_filter_1_1_sustain )
                {
                    if( button_show_active_input_r_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_2_1->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_2_1->repaint();
                    }
                }
            }
            else if( filter_2_1_sustain < 0 )
            {
                if( button_show_active_input_l_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_2_1->repaint();
                }
                if( button_show_active_input_r_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_1->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_1->repaint();
                }
                if( button_show_active_input_r_2_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_1->repaint();
                }
            }

            // FILTER 2 INPUT 2
            if( filter_2_2_sustain > 0 )
            {
                if( button_show_active_input_l_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_2->repaint();
                }
                if( last_filter_1_2_sustain )
                {
                    if( button_show_active_input_r_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_2_2->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_2_2->repaint();
                    }
                }
            }
            else if( filter_2_2_sustain < 0 )
            {
                if( button_show_active_input_l_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_2_2->repaint();
                }
                if( button_show_active_input_r_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_2->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_2->repaint();
                }
                if( button_show_active_input_r_2_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_2->repaint();
                }
            }

            // FILTER 2 INPUT 3
            if( filter_2_3_sustain > 0 )
            {
                if( button_show_active_input_l_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_3->repaint();
                }
                if( last_filter_1_3_sustain )
                {
                    if( button_show_active_input_r_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_2_3->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_2_3->repaint();
                    }
                }
            }
            else if( filter_2_3_sustain < 0 )
            {
                if( button_show_active_input_l_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_2_3->repaint();
                }
                if( button_show_active_input_r_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_3->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_2_3->repaint();
                }
                if( button_show_active_input_r_2_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_2_3->repaint();
                }
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
                if( button_show_active_input_l_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_1->repaint();
                }
                if( last_filter_2_1_sustain )
                {
                    if( button_show_active_input_r_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_3_1->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_3_1->repaint();
                    }
                }
            }
            else if( filter_3_1_sustain < 0 )
            {
                if( button_show_active_input_l_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_3_1->repaint();
                }
                if( button_show_active_input_r_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_1->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_1->repaint();
                }
                if( button_show_active_input_r_3_1->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_1->repaint();
                }
            }

            // FILTER 3 INPUT 2
            if( filter_3_2_sustain > 0  )
            {
                if( button_show_active_input_l_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_2->repaint();
                }
                if( last_filter_2_2_sustain )
                {
                    if( button_show_active_input_r_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_3_2->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_3_2->repaint();
                    }
                }
            }
            else if( filter_3_2_sustain < 0 )
            {
                if( button_show_active_input_l_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_3_2->repaint();
                }
                if( button_show_active_input_r_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_2->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_2->repaint();
                }
                if( button_show_active_input_r_3_2->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_2->repaint();
                }
            }

            // FILTER 3 INPUT 3
            if( filter_3_3_sustain > 0 )
            {
                if( button_show_active_input_l_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_3->repaint();
                }
                if( last_filter_2_3_sustain )
                {
                    if( button_show_active_input_r_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                        button_show_active_input_r_3_3->repaint();
                    }
                }
                else
                {
                    if( button_show_active_input_r_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, FORCE_RED ) ) {
                        button_show_active_input_r_3_3->repaint();
                    }
                }
            }
            else if( filter_3_3_sustain < 0 )
            {
                if( button_show_active_input_l_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) {
                    button_show_active_input_l_3_3->repaint();
                }
                if( button_show_active_input_r_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_3->repaint();
                }
            }
            else
            {
                if( button_show_active_input_l_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_l_3_3->repaint();
                }
                if( button_show_active_input_r_3_3->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) {
                    button_show_active_input_r_3_3->repaint();
                }
            }
        }
    }

    const int current_note = voice->get_current_note();
    if( current_note != last_refreshed_note )
    {
        last_refreshed_note = current_note;

        arp_step_1->force_repaint = true;
        arp_step_2->force_repaint = true;
        arp_step_3->force_repaint = true;
        arp_step_4->force_repaint = true;
        arp_step_5->force_repaint = true;
        arp_step_6->force_repaint = true;
        arp_step_7->force_repaint = true;
        arp_step_8->force_repaint = true;
        arp_step_9->force_repaint = true;
        arp_step_10->force_repaint = true;
        arp_step_11->force_repaint = true;
        arp_step_12->force_repaint = true;
        arp_step_13->force_repaint = true;
        arp_step_14->force_repaint = true;
        arp_step_15->force_repaint = true;
    }
    if( synth_data->get_current_program() == -1 )
    {
        button_programm_load->setEnabled(false);
        button_programm_replace->setEnabled(false);
        button_programm_rename->setEnabled(false);
        button_programm_delete->setEnabled(false);
    }
    else
    {
        button_programm_load->setEnabled(true);
        button_programm_replace->setEnabled(true);
        button_programm_rename->setEnabled(true);
        button_programm_delete->setEnabled(true);
    }
}

void Monique_Ui_Mainwindow::resize_sequence_buttons()
{
    const float width_factor = 1.0f/original_w*getWidth();
    const float height_factor = 1.0f/original_h*getHeight();
    float shuffle = floor(60.0f * width_factor * ArpSequencerData::shuffle_to_value( synth_data->arp_sequencer_data->shuffle ));
    if( shuffle != last_shuffle )
    {
        const int use_shuffle = floor(60.0f*width_factor+shuffle);
        last_shuffle = shuffle;
        for( int i = 0 ; i != sequence_buttons.size() ; ++i )
        {
            TextButton* button = sequence_buttons[i];
            if( i % 4 == 0 )
            {
                button->setSize( use_shuffle, 30.0f * height_factor );
            }
            else if( i % 2 == 0  )
            {
                button->setSize( use_shuffle, 30.0f * height_factor );
            }
            else
            {
                button->setBounds
                (
                    float(sequence_buttons[i-1]->getX()+use_shuffle),
                    float(button->getY()),
                    60.0f*width_factor-shuffle,
                    30.0f * height_factor
                );
            }
        }
    }
}
void Monique_Ui_Mainwindow::switch_finalizer_tab( bool fx_ )
{
    if( fx_ )
    {
        open_mfo_popup( nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME );
    }

    if( effect_finalizer_switch2->getProperties().set( VAR_INDEX_BUTTON_AMP, not fx_ ? USE_AREA_COLOUR : USE_AREA_TRANSCULENT ) )
    {
        effect_finalizer_switch2->repaint();
    }
    if( effect_finalizer_switch->getProperties().set( VAR_INDEX_BUTTON_AMP, fx_ ? USE_AREA_COLOUR : USE_AREA_TRANSCULENT ) )
    {
        effect_finalizer_switch->repaint();
    }

    reverb_room->setVisible( fx_ );
    reverb_dry->setVisible( fx_ );
    delay2->setVisible( fx_ );
    delay3->setVisible( fx_ );
    delay4->setVisible( fx_ );
    chorus_modulation->setVisible( fx_ );
    bypass->setVisible( fx_ );
    label_reverb->setVisible( fx_ );
    label_fx_delay->setVisible( fx_ );
    distortion->setVisible( fx_ );

    //eg
    colour->setVisible( !fx_ );
    eq_1->setVisible( !fx_ );
    eq_2->setVisible( !fx_ );
    eq_3->setVisible( !fx_ );
    eq_4->setVisible( !fx_ );
    eq_5->setVisible( !fx_ );
    eq_6->setVisible( !fx_ );
    eq_7->setVisible( !fx_ );
    label_band_hz_1->setVisible( !fx_ );
    label_band_hz_2->setVisible( !fx_ );
    label_band_hz_3->setVisible( !fx_ );
    label_band_hz_4->setVisible( !fx_ );
    label_band_hz_5->setVisible( !fx_ );
    label_band_hz_6->setVisible( !fx_ );
    label_band_hz_7->setVisible( !fx_ );
    button_edit_input_env_band_1->setVisible( !fx_ );
    button_edit_input_env_band_2->setVisible( !fx_ );
    button_edit_input_env_band_3->setVisible( !fx_ );
    button_edit_input_env_band_4->setVisible( !fx_ );
    button_edit_input_env_band_5->setVisible( !fx_ );
    button_edit_input_env_band_6->setVisible( !fx_ );
    button_edit_input_env_band_7->setVisible( !fx_ );
}
void Monique_Ui_Mainwindow::update_size()
{
    float ui_scale_factor = synth_data->ui_scale_factor;
    const Desktop::Displays::Display& main_display( Desktop::getInstance().getDisplays().getDisplayContaining( Point<int>( getBounds().getX(), getBounds().getY() ) ) );
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


void Monique_Ui_Mainwindow::toggle_modulation_slider_top_button( Button*button_, bool by_force_ ) noexcept
{
    if( not clear_record_timer )
    {
        struct ChorusCleaner : Timer, AsyncUpdater
        {
            Button*const button_down;
            String button_text;
            Monique_Ui_Mainwindow*const parent;
            int counter;
            bool force;
            void timerCallback() override
            {
                if( ++counter > 31 )
                {
                    if( force or Desktop::getInstance().getMainMouseSource().getComponentUnderMouse() == button_down )
                    {
                        if( force )
                        {
                            //parent->synth_data->delay_record = false;
                        }
                        parent->clear_record_buffer();
                        // TODO flash
                    }
                    stopTimer();
                    triggerAsyncUpdate();
                }
                else
                {
                    if( counter % 2 == 0)
                    {
                        String value( 3.2f - (float(counter)/10) );
                        if( value == "3" )
                        {
                            value = "3.0";
                        }
                        else if( value == "2" )
                        {
                            value = "2.0";
                        }
                        else if( value == "1" )
                        {
                            value = "1.0";
                        }
                        button_down->setButtonText( String("CLR ") + value );
                    }
                    else
                    {
                        button_down->setButtonText( String("") );
                    }
                }
            }
            void handleAsyncUpdate() override
            {
                parent->clear_record_timer = nullptr;
            }

            ChorusCleaner( Button*button__, Monique_Ui_Mainwindow*const parent_, bool force_ ) : button_down( button__ ), parent(parent_), counter(0), force(force_)
            {
                button_text = button_down->getButtonText();
                if(force)
                {
                    //parent_->synth_data->delay_record = false;
                }
            }
            ~ChorusCleaner()
            {
                button_down->setButtonText( button_text );
                if(force)
                {
                    //parent->synth_data->delay_record = false;
                }
            }
        };

        clear_record_timer = new ChorusCleaner(button_,this,by_force_);
        clear_record_timer->startTimer(100);
    }
    else
    {
        stop_clear_chorus();
    }
}

void Monique_Ui_Mainwindow::clear_record_buffer() noexcept
{
    std::cout<<"clear"<<std::endl;
    voice->clear_record_buffer();
}
void Monique_Ui_Mainwindow::stop_clear_chorus() noexcept
{
    if( clear_record_timer )
    {
        clear_record_timer = nullptr;
    }
}

//[/MiscUserDefs]

//==============================================================================
Monique_Ui_Mainwindow::Monique_Ui_Mainwindow (Monique_Ui_Refresher*ui_refresher_)
    : Monique_Ui_Refreshable(ui_refresher_),
      AudioProcessorEditor(ui_refresher_->audio_processor),
      original_w(1465), original_h(1235)
{
    //[Constructor_pre] You can add your own custom stuff here..
    last_refreshed_note = -1;
    audio_processor = reinterpret_cast< MoniqueAudioProcessor* >( &processor );
    amp_painter = nullptr;
    is_ctrl_down = false;
    flash_counter = 0;
    program_edit_type = NOT_SET;
    ui_refresher_->editor = this;
    look_and_feel->mainwindow = this;
    clear_record_timer = nullptr;
    last_lfo_popup_open = synth_data->midi_lfo_popup;
    last_env_popup_open = synth_data->midi_env_popup;
    //[/Constructor_pre]

    addAndMakeVisible (label_fx_delay = new Label (String::empty,
            TRANS("DELAY")));
    label_fx_delay->setFont (Font (30.00f, Font::plain));
    label_fx_delay->setJustificationType (Justification::centred);
    label_fx_delay->setEditable (false, false, false);
    label_fx_delay->setColour (Label::textColourId, Colour (0xff050505));
    label_fx_delay->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_fx_delay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (eq_7 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,6)));

    addAndMakeVisible (eq_6 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,5)));

    addAndMakeVisible (eq_5 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,4)));

    addAndMakeVisible (eq_4 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,3)));

    addAndMakeVisible (eq_3 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,2)));

    addAndMakeVisible (eq_2 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,1)));

    addAndMakeVisible (eq_1 = new Monique_Ui_DualSlider (ui_refresher,
            new EQSlConfig(synth_data,0)));

    addAndMakeVisible (distortion = new Monique_Ui_DualSlider (ui_refresher,
            new FXDistortionSlConfig(synth_data)));

    addAndMakeVisible (chorus_modulation = new Monique_Ui_DualSlider (ui_refresher,
            new CModSlConfig(synth_data)));

    addAndMakeVisible (delay3 = new Monique_Ui_DualSlider (ui_refresher,
            new DelaySlConfig(synth_data)));

    addAndMakeVisible (delay2 = new Monique_Ui_DualSlider (ui_refresher,
            new DelayReflexSlConfig(synth_data)));

    addAndMakeVisible (delay4 = new Monique_Ui_DualSlider (ui_refresher,
            new DelayRecordSlConfig(synth_data)));

    addAndMakeVisible (reverb_room = new Monique_Ui_DualSlider (ui_refresher,
            new RRoomSlConfig(synth_data)));

    addAndMakeVisible (reverb_dry = new Monique_Ui_DualSlider (ui_refresher,
            new RDrySlConfig(synth_data)));

    addAndMakeVisible (label_lfo_3 = new Label (String::empty,
            TRANS("LFO 3")));
    label_lfo_3->setFont (Font (30.00f, Font::plain));
    label_lfo_3->setJustificationType (Justification::centred);
    label_lfo_3->setEditable (false, false, false);
    label_lfo_3->setColour (Label::textColourId, Colour (0xff050505));
    label_lfo_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_lfo_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_lfo_2 = new Label (String::empty,
            TRANS("LFO 2")));
    label_lfo_2->setFont (Font (30.00f, Font::plain));
    label_lfo_2->setJustificationType (Justification::centred);
    label_lfo_2->setEditable (false, false, false);
    label_lfo_2->setColour (Label::textColourId, Colour (0xff050505));
    label_lfo_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_lfo_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_lfo_1 = new Label (String::empty,
            TRANS("LFO 1")));
    label_lfo_1->setFont (Font (30.00f, Font::plain));
    label_lfo_1->setJustificationType (Justification::centred);
    label_lfo_1->setEditable (false, false, false);
    label_lfo_1->setColour (Label::textColourId, Colour (0xff050505));
    label_lfo_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_lfo_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lfo_1 = new Monique_Ui_DualSlider (ui_refresher,
            new LFOSlConfig(synth_data,0)));

    addAndMakeVisible (lfo_2 = new Monique_Ui_DualSlider (ui_refresher,
            new LFOSlConfig(synth_data,1)));

    addAndMakeVisible (lfo_3 = new Monique_Ui_DualSlider (ui_refresher,
            new LFOSlConfig(synth_data,2)));

    addAndMakeVisible (morpher_1 = new Monique_Ui_DualSlider (ui_refresher,
            new MorphSLConfig(synth_data,0)));

    addAndMakeVisible (morpher_2 = new Monique_Ui_DualSlider (ui_refresher,
            new MorphSLConfig(synth_data,1)));

    addAndMakeVisible (morpher_4 = new Monique_Ui_DualSlider (ui_refresher,
            new MorphSLConfig(synth_data,3)));

    addAndMakeVisible (morpher_3 = new Monique_Ui_DualSlider (ui_refresher,
            new MorphSLConfig(synth_data,2)));

    addAndMakeVisible (flt_input_13 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,2,2)));

    addAndMakeVisible (flt_input_12 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,2,1)));

    addAndMakeVisible (flt_input_11 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,2,0)));

    addAndMakeVisible (flt_input_6 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,1,0)));

    addAndMakeVisible (flt_input_7 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,1,1)));

    addAndMakeVisible (flt_input_8 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,1,2)));

    addAndMakeVisible (flt_input_3 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,0,2)));

    addAndMakeVisible (flt_input_2 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,0,1)));

    addAndMakeVisible (flt_input_1 = new Monique_Ui_DualSlider (ui_refresher,
            new InputSlConfig(synth_data,0,0)));

    addAndMakeVisible (button_edit_lfo_1 = new TextButton (String::empty));
    button_edit_lfo_1->setTooltip (TRANS("Open/Close a popup to edit this LFO.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_lfo_1->addListener (this);
    button_edit_lfo_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_lfo_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_lfo_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_lfo_2 = new TextButton (String::empty));
    button_edit_lfo_2->setTooltip (TRANS("Open/Close a popup to edit this LFO.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_lfo_2->addListener (this);
    button_edit_lfo_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_lfo_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_lfo_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_lfo_3 = new TextButton (String::empty));
    button_edit_lfo_3->setTooltip (TRANS("Open/Close a popup to edit this LFO.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_lfo_3->addListener (this);
    button_edit_lfo_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_lfo_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_lfo_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_3 = new TextButton (String::empty));
    button_edit_input_env_3_3->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_3_3->addListener (this);
    button_edit_input_env_3_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_2 = new TextButton (String::empty));
    button_edit_input_env_3_2->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_3_2->addListener (this);
    button_edit_input_env_3_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_3_1 = new TextButton (String::empty));
    button_edit_input_env_3_1->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_3_1->addListener (this);
    button_edit_input_env_3_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_1 = new TextButton (String::empty));
    button_edit_input_env_2_1->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_2_1->addListener (this);
    button_edit_input_env_2_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_2 = new TextButton (String::empty));
    button_edit_input_env_2_2->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_2_2->addListener (this);
    button_edit_input_env_2_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_2_3 = new TextButton (String::empty));
    button_edit_input_env_2_3->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_2_3->addListener (this);
    button_edit_input_env_2_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_1_3 = new TextButton (String::empty));
    button_edit_input_env_1_3->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_1_3->addListener (this);
    button_edit_input_env_1_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_1_2 = new TextButton (String::empty));
    button_edit_input_env_1_2->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_1_2->addListener (this);
    button_edit_input_env_1_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_1_1 = new TextButton (String::empty));
    button_edit_input_env_1_1->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this input.\n"
                                           "\n"
                                           "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_1_1->addListener (this);
    button_edit_input_env_1_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_1_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_1_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_mfo_4 = new TextButton (String::empty));
    button_edit_mfo_4->setTooltip (TRANS("Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_4->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_mfo_4->addListener (this);
    button_edit_mfo_4->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_mfo_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_mfo_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_mfo_3 = new TextButton (String::empty));
    button_edit_mfo_3->setTooltip (TRANS("Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_mfo_3->addListener (this);
    button_edit_mfo_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_mfo_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_mfo_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_mfo_2 = new TextButton (String::empty));
    button_edit_mfo_2->setTooltip (TRANS("Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_mfo_2->addListener (this);
    button_edit_mfo_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_mfo_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_mfo_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_mfo_1 = new TextButton (String::empty));
    button_edit_mfo_1->setTooltip (TRANS("Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
                                         "\n"
                                         "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_mfo_1->addListener (this);
    button_edit_mfo_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_mfo_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_mfo_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_1 = new TextButton (String::empty));
    button_edit_input_env_band_1->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_1->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_1->addListener (this);
    button_edit_input_env_band_1->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_2 = new TextButton (String::empty));
    button_edit_input_env_band_2->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_2->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_2->addListener (this);
    button_edit_input_env_band_2->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (effect_finalizer_switch2 = new TextButton (String::empty));
    effect_finalizer_switch2->setTooltip (TRANS("Switch to the EQ bank."));
    effect_finalizer_switch2->setButtonText (TRANS("EQ"));
    effect_finalizer_switch2->setConnectedEdges (Button::ConnectedOnTop);
    effect_finalizer_switch2->addListener (this);
    effect_finalizer_switch2->setColour (TextButton::buttonColourId, Colour (0xffff11ff));
    effect_finalizer_switch2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    effect_finalizer_switch2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_7 = new TextButton (String::empty));
    button_edit_input_env_band_7->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_7->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_7->addListener (this);
    button_edit_input_env_band_7->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_7->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_7->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_3 = new TextButton (String::empty));
    button_edit_input_env_band_3->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_3->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_3->addListener (this);
    button_edit_input_env_band_3->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_4 = new TextButton (String::empty));
    button_edit_input_env_band_4->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_4->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_4->addListener (this);
    button_edit_input_env_band_4->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_5 = new TextButton (String::empty));
    button_edit_input_env_band_5->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_5->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_5->addListener (this);
    button_edit_input_env_band_5->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_5->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_5->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_edit_input_env_band_6 = new TextButton (String::empty));
    button_edit_input_env_band_6->setTooltip (TRANS("Open/Close the envelope popup to edit the envelope for this band.\n"
            "\n"
            "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_6->setButtonText (CharPointer_UTF8 ("\xe2\x97\x8b"));
    button_edit_input_env_band_6->addListener (this);
    button_edit_input_env_band_6->setColour (TextButton::buttonColourId, Colours::black);
    button_edit_input_env_band_6->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_edit_input_env_band_6->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_bg_button_3 = new TextButton (String::empty));
    filter_type_bg_button_3->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_bg_button_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_bg_button_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_3 = new TextButton ("VOICE 1"));
    filter_type_2_3->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_3->setButtonText (TRANS("HP"));
    filter_type_2_3->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_2_3->addListener (this);
    filter_type_2_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_bg_button_2 = new TextButton (String::empty));
    filter_type_bg_button_2->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_bg_button_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_bg_button_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_2 = new TextButton ("VOICE 1"));
    filter_type_2_2->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_2->setButtonText (TRANS("HP"));
    filter_type_2_2->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_2_2->addListener (this);
    filter_type_2_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_bg_button_1 = new TextButton (String::empty));
    filter_type_bg_button_1->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_bg_button_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_bg_button_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_2_1 = new TextButton ("VOICE 1"));
    filter_type_2_1->setTooltip (TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_1->setButtonText (TRANS("HP"));
    filter_type_2_1->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_2_1->addListener (this);
    filter_type_2_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_2_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_2_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_toggle_morph_buttons_1 = new TextButton (String::empty));
    button_toggle_morph_buttons_1->setTooltip (TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_1->setButtonText (TRANS("OSC-T"));
    button_toggle_morph_buttons_1->addListener (this);
    button_toggle_morph_buttons_1->setColour (TextButton::buttonColourId, Colours::black);
    button_toggle_morph_buttons_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_toggle_morph_buttons_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_toggle_morph_buttons_2 = new TextButton (String::empty));
    button_toggle_morph_buttons_2->setTooltip (TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_2->setButtonText (TRANS("FLT-T"));
    button_toggle_morph_buttons_2->addListener (this);
    button_toggle_morph_buttons_2->setColour (TextButton::buttonColourId, Colours::black);
    button_toggle_morph_buttons_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_toggle_morph_buttons_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_toggle_morph_buttons_3 = new TextButton (String::empty));
    button_toggle_morph_buttons_3->setTooltip (TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_3->setButtonText (TRANS("ARP-T"));
    button_toggle_morph_buttons_3->addListener (this);
    button_toggle_morph_buttons_3->setColour (TextButton::buttonColourId, Colours::black);
    button_toggle_morph_buttons_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_toggle_morph_buttons_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_toggle_morph_buttons_4 = new TextButton (String::empty));
    button_toggle_morph_buttons_4->setTooltip (TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_4->setButtonText (TRANS("FX-T"));
    button_toggle_morph_buttons_4->addListener (this);
    button_toggle_morph_buttons_4->setColour (TextButton::buttonColourId, Colours::black);
    button_toggle_morph_buttons_4->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_toggle_morph_buttons_4->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_band_hz_5 = new Label ("DL",
            TRANS("1.3kHz")));
    label_band_hz_5->setFont (Font (30.00f, Font::plain));
    label_band_hz_5->setJustificationType (Justification::centred);
    label_band_hz_5->setEditable (false, false, false);
    label_band_hz_5->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_6 = new Label ("DL",
            TRANS("2.6kHz")));
    label_band_hz_6->setFont (Font (30.00f, Font::plain));
    label_band_hz_6->setJustificationType (Justification::centred);
    label_band_hz_6->setEditable (false, false, false);
    label_band_hz_6->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_4 = new Label ("DL",
            TRANS("660Hz")));
    label_band_hz_4->setFont (Font (30.00f, Font::plain));
    label_band_hz_4->setJustificationType (Justification::centred);
    label_band_hz_4->setEditable (false, false, false);
    label_band_hz_4->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_1 = new Label ("DL",
            TRANS("<82Hz")));
    label_band_hz_1->setFont (Font (30.00f, Font::plain));
    label_band_hz_1->setJustificationType (Justification::centred);
    label_band_hz_1->setEditable (false, false, false);
    label_band_hz_1->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_morph = new Label (String::empty,
            TRANS("MORPH (MO) SECTION")));
    label_morph->setFont (Font (30.00f, Font::plain));
    label_morph->setJustificationType (Justification::centred);
    label_morph->setEditable (false, false, false);
    label_morph->setColour (Label::textColourId, Colour (0xff050505));
    label_morph->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_morph->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_7 = new Label ("DL",
            TRANS(">2.6kHz")));
    label_band_hz_7->setFont (Font (30.00f, Font::plain));
    label_band_hz_7->setJustificationType (Justification::centred);
    label_band_hz_7->setEditable (false, false, false);
    label_band_hz_7->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_arpeggiator = new Label (String::empty,
            TRANS("ARPEGGIATOR")));
    label_arpeggiator->setFont (Font (30.00f, Font::plain));
    label_arpeggiator->setJustificationType (Justification::centred);
    label_arpeggiator->setEditable (false, false, false);
    label_arpeggiator->setColour (Label::textColourId, Colour (0xff1111ff));
    label_arpeggiator->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_arpeggiator->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_monique = new Label ("DL",
            TRANS("M O N I Q U E")));
    label_monique->setFont (Font (250.00f, Font::plain));
    label_monique->setJustificationType (Justification::centred);
    label_monique->setEditable (false, false, false);
    label_monique->setColour (Label::textColourId, Colour (0xffff3b00));
    label_monique->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_monique->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_programm_replace = new TextButton (String::empty));
    button_programm_replace->setTooltip (TRANS("Replaces the selected program."));
    button_programm_replace->setButtonText (TRANS("SAVE"));
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

    addAndMakeVisible (bypass = new Monique_Ui_DualSlider (ui_refresher,
            new BypassConfig(synth_data)));

    addAndMakeVisible (colour = new Monique_Ui_DualSlider (ui_refresher,
            new FColourSlConfig(synth_data)));

    addAndMakeVisible (label_band_hz_2 = new Label ("DL",
            TRANS("165Hz")));
    label_band_hz_2->setFont (Font (30.00f, Font::plain));
    label_band_hz_2->setJustificationType (Justification::centred);
    label_band_hz_2->setEditable (false, false, false);
    label_band_hz_2->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_band_hz_3 = new Label ("DL",
            TRANS("330Hz")));
    label_band_hz_3->setFont (Font (30.00f, Font::plain));
    label_band_hz_3->setJustificationType (Justification::centred);
    label_band_hz_3->setEditable (false, false, false);
    label_band_hz_3->setColour (Label::textColourId, Colour (0xff050505));
    label_band_hz_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_band_hz_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (speed_multi = new Monique_Ui_DualSlider (ui_refresher,
            new SpeedMultiSlConfig(synth_data)));

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

    addAndMakeVisible (filter_type_3_1 = new TextButton ("VOICE 1"));
    filter_type_3_1->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_1->setButtonText (TRANS("BAND"));
    filter_type_3_1->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_3_1->addListener (this);
    filter_type_3_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_2 = new TextButton ("VOICE 1"));
    filter_type_3_2->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_2->setButtonText (TRANS("BAND"));
    filter_type_3_2->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_3_2->addListener (this);
    filter_type_3_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_3_3 = new TextButton ("VOICE 1"));
    filter_type_3_3->setTooltip (TRANS("Set the filter type to BAND PASS."));
    filter_type_3_3->setButtonText (TRANS("BAND"));
    filter_type_3_3->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    filter_type_3_3->addListener (this);
    filter_type_3_3->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_3_3->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_3_3->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_1 = new TextButton ("VOICE 1"));
    filter_type_5_1->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_1->setButtonText (TRANS("PASS"));
    filter_type_5_1->setConnectedEdges (Button::ConnectedOnTop);
    filter_type_5_1->addListener (this);
    filter_type_5_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_2 = new TextButton ("VOICE 1"));
    filter_type_5_2->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_2->setButtonText (TRANS("PASS"));
    filter_type_5_2->setConnectedEdges (Button::ConnectedOnTop);
    filter_type_5_2->addListener (this);
    filter_type_5_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_5_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_5_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_5_3 = new TextButton ("VOICE 1"));
    filter_type_5_3->setTooltip (TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_3->setButtonText (TRANS("PASS"));
    filter_type_5_3->setConnectedEdges (Button::ConnectedOnTop);
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
    combo_programm->setTextWhenNothingSelected (TRANS("FROM SCRATCH"));
    combo_programm->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_programm->addListener (this);

    addAndMakeVisible (button_programm_left = new TextButton (String::empty));
    button_programm_left->setTooltip (TRANS("Load the program before of the selected bank (right of this button)."));
    button_programm_left->setButtonText (TRANS("<"));
    button_programm_left->setConnectedEdges (Button::ConnectedOnRight);
    button_programm_left->addListener (this);
    button_programm_left->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_left->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_left->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (button_programm_right = new TextButton (String::empty));
    button_programm_right->setTooltip (TRANS("Load the next program of the selected bank."));
    button_programm_right->setButtonText (TRANS(">"));
    button_programm_right->setConnectedEdges (Button::ConnectedOnLeft);
    button_programm_right->addListener (this);
    button_programm_right->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_right->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_programm_right->setColour (TextButton::textColourOffId, Colours::yellow);

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

    addAndMakeVisible (flt_cutoff_1 = new Monique_Ui_DualSlider (ui_refresher,
            new FCutoffSLConfig(synth_data,0)));

    addAndMakeVisible (flt_cutoff_2 = new Monique_Ui_DualSlider (ui_refresher,
            new FCutoffSLConfig(synth_data,1)));

    addAndMakeVisible (flt_cutoff_3 = new Monique_Ui_DualSlider (ui_refresher,
            new FCutoffSLConfig(synth_data,2)));

    addAndMakeVisible (flt_distortion_1 = new Monique_Ui_DualSlider (ui_refresher,
            new GForceSlConfig(synth_data,0)));

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
    filter_type_6_1->setConnectedEdges (Button::ConnectedOnBottom);
    filter_type_6_1->addListener (this);
    filter_type_6_1->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_1->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_1->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_2 = new TextButton ("VOICE 1"));
    filter_type_6_2->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_6_2->setButtonText (TRANS("LP"));
    filter_type_6_2->setConnectedEdges (Button::ConnectedOnBottom);
    filter_type_6_2->addListener (this);
    filter_type_6_2->setColour (TextButton::buttonColourId, Colours::black);
    filter_type_6_2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    filter_type_6_2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (filter_type_6_3 = new TextButton ("VOICE 1"));
    filter_type_6_3->setTooltip (TRANS("Set the filter type to LOW PASS."));
    filter_type_6_3->setButtonText (TRANS("LP"));
    filter_type_6_3->setConnectedEdges (Button::ConnectedOnBottom);
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
    effect_finalizer_switch->setTooltip (TRANS("Switch to the FX section."));
    effect_finalizer_switch->setButtonText (TRANS("FX"));
    effect_finalizer_switch->setConnectedEdges (Button::ConnectedOnTop);
    effect_finalizer_switch->addListener (this);
    effect_finalizer_switch->setColour (TextButton::buttonColourId, Colour (0xffff11ff));
    effect_finalizer_switch->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    effect_finalizer_switch->setColour (TextButton::textColourOffId, Colours::yellow);

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

    addAndMakeVisible (label_filter_inputs = new Label (String::empty,
            TRANS("FILTER INPUTS")));
    label_filter_inputs->setFont (Font (30.00f, Font::plain));
    label_filter_inputs->setJustificationType (Justification::centred);
    label_filter_inputs->setEditable (false, false, false);
    label_filter_inputs->setColour (Label::textColourId, Colour (0xff1111ff));
    label_filter_inputs->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_filter_inputs->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_oscillators = new Label (String::empty,
            TRANS("OSCILLATORS (O)")));
    label_oscillators->setFont (Font (30.00f, Font::plain));
    label_oscillators->setJustificationType (Justification::centred);
    label_oscillators->setEditable (false, false, false);
    label_oscillators->setColour (Label::textColourId, Colour (0xff1111ff));
    label_oscillators->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_oscillators->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_filter_envelope = new Label (String::empty,
            TRANS("FILTER ENVELOPE")));
    label_filter_envelope->setFont (Font (30.00f, Font::plain));
    label_filter_envelope->setJustificationType (Justification::centred);
    label_filter_envelope->setEditable (false, false, false);
    label_filter_envelope->setColour (Label::textColourId, Colour (0xff1111ff));
    label_filter_envelope->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_filter_envelope->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_lfo = new Label (String::empty,
            TRANS("LFO (L)")));
    label_lfo->setFont (Font (30.00f, Font::plain));
    label_lfo->setJustificationType (Justification::centred);
    label_lfo->setEditable (false, false, false);
    label_lfo->setColour (Label::textColourId, Colour (0xff1111ff));
    label_lfo->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_lfo->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_filter_config = new Label (String::empty,
            TRANS("FILTER CONFIGURATION")));
    label_filter_config->setFont (Font (30.00f, Font::plain));
    label_filter_config->setJustificationType (Justification::centred);
    label_filter_config->setEditable (false, false, false);
    label_filter_config->setColour (Label::textColourId, Colour (0xff1111ff));
    label_filter_config->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_filter_config->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_filter_fx = new Label (String::empty,
            TRANS("FILTER FX")));
    label_filter_fx->setFont (Font (30.00f, Font::plain));
    label_filter_fx->setJustificationType (Justification::centred);
    label_filter_fx->setEditable (false, false, false);
    label_filter_fx->setColour (Label::textColourId, Colour (0xff1111ff));
    label_filter_fx->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_filter_fx->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_out = new Label (String::empty,
            TRANS("OUTPUT")));
    label_out->setFont (Font (30.00f, Font::plain));
    label_out->setJustificationType (Justification::centred);
    label_out->setEditable (false, false, false);
    label_out->setColour (Label::textColourId, Colour (0xff1111ff));
    label_out->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_out->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_amp_envelope = new Label (String::empty,
            TRANS("AMP ENVELOPE")));
    label_amp_envelope->setFont (Font (30.00f, Font::plain));
    label_amp_envelope->setJustificationType (Justification::centred);
    label_amp_envelope->setEditable (false, false, false);
    label_amp_envelope->setColour (Label::textColourId, Colour (0xff050505));
    label_amp_envelope->setColour (TextEditor::textColourId, Colour (0xff796660));
    label_amp_envelope->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_glide = new Label (String::empty,
            TRANS("GLIDE")));
    label_glide->setFont (Font (30.00f, Font::plain));
    label_glide->setJustificationType (Justification::centred);
    label_glide->setEditable (false, false, false);
    label_glide->setColour (Label::textColourId, Colour (0xff1111ff));
    label_glide->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_glide->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_speed = new Label (String::empty,
            TRANS("SPEED")));
    label_speed->setFont (Font (30.00f, Font::plain));
    label_speed->setJustificationType (Justification::centred);
    label_speed->setEditable (false, false, false);
    label_speed->setColour (Label::textColourId, Colour (0xff1111ff));
    label_speed->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_speed->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_tune = new Label (String::empty,
            TRANS("TUNE")));
    label_tune->setFont (Font (30.00f, Font::plain));
    label_tune->setJustificationType (Justification::centred);
    label_tune->setEditable (false, false, false);
    label_tune->setColour (Label::textColourId, Colour (0xff1111ff));
    label_tune->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_tune->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_fm = new Label (String::empty,
            TRANS("FM (F)")));
    label_fm->setFont (Font (30.00f, Font::plain));
    label_fm->setJustificationType (Justification::centred);
    label_fm->setEditable (false, false, false);
    label_fm->setColour (Label::textColourId, Colour (0xff1111ff));
    label_fm->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_fm->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (volume_master_meter = new Monique_Ui_SegmentedMeter (ui_refresher));

    addAndMakeVisible (button_open_config2 = new TextButton (String::empty));
    button_open_config2->setTooltip (TRANS("Open/Close the setup.\n"
                                           "\n"
                                           "Note: press ESC to close editors."));
    button_open_config2->setButtonText (TRANS("SETUP"));
    button_open_config2->addListener (this);
    button_open_config2->setColour (TextButton::buttonColourId, Colours::black);
    button_open_config2->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_open_config2->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_mod_mix = new Label (String::empty,
            TRANS("MOD MIX (X)")));
    label_mod_mix->setFont (Font (30.00f, Font::plain));
    label_mod_mix->setJustificationType (Justification::centred);
    label_mod_mix->setEditable (false, false, false);
    label_mod_mix->setColour (Label::textColourId, Colour (0xff1111ff));
    label_mod_mix->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_mod_mix->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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

    addAndMakeVisible (button_programm_scratch = new TextButton (String::empty));
    button_programm_scratch->setTooltip (TRANS("Load the factory defaults to start from scratch.\n"
                                         "\n"
                                         "Push this button again to stop clearing the record buffer."));
    button_programm_scratch->setButtonText (TRANS("INIT"));
    button_programm_scratch->addListener (this);
    button_programm_scratch->setColour (TextButton::buttonColourId, Colours::black);
    button_programm_scratch->setColour (TextButton::textColourOnId, Colour (0xffbcff00));
    button_programm_scratch->setColour (TextButton::textColourOffId, Colour (0xffd0ff00));

    addAndMakeVisible (flt_shape_4 = new Monique_Ui_DualSlider (ui_refresher, new FShapeSlConfig(synth_data)));

    addAndMakeVisible (label_monoplugs = new Label ("TOP",
            TRANS("M  O  N  O  P  L  U  G  S")));
    label_monoplugs->setFont (Font (30.00f, Font::plain));
    label_monoplugs->setJustificationType (Justification::centred);
    label_monoplugs->setEditable (false, false, false);
    label_monoplugs->setColour (Label::textColourId, Colour (0xffffff11));
    label_monoplugs->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_monoplugs->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_reverb = new Label (String::empty,
            TRANS("REVERB")));
    label_reverb->setFont (Font (30.00f, Font::plain));
    label_reverb->setJustificationType (Justification::centred);
    label_reverb->setEditable (false, false, false);
    label_reverb->setColour (Label::textColourId, Colour (0xff050505));
    label_reverb->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_reverb->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


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

    // OPAQUE
    {
        for( int i = 0 ;  i != getNumChildComponents() ; ++i )
        {
            Component* comp( getChildComponent(i) );
            comp->setOpaque(true);
            this->setRepaintsOnMouseActivity(false);

            if( Monique_Ui_DualSlider* slider = dynamic_cast< Monique_Ui_DualSlider* >( comp ) )
            {
                dual_sliders.add(slider);
                slider->setOpaque(false); // controlled by the slider itself
            }
            else if( Label*label = dynamic_cast< Label* >( comp ) )
            {
                label->setInterceptsMouseClicks(false,false);
            }
        }
        adsr_lfo_mix->setOpaque(false);
        lfo_1->setOpaque(false);
        lfo_2->setOpaque(false);
        lfo_3->setOpaque(false);
        lfo_opt_2->setOpaque(false);
        lfo_opt_3->setOpaque(false);
        label_fx_delay->setOpaque(false);
        label_reverb->setOpaque(false);
        effect_finalizer_switch->setOpaque(false);
        effect_finalizer_switch2->setOpaque(false);

        label_band_hz_1->setOpaque(false);
        label_band_hz_2->setOpaque(false);
        label_band_hz_3->setOpaque(false);
        label_band_hz_4->setOpaque(false);
        label_band_hz_5->setOpaque(false);
        label_band_hz_6->setOpaque(false);
        label_band_hz_7->setOpaque(false);

        label_monique->setOpaque(false);
        label_monoplugs->setOpaque(false);
        label_glide->setOpaque(false);

        this->setOpaque(true);
    }

    // THEMES
    {
        for( int i = 0 ; i != sequence_buttons.size() ; ++i )
        {
            sequence_buttons.getUnchecked(i)->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );
        }

        label_oscillators->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::OSC_THEME );

        label_fm->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FM_THEME );

        label_glide->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_arpeggiator->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_speed->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_tune->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_filter_inputs->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_filter_envelope->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_mod_mix->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_lfo->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_filter_config->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_filter_fx->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );

        label_monoplugs->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );
        label_monoplugs->getProperties().set( VAR_INDEX_COLOUR_THEME_INVERTED, true );
        label_monique->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        label_monique->getProperties().set( VAR_INDEX_COLOUR_THEME_INVERTED, true );

        label_out->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        button_reset_arp_tune->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );
        button_arp_speed_XNORM->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );

        button_values_toggle->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_ctrl_toggle->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_left->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        combo_bank->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        combo_programm->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_right->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_load->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_scratch->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_replace->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_new->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_rename->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_delete->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_open_midi_io_settings->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_open_config2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_open_oszi->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );


        button_programm_new->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_scratch->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_replace->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_delete->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_load->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_left->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_right->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_programm_rename->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_open_oszi->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );
        button_open_midi_io_settings->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME );

        button_edit_input_env_1_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_1_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_1_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_2_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_2_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_2_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_3_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_3_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_3_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_input_env_band_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_4->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_5->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_6->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_edit_input_env_band_7->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_reset_arp_tune->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );
        button_edit_mfo_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_edit_mfo_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_edit_mfo_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_edit_mfo_4->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_show_active_input_l_2_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_2_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_l_2_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_2_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_l_2_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_2_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_l_3_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_3_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_l_3_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_3_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_l_3_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_show_active_input_r_3_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_6_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_2_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_3_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_5_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_6_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_2_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_3_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_5_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_6_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_2_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_3_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_5_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_bg_button_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_bg_button_1->setEnabled(false);
        filter_type_bg_button_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_bg_button_2->setEnabled(false);
        filter_type_bg_button_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        filter_type_bg_button_3->setEnabled(false);

        label_morph->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_open_morph->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_toggle_morph_buttons_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_toggle_morph_buttons_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_toggle_morph_buttons_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_toggle_morph_buttons_4->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );

        button_edit_lfo_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_lfo_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_edit_lfo_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        label_lfo_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        label_lfo_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        label_lfo_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );

        label_amp_envelope->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_fx_delay->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_reverb->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_4->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_5->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_6->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        label_band_hz_7->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );

        effect_finalizer_switch2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        effect_finalizer_switch->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
    }

    switch_finalizer_tab(true);

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

    /*
    lfo_1->setInterceptsMouseClicks( false, true );
    removeChildComponent( lfo_minus_1 );
    lfo_1->addAndMakeVisible( lfo_minus_1 );
    lfo_minus_1->setTopLeftPosition( 0,0 );
    lfo_2->setInterceptsMouseClicks( false, true );
    lfo_3->setInterceptsMouseClicks( false, true );
    */

    combo_programm->setEditableText(false);
    update_tooltip_handling(false);
    //update_slider_return_values();

    /*
    //[/UserPreSize]

    setSize (1465, 1235);


    //[Constructor] You can add your own custom stuff here..
    */

#ifdef IS_STANDALONE
    addChildComponent (resizer = new ResizableCornerComponent (this, &resizeLimits));
#else
    resizeLimits.setFixedAspectRatio(original_w/original_h);
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizer->setAlwaysOnTop(true);
#endif

    // resizer->setTooltip("Global shortcut: CTRL + or CTRL -");


    //setVisible(true);
    audio_processor->set_peak_meter( volume_master_meter );

    global_slider_settings_changed(this);
    update_slider_return_values();
    update_size();

    keyboard->setLowestVisibleKey(24);
    keyboard->setAvailableRange( 12, 60 + 24 );
    keyboard->setKeyWidth(45);

    if( synth_data->is_osci_open.get_value() )
        buttonClicked( button_open_oszi );

    show_current_voice_data();

    ui_refresher->startTimer( UI_REFRESH_RATE );

    // resizer->setTooltip( "Global shortcut: CTRL + PLUS or CTRL + MINUS" );
    //look_and_feel->colours.edit();
    delay4->get_top_button()->main_window = this;
    /*
    monique_ui_Activate* activate = new monique_ui_Activate();
    addAndMakeVisible( activate );
    activate->setBounds( getWidth()/2 - 270, getHeight()/2 -200, 540, 400 );
    */
    //[/Constructor]
}

Monique_Ui_Mainwindow::~Monique_Ui_Mainwindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //synth_data->midi_lfo_popup.remove_listener(this);
    //synth_data->midi_env_popup.remove_listener(this);

    PopupMenu::dismissAllActiveMenus();

    button_flasher = nullptr;
    clear_record_timer = nullptr;

    look_and_feel->mainwindow = nullptr;
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
    mfo_popup = nullptr;
    option_popup = nullptr;
    editor_midiio = nullptr;
    editor_morph = nullptr;
    editor_global_settings = nullptr;
    popup = nullptr;

    audio_processor->clear_preak_meter();
    //[/Destructor_pre]

    label_fx_delay = nullptr;
    eq_7 = nullptr;
    eq_6 = nullptr;
    eq_5 = nullptr;
    eq_4 = nullptr;
    eq_3 = nullptr;
    eq_2 = nullptr;
    eq_1 = nullptr;
    distortion = nullptr;
    chorus_modulation = nullptr;
    delay3 = nullptr;
    delay2 = nullptr;
    delay4 = nullptr;
    reverb_room = nullptr;
    reverb_dry = nullptr;
    label_lfo_3 = nullptr;
    label_lfo_2 = nullptr;
    label_lfo_1 = nullptr;
    lfo_1 = nullptr;
    lfo_2 = nullptr;
    lfo_3 = nullptr;
    morpher_1 = nullptr;
    morpher_2 = nullptr;
    morpher_4 = nullptr;
    morpher_3 = nullptr;
    flt_input_13 = nullptr;
    flt_input_12 = nullptr;
    flt_input_11 = nullptr;
    flt_input_6 = nullptr;
    flt_input_7 = nullptr;
    flt_input_8 = nullptr;
    flt_input_3 = nullptr;
    flt_input_2 = nullptr;
    flt_input_1 = nullptr;
    button_edit_lfo_1 = nullptr;
    button_edit_lfo_2 = nullptr;
    button_edit_lfo_3 = nullptr;
    button_edit_input_env_3_3 = nullptr;
    button_edit_input_env_3_2 = nullptr;
    button_edit_input_env_3_1 = nullptr;
    button_edit_input_env_2_1 = nullptr;
    button_edit_input_env_2_2 = nullptr;
    button_edit_input_env_2_3 = nullptr;
    button_edit_input_env_1_3 = nullptr;
    button_edit_input_env_1_2 = nullptr;
    button_edit_input_env_1_1 = nullptr;
    button_edit_mfo_4 = nullptr;
    button_edit_mfo_3 = nullptr;
    button_edit_mfo_2 = nullptr;
    button_edit_mfo_1 = nullptr;
    button_edit_input_env_band_1 = nullptr;
    button_edit_input_env_band_2 = nullptr;
    effect_finalizer_switch2 = nullptr;
    button_edit_input_env_band_7 = nullptr;
    button_edit_input_env_band_3 = nullptr;
    button_edit_input_env_band_4 = nullptr;
    button_edit_input_env_band_5 = nullptr;
    button_edit_input_env_band_6 = nullptr;
    filter_type_bg_button_3 = nullptr;
    filter_type_2_3 = nullptr;
    filter_type_bg_button_2 = nullptr;
    filter_type_2_2 = nullptr;
    filter_type_bg_button_1 = nullptr;
    filter_type_2_1 = nullptr;
    button_toggle_morph_buttons_1 = nullptr;
    button_toggle_morph_buttons_2 = nullptr;
    button_toggle_morph_buttons_3 = nullptr;
    button_toggle_morph_buttons_4 = nullptr;
    label_band_hz_5 = nullptr;
    label_band_hz_6 = nullptr;
    label_band_hz_4 = nullptr;
    label_band_hz_1 = nullptr;
    label_morph = nullptr;
    label_band_hz_7 = nullptr;
    label_arpeggiator = nullptr;
    label_monique = nullptr;
    button_programm_replace = nullptr;
    button_programm_new = nullptr;
    bypass = nullptr;
    colour = nullptr;
    label_band_hz_2 = nullptr;
    label_band_hz_3 = nullptr;
    speed_multi = nullptr;
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
    filter_type_3_1 = nullptr;
    filter_type_3_2 = nullptr;
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
    button_open_oszi = nullptr;
    button_open_midi_io_settings = nullptr;
    combo_bank = nullptr;
    button_programm_load = nullptr;
    osc_1 = nullptr;
    osc_2 = nullptr;
    osc_3 = nullptr;
    flt_cutoff_1 = nullptr;
    flt_cutoff_2 = nullptr;
    flt_cutoff_3 = nullptr;
    flt_distortion_1 = nullptr;
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
    button_programm_delete = nullptr;
    filter_type_6_1 = nullptr;
    filter_type_6_2 = nullptr;
    filter_type_6_3 = nullptr;
    button_ctrl_toggle = nullptr;
    speed = nullptr;
    button_open_morph = nullptr;
    effect_finalizer_switch = nullptr;
    button_values_toggle = nullptr;
    octave_offset = nullptr;
    label_filter_inputs = nullptr;
    label_oscillators = nullptr;
    label_filter_envelope = nullptr;
    label_lfo = nullptr;
    label_filter_config = nullptr;
    label_filter_fx = nullptr;
    label_out = nullptr;
    label_amp_envelope = nullptr;
    label_glide = nullptr;
    label_speed = nullptr;
    label_tune = nullptr;
    label_fm = nullptr;
    volume_master_meter = nullptr;
    button_open_config2 = nullptr;
    label_mod_mix = nullptr;
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
    button_programm_scratch = nullptr;
    flt_shape_4 = nullptr;
    label_monoplugs = nullptr;
    label_reverb = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_Mainwindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    //g.fillAll (Colour (0xff161616));
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffff11ff));
    g.fillRoundedRectangle (810.0f, 670.0f, 550.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (1200.0f, 870.0f, 160.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (180.0f, 870.0f, 1020.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xffff11ff));
    g.fillRect (810, 790, 41, 30);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1000.0f, 170.0f,
                                       Colour (0x301111ff),
                                       970.0f, 170.0f,
                                       false));
    g.fillRect (950, 120, 60, 130);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       800.0f, 170.0f,
                                       Colour (0x301111ff),
                                       850.0f, 170.0f,
                                       false));
    g.fillRect (790, 120, 110, 130);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       800.0f, 170.0f,
                                       Colour (0x301111ff),
                                       850.0f, 170.0f,
                                       false));
    g.fillRect (790, 300, 110, 130);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1000.0f, 170.0f,
                                       Colour (0x301111ff),
                                       970.0f, 170.0f,
                                       false));
    g.fillRect (960, 300, 60, 130);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1000.0f, 170.0f,
                                       Colour (0x301111ff),
                                       970.0f, 170.0f,
                                       false));
    g.fillRect (950, 480, 60, 130);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       800.0f, 170.0f,
                                       Colour (0x301111ff),
                                       850.0f, 170.0f,
                                       false));
    g.fillRect (790, 480, 110, 130);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (920, 530, 140, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (920, 170, 140, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (920, 350, 140, 30);

    g.setColour (Colour (0xff11ffff));
    g.fillRoundedRectangle (10.0f, 60.0f, 160.0f, 760.0f, 10.000f);

    g.setColour (Colour (0xffff1111));
    g.fillRoundedRectangle (20.0f, 60.0f, 150.0f, 570.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (190.0f, 110.0f, 200.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (190.0f, 270.0f, 200.0f, 170.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (190.0f, 450.0f, 200.0f, 170.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (410.0f, 110.0f, 390.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (410.0f, 290.0f, 390.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (410.0f, 470.0f, 390.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (380, 119, 58, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (380, 299, 58, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRect (380, 479, 58, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (890.0f, 110.0f, 80.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (890.0f, 290.0f, 80.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (890.0f, 470.0f, 80.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff050505));
    g.fillEllipse (797.0f, 134.0f, 100.0f, 100.0f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1000.0f, 110.0f, 450.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1000.0f, 290.0f, 450.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillEllipse (802.0f, 139.0f, 90.0f, 90.0f);

    g.setColour (Colour (0xff050505));
    g.fillEllipse (797.0f, 314.0f, 100.0f, 100.0f);

    g.setColour (Colour (0xff1111ff));
    g.fillEllipse (802.0f, 319.0f, 90.0f, 90.0f);

    g.setColour (Colour (0xff050505));
    g.fillEllipse (797.0f, 494.0f, 100.0f, 100.0f);

    g.setColour (Colour (0xff1111ff));
    g.fillEllipse (802.0f, 499.0f, 90.0f, 90.0f);

    g.setColour (Colour (0xffffffee));
    g.fillRoundedRectangle (190.0f, 640.0f, 260.0f, 210.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1255.0f, 240.0f, 10.0f, 40.0f, 5.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1255.0f, 420.0f, 10.0f, 40.0f, 5.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, 870.0f, 170.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff11ff11));
    g.fillRoundedRectangle (1370.0f, 670.0f, 80.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (200.0f, 450.0f, 1064.0f, 10.0f, 5.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (200.0f, 270.0f, 1064.0f, 10.0f, 5.000f);

    g.setGradientFill (ColourGradient (Colour (0xffff11ff),
                                       815.0f, 660.0f,
                                       Colour (0xff1111ff),
                                       1360.0f, 660.0f,
                                       false));
    g.fillRoundedRectangle (470.0f, 640.0f, 950.0f, 20.0f, 5.000f);

    g.setColour (Colour (0xffff11ff));
    g.fillRect (780, 730, 41, 30);

    g.setGradientFill (ColourGradient (Colour (0xffff11ff),
                                       1360.0f, 740.0f,
                                       Colour (0xff11ff11),
                                       1370.0f, 740.0f,
                                       false));
    g.fillRect (1340, 730, 40, 30);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1410.0f, 260.0f,
                                       Colour (0x00ff11ff),
                                       1410.0f, 285.0f,
                                       false));
    g.fillRect (1400, 250, 20, 40);

    g.setGradientFill (ColourGradient (Colour (0xff1111ff),
                                       1410.0f, 440.0f,
                                       Colour (0x00ff11ff),
                                       1410.0f, 465.0f,
                                       false));
    g.fillRect (1400, 430, 20, 40);

    g.setGradientFill (ColourGradient (Colour (0xffffff11),
                                       90.0f, 870.0f,
                                       Colour (0xff11ffff),
                                       90.0f, 820.0f,
                                       false));
    g.fillRect (80, 810, 30, 70);

    g.setColour (Colour (0xffff11ff));
    g.fillRoundedRectangle (470.0f, 640.0f, 330.0f, 180.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, static_cast<float> (-10), 1440.0f, 60.0f, 10.000f);

    g.setGradientFill (ColourGradient (Colour (0x00000000),
                                       290.0f, 620.0f,
                                       Colour (0xffffffee),
                                       290.0f, 635.0f,
                                       false));
    g.fillRect (270, 610, 40, 40);

    g.setGradientFill (ColourGradient (Colour (0x00000000),
                                       240.0f, 870.0f,
                                       Colour (0xffffffee),
                                       240.0f, 855.0f,
                                       false));
    g.fillRect (330, 845, 40, 25);

    g.setGradientFill (ColourGradient (Colour (0x00000000),
                                       170.0f, 720.0f,
                                       Colour (0xffffffee),
                                       185.0f, 720.0f,
                                       false));
    g.fillRect (170, 725, 30, 40);

    g.setGradientFill (ColourGradient (Colour (0x00000000),
                                       470.0f, 730.0f,
                                       Colour (0xffffffee),
                                       455.0f, 730.0f,
                                       false));
    g.fillRect (440, 725, 40, 40);

    g.setGradientFill (ColourGradient (Colour (0xffff1111),
                                       170.0f, 130.0f,
                                       Colour (0xff1111ff),
                                       190.0f, 130.0f,
                                       false));
    g.fillRect (160, 120, 40, 30);

    g.setGradientFill (ColourGradient (Colour (0xffff1111),
                                       170.0f, 130.0f,
                                       Colour (0xff1111ff),
                                       190.0f, 130.0f,
                                       false));
    g.fillRect (160, 300, 40, 30);

    g.setGradientFill (ColourGradient (Colour (0xffff1111),
                                       170.0f, 130.0f,
                                       Colour (0xff1111ff),
                                       190.0f, 130.0f,
                                       false));
    g.fillRect (160, 480, 40, 30);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1395.0f, 610.0f, 30.0f, 50.0f, 5.000f);

    g.setColour (Colour (0xff1111ff));
    g.fillRoundedRectangle (1000.0f, 470.0f, 450.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (1360.0f, 870.0f, 90.0f, 150.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (20.0f, 880.0f, 1420.0f, 130.0f, 10.000f);

    //[UserPaint] Add your own custom painting code here..
    keyboard->setColour(MidiKeyboardComponent::keyDownOverlayColourId,look_and_feel->colours.get_theme(COLOUR_THEMES::BG_THEME).button_on_colour);
    keyboard->setColour(MidiKeyboardComponent::mouseOverKeyOverlayColourId,look_and_feel->colours.get_theme(COLOUR_THEMES::BG_THEME).button_on_colour.withAlpha(0.5f));
    //[/UserPaint]
}

void Monique_Ui_Mainwindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    synth_data->ui_scale_factor = getHeight()/original_h;
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    label_fx_delay->setBounds (960, 680, 120, 30);
    eq_7->setBounds (1270 - 60, 810 - 130, 60, 130);
    eq_6->setBounds (1200 - 60, 810 - 130, 60, 130);
    eq_5->setBounds (1140 - 60, 810 - 130, 60, 130);
    eq_4->setBounds (1070 - 60, 810 - 130, 60, 130);
    eq_3->setBounds (1010 - 60, 810 - 130, 60, 130);
    eq_2->setBounds (940 - 60, 810 - 130, 60, 130);
    eq_1->setBounds (880 - 60, 810 - 130, 60, 130);
    distortion->setBounds (880 - 60, 810 - 130, 60, 130);
    chorus_modulation->setBounds (890, 810 - 130, 60, 130);
    delay3->setBounds (1020 - 60, 810 - 130, 60, 130);
    delay2->setBounds (1080 - 60, 810 - 130, 60, 130);
    delay4->setBounds (1140 - 60, 810 - 130, 60, 130);
    reverb_room->setBounds (1210 - 60, 810 - 130, 60, 130);
    reverb_dry->setBounds (1270 - 60, 810 - 130, 60, 130);
    label_lfo_3->setBounds (900, 480, 60, 30);
    label_lfo_2->setBounds (900, 300, 60, 30);
    label_lfo_1->setBounds (900, 120, 60, 30);
    lfo_1->setBounds (900, 250 - 130, 60, 130);
    lfo_2->setBounds (900, 430 - 130, 60, 130);
    lfo_3->setBounds (900, 610 - 130, 60, 130);
    morpher_1->setBounds (260 - 60, 680, 60, 130);
    morpher_2->setBounds (320 - 60, 680, 60, 130);
    morpher_4->setBounds (380 - 60, 680, 60, 130);
    morpher_3->setBounds (440 - 60, 680, 60, 130);
    flt_input_13->setBounds (380 - 60, 610 - 130, 60, 130);
    flt_input_12->setBounds (320 - 60, 610 - 130, 60, 130);
    flt_input_11->setBounds (260 - 60, 610 - 130, 60, 130);
    flt_input_6->setBounds (200, 300, 60, 130);
    flt_input_7->setBounds (320 - 60, 430 - 130, 60, 130);
    flt_input_8->setBounds (380 - 60, 430 - 130, 60, 130);
    flt_input_3->setBounds (320, 250 - 130, 60, 130);
    flt_input_2->setBounds (320 - 60, 250 - 130, 60, 130);
    flt_input_1->setBounds (200, 120, 60, 130);
    button_edit_lfo_1->setBounds (960 - 60, 220, 60, 30);
    button_edit_lfo_2->setBounds (960 - 60, 400, 60, 30);
    button_edit_lfo_3->setBounds (960 - 60, 580, 60, 30);
    button_edit_input_env_3_3->setBounds (320, 580, 60, 30);
    button_edit_input_env_3_2->setBounds (260, 580, 60, 30);
    button_edit_input_env_3_1->setBounds (200, 580, 60, 30);
    button_edit_input_env_2_1->setBounds (200, 400, 60, 30);
    button_edit_input_env_2_2->setBounds (260, 400, 60, 30);
    button_edit_input_env_2_3->setBounds (320, 400, 60, 30);
    button_edit_input_env_1_3->setBounds (320, 220, 60, 30);
    button_edit_input_env_1_2->setBounds (260, 220, 60, 30);
    button_edit_input_env_1_1->setBounds (200, 220, 60, 30);
    button_edit_mfo_4->setBounds (440 - 60, 780, 60, 30);
    button_edit_mfo_3->setBounds (380 - 60, 780, 60, 30);
    button_edit_mfo_2->setBounds (320 - 60, 780, 60, 30);
    button_edit_mfo_1->setBounds (260 - 60, 780, 60, 30);
    button_edit_input_env_band_1->setBounds (820, 780, 60, 30);
    button_edit_input_env_band_2->setBounds (880, 780, 60, 30);
    effect_finalizer_switch2->setBounds (810, 819, 120, 30);
    button_edit_input_env_band_7->setBounds (1210, 780, 60, 30);
    button_edit_input_env_band_3->setBounds (950, 780, 60, 30);
    button_edit_input_env_band_4->setBounds (1010, 780, 60, 30);
    button_edit_input_env_band_5->setBounds (1080, 780, 60, 30);
    button_edit_input_env_band_6->setBounds (1140, 780, 60, 30);
    filter_type_bg_button_3->setBounds (1010, 480, 60, 130);
    filter_type_2_3->setBounds (1070 - 60, 513, 60, 30);
    filter_type_bg_button_2->setBounds (1010, 300, 60, 130);
    filter_type_2_2->setBounds (1070 - 60, 333, 60, 30);
    filter_type_bg_button_1->setBounds (1010, 120, 60, 130);
    filter_type_2_1->setBounds (1010, 153, 60, 30);
    button_toggle_morph_buttons_1->setBounds (260 - 60, 810, 60, 30);
    button_toggle_morph_buttons_2->setBounds (320 - 60, 810, 60, 30);
    button_toggle_morph_buttons_3->setBounds (380 - 60, 810, 60, 30);
    button_toggle_morph_buttons_4->setBounds (440 - 60, 810, 60, 30);
    label_band_hz_5->setBounds (1085, 636, 50, 30);
    label_band_hz_6->setBounds (1145, 636, 50, 30);
    label_band_hz_4->setBounds (1015, 636, 50, 30);
    label_band_hz_1->setBounds (825, 636, 50, 30);
    label_morph->setBounds (200, 645, 180, 30);
    label_band_hz_7->setBounds (1215, 636, 50, 30);
    label_arpeggiator->setBounds (580, 835, 220, 30);
    label_monique->setBounds (1220 - 180, 0, 180, 50);
    button_programm_replace->setBounds (850 - 60, 10, 60, 30);
    button_programm_new->setBounds (910 - 60, 10, 60, 30);
    bypass->setBounds (1345 - 60, 810 - 130, 60, 130);
    colour->setBounds (1285, 810 - 130, 60, 130);
    label_band_hz_2->setBounds (885, 636, 50, 30);
    label_band_hz_3->setBounds (955, 636, 50, 30);
    speed_multi->setBounds (1285, 1010 - 130, 60, 130);
    osc_wave_3->setBounds (90 - 60, 610 - 130, 60, 130);
    keyboard->setBounds (0, 1050, 1465, 185);
    glide2->setBounds (100, 880, 60, 130);
    arp_step_16->setBounds (1185 - 60, 1010 - 130, 60, 130);
    arp_step_15->setBounds (1125 - 60, 1010 - 130, 60, 130);
    arp_step_14->setBounds (1065 - 60, 1010 - 130, 60, 130);
    arp_step_13->setBounds (945, 880, 60, 130);
    arp_step_12->setBounds (935 - 60, 1010 - 130, 60, 130);
    arp_step_11->setBounds (875 - 60, 1010 - 130, 60, 130);
    arp_step_10->setBounds (815 - 60, 1010 - 130, 60, 130);
    arp_step_9->setBounds (755 - 60, 1010 - 130, 60, 130);
    arp_step_8->setBounds (685 - 60, 1010 - 130, 60, 130);
    arp_step_7->setBounds (625 - 60, 1010 - 130, 60, 130);
    arp_step_6->setBounds (565 - 60, 1010 - 130, 60, 130);
    arp_step_5->setBounds (505 - 60, 1010 - 130, 60, 130);
    arp_step_4->setBounds (435 - 60, 1010 - 130, 60, 130);
    arp_step_3->setBounds (375 - 60, 1010 - 130, 60, 130);
    arp_step_2->setBounds (315 - 60, 1010 - 130, 60, 130);
    arp_step_1->setBounds (255 - 60, 1010 - 130, 60, 130);
    shuffle->setBounds (30, 880, 60, 130);
    flt_sustain_4->setBounds (660 - 60, 810 - 130, 60, 130);
    flt_decay_4->setBounds (600 - 60, 810 - 130, 60, 130);
    flt_attack_4->setBounds (540 - 60, 810 - 130, 60, 130);
    flt_release_3->setBounds (720 - 60, 610 - 130, 60, 130);
    flt_sustain_time_3->setBounds (660 - 60, 610 - 130, 60, 130);
    flt_sustain_3->setBounds (600 - 60, 610 - 130, 60, 130);
    flt_decay_3->setBounds (540 - 60, 610 - 130, 60, 130);
    flt_attack_3->setBounds (480 - 60, 610 - 130, 60, 130);
    flt_release_2->setBounds (720 - 60, 430 - 130, 60, 130);
    flt_sustain_time_2->setBounds (660 - 60, 430 - 130, 60, 130);
    flt_sustain_2->setBounds (600 - 60, 430 - 130, 60, 130);
    flt_decay_2->setBounds (540 - 60, 430 - 130, 60, 130);
    flt_attack_2->setBounds (480 - 60, 430 - 130, 60, 130);
    flt_release_1->setBounds (720 - 60, 250 - 130, 60, 130);
    flt_sustain_time_1->setBounds (660 - 60, 250 - 130, 60, 130);
    flt_sustain_1->setBounds (600 - 60, 250 - 130, 60, 130);
    flt_decay_1->setBounds (540 - 60, 250 - 130, 60, 130);
    flt_attack_1->setBounds (420, 250 - 130, 60, 130);
    filter_type_3_1->setBounds (1010, 187, 60, 30);
    filter_type_3_2->setBounds (1070 - 60, 367, 60, 30);
    filter_type_3_3->setBounds (1070 - 60, 547, 60, 30);
    filter_type_5_1->setBounds (1010, 220, 60, 30);
    filter_type_5_2->setBounds (1070 - 60, 400, 60, 30);
    filter_type_5_3->setBounds (1070 - 60, 580, 60, 30);
    button_sequence_2->setBounds (315 - 60, 880, 60, 30);
    button_sequence_3->setBounds (375 - 60, 880, 60, 30);
    button_sequence_4->setBounds (435 - 60, 880, 60, 30);
    button_sequence_5->setBounds (505 - 60, 880, 60, 30);
    button_sequence_6->setBounds (565 - 60, 880, 60, 30);
    button_sequence_7->setBounds (625 - 60, 880, 60, 30);
    button_sequence_8->setBounds (685 - 60, 880, 60, 30);
    button_sequence_9->setBounds (755 - 60, 880, 60, 30);
    button_sequence_10->setBounds (815 - 60, 880, 60, 30);
    button_sequence_11->setBounds (875 - 60, 880, 60, 30);
    button_sequence_12->setBounds (935 - 60, 880, 60, 30);
    button_sequence_13->setBounds (1005 - 60, 880, 60, 30);
    button_sequence_14->setBounds (1065 - 60, 880, 60, 30);
    button_sequence_15->setBounds (1125 - 60, 880, 60, 30);
    button_sequence_16->setBounds (1185 - 60, 880, 60, 30);
    combo_programm->setBounds (335, 10, 255, 30);
    button_programm_left->setBounds (215, 10, 60, 30);
    button_programm_right->setBounds (650 - 60, 10, 60, 30);
    button_open_oszi->setBounds (1380, 10, 60, 30);
    button_open_midi_io_settings->setBounds (1290 - 60, 40 - 30, 60, 30);
    combo_bank->setBounds (275, 10, 60, 30);
    button_programm_load->setBounds (710 - 60, 10, 60, 30);
    osc_1->setBounds (100, 120, 60, 130);
    osc_2->setBounds (100, 300, 60, 130);
    osc_3->setBounds (160 - 60, 610 - 130, 60, 130);
    flt_cutoff_1->setBounds (1080, 250 - 130, 60, 130);
    flt_cutoff_2->setBounds (1140 - 60, 430 - 130, 60, 130);
    flt_cutoff_3->setBounds (1140 - 60, 610 - 130, 60, 130);
    flt_distortion_1->setBounds (1290 - 60, 250 - 130, 60, 130);
    flt_resonance_1->setBounds (1200 - 60, 250 - 130, 60, 130);
    flt_resonance_2->setBounds (1200 - 60, 430 - 130, 60, 130);
    flt_resonance_3->setBounds (1200 - 60, 610 - 130, 60, 130);
    flt_volume_1->setBounds (1380, 120, 60, 130);
    flt_volume_2->setBounds (1380, 300, 60, 130);
    flt_volume_3->setBounds (1380, 480, 60, 130);
    adsr_lfo_mix->setBounds (818, 120, 60, 130);
    lfo_opt_2->setBounds (818, 300, 60, 130);
    lfo_opt_3->setBounds (818, 480, 60, 130);
    button_sequence_1->setBounds (255 - 60, 880, 60, 30);
    flt_release_4->setBounds (720 - 60, 810 - 130, 60, 130);
    volume->setBounds (1380, 680, 60, 130);
    flt_distortion_2->setBounds (1290 - 60, 430 - 130, 60, 130);
    flt_distortion_3->setBounds (1290 - 60, 610 - 130, 60, 130);
    button_arp_speed_XNORM->setBounds (1285, 880, 60, 30);
    flt_attack_5->setBounds (90 - 60, 680, 60, 130);
    flt_attack_6->setBounds (160 - 60, 680, 60, 130);
    osc_wave_1->setBounds (30, 120, 60, 130);
    osc_wave_2->setBounds (30, 300, 60, 130);
    button_programm_delete->setBounds (1030 - 60, 10, 60, 30);
    filter_type_6_1->setBounds (1010, 120, 60, 30);
    filter_type_6_2->setBounds (1070 - 60, 300, 60, 30);
    filter_type_6_3->setBounds (1070 - 60, 480, 60, 30);
    button_ctrl_toggle->setBounds (100, 10, 60, 30);
    speed->setBounds (1275 - 60, 1010 - 130, 60, 130);
    button_open_morph->setBounds (440 - 60, 645, 60, 30);
    effect_finalizer_switch->setBounds (930, 819, 120, 30);
    button_values_toggle->setBounds (30, 10, 60, 30);
    octave_offset->setBounds (1380, 880, 60, 130);
    label_filter_inputs->setBounds (210, 75, 170, 30);
    label_oscillators->setBounds (30, 75, 130, 30);
    label_filter_envelope->setBounds (420, 75, 370, 30);
    label_lfo->setBounds (890, 75, 80, 30);
    label_filter_config->setBounds (980, 75, 250, 30);
    label_filter_fx->setBounds (1220, 75, 140, 30);
    label_out->setBounds (1370, 75, 80, 30);
    label_amp_envelope->setBounds (480, 645, 310, 30);
    label_glide->setBounds (100, 835, 60, 30);
    label_speed->setBounds (1225, 835, 120, 30);
    label_tune->setBounds (1380, 835, 60, 30);
    label_fm->setBounds (30, 645, 130, 30);
    volume_master_meter->setBounds (1380, 680, 60, 27);
    button_open_config2->setBounds (1350 - 60, 40 - 30, 60, 30);
    label_mod_mix->setBounds (793, 75, 110, 30);
    flt_pan_3->setBounds (1350 - 60, 610 - 130, 60, 130);
    flt_pan_2->setBounds (1350 - 60, 430 - 130, 60, 130);
    flt_pan_1->setBounds (1350 - 60, 250 - 130, 60, 130);
    button_reset_arp_tune->setBounds (1380, 880, 60, 30);
    button_show_active_input_r_2_3->setBounds (361, 289, 10, 10);
    button_show_active_input_l_2_3->setBounds (331, 289, 10, 10);
    button_show_active_input_r_2_2->setBounds (301, 289, 10, 10);
    button_show_active_input_l_2_2->setBounds (271, 289, 10, 10);
    button_show_active_input_r_2_1->setBounds (241, 289, 10, 10);
    button_show_active_input_l_2_1->setBounds (211, 289, 10, 10);
    button_show_active_input_r_3_3->setBounds (361, 469, 10, 10);
    button_show_active_input_l_3_3->setBounds (331, 469, 10, 10);
    button_show_active_input_r_3_2->setBounds (301, 469, 10, 10);
    button_show_active_input_l_3_2->setBounds (271, 469, 10, 10);
    button_show_active_input_r_3_1->setBounds (241, 469, 10, 10);
    button_show_active_input_l_3_1->setBounds (210, 469, 10, 10);
    button_programm_rename->setBounds (970 - 60, 10, 60, 30);
    flt_shape_1->setBounds (730, 250 - 130, 60, 130);
    flt_shape_2->setBounds (790 - 60, 430 - 130, 60, 130);
    flt_shape_3->setBounds (790 - 60, 610 - 130, 60, 130);
    button_programm_scratch->setBounds (780 - 60, 10, 60, 30);
    flt_shape_4->setBounds (790 - 60, 810 - 130, 60, 130);
    label_monoplugs->setBounds (1220 - 180, 40, 180, 30);
    label_reverb->setBounds (1150, 680, 120, 30);
    //[UserResized] Add your own custom resize handling here..

#include "mono_ui_includeHacks_END.h"
    resize_subeditors();

    resizer->setBounds( getWidth()-16, getHeight()-16, 16,16 );
    //[/UserResized]
}

void Monique_Ui_Mainwindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_edit_lfo_1)
    {
        //[UserButtonCode_button_edit_lfo_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->lfo_datas[0], button_edit_lfo_1, lfo_1, COLOUR_THEMES::FILTER_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_lfo_1]
    }
    else if (buttonThatWasClicked == button_edit_lfo_2)
    {
        //[UserButtonCode_button_edit_lfo_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->lfo_datas[1], button_edit_lfo_2, lfo_2, COLOUR_THEMES::FILTER_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_lfo_2]
    }
    else if (buttonThatWasClicked == button_edit_lfo_3)
    {
        //[UserButtonCode_button_edit_lfo_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->lfo_datas[2], button_edit_lfo_3, lfo_3, COLOUR_THEMES::FILTER_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_lfo_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_3)
    {
        //[UserButtonCode_button_edit_input_env_3_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[2]->input_envs[2], &synth_data->filter_datas[2]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_13, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_3_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_2)
    {
        //[UserButtonCode_button_edit_input_env_3_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[2]->input_envs[1], &synth_data->filter_datas[2]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_12, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_3_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_1)
    {
        //[UserButtonCode_button_edit_input_env_3_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[2]->input_envs[0], &synth_data->filter_datas[2]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_11, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_3_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_1)
    {
        //[UserButtonCode_button_edit_input_env_2_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[1]->input_envs[0], &synth_data->filter_datas[1]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_6, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_2_1]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_2)
    {
        //[UserButtonCode_button_edit_input_env_2_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[1]->input_envs[1], &synth_data->filter_datas[1]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_7, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_2_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_3)
    {
        //[UserButtonCode_button_edit_input_env_2_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[1]->input_envs[2], &synth_data->filter_datas[1]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_8, true );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_1_3]
        //[/UserButtonCode_button_edit_input_env_2_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_3)
    {
        //[UserButtonCode_button_edit_input_env_1_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[0]->input_envs[2], &synth_data->filter_datas[0]->input_envs[2]->sustain, buttonThatWasClicked, flt_input_3, false );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_1_3]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_2)
    {
        //[UserButtonCode_button_edit_input_env_1_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[0]->input_envs[1], &synth_data->filter_datas[0]->input_envs[1]->sustain, buttonThatWasClicked, flt_input_2, false );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_1_2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_1)
    {
        //[UserButtonCode_button_edit_input_env_1_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_env_popup,
            buttonThatWasClicked
        )
        else
        {
            open_env_popup( synth_data->filter_datas[0]->input_envs[0], &synth_data->filter_datas[0]->input_envs[0]->sustain, buttonThatWasClicked, flt_input_1, false );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_env_popup.midi_control );
        //[/UserButtonCode_button_edit_input_env_1_1]
    }
    else if (buttonThatWasClicked == button_edit_mfo_4)
    {
        //[UserButtonCode_button_edit_mfo_4] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->mfo_datas[2], button_edit_mfo_4, morpher_3, COLOUR_THEMES::MORPH_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_mfo_4]
    }
    else if (buttonThatWasClicked == button_edit_mfo_3)
    {
        //[UserButtonCode_button_edit_mfo_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->mfo_datas[3], button_edit_mfo_3, morpher_4, COLOUR_THEMES::MORPH_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_mfo_3]
    }
    else if (buttonThatWasClicked == button_edit_mfo_2)
    {
        //[UserButtonCode_button_edit_mfo_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->mfo_datas[1], button_edit_mfo_2, morpher_2, COLOUR_THEMES::MORPH_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_mfo_2]
    }
    else if (buttonThatWasClicked == button_edit_mfo_1)
    {
        //[UserButtonCode_button_edit_mfo_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->midi_lfo_popup,
            buttonThatWasClicked
        )
        else
        {
            open_mfo_popup( synth_data->mfo_datas[0], button_edit_mfo_1, morpher_1, COLOUR_THEMES::MORPH_THEME );
        }
        show_info_popup( buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control );
        //[/UserButtonCode_button_edit_mfo_1]
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
    else if (buttonThatWasClicked == effect_finalizer_switch2)
    {
        //[UserButtonCode_effect_finalizer_switch2] -- add your button handler code here..
        switch_finalizer_tab(false);
        //[/UserButtonCode_effect_finalizer_switch2]
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_7)
    {
        //[UserButtonCode_button_edit_input_env_band_7] -- add your button handler code here..
        open_env_popup( synth_data->eq_data->envs[6], &synth_data->eq_data->envs[6]->sustain, buttonThatWasClicked, eq_7, true );
        //[/UserButtonCode_button_edit_input_env_band_7]
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
            synth_data->filter_datas[flt_id]->filter_type = HPF;
        }
        show_info_popup( buttonThatWasClicked, synth_data->filter_datas[flt_id]->filter_type.midi_control );
        //[/UserButtonCode_filter_type_2_1]
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_1)
    {
        //[UserButtonCode_button_toggle_morph_buttons_1] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->morhp_switch_states[0],
            buttonThatWasClicked
        )
        else
        {
            synth_data->morhp_switch_states[0] ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->morhp_switch_states[0].midi_control );
        //[/UserButtonCode_button_toggle_morph_buttons_1]
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_2)
    {
        //[UserButtonCode_button_toggle_morph_buttons_2] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->morhp_switch_states[1],
            buttonThatWasClicked
        )
        else
        {
            synth_data->morhp_switch_states[1] ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->morhp_switch_states[1].midi_control );
        //[/UserButtonCode_button_toggle_morph_buttons_2]
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_3)
    {
        //[UserButtonCode_button_toggle_morph_buttons_3] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->morhp_switch_states[3],
            buttonThatWasClicked
        )
        else
        {
            synth_data->morhp_switch_states[3] ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->morhp_switch_states[3].midi_control );
        //[/UserButtonCode_button_toggle_morph_buttons_3]
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_4)
    {
        //[UserButtonCode_button_toggle_morph_buttons_4] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &synth_data->morhp_switch_states[2],
            buttonThatWasClicked
        )
        else
        {
            synth_data->morhp_switch_states[2] ^= true;
        }
        show_info_popup( buttonThatWasClicked, synth_data->morhp_switch_states[2].midi_control );
        //[/UserButtonCode_button_toggle_morph_buttons_4]
    }
    else if (buttonThatWasClicked == button_programm_replace)
    {
        //[UserButtonCode_button_programm_replace] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::REPLACE;
        const bool success = synth_data->replace();
        show_programs_and_select(true);
        button_flasher = new ButtonFlasher(this,buttonThatWasClicked,success);
        //[/UserButtonCode_button_programm_replace]
    }
    else if (buttonThatWasClicked == button_programm_new)
    {
        //[UserButtonCode_button_programm_new] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::CREATE;
        if( combo_programm->getText() == FACTORY_NAME )
        {
            combo_programm->setText( "PROGRAM FROM SCRATCH", dontSendNotification );
        }
        combo_programm->setEditableText(true);
        combo_programm->showEditor();
        //[/UserButtonCode_button_programm_new]
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
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        synth_data->load_prev();
        show_programs_and_select(true);
        //button_flasher = new ButtonFlasher(button_programm_load,success,1);
        //[/UserButtonCode_button_programm_left]
    }
    else if (buttonThatWasClicked == button_programm_right)
    {
        //[UserButtonCode_button_programm_right] -- add your button handler code here..
        program_edit_type = EDIT_TYPES::LOAD;
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        synth_data->load_next();
        show_programs_and_select(true);
        //button_flasher = new ButtonFlasher(button_programm_load,success,1);
        //[/UserButtonCode_button_programm_right]
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

            addChildComponent( amp_painter );
            resize_subeditors();
            amp_painter->setVisible(true);


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
        const bool success = synth_data->load();
        show_programs_and_select(true);
        button_flasher = new ButtonFlasher(this,button_programm_load,success);
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
            button_flasher = new ButtonFlasher(this,button_programm_delete,true);
        }
        else
        {
            button_flasher = new ButtonFlasher(this,button_programm_delete,false);
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

            addChildComponent(editor_morph = new Monique_Ui_MorphConfig( ui_refresher, look_and_feel ) );
            resize_subeditors();
            editor_morph->setVisible(true);
        }
        else
            editor_morph = nullptr;
        //[/UserButtonCode_button_open_morph]
    }
    else if (buttonThatWasClicked == effect_finalizer_switch)
    {
        //[UserButtonCode_effect_finalizer_switch] -- add your button handler code here..
        switch_finalizer_tab(true);
        //[/UserButtonCode_effect_finalizer_switch]
    }
    else if (buttonThatWasClicked == button_values_toggle)
    {
        //[UserButtonCode_button_values_toggle] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &look_and_feel->show_values_always,
            buttonThatWasClicked
        )
        else
        {
            look_and_feel->show_values_always ^= true;
        }
        show_info_popup( buttonThatWasClicked, look_and_feel->show_values_always.midi_control );
        //[/UserButtonCode_button_values_toggle]
    }
    else if (buttonThatWasClicked == button_open_config2)
    {
        //[UserButtonCode_button_open_config2] -- add your button handler code here..
        if( ! editor_global_settings )
        {
            close_all_subeditors();

            addChildComponent( editor_global_settings = new Monique_Ui_GlobalSettings(ui_refresher) );
            resize_subeditors();
            editor_global_settings->setVisible(true);
        }
        else
            editor_global_settings = nullptr;
        //[/UserButtonCode_button_open_config2]
    }
    else if (buttonThatWasClicked == button_reset_arp_tune)
    {
        //[UserButtonCode_button_reset_arp_tune] -- add your button handler code here..
        audio_processor->noteOn( 1, 60+synth_data->note_offset.get_value()-24, 1.0f );
        audio_processor->noteOff( 1, 60+synth_data->note_offset.get_value()-24, 0 );

        button_flasher = new ButtonFlasher(this,buttonThatWasClicked,true,1);
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
    else if (buttonThatWasClicked == button_programm_scratch)
    {
        //[UserButtonCode_button_programm_scratch] -- add your button handler code here..
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        if( clear_record_timer )
        {
            clear_record_timer = nullptr;
            button_flasher = new ButtonFlasher(this,buttonThatWasClicked,false);
        }
        else
        {
            synth_data->load_default();
            show_programs_and_select(true);
            button_flasher = new ButtonFlasher(this,buttonThatWasClicked,true);

            toggle_modulation_slider_top_button(delay4->get_top_button(),true);
        }
        //[/UserButtonCode_button_programm_scratch]
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
                const bool success = synth_data->create_new(new_name);
                button_flasher = new ButtonFlasher(this,button_programm_new,success);
            }
            else if( program_edit_type == RENAME )
            {
                const bool success = synth_data->rename(new_name);
                button_flasher = new ButtonFlasher(this,button_programm_rename,success);
            }
            show_programs_and_select(true);
        }
        else
        {
            String program_to_load = combo_programm->getText();
#if ASK_FOR_SAVE
            synth_data->ask_and_save_if_changed();
#endif
            int new_index = synth_data->get_current_bank_programms().indexOf(program_to_load);
            synth_data->set_current_program( new_index );
            const bool success = synth_data->load();
            button_flasher = new ButtonFlasher(this,button_programm_load,success,1);
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
        else if( mfo_popup )
        {
            open_mfo_popup( nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME );
        }
        else
        {
            close_all_subeditors();
        }

        success = true;
    }
    else if( key.getTextDescription() == "ctrl + +" ) // key.isCurrentlyDown(KeyPress::numberPadAdd)  )
    {
        synth_data->ui_scale_factor = synth_data->ui_scale_factor + 0.1;
        update_size();
        success = true;
    }
    else if( key.getTextDescription() == "ctrl + -"  )
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
    else if( key.getTextDescription() == "ctrl + E" )
    {
        synth_data->animate_envs ^= true;
        success = true;
    }
    else if( key.getTextDescription() == "ctrl + R" )
    {
        synth_data->animate_sliders ^= true;
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
                    slider->slider_value->triggerClick();

                    found = true;
                    break;
                }
                // SUPPORT FOR SINGLE BUTTONS
                else if( TextButton* button = dynamic_cast< TextButton* >( c ) )
                {
                    bool trigger_click = true;
                    if( button == button_programm_left )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_right )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_load )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_scratch )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_replace )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_new )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_rename )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_programm_delete )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_open_midi_io_settings )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_open_config2 )
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if( button == button_open_oszi )
                    {
                        trigger_click = false;
                        found = true;
                    }

                    if( trigger_click)
                    {
                        buttonClicked( button );
                        found = true;
                    }
                }
                else if( Slider* slider = dynamic_cast< Slider* >( c ) )
                {
                    slider->triggerClick();
                    found = true;
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
                                slider->slider_value->triggerClick();

                                found = true;
                                break;
                            }
                        }
                        while( (parent = parent->getParentComponent()) );
                    }
                }

                if( found )
                {
                    break;
                }
            }

            success = true;
        }
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
    if( not dynamic_cast< TextEditor* >( getCurrentlyFocusedComponent() ) ) // not combo_programm->isTextEditable() )
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

void Monique_Ui_Mainwindow::open_mfo_popup( LFOData*const mfo_data_, Button*const for_comp_, Monique_Ui_DualSlider*slider_, COLOUR_THEMES theme_ ) noexcept
{
    TURN_BUTTON_OFF( button_edit_mfo_1 )
    TURN_BUTTON_OFF( button_edit_mfo_2 )
    TURN_BUTTON_OFF( button_edit_mfo_3 )
    TURN_BUTTON_OFF( button_edit_mfo_4 )
    TURN_BUTTON_OFF( button_edit_lfo_1 )
    TURN_BUTTON_OFF( button_edit_lfo_2 )
    TURN_BUTTON_OFF( button_edit_lfo_3 )

    if( ! mfo_popup  )
    {
        if( mfo_data_ )
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
            if( env_popup ) // IGNORE POPUP
            {
                removeChildComponent(env_popup);
            }
            for( int i = 0 ;  i != getNumChildComponents() ; ++i )
            {
                comps_to_observe.add( getChildComponent(i) );
            }
            comps_to_observe.removeFirstMatchingValue(for_comp_);
            comps_to_observe.removeFirstMatchingValue(button_open_morph);

            if( editor_morph )
            {
                for( int i = 0 ;  i != editor_morph->getNumChildComponents() ; ++i )
                {
                    comps_to_observe.removeFirstMatchingValue( editor_morph->getChildComponent(i) );
                }
                comps_to_observe.removeFirstMatchingValue(editor_morph);
            }
            if( amp_painter )
            {
                for( int i = 0 ;  i != amp_painter->getNumChildComponents() ; ++i )
                {
                    comps_to_observe.removeFirstMatchingValue( amp_painter->getChildComponent(i) );
                }
                comps_to_observe.removeFirstMatchingValue(amp_painter);
            }
            comps_to_observe.removeFirstMatchingValue(button_open_oszi);

            comps_to_observe.add(this);
            if( popup )
            {
                addChildComponent(popup);
            }
            if( option_popup )
            {
                addChildComponent(option_popup);
            }
            if( env_popup )
            {
                addChildComponent(env_popup);
            }

            addChildComponent( mfo_popup = new Monique_Ui_MFOPopup( ui_refresher, this, mfo_data_, theme_ ) );
            mfo_popup->set_element_to_show( for_comp_,slider_);
            mfo_popup->set_clickable_components( comps_to_observe );
            if( for_comp_ )
            {
                TURN_BUTTON_ON( for_comp_ )
            }
            resize_subeditors();
            mfo_popup->refresh();
            mfo_popup->setVisible(true);
        }
    }
    else
    {
        if( mfo_popup->is_open_for() == mfo_data_ )
        {
            if( for_comp_ )
            {
                TURN_BUTTON_OFF( for_comp_ )
            }
            mfo_popup = nullptr;
        }
        else if( mfo_data_ == nullptr )
        {
            if( for_comp_ )
            {
                TURN_BUTTON_OFF( for_comp_ )
            }
            mfo_popup = nullptr;
        }
        else
        {
            mfo_popup = nullptr;
            open_mfo_popup( mfo_data_, for_comp_, slider_, theme_ );
        }
    }
}

void Monique_Ui_Mainwindow::open_env_popup( ENVData*const env_data_,
        Parameter*const sustain_,
        Button*const for_comp_,
        Monique_Ui_DualSlider*slider_,
        bool has_negative_sustain_ ) noexcept
{

    TURN_BUTTON_OFF( button_edit_input_env_1_1 )
    TURN_BUTTON_OFF( button_edit_input_env_1_2 )
    TURN_BUTTON_OFF( button_edit_input_env_1_3 )
    TURN_BUTTON_OFF( button_edit_input_env_2_1 )
    TURN_BUTTON_OFF( button_edit_input_env_2_2 )
    TURN_BUTTON_OFF( button_edit_input_env_2_3 )
    TURN_BUTTON_OFF( button_edit_input_env_3_1 )
    TURN_BUTTON_OFF( button_edit_input_env_3_2 )
    TURN_BUTTON_OFF( button_edit_input_env_3_3 )

    TURN_BUTTON_OFF( button_edit_input_env_band_1 )
    TURN_BUTTON_OFF( button_edit_input_env_band_2 )
    TURN_BUTTON_OFF( button_edit_input_env_band_3 )
    TURN_BUTTON_OFF( button_edit_input_env_band_4 )
    TURN_BUTTON_OFF( button_edit_input_env_band_5 )
    TURN_BUTTON_OFF( button_edit_input_env_band_6 )
    TURN_BUTTON_OFF( button_edit_input_env_band_7 )

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
            if( mfo_popup ) // IGNORE POPUP
            {
                removeChildComponent(mfo_popup);
            }
            for( int i = 0 ;  i != getNumChildComponents() ; ++i )
            {
                comps_to_observe.add( getChildComponent(i) );
            }
            comps_to_observe.removeFirstMatchingValue(for_comp_);

            if( amp_painter )
            {
                for( int i = 0 ;  i != amp_painter->getNumChildComponents() ; ++i )
                {
                    comps_to_observe.removeFirstMatchingValue( amp_painter->getChildComponent(i) );
                }
                comps_to_observe.removeFirstMatchingValue(amp_painter);
            }
            comps_to_observe.removeFirstMatchingValue(button_open_oszi);

            comps_to_observe.add(this);
            if( popup )
            {
                addChildComponent(popup);
            }
            if( option_popup )
            {
                addChildComponent(option_popup);
            }
            if( mfo_popup )
            {
                addChildComponent(mfo_popup);
            }

            addChildComponent( env_popup = new Monique_Ui_ENVPopup( ui_refresher, this, env_data_, sustain_, for_comp_->getX() < getWidth()/2, has_negative_sustain_ ) );
            env_popup->set_element_to_show(for_comp_,slider_);
            env_popup->set_clickable_components( comps_to_observe );

            if( for_comp_ )
            {
                TURN_BUTTON_ON( for_comp_ )
            }
            resize_subeditors();
            env_popup->refresh();
            env_popup->setVisible(true);
        }
    }
    else
    {
        if( env_popup->is_open_for() == env_data_ )
        {
            if( for_comp_ )
            {
                TURN_BUTTON_OFF( for_comp_ )
            }
            env_popup = nullptr;
        }
        else if( env_data_ == nullptr )
        {
            if( for_comp_ )
            {
                TURN_BUTTON_OFF( for_comp_ )
            }
            env_popup = nullptr;
        }
        else
        {
            env_popup = nullptr;
            open_env_popup( env_data_, sustain_, for_comp_, slider_, has_negative_sustain_ );
        }
    }
}
void Monique_Ui_Mainwindow::open_mfo_popup( Monique_Ui_DualSlider*dual_slider_ ) noexcept
{
    if( dual_slider_ == morpher_1 )
    {
        buttonClicked(button_edit_mfo_1);
    }
    else if( dual_slider_ == morpher_2 )
    {
        buttonClicked(button_edit_mfo_2);
    }
    else if( dual_slider_ == morpher_3 )
    {
        buttonClicked(button_edit_mfo_4);
    }
    else if( dual_slider_ == morpher_4 )
    {
        buttonClicked(button_edit_mfo_3);
    }
    else if( dual_slider_ == lfo_1 )
    {
        buttonClicked(button_edit_lfo_1);
    }
    else if( dual_slider_ == lfo_2 )
    {
        buttonClicked(button_edit_lfo_2);
    }
    else if( dual_slider_ == lfo_3 )
    {
        buttonClicked(button_edit_lfo_3);
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
        addChildComponent( option_popup = new Monique_Ui_OptionPopup( ui_refresher, this, param_a_, param_b_ ) );
        option_popup->set_element_to_show(for_comp_);
        option_popup->set_infos(text_a_, text_b_, tool_tip_a_, tool_tip_b_);
        resize_subeditors();
        option_popup->setVisible(true);
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
bool Monique_Ui_Mainwindow::handle_keep_mfo_open( ModulationSliderConfigBase*const caller_config_ ) noexcept
{
    bool keeps_open = false;
    if( mfo_popup )
    {
        if( not caller_config_->action_keep_env_pop_open_for( mfo_popup->is_open_for() ) )
        {
            if( synth_data->auto_close_env_popup )
            {
                open_mfo_popup(nullptr,nullptr,nullptr,COLOUR_THEMES::DUMMY_THEME);
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
        global_slider_settings_changed(env_popup);
    }
    if( mfo_popup )
    {
        mfo_popup->setSize( mfo_popup->original_w*(1.0f/original_w*getWidth()), mfo_popup->original_h*(1.0f/original_h*getHeight() ) );
        mfo_popup->update_positions();
        global_slider_settings_changed(mfo_popup);
    }
    if( option_popup )
    {
        option_popup->setSize( option_popup->original_w*(1.0f/original_w*getWidth()), option_popup->original_h*(1.0f/original_h*getHeight() ) );
        option_popup->update_positions();
    }

    if( amp_painter )
    {
        amp_painter->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(), keyboard->getHeight());
        //amp_painter->setBounds(0, 50, getWidth(), getHeight()-50 );
    }

    resizer->toFront(true);
}

void Monique_Ui_Mainwindow::open_midi_editor_if_closed() noexcept
{
#ifdef IS_STANDALONE
    if( not editor_midiio )
    {
        //close_all_subeditors();

        addChildComponent( editor_midiio = new Monique_Ui_MidiIO( ui_refresher, audio_processor ) );
        resize_subeditors();
        editor_midiio->setVisible(true);
    }
#else
    if( popup )
    {
        show_info_popup(nullptr,nullptr);
    }
    else
    {
        midi_control_handler->toggle_midi_learn();
    }
#endif
}
void Monique_Ui_Mainwindow::open_setup_editor_if_closed() noexcept
{
    if( not editor_global_settings )
    {
        close_all_subeditors();

        addChildComponent( editor_global_settings = new Monique_Ui_GlobalSettings(ui_refresher) );
        resize_subeditors();
        editor_global_settings->setVisible(true);
    }
}

void Monique_Ui_Mainwindow::flash_midi_editor_button() noexcept
{
    flash_counter = 30;
}

void Monique_Ui_Mainwindow::mouseEnter (const MouseEvent& event)
{
    if( option_popup )
    {
        open_option_popup(nullptr,nullptr,nullptr,"","","","");
    }
}

void Monique_Ui_Mainwindow::open_env_or_lfo_popup_by_midi( Parameter* param_ ) noexcept
{
    if( IS_MIDI_LEARN )
    {
        return;
    }

    if( param_ == &synth_data->midi_lfo_popup )
    {
        switch( synth_data->midi_lfo_popup )
        {
        case 9 :
        case 5 :
        case 0 :
            open_mfo_popup( nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME );
            break;
        case 1 :
            buttonClicked( button_edit_mfo_1 );
            break;
        case 2 :
            buttonClicked( button_edit_mfo_2 );
            break;
        case 3 :
            buttonClicked( button_edit_mfo_3 );
            break;
        case 4 :
            buttonClicked( button_edit_mfo_4 );
            break;
        case 6 :
            buttonClicked( button_edit_lfo_1 );
            break;
        case 7 :
            buttonClicked( button_edit_lfo_2 );
            break;
        case 8 :
            buttonClicked( button_edit_lfo_3 );
            break;
        }
    }
    if( param_ == &synth_data->midi_env_popup )
    {
        switch( synth_data->midi_env_popup )
        {
        case 10 :
        case 18 :
        case 0 :
            open_env_popup( nullptr, nullptr, nullptr, nullptr, nullptr );
            break;
        case 1 :
            buttonClicked( button_edit_input_env_1_1 );
            break;
        case 2 :
            buttonClicked( button_edit_input_env_1_2 );
            break;
        case 3 :
            buttonClicked( button_edit_input_env_1_3 );
            break;
        case 4 :
            buttonClicked( button_edit_input_env_2_1 );
            break;
        case 5 :
            buttonClicked( button_edit_input_env_2_2 );
            break;
        case 6 :
            buttonClicked( button_edit_input_env_2_3 );
            break;
        case 7 :
            buttonClicked( button_edit_input_env_3_1 );
            break;
        case 8 :
            buttonClicked( button_edit_input_env_3_2 );
            break;
        case 9 :
            buttonClicked( button_edit_input_env_3_3 );
            break;
        case 11 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_1 );
            break;
        case 12 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_2 );
            break;
        case 13 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_3 );
            break;
        case 14 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_4 );
            break;
        case 15 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_5 );
            break;
        case 16 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_6 );
            break;
        case 17 :
            switch_finalizer_tab(false);
            buttonClicked( button_edit_input_env_band_7 );
            break;
        }
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_Mainwindow" componentName=""
                 parentClasses="public AudioProcessorEditor, public Monique_Ui_Refreshable, public AsyncUpdater, public ParameterListener"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_" variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;AudioProcessorEditor(ui_refresher_-&gt;audio_processor),&#10;original_w(1465), original_h(1235)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="1235">
  <METHODS>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
    <METHOD name="keyStateChanged (const bool isKeyDown)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="810 670 550 150" cornerSize="10" fill="solid: ffff11ff"
               hasStroke="0"/>
    <ROUNDRECT pos="1200 870 160 150" cornerSize="10" fill="solid: ffffff11"
               hasStroke="0"/>
    <ROUNDRECT pos="180 870 1020 150" cornerSize="10" fill="solid: ffffff11"
               hasStroke="0"/>
    <RECT pos="810 790 41 30" fill="solid: ffff11ff" hasStroke="0"/>
    <RECT pos="950 120 60 130" fill="linear: 1000 170, 970 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="790 120 110 130" fill="linear: 800 170, 850 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="790 300 110 130" fill="linear: 800 170, 850 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="960 300 60 130" fill="linear: 1000 170, 970 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="950 480 60 130" fill="linear: 1000 170, 970 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="790 480 110 130" fill="linear: 800 170, 850 170, 0=ff1111ff, 1=301111ff"
          hasStroke="0"/>
    <RECT pos="920 530 140 30" fill="solid: ff1111ff" hasStroke="0"/>
    <RECT pos="920 170 140 30" fill="solid: ff1111ff" hasStroke="0"/>
    <RECT pos="920 350 140 30" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="10 60 160 760" cornerSize="10" fill="solid: ff11ffff" hasStroke="0"/>
    <ROUNDRECT pos="20 60 150 570" cornerSize="10" fill="solid: ffff1111" hasStroke="0"/>
    <ROUNDRECT pos="190 110 200 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="190 270 200 170" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="190 450 200 170" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="410 110 390 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="410 290 390 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="410 470 390 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <RECT pos="380 119 58 30" fill="solid: ff1111ff" hasStroke="0"/>
    <RECT pos="380 299 58 30" fill="solid: ff1111ff" hasStroke="0"/>
    <RECT pos="380 479 58 30" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="890 110 80 150" cornerSize="10" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="890 290 80 150" cornerSize="10" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="890 470 80 150" cornerSize="10" fill="solid: ff1111ff" hasStroke="0"/>
    <ELLIPSE pos="797 134 100 100" fill="solid: ff050505" hasStroke="0"/>
    <ROUNDRECT pos="1000 110 450 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="1000 290 450 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ELLIPSE pos="802 139 90 90" fill="solid: ff1111ff" hasStroke="0"/>
    <ELLIPSE pos="797 314 100 100" fill="solid: ff050505" hasStroke="0"/>
    <ELLIPSE pos="802 319 90 90" fill="solid: ff1111ff" hasStroke="0"/>
    <ELLIPSE pos="797 494 100 100" fill="solid: ff050505" hasStroke="0"/>
    <ELLIPSE pos="802 499 90 90" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="190 640 260 210" cornerSize="10" fill="solid: ffffffee"
               hasStroke="0"/>
    <ROUNDRECT pos="1255 240 10 40" cornerSize="5" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="1255 420 10 40" cornerSize="5" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="10 870 170 150" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1370 670 80 150" cornerSize="10" fill="solid: ff11ff11"
               hasStroke="0"/>
    <ROUNDRECT pos="200 450 1064 10" cornerSize="5" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="200 270 1064 10" cornerSize="5" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="470 640 950 20" cornerSize="5" fill="linear: 815 660, 1360 660, 0=ffff11ff, 1=ff1111ff"
               hasStroke="0"/>
    <RECT pos="780 730 41 30" fill="solid: ffff11ff" hasStroke="0"/>
    <RECT pos="1340 730 40 30" fill="linear: 1360 740, 1370 740, 0=ffff11ff, 1=ff11ff11"
          hasStroke="0"/>
    <RECT pos="1400 250 20 40" fill="linear: 1410 260, 1410 285, 0=ff1111ff, 1=ff11ff"
          hasStroke="0"/>
    <RECT pos="1400 430 20 40" fill="linear: 1410 440, 1410 465, 0=ff1111ff, 1=ff11ff"
          hasStroke="0"/>
    <RECT pos="80 810 30 70" fill="linear: 90 870, 90 820, 0=ffffff11, 1=ff11ffff"
          hasStroke="0"/>
    <ROUNDRECT pos="470 640 330 180" cornerSize="10" fill="solid: ffff11ff"
               hasStroke="0"/>
    <ROUNDRECT pos="10 -10 1440 60" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <RECT pos="270 610 40 40" fill="linear: 290 620, 290 635, 0=0, 1=ffffffee"
          hasStroke="0"/>
    <RECT pos="330 845 40 25" fill="linear: 240 870, 240 855, 0=0, 1=ffffffee"
          hasStroke="0"/>
    <RECT pos="170 725 30 40" fill="linear: 170 720, 185 720, 0=0, 1=ffffffee"
          hasStroke="0"/>
    <RECT pos="440 725 40 40" fill="linear: 470 730, 455 730, 0=0, 1=ffffffee"
          hasStroke="0"/>
    <RECT pos="160 120 40 30" fill="linear: 170 130, 190 130, 0=ffff1111, 1=ff1111ff"
          hasStroke="0"/>
    <RECT pos="160 300 40 30" fill="linear: 170 130, 190 130, 0=ffff1111, 1=ff1111ff"
          hasStroke="0"/>
    <RECT pos="160 480 40 30" fill="linear: 170 130, 190 130, 0=ffff1111, 1=ff1111ff"
          hasStroke="0"/>
    <ROUNDRECT pos="1395 610 30 50" cornerSize="5" fill="solid: ff1111ff" hasStroke="0"/>
    <ROUNDRECT pos="1000 470 450 150" cornerSize="10" fill="solid: ff1111ff"
               hasStroke="0"/>
    <ROUNDRECT pos="1360 870 90 150" cornerSize="10" fill="solid: ffffff11"
               hasStroke="0"/>
    <ROUNDRECT pos="20 880 1420 130" cornerSize="10" fill="solid: ffffff11"
               hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="e42bec80710ce3bc" memberName="label_fx_delay" virtualName=""
         explicitFocusOrder="0" pos="960 680 120 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DELAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="12a573d837478d38" memberName="eq_7" virtualName=""
                    explicitFocusOrder="0" pos="1270r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,6)"/>
  <GENERICCOMPONENT name="" id="2b128fb147c2823c" memberName="eq_6" virtualName=""
                    explicitFocusOrder="0" pos="1200r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,5)"/>
  <GENERICCOMPONENT name="" id="8a0f89a0c0f219b8" memberName="eq_5" virtualName=""
                    explicitFocusOrder="0" pos="1140r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,4)"/>
  <GENERICCOMPONENT name="" id="3b0e3a8ef55d061a" memberName="eq_4" virtualName=""
                    explicitFocusOrder="0" pos="1070r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="1dbf561cd93cbd59" memberName="eq_3" virtualName=""
                    explicitFocusOrder="0" pos="1010r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="30a759af59bc090b" memberName="eq_2" virtualName=""
                    explicitFocusOrder="0" pos="940r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="5d07e2bb48e90cc6" memberName="eq_1" virtualName=""
                    explicitFocusOrder="0" pos="880r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EQSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="b482d3e604966296" memberName="distortion" virtualName=""
                    explicitFocusOrder="0" pos="880r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FXDistortionSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9378cae1ce589256" memberName="chorus_modulation"
                    virtualName="" explicitFocusOrder="0" pos="890 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new CModSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="563dffc9e556e1d7" memberName="delay3" virtualName=""
                    explicitFocusOrder="0" pos="1020r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new DelaySlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="49d3d717347ff877" memberName="delay2" virtualName=""
                    explicitFocusOrder="0" pos="1080r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new DelayReflexSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="1e7a797188cff129" memberName="delay4" virtualName=""
                    explicitFocusOrder="0" pos="1140r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new DelayRecordSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="19311f1c6e549e68" memberName="reverb_room" virtualName=""
                    explicitFocusOrder="0" pos="1210r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new RRoomSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9d2507984890a079" memberName="reverb_dry" virtualName=""
                    explicitFocusOrder="0" pos="1270r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new RDrySlConfig(synth_data)"/>
  <LABEL name="" id="e5adef150f6a349b" memberName="label_lfo_3" virtualName=""
         explicitFocusOrder="0" pos="900 480 60 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="5fdd521f76fef95d" memberName="label_lfo_2" virtualName=""
         explicitFocusOrder="0" pos="900 300 60 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="205fd7cb7eabe6d3" memberName="label_lfo_1" virtualName=""
         explicitFocusOrder="0" pos="900 120 60 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO 1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="2a31f2713e80bed3" memberName="lfo_1" virtualName=""
                    explicitFocusOrder="0" pos="900 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="e36ec1f3ea5f1edf" memberName="lfo_2" virtualName=""
                    explicitFocusOrder="0" pos="900 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="10b142e0e3bd1edf" memberName="lfo_3" virtualName=""
                    explicitFocusOrder="0" pos="900 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new LFOSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="ab7bfe937e5ada83" memberName="morpher_1" virtualName=""
                    explicitFocusOrder="0" pos="260r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="d7bed13dc76b014a" memberName="morpher_2" virtualName=""
                    explicitFocusOrder="0" pos="320r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="f1f5ea6816f11113" memberName="morpher_4" virtualName=""
                    explicitFocusOrder="0" pos="380r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="6319f13308da05dc" memberName="morpher_3" virtualName=""
                    explicitFocusOrder="0" pos="440r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new MorphSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="d2e2be5869047a2e" memberName="flt_input_13" virtualName=""
                    explicitFocusOrder="0" pos="380r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,2)"/>
  <GENERICCOMPONENT name="" id="7371ee7afd1877b4" memberName="flt_input_12" virtualName=""
                    explicitFocusOrder="0" pos="320r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,1)"/>
  <GENERICCOMPONENT name="" id="f3d6d4daa7867cda" memberName="flt_input_11" virtualName=""
                    explicitFocusOrder="0" pos="260r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,2,0)"/>
  <GENERICCOMPONENT name="" id="cc59ad897708e932" memberName="flt_input_6" virtualName=""
                    explicitFocusOrder="0" pos="200 300 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,0)"/>
  <GENERICCOMPONENT name="" id="30402f9a5bf56bfb" memberName="flt_input_7" virtualName=""
                    explicitFocusOrder="0" pos="320r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,1)"/>
  <GENERICCOMPONENT name="" id="e54fd10f87874627" memberName="flt_input_8" virtualName=""
                    explicitFocusOrder="0" pos="380r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,1,2)"/>
  <GENERICCOMPONENT name="" id="9abcdbe824977dbc" memberName="flt_input_3" virtualName=""
                    explicitFocusOrder="0" pos="320 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,2)"/>
  <GENERICCOMPONENT name="" id="6af45f57190e5260" memberName="flt_input_2" virtualName=""
                    explicitFocusOrder="0" pos="320r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,1)"/>
  <GENERICCOMPONENT name="" id="ecbcc81adebe9850" memberName="flt_input_1" virtualName=""
                    explicitFocusOrder="0" pos="200 120 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new InputSlConfig(synth_data,0,0)"/>
  <TEXTBUTTON name="" id="4b37bf0a7963b5f6" memberName="button_edit_lfo_1"
              virtualName="" explicitFocusOrder="0" pos="960r 220 60 30" tooltip="Open/Close a popup to edit this LFO.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d7419bd08a298b54" memberName="button_edit_lfo_2"
              virtualName="" explicitFocusOrder="0" pos="960r 400 60 30" tooltip="Open/Close a popup to edit this LFO.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7363964abd478946" memberName="button_edit_lfo_3"
              virtualName="" explicitFocusOrder="0" pos="960r 580 60 30" tooltip="Open/Close a popup to edit this LFO.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4ed4547628245141" memberName="button_edit_input_env_3_3"
              virtualName="" explicitFocusOrder="0" pos="320 580 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d93900dcb94156f" memberName="button_edit_input_env_3_2"
              virtualName="" explicitFocusOrder="0" pos="260 580 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="1d4a552f9f250c90" memberName="button_edit_input_env_3_1"
              virtualName="" explicitFocusOrder="0" pos="200 580 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6c31c6d826c76bfd" memberName="button_edit_input_env_2_1"
              virtualName="" explicitFocusOrder="0" pos="200 400 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="283cd78e11cc7a6e" memberName="button_edit_input_env_2_2"
              virtualName="" explicitFocusOrder="0" pos="260 400 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="47db5d103cbca51f" memberName="button_edit_input_env_2_3"
              virtualName="" explicitFocusOrder="0" pos="320 400 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f7dd0a8e5d005547" memberName="button_edit_input_env_1_3"
              virtualName="" explicitFocusOrder="0" pos="320 220 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="cfad8aa44907e8cf" memberName="button_edit_input_env_1_2"
              virtualName="" explicitFocusOrder="0" pos="260 220 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b915f30bfb983dc7" memberName="button_edit_input_env_1_1"
              virtualName="" explicitFocusOrder="0" pos="200 220 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this input.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="971beb8b5ba3cae1" memberName="button_edit_mfo_4"
              virtualName="" explicitFocusOrder="0" pos="440r 780 60 30" tooltip="Open/Close the morph oscillator popup to edit the mfo for this morph group.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bc837211a67f57ac" memberName="button_edit_mfo_3"
              virtualName="" explicitFocusOrder="0" pos="380r 780 60 30" tooltip="Open/Close the morph oscillator popup to edit the mfo for this morph group.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b0af912df34ced45" memberName="button_edit_mfo_2"
              virtualName="" explicitFocusOrder="0" pos="320r 780 60 30" tooltip="Open/Close the morph oscillator popup to edit the mfo for this morph group.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ebfbc2e7bb21bbdc" memberName="button_edit_mfo_1"
              virtualName="" explicitFocusOrder="0" pos="260r 780 60 30" tooltip="Open/Close the morph oscillator popup to edit the mfo for this morph group.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="972bb72707bb206b" memberName="button_edit_input_env_band_1"
              virtualName="" explicitFocusOrder="0" pos="820 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e4790e6c91bf6fec" memberName="button_edit_input_env_band_2"
              virtualName="" explicitFocusOrder="0" pos="880 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ec6e85c0b9db24e4" memberName="effect_finalizer_switch2"
              virtualName="" explicitFocusOrder="0" pos="810 819 120 30" tooltip="Switch to the EQ bank."
              bgColOff="ffff11ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="EQ"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ac8fdd08954a9d8" memberName="button_edit_input_env_band_7"
              virtualName="" explicitFocusOrder="0" pos="1210 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c5b44a8882d48b9" memberName="button_edit_input_env_band_3"
              virtualName="" explicitFocusOrder="0" pos="950 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="55e4c7770dd6c6a5" memberName="button_edit_input_env_band_4"
              virtualName="" explicitFocusOrder="0" pos="1010 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="325021dee5961ad1" memberName="button_edit_input_env_band_5"
              virtualName="" explicitFocusOrder="0" pos="1080 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7d42e1a02a42a873" memberName="button_edit_input_env_band_6"
              virtualName="" explicitFocusOrder="0" pos="1140 780 60 30" tooltip="Open/Close the envelope popup to edit the envelope for this band.&#10;&#10;Note: check the popup settings on the right of the popup to setup its close handling."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&#9675;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6e6e54760ba17ed8" memberName="filter_type_bg_button_3"
              virtualName="" explicitFocusOrder="0" pos="1010 480 60 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="e53892e80132f60c" memberName="filter_type_2_3"
              virtualName="" explicitFocusOrder="0" pos="1070r 513 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bc60a434f283f91e" memberName="filter_type_bg_button_2"
              virtualName="" explicitFocusOrder="0" pos="1010 300 60 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="121af38bc5254d57" memberName="filter_type_2_2"
              virtualName="" explicitFocusOrder="0" pos="1070r 333 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="40809d639d290176" memberName="filter_type_bg_button_1"
              virtualName="" explicitFocusOrder="0" pos="1010 120 60 130" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="ab74fcbdb09aa48f" memberName="filter_type_2_1"
              virtualName="" explicitFocusOrder="0" pos="1010 153 60 30" tooltip="Set the filter type to HIGH PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="HP"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="47ae14bae130124a" memberName="button_toggle_morph_buttons_1"
              virtualName="" explicitFocusOrder="0" pos="260r 810 60 30" tooltip="Toggles between the button states of the left and right morph side."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="OSC-T"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fbd7e42d8c2cdede" memberName="button_toggle_morph_buttons_2"
              virtualName="" explicitFocusOrder="0" pos="320r 810 60 30" tooltip="Toggles between the button states of the left and right morph side."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="FLT-T"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b52d35686740fb7e" memberName="button_toggle_morph_buttons_3"
              virtualName="" explicitFocusOrder="0" pos="380r 810 60 30" tooltip="Toggles between the button states of the left and right morph side."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="ARP-T"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8f9c476f5a07fa21" memberName="button_toggle_morph_buttons_4"
              virtualName="" explicitFocusOrder="0" pos="440r 810 60 30" tooltip="Toggles between the button states of the left and right morph side."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="FX-T"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="DL" id="89a933c800933b8f" memberName="label_band_hz_5"
         virtualName="" explicitFocusOrder="0" pos="1085 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="1.3kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="8a859696dca2f1f0" memberName="label_band_hz_6"
         virtualName="" explicitFocusOrder="0" pos="1145 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="2.6kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ec2bcb9f1c216890" memberName="label_band_hz_4"
         virtualName="" explicitFocusOrder="0" pos="1015 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="660Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="b8619ebb44316d58" memberName="label_band_hz_1"
         virtualName="" explicitFocusOrder="0" pos="825 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="&lt;82Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="660b9ead77642f4f" memberName="label_morph" virtualName=""
         explicitFocusOrder="0" pos="200 645 180 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MORPH (MO) SECTION"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="ceab7b618b72215f" memberName="label_band_hz_7"
         virtualName="" explicitFocusOrder="0" pos="1215 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="&gt;2.6kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="c72f149b5e4ef3a1" memberName="label_arpeggiator"
         virtualName="" explicitFocusOrder="0" pos="580 835 220 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ARPEGGIATOR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="39e8fb50cf1d668d" memberName="label_monique" virtualName=""
         explicitFocusOrder="0" pos="1220r 0 180 50" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="M O N I Q U E" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="250" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="b91a29c51d2d93f1" memberName="button_programm_replace"
              virtualName="" explicitFocusOrder="0" pos="850r 10 60 30" tooltip="Replaces the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="SAVE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9481aeac211cafc0" memberName="button_programm_new"
              virtualName="" explicitFocusOrder="0" pos="910r 10 60 30" tooltip="Create a new program from the current state."
              bgColOff="ff000000" textCol="ffbcff00" textColOn="ffd0ff00" buttonText="SAVE AS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="83c667b94dd3ef45" memberName="bypass" virtualName=""
                    explicitFocusOrder="0" pos="1345r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new BypassConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="1f9f546ceacaa4b2" memberName="colour" virtualName=""
                    explicitFocusOrder="0" pos="1285 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FColourSlConfig(synth_data)"/>
  <LABEL name="DL" id="4c9a611da59481e8" memberName="label_band_hz_2"
         virtualName="" explicitFocusOrder="0" pos="885 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="165Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="9e42519baddf946e" memberName="label_band_hz_3"
         virtualName="" explicitFocusOrder="0" pos="955 636 50 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="330Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="8916123bb68766dc" memberName="speed_multi" virtualName=""
                    explicitFocusOrder="0" pos="1285 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new SpeedMultiSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="92e86ca444a56d1e" memberName="osc_wave_3" virtualName=""
                    explicitFocusOrder="0" pos="90r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="a8343a0b5df2dc06" memberName="keyboard" virtualName="MidiKeyboardComponent"
                    explicitFocusOrder="0" pos="0 1050 1465 185" class="Component"
                    params="*reinterpret_cast&lt; MoniqueAudioProcessor* &gt;( &amp;processor ), MidiKeyboardComponent::horizontalKeyboard"/>
  <GENERICCOMPONENT name="" id="35003b6b21577713" memberName="glide2" virtualName=""
                    explicitFocusOrder="0" pos="100 880 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GlideConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="d8ef93ac038fadca" memberName="arp_step_16" virtualName=""
                    explicitFocusOrder="0" pos="1185r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,15)"/>
  <GENERICCOMPONENT name="" id="7761deb0276debbd" memberName="arp_step_15" virtualName=""
                    explicitFocusOrder="0" pos="1125r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,14)"/>
  <GENERICCOMPONENT name="" id="20a9ed6504a039e2" memberName="arp_step_14" virtualName=""
                    explicitFocusOrder="0" pos="1065r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,13)"/>
  <GENERICCOMPONENT name="" id="791739ade4aee5df" memberName="arp_step_13" virtualName=""
                    explicitFocusOrder="0" pos="945 880 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,12)"/>
  <GENERICCOMPONENT name="" id="1e3ef8bba1be4b28" memberName="arp_step_12" virtualName=""
                    explicitFocusOrder="0" pos="935r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,11)"/>
  <GENERICCOMPONENT name="" id="fe823ea88a7a2471" memberName="arp_step_11" virtualName=""
                    explicitFocusOrder="0" pos="875r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,10)"/>
  <GENERICCOMPONENT name="" id="ee7d6057133dde55" memberName="arp_step_10" virtualName=""
                    explicitFocusOrder="0" pos="815r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,9)"/>
  <GENERICCOMPONENT name="" id="b4852f8bf0385747" memberName="arp_step_9" virtualName=""
                    explicitFocusOrder="0" pos="755r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,8)"/>
  <GENERICCOMPONENT name="" id="fd84ed45f47ab8b9" memberName="arp_step_8" virtualName=""
                    explicitFocusOrder="0" pos="685r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,7)"/>
  <GENERICCOMPONENT name="" id="cf5a0e63bd7f558a" memberName="arp_step_7" virtualName=""
                    explicitFocusOrder="0" pos="625r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,6)"/>
  <GENERICCOMPONENT name="" id="31712e752afeb9b5" memberName="arp_step_6" virtualName=""
                    explicitFocusOrder="0" pos="565r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,5)"/>
  <GENERICCOMPONENT name="" id="a729cce2b51e5737" memberName="arp_step_5" virtualName=""
                    explicitFocusOrder="0" pos="505r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,4)"/>
  <GENERICCOMPONENT name="" id="4ea4b03b58657c40" memberName="arp_step_4" virtualName=""
                    explicitFocusOrder="0" pos="435r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,3)"/>
  <GENERICCOMPONENT name="" id="b45b0bde6cb27e9d" memberName="arp_step_3" virtualName=""
                    explicitFocusOrder="0" pos="375r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="be72c3cee3e34864" memberName="arp_step_2" virtualName=""
                    explicitFocusOrder="0" pos="315r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="6665063ca7bdff41" memberName="arp_step_1" virtualName=""
                    explicitFocusOrder="0" pos="255r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ArpStepSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="bb9c66366ce375c" memberName="shuffle" virtualName=""
                    explicitFocusOrder="0" pos="30 880 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new ShuffleConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="6e5608d47c1be7c4" memberName="flt_sustain_4" virtualName=""
                    explicitFocusOrder="0" pos="660r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FSustainSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="8386fe429fe8a2e6" memberName="flt_decay_4" virtualName=""
                    explicitFocusOrder="0" pos="600r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FDecaySlConfig(synth_data)&#10;"/>
  <GENERICCOMPONENT name="" id="bb503e115ddb6edb" memberName="flt_attack_4" virtualName=""
                    explicitFocusOrder="0" pos="540r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FAttackSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="ca4537ccb809ca96" memberName="flt_release_3" virtualName=""
                    explicitFocusOrder="0" pos="720r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="a60fcd747c533a26" memberName="flt_sustain_time_3"
                    virtualName="" explicitFocusOrder="0" pos="660r 610r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="78d4de9e0ffe3029" memberName="flt_sustain_3" virtualName=""
                    explicitFocusOrder="0" pos="600r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="e8b49d00205726e6" memberName="flt_decay_3" virtualName=""
                    explicitFocusOrder="0" pos="540r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="638e13e96c94deb1" memberName="flt_attack_3" virtualName=""
                    explicitFocusOrder="0" pos="480r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FAttackSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="4e206df1142d5f1d" memberName="flt_release_2" virtualName=""
                    explicitFocusOrder="0" pos="720r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="b2e468ddbdcb0be8" memberName="flt_sustain_time_2"
                    virtualName="" explicitFocusOrder="0" pos="660r 430r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="aa6aa381eebdd61" memberName="flt_sustain_2" virtualName=""
                    explicitFocusOrder="0" pos="600r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="140fc1e77383e0f9" memberName="flt_decay_2" virtualName=""
                    explicitFocusOrder="0" pos="540r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="8a6f21a1f4a86dd" memberName="flt_attack_2" virtualName=""
                    explicitFocusOrder="0" pos="480r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FAttackSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="b17b21322ed6df73" memberName="flt_release_1" virtualName=""
                    explicitFocusOrder="0" pos="720r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FReleaseSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="1460533da718423d" memberName="flt_sustain_time_1"
                    virtualName="" explicitFocusOrder="0" pos="660r 250r 60 130"
                    class="Monique_Ui_DualSlider" params="ui_refresher, &#10;new FSustainTimeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="16f83a5a025850d0" memberName="flt_sustain_1" virtualName=""
                    explicitFocusOrder="0" pos="600r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FSustainSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="dc67a284425c81d9" memberName="flt_decay_1" virtualName=""
                    explicitFocusOrder="0" pos="540r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FDecaySlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="1a64935d9407f5bb" memberName="flt_attack_1" virtualName=""
                    explicitFocusOrder="0" pos="420 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher,&#10;new FAttackSlConfig(synth_data,0)"/>
  <TEXTBUTTON name="VOICE 1" id="cf1d3ce65d7cdcdc" memberName="filter_type_3_1"
              virtualName="" explicitFocusOrder="0" pos="1010 187 60 30" tooltip="Set the filter type to BAND PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="43c53216b803243b" memberName="filter_type_3_2"
              virtualName="" explicitFocusOrder="0" pos="1070r 367 60 30" tooltip="Set the filter type to BAND PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="f44bd17c008d0db3" memberName="filter_type_3_3"
              virtualName="" explicitFocusOrder="0" pos="1070r 547 60 30" tooltip="Set the filter type to BAND PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="BAND"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c1a5cea82178d7f1" memberName="filter_type_5_1"
              virtualName="" explicitFocusOrder="0" pos="1010 220 60 30" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="375699baffcdc070" memberName="filter_type_5_2"
              virtualName="" explicitFocusOrder="0" pos="1070r 400 60 30" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="74deee6f861b7bf5" memberName="filter_type_5_3"
              virtualName="" explicitFocusOrder="0" pos="1070r 580 60 30" tooltip="Set the filter type to PASS (not filtered)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="PASS"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7a60e9dcf8b32a0a" memberName="button_sequence_2"
              virtualName="" explicitFocusOrder="0" pos="315r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="17704b0ee658c01b" memberName="button_sequence_3"
              virtualName="" explicitFocusOrder="0" pos="375r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="70bcd1e56b41c2c6" memberName="button_sequence_4"
              virtualName="" explicitFocusOrder="0" pos="435r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e835074126c3a82d" memberName="button_sequence_5"
              virtualName="" explicitFocusOrder="0" pos="505r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7ebcf311504b804b" memberName="button_sequence_6"
              virtualName="" explicitFocusOrder="0" pos="565r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7964742ab1d9d236" memberName="button_sequence_7"
              virtualName="" explicitFocusOrder="0" pos="625r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="290ac159e50889a3" memberName="button_sequence_8"
              virtualName="" explicitFocusOrder="0" pos="685r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="444c07bec0f97ff4" memberName="button_sequence_9"
              virtualName="" explicitFocusOrder="0" pos="755r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d8ad414b935f59d8" memberName="button_sequence_10"
              virtualName="" explicitFocusOrder="0" pos="815r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="af5f2477751cef2c" memberName="button_sequence_11"
              virtualName="" explicitFocusOrder="0" pos="875r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="d3687d6b28982234" memberName="button_sequence_12"
              virtualName="" explicitFocusOrder="0" pos="935r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="804115318ad213c1" memberName="button_sequence_13"
              virtualName="" explicitFocusOrder="0" pos="1005r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="13d758647984d5d5" memberName="button_sequence_14"
              virtualName="" explicitFocusOrder="0" pos="1065r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8cbd35271097248c" memberName="button_sequence_15"
              virtualName="" explicitFocusOrder="0" pos="1125r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="2370645873984939" memberName="button_sequence_16"
              virtualName="" explicitFocusOrder="0" pos="1185r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="7c9b1844748d88e" memberName="combo_programm" virtualName=""
            explicitFocusOrder="0" pos="335 10 255 30" tooltip="Select and load a program of the selected bank (one box left)."
            editable="1" layout="33" items="" textWhenNonSelected="FROM SCRATCH"
            textWhenNoItems="EMPTY BANK"/>
  <TEXTBUTTON name="" id="dd0cd965aaddf5ba" memberName="button_programm_left"
              virtualName="" explicitFocusOrder="0" pos="215 10 60 30" tooltip="Load the program before of the selected bank (right of this button)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6ccb0337451b3a26" memberName="button_programm_right"
              virtualName="" explicitFocusOrder="0" pos="650r 10 60 30" tooltip="Load the next program of the selected bank."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e5ff1639c5671984" memberName="button_open_oszi" virtualName=""
              explicitFocusOrder="0" pos="1380 10 60 30" tooltip="Open/Close the oscilloscope.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="OSCI"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="57c6c986fc98dac" memberName="button_open_midi_io_settings"
              virtualName="" explicitFocusOrder="0" pos="1290r 40r 60 30" tooltip="Open/Close the MIDI settings.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="MIDI"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_bank" virtualName=""
            explicitFocusOrder="0" pos="275 10 60 30" tooltip="Select the current program bank."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="" id="aa7c44443637097c" memberName="button_programm_load"
              virtualName="" explicitFocusOrder="0" pos="710r 10 60 30" tooltip="Load the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LOAD"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="e8e2f9e6488018da" memberName="osc_1" virtualName=""
                    explicitFocusOrder="0" pos="100 120 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="308060a72bcb3066" memberName="osc_2" virtualName=""
                    explicitFocusOrder="0" pos="100 300 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="17d8341f811bcb5a" memberName="osc_3" virtualName=""
                    explicitFocusOrder="0" pos="160r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OSCSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="ffb8076636239778" memberName="flt_cutoff_1" virtualName=""
                    explicitFocusOrder="0" pos="1080 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="9eb8d35cf54eee3" memberName="flt_cutoff_2" virtualName=""
                    explicitFocusOrder="0" pos="1140r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="d7143931caaf1976" memberName="flt_cutoff_3" virtualName=""
                    explicitFocusOrder="0" pos="1140r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FCutoffSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="16470f25818b13ce" memberName="flt_distortion_1" virtualName=""
                    explicitFocusOrder="0" pos="1290r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="31da059865f2567b" memberName="flt_resonance_1" virtualName=""
                    explicitFocusOrder="0" pos="1200r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="75550ba5bb7ce4e0" memberName="flt_resonance_2" virtualName=""
                    explicitFocusOrder="0" pos="1200r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="aa2b2c2864221426" memberName="flt_resonance_3" virtualName=""
                    explicitFocusOrder="0" pos="1200r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FResonanceSLConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="ba71384f051dd23" memberName="flt_volume_1" virtualName=""
                    explicitFocusOrder="0" pos="1380 120 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="32dd3f586d1d81eb" memberName="flt_volume_2" virtualName=""
                    explicitFocusOrder="0" pos="1380 300 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="18f72cc654c99917" memberName="flt_volume_3" virtualName=""
                    explicitFocusOrder="0" pos="1380 480 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FVolumeSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="68cebc996c492894" memberName="adsr_lfo_mix" virtualName=""
                    explicitFocusOrder="0" pos="818 120 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="944e7d4439e86773" memberName="lfo_opt_2" virtualName=""
                    explicitFocusOrder="0" pos="818 300 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="2d0d9d7f81f143" memberName="lfo_opt_3" virtualName=""
                    explicitFocusOrder="0" pos="818 480 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new EnvLfoSlConfig(synth_data,2)"/>
  <TEXTBUTTON name="" id="9669ee100bf4ee95" memberName="button_sequence_1"
              virtualName="" explicitFocusOrder="0" pos="255r 880 60 30" tooltip="Turns this step on or off.&#10;(Has no effect if the arpeggiator (ARP) is turned off)"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="3eaa1962698c14dc" memberName="flt_release_4" virtualName=""
                    explicitFocusOrder="0" pos="720r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FReleaseSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="94c6b03ecc4d4642" memberName="volume" virtualName=""
                    explicitFocusOrder="0" pos="1380 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new VolumeConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="9771b840efca92c2" memberName="flt_distortion_2" virtualName=""
                    explicitFocusOrder="0" pos="1290r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="766d923ef01630c7" memberName="flt_distortion_3" virtualName=""
                    explicitFocusOrder="0" pos="1290r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new GForceSlConfig(synth_data,2)"/>
  <TEXTBUTTON name="" id="28379674f941d830" memberName="button_arp_speed_XNORM"
              virtualName="" explicitFocusOrder="0" pos="1285 880 60 30" tooltip="Shortcut to set the speed multiplier back to 1x (in sync)."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="x1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="9f8319dda0065826" memberName="flt_attack_5" virtualName=""
                    explicitFocusOrder="0" pos="90r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FMFreqSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="53fd0bab31e1ce" memberName="flt_attack_6" virtualName=""
                    explicitFocusOrder="0" pos="160r 680 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FMAmountSlConfig(synth_data)"/>
  <GENERICCOMPONENT name="" id="7abd69d58b16456c" memberName="osc_wave_1" virtualName=""
                    explicitFocusOrder="0" pos="30 120 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="289652ee3553683c" memberName="osc_wave_2" virtualName=""
                    explicitFocusOrder="0" pos="30 300 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new WAVESlConfig(synth_data,1)"/>
  <TEXTBUTTON name="" id="87db63f1017ff04b" memberName="button_programm_delete"
              virtualName="" explicitFocusOrder="0" pos="1030r 10 60 30" tooltip="Delete the selected program."
              bgColOff="ff000000" textCol="ffff0000" textColOn="ffff7900" buttonText="DELETE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="bcfd327216c64e93" memberName="filter_type_6_1"
              virtualName="" explicitFocusOrder="0" pos="1010 120 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="78dd95fdf1cece7e" memberName="filter_type_6_2"
              virtualName="" explicitFocusOrder="0" pos="1070r 300 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="VOICE 1" id="c642f9acf4b813ef" memberName="filter_type_6_3"
              virtualName="" explicitFocusOrder="0" pos="1070r 480 60 30" tooltip="Set the filter type to LOW PASS."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="LP"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4d29473e06fd562f" memberName="button_ctrl_toggle"
              virtualName="" explicitFocusOrder="0" pos="100 10 60 30" tooltip="Turns the SHIFT mode on or off.&#10;&#10;The shift mode moves all back sliders to front and front sliders to back."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="SHIFT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="ca562cfd2b6999c4" memberName="speed" virtualName=""
                    explicitFocusOrder="0" pos="1275r 1010r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new BPMSlConfig(synth_data)"/>
  <TEXTBUTTON name="" id="8f0b48518cbff149" memberName="button_open_morph"
              virtualName="" explicitFocusOrder="0" pos="440r 645 60 30" tooltip="Open/Close the morph editor.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="EDIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f57674183a67085" memberName="effect_finalizer_switch"
              virtualName="" explicitFocusOrder="0" pos="930 819 120 30" tooltip="Switch to the FX section."
              bgColOff="ffff11ff" textCol="ffff3b00" textColOn="ffffff00" buttonText="FX"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8b8fa534e67fede0" memberName="button_values_toggle"
              virtualName="" explicitFocusOrder="0" pos="30 10 60 30" tooltip="Turns the CTRL mode on or off.&#10;&#10;In CTRL mode are all values visble.&#10;&#10;Hold down CTRL/CMD on your keyboard and drag a slider to control it in velocity mode.&#10;&#10;Hold down CTRL/CMD on your keyboard and press + or - to resize the user interface. Press F11 to toggle fullscreen mode."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="CTRL"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="6c9f41765f0f3e8a" memberName="octave_offset" virtualName=""
                    explicitFocusOrder="0" pos="1380 880 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new OctaveOffsetSlConfig(synth_data)"/>
  <LABEL name="" id="b59f286362d58d43" memberName="label_filter_inputs"
         virtualName="" explicitFocusOrder="0" pos="210 75 170 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER INPUTS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="706628ef52338a3" memberName="label_oscillators" virtualName=""
         explicitFocusOrder="0" pos="30 75 130 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OSCILLATORS (O)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="40822f39512f59ee" memberName="label_filter_envelope"
         virtualName="" explicitFocusOrder="0" pos="420 75 370 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER ENVELOPE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="4a296c8c1b36d5b5" memberName="label_lfo" virtualName=""
         explicitFocusOrder="0" pos="890 75 80 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LFO (L)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="8dfe8598a2227d6" memberName="label_filter_config"
         virtualName="" explicitFocusOrder="0" pos="980 75 250 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER CONFIGURATION"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="624597a6e0fd4f43" memberName="label_filter_fx" virtualName=""
         explicitFocusOrder="0" pos="1220 75 140 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FILTER FX" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="d111f7d6b78091bd" memberName="label_out" virtualName=""
         explicitFocusOrder="0" pos="1370 75 80 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="OUTPUT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="cc8c514c76739c41" memberName="label_amp_envelope"
         virtualName="" explicitFocusOrder="0" pos="480 645 310 30" textCol="ff050505"
         edTextCol="ff796660" edBkgCol="0" labelText="AMP ENVELOPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="365d9d273db6db3a" memberName="label_glide" virtualName=""
         explicitFocusOrder="0" pos="100 835 60 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLIDE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="9b0c123898785ff8" memberName="label_speed" virtualName=""
         explicitFocusOrder="0" pos="1225 835 120 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="de3bbc3ac23fc36a" memberName="label_tune" virtualName=""
         explicitFocusOrder="0" pos="1380 835 60 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="TUNE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="ecf3af4d030b7b19" memberName="label_fm" virtualName=""
         explicitFocusOrder="0" pos="30 645 130 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="FM (F)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c54e3e2b543626c5" memberName="volume_master_meter"
                    virtualName="Monique_Ui_SegmentedMeter" explicitFocusOrder="0"
                    pos="1380 680 60 27" class="Component" params="ui_refresher"/>
  <TEXTBUTTON name="" id="30ecdded1d4d2622" memberName="button_open_config2"
              virtualName="" explicitFocusOrder="0" pos="1350r 40r 60 30" tooltip="Open/Close the setup.&#10;&#10;Note: press ESC to close editors."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="SETUP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="657dea65d9f85585" memberName="label_mod_mix" virtualName=""
         explicitFocusOrder="0" pos="793 75 110 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MOD MIX (X)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="fa465a4afae26fc7" memberName="flt_pan_3" virtualName=""
                    explicitFocusOrder="0" pos="1350r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,2)"/>
  <GENERICCOMPONENT name="" id="6397e9617b7dcaf9" memberName="flt_pan_2" virtualName=""
                    explicitFocusOrder="0" pos="1350r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="e272cc245f5b87a1" memberName="flt_pan_1" virtualName=""
                    explicitFocusOrder="0" pos="1350r 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FPanSlConfig(synth_data,0)"/>
  <TEXTBUTTON name="" id="db06e124f1fcbf4d" memberName="button_reset_arp_tune"
              virtualName="" explicitFocusOrder="0" pos="1380 880 60 30" tooltip="Resets the arpeggiator to the defined program note.&#10;(Triggers a note which is defineable by the note dial (back dial))"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="RESET"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bcf2554ab289ccda" memberName="button_show_active_input_r_2_3"
              virtualName="" explicitFocusOrder="0" pos="361 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="e67decdf6ffaf352" memberName="button_show_active_input_l_2_3"
              virtualName="" explicitFocusOrder="0" pos="331 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="327a90a8c4ed5aa9" memberName="button_show_active_input_r_2_2"
              virtualName="" explicitFocusOrder="0" pos="301 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="6d0f49fc4b55f79e" memberName="button_show_active_input_l_2_2"
              virtualName="" explicitFocusOrder="0" pos="271 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="8be6e66ece9ca751" memberName="button_show_active_input_r_2_1"
              virtualName="" explicitFocusOrder="0" pos="241 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="69acf26b6e8a591e" memberName="button_show_active_input_l_2_1"
              virtualName="" explicitFocusOrder="0" pos="211 289 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c04afcf396fb5173" memberName="button_show_active_input_r_3_3"
              virtualName="" explicitFocusOrder="0" pos="361 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="73aa09a176276d09" memberName="button_show_active_input_l_3_3"
              virtualName="" explicitFocusOrder="0" pos="331 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bf707b726ad7a93b" memberName="button_show_active_input_r_3_2"
              virtualName="" explicitFocusOrder="0" pos="301 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="c895ade2dcdf51aa" memberName="button_show_active_input_l_3_2"
              virtualName="" explicitFocusOrder="0" pos="271 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="a84a66eefd216154" memberName="button_show_active_input_r_3_1"
              virtualName="" explicitFocusOrder="0" pos="241 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="aab3f66b90ddf6" memberName="button_show_active_input_l_3_1"
              virtualName="" explicitFocusOrder="0" pos="210 469 10 10" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="5e29190ea95f8441" memberName="button_programm_rename"
              virtualName="" explicitFocusOrder="0" pos="970r 10 60 30" tooltip="Rename the selected program."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="RENAME"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="f27a090801db5056" memberName="flt_shape_1" virtualName=""
                    explicitFocusOrder="0" pos="730 250r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,0)"/>
  <GENERICCOMPONENT name="" id="8420242133122df9" memberName="flt_shape_2" virtualName=""
                    explicitFocusOrder="0" pos="790r 430r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,1)"/>
  <GENERICCOMPONENT name="" id="fda8401083c9b835" memberName="flt_shape_3" virtualName=""
                    explicitFocusOrder="0" pos="790r 610r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, &#10;new FShapeSlConfig(synth_data,2)"/>
  <TEXTBUTTON name="" id="cdfed104ed06cd19" memberName="button_programm_scratch"
              virtualName="" explicitFocusOrder="0" pos="780r 10 60 30" tooltip="Load the factory defaults to start from scratch.&#10;&#10;Push this button again to stop clearing the record buffer."
              bgColOff="ff000000" textCol="ffbcff00" textColOn="ffd0ff00" buttonText="INIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="7cfc16f7f0064a2d" memberName="flt_shape_4" virtualName=""
                    explicitFocusOrder="0" pos="790r 810r 60 130" class="Monique_Ui_DualSlider"
                    params="ui_refresher, new FShapeSlConfig(synth_data)"/>
  <LABEL name="TOP" id="3cf5d2a2e81a8504" memberName="label_monoplugs"
         virtualName="" explicitFocusOrder="0" pos="1220r 40 180 30" textCol="ffffff11"
         edTextCol="ffff3b00" edBkgCol="0" labelText="M  O  N  O  P  L  U  G  S"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="879633575acf68ee" memberName="label_reverb" virtualName=""
         explicitFocusOrder="0" pos="1150 680 120 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="REVERB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: empty_png, 159, "../Images/empty.png"
static const unsigned char resource_Monique_Ui_Mainwindow_empty_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,1,0,0,0,1,8,4,0,0,0,181,28,12,2,0,0,0,2,98,75,71,68,0,0,170,141,35,50,0,0,
        0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,223,9,18,16,13,44,67,236,72,225,0,0,0,25,116,69,88,116,67,111,109,109,101,110,116,0,67,114,101,97,116,101,100,32,119,105,116,
        104,32,71,73,77,80,87,129,14,23,0,0,0,11,73,68,65,84,8,215,99,248,255,31,0,3,0,1,255,100,93,114,245,0,0,0,0,73,69,78,68,174,66,96,130,0,0
                                                                        };

const char* Monique_Ui_Mainwindow::empty_png = (const char*) resource_Monique_Ui_Mainwindow_empty_png;
const int Monique_Ui_Mainwindow::empty_pngSize = 159;

// JUCER_RESOURCE: _01hintergrundalles_svg, 23727, "../Files/STYLE/Monique-Hintergrund-1/01Hintergrund-Alles.svg"
static const unsigned char resource_Monique_Ui_Mainwindow__01hintergrundalles_svg[] = { 60,63,120,109,108,32,118,101,114,115,105,111,110,61,34,49,46,48,34,32,101,110,99,111,100,105,110,103,61,34,85,84,
        70,45,56,34,32,115,116,97,110,100,97,108,111,110,101,61,34,110,111,34,63,62,10,60,115,118,103,10,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,
        100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,120,109,108,110,115,58,99,99,61,34,104,116,116,112,58,47,47,99,114,101,97,116,105,118,101,99,111,109,109,111,110,115,46,111,114,
        103,47,110,115,35,34,10,32,32,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,
        97,120,45,110,115,35,34,10,32,32,32,120,109,108,110,115,58,115,118,103,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,
        115,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,50,48,48,48,47,115,118,103,34,10,32,32,32,120,109,108,110,115,58,120,108,105,110,107,61,34,104,116,116,112,58,47,47,119,119,119,
        46,119,51,46,111,114,103,47,49,57,57,57,47,120,108,105,110,107,34,10,32,32,32,120,109,108,110,115,58,115,111,100,105,112,111,100,105,61,34,104,116,116,112,58,47,47,115,111,100,105,112,111,100,105,46,115,
        111,117,114,99,101,102,111,114,103,101,46,110,101,116,47,68,84,68,47,115,111,100,105,112,111,100,105,45,48,46,100,116,100,34,10,32,32,32,120,109,108,110,115,58,105,110,107,115,99,97,112,101,61,34,104,
        116,116,112,58,47,47,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,47,110,97,109,101,115,112,97,99,101,115,47,105,110,107,115,99,97,112,101,34,10,32,32,32,119,105,100,116,104,61,34,49,48,
        48,37,34,10,32,32,32,104,101,105,103,104,116,61,34,49,48,48,37,34,10,32,32,32,118,105,101,119,66,111,120,61,34,48,32,48,32,49,50,55,52,32,56,52,48,34,10,32,32,32,118,101,114,115,105,111,110,61,34,49,46,
        49,34,10,32,32,32,120,109,108,58,115,112,97,99,101,61,34,112,114,101,115,101,114,118,101,34,10,32,32,32,115,116,121,108,101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,99,
        108,105,112,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,
        58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,49,46,52,49,52,50,49,59,34,10,32,32,32,105,100,61,34,115,118,103,50,34,10,32,32,32,105,110,107,115,99,97,
        112,101,58,118,101,114,115,105,111,110,61,34,48,46,52,56,46,52,32,114,57,57,51,57,34,10,32,32,32,115,111,100,105,112,111,100,105,58,100,111,99,110,97,109,101,61,34,48,49,72,105,110,116,101,114,103,114,
        117,110,100,45,65,108,108,101,115,46,115,118,103,34,62,60,109,101,116,97,100,97,116,97,10,32,32,32,32,32,105,100,61,34,109,101,116,97,100,97,116,97,50,56,53,34,62,60,114,100,102,58,82,68,70,62,60,99,99,
        58,87,111,114,107,10,32,32,32,32,32,32,32,32,32,114,100,102,58,97,98,111,117,116,61,34,34,62,60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,115,118,103,43,120,109,108,60,47,100,99,58,102,
        111,114,109,97,116,62,60,100,99,58,116,121,112,101,10,32,32,32,32,32,32,32,32,32,32,32,114,100,102,58,114,101,115,111,117,114,99,101,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
        99,47,100,99,109,105,116,121,112,101,47,83,116,105,108,108,73,109,97,103,101,34,32,47,62,60,47,99,99,58,87,111,114,107,62,60,47,114,100,102,58,82,68,70,62,60,47,109,101,116,97,100,97,116,97,62,60,115,
        111,100,105,112,111,100,105,58,110,97,109,101,100,118,105,101,119,10,32,32,32,32,32,112,97,103,101,99,111,108,111,114,61,34,35,102,102,102,102,102,102,34,10,32,32,32,32,32,98,111,114,100,101,114,99,111,
        108,111,114,61,34,35,54,54,54,54,54,54,34,10,32,32,32,32,32,98,111,114,100,101,114,111,112,97,99,105,116,121,61,34,49,34,10,32,32,32,32,32,111,98,106,101,99,116,116,111,108,101,114,97,110,99,101,61,34,
        49,48,34,10,32,32,32,32,32,103,114,105,100,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,103,117,105,100,101,116,111,108,101,114,97,110,99,101,61,34,49,48,34,10,32,32,32,32,32,105,
        110,107,115,99,97,112,101,58,112,97,103,101,111,112,97,99,105,116,121,61,34,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,112,97,103,101,115,104,97,100,111,119,61,34,50,34,10,32,32,32,32,32,
        105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,119,105,100,116,104,61,34,49,57,50,48,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,104,101,105,103,104,116,
        61,34,49,48,54,48,34,10,32,32,32,32,32,105,100,61,34,110,97,109,101,100,118,105,101,119,50,56,51,34,10,32,32,32,32,32,115,104,111,119,103,114,105,100,61,34,102,97,108,115,101,34,10,32,32,32,32,32,105,
        110,107,115,99,97,112,101,58,122,111,111,109,61,34,49,46,49,50,51,56,48,57,53,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,120,61,34,54,50,56,46,49,48,55,56,49,34,10,32,32,32,32,32,105,110,
        107,115,99,97,112,101,58,99,121,61,34,53,51,50,46,53,53,50,56,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,120,61,34,45,51,34,10,32,32,32,32,32,105,110,107,115,99,97,
        112,101,58,119,105,110,100,111,119,45,121,61,34,45,51,34,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,119,105,110,100,111,119,45,109,97,120,105,109,105,122,101,100,61,34,49,34,10,32,32,32,32,32,
        105,110,107,115,99,97,112,101,58,99,117,114,114,101,110,116,45,108,97,121,101,114,61,34,115,118,103,50,34,32,47,62,60,103,10,32,32,32,32,32,105,100,61,34,103,50,50,51,34,10,32,32,32,32,32,116,114,97,110,
        115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,55,54,52,57,56,57,50,50,44,48,44,48,44,48,46,57,56,48,56,49,51,44,50,51,56,46,53,52,57,55,49,44,49,53,46,53,49,54,50,51,41,34,62,60,112,97,116,
        104,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,50,
        50,53,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,117,114,108,40,35,95,76,105,110,101,97,114,54,41,59,115,116,114,111,107,101,
        45,119,105,100,116,104,58,49,51,46,50,55,57,57,57,57,55,51,112,120,34,10,32,32,32,32,32,32,32,100,61,34,109,32,49,50,53,53,46,55,44,49,48,52,46,50,57,49,32,48,44,51,57,51,46,55,56,52,32,45,49,48,51,49,
        46,48,50,56,44,48,32,48,44,51,54,46,48,55,49,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,50,46,50,48,52,51,56,55,44,48,44,48,44,
        48,46,56,54,56,50,52,56,44,51,53,54,46,49,48,48,57,51,44,51,51,56,46,50,50,48,57,41,34,10,32,32,32,32,32,105,100,61,34,103,49,48,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,
        55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,51,46,48,50,52,44,45,49,52,46,55,56,56,32,45,54,46,55,53,44,45,49,52,46,55,56,56,32,108,32,45,49,51,48,46,55,48,53,44,48,32,67,
        32,54,46,48,50,53,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,51,46,48,50,53,44,49,52,46,55,56,56,32,
        54,46,55,53,49,44,49,52,46,55,56,56,32,108,32,49,51,48,46,55,48,53,44,48,32,99,32,51,46,55,50,54,44,48,32,54,46,55,53,44,45,54,46,54,50,54,32,54,46,55,53,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,
        49,49,46,52,53,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,50,34,10,32,32,32,32,32,
        32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,
        109,61,34,109,97,116,114,105,120,40,50,46,57,51,48,50,53,50,51,44,48,44,48,44,51,46,57,49,48,57,55,44,50,55,55,46,55,55,57,57,54,44,49,55,52,46,49,52,41,34,10,32,32,32,32,32,105,100,61,34,103,49,52,34,
        62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,
        56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,117,114,108,40,35,108,105,
        110,101,97,114,71,114,97,100,105,101,110,116,51,51,52,56,41,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,54,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,
        109,61,34,109,97,116,114,105,120,40,48,46,53,54,53,48,57,54,44,48,44,48,44,48,46,56,54,56,53,56,56,44,55,53,51,46,51,56,44,51,51,56,46,50,49,49,50,41,34,10,32,32,32,32,32,105,100,61,34,103,49,56,34,62,
        60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,56,46,51,53,32,99,32,48,44,45,53,46,52,50,57,32,45,54,46,55,55,53,44,45,57,46,56,51,55,32,45,49,53,46,49,50,49,44,
        45,57,46,56,51,55,32,108,32,45,49,49,51,46,57,54,52,44,48,32,67,32,57,46,55,55,54,44,50,56,46,53,49,51,32,51,44,51,50,46,57,50,49,32,51,44,51,56,46,51,53,32,108,32,48,44,49,50,49,46,51,53,56,32,99,32,
        48,44,53,46,52,51,32,54,46,55,55,54,44,57,46,56,51,56,32,49,53,46,49,50,49,44,57,46,56,51,56,32,108,32,49,49,51,46,57,54,52,44,48,32,99,32,56,46,51,52,54,44,48,32,49,53,46,49,50,49,44,45,52,46,52,48,56,
        32,49,53,46,49,50,49,44,45,57,46,56,51,56,32,108,32,48,44,45,49,50,49,46,51,53,56,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,
        32,32,32,105,100,61,34,112,97,116,104,50,48,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,
        103,62,60,112,97,116,104,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,105,100,61,34,112,97,116,
        104,50,52,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,100,61,34,109,32,49,50,52,48,46,48,53,57,52,44,51,55,53,46,56,52,53,48,51,32,99,32,
        48,44,45,55,46,48,56,57,52,50,32,45,53,46,50,52,53,57,44,45,49,50,46,56,52,52,54,56,32,45,49,49,46,55,48,56,54,44,45,49,50,46,56,52,52,54,56,32,108,32,45,51,54,51,46,57,51,51,53,50,44,48,32,99,32,45,54,
        46,52,54,50,55,52,44,48,32,45,49,49,46,55,48,56,54,55,44,53,46,55,53,53,50,54,32,45,49,49,46,55,48,56,54,55,44,49,50,46,56,52,52,54,56,32,108,32,48,44,57,54,46,56,49,48,50,49,32,99,32,48,44,55,46,48,56,
        57,52,50,32,53,46,50,52,53,57,51,44,49,50,46,56,52,52,54,56,32,49,49,46,55,48,56,54,55,44,49,50,46,56,52,52,54,56,32,108,32,51,54,51,46,57,51,51,53,50,44,48,32,99,32,54,46,52,54,50,55,44,48,32,49,49,46,
        55,48,56,54,44,45,53,46,55,53,53,50,54,32,49,49,46,55,48,56,54,44,45,49,50,46,56,52,52,54,56,32,108,32,48,44,45,57,54,46,56,49,48,50,49,32,122,34,32,47,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,
        111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,51,54,56,46,53,49,41,34,10,32,32,32,32,32,105,100,61,34,103,51,52,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,
        54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,
        61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,51,54,34,32,47,62,
        60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,46,49,56,54,57,57,44,48,44,48,44,49,46,48,48,53,53,49,44,49,55,55,46,56,54,57,44,51,49,52,46,
        57,52,57,49,55,41,34,10,32,32,32,32,32,105,100,61,34,103,51,56,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,
        50,32,45,53,46,54,49,51,44,45,49,52,46,55,56,56,32,45,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,45,49,49,57,46,49,53,49,44,48,32,67,32,56,46,54,49,51,44,50,56,46,53,49,51,32,51,44,51,53,46,49,
        51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,53,46,54,49,51,44,49,52,46,55,56,56,32,49,50,46,53,50,56,44,49,52,46,55,56,56,32,108,32,49,49,57,
        46,49,53,49,44,48,32,99,32,54,46,57,49,52,44,48,32,49,50,46,53,50,55,44,45,54,46,54,50,54,32,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,10,32,32,32,32,
        32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,52,48,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,
        111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,
        40,49,57,48,44,51,54,54,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,52,50,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,
        48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,
        32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,52,52,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,
        97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,54,55,54,44,51,54,54,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,52,54,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,
        61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,
        101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,
        52,56,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,46,48,49,56,53,54,44,48,44,48,44,49,46,51,48,51,54,50,44,45,50,49,46,54,52,
        52,57,44,57,57,46,48,55,51,41,34,10,32,32,32,32,32,105,100,61,34,103,53,48,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,51,52,56,46,50,55,49,44,49,57,50,46,49,57,51,32,56,49,55,46,
        56,52,57,44,48,32,48,44,45,49,56,46,49,57,53,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,35,97,48,98,49,98,98,59,115,116,114,111,
        107,101,45,119,105,100,116,104,58,49,48,46,50,54,48,48,48,48,50,51,112,120,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,53,50,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,
        111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,50,46,
        50,49,48,53,53,55,54,44,48,44,48,44,48,46,56,54,56,50,52,56,44,51,53,54,46,48,56,50,52,50,44,49,56,54,46,50,50,48,57,41,34,10,32,32,32,32,32,105,100,61,34,103,53,53,34,62,60,112,97,116,104,10,32,32,32,
        32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,51,46,48,50,52,44,45,49,52,46,55,56,56,32,45,54,46,55,53,44,45,49,52,46,55,56,56,32,108,32,
        45,49,51,48,46,55,48,53,44,48,32,67,32,54,46,48,50,53,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,51,
        46,48,50,53,44,49,52,46,55,56,56,32,54,46,55,53,49,44,49,52,46,55,56,56,32,108,32,49,51,48,46,55,48,53,44,48,32,99,32,51,46,55,50,54,44,48,32,54,46,55,53,44,45,54,46,54,50,54,32,54,46,55,53,44,45,49,52,
        46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,
        116,104,53,55,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,
        32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,50,46,56,55,51,57,53,55,55,44,48,44,48,44,51,46,57,48,52,50,55,44,50,56,53,46,54,53,53,54,53,44,50,50,46,54,54,50,41,34,10,32,32,32,
        32,32,105,100,61,34,103,53,57,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,
        119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,
        108,58,117,114,108,40,35,108,105,110,101,97,114,71,114,97,100,105,101,110,116,51,51,53,48,41,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,54,49,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,
        32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,53,54,53,48,57,54,44,48,44,48,44,48,46,56,54,56,53,56,56,44,55,53,51,46,51,56,44,49,56,54,46,50,49,49,50,41,34,10,32,32,32,32,
        32,105,100,61,34,103,54,51,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,56,46,51,53,32,99,32,48,44,45,53,46,52,50,57,32,45,54,46,55,55,53,44,45,57,46,56,
        51,55,32,45,49,53,46,49,50,49,44,45,57,46,56,51,55,32,108,32,45,49,49,51,46,57,54,52,44,48,32,67,32,57,46,55,55,54,44,50,56,46,53,49,51,32,51,44,51,50,46,57,50,49,32,51,44,51,56,46,51,53,32,108,32,48,
        44,49,50,49,46,51,53,56,32,99,32,48,44,53,46,52,51,32,54,46,55,55,54,44,57,46,56,51,56,32,49,53,46,49,50,49,44,57,46,56,51,56,32,108,32,49,49,51,46,57,54,52,44,48,32,99,32,56,46,51,52,54,44,48,32,49,53,
        46,49,50,49,44,45,52,46,52,48,56,32,49,53,46,49,50,49,44,45,57,46,56,51,56,32,108,32,48,44,45,49,50,49,46,51,53,56,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,
        48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,54,53,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,
        114,101,61,34,48,34,32,47,62,60,47,103,62,60,112,97,116,104,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,
        32,32,32,32,105,100,61,34,112,97,116,104,54,57,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,100,61,34,109,32,49,50,52,48,46,48,53,57,52,44,
        50,50,51,46,56,52,53,48,51,32,99,32,48,44,45,55,46,48,56,57,52,50,32,45,53,46,50,55,50,56,44,45,49,50,46,56,52,52,54,56,32,45,49,49,46,55,54,56,55,44,45,49,50,46,56,52,52,54,56,32,108,32,45,51,54,53,46,
        55,57,57,54,57,44,48,32,99,32,45,54,46,52,57,53,56,56,44,48,32,45,49,49,46,55,54,56,55,49,44,53,46,55,53,53,50,54,32,45,49,49,46,55,54,56,55,49,44,49,50,46,56,52,52,54,56,32,108,32,48,44,57,54,46,56,49,
        48,50,49,32,99,32,48,44,55,46,48,56,57,52,50,32,53,46,50,55,50,56,51,44,49,50,46,56,52,52,54,56,32,49,49,46,55,54,56,55,49,44,49,50,46,56,52,52,54,56,32,108,32,51,54,53,46,55,57,57,54,57,44,48,32,99,32,
        54,46,52,57,53,57,44,48,32,49,49,46,55,54,56,55,44,45,53,46,55,53,53,50,54,32,49,49,46,55,54,56,55,44,45,49,50,46,56,52,52,54,56,32,108,32,48,44,45,57,54,46,56,49,48,50,49,32,122,34,32,47,62,60,103,10,
        32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,50,49,54,46,53,49,41,34,10,32,32,32,32,32,105,100,61,34,103,55,57,34,62,60,114,101,99,116,10,32,32,32,
        32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,
        32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,
        34,114,101,99,116,56,49,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,46,49,56,54,57,57,44,48,44,48,44,49,46,48,48,53,53,49,44,
        49,55,55,46,56,54,57,44,49,54,50,46,57,52,57,49,55,41,34,10,32,32,32,32,32,105,100,61,34,103,56,51,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,
        48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,53,46,54,49,51,44,45,49,52,46,55,56,56,32,45,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,45,49,49,57,46,49,53,49,44,48,32,67,32,56,46,54,49,51,44,50,
        56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,53,46,54,49,51,44,49,52,46,55,56,56,32,49,50,46,53,50,56,44,49,
        52,46,55,56,56,32,108,32,49,49,57,46,49,53,49,44,48,32,99,32,54,46,57,49,52,44,48,32,49,50,46,53,50,55,44,45,54,46,54,50,54,32,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,
        52,53,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,56,53,34,10,32,32,32,32,32,32,32,105,
        110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,
        116,114,97,110,115,108,97,116,101,40,49,57,48,44,50,49,52,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,56,55,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,
        32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,
        46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,56,57,34,32,47,62,60,47,103,62,
        60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,54,55,54,44,50,49,52,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,57,49,34,62,60,114,101,99,
        116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,
        57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,
        32,32,105,100,61,34,114,101,99,116,57,51,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,46,48,49,56,53,54,44,48,44,48,44,49,46,
        51,48,51,54,50,44,45,50,49,46,54,52,52,57,44,45,53,50,46,57,50,55,41,34,10,32,32,32,32,32,105,100,61,34,103,57,53,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,51,52,56,46,50,55,49,
        44,49,57,50,46,49,57,51,32,56,49,55,46,56,52,57,44,48,32,48,44,45,49,56,46,49,57,53,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,110,111,110,101,59,115,116,114,111,107,101,58,
        35,97,48,98,49,98,98,59,115,116,114,111,107,101,45,119,105,100,116,104,58,49,48,46,50,54,48,48,48,48,50,51,112,120,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,57,55,34,10,32,32,32,32,32,32,
        32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,
        61,34,109,97,116,114,105,120,40,50,46,50,49,48,53,53,55,54,44,48,44,48,44,48,46,56,54,56,50,52,56,44,51,53,54,46,48,56,50,52,50,44,51,49,46,50,50,48,57,41,34,10,32,32,32,32,32,105,100,61,34,103,49,48,
        48,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,51,46,48,50,52,44,45,49,52,46,55,56,56,32,45,54,46,
        55,53,44,45,49,52,46,55,56,56,32,108,32,45,49,51,48,46,55,48,53,44,48,32,67,32,54,46,48,50,53,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,
        53,55,32,99,32,48,44,56,46,49,54,50,32,51,46,48,50,53,44,49,52,46,55,56,56,32,54,46,55,53,49,44,49,52,46,55,56,56,32,108,32,49,51,48,46,55,48,53,44,48,32,99,32,51,46,55,50,54,44,48,32,54,46,55,53,44,45,
        54,46,54,50,54,32,54,46,55,53,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,
        32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,48,50,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,
        32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,50,46,56,57,54,57,52,56,53,44,48,44,48,44,51,46,57,48,48,51,55,44,50,56,50,46,54,48,57,
        52,52,44,45,49,51,50,46,48,51,52,41,34,10,32,32,32,32,32,105,100,61,34,103,49,48,52,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,
        53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,
        32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,117,114,108,40,35,108,105,110,101,97,114,71,114,97,100,105,101,110,116,51,51,53,50,41,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,
        116,49,48,54,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,53,54,53,48,57,54,44,48,44,48,44,48,46,56,54,56,53,56,56,44,55,53,
        51,46,51,56,44,51,49,46,50,49,49,50,41,34,10,32,32,32,32,32,105,100,61,34,103,49,48,56,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,56,46,51,53,32,99,32,
        48,44,45,53,46,52,50,57,32,45,54,46,55,55,53,44,45,57,46,56,51,55,32,45,49,53,46,49,50,49,44,45,57,46,56,51,55,32,108,32,45,49,49,51,46,57,54,52,44,48,32,67,32,57,46,55,55,54,44,50,56,46,53,49,51,32,51,
        44,51,50,46,57,50,49,32,51,44,51,56,46,51,53,32,108,32,48,44,49,50,49,46,51,53,56,32,99,32,48,44,53,46,52,51,32,54,46,55,55,54,44,57,46,56,51,56,32,49,53,46,49,50,49,44,57,46,56,51,56,32,108,32,49,49,
        51,46,57,54,52,44,48,32,99,32,56,46,51,52,54,44,48,32,49,53,46,49,50,49,44,45,52,46,52,48,56,32,49,53,46,49,50,49,44,45,57,46,56,51,56,32,108,32,48,44,45,49,50,49,46,51,53,56,32,122,34,10,32,32,32,32,
        32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,49,48,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,
        99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,112,97,116,104,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,
        111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,105,100,61,34,112,97,116,104,49,49,52,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,
        34,10,32,32,32,32,32,100,61,34,109,32,49,50,52,48,46,48,53,57,52,44,54,56,46,56,52,53,48,50,57,32,99,32,48,44,45,55,46,48,56,57,52,49,53,32,45,53,46,50,52,49,54,44,45,49,50,46,56,52,52,54,55,57,32,45,
        49,49,46,54,57,57,49,44,45,49,50,46,56,52,52,54,55,57,32,108,32,45,51,54,51,46,54,51,55,57,50,44,48,32,99,32,45,54,46,52,53,55,53,49,44,48,32,45,49,49,46,54,57,57,49,56,44,53,46,55,53,53,50,54,52,32,45,
        49,49,46,54,57,57,49,56,44,49,50,46,56,52,52,54,55,57,32,108,32,48,44,57,54,46,56,49,48,50,49,49,32,99,32,48,44,55,46,48,56,57,52,50,32,53,46,50,52,49,54,55,44,49,50,46,56,52,52,54,56,32,49,49,46,54,57,
        57,49,56,44,49,50,46,56,52,52,54,56,32,108,32,51,54,51,46,54,51,55,57,50,44,48,32,99,32,54,46,52,53,55,53,44,48,32,49,49,46,54,57,57,49,44,45,53,46,55,53,53,50,54,32,49,49,46,54,57,57,49,44,45,49,50,46,
        56,52,52,54,56,32,108,32,48,44,45,57,54,46,56,49,48,50,49,49,32,122,34,32,47,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,53,48,53,56,53,44,48,
        44,48,44,48,46,57,53,48,53,56,53,44,51,50,46,57,53,51,56,44,48,46,54,54,57,41,34,10,32,32,32,32,32,105,100,61,34,103,49,49,54,34,32,47,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,
        34,116,114,97,110,115,108,97,116,101,40,48,44,54,49,46,53,49,41,34,10,32,32,32,32,32,105,100,61,34,103,49,50,52,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,
        32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,
        55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,50,54,34,32,47,62,60,47,103,62,
        60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,46,49,56,54,57,57,44,48,44,48,44,49,46,48,48,53,53,49,44,49,55,55,46,56,54,57,44,55,46,57,52,57,49,55,41,
        34,10,32,32,32,32,32,105,100,61,34,103,49,50,56,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,53,46,
        54,49,51,44,45,49,52,46,55,56,56,32,45,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,45,49,49,57,46,49,53,49,44,48,32,67,32,56,46,54,49,51,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,
        52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,53,46,54,49,51,44,49,52,46,55,56,56,32,49,50,46,53,50,56,44,49,52,46,55,56,56,32,108,32,49,49,57,46,49,53,49,44,
        48,32,99,32,54,46,57,49,52,44,48,32,49,50,46,53,50,55,44,45,54,46,54,50,54,32,49,50,46,53,50,55,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,10,32,32,32,32,32,32,32,115,116,
        121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,51,48,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,
        99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,49,57,48,44,
        53,57,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,49,51,50,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,
        57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,
        115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,51,52,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,
        102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,54,55,54,44,53,57,46,51,54,41,34,10,32,32,32,32,32,105,100,61,34,103,49,51,54,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,120,61,34,49,51,
        55,46,49,54,52,34,10,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,104,101,105,103,
        104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,51,56,34,
        32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,105,110,107,115,99,97,112,101,58,108,97,98,101,108,61,34,76,97,121,101,114,34,10,32,32,32,32,32,105,100,61,34,108,97,121,101,114,49,34,10,32,32,32,32,32,
        105,110,107,115,99,97,112,101,58,103,114,111,117,112,109,111,100,101,61,34,108,97,121,101,114,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,49,
        49,41,34,32,47,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,50,49,41,34,10,32,32,32,32,32,105,100,61,34,79,115,99,105,108,97,116,111,
        114,115,34,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,52,49,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,50,56,54,52,54,44,48,44,48,
        44,52,46,54,50,51,48,55,44,49,49,46,54,53,52,49,44,45,49,51,53,46,53,48,53,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,
        99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,51,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,55,52,
        55,55,55,97,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,49,46,51,53,51,32,99,32,48,44,45,49,46,53,54,56,32,45,54,46,51,51,54,44,45,50,46,56,52,32,45,49,52,46,49,52,44,
        45,50,46,56,52,32,108,32,45,49,49,53,46,57,50,53,44,48,32,67,32,57,46,51,51,54,44,50,56,46,53,49,51,32,51,44,50,57,46,55,56,53,32,51,44,51,49,46,51,53,51,32,108,32,48,44,49,51,53,46,51,53,51,32,99,32,
        48,44,49,46,53,54,55,32,54,46,51,51,54,44,50,46,56,52,32,49,52,46,49,52,49,44,50,46,56,52,32,108,32,49,49,53,46,57,50,53,44,48,32,99,32,55,46,56,48,52,44,48,32,49,52,46,49,52,44,45,49,46,50,55,51,32,49,
        52,46,49,52,44,45,50,46,56,52,32,108,32,48,44,45,49,51,53,46,51,53,51,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,52,53,34,10,32,32,32,32,32,32,32,116,114,97,110,
        115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,56,50,53,54,48,51,44,48,44,48,44,51,46,51,56,50,50,51,44,50,54,46,56,50,50,55,44,45,49,48,48,46,49,50,53,41,34,62,60,112,97,116,104,10,32,32,32,
        32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,52,55,
        34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,49,46,57,54,52,32,99,32,
        48,44,45,49,46,57,48,53,32,45,54,46,51,51,54,44,45,51,46,52,53,49,32,45,49,52,46,49,52,44,45,51,46,52,53,49,32,108,32,45,49,49,53,46,57,50,53,44,48,32,67,32,57,46,51,51,54,44,50,56,46,53,49,51,32,51,44,
        51,48,46,48,53,57,32,51,44,51,49,46,57,54,52,32,108,32,48,44,49,51,52,46,49,51,32,99,32,48,44,49,46,57,48,53,32,54,46,51,51,54,44,51,46,52,53,50,32,49,52,46,49,52,49,44,51,46,52,53,50,32,108,32,49,49,
        53,46,57,50,53,44,48,32,99,32,55,46,56,48,52,44,48,32,49,52,46,49,52,44,45,49,46,53,52,55,32,49,52,46,49,52,44,45,51,46,52,53,50,32,108,32,48,44,45,49,51,52,46,49,51,32,122,34,32,47,62,60,47,103,62,60,
        47,103,62,60,103,10,32,32,32,32,32,105,100,61,34,103,49,52,57,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,56,46,53,50,50,56,55,44,48,44,48,44,49,46,48,48,53,
        53,49,44,45,49,49,46,49,54,56,54,44,54,54,49,46,57,52,57,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,
        117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,53,49,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,49,97,97,98,50,34,10,32,32,32,32,32,
        32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,48,46,55,56,49,44,45,49,52,46,55,56,56,32,45,49,46,55,52,52,44,45,49,52,46,55,56,56,32,108,32,45,
        49,52,48,46,55,49,55,44,48,32,67,32,51,46,55,56,50,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,48,46,
        55,56,50,44,49,52,46,55,56,56,32,49,46,55,52,53,44,49,52,46,55,56,56,32,108,32,49,52,48,46,55,49,55,44,48,32,99,32,48,46,57,54,51,44,48,32,49,46,55,52,52,44,45,54,46,54,50,54,32,49,46,55,52,52,44,45,49,
        52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,105,100,61,34,103,49,53,51,34,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,
        109,97,116,114,105,120,40,48,44,48,46,53,57,55,54,56,56,44,45,48,46,54,49,48,50,50,51,44,48,44,52,57,50,46,55,51,52,44,54,51,51,46,50,54,53,41,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,105,100,61,
        34,114,101,99,116,49,53,53,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,117,114,108,40,35,95,76,105,110,101,97,114,52,41,34,10,32,32,32,32,32,32,32,104,101,105,103,104,116,61,
        34,51,52,46,57,48,52,57,57,57,34,10,32,32,32,32,32,32,32,119,105,100,116,104,61,34,52,49,46,54,48,54,57,57,56,34,10,32,32,32,32,32,32,32,121,61,34,54,53,53,46,55,49,51,57,57,34,10,32,32,32,32,32,32,32,
        120,61,34,53,55,46,49,52,56,57,57,56,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,52,57,49,41,34,10,32,32,32,32,
        32,105,100,61,34,77,97,115,116,101,114,34,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,53,56,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,
        46,55,50,50,54,57,44,48,44,48,44,49,46,49,52,49,48,54,44,49,55,54,46,50,54,50,44,45,50,54,46,48,51,51,56,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,
        110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,54,48,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,
        34,102,105,108,108,58,35,56,53,56,53,56,53,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,52,46,51,56,57,44,45,49,52,
        46,55,56,56,32,45,57,46,55,57,53,44,45,49,52,46,55,56,56,32,108,32,45,49,50,52,46,54,49,53,44,48,32,67,32,55,46,51,56,57,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,
        32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,52,46,51,56,57,44,49,52,46,55,56,56,32,57,46,55,57,54,44,49,52,46,55,56,56,32,108,32,49,50,52,46,54,49,53,44,48,32,99,32,53,46,52,48,54,44,
        48,32,57,46,55,57,53,44,45,54,46,54,50,54,32,57,46,55,57,53,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,
        103,49,54,50,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,51,46,51,51,56,52,49,44,48,44,48,44,49,46,48,48,53,53,49,44,52,51,48,46,54,49,49,44,45,51,46,
        48,53,48,56,51,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,
        32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,54,52,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,56,53,56,53,56,53,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,
        32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,49,46,57,57,53,44,45,49,52,46,55,56,56,32,45,52,46,52,53,52,44,45,49,52,46,55,56,56,32,108,32,45,49,51,53,46,50,57,56,
        44,48,32,67,32,52,46,57,57,54,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,49,46,57,57,54,44,49,52,46,
        55,56,56,32,52,46,52,53,52,44,49,52,46,55,56,56,32,108,32,49,51,53,46,50,57,56,44,48,32,99,32,50,46,52,53,57,44,48,32,52,46,52,53,52,44,45,54,46,54,50,54,32,52,46,52,53,52,44,45,49,52,46,55,56,56,32,108,
        32,48,44,45,49,49,49,46,52,53,55,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,54,54,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,
        114,105,120,40,49,46,53,52,54,50,54,44,48,44,48,44,49,46,48,48,53,53,49,44,57,50,52,46,56,57,57,44,45,51,46,48,53,48,56,51,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,
        112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,54,56,34,10,32,32,32,32,32,32,32,32,32,115,116,
        121,108,101,61,34,102,105,108,108,58,35,56,53,56,53,56,53,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,52,51,46,51,48,49,32,99,32,48,44,45,56,46,49,54,50,32,45,52,46,51,48,
        57,44,45,49,52,46,55,56,56,32,45,57,46,54,49,54,44,45,49,52,46,55,56,56,32,108,32,45,49,50,52,46,57,55,51,44,48,32,67,32,55,46,51,48,57,44,50,56,46,53,49,51,32,51,44,51,53,46,49,51,57,32,51,44,52,51,46,
        51,48,49,32,108,32,48,44,49,49,49,46,52,53,55,32,99,32,48,44,56,46,49,54,50,32,52,46,51,48,57,44,49,52,46,55,56,56,32,57,46,54,49,55,44,49,52,46,55,56,56,32,108,32,49,50,52,46,57,55,51,44,48,32,99,32,
        53,46,51,48,55,44,48,32,57,46,54,49,54,44,45,54,46,54,50,54,32,57,46,54,49,54,44,45,49,52,46,55,56,56,32,108,32,48,44,45,49,49,49,46,52,53,55,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,
        32,105,100,61,34,103,49,55,48,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,53,53,56,53,57,57,44,48,44,48,44,49,46,48,48,53,53,49,44,49,49,54,48,46,
        50,57,44,45,51,46,48,53,48,56,51,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,
        48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,55,50,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,102,98,52,101,52,102,34,10,32,32,32,32,32,32,
        32,32,32,100,61,34,109,32,49,52,55,46,50,48,54,44,51,54,46,57,49,51,32,99,32,48,44,45,52,46,54,51,54,32,45,54,46,55,55,53,44,45,56,46,52,32,45,49,53,46,49,50,49,44,45,56,46,52,32,108,32,45,49,49,51,46,
        57,54,52,44,48,32,67,32,57,46,55,55,54,44,50,56,46,53,49,51,32,51,44,51,50,46,50,55,55,32,51,44,51,54,46,57,49,51,32,108,32,48,44,49,50,52,46,50,51,51,32,99,32,48,44,52,46,54,51,54,32,54,46,55,55,54,44,
        56,46,52,32,49,53,46,49,50,49,44,56,46,52,32,108,32,49,49,51,46,57,54,52,44,48,32,99,32,56,46,51,52,54,44,48,32,49,53,46,49,50,49,44,45,51,46,55,54,52,32,49,53,46,49,50,49,44,45,56,46,52,32,108,32,48,
        44,45,49,50,52,46,50,51,51,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,55,52,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,
        115,108,97,116,101,40,50,55,48,44,50,56,46,53,49,41,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,55,54,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,
        34,102,105,108,108,58,35,56,53,56,53,56,53,34,10,32,32,32,32,32,32,32,32,32,104,101,105,103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,
        56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,
        32,32,105,100,61,34,103,49,55,56,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,55,54,48,44,50,56,46,53,49,41,34,62,60,114,101,99,116,10,32,32,
        32,32,32,32,32,32,32,105,100,61,34,114,101,99,116,49,56,48,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,56,53,56,53,56,53,34,10,32,32,32,32,32,32,32,32,32,104,101,105,
        103,104,116,61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,
        34,10,32,32,32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,56,50,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,
        114,109,61,34,109,97,116,114,105,120,40,48,46,50,51,56,51,52,56,44,48,44,48,44,49,44,49,49,49,55,46,49,54,44,50,56,46,53,49,41,34,62,60,114,101,99,116,10,32,32,32,32,32,32,32,32,32,105,100,61,34,114,101,
        99,116,49,56,52,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,117,114,108,40,35,95,76,105,110,101,97,114,53,41,34,10,32,32,32,32,32,32,32,32,32,104,101,105,103,104,116,
        61,34,50,55,46,55,48,53,57,57,57,34,10,32,32,32,32,32,32,32,32,32,119,105,100,116,104,61,34,54,49,46,56,48,54,57,57,57,34,10,32,32,32,32,32,32,32,32,32,121,61,34,53,48,46,50,48,50,57,57,57,34,10,32,32,
        32,32,32,32,32,32,32,120,61,34,49,51,55,46,49,54,52,34,32,47,62,60,47,103,62,60,47,103,62,60,103,10,32,32,32,32,32,105,100,61,34,68,114,101,105,101,99,107,101,34,10,32,32,32,32,32,116,114,97,110,115,102,
        111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,48,44,49,49,41,34,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,56,55,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,
        109,97,116,114,105,120,40,45,48,46,53,52,53,51,53,52,44,48,44,48,44,45,48,46,53,52,53,51,53,52,44,50,51,53,46,56,52,54,44,49,48,54,53,46,52,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,
        110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,56,57,34,10,32,32,32,32,32,
        32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,56,54,46,50,53,49,32,49,53,46,52,53,50,44,51,
        48,46,57,48,52,32,45,51,48,46,57,48,52,44,48,32,49,53,46,52,53,50,44,45,51,48,46,57,48,52,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,57,49,34,10,32,32,32,32,32,
        32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,45,48,46,53,52,53,51,53,52,44,48,44,48,44,45,48,46,53,52,53,51,53,52,44,49,49,50,50,46,56,53,44,49,48,54,53,46,52,41,34,62,60,112,
        97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,
        112,97,116,104,49,57,51,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,
        56,54,46,50,53,49,32,49,53,46,52,53,50,44,51,48,46,57,48,52,32,45,51,48,46,57,48,52,44,48,32,49,53,46,52,53,50,44,45,51,48,46,57,48,52,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,
        100,61,34,103,49,57,53,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,45,48,46,53,52,53,51,53,52,44,48,44,48,44,45,48,46,53,52,53,51,53,52,44,49,50,53,49,
        46,56,53,44,49,48,54,53,46,52,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,
        34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,49,57,55,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,32,
        32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,56,54,46,50,53,49,32,49,53,46,52,53,50,44,51,48,46,57,48,52,32,45,51,48,46,57,48,52,44,48,32,49,53,46,52,53,50,44,45,51,48,46,57,48,52,32,122,34,32,47,62,
        60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,49,57,57,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,53,49,50,52,55,57,44,48,44,48,
        44,48,46,53,49,50,52,55,57,44,55,52,46,52,48,51,53,44,52,53,56,46,53,54,50,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,
        99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,49,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,
        52,101,53,52,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,56,54,46,50,53,49,32,49,53,46,52,53,50,44,51,48,46,57,48,52,32,45,51,48,46,57,48,52,44,48,32,49,53,46,52,53,50,
        44,45,51,48,46,57,48,52,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,50,48,51,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,
        120,40,48,46,53,49,50,52,55,57,44,48,44,48,44,48,46,53,49,50,52,55,57,44,49,48,56,57,46,52,44,52,53,56,46,53,54,50,41,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,
        58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,53,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
        101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,56,54,46,50,53,49,32,49,53,46,52,53,50,44,51,48,46,57,48,52,32,45,51,48,
        46,57,48,52,44,48,32,49,53,46,52,53,50,44,45,51,48,46,57,48,52,32,122,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,32,32,105,100,61,34,103,50,48,55,34,10,32,32,32,32,32,32,32,116,114,97,110,115,102,
        111,114,109,61,34,109,97,116,114,105,120,40,48,46,53,49,50,52,55,57,44,48,44,48,44,48,46,53,49,50,52,55,57,44,57,54,49,46,52,48,51,44,52,53,56,46,53,54,50,41,34,62,60,112,97,116,104,10,32,32,32,32,32,
        32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,50,48,57,34,10,
        32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,32,32,100,61,34,109,32,49,53,50,46,54,49,54,44,54,56,54,46,50,53,49,32,49,53,46,
        52,53,50,44,51,48,46,57,48,52,32,45,51,48,46,57,48,52,44,48,32,49,53,46,52,53,50,44,45,51,48,46,57,48,52,32,122,34,32,47,62,60,47,103,62,60,47,103,62,60,100,101,102,115,10,32,32,32,32,32,105,100,61,34,
        100,101,102,115,50,50,55,34,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,100,61,34,95,76,105,110,101,97,114,49,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,
        32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,
        61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,
        44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,52,46,48,53,53,53,41,34,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,10,32,32,32,32,32,32,
        32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,
        32,32,32,32,105,100,61,34,115,116,111,112,50,51,48,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,55,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,
        101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,55,48,44,54,57,44,54,53,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,
        111,112,50,51,50,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,51,57,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,
        108,111,114,58,114,103,98,40,57,56,44,57,56,44,57,56,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,51,52,34,32,47,62,60,115,
        116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,56,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,57,53,
        44,57,53,44,57,53,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,51,54,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,
        32,32,111,102,102,115,101,116,61,34,57,54,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,
        116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,51,56,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,
        116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,
        97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,52,48,34,32,47,62,60,47,108,105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,101,97,114,71,114,
        97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,100,61,34,95,76,105,110,101,97,114,50,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,
        120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,
        32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,
        52,46,48,53,53,53,41,34,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,
        111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,52,51,34,32,47,62,60,
        115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,55,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,55,
        48,44,54,57,44,54,53,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,52,53,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,
        32,32,32,111,102,102,115,101,116,61,34,51,57,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,57,56,44,57,56,44,57,56,41,59,115,116,
        111,112,45,111,112,97,99,105,116,121,58,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,52,55,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,
        61,34,55,54,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,57,53,44,57,53,44,57,53,41,59,115,116,111,112,45,111,112,97,99,105,116,
        121,58,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,52,57,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,57,54,37,34,10,32,32,32,32,
        32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,
        32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,53,49,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,
        121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,
        100,61,34,115,116,111,112,50,53,51,34,32,47,62,60,47,108,105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,100,61,
        34,95,76,105,110,101,97,114,51,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,
        48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,
        97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,52,46,48,53,53,53,41,34,62,60,115,116,111,112,10,32,32,
        32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,
        55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,53,54,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,
        102,115,101,116,61,34,49,55,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,55,48,44,54,57,44,54,53,41,59,115,116,111,112,45,111,112,
        97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,53,56,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,51,57,37,34,10,
        32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,57,56,44,57,56,44,57,56,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,48,34,10,32,32,
        32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,54,48,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,56,49,37,34,10,32,32,32,32,32,32,32,32,32,115,116,
        121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,57,53,44,57,53,44,57,53,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,48,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,
        115,116,111,112,50,54,50,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,57,54,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,
        45,99,111,108,111,114,58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,54,52,
        34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,
        58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,54,54,34,32,47,62,60,47,108,
        105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,100,61,34,95,76,105,110,101,97,114,52,34,10,32,32,32,32,32,32,32,
        120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,
        85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,
        52,49,46,54,48,55,49,44,48,44,48,44,51,52,46,57,48,53,51,44,53,55,46,49,52,56,54,44,54,55,51,46,49,54,54,41,34,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,
        10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,49,54,44,49,49,57,44,49,50,50,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,
        34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,54,57,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,
        32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,54,49,44,49,55,48,44,49,55,56,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,
        32,32,32,105,100,61,34,115,116,111,112,50,55,49,34,32,47,62,60,47,108,105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,
        105,100,61,34,95,76,105,110,101,97,114,53,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,
        50,61,34,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,
        116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,52,46,48,53,53,53,41,34,62,60,115,116,111,112,
        10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,49,51,51,44,49,51,51,
        44,49,51,51,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,55,52,34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,
        111,102,102,115,101,116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,58,114,103,98,40,50,53,49,44,55,56,44,56,49,41,59,115,116,111,
        112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,55,54,34,32,47,62,60,47,108,105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,
        101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,100,61,34,95,76,105,110,101,97,114,54,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,
        32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,
        85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,49,48,51,49,46,48,51,44,48,44,48,44,52,49,54,46,48,49,56,44,50,50,
        52,46,54,55,50,44,51,49,50,46,51,41,34,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,
        45,99,111,108,111,114,58,114,103,98,40,49,51,51,44,49,51,51,44,49,51,51,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,55,57,
        34,32,47,62,60,115,116,111,112,10,32,32,32,32,32,32,32,32,32,111,102,102,115,101,116,61,34,49,48,48,37,34,10,32,32,32,32,32,32,32,32,32,115,116,121,108,101,61,34,115,116,111,112,45,99,111,108,111,114,
        58,114,103,98,40,49,54,48,44,49,55,55,44,49,56,55,41,59,115,116,111,112,45,111,112,97,99,105,116,121,58,49,34,10,32,32,32,32,32,32,32,32,32,105,100,61,34,115,116,111,112,50,56,49,34,32,47,62,60,47,108,
        105,110,101,97,114,71,114,97,100,105,101,110,116,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,
        108,119,97,121,115,34,10,32,32,32,32,32,32,32,120,108,105,110,107,58,104,114,101,102,61,34,35,95,76,105,110,101,97,114,49,34,10,32,32,32,32,32,32,32,105,100,61,34,108,105,110,101,97,114,71,114,97,100,
        105,101,110,116,51,51,52,56,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,
        100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,52,46,48,53,53,53,41,34,10,
        32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,32,47,62,60,108,105,110,101,97,
        114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,120,108,105,110,107,58,104,114,
        101,102,61,34,35,95,76,105,110,101,97,114,50,34,10,32,32,32,32,32,32,32,105,100,61,34,108,105,110,101,97,114,71,114,97,100,105,101,110,116,51,51,53,48,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,
        110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,
        120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,49,54,52,44,54,52,46,48,53,53,53,41,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,
        34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,34,48,34,32,47,62,60,108,105,110,101,97,114,71,114,97,100,105,101,110,116,10,32,32,32,32,32,32,32,105,110,107,115,99,97,
        112,101,58,99,111,108,108,101,99,116,61,34,97,108,119,97,121,115,34,10,32,32,32,32,32,32,32,120,108,105,110,107,58,104,114,101,102,61,34,35,95,76,105,110,101,97,114,51,34,10,32,32,32,32,32,32,32,105,100,
        61,34,108,105,110,101,97,114,71,114,97,100,105,101,110,116,51,51,53,50,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,85,110,105,116,115,61,34,117,115,101,114,83,112,97,99,101,79,110,85,115,
        101,34,10,32,32,32,32,32,32,32,103,114,97,100,105,101,110,116,84,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,54,49,46,56,48,54,57,44,48,44,48,44,50,55,46,55,48,53,57,44,49,51,55,46,
        49,54,52,44,54,52,46,48,53,53,53,41,34,10,32,32,32,32,32,32,32,120,49,61,34,48,34,10,32,32,32,32,32,32,32,121,49,61,34,48,34,10,32,32,32,32,32,32,32,120,50,61,34,49,34,10,32,32,32,32,32,32,32,121,50,61,
        34,48,34,32,47,62,60,47,100,101,102,115,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,53,48,53,56,53,44,48,44,48,44,48,46,57,53,48,53,56,53,44,
        55,56,46,57,53,51,56,44,51,48,55,46,54,54,57,41,34,10,32,32,32,32,32,105,100,61,34,103,50,54,34,62,60,99,105,114,99,108,101,10,32,32,32,32,32,32,32,99,120,61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,
        32,32,32,32,32,99,121,61,34,49,50,55,46,56,49,50,34,10,32,32,32,32,32,32,32,114,61,34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,
        98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,99,105,114,99,108,101,50,56,34,10,32,32,32,32,32,32,32,100,61,34,109,32,55,49,57,46,53,50,48,57,57,44,49,50,55,46,56,49,50,32,99,32,48,44,50,53,46,55,49,
        50,55,50,32,45,50,48,46,56,52,52,50,56,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,44,52,54,46,53,53,55,32,45,50,53,46,55,49,50,55,50,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,52,52,50,56,32,45,
        52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,48,44,45,50,53,46,55,49,50,55,50,32,50,48,46,56,52,52,50,56,44,45,52,54,46,53,53,55,48,48,51,32,52,54,46,53,53,55,44,45,52,54,46,53,53,55,48,48,51,32,50,53,
        46,55,49,50,55,50,44,48,32,52,54,46,53,53,55,44,50,48,46,56,52,52,50,56,51,32,52,54,46,53,53,55,44,52,54,46,53,53,55,48,48,51,32,122,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,99,120,
        61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,99,121,61,34,49,50,55,46,56,49,50,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,120,61,
        34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,121,61,34,52,54,46,53,53,54,57,57,57,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,
        102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,52,54,44,51,48,49,46,54,53,51,56,41,34,10,32,32,32,32,32,105,100,61,34,103,51,48,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,
        32,54,55,50,46,57,54,52,44,56,49,46,50,53,53,32,99,32,50,53,46,54,57,53,44,48,32,52,54,46,53,53,55,44,50,48,46,56,54,49,32,52,54,46,53,53,55,44,52,54,46,53,53,55,32,48,44,50,53,46,54,57,53,32,45,50,48,
        46,56,54,50,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,44,52,54,46,53,53,55,32,45,50,53,46,54,57,54,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,54,50,32,45,52,54,46,53,53,55,44,45,52,54,46,53,53,
        55,32,48,44,45,50,53,46,54,57,54,32,50,48,46,56,54,49,44,45,52,54,46,53,53,55,32,52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,122,32,109,32,48,44,52,46,49,57,32,99,32,50,51,46,51,56,51,44,48,32,52,50,
        46,51,54,55,44,49,56,46,57,56,52,32,52,50,46,51,54,55,44,52,50,46,51,54,55,32,48,44,50,51,46,51,56,51,32,45,49,56,46,57,56,52,44,52,50,46,51,54,55,32,45,52,50,46,51,54,55,44,52,50,46,51,54,55,32,45,50,
        51,46,51,56,51,44,48,32,45,52,50,46,51,54,55,44,45,49,56,46,57,56,52,32,45,52,50,46,51,54,55,44,45,52,50,46,51,54,55,32,48,44,45,50,51,46,51,56,51,32,49,56,46,57,56,52,44,45,52,50,46,51,54,55,32,52,50,
        46,51,54,55,44,45,52,50,46,51,54,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,51,50,34,
        10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,
        115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,53,48,53,56,53,44,48,44,48,44,48,46,57,53,48,53,56,53,44,55,56,46,57,53,51,56,44,49,53,53,46,54,54,57,41,34,10,32,32,32,32,32,105,100,61,34,
        103,55,49,34,62,60,99,105,114,99,108,101,10,32,32,32,32,32,32,32,99,120,61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,32,32,32,32,32,99,121,61,34,49,50,55,46,56,49,50,34,10,32,32,32,32,32,32,32,114,61,
        34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,34,10,32,32,32,32,32,32,32,105,100,61,34,99,105,114,99,108,101,55,51,34,10,32,
        32,32,32,32,32,32,100,61,34,109,32,55,49,57,46,53,50,48,57,57,44,49,50,55,46,56,49,50,32,99,32,48,44,50,53,46,55,49,50,55,50,32,45,50,48,46,56,52,52,50,56,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,
        44,52,54,46,53,53,55,32,45,50,53,46,55,49,50,55,50,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,52,52,50,56,32,45,52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,48,44,45,50,53,46,55,49,50,55,50,32,50,
        48,46,56,52,52,50,56,44,45,52,54,46,53,53,55,48,48,51,32,52,54,46,53,53,55,44,45,52,54,46,53,53,55,48,48,51,32,50,53,46,55,49,50,55,50,44,48,32,52,54,46,53,53,55,44,50,48,46,56,52,52,50,56,51,32,52,54,
        46,53,53,55,44,52,54,46,53,53,55,48,48,51,32,122,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,99,120,61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,
        100,105,58,99,121,61,34,49,50,55,46,56,49,50,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,120,61,34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,32,32,115,111,100,105,112,111,100,
        105,58,114,121,61,34,52,54,46,53,53,54,57,57,57,34,32,47,62,60,47,103,62,60,103,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,52,54,44,49,52,57,46,54,
        53,51,56,41,34,10,32,32,32,32,32,105,100,61,34,103,55,53,34,62,60,112,97,116,104,10,32,32,32,32,32,32,32,100,61,34,109,32,54,55,50,46,57,54,52,44,56,49,46,50,53,53,32,99,32,50,53,46,54,57,53,44,48,32,
        52,54,46,53,53,55,44,50,48,46,56,54,49,32,52,54,46,53,53,55,44,52,54,46,53,53,55,32,48,44,50,53,46,54,57,53,32,45,50,48,46,56,54,50,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,44,52,54,46,53,53,55,32,
        45,50,53,46,54,57,54,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,54,50,32,45,52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,48,44,45,50,53,46,54,57,54,32,50,48,46,56,54,49,44,45,52,54,46,53,53,55,32,
        52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,122,32,109,32,48,44,52,46,49,57,32,99,32,50,51,46,51,56,51,44,48,32,52,50,46,51,54,55,44,49,56,46,57,56,52,32,52,50,46,51,54,55,44,52,50,46,51,54,55,32,48,
        44,50,51,46,51,56,51,32,45,49,56,46,57,56,52,44,52,50,46,51,54,55,32,45,52,50,46,51,54,55,44,52,50,46,51,54,55,32,45,50,51,46,51,56,51,44,48,32,45,52,50,46,51,54,55,44,45,49,56,46,57,56,52,32,45,52,50,
        46,51,54,55,44,45,52,50,46,51,54,55,32,48,44,45,50,51,46,51,56,51,32,49,56,46,57,56,52,44,45,52,50,46,51,54,55,32,52,50,46,51,54,55,44,45,52,50,46,51,54,55,32,122,34,10,32,32,32,32,32,32,32,115,116,121,
        108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,105,100,61,34,112,97,116,104,55,55,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,
        111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,99,105,114,99,108,101,10,32,32,32,32,32,116,114,97,110,115,102,111,114,109,61,34,109,97,116,114,105,120,40,48,46,57,53,
        48,53,56,53,44,48,44,48,44,48,46,57,53,48,53,56,53,44,55,56,46,57,53,51,56,44,48,46,54,54,57,41,34,10,32,32,32,32,32,99,120,61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,32,32,32,99,121,61,34,49,50,55,
        46,56,49,50,34,10,32,32,32,32,32,114,61,34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,97,48,98,49,98,98,59,102,105,108,108,45,114,117,108,101,58,101,
        118,101,110,111,100,100,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,
        111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,49,46,52,49,52,50,48,57,57,54,34,10,32,32,32,32,32,105,100,61,34,99,105,114,99,108,101,49,49,56,34,10,32,32,32,32,32,100,61,34,109,32,55,49,57,
        46,53,50,48,57,57,44,49,50,55,46,56,49,50,32,99,32,48,44,50,53,46,55,49,50,55,50,32,45,50,48,46,56,52,52,50,56,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,44,52,54,46,53,53,55,32,45,50,53,46,55,49,50,
        55,50,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,52,52,50,56,32,45,52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,48,44,45,50,53,46,55,49,50,55,50,32,50,48,46,56,52,52,50,56,44,45,52,54,46,53,53,55,
        48,48,51,32,52,54,46,53,53,55,44,45,52,54,46,53,53,55,48,48,51,32,50,53,46,55,49,50,55,50,44,48,32,52,54,46,53,53,55,44,50,48,46,56,52,52,50,56,51,32,52,54,46,53,53,55,44,52,54,46,53,53,55,48,48,51,32,
        122,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,99,120,61,34,54,55,50,46,57,54,51,57,57,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,99,121,61,34,49,50,55,46,56,49,50,34,10,32,32,
        32,32,32,115,111,100,105,112,111,100,105,58,114,120,61,34,52,54,46,53,53,54,57,57,57,34,10,32,32,32,32,32,115,111,100,105,112,111,100,105,58,114,121,61,34,52,54,46,53,53,54,57,57,57,34,32,47,62,60,103,
        10,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,45,114,117,108,101,58,101,118,101,110,111,100,100,59,115,116,114,111,107,101,45,108,105,110,101,99,97,112,58,114,111,117,110,100,59,115,116,
        114,111,107,101,45,108,105,110,101,106,111,105,110,58,114,111,117,110,100,59,115,116,114,111,107,101,45,109,105,116,101,114,108,105,109,105,116,58,49,46,52,49,52,50,48,57,57,54,34,10,32,32,32,32,32,116,
        114,97,110,115,102,111,114,109,61,34,116,114,97,110,115,108,97,116,101,40,52,54,44,45,53,46,51,52,54,50,41,34,10,32,32,32,32,32,105,100,61,34,103,49,50,48,34,62,60,112,97,116,104,10,32,32,32,32,32,32,
        32,100,61,34,109,32,54,55,50,46,57,54,52,44,56,49,46,50,53,53,32,99,32,50,53,46,54,57,53,44,48,32,52,54,46,53,53,55,44,50,48,46,56,54,49,32,52,54,46,53,53,55,44,52,54,46,53,53,55,32,48,44,50,53,46,54,
        57,53,32,45,50,48,46,56,54,50,44,52,54,46,53,53,55,32,45,52,54,46,53,53,55,44,52,54,46,53,53,55,32,45,50,53,46,54,57,54,44,48,32,45,52,54,46,53,53,55,44,45,50,48,46,56,54,50,32,45,52,54,46,53,53,55,44,
        45,52,54,46,53,53,55,32,48,44,45,50,53,46,54,57,54,32,50,48,46,56,54,49,44,45,52,54,46,53,53,55,32,52,54,46,53,53,55,44,45,52,54,46,53,53,55,32,122,32,109,32,48,44,52,46,50,55,54,32,99,32,50,51,46,51,
        51,53,44,48,32,52,50,46,50,56,49,44,49,56,46,57,52,53,32,52,50,46,50,56,49,44,52,50,46,50,56,49,32,48,44,50,51,46,51,51,53,32,45,49,56,46,57,52,54,44,52,50,46,50,56,49,32,45,52,50,46,50,56,49,44,52,50,
        46,50,56,49,32,45,50,51,46,51,51,54,44,48,32,45,52,50,46,50,56,49,44,45,49,56,46,57,52,54,32,45,52,50,46,50,56,49,44,45,52,50,46,50,56,49,32,48,44,45,50,51,46,51,51,54,32,49,56,46,57,52,53,44,45,52,50,
        46,50,56,49,32,52,50,46,50,56,49,44,45,52,50,46,50,56,49,32,122,34,10,32,32,32,32,32,32,32,115,116,121,108,101,61,34,102,105,108,108,58,35,53,48,52,101,53,52,34,10,32,32,32,32,32,32,32,105,100,61,34,112,
        97,116,104,49,50,50,34,10,32,32,32,32,32,32,32,105,110,107,115,99,97,112,101,58,99,111,110,110,101,99,116,111,114,45,99,117,114,118,97,116,117,114,101,61,34,48,34,32,47,62,60,47,103,62,60,47,115,118,103,
        62,0,0
                                                                                      };

const char* Monique_Ui_Mainwindow::_01hintergrundalles_svg = (const char*) resource_Monique_Ui_Mainwindow__01hintergrundalles_svg;
const int Monique_Ui_Mainwindow::_01hintergrundalles_svgSize = 23727;


//[EndFile] You can add extra defines here...
//[/EndFile]
