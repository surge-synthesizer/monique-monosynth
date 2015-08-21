/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "UiLookAndFeel.h"
#include "Synth.h"
#include "UiEditorSynthLite.h"
//[/Headers]

#include "mono_ModulationSlider.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
bool mono_ModulationSlider::is_in_ctrl_view() const {
    return _parameter.get_param()->midi_control->get_ctrl_mode();
}
void mono_ModulationSlider::set_ctrl_view_mode( bool mode_ ) const {
    _parameter.get_param()->midi_control->set_ctrl_mode( mode_ );
    if( _modulator_is_own_parameter )
        _modulator_parameter.get_param()->midi_control->set_ctrl_mode( mode_ );
}
void mono_ModulationSlider::refresh() {
    _parameter.write_value_to( slider_value );
    if( _button_parameter )
    {
        float amp =_config->get_top_button_amp();
        if( amp > -2 )
        {
            if( amp < 0 )
                amp*=-1;
            button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f-amp ).interpolatedWith(UiLookAndFeel::getInstance()->colours.button_off_colour,1.0f-amp) );
        }
        else if( amp == FIXED_TOP_BUTTON_COLOUR )
            button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f ) );
        else if( _button_parameter.get_value() )
        {
            if( ! _modulator_is_own_parameter )
            {
                if( DATA( synth_data ).animate_modulations )
                {
                    float modulation = _parameter.get_last_modulation();
                    button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f-modulation ) );
                }
                else
                    button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour.darker( 1.0f ) );
            }
            else
                button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour );
        }
        else
            button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
    }

    if( show_value_popup_type == ModulationSliderConfigBase::DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE )
    {
        if( (show_value_popup || UiLookAndFeel::getInstance()->show_values_always) and slider_modulation->isEnabled() and !_modulator_is_own_parameter )
        {
            bool is_repaint_required = false;
            if( slider_value->isVertical() )
            {
                SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            }
            else if( slider_value->isHorizontal() )
            {
                SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
                SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            }
            else
            {
                float modulation_value = slider_modulation->getValue();
                if( last_painted_mod_slider_val != modulation_value )
                {
                    last_painted_mod_slider_val = modulation_value;
                    slider_modulation->SET_VALUE_TO_PAINT( String(std::floor(modulation_value)) + String( "@") + String("%") );
                    is_repaint_required = true;
                }
                SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
            }

            if( is_repaint_required )
            {
                slider_modulation->repaint();
                slider_value->repaint();
            }
        }
        else
        {
            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
        }
    }
    else if( show_value_popup_type == ModulationSliderConfigBase::SHOW_OWN_VALUE )
    {
        if( (show_value_popup || UiLookAndFeel::getInstance()->show_values_always) )
        {
            bool is_repaint_required = false;
            if( slider_value->isVertical() )
            {
                label_top->setVisible(true);
		label_top->setText( _config->get_top_value()+String(_config->get_top_suffix().text), dontSendNotification );
            }
            else if( slider_value->isHorizontal() )
            {

            }
            else
            {
                if( slider_value->isEnabled() )
                {
                    float value = slider_value->getValue();
                    if( last_painted_value_slider_val != value )
                    {
                        last_painted_value_slider_val = value;
                        slider_value->SET_VALUE_TO_PAINT( _config->get_top_value() + String( "@") + String(_config->get_top_suffix().text) );
                        is_repaint_required = true;
                    }
                    SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                }
                else
                {
                    float modulation_value = slider_modulation->getValue();
                    if( last_painted_mod_slider_val != modulation_value )
                    {
                        last_painted_mod_slider_val = modulation_value;
                        slider_modulation->SET_VALUE_TO_PAINT( _config->get_top_value() + String( "@") + String(_config->get_top_suffix().text) );
                        is_repaint_required = true;
                    }
                    SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
                    SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::JUST_HIDE_CENTER);
                }
            }

            if( is_repaint_required )
            {
                slider_modulation->repaint();
                slider_value->repaint();
            }
        }
        else
        {
            SET_SLIDER_LABEL_STYLE(slider_modulation,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            SET_SLIDER_LABEL_STYLE(slider_value,SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
            label_top->setVisible(false);
        }
    }

    if( _config->get_is_bottom_button_text_dynamic() ) {
        label->setText( _config->get_bottom_button_text().text, dontSendNotification );
    }

    if( !_modulator_is_own_parameter )
        _modulator_parameter.write_modulation_to( slider_modulation, _config->is_modulation_slider_centered() );
    else
        _modulator_parameter.write_value_to( slider_modulation );
}
void mono_ModulationSlider::show_view_mode() {

    if( ! _parameter.get_param()->midi_control->get_ctrl_mode() )
    {
        if( _parameter.has_modulation() || _modulator_is_own_parameter )
            slider_value->setOpaque(false);
        slider_value->toFront(true);
        slider_modulation->setOpaque(true);

        slider_value->setEnabled(true);
        slider_modulation->setEnabled(false);

        button_switch->setButtonText( _config->get_bottom_button_text().text );
        button_switch->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
	
	
	label_top->SET_LABEL_STYLE(IS_VALUE_LABEL);
	label_top->repaint();
    }
    else
    {
        slider_modulation->toFront(true);

        slider_modulation->setOpaque(false);
        slider_value->setOpaque(true);

        slider_modulation->setEnabled(true);
        slider_value->setEnabled(false);

        button_switch->setButtonText( _config->get_botton_button_switch_text().text );
        button_switch->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour );
	
	
	label_top->SET_LABEL_STYLE(IS_SECOND_VALUE_LABEL);
	label_top->repaint();
    }
}
void mono_ModulationSlider::sliderClicked (Slider*s_) {
    if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() )
        sliderValueChanged(s_);
}
//[/MiscUserDefs]

//==============================================================================
mono_ModulationSlider::mono_ModulationSlider (ModulationSliderConfigBase* config_)
    : _config(config_),
      _parameter(_config->get_parameter_compatibility_base()),
      _modulator_parameter(_config->get_modulation_parameter_compatibility_base()),
      _button_parameter(_config->get_button_parameter_compatibility_base())
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

    addAndMakeVisible (button_switch = new TextButton (String::empty));
    button_switch->addListener (this);
    button_switch->setColour (TextButton::buttonColourId, Colours::black);
    button_switch->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_switch->setColour (TextButton::textColourOffId, Colours::yellow);

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

    addAndMakeVisible (button_modulator = new TextButton (String::empty));
    button_modulator->addListener (this);
    button_modulator->setColour (TextButton::buttonColourId, Colours::black);
    button_modulator->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_modulator->setColour (TextButton::textColourOffId, Colours::yellow);

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
    
    // IS THE SECOND SLIDER A OWN PARAM?
    _modulator_is_own_parameter = _parameter != _modulator_parameter;

    slider_value->owner = this;
    slider_modulation->owner = this;

    // STYLE
    if( _config->get_is_linear() )
    {
        slider_modulation->setSliderStyle (Slider::LinearVertical );
        slider_value->setSliderStyle (Slider::LinearVertical );
    }

    // INIT SLIDERS AND BUTTONS
    {
        if( ! _parameter.has_modulation() && ! _modulator_is_own_parameter )
        {
            button_switch->setVisible(false);
            slider_modulation->setVisible( false );
        }
        else
        {
            label->setVisible(false);
        }

        if( ! _button_parameter )
        {
            button_modulator->setVisible( false );
        }

        SET_SLIDER_STYLE( slider_value, SLIDER_STYLES::VALUE_SLIDER );
        _parameter.init_value_slider( slider_value, _config->get_override_min_value() );
        if( !_modulator_is_own_parameter )
        {
            SET_SLIDER_STYLE( slider_modulation, SLIDER_STYLES::MODULATION_SLIDER );
            _modulator_parameter.init_modulation_slider( slider_modulation, _config->is_modulation_slider_centered());
        }
        else
        {
            SET_SLIDER_STYLE( slider_modulation, SLIDER_STYLES::VALUE_SLIDER_2 );
            _modulator_parameter.init_value_slider( slider_modulation, -99999 );
        }

        show_value_popup = false;
        show_value_popup_type = _config->show_slider_value_on_top_on_change();
        label_top->setVisible(false);
    }

    // SET COLOURS AND TEXT
    {
        button_modulator->setButtonText( _config->get_top_button_text().text );
        button_modulator->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
        button_modulator->setColour (TextButton::textColourOnId, UiLookAndFeel::getInstance()->colours.button_text_colour );

        // BUTTON / LABEL
        label->setText( _config->get_bottom_button_text().text, dontSendNotification );
        button_switch->setButtonText( _config->get_bottom_button_text().text );
        button_switch->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
        button_switch->setColour (TextButton::textColourOnId, UiLookAndFeel::getInstance()->colours.button_text_colour );
    }

    slider_modulation->setEnabled(false);

    setOpaque(true);
    //[/UserPreSize]

    setSize (60, 130);


    //[Constructor] You can add your own custom stuff here..
    setWantsKeyboardFocus(false);
    for( int i = 0 ; i < getNumChildComponents() ; ++i ) {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        // getChildComponent(i)->setPaintingIsUnclipped(true);
        getChildComponent(i)->setOpaque(true);
        getChildComponent(i)->setRepaintsOnMouseActivity(false);
    }
    if( ! _modulator_parameter )
        slider_value->setOpaque(true);
    slider_modulation->setOpaque(true);

    show_view_mode();
    //[/Constructor]
}

mono_ModulationSlider::~mono_ModulationSlider()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    slider_value = nullptr;
    button_switch = nullptr;
    slider_modulation = nullptr;
    label = nullptr;
    button_modulator = nullptr;
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
    return;
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
#define original_w 60.0f
#define original_h 130.0f
#include "UiDynamicSizeStart.h"
    //[/UserPaint]
}

void mono_ModulationSlider::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    slider_value->setBounds (0, 37, 60, 56);
    button_switch->setBounds (0, 95, 60, 33);
    slider_modulation->setBounds (0, 37, 60, 56);
    label->setBounds (0, 95, 60, 33);
    button_modulator->setBounds (0, 0, 60, 27);
    label_top->setBounds (0, -4, 60, 35);
    //[UserResized] Add your own custom resize handling here..
    {
        // const float width = 56.0f*(1.0f/original_w*getWidth());
        // const float height = 20.0f*(1.0f/original_h*getHeight());
        // slider_modulation->setTextBoxStyle (Slider::TextBoxBelow, false,  width, height);
        // slider_value->setTextBoxStyle (Slider::TextBoxBelow, false, width, height);
    }
#include "UiDynamicSizeEnd.h"
#undef original_w
#undef original_h
    //[/UserResized]
}

void mono_ModulationSlider::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_value)
    {
        //[UserSliderCode_slider_value] -- add your slider handling code here..
        if( _modulator_is_own_parameter )
        {
            IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT
            (
                _parameter.get_param(),
                _modulator_parameter.get_param(),
                sliderThatWasMoved
            )
            else
            {
                _parameter.read_value_from( slider_value );
            }
        }
        else
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
            (
                _parameter.get_param(),
                sliderThatWasMoved
            )
            else
            {
                _parameter.read_value_from( slider_value );
            }
        }
        AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, _parameter.get_param()->midi_control );
        //[/UserSliderCode_slider_value]
    }
    else if (sliderThatWasMoved == slider_modulation)
    {
        //[UserSliderCode_slider_modulation] -- add your slider handling code here..
        if( _modulator_is_own_parameter )
        {
            IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT
            (
                _parameter.get_param(),
                _modulator_parameter.get_param(),
                sliderThatWasMoved
            )
            else
            {
                _modulator_parameter.read_value_from( slider_modulation );
            }
            AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, _modulator_parameter.get_param()->midi_control );
        }
        else
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
            (
                _parameter.get_param(),
                sliderThatWasMoved
            )
            else
            {
                _modulator_parameter.read_modulation_from( slider_modulation );
            }
            AppInstanceStore::getInstance()->editor->show_info_popup( sliderThatWasMoved, _parameter.get_param()->midi_control );
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

    if (buttonThatWasClicked == button_switch)
    {
        //[UserButtonCode_button_switch] -- add your button handler code here..
        set_ctrl_view_mode( ! _parameter.get_param()->midi_control->get_ctrl_mode() );
        show_view_mode();
        //[/UserButtonCode_button_switch]
    }
    else if (buttonThatWasClicked == button_modulator)
    {
        //[UserButtonCode_button_modulator] -- add your button handler code here..
        if( _button_parameter )
        {
            IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
            (
                _button_parameter.get_param(), buttonThatWasClicked
            )
            else
            {
                _button_parameter.invert();
            }
            AppInstanceStore::getInstance()->editor->show_info_popup( buttonThatWasClicked, _button_parameter.get_param()->midi_control );
        }
        //[/UserButtonCode_button_modulator]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

bool mono_ModulationSlider::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}

bool mono_ModulationSlider::keyStateChanged (const bool isKeyDown)
{
    //[UserCode_keyStateChanged] -- Add your code here...
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyStateChanged]
}

void mono_ModulationSlider::modifierKeysChanged (const ModifierKeys& modifiers)
{
    //[UserCode_modifierKeysChanged] -- Add your code here...
    //[/UserCode_modifierKeysChanged]
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
void mono_ModulationSlider::sliderValueEnter (Slider*s_) {
    show_value_popup = true;
};
void mono_ModulationSlider::sliderValueExit (Slider*s_) {
    show_value_popup = false;
};
void mono_ModulationSlider::sliderModEnter (Slider*s_) {
    show_value_popup = true;
};
void mono_ModulationSlider::sliderModExit (Slider*s_) {
    show_value_popup = false;
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
                 variableInitialisers="_config(config_),&#10;_parameter(_config-&gt;get_parameter_compatibility_base()),&#10;_modulator_parameter(_config-&gt;get_modulation_parameter_compatibility_base()),&#10;_button_parameter(_config-&gt;get_button_parameter_compatibility_base())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="60" initialHeight="130">
  <METHODS>
    <METHOD name="modifierKeysChanged (const ModifierKeys&amp; modifiers)"/>
    <METHOD name="keyStateChanged (const bool isKeyDown)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff000000"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_value" virtualName="SnapSlider"
          explicitFocusOrder="0" pos="0 37 60 56" bkgcol="ff101010" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1000" int="0.010000000000000000208" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="75a8f1f978ef7ab5" memberName="button_switch" virtualName=""
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
  <TEXTBUTTON name="" id="5c83d26e935f379d" memberName="button_modulator" virtualName=""
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
