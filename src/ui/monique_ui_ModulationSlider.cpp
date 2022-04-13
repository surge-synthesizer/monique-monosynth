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

#include "monique_ui_ModulationSlider.h"

#include "monique_ui_ENVPopup.h"
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "core/monique_core_Datastructures.h"
#include "core/monique_core_Processor.h"
#include "core/monique_core_Synth.h"

#define SET_MOUSE_WHEEL_SNAP_TO_1000()                                                             \
    setIncDecButtonsMode(juce::Slider::incDecButtonsDraggable_AutoDirection)
#define SET_MOUSE_WHEEL_DOES_NOT_SNAP()                                                            \
    setIncDecButtonsMode(juce::Slider::incDecButtonsNotDraggable)

static inline void setup_slider(juce::Slider *const front_slider_, juce::Slider *const back_slider_,
                                juce::TextButton *const top_button_,
                                juce::TextButton *const bottom_button_,
                                juce::Label *const top_label_, juce::Label *const bottom_label_,
                                ModulationSliderConfigBase *slider_config_,
                                UiLookAndFeel *const look_and_feel, SectionTheme *theme) noexcept
{
    Parameter *front_parameter = slider_config_->get_front_parameter_base();
    Parameter *back_parameter = slider_config_->get_back_parameter_base();
    BoolParameter *top_parameter = slider_config_->get_top_button_parameter_base();
    bool has_bottom_button = slider_config_->get_back_parameter_base() != nullptr;
    bool has_bottom_label = !has_bottom_button;
    if (juce::String(slider_config_->get_bottom_button_text().text) == "")
    {
        has_bottom_label = false;
    }

    bool is_linear = slider_config_->get_is_linear();

    front_slider_->setOpaque(true);
    back_slider_->setVisible(false);
    back_slider_->setEnabled(false);
    back_slider_->setOpaque(true);
    top_button_->setVisible(false);
    top_button_->setEnabled(false);
    top_button_->setOpaque(true);
    bottom_button_->setVisible(false);
    bottom_button_->setEnabled(false);
    bottom_button_->setOpaque(true);
    top_label_->setVisible(slider_config_->get_is_linear());
    top_label_->setEnabled(true);
    top_label_->setOpaque(true);
    top_label_->setEditable(true);
    top_label_->setInterceptsMouseClicks(is_linear, is_linear);
    bottom_label_->setVisible(has_bottom_label);
    bottom_label_->setEnabled(has_bottom_label);
    bottom_label_->setOpaque(true);
    bottom_label_->setInterceptsMouseClicks(false, false);

    // FRONT
    if (front_parameter)
    {
        front_slider_->toFront(true);
        front_slider_->getProperties().set(VAR_INDEX_SLIDER_TYPE,
                                           slider_config_->get_front_slider_style());

        const ParameterInfo &info = front_parameter->get_info();
        {
            const float init = info.init_value;
            front_slider_->setDoubleClickReturnValue(true, init);
            front_slider_->setPopupMenuEnabled(true);
            front_slider_->setValue(front_parameter->get_value(), juce::dontSendNotification);

            if (slider_config_->get_override_front_screw_value())
            {
                front_slider_->SET_MOUSE_WHEEL_SNAP_TO_1000();
            }
            else
            {
                front_slider_->SET_MOUSE_WHEEL_DOES_NOT_SNAP();
            }
        }

        {
            const float max = info.max_value;
            const int override_min_value = slider_config_->get_override_front_min_value();
            if (override_min_value != DONT_OVERRIDE_SLIDER_VALUE)
            {
                const float interval = (max - override_min_value) / info.num_steps;
                front_slider_->setRange(override_min_value, max, interval);
            }
            else
            {
                const float min = info.min_value;
                const float interval = (max - min) / info.num_steps;
                front_slider_->setRange(min, max, interval);
            }
        }

        juce::StringRef tooltip = slider_config_->get_tootip_front();
        if (!tooltip.isEmpty())
        {
            front_slider_->setTooltip(tooltip.text);
        }
    }

    // BACK - SECOND AND MOD
    if (back_parameter)
    {
        front_slider_->setOpaque(false);
        back_slider_->setVisible(true);
        back_slider_->setEnabled(false);
        back_slider_->toBack();
        bottom_button_->setVisible(true);
        bottom_button_->setEnabled(true);

        // SLIDER
        {
            SLIDER_STYLES style = slider_config_->get_back_slider_style();
            back_slider_->getProperties().set(VAR_INDEX_SLIDER_TYPE, style);

            if (style == MODULATION_SLIDER)
            {
                back_slider_->setRange(-1, 1, 0.001);
                // TODO dynamically update?
                back_slider_->setDoubleClickReturnValue(true, 0);
                back_slider_->setPopupMenuEnabled(true);
                back_slider_->setValue(back_parameter->get_modulation_amount(),
                                       juce::dontSendNotification);
            }
            else
            {
                const ParameterInfo &info = back_parameter->get_info();
                {
                    const float init = info.init_value;
                    // TODO dynamically update?
                    back_slider_->setDoubleClickReturnValue(true, init);
                    back_slider_->setPopupMenuEnabled(true);
                    back_slider_->setValue(back_parameter->get_value(), juce::dontSendNotification);
                }

                {
                    const int override_min_value = slider_config_->get_override_front_min_value();
                    const float max = info.max_value;
                    if (override_min_value != DONT_OVERRIDE_SLIDER_VALUE)
                    {
                        const float interval = (max - override_min_value) / info.num_steps;
                        back_slider_->setRange(override_min_value, max, interval);
                    }
                    else
                    {
                        const float min = info.min_value;
                        const float interval = (max - min) / info.num_steps;
                        back_slider_->setRange(min, max, interval);
                    }
                }

                // TODO get_override_front_max_value
            }
        }

        // BOTTOM BUTTON
        bottom_button_->setButtonText(slider_config_->get_bottom_button_text().text);

        juce::StringRef tooltip = slider_config_->get_tootip_bottom();
        if (!tooltip.isEmpty())
        {
            bottom_button_->setTooltip(tooltip.text);
        }

        tooltip = slider_config_->get_tootip_back();
        if (!tooltip.isEmpty())
        {
            back_slider_->setTooltip(tooltip.text);
        }
    }

    if (top_parameter)
    {
        top_button_->setVisible(true);
        top_button_->setEnabled(true);
        top_button_->setButtonText(slider_config_->get_top_button_text().text);

        juce::StringRef tooltip = slider_config_->get_tootip_top();
        if (!tooltip.isEmpty())
        {
            top_button_->setTooltip(tooltip.text);
        }
    }

    if (has_bottom_label)
    {
        bottom_label_->setText(slider_config_->get_bottom_button_text().text,
                               juce::dontSendNotification);
    }
}

bool Monique_Ui_DualSlider::is_in_shift_view() const
{
    return front_parameter->midi_control->get_ctrl_mode();
}

void Monique_Ui_DualSlider::show_view_mode()
{
    const bool is_in_ctrl_mode = front_parameter->midi_control->get_ctrl_mode();
    if (slider_modulation)
    {
        slider_modulation->setOpaque(!is_in_ctrl_mode);
        slider_modulation->setEnabled(is_in_ctrl_mode);

        is_in_ctrl_mode ? slider_value->toBack() : slider_modulation->toBack();
    }
    {
        slider_value->setOpaque(is_in_ctrl_mode || !slider_modulation);
        slider_value->setEnabled(!is_in_ctrl_mode);
    }

    if (button_bottom)
    {
        button_bottom->setButtonText(!is_in_ctrl_mode
                                         ? _config->get_bottom_button_text().text
                                         : _config->get_bottom_button_switch_text().text);
        if (modulation_parameter)
        {
            button_bottom->setToggleState(is_in_ctrl_mode, juce::dontSendNotification);
            if (button_bottom->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                   is_in_ctrl_mode ? MOD_SLIDER_COLOUR : 0))
            {
                button_bottom->repaint();
            }
        }
        else
        {
            button_bottom->setToggleState(is_in_ctrl_mode, juce::dontSendNotification);
            if (button_bottom->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                   is_in_ctrl_mode ? VALUE_SLIDER_2_COLOUR : 0))
            {
                button_bottom->repaint();
            }
        }
    }

    runtime_show_value_popup = runtime_show_value_popup || force_show_center_value ||
                               _config->show_slider_value_on_top_on_change() ==
                                   ModulationSliderConfigBase::SHOW_OWN_VALUE_ALWAYS ||
                               runtime_show_value_popup;
    if (runtime_show_value_popup)
    {
        refresh();
    }
}

void Monique_Ui_DualSlider::update_return_values() noexcept
{
    if (slider_value)
    {
        slider_value->getProperties().set(RETURN_VALUE_FACTORY,
                                          front_parameter->get_info().factory_default_value);
        slider_value->getProperties().set(RETURN_VALUE_PROGRAM,
                                          front_parameter->get_info().program_on_load_value);

        if (int(slider_value->getProperties().getWithDefault(RETURN_VALUE_UNDO, -999)) == -999)
        {
            slider_value->getProperties().set(RETURN_VALUE_UNDO,
                                              front_parameter->get_info().factory_default_value);
        }
    }
    if (slider_modulation)
    {
        if (modulation_parameter)
        {
            slider_modulation->getProperties().set(
                RETURN_VALUE_FACTORY,
                front_parameter->get_info().factory_default_modulation_amount);
            slider_modulation->getProperties().set(
                RETURN_VALUE_PROGRAM,
                front_parameter->get_info().program_on_load_modulation_amount);

            if (int(slider_modulation->getProperties().getWithDefault(RETURN_VALUE_UNDO, -999)) ==
                -999)
            {
                slider_modulation->getProperties().set(
                    RETURN_VALUE_UNDO,
                    front_parameter->get_info().factory_default_modulation_amount);
            }
        }
        else if (back_parameter)
        {
            slider_modulation->getProperties().set(
                RETURN_VALUE_FACTORY, back_parameter->get_info().factory_default_value);
            slider_modulation->getProperties().set(
                RETURN_VALUE_PROGRAM, back_parameter->get_info().program_on_load_value);

            if (int(slider_modulation->getProperties().getWithDefault(RETURN_VALUE_UNDO, -999)) ==
                -999)
            {
                slider_modulation->getProperties().set(
                    RETURN_VALUE_UNDO,
                    back_parameter->get_info().factory_default_modulation_amount);
            }
        }
    }
}

void Monique_Ui_DualSlider::refresh() noexcept
{
    //==============================================================================
    // UPDATE TOP BUTTON
    if (button_top)
    {
        if (button_top->getButtonText() != _config->get_top_button_text().text)
        {
            button_top->setButtonText(_config->get_top_button_text().text);
        }

        if (top_button_type == ModulationSliderConfigBase::TOP_BUTTON_IS_MODULATOR)
        {
            float amp = _config->get_top_button_amp();
            if (amp > FIXED_TOP_BUTTON_COLOUR)
            {
                if (amp < 0)
                    amp *= -1;
                if (amp > 1)
                    amp = 1;
                amp = (amp + 1) * 0.5f;

                button_top->setToggleState(true, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, amp))
                {
                    button_top->repaint();
                }
            }
            else if (amp == TOP_BUTTON_IS_ON)
            {
                button_top->setToggleState(true, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 1))
                {
                    button_top->repaint();
                }
            }
            else if (amp == TOP_BUTTON_IS_OFF)
            {
                button_top->setToggleState(false, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 0))
                {
                    button_top->repaint();
                }
            }
            else if (amp == FIXED_TOP_BUTTON_COLOUR)
            {
                button_top->setToggleState(true, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 1))
                {
                    button_top->repaint();
                }
            }
            else if (top_parameter->get_value() != static_cast<int>(false))
            {
                if (modulation_parameter)
                {
                    if (synth_data->animate_envs)
                    {
                        float modulation =
                            modulation_parameter->get_runtime_info().get_last_modulation_amount();
                        button_top->setToggleState(true, juce::dontSendNotification);
                        if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP,
                                                            (modulation + 1) * 0.5))
                        {
                            button_top->repaint();
                        }
                    }
                    else
                    {
                        button_top->setToggleState(true, juce::dontSendNotification);
                        if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 1))
                        {
                            button_top->repaint();
                        }
                    }
                }
                else
                {
                    button_top->setToggleState(true, juce::dontSendNotification);
                    if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 1))
                    {
                        button_top->repaint();
                    }
                }
            }
            else
            {
                button_top->setToggleState(false, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, 0))
                {
                    button_top->repaint();
                }
            }
        }
        else if (top_button_type == ModulationSliderConfigBase::TOP_BUTTON_IS_ON_OFF)
        {
            bool is_forced_on = false;
            bool is_forced_off = false;
            if (opt_a_parameter)
            {
                is_forced_on = *opt_a_parameter;
            }
            if (opt_b_parameter)
            {
                is_forced_off = *opt_b_parameter;
            }

            if (is_forced_off)
            {
                button_top->setToggleState(false, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, FORCE_BIT_RED))
                {
                    button_top->repaint();
                }
            }
            else if (is_forced_on)
            {
                button_top->setToggleState(true, juce::dontSendNotification);
                if (button_top->getProperties().set(VAR_INDEX_BUTTON_AMP, FORCE_RED))
                {
                    button_top->repaint();
                }
            }
            else
            {
                button_top->setToggleState(
                    top_parameter->get_value() == static_cast<int>(true) ? true : false,
                    juce::dontSendNotification);
                if (button_top->getProperties().set(
                        VAR_INDEX_BUTTON_AMP,
                        top_parameter->get_value() == static_cast<int>(true) ? 1 : 0))
                {
                    button_top->repaint();
                }
            }
        }
    }

    //==============================================================================
    // UPDATE BOTTOM BUTTON
    // -> see show_view_mode
    if (_config->get_is_bottom_button_text_dynamic())
    {
        if (label)
        {
            label->setText(_config->get_bottom_button_text().text, juce::dontSendNotification);
        }
        else if (button_bottom)
        {
            button_bottom->setButtonText(_config->get_bottom_button_text().text);
        }
    }

    //==============================================================================
    // UPDATE SLIDERS
    {
        const float front_value = front_parameter->get_value();
        const bool animate_slider = synth_data->animate_sliders;
        const juce::Component *comp_under_mouse =
            juce::Desktop::getInstance().getMainMouseSource().getComponentUnderMouse();
        bool this_is_under_mouse = comp_under_mouse;
        if (this_is_under_mouse)
        {
            this_is_under_mouse =
                comp_under_mouse == this || comp_under_mouse == slider_value.get() ||
                comp_under_mouse == button_top.get() || comp_under_mouse == button_bottom.get() ||
                comp_under_mouse == slider_modulation.get() || comp_under_mouse == label.get() ||
                comp_under_mouse == label_top.get();
        }
        if (getCurrentlyFocusedComponent() != slider_value.get())
        {
            if (animate_slider)
            {
                const float value_state =
                    front_parameter->get_runtime_info().get_last_value_state();
                if (!this_is_under_mouse && value_state != HAS_NO_VALUE_STATE)
                {
                    slider_value->setValue(value_state, juce::dontSendNotification);
                }
                else
                {
                    slider_value->setValue(front_value, juce::dontSendNotification);
                }
            }
            else
            {
                slider_value->setValue(front_value, juce::dontSendNotification);
            }
        }
        if (is_linear && front_value != last_value)
        {
            // slider_value->setMouseDragSensitivity( 2000.0f*exp(1.0f-front_value) + 100 );
        }
        if (getCurrentlyFocusedComponent() != slider_modulation.get())
        {
            if (slider_modulation)
            {
                if (modulation_parameter)
                {
                    if (animate_slider)
                    {
                        const float value_state =
                            modulation_parameter->get_runtime_info().get_last_modulation_state();
                        if (!this_is_under_mouse)
                        {
                            slider_modulation->setValue(value_state, juce::dontSendNotification);
                        }
                        else
                        {
                            slider_modulation->setValue(
                                modulation_parameter->get_modulation_amount(),
                                juce::dontSendNotification);
                        }
                    }
                    else
                    {
                        slider_modulation->setValue(modulation_parameter->get_modulation_amount(),
                                                    juce::dontSendNotification);
                    }
                }
                else if (back_parameter)
                {
                    if (animate_slider)
                    {
                        const float value_state =
                            back_parameter->get_runtime_info().get_last_value_state();
                        if (!this_is_under_mouse && value_state != HAS_NO_VALUE_STATE)
                        {
                            slider_modulation->setValue(value_state, juce::dontSendNotification);
                        }
                        else
                        {
                            slider_modulation->setValue(back_parameter->get_value(),
                                                        juce::dontSendNotification);
                        }
                    }
                    else
                    {
                        slider_modulation->setValue(back_parameter->get_value(),
                                                    juce::dontSendNotification);
                    }
                }
            }
        }

        last_value = front_value;
    }

    //==============================================================================
    // UPDATE SLIDER CENTER LABEL
    {
        bool is_repaint_required = false; // force_repaint;
        const bool is_in_shift_mode = is_in_shift_view();
        const bool show_popup = runtime_show_value_popup || look_and_feel->show_values_always ||
                                force_show_center_value;
        const int show_value_popup_type = _config->show_slider_value_on_top_on_change();
        if (_config->get_is_linear())
        {
            if (label_top)
            {
                if (!label_top->isBeingEdited())
                {
                    label_top->setVisible(true);
                    label_top->setEnabled(slider_value->isEnabled());
                    label_top->setEditable(slider_value->isEnabled());
                    label_top->setText(_config->get_center_value() +
                                           juce::String(_config->get_center_suffix().text),
                                       juce::dontSendNotification);
                }
            }
        }
        else if (show_popup ||
                 show_value_popup_type == ModulationSliderConfigBase::SHOW_OWN_VALUE_ALWAYS)
        {

            // SHOW DEFAUL CENTER LABEL
            if (show_value_popup_type ==
                ModulationSliderConfigBase::DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE)
            {
                // NON ROTARY
                if (slider_value->isVertical())
                {
                    // if( slider_modulation )
                    //    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
                else if (slider_value->isHorizontal())
                {
                    if (slider_modulation)
                    {
                        slider_modulation->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                               SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                    }
                }
                // ROTARY
                else
                {
                    // BACK SLIDER
                    if (is_in_shift_mode)
                    {
                        if (slider_modulation)
                        {
                            float modulation_value = back_parameter
                                                         ? back_parameter->get_value()
                                                         : front_parameter->get_modulation_amount();
                            last_painted_mod_slider_val = modulation_value;
                            if (slider_modulation->getProperties().set(
                                    VAR_INDEX_VALUE_TO_SHOW,
                                    juce::String(auto_round(modulation_value * 100)) +
                                        juce::String("@") + juce::String("%")))
                            {
                                is_repaint_required = true;
                            }
                            slider_modulation->getProperties().set(
                                VAR_INDEX_SLIDER_LABEL_STYLE,
                                SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        }
                        slider_value->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                          SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                    // FRONT SLIDER
                    else
                    {
                        float value = front_parameter->get_value();
                        last_painted_value_slider_val = value;
                        if (slider_value->getProperties().set(
                                VAR_INDEX_VALUE_TO_SHOW, juce::String(auto_round(value * 100))))
                        {
                            is_repaint_required = true;
                        }
                        slider_value->getProperties().set(
                            VAR_INDEX_SLIDER_LABEL_STYLE,
                            SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        if (slider_modulation)
                        {
                            slider_modulation->getProperties().set(
                                VAR_INDEX_SLIDER_LABEL_STYLE,
                                SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                        }
                    }
                }
            }
            // SHOW DEFINED CENTER LABEL
            else if (show_value_popup_type == ModulationSliderConfigBase::SHOW_OWN_VALUE ||
                     show_value_popup_type == ModulationSliderConfigBase::SHOW_OWN_VALUE_ALWAYS)
            {
                // NON ROTARY
                if (_config->get_is_linear())
                {
                    // label_top->setVisible(true);
                    // label_top->setText(
                    // _config->get_center_value()+String(_config->get_center_suffix().text),
                    // dontSendNotification );
                }
                /*
                else if( slider_value->isHorizontal() )
                {

                }
                */
                // ROTARY
                else
                {
                    // BACK SLIDER
                    if (is_in_shift_mode)
                    {
                        if (slider_modulation)
                        {
                            float modulation_value = back_parameter
                                                         ? back_parameter->get_value()
                                                         : front_parameter->get_modulation_amount();
                            last_painted_mod_slider_val = modulation_value;
                            if (slider_modulation->getProperties().set(
                                    VAR_INDEX_VALUE_TO_SHOW,
                                    _config->get_center_value() + juce::String("@") +
                                        juce::String(_config->get_center_suffix().text)))
                            {
                                is_repaint_required = true;
                            }
                            slider_modulation->getProperties().set(
                                VAR_INDEX_SLIDER_LABEL_STYLE,
                                SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        }
                        slider_value->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                          SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                    // FRONT SLIDER
                    else
                    {
                        float value = front_parameter->get_value();
                        last_painted_value_slider_val = value;
                        if (slider_value->getProperties().set(
                                VAR_INDEX_VALUE_TO_SHOW,
                                _config->get_center_value() + juce::String("@") +
                                    juce::String(_config->get_center_suffix().text)))
                        {
                            is_repaint_required = true;
                        }
                        slider_value->getProperties().set(
                            VAR_INDEX_SLIDER_LABEL_STYLE,
                            SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        if (slider_modulation)
                        {
                            slider_modulation->getProperties().set(
                                VAR_INDEX_SLIDER_LABEL_STYLE,
                                SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                        }
                    }
                }
            }
            else
            {
                slider_value->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                  SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                if (slider_modulation)
                {
                    slider_modulation->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                           SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
            }
        }
        else
        {
            slider_value->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                              SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            if (slider_modulation)
            {
                slider_modulation->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                                       SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            }

            if (label_top)
            {
                label_top->setVisible(false);
            }
        }

        // REPAINT
        if (is_repaint_required || show_popup != last_runtime_show_value_popup)
        {
            last_runtime_show_value_popup = show_popup;

            if (slider_modulation)
            {
                slider_modulation->repaint();
            }
            slider_value->repaint();

            force_repaint = false;
        }
    }
}

void Monique_Ui_DualSlider::set_shift_view_mode(bool mode_)
{
    front_parameter->midi_control->set_ctrl_mode(mode_);
    if (back_parameter)
    {
        back_parameter->midi_control->set_ctrl_mode(mode_);
    }
    // force_repaint = true;
}

void Monique_Ui_DualSlider::sliderClicked(juce::Slider *s_)
{
    if (midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning())
    {
        sliderValueChanged(s_);
    }
}

//==============================================================================
Monique_Ui_DualSlider::Monique_Ui_DualSlider(Monique_Ui_Refresher *ui_refresher_,
                                             ModulationSliderConfigBase *config_)
    : Monique_Ui_Refreshable(ui_refresher_), original_w(60), original_h(130), _config(config_)
{
    force_repaint = false;
    is_opaque = config_->is_opaque();
    is_linear = _config->get_is_linear();
    last_value = 99999;
    force_show_center_value = false;
    audio_processor = ui_refresher_->audio_processor;

    button_bottom = std::make_unique<BottomButton>(juce::String());
    addAndMakeVisible(*button_bottom);
    button_bottom->addListener(this);
    button_bottom->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_bottom->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_bottom->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    slider_modulation = std::make_unique<Left2MiddleSlider>("1");
    addAndMakeVisible(*slider_modulation);
    slider_modulation->setRange(0, 100, 1);
    slider_modulation->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_modulation->setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 20);
    slider_modulation->setColour(juce::Slider::backgroundColourId, juce::Colour(0xff101010));
    slider_modulation->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    slider_modulation->setColour(juce::Slider::trackColourId, juce::Colours::black);
    slider_modulation->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    slider_modulation->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    slider_modulation->setColour(juce::Slider::textBoxTextColourId, juce::Colours::red);
    slider_modulation->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_modulation->setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::black);
    slider_modulation->setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
    slider_modulation->addListener(this);

    label = std::make_unique<Labels>(juce::String(), juce::String());
    addAndMakeVisible(*label);
    label->setFont(juce::Font(15.00f, juce::Font::plain));
    label->setJustificationType(juce::Justification::centred);
    label->setEditable(false, false, false);
    label->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    button_top = std::make_unique<EventButton>(juce::String());
    addAndMakeVisible(*button_top);
    button_top->addListener(this);
    button_top->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    button_top->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    button_top->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    slider_value = std::make_unique<SnapSlider>("0");
    addAndMakeVisible(*slider_value);
    slider_value->setRange(0, 1000, 0.01);
    slider_value->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_value->setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);
    slider_value->setColour(juce::Slider::backgroundColourId, juce::Colour(0xff101010));
    slider_value->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_value->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_value->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_value->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_value->addListener(this);

    label_top = std::make_unique<Labels>(juce::String(), juce::String());
    addAndMakeVisible(*label_top);
    label_top->setFont(juce::Font(15.00f, juce::Font::plain));
    label_top->setJustificationType(juce::Justification::centred);
    label_top->setEditable(true, true, false);
    label_top->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_top->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_top->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_top->addListener(this);

    // SET THEME
    {
        theme = &look_and_feel->colours.get_theme(_config->get_colour_theme());
        getProperties().set(VAR_INDEX_COLOUR_THEME, _config->get_colour_theme());
        for (int i = 0; i != getNumChildComponents(); ++i)
        {
            getChildComponent(i)->getProperties().set(VAR_INDEX_COLOUR_THEME,
                                                      _config->get_colour_theme());
        }
    }
    const juce::Colour &bg_colour = theme->area_colour;
    label->setColour(juce::TextEditor::backgroundColourId, bg_colour);
    label_top->setColour(juce::TextEditor::backgroundColourId, bg_colour);
    slider_value->setColour(juce::Slider::backgroundColourId, bg_colour);
    slider_modulation->setColour(juce::Slider::backgroundColourId, bg_colour);
    // button_bottom->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR,true);
    // button_top->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR,true);

    last_painted_mod_slider_val = -999999;
    last_painted_value_slider_val = -999999;

    // INIT SLIDERS AND BUTTONS
    slider_value->owner = this;
    slider_modulation->owner = this;
    button_top->owner = this;
    button_bottom->owner = this;
    label_top->owner = this;
    label->owner = this;
    if (_config->get_is_linear())
    {
        slider_modulation->setSliderStyle(juce::Slider::LinearVertical);
        slider_value->setSliderStyle(juce::Slider::LinearVertical);
    }
    setup_slider(slider_value.get(), slider_modulation.get(), button_top.get(), button_bottom.get(),
                 label_top.get(), label.get(), _config, look_and_feel, theme);

    jassert(slider_value->isVisible());
    if (!slider_modulation->isVisible())
    {
        slider_modulation = nullptr;
    }
    if (!button_top->isVisible())
    {
        button_top = nullptr;
        opt_a_parameter = nullptr;
        opt_b_parameter = nullptr;
    }
    else
    {
        opt_a_parameter = config_->get_top_button_option_param_a();
        opt_b_parameter = config_->get_top_button_option_param_b();
    }

    if (!button_bottom->isVisible())
    {
        button_bottom = nullptr;
    }
    // if( !label_top->isVisible() )
    //  label_top = nullptr;
    if (!label->isVisible())
    {
        label = nullptr;
    }
    if (!label_top->isVisible())
    {
        label_top = nullptr;
    }

    front_parameter = _config->get_front_parameter_base();
    back_parameter = _config->get_back_parameter_base();
    if (front_parameter == back_parameter)
    {
        modulation_parameter = front_parameter;
        back_parameter = nullptr;
    }
    else
    {
        modulation_parameter = nullptr;
    }
    top_parameter = _config->get_top_button_parameter_base();
    top_button_type = _config->get_top_button_type();

    runtime_show_value_popup = false;
    last_runtime_show_value_popup = false;

    setWantsKeyboardFocus(false);
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
    }

    show_view_mode();
    setOpaque(!_config->use_click_through_hack());
    if (label)
    {
        label->setOpaque(!_config->use_click_through_hack());
    }
    if (label_top)
    {
        label_top->setOpaque(!_config->use_click_through_hack());
    }
    if (button_bottom)
    {
        button_bottom->setOpaque(!_config->use_click_through_hack());
    }
    if (button_top)
    {
        button_top->setOpaque(!_config->use_click_through_hack());
    }
    /*

    setSize (60, 130);

    */
}

Monique_Ui_DualSlider::~Monique_Ui_DualSlider()
{
    button_bottom = nullptr;
    slider_modulation = nullptr;
    label = nullptr;
    button_top = nullptr;
    slider_value = nullptr;
    label_top = nullptr;

    delete _config;
}

//==============================================================================
void Monique_Ui_DualSlider::paint(juce::Graphics &g)
{
    if (!_config->use_click_through_hack())
    {
        g.fillAll(theme->area_colour);
    }
    else
    {
        g.fillAll(juce::Colour(0x00000000));
    }
    /*

    g.fillAll (Colours::black);

    */
#include "mono_ui_includeHacks_BEGIN.h"
}

void Monique_Ui_DualSlider::resized()
{
    WIDTH_AND_HIGHT_FACTORS

    if (slider_value)
        slider_value->setBounds(3, 38, 56, 56);
    if (button_bottom)
        button_bottom->setBounds(0, 100, 60, 30);
    if (slider_modulation)
        slider_modulation->setBounds(3, 38, 56, 56);
    if (label)
        label->setBounds(0, 100, 60, 30);
    if (button_top)
        button_top->setBounds(0, 0, 60, 30);
    if (label_top)
        label_top->setBounds(0, 0, 60, 30);
        /*

        button_bottom->setBounds (0, 100, 60, 30);
        slider_modulation->setBounds (0, 38, 60, 56);
        label->setBounds (0, 100, 60, 30);
        button_top->setBounds (0, 0, 60, 30);
        slider_value->setBounds (0, 38, 60, 56);
        label_top->setBounds (0, 0, 60, 30);
        */
#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_DualSlider::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == button_bottom.get())
    {
        set_shift_view_mode(!front_parameter->midi_control->get_ctrl_mode());
        show_view_mode();
    }
    else if (buttonThatWasClicked == button_top.get())
    {
        if (opt_a_parameter)
        {
            opt_a_parameter->set_value(false);
        }
        if (opt_b_parameter)
        {
            opt_b_parameter->set_value(false);
        }

        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(top_parameter, buttonThatWasClicked)
        else
        {
            if (_config->has_click_impl())
            {
                _config->on_click();
            }
            else
            {
                top_parameter->set_value(!top_parameter->get_value());
            }
        }
        get_editor()->show_info_popup(buttonThatWasClicked, top_parameter->midi_control);
    }
}

void Monique_Ui_DualSlider::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{

    if (sliderThatWasMoved == slider_modulation.get())
    {
        if (back_parameter)
        {
            IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT(front_parameter, back_parameter,
                                                                   sliderThatWasMoved)
            else { back_parameter->set_value(sliderThatWasMoved->getValue()); }
            get_editor()->show_info_popup(sliderThatWasMoved, back_parameter->midi_control);
        }
        else
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(front_parameter, sliderThatWasMoved)
            else { modulation_parameter->set_modulation_amount(sliderThatWasMoved->getValue()); }
            get_editor()->show_info_popup(sliderThatWasMoved, front_parameter->midi_control);
        }
    }
    else if (sliderThatWasMoved == slider_value.get())
    {
        if (back_parameter)
        {
            IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT(front_parameter, back_parameter,
                                                                   sliderThatWasMoved)
            else { front_parameter->set_value(sliderThatWasMoved->getValue()); }
            get_editor()->show_info_popup(sliderThatWasMoved, front_parameter->midi_control);
        }
        else
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(front_parameter, sliderThatWasMoved)
            else { front_parameter->set_value(sliderThatWasMoved->getValue()); }
            get_editor()->show_info_popup(sliderThatWasMoved, front_parameter->midi_control);
        }
    }
}

void Monique_Ui_DualSlider::labelTextChanged(juce::Label *labelThatHasChanged)
{
    if (labelThatHasChanged == label_top.get())
    {
        if (slider_value->isEnabled())
        {
            front_parameter->set_value(
                _config->get_label_edit_value(label_top->getText().getFloatValue()));
            // slider_value->setValue( _config->get_label_edit_value(
            // label_top->getText().getFloatValue()/front_parameter->get_info().num_steps ),
            // sendNotification );
        }
        /*
            else if( slider_modulation )
            {
                if( slider_modulation->isEnabled() )
            {
                    slider_modulation->setValue( label_top->getText().getFloatValue()/5000,
           sendNotification );
            }
            }
            */
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
void Monique_Ui_DualSlider::sliderDragStarted(juce::Slider *slider_)
{
    if (slider_value.get() == slider_)
    {
        audio_processor->beginParameterChangeGesture(front_parameter->get_info().parameter_host_id);
    }
    else if (slider_modulation.get() == slider_)
    {
        if (back_parameter)
        {
            audio_processor->beginParameterChangeGesture(
                back_parameter->get_info().parameter_host_id);
        }
        else
        {
            audio_processor->beginParameterChangeGesture(
                front_parameter->get_info().parameter_host_id + 1);
        }
    }
}
void Monique_Ui_DualSlider::sliderDragEnded(juce::Slider *slider_)
{
    if (slider_value.get() == slider_)
    {
        audio_processor->endParameterChangeGesture(front_parameter->get_info().parameter_host_id);
    }
    else if (slider_modulation.get() == slider_)
    {
        if (back_parameter)
        {
            audio_processor->endParameterChangeGesture(
                back_parameter->get_info().parameter_host_id);
        }
        else
        {
            audio_processor->endParameterChangeGesture(
                front_parameter->get_info().parameter_host_id + 1);
        }
    }
}
//==============================================================================
//==============================================================================
//==============================================================================
void SnapSlider::mouseEnter(const juce::MouseEvent &event) { owner->mouseEnter(event); }
void SnapSlider::mouseExit(const juce::MouseEvent &event) { owner->mouseExit(event); }

void Left2MiddleSlider::mouseEnter(const juce::MouseEvent &event) { owner->mouseEnter(event); }
void Left2MiddleSlider::mouseExit(const juce::MouseEvent &event) { owner->mouseExit(event); }
//==============================================================================
void Monique_Ui_DualSlider::sliderValueEnter(juce::Slider *s_)
{
    runtime_show_value_popup = true;
    if (synth_data->animate_sliders)
    {
        refresh();
    }
}
void Monique_Ui_DualSlider::sliderValueExit(juce::Slider *s_) { runtime_show_value_popup = false; }
void Monique_Ui_DualSlider::sliderModEnter(juce::Slider *s_)
{
    runtime_show_value_popup = true;
    if (synth_data->animate_sliders)
    {
        refresh();
    }
}
void Monique_Ui_DualSlider::sliderModExit(juce::Slider *s_) { runtime_show_value_popup = false; }

//==============================================================================
//==============================================================================
//==============================================================================
void EventButton::mouseDown(const juce::MouseEvent &event)
{
    if (main_window)
    {
        main_window->toggle_modulation_slider_top_button(this, false);
    }
    else
    {
        juce::TextButton::mouseDown(event);
    }
}
void EventButton::mouseUp(const juce::MouseEvent &event)
{
    if (main_window)
    {
        if (main_window->clear_record_timer)
        {
            main_window->stop_clear_chorus();
            juce::TextButton::triggerClick();
        }
    }
    else
    {
        juce::TextButton::mouseUp(event);
    }
}

void EventButton::mouseEnter(const juce::MouseEvent &event)
{
    owner->topButtonEnter(event.eventComponent);
}
void EventButton::mouseExit(const juce::MouseEvent &event)
{
    owner->topButtonExit(event.eventComponent);
}
void BottomButton::mouseEnter(const juce::MouseEvent &event) { owner->mouseEnter(event); }
void BottomButton::mouseExit(const juce::MouseEvent &event) { owner->mouseExit(event); }
void Labels::mouseEnter(const juce::MouseEvent &event) { owner->mouseEnter(event); }
void Labels::mouseExit(const juce::MouseEvent &event) { owner->mouseExit(event); }
//==============================================================================
void Monique_Ui_DualSlider::topButtonEnter(juce::Component *a_)
{
    runtime_show_value_popup = true;
    if (opt_a_parameter != nullptr)
    {
        if (opt_b_parameter != nullptr)
        {
            get_editor()->open_option_popup(button_top.get(), opt_a_parameter, opt_b_parameter,
                                            &synth_data->arp_sequencer_data->is_sequencer,
                                            _config->get_top_button_option_param_a_text(),
                                            _config->get_top_button_option_param_b_text(),
                                            _config->get_top_button_option_param_a_tool_tip(),
                                            _config->get_top_button_option_param_b_tool_tip());
        }
    }
}
void Monique_Ui_DualSlider::topButtonExit(juce::Component *b_)
{
    runtime_show_value_popup = false;
    if (opt_a_parameter != nullptr)
    {
        if (opt_b_parameter != nullptr)
        {
            get_editor()->open_option_popup(nullptr, nullptr, nullptr, nullptr, "", "", "", "");
        }
    }
}

void Monique_Ui_DualSlider::mouseEnter(const juce::MouseEvent &event)
{
    runtime_show_value_popup = true;
}
void Monique_Ui_DualSlider::mouseExit(const juce::MouseEvent &event)
{
    runtime_show_value_popup = false;
}

void Monique_Ui_DualSlider::mouseDown(const juce::MouseEvent &event)
{
    /*
      if( _config->use_click_through_hack() )
      {
          if( this == ui_refresher->editor->getComponentAt( event.x + getX(), event.y + getY() ) )
          {
              toBack();
              if( Button* button = dynamic_cast< Button* >( ui_refresher->editor->getComponentAt(
      event.x + getX(), event.y + getY() ) ) )
              {
                  button->triggerClick();
              }
              toFront(false);
          }
      }
    */
}
