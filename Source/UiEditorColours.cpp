/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "UiLookAndFeel.h"
//[/Headers]

#include "UiEditorColours.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorColourStyler::timerCallback() {
    *editable_colour = colour_selector->getCurrentColour();
}
//[/MiscUserDefs]

//==============================================================================
UiEditorColourStyler::UiEditorColourStyler (ComponentColours* const colours_)
    : _colours( colours_ ),editable_colour( &colours_->slider_text_colour )
{
    addAndMakeVisible (colour_selector = new ColourSelector (ColourSelector::showSliders | ColourSelector::showColourspace));

    addAndMakeVisible (col_1 = new TextButton (String::empty));
    col_1->addListener (this);

    addAndMakeVisible (style_1 = new TextButton (String::empty));
    style_1->setButtonText (TRANS("PRE 1"));
    style_1->addListener (this);
    style_1->setColour (TextButton::buttonColourId, Colour (0xff080d0f));
    style_1->setColour (TextButton::textColourOffId, Colour (0xff9b9dc8));

    addAndMakeVisible (col_2 = new TextButton (String::empty));
    col_2->addListener (this);

    addAndMakeVisible (col_3 = new TextButton (String::empty));
    col_3->addListener (this);

    addAndMakeVisible (col_4 = new TextButton (String::empty));
    col_4->addListener (this);

    addAndMakeVisible (col_5 = new TextButton (String::empty));
    col_5->addListener (this);

    addAndMakeVisible (col_6 = new TextButton (String::empty));
    col_6->addListener (this);

    addAndMakeVisible (col_7 = new TextButton (String::empty));
    col_7->addListener (this);

    addAndMakeVisible (col_8 = new TextButton (String::empty));
    col_8->addListener (this);

    addAndMakeVisible (col_9 = new TextButton (String::empty));
    col_9->addListener (this);

    addAndMakeVisible (col_10 = new TextButton (String::empty));
    col_10->addListener (this);

    addAndMakeVisible (style_2 = new TextButton (String::empty));
    style_2->setButtonText (TRANS("PRE 1"));
    style_2->addListener (this);
    style_2->setColour (TextButton::buttonColourId, Colour (0xff080d0f));
    style_2->setColour (TextButton::textColourOffId, Colour (0xff9b9dc8));

    addAndMakeVisible (style_3 = new TextButton (String::empty));
    style_3->setButtonText (TRANS("PRE 1"));
    style_3->addListener (this);
    style_3->setColour (TextButton::buttonColourId, Colour (0xff080d0f));
    style_3->setColour (TextButton::textColourOffId, Colour (0xff9b9dc8));

    addAndMakeVisible (style_4 = new TextButton (String::empty));
    style_4->setButtonText (TRANS("PRE 1"));
    style_4->addListener (this);
    style_4->setColour (TextButton::buttonColourId, Colour (0xff080d0f));
    style_4->setColour (TextButton::textColourOffId, Colour (0xff9b9dc8));

    addAndMakeVisible (style_5 = new TextButton (String::empty));
    style_5->setButtonText (TRANS("PRE 1"));
    style_5->addListener (this);
    style_5->setColour (TextButton::buttonColourId, Colour (0xff080d0f));
    style_5->setColour (TextButton::textColourOffId, Colour (0xff9b9dc8));

    addAndMakeVisible (col_11 = new TextButton (String::empty));
    col_11->addListener (this);


    //[UserPreSize]
    //font_selector->addItemList(Font::findAllTypefaceNames (), 0);
    //[/UserPreSize]

    setSize (420, 380);


    //[Constructor] You can add your own custom stuff here..

    startTimer(150);
    //[/Constructor]
}

UiEditorColourStyler::~UiEditorColourStyler()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    colour_selector = nullptr;
    col_1 = nullptr;
    style_1 = nullptr;
    col_2 = nullptr;
    col_3 = nullptr;
    col_4 = nullptr;
    col_5 = nullptr;
    col_6 = nullptr;
    col_7 = nullptr;
    col_8 = nullptr;
    col_9 = nullptr;
    col_10 = nullptr;
    style_2 = nullptr;
    style_3 = nullptr;
    style_4 = nullptr;
    style_5 = nullptr;
    col_11 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorColourStyler::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colour (0xff161616));
    g.fillRect (0, 0, getWidth() - 0, getHeight() - 0);

    g.setColour (Colour (0xffff3b00));
    g.drawRect (0, 0, getWidth() - 0, getHeight() - 0, 2);

    g.setColour (Colour (0xffff3b00));
    g.fillRoundedRectangle (20.0f, 418.0f, static_cast<float> (proportionOfWidth (0.8644f)), 1.0f, 10.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorColourStyler::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    colour_selector->setBounds (20, 20, 250, 300);
    col_1->setBounds (280, 20, 118, 20);
    style_1->setBounds (20, 330, 50, 30);
    col_2->setBounds (280, 50, 118, 20);
    col_3->setBounds (280, 80, 118, 20);
    col_4->setBounds (280, 110, 118, 20);
    col_5->setBounds (280, 140, 118, 20);
    col_6->setBounds (280, 170, 118, 20);
    col_7->setBounds (280, 200, 118, 20);
    col_8->setBounds (280, 230, 118, 20);
    col_9->setBounds (280, 260, 118, 20);
    col_10->setBounds (280, 290, 118, 20);
    style_2->setBounds (70, 330, 50, 30);
    style_3->setBounds (120, 330, 50, 30);
    style_4->setBounds (170, 330, 50, 30);
    style_5->setBounds (220, 330, 50, 30);
    col_11->setBounds (280, 330, 118, 30);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void UiEditorColourStyler::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    stopTimer();
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == col_1)
    {
        //[UserButtonCode_col_1] -- add your button handler code here..
        editable_colour = &_colours->slider_track_colour;
        buttonThatWasClicked->setButtonText( "SL - TRCK" );
        //[/UserButtonCode_col_1]
    }
    else if (buttonThatWasClicked == style_1)
    {
        //[UserButtonCode_style_1] -- add your button handler code here..
        //[/UserButtonCode_style_1]
    }
    else if (buttonThatWasClicked == col_2)
    {
        //[UserButtonCode_col_2] -- add your button handler code here..

        editable_colour = &_colours->slider_line_colour;
        buttonThatWasClicked->setButtonText( "SL - LINE" );
        //[/UserButtonCode_col_2]
    }
    else if (buttonThatWasClicked == col_3)
    {
        //[UserButtonCode_col_3] -- add your button handler code here..
        editable_colour = &_colours->slider_text_colour;
        buttonThatWasClicked->setButtonText( "SL - TXT" );
        //[/UserButtonCode_col_3]
    }
    else if (buttonThatWasClicked == col_4)
    {
        //[UserButtonCode_col_4] -- add your button handler code here..
        editable_colour = &_colours->button_on_colour;
        buttonThatWasClicked->setButtonText( "BTN - ON" );
        //[/UserButtonCode_col_4]
    }
    else if (buttonThatWasClicked == col_5)
    {
        //[UserButtonCode_col_5] -- add your button handler code here..
        editable_colour = &_colours->button_off_colour;
        buttonThatWasClicked->setButtonText( "BTN - OFF" );
        //[/UserButtonCode_col_5]
    }
    else if (buttonThatWasClicked == col_6)
    {
        //[UserButtonCode_col_6] -- add your button handler code here..
        editable_colour = &_colours->button_text_colour;
        buttonThatWasClicked->setButtonText( "BTN - TXT" );
        //[/UserButtonCode_col_6]
    }
    else if (buttonThatWasClicked == col_7)
    {
        //[UserButtonCode_col_7] -- add your button handler code here..
        editable_colour = &_colours->slider_line_colour;
        buttonThatWasClicked->setButtonText( "SL LINE" );
        //[/UserButtonCode_col_7]
    }
    else if (buttonThatWasClicked == col_8)
    {
        //[UserButtonCode_col_8] -- add your button handler code here..
        editable_colour = &_colours->bg;
        buttonThatWasClicked->setButtonText( "BG" );
        //[/UserButtonCode_col_8]
    }
    else if (buttonThatWasClicked == col_9)
    {
        //[UserButtonCode_col_9] -- add your button handler code here..
        editable_colour = &_colours->bg_lines;
        buttonThatWasClicked->setButtonText( "BG - LINES" );
        //[/UserButtonCode_col_9]
    }
    else if (buttonThatWasClicked == col_10)
    {
        //[UserButtonCode_col_10] -- add your button handler code here..
        editable_colour = &_colours->label_text_colour;
        buttonThatWasClicked->setButtonText( "LBL - TXT" );
        //[/UserButtonCode_col_10]
    }
    else if (buttonThatWasClicked == style_2)
    {
        //[UserButtonCode_style_2] -- add your button handler code here..
        //[/UserButtonCode_style_2]
    }
    else if (buttonThatWasClicked == style_3)
    {
        //[UserButtonCode_style_3] -- add your button handler code here..
        //[/UserButtonCode_style_3]
    }
    else if (buttonThatWasClicked == style_4)
    {
        //[UserButtonCode_style_4] -- add your button handler code here..
        //[/UserButtonCode_style_4]
    }
    else if (buttonThatWasClicked == style_5)
    {
        //[UserButtonCode_style_5] -- add your button handler code here..
        //[/UserButtonCode_style_5]
    }
    else if (buttonThatWasClicked == col_11)
    {
        //[UserButtonCode_col_11] -- add your button handler code here..
        //[/UserButtonCode_col_11]
    }

    //[UserbuttonClicked_Post]
    colour_selector->setCurrentColour( *editable_colour );
    startTimer(150);
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

<JUCER_COMPONENT documentType="Component" className="UiEditorColourStyler" componentName=""
                 parentClasses="public Component, public Timer" constructorParams="ComponentColours* const colours_"
                 variableInitialisers="_colours( colours_ ),editable_colour( &colours_->slider_text_colour )&#10;"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="420" initialHeight="380">
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="0 0 0M 0M" fill="solid: ff161616" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="20 418 86.441% 1" cornerSize="10" fill="solid: ffff3b00"
               hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="f80f96857b452ce6" memberName="colour_selector" virtualName="ColourSelector"
                    explicitFocusOrder="0" pos="20 20 250 300" class="ColourSelector"
                    params="ColourSelector::showSliders | ColourSelector::showColourspace"/>
  <TEXTBUTTON name="" id="b2a743289dc6b133" memberName="col_1" virtualName=""
              explicitFocusOrder="0" pos="280 20 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="96cf75141d58b8ed" memberName="style_1" virtualName=""
              explicitFocusOrder="0" pos="20 330 50 30" bgColOff="ff080d0f"
              textColOn="ff9b9dc8" buttonText="PRE 1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="1c45518db2fd986a" memberName="col_2" virtualName=""
              explicitFocusOrder="0" pos="280 50 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b5d2d92344886337" memberName="col_3" virtualName=""
              explicitFocusOrder="0" pos="280 80 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="53a645eaa0314068" memberName="col_4" virtualName=""
              explicitFocusOrder="0" pos="280 110 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b35ed75a261b4e79" memberName="col_5" virtualName=""
              explicitFocusOrder="0" pos="280 140 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="bd41ac14250fbffa" memberName="col_6" virtualName=""
              explicitFocusOrder="0" pos="280 170 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="43f033626dc244a" memberName="col_7" virtualName=""
              explicitFocusOrder="0" pos="280 200 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="7ff9bef09d45f8a5" memberName="col_8" virtualName=""
              explicitFocusOrder="0" pos="280 230 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="ee2bfc5789b7c561" memberName="col_9" virtualName=""
              explicitFocusOrder="0" pos="280 260 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="431cd479814c4b74" memberName="col_10" virtualName=""
              explicitFocusOrder="0" pos="280 290 118 20" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="4f889ab146208639" memberName="style_2" virtualName=""
              explicitFocusOrder="0" pos="70 330 50 30" bgColOff="ff080d0f"
              textColOn="ff9b9dc8" buttonText="PRE 1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="b69e2c3bce1d8573" memberName="style_3" virtualName=""
              explicitFocusOrder="0" pos="120 330 50 30" bgColOff="ff080d0f"
              textColOn="ff9b9dc8" buttonText="PRE 1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="bdc9250af2f76391" memberName="style_4" virtualName=""
              explicitFocusOrder="0" pos="170 330 50 30" bgColOff="ff080d0f"
              textColOn="ff9b9dc8" buttonText="PRE 1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="c32a0dd8af2edc33" memberName="style_5" virtualName=""
              explicitFocusOrder="0" pos="220 330 50 30" bgColOff="ff080d0f"
              textColOn="ff9b9dc8" buttonText="PRE 1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="ec1ad77ffcb746ed" memberName="col_11" virtualName=""
              explicitFocusOrder="0" pos="280 330 118 30" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
