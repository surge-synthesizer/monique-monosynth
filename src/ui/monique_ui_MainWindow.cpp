/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
*/

#include "monique_ui_MainWindow.h"

#include "monique_ui_AmpPainter.h"
#include "monique_ui_Credits.h"
#include "monique_ui_ENVPopup.h"
#include "monique_ui_GlobalSettings.h"
#include "monique_ui_Info.h"
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MFOPopup.h"
#include "monique_ui_MIDIIO.h"
#include "monique_ui_MidiLearnPopup.h"
#include "monique_ui_ModulationSlider.h"
#include "monique_ui_Morph.h"
#include "monique_ui_OptionPopup.h"
#include "monique_ui_Overlay.h"
#include "monique_ui_Playback.h"
#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_SliderConfig.h"
#include "core/monique_core_Processor.h"
#include "core/monique_core_Synth.h"

#include <memory>

//==============================================================================
//==============================================================================
class ButtonFlasher : public juce::Timer, juce::AsyncUpdater
{
    Monique_Ui_Mainwindow *const parent;
    juce::Button *const button;
    int counter;
    const bool button_state;
    const bool success;
    const int num_flashs;

    void timerCallback() override
    {
        if (++counter >= num_flashs * 2)
        {
            stopTimer();
            triggerAsyncUpdate();
            return;
        }
        else
        {
            if (success)
            {
                button->setColour(juce::TextButton::buttonColourId, juce::Colours::lightgreen);
            }
            else
            {
                button->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
            }
            button->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, counter % 2);
            button->repaint();
        }
    }
    void handleAsyncUpdate() override { parent->button_flasher = nullptr; }

  public:
    ButtonFlasher(Monique_Ui_Mainwindow *parent_, juce::Button *button_, bool success_,
                  int num_flashs_ = 2) noexcept
        : parent(parent_), button(button_), counter(0), button_state(IS_BUTTON_ON(button)),
          success(success_), num_flashs(num_flashs_)
    {
        button->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
        timerCallback();
        startTimer(400);
    }
    ~ButtonFlasher() noexcept
    {
        TURN_BUTTON_ON_OR_OFF(button, button_state);
        button->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);
        button->repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonFlasher)
};

void Monique_Ui_Mainwindow::refresh() noexcept
{
    if (last_env_popup_open != synth_data->midi_env_popup.get_value())
    {
        last_env_popup_open = synth_data->midi_env_popup;
        open_env_or_lfo_popup_by_midi(&synth_data->midi_env_popup);
    }
    else if (last_lfo_popup_open != synth_data->midi_lfo_popup.get_value())
    {
        last_lfo_popup_open = synth_data->midi_lfo_popup;
        open_env_or_lfo_popup_by_midi(&synth_data->midi_lfo_popup);
    }

    if (!combo_programm->hasKeyboardFocus(false) && combo_programm->isTextEditable())
    {
        combo_programm->setEditableText(false);
    }

    show_current_voice_data();
    show_current_poly_data();
    resize_sequence_buttons();
    show_programs_and_select(false);
    show_ctrl_state();
}
void Monique_Ui_Mainwindow::update_tooltip_handling(bool is_help_key_down_) noexcept
{
    // TOOLTIP TIME
    if (synth_data->show_tooltips)
    {
        if (tooltipWindow == nullptr)
        {
            tooltipWindow =
                std::make_unique<juce::TooltipWindow>(nullptr, is_help_key_down_ ? 30 : 3000);
        }
        else if (is_help_key_down_)
        {
            tooltipWindow->setMillisecondsBeforeTipAppears(30);
        }
        else if (!is_help_key_down_)
        {
            tooltipWindow->setMillisecondsBeforeTipAppears(3000);
        }
    }
    else
    {
        if (is_help_key_down_ && tooltipWindow == nullptr)
        {
            tooltipWindow = std::make_unique<juce::TooltipWindow>(nullptr, 20);
        }
        else if (!is_help_key_down_ && tooltipWindow)
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
    for (int i = 0; i != dual_sliders.size(); ++i)
    {
        Monique_Ui_DualSlider *slider = dual_sliders.getUnchecked(i);
        slider->update_return_values();
    }
}
void Monique_Ui_Mainwindow::show_programs_and_select(bool force)
{
    const int current_bank = synth_data->get_current_bank();
    const int current_programm = synth_data->get_current_program();
    if (force || current_bank != last_bank || current_programm != last_programm)
    {
        last_bank = current_bank;
        last_programm = current_programm;

        combo_bank->clear(juce::dontSendNotification);
        combo_bank->addItemList(synth_data->get_banks(), 1);
        combo_bank->setSelectedId(synth_data->get_current_bank() + 1, juce::dontSendNotification);

        combo_programm->clear(juce::dontSendNotification);
        auto programms = synth_data->get_current_bank_programms();
        combo_programm->addItemList(programms, 1);
        // int programm_id = synth_data->get_current_program();
        /*
        if( programm_id >= 0 )
               {
                   combo_programm->setSelectedItemIndex(programm_id,dontSendNotification);
               }
               else
               {
          */
        combo_programm->setText(synth_data->alternative_program_name, juce::dontSendNotification);
        combo_programm->setTextWhenNothingSelected(synth_data->alternative_program_name);
        combo_programm->setTextWhenNoChoicesAvailable("EMPTY BANK");

        if (programms.size() == 0)
        {
            combo_programm->addItem(synth_data->alternative_program_name, 1);
        }
        //}
    }
}
void Monique_Ui_Mainwindow::global_slider_settings_changed(juce::Component *parent_) noexcept
{

    const bool rotary_sliders_are_in_rotary_mode = synth_data->sliders_in_rotary_mode;
    const int rotary_sensitivity = synth_data->sliders_sensitivity;
    const int linear_sensitivity = synth_data->sliders_linear_sensitivity;
    for (int i = 0; i < parent_->getNumChildComponents(); ++i)
    {
        /*
        if( Button* button = dynamic_cast< Button* >( parent_->getChildComponent(i) ) )
        {
            if( slider->getSliderStyle() == Slider::Rotary )
            {
                slider->setSliderStyle ( rotary_sliders_are_in_rotary_mode ? Slider::Rotary :
        Slider::RotaryHorizontalVerticalDrag ); slider->setMouseDragSensitivity( rotary_sensitivity
        );
            }
            else
            {
                slider->setMouseDragSensitivity( linear_sensitivity );
            }
        }
        */
        if (juce::Slider *slider = dynamic_cast<juce::Slider *>(parent_->getChildComponent(i)))
        {
            juce::Slider::SliderStyle current_style = slider->getSliderStyle();
            if ((current_style == juce::Slider::LinearVertical ||
                 current_style == juce::Slider::Slider::LinearHorizontal))
            {
                slider->setMouseDragSensitivity(linear_sensitivity);
                slider->setVelocityBasedMode(synth_data->is_linear_sliders_velocity_mode);
                slider->setVelocityModeParameters(float(linear_sensitivity) / 1000, 1, 0, false);
            }
            else
            {
                slider->setSliderStyle(rotary_sliders_are_in_rotary_mode
                                           ? (synth_data->is_rotary_sliders_velocity_mode
                                                  ? juce::Slider::RotaryHorizontalVerticalDrag
                                                  : juce::Slider::Rotary)
                                           : juce::Slider::RotaryHorizontalVerticalDrag);
                slider->setMouseDragSensitivity(rotary_sensitivity);
                slider->setVelocityBasedMode(synth_data->is_rotary_sliders_velocity_mode);
            }
        }
        else if (Monique_Ui_DualSlider *mod_slider =
                     dynamic_cast<Monique_Ui_DualSlider *>(parent_->getChildComponent(i)))
        {
            for (int n = 0; n < mod_slider->getNumChildComponents(); ++n)
            {
                if (juce::Slider *slider =
                        dynamic_cast<juce::Slider *>(mod_slider->getChildComponent(n)))
                {
                    const bool is_linear = mod_slider->_config->get_is_linear();
                    if (is_linear && !synth_data->only_use_rotary_sliders)
                    {
                        slider->setSliderStyle(
                            rotary_sliders_are_in_rotary_mode
                                ? synth_data->is_rotary_sliders_velocity_mode
                                      ? juce::Slider::RotaryHorizontalVerticalDrag
                                      : juce::Slider::Rotary
                                : juce::Slider::LinearVertical);
                        slider->setMouseDragSensitivity(linear_sensitivity);
                        slider->setVelocityBasedMode(synth_data->is_linear_sliders_velocity_mode);
                        slider->setVelocityModeParameters(float(linear_sensitivity) / 1000, 1, 0,
                                                          false);
                    }
                    else
                    {
                        slider->setSliderStyle(
                            rotary_sliders_are_in_rotary_mode
                                ? synth_data->is_rotary_sliders_velocity_mode
                                      ? juce::Slider::RotaryHorizontalVerticalDrag
                                      : juce::Slider::Rotary
                                : juce::Slider::RotaryHorizontalVerticalDrag);
                        slider->setMouseDragSensitivity(rotary_sensitivity);
                        slider->setVelocityBasedMode(synth_data->is_rotary_sliders_velocity_mode);
                    }
                }
            }
        }
    }
}

void Monique_Ui_Mainwindow::show_ctrl_state()
{
    if (last_shift_state != synth_data->shift)
    {
        last_shift_state = synth_data->shift;
        ui_refresher->pause();
        for (int i = 0; i < dual_sliders.size(); ++i)
        {
            if (Monique_Ui_DualSlider *mod_slider = dual_sliders.getUnchecked(i))
            {
                if (mod_slider->is_in_shift_view() != synth_data->shift)
                {
                    mod_slider->set_shift_view_mode(synth_data->shift);
                    mod_slider->show_view_mode();
                }
            }
        }
        ui_refresher->go_on();
    }
}
Parameter *
Monique_Ui_Mainwindow::find_parameter_in_dual_sliders(const juce::Component *comp_) noexcept
{
    if (const juce::Slider *slider = dynamic_cast<const juce::Slider *>(comp_))
    {
        for (int i = 0; i < dual_sliders.size(); ++i)
        {
            Monique_Ui_DualSlider *dual_slider = dual_sliders.getUnchecked(i);
            if (Parameter *param = dual_slider->get_parameter(slider))
            {
                return param;
            }
        }
    }

    return nullptr;
}
Parameter *
Monique_Ui_Mainwindow::find_back_parameter_in_dual_sliders(const juce::Component *comp_) noexcept
{
    if (const juce::Slider *slider = dynamic_cast<const juce::Slider *>(comp_))
    {
        for (int i = 0; i < dual_sliders.size(); ++i)
        {
            Monique_Ui_DualSlider *dual_slider = dual_sliders.getUnchecked(i);
            if (dual_slider->get_parameter(slider))
            {
                return dual_slider->get_back_parameter();
            }
        }
    }

    return nullptr;
}

void Monique_Ui_Mainwindow::show_info_popup(juce::Component *comp_, MIDIControl *midi_conrtrol_,
                                            bool force_turn_on_)
{
    popup = nullptr;
    if (force_turn_on_)
    {
        if (!midi_control_handler->is_learning() || !midi_control_handler->is_waiting_for_param())
        {
            midi_control_handler->toggle_midi_learn();
        }

        if (!midi_conrtrol_)
        {
            if (Monique_Ui_DualSlider *slider = dynamic_cast<Monique_Ui_DualSlider *>(comp_))
            {
                // slider->slider_value->triggerClick();
                return;
            }
            else if (juce::Slider *slider = dynamic_cast<juce::Slider *>(comp_))
            {
                // slider->triggerClick();
                return;
            }
            else
            {
                juce::Component *parent = comp_->getParentComponent();
                if (parent)
                {
                    do
                    {
                        if (Monique_Ui_DualSlider *slider =
                                dynamic_cast<Monique_Ui_DualSlider *>(parent))
                        {
                            // slider->slider_value->triggerClick();

                            return;
                        }
                    } while ((parent = parent->getParentComponent()));
                }
            }
        }
    }

    if (midi_control_handler->is_learning() && midi_conrtrol_)
    {
        popup = std::make_unique<Monique_Ui_MainwindowPopup>(ui_refresher, this, midi_conrtrol_);
        addChildComponent(*popup);
        popup->set_element_to_show(comp_);
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
    // FIXME
    {
        static bool fix_oss_port_issue = false;
        jassert(fix_oss_port_issue);
        fix_oss_port_issue = true;
    }
    // keyboard->setRootNote( 60+synth_data->note_offset.get_value()-24 );

    ComponentColours &colours = look_and_feel->colours;

    // FILTER 1
    int f_type = synth_data->filter_datas[0]->filter_type;
    TURN_BUTTON_ON_OR_OFF(filter_type_6_1,
                          f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_2_1, f_type == HPF || f_type == HIGH_2_PASS)
    TURN_BUTTON_ON_OR_OFF(filter_type_3_1, f_type == BPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_5_1, f_type == PASS || f_type == UNKNOWN)

    // FILTER 2
    f_type = synth_data->filter_datas[1]->filter_type;
    TURN_BUTTON_ON_OR_OFF(filter_type_6_2,
                          f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_2_2, f_type == HPF || f_type == HIGH_2_PASS)
    TURN_BUTTON_ON_OR_OFF(filter_type_3_2, f_type == BPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_5_2, f_type == PASS || f_type == UNKNOWN)

    // FILTER 3
    f_type = synth_data->filter_datas[2]->filter_type;
    TURN_BUTTON_ON_OR_OFF(filter_type_6_3,
                          f_type == LPF || f_type == LPF_2_PASS || f_type == MOOG_AND_LPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_2_3, f_type == HPF || f_type == HIGH_2_PASS)
    TURN_BUTTON_ON_OR_OFF(filter_type_3_3, f_type == BPF)
    TURN_BUTTON_ON_OR_OFF(filter_type_5_3, f_type == PASS || f_type == UNKNOWN)

    {
        juce::ScopedLock resize_locked(resize_lock);
        int current_start_id = 0;
#define UPDATE_SEQUENCE_BUTTON(id)                                                                 \
    if (sequence_buttons.getUnchecked(id)->getProperties().set(                                    \
            VAR_INDEX_BUTTON_AMP,                                                                  \
            synth_data->arp_sequencer_data->step[id]                                               \
                ? synth_data->animate_arp           ? voice->get_arp_sequence_amp(id) * 0.4 + 0.6  \
                  : voice->get_arp_sequence_amp(id) ? 1                                            \
                                                    : 0.6                                          \
            : voice->get_arp_sequence_amp(id) > 0 && synth_data->animate_arp ? 0.3f                \
                                                                             : 0))                 \
    {                                                                                              \
        sequence_buttons.getUnchecked(id)->repaint();                                              \
    }

        UPDATE_SEQUENCE_BUTTON(0);
        UPDATE_SEQUENCE_BUTTON(1);
        UPDATE_SEQUENCE_BUTTON(2);
        UPDATE_SEQUENCE_BUTTON(3);
        UPDATE_SEQUENCE_BUTTON(4);
        UPDATE_SEQUENCE_BUTTON(5);
        UPDATE_SEQUENCE_BUTTON(6);
        UPDATE_SEQUENCE_BUTTON(7);
        UPDATE_SEQUENCE_BUTTON(8);
        UPDATE_SEQUENCE_BUTTON(9);
        UPDATE_SEQUENCE_BUTTON(10);
        UPDATE_SEQUENCE_BUTTON(11);
        UPDATE_SEQUENCE_BUTTON(12);
        UPDATE_SEQUENCE_BUTTON(13);
        UPDATE_SEQUENCE_BUTTON(14);
        UPDATE_SEQUENCE_BUTTON(15);
    }

    TURN_BUTTON_ON_OR_OFF(button_arp_speed_XNORM,
                          synth_data->arp_sequencer_data->speed_multi.get_value() == 0)

    // MORPHERS
    TURN_BUTTON_ON_OR_OFF(button_open_morph, editor_morph)
    bool state = synth_data->get_morph_switch_state(0);
    TURN_BUTTON_ON_OR_OFF(button_toggle_morph_buttons_1, state)
    if (state == LEFT)
    {
        button_toggle_morph_buttons_1->setButtonText("OSC-L");
    }
    else
    {
        button_toggle_morph_buttons_1->setButtonText("OSC-R");
    }

    state = synth_data->get_morph_switch_state(1);
    TURN_BUTTON_ON_OR_OFF(button_toggle_morph_buttons_2, state)
    if (state == LEFT)
    {
        button_toggle_morph_buttons_2->setButtonText("FLT-L");
    }
    else
    {
        button_toggle_morph_buttons_2->setButtonText("FLT-R");
    }

    state = synth_data->get_morph_switch_state(3);
    TURN_BUTTON_ON_OR_OFF(button_toggle_morph_buttons_3, state)
    if (state == LEFT)
    {
        button_toggle_morph_buttons_3->setButtonText("ARP-L");
    }
    else
    {
        button_toggle_morph_buttons_3->setButtonText("ARP-R");
    }

    state = synth_data->get_morph_switch_state(2);
    TURN_BUTTON_ON_OR_OFF(button_toggle_morph_buttons_4, state)
    if (state == LEFT)
    {
        button_toggle_morph_buttons_4->setButtonText("FX-L");
    }
    else
    {
        button_toggle_morph_buttons_4->setButtonText("FX-R");
    }

    // EDITORS
#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
    if (flash_counter > 0)
    {
        flash_counter--;
        if (button_open_midi_io_settings->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                              VALUE_SLIDER_COLOUR))
        {
            button_open_midi_io_settings->repaint();
        }
    }
    else
    {
        {
            static bool fix_oss_port_issue = false;
            jassert(fix_oss_port_issue);
            fix_oss_port_issue = true;
        }
        /*
         if( button_open_midi_io_settings->getProperties().set( VAR_INDEX_BUTTON_AMP,
         *editor_midiio ) ) { button_open_midi_io_settings->repaint();
         }
         */
    }
#else
    if (button_open_midi_io_settings->getProperties().set(
            VAR_INDEX_BUTTON_AMP, midi_control_handler->is_waiting_for_param() ? FORCE_RED
                                  : midi_control_handler->is_learning()        ? FORCE_RED
                                                                               : TURN_OFF))
    {
        button_open_midi_io_settings->repaint();
    }
#endif
    if (button_open_oszi->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                              amp_painter ? static_cast<int>(VALUE_SLIDER_COLOUR)
                                                          : static_cast<int>(TURN_OFF)))
    {
        button_open_oszi->repaint();
    }
    if (button_vers_info->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                              vers_info ? static_cast<int>(VALUE_SLIDER_COLOUR)
                                                        : static_cast<int>(TURN_OFF)))
    {
        button_vers_info->repaint();
    }
    if (button_open_config2->getProperties().set(
            VAR_INDEX_BUTTON_AMP, editor_global_settings ? static_cast<int>(VALUE_SLIDER_2_COLOUR)
                                                         : static_cast<int>(TURN_OFF)))
    {
        button_open_config2->repaint();
    }
    if (button_ctrl_toggle->getProperties().set(
            VAR_INDEX_BUTTON_AMP,
            synth_data->shift ? static_cast<int>(MOD_SLIDER_COLOUR) : static_cast<int>(TURN_OFF)))
    {
        button_ctrl_toggle->repaint();
    }
    if (button_values_toggle->getProperties().set(
            VAR_INDEX_BUTTON_AMP, look_and_feel->show_values_always ? TURN_ON : TURN_OFF))
    {
        button_values_toggle->repaint();
    }
    if (button_open_playback->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                  synth_data->ui_is_large.get_value() == 1
                                                      ? static_cast<int>(VALUE_SLIDER_COLOUR)
                                                      : static_cast<int>(TURN_OFF)))
    {
        button_open_playback->repaint();
    }

    // ACTIVE FILTER INPUTS
    {
        const FilterData *const filter_1_data = synth_data->filter_datas[0];
        const FilterData *const filter_2_data = synth_data->filter_datas[1];
        const FilterData *const filter_3_data = synth_data->filter_datas[2];

        bool last_filter_1_1_sustain = filter_1_data->input_sustains[0] > 0 ? 1 : 0;
        bool last_filter_1_2_sustain = filter_1_data->input_sustains[1] > 0 ? 1 : 0;
        bool last_filter_1_3_sustain = filter_1_data->input_sustains[2] > 0 ? 1 : 0;

        bool last_filter_2_1_sustain =
            filter_2_data->input_sustains[0] < 0
                ? true
                : (filter_2_data->input_sustains[0] > 0) && last_filter_1_1_sustain;
        bool last_filter_2_2_sustain =
            filter_2_data->input_sustains[1] < 0
                ? true
                : (filter_2_data->input_sustains[1] > 0) && last_filter_1_2_sustain;
        bool last_filter_2_3_sustain =
            filter_2_data->input_sustains[2] < 0
                ? true
                : (filter_2_data->input_sustains[2] > 0) && last_filter_1_3_sustain;

        // FILTER 2
        {
            float filter_2_1_sustain = filter_2_data->input_sustains[0];
            float filter_2_2_sustain = filter_2_data->input_sustains[1];
            float filter_2_3_sustain = filter_2_data->input_sustains[2];

            // FILTER 2 INPUT 1
            if (filter_2_1_sustain > 0)
            {
                if (button_show_active_input_l_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_1->repaint();
                }
                if (last_filter_1_1_sustain)
                {
                    if (button_show_active_input_r_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_2_1->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_2_1->repaint();
                    }
                }
            }
            else if (filter_2_1_sustain < 0)
            {
                if (button_show_active_input_l_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_2_1->repaint();
                }
                if (button_show_active_input_r_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_2_1->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_1->repaint();
                }
                if (button_show_active_input_r_2_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_2_1->repaint();
                }
            }

            // FILTER 2 INPUT 2
            if (filter_2_2_sustain > 0)
            {
                if (button_show_active_input_l_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_2->repaint();
                }
                if (last_filter_1_2_sustain)
                {
                    if (button_show_active_input_r_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_2_2->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_2_2->repaint();
                    }
                }
            }
            else if (filter_2_2_sustain < 0)
            {
                if (button_show_active_input_l_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_2_2->repaint();
                }
                if (button_show_active_input_r_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_2_2->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_2->repaint();
                }
                if (button_show_active_input_r_2_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_2_2->repaint();
                }
            }

            // FILTER 2 INPUT 3
            if (filter_2_3_sustain > 0)
            {
                if (button_show_active_input_l_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_3->repaint();
                }
                if (last_filter_1_3_sustain)
                {
                    if (button_show_active_input_r_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_2_3->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_2_3->repaint();
                    }
                }
            }
            else if (filter_2_3_sustain < 0)
            {
                if (button_show_active_input_l_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_2_3->repaint();
                }
                if (button_show_active_input_r_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_2_3->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_2_3->repaint();
                }
                if (button_show_active_input_r_2_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
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
            if (filter_3_1_sustain > 0)
            {
                if (button_show_active_input_l_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_1->repaint();
                }
                if (last_filter_2_1_sustain)
                {
                    if (button_show_active_input_r_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_3_1->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_3_1->repaint();
                    }
                }
            }
            else if (filter_3_1_sustain < 0)
            {
                if (button_show_active_input_l_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_3_1->repaint();
                }
                if (button_show_active_input_r_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_1->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_1->repaint();
                }
                if (button_show_active_input_r_3_1->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_1->repaint();
                }
            }

            // FILTER 3 INPUT 2
            if (filter_3_2_sustain > 0)
            {
                if (button_show_active_input_l_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_2->repaint();
                }
                if (last_filter_2_2_sustain)
                {
                    if (button_show_active_input_r_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_3_2->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_3_2->repaint();
                    }
                }
            }
            else if (filter_3_2_sustain < 0)
            {
                if (button_show_active_input_l_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_3_2->repaint();
                }
                if (button_show_active_input_r_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_2->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_2->repaint();
                }
                if (button_show_active_input_r_3_2->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_2->repaint();
                }
            }

            // FILTER 3 INPUT 3
            if (filter_3_3_sustain > 0)
            {
                if (button_show_active_input_l_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_3->repaint();
                }
                if (last_filter_2_3_sustain)
                {
                    if (button_show_active_input_r_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            TURN_ON))
                    {
                        button_show_active_input_r_3_3->repaint();
                    }
                }
                else
                {
                    if (button_show_active_input_r_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                            FORCE_RED))
                    {
                        button_show_active_input_r_3_3->repaint();
                    }
                }
            }
            else if (filter_3_3_sustain < 0)
            {
                if (button_show_active_input_l_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_ON))
                {
                    button_show_active_input_l_3_3->repaint();
                }
                if (button_show_active_input_r_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_3->repaint();
                }
            }
            else
            {
                if (button_show_active_input_l_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_l_3_3->repaint();
                }
                if (button_show_active_input_r_3_3->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                                        TURN_OFF))
                {
                    button_show_active_input_r_3_3->repaint();
                }
            }
        }
    }

    const int current_note = voice->get_current_note();
    if (current_note != last_refreshed_note)
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
    if (synth_data->get_current_program() == -1)
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

void Monique_Ui_Mainwindow::show_current_poly_data()
{
    if (original_w == 1760)
    {
#ifdef POLY
        // KEYTRACK
        const bool is_key_0_down = synth_data->is_key_down(0);
        const bool is_key_1_down = synth_data->is_key_down(1);
        const bool is_key_2_down = synth_data->is_key_down(2);
        // MODES
        TURN_BUTTON_ON_OR_OFF(button_tracking_mode_lf, synth_data->keytrack_osci_play_mode == 0);

        TURN_BUTTON_ON_OR_OFF(button_tracking_mode_hf, synth_data->keytrack_osci_play_mode == 1);
        TURN_BUTTON_ON_OR_OFF(button_tracking_mode_keep, synth_data->keytrack_osci_play_mode == 2);
        TURN_BUTTON_ON_OR_OFF(button_tracking_mode_hm, synth_data->keytrack_osci_play_mode == 3);

        // OSC TRACKING
        TURN_BUTTON_ON_OR_OFF(button_osc_tracking_1,
                              synth_data->keytrack_osci[0] ? 0.7 + is_key_0_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_osc_tracking_2,
                              synth_data->keytrack_osci[1] ? 0.7 + is_key_1_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_osc_tracking_3,
                              synth_data->keytrack_osci[2] ? 0.7 + is_key_2_down * 0.3 : 0);

        slider_osc_tracking_oct_2->setValue(synth_data->keytrack_osci_octave_offset[1].get_value(),
                                            dontSendNotification);
        slider_osc_tracking_oct_3->setValue(synth_data->keytrack_osci_octave_offset[2].get_value(),
                                            dontSendNotification);

        TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_1,
                              synth_data->keytrack_cutoff[0] ? 0.7 + is_key_0_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_2,
                              synth_data->keytrack_cutoff[1] ? 0.7 + is_key_1_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_cutoff_tracking_3,
                              synth_data->keytrack_cutoff[2] ? 0.7 + is_key_2_down * 0.3 : 0);

        slider_cutoff_tracking_oct_1->setValue(
            synth_data->keytrack_cutoff_octave_offset[0].get_value(), dontSendNotification);
        slider_cutoff_tracking_oct_2->setValue(
            synth_data->keytrack_cutoff_octave_offset[1].get_value(), dontSendNotification);
        slider_cutoff_tracking_oct_3->setValue(
            synth_data->keytrack_cutoff_octave_offset[2].get_value(), dontSendNotification);

        // INPUT TRIG
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_1_1, synth_data->keytrack_filter_inputs[0]
                                                                   ? 0.7 + is_key_0_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_1_2, synth_data->keytrack_filter_inputs[1]
                                                                   ? 0.7 + is_key_1_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_1_3, synth_data->keytrack_filter_inputs[2]
                                                                   ? 0.7 + is_key_2_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_2_1, synth_data->keytrack_filter_inputs[3]
                                                                   ? 0.7 + is_key_0_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_2_2, synth_data->keytrack_filter_inputs[4]
                                                                   ? 0.7 + is_key_1_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_2_3, synth_data->keytrack_filter_inputs[5]
                                                                   ? 0.7 + is_key_2_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_3_1, synth_data->keytrack_filter_inputs[6]
                                                                   ? 0.7 + is_key_0_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_3_2, synth_data->keytrack_filter_inputs[7]
                                                                   ? 0.7 + is_key_1_down * 0.3
                                                                   : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_input_triggering_3_3, synth_data->keytrack_filter_inputs[8]
                                                                   ? 0.7 + is_key_2_down * 0.3
                                                                   : 0);

        // ENV TRIG
        TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_1,
                              synth_data->keytrack_filter_env[0] ? 0.7 + is_key_0_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_2,
                              synth_data->keytrack_filter_env[1] ? 0.7 + is_key_1_down * 0.3 : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_env_triggering_3,
                              synth_data->keytrack_filter_env[2] ? 0.7 + is_key_2_down * 0.3 : 0);

        // OUTPUT TRIG
        TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_1, synth_data->keytrack_filter_volume[0]
                                                               ? 0.7 + is_key_0_down * 0.3
                                                               : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_2, synth_data->keytrack_filter_volume[1]
                                                               ? 0.7 + is_key_1_down * 0.3
                                                               : 0);
        TURN_BUTTON_ON_OR_OFF(button_flt_out_triggering_3, synth_data->keytrack_filter_volume[2]
                                                               ? 0.7 + is_key_2_down * 0.3
                                                               : 0);

        slider_flt_out_sesitivity_1->setValue(
            synth_data->keytrack_filter_volume_offset[0].get_value(), dontSendNotification);
        slider_flt_out_sesitivity_2->setValue(
            synth_data->keytrack_filter_volume_offset[1].get_value(), dontSendNotification);
        slider_flt_out_sesitivity_3->setValue(
            synth_data->keytrack_filter_volume_offset[2].get_value(), dontSendNotification);
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

        TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_1,
        synth_data->keytrack_filter_volume_1 ); if(
        button_flt_key_tracking_volume_1->getProperties().set( VAR_INDEX_BUTTON_AMP,
        synth_data->keytrack_filter_volume_1 ? 0.7 + synth_data->get_tracking_env_state(0)*0.3 : 0 )
        )
        {
            button_flt_key_tracking_volume_1->repaint();
        }
        TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_2,
        synth_data->keytrack_filter_volume_2 ); if(
        button_flt_key_tracking_volume_2->getProperties().set( VAR_INDEX_BUTTON_AMP,
        synth_data->keytrack_filter_volume_2 ? 0.7 + synth_data->get_tracking_env_state(1)*0.3 : 0 )
        )
        {
            button_flt_key_tracking_volume_2->repaint();
        }
        TURN_BUTTON_ON_OR_OFF( button_flt_key_tracking_volume_3,
        synth_data->keytrack_filter_volume_3 ); if(
        button_flt_key_tracking_volume_3->getProperties().set( VAR_INDEX_BUTTON_AMP,
        synth_data->keytrack_filter_volume_3 ? 0.7 + synth_data->get_tracking_env_state(2)*0.3 : 0 )
        )
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
        // midi_control_handler->is_waiting_for_param() ? true : midi_control_handler->is_learning()
        // ? true : false );
    }
}

void Monique_Ui_Mainwindow::resize_sequence_buttons(bool force_)
{
    juce::ScopedLock locked(resize_lock);

    if (last_step_offset != synth_data->arp_sequencer_data->step_offset.get_value() || force_)
    {
        last_step_offset = synth_data->arp_sequencer_data->step_offset;

        // SET NEWL
        for (int i = 0; i != SUM_ENV_ARP_STEPS; ++i)
        {
            int current_start_id = i + last_step_offset;
            if (current_start_id >= SUM_ENV_ARP_STEPS)
            {
                current_start_id -= SUM_ENV_ARP_STEPS;
            }
            Monique_Ui_DualSlider *slider = sequence_sliders.getUnchecked(i);
            slider->setTopLeftPosition(original_slider_positions.getUnchecked(current_start_id));

            sequence_buttons.set(i, sequence_buttons_original_order.getUnchecked(current_start_id));
        }
    }

    const float width_factor = 1.0f / original_w * getWidth();
    const float height_factor = 1.0f / original_h * getHeight();
    float shuffle =
        floor(60.0f * width_factor *
              ArpSequencerData::shuffle_to_value(synth_data->arp_sequencer_data->shuffle));
    const int fine_offset = synth_data->arp_sequencer_data->fine_offset;
    if (shuffle != last_shuffle || force_ || last_fine_offset != fine_offset)
    {
        const int use_shuffle = floor(60.0f * width_factor + shuffle);
        last_shuffle = shuffle;
        last_fine_offset = fine_offset;

        float fine_offset_pos = sequence_sliders[0]->getWidth() * 0.3;
        switch (synth_data->arp_sequencer_data->fine_offset)
        {
        case -5:
            fine_offset_pos /= -1.5;
            break;
        case -4:
            fine_offset_pos /= -2;
            break;
        case -3:
            fine_offset_pos /= -3;
            break;
        case -2:
            fine_offset_pos /= -6;
            break;
        case -1:
            fine_offset_pos /= -8;
            break;
        case 0:
            fine_offset_pos = 0;
            break;
        case 1:
            fine_offset_pos /= 8;
            break;
        case 2:
            fine_offset_pos /= 6;
            break;
        case 3:
            fine_offset_pos /= 3;
            break;
        case 4:
            fine_offset_pos /= 2;
            break;
        case 5:
            fine_offset_pos /= 1.5;
            break;
        }

        for (int i = 0; i != sequence_buttons_original_order.size(); ++i)
        {
            int current_start_id = i + last_step_offset;
            if (current_start_id >= SUM_ENV_ARP_STEPS)
            {
                current_start_id -= SUM_ENV_ARP_STEPS;
            }
            juce::TextButton *button = sequence_buttons_original_order[i];
            juce::Point<int> point = original_slider_positions[i];
            if (current_start_id % 4 == 0)
            {
                button->setBounds(fine_offset_pos + point.getX(), point.getY(), use_shuffle,
                                  30.0f * height_factor);
            }
            else if (current_start_id % 2 == 0)
            {
                button->setBounds(fine_offset_pos + point.getX(), point.getY(), use_shuffle,
                                  30.0f * height_factor);
            }
            else
            {
                int id = i - 1;
                if (id < 0)
                {
                    id += SUM_ENV_ARP_STEPS;
                }
                juce::Point<int> point_before = original_slider_positions[id];
                button->setBounds(float(fine_offset_pos + use_shuffle + point_before.getX()),
                                  float(point.getY()), 60.0f * width_factor - shuffle,
                                  30.0f * height_factor);
            }
        }
    }
}
void Monique_Ui_Mainwindow::switch_finalizer_tab(bool fx_)
{
    if (fx_)
    {
        open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
    }

    if (effect_finalizer_switch2->getProperties().set(
            VAR_INDEX_BUTTON_AMP, !fx_ ? USE_AREA_COLOUR : USE_AREA_TRANSCULENT))
    {
        effect_finalizer_switch2->repaint();
    }
    if (effect_finalizer_switch->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                     fx_ ? USE_AREA_COLOUR : USE_AREA_TRANSCULENT))
    {
        effect_finalizer_switch->repaint();
    }

    reverb_room->setVisible(fx_);
    reverb_dry->setVisible(fx_);
    delay2->setVisible(fx_);
    delay3->setVisible(fx_);
    delay4->setVisible(fx_);
    chorus_modulation->setVisible(fx_);
    bypass->setVisible(fx_);
    label_reverb->setVisible(fx_);
    label_fx_delay->setVisible(fx_);
    distortion->setVisible(fx_);

    // eg
    colour->setVisible(!fx_);
    eq_1->setVisible(!fx_);
    eq_2->setVisible(!fx_);
    eq_3->setVisible(!fx_);
    eq_4->setVisible(!fx_);
    eq_5->setVisible(!fx_);
    eq_6->setVisible(!fx_);
    eq_7->setVisible(!fx_);
    label_band_hz_1->setVisible(!fx_);
    label_band_hz_2->setVisible(!fx_);
    label_band_hz_3->setVisible(!fx_);
    label_band_hz_4->setVisible(!fx_);
    label_band_hz_5->setVisible(!fx_);
    label_band_hz_6->setVisible(!fx_);
    label_band_hz_7->setVisible(!fx_);
    button_edit_input_env_band_1->setVisible(!fx_);
    button_edit_input_env_band_2->setVisible(!fx_);
    button_edit_input_env_band_3->setVisible(!fx_);
    button_edit_input_env_band_4->setVisible(!fx_);
    button_edit_input_env_band_5->setVisible(!fx_);
    button_edit_input_env_band_6->setVisible(!fx_);
    button_edit_input_env_band_7->setVisible(!fx_);
}

/**
 * Finds and returns the hosting display of a given component
 * depending on the component's top left corner.
 */
auto get_host_display(const juce::Component &component)
{
    const auto &desktop = juce::Desktop::getInstance();
    const auto &displays = desktop.getDisplays();
    const auto component_bounds = component.getBounds();
    const auto component_top_left_corner =
        juce::Point<int>{component_bounds.getX(), component_bounds.getY()};
    const auto &host_display = displays.getDisplayForPoint(component_top_left_corner);

    return host_display;
}

/**
 * Resizes a component to a scale of its original bounds.
 * But if the scaled component will be to large to fit its hosting display,
 * then it will be shrink to fit. (hosting display bounds*0.9)
 *
 * @param component to resize
 * @param original_bounds of the component without scale
 * @param wanted_scale of the the original component bounds we actually like to have
 */
void resize_component_to_host_display(juce::Component &component,
                                      const juce::Rectangle<float> &original_bounds,
                                      float wanted_scale)
{
    const auto host_display = get_host_display(component);
    const auto &user_area = host_display->userArea;

    auto height = original_bounds.getHeight() * wanted_scale;
    auto width = original_bounds.getWidth() * wanted_scale;

    const auto host_height = host_display->userArea.getHeight();
    const auto host_width = user_area.getWidth();
    if (host_height < height)
    {
        height = host_height * 0.9;
        float shrink_to_height_scale = 1.0f / original_bounds.getHeight() * height;
        width = original_bounds.getWidth() * shrink_to_height_scale;
    }

    if (host_width < width)
    {
        width = host_height * 0.9;
        float shrink_to_with_scale = 1.0f / original_bounds.getWidth() * width;
        width = original_bounds.getWidth() * shrink_to_with_scale;
        height = original_bounds.getHeight() * shrink_to_with_scale;
    }

    component.setSize(width, height);
}

/**
 * Resize this window to original bounds * synth_data->ui_scale_factor
 * or shrinks it to fit into its hosting display.
 */
void Monique_Ui_Mainwindow::update_size()
{
#ifndef IS_MOBILE
    const auto original_bounds = juce::Rectangle<float>{original_w, original_h};
    resize_component_to_host_display(*this, original_bounds, synth_data->ui_scale_factor);
#endif
}

void Monique_Ui_Mainwindow::show_overlay() noexcept
{
    overlay->setAlwaysOnTop(true);
    overlay->setVisible(true);
}
void Monique_Ui_Mainwindow::show_credits(bool force_) noexcept
{
    if (force_ || synth_data->show_tooltips)
    {
        credits->setAlwaysOnTop(true);
        credits->setVisible(true);
    }
}
void Monique_Ui_Mainwindow::hide_credits() noexcept
{
    credits->setAlwaysOnTop(false);
    credits->setVisible(false);
}
void CreditsPoper::mouseEnter(const juce::MouseEvent &e_) { parent->show_credits(force); }
void CreditsPoper::mouseExit(const juce::MouseEvent &e_) { parent->hide_credits(); }

void Monique_Ui_Mainwindow::toggle_modulation_slider_top_button(juce::Button *button_,
                                                                bool by_force_) noexcept
{
    if (!clear_record_timer)
    {
        struct ChorusCleaner : juce::Timer, juce::AsyncUpdater
        {
            juce::Button *const button_down;
            juce::String button_text;
            Monique_Ui_Mainwindow *const parent;
            int counter;
            bool force;
            void timerCallback() override
            {
                if (++counter > 31)
                {
                    if (force || juce::Desktop::getInstance()
                                         .getMainMouseSource()
                                         .getComponentUnderMouse() == button_down)
                    {
                        if (force)
                        {
                            // parent->synth_data->delay_record = false;
                        }
                        parent->clear_record_buffer();
                        // TODO flash
                    }
                    stopTimer();
                    triggerAsyncUpdate();
                }
                else
                {
                    if (counter % 2 == 0)
                    {
                        juce::String value(3.2f - (float(counter) / 10));
                        if (value == "3")
                        {
                            value = "3.0";
                        }
                        else if (value == "2")
                        {
                            value = "2.0";
                        }
                        else if (value == "1")
                        {
                            value = "1.0";
                        }
                        button_down->setButtonText(juce::String("CLR ") + value);
                    }
                    else
                    {
                        button_down->setButtonText(juce::String(""));
                    }
                }
            }
            void handleAsyncUpdate() override { parent->clear_record_timer = nullptr; }

            ChorusCleaner(juce::Button *button__, Monique_Ui_Mainwindow *const parent_, bool force_)
                : button_down(button__), parent(parent_), counter(0), force(force_)
            {
                button_text = button_down->getButtonText();
                if (force)
                {
                    // parent_->synth_data->delay_record = false;
                }
            }
            ~ChorusCleaner()
            {
                button_down->setButtonText(button_text);
                if (force)
                {
                    // parent->synth_data->delay_record = false;
                }
            }
        };

        clear_record_timer = std::make_unique<ChorusCleaner>(button_, this, by_force_);
        clear_record_timer->startTimer(100);
    }
    else
    {
        stop_clear_chorus();
    }
}

void Monique_Ui_Mainwindow::clear_record_buffer() noexcept
{
#ifdef JUCE_DEBUG
    std::cout << "clear" << std::endl;
#endif

    voice->clear_record_buffer();
}
void Monique_Ui_Mainwindow::stop_clear_chorus() noexcept
{
    if (clear_record_timer)
    {
        clear_record_timer = nullptr;
    }
}
// OpenGLContext

static const char *openGLRendererName = "OpenGL Renderer";

juce::StringArray Monique_Ui_Mainwindow::getRenderingEngines() const
{
    juce::StringArray renderingEngines;

    if (juce::ComponentPeer *peer = getPeer())
        renderingEngines = peer->getAvailableRenderingEngines();

#ifdef JUCE_OPENGL
    renderingEngines.add(openGLRendererName);
#endif

    return renderingEngines;
}

void Monique_Ui_Mainwindow::setRenderingEngine(int index)
{
#ifdef JUCE_OPENGL
    if (getRenderingEngines()[index] == openGLRendererName)
    {
        openGLContext.attachTo(*getTopLevelComponent());
        return;
    }

    openGLContext.detach();
#endif

    if (juce::ComponentPeer *peer = getPeer())
        peer->setCurrentRenderingEngine(index);
}

void Monique_Ui_Mainwindow::setOpenGLRenderingEngine()
{
    setRenderingEngine(getRenderingEngines().indexOf(openGLRendererName));
}

int Monique_Ui_Mainwindow::getActiveRenderingEngine() const
{
#ifdef JUCE_OPENGL
    if (openGLContext.isAttached())
        return getRenderingEngines().indexOf(openGLRendererName);
#endif

    if (juce::ComponentPeer *peer = getPeer())
        return peer->getCurrentRenderingEngine();

    return 0;
}

//==============================================================================
Monique_Ui_Mainwindow::Monique_Ui_Mainwindow(Monique_Ui_Refresher *ui_refresher_)
    : juce::AudioProcessorEditor(ui_refresher_->audio_processor),
      Monique_Ui_Refreshable(ui_refresher_), original_w(1760), original_h(1210)
{
#ifndef JUCE_WINDOWS
    setOpenGLRenderingEngine();
#endif

    /*
#ifdef IS_MOBILE
    // its big by default
    //original_w = 1760;
    synth_data->ui_is_large.set_value(false);
#else
     */
#ifdef POLY
    if (!synth_data->ui_is_large)
    {
        original_w = 1465;
    }
#else
    original_w = 1465;
#endif

    last_refreshed_note = -1;
    audio_processor = reinterpret_cast<MoniqueAudioProcessor *>(&processor);
    amp_painter = nullptr;
    vers_info = nullptr;
    flash_counter = 0;
    program_edit_type = NOT_SET;
    ui_refresher_->editor = this;
    look_and_feel->mainwindow = this;
    clear_record_timer = nullptr;
    last_lfo_popup_open = synth_data->midi_lfo_popup;
    last_env_popup_open = synth_data->midi_env_popup;
    last_step_offset = 0;
    last_fine_offset = 0;
    last_shift_state = 0;

    filter_type_bg_button_5 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_5);
    filter_type_bg_button_5->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_5->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_5->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_5->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_bg_button_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_4);
    filter_type_bg_button_4->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_4->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    overlay = std::make_unique<monique_ui_Overlay>();
    addAndMakeVisible(*overlay);

#ifdef JUCE_DEBUG
    label_monique = std::make_unique<juce::Label>("DL", TRANS("D E B U G"));
#else
    label_monique = std::make_unique<juce::Label>("DL", TRANS("M O N I Q U E"));
#endif
    addAndMakeVisible(*label_monique);
    label_monique->setFont(juce::Font(250.00f, juce::Font::plain));
    label_monique->setJustificationType(juce::Justification::centred);
    label_monique->setEditable(false, false, false);
    label_monique->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_monique->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_monique->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    pop_credits = std::make_unique<CreditsPoper>(this);
    addAndMakeVisible(*pop_credits);

    label_fx_delay = std::make_unique<juce::Label>(juce::String(), TRANS("DELAY"));
    addAndMakeVisible(*label_fx_delay);
    label_fx_delay->setFont(juce::Font(30.00f, juce::Font::plain));
    label_fx_delay->setJustificationType(juce::Justification::centred);
    label_fx_delay->setEditable(false, false, false);
    label_fx_delay->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_fx_delay->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_fx_delay->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    eq_7 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 6));
    addAndMakeVisible(*eq_7);

    eq_6 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 5));
    addAndMakeVisible(*eq_6);

    eq_5 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 4));
    addAndMakeVisible(*eq_5);

    eq_4 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 3));
    addAndMakeVisible(*eq_4);

    eq_3 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 2));
    addAndMakeVisible(*eq_3);

    eq_2 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 1));
    addAndMakeVisible(*eq_2);

    eq_1 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EQSlConfig(synth_data, 0));
    addAndMakeVisible(*eq_1);

    distortion =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FXDistortionSlConfig(synth_data));
    addAndMakeVisible(*distortion);

    chorus_modulation =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new CModSlConfig(synth_data));
    addAndMakeVisible(*chorus_modulation);

    delay3 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new DelaySlConfig(synth_data));
    addAndMakeVisible(*delay3);

    delay2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new DelayReflexSlConfig(synth_data));
    addAndMakeVisible(*delay2);

    delay4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new DelayRecordSlConfig(synth_data));
    addAndMakeVisible(*delay4);

    reverb_room =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new RRoomSlConfig(synth_data));
    addAndMakeVisible(*reverb_room);

    reverb_dry =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new RDrySlConfig(synth_data));
    addAndMakeVisible(*reverb_dry);

    label_lfo_3 = std::make_unique<juce::Label>(juce::String(), TRANS("LFO 3"));
    addAndMakeVisible(*label_lfo_3);
    label_lfo_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_lfo_3->setJustificationType(juce::Justification::centred);
    label_lfo_3->setEditable(false, false, false);
    label_lfo_3->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_lfo_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_lfo_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_lfo_2 = std::make_unique<juce::Label>(juce::String(), TRANS("LFO 2"));
    addAndMakeVisible(*label_lfo_2);
    label_lfo_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_lfo_2->setJustificationType(juce::Justification::centred);
    label_lfo_2->setEditable(false, false, false);
    label_lfo_2->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_lfo_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_lfo_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_lfo_1 = std::make_unique<juce::Label>(juce::String(), TRANS("LFO 1"));
    addAndMakeVisible(*label_lfo_1);
    label_lfo_1->setFont(juce::Font(30.00f, juce::Font::plain));
    label_lfo_1->setJustificationType(juce::Justification::centred);
    label_lfo_1->setEditable(false, false, false);
    label_lfo_1->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_lfo_1->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_lfo_1->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    lfo_1 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new LFOSlConfig(synth_data, 0));
    addAndMakeVisible(*lfo_1);

    lfo_2 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new LFOSlConfig(synth_data, 1));
    addAndMakeVisible(*lfo_2);

    lfo_3 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new LFOSlConfig(synth_data, 2));
    addAndMakeVisible(*lfo_3);

    morpher_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new MorphSLConfig(synth_data, 0));
    addAndMakeVisible(*morpher_1);

    morpher_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new MorphSLConfig(synth_data, 1));
    addAndMakeVisible(*morpher_2);

    morpher_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new MorphSLConfig(synth_data, 3));
    addAndMakeVisible(*morpher_4);

    morpher_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new MorphSLConfig(synth_data, 2));
    addAndMakeVisible(*morpher_3);

    flt_input_13 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 2, 2));
    addAndMakeVisible(*flt_input_13);

    flt_input_12 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 2, 1));
    addAndMakeVisible(*flt_input_12);

    flt_input_11 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 2, 0));
    addAndMakeVisible(*flt_input_11);

    flt_input_6 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 1, 0));
    addAndMakeVisible(*flt_input_6);

    flt_input_7 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 1, 1));
    addAndMakeVisible(*flt_input_7);

    flt_input_8 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 1, 2));
    addAndMakeVisible(*flt_input_8);

    flt_input_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 0, 2));
    addAndMakeVisible(*flt_input_3);

    flt_input_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 0, 1));
    addAndMakeVisible(*flt_input_2);

    flt_input_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new InputSlConfig(synth_data, 0, 0));
    addAndMakeVisible(*flt_input_1);

    button_edit_lfo_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_lfo_1);
    button_edit_lfo_1->setTooltip(TRANS(
        "Open/Close a popup to edit this LFO.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_lfo_1->addListener(this);
    button_edit_lfo_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_lfo_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_lfo_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_lfo_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_lfo_2);
    button_edit_lfo_2->setTooltip(TRANS(
        "Open/Close a popup to edit this LFO.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_lfo_2->addListener(this);
    button_edit_lfo_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_lfo_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_lfo_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_lfo_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_lfo_3);
    button_edit_lfo_3->setTooltip(TRANS(
        "Open/Close a popup to edit this LFO.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_lfo_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_lfo_3->addListener(this);
    button_edit_lfo_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_lfo_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_lfo_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_3_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_3_3);
    button_edit_input_env_3_3->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_3_3->addListener(this);
    button_edit_input_env_3_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_3_3->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_3_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_3_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_3_2);
    button_edit_input_env_3_2->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_3_2->addListener(this);
    button_edit_input_env_3_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_3_2->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_3_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_3_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_3_1);
    button_edit_input_env_3_1->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_3_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_3_1->addListener(this);
    button_edit_input_env_3_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_3_1->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_3_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_2_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_2_1);
    button_edit_input_env_2_1->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_2_1->addListener(this);
    button_edit_input_env_2_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_2_1->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_2_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_2_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_2_2);
    button_edit_input_env_2_2->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_2_2->addListener(this);
    button_edit_input_env_2_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_2_2->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_2_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_2_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_2_3);
    button_edit_input_env_2_3->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_2_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_2_3->addListener(this);
    button_edit_input_env_2_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_2_3->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_2_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_1_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_1_3);
    button_edit_input_env_1_3->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_1_3->addListener(this);
    button_edit_input_env_1_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_1_3->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_1_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_1_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_1_2);
    button_edit_input_env_1_2->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_1_2->addListener(this);
    button_edit_input_env_1_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_1_2->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_1_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_1_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_1_1);
    button_edit_input_env_1_1->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this input.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_1_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_1_1->addListener(this);
    button_edit_input_env_1_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_1_1->setColour(juce::TextButton::textColourOnId,
                                         juce::Colour(0xffff3b00));
    button_edit_input_env_1_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_mfo_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_mfo_4);
    button_edit_mfo_4->setTooltip(TRANS(
        "Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_4->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_mfo_4->addListener(this);
    button_edit_mfo_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_mfo_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_mfo_4->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_mfo_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_mfo_3);
    button_edit_mfo_3->setTooltip(TRANS(
        "Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_mfo_3->addListener(this);
    button_edit_mfo_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_mfo_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_mfo_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_mfo_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_mfo_2);
    button_edit_mfo_2->setTooltip(TRANS(
        "Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_mfo_2->addListener(this);
    button_edit_mfo_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_mfo_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_mfo_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_mfo_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_mfo_1);
    button_edit_mfo_1->setTooltip(TRANS(
        "Open/Close the morph oscillator popup to edit the mfo for this morph group.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_mfo_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_mfo_1->addListener(this);
    button_edit_mfo_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_mfo_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_edit_mfo_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_band_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_1);
    button_edit_input_env_band_1->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_1->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_1->addListener(this);
    button_edit_input_env_band_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_1->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_1->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    button_edit_input_env_band_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_2);
    button_edit_input_env_band_2->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_2->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_2->addListener(this);
    button_edit_input_env_band_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_2->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_2->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    effect_finalizer_switch2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*effect_finalizer_switch2);
    effect_finalizer_switch2->setTooltip(TRANS("Switch to the EQ bank."));
    effect_finalizer_switch2->setButtonText(TRANS("EQ"));
    effect_finalizer_switch2->setConnectedEdges(juce::Button::ConnectedOnTop);
    effect_finalizer_switch2->addListener(this);
    effect_finalizer_switch2->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff11ff));
    effect_finalizer_switch2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    effect_finalizer_switch2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_edit_input_env_band_7 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_7);
    button_edit_input_env_band_7->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_7->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_7->addListener(this);
    button_edit_input_env_band_7->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_7->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_7->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    button_edit_input_env_band_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_3);
    button_edit_input_env_band_3->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_3->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_3->addListener(this);
    button_edit_input_env_band_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_3->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_3->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    button_edit_input_env_band_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_4);
    button_edit_input_env_band_4->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_4->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_4->addListener(this);
    button_edit_input_env_band_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_4->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_4->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    button_edit_input_env_band_5 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_5);
    button_edit_input_env_band_5->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_5->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_5->addListener(this);
    button_edit_input_env_band_5->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_5->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_5->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    button_edit_input_env_band_6 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_edit_input_env_band_6);
    button_edit_input_env_band_6->setTooltip(TRANS(
        "Open/Close the envelope popup to edit the envelope for this band.\n"
        "\n"
        "Note: check the popup settings on the right of the popup to setup its close handling."));
    button_edit_input_env_band_6->setButtonText(juce::CharPointer_UTF8("\xe2\x97\x8b"));
    button_edit_input_env_band_6->addListener(this);
    button_edit_input_env_band_6->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_edit_input_env_band_6->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_edit_input_env_band_6->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    filter_type_bg_button_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_3);
    filter_type_bg_button_3->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_2_3 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_2_3);
    filter_type_2_3->setTooltip(TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_3->setButtonText(TRANS("HP"));
    filter_type_2_3->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_2_3->addListener(this);
    filter_type_2_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_2_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_2_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_bg_button_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_2);
    filter_type_bg_button_2->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_2_2 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_2_2);
    filter_type_2_2->setTooltip(TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_2->setButtonText(TRANS("HP"));
    filter_type_2_2->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_2_2->addListener(this);
    filter_type_2_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_2_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_2_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_bg_button_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*filter_type_bg_button_1);
    filter_type_bg_button_1->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_bg_button_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_bg_button_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_bg_button_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_2_1 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_2_1);
    filter_type_2_1->setTooltip(TRANS("Set the filter type to HIGH PASS."));
    filter_type_2_1->setButtonText(TRANS("HP"));
    filter_type_2_1->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_2_1->addListener(this);
    filter_type_2_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_2_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_2_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_toggle_morph_buttons_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_toggle_morph_buttons_1);
    button_toggle_morph_buttons_1->setTooltip(
        TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_1->setButtonText(TRANS("OSC-T"));
    button_toggle_morph_buttons_1->addListener(this);
    button_toggle_morph_buttons_1->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_toggle_morph_buttons_1->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_toggle_morph_buttons_1->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    button_toggle_morph_buttons_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_toggle_morph_buttons_2);
    button_toggle_morph_buttons_2->setTooltip(
        TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_2->setButtonText(TRANS("FLT-T"));
    button_toggle_morph_buttons_2->addListener(this);
    button_toggle_morph_buttons_2->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_toggle_morph_buttons_2->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_toggle_morph_buttons_2->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    button_toggle_morph_buttons_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_toggle_morph_buttons_3);
    button_toggle_morph_buttons_3->setTooltip(
        TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_3->setButtonText(TRANS("ARP-T"));
    button_toggle_morph_buttons_3->addListener(this);
    button_toggle_morph_buttons_3->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_toggle_morph_buttons_3->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_toggle_morph_buttons_3->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    button_toggle_morph_buttons_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_toggle_morph_buttons_4);
    button_toggle_morph_buttons_4->setTooltip(
        TRANS("Toggles between the button states of the left and right morph side."));
    button_toggle_morph_buttons_4->setButtonText(TRANS("FX-T"));
    button_toggle_morph_buttons_4->addListener(this);
    button_toggle_morph_buttons_4->setColour(juce::TextButton::buttonColourId,
                                             juce::Colours::black);
    button_toggle_morph_buttons_4->setColour(juce::TextButton::textColourOnId,
                                             juce::Colour(0xffff3b00));
    button_toggle_morph_buttons_4->setColour(juce::TextButton::textColourOffId,
                                             juce::Colours::yellow);

    label_band_hz_5 = std::make_unique<juce::Label>("DL", TRANS("1.3kHz"));
    addAndMakeVisible(*label_band_hz_5);
    label_band_hz_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_5->setJustificationType(juce::Justification::centred);
    label_band_hz_5->setEditable(false, false, false);
    label_band_hz_5->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_band_hz_6 = std::make_unique<juce::Label>("DL", TRANS("2.6kHz"));
    addAndMakeVisible(*label_band_hz_6);
    label_band_hz_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_6->setJustificationType(juce::Justification::centred);
    label_band_hz_6->setEditable(false, false, false);
    label_band_hz_6->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_band_hz_4 = std::make_unique<juce::Label>("DL", TRANS("660Hz"));
    addAndMakeVisible(*label_band_hz_4);
    label_band_hz_4->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_4->setJustificationType(juce::Justification::centred);
    label_band_hz_4->setEditable(false, false, false);
    label_band_hz_4->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_4->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_band_hz_1 = std::make_unique<juce::Label>("DL", TRANS("<82Hz"));
    addAndMakeVisible(*label_band_hz_1);
    label_band_hz_1->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_1->setJustificationType(juce::Justification::centred);
    label_band_hz_1->setEditable(false, false, false);
    label_band_hz_1->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_1->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_1->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_morph = std::make_unique<juce::Label>(juce::String(), TRANS("MORPH (MO) MIXER"));
    addAndMakeVisible(*label_morph);
    label_morph->setFont(juce::Font(30.00f, juce::Font::plain));
    label_morph->setJustificationType(juce::Justification::centred);
    label_morph->setEditable(false, false, false);
    label_morph->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_morph->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_morph->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_band_hz_7 = std::make_unique<juce::Label>("DL", TRANS(">2.6kHz"));
    addAndMakeVisible(*label_band_hz_7);
    label_band_hz_7->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_7->setJustificationType(juce::Justification::centred);
    label_band_hz_7->setEditable(false, false, false);
    label_band_hz_7->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_7->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_arpeggiator = std::make_unique<juce::Label>(juce::String(), TRANS("ARPEGGIATOR"));
    addAndMakeVisible(*label_arpeggiator);
    label_arpeggiator->setFont(juce::Font(30.00f, juce::Font::plain));
    label_arpeggiator->setJustificationType(juce::Justification::centred);
    label_arpeggiator->setEditable(false, false, false);
    label_arpeggiator->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_arpeggiator->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_arpeggiator->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_programm_replace = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_replace);
    button_programm_replace->setTooltip(TRANS("Replaces the selected program."));
    button_programm_replace->setButtonText(TRANS("SAVE"));
    button_programm_replace->addListener(this);
    button_programm_replace->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_replace->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_programm_replace->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_programm_new = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_new);
    button_programm_new->setTooltip(TRANS("Create a new program from the current state."));
    button_programm_new->setButtonText(TRANS("SAVE AS"));
    button_programm_new->addListener(this);
    button_programm_new->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_new->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffbcff00));
    button_programm_new->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffd0ff00));

    bypass = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new BypassConfig(synth_data));
    addAndMakeVisible(*bypass);

    colour = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FColourSlConfig(synth_data));
    addAndMakeVisible(*colour);

    label_band_hz_2 = std::make_unique<juce::Label>("DL", TRANS("165Hz"));
    addAndMakeVisible(*label_band_hz_2);
    label_band_hz_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_2->setJustificationType(juce::Justification::centred);
    label_band_hz_2->setEditable(false, false, false);
    label_band_hz_2->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_band_hz_3 = std::make_unique<juce::Label>("DL", TRANS("330Hz"));
    addAndMakeVisible(*label_band_hz_3);
    label_band_hz_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_band_hz_3->setJustificationType(juce::Justification::centred);
    label_band_hz_3->setEditable(false, false, false);
    label_band_hz_3->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_band_hz_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_band_hz_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    speed_multi =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new SpeedMultiSlConfig(synth_data));
    addAndMakeVisible(*speed_multi);

    osc_wave_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new WAVESlConfig(synth_data, 2));
    addAndMakeVisible(*osc_wave_3);

    keyboard = std::make_unique<juce::MidiKeyboardComponent>(
        *reinterpret_cast<MoniqueAudioProcessor *>(&processor),
        juce::MidiKeyboardComponent::horizontalKeyboard);
    addAndMakeVisible(*keyboard);

    glide2 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new GlideConfig(synth_data));
    addAndMakeVisible(*glide2);

    arp_step_16 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 15));
    addAndMakeVisible(*arp_step_16);

    arp_step_15 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 14));
    addAndMakeVisible(*arp_step_15);

    arp_step_14 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 13));
    addAndMakeVisible(*arp_step_14);

    arp_step_13 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 12));
    addAndMakeVisible(*arp_step_13);

    arp_step_12 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 11));
    addAndMakeVisible(*arp_step_12);

    arp_step_11 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 10));
    addAndMakeVisible(*arp_step_11);

    arp_step_10 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 9));
    addAndMakeVisible(*arp_step_10);

    arp_step_9 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 8));
    addAndMakeVisible(*arp_step_9);

    arp_step_8 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 7));
    addAndMakeVisible(*arp_step_8);

    arp_step_7 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 6));
    addAndMakeVisible(*arp_step_7);

    arp_step_6 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 5));
    addAndMakeVisible(*arp_step_6);

    arp_step_5 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 4));
    addAndMakeVisible(*arp_step_5);

    arp_step_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 3));
    addAndMakeVisible(*arp_step_4);

    arp_step_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 2));
    addAndMakeVisible(*arp_step_3);

    arp_step_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 1));
    addAndMakeVisible(*arp_step_2);

    arp_step_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ArpStepSlConfig(synth_data, 0));
    addAndMakeVisible(*arp_step_1);

    shuffle = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new ShuffleConfig(synth_data));
    addAndMakeVisible(*shuffle);

    flt_sustain_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FSustainSlConfig(synth_data));
    addAndMakeVisible(*flt_sustain_4);

    flt_decay_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FDecaySlConfig(synth_data));
    addAndMakeVisible(*flt_decay_4);

    flt_attack_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FAttackSlConfig(synth_data));
    addAndMakeVisible(*flt_attack_4);

    flt_release_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FReleaseSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_release_3);

    flt_sustain_time_3 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FSustainTimeSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_sustain_time_3);

    flt_sustain_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FSustainSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_sustain_3);

    flt_decay_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FDecaySlConfig(synth_data, 2));
    addAndMakeVisible(*flt_decay_3);

    flt_attack_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FAttackSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_attack_3);

    flt_release_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FReleaseSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_release_2);

    flt_sustain_time_2 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FSustainTimeSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_sustain_time_2);

    flt_sustain_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FSustainSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_sustain_2);

    flt_decay_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FDecaySlConfig(synth_data, 1));
    addAndMakeVisible(*flt_decay_2);

    flt_attack_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FAttackSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_attack_2);

    flt_release_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FReleaseSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_release_1);

    flt_sustain_time_1 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FSustainTimeSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_sustain_time_1);

    flt_sustain_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FSustainSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_sustain_1);

    flt_decay_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FDecaySlConfig(synth_data, 0));
    addAndMakeVisible(*flt_decay_1);

    flt_attack_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FAttackSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_attack_1);

    filter_type_3_1 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_3_1);
    filter_type_3_1->setTooltip(TRANS("Set the filter type to BAND PASS."));
    filter_type_3_1->setButtonText(TRANS("BP"));
    filter_type_3_1->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_3_1->addListener(this);
    filter_type_3_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_3_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_3_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_3_2 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_3_2);
    filter_type_3_2->setTooltip(TRANS("Set the filter type to BAND PASS."));
    filter_type_3_2->setButtonText(TRANS("BP"));
    filter_type_3_2->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_3_2->addListener(this);
    filter_type_3_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_3_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_3_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_3_3 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_3_3);
    filter_type_3_3->setTooltip(TRANS("Set the filter type to BAND PASS."));
    filter_type_3_3->setButtonText(TRANS("BP"));
    filter_type_3_3->setConnectedEdges(juce::Button::ConnectedOnTop |
                                       juce::Button::ConnectedOnBottom);
    filter_type_3_3->addListener(this);
    filter_type_3_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_3_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_3_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_5_1 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_5_1);
    filter_type_5_1->setTooltip(TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_1->setButtonText(TRANS("PASS"));
    filter_type_5_1->setConnectedEdges(juce::Button::ConnectedOnTop);
    filter_type_5_1->addListener(this);
    filter_type_5_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_5_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_5_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_5_2 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_5_2);
    filter_type_5_2->setTooltip(TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_2->setButtonText(TRANS("PASS"));
    filter_type_5_2->setConnectedEdges(juce::Button::ConnectedOnTop);
    filter_type_5_2->addListener(this);
    filter_type_5_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_5_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_5_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_5_3 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_5_3);
    filter_type_5_3->setTooltip(TRANS("Set the filter type to PASS (not filtered)."));
    filter_type_5_3->setButtonText(TRANS("PASS"));
    filter_type_5_3->setConnectedEdges(juce::Button::ConnectedOnTop);
    filter_type_5_3->addListener(this);
    filter_type_5_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_5_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_5_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_2);
    button_sequence_2->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_2->addListener(this);
    button_sequence_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_3);
    button_sequence_3->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_3->addListener(this);
    button_sequence_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_4 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_4);
    button_sequence_4->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_4->addListener(this);
    button_sequence_4->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_4->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_4->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_5 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_5);
    button_sequence_5->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_5->addListener(this);
    button_sequence_5->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_5->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_5->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_6 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_6);
    button_sequence_6->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_6->addListener(this);
    button_sequence_6->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_6->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_6->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_7 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_7);
    button_sequence_7->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_7->addListener(this);
    button_sequence_7->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_7->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_7->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_8 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_8);
    button_sequence_8->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_8->addListener(this);
    button_sequence_8->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_8->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_8->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_9 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_9);
    button_sequence_9->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_9->addListener(this);
    button_sequence_9->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_9->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_9->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_10 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_10);
    button_sequence_10->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_10->addListener(this);
    button_sequence_10->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_10->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_10->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_11 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_11);
    button_sequence_11->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_11->addListener(this);
    button_sequence_11->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_11->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_11->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_12 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_12);
    button_sequence_12->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_12->addListener(this);
    button_sequence_12->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_12->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_12->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_13 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_13);
    button_sequence_13->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_13->addListener(this);
    button_sequence_13->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_13->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_13->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_14 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_14);
    button_sequence_14->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_14->addListener(this);
    button_sequence_14->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_14->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_14->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_15 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_15);
    button_sequence_15->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_15->addListener(this);
    button_sequence_15->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_15->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_15->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_sequence_16 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_16);
    button_sequence_16->setTooltip(TRANS("Turns this step on or off.\n"
                                         "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_16->addListener(this);
    button_sequence_16->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_16->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_16->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    combo_programm = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_programm);
    combo_programm->setTooltip(
        TRANS("Select and load a program of the selected bank (one box left)."));
    combo_programm->setEditableText(true);
    combo_programm->setJustificationType(juce::Justification::centredLeft);
    combo_programm->setTextWhenNothingSelected(TRANS("FROM SCRATCH"));
    combo_programm->setTextWhenNoChoicesAvailable(TRANS("EMPTY BANK"));
    combo_programm->addListener(this);

    button_programm_left = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_left);
    button_programm_left->setTooltip(TRANS("Load the previous program in the selected bank."));
    button_programm_left->setButtonText(TRANS("<"));
    button_programm_left->setConnectedEdges(juce::Button::ConnectedOnRight);
    button_programm_left->addListener(this);
    button_programm_left->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_left->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_programm_left->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_programm_right = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_right);
    button_programm_right->setTooltip(TRANS("Load the next program in the selected bank"));
    button_programm_right->setButtonText(TRANS(">"));
    button_programm_right->setConnectedEdges(juce::Button::ConnectedOnLeft);
    button_programm_right->addListener(this);
    button_programm_right->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_right->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_programm_right->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_open_oszi = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_open_oszi);
    button_open_oszi->setTooltip(TRANS("Open/Close the oscilloscope.\n"
                                       "\n"
                                       "Note: press ESC to close editors."));
    button_open_oszi->setButtonText(TRANS("OSCI"));
    button_open_oszi->addListener(this);
    button_open_oszi->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_open_oszi->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_open_oszi->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_vers_info = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_vers_info);
    button_vers_info->setTooltip(TRANS("Open/Close Build Info.\n"
                                       "\n"
                                       "Note: press ESC to close editors."));
    button_vers_info->setButtonText(TRANS("INFO"));
    button_vers_info->addListener(this);
    button_vers_info->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_vers_info->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_vers_info->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_open_midi_io_settings = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_open_midi_io_settings);

#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
    button_open_midi_io_settings->setTooltip(TRANS("Open/Close the MIDI settings.\n"
                                                   "\n"
                                                   "Note: press ESC to close editors."));
#else
    button_open_midi_io_settings->setTooltip(TRANS(
        "MIDI-Learn.\n"
        "Handling: Enable MIDI-Learn and select a slider or button on the main user interface. A "
        "little window pops up. Now you can move a slider on your MIDI controller (sender) to "
        "assign it to the element on the user interface (listener).\n\n"
        "Note: press ESC to close editors."
        "\n\n"
        "Shortcut: CTRL + m"));
#endif

    button_open_midi_io_settings->setButtonText(TRANS("MIDI"));
    button_open_midi_io_settings->addListener(this);
    button_open_midi_io_settings->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_open_midi_io_settings->setColour(juce::TextButton::textColourOnId,
                                            juce::Colour(0xffff3b00));
    button_open_midi_io_settings->setColour(juce::TextButton::textColourOffId,
                                            juce::Colours::yellow);

    combo_bank = std::make_unique<juce::ComboBox>(juce::String());
    addAndMakeVisible(*combo_bank);
    combo_bank->setTooltip(TRANS("Select the current program bank."));
    combo_bank->setEditableText(false);
    combo_bank->setJustificationType(juce::Justification::centredLeft);
    combo_bank->setTextWhenNothingSelected(juce::String());
    combo_bank->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    combo_bank->addListener(this);

    button_programm_load = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_load);
    button_programm_load->setTooltip(TRANS("Load the selected program."));
    button_programm_load->setButtonText(TRANS("LOAD"));
    button_programm_load->addListener(this);
    button_programm_load->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_load->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_programm_load->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    osc_1 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new OSCSlConfig(synth_data, 0));
    addAndMakeVisible(*osc_1);

    osc_2 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new OSCSlConfig(synth_data, 1));
    addAndMakeVisible(*osc_2);

    osc_3 = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new OSCSlConfig(synth_data, 2));
    addAndMakeVisible(*osc_3);

    flt_cutoff_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FCutoffSLConfig(synth_data, 0));
    addAndMakeVisible(*flt_cutoff_1);

    flt_cutoff_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FCutoffSLConfig(synth_data, 1));
    addAndMakeVisible(*flt_cutoff_2);

    flt_cutoff_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FCutoffSLConfig(synth_data, 2));
    addAndMakeVisible(*flt_cutoff_3);

    flt_distortion_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new GForceSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_distortion_1);

    flt_resonance_1 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FResonanceSLConfig(synth_data, 0));
    addAndMakeVisible(*flt_resonance_1);

    flt_resonance_2 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FResonanceSLConfig(synth_data, 1));
    addAndMakeVisible(*flt_resonance_2);

    flt_resonance_3 = std::make_unique<Monique_Ui_DualSlider>(
        ui_refresher, new FResonanceSLConfig(synth_data, 2));
    addAndMakeVisible(*flt_resonance_3);

    flt_volume_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FVolumeSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_volume_1);

    flt_volume_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FVolumeSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_volume_2);

    flt_volume_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FVolumeSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_volume_3);

    adsr_lfo_mix =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EnvLfoSlConfig(synth_data, 0));
    addAndMakeVisible(*adsr_lfo_mix);

    lfo_opt_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EnvLfoSlConfig(synth_data, 1));
    addAndMakeVisible(*lfo_opt_2);

    lfo_opt_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new EnvLfoSlConfig(synth_data, 2));
    addAndMakeVisible(*lfo_opt_3);

    button_sequence_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_sequence_1);
    button_sequence_1->setTooltip(TRANS("Turns this step on or off.\n"
                                        "(Has no effect if the arpeggiator (ARP) is turned off)"));
    button_sequence_1->addListener(this);
    button_sequence_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_sequence_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_sequence_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    flt_release_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FReleaseSlConfig(synth_data));
    addAndMakeVisible(*flt_release_4);

    volume = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new VolumeConfig(synth_data));
    addAndMakeVisible(*volume);

    flt_distortion_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new GForceSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_distortion_2);

    flt_distortion_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new GForceSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_distortion_3);

    button_arp_speed_XNORM = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_arp_speed_XNORM);
    button_arp_speed_XNORM->setTooltip(
        TRANS("Shortcut to set the speed multiplier back to 1x (in sync)."));
    button_arp_speed_XNORM->setButtonText(TRANS("x1"));
    button_arp_speed_XNORM->addListener(this);
    button_arp_speed_XNORM->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_arp_speed_XNORM->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_arp_speed_XNORM->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    flt_attack_5 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FMFreqSlConfig(synth_data));
    addAndMakeVisible(*flt_attack_5);

    flt_attack_6 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FMAmountSlConfig(synth_data));
    addAndMakeVisible(*flt_attack_6);

    osc_wave_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new WAVESlConfig(synth_data, 0));
    addAndMakeVisible(*osc_wave_1);

    osc_wave_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new WAVESlConfig(synth_data, 1));
    addAndMakeVisible(*osc_wave_2);

    button_programm_delete = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_delete);
    button_programm_delete->setTooltip(TRANS("Delete the selected program."));
    button_programm_delete->setButtonText(TRANS("DELETE"));
    button_programm_delete->addListener(this);
    button_programm_delete->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_delete->setColour(juce::TextButton::textColourOnId, juce::Colours::red);
    button_programm_delete->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffff7900));

    filter_type_6_1 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_6_1);
    filter_type_6_1->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_6_1->setButtonText(TRANS("LP"));
    filter_type_6_1->setConnectedEdges(juce::Button::ConnectedOnBottom);
    filter_type_6_1->addListener(this);
    filter_type_6_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_6_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_6_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_6_2 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_6_2);
    filter_type_6_2->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_6_2->setButtonText(TRANS("LP"));
    filter_type_6_2->setConnectedEdges(juce::Button::ConnectedOnBottom);
    filter_type_6_2->addListener(this);
    filter_type_6_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_6_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_6_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    filter_type_6_3 = std::make_unique<juce::TextButton>("VOICE 1");
    addAndMakeVisible(*filter_type_6_3);
    filter_type_6_3->setTooltip(TRANS("Set the filter type to LOW PASS."));
    filter_type_6_3->setButtonText(TRANS("LP"));
    filter_type_6_3->setConnectedEdges(juce::Button::ConnectedOnBottom);
    filter_type_6_3->addListener(this);
    filter_type_6_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    filter_type_6_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    filter_type_6_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_ctrl_toggle = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_ctrl_toggle);
    button_ctrl_toggle->setTooltip(
        TRANS("Turns the SHIFT mode on or off.\n"
              "\n"
              "The shift mode moves all back sliders to front and front sliders to back."));
    button_ctrl_toggle->setButtonText(TRANS("SHIFT"));
    button_ctrl_toggle->addListener(this);
    button_ctrl_toggle->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_ctrl_toggle->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_ctrl_toggle->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    speed = std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new BPMSlConfig(synth_data));
    addAndMakeVisible(*speed);

    button_open_morph = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_open_morph);
    button_open_morph->setTooltip(TRANS("Open/Close the morph editor.\n"
                                        "\n"
                                        "Note: press ESC to close editors."));
    button_open_morph->setButtonText(TRANS("EDIT"));
    button_open_morph->addListener(this);
    button_open_morph->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_open_morph->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_open_morph->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    effect_finalizer_switch = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*effect_finalizer_switch);
    effect_finalizer_switch->setTooltip(TRANS("Switch to the FX section."));
    effect_finalizer_switch->setButtonText(TRANS("FX"));
    effect_finalizer_switch->setConnectedEdges(juce::Button::ConnectedOnTop);
    effect_finalizer_switch->addListener(this);
    effect_finalizer_switch->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff11ff));
    effect_finalizer_switch->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    effect_finalizer_switch->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_values_toggle = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_values_toggle);
    button_values_toggle->setTooltip(TRANS(
        "Turns the CTRL mode on or off.\n"
        "\n"
        "In CTRL mode all values are visible\n"
        "\n"
        "Hold down CTRL/CMD on your keyboard and drag a slider to control it in velocity mode.\n"
        "\n"
        "Hold down CTRL/CMD on your keyboard and press + or - to resize the user interface. Press "
        "F11 to toggle fullscreen mode."));
    button_values_toggle->setButtonText(TRANS("CTRL"));
    button_values_toggle->addListener(this);
    button_values_toggle->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_values_toggle->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_values_toggle->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    octave_offset =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new OctaveOffsetSlConfig(synth_data));
    addAndMakeVisible(*octave_offset);

    label_filter_inputs = std::make_unique<juce::Label>(juce::String(), TRANS("FILTER INPUTS"));
    addAndMakeVisible(*label_filter_inputs);
    label_filter_inputs->setFont(juce::Font(30.00f, juce::Font::plain));
    label_filter_inputs->setJustificationType(juce::Justification::centred);
    label_filter_inputs->setEditable(false, false, false);
    label_filter_inputs->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_filter_inputs->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_filter_inputs->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_oscillators = std::make_unique<juce::Label>(juce::String(), TRANS("OSCs (O)"));
    addAndMakeVisible(*label_oscillators);
    label_oscillators->setFont(juce::Font(30.00f, juce::Font::plain));
    label_oscillators->setJustificationType(juce::Justification::centred);
    label_oscillators->setEditable(false, false, false);
    label_oscillators->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_oscillators->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_oscillators->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_filter_envelope = std::make_unique<juce::Label>(juce::String(), TRANS("FILTER ENVELOPE"));
    addAndMakeVisible(*label_filter_envelope);
    label_filter_envelope->setFont(juce::Font(30.00f, juce::Font::plain));
    label_filter_envelope->setJustificationType(juce::Justification::centred);
    label_filter_envelope->setEditable(false, false, false);
    label_filter_envelope->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_filter_envelope->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_filter_envelope->setColour(juce::TextEditor::backgroundColourId,
                                     juce::Colour(0x00000000));

    label_lfo = std::make_unique<juce::Label>(juce::String(), TRANS("LFO (L)"));
    addAndMakeVisible(*label_lfo);
    label_lfo->setFont(juce::Font(30.00f, juce::Font::plain));
    label_lfo->setJustificationType(juce::Justification::centred);
    label_lfo->setEditable(false, false, false);
    label_lfo->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_lfo->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_lfo->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_filter_config = std::make_unique<juce::Label>(juce::String(), TRANS("FILTER CONFIG"));
    addAndMakeVisible(*label_filter_config);
    label_filter_config->setFont(juce::Font(30.00f, juce::Font::plain));
    label_filter_config->setJustificationType(juce::Justification::centred);
    label_filter_config->setEditable(false, false, false);
    label_filter_config->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_filter_config->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_filter_config->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_filter_fx = std::make_unique<juce::Label>(juce::String(), TRANS("FILTER FX"));
    addAndMakeVisible(*label_filter_fx);
    label_filter_fx->setFont(juce::Font(30.00f, juce::Font::plain));
    label_filter_fx->setJustificationType(juce::Justification::centred);
    label_filter_fx->setEditable(false, false, false);
    label_filter_fx->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_filter_fx->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_filter_fx->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_out = std::make_unique<juce::Label>(juce::String(), TRANS("OUTPUT"));
    addAndMakeVisible(*label_out);
    label_out->setFont(juce::Font(30.00f, juce::Font::plain));
    label_out->setJustificationType(juce::Justification::centred);
    label_out->setEditable(false, false, false);
    label_out->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_out->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_out->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_amp_envelope = std::make_unique<juce::Label>(juce::String(), TRANS("AMP ENVELOPE"));
    addAndMakeVisible(*label_amp_envelope);
    label_amp_envelope->setFont(juce::Font(30.00f, juce::Font::plain));
    label_amp_envelope->setJustificationType(juce::Justification::centred);
    label_amp_envelope->setEditable(false, false, false);
    label_amp_envelope->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_amp_envelope->setColour(juce::TextEditor::textColourId, juce::Colour(0xff796660));
    label_amp_envelope->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_glide = std::make_unique<juce::Label>(juce::String(), TRANS("GLIDE"));
    addAndMakeVisible(*label_glide);
    label_glide->setFont(juce::Font(30.00f, juce::Font::plain));
    label_glide->setJustificationType(juce::Justification::centred);
    label_glide->setEditable(false, false, false);
    label_glide->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_glide->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_glide->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_speed = std::make_unique<juce::Label>(juce::String(), TRANS("SPEED"));
    addAndMakeVisible(*label_speed);
    label_speed->setFont(juce::Font(30.00f, juce::Font::plain));
    label_speed->setJustificationType(juce::Justification::centred);
    label_speed->setEditable(false, false, false);
    label_speed->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_speed->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_speed->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_tune = std::make_unique<juce::Label>(juce::String(), TRANS("TUNE"));
    addAndMakeVisible(*label_tune);
    label_tune->setFont(juce::Font(30.00f, juce::Font::plain));
    label_tune->setJustificationType(juce::Justification::centred);
    label_tune->setEditable(false, false, false);
    label_tune->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_tune->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_tune->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_fm = std::make_unique<juce::Label>(juce::String(), TRANS("FM (F)"));
    addAndMakeVisible(*label_fm);
    label_fm->setFont(juce::Font(30.00f, juce::Font::plain));
    label_fm->setJustificationType(juce::Justification::centred);
    label_fm->setEditable(false, false, false);
    label_fm->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_fm->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_fm->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    volume_master_meter = std::make_unique<Monique_Ui_SegmentedMeter>(ui_refresher);
    addAndMakeVisible(*volume_master_meter);

    button_open_config2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_open_config2);
    button_open_config2->setTooltip(TRANS("Open/Close the setup.\n"
                                          "\n"
                                          "Note: press ESC to close editors."));
    button_open_config2->setButtonText(TRANS("SETUP"));
    button_open_config2->addListener(this);
    button_open_config2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_open_config2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_open_config2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    label_mod_mix = std::make_unique<juce::Label>(juce::String(), TRANS("MOD MIX (X)"));
    addAndMakeVisible(*label_mod_mix);
    label_mod_mix->setFont(juce::Font(30.00f, juce::Font::plain));
    label_mod_mix->setJustificationType(juce::Justification::centred);
    label_mod_mix->setEditable(false, false, false);
    label_mod_mix->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_mod_mix->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_mod_mix->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    flt_pan_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FPanSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_pan_3);

    flt_pan_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FPanSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_pan_2);

    flt_pan_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FPanSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_pan_1);

    button_reset_arp_tune = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_reset_arp_tune);
    button_reset_arp_tune->setTooltip(
        TRANS("Resets the arpeggiator to the defined program note.\n"
              "(Triggers a note which is defineable by the note dial (back dial))"));
    button_reset_arp_tune->setButtonText(TRANS("RESET"));
    button_reset_arp_tune->addListener(this);
    button_reset_arp_tune->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_reset_arp_tune->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_reset_arp_tune->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_show_active_input_r_2_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_2_3);
    button_show_active_input_r_2_3->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_2_3->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_2_3->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_2_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_2_3);
    button_show_active_input_l_2_3->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_2_3->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_2_3->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_r_2_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_2_2);
    button_show_active_input_r_2_2->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_2_2->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_2_2->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_2_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_2_2);
    button_show_active_input_l_2_2->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_2_2->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_2_2->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_r_2_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_2_1);
    button_show_active_input_r_2_1->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_2_1->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_2_1->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_2_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_2_1);
    button_show_active_input_l_2_1->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_2_1->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_2_1->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_r_3_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_3_3);
    button_show_active_input_r_3_3->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_3_3->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_3_3->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_3_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_3_3);
    button_show_active_input_l_3_3->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_3_3->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_3_3->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_r_3_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_3_2);
    button_show_active_input_r_3_2->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_3_2->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_3_2->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_3_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_3_2);
    button_show_active_input_l_3_2->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_3_2->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_3_2->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_r_3_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_r_3_1);
    button_show_active_input_r_3_1->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_r_3_1->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_r_3_1->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_show_active_input_l_3_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_show_active_input_l_3_2);
    button_show_active_input_l_3_1->setColour(juce::TextButton::buttonColourId,
                                              juce::Colours::black);
    button_show_active_input_l_3_1->setColour(juce::TextButton::textColourOnId,
                                              juce::Colour(0xffff3b00));
    button_show_active_input_l_3_1->setColour(juce::TextButton::textColourOffId,
                                              juce::Colours::yellow);

    button_programm_rename = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_rename);
    button_programm_rename->setTooltip(TRANS("Rename the selected program."));
    button_programm_rename->setButtonText(TRANS("RENAME"));
    button_programm_rename->addListener(this);
    button_programm_rename->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_rename->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_programm_rename->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    flt_shape_1 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FShapeSlConfig(synth_data, 0));
    addAndMakeVisible(*flt_shape_1);

    flt_shape_2 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FShapeSlConfig(synth_data, 1));
    addAndMakeVisible(*flt_shape_2);

    flt_shape_3 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FShapeSlConfig(synth_data, 2));
    addAndMakeVisible(*flt_shape_3);

    button_programm_scratch = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_programm_scratch);
    button_programm_scratch->setTooltip(
        TRANS("Load the factory defaults to start from scratch.\n"
              "\n"
              "Push this button again to stop clearing the record buffer."));
    button_programm_scratch->setButtonText(TRANS("INIT"));
    button_programm_scratch->addListener(this);
    button_programm_scratch->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_programm_scratch->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffbcff00));
    button_programm_scratch->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffd0ff00));

    flt_shape_4 =
        std::make_unique<Monique_Ui_DualSlider>(ui_refresher, new FShapeSlConfig(synth_data));
    addAndMakeVisible(*flt_shape_4);

    label_reverb = std::make_unique<juce::Label>(juce::String(), TRANS("REVERB"));
    addAndMakeVisible(*label_reverb);
    label_reverb->setFont(juce::Font(30.00f, juce::Font::plain));
    label_reverb->setJustificationType(juce::Justification::centred);
    label_reverb->setEditable(false, false, false);
    label_reverb->setColour(juce::Label::textColourId, juce::Colour(0xff050505));
    label_reverb->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_reverb->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_open_playback = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_open_playback);
    button_open_playback->setTooltip(TRANS("Open/Close the Playback settings.\n"
                                           "\n"
                                           "Note: press ESC to close editors."));
    button_open_playback->setButtonText(TRANS("POLY >"));
    button_open_playback->addListener(this);
    button_open_playback->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_open_playback->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_open_playback->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_preset_agro = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_agro);
    button_preset_agro->setButtonText(TRANS("AGGRESSIVE"));
    button_preset_agro->setConnectedEdges(juce::Button::ConnectedOnTop);
    button_preset_agro->addListener(this);
    button_preset_agro->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_agro->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_agro->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_agro->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

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

    button_preset_down = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_down);
    button_preset_down->setButtonText(TRANS("DOWN"));
    button_preset_down->setConnectedEdges(juce::Button::ConnectedOnTop |
                                          juce::Button::ConnectedOnBottom);
    button_preset_down->addListener(this);
    button_preset_down->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_down->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_down->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_down->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

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

    button_preset_rising = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_rising);
    button_preset_rising->setButtonText(TRANS("RISING"));
    button_preset_rising->setConnectedEdges(juce::Button::ConnectedOnTop |
                                            juce::Button::ConnectedOnBottom);
    button_preset_rising->addListener(this);
    button_preset_rising->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_rising->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_rising->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_rising->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

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

    button_preset_soft = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_preset_soft);
    button_preset_soft->setButtonText(TRANS("SOFT"));
    button_preset_soft->setConnectedEdges(juce::Button::ConnectedOnBottom);
    button_preset_soft->addListener(this);
    button_preset_soft->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_preset_soft->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_preset_soft->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_preset_soft->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

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

    label_2 = std::make_unique<juce::Label>(juce::String(), TRANS("PRESETS"));
    addAndMakeVisible(*label_2);
    label_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_2->setJustificationType(juce::Justification::centredLeft);
    label_2->setEditable(false, false, false);
    label_2->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_24 = std::make_unique<juce::Label>(juce::String(), TRANS("PLAY ORDER"));
    addAndMakeVisible(*label_24);
    label_24->setFont(juce::Font(30.00f, juce::Font::plain));
    label_24->setJustificationType(juce::Justification::centredLeft);
    label_24->setEditable(false, false, false);
    label_24->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_24->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_24->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_23 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_23);
    label_23->setFont(juce::Font(30.00f, juce::Font::plain));
    label_23->setJustificationType(juce::Justification::centredLeft);
    label_23->setEditable(false, false, false);
    label_23->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_23->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_23->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_22 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_22);
    label_22->setFont(juce::Font(30.00f, juce::Font::plain));
    label_22->setJustificationType(juce::Justification::centredLeft);
    label_22->setEditable(false, false, false);
    label_22->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_22->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_22->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_21 = std::make_unique<juce::Label>(juce::String(), TRANS("MIN"));
    addAndMakeVisible(*label_21);
    label_21->setFont(juce::Font(30.00f, juce::Font::plain));
    label_21->setJustificationType(juce::Justification::centredLeft);
    label_21->setEditable(false, false, false);
    label_21->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_21->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_21->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

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

    button_flt_out_triggering_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_1);
    button_flt_out_triggering_1->setButtonText(TRANS("OUT 1"));
    button_flt_out_triggering_1->addListener(this);
    button_flt_out_triggering_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_1->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_1->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_out_triggering_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_2);
    button_flt_out_triggering_2->setButtonText(TRANS("OUT 2"));
    button_flt_out_triggering_2->addListener(this);
    button_flt_out_triggering_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_2->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xffff1111));
    button_flt_out_triggering_2->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_2->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_out_triggering_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_out_triggering_3);
    button_flt_out_triggering_3->setButtonText(TRANS("OUT 3"));
    button_flt_out_triggering_3->addListener(this);
    button_flt_out_triggering_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_out_triggering_3->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xffff1111));
    button_flt_out_triggering_3->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_out_triggering_3->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    label_13 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_13);
    label_13->setFont(juce::Font(30.00f, juce::Font::plain));
    label_13->setJustificationType(juce::Justification::centredLeft);
    label_13->setEditable(false, false, false);
    label_13->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_13->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_13->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_7 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_7);
    label_7->setFont(juce::Font(30.00f, juce::Font::plain));
    label_7->setJustificationType(juce::Justification::centredLeft);
    label_7->setEditable(false, false, false);
    label_7->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_flt_env_triggering_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_3);
    button_flt_env_triggering_3->setButtonText(TRANS("ENV 3"));
    button_flt_env_triggering_3->addListener(this);
    button_flt_env_triggering_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_3->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_3->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_3->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

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

    button_flt_input_triggering_3_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_3_1);
    button_flt_input_triggering_3_1->setButtonText(TRANS("I-ENV 1"));
    button_flt_input_triggering_3_1->addListener(this);
    button_flt_input_triggering_3_1->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_3_1->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_3_1->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_3_1->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_osc_tracking_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_3);
    button_osc_tracking_3->setButtonText(TRANS("OSC 3"));
    button_osc_tracking_3->addListener(this);
    button_osc_tracking_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_3->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_cutoff_tracking_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_3);
    button_cutoff_tracking_3->setButtonText(TRANS("CUT 3"));
    button_cutoff_tracking_3->addListener(this);
    button_cutoff_tracking_3->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_3->setColour(juce::TextButton::buttonOnColourId,
                                        juce::Colour(0xffff1111));
    button_cutoff_tracking_3->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_3->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    label_12 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_12);
    label_12->setFont(juce::Font(30.00f, juce::Font::plain));
    label_12->setJustificationType(juce::Justification::centredLeft);
    label_12->setEditable(false, false, false);
    label_12->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_6 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_6);
    label_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_6->setJustificationType(juce::Justification::centredLeft);
    label_6->setEditable(false, false, false);
    label_6->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_flt_input_triggering_1_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_1_1);
    button_flt_input_triggering_1_1->setButtonText(TRANS("I-ENV 1"));
    button_flt_input_triggering_1_1->addListener(this);
    button_flt_input_triggering_1_1->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_1_1->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_1_1->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_1_1->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_2_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_2_1);
    button_flt_input_triggering_2_1->setButtonText(TRANS("I-ENV 1"));
    button_flt_input_triggering_2_1->addListener(this);
    button_flt_input_triggering_2_1->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_2_1->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_2_1->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_2_1->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_env_triggering_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_1);
    button_flt_env_triggering_1->setButtonText(TRANS("ENV 1"));
    button_flt_env_triggering_1->addListener(this);
    button_flt_env_triggering_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_1->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_1->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_1->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

    button_flt_env_triggering_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_env_triggering_2);
    button_flt_env_triggering_2->setButtonText(TRANS("ENV 2"));
    button_flt_env_triggering_2->addListener(this);
    button_flt_env_triggering_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_flt_env_triggering_2->setColour(juce::TextButton::buttonOnColourId,
                                           juce::Colour(0xff4444ff));
    button_flt_env_triggering_2->setColour(juce::TextButton::textColourOnId,
                                           juce::Colour(0xffff3b00));
    button_flt_env_triggering_2->setColour(juce::TextButton::textColourOffId,
                                           juce::Colours::yellow);

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

    button_osc_tracking_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_2);
    button_osc_tracking_2->setButtonText(TRANS("OSC 2"));
    button_osc_tracking_2->addListener(this);
    button_osc_tracking_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_2->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    button_cutoff_tracking_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_2);
    button_cutoff_tracking_2->setButtonText(TRANS("CUT 2"));
    button_cutoff_tracking_2->addListener(this);
    button_cutoff_tracking_2->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_2->setColour(juce::TextButton::buttonOnColourId,
                                        juce::Colour(0xffff1111));
    button_cutoff_tracking_2->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_2->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    label_5 = std::make_unique<juce::Label>(juce::String(), TRANS("OCT"));
    addAndMakeVisible(*label_5);
    label_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_5->setJustificationType(juce::Justification::centredLeft);
    label_5->setEditable(false, false, false);
    label_5->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_cutoff_tracking_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_cutoff_tracking_1);
    button_cutoff_tracking_1->setButtonText(TRANS("CUT 1"));
    button_cutoff_tracking_1->addListener(this);
    button_cutoff_tracking_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_cutoff_tracking_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_cutoff_tracking_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

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

    button_osc_tracking_1 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_osc_tracking_1);
    button_osc_tracking_1->setButtonText(TRANS("OSC 1"));
    button_osc_tracking_1->addListener(this);
    button_osc_tracking_1->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_osc_tracking_1->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff1111));
    button_osc_tracking_1->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_osc_tracking_1->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    label_oscillators2 =
        std::make_unique<juce::Label>(juce::String(), TRANS("KEY TRACK AND TRIGGERING"));
    addAndMakeVisible(*label_oscillators2);
    label_oscillators2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_oscillators2->setJustificationType(juce::Justification::centred);
    label_oscillators2->setEditable(false, false, false);
    label_oscillators2->setColour(juce::Label::textColourId, juce::Colour(0xff1111ff));
    label_oscillators2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_oscillators2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_sub_poly = std::make_unique<juce::Label>("DL", TRANS("SUB POLYPHONY"));
    addAndMakeVisible(*label_sub_poly);
    label_sub_poly->setFont(juce::Font(250.00f, juce::Font::plain));
    label_sub_poly->setJustificationType(juce::Justification::centred);
    label_sub_poly->setEditable(false, false, false);
    label_sub_poly->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_sub_poly->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_sub_poly->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_1 =
        std::make_unique<juce::Label>(juce::String(), TRANS("KEY TRACKING AND TRIGGERING\n"));
    addAndMakeVisible(*label_poly_desc_1);
    label_poly_desc_1->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_1->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_1->setEditable(false, false, false);
    label_poly_desc_1->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_1->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_1->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_2 = std::make_unique<juce::Label>(
        juce::String(), TRANS("OSC 1 to 3: re-tunes the corresponding"));
    addAndMakeVisible(*label_poly_desc_2);
    label_poly_desc_2->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_2->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_2->setEditable(false, false, false);
    label_poly_desc_2->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_2->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_3 =
        std::make_unique<juce::Label>(juce::String(), TRANS("oscillator to the key number down."));
    addAndMakeVisible(*label_poly_desc_3);
    label_poly_desc_3->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_3->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_3->setEditable(false, false, false);
    label_poly_desc_3->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_3->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_4 = std::make_unique<juce::Label>(
        juce::String(), TRANS("CUT 1 to 3: adjusts the corresponding"));
    addAndMakeVisible(*label_poly_desc_4);
    label_poly_desc_4->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_4->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_4->setEditable(false, false, false);
    label_poly_desc_4->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_4->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_5 =
        std::make_unique<juce::Label>(juce::String(), TRANS("filter cutoff frequency to the key"));
    addAndMakeVisible(*label_poly_desc_5);
    label_poly_desc_5->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_5->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_5->setEditable(false, false, false);
    label_poly_desc_5->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_5->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_6 = std::make_unique<juce::Label>(juce::String(), TRANS("number down."));
    addAndMakeVisible(*label_poly_desc_6);
    label_poly_desc_6->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_6->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_6->setEditable(false, false, false);
    label_poly_desc_6->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_6->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_7 =
        std::make_unique<juce::Label>(juce::String(), TRANS("I-ENV 1 to 3: triggers the "));
    addAndMakeVisible(*label_poly_desc_7);
    label_poly_desc_7->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_7->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_7->setEditable(false, false, false);
    label_poly_desc_7->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_7->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_8 = std::make_unique<juce::Label>(
        juce::String(), TRANS("corresponding filter input envelope by"));
    addAndMakeVisible(*label_poly_desc_8);
    label_poly_desc_8->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_8->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_8->setEditable(false, false, false);
    label_poly_desc_8->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_8->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_8->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_9 =
        std::make_unique<juce::Label>(juce::String(), TRANS("the key number down."));
    addAndMakeVisible(*label_poly_desc_9);
    label_poly_desc_9->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_9->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_9->setEditable(false, false, false);
    label_poly_desc_9->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_9->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_9->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_10 = std::make_unique<juce::Label>(
        juce::String(), TRANS("ENV 1 to 3: triggers the corresponding"));
    addAndMakeVisible(*label_poly_desc_10);
    label_poly_desc_10->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_10->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_10->setEditable(false, false, false);
    label_poly_desc_10->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_10->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_10->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_11 =
        std::make_unique<juce::Label>(juce::String(), TRANS("filter envelope by the key number"));
    addAndMakeVisible(*label_poly_desc_11);
    label_poly_desc_11->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_11->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_11->setEditable(false, false, false);
    label_poly_desc_11->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_11->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_11->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_12 = std::make_unique<juce::Label>(juce::String(), TRANS("down."));
    addAndMakeVisible(*label_poly_desc_12);
    label_poly_desc_12->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_12->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_12->setEditable(false, false, false);
    label_poly_desc_12->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_12->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_13 = std::make_unique<juce::Label>(
        juce::String(), TRANS("OUT 1 to 3: triggers a hidden envelope"));
    addAndMakeVisible(*label_poly_desc_13);
    label_poly_desc_13->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_13->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_13->setEditable(false, false, false);
    label_poly_desc_13->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_13->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_13->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_14 =
        std::make_unique<juce::Label>(juce::String(), TRANS("which controls the corresponding"));
    addAndMakeVisible(*label_poly_desc_14);
    label_poly_desc_14->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_14->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_14->setEditable(false, false, false);
    label_poly_desc_14->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_14->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_14->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_15 = std::make_unique<juce::Label>(
        juce::String(), TRANS("filter output level by the key number"));
    addAndMakeVisible(*label_poly_desc_15);
    label_poly_desc_15->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_15->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_15->setEditable(false, false, false);
    label_poly_desc_15->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_15->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_15->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_poly_desc_16 = std::make_unique<juce::Label>(juce::String(), TRANS("down."));
    addAndMakeVisible(*label_poly_desc_16);
    label_poly_desc_16->setFont(juce::Font(30.00f, juce::Font::plain));
    label_poly_desc_16->setJustificationType(juce::Justification::centredLeft);
    label_poly_desc_16->setEditable(false, false, false);
    label_poly_desc_16->setColour(juce::Label::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_16->setColour(juce::TextEditor::textColourId, juce::Colour(0xffff3b00));
    label_poly_desc_16->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_flt_input_triggering_1_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_1_2);
    button_flt_input_triggering_1_2->setButtonText(TRANS("I-ENV 2"));
    button_flt_input_triggering_1_2->addListener(this);
    button_flt_input_triggering_1_2->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_1_2->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_1_2->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_1_2->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_1_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_1_3);
    button_flt_input_triggering_1_3->setButtonText(TRANS("I-ENV 3"));
    button_flt_input_triggering_1_3->addListener(this);
    button_flt_input_triggering_1_3->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_1_3->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_1_3->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_1_3->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_2_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_2_2);
    button_flt_input_triggering_2_2->setButtonText(TRANS("I-ENV 2"));
    button_flt_input_triggering_2_2->addListener(this);
    button_flt_input_triggering_2_2->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_2_2->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_2_2->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_2_2->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_2_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_2_3);
    button_flt_input_triggering_2_3->setButtonText(TRANS("I-ENV 3"));
    button_flt_input_triggering_2_3->addListener(this);
    button_flt_input_triggering_2_3->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_2_3->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_2_3->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_2_3->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_3_2 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_3_2);
    button_flt_input_triggering_3_2->setButtonText(TRANS("I-ENV 2"));
    button_flt_input_triggering_3_2->addListener(this);
    button_flt_input_triggering_3_2->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_3_2->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_3_2->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_3_2->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

    button_flt_input_triggering_3_3 = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*button_flt_input_triggering_3_3);
    button_flt_input_triggering_3_3->setButtonText(TRANS("I-ENV 3"));
    button_flt_input_triggering_3_3->addListener(this);
    button_flt_input_triggering_3_3->setColour(juce::TextButton::buttonColourId,
                                               juce::Colours::black);
    button_flt_input_triggering_3_3->setColour(juce::TextButton::buttonOnColourId,
                                               juce::Colour(0xff4444ff));
    button_flt_input_triggering_3_3->setColour(juce::TextButton::textColourOnId,
                                               juce::Colour(0xffff3b00));
    button_flt_input_triggering_3_3->setColour(juce::TextButton::textColourOffId,
                                               juce::Colours::yellow);

#ifndef POLY
    button_open_playback->setVisible(false);
#endif
    overlay->setVisible(false);
    credits = std::make_unique<monique_ui_Credits>(ui_refresher_);
    addAndMakeVisible(*credits);
    credits->setVisible(false);

#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
    button_open_midi_io_settings->setTooltip(
        "Enables the MIDI train/learn mode.\n"
        "\n"
        "Handling: enable MIDI train and select a slider or button on the main user interface. A "
        "little window pops up. Now you can move a slider on your MIDI controller (sender) to "
        "assign it to the element on the user interface (listener).\n"
        "\n"
        "Shortcut: CTRL + m");
    button_open_midi_io_settings->setButtonText(TRANS("MAP"));
#endif

    last_bank = -1;
    last_programm = -1;
    is_in_help_mode = false;

    voice = audio_processor->voice;
    synth_data->shift = false;

    last_shuffle = -1;
    sequence_buttons_original_order.add(button_sequence_1.get());
    sequence_buttons_original_order.add(button_sequence_2.get());
    sequence_buttons_original_order.add(button_sequence_3.get());
    sequence_buttons_original_order.add(button_sequence_4.get());
    sequence_buttons_original_order.add(button_sequence_5.get());
    sequence_buttons_original_order.add(button_sequence_6.get());
    sequence_buttons_original_order.add(button_sequence_7.get());
    sequence_buttons_original_order.add(button_sequence_8.get());
    sequence_buttons_original_order.add(button_sequence_9.get());
    sequence_buttons_original_order.add(button_sequence_10.get());
    sequence_buttons_original_order.add(button_sequence_11.get());
    sequence_buttons_original_order.add(button_sequence_12.get());
    sequence_buttons_original_order.add(button_sequence_13.get());
    sequence_buttons_original_order.add(button_sequence_14.get());
    sequence_buttons_original_order.add(button_sequence_15.get());
    sequence_buttons_original_order.add(button_sequence_16.get());
    sequence_buttons.addArray(sequence_buttons_original_order);
    sequence_sliders.add(arp_step_1.get());
    sequence_sliders.add(arp_step_2.get());
    sequence_sliders.add(arp_step_3.get());
    sequence_sliders.add(arp_step_4.get());
    sequence_sliders.add(arp_step_5.get());
    sequence_sliders.add(arp_step_6.get());
    sequence_sliders.add(arp_step_7.get());
    sequence_sliders.add(arp_step_8.get());
    sequence_sliders.add(arp_step_9.get());
    sequence_sliders.add(arp_step_10.get());
    sequence_sliders.add(arp_step_11.get());
    sequence_sliders.add(arp_step_12.get());
    sequence_sliders.add(arp_step_13.get());
    sequence_sliders.add(arp_step_14.get());
    sequence_sliders.add(arp_step_15.get());
    sequence_sliders.add(arp_step_16.get());

    this->setLookAndFeel(audio_processor->ui_look_and_feel.get());
    // OPAQUE
    {
        for (int i = 0; i != getNumChildComponents(); ++i)
        {
            juce::Component *comp(getChildComponent(i));
            comp->setOpaque(true);
            // comp->setLookAndFeel( audio_processor->ui_look_and_feel );
            this->setRepaintsOnMouseActivity(false);

            if (Monique_Ui_DualSlider *slider = dynamic_cast<Monique_Ui_DualSlider *>(comp))
            {
                dual_sliders.add(slider);
                slider->setOpaque(false); // controlled by the slider itself
            }
            else if (juce::Label *label = dynamic_cast<juce::Label *>(comp))
            {
                label->setInterceptsMouseClicks(false, false);
            }
        }

        overlay->setOpaque(false);
        credits->setOpaque(false);
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
        label_sub_poly->setOpaque(false);
        label_glide->setOpaque(false);

        pop_credits->setOpaque(false);

        label_poly_desc_1->setOpaque(false);
        label_poly_desc_2->setOpaque(false);
        label_poly_desc_3->setOpaque(false);
        label_poly_desc_4->setOpaque(false);
        label_poly_desc_5->setOpaque(false);
        label_poly_desc_6->setOpaque(false);
        label_poly_desc_7->setOpaque(false);
        label_poly_desc_8->setOpaque(false);
        label_poly_desc_9->setOpaque(false);
        label_poly_desc_10->setOpaque(false);
        label_poly_desc_11->setOpaque(false);
        label_poly_desc_12->setOpaque(false);
        label_poly_desc_13->setOpaque(false);
        label_poly_desc_14->setOpaque(false);
        label_poly_desc_15->setOpaque(false);
        label_poly_desc_16->setOpaque(false);

        this->setOpaque(true);
    }

    // THEMES
    {
        for (int i = 0; i != sequence_buttons.size(); ++i)
        {
            sequence_buttons.getUnchecked(i)->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                                  COLOUR_THEMES::ARP_THEME);
        }

        label_oscillators->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_fm->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FM_THEME);

        label_glide->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_arpeggiator->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_speed->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_tune->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_filter_inputs->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_filter_envelope->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_mod_mix->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_lfo->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_filter_config->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        label_filter_fx->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);

        label_monique->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
#ifndef JUCE_DEBUG
        label_monique->getProperties().set(VAR_INDEX_COLOUR_THEME_INVERTED, true);
#endif

        label_out->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);
        button_reset_arp_tune->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::ARP_THEME);
        button_arp_speed_XNORM->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                    COLOUR_THEMES::ARP_THEME);

        button_values_toggle->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_ctrl_toggle->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_programm_left->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        combo_bank->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        combo_programm->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_programm_right->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::TOP_THEME);
        button_programm_load->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_programm_scratch->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::TOP_THEME);
        button_programm_replace->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::TOP_THEME);
        button_programm_new->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_programm_rename->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                    COLOUR_THEMES::TOP_THEME);
        button_programm_delete->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                    COLOUR_THEMES::TOP_THEME);
        button_open_midi_io_settings->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::TOP_THEME);
        button_open_config2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_open_oszi->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_vers_info->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);
        button_open_playback->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::TOP_THEME);

        button_edit_input_env_1_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_1_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_1_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_2_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_2_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_2_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_3_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_3_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_3_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::FILTER_THEME);
        button_edit_input_env_band_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_4->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_5->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_6->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_edit_input_env_band_7->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::FX_THEME);
        button_reset_arp_tune->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::ARP_THEME);
        button_edit_mfo_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_edit_mfo_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_edit_mfo_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_edit_mfo_4->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_show_active_input_l_2_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_2_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_l_2_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_2_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_l_2_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_2_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_l_3_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_3_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_l_3_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_3_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_l_3_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        button_show_active_input_r_3_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                            COLOUR_THEMES::FILTER_THEME);
        filter_type_6_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_2_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_3_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_5_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_6_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_2_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_3_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_5_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_6_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_2_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_3_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_5_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        filter_type_bg_button_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::FILTER_THEME);
        filter_type_bg_button_1->setEnabled(false);
        filter_type_bg_button_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::FILTER_THEME);
        filter_type_bg_button_2->setEnabled(false);
        filter_type_bg_button_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::FILTER_THEME);
        filter_type_bg_button_3->setEnabled(false);

        label_morph->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_open_morph->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME);
        button_toggle_morph_buttons_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                           COLOUR_THEMES::MORPH_THEME);
        button_toggle_morph_buttons_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                           COLOUR_THEMES::MORPH_THEME);
        button_toggle_morph_buttons_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                           COLOUR_THEMES::MORPH_THEME);
        button_toggle_morph_buttons_4->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                           COLOUR_THEMES::MORPH_THEME);

        button_edit_lfo_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        button_edit_lfo_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        button_edit_lfo_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        label_lfo_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        label_lfo_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);
        label_lfo_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME);

        label_amp_envelope->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_fx_delay->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_reverb->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_4->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_5->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_6->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);
        label_band_hz_7->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME);

        effect_finalizer_switch2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                      COLOUR_THEMES::FX_THEME);
        effect_finalizer_switch->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::FX_THEME);

        label_oscillators2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME);

        slider_cutoff_tracking_oct_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::ARP_THEME);
        slider_cutoff_tracking_oct_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::ARP_THEME);
        slider_cutoff_tracking_oct_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                          COLOUR_THEMES::ARP_THEME);
        slider_osc_tracking_oct_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::ARP_THEME);
        slider_osc_tracking_oct_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::ARP_THEME);
        button_cutoff_tracking_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                      COLOUR_THEMES::ARP_THEME);
        button_cutoff_tracking_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                      COLOUR_THEMES::ARP_THEME);
        button_cutoff_tracking_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                      COLOUR_THEMES::ARP_THEME);
        button_osc_tracking_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::ARP_THEME);
        button_osc_tracking_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::ARP_THEME);
        button_osc_tracking_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                   COLOUR_THEMES::ARP_THEME);
        label_5->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_6->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_7->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_12->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_13->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        button_flt_env_triggering_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        button_flt_env_triggering_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        button_flt_env_triggering_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_1_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_1_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_1_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_2_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_2_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_2_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_3_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_3_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_flt_input_triggering_3_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                             COLOUR_THEMES::ARP_THEME);
        button_tracking_mode_hm->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::ARP_THEME);
        button_tracking_mode_keep->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                       COLOUR_THEMES::ARP_THEME);
        button_tracking_mode_hf->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::ARP_THEME);
        button_tracking_mode_lf->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::ARP_THEME);
        label_24->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        button_flt_out_triggering_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        button_flt_out_triggering_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        button_flt_out_triggering_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        slider_flt_out_sesitivity_1->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        slider_flt_out_sesitivity_2->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        slider_flt_out_sesitivity_3->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                         COLOUR_THEMES::ARP_THEME);
        label_21->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_22->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_23->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);

        button_preset_agro->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        button_preset_down->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        button_preset_rising->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        button_preset_soft->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);

        filter_type_bg_button_5->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::ARP_THEME);
        filter_type_bg_button_4->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                     COLOUR_THEMES::ARP_THEME);

        label_sub_poly->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_1->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_2->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_3->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_4->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_5->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_6->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_7->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_8->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_9->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_10->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_11->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_12->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_13->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_14->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_15->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
        label_poly_desc_16->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
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
    // update_slider_return_values();

    /*

    setSize (1760, 1210);

    */

    if (!is_mobile())
    {
        resizeLimits.setFixedAspectRatio(original_w / original_h);
        resizer = std::make_unique<juce::ResizableCornerComponent>(this, &resizeLimits);
        addAndMakeVisible(*resizer);
    }

#ifdef IS_MOBILE
    button_open_playback->setButtonText("ZOOM");
#endif
    // resizer->setTooltip("Global shortcut: CTRL + or CTRL -");

    // setVisible(true);
    audio_processor->set_peak_meter(volume_master_meter.get());

    keyboard->setLowestVisibleKey(24);
    keyboard->setAvailableRange(12, 60 + 24);

    global_slider_settings_changed(this);
    update_slider_return_values();

    update_size();

    if (synth_data->is_osci_open.get_value())
    {
        buttonClicked(button_open_oszi.get());
    }

    show_current_voice_data();

    ui_refresher->startTimer(UI_REFRESH_RATE);

    delay4->get_top_button()->main_window = this;
}

Monique_Ui_Mainwindow::~Monique_Ui_Mainwindow()
{
    // synth_data->midi_lfo_popup.remove_listener(this);
    // synth_data->midi_env_popup.remove_listener(this);

    juce::PopupMenu::dismissAllActiveMenus();

    button_flasher = nullptr;
    clear_record_timer = nullptr;

    look_and_feel->mainwindow = nullptr;
    ui_refresher->editor = nullptr;
    if (amp_painter)
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
    vers_info = nullptr;
    popup = nullptr;
    playback = nullptr;

    audio_processor->clear_peak_meter();

    filter_type_bg_button_5 = nullptr;
    filter_type_bg_button_4 = nullptr;
    overlay = nullptr;
    label_monique = nullptr;
    pop_credits = nullptr;
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
    button_vers_info = nullptr;
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
    label_reverb = nullptr;
    button_open_playback = nullptr;
    button_preset_agro = nullptr;
    button_tracking_mode_hm = nullptr;
    button_preset_down = nullptr;
    button_tracking_mode_lf = nullptr;
    button_preset_rising = nullptr;
    button_tracking_mode_hf = nullptr;
    button_preset_soft = nullptr;
    button_tracking_mode_keep = nullptr;
    label_2 = nullptr;
    label_24 = nullptr;
    label_23 = nullptr;
    label_22 = nullptr;
    label_21 = nullptr;
    slider_flt_out_sesitivity_3 = nullptr;
    slider_flt_out_sesitivity_2 = nullptr;
    slider_flt_out_sesitivity_1 = nullptr;
    button_flt_out_triggering_1 = nullptr;
    button_flt_out_triggering_2 = nullptr;
    button_flt_out_triggering_3 = nullptr;
    label_13 = nullptr;
    label_7 = nullptr;
    button_flt_env_triggering_3 = nullptr;
    slider_osc_tracking_oct_3 = nullptr;
    slider_cutoff_tracking_oct_3 = nullptr;
    button_flt_input_triggering_3_1 = nullptr;
    button_osc_tracking_3 = nullptr;
    button_cutoff_tracking_3 = nullptr;
    label_12 = nullptr;
    label_6 = nullptr;
    button_flt_input_triggering_1_1 = nullptr;
    button_flt_input_triggering_2_1 = nullptr;
    button_flt_env_triggering_1 = nullptr;
    button_flt_env_triggering_2 = nullptr;
    slider_osc_tracking_oct_2 = nullptr;
    slider_cutoff_tracking_oct_2 = nullptr;
    button_osc_tracking_2 = nullptr;
    button_cutoff_tracking_2 = nullptr;
    label_5 = nullptr;
    button_cutoff_tracking_1 = nullptr;
    slider_cutoff_tracking_oct_1 = nullptr;
    button_osc_tracking_1 = nullptr;
    label_oscillators2 = nullptr;
    label_sub_poly = nullptr;
    label_poly_desc_1 = nullptr;
    label_poly_desc_2 = nullptr;
    label_poly_desc_3 = nullptr;
    label_poly_desc_4 = nullptr;
    label_poly_desc_5 = nullptr;
    label_poly_desc_6 = nullptr;
    label_poly_desc_7 = nullptr;
    label_poly_desc_8 = nullptr;
    label_poly_desc_9 = nullptr;
    label_poly_desc_10 = nullptr;
    label_poly_desc_11 = nullptr;
    label_poly_desc_12 = nullptr;
    label_poly_desc_13 = nullptr;
    label_poly_desc_14 = nullptr;
    label_poly_desc_15 = nullptr;
    label_poly_desc_16 = nullptr;
    button_flt_input_triggering_1_2 = nullptr;
    button_flt_input_triggering_1_3 = nullptr;
    button_flt_input_triggering_2_2 = nullptr;
    button_flt_input_triggering_2_3 = nullptr;
    button_flt_input_triggering_3_2 = nullptr;
    button_flt_input_triggering_3_3 = nullptr;

#ifdef JUCE_OPENGL
    openGLContext.detach();
#endif
}

//==============================================================================
void Monique_Ui_Mainwindow::paint(juce::Graphics &g)
{

    // g.fillAll (Colour (0xff161616));
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.fillAll(Colour(0xff050505));

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, 620.0f, 280.0f, 180.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 985.0f, 95.0f, Colour(0x301111ff), 975.0f,
                                     95.0f, false));
    g.fillRect(950, 100, 50, 130);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 170.0f, 80.0f, Colour(0x30ffff11), 170.0f,
                                     90.0f, false));
    g.drawRoundedRectangle(994.0f, 70.0f, 454.0f, 55.0f, 10.000f, 2.000f);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 170.0f, 80.0f, Colour(0x30ffff11), 170.0f,
                                     90.0f, false));
    g.drawRoundedRectangle(412.0f, 70.0f, 556.0f, 103.0f, 10.000f, 2.000f);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 170.0f, 80.0f, Colour(0x30ffff11), 170.0f,
                                     90.0f, false));
    g.drawRoundedRectangle(192.0f, 70.0f, 196.0f, 55.0f, 10.000f, 2.000f);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 170.0f, 80.0f, Colour(0x30ffff11), 170.0f,
                                     90.0f, false));
    g.drawRoundedRectangle(12.0f, 70.0f, 156.0f, 55.0f, 10.000f, 2.000f);

    g.setColour(Colour(0xffff11ff));
    g.fillRoundedRectangle(810.0f, 650.0f, 550.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1200.0f, 850.0f, 160.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(180.0f, 850.0f, 1020.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xffff11ff));
    g.fillRect(810, 770, 41, 30);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 800.0f, 170.0f, Colour(0x301111ff), 850.0f,
                                     170.0f, false));
    g.fillRect(790, 100, 110, 130);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 800.0f, 170.0f, Colour(0x301111ff), 850.0f,
                                     170.0f, false));
    g.fillRect(790, 280, 110, 130);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 985.0f, 285.0f, Colour(0x301111ff), 975.0f,
                                     285.0f, false));
    g.fillRect(960, 280, 60, 130);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 985.0f, 460.0f, Colour(0x301111ff), 975.0f,
                                     460.0f, false));
    g.fillRect(950, 460, 60, 130);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 800.0f, 170.0f, Colour(0x301111ff), 850.0f,
                                     170.0f, false));
    g.fillRect(790, 460, 110, 130);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(920, 510, 140, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(920, 150, 140, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(920, 330, 140, 30);

    g.setColour(Colour(0xff11ffff));
    g.fillRoundedRectangle(10.0f, 90.0f, 160.0f, 710.0f, 10.000f);

    g.setColour(Colour(0xffff1111));
    g.fillRoundedRectangle(20.0f, 90.0f, 150.0f, 520.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(190.0f, 90.0f, 200.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(190.0f, 250.0f, 200.0f, 170.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(190.0f, 430.0f, 200.0f, 170.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(410.0f, 90.0f, 390.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(410.0f, 270.0f, 390.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(410.0f, 450.0f, 390.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(380, 99, 58, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(380, 279, 58, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRect(380, 459, 58, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(890.0f, 90.0f, 80.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(890.0f, 270.0f, 80.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(890.0f, 450.0f, 80.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff050505));
    g.fillEllipse(797.0f, 114.0f, 100.0f, 100.0f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(990.0f, 90.0f, 460.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(990.0f, 270.0f, 460.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillEllipse(802.0f, 119.0f, 90.0f, 90.0f);

    g.setColour(Colour(0xff050505));
    g.fillEllipse(797.0f, 294.0f, 100.0f, 100.0f);

    g.setColour(Colour(0xff1111ff));
    g.fillEllipse(802.0f, 299.0f, 90.0f, 90.0f);

    g.setColour(Colour(0xff050505));
    g.fillEllipse(797.0f, 474.0f, 100.0f, 100.0f);

    g.setColour(Colour(0xff1111ff));
    g.fillEllipse(802.0f, 479.0f, 90.0f, 90.0f);

    g.setColour(Colour(0xffffffee));
    g.fillRoundedRectangle(190.0f, 620.0f, 260.0f, 210.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(1255.0f, 220.0f, 10.0f, 40.0f, 5.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(1255.0f, 400.0f, 10.0f, 40.0f, 5.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(10.0f, 850.0f, 170.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff11ff11));
    g.fillRoundedRectangle(1370.0f, 650.0f, 80.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(200.0f, 430.0f, 1064.0f, 10.0f, 5.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(200.0f, 250.0f, 1064.0f, 10.0f, 5.000f);

    g.setGradientFill(ColourGradient(Colour(0xffff11ff), 820.0f, 640.0f, Colour(0xff1111ff),
                                     1350.0f, 640.0f, false));
    g.fillRoundedRectangle(470.0f, 620.0f, 950.0f, 20.0f, 5.000f);

    g.setColour(Colour(0xffff11ff));
    g.fillRect(780, 710, 41, 30);

    g.setGradientFill(ColourGradient(Colour(0xffff11ff), 1660.0f, 750.0f, Colour(0xff11ff11),
                                     1670.0f, 750.0f, false));
    g.fillRect(1340, 710, 40, 30);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 1410.0f, 245.0f, Colour(0x301111ff),
                                     1410.0f, 270.0f, false));
    g.fillRect(1400, 230, 20, 40);

    g.setGradientFill(ColourGradient(Colour(0xff1111ff), 1410.0f, 425.0f, Colour(0x301111ff),
                                     1410.0f, 450.0f, false));
    g.fillRect(1400, 410, 20, 40);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 90.0f, 850.0f, Colour(0xff11ffff), 90.0f,
                                     805.0f, false));
    g.fillRect(80, 790, 30, 70);

    g.setColour(Colour(0xffff11ff));
    g.fillRoundedRectangle(470.0f, 620.0f, 330.0f, 180.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(10.0f, static_cast<float>(-10), 1440.0f, 60.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000), 290.0f, 600.0f, Colour(0xffffffee), 290.0f,
                                     615.0f, false));
    g.fillRect(270, 590, 40, 40);

    g.setGradientFill(ColourGradient(Colour(0x00000000), 320.0f, 850.0f, Colour(0xffffffee), 320.0f,
                                     835.0f, false));
    g.fillRect(330, 825, 40, 25);

    g.setGradientFill(ColourGradient(Colour(0x00000000), 170.0f, 760.0f, Colour(0xffffffee), 190.0f,
                                     760.0f, false));
    g.fillRect(170, 705, 30, 40);

    g.setGradientFill(ColourGradient(Colour(0x00000000), 470.0f, 760.0f, Colour(0xffffffee), 450.0f,
                                     760.0f, false));
    g.fillRect(440, 705, 40, 40);

    g.setGradientFill(ColourGradient(Colour(0xffff1111), 170.0f, 140.0f, Colour(0xff1111ff), 190.0f,
                                     140.0f, false));
    g.fillRect(160, 100, 40, 30);

    g.setGradientFill(ColourGradient(Colour(0xffff1111), 170.0f, 150.0f, Colour(0xff1111ff), 190.0f,
                                     150.0f, false));
    g.fillRect(160, 280, 40, 30);

    g.setGradientFill(ColourGradient(Colour(0xffff1111), 170.0f, 170.0f, Colour(0xff1111ff), 190.0f,
                                     170.0f, false));
    g.fillRect(160, 460, 40, 30);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(1395.0f, 590.0f, 30.0f, 50.0f, 5.000f);

    g.setColour(Colour(0xff1111ff));
    g.fillRoundedRectangle(990.0f, 450.0f, 460.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1360.0f, 850.0f, 90.0f, 150.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(20.0f, 860.0f, 1420.0f, 130.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, 90.0f, 280.0f, 160.0f, 10.000f);

    g.setGradientFill(ColourGradient(Colour(0xffffff11), 170.0f, 80.0f, Colour(0x30ffff11), 170.0f,
                                     90.0f, false));
    g.drawRoundedRectangle(1472.0f, 70.0f, 276.0f, 55.0f, 10.000f, 2.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, static_cast<float>(-11), 280.0f, 60.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, 810.0f, 280.0f, 410.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, 440.0f, 280.0f, 170.0f, 10.000f);

    g.setColour(Colour(0xffffff11));
    g.fillRoundedRectangle(1470.0f, 260.0f, 280.0f, 170.0f, 10.000f);

    keyboard->setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId,
                        look_and_feel->colours.get_theme(COLOUR_THEMES::BG_THEME).button_on_colour);
    keyboard->setColour(
        juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId,
        look_and_feel->colours.get_theme(COLOUR_THEMES::BG_THEME).button_on_colour.withAlpha(0.5f));
}

void Monique_Ui_Mainwindow::resized()
{
    synth_data->ui_scale_factor = getHeight() / original_h;
    WIDTH_AND_HIGHT_FACTORS

    filter_type_bg_button_5->setBounds(1620, 655, 120, 130);
    filter_type_bg_button_4->setBounds(1480, 655, 120, 130);
    overlay->setBounds(0 - 1, 0 - 13, 1466, 1236);
    label_monique->setBounds(1180 - 160, 0, 160, 50);
    pop_credits->setBounds(1020, 10, 155, 30);
    label_fx_delay->setBounds(960, 660, 120, 30);
    eq_7->setBounds(1270 - 60, 790 - 130, 60, 130);
    eq_6->setBounds(1200 - 60, 790 - 130, 60, 130);
    eq_5->setBounds(1140 - 60, 790 - 130, 60, 130);
    eq_4->setBounds(1070 - 60, 790 - 130, 60, 130);
    eq_3->setBounds(1010 - 60, 790 - 130, 60, 130);
    eq_2->setBounds(940 - 60, 790 - 130, 60, 130);
    eq_1->setBounds(880 - 60, 790 - 130, 60, 130);
    distortion->setBounds(880 - 60, 790 - 130, 60, 130);
    chorus_modulation->setBounds(890, 790 - 130, 60, 130);
    delay3->setBounds(1020 - 60, 790 - 130, 60, 130);
    delay2->setBounds(1080 - 60, 790 - 130, 60, 130);
    delay4->setBounds(1140 - 60, 790 - 130, 60, 130);
    reverb_room->setBounds(1210 - 60, 790 - 130, 60, 130);
    reverb_dry->setBounds(1270 - 60, 790 - 130, 60, 130);
    label_lfo_3->setBounds(900, 460, 60, 30);
    label_lfo_2->setBounds(900, 280, 60, 30);
    label_lfo_1->setBounds(900, 100, 60, 30);
    lfo_1->setBounds(900, 230 - 130, 60, 130);
    lfo_2->setBounds(900, 410 - 130, 60, 130);
    lfo_3->setBounds(900, 590 - 130, 60, 130);
    morpher_1->setBounds(260 - 60, 660, 60, 130);
    morpher_2->setBounds(320 - 60, 660, 60, 130);
    morpher_4->setBounds(380 - 60, 660, 60, 130);
    morpher_3->setBounds(440 - 60, 660, 60, 130);
    flt_input_13->setBounds(380 - 60, 590 - 130, 60, 130);
    flt_input_12->setBounds(320 - 60, 590 - 130, 60, 130);
    flt_input_11->setBounds(260 - 60, 590 - 130, 60, 130);
    flt_input_6->setBounds(200, 280, 60, 130);
    flt_input_7->setBounds(320 - 60, 410 - 130, 60, 130);
    flt_input_8->setBounds(380 - 60, 410 - 130, 60, 130);
    flt_input_3->setBounds(320, 230 - 130, 60, 130);
    flt_input_2->setBounds(320 - 60, 230 - 130, 60, 130);
    flt_input_1->setBounds(200, 100, 60, 130);
    button_edit_lfo_1->setBounds(960 - 60, 200, 60, 30);
    button_edit_lfo_2->setBounds(960 - 60, 380, 60, 30);
    button_edit_lfo_3->setBounds(960 - 60, 560, 60, 30);
    button_edit_input_env_3_3->setBounds(320, 560, 60, 30);
    button_edit_input_env_3_2->setBounds(260, 560, 60, 30);
    button_edit_input_env_3_1->setBounds(200, 560, 60, 30);
    button_edit_input_env_2_1->setBounds(200, 380, 60, 30);
    button_edit_input_env_2_2->setBounds(260, 380, 60, 30);
    button_edit_input_env_2_3->setBounds(320, 380, 60, 30);
    button_edit_input_env_1_3->setBounds(320, 200, 60, 30);
    button_edit_input_env_1_2->setBounds(260, 200, 60, 30);
    button_edit_input_env_1_1->setBounds(200, 200, 60, 30);
    button_edit_mfo_4->setBounds(440 - 60, 760, 60, 30);
    button_edit_mfo_3->setBounds(380 - 60, 760, 60, 30);
    button_edit_mfo_2->setBounds(320 - 60, 760, 60, 30);
    button_edit_mfo_1->setBounds(260 - 60, 760, 60, 30);
    button_edit_input_env_band_1->setBounds(820, 760, 60, 30);
    button_edit_input_env_band_2->setBounds(880, 760, 60, 30);
    effect_finalizer_switch2->setBounds(810, 799, 120, 30);
    button_edit_input_env_band_7->setBounds(1210, 760, 60, 30);
    button_edit_input_env_band_3->setBounds(950, 760, 60, 30);
    button_edit_input_env_band_4->setBounds(1010, 760, 60, 30);
    button_edit_input_env_band_5->setBounds(1080, 760, 60, 30);
    button_edit_input_env_band_6->setBounds(1140, 760, 60, 30);
    filter_type_bg_button_3->setBounds(1000, 460, 60, 130);
    filter_type_2_3->setBounds(1060 - 60, 493, 60, 30);
    filter_type_bg_button_2->setBounds(1000, 280, 60, 130);
    filter_type_2_2->setBounds(1060 - 60, 313, 60, 30);
    filter_type_bg_button_1->setBounds(1000, 100, 60, 130);
    filter_type_2_1->setBounds(1000, 133, 60, 30);
    button_toggle_morph_buttons_1->setBounds(260 - 60, 790, 60, 30);
    button_toggle_morph_buttons_2->setBounds(320 - 60, 790, 60, 30);
    button_toggle_morph_buttons_3->setBounds(380 - 60, 790, 60, 30);
    button_toggle_morph_buttons_4->setBounds(440 - 60, 790, 60, 30);
    label_band_hz_5->setBounds(1085, 616, 50, 30);
    label_band_hz_6->setBounds(1145, 616, 50, 30);
    label_band_hz_4->setBounds(1015, 616, 50, 30);
    label_band_hz_1->setBounds(825, 616, 50, 30);
    label_morph->setBounds(200, 625, 180, 30);
    label_band_hz_7->setBounds(1215, 616, 50, 30);
    label_arpeggiator->setBounds(580, 815, 220, 30);
    button_programm_replace->setBounds(835 - 60, 10, 60, 30);
    button_programm_new->setBounds(895 - 60, 10, 60, 30);
    bypass->setBounds(1345 - 60, 790 - 130, 60, 130);
    colour->setBounds(1285, 790 - 130, 60, 130);
    label_band_hz_2->setBounds(885, 616, 50, 30);
    label_band_hz_3->setBounds(955, 616, 50, 30);
    speed_multi->setBounds(1285, 990 - 130, 60, 130);
    osc_wave_3->setBounds(90 - 60, 590 - 130, 60, 130);
    keyboard->setBounds(0, 1030, 1465, 180);
    glide2->setBounds(100, 860, 60, 130);
    arp_step_16->setBounds(1185 - 60, 990 - 130, 60, 130);
    arp_step_15->setBounds(1125 - 60, 990 - 130, 60, 130);
    arp_step_14->setBounds(1065 - 60, 990 - 130, 60, 130);
    arp_step_13->setBounds(945, 860, 60, 130);
    arp_step_12->setBounds(935 - 60, 990 - 130, 60, 130);
    arp_step_11->setBounds(875 - 60, 990 - 130, 60, 130);
    arp_step_10->setBounds(815 - 60, 990 - 130, 60, 130);
    arp_step_9->setBounds(755 - 60, 990 - 130, 60, 130);
    arp_step_8->setBounds(685 - 60, 990 - 130, 60, 130);
    arp_step_7->setBounds(625 - 60, 990 - 130, 60, 130);
    arp_step_6->setBounds(565 - 60, 990 - 130, 60, 130);
    arp_step_5->setBounds(505 - 60, 990 - 130, 60, 130);
    arp_step_4->setBounds(435 - 60, 990 - 130, 60, 130);
    arp_step_3->setBounds(375 - 60, 990 - 130, 60, 130);
    arp_step_2->setBounds(315 - 60, 990 - 130, 60, 130);
    arp_step_1->setBounds(255 - 60, 990 - 130, 60, 130);
    shuffle->setBounds(30, 860, 60, 130);
    flt_sustain_4->setBounds(660 - 60, 790 - 130, 60, 130);
    flt_decay_4->setBounds(600 - 60, 790 - 130, 60, 130);
    flt_attack_4->setBounds(540 - 60, 790 - 130, 60, 130);
    flt_release_3->setBounds(720 - 60, 590 - 130, 60, 130);
    flt_sustain_time_3->setBounds(660 - 60, 590 - 130, 60, 130);
    flt_sustain_3->setBounds(600 - 60, 590 - 130, 60, 130);
    flt_decay_3->setBounds(540 - 60, 590 - 130, 60, 130);
    flt_attack_3->setBounds(480 - 60, 590 - 130, 60, 130);
    flt_release_2->setBounds(720 - 60, 410 - 130, 60, 130);
    flt_sustain_time_2->setBounds(660 - 60, 410 - 130, 60, 130);
    flt_sustain_2->setBounds(600 - 60, 410 - 130, 60, 130);
    flt_decay_2->setBounds(540 - 60, 410 - 130, 60, 130);
    flt_attack_2->setBounds(480 - 60, 410 - 130, 60, 130);
    flt_release_1->setBounds(720 - 60, 230 - 130, 60, 130);
    flt_sustain_time_1->setBounds(660 - 60, 230 - 130, 60, 130);
    flt_sustain_1->setBounds(600 - 60, 230 - 130, 60, 130);
    flt_decay_1->setBounds(540 - 60, 230 - 130, 60, 130);
    flt_attack_1->setBounds(420, 230 - 130, 60, 130);
    filter_type_3_1->setBounds(1000, 167, 60, 30);
    filter_type_3_2->setBounds(1060 - 60, 347, 60, 30);
    filter_type_3_3->setBounds(1060 - 60, 527, 60, 30);
    filter_type_5_1->setBounds(1000, 200, 60, 30);
    filter_type_5_2->setBounds(1060 - 60, 380, 60, 30);
    filter_type_5_3->setBounds(1060 - 60, 560, 60, 30);
    button_sequence_2->setBounds(315 - 60, 860, 60, 30);
    button_sequence_3->setBounds(375 - 60, 860, 60, 30);
    button_sequence_4->setBounds(435 - 60, 860, 60, 30);
    button_sequence_5->setBounds(505 - 60, 860, 60, 30);
    button_sequence_6->setBounds(565 - 60, 860, 60, 30);
    button_sequence_7->setBounds(625 - 60, 860, 60, 30);
    button_sequence_8->setBounds(685 - 60, 860, 60, 30);
    button_sequence_9->setBounds(755 - 60, 860, 60, 30);
    button_sequence_10->setBounds(815 - 60, 860, 60, 30);
    button_sequence_11->setBounds(875 - 60, 860, 60, 30);
    button_sequence_12->setBounds(935 - 60, 860, 60, 30);
    button_sequence_13->setBounds(1005 - 60, 860, 60, 30);
    button_sequence_14->setBounds(1065 - 60, 860, 60, 30);
    button_sequence_15->setBounds(1125 - 60, 860, 60, 30);
    button_sequence_16->setBounds(1185 - 60, 860, 60, 30);
    combo_programm->setBounds(320, 10, 255, 30);
    button_programm_left->setBounds(200, 10, 60, 30);
    button_programm_right->setBounds(635 - 60, 10, 60, 30);
    button_open_oszi->setBounds(1310, 10, 60, 30);
    button_vers_info->setBounds(1370, 10, 60, 30);
    button_open_midi_io_settings->setBounds(1240 - 60, 40 - 30, 60, 30);
    combo_bank->setBounds(260, 10, 60, 30);
    button_programm_load->setBounds(695 - 60, 10, 60, 30);
    osc_1->setBounds(100, 100, 60, 130);
    osc_2->setBounds(100, 280, 60, 130);
    osc_3->setBounds(160 - 60, 590 - 130, 60, 130);
    flt_cutoff_1->setBounds(1072, 230 - 130, 60, 130);
    flt_cutoff_2->setBounds(1132 - 60, 410 - 130, 60, 130);
    flt_cutoff_3->setBounds(1132 - 60, 590 - 130, 60, 130);
    flt_distortion_1->setBounds(1280 - 60, 230 - 130, 60, 130);
    flt_resonance_1->setBounds(1192 - 60, 230 - 130, 60, 130);
    flt_resonance_2->setBounds(1192 - 60, 410 - 130, 60, 130);
    flt_resonance_3->setBounds(1192 - 60, 590 - 130, 60, 130);
    flt_volume_1->setBounds(1380, 100, 60, 130);
    flt_volume_2->setBounds(1380, 280, 60, 130);
    flt_volume_3->setBounds(1380, 460, 60, 130);
    adsr_lfo_mix->setBounds(818, 100, 60, 130);
    lfo_opt_2->setBounds(818, 280, 60, 130);
    lfo_opt_3->setBounds(818, 460, 60, 130);
    button_sequence_1->setBounds(255 - 60, 860, 60, 30);
    flt_release_4->setBounds(720 - 60, 790 - 130, 60, 130);
    volume->setBounds(1380, 660, 60, 130);
    flt_distortion_2->setBounds(1280 - 60, 410 - 130, 60, 130);
    flt_distortion_3->setBounds(1280 - 60, 590 - 130, 60, 130);
    button_arp_speed_XNORM->setBounds(1285, 860, 60, 30);
    flt_attack_5->setBounds(90 - 60, 660, 60, 130);
    flt_attack_6->setBounds(160 - 60, 660, 60, 130);
    osc_wave_1->setBounds(30, 100, 60, 130);
    osc_wave_2->setBounds(30, 280, 60, 130);
    button_programm_delete->setBounds(1015 - 55, 10, 60, 30);
    filter_type_6_1->setBounds(1000, 100, 60, 30);
    filter_type_6_2->setBounds(1060 - 60, 280, 60, 30);
    filter_type_6_3->setBounds(1060 - 60, 460, 60, 30);
    button_ctrl_toggle->setBounds(100, 10, 60, 30);
    speed->setBounds(1275 - 60, 990 - 130, 60, 130);
    button_open_morph->setBounds(440 - 60, 625, 60, 30);
    effect_finalizer_switch->setBounds(930, 799, 120, 30);
    button_values_toggle->setBounds(30, 10, 60, 30);
    octave_offset->setBounds(1380, 860, 60, 130);
    label_filter_inputs->setBounds(230, 55, 120, 30);
    label_oscillators->setBounds(60, 55, 70, 30);
    label_filter_envelope->setBounds(510, 55, 190, 30);
    label_lfo->setBounds(900, 55, 60, 30);
    label_filter_config->setBounds(1030, 56, 130, 30);
    label_filter_fx->setBounds(1240, 55, 96, 30);
    label_out->setBounds(1380, 55, 65, 30);
    label_amp_envelope->setBounds(480, 625, 310, 30);
    label_glide->setBounds(100, 815, 60, 30);
    label_speed->setBounds(1225, 815, 120, 30);
    label_tune->setBounds(1380, 815, 60, 30);
    label_fm->setBounds(30, 625, 130, 30);
    volume_master_meter->setBounds(1380, 660, 60, 27);
    button_open_config2->setBounds(1300 - 60, 40 - 30, 60, 30);
    label_mod_mix->setBounds(800, 55, 90, 30);
    flt_pan_3->setBounds(1350 - 60, 590 - 130, 60, 130);
    flt_pan_2->setBounds(1350 - 60, 410 - 130, 60, 130);
    flt_pan_1->setBounds(1350 - 60, 230 - 130, 60, 130);
    button_reset_arp_tune->setBounds(1380, 860, 60, 30);
    button_show_active_input_r_2_3->setBounds(361, 269, 10, 10);
    button_show_active_input_l_2_3->setBounds(331, 269, 10, 10);
    button_show_active_input_r_2_2->setBounds(301, 269, 10, 10);
    button_show_active_input_l_2_2->setBounds(271, 269, 10, 10);
    button_show_active_input_r_2_1->setBounds(241, 269, 10, 10);
    button_show_active_input_l_2_1->setBounds(211, 269, 10, 10);
    button_show_active_input_r_3_3->setBounds(361, 449, 10, 10);
    button_show_active_input_l_3_3->setBounds(331, 449, 10, 10);
    button_show_active_input_r_3_2->setBounds(301, 449, 10, 10);
    button_show_active_input_l_3_2->setBounds(271, 449, 10, 10);
    button_show_active_input_r_3_1->setBounds(241, 449, 10, 10);
    button_show_active_input_l_3_1->setBounds(210, 449, 10, 10);
    button_programm_rename->setBounds(955 - 60, 10, 65, 30);
    flt_shape_1->setBounds(730, 230 - 130, 60, 130);
    flt_shape_2->setBounds(790 - 60, 410 - 130, 60, 130);
    flt_shape_3->setBounds(790 - 60, 590 - 130, 60, 130);
    button_programm_scratch->setBounds(765 - 60, 10, 60, 30);
    flt_shape_4->setBounds(790 - 60, 790 - 130, 60, 130);
    label_reverb->setBounds(1150, 660, 120, 30);
    button_open_playback->setBounds(1440 - 60, 40 - 30, 60, 30);
    button_preset_agro->setBounds(1620, 755, 120, 30);
    button_tracking_mode_hm->setBounds(1480, 755, 120, 30);
    button_preset_down->setBounds(1620, 722, 120, 30);
    button_tracking_mode_lf->setBounds(1480, 722, 120, 30);
    button_preset_rising->setBounds(1620, 688, 120, 30);
    button_tracking_mode_hf->setBounds(1480, 688, 120, 30);
    button_preset_soft->setBounds(1620, 655, 120, 30);
    button_tracking_mode_keep->setBounds(1480, 655, 120, 30);
    label_2->setBounds(1620, 625, 120, 30);
    label_24->setBounds(1480, 625, 120, 30);
    label_23->setBounds(1680, 560, 60, 30);
    label_22->setBounds(1680, 380, 60, 30);
    label_21->setBounds(1680, 200, 60, 30);
    slider_flt_out_sesitivity_3->setBounds(1680, 500, 60, 56);
    slider_flt_out_sesitivity_2->setBounds(1680, 320, 60, 56);
    slider_flt_out_sesitivity_1->setBounds(1680, 140, 60, 56);
    button_flt_out_triggering_1->setBounds(1680, 100, 60, 30);
    button_flt_out_triggering_2->setBounds(1680, 280, 60, 30);
    button_flt_out_triggering_3->setBounds(1680, 460, 60, 30);
    label_13->setBounds(1480, 560, 60, 30);
    label_7->setBounds(1540, 560, 60, 30);
    button_flt_env_triggering_3->setBounds(1610, 560, 60, 30);
    slider_osc_tracking_oct_3->setBounds(1480, 500, 60, 56);
    slider_cutoff_tracking_oct_3->setBounds(1540, 500, 60, 56);
    button_flt_input_triggering_3_1->setBounds(1610, 460, 60, 30);
    button_osc_tracking_3->setBounds(1480, 460, 60, 30);
    button_cutoff_tracking_3->setBounds(1540, 460, 60, 30);
    label_12->setBounds(1480, 380, 60, 30);
    label_6->setBounds(1540, 380, 60, 30);
    button_flt_input_triggering_1_1->setBounds(1610, 100, 60, 30);
    button_flt_input_triggering_2_1->setBounds(1610, 280, 60, 30);
    button_flt_env_triggering_1->setBounds(1610, 200, 60, 30);
    button_flt_env_triggering_2->setBounds(1610, 380, 60, 30);
    slider_osc_tracking_oct_2->setBounds(1480, 320, 60, 56);
    slider_cutoff_tracking_oct_2->setBounds(1540, 320, 60, 56);
    button_osc_tracking_2->setBounds(1480, 280, 60, 30);
    button_cutoff_tracking_2->setBounds(1540, 280, 60, 30);
    label_5->setBounds(1540, 200, 60, 30);
    button_cutoff_tracking_1->setBounds(1540, 100, 60, 30);
    slider_cutoff_tracking_oct_1->setBounds(1540, 140, 60, 56);
    button_osc_tracking_1->setBounds(1480, 100, 60, 30);
    label_oscillators2->setBounds(1500, 55, 220, 30);
    label_sub_poly->setBounds(1730 - 240, 0, 240, 50);
    label_poly_desc_1->setBounds(1480, 820, 260, 30);
    label_poly_desc_2->setBounds(1480, 860, 260, 30);
    label_poly_desc_3->setBounds(1480, 880, 260, 30);
    label_poly_desc_4->setBounds(1480, 910, 260, 30);
    label_poly_desc_5->setBounds(1480, 930, 260, 30);
    label_poly_desc_6->setBounds(1480, 950, 260, 30);
    label_poly_desc_7->setBounds(1480, 980, 260, 30);
    label_poly_desc_8->setBounds(1480, 1000, 260, 30);
    label_poly_desc_9->setBounds(1480, 1020, 260, 30);
    label_poly_desc_10->setBounds(1480, 1050, 260, 30);
    label_poly_desc_11->setBounds(1480, 1070, 260, 30);
    label_poly_desc_12->setBounds(1480, 1090, 260, 30);
    label_poly_desc_13->setBounds(1480, 1120, 260, 30);
    label_poly_desc_14->setBounds(1480, 1140, 260, 30);
    label_poly_desc_15->setBounds(1480, 1160, 260, 30);
    label_poly_desc_16->setBounds(1480, 1180, 260, 30);
    button_flt_input_triggering_1_2->setBounds(1610, 130, 60, 30);
    button_flt_input_triggering_1_3->setBounds(1610, 160, 60, 30);
    button_flt_input_triggering_2_2->setBounds(1610, 310, 60, 30);
    button_flt_input_triggering_2_3->setBounds(1610, 340, 60, 30);
    button_flt_input_triggering_3_2->setBounds(1610, 490, 60, 30);
    button_flt_input_triggering_3_3->setBounds(1610, 520, 60, 30);

#include "mono_ui_includeHacks_END.h"

    {
        // GET ORIGINAL SLIDER POSIES
        {
            juce::ScopedLock locked(resize_lock);

            original_slider_positions.clearQuick();
            for (int i = 0; i != SUM_ENV_ARP_STEPS; ++i)
            {
                Monique_Ui_DualSlider *slider = sequence_sliders.getUnchecked(i);
                original_slider_positions.add(slider->getPosition());
            }
        }
        resize_sequence_buttons(true);
    }

    resize_subeditors();

    resizer->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
#ifdef IS_MOBILE
    keyboard->setKeyWidth((original_w / 18) * 1.0f / original_w * getWidth());
#else
    keyboard->setKeyWidth(60.0f * 1.0f / original_w * getWidth());
#endif

    if (!isVisible())
    {
        setVisible(true);
    }
}

void Monique_Ui_Mainwindow::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (credits->isVisible())
    {
        credits->setVisible(false);
    }

    if (buttonThatWasClicked == button_edit_lfo_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->lfo_datas[0], button_edit_lfo_1.get(), lfo_1.get(),
                           COLOUR_THEMES::FILTER_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_lfo_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->lfo_datas[1], button_edit_lfo_2.get(), lfo_2.get(),
                           COLOUR_THEMES::FILTER_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_lfo_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->lfo_datas[2], button_edit_lfo_3.get(), lfo_3.get(),
                           COLOUR_THEMES::FILTER_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[2]->input_envs[2],
                           &synth_data->filter_datas[2]->input_envs[2]->sustain,
                           buttonThatWasClicked, flt_input_13.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[2]->input_envs[1],
                           &synth_data->filter_datas[2]->input_envs[1]->sustain,
                           buttonThatWasClicked, flt_input_12.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_3_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[2]->input_envs[0],
                           &synth_data->filter_datas[2]->input_envs[0]->sustain,
                           buttonThatWasClicked, flt_input_11.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[1]->input_envs[0],
                           &synth_data->filter_datas[1]->input_envs[0]->sustain,
                           buttonThatWasClicked, flt_input_6.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[1]->input_envs[1],
                           &synth_data->filter_datas[1]->input_envs[1]->sustain,
                           buttonThatWasClicked, flt_input_7.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_2_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[1]->input_envs[2],
                           &synth_data->filter_datas[1]->input_envs[2]->sustain,
                           buttonThatWasClicked, flt_input_8.get(), true);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[0]->input_envs[2],
                           &synth_data->filter_datas[0]->input_envs[2]->sustain,
                           buttonThatWasClicked, flt_input_3.get(), false);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[0]->input_envs[1],
                           &synth_data->filter_datas[0]->input_envs[1]->sustain,
                           buttonThatWasClicked, flt_input_2.get(), false);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_1_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_env_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_env_popup(synth_data->filter_datas[0]->input_envs[0],
                           &synth_data->filter_datas[0]->input_envs[0]->sustain,
                           buttonThatWasClicked, flt_input_1.get(), false);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_env_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_mfo_4.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->mfo_datas[2], button_edit_mfo_4.get(), morpher_3.get(),
                           COLOUR_THEMES::MORPH_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_mfo_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->mfo_datas[3], button_edit_mfo_3.get(), morpher_4.get(),
                           COLOUR_THEMES::MORPH_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_mfo_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->mfo_datas[1], button_edit_mfo_2.get(), morpher_2.get(),
                           COLOUR_THEMES::MORPH_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_mfo_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->midi_lfo_popup,
                                                    buttonThatWasClicked)
        else
        {
            open_mfo_popup(synth_data->mfo_datas[0], button_edit_mfo_1.get(), morpher_1.get(),
                           COLOUR_THEMES::MORPH_THEME);
        }
        show_info_popup(buttonThatWasClicked, synth_data->midi_lfo_popup.midi_control);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_1.get())
    {
        open_env_popup(synth_data->eq_data->envs[0], &synth_data->eq_data->envs[0]->sustain,
                       buttonThatWasClicked, eq_1.get(), true);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_2.get())
    {
        open_env_popup(synth_data->eq_data->envs[1], &synth_data->eq_data->envs[1]->sustain,
                       buttonThatWasClicked, eq_2.get(), true);
    }
    else if (buttonThatWasClicked == effect_finalizer_switch2.get())
    {
        switch_finalizer_tab(false);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_7.get())
    {
        open_env_popup(synth_data->eq_data->envs[6], &synth_data->eq_data->envs[6]->sustain,
                       buttonThatWasClicked, eq_7.get(), true);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_3.get())
    {
        open_env_popup(synth_data->eq_data->envs[2], &synth_data->eq_data->envs[2]->sustain,
                       buttonThatWasClicked, eq_3.get(), true);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_4.get())
    {
        open_env_popup(synth_data->eq_data->envs[3], &synth_data->eq_data->envs[3]->sustain,
                       buttonThatWasClicked, eq_4.get(), true);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_5.get())
    {
        open_env_popup(synth_data->eq_data->envs[4], &synth_data->eq_data->envs[4]->sustain,
                       buttonThatWasClicked, eq_5.get(), true);
    }
    else if (buttonThatWasClicked == button_edit_input_env_band_6.get())
    {
        open_env_popup(synth_data->eq_data->envs[5], &synth_data->eq_data->envs[5]->sustain,
                       buttonThatWasClicked, eq_6.get(), true);
    }
    // clang-format off
    else if (buttonThatWasClicked == filter_type_2_3.get())
    {
        int flt_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[flt_id]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = HPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[flt_id]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_2_2.get())
    {
        int flt_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[1]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = HPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[1]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_2_1.get())
    {
        int flt_id = 0;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[flt_id]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = HPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[flt_id]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->morhp_switch_states[0],
                                                    buttonThatWasClicked)
        else { synth_data->morhp_switch_states[0] ^= true; }
        show_info_popup(buttonThatWasClicked, synth_data->morhp_switch_states[0].midi_control);
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->morhp_switch_states[1],
                                                    buttonThatWasClicked)
        else { synth_data->morhp_switch_states[1] ^= true; }
        show_info_popup(buttonThatWasClicked, synth_data->morhp_switch_states[1].midi_control);
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->morhp_switch_states[3],
                                                    buttonThatWasClicked)
        else { synth_data->morhp_switch_states[3] ^= true; }
        show_info_popup(buttonThatWasClicked, synth_data->morhp_switch_states[3].midi_control);
    }
    else if (buttonThatWasClicked == button_toggle_morph_buttons_4.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->morhp_switch_states[2],
                                                    buttonThatWasClicked)
        else { synth_data->morhp_switch_states[2] ^= true; }
        show_info_popup(buttonThatWasClicked, synth_data->morhp_switch_states[2].midi_control);
    }
    else if (buttonThatWasClicked == button_programm_replace.get())
    {
        program_edit_type = EDIT_TYPES::REPLACE;
        const bool success = synth_data->replace();
        show_programs_and_select(true);
        button_flasher = std::make_unique<ButtonFlasher>(this, buttonThatWasClicked, success);
    }
    else if (buttonThatWasClicked == button_programm_new.get())
    {
        program_edit_type = EDIT_TYPES::CREATE;
        if (combo_programm->getText() == FACTORY_NAME)
        {
            combo_programm->setText("PROGRAM FROM SCRATCH", juce::dontSendNotification);
        }
        combo_programm->setEditableText(true);
        juce::String bank = combo_bank->getText();
        juce::String name = combo_programm->getText();
        combo_programm->setText(synth_data->generate_programm_name(bank, name),
                                juce::dontSendNotification);
        combo_programm->showEditor();
    }
    else if (buttonThatWasClicked == filter_type_3_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[0]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[0]->filter_type = BPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[0]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_3_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[1]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[1]->filter_type = BPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[1]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_3_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[2]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[2]->filter_type = BPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[2]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_5_1.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[0]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[0]->filter_type = PASS; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[0]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_5_2.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[1]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[1]->filter_type = PASS; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[1]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_5_3.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[2]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[2]->filter_type = PASS; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[2]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_2.get())
    {
        int step_id = 1;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_3.get())
    {
        int step_id = 2;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_4.get())
    {
        int step_id = 3;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_5.get())
    {
        int step_id = 4;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_6.get())
    {
        int step_id = 5;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_7.get())
    {
        int step_id = 6;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_8.get())
    {
        int step_id = 7;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_9.get())
    {
        int step_id = 8;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_10.get())
    {
        int step_id = 9;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_11.get())
    {
        int step_id = 10;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_12.get())
    {
        int step_id = 11;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_13.get())
    {
        int step_id = 12;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_14.get())
    {
        int step_id = 13;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_15.get())
    {
        int step_id = 14;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_sequence_16.get())
    {
        int step_id = 15;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_programm_left.get())
    {
        program_edit_type = EDIT_TYPES::LOAD;
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        synth_data->load_prev();
        show_programs_and_select(true);
        // button_flasher = new ButtonFlasher(button_programm_load,success,1);
    }
    else if (buttonThatWasClicked == button_programm_right.get())
    {
        program_edit_type = EDIT_TYPES::LOAD;
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        synth_data->load_next();
        show_programs_and_select(true);
        // button_flasher = new ButtonFlasher(button_programm_load,success,1);
    }
    else if (buttonThatWasClicked == button_open_oszi.get())
    {
        if (amp_painter)
        {
            audio_processor->amp_painter = nullptr;
            removeChildComponent(amp_painter);
            delete amp_painter;
            amp_painter = nullptr;

            synth_data->is_osci_open = false;
        }
        else
        {
            close_all_subeditors();

            amp_painter = new Monique_Ui_AmpPainter(synth_data, look_and_feel);

            addChildComponent(amp_painter);
            resize_subeditors();
            amp_painter->setVisible(true);

            audio_processor->amp_painter = amp_painter;

            synth_data->is_osci_open = true;
        }
    }
    else if (buttonThatWasClicked == button_vers_info.get())
    {
        if (vers_info)
        {
            removeChildComponent(vers_info.get());
            vers_info = nullptr;
        }
        else
        {
            close_all_subeditors();

            vers_info = std::make_unique<Monique_Ui_Info>(look_and_feel);

            addChildComponent(vers_info.get());
            resize_subeditors();
            vers_info->setVisible(true);
        }
    }
    else if (buttonThatWasClicked == button_open_midi_io_settings.get())
    {
        if (editor_midiio)
            editor_midiio = nullptr;
        else
        {
            open_midi_editor_if_closed();
        }
    }
    else if (buttonThatWasClicked == button_programm_load.get())
    {
        program_edit_type = EDIT_TYPES::LOAD;
        const bool success = synth_data->load();
        show_programs_and_select(true);
        button_flasher = std::make_unique<ButtonFlasher>(this, button_programm_load.get(), success);
    }
    else if (buttonThatWasClicked == button_sequence_1.get())
    {
        int step_id = 0;
        step_id = step_id - last_step_offset;
        if (step_id < 0)
        {
            step_id += SUM_ENV_ARP_STEPS;
        }
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->step[step_id],
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->step[step_id] ^= true; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->step[step_id].midi_control);
    }
    else if (buttonThatWasClicked == button_arp_speed_XNORM.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->arp_sequencer_data->speed_multi,
                                                    buttonThatWasClicked)
        else { synth_data->arp_sequencer_data->speed_multi = 0; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->arp_sequencer_data->speed_multi.midi_control);
    }
    else if (buttonThatWasClicked == button_programm_delete.get())
    {
        program_edit_type = EDIT_TYPES::REMOVE;
        if (synth_data->remove())
        {
            show_programs_and_select(true);
            button_flasher =
                std::make_unique<ButtonFlasher>(this, button_programm_delete.get(), true);
        }
        else
        {
            button_flasher =
                std::make_unique<ButtonFlasher>(this, button_programm_delete.get(), false);
        }
    }
    else if (buttonThatWasClicked == filter_type_6_1.get())
    {
        int flt_id = 0;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[flt_id]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = LPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[flt_id]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_6_2.get())
    {
        int flt_id = 1;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[flt_id]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = LPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[flt_id]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == filter_type_6_3.get())
    {
        int flt_id = 2;
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->filter_datas[flt_id]->filter_type,
                                                    buttonThatWasClicked)
        else { synth_data->filter_datas[flt_id]->filter_type = LPF; }
        show_info_popup(buttonThatWasClicked,
                        synth_data->filter_datas[flt_id]->filter_type.midi_control);
    }
    else if (buttonThatWasClicked == button_ctrl_toggle.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&synth_data->shift, buttonThatWasClicked)
        else
        {
            synth_data->shift ^= true;
            show_ctrl_state();
        }
        show_info_popup(buttonThatWasClicked, synth_data->shift.midi_control);
    }
    else if (buttonThatWasClicked == button_open_morph.get())
    {
        if (!editor_morph)
        {
            close_all_subeditors();
            editor_morph = std::make_unique<Monique_Ui_MorphConfig>(ui_refresher, look_and_feel);
            addChildComponent(editor_morph.get());
            resize_subeditors();
            editor_morph->setVisible(true);
        }
        else
            editor_morph = nullptr;
    }
    else if (buttonThatWasClicked == effect_finalizer_switch.get())
    {
        switch_finalizer_tab(true);
    }
    else if (buttonThatWasClicked == button_values_toggle.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&look_and_feel->show_values_always,
                                                    buttonThatWasClicked)
        else { look_and_feel->show_values_always ^= true; }
        show_info_popup(buttonThatWasClicked, look_and_feel->show_values_always.midi_control);
    }
    // clang-format on
    else if (buttonThatWasClicked == button_open_config2.get())
    {
        if (!editor_global_settings)
        {
            close_all_subeditors();

            editor_global_settings =
                std::make_unique<Monique_Ui_GlobalSettings>(ui_refresher, this);
            addChildComponent(editor_global_settings.get());
            resize_subeditors();
            editor_global_settings->setVisible(true);
        }
        else
            editor_global_settings = nullptr;
    }
    else if (buttonThatWasClicked == button_reset_arp_tune.get())
    {
        synth_data->keep_arp_always_on = false;
        synth_data->keep_arp_always_off = false;
        audio_processor->reset_pending_notes();

        audio_processor->noteOn(1, 60 + synth_data->note_offset.get_value() - 24, 1.0f);
        audio_processor->noteOff(1, 60 + synth_data->note_offset.get_value() - 24, 0);

        button_flasher = std::make_unique<ButtonFlasher>(this, buttonThatWasClicked, true, 1);
    }
    else if (buttonThatWasClicked == button_programm_rename.get())
    {
        program_edit_type = EDIT_TYPES::RENAME;
        combo_programm->setEditableText(true);
        combo_programm->showEditor();
    }
    else if (buttonThatWasClicked == button_programm_scratch.get())
    {
#if ASK_FOR_SAVE
        synth_data->ask_and_save_if_changed();
#endif
        if (clear_record_timer)
        {
            clear_record_timer = nullptr;
            button_flasher = std::make_unique<ButtonFlasher>(this, buttonThatWasClicked, false);
        }
        else
        {
            synth_data->load_default();
            show_programs_and_select(true);
            button_flasher = std::make_unique<ButtonFlasher>(this, buttonThatWasClicked, true);

            toggle_modulation_slider_top_button(delay4->get_top_button(), true);
        }
    }
    else if (buttonThatWasClicked == button_open_playback.get())
    {
#ifdef POLY
        if (synth_data->ui_is_large)
        {
#ifdef IS_MOBILE
            this->setVisible(false);
            reinterpret_cast<StandaloneFilterWindow *>(getParentComponent())->swap_size();
            synth_data->ui_is_large.set_value(true);
#else
            original_w = 1465;
            synth_data->ui_is_large.set_value(false);
#endif
        }
        else
        {
#ifdef IS_MOBILE
            this->setVisible(false);
            reinterpret_cast<StandaloneFilterWindow *>(getParentComponent())->swap_size();
            synth_data->ui_is_large.set_value(false);
#else
            original_w = 1760;
            synth_data->ui_is_large.set_value(true);
#endif
        }
#ifndef IS_MOBILE
        update_size();
#endif
#endif
    }
    else if (buttonThatWasClicked == button_preset_agro.get())
    {
#ifdef POLY
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
        /*
            synth_data->keytrack_filter_inputs[0] = false;
            synth_data->keytrack_filter_inputs[1] = false;
            synth_data->keytrack_filter_inputs[2] = false;
            synth_data->keytrack_filter_env[0] = true;
            synth_data->keytrack_filter_env[1] = true;
            synth_data->keytrack_filter_env[2] = true;
        */
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 1;
    }
    else if (buttonThatWasClicked == button_tracking_mode_hm)
    {
        synth_data->keytrack_osci_play_mode = 3;
    }
    else if (buttonThatWasClicked == button_preset_down)
    {
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
        /*
            synth_data->keytrack_filter_inputs[0] = false;
            synth_data->keytrack_filter_inputs[1] = false;
            synth_data->keytrack_filter_inputs[2] = false;
            synth_data->keytrack_filter_env[0] = true;
            synth_data->keytrack_filter_env[1] = true;
            synth_data->keytrack_filter_env[2] = true;
        */
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 0;
    }
    else if (buttonThatWasClicked == button_tracking_mode_lf)
    {
        synth_data->keytrack_osci_play_mode = 0;
    }
    else if (buttonThatWasClicked == button_preset_rising)
    {
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
        /*
            synth_data->keytrack_filter_inputs[0] = false;
            synth_data->keytrack_filter_inputs[1] = false;
            synth_data->keytrack_filter_inputs[2] = false;
            synth_data->keytrack_filter_env[0] = true;
            synth_data->keytrack_filter_env[1] = true;
            synth_data->keytrack_filter_env[2] = true;
        */
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 0;
    }
    else if (buttonThatWasClicked == button_tracking_mode_hf)
    {
        synth_data->keytrack_osci_play_mode = 1;
    }
    else if (buttonThatWasClicked == button_preset_soft)
    {
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
        /*
            synth_data->keytrack_filter_inputs[0] = false;
            synth_data->keytrack_filter_inputs[1] = false;
            synth_data->keytrack_filter_inputs[2] = false;
            synth_data->keytrack_filter_env[0] = true;
            synth_data->keytrack_filter_env[1] = true;
            synth_data->keytrack_filter_env[2] = true;
        */
        synth_data->keytrack_filter_volume[0] = true;
        synth_data->keytrack_filter_volume[1] = true;
        synth_data->keytrack_filter_volume[2] = true;
        synth_data->keytrack_filter_volume_offset[0] = 0;
        synth_data->keytrack_filter_volume_offset[1] = 0;
        synth_data->keytrack_filter_volume_offset[2] = 0;

        synth_data->keytrack_osci_play_mode = 1;
    }
    else if (buttonThatWasClicked == button_tracking_mode_keep)
    {
        synth_data->keytrack_osci_play_mode = 2;
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_1)
    {
        synth_data->keytrack_filter_volume[0] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_2)
    {
        synth_data->keytrack_filter_volume[1] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_out_triggering_3)
    {
        synth_data->keytrack_filter_volume[2] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_3)
    {
        synth_data->keytrack_filter_env[2] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_3_1)
    {
        synth_data->keytrack_filter_inputs[6] ^= true;
    }
    else if (buttonThatWasClicked == button_osc_tracking_3)
    {
        synth_data->keytrack_osci[2] ^= true;
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_3)
    {
        synth_data->keytrack_cutoff[2] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_1_1)
    {
        synth_data->keytrack_filter_inputs[0] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_2_1)
    {
        synth_data->keytrack_filter_inputs[3] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_1)
    {
        synth_data->keytrack_filter_env[0] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_env_triggering_2)
    {
        synth_data->keytrack_filter_env[1] ^= true;
    }
    else if (buttonThatWasClicked == button_osc_tracking_2)
    {
        synth_data->keytrack_osci[1] ^= true;
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_2)
    {
        synth_data->keytrack_cutoff[1] ^= true;
    }
    else if (buttonThatWasClicked == button_cutoff_tracking_1)
    {
        synth_data->keytrack_cutoff[0] ^= true;
    }
    else if (buttonThatWasClicked == button_osc_tracking_1)
    {
        synth_data->keytrack_osci[0] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_1_2)
    {
        synth_data->keytrack_filter_inputs[1] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_1_3)
    {
        synth_data->keytrack_filter_inputs[2] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_2_2)
    {
        synth_data->keytrack_filter_inputs[4] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_2_3)
    {
        synth_data->keytrack_filter_inputs[5] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_3_2)
    {
        synth_data->keytrack_filter_inputs[7] ^= true;
    }
    else if (buttonThatWasClicked == button_flt_input_triggering_3_3)
    {
        synth_data->keytrack_filter_inputs[8] ^= true;
#endif
    }
}

void Monique_Ui_Mainwindow::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == combo_programm.get())
    {
        combo_programm->setEditableText(false);

        juce::String new_name = combo_programm->getText();
        juce::String old_name = combo_programm->getItemText(combo_programm->getSelectedItemIndex());
        if (old_name != new_name && combo_programm->getSelectedItemIndex() == -1)
        {
            if (program_edit_type == CREATE)
            {
                const bool success = synth_data->create_new(new_name);
                button_flasher =
                    std::make_unique<ButtonFlasher>(this, button_programm_new.get(), success);
            }
            else if (program_edit_type == RENAME)
            {
                const bool success = synth_data->rename(new_name);
                button_flasher =
                    std::make_unique<ButtonFlasher>(this, button_programm_rename.get(), success);
            }
            show_programs_and_select(true);
        }
        else
        {
            juce::String program_to_load = combo_programm->getText();
#if ASK_FOR_SAVE
            synth_data->ask_and_save_if_changed();
#endif
            int new_index = synth_data->get_current_bank_programms().indexOf(program_to_load);
            synth_data->set_current_program(new_index);
            const bool success = synth_data->load();
            button_flasher =
                std::make_unique<ButtonFlasher>(this, button_programm_load.get(), success, 1);
        }
        program_edit_type = NOT_SET;
    }
    else if (comboBoxThatHasChanged == combo_bank.get())
    {
        synth_data->set_current_bank(combo_bank->getSelectedItemIndex());
        show_programs_and_select(true);
    }
}

void Monique_Ui_Mainwindow::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
#ifdef POLY

    if (sliderThatWasMoved == slider_flt_out_sesitivity_3)
    {
        synth_data->keytrack_filter_volume_offset[2] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_flt_out_sesitivity_2)
    {
        synth_data->keytrack_filter_volume_offset[1] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_flt_out_sesitivity_1)
    {
        synth_data->keytrack_filter_volume_offset[0] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_osc_tracking_oct_3)
    {
        synth_data->keytrack_osci_octave_offset[2] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_cutoff_tracking_oct_3)
    {
        synth_data->keytrack_cutoff_octave_offset[2] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_osc_tracking_oct_2)
    {
        synth_data->keytrack_osci_octave_offset[1] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_cutoff_tracking_oct_2)
    {
        synth_data->keytrack_cutoff_octave_offset[1] = sliderThatWasMoved->getValue();
    }
    else if (sliderThatWasMoved == slider_cutoff_tracking_oct_1)
    {
        synth_data->keytrack_cutoff_octave_offset[0] = sliderThatWasMoved->getValue();
    }

#endif
}

bool Monique_Ui_Mainwindow::keyPressed(const juce::KeyPress &key)
{
    bool success = false;
    if (key == juce::KeyPress::escapeKey)
    {
        clear_record_timer = nullptr;
        midi_control_handler->clear();
        synth_data->shift = false;

        if (env_popup)
        {
            open_env_popup(nullptr, nullptr, nullptr, nullptr, false);
        }
        else if (mfo_popup)
        {
            open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
        }
        else
        {
            close_all_subeditors();
        }

        success = true;
    }
    else if (key.getTextDescription() ==
             "ctrl + +") // key.isCurrentlyDown(KeyPress::numberPadAdd)  )
    {
        synth_data->ui_scale_factor = synth_data->ui_scale_factor + 0.1;
        update_size();
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + -")
    {
        synth_data->ui_scale_factor = synth_data->ui_scale_factor - 0.1;
        update_size();
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + H")
    {
        update_tooltip_handling(true);
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + E")
    {
        synth_data->animate_envs ^= true;
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + R")
    {
        synth_data->animate_sliders ^= true;
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + W")
    {
        overlay->setOpaque(false);
        overlay->setVisible(!overlay->isVisible());
        overlay->setAlwaysOnTop(overlay->isVisible());
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + I" or key.getTextDescription() == "ctrl + A")
    {
        credits->setOpaque(false);
        credits->setVisible(!credits->isVisible());
        credits->setAlwaysOnTop(credits->isVisible());
        success = true;
    }
    else if (key.getTextDescription() == "ctrl + M")
    {
        midi_control_handler->toggle_midi_learn();
        const juce::Desktop &desktop = juce::Desktop::getInstance();
        bool found = false;
        for (int i = 0; i != desktop.getNumMouseSources(); ++i)
        {
            const juce::MouseInputSource *const mi = desktop.getMouseSource(i);
            juce::Component *const c = mi->getComponentUnderMouse();
            if (c)
            {
                if (Monique_Ui_DualSlider *slider = dynamic_cast<Monique_Ui_DualSlider *>(c))
                {
                    jassert(false);
                    // slider->slider_value->triggerClick();

                    found = true;
                    break;
                }
                // SUPPORT FOR SINGLE BUTTONS
                else if (juce::TextButton *button = dynamic_cast<juce::TextButton *>(c))
                {
                    bool trigger_click = true;
                    if (button == button_programm_left.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_right.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_load.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_scratch.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_replace.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_new.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_rename.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_programm_delete.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_open_midi_io_settings.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_open_config2.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_open_oszi.get())
                    {
                        trigger_click = false;
                        found = true;
                    }
                    else if (button == button_vers_info.get())
                    {
                        trigger_click = false;
                        found = true;
                    }

                    if (trigger_click)
                    {
                        buttonClicked(button);
                        found = true;
                    }
                }
                else if (juce::Slider *slider = dynamic_cast<juce::Slider *>(c))
                {
                    // slider->triggerClick();
                    found = true;
                }
                else
                {
                    juce::Component *parent = c->getParentComponent();
                    if (parent)
                    {
                        do
                        {
                            if (Monique_Ui_DualSlider *slider =
                                    dynamic_cast<Monique_Ui_DualSlider *>(parent))
                            {
                                // slider->slider_value->triggerClick();

                                found = true;
                                break;
                            }
                        } while ((parent = parent->getParentComponent()));
                    }
                }

                if (found)
                {
                    break;
                }
            }

            success = true;
        }
    }
    else if (key.getTextDescription() == "F11")
    {
        if (this->getPeer() != nullptr)
        {
            if (juce::Desktop::getInstance().getKioskModeComponent())
            {
                juce::Desktop::getInstance().setKioskModeComponent(nullptr);
            }
            else
            {
                juce::Desktop::getInstance().setKioskModeComponent(this);
            }
            success = true;
        }
    }

    return success; // Return true if your handler uses this key event, or false to allow it to be
                    // passed-on.
}

bool Monique_Ui_Mainwindow::keyStateChanged(const bool isKeyDown)
{
    return false; // Return true if your handler uses this key event, or false to allow it to be
                  // passed-on.
}

void Monique_Ui_Mainwindow::modifierKeysChanged(const juce::ModifierKeys &modifiers)
{
    if (!dynamic_cast<juce::TextEditor *>(
            getCurrentlyFocusedComponent())) // !combo_programm->isTextEditable() )
    {
        if (modifiers.isShiftDown() != synth_data->shift)
        {
            synth_data->shift = modifiers.isShiftDown();
            look_and_feel->show_values_always = false; // hide old state
            show_ctrl_state();
            // look_and_feel->show_values_always = modifiers.isCtrlDown(); // reactivate state
        }
        else if (modifiers.isCtrlDown() != look_and_feel->show_values_always)
        {
            look_and_feel->show_values_always = modifiers.isCtrlDown();
            update_tooltip_handling(look_and_feel->show_values_always);
        }
    }

    // else
    //    Component::modifierKeysChanged( modifiers );
}

void Monique_Ui_Mainwindow::close_all_subeditors()
{
    overlay->setVisible(false);
    credits->setVisible(false);
    editor_midiio = nullptr;
    editor_morph = nullptr;
    editor_global_settings = nullptr;
    popup = nullptr;
    option_popup = nullptr;
    playback = nullptr;
    vers_info = nullptr;

    if (amp_painter)
    {
        audio_processor->amp_painter = nullptr;
        delete amp_painter;
        amp_painter = nullptr;
    }
}

void Monique_Ui_Mainwindow::open_mfo_popup(LFOData *const mfo_data_, juce::Button *const for_comp_,
                                           Monique_Ui_DualSlider *slider_,
                                           COLOUR_THEMES theme_) noexcept
{
    TURN_BUTTON_OFF(button_edit_mfo_1)
    TURN_BUTTON_OFF(button_edit_mfo_2)
    TURN_BUTTON_OFF(button_edit_mfo_3)
    TURN_BUTTON_OFF(button_edit_mfo_4)
    TURN_BUTTON_OFF(button_edit_lfo_1)
    TURN_BUTTON_OFF(button_edit_lfo_2)
    TURN_BUTTON_OFF(button_edit_lfo_3)

    if (!mfo_popup)
    {
        if (mfo_data_)
        {
            juce::Array<juce::Component *> comps_to_observe;
            if (popup) // IGNORE POPUP
            {
                removeChildComponent(popup.get());
            }
            if (option_popup) // IGNORE POPUP
            {
                removeChildComponent(option_popup.get());
            }
            if (env_popup) // IGNORE POPUP
            {
                removeChildComponent(env_popup.get());
            }
            for (int i = 0; i != getNumChildComponents(); ++i)
            {
                comps_to_observe.add(getChildComponent(i));
            }
            comps_to_observe.removeFirstMatchingValue(for_comp_);
            comps_to_observe.removeFirstMatchingValue(button_open_morph.get());

            if (editor_morph)
            {
                for (int i = 0; i != editor_morph->getNumChildComponents(); ++i)
                {
                    comps_to_observe.removeFirstMatchingValue(editor_morph->getChildComponent(i));
                }
                comps_to_observe.removeFirstMatchingValue(editor_morph.get());
            }
            if (amp_painter)
            {
                for (int i = 0; i != amp_painter->getNumChildComponents(); ++i)
                {
                    comps_to_observe.removeFirstMatchingValue(amp_painter->getChildComponent(i));
                }
                comps_to_observe.removeFirstMatchingValue(amp_painter);
            }
            comps_to_observe.removeFirstMatchingValue(button_open_oszi.get());
            if (vers_info)
            {
                for (int i = 0; i != vers_info->getNumChildComponents(); ++i)
                {
                    comps_to_observe.removeFirstMatchingValue(vers_info->getChildComponent(i));
                }
                comps_to_observe.removeFirstMatchingValue(vers_info.get());
            }
            comps_to_observe.removeFirstMatchingValue(button_vers_info.get());

            comps_to_observe.add(this);
            if (popup)
            {
                addChildComponent(popup.get());
            }
            if (option_popup)
            {
                addChildComponent(option_popup.get());
            }
            if (env_popup)
            {
                addChildComponent(env_popup.get());
            }

            mfo_popup =
                std::make_unique<Monique_Ui_MFOPopup>(ui_refresher, this, mfo_data_, theme_);
            addChildComponent(mfo_popup.get());
            mfo_popup->set_element_to_show(for_comp_, slider_);
            mfo_popup->set_clickable_components(comps_to_observe);
            global_slider_settings_changed(mfo_popup.get());
            if (for_comp_)
            {
                TURN_BUTTON_ON(for_comp_)
            }
            resize_subeditors();
            mfo_popup->refresh();
            mfo_popup->setVisible(true);
        }
    }
    else
    {
        if (mfo_popup->is_open_for() == mfo_data_)
        {
            if (for_comp_)
            {
                TURN_BUTTON_OFF(for_comp_)
            }
            mfo_popup = nullptr;
        }
        else if (mfo_data_ == nullptr)
        {
            if (for_comp_)
            {
                TURN_BUTTON_OFF(for_comp_)
            }
            mfo_popup = nullptr;
        }
        else
        {
            mfo_popup = nullptr;
            open_mfo_popup(mfo_data_, for_comp_, slider_, theme_);
        }
    }
}

void Monique_Ui_Mainwindow::open_env_popup(ENVData *const env_data_, Parameter *const sustain_,
                                           juce::Button *const for_comp_,
                                           Monique_Ui_DualSlider *slider_,
                                           bool has_negative_sustain_) noexcept
{

    TURN_BUTTON_OFF(button_edit_input_env_1_1)
    TURN_BUTTON_OFF(button_edit_input_env_1_2)
    TURN_BUTTON_OFF(button_edit_input_env_1_3)
    TURN_BUTTON_OFF(button_edit_input_env_2_1)
    TURN_BUTTON_OFF(button_edit_input_env_2_2)
    TURN_BUTTON_OFF(button_edit_input_env_2_3)
    TURN_BUTTON_OFF(button_edit_input_env_3_1)
    TURN_BUTTON_OFF(button_edit_input_env_3_2)
    TURN_BUTTON_OFF(button_edit_input_env_3_3)

    TURN_BUTTON_OFF(button_edit_input_env_band_1)
    TURN_BUTTON_OFF(button_edit_input_env_band_2)
    TURN_BUTTON_OFF(button_edit_input_env_band_3)
    TURN_BUTTON_OFF(button_edit_input_env_band_4)
    TURN_BUTTON_OFF(button_edit_input_env_band_5)
    TURN_BUTTON_OFF(button_edit_input_env_band_6)
    TURN_BUTTON_OFF(button_edit_input_env_band_7)

    if (!env_popup)
    {
        if (env_data_)
        {
            juce::Array<juce::Component *> comps_to_observe;
            if (popup) // IGNORE POPUP
            {
                removeChildComponent(popup.get());
            }
            if (option_popup) // IGNORE POPUP
            {
                removeChildComponent(option_popup.get());
            }
            if (mfo_popup) // IGNORE POPUP
            {
                removeChildComponent(mfo_popup.get());
            }
            for (int i = 0; i != getNumChildComponents(); ++i)
            {
                comps_to_observe.add(getChildComponent(i));
            }
            comps_to_observe.removeFirstMatchingValue(for_comp_);

            if (amp_painter)
            {
                for (int i = 0; i != amp_painter->getNumChildComponents(); ++i)
                {
                    comps_to_observe.removeFirstMatchingValue(amp_painter->getChildComponent(i));
                }
                comps_to_observe.removeFirstMatchingValue(amp_painter);
            }
            comps_to_observe.removeFirstMatchingValue(button_open_oszi.get());

            if (vers_info)
            {
                for (int i = 0; i != vers_info->getNumChildComponents(); ++i)
                {
                    comps_to_observe.removeFirstMatchingValue(vers_info->getChildComponent(i));
                }
                comps_to_observe.removeFirstMatchingValue(vers_info.get());
            }
            comps_to_observe.removeFirstMatchingValue(button_vers_info.get());

            comps_to_observe.add(this);
            if (popup)
            {
                addChildComponent(popup.get());
            }
            if (option_popup)
            {
                addChildComponent(option_popup.get());
            }
            if (mfo_popup)
            {
                addChildComponent(mfo_popup.get());
            }

            env_popup = std::make_unique<Monique_Ui_ENVPopup>(
                ui_refresher, this, env_data_, sustain_, for_comp_->getX() < getWidth() / 2,
                has_negative_sustain_);
            addChildComponent(env_popup.get());
            env_popup->set_element_to_show(for_comp_, slider_);
            env_popup->set_clickable_components(comps_to_observe);
            global_slider_settings_changed(env_popup.get()); // UPDATE TO ONLY ROTARY

            if (for_comp_)
            {
                TURN_BUTTON_ON(for_comp_)
            }
            resize_subeditors();
            env_popup->refresh();
            env_popup->setVisible(true);
        }
    }
    else
    {
        if (env_popup->is_open_for() == env_data_)
        {
            if (for_comp_)
            {
                TURN_BUTTON_OFF(for_comp_)
            }
            env_popup = nullptr;
        }
        else if (env_data_ == nullptr)
        {
            if (for_comp_)
            {
                TURN_BUTTON_OFF(for_comp_)
            }
            env_popup = nullptr;
        }
        else
        {
            env_popup = nullptr;
            open_env_popup(env_data_, sustain_, for_comp_, slider_, has_negative_sustain_);
        }
    }
}
void Monique_Ui_Mainwindow::open_mfo_popup(Monique_Ui_DualSlider *dual_slider_) noexcept
{
    if (dual_slider_ == morpher_1.get())
    {
        buttonClicked(button_edit_mfo_1.get());
    }
    else if (dual_slider_ == morpher_2.get())
    {
        buttonClicked(button_edit_mfo_2.get());
    }
    else if (dual_slider_ == morpher_3.get())
    {
        buttonClicked(button_edit_mfo_4.get());
    }
    else if (dual_slider_ == morpher_4.get())
    {
        buttonClicked(button_edit_mfo_3.get());
    }
    else if (dual_slider_ == lfo_1.get())
    {
        buttonClicked(button_edit_lfo_1.get());
    }
    else if (dual_slider_ == lfo_2.get())
    {
        buttonClicked(button_edit_lfo_2.get());
    }
    else if (dual_slider_ == lfo_3.get())
    {
        buttonClicked(button_edit_lfo_3.get());
    }
}
void Monique_Ui_Mainwindow::open_env_popup(Monique_Ui_DualSlider *dual_slider_) noexcept
{
    if (dual_slider_ == flt_input_1.get())
    {
        buttonClicked(button_edit_input_env_1_1.get());
    }
    else if (dual_slider_ == flt_input_2.get())
    {
        buttonClicked(button_edit_input_env_1_2.get());
    }
    else if (dual_slider_ == flt_input_3.get())
    {
        buttonClicked(button_edit_input_env_1_3.get());
    }
    else if (dual_slider_ == flt_input_6.get())
    {
        buttonClicked(button_edit_input_env_2_1.get());
    }
    else if (dual_slider_ == flt_input_7.get())
    {
        buttonClicked(button_edit_input_env_2_2.get());
    }
    else if (dual_slider_ == flt_input_8.get())
    {
        buttonClicked(button_edit_input_env_2_3.get());
    }
    else if (dual_slider_ == flt_input_11.get())
    {
        buttonClicked(button_edit_input_env_3_1.get());
    }
    else if (dual_slider_ == flt_input_12.get())
    {
        buttonClicked(button_edit_input_env_3_2.get());
    }
    else if (dual_slider_ == flt_input_13.get())
    {
        buttonClicked(button_edit_input_env_3_3.get());
    }
    else if (dual_slider_ == eq_1.get())
    {
        buttonClicked(button_edit_input_env_band_1.get());
    }
    else if (dual_slider_ == eq_2.get())
    {
        buttonClicked(button_edit_input_env_band_2.get());
    }
    else if (dual_slider_ == eq_3.get())
    {
        buttonClicked(button_edit_input_env_band_3.get());
    }
    else if (dual_slider_ == eq_4.get())
    {
        buttonClicked(button_edit_input_env_band_4.get());
    }
    else if (dual_slider_ == eq_5.get())
    {
        buttonClicked(button_edit_input_env_band_5.get());
    }
    else if (dual_slider_ == eq_6.get())
    {
        buttonClicked(button_edit_input_env_band_6.get());
    }
    else if (dual_slider_ == eq_7.get())
    {
        buttonClicked(button_edit_input_env_band_7.get());
    }
}
void Monique_Ui_Mainwindow::open_option_popup(juce::Component *const for_comp_,
                                              BoolParameter *param_a_, BoolParameter *param_b_,
                                              BoolParameter *param_arp_or_seq_,
                                              juce::StringRef text_a_, juce::StringRef text_b_,
                                              juce::StringRef tool_tip_a_,
                                              juce::StringRef tool_tip_b_) noexcept
{
    if (!for_comp_ or !param_a_ or !param_b_)
    {
        if (option_popup)
        {
            if (!option_popup->isMouseOver(true))
            {
                option_popup = nullptr;
            }
        }
    }
    else
    {
        if (option_popup)
        {
            if (juce::Desktop::getInstance().getMainMouseSource().getComponentUnderMouse() !=
                option_popup.get())
            {
                option_popup = nullptr;
                if (for_comp_ && param_a_ && param_b_)
                {
                    option_popup = std::make_unique<Monique_Ui_OptionPopup>(ui_refresher, this,
                                                                            param_a_, param_b_);
                    option_popup->set_element_to_show(for_comp_);
                    option_popup->set_infos(text_a_, text_b_, tool_tip_a_, tool_tip_b_);
                    resize_subeditors();
                    option_popup->setVisible(true);
                }
            }
        }
        else
        {
            option_popup =
                std::make_unique<Monique_Ui_OptionPopup>(ui_refresher, this, param_a_, param_b_);
            option_popup->set_element_to_show(for_comp_);
            option_popup->set_infos(text_a_, text_b_, tool_tip_a_, tool_tip_b_);
            resize_subeditors();
            option_popup->setVisible(true);
        }
    }
}

bool Monique_Ui_Mainwindow::handle_keep_env_open(
    ModulationSliderConfigBase *const caller_config_) noexcept
{
    bool keeps_open = false;
    if (env_popup)
    {
        if (!caller_config_->action_keep_env_pop_open_for(env_popup->is_open_for()))
        {
            if (synth_data->auto_close_env_popup)
            {
                open_env_popup(nullptr, nullptr, nullptr, nullptr, false);
            }
        }
        else
        {
            keeps_open = true;
        }
    }

    return keeps_open;
}
bool Monique_Ui_Mainwindow::handle_keep_mfo_open(
    ModulationSliderConfigBase *const caller_config_) noexcept
{
    bool keeps_open = false;
    if (mfo_popup)
    {
        if (!caller_config_->action_keep_env_pop_open_for(mfo_popup->is_open_for()))
        {
            if (synth_data->auto_close_env_popup)
            {
                open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
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
#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
    if (editor_midiio)
    {
        addChildComponent(editor_midiio);
        editor_midiio->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                                 keyboard->getHeight());
    }
#endif

    if (playback)
    {
        addChildComponent(playback.get());
        playback->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                            keyboard->getHeight());
    }

    if (editor_morph)
    {
        addChildComponent(editor_morph.get());
        editor_morph->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                                keyboard->getHeight());
    }

    if (editor_global_settings)
    {
        addChildComponent(editor_global_settings.get());
        editor_global_settings->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                                          keyboard->getHeight());
    }

    if (popup)
    {
        addChildComponent(popup.get());
        popup->setSize(popup->original_w * (1.0f / original_w * getWidth()),
                       popup->original_h * (1.0f / original_h * getHeight()));
        popup->update_positions();
    }

    if (env_popup)
    {
        addChildComponent(env_popup.get());
        env_popup->setSize(env_popup->original_w * (1.0f / original_w * getWidth()),
                           env_popup->original_h * (1.0f / original_h * getHeight()));
        env_popup->update_positions();
        global_slider_settings_changed(env_popup.get());
    }

    if (mfo_popup)
    {
        addChildComponent(mfo_popup.get());
        mfo_popup->setSize(mfo_popup->original_w * (1.0f / original_w * getWidth()),
                           mfo_popup->original_h * (1.0f / original_h * getHeight()));
        mfo_popup->update_positions();
        global_slider_settings_changed(mfo_popup.get());
    }

    if (option_popup)
    {
        addChildComponent(option_popup.get());
        option_popup->setSize(option_popup->original_w * (1.0f / original_w * getWidth()),
                              option_popup->original_h * (1.0f / original_h * getHeight()));
        option_popup->update_positions();
    }

    if (amp_painter)
    {
        addChildComponent(amp_painter);
        amp_painter->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                               keyboard->getHeight());
        // amp_painter->setBounds(0, 50, getWidth(), getHeight()-50 );
    }

    if (vers_info)
    {
        addChildComponent(vers_info.get());
        vers_info->setBounds(keyboard->getX(), keyboard->getY(), keyboard->getWidth(),
                             keyboard->getHeight());
    }

    if (credits)
    {
        addChildComponent(credits.get());
        credits->setBounds(this->getWidth() / 2 - credits->getWidth() / 2,
                           this->getHeight() / 2 - credits->getHeight() / 2 -
                               keyboard->getHeight() / 2,
                           credits->getWidth(), credits->getHeight());
        // amp_painter->setBounds(0, 50, getWidth(), getHeight()-50 );
    }

    resizer->toFront(true);
}

void Monique_Ui_Mainwindow::open_midi_editor_if_closed() noexcept
{
#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
    if (!editor_midiio)
    {
        close_all_subeditors();

        addChildComponent(editor_midiio =
                              new Monique_Ui_MidiIO(ui_refresher, audio_processor, this));
        resize_subeditors();
        editor_midiio->setVisible(true);
    }
#else
    if (popup)
    {
        show_info_popup(nullptr, nullptr);
    }
    else
    {
        midi_control_handler->toggle_midi_learn();
    }
#endif
}

void Monique_Ui_Mainwindow::flash_midi_editor_button() noexcept { flash_counter = 30; }

void Monique_Ui_Mainwindow::mouseEnter(const juce::MouseEvent &event)
{
    if (option_popup)
    {
        open_option_popup(nullptr, nullptr, nullptr, nullptr, "", "", "", "");
    }
}

void Monique_Ui_Mainwindow::open_env_or_lfo_popup_by_midi(Parameter *param_) noexcept
{
    if (IS_MIDI_LEARN)
    {
        return;
    }

    if (param_ == &synth_data->midi_lfo_popup)
    {
        switch (synth_data->midi_lfo_popup)
        {
        case 9:
        case 5:
        case 0:
            open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
            break;
        case 1:
            buttonClicked(button_edit_mfo_1.get());
            break;
        case 2:
            buttonClicked(button_edit_mfo_2.get());
            break;
        case 3:
            buttonClicked(button_edit_mfo_3.get());
            break;
        case 4:
            buttonClicked(button_edit_mfo_4.get());
            break;
        case 6:
            buttonClicked(button_edit_lfo_1.get());
            break;
        case 7:
            buttonClicked(button_edit_lfo_2.get());
            break;
        case 8:
            buttonClicked(button_edit_lfo_3.get());
            break;
        }
    }
    if (param_ == &synth_data->midi_env_popup)
    {
        switch (synth_data->midi_env_popup)
        {
        case 10:
        case 18:
        case 0:
            open_env_popup(nullptr, nullptr, nullptr, nullptr, false);
            break;
        case 1:
            buttonClicked(button_edit_input_env_1_1.get());
            break;
        case 2:
            buttonClicked(button_edit_input_env_1_2.get());
            break;
        case 3:
            buttonClicked(button_edit_input_env_1_3.get());
            break;
        case 4:
            buttonClicked(button_edit_input_env_2_1.get());
            break;
        case 5:
            buttonClicked(button_edit_input_env_2_2.get());
            break;
        case 6:
            buttonClicked(button_edit_input_env_2_3.get());
            break;
        case 7:
            buttonClicked(button_edit_input_env_3_1.get());
            break;
        case 8:
            buttonClicked(button_edit_input_env_3_2.get());
            break;
        case 9:
            buttonClicked(button_edit_input_env_3_3.get());
            break;
        case 11:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_1.get());
            break;
        case 12:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_2.get());
            break;
        case 13:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_3.get());
            break;
        case 14:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_4.get());
            break;
        case 15:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_5.get());
            break;
        case 16:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_6.get());
            break;
        case 17:
            switch_finalizer_tab(false);
            buttonClicked(button_edit_input_env_band_7.get());
            break;
        }
    }
}

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: empty_png, 159, "../Images/empty.png"
static const unsigned char resource_Monique_Ui_Mainwindow_empty_png[] = {
    137, 80,  78,  71,  13,  10,  26,  10,  0,   0,   0,   13,  73,  72,  68,  82,  0,   0,
    0,   1,   0,   0,   0,   1,   8,   4,   0,   0,   0,   181, 28,  12,  2,   0,   0,   0,
    2,   98,  75,  71,  68,  0,   0,   170, 141, 35,  50,  0,   0,   0,   9,   112, 72,  89,
    115, 0,   0,   11,  19,  0,   0,   11,  19,  1,   0,   154, 156, 24,  0,   0,   0,   7,
    116, 73,  77,  69,  7,   223, 9,   18,  16,  13,  44,  67,  236, 72,  225, 0,   0,   0,
    25,  116, 69,  88,  116, 67,  111, 109, 109, 101, 110, 116, 0,   67,  114, 101, 97,  116,
    101, 100, 32,  119, 105, 116, 104, 32,  71,  73,  77,  80,  87,  129, 14,  23,  0,   0,
    0,   11,  73,  68,  65,  84,  8,   215, 99,  248, 255, 31,  0,   3,   0,   1,   255, 100,
    93,  114, 245, 0,   0,   0,   0,   73,  69,  78,  68,  174, 66,  96,  130, 0,   0};

const char *Monique_Ui_Mainwindow::empty_png =
    (const char *)resource_Monique_Ui_Mainwindow_empty_png;
const int Monique_Ui_Mainwindow::empty_pngSize = 159;

// JUCER_RESOURCE: _01hintergrundalles_svg, 23727,
// "../Files/STYLE/Monique-Hintergrund-1/01Hintergrund-Alles.svg"
static const unsigned char resource_Monique_Ui_Mainwindow__01hintergrundalles_svg[] = {
    60,  63,  120, 109, 108, 32,  118, 101, 114, 115, 105, 111, 110, 61,  34,  49,  46,  48,  34,
    32,  101, 110, 99,  111, 100, 105, 110, 103, 61,  34,  85,  84,  70,  45,  56,  34,  32,  115,
    116, 97,  110, 100, 97,  108, 111, 110, 101, 61,  34,  110, 111, 34,  63,  62,  10,  60,  115,
    118, 103, 10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  100, 99,  61,  34,  104, 116, 116,
    112, 58,  47,  47,  112, 117, 114, 108, 46,  111, 114, 103, 47,  100, 99,  47,  101, 108, 101,
    109, 101, 110, 116, 115, 47,  49,  46,  49,  47,  34,  10,  32,  32,  32,  120, 109, 108, 110,
    115, 58,  99,  99,  61,  34,  104, 116, 116, 112, 58,  47,  47,  99,  114, 101, 97,  116, 105,
    118, 101, 99,  111, 109, 109, 111, 110, 115, 46,  111, 114, 103, 47,  110, 115, 35,  34,  10,
    32,  32,  32,  120, 109, 108, 110, 115, 58,  114, 100, 102, 61,  34,  104, 116, 116, 112, 58,
    47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  49,  57,  57,  57,  47,  48,
    50,  47,  50,  50,  45,  114, 100, 102, 45,  115, 121, 110, 116, 97,  120, 45,  110, 115, 35,
    34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  115, 118, 103, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  120, 108, 105, 110,
    107, 61,  34,  104, 116, 116, 112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114,
    103, 47,  49,  57,  57,  57,  47,  120, 108, 105, 110, 107, 34,  10,  32,  32,  32,  120, 109,
    108, 110, 115, 58,  115, 111, 100, 105, 112, 111, 100, 105, 61,  34,  104, 116, 116, 112, 58,
    47,  47,  115, 111, 100, 105, 112, 111, 100, 105, 46,  115, 111, 117, 114, 99,  101, 102, 111,
    114, 103, 101, 46,  110, 101, 116, 47,  68,  84,  68,  47,  115, 111, 100, 105, 112, 111, 100,
    105, 45,  48,  46,  100, 116, 100, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  105,
    110, 107, 115, 99,  97,  112, 101, 61,  34,  104, 116, 116, 112, 58,  47,  47,  119, 119, 119,
    46,  105, 110, 107, 115, 99,  97,  112, 101, 46,  111, 114, 103, 47,  110, 97,  109, 101, 115,
    112, 97,  99,  101, 115, 47,  105, 110, 107, 115, 99,  97,  112, 101, 34,  10,  32,  32,  32,
    119, 105, 100, 116, 104, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  104, 101, 105,
    103, 104, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  118, 105, 101, 119, 66,
    111, 120, 61,  34,  48,  32,  48,  32,  49,  50,  55,  52,  32,  56,  52,  48,  34,  10,  32,
    32,  32,  118, 101, 114, 115, 105, 111, 110, 61,  34,  49,  46,  49,  34,  10,  32,  32,  32,
    120, 109, 108, 58,  115, 112, 97,  99,  101, 61,  34,  112, 114, 101, 115, 101, 114, 118, 101,
    34,  10,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 45,  114, 117,
    108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  99,  108, 105, 112, 45,  114, 117, 108,
    101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105,
    110, 101, 99,  97,  112, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107, 101, 45,
    108, 105, 110, 101, 106, 111, 105, 110, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111,
    107, 101, 45,  109, 105, 116, 101, 114, 108, 105, 109, 105, 116, 58,  49,  46,  52,  49,  52,
    50,  49,  59,  34,  10,  32,  32,  32,  105, 100, 61,  34,  115, 118, 103, 50,  34,  10,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  118, 101, 114, 115, 105, 111, 110, 61,
    34,  48,  46,  52,  56,  46,  52,  32,  114, 57,  57,  51,  57,  34,  10,  32,  32,  32,  115,
    111, 100, 105, 112, 111, 100, 105, 58,  100, 111, 99,  110, 97,  109, 101, 61,  34,  48,  49,
    72,  105, 110, 116, 101, 114, 103, 114, 117, 110, 100, 45,  65,  108, 108, 101, 115, 46,  115,
    118, 103, 34,  62,  60,  109, 101, 116, 97,  100, 97,  116, 97,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  109, 101, 116, 97,  100, 97,  116, 97,  50,  56,  53,  34,  62,  60,  114,
    100, 102, 58,  82,  68,  70,  62,  60,  99,  99,  58,  87,  111, 114, 107, 10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  114, 100, 102, 58,  97,  98,  111, 117, 116, 61,  34,  34,  62,
    60,  100, 99,  58,  102, 111, 114, 109, 97,  116, 62,  105, 109, 97,  103, 101, 47,  115, 118,
    103, 43,  120, 109, 108, 60,  47,  100, 99,  58,  102, 111, 114, 109, 97,  116, 62,  60,  100,
    99,  58,  116, 121, 112, 101, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114,
    100, 102, 58,  114, 101, 115, 111, 117, 114, 99,  101, 61,  34,  104, 116, 116, 112, 58,  47,
    47,  112, 117, 114, 108, 46,  111, 114, 103, 47,  100, 99,  47,  100, 99,  109, 105, 116, 121,
    112, 101, 47,  83,  116, 105, 108, 108, 73,  109, 97,  103, 101, 34,  32,  47,  62,  60,  47,
    99,  99,  58,  87,  111, 114, 107, 62,  60,  47,  114, 100, 102, 58,  82,  68,  70,  62,  60,
    47,  109, 101, 116, 97,  100, 97,  116, 97,  62,  60,  115, 111, 100, 105, 112, 111, 100, 105,
    58,  110, 97,  109, 101, 100, 118, 105, 101, 119, 10,  32,  32,  32,  32,  32,  112, 97,  103,
    101, 99,  111, 108, 111, 114, 61,  34,  35,  102, 102, 102, 102, 102, 102, 34,  10,  32,  32,
    32,  32,  32,  98,  111, 114, 100, 101, 114, 99,  111, 108, 111, 114, 61,  34,  35,  54,  54,
    54,  54,  54,  54,  34,  10,  32,  32,  32,  32,  32,  98,  111, 114, 100, 101, 114, 111, 112,
    97,  99,  105, 116, 121, 61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  111, 98,  106, 101,
    99,  116, 116, 111, 108, 101, 114, 97,  110, 99,  101, 61,  34,  49,  48,  34,  10,  32,  32,
    32,  32,  32,  103, 114, 105, 100, 116, 111, 108, 101, 114, 97,  110, 99,  101, 61,  34,  49,
    48,  34,  10,  32,  32,  32,  32,  32,  103, 117, 105, 100, 101, 116, 111, 108, 101, 114, 97,
    110, 99,  101, 61,  34,  49,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,
    97,  112, 101, 58,  112, 97,  103, 101, 111, 112, 97,  99,  105, 116, 121, 61,  34,  48,  34,
    10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  112, 97,  103, 101,
    115, 104, 97,  100, 111, 119, 61,  34,  50,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  119, 105, 100, 116, 104, 61,
    34,  49,  57,  50,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112,
    101, 58,  119, 105, 110, 100, 111, 119, 45,  104, 101, 105, 103, 104, 116, 61,  34,  49,  48,
    54,  48,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  110, 97,  109, 101, 100, 118,
    105, 101, 119, 50,  56,  51,  34,  10,  32,  32,  32,  32,  32,  115, 104, 111, 119, 103, 114,
    105, 100, 61,  34,  102, 97,  108, 115, 101, 34,  10,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  122, 111, 111, 109, 61,  34,  49,  46,  49,  50,  51,  56,  48,
    57,  53,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,
    120, 61,  34,  54,  50,  56,  46,  49,  48,  55,  56,  49,  34,  10,  32,  32,  32,  32,  32,
    105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  121, 61,  34,  53,  51,  50,  46,  53,  53,
    50,  56,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119,
    105, 110, 100, 111, 119, 45,  120, 61,  34,  45,  51,  34,  10,  32,  32,  32,  32,  32,  105,
    110, 107, 115, 99,  97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  121, 61,  34,  45,
    51,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119, 105,
    110, 100, 111, 119, 45,  109, 97,  120, 105, 109, 105, 122, 101, 100, 61,  34,  49,  34,  10,
    32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  117, 114, 114, 101,
    110, 116, 45,  108, 97,  121, 101, 114, 61,  34,  115, 118, 103, 50,  34,  32,  47,  62,  60,
    103, 10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 50,  50,  51,  34,  10,  32,  32,
    32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105,
    120, 40,  48,  46,  55,  54,  52,  57,  56,  57,  50,  50,  44,  48,  44,  48,  44,  48,  46,
    57,  56,  48,  56,  49,  51,  44,  50,  51,  56,  46,  53,  52,  57,  55,  49,  44,  49,  53,
    46,  53,  49,  54,  50,  51,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116,
    111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 50,  50,  53,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  110,
    111, 110, 101, 59,  115, 116, 114, 111, 107, 101, 58,  117, 114, 108, 40,  35,  95,  76,  105,
    110, 101, 97,  114, 54,  41,  59,  115, 116, 114, 111, 107, 101, 45,  119, 105, 100, 116, 104,
    58,  49,  51,  46,  50,  55,  57,  57,  57,  57,  55,  51,  112, 120, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  50,  53,  53,  46,  55,  44,  49,  48,  52,
    46,  50,  57,  49,  32,  48,  44,  51,  57,  51,  46,  55,  56,  52,  32,  45,  49,  48,  51,
    49,  46,  48,  50,  56,  44,  48,  32,  48,  44,  51,  54,  46,  48,  55,  49,  34,  32,  47,
    62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102,
    111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  50,  46,  50,  48,  52,  51,  56,
    55,  44,  48,  44,  48,  44,  48,  46,  56,  54,  56,  50,  52,  56,  44,  51,  53,  54,  46,
    49,  48,  48,  57,  51,  44,  51,  51,  56,  46,  50,  50,  48,  57,  41,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 49,  48,  34,  62,  60,  112, 97,  116, 104, 10,  32,
    32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,
    52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,
    51,  46,  48,  50,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,  45,  54,  46,  55,  53,
    44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  45,  49,  51,  48,  46,  55,  48,  53,
    44,  48,  32,  67,  32,  54,  46,  48,  50,  53,  44,  50,  56,  46,  53,  49,  51,  32,  51,
    44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108, 32,
    48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,  50,
    32,  51,  46,  48,  50,  53,  44,  49,  52,  46,  55,  56,  56,  32,  54,  46,  55,  53,  49,
    44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  51,  48,  46,  55,  48,  53,  44,  48,
    32,  99,  32,  51,  46,  55,  50,  54,  44,  48,  32,  54,  46,  55,  53,  44,  45,  54,  46,
    54,  50,  54,  32,  54,  46,  55,  53,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,
    48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  10,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,
    98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104,
    49,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101,
    58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114,
    101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,
    50,  46,  57,  51,  48,  50,  53,  50,  51,  44,  48,  44,  48,  44,  51,  46,  57,  49,  48,
    57,  55,  44,  50,  55,  55,  46,  55,  55,  57,  57,  54,  44,  49,  55,  52,  46,  49,  52,
    41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  52,  34,  62,  60,  114,
    101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,
    54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,
    50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,
    34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    104, 101, 105, 103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,
    117, 114, 108, 40,  35,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116,
    51,  51,  52,  56,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114,
    101, 99,  116, 49,  54,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,
    32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120,
    40,  48,  46,  53,  54,  53,  48,  57,  54,  44,  48,  44,  48,  44,  48,  46,  56,  54,  56,
    53,  56,  56,  44,  55,  53,  51,  46,  51,  56,  44,  51,  51,  56,  46,  50,  49,  49,  50,
    41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  56,  34,  62,  60,  112,
    97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  55,
    46,  50,  48,  54,  44,  51,  56,  46,  51,  53,  32,  99,  32,  48,  44,  45,  53,  46,  52,
    50,  57,  32,  45,  54,  46,  55,  55,  53,  44,  45,  57,  46,  56,  51,  55,  32,  45,  49,
    53,  46,  49,  50,  49,  44,  45,  57,  46,  56,  51,  55,  32,  108, 32,  45,  49,  49,  51,
    46,  57,  54,  52,  44,  48,  32,  67,  32,  57,  46,  55,  55,  54,  44,  50,  56,  46,  53,
    49,  51,  32,  51,  44,  51,  50,  46,  57,  50,  49,  32,  51,  44,  51,  56,  46,  51,  53,
    32,  108, 32,  48,  44,  49,  50,  49,  46,  51,  53,  56,  32,  99,  32,  48,  44,  53,  46,
    52,  51,  32,  54,  46,  55,  55,  54,  44,  57,  46,  56,  51,  56,  32,  49,  53,  46,  49,
    50,  49,  44,  57,  46,  56,  51,  56,  32,  108, 32,  49,  49,  51,  46,  57,  54,  52,  44,
    48,  32,  99,  32,  56,  46,  51,  52,  54,  44,  48,  32,  49,  53,  46,  49,  50,  49,  44,
    45,  52,  46,  52,  48,  56,  32,  49,  53,  46,  49,  50,  49,  44,  45,  57,  46,  56,  51,
    56,  32,  108, 32,  48,  44,  45,  49,  50,  49,  46,  51,  53,  56,  32,  122, 34,  10,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,
    97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    112, 97,  116, 104, 50,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115,
    99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118,
    97,  116, 117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  112, 97,
    116, 104, 10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111,
    110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 50,  52,  34,
    10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,
    97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,
    50,  52,  48,  46,  48,  53,  57,  52,  44,  51,  55,  53,  46,  56,  52,  53,  48,  51,  32,
    99,  32,  48,  44,  45,  55,  46,  48,  56,  57,  52,  50,  32,  45,  53,  46,  50,  52,  53,
    57,  44,  45,  49,  50,  46,  56,  52,  52,  54,  56,  32,  45,  49,  49,  46,  55,  48,  56,
    54,  44,  45,  49,  50,  46,  56,  52,  52,  54,  56,  32,  108, 32,  45,  51,  54,  51,  46,
    57,  51,  51,  53,  50,  44,  48,  32,  99,  32,  45,  54,  46,  52,  54,  50,  55,  52,  44,
    48,  32,  45,  49,  49,  46,  55,  48,  56,  54,  55,  44,  53,  46,  55,  53,  53,  50,  54,
    32,  45,  49,  49,  46,  55,  48,  56,  54,  55,  44,  49,  50,  46,  56,  52,  52,  54,  56,
    32,  108, 32,  48,  44,  57,  54,  46,  56,  49,  48,  50,  49,  32,  99,  32,  48,  44,  55,
    46,  48,  56,  57,  52,  50,  32,  53,  46,  50,  52,  53,  57,  51,  44,  49,  50,  46,  56,
    52,  52,  54,  56,  32,  49,  49,  46,  55,  48,  56,  54,  55,  44,  49,  50,  46,  56,  52,
    52,  54,  56,  32,  108, 32,  51,  54,  51,  46,  57,  51,  51,  53,  50,  44,  48,  32,  99,
    32,  54,  46,  52,  54,  50,  55,  44,  48,  32,  49,  49,  46,  55,  48,  56,  54,  44,  45,
    53,  46,  55,  53,  53,  50,  54,  32,  49,  49,  46,  55,  48,  56,  54,  44,  45,  49,  50,
    46,  56,  52,  52,  54,  56,  32,  108, 32,  48,  44,  45,  57,  54,  46,  56,  49,  48,  50,
    49,  32,  122, 34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  48,  44,
    51,  54,  56,  46,  53,  49,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103,
    51,  52,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  120, 61,
    34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,
    34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  55,  46,  55,  48,
    53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 51,  54,  34,  32,  47,  62,  60,  47,
    103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109,
    61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  49,  56,  54,  57,  57,  44,  48,  44,
    48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  49,  55,  55,  46,  56,  54,  57,  44,  51,
    49,  52,  46,  57,  52,  57,  49,  55,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  103, 51,  56,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,
    100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,
    32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,  53,  46,  54,  49,  51,  44,
    45,  49,  52,  46,  55,  56,  56,  32,  45,  49,  50,  46,  53,  50,  55,  44,  45,  49,  52,
    46,  55,  56,  56,  32,  108, 32,  45,  49,  49,  57,  46,  49,  53,  49,  44,  48,  32,  67,
    32,  56,  46,  54,  49,  51,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,
    49,  51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,
    49,  46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,  50,  32,  53,  46,  54,
    49,  51,  44,  49,  52,  46,  55,  56,  56,  32,  49,  50,  46,  53,  50,  56,  44,  49,  52,
    46,  55,  56,  56,  32,  108, 32,  49,  49,  57,  46,  49,  53,  49,  44,  48,  32,  99,  32,
    54,  46,  57,  49,  52,  44,  48,  32,  49,  50,  46,  53,  50,  55,  44,  45,  54,  46,  54,
    50,  54,  32,  49,  50,  46,  53,  50,  55,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108,
    32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  10,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,
    49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116,
    104, 52,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112,
    101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117,
    114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,
    32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108,
    97,  116, 101, 40,  49,  57,  48,  44,  51,  54,  54,  46,  51,  54,  41,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 52,  50,  34,  62,  60,  114, 101, 99,  116, 10,  32,
    32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,
    48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104,
    116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,
    98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116,
    52,  52,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116,
    114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101,
    40,  54,  55,  54,  44,  51,  54,  54,  46,  51,  54,  41,  34,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 52,  54,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,
    32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,
    50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 52,  56,  34,
    32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  48,  49,  56,
    53,  54,  44,  48,  44,  48,  44,  49,  46,  51,  48,  51,  54,  50,  44,  45,  50,  49,  46,
    54,  52,  52,  57,  44,  57,  57,  46,  48,  55,  51,  41,  34,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 53,  48,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,
    32,  32,  32,  100, 61,  34,  109, 32,  51,  52,  56,  46,  50,  55,  49,  44,  49,  57,  50,
    46,  49,  57,  51,  32,  56,  49,  55,  46,  56,  52,  57,  44,  48,  32,  48,  44,  45,  49,
    56,  46,  49,  57,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  102, 105, 108, 108, 58,  110, 111, 110, 101, 59,  115, 116, 114, 111, 107, 101, 58,
    35,  97,  48,  98,  49,  98,  98,  59,  115, 116, 114, 111, 107, 101, 45,  119, 105, 100, 116,
    104, 58,  49,  48,  46,  50,  54,  48,  48,  48,  48,  50,  51,  112, 120, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 53,  50,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101,
    99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  32,
    47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115,
    102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  50,  46,  50,  49,  48,  53,
    53,  55,  54,  44,  48,  44,  48,  44,  48,  46,  56,  54,  56,  50,  52,  56,  44,  51,  53,
    54,  46,  48,  56,  50,  52,  50,  44,  49,  56,  54,  46,  50,  50,  48,  57,  41,  34,  10,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 53,  53,  34,  62,  60,  112, 97,  116, 104,
    10,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,
    54,  44,  52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,
    32,  45,  51,  46,  48,  50,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,  45,  54,  46,
    55,  53,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  45,  49,  51,  48,  46,  55,
    48,  53,  44,  48,  32,  67,  32,  54,  46,  48,  50,  53,  44,  50,  56,  46,  53,  49,  51,
    32,  51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,
    108, 32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,
    54,  50,  32,  51,  46,  48,  50,  53,  44,  49,  52,  46,  55,  56,  56,  32,  54,  46,  55,
    53,  49,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  51,  48,  46,  55,  48,  53,
    44,  48,  32,  99,  32,  51,  46,  55,  50,  54,  44,  48,  32,  54,  46,  55,  53,  44,  45,
    54,  46,  54,  50,  54,  32,  54,  46,  55,  53,  44,  45,  49,  52,  46,  55,  56,  56,  32,
    108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,
    98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,
    116, 104, 53,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,
    112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116,
    117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,
    32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105,
    120, 40,  50,  46,  56,  55,  51,  57,  53,  55,  55,  44,  48,  44,  48,  44,  51,  46,  57,
    48,  52,  50,  55,  44,  50,  56,  53,  46,  54,  53,  53,  54,  53,  44,  50,  50,  46,  54,
    54,  50,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 53,  57,  34,  62,
    60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,
    46,  49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,
    50,  48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116,
    104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108,
    108, 58,  117, 114, 108, 40,  35,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101,
    110, 116, 51,  51,  53,  48,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  114, 101, 99,  116, 54,  49,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,
    32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114,
    105, 120, 40,  48,  46,  53,  54,  53,  48,  57,  54,  44,  48,  44,  48,  44,  48,  46,  56,
    54,  56,  53,  56,  56,  44,  55,  53,  51,  46,  51,  56,  44,  49,  56,  54,  46,  50,  49,
    49,  50,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 54,  51,  34,  62,
    60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,
    52,  55,  46,  50,  48,  54,  44,  51,  56,  46,  51,  53,  32,  99,  32,  48,  44,  45,  53,
    46,  52,  50,  57,  32,  45,  54,  46,  55,  55,  53,  44,  45,  57,  46,  56,  51,  55,  32,
    45,  49,  53,  46,  49,  50,  49,  44,  45,  57,  46,  56,  51,  55,  32,  108, 32,  45,  49,
    49,  51,  46,  57,  54,  52,  44,  48,  32,  67,  32,  57,  46,  55,  55,  54,  44,  50,  56,
    46,  53,  49,  51,  32,  51,  44,  51,  50,  46,  57,  50,  49,  32,  51,  44,  51,  56,  46,
    51,  53,  32,  108, 32,  48,  44,  49,  50,  49,  46,  51,  53,  56,  32,  99,  32,  48,  44,
    53,  46,  52,  51,  32,  54,  46,  55,  55,  54,  44,  57,  46,  56,  51,  56,  32,  49,  53,
    46,  49,  50,  49,  44,  57,  46,  56,  51,  56,  32,  108, 32,  49,  49,  51,  46,  57,  54,
    52,  44,  48,  32,  99,  32,  56,  46,  51,  52,  54,  44,  48,  32,  49,  53,  46,  49,  50,
    49,  44,  45,  52,  46,  52,  48,  56,  32,  49,  53,  46,  49,  50,  49,  44,  45,  57,  46,
    56,  51,  56,  32,  108, 32,  48,  44,  45,  49,  50,  49,  46,  51,  53,  56,  32,  122, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  112, 97,  116, 104, 54,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117,
    114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,
    112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,
    99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 54,
    57,  34,  10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  100, 61,  34,  109,
    32,  49,  50,  52,  48,  46,  48,  53,  57,  52,  44,  50,  50,  51,  46,  56,  52,  53,  48,
    51,  32,  99,  32,  48,  44,  45,  55,  46,  48,  56,  57,  52,  50,  32,  45,  53,  46,  50,
    55,  50,  56,  44,  45,  49,  50,  46,  56,  52,  52,  54,  56,  32,  45,  49,  49,  46,  55,
    54,  56,  55,  44,  45,  49,  50,  46,  56,  52,  52,  54,  56,  32,  108, 32,  45,  51,  54,
    53,  46,  55,  57,  57,  54,  57,  44,  48,  32,  99,  32,  45,  54,  46,  52,  57,  53,  56,
    56,  44,  48,  32,  45,  49,  49,  46,  55,  54,  56,  55,  49,  44,  53,  46,  55,  53,  53,
    50,  54,  32,  45,  49,  49,  46,  55,  54,  56,  55,  49,  44,  49,  50,  46,  56,  52,  52,
    54,  56,  32,  108, 32,  48,  44,  57,  54,  46,  56,  49,  48,  50,  49,  32,  99,  32,  48,
    44,  55,  46,  48,  56,  57,  52,  50,  32,  53,  46,  50,  55,  50,  56,  51,  44,  49,  50,
    46,  56,  52,  52,  54,  56,  32,  49,  49,  46,  55,  54,  56,  55,  49,  44,  49,  50,  46,
    56,  52,  52,  54,  56,  32,  108, 32,  51,  54,  53,  46,  55,  57,  57,  54,  57,  44,  48,
    32,  99,  32,  54,  46,  52,  57,  53,  57,  44,  48,  32,  49,  49,  46,  55,  54,  56,  55,
    44,  45,  53,  46,  55,  53,  53,  50,  54,  32,  49,  49,  46,  55,  54,  56,  55,  44,  45,
    49,  50,  46,  56,  52,  52,  54,  56,  32,  108, 32,  48,  44,  45,  57,  54,  46,  56,  49,
    48,  50,  49,  32,  122, 34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,
    48,  44,  50,  49,  54,  46,  53,  49,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  103, 55,  57,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,
    120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  55,  46,
    55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108,
    101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 56,  49,  34,  32,  47,  62,
    60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  49,  56,  54,  57,  57,  44,
    48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  49,  55,  55,  46,  56,  54,  57,
    44,  49,  54,  50,  46,  57,  52,  57,  49,  55,  41,  34,  10,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  103, 56,  51,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,
    32,  32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,
    48,  49,  32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,  53,  46,  54,  49,
    51,  44,  45,  49,  52,  46,  55,  56,  56,  32,  45,  49,  50,  46,  53,  50,  55,  44,  45,
    49,  52,  46,  55,  56,  56,  32,  108, 32,  45,  49,  49,  57,  46,  49,  53,  49,  44,  48,
    32,  67,  32,  56,  46,  54,  49,  51,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,
    53,  46,  49,  51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108, 32,  48,  44,
    49,  49,  49,  46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,  50,  32,  53,
    46,  54,  49,  51,  44,  49,  52,  46,  55,  56,  56,  32,  49,  50,  46,  53,  50,  56,  44,
    49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  49,  57,  46,  49,  53,  49,  44,  48,  32,
    99,  32,  54,  46,  57,  49,  52,  44,  48,  32,  49,  50,  46,  53,  50,  55,  44,  45,  54,
    46,  54,  50,  54,  32,  49,  50,  46,  53,  50,  55,  44,  45,  49,  52,  46,  55,  56,  56,
    32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,
    48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112,
    97,  116, 104, 56,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,
    97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,
    116, 117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,
    32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110,
    115, 108, 97,  116, 101, 40,  49,  57,  48,  44,  50,  49,  52,  46,  51,  54,  41,  34,  10,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 56,  55,  34,  62,  60,  114, 101, 99,  116,
    10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,
    57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,
    46,  56,  48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105,
    103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,
    98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101,
    99,  116, 56,  57,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,
    116, 101, 40,  54,  55,  54,  44,  50,  49,  52,  46,  51,  54,  41,  34,  10,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  103, 57,  49,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,
    32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,
    54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116,
    61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,
    98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 57,
    51,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  48,
    49,  56,  53,  54,  44,  48,  44,  48,  44,  49,  46,  51,  48,  51,  54,  50,  44,  45,  50,
    49,  46,  54,  52,  52,  57,  44,  45,  53,  50,  46,  57,  50,  55,  41,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 57,  53,  34,  62,  60,  112, 97,  116, 104, 10,  32,
    32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  51,  52,  56,  46,  50,  55,  49,  44,
    49,  57,  50,  46,  49,  57,  51,  32,  56,  49,  55,  46,  56,  52,  57,  44,  48,  32,  48,
    44,  45,  49,  56,  46,  49,  57,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  110, 111, 110, 101, 59,  115, 116, 114, 111,
    107, 101, 58,  35,  97,  48,  98,  49,  98,  98,  59,  115, 116, 114, 111, 107, 101, 45,  119,
    105, 100, 116, 104, 58,  49,  48,  46,  50,  54,  48,  48,  48,  48,  50,  51,  112, 120, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 57,  55,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111,
    110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,
    48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  50,  46,  50,
    49,  48,  53,  53,  55,  54,  44,  48,  44,  48,  44,  48,  46,  56,  54,  56,  50,  52,  56,
    44,  51,  53,  54,  46,  48,  56,  50,  52,  50,  44,  51,  49,  46,  50,  50,  48,  57,  41,
    34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  48,  48,  34,  62,  60,  112,
    97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  55,
    46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,  56,  46,
    49,  54,  50,  32,  45,  51,  46,  48,  50,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,
    45,  54,  46,  55,  53,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  45,  49,  51,
    48,  46,  55,  48,  53,  44,  48,  32,  67,  32,  54,  46,  48,  50,  53,  44,  50,  56,  46,
    53,  49,  51,  32,  51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,  51,  46,  51,
    48,  49,  32,  108, 32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,  32,  48,  44,
    56,  46,  49,  54,  50,  32,  51,  46,  48,  50,  53,  44,  49,  52,  46,  55,  56,  56,  32,
    54,  46,  55,  53,  49,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  51,  48,  46,
    55,  48,  53,  44,  48,  32,  99,  32,  51,  46,  55,  50,  54,  44,  48,  32,  54,  46,  55,
    53,  44,  45,  54,  46,  54,  50,  54,  32,  54,  46,  55,  53,  44,  45,  49,  52,  46,  55,
    56,  56,  32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  10,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,
    35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  112, 97,  116, 104, 49,  48,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117,
    114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,  60,
    103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109,
    97,  116, 114, 105, 120, 40,  50,  46,  56,  57,  54,  57,  52,  56,  53,  44,  48,  44,  48,
    44,  51,  46,  57,  48,  48,  51,  55,  44,  50,  56,  50,  46,  54,  48,  57,  52,  52,  44,
    45,  49,  51,  50,  46,  48,  51,  52,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  103, 49,  48,  52,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,
    32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  55,
    46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  102, 105, 108, 108, 58,  117, 114, 108, 40,  35,  108, 105, 110, 101, 97,
    114, 71,  114, 97,  100, 105, 101, 110, 116, 51,  51,  53,  50,  41,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 49,  48,  54,  34,  32,  47,  62,
    60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  53,  54,  53,  48,  57,  54,
    44,  48,  44,  48,  44,  48,  46,  56,  54,  56,  53,  56,  56,  44,  55,  53,  51,  46,  51,
    56,  44,  51,  49,  46,  50,  49,  49,  50,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  103, 49,  48,  56,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,
    32,  32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  51,  56,  46,  51,
    53,  32,  99,  32,  48,  44,  45,  53,  46,  52,  50,  57,  32,  45,  54,  46,  55,  55,  53,
    44,  45,  57,  46,  56,  51,  55,  32,  45,  49,  53,  46,  49,  50,  49,  44,  45,  57,  46,
    56,  51,  55,  32,  108, 32,  45,  49,  49,  51,  46,  57,  54,  52,  44,  48,  32,  67,  32,
    57,  46,  55,  55,  54,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  50,  46,  57,
    50,  49,  32,  51,  44,  51,  56,  46,  51,  53,  32,  108, 32,  48,  44,  49,  50,  49,  46,
    51,  53,  56,  32,  99,  32,  48,  44,  53,  46,  52,  51,  32,  54,  46,  55,  55,  54,  44,
    57,  46,  56,  51,  56,  32,  49,  53,  46,  49,  50,  49,  44,  57,  46,  56,  51,  56,  32,
    108, 32,  49,  49,  51,  46,  57,  54,  52,  44,  48,  32,  99,  32,  56,  46,  51,  52,  54,
    44,  48,  32,  49,  53,  46,  49,  50,  49,  44,  45,  52,  46,  52,  48,  56,  32,  49,  53,
    46,  49,  50,  49,  44,  45,  57,  46,  56,  51,  56,  32,  108, 32,  48,  44,  45,  49,  50,
    49,  46,  51,  53,  56,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  49,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110,
    110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,
    34,  32,  47,  62,  60,  47,  103, 62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,
    105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,
    99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  112, 97,  116, 104, 49,  49,  52,  34,  10,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,
    10,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  50,  52,  48,  46,  48,  53,  57,
    52,  44,  54,  56,  46,  56,  52,  53,  48,  50,  57,  32,  99,  32,  48,  44,  45,  55,  46,
    48,  56,  57,  52,  49,  53,  32,  45,  53,  46,  50,  52,  49,  54,  44,  45,  49,  50,  46,
    56,  52,  52,  54,  55,  57,  32,  45,  49,  49,  46,  54,  57,  57,  49,  44,  45,  49,  50,
    46,  56,  52,  52,  54,  55,  57,  32,  108, 32,  45,  51,  54,  51,  46,  54,  51,  55,  57,
    50,  44,  48,  32,  99,  32,  45,  54,  46,  52,  53,  55,  53,  49,  44,  48,  32,  45,  49,
    49,  46,  54,  57,  57,  49,  56,  44,  53,  46,  55,  53,  53,  50,  54,  52,  32,  45,  49,
    49,  46,  54,  57,  57,  49,  56,  44,  49,  50,  46,  56,  52,  52,  54,  55,  57,  32,  108,
    32,  48,  44,  57,  54,  46,  56,  49,  48,  50,  49,  49,  32,  99,  32,  48,  44,  55,  46,
    48,  56,  57,  52,  50,  32,  53,  46,  50,  52,  49,  54,  55,  44,  49,  50,  46,  56,  52,
    52,  54,  56,  32,  49,  49,  46,  54,  57,  57,  49,  56,  44,  49,  50,  46,  56,  52,  52,
    54,  56,  32,  108, 32,  51,  54,  51,  46,  54,  51,  55,  57,  50,  44,  48,  32,  99,  32,
    54,  46,  52,  53,  55,  53,  44,  48,  32,  49,  49,  46,  54,  57,  57,  49,  44,  45,  53,
    46,  55,  53,  53,  50,  54,  32,  49,  49,  46,  54,  57,  57,  49,  44,  45,  49,  50,  46,
    56,  52,  52,  54,  56,  32,  108, 32,  48,  44,  45,  57,  54,  46,  56,  49,  48,  50,  49,
    49,  32,  122, 34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  57,  53,  48,
    53,  56,  53,  44,  48,  44,  48,  44,  48,  46,  57,  53,  48,  53,  56,  53,  44,  51,  50,
    46,  57,  53,  51,  56,  44,  48,  46,  54,  54,  57,  41,  34,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 49,  49,  54,  34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,
    116, 101, 40,  48,  44,  54,  49,  46,  53,  49,  41,  34,  10,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  103, 49,  50,  52,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,
    32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,
    50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 49,  50,  54,
    34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  49,  56,
    54,  57,  57,  44,  48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  49,  55,  55,
    46,  56,  54,  57,  44,  55,  46,  57,  52,  57,  49,  55,  41,  34,  10,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  103, 49,  50,  56,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,
    32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  52,
    51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,  53,
    46,  54,  49,  51,  44,  45,  49,  52,  46,  55,  56,  56,  32,  45,  49,  50,  46,  53,  50,
    55,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  45,  49,  49,  57,  46,  49,  53,
    49,  44,  48,  32,  67,  32,  56,  46,  54,  49,  51,  44,  50,  56,  46,  53,  49,  51,  32,
    51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108,
    32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,
    50,  32,  53,  46,  54,  49,  51,  44,  49,  52,  46,  55,  56,  56,  32,  49,  50,  46,  53,
    50,  56,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  49,  57,  46,  49,  53,  49,
    44,  48,  32,  99,  32,  54,  46,  57,  49,  52,  44,  48,  32,  49,  50,  46,  53,  50,  55,
    44,  45,  54,  46,  54,  50,  54,  32,  49,  50,  46,  53,  50,  55,  44,  45,  49,  52,  46,
    55,  56,  56,  32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,  32,  122, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  112, 97,  116, 104, 49,  51,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105,
    110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,
    117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  32,  47,  62,  60,  47,  103, 62,
    60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,
    116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  49,  57,  48,  44,  53,  57,  46,  51,  54,
    41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  51,  50,  34,  62,  60,
    114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,
    49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,
    48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104,
    61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  114, 101, 99,  116, 49,  51,  52,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103,
    10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114,
    97,  110, 115, 108, 97,  116, 101, 40,  54,  55,  54,  44,  53,  57,  46,  51,  54,  41,  34,
    10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  51,  54,  34,  62,  60,  114, 101,
    99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,  49,  54,
    52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,
    57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,
    54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104,
    101, 105, 103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,
    97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    114, 101, 99,  116, 49,  51,  56,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,
    32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  108, 97,  98,  101, 108, 61,
    34,  76,  97,  121, 101, 114, 34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  108, 97,
    121, 101, 114, 49,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101,
    58,  103, 114, 111, 117, 112, 109, 111, 100, 101, 61,  34,  108, 97,  121, 101, 114, 34,  10,
    32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,
    110, 115, 108, 97,  116, 101, 40,  48,  44,  49,  49,  41,  34,  32,  47,  62,  60,  103, 10,
    32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,
    110, 115, 108, 97,  116, 101, 40,  48,  44,  50,  49,  41,  34,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  79,  115, 99,  105, 108, 97,  116, 111, 114, 115, 34,  62,  60,  103, 10,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  52,  49,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116,
    114, 105, 120, 40,  48,  46,  57,  50,  56,  54,  52,  54,  44,  48,  44,  48,  44,  52,  46,
    54,  50,  51,  48,  55,  44,  49,  49,  46,  54,  53,  52,  49,  44,  45,  49,  51,  53,  46,
    53,  48,  53,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111,
    114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  52,  51,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108,
    108, 58,  35,  55,  52,  55,  55,  55,  97,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  51,  49,  46,  51,  53,
    51,  32,  99,  32,  48,  44,  45,  49,  46,  53,  54,  56,  32,  45,  54,  46,  51,  51,  54,
    44,  45,  50,  46,  56,  52,  32,  45,  49,  52,  46,  49,  52,  44,  45,  50,  46,  56,  52,
    32,  108, 32,  45,  49,  49,  53,  46,  57,  50,  53,  44,  48,  32,  67,  32,  57,  46,  51,
    51,  54,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  50,  57,  46,  55,  56,  53,  32,
    51,  44,  51,  49,  46,  51,  53,  51,  32,  108, 32,  48,  44,  49,  51,  53,  46,  51,  53,
    51,  32,  99,  32,  48,  44,  49,  46,  53,  54,  55,  32,  54,  46,  51,  51,  54,  44,  50,
    46,  56,  52,  32,  49,  52,  46,  49,  52,  49,  44,  50,  46,  56,  52,  32,  108, 32,  49,
    49,  53,  46,  57,  50,  53,  44,  48,  32,  99,  32,  55,  46,  56,  48,  52,  44,  48,  32,
    49,  52,  46,  49,  52,  44,  45,  49,  46,  50,  55,  51,  32,  49,  52,  46,  49,  52,  44,
    45,  50,  46,  56,  52,  32,  108, 32,  48,  44,  45,  49,  51,  53,  46,  51,  53,  51,  32,
    122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 49,  52,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  56,
    50,  53,  54,  48,  51,  44,  48,  44,  48,  44,  51,  46,  51,  56,  50,  50,  51,  44,  50,
    54,  46,  56,  50,  50,  55,  44,  45,  49,  48,  48,  46,  49,  50,  53,  41,  34,  62,  60,
    112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,
    97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,
    116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  112, 97,  116, 104, 49,  52,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,
    98,  98,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,
    52,  55,  46,  50,  48,  54,  44,  51,  49,  46,  57,  54,  52,  32,  99,  32,  48,  44,  45,
    49,  46,  57,  48,  53,  32,  45,  54,  46,  51,  51,  54,  44,  45,  51,  46,  52,  53,  49,
    32,  45,  49,  52,  46,  49,  52,  44,  45,  51,  46,  52,  53,  49,  32,  108, 32,  45,  49,
    49,  53,  46,  57,  50,  53,  44,  48,  32,  67,  32,  57,  46,  51,  51,  54,  44,  50,  56,
    46,  53,  49,  51,  32,  51,  44,  51,  48,  46,  48,  53,  57,  32,  51,  44,  51,  49,  46,
    57,  54,  52,  32,  108, 32,  48,  44,  49,  51,  52,  46,  49,  51,  32,  99,  32,  48,  44,
    49,  46,  57,  48,  53,  32,  54,  46,  51,  51,  54,  44,  51,  46,  52,  53,  50,  32,  49,
    52,  46,  49,  52,  49,  44,  51,  46,  52,  53,  50,  32,  108, 32,  49,  49,  53,  46,  57,
    50,  53,  44,  48,  32,  99,  32,  55,  46,  56,  48,  52,  44,  48,  32,  49,  52,  46,  49,
    52,  44,  45,  49,  46,  53,  52,  55,  32,  49,  52,  46,  49,  52,  44,  45,  51,  46,  52,
    53,  50,  32,  108, 32,  48,  44,  45,  49,  51,  52,  46,  49,  51,  32,  122, 34,  32,  47,
    62,  60,  47,  103, 62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  103, 49,  52,  57,  34,  10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102,
    111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  56,  46,  53,  50,  50,  56,  55,
    44,  48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  45,  49,  49,  46,  49,  54,
    56,  54,  44,  54,  54,  49,  46,  57,  52,  57,  41,  34,  62,  60,  112, 97,  116, 104, 10,
    32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110,
    110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  53,
    49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105,
    108, 108, 58,  35,  97,  49,  97,  97,  98,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    100, 61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,
    32,  99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,  48,  46,  55,  56,  49,  44,
    45,  49,  52,  46,  55,  56,  56,  32,  45,  49,  46,  55,  52,  52,  44,  45,  49,  52,  46,
    55,  56,  56,  32,  108, 32,  45,  49,  52,  48,  46,  55,  49,  55,  44,  48,  32,  67,  32,
    51,  46,  55,  56,  50,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,  49,
    51,  57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,  49,
    46,  52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,  50,  32,  48,  46,  55,  56,
    50,  44,  49,  52,  46,  55,  56,  56,  32,  49,  46,  55,  52,  53,  44,  49,  52,  46,  55,
    56,  56,  32,  108, 32,  49,  52,  48,  46,  55,  49,  55,  44,  48,  32,  99,  32,  48,  46,
    57,  54,  51,  44,  48,  32,  49,  46,  55,  52,  52,  44,  45,  54,  46,  54,  50,  54,  32,
    49,  46,  55,  52,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  48,  44,  45,
    49,  49,  49,  46,  52,  53,  55,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103,
    10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  53,  51,  34,  10,  32,  32,  32,
    32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120,
    40,  48,  44,  48,  46,  53,  57,  55,  54,  56,  56,  44,  45,  48,  46,  54,  49,  48,  50,
    50,  51,  44,  48,  44,  52,  57,  50,  46,  55,  51,  52,  44,  54,  51,  51,  46,  50,  54,
    53,  41,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  114, 101, 99,  116, 49,  53,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  117, 114, 108, 40,  35,  95,  76,  105,
    110, 101, 97,  114, 52,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103,
    104, 116, 61,  34,  51,  52,  46,  57,  48,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  52,  49,  46,  54,  48,  54,  57,  57,  56,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  54,  53,  53,  46,  55,  49,  51,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  53,  55,  46,  49,  52,
    56,  57,  57,  56,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,
    116, 101, 40,  48,  44,  52,  57,  49,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  77,  97,  115, 116, 101, 114, 34,  62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 49,  53,  56,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  55,
    50,  50,  54,  57,  44,  48,  44,  48,  44,  49,  46,  49,  52,  49,  48,  54,  44,  49,  55,
    54,  46,  50,  54,  50,  44,  45,  50,  54,  46,  48,  51,  51,  56,  41,  34,  62,  60,  112,
    97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,
    112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116,
    117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  112, 97,  116, 104, 49,  54,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,  53,  56,  53,  56,
    53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,
    55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,  56,
    46,  49,  54,  50,  32,  45,  52,  46,  51,  56,  57,  44,  45,  49,  52,  46,  55,  56,  56,
    32,  45,  57,  46,  55,  57,  53,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  45,
    49,  50,  52,  46,  54,  49,  53,  44,  48,  32,  67,  32,  55,  46,  51,  56,  57,  44,  50,
    56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,  51,
    46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,  32,
    48,  44,  56,  46,  49,  54,  50,  32,  52,  46,  51,  56,  57,  44,  49,  52,  46,  55,  56,
    56,  32,  57,  46,  55,  57,  54,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,  50,
    52,  46,  54,  49,  53,  44,  48,  32,  99,  32,  53,  46,  52,  48,  54,  44,  48,  32,  57,
    46,  55,  57,  53,  44,  45,  54,  46,  54,  50,  54,  32,  57,  46,  55,  57,  53,  44,  45,
    49,  52,  46,  55,  56,  56,  32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,  55,
    32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  103, 49,  54,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116,
    114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  51,  46,
    51,  51,  56,  52,  49,  44,  48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  52,
    51,  48,  46,  54,  49,  49,  44,  45,  51,  46,  48,  53,  48,  56,  51,  41,  34,  62,  60,
    112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,
    97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,
    116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  112, 97,  116, 104, 49,  54,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,  53,  56,  53,
    56,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,
    52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,  45,
    56,  46,  49,  54,  50,  32,  45,  49,  46,  57,  57,  53,  44,  45,  49,  52,  46,  55,  56,
    56,  32,  45,  52,  46,  52,  53,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,
    45,  49,  51,  53,  46,  50,  57,  56,  44,  48,  32,  67,  32,  52,  46,  57,  57,  54,  44,
    50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,  52,
    51,  46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,  99,
    32,  48,  44,  56,  46,  49,  54,  50,  32,  49,  46,  57,  57,  54,  44,  49,  52,  46,  55,
    56,  56,  32,  52,  46,  52,  53,  52,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,  49,
    51,  53,  46,  50,  57,  56,  44,  48,  32,  99,  32,  50,  46,  52,  53,  57,  44,  48,  32,
    52,  46,  52,  53,  52,  44,  45,  54,  46,  54,  50,  54,  32,  52,  46,  52,  53,  52,  44,
    45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,  53,
    55,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  103, 49,  54,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,
    46,  53,  52,  54,  50,  54,  44,  48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,
    57,  50,  52,  46,  56,  57,  57,  44,  45,  51,  46,  48,  53,  48,  56,  51,  41,  34,  62,
    60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115,
    99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118,
    97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  112, 97,  116, 104, 49,  54,  56,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,  53,  56,
    53,  56,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,
    49,  52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,  32,  99,  32,  48,  44,
    45,  56,  46,  49,  54,  50,  32,  45,  52,  46,  51,  48,  57,  44,  45,  49,  52,  46,  55,
    56,  56,  32,  45,  57,  46,  54,  49,  54,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108,
    32,  45,  49,  50,  52,  46,  57,  55,  51,  44,  48,  32,  67,  32,  55,  46,  51,  48,  57,
    44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,  49,  51,  57,  32,  51,  44,
    52,  51,  46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,  49,  46,  52,  53,  55,  32,
    99,  32,  48,  44,  56,  46,  49,  54,  50,  32,  52,  46,  51,  48,  57,  44,  49,  52,  46,
    55,  56,  56,  32,  57,  46,  54,  49,  55,  44,  49,  52,  46,  55,  56,  56,  32,  108, 32,
    49,  50,  52,  46,  57,  55,  51,  44,  48,  32,  99,  32,  53,  46,  51,  48,  55,  44,  48,
    32,  57,  46,  54,  49,  54,  44,  45,  54,  46,  54,  50,  54,  32,  57,  46,  54,  49,  54,
    44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  48,  44,  45,  49,  49,  49,  46,  52,
    53,  55,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 49,  55,  48,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,
    48,  46,  53,  53,  56,  53,  57,  57,  44,  48,  44,  48,  44,  49,  46,  48,  48,  53,  53,
    49,  44,  49,  49,  54,  48,  46,  50,  57,  44,  45,  51,  46,  48,  53,  48,  56,  51,  41,
    34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117,
    114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  55,  50,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  102,
    98,  52,  101, 52,  102, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,
    109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  51,  54,  46,  57,  49,  51,  32,  99,  32,
    48,  44,  45,  52,  46,  54,  51,  54,  32,  45,  54,  46,  55,  55,  53,  44,  45,  56,  46,
    52,  32,  45,  49,  53,  46,  49,  50,  49,  44,  45,  56,  46,  52,  32,  108, 32,  45,  49,
    49,  51,  46,  57,  54,  52,  44,  48,  32,  67,  32,  57,  46,  55,  55,  54,  44,  50,  56,
    46,  53,  49,  51,  32,  51,  44,  51,  50,  46,  50,  55,  55,  32,  51,  44,  51,  54,  46,
    57,  49,  51,  32,  108, 32,  48,  44,  49,  50,  52,  46,  50,  51,  51,  32,  99,  32,  48,
    44,  52,  46,  54,  51,  54,  32,  54,  46,  55,  55,  54,  44,  56,  46,  52,  32,  49,  53,
    46,  49,  50,  49,  44,  56,  46,  52,  32,  108, 32,  49,  49,  51,  46,  57,  54,  52,  44,
    48,  32,  99,  32,  56,  46,  51,  52,  54,  44,  48,  32,  49,  53,  46,  49,  50,  49,  44,
    45,  51,  46,  55,  54,  52,  32,  49,  53,  46,  49,  50,  49,  44,  45,  56,  46,  52,  32,
    108, 32,  48,  44,  45,  49,  50,  52,  46,  50,  51,  51,  32,  122, 34,  32,  47,  62,  60,
    47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,
    55,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  50,  55,  48,  44,  50,  56,
    46,  53,  49,  41,  34,  62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 49,  55,  54,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,
    53,  56,  53,  56,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105,
    103, 104, 116, 61,  34,  50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,
    57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,
    46,  50,  48,  50,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120,
    61,  34,  49,  51,  55,  46,  49,  54,  52,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  55,  56,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114,
    97,  110, 115, 108, 97,  116, 101, 40,  55,  54,  48,  44,  50,  56,  46,  53,  49,  41,  34,
    62,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  114, 101, 99,  116, 49,  56,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,  53,  56,  53,  56,  53,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,
    50,  55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,
    46,  49,  54,  52,  34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 49,  56,  50,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,
    48,  46,  50,  51,  56,  51,  52,  56,  44,  48,  44,  48,  44,  49,  44,  49,  49,  49,  55,
    46,  49,  54,  44,  50,  56,  46,  53,  49,  41,  34,  62,  60,  114, 101, 99,  116, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 49,  56,  52,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102,
    105, 108, 108, 58,  117, 114, 108, 40,  35,  95,  76,  105, 110, 101, 97,  114, 53,  41,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,
    55,  46,  55,  48,  53,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    119, 105, 100, 116, 104, 61,  34,  54,  49,  46,  56,  48,  54,  57,  57,  57,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  53,  48,  46,  50,  48,  50,  57,  57,
    57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  49,  51,  55,  46,
    49,  54,  52,  34,  32,  47,  62,  60,  47,  103, 62,  60,  47,  103, 62,  60,  103, 10,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  68,  114, 101, 105, 101, 99,  107, 101, 34,  10,  32,
    32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110,
    115, 108, 97,  116, 101, 40,  48,  44,  49,  49,  41,  34,  62,  60,  103, 10,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  56,  55,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120,
    40,  45,  48,  46,  53,  52,  53,  51,  53,  52,  44,  48,  44,  48,  44,  45,  48,  46,  53,
    52,  53,  51,  53,  52,  44,  50,  51,  53,  46,  56,  52,  54,  44,  49,  48,  54,  53,  46,
    52,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,
    99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  56,  57,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,
    35,  53,  48,  52,  101, 53,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100,
    61,  34,  109, 32,  49,  53,  50,  46,  54,  49,  54,  44,  54,  56,  54,  46,  50,  53,  49,
    32,  49,  53,  46,  52,  53,  50,  44,  51,  48,  46,  57,  48,  52,  32,  45,  51,  48,  46,
    57,  48,  52,  44,  48,  32,  49,  53,  46,  52,  53,  50,  44,  45,  51,  48,  46,  57,  48,
    52,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  103, 49,  57,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  45,
    48,  46,  53,  52,  53,  51,  53,  52,  44,  48,  44,  48,  44,  45,  48,  46,  53,  52,  53,
    51,  53,  52,  44,  49,  49,  50,  50,  46,  56,  53,  44,  49,  48,  54,  53,  46,  52,  41,
    34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117,
    114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  57,  51,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,
    48,  52,  101, 53,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,
    109, 32,  49,  53,  50,  46,  54,  49,  54,  44,  54,  56,  54,  46,  50,  53,  49,  32,  49,
    53,  46,  52,  53,  50,  44,  51,  48,  46,  57,  48,  52,  32,  45,  51,  48,  46,  57,  48,
    52,  44,  48,  32,  49,  53,  46,  52,  53,  50,  44,  45,  51,  48,  46,  57,  48,  52,  32,
    122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 49,  57,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  45,  48,  46,
    53,  52,  53,  51,  53,  52,  44,  48,  44,  48,  44,  45,  48,  46,  53,  52,  53,  51,  53,
    52,  44,  49,  50,  53,  49,  46,  56,  53,  44,  49,  48,  54,  53,  46,  52,  41,  34,  62,
    60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115,
    99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118,
    97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  112, 97,  116, 104, 49,  57,  55,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,
    101, 53,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,
    49,  53,  50,  46,  54,  49,  54,  44,  54,  56,  54,  46,  50,  53,  49,  32,  49,  53,  46,
    52,  53,  50,  44,  51,  48,  46,  57,  48,  52,  32,  45,  51,  48,  46,  57,  48,  52,  44,
    48,  32,  49,  53,  46,  52,  53,  50,  44,  45,  51,  48,  46,  57,  48,  52,  32,  122, 34,
    32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  103, 49,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  53,  49,  50,
    52,  55,  57,  44,  48,  44,  48,  44,  48,  46,  53,  49,  50,  52,  55,  57,  44,  55,  52,
    46,  52,  48,  51,  53,  44,  52,  53,  56,  46,  53,  54,  50,  41,  34,  62,  60,  112, 97,
    116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112,
    101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117,
    114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  112, 97,  116, 104, 50,  48,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  53,  50,
    46,  54,  49,  54,  44,  54,  56,  54,  46,  50,  53,  49,  32,  49,  53,  46,  52,  53,  50,
    44,  51,  48,  46,  57,  48,  52,  32,  45,  51,  48,  46,  57,  48,  52,  44,  48,  32,  49,
    53,  46,  52,  53,  50,  44,  45,  51,  48,  46,  57,  48,  52,  32,  122, 34,  32,  47,  62,
    60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103,
    50,  48,  51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  53,  49,  50,  52,  55,  57,
    44,  48,  44,  48,  44,  48,  46,  53,  49,  50,  52,  55,  57,  44,  49,  48,  56,  57,  46,
    52,  44,  52,  53,  56,  46,  53,  54,  50,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111,
    110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116,
    104, 50,  48,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108,
    101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  53,  50,  46,  54,  49,  54,
    44,  54,  56,  54,  46,  50,  53,  49,  32,  49,  53,  46,  52,  53,  50,  44,  51,  48,  46,
    57,  48,  52,  32,  45,  51,  48,  46,  57,  48,  52,  44,  48,  32,  49,  53,  46,  52,  53,
    50,  44,  45,  51,  48,  46,  57,  48,  52,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,
    60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 50,  48,  55,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,
    109, 97,  116, 114, 105, 120, 40,  48,  46,  53,  49,  50,  52,  55,  57,  44,  48,  44,  48,
    44,  48,  46,  53,  49,  50,  52,  55,  57,  44,  57,  54,  49,  46,  52,  48,  51,  44,  52,
    53,  56,  46,  53,  54,  50,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101,
    99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 50,  48,
    57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,
    102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  53,  50,  46,  54,  49,  54,  44,  54,  56,
    54,  46,  50,  53,  49,  32,  49,  53,  46,  52,  53,  50,  44,  51,  48,  46,  57,  48,  52,
    32,  45,  51,  48,  46,  57,  48,  52,  44,  48,  32,  49,  53,  46,  52,  53,  50,  44,  45,
    51,  48,  46,  57,  48,  52,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  47,  103,
    62,  60,  100, 101, 102, 115, 10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  100, 101, 102,
    115, 50,  50,  55,  34,  62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101,
    110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101,
    97,  114, 49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,
    34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116,
    85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,
    115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116,
    84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,
    49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,
    49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  62,  60,
    115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101,
    116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,
    49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111,
    112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  115, 116, 111, 112, 50,  51,  48,  34,  32,  47,  62,  60,  115, 116, 111,
    112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,
    49,  55,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  55,  48,
    44,  54,  57,  44,  54,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116,
    121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115,
    116, 111, 112, 50,  51,  50,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  51,  57,  37,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111,
    112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  57,  56,  44,  57,  56,  44,  57,
    56,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  51,
    52,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  111, 102, 102, 115, 101, 116, 61,  34,  56,  48,  37,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108,
    111, 114, 58,  114, 103, 98,  40,  57,  53,  44,  57,  53,  44,  57,  53,  41,  59,  115, 116,
    111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  51,  54,  34,  32,  47,  62,
    60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115,
    101, 116, 61,  34,  57,  54,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103,
    98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112,
    45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  51,  56,  34,  32,  47,  62,  60,  115,
    116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116,
    61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,
    40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,
    111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  52,  48,  34,  32,  47,  62,  60,  47,  108,
    105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101,
    97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 50,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114,
    83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,
    103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,
    109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,
    50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,
    48,  53,  53,  53,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111,
    108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,
    41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  52,  51,
    34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    111, 102, 102, 115, 101, 116, 61,  34,  49,  55,  37,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111,
    114, 58,  114, 103, 98,  40,  55,  48,  44,  54,  57,  44,  54,  53,  41,  59,  115, 116, 111,
    112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  52,  53,  34,  32,  47,  62,  60,
    115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101,
    116, 61,  34,  51,  57,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,
    40,  57,  56,  44,  57,  56,  44,  57,  56,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,
    99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  115, 116, 111, 112, 50,  52,  55,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  55,  54,
    37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,
    115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  57,  53,  44,  57,
    53,  44,  57,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111,
    112, 50,  52,  57,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  57,  54,  37,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,
    99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,
    56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,
    53,  49,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,
    111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,
    55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  53,
    51,  34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101,
    110, 116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 51,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120,
    50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105,
    116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,
    48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,
    46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  62,  60,  115, 116, 111,
    112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,
    48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,
    44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  115, 116, 111, 112, 50,  53,  54,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  55,  37,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115,
    116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  55,  48,  44,  54,  57,
    44,  54,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112,
    50,  53,  56,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  51,  57,  37,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,
    111, 108, 111, 114, 58,  114, 103, 98,  40,  57,  56,  44,  57,  56,  44,  57,  56,  41,  59,
    115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  54,  48,  34,  32,
    47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102,
    102, 115, 101, 116, 61,  34,  56,  49,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,
    114, 103, 98,  40,  57,  53,  44,  57,  53,  44,  57,  53,  41,  59,  115, 116, 111, 112, 45,
    111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  54,  50,  34,  32,  47,  62,  60,  115, 116,
    111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,
    34,  57,  54,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108,
    101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,
    54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  115, 116, 111, 112, 50,  54,  52,  34,  32,  47,  62,  60,  115, 116, 111, 112,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,
    48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,
    48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,
    99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  115, 116, 111, 112, 50,  54,  54,  34,  32,  47,  62,  60,  47,  108, 105, 110, 101,
    97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101, 97,  114, 71,
    114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    95,  76,  105, 110, 101, 97,  114, 52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,
    100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,
    99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,
    100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116,
    114, 105, 120, 40,  52,  49,  46,  54,  48,  55,  49,  44,  48,  44,  48,  44,  51,  52,  46,
    57,  48,  53,  51,  44,  53,  55,  46,  49,  52,  56,  54,  44,  54,  55,  51,  46,  49,  54,
    54,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114,
    58,  114, 103, 98,  40,  49,  49,  54,  44,  49,  49,  57,  44,  49,  50,  50,  41,  59,  115,
    116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  54,  57,  34,  32,  47,
    62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102,
    115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,
    114, 103, 98,  40,  49,  54,  49,  44,  49,  55,  48,  44,  49,  55,  56,  41,  59,  115, 116,
    111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  55,  49,  34,  32,  47,  62,
    60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108,
    105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 53,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121,
    49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117,
    115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,
    32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,
    44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,
    54,  52,  46,  48,  53,  53,  53,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112,
    45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  51,  51,  44,  49,  51,  51,  44,
    49,  51,  51,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112,
    50,  55,  52,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,
    99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  50,  53,  49,  44,  55,  56,  44,  56,  49,
    41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  55,  54,
    34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110,
    116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 54,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,
    61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116,
    115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,
    32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  48,  51,  49,  46,
    48,  51,  44,  48,  44,  48,  44,  52,  49,  54,  46,  48,  49,  56,  44,  50,  50,  52,  46,
    54,  55,  50,  44,  51,  49,  50,  46,  51,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116,
    111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  51,  51,  44,  49,  51,
    51,  44,  49,  51,  51,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121,
    58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116,
    111, 112, 50,  55,  57,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111,
    112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,
    44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111,
    112, 50,  56,  49,  34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,
    100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101,
    110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,
    99,  111, 108, 108, 101, 99,  116, 61,  34,  97,  108, 119, 97,  121, 115, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  120, 108, 105, 110, 107, 58,  104, 114, 101, 102, 61,  34,  35,  95,
    76,  105, 110, 101, 97,  114, 49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 51,  51,  52,  56,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110,
    105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114,
    97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,
    56,  48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,
    55,  46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121,
    49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  32,  47,  62,  60,  108,
    105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 108, 108, 101, 99,  116, 61,
    34,  97,  108, 119, 97,  121, 115, 34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 108, 105,
    110, 107, 58,  104, 114, 101, 102, 61,  34,  35,  95,  76,  105, 110, 101, 97,  114, 50,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  108, 105, 110, 101, 97,  114, 71,
    114, 97,  100, 105, 101, 110, 116, 51,  51,  53,  48,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101,
    114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,
    34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,
    44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,
    46,  48,  53,  53,  53,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    121, 50,  61,  34,  48,  34,  32,  47,  62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,
    100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,
    112, 101, 58,  99,  111, 108, 108, 101, 99,  116, 61,  34,  97,  108, 119, 97,  121, 115, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  120, 108, 105, 110, 107, 58,  104, 114, 101, 102, 61,
    34,  35,  95,  76,  105, 110, 101, 97,  114, 51,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 51,
    51,  53,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110,
    116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110,
    85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110,
    116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,
    54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,
    44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,
    34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  32,  47,
    62,  60,  47,  100, 101, 102, 115, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  57,  53,
    48,  53,  56,  53,  44,  48,  44,  48,  44,  48,  46,  57,  53,  48,  53,  56,  53,  44,  55,
    56,  46,  57,  53,  51,  56,  44,  51,  48,  55,  46,  54,  54,  57,  41,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 50,  54,  34,  62,  60,  99,  105, 114, 99,  108, 101,
    10,  32,  32,  32,  32,  32,  32,  32,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  99,  121, 61,  34,  49,  50,  55,  46,
    56,  49,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  114, 61,  34,  52,  54,  46,  53,
    53,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  99,  105, 114, 99,  108, 101, 50,  56,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  55,  49,  57,  46,  53,  50,  48,  57,
    57,  44,  49,  50,  55,  46,  56,  49,  50,  32,  99,  32,  48,  44,  50,  53,  46,  55,  49,
    50,  55,  50,  32,  45,  50,  48,  46,  56,  52,  52,  50,  56,  44,  52,  54,  46,  53,  53,
    55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,  45,  50,
    53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,
    50,  48,  46,  56,  52,  52,  50,  56,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,  52,
    54,  46,  53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  55,  49,  50,  55,  50,  32,  50,
    48,  46,  56,  52,  52,  50,  56,  44,  45,  52,  54,  46,  53,  53,  55,  48,  48,  51,  32,
    52,  54,  46,  53,  53,  55,  44,  45,  52,  54,  46,  53,  53,  55,  48,  48,  51,  32,  50,
    53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  52,  54,  46,  53,  53,  55,  44,  50,  48,
    46,  56,  52,  52,  50,  56,  51,  32,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,
    53,  55,  48,  48,  51,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100,
    105, 112, 111, 100, 105, 58,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,
    121, 61,  34,  49,  50,  55,  46,  56,  49,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    115, 111, 100, 105, 112, 111, 100, 105, 58,  114, 120, 61,  34,  52,  54,  46,  53,  53,  54,
    57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100,
    105, 58,  114, 121, 61,  34,  52,  54,  46,  53,  53,  54,  57,  57,  57,  34,  32,  47,  62,
    60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  52,  54,  44,  51,  48,
    49,  46,  54,  53,  51,  56,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103,
    51,  48,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,
    34,  109, 32,  54,  55,  50,  46,  57,  54,  52,  44,  56,  49,  46,  50,  53,  53,  32,  99,
    32,  50,  53,  46,  54,  57,  53,  44,  48,  32,  52,  54,  46,  53,  53,  55,  44,  50,  48,
    46,  56,  54,  49,  32,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,
    48,  44,  50,  53,  46,  54,  57,  53,  32,  45,  50,  48,  46,  56,  54,  50,  44,  52,  54,
    46,  53,  53,  55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,
    32,  45,  50,  53,  46,  54,  57,  54,  44,  48,  32,  45,  52,  54,  46,  53,  53,  55,  44,
    45,  50,  48,  46,  56,  54,  50,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,  52,  54,
    46,  53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  54,  57,  54,  32,  50,  48,  46,  56,
    54,  49,  44,  45,  52,  54,  46,  53,  53,  55,  32,  52,  54,  46,  53,  53,  55,  44,  45,
    52,  54,  46,  53,  53,  55,  32,  122, 32,  109, 32,  48,  44,  52,  46,  49,  57,  32,  99,
    32,  50,  51,  46,  51,  56,  51,  44,  48,  32,  52,  50,  46,  51,  54,  55,  44,  49,  56,
    46,  57,  56,  52,  32,  52,  50,  46,  51,  54,  55,  44,  52,  50,  46,  51,  54,  55,  32,
    48,  44,  50,  51,  46,  51,  56,  51,  32,  45,  49,  56,  46,  57,  56,  52,  44,  52,  50,
    46,  51,  54,  55,  32,  45,  52,  50,  46,  51,  54,  55,  44,  52,  50,  46,  51,  54,  55,
    32,  45,  50,  51,  46,  51,  56,  51,  44,  48,  32,  45,  52,  50,  46,  51,  54,  55,  44,
    45,  49,  56,  46,  57,  56,  52,  32,  45,  52,  50,  46,  51,  54,  55,  44,  45,  52,  50,
    46,  51,  54,  55,  32,  48,  44,  45,  50,  51,  46,  51,  56,  51,  32,  49,  56,  46,  57,
    56,  52,  44,  45,  52,  50,  46,  51,  54,  55,  32,  52,  50,  46,  51,  54,  55,  44,  45,
    52,  50,  46,  51,  54,  55,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 51,  50,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110,
    110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,
    34,  32,  47,  62,  60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  57,  53,
    48,  53,  56,  53,  44,  48,  44,  48,  44,  48,  46,  57,  53,  48,  53,  56,  53,  44,  55,
    56,  46,  57,  53,  51,  56,  44,  49,  53,  53,  46,  54,  54,  57,  41,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 55,  49,  34,  62,  60,  99,  105, 114, 99,  108, 101,
    10,  32,  32,  32,  32,  32,  32,  32,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,
    57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  99,  121, 61,  34,  49,  50,  55,  46,
    56,  49,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  114, 61,  34,  52,  54,  46,  53,
    53,  54,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  102, 105, 108, 108, 58,  35,  97,  48,  98,  49,  98,  98,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  99,  105, 114, 99,  108, 101, 55,  51,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  55,  49,  57,  46,  53,  50,  48,  57,
    57,  44,  49,  50,  55,  46,  56,  49,  50,  32,  99,  32,  48,  44,  50,  53,  46,  55,  49,
    50,  55,  50,  32,  45,  50,  48,  46,  56,  52,  52,  50,  56,  44,  52,  54,  46,  53,  53,
    55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,  45,  50,
    53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,
    50,  48,  46,  56,  52,  52,  50,  56,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,  52,
    54,  46,  53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  55,  49,  50,  55,  50,  32,  50,
    48,  46,  56,  52,  52,  50,  56,  44,  45,  52,  54,  46,  53,  53,  55,  48,  48,  51,  32,
    52,  54,  46,  53,  53,  55,  44,  45,  52,  54,  46,  53,  53,  55,  48,  48,  51,  32,  50,
    53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  52,  54,  46,  53,  53,  55,  44,  50,  48,
    46,  56,  52,  52,  50,  56,  51,  32,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,
    53,  55,  48,  48,  51,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100,
    105, 112, 111, 100, 105, 58,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,
    121, 61,  34,  49,  50,  55,  46,  56,  49,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    115, 111, 100, 105, 112, 111, 100, 105, 58,  114, 120, 61,  34,  52,  54,  46,  53,  53,  54,
    57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100,
    105, 58,  114, 121, 61,  34,  52,  54,  46,  53,  53,  54,  57,  57,  57,  34,  32,  47,  62,
    60,  47,  103, 62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  52,  54,  44,  49,  52,
    57,  46,  54,  53,  51,  56,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103,
    55,  53,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,
    34,  109, 32,  54,  55,  50,  46,  57,  54,  52,  44,  56,  49,  46,  50,  53,  53,  32,  99,
    32,  50,  53,  46,  54,  57,  53,  44,  48,  32,  52,  54,  46,  53,  53,  55,  44,  50,  48,
    46,  56,  54,  49,  32,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,
    48,  44,  50,  53,  46,  54,  57,  53,  32,  45,  50,  48,  46,  56,  54,  50,  44,  52,  54,
    46,  53,  53,  55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,
    32,  45,  50,  53,  46,  54,  57,  54,  44,  48,  32,  45,  52,  54,  46,  53,  53,  55,  44,
    45,  50,  48,  46,  56,  54,  50,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,  52,  54,
    46,  53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  54,  57,  54,  32,  50,  48,  46,  56,
    54,  49,  44,  45,  52,  54,  46,  53,  53,  55,  32,  52,  54,  46,  53,  53,  55,  44,  45,
    52,  54,  46,  53,  53,  55,  32,  122, 32,  109, 32,  48,  44,  52,  46,  49,  57,  32,  99,
    32,  50,  51,  46,  51,  56,  51,  44,  48,  32,  52,  50,  46,  51,  54,  55,  44,  49,  56,
    46,  57,  56,  52,  32,  52,  50,  46,  51,  54,  55,  44,  52,  50,  46,  51,  54,  55,  32,
    48,  44,  50,  51,  46,  51,  56,  51,  32,  45,  49,  56,  46,  57,  56,  52,  44,  52,  50,
    46,  51,  54,  55,  32,  45,  52,  50,  46,  51,  54,  55,  44,  52,  50,  46,  51,  54,  55,
    32,  45,  50,  51,  46,  51,  56,  51,  44,  48,  32,  45,  52,  50,  46,  51,  54,  55,  44,
    45,  49,  56,  46,  57,  56,  52,  32,  45,  52,  50,  46,  51,  54,  55,  44,  45,  52,  50,
    46,  51,  54,  55,  32,  48,  44,  45,  50,  51,  46,  51,  56,  51,  32,  49,  56,  46,  57,
    56,  52,  44,  45,  52,  50,  46,  51,  54,  55,  32,  52,  50,  46,  51,  54,  55,  44,  45,
    52,  50,  46,  51,  54,  55,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 55,  55,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110,
    110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,
    34,  32,  47,  62,  60,  47,  103, 62,  60,  99,  105, 114, 99,  108, 101, 10,  32,  32,  32,
    32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120,
    40,  48,  46,  57,  53,  48,  53,  56,  53,  44,  48,  44,  48,  44,  48,  46,  57,  53,  48,
    53,  56,  53,  44,  55,  56,  46,  57,  53,  51,  56,  44,  48,  46,  54,  54,  57,  41,  34,
    10,  32,  32,  32,  32,  32,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  99,  121, 61,  34,  49,  50,  55,  46,  56,  49,  50,  34,
    10,  32,  32,  32,  32,  32,  114, 61,  34,  52,  54,  46,  53,  53,  54,  57,  57,  57,  34,
    10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,
    97,  48,  98,  49,  98,  98,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118,
    101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 99,  97,
    112, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101,
    106, 111, 105, 110, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107, 101, 45,  109,
    105, 116, 101, 114, 108, 105, 109, 105, 116, 58,  49,  46,  52,  49,  52,  50,  48,  57,  57,
    54,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  99,  105, 114, 99,  108, 101, 49,
    49,  56,  34,  10,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  55,  49,  57,  46,  53,
    50,  48,  57,  57,  44,  49,  50,  55,  46,  56,  49,  50,  32,  99,  32,  48,  44,  50,  53,
    46,  55,  49,  50,  55,  50,  32,  45,  50,  48,  46,  56,  52,  52,  50,  56,  44,  52,  54,
    46,  53,  53,  55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,
    32,  45,  50,  53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  45,  52,  54,  46,  53,  53,
    55,  44,  45,  50,  48,  46,  56,  52,  52,  50,  56,  32,  45,  52,  54,  46,  53,  53,  55,
    44,  45,  52,  54,  46,  53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  55,  49,  50,  55,
    50,  32,  50,  48,  46,  56,  52,  52,  50,  56,  44,  45,  52,  54,  46,  53,  53,  55,  48,
    48,  51,  32,  52,  54,  46,  53,  53,  55,  44,  45,  52,  54,  46,  53,  53,  55,  48,  48,
    51,  32,  50,  53,  46,  55,  49,  50,  55,  50,  44,  48,  32,  52,  54,  46,  53,  53,  55,
    44,  50,  48,  46,  56,  52,  52,  50,  56,  51,  32,  52,  54,  46,  53,  53,  55,  44,  52,
    54,  46,  53,  53,  55,  48,  48,  51,  32,  122, 34,  10,  32,  32,  32,  32,  32,  115, 111,
    100, 105, 112, 111, 100, 105, 58,  99,  120, 61,  34,  54,  55,  50,  46,  57,  54,  51,  57,
    57,  34,  10,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,  121,
    61,  34,  49,  50,  55,  46,  56,  49,  50,  34,  10,  32,  32,  32,  32,  32,  115, 111, 100,
    105, 112, 111, 100, 105, 58,  114, 120, 61,  34,  52,  54,  46,  53,  53,  54,  57,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  114, 121, 61,
    34,  52,  54,  46,  53,  53,  54,  57,  57,  57,  34,  32,  47,  62,  60,  103, 10,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 45,  114, 117, 108, 101,
    58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110,
    101, 99,  97,  112, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108,
    105, 110, 101, 106, 111, 105, 110, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107,
    101, 45,  109, 105, 116, 101, 114, 108, 105, 109, 105, 116, 58,  49,  46,  52,  49,  52,  50,
    48,  57,  57,  54,  34,  10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  52,  54,  44,  45,  53,  46,
    51,  52,  54,  50,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 49,  50,
    48,  34,  62,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,
    109, 32,  54,  55,  50,  46,  57,  54,  52,  44,  56,  49,  46,  50,  53,  53,  32,  99,  32,
    50,  53,  46,  54,  57,  53,  44,  48,  32,  52,  54,  46,  53,  53,  55,  44,  50,  48,  46,
    56,  54,  49,  32,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,  48,
    44,  50,  53,  46,  54,  57,  53,  32,  45,  50,  48,  46,  56,  54,  50,  44,  52,  54,  46,
    53,  53,  55,  32,  45,  52,  54,  46,  53,  53,  55,  44,  52,  54,  46,  53,  53,  55,  32,
    45,  50,  53,  46,  54,  57,  54,  44,  48,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,
    50,  48,  46,  56,  54,  50,  32,  45,  52,  54,  46,  53,  53,  55,  44,  45,  52,  54,  46,
    53,  53,  55,  32,  48,  44,  45,  50,  53,  46,  54,  57,  54,  32,  50,  48,  46,  56,  54,
    49,  44,  45,  52,  54,  46,  53,  53,  55,  32,  52,  54,  46,  53,  53,  55,  44,  45,  52,
    54,  46,  53,  53,  55,  32,  122, 32,  109, 32,  48,  44,  52,  46,  50,  55,  54,  32,  99,
    32,  50,  51,  46,  51,  51,  53,  44,  48,  32,  52,  50,  46,  50,  56,  49,  44,  49,  56,
    46,  57,  52,  53,  32,  52,  50,  46,  50,  56,  49,  44,  52,  50,  46,  50,  56,  49,  32,
    48,  44,  50,  51,  46,  51,  51,  53,  32,  45,  49,  56,  46,  57,  52,  54,  44,  52,  50,
    46,  50,  56,  49,  32,  45,  52,  50,  46,  50,  56,  49,  44,  52,  50,  46,  50,  56,  49,
    32,  45,  50,  51,  46,  51,  51,  54,  44,  48,  32,  45,  52,  50,  46,  50,  56,  49,  44,
    45,  49,  56,  46,  57,  52,  54,  32,  45,  52,  50,  46,  50,  56,  49,  44,  45,  52,  50,
    46,  50,  56,  49,  32,  48,  44,  45,  50,  51,  46,  51,  51,  54,  32,  49,  56,  46,  57,
    52,  53,  44,  45,  52,  50,  46,  50,  56,  49,  32,  52,  50,  46,  50,  56,  49,  44,  45,
    52,  50,  46,  50,  56,  49,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  53,  48,  52,  101, 53,  52,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  50,  50,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111,
    110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,
    48,  34,  32,  47,  62,  60,  47,  103, 62,  60,  47,  115, 118, 103, 62,  0,   0};

const char *Monique_Ui_Mainwindow::_01hintergrundalles_svg =
    (const char *)resource_Monique_Ui_Mainwindow__01hintergrundalles_svg;
const int Monique_Ui_Mainwindow::_01hintergrundalles_svgSize = 23727;
