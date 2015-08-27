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
#include "Synth.h"
#include "UiEditorSynthLite.h"

// BACKUP FROM MONO UI UTILS
/*
// ==============================================================================
ParameterReference::ParameterReference( mono_ParameterCompatibilityBase*const param_compatibility_base_ ) : _base( param_compatibility_base_ ) {}

ParameterReference::operator bool() const {
    return _base;
}

void ParameterReference::write_value_to( Slider*const slider_ ) const {
    slider_->setValue( _base->get_scaled_value()*_base->scale() ,dontSendNotification );
}
void ParameterReference::read_value_from( const Slider*const slider_ ) {
    _base->set_scaled_value( slider_->getValue()/_base->scale() );
}
float ParameterReference::get_value() const {
    return _base->get_scaled_value()/_base->scale();
}
void ParameterReference::invert () {
    _base->set_scaled_value( !_base->get_scaled_value() );
}
void ParameterReference::init_modulation_slider( Slider*const slider_, bool is_modulation_slider_centered_ ) const {
    slider_->setRange( -100, MODULATION_AMOUNT_MAX, 0.1 );
    slider_->setDoubleClickReturnValue( true, 0 );
    slider_->setPopupMenuEnabled( true );
    write_modulation_to( slider_, is_modulation_slider_centered_ );
}
void ParameterReference::write_modulation_to( Slider*const slider_, bool is_modulation_slider_centered_ ) const {
    const float modulation_value = _base->get_modulation_amount();
    float scaled_value = _base->get_scaled_value();
    const float scale = _base->scale();
    if( scale != 1000 )
    {
        if( scaled_value >= 0 )
            scaled_value /= _base->max_unscaled();
        else
            scaled_value /= _base->min_unscaled();
    }

    int modulation_slider_style;
    if( is_modulation_slider_centered_ )
        modulation_slider_style = MODULATION_SLIDER_IS_FIXED_CENTERED;
    else if( _base->min_unscaled() < 0 )
        modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER;
    else
        modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER_FROM_ZERO;

    slider_->setRotaryParameters( scaled_value, modulation_slider_style, true );
    slider_->setValue( modulation_value*MODULATION_AMOUNT_MAX ,dontSendNotification );
}
void ParameterReference::read_modulation_from ( const Slider*const slider_ ) {
    _base->set_modulation_amount( slider_->getValue() / MODULATION_AMOUNT_MAX );
}

float ParameterReference::get_modulation() const {
    return _base->get_modulation_amount();
}
float ParameterReference::get_last_modulation() const {
    return _base->get_last_modulation_amount();
}
bool ParameterReference::has_modulation() const {
    return _base->has_modulation();
}
*/

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
    ModulationSliderConfigBase* slider_config_
)
noexcept
{
    mono_ParameterCompatibilityBase* front_parameter = slider_config_->get_front_parameter_base();
    mono_ParameterCompatibilityBase* back_parameter = slider_config_->get_back_parameter_base();
    mono_ParameterCompatibilityBase* top_parameter = slider_config_->get_top_button_parameter_base();
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
    top_label_->setEnabled(false);
    top_label_->setOpaque(true);
    bottom_label_->setVisible(has_bottom_label);
    bottom_label_->setEnabled(has_bottom_label);
    bottom_label_->setOpaque(true);

    // FRONT
    if( front_parameter )
    {
        front_slider_->toFront(true);

        SET_SLIDER_STYLE( front_slider_, slider_config_->get_front_slider_style() );

        front_slider_->setRange( front_parameter->min_unscaled(), front_parameter->max_unscaled(), front_parameter->slider_interval() );
        front_slider_->setDoubleClickReturnValue( true, front_parameter->reset_unscaled() );
        front_slider_->setPopupMenuEnabled( true );
        front_slider_->setValue( front_parameter->get_scaled_value(), dontSendNotification );

        const int override_front_value = slider_config_->get_override_front_min_value();
        if( override_front_value == DONT_OVERRIDE_SLIDER_VALUE )
            front_slider_->setRange( front_parameter->min_unscaled(), front_parameter->max_unscaled(), front_parameter->slider_interval() );
        else
            front_slider_->setRange( override_front_value, front_parameter->max_unscaled(), front_parameter->slider_interval() );
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
                back_slider_->setRange( -100, MODULATION_AMOUNT_MAX, 0.1 );
                back_slider_->setDoubleClickReturnValue( true, 0 );
                back_slider_->setPopupMenuEnabled( true );
                back_slider_->setValue( back_parameter->get_modulation_amount()*MODULATION_AMOUNT_MAX, dontSendNotification );
            }
            else
            {
                back_slider_->setRange( back_parameter->min_unscaled(), back_parameter->max_unscaled(), back_parameter->slider_interval() );
                back_slider_->setDoubleClickReturnValue( true, back_parameter->reset_unscaled() );
                back_slider_->setPopupMenuEnabled( true );
                back_slider_->setValue( back_parameter->get_scaled_value(), dontSendNotification );

                const int override_front_value = slider_config_->get_override_front_min_value();
                if( override_front_value == DONT_OVERRIDE_SLIDER_VALUE )
                    back_slider_->setRange( back_parameter->min_unscaled(), back_parameter->max_unscaled(), back_parameter->slider_interval() );
                else
                    back_slider_->setRange( override_front_value, back_parameter->max_unscaled(), back_parameter->slider_interval() );

                // TODO get_override_front_max_value
            }
        }

        // BOTTOM BUTTON
        bottom_button_->setButtonText( slider_config_->get_bottom_button_text().text );
        bottom_button_->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
        bottom_button_->setColour (TextButton::textColourOnId, UiLookAndFeel::getInstance()->colours.label_text_colour );
    }


    if( top_parameter )
    {
        top_button_->setVisible(true);
        top_button_->setEnabled(true);

        top_button_->setButtonText( slider_config_->get_top_button_text().text );
        top_button_->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
        top_button_->setColour (TextButton::textColourOnId, UiLookAndFeel::getInstance()->colours.label_text_colour );
    }

    if( has_bottom_label )
    {
        bottom_label_->setText( slider_config_->get_bottom_button_text().text, dontSendNotification );
    }
}

bool mono_ModulationSlider::is_in_ctrl_view() const
{
    return front_parameter->midi_control->get_ctrl_mode();
}

void mono_ModulationSlider::show_view_mode()
{
    const bool is_in_ctrl_mode = front_parameter->midi_control->get_ctrl_mode();
    if( slider_modulation )
    {
        slider_modulation->setOpaque( not is_in_ctrl_mode );
        slider_value->setOpaque( is_in_ctrl_mode );

        slider_modulation->setEnabled( is_in_ctrl_mode );
        slider_value->setEnabled( not is_in_ctrl_mode );

        is_in_ctrl_mode ? slider_value->toBack() : slider_modulation->toBack();

        if( label_top )
        {
            label_top->SET_LABEL_STYLE( is_in_ctrl_mode ? IS_SECOND_VALUE_LABEL : IS_VALUE_LABEL );
            label_top->repaint();
        }
    }
    if( button_bottom )
    {
        button_bottom->setButtonText( not is_in_ctrl_mode ? _config->get_bottom_button_text().text : _config->get_bottom_button_switch_text().text );
        button_bottom->setColour(TextButton::buttonColourId, is_in_ctrl_mode ? UiLookAndFeel::getInstance()->colours.button_on_colour : UiLookAndFeel::getInstance()->colours.button_off_colour );
    }
}

void mono_ModulationSlider::refresh() noexcept
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

                button_top->setColour
                (
                    TextButton::buttonColourId,
                    UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f-amp ).interpolatedWith(UiLookAndFeel::getInstance()->colours.button_off_colour,1.0f-amp)
                );
            }
            else if( amp == FIXED_TOP_BUTTON_COLOUR )
            {
                button_top->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f ) );
            }
            else if( top_parameter->get_scaled_value() > 0 )
            {
                if( modulation_parameter )
                {
                    if( DATA( synth_data ).animate_modulations )
                    {
                        float modulation = modulation_parameter->get_last_modulation_amount();
                        button_top->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f-modulation ) );
                    }
                    else
                        button_top->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f ) );
                }
                else
                    button_top->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour );
            }
        }
        else if( top_button_type == ModulationSliderConfigBase::TOP_BUTTON_IS_ON_OFF )
        {
            button_top->setColour (TextButton::buttonColourId, top_parameter->get_scaled_value() == true ? UiLookAndFeel::getInstance()->colours.button_on_colour : UiLookAndFeel::getInstance()->colours.button_off_colour );
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
    slider_value->setValue( front_parameter->get_scaled_value()*front_parameter->scale(), dontSendNotification );
    if( slider_modulation )
    {
        if( modulation_parameter )
        {
            // _modulator_parameter.write_modulation_to( slider_modulation, _config->is_modulation_slider_centered() );

            /*
                const float modulation_value = _base->get_modulation_amount();
            float scaled_value = _base->get_scaled_value();
            const float scale = _base->scale();
            if( scale != 1000 )
            {
              if( scaled_value >= 0 )
                  scaled_value /= _base->max_unscaled();
              else
                  scaled_value /= _base->min_unscaled();
            }

            int modulation_slider_style;
            if( is_modulation_slider_centered_ )
              modulation_slider_style = MODULATION_SLIDER_IS_FIXED_CENTERED;
            else if( _base->min_unscaled() < 0 )
              modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER;
            else
              modulation_slider_style = MODULATION_SLIDER_MOVES_WITH_MASTER_FROM_ZERO;

            slider_->setRotaryParameters( scaled_value, modulation_slider_style, true );
            slider_->setValue( modulation_value*MODULATION_AMOUNT_MAX ,dontSendNotification );
            */
        }
        else if( back_parameter )
        {
            slider_modulation->setValue( back_parameter->get_scaled_value()*back_parameter->scale(), dontSendNotification );
        }
    }

    //==============================================================================
    // UPDATE SLIDER CENTER LABEL
    {
        bool is_repaint_required = false;
        const bool show_popup = runtime_show_value_popup || UiLookAndFeel::getInstance()->show_values_always;
        if( show_popup )
        {
            const bool show_value_popup_type = _config->show_slider_value_on_top_on_change();
            const bool is_in_ctrl_mode = is_in_ctrl_view();

            // SHOW DEFAUL CENTER LABEL
            if( show_value_popup_type == ModulationSliderConfigBase::DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE )
            {
                // NON ROTARY
                if( slider_value->isVertical() )
                {
                    if( slider_modulation )
                        SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                    SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                }
                else if( slider_value->isHorizontal() )
                {
                    if( slider_modulation )
                        SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                    SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
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
                            if( last_painted_mod_slider_val != modulation_value )
                            {
                                last_painted_mod_slider_val = modulation_value;
                                slider_modulation->SET_VALUE_TO_PAINT( String(mono_floor(modulation_value)) + String("@") + String("%") );

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
                        if( last_painted_value_slider_val != value )
                        {
                            last_painted_value_slider_val = value;
                            slider_value->SET_VALUE_TO_PAINT( String(mono_floor(value/10)) );

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
                    label_top->setVisible(true);
                    label_top->setText( _config->get_center_value()+String(_config->get_center_suffix().text), dontSendNotification );
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
                            if( last_painted_mod_slider_val != modulation_value )
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
                        if( last_painted_value_slider_val != value )
                        {
                            last_painted_value_slider_val = value;
                            slider_value->SET_VALUE_TO_PAINT( _config->get_center_value() + String("@") + String(_config->get_center_suffix().text) );

                            is_repaint_required = true;
                        }
                        SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                        if( slider_modulation )
                            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                    }
                }
            }
            else
            {
                SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                if( slider_modulation )
                    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            }
        }
        else
        {
            SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            if( slider_modulation )
                SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);

            if( label_top )
                label_top->setVisible(false);
        }

        // REPAINT
        if( is_repaint_required or show_popup != last_runtime_show_value_popup )
        {
            last_runtime_show_value_popup = show_popup;

            if( slider_modulation )
                slider_modulation->repaint();
            slider_value->repaint();
        }
    }
}

void mono_ModulationSlider::set_ctrl_view_mode( bool mode_ ) const
{
    front_parameter->midi_control->set_ctrl_mode( mode_ );
    if( back_parameter )
        back_parameter->midi_control->set_ctrl_mode( mode_ );
}

void mono_ModulationSlider::sliderClicked (Slider*s_)
{
    if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() )
        sliderValueChanged(s_);
}
//[/MiscUserDefs]

//==============================================================================
mono_ModulationSlider::mono_ModulationSlider (ModulationSliderConfigBase* config_)
    : _config(config_),original_w(60), original_h(130)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

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

    addAndMakeVisible (button_top = new TextButton (String::empty));
    button_top->addListener (this);
    button_top->setColour (TextButton::buttonColourId, Colours::black);
    button_top->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_top->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_top = new Label (String::empty,
                                              String::empty));
    label_top->setFont (Font (15.00f, Font::plain));
    label_top->setJustificationType (Justification::centred);
    label_top->setEditable (false, false, false);
    label_top->setColour (Label::textColourId, Colours::yellow);
    label_top->setColour (TextEditor::textColourId, Colours::black);
    label_top->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    last_painted_mod_slider_val = -999999;
    last_painted_value_slider_val = -999999;

    // INIT SLIDERS AND BUTTONS
    slider_value->owner = this;
    slider_modulation->owner = this;
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
        _config
    );

    jassert( slider_value->isVisible() );
    if( not slider_modulation->isVisible() )
        slider_modulation = nullptr;
    if( not button_top->isVisible() )
        button_top = nullptr;
    if( not button_bottom->isVisible() )
        button_bottom = nullptr;
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
        modulation_parameter = nullptr;
    top_parameter = _config->get_top_button_parameter_base();
    top_button_type = _config->get_top_button_type();

    runtime_show_value_popup = false;
    last_runtime_show_value_popup = false;

    setWantsKeyboardFocus(false);
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        getChildComponent(i)->setRepaintsOnMouseActivity(false);
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

mono_ModulationSlider::~mono_ModulationSlider()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    slider_value = nullptr;
    button_bottom = nullptr;
    slider_modulation = nullptr;
    label = nullptr;
    button_top = nullptr;
    label_top = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    delete _config;
    //[/Destructor]
}

//==============================================================================
void mono_ModulationSlider::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.fillAll (UiLookAndFeel::getInstance()->colours.bg);
    /*
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    */
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPaint]
}

void mono_ModulationSlider::resized()
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

    slider_value->setBounds (0, 37, 60, 56);
    button_bottom->setBounds (0, 95, 60, 33);
    slider_modulation->setBounds (0, 37, 60, 56);
    label->setBounds (0, 95, 60, 33);
    button_top->setBounds (0, 0, 60, 27);
    label_top->setBounds (0, -4, 60, 35);
    //[UserResized] Add your own custom resize handling here..
    */
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void mono_ModulationSlider::sliderValueChanged (Slider* sliderThatWasMoved)
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
                front_parameter->set_scaled_value( sliderThatWasMoved->getValue()/front_parameter->scale() );
            }
        }
        AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, front_parameter->midi_control );
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
                back_parameter->set_scaled_value( sliderThatWasMoved->getValue()/back_parameter->scale() );
            }
            AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, back_parameter->midi_control );
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
                modulation_parameter->set_modulation_amount( sliderThatWasMoved->getValue() / MODULATION_AMOUNT_MAX );
            }
            AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, front_parameter->midi_control );
        }
        //[/UserSliderCode_slider_modulation]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void mono_ModulationSlider::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_bottom)
    {
        //[UserButtonCode_button_bottom] -- add your button handler code here..
        set_ctrl_view_mode( ! front_parameter->midi_control->get_ctrl_mode() );
        show_view_mode();
        //[/UserButtonCode_button_bottom]
    }
    else if (buttonThatWasClicked == button_top)
    {
        //[UserButtonCode_button_top] -- add your button handler code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            top_parameter, buttonThatWasClicked
        )
        else
        {
            top_parameter->set_scaled_value( top_parameter->get_scaled_value() == true ? false : true );
        }
        AppInstanceStore::getInstance()->editor->show_info_popup( buttonThatWasClicked, top_parameter->midi_control );
        //[/UserButtonCode_button_top]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
void mono_ModulationSlider::sliderValueEnter (Slider*s_)
{
    runtime_show_value_popup = true;
};
void mono_ModulationSlider::sliderValueExit (Slider*s_)
{
    runtime_show_value_popup = false;
};
void mono_ModulationSlider::sliderModEnter (Slider*s_)
{
    runtime_show_value_popup = true;
};
void mono_ModulationSlider::sliderModExit (Slider*s_)
{
    runtime_show_value_popup = false;
};
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="mono_ModulationSlider" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams="ModulationSliderConfigBase* config_"
                 variableInitialisers="_config(config_),original_w(60), original_h(130)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="60" initialHeight="130">
  <BACKGROUND backgroundColour="ff000000"/>
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
  <TEXTBUTTON name="" id="5c83d26e935f379d" memberName="button_top" virtualName=""
              explicitFocusOrder="0" pos="0 0 60 27" bgColOff="ff000000" textCol="ffff3b00"
              textColOn="ffffff00" buttonText="" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="" id="91e2ef3f217d2a8f" memberName="label_top" virtualName=""
         explicitFocusOrder="0" pos="0 -4 60 35" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
