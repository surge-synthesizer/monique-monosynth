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
#include "UiEditorSynthLite.h"
#include "mono_AudioDeviceManager.h"
#include "UiLookAndFeel.h"
//[/Headers]

#include "UiEditorMIDIIO.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorMIDIIO::refresh()
{
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        if( ComboBox*combo_box = dynamic_cast< ComboBox*>( getChildComponent(i) ) )
        {
            // PORTS
            if( combo_box->getTextWhenNoChoicesAvailable() == "NO DEVICE CONNECTED" )
            {
                combo_box->clear( dontSendNotification );
                combo_box->addItemList( _audio_device_manager->get_available_ports( combo_box->getName() ), 2 );
                combo_box->setText( _audio_device_manager->get_selected_device_name( combo_box->getName() ), dontSendNotification );
                if( _audio_device_manager->is_port_open( combo_box->getName() ) ) {
                    // TODO change colour
                }
            }

            // CHANNELS
            else if( combo_box->getTextWhenNoChoicesAvailable() == "OMNI" )
            {
                combo_box->clear( dontSendNotification );
                for( int i = 0+1 ; i != 16+1 ; ++i ) {
                    combo_box->addItem( String( i ) , i );
                }
            }

            // TYPES
        }
    }
}
//[/MiscUserDefs]

//==============================================================================
UiEditorMIDIIO::UiEditorMIDIIO (mono_AudioDeviceManager*const audio_device_manager_)
    : _audio_device_manager(audio_device_manager_), original_w(1040), original_h(500)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("RECEIVE CC:")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredLeft);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_main = new ComboBox ("RECIEVE_MIDI_MAIN"));
    combo_input_main->setEditableText (false);
    combo_input_main->setJustificationType (Justification::centredLeft);
    combo_input_main->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_input_main->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_input_main->addListener (this);

    addAndMakeVisible (combo_input_main_channel = new ComboBox (String::empty));
    combo_input_main_channel->setEditableText (false);
    combo_input_main_channel->setJustificationType (Justification::centredLeft);
    combo_input_main_channel->setTextWhenNothingSelected (TRANS("CH"));
    combo_input_main_channel->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_input_main_channel->addListener (this);

    addAndMakeVisible (label_1 = new Label (String::empty,
                                            TRANS("PORT")));
    label_1->setFont (Font (30.00f, Font::plain));
    label_1->setJustificationType (Justification::centred);
    label_1->setEditable (false, false, false);
    label_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("INPUT:")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_input_main_thru = new ToggleButton (String::empty));
    toggle_input_main_thru->addListener (this);

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredLeft);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_thru = new ComboBox ("SEND_MIDI_THRU"));
    combo_output_thru->setEditableText (false);
    combo_output_thru->setJustificationType (Justification::centredLeft);
    combo_output_thru->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_thru->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_thru->addListener (this);

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("CC IN:")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_cc = new ComboBox ("RECIEVE_CC"));
    combo_input_cc->setEditableText (false);
    combo_input_cc->setJustificationType (Justification::centredLeft);
    combo_input_cc->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_input_cc->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_input_cc->addListener (this);

    addAndMakeVisible (toggle_input_main_cc = new ToggleButton (String::empty));
    toggle_input_main_cc->addListener (this);

    addAndMakeVisible (toggle_input_cc_thru = new ToggleButton (String::empty));
    toggle_input_cc_thru->addListener (this);

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("THRU:")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredLeft);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("CC FB (OUT):")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredLeft);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_cc = new ComboBox ("SEND_MIDI_CC_FEEDBACK"));
    combo_output_cc->setEditableText (false);
    combo_output_cc->setJustificationType (Justification::centredLeft);
    combo_output_cc->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_cc->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_cc->addListener (this);

    addAndMakeVisible (toggle_output_cc_mute = new ToggleButton (String::empty));
    toggle_output_cc_mute->addListener (this);

    addAndMakeVisible (label_23 = new Label (String::empty,
                                             TRANS("CLOCK:")));
    label_23->setFont (Font (30.00f, Font::plain));
    label_23->setJustificationType (Justification::centredLeft);
    label_23->setEditable (false, false, false);
    label_23->setColour (Label::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_clock = new ComboBox ("SEND_MIDI_CLOCK"));
    combo_output_clock->setEditableText (false);
    combo_output_clock->setJustificationType (Justification::centredLeft);
    combo_output_clock->setTextWhenNothingSelected (TRANS("SELECT A DEVICE"));
    combo_output_clock->setTextWhenNoChoicesAvailable (TRANS("NO DEVICE CONNECTED"));
    combo_output_clock->addListener (this);

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setButtonText (TRANS("ESC X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (slider_midi_pickup = new Slider ("0"));
    slider_midi_pickup->setRange (0, 1000, 1);
    slider_midi_pickup->setSliderStyle (Slider::LinearHorizontal);
    slider_midi_pickup->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_midi_pickup->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup->addListener (this);

    addAndMakeVisible (button_midi_learn = new TextButton (String::empty));
    button_midi_learn->setButtonText (TRANS("REFRESH"));
    button_midi_learn->addListener (this);
    button_midi_learn->setColour (TextButton::buttonColourId, Colours::black);
    button_midi_learn->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_midi_learn->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (label_17 = new Label (String::empty,
                                             TRANS("MUTE")));
    label_17->setFont (Font (30.00f, Font::plain));
    label_17->setJustificationType (Justification::centredLeft);
    label_17->setEditable (false, false, false);
    label_17->setColour (Label::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("PICKUP:")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredLeft);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    slider_midi_pickup->setValue( DATA(synth_data).midi_pickup_offset*1000, dontSendNotification );
    refresh();
    //[/UserPreSize]

    setSize (610, 360);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UiEditorMIDIIO::~UiEditorMIDIIO()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_7 = nullptr;
    combo_input_main = nullptr;
    combo_input_main_channel = nullptr;
    label_1 = nullptr;
    label_3 = nullptr;
    toggle_input_main_thru = nullptr;
    label_4 = nullptr;
    label_5 = nullptr;
    combo_output_thru = nullptr;
    label_6 = nullptr;
    combo_input_cc = nullptr;
    toggle_input_main_cc = nullptr;
    toggle_input_cc_thru = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    combo_output_cc = nullptr;
    toggle_output_cc_mute = nullptr;
    label_23 = nullptr;
    combo_output_clock = nullptr;
    close = nullptr;
    slider_midi_pickup = nullptr;
    button_midi_learn = nullptr;
    label_17 = nullptr;
    label_2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorMIDIIO::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "UiDynamicSizeStart.h"
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour (0xffff3b00));
    g.drawRect (0, 0, 610, 360, 2);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (20.0f, 118.0f, 570.0f, 4.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (20.0f, 54.0f, 570.0f, 2.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (15.0f, 256.0f, 570.0f, 1.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorMIDIIO::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_7->setBounds (530 - 90, 100 - 30, 90, 30);
    combo_input_main->setBounds (240 - 130, 100 - 30, 130, 30);
    combo_input_main_channel->setBounds (310 - 60, 100 - 30, 60, 30);
    label_1->setBounds (240 - 130, 50 - 30, 130, 30);
    label_3->setBounds (20, 100 - 30, 80, 30);
    toggle_input_main_thru->setBounds (400, 70, 30, 30);
    label_4->setBounds (390 - 60, 100 - 30, 60, 30);
    label_5->setBounds (20, 300 - 30, 80, 30);
    combo_output_thru->setBounds (240 - 130, 300 - 30, 130, 30);
    label_6->setBounds (100 - 80, 170 - 30, 80, 30);
    combo_input_cc->setBounds (240 - 130, 170 - 30, 130, 30);
    toggle_input_main_cc->setBounds (540, 70, 30, 30);
    toggle_input_cc_thru->setBounds (400, 140, 30, 30);
    label_8->setBounds (390 - 60, 170 - 30, 60, 30);
    label_9->setBounds (100 - 80, 230 - 30, 80, 30);
    combo_output_cc->setBounds (240 - 130, 230 - 30, 130, 30);
    toggle_output_cc_mute->setBounds (540, 200, 30, 30);
    label_23->setBounds (100 - 80, 340 - 30, 80, 30);
    combo_output_clock->setBounds (240 - 130, 340 - 30, 130, 30);
    close->setBounds (605 - 45, 5, 45, 20);
    slider_midi_pickup->setBounds (110, 190 - 10, 480, 10);
    button_midi_learn->setBounds (310 - 60, 230 - 30, 60, 30);
    label_17->setBounds (530 - 50, 230 - 30, 50, 30);
    label_2->setBounds (100 - 80, 200 - 30, 80, 30);
    //[UserResized] Add your own custom resize handling here..
#include "UiDynamicSizeEnd.h"
    //[/UserResized]
}

void UiEditorMIDIIO::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_input_main)
    {
        //[UserComboBoxCode_combo_input_main] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_main]
    }
    else if (comboBoxThatHasChanged == combo_input_main_channel)
    {
        //[UserComboBoxCode_combo_input_main_channel] -- add your combo box handling code here..
        //[/UserComboBoxCode_combo_input_main_channel]
    }
    else if (comboBoxThatHasChanged == combo_output_thru)
    {
        //[UserComboBoxCode_combo_output_thru] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_thru]
    }
    else if (comboBoxThatHasChanged == combo_input_cc)
    {
        //[UserComboBoxCode_combo_input_cc] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_cc]
    }
    else if (comboBoxThatHasChanged == combo_output_cc)
    {
        //[UserComboBoxCode_combo_output_cc] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_cc]
    }
    else if (comboBoxThatHasChanged == combo_output_clock)
    {
        //[UserComboBoxCode_combo_output_clock] -- add your combo box handling code here..
        _audio_device_manager->open_port( comboBoxThatHasChanged->getName(), comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_clock]
    }

    //[UsercomboBoxChanged_Post]
    refresh();
    //[/UsercomboBoxChanged_Post]
}

void UiEditorMIDIIO::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggle_input_main_thru)
    {
        //[UserButtonCode_toggle_input_main_thru] -- add your button handler code here..
        //[/UserButtonCode_toggle_input_main_thru]
    }
    else if (buttonThatWasClicked == toggle_input_main_cc)
    {
        //[UserButtonCode_toggle_input_main_cc] -- add your button handler code here..
        _audio_device_manager->use_main_input_as_cc = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_input_main_cc]
    }
    else if (buttonThatWasClicked == toggle_input_cc_thru)
    {
        //[UserButtonCode_toggle_input_cc_thru] -- add your button handler code here..
        _audio_device_manager->main_input_thru = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_input_cc_thru]
    }
    else if (buttonThatWasClicked == toggle_output_cc_mute)
    {
        //[UserButtonCode_toggle_output_cc_mute] -- add your button handler code here..
        //[/UserButtonCode_toggle_output_cc_mute]
    }
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        AppInstanceStore::getInstance()->editor->editor_midiio = nullptr;
        return;
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == button_midi_learn)
    {
        //[UserButtonCode_button_midi_learn] -- add your button handler code here..
        //[/UserButtonCode_button_midi_learn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void UiEditorMIDIIO::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_midi_pickup)
    {
        //[UserSliderCode_slider_midi_pickup] -- add your slider handling code here..
        DATA(synth_data).midi_pickup_offset = slider_midi_pickup->getValue()/1000;
        //[/UserSliderCode_slider_midi_pickup]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

bool UiEditorMIDIIO::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    bool success = false;
    if( key == KeyPress::escapeKey )
    {
        AppInstanceStore::getInstance()->editor->editor_midiio = nullptr;
        return true;
    }
    return success;
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiEditorMIDIIO" componentName=""
                 parentClasses="public Component" constructorParams="mono_AudioDeviceManager*const audio_device_manager_"
                 variableInitialisers="_audio_device_manager(audio_device_manager_), original_w(1040), original_h(500)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="610" initialHeight="360">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="0 0 610 360" fill="solid: 161616" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="20 118 570 4" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="20 54 570 2" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="15 256 570 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="cc90d2b25e08ea4d" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="530r 100r 90 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RECEIVE CC:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="RECIEVE_MIDI_MAIN" id="7c9b1844748d88e" memberName="combo_input_main"
            virtualName="" explicitFocusOrder="0" pos="240r 100r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <COMBOBOX name="" id="f28f9452a84a6616" memberName="combo_input_main_channel"
            virtualName="" explicitFocusOrder="0" pos="310r 100r 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <LABEL name="" id="d152f9e6390795d1" memberName="label_1" virtualName=""
         explicitFocusOrder="0" pos="240r 50r 130 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="PORT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="af53a5122473eec4" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="20 100r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="INPUT:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="de81426eb5b7f19d" memberName="toggle_input_main_thru"
                virtualName="" explicitFocusOrder="0" pos="400 70 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="a1c517f047c587f1" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="390r 100r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="1b0bd4421c8d9acd" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="20 300r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_THRU" id="8d7dd9d502564afb" memberName="combo_output_thru"
            virtualName="" explicitFocusOrder="0" pos="240r 300r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <LABEL name="" id="9e4ab2352c294adf" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="100r 170r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC IN:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="RECIEVE_CC" id="db50823c1df0e85" memberName="combo_input_cc"
            virtualName="" explicitFocusOrder="0" pos="240r 170r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TOGGLEBUTTON name="" id="9b95f066f9f18093" memberName="toggle_input_main_cc"
                virtualName="" explicitFocusOrder="0" pos="540 70 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="c29d5dd5da0a9644" memberName="toggle_input_cc_thru"
                virtualName="" explicitFocusOrder="0" pos="400 140 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="299bb88252c83194" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="390r 170r 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d17ed12f73d131d7" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="100r 230r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC FB (OUT):" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_CC_FEEDBACK" id="997c13a17c6bb37" memberName="combo_output_cc"
            virtualName="" explicitFocusOrder="0" pos="240r 230r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TOGGLEBUTTON name="" id="baa1942f449ba064" memberName="toggle_output_cc_mute"
                virtualName="" explicitFocusOrder="0" pos="540 200 30 30" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="617ff70d12af87f6" memberName="label_23" virtualName=""
         explicitFocusOrder="0" pos="100r 340r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CLOCK:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_CLOCK" id="82904a7966365c28" memberName="combo_output_clock"
            virtualName="" explicitFocusOrder="0" pos="240r 340r 130 30"
            editable="0" layout="33" items="" textWhenNonSelected="SELECT A DEVICE"
            textWhenNoItems="NO DEVICE CONNECTED"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="605r 5 45 20" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_midi_pickup"
          virtualName="Slider" explicitFocusOrder="0" pos="110 190r 480 10"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="LinearHorizontal"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="53f01906b113ce41" memberName="button_midi_learn"
              virtualName="" explicitFocusOrder="0" pos="310r 230r 60 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="REFRESH"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="833d176af8d85d12" memberName="label_17" virtualName=""
         explicitFocusOrder="0" pos="530r 230r 50 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MUTE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="2416a86178a53ffa" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="100r 200r 80 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="PICKUP:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
