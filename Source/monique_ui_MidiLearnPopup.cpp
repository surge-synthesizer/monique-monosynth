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
#include "monique_ui_MainWindow.h"
#include "monique_ui_LookAndFeel.h"

#include "monique_core_Processor.h"
//[/Headers]

#include "monique_ui_MidiLearnPopup.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_MainwindowPopup::refresh() noexcept
{
    if( _midi_control )
    {
        if( not combo_midi_number->isMouseOverOrDragging() )
        {
            combo_midi_number->setSelectedItemIndex( _midi_control->get_midi_number(), dontSendNotification );
        }
    }
    else
    {
        combo_midi_number->setEnabled(false);
    }
}

void Monique_Ui_MainwindowPopup::set_element_to_show( Component*const comp_ )
{
    related_to_comp = comp_;
    int x = get_editor()->getLocalPoint(comp_,Point<int>(0,0)).getX();
    int y = get_editor()->getLocalPoint(comp_,Point<int>(0,0)).getY();
    setTopLeftPosition( x+comp_->getWidth()/2 - getWidth()/2, y+comp_->getHeight() );

    COLOUR_THEMES theme = static_cast<COLOUR_THEMES>( int(comp_->getProperties().getWithDefault( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME )) );
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        Component*child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set( VAR_INDEX_COLOUR_THEME, theme );
    }
    this->getProperties().set( VAR_INDEX_COLOUR_THEME, theme );
}
void Monique_Ui_MainwindowPopup::update_positions( )
{
    if( related_to_comp )
    {
        set_element_to_show( related_to_comp );
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_MainwindowPopup::Monique_Ui_MainwindowPopup (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, MIDIControl* midi_control_)
    : Monique_Ui_Refreshable(ui_refresher_),
      DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point<int>(10,10))),
      parent(parent_),
      _midi_control(midi_control_),
      original_w(80), original_h(95)
{
    //[Constructor_pre] You can add your own custom stuff here..
    setOwner(this);
    //[/Constructor_pre]

    addAndMakeVisible (combo_midi_number = new ComboBox (String::empty));
    combo_midi_number->setTooltip (TRANS("Select the MIDI controller number manually."));
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


    //[UserPreSize]
    related_to_comp = nullptr;
    
    close->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );

    for( int i = 0 ; i != 128 ; ++i )
    {
        combo_midi_number->addItem( String(i), i+1 );
    }

    /*
    //[/UserPreSize]

    setSize (80, 95);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_MainwindowPopup::~Monique_Ui_MainwindowPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    combo_midi_number = nullptr;
    close = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_MainwindowPopup::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect( getWidth()-10, getHeight()-10, 10,10);
#include "mono_ui_includeHacks_BEGIN.h"

    COLOUR_THEMES theme_id =  static_cast<COLOUR_THEMES>( int(this->getProperties().getWithDefault( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME ) ) );
    g.setColour (colours.get_theme( theme_id ).area_colour);
    g.fillRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f);

    g.setColour (colours.get_theme( theme_id ).value_slider_track_colour);
    g.drawRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f, 1.000f);
    
    g.fillPath (internalPath1);

    /*
    //[/UserPrePaint]

    g.setColour (Colour (0xbaffffff));
    g.fillRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 78.0f, 84.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    */
    //[/UserPaint]
}

void Monique_Ui_MainwindowPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    combo_midi_number->setBounds (10, 20, 60, 30);
    close->setBounds (10, 55, 60, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f, 0.0f);
    internalPath1.lineTo (50.0f, 10.0f);
    internalPath1.lineTo (30.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_MainwindowPopup::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_midi_number)
    {
        //[UserComboBoxCode_combo_midi_number] -- add your combo box handling code here..
        if( _midi_control )
        {
            _midi_control->train( comboBoxThatHasChanged->getSelectedItemIndex(), _midi_control->get_is_ctrl_version_of_name(), synth_data->audio_processor );
        }
        //[/UserComboBoxCode_combo_midi_number]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_MainwindowPopup::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        midi_control_handler->clear();
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_MainwindowPopup"
                 componentName="" parentClasses="public Component, public Monique_Ui_Refreshable, public DropShadower"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, MIDIControl* midi_control_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10))),&#10;parent(parent_),&#10;_midi_control(midi_control_),&#10;original_w(80), original_h(95)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="80" initialHeight="95">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="1 10 78 84" cornerSize="10" fill="solid: baffffff" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 40 0 l 50 10 l 30 10 x</PATH>
  </BACKGROUND>
  <COMBOBOX name="" id="eaa2024654148814" memberName="combo_midi_number"
            virtualName="" explicitFocusOrder="0" pos="10 20 60 30" tooltip="Select the MIDI controller number manually."
            editable="0" layout="33" items="" textWhenNonSelected="NR" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="10 55 60 30" bgColOff="ffff0000"
              bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000" buttonText="ESC X"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
