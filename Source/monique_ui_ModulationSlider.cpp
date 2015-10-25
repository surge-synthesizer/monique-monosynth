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
#include "monique_ui_MainWindow.h"
#include "monique_ui_ENVPopup.h"

#include "monique_core_Synth.h"
#include "monique_core_Datastructures.h"

#define SET_MOUSE_WHEEL_SNAP_TO_1000() setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection)
#define SET_MOUSE_WHEEL_DOES_NOT_SNAP() setIncDecButtonsMode(Slider::incDecButtonsNotDraggable)
//[/Headers]

#include "monique_ui_ModulationSlider.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
static inline void setup_slider
(
    Slider*const front_slider_,
    Slider*const back_slider_,
    TextButton*const top_button_,
    TextButton*const bottom_button_,
    Label*const top_label_,
    Label*const bottom_label_,
    ModulationSliderConfigBase* slider_config_,
    UiLookAndFeel*const look_and_feel
)
noexcept
{
    Parameter* front_parameter = slider_config_->get_front_parameter_base();
    Parameter* back_parameter = slider_config_->get_back_parameter_base();
    BoolParameter* top_parameter = slider_config_->get_top_button_parameter_base();
    bool has_bottom_button = slider_config_->get_back_parameter_base() != nullptr;
    bool has_bottom_label = not has_bottom_button;

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
    top_label_->setVisible(false);
    top_label_->setEnabled(true);
    top_label_->setOpaque(true);
    top_label_->setEditable(true);
    bottom_label_->setVisible(has_bottom_label);
    bottom_label_->setEnabled(has_bottom_label);
    bottom_label_->setOpaque(true);

    // FRONT
    if( front_parameter )
    {
        front_slider_->toFront(true);

        SET_SLIDER_STYLE( front_slider_, slider_config_->get_front_slider_style() );

        const ParameterInfo& info = front_parameter->get_info();
        {
            const float init = info.init_value;
            // TODO dynamically update?
            front_slider_->setDoubleClickReturnValue( true, init );
            front_slider_->setPopupMenuEnabled( true );
            front_slider_->setValue( front_parameter->get_value(), dontSendNotification );

            if( slider_config_->get_override_front_screw_value() )
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
            if( override_min_value != DONT_OVERRIDE_SLIDER_VALUE )
            {
                const float interval = (max-override_min_value)/info.num_steps;
                front_slider_->setRange( override_min_value, max, interval );
            }
            else
            {
                const float min = info.min_value;
                const float interval = (max-min)/info.num_steps;
                front_slider_->setRange( min, max, interval );
            }
        }

        StringRef tooltip = slider_config_->get_tootip_front();
        if( not tooltip.isEmpty() )
        {
            front_slider_->setTooltip( tooltip.text );
        }
    }

    // BACK - SECOND AND MOD
    if( back_parameter )
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

            SET_SLIDER_STYLE( back_slider_, style );

            if( style == MODULATION_SLIDER )
            {
                back_slider_->setRange( -1, 1, 0.001 );
                // TODO dynamically update?
                back_slider_->setDoubleClickReturnValue( true, 0 );
                back_slider_->setPopupMenuEnabled( true );
                back_slider_->setValue( back_parameter->get_modulation_amount(), dontSendNotification );
            }
            else
            {
                const ParameterInfo& info = back_parameter->get_info();
                {
                    const float init = info.init_value;
                    // TODO dynamically update?
                    back_slider_->setDoubleClickReturnValue( true, init );
                    back_slider_->setPopupMenuEnabled( true );
                    back_slider_->setValue( back_parameter->get_value(), dontSendNotification );
                }

                {
                    const int override_min_value = slider_config_->get_override_front_min_value();
                    const float max = info.max_value;
                    if( override_min_value != DONT_OVERRIDE_SLIDER_VALUE )
                    {
                        const float interval = (max-override_min_value)/info.num_steps;
                        back_slider_->setRange( override_min_value, max, interval );
                    }
                    else
                    {
                        const float min = info.min_value;
                        const float interval = (max-min)/info.num_steps;
                        back_slider_->setRange( min, max, interval );
                    }
                }

                // TODO get_override_front_max_value
            }
        }

        // BOTTOM BUTTON
        bottom_button_->setButtonText( slider_config_->get_bottom_button_text().text );
        bottom_button_->setColour (TextButton::buttonColourId, look_and_feel->colours.button_off_colour );
        bottom_button_->setColour (TextButton::textColourOnId, look_and_feel->colours.label_text_colour );

        StringRef tooltip = slider_config_->get_tootip_bottom();
        if( not tooltip.isEmpty() )
        {
            bottom_button_->setTooltip( tooltip.text );
        }

        tooltip = slider_config_->get_tootip_back();
        if( not tooltip.isEmpty() )
        {
            back_slider_->setTooltip( tooltip.text );
        }
    }

    if( top_parameter )
    {
        top_button_->setVisible(true);
        top_button_->setEnabled(true);

        top_button_->setButtonText( slider_config_->get_top_button_text().text );
        top_button_->setColour (TextButton::buttonColourId, look_and_feel->colours.button_off_colour );
        top_button_->setColour (TextButton::textColourOnId, look_and_feel->colours.label_text_colour );

        StringRef tooltip = slider_config_->get_tootip_top();
        if( not tooltip.isEmpty() )
        {
            top_button_->setTooltip( tooltip.text );
        }
    }

    if( has_bottom_label )
    {
        bottom_label_->setText( slider_config_->get_bottom_button_text().text, dontSendNotification );
    }
}

bool Monique_Ui_DualSlider::is_in_ctrl_view() const
{
    return front_parameter->midi_control->get_ctrl_mode();
}

void Monique_Ui_DualSlider::show_view_mode()
{
    const bool is_in_ctrl_mode = front_parameter->midi_control->get_ctrl_mode();
    if( slider_modulation )
    {
        slider_modulation->setOpaque( not is_in_ctrl_mode );
        slider_modulation->setEnabled( is_in_ctrl_mode );

        is_in_ctrl_mode ? slider_value->toBack() : slider_modulation->toBack();
    }
    {
        slider_value->setOpaque( is_in_ctrl_mode or not slider_modulation );
        slider_value->setEnabled( not is_in_ctrl_mode );
    }

    if( button_bottom )
    {
        button_bottom->setButtonText( not is_in_ctrl_mode ? _config->get_bottom_button_text().text : _config->get_bottom_button_switch_text().text );
        if( modulation_parameter )
        {
            button_bottom->setColour(TextButton::buttonColourId, is_in_ctrl_mode ? look_and_feel->colours.slider_track_colour_modulation : look_and_feel->colours.button_off_colour );
        }
        else
        {
            button_bottom->setColour(TextButton::buttonColourId, is_in_ctrl_mode ? look_and_feel->colours.slider_track_colour_2 : look_and_feel->colours.button_off_colour );
        }
    }

    if( label_top )
    {
        if( not label_top->isBeingEdited() )
        {
            label_top->SET_LABEL_STYLE( is_in_ctrl_mode ? IS_SECOND_VALUE_LABEL : IS_VALUE_LABEL );
            //label_top->repaint();
        }
    }
}

void Monique_Ui_DualSlider::update_return_values() noexcept
{
    if( slider_value )
    {
        slider_value->setDoubleClickReturnValue( true, front_parameter->get_info().init_value );
    }
    if( slider_modulation )
    {
        if( modulation_parameter )
        {
            slider_modulation->setDoubleClickReturnValue( true, modulation_parameter->get_info().init_modulation_amount );
        }
        else if( back_parameter )
        {
            slider_modulation->setDoubleClickReturnValue( true, back_parameter->get_info().init_value );
        }
    }
}

void Monique_Ui_DualSlider::refresh() noexcept
{
    //==============================================================================
    // UPDATE TOP BUTTON
    if( button_top )
    {
        if( top_button_type == ModulationSliderConfigBase::TOP_BUTTON_IS_MODULATOR )
        {
            float amp = _config->get_top_button_amp();
            if( amp > FIXED_TOP_BUTTON_COLOUR )
            {
                if( amp < 0 )
                    amp*=-1;
                if( amp > 1 )
                    amp = 1;
                amp = (amp+1)*0.5f;

                button_top->setColour
                (
                    TextButton::buttonColourId,
                    look_and_feel->colours.button_on_colour.darker( 1.0f-amp ).interpolatedWith(look_and_feel->colours.button_off_colour,1.0f-amp)
                );
            }
            else if( amp == TOP_BUTTON_IS_ON )
            {
                button_top->setColour
                (
                    TextButton::buttonColourId,
                    look_and_feel->colours.button_on_colour
                );
            }
            else if( amp == TOP_BUTTON_IS_OFF )
            {
                button_top->setColour
                (
                    TextButton::buttonColourId,
                    look_and_feel->colours.button_off_colour
                );
            }
            else if( amp == FIXED_TOP_BUTTON_COLOUR )
            {
                button_top->setColour (TextButton::buttonColourId, look_and_feel->colours.button_on_colour.darker( 1.0f ) );
            }
            else if( top_parameter->get_value() != false )
            {
                if( modulation_parameter )
                {
                    if( synth_data->animate_envs )
                    {
                        float modulation = modulation_parameter->get_runtime_info().get_last_modulation_amount();
                        button_top->setColour (TextButton::buttonColourId, look_and_feel->colours.button_on_colour.darker( 1.0f-modulation ) );
                    }
                    else
                        button_top->setColour (TextButton::buttonColourId, look_and_feel->colours.button_on_colour );
                }
                else
                    button_top->setColour (TextButton::buttonColourId, look_and_feel->colours.button_on_colour );
            }
            else
            {
                button_top->setColour (TextButton::buttonColourId, look_and_feel->colours.button_off_colour );
            }
        }
        else if( top_button_type == ModulationSliderConfigBase::TOP_BUTTON_IS_ON_OFF )
        {
            bool is_forced_on = false;
            bool is_forced_off = false;
            if( opt_a_parameter )
            {
                is_forced_on = *opt_a_parameter;
            }
            if( opt_b_parameter )
            {
                is_forced_off = *opt_b_parameter;
            }

            Colour col;
            if( is_forced_off )
            {
                col = look_and_feel->colours.button_off_colour.interpolatedWith(Colours::red,0.2f);
            }
            else if( is_forced_on )
            {
                col = look_and_feel->colours.button_on_colour.interpolatedWith(Colours::red,0.5f);
            }
            else
            {
                col = top_parameter->get_value() == true ? look_and_feel->colours.button_on_colour : look_and_feel->colours.button_off_colour;
            }

            button_top->setColour (TextButton::buttonColourId, col );
        }
    }

    //==============================================================================
    // UPDATE BOTTOM BUTTON
    // -> see show_view_mode
    if( _config->get_is_bottom_button_text_dynamic() )
    {
        label->setText( _config->get_bottom_button_text().text, dontSendNotification );
    }

    //==============================================================================
    // UPDATE SLIDERS
    slider_value->setValue( front_parameter->get_value(), dontSendNotification );
    if( slider_modulation )
    {
        if( modulation_parameter )
        {
            slider_modulation->setValue( modulation_parameter->get_modulation_amount(), dontSendNotification );
        }
        else if( back_parameter )
        {
            slider_modulation->setValue( back_parameter->get_value(), dontSendNotification );
        }
    }

    //==============================================================================
    // UPDATE SLIDER CENTER LABEL
    {
        bool is_repaint_required = force_repaint;
        const bool show_popup = runtime_show_value_popup || look_and_feel->show_values_always;
        if( slider_value->isVertical() )
        {
            if( not label_top->isBeingEdited() )
            {
                label_top->setVisible(true);
                label_top->setEnabled(slider_value->isEnabled());
                label_top->setEditable(slider_value->isEnabled());
                label_top->setText( _config->get_center_value()+String(_config->get_center_suffix().text), dontSendNotification );
            }
        }
        else if( show_popup )
        {
            const bool show_value_popup_type = _config->show_slider_value_on_top_on_change();
            const bool is_in_ctrl_mode = is_in_ctrl_view();

            // SHOW DEFAUL CENTER LABEL
            if( show_value_popup_type == ModulationSliderConfigBase::DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE )
            {
                // NON ROTARY
                if( slider_value->isVertical() )
                {
                    //if( slider_modulation )
                    //    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
                else if( slider_value->isHorizontal() )
                {
                    if( slider_modulation )
                        SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
                // ROTARY
                else
                {
                    // BACK SLIDER
                    if( is_in_ctrl_mode )
                    {
                        if( slider_modulation )
                        {
                            float modulation_value = slider_modulation->getValue();
                            if( last_painted_mod_slider_val != modulation_value or force_repaint )
                            {
                                last_painted_mod_slider_val = modulation_value;
                                slider_modulation->SET_VALUE_TO_PAINT( String(round01(modulation_value*100)) + String("@") + String("%") );

                                is_repaint_required = true;
                            }
                            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        }
                        SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                    // FRONT SLIDER
                    else
                    {
                        float value = slider_value->getValue();
                        if( last_painted_value_slider_val != value or force_repaint )
                        {
                            last_painted_value_slider_val = value;
                            slider_value->SET_VALUE_TO_PAINT( String(round01(value*100)) );

                            is_repaint_required = true;
                        }
                        SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        if( slider_modulation )
                            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                }
            }
            // SHOW DEFINED CENTER LABEL
            else if( show_value_popup_type == ModulationSliderConfigBase::SHOW_OWN_VALUE )
            {
                // NON ROTARY
                if( slider_value->isVertical() )
                {
                    // label_top->setVisible(true);
                    // label_top->setText( _config->get_center_value()+String(_config->get_center_suffix().text), dontSendNotification );
                }
                else if( slider_value->isHorizontal() )
                {

                }
                // ROTARY
                else
                {
                    // BACK SLIDER
                    if( is_in_ctrl_mode )
                    {
                        if( slider_modulation )
                        {
                            float modulation_value = slider_modulation->getValue();
                            if( last_painted_mod_slider_val != modulation_value or force_repaint )
                            {
                                last_painted_mod_slider_val = modulation_value;
                                slider_modulation->SET_VALUE_TO_PAINT( _config->get_center_value() + String("@") + String(_config->get_center_suffix().text) );
                                is_repaint_required = true;
                            }
                            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        }
                        SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                    // FRONT SLIDER
                    else
                    {
                        float value = slider_value->getValue();
                        // TODO or switched!
                        if( last_painted_value_slider_val != value or force_repaint )
                        {
                            last_painted_value_slider_val = value;
                            slider_value->SET_VALUE_TO_PAINT( _config->get_center_value() + String("@") + String(_config->get_center_suffix().text) );

                            is_repaint_required = true;
                        }
                        SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        if( slider_modulation )
                        {
                            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                        }
                    }
                }
            }
            else
            {
                SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                if( slider_modulation )
                {
                    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
            }
        }
        else
        {
            SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            if( slider_modulation )
            {
                SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            }

            if( label_top )
            {
                label_top->setVisible(false);
            }
        }

        // REPAINT
        if( is_repaint_required or show_popup != last_runtime_show_value_popup )
        {
            last_runtime_show_value_popup = show_popup;

            if( slider_modulation )
            {
               // slider_modulation->repaint();
            }
           // slider_value->repaint();
	    
            force_repaint = false;
        }
    }
}

void Monique_Ui_DualSlider::set_ctrl_view_mode( bool mode_ )
{
    front_parameter->midi_control->set_ctrl_mode( mode_ );
    if( back_parameter )
    {
        back_parameter->midi_control->set_ctrl_mode( mode_ );
    }
    //force_repaint = true;
}

void Monique_Ui_DualSlider::sliderClicked (Slider*s_)
{
    if( midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning() )
    {
        sliderValueChanged(s_);
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_DualSlider::Monique_Ui_DualSlider (Monique_Ui_Refresher*ui_refresher_, ModulationSliderConfigBase* config_)
    : Monique_Ui_Refreshable(ui_refresher_),
      _config(config_),
      original_w(60), original_h(130)
{
    //[Constructor_pre] You can add your own custom stuff here..
    force_repaint = false;
    //[/Constructor_pre]

    addAndMakeVisible (button_top = new EventButton (String::empty));
    button_top->addListener (this);
    button_top->setColour (TextButton::buttonColourId, Colours::black);
    button_top->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_top->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_top = new Label (String::empty,
            String::empty));
    label_top->setFont (Font (15.00f, Font::plain));
    label_top->setJustificationType (Justification::centred);
    label_top->setEditable (true, true, false);
    label_top->setColour (Label::textColourId, Colours::yellow);
    label_top->setColour (TextEditor::textColourId, Colours::black);
    label_top->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    label_top->addListener (this);

    addAndMakeVisible (slider_value = new SnapSlider ("0"));
    slider_value->setRange (0, 1000, 0.01);
    slider_value->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_value->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider_value->setColour (Slider::backgroundColourId, Colour (0xff101010));
    slider_value->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_value->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_value->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_value->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_value->addListener (this);

    addAndMakeVisible (button_bottom = new TextButton (String::empty));
    button_bottom->addListener (this);
    button_bottom->setColour (TextButton::buttonColourId, Colours::black);
    button_bottom->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_bottom->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (slider_modulation = new Left2MiddleSlider ("1"));
    slider_modulation->setRange (0, 100, 1);
    slider_modulation->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_modulation->setTextBoxStyle (Slider::NoTextBox, true, 70, 20);
    slider_modulation->setColour (Slider::backgroundColourId, Colour (0xff101010));
    slider_modulation->setColour (Slider::thumbColourId, Colours::black);
    slider_modulation->setColour (Slider::trackColourId, Colours::black);
    slider_modulation->setColour (Slider::rotarySliderFillColourId, Colours::red);
    slider_modulation->setColour (Slider::rotarySliderOutlineColourId, Colours::black);
    slider_modulation->setColour (Slider::textBoxTextColourId, Colours::red);
    slider_modulation->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_modulation->setColour (Slider::textBoxHighlightColourId, Colours::black);
    slider_modulation->setColour (Slider::textBoxOutlineColourId, Colours::black);
    slider_modulation->addListener (this);

    addAndMakeVisible (label = new Label (String::empty,
                                          String::empty));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::yellow);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    last_painted_mod_slider_val = -999999;
    last_painted_value_slider_val = -999999;

    // INIT SLIDERS AND BUTTONS
    slider_value->owner = this;
    slider_modulation->owner = this;
    button_top->owner = this;
    if( _config->get_is_linear() )
    {
        slider_modulation->setSliderStyle (Slider::LinearVertical );
        slider_value->setSliderStyle (Slider::LinearVertical );
    }
    setup_slider
    (
        slider_value,
        slider_modulation,
        button_top,
        button_bottom,
        label_top,
        label,
        _config,
        look_and_feel
    );

    jassert( slider_value->isVisible() );
    if( not slider_modulation->isVisible() )
    {
        slider_modulation = nullptr;
    }
    if( not button_top->isVisible() )
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

    if( not button_bottom->isVisible() )
    {
        button_bottom = nullptr;
    }
    //if( not label_top->isVisible() )
    //  label_top = nullptr;
    if( not label->isVisible() )
        label = nullptr;

    front_parameter = _config->get_front_parameter_base();
    back_parameter = _config->get_back_parameter_base();
    if( front_parameter == back_parameter )
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
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
    }

    show_view_mode();
    setOpaque(true);
    /*
    //[/UserPreSize]

    setSize (60, 130);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_DualSlider::~Monique_Ui_DualSlider()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    button_top = nullptr;
    label_top = nullptr;
    slider_value = nullptr;
    button_bottom = nullptr;
    slider_modulation = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    delete _config;
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_DualSlider::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.fillAll (look_and_feel->colours.bg);
    /*
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    */
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPaint]
}

void Monique_Ui_DualSlider::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    if( slider_value )
        slider_value->setBounds (0, 37, 60, 56);
    if( button_bottom )
        button_bottom->setBounds (0, 95, 60, 33);
    if( slider_modulation )
        slider_modulation->setBounds (0, 37, 60, 56);
    if( label )
        label->setBounds (0, 95, 60, 33);
    if( button_top )
        button_top->setBounds (0, 0, 60, 27);
    if( label_top )
        label_top->setBounds (0, -4, 60, 35);
    /*
    //[/UserPreResize]

    button_top->setBounds (0, 0, 60, 27);
    label_top->setBounds (0, -4, 60, 35);
    slider_value->setBounds (0, 37, 60, 56);
    button_bottom->setBounds (0, 95, 60, 33);
    slider_modulation->setBounds (0, 37, 60, 56);
    label->setBounds (0, 95, 60, 33);
    //[UserResized] Add your own custom resize handling here..
    */
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_DualSlider::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_top)
    {
        //[UserButtonCode_button_top] -- add your button handler code here..
        if( opt_a_parameter )
        {
            opt_a_parameter->set_value(false);
        }
        if( opt_b_parameter )
        {
            opt_b_parameter->set_value(false);
        }

        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            top_parameter, buttonThatWasClicked
        )
        else
        {
            top_parameter->set_value( top_parameter->get_value() == 1 ? false : true );
        }
        get_editor()->show_info_popup( buttonThatWasClicked, top_parameter->midi_control );
        //[/UserButtonCode_button_top]
    }
    else if (buttonThatWasClicked == button_bottom)
    {
        //[UserButtonCode_button_bottom] -- add your button handler code here..
        set_ctrl_view_mode( ! front_parameter->midi_control->get_ctrl_mode() );
        show_view_mode();
        //[/UserButtonCode_button_bottom]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_DualSlider::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == label_top)
    {
        //[UserLabelCode_label_top] -- add your label text handling code here..
        if( slider_value->isEnabled() )
            slider_value->setValue( _config->get_label_edit_value( label_top->getText().getFloatValue() ), sendNotification );
        else if( slider_modulation )
        {
            if( slider_modulation->isEnabled() )
                slider_modulation->setValue( label_top->getText().getFloatValue()/1000, sendNotification );
        }
        //[/UserLabelCode_label_top]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}

void Monique_Ui_DualSlider::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_value)
    {
        //[UserSliderCode_slider_value] -- add your slider handling code here..
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
            (
                front_parameter,
                sliderThatWasMoved
            )
            else
            {
                front_parameter->set_value( sliderThatWasMoved->getValue() );
            }
        }
        get_editor()->show_info_popup( sliderThatWasMoved, front_parameter->midi_control );
        //[/UserSliderCode_slider_value]
    }
    else if (sliderThatWasMoved == slider_modulation)
    {
        //[UserSliderCode_slider_modulation] -- add your slider handling code here..
        if( back_parameter )
        {
            IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT
            (
                front_parameter,
                back_parameter,
                sliderThatWasMoved
            )
            else
            {
                back_parameter->set_value( sliderThatWasMoved->getValue() );
            }
            get_editor()->show_info_popup( sliderThatWasMoved, back_parameter->midi_control );
        }
        else
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
            (
                front_parameter,
                sliderThatWasMoved
            )
            else
            {
                modulation_parameter->set_modulation_amount( sliderThatWasMoved->getValue() );
            }
            get_editor()->show_info_popup( sliderThatWasMoved, front_parameter->midi_control );
        }
        //[/UserSliderCode_slider_modulation]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//==============================================================================
//==============================================================================
//==============================================================================
void SnapSlider::mouseEnter(const MouseEvent& event)
{
    owner->sliderValueEnter(this);
}
void SnapSlider::mouseExit(const MouseEvent& event)
{
    owner->sliderValueExit(this);
}
void Left2MiddleSlider::mouseEnter(const MouseEvent& event)
{
    owner->sliderModEnter(this);
}
void Left2MiddleSlider::mouseExit(const MouseEvent& event)
{
    owner->sliderModExit(this);
}
//==============================================================================
void Monique_Ui_DualSlider::sliderValueEnter (Slider*s_)
{
    runtime_show_value_popup = true;
    force_repaint = true;
}
void Monique_Ui_DualSlider::sliderValueExit (Slider*s_)
{
    runtime_show_value_popup = false;
}
void Monique_Ui_DualSlider::sliderModEnter (Slider*s_)
{
    runtime_show_value_popup = true;
    force_repaint = true;
}
void Monique_Ui_DualSlider::sliderModExit (Slider*s_)
{
    runtime_show_value_popup = false;
}

//==============================================================================
//==============================================================================
//==============================================================================
void EventButton::mouseEnter(const MouseEvent& event)
{
    owner->topButtonEnter(event.eventComponent);
}
void EventButton::mouseExit(const MouseEvent& event)
{
    owner->topButtonExit(event.eventComponent);
}
//==============================================================================
void Monique_Ui_DualSlider::topButtonEnter (Component*a_)
{
    if( opt_a_parameter != nullptr and opt_b_parameter != nullptr )
    {
        get_editor()->open_option_popup( button_top, opt_a_parameter, opt_b_parameter,
                                         _config->get_top_button_option_param_a_text(), _config->get_top_button_option_param_b_text(),
                                         _config->get_top_button_option_param_a_tool_tip(), _config->get_top_button_option_param_b_tool_tip()
                                       );
    }
}
void Monique_Ui_DualSlider::topButtonExit (Component*b_)
{
    if( opt_a_parameter != nullptr and opt_b_parameter != nullptr )
    {
        get_editor()->open_option_popup( nullptr, nullptr, nullptr, "", "", "", "" );
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_DualSlider" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, ModulationSliderConfigBase* config_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;_config(config_),&#10;original_w(60), original_h(130)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="60" initialHeight="130">
  <BACKGROUND backgroundColour="ff000000"/>
  <TEXTBUTTON name="" id="5c83d26e935f379d" memberName="button_top" virtualName="EventButton"
              explicitFocusOrder="0" pos="0 0 60 27" bgColOff="ff000000" textCol="ffff3b00"
              textColOn="ffffff00" buttonText="" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="" id="91e2ef3f217d2a8f" memberName="label_top" virtualName=""
         explicitFocusOrder="0" pos="0 -4 60 35" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="1" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_value" virtualName="SnapSlider"
          explicitFocusOrder="0" pos="0 37 60 56" bkgcol="ff101010" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="75a8f1f978ef7ab5" memberName="button_bottom" virtualName=""
              explicitFocusOrder="0" pos="0 95 60 33" bgColOff="ff000000" textCol="ffff3b00"
              textColOn="ffffff00" buttonText="" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="1" id="90fdc3c941049bf6" memberName="slider_modulation"
          virtualName="Left2MiddleSlider" explicitFocusOrder="0" pos="0 37 60 56"
          bkgcol="ff101010" thumbcol="ff000000" trackcol="ff000000" rotarysliderfill="ffff0000"
          rotaryslideroutline="ff000000" textboxtext="ffff0000" textboxbkgd="ff161616"
          textboxhighlight="ff000000" textboxoutline="ff000000" min="0"
          max="100" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="70" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 95 60 33" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
