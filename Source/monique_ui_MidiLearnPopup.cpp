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
#include "mono_Parameters.h"
#include "monique_ui_LookAndFeel.h"
#include "PluginProcessor.h"
//[/Headers]

#include "monique_ui_MidiLearnPopup.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorSynthLitePopup::refresh() noexcept
{
    if( _midi_control )
    {
        if( ! combo_midi_listen_type->isMouseOverOrDragging() )
            combo_midi_listen_type->setSelectedItemIndex( _midi_control->get_listen_type()+1, dontSendNotification );
        if( ! combo_midi_number->isMouseOverOrDragging() )
            combo_midi_number->setSelectedItemIndex( _midi_control->get_midi_number(), dontSendNotification );
        if( ! combo_midi_channel->isMouseOverOrDragging() )
            combo_midi_channel->setSelectedItemIndex( _midi_control->get_chnanel()-1, dontSendNotification );
    }
    else
    {
        combo_midi_listen_type->setSelectedItemIndex( 0, dontSendNotification );
        combo_midi_listen_type->setEnabled(false);
        combo_midi_number->setEnabled(false);
        combo_midi_channel->setEnabled(false);
    }
}

void UiEditorSynthLitePopup::set_element_to_show( Component*const comp_ )
{
    related_to_comp = comp_;
    Component* parent = comp_->getParentComponent();
    int x = comp_->getX();
    int y = comp_->getY()+comp_->getHeight();
    while( parent )
    {
        if( parent->getParentComponent() ) // IGNORES THE MAIN WINDOW
        {
            x += parent->getX();
            y += parent->getY();
        }
        parent = parent->getParentComponent();
    }
    setBounds( x-10, y, comp_->getWidth()+20, original_h );
}
void UiEditorSynthLitePopup::update_positions( )
{
    if( related_to_comp )
        set_element_to_show( related_to_comp );
}
//[/MiscUserDefs]

//==============================================================================
UiEditorSynthLitePopup::UiEditorSynthLitePopup (UiEditorSynthLite*const parent_, MIDIControl* midi_control_)
    : parent(parent_),_midi_control(midi_control_), original_w(80), original_h(140)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (combo_midi_listen_type = new ComboBox (String::empty));
    combo_midi_listen_type->setEditableText (false);
    combo_midi_listen_type->setJustificationType (Justification::centredLeft);
    combo_midi_listen_type->setTextWhenNothingSelected (TRANS("TYPE"));
    combo_midi_listen_type->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_midi_listen_type->addListener (this);

    addAndMakeVisible (combo_midi_number = new ComboBox (String::empty));
    combo_midi_number->setEditableText (false);
    combo_midi_number->setJustificationType (Justification::centredLeft);
    combo_midi_number->setTextWhenNothingSelected (TRANS("NR"));
    combo_midi_number->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_midi_number->addListener (this);

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setButtonText (TRANS("ESC X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (combo_midi_channel = new ComboBox (String::empty));
    combo_midi_channel->setEditableText (false);
    combo_midi_channel->setJustificationType (Justification::centredLeft);
    combo_midi_channel->setTextWhenNothingSelected (TRANS("CH"));
    combo_midi_channel->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_midi_channel->addListener (this);


    //[UserPreSize]
    related_to_comp = nullptr;

    combo_midi_listen_type->addItem( "N/A", 1 );
    combo_midi_listen_type->addItem( "CC", 2 );
    combo_midi_listen_type->addItem( "NOTE", 3 );

    for( int i = 1 ; i != 129 ; ++i )
    {
        combo_midi_number->addItem( String(i), i );
    }

    for( int i = 1 ; i != 17 ; ++i )
    {
        combo_midi_channel->addItem( String(i), i );
    }

    /*
    //[/UserPreSize]

    setSize (80, 140);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

UiEditorSynthLitePopup::~UiEditorSynthLitePopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    combo_midi_listen_type = nullptr;
    combo_midi_number = nullptr;
    close = nullptr;
    combo_midi_channel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorSynthLitePopup::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.setColour (Colour (0xbaffffff));
    g.fillRoundedRectangle (1.0f, 10.0f, 78.0f, 129.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 78.0f, 129.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorSynthLitePopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    combo_midi_listen_type->setBounds (10, 20, 60, 30);
    combo_midi_number->setBounds (10, 80, 60, 30);
    close->setBounds (10, 113, 60, 20);
    combo_midi_channel->setBounds (10, 50, 60, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f, 0.0f);
    internalPath1.lineTo (50.0f, 10.0f);
    internalPath1.lineTo (30.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void UiEditorSynthLitePopup::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_midi_listen_type)
    {
        //[UserComboBoxCode_combo_midi_listen_type] -- add your combo box handling code here..
        if( _midi_control )
        {
            _midi_control->train( comboBoxThatHasChanged->getSelectedItemIndex()-1, _midi_control->get_midi_number(), _midi_control->get_chnanel(), _midi_control->get_is_ctrl_version_of_name() );
        }
        //[/UserComboBoxCode_combo_midi_listen_type]
    }
    else if (comboBoxThatHasChanged == combo_midi_number)
    {
        //[UserComboBoxCode_combo_midi_number] -- add your combo box handling code here..
        if( _midi_control )
        {
            _midi_control->train( _midi_control->get_listen_type(), comboBoxThatHasChanged->getSelectedItemIndex(), _midi_control->get_chnanel(), _midi_control->get_is_ctrl_version_of_name() );
        }
        //[/UserComboBoxCode_combo_midi_number]
    }
    else if (comboBoxThatHasChanged == combo_midi_channel)
    {
        //[UserComboBoxCode_combo_midi_channel] -- add your combo box handling code here..
        if( _midi_control )
        {
            _midi_control->train( _midi_control->get_listen_type(), _midi_control->get_midi_number(), comboBoxThatHasChanged->getSelectedItemIndex()+1, _midi_control->get_is_ctrl_version_of_name() );
        }
        //[/UserComboBoxCode_combo_midi_channel]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void UiEditorSynthLitePopup::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        MIDIControlHandler::getInstance()->clear();
        parent->popup = nullptr;
        return;
        //[/UserButtonCode_close]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiEditorSynthLitePopup" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams="UiEditorSynthLite*const parent_, MIDIControl* midi_control_"
                 variableInitialisers="parent(parent_),_midi_control(midi_control_), original_w(80), original_h(140)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="80" initialHeight="140">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="1 10 78 129" cornerSize="10" fill="solid: baffffff" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 40 0 l 50 10 l 30 10 x</PATH>
  </BACKGROUND>
  <COMBOBOX name="" id="c07cef797cf3a611" memberName="combo_midi_listen_type"
            virtualName="" explicitFocusOrder="0" pos="10 20 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="TYPE" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="" id="eaa2024654148814" memberName="combo_midi_number"
            virtualName="" explicitFocusOrder="0" pos="10 80 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="10 113 60 20" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="" id="10b3f623cea9c053" memberName="combo_midi_channel"
            virtualName="" explicitFocusOrder="0" pos="10 50 60 30" editable="0"
            layout="33" items="" textWhenNonSelected="CH" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
