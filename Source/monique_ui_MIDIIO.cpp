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
#ifdef IS_STANDALONE

#include "monique_ui_MainWindow.h"
#include "monique_ui_LookAndFeel.h"

#include "mono_AudioDeviceManager.h"
//[/Headers]

#include "monique_ui_MIDIIO.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_MidiIO::refresh() noexcept
{
    toggle_input_main_thru->setToggleState( _audio_device_manager->main_input_thru, dontSendNotification );
    toggle_input_main_cc->setToggleState( _audio_device_manager->use_main_input_as_cc, dontSendNotification );
    toggle_input_cc_thru->setToggleState( _audio_device_manager->cc_input_thru, dontSendNotification );

    int state_change_counter = _audio_device_manager->get_state_change_counter();
    if( state_change_counter != last_state_change )
    {
        last_state_change = state_change_counter;
        update_combo_boxed();
    }
    
    TURN_BUTTON_ON_OR_OFF( button_midi_learn, midi_control_handler->is_waiting_for_param() ? true : midi_control_handler->is_learning() ? true : false );
}
void Monique_Ui_MidiIO::update_combo_boxed()
{
    // INPUT
    {
        StringArray input_devices = _audio_device_manager->get_available_in_ports();

        // NOTE INPUT
        {
            String selected_item_text = combo_input_main->getText();
            combo_input_main->clear( dontSendNotification );
            combo_input_main->addItemList( input_devices, 1 );
            combo_input_main->addItem( CLOSED_PORT, 1+input_devices.size() );

            // SELECT THE CURRENT ONE
            String selected_note_in_device( _audio_device_manager->get_selected_in_device( mono_AudioDeviceManager::INPUT_ID::NOTES ) );
            const mono_AudioDeviceManager::DEVICE_STATE state( _audio_device_manager->get_selected_in_device_state( mono_AudioDeviceManager::INPUT_ID::NOTES ) );
            switch( state )
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN :
                combo_input_main->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));
                combo_input_main->setSelectedItemIndex( input_devices.indexOf( selected_note_in_device ), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED :
                combo_input_main->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
                combo_input_main->setSelectedItemIndex( input_devices.size(), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED :
                combo_input_main->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));
                combo_input_main->setText( "REMOVED: " +selected_note_in_device, dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR :
                combo_input_main->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.2f));
                combo_input_main->setText( "ERROR: " +selected_note_in_device, dontSendNotification );
                break;
            }
        }

        // CC INPUT
        {
            String selected_item_text = combo_input_cc->getText();
            combo_input_cc->clear( dontSendNotification );
            combo_input_cc->addItemList( input_devices, 1 );
            combo_input_cc->addItem( CLOSED_PORT, 1+input_devices.size() );

            // SELECT THE CURRENT ONE
            String selected_cc_in_device( _audio_device_manager->get_selected_in_device( mono_AudioDeviceManager::INPUT_ID::CC ) );
            const mono_AudioDeviceManager::DEVICE_STATE state( _audio_device_manager->get_selected_in_device_state( mono_AudioDeviceManager::INPUT_ID::CC ) );
            switch( state )
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN :
                combo_input_cc->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));
                combo_input_cc->setSelectedItemIndex( input_devices.indexOf( selected_cc_in_device ), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED :
                combo_input_cc->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
                combo_input_cc->setSelectedItemIndex( input_devices.size(), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED :
                combo_input_cc->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));
                combo_input_cc->setText( "REMOVED: " +selected_cc_in_device, dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR :
                combo_input_cc->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.2f));
                combo_input_cc->setText( "ERROR: " +selected_cc_in_device, dontSendNotification );
                break;
            }
        }

        // INPUT CHANNEL
        int current_channel = _audio_device_manager->input_channel;
        combo_input_main_channel->clear( dontSendNotification );
        {
            combo_input_main_channel->addItem( "OMNI", 1 );
            for( int i = 0 ; i != 16 ; ++i )
            {
                combo_input_main_channel->addItem( String( i+1 ), i+2 );
            }
            combo_input_main_channel->setSelectedItemIndex( current_channel, dontSendNotification );
        }
    }

    // OUTPUT
    {
        StringArray output_devices = _audio_device_manager->get_available_out_ports();

        // CC OUTPUT
        {
            String selected_item_text = combo_output_cc->getText();
            combo_output_cc->clear( dontSendNotification );
            combo_output_cc->addItemList( output_devices, 1 );
            combo_output_cc->addItem( CLOSED_PORT, 1+output_devices.size() );

            // SELECT THE CURRENT ONE
            String selected_cc_out_device( _audio_device_manager->get_selected_out_device( mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK ) );
            const mono_AudioDeviceManager::DEVICE_STATE state( _audio_device_manager->get_selected_out_device_state( mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK ) );
            switch( state )
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN :
                combo_output_cc->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));
                combo_output_cc->setSelectedItemIndex( output_devices.indexOf( selected_cc_out_device ), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED :
                combo_output_cc->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
                combo_output_cc->setSelectedItemIndex( output_devices.size(), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED :
                combo_output_cc->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));
                combo_output_cc->setText( "REMOVED: " +selected_cc_out_device, dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR :
                combo_output_cc->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.2f));
                combo_output_cc->setText( "ERROR: " +selected_cc_out_device, dontSendNotification );
                break;
            }
        }

        // THRU OUTPUT
        {
            String selected_item_text = combo_output_thru->getText();
            combo_output_thru->clear( dontSendNotification );
            combo_output_thru->addItemList( output_devices, 1 );
            combo_output_thru->addItem( CLOSED_PORT, 1+output_devices.size() );

            // SELECT THE CURRENT ONE
            String selected_thru_out_device( _audio_device_manager->get_selected_out_device( mono_AudioDeviceManager::OUTPUT_ID::THRU ) );
            const mono_AudioDeviceManager::DEVICE_STATE state( _audio_device_manager->get_selected_out_device_state( mono_AudioDeviceManager::OUTPUT_ID::THRU ) );
            switch( state )
            {
            case mono_AudioDeviceManager::DEVICE_STATE::OPEN :
                combo_output_thru->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));
                combo_output_thru->setSelectedItemIndex( output_devices.indexOf( selected_thru_out_device ), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::CLOSED :
                combo_output_thru->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
                combo_output_thru->setSelectedItemIndex( output_devices.size(), dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::REMOVED :
                combo_output_thru->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));
                combo_output_thru->setText( "REMOVED: " +selected_thru_out_device, dontSendNotification );
                break;
            case mono_AudioDeviceManager::DEVICE_STATE::ERROR :
                combo_output_thru->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.2f));
                combo_output_thru->setText( "ERROR: " +selected_thru_out_device, dontSendNotification );
                break;
            }
        }
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_MidiIO::Monique_Ui_MidiIO (Monique_Ui_Refresher*ui_refresher_, mono_AudioDeviceManager*const audio_device_manager_)
    : Monique_Ui_Refreshable(ui_refresher_),
      _audio_device_manager(audio_device_manager_),
      original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    last_state_change = -1;
    //[/Constructor_pre]

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("CC")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centred);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_main = new ComboBox ("RECIEVE_MIDI_MAIN"));
    combo_input_main->setTooltip (TRANS("Select a MIDI device as input for notes.\n"
    "\n"
    "PLUGIN: normaly you should keep \"Receive from host\" here.\n"
    "STANDALONE: MIDI-Clock will be received at this input."));
    combo_input_main->setEditableText (false);
    combo_input_main->setJustificationType (Justification::centredLeft);
    combo_input_main->setTextWhenNothingSelected (String::empty);
    combo_input_main->setTextWhenNoChoicesAvailable (String::empty);
    combo_input_main->addListener (this);

    addAndMakeVisible (combo_input_main_channel = new ComboBox (String::empty));
    combo_input_main_channel->setTooltip (TRANS("Select a the MIDI channel there you like to listen to notes.\n"
    "(Kepp OMNI if you are not familiar with MIDI)"));
    combo_input_main_channel->setEditableText (false);
    combo_input_main_channel->setJustificationType (Justification::centredLeft);
    combo_input_main_channel->setTextWhenNothingSelected (TRANS("CH"));
    combo_input_main_channel->setTextWhenNoChoicesAvailable (TRANS("OMNI"));
    combo_input_main_channel->addListener (this);

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("INPUT (Notes, CC optional, Clock)")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_input_main_thru = new ToggleButton (String::empty));
    toggle_input_main_thru->setTooltip (TRANS("Enable this to forward messages from the INPUT device to the THRU device."));
    toggle_input_main_thru->addListener (this);

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("THRU (OUT)")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_thru = new ComboBox ("SEND_MIDI_THRU"));
    combo_output_thru->setTooltip (TRANS("Select a MIDI device there you like to forward incoming MIDI messages.\n"
    "\n"
    "PLUGIN: normaly you should keep \"Send to host\" here."));
    combo_output_thru->setEditableText (false);
    combo_output_thru->setJustificationType (Justification::centredLeft);
    combo_output_thru->setTextWhenNothingSelected (String::empty);
    combo_output_thru->setTextWhenNoChoicesAvailable (String::empty);
    combo_output_thru->addListener (this);

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("Controller INPUT (CC only)")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_input_cc = new ComboBox ("RECIEVE_CC"));
    combo_input_cc->setTooltip (TRANS("Select a MIDI device as input for CC messages.\n"
    "\n"
    "PLUGIN: normaly you should keep \"Receive from host\" here, but if the routing of your host isn\'t the best, you can directly select a MIDI controller device here.\n"
    "\n"
    "See: MIDI TRAIN (right)"));
    combo_input_cc->setEditableText (false);
    combo_input_cc->setJustificationType (Justification::centredLeft);
    combo_input_cc->setTextWhenNothingSelected (String::empty);
    combo_input_cc->setTextWhenNoChoicesAvailable (String::empty);
    combo_input_cc->addListener (this);

    addAndMakeVisible (toggle_input_main_cc = new ToggleButton (String::empty));
    toggle_input_main_cc->setTooltip (TRANS("Enable this to receive also CC MIDI messages from the selected input device."));
    toggle_input_main_cc->addListener (this);

    addAndMakeVisible (toggle_input_cc_thru = new ToggleButton (String::empty));
    toggle_input_cc_thru->setTooltip (TRANS("Enable this to forward messages from the Controller INPUT device to the THRU device."));
    toggle_input_cc_thru->addListener (this);

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("Controller Feedback (OUT)")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredLeft);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_output_cc = new ComboBox ("SEND_MIDI_CC_FEEDBACK"));
    combo_output_cc->setTooltip (TRANS("Select a MIDI device there you like to send the controller feedback messages. Normally this is the input of your MIDI contoller which you have selected as Controller INPUT."));
    combo_output_cc->setEditableText (false);
    combo_output_cc->setJustificationType (Justification::centredLeft);
    combo_output_cc->setTextWhenNothingSelected (String::empty);
    combo_output_cc->setTextWhenNoChoicesAvailable (String::empty);
    combo_output_cc->addListener (this);

    addAndMakeVisible (slider_midi_pickup = new Slider ("0"));
    slider_midi_pickup->setTooltip (TRANS("Define the CC PICKUP in percent. \n"
    "\n"
    "Example:\n"
    "A listen sliders value is 50 (MIN:0, MAX:100).\n"
    "The current position of your MIDI controller slider is 0% or 0.\n"
    "The PICKUP offset is about 50% (middle).\n"
    "\n"
    "If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."));
    slider_midi_pickup->setRange (0, 1000, 1);
    slider_midi_pickup->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_midi_pickup->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_midi_pickup->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_midi_pickup->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_midi_pickup->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_midi_pickup->addListener (this);

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("CC PICKUP OFFSET:")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredRight);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("THRU")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centred);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_midi_learn = new TextButton (String::empty));
    button_midi_learn->setTooltip (TRANS("Enables the MIDI train/learn mode.\n"
    "\n"
    "Handling: enable MIDI train and select a slider or button on the main user interface. A little window pops up. Now you can move a slider on your MIDI controller (sender) to assign it to the element on the user interface (listener).\n"
    "\n"
    "Shortcut: CTRL + m"));
    button_midi_learn->setButtonText (TRANS("TRAIN"));
    button_midi_learn->addListener (this);
    button_midi_learn->setColour (TextButton::buttonColourId, Colours::black);
    button_midi_learn->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    button_midi_learn->setColour (TextButton::textColourOffId, Colours::yellow);


    //[UserPreSize]
    //slider_midi_pickup->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
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
    toggle_input_main_thru->setOpaque(false);
    toggle_input_main_cc->setOpaque(false);
    toggle_input_cc_thru->setOpaque(false);

    slider_midi_pickup->setValue( synth_data->midi_pickup_offset*1000, dontSendNotification );

    update_combo_boxed();

    has_grabbed_focus = false;
    /*
    //[/UserPreSize]

    setSize (1465, 180);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_MidiIO::~Monique_Ui_MidiIO()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_7 = nullptr;
    combo_input_main = nullptr;
    combo_input_main_channel = nullptr;
    label_3 = nullptr;
    toggle_input_main_thru = nullptr;
    label_5 = nullptr;
    combo_output_thru = nullptr;
    label_6 = nullptr;
    combo_input_cc = nullptr;
    toggle_input_main_cc = nullptr;
    toggle_input_cc_thru = nullptr;
    label_9 = nullptr;
    combo_output_cc = nullptr;
    slider_midi_pickup = nullptr;
    label_2 = nullptr;
    label_4 = nullptr;
    button_midi_learn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_MidiIO::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, 0.0f, 1440.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (15.0f, 256.0f, 570.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (560.0f, 70.0f, 1.0f, 51.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (625.0f, 145.0f, 20.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (645.0f, 65.0f, 1.0f, 81.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (625.0f, 65.0f, 350.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (450.0f, 120.0f, 110.0f, 1.0f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (450.0f, 120.0f, 1.0f, 16.0f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_MidiIO::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_7->setBounds (545, 15, 35, 30);
    combo_input_main->setBounds (35, 50, 430, 30);
    combo_input_main_channel->setBounds (475, 50, 60, 30);
    label_3->setBounds (35, 15, 430, 30);
    toggle_input_main_thru->setBounds (595, 50, 60, 30);
    label_5->setBounds (975, 15, 430, 30);
    combo_output_thru->setBounds (975, 50, 465, 30);
    label_6->setBounds (35, 95, 430, 30);
    combo_input_cc->setBounds (35, 130, 430, 30);
    toggle_input_main_cc->setBounds (545, 50, 35, 35);
    toggle_input_cc_thru->setBounds (595, 130, 60, 30);
    label_9->setBounds (975, 95, 430, 30);
    combo_output_cc->setBounds (975, 130, 465, 30);
    slider_midi_pickup->setBounds (865, 100, 60, 60);
    label_2->setBounds (855 - 180, 115, 180, 30);
    label_4->setBounds (595, 15, 60, 30);
    button_midi_learn->setBounds (560 - 85, 130, 85, 30);
    //[UserResized] Add your own custom resize handling here..

    if( not has_grabbed_focus )
    {
        combo_input_main->grabKeyboardFocus();
        has_grabbed_focus = true;
    }
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_MidiIO::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_input_main)
    {
        //[UserComboBoxCode_combo_input_main] -- add your combo box handling code here..
        _audio_device_manager->open_in_port( mono_AudioDeviceManager::INPUT_ID::NOTES, comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_main]
    }
    else if (comboBoxThatHasChanged == combo_input_main_channel)
    {
        //[UserComboBoxCode_combo_input_main_channel] -- add your combo box handling code here..
        _audio_device_manager->input_channel = combo_input_main_channel->getSelectedItemIndex();
        //[/UserComboBoxCode_combo_input_main_channel]
    }
    else if (comboBoxThatHasChanged == combo_output_thru)
    {
        //[UserComboBoxCode_combo_output_thru] -- add your combo box handling code here..
        _audio_device_manager->open_out_port( mono_AudioDeviceManager::OUTPUT_ID::THRU, comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_thru]
    }
    else if (comboBoxThatHasChanged == combo_input_cc)
    {
        //[UserComboBoxCode_combo_input_cc] -- add your combo box handling code here..
        _audio_device_manager->open_in_port( mono_AudioDeviceManager::INPUT_ID::CC, comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_input_cc]
    }
    else if (comboBoxThatHasChanged == combo_output_cc)
    {
        //[UserComboBoxCode_combo_output_cc] -- add your combo box handling code here..
        _audio_device_manager->open_out_port( mono_AudioDeviceManager::OUTPUT_ID::FEEDBACK, comboBoxThatHasChanged->getText() );
        //[/UserComboBoxCode_combo_output_cc]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_MidiIO::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggle_input_main_thru)
    {
        //[UserButtonCode_toggle_input_main_thru] -- add your button handler code here..
        _audio_device_manager->main_input_thru = buttonThatWasClicked->getToggleState();
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
        _audio_device_manager->cc_input_thru = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_input_cc_thru]
    }
    else if (buttonThatWasClicked == button_midi_learn)
    {
        //[UserButtonCode_button_midi_learn] -- add your button handler code here..
        midi_control_handler->toggle_midi_learn();
        get_editor()->show_info_popup(nullptr,nullptr);
        //[/UserButtonCode_button_midi_learn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_MidiIO::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_midi_pickup)
    {
        //[UserSliderCode_slider_midi_pickup] -- add your slider handling code here..
        synth_data->midi_pickup_offset = slider_midi_pickup->getValue()/1000;
        //[/UserSliderCode_slider_midi_pickup]
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_MidiIO" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, mono_AudioDeviceManager*const audio_device_manager_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;_audio_device_manager(audio_device_manager_),&#10;original_w(1465), original_h(180)"
                 snapPixels="5" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="10 0 1440 198" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="15 256 570 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="560 70 1 51" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="625 145 20 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="645 65 1 81" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="625 65 350 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="450 120 110 1" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
    <ROUNDRECT pos="450 120 1 16" cornerSize="1" fill="solid: ffff3b00" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="cc90d2b25e08ea4d" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="545 15 35 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="RECIEVE_MIDI_MAIN" id="7c9b1844748d88e" memberName="combo_input_main"
            virtualName="" explicitFocusOrder="0" pos="35 50 430 30" tooltip="Select a MIDI device as input for notes.&#10;&#10;PLUGIN: normaly you should keep &quot;Receive from host&quot; here.&#10;STANDALONE: MIDI-Clock will be received at this input."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <COMBOBOX name="" id="f28f9452a84a6616" memberName="combo_input_main_channel"
            virtualName="" explicitFocusOrder="0" pos="475 50 60 30" tooltip="Select a the MIDI channel there you like to listen to notes.&#10;(Kepp OMNI if you are not familiar with MIDI)"
            editable="0" layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="OMNI"/>
  <LABEL name="" id="af53a5122473eec4" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="35 15 430 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="INPUT (Notes, CC optional, Clock)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="de81426eb5b7f19d" memberName="toggle_input_main_thru"
                virtualName="" explicitFocusOrder="0" pos="595 50 60 30" tooltip="Enable this to forward messages from the INPUT device to the THRU device."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="" id="1b0bd4421c8d9acd" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="975 15 430 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU (OUT)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_THRU" id="8d7dd9d502564afb" memberName="combo_output_thru"
            virtualName="" explicitFocusOrder="0" pos="975 50 465 30" tooltip="Select a MIDI device there you like to forward incoming MIDI messages.&#10;&#10;PLUGIN: normaly you should keep &quot;Send to host&quot; here."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <LABEL name="" id="9e4ab2352c294adf" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="35 95 430 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="Controller INPUT (CC only)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="RECIEVE_CC" id="db50823c1df0e85" memberName="combo_input_cc"
            virtualName="" explicitFocusOrder="0" pos="35 130 430 30" tooltip="Select a MIDI device as input for CC messages.&#10;&#10;PLUGIN: normaly you should keep &quot;Receive from host&quot; here, but if the routing of your host isn't the best, you can directly select a MIDI controller device here.&#10;&#10;See: MIDI TRAIN (right)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <TOGGLEBUTTON name="" id="9b95f066f9f18093" memberName="toggle_input_main_cc"
                virtualName="" explicitFocusOrder="0" pos="545 50 35 35" tooltip="Enable this to receive also CC MIDI messages from the selected input device."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="" id="c29d5dd5da0a9644" memberName="toggle_input_cc_thru"
                virtualName="" explicitFocusOrder="0" pos="595 130 60 30" tooltip="Enable this to forward messages from the Controller INPUT device to the THRU device."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="" id="d17ed12f73d131d7" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="975 95 430 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="Controller Feedback (OUT)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="SEND_MIDI_CC_FEEDBACK" id="997c13a17c6bb37" memberName="combo_output_cc"
            virtualName="" explicitFocusOrder="0" pos="975 130 465 30" tooltip="Select a MIDI device there you like to send the controller feedback messages. Normally this is the input of your MIDI contoller which you have selected as Controller INPUT."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems=""/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_midi_pickup"
          virtualName="Slider" explicitFocusOrder="0" pos="865 100 60 60"
          tooltip="Define the CC PICKUP in percent. &#10;&#10;Example:&#10;A listen sliders value is 50 (MIN:0, MAX:100).&#10;The current position of your MIDI controller slider is 0% or 0.&#10;The PICKUP offset is about 50% (middle).&#10;&#10;If you move the MIDI controller slider the slider on the user inderface does not change until the slider on your MIDI controller reaches the position of about 25%."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="2416a86178a53ffa" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="855r 115 180 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CC PICKUP OFFSET:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="8b2ddb83988f0903" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="595 15 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THRU" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="53f01906b113ce41" memberName="button_midi_learn"
              virtualName="" explicitFocusOrder="0" pos="560r 130 85 30" tooltip="Enables the MIDI train/learn mode.&#10;&#10;Handling: enable MIDI train and select a slider or button on the main user interface. A little window pops up. Now you can move a slider on your MIDI controller (sender) to assign it to the element on the user interface (listener).&#10;&#10;Shortcut: CTRL + m"
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="TRAIN"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]
