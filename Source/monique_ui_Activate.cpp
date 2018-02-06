/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "monique_core_Datastructures.h"
#include "monique_ui_MainWindow.h"
//[/Headers]

#include "monique_ui_Activate.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
monique_ui_Activate::monique_ui_Activate (Monique_Ui_Mainwindow*parent_)
    : DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point<int>(10,10)))
{
    //[Constructor_pre] You can add your own custom stuff here..
    activation_look_and_feel = new LookAndFeel_V3();
    parent = parent_;
    //[/Constructor_pre]

    addAndMakeVisible (label_header = new Label (String(),
                                                 TRANS("Monique Demo")));
    label_header->setFont (Font (Font::getDefaultSansSerifFontName(), 40.00f, Font::plain));
    label_header->setJustificationType (Justification::centred);
    label_header->setEditable (false, false, false);
    label_header->setColour (Label::textColourId, Colour (0xe3ffb500));
    label_header->setColour (TextEditor::textColourId, Colours::black);
    label_header->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_buy = new TextButton ("new button"));
    button_buy->setButtonText (TRANS("Buy Monique Online"));
    button_buy->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_buy->addListener (this);
    button_buy->setColour (TextButton::buttonColourId, Colour (0xc0ffb500));

    addAndMakeVisible (button_evaluate = new TextButton ("new button"));
    button_evaluate->setButtonText (TRANS("Test Monique (free)"));
    button_evaluate->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_evaluate->addListener (this);
    button_evaluate->setColour (TextButton::buttonColourId, Colour (0xff240a6a));
    button_evaluate->setColour (TextButton::textColourOffId, Colours::white);
    button_evaluate->setColour (TextButton::textColourOnId, Colours::aqua);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Demo limits: periodical returning noise, no skinning")));
    label2->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_header2 = new Label (String(),
                                                  TRANS("Need Help?")));
    label_header2->setFont (Font (Font::getDefaultSansSerifFontName(), 25.00f, Font::plain));
    label_header2->setJustificationType (Justification::centred);
    label_header2->setEditable (false, false, false);
    label_header2->setColour (Label::textColourId, Colour (0xe3ffb500));
    label_header2->setColour (TextEditor::textColourId, Colours::black);
    label_header2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sub_header2 = new Label (String(),
                                                      TRANS("Check out the Monoplugs Magazine for the latest video tutorials and news.")));
    label_sub_header2->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header2->setJustificationType (Justification::centred);
    label_sub_header2->setEditable (false, false, false);
    label_sub_header2->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label_sub_header2->setColour (TextEditor::textColourId, Colours::black);
    label_sub_header2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sub_header3 = new Label (String(),
                                                      TRANS(">> http://monoplugs.com/magazine")));
    label_sub_header3->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header3->setJustificationType (Justification::centred);
    label_sub_header3->setEditable (false, false, false);
    label_sub_header3->setColour (Label::textColourId, Colour (0xe000b5bc));
    label_sub_header3->setColour (TextEditor::textColourId, Colour (0xe900b5bc));
    label_sub_header3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (hyperlinkButton = new HyperlinkButton (String(),
                                                              URL ("http://monoplugs.com/magazine")));
    hyperlinkButton->setTooltip (TRANS("http://monoplugs.com/magazine"));


    //[UserPreSize]
    //setOpaque (true);

    Colour found_colour = label_sub_header2->findColour( Label::textColourId );
    Font style_font = label_sub_header2->getFont();

    label_header->setLookAndFeel(activation_look_and_feel);
    label2->setLookAndFeel(activation_look_and_feel);
    label_header2->setLookAndFeel(activation_look_and_feel);
    label_sub_header2->setLookAndFeel(activation_look_and_feel);
    label_sub_header3->setLookAndFeel(activation_look_and_feel);
    hyperlinkButton->setTooltip("");

    label_header->setFont(style_font.withHeight(label_header->getFont().getHeight()));
    label2->setFont(style_font.withHeight(label2->getFont().getHeight()));

    button_buy->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_evaluate->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );

    //offline_text->setEnabled(false);

    //[/UserPreSize]

    setSize (540, 330);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

monique_ui_Activate::~monique_ui_Activate()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_header = nullptr;
    button_buy = nullptr;
    button_evaluate = nullptr;
    label2 = nullptr;
    label_header2 = nullptr;
    label_sub_header2 = nullptr;
    label_sub_header3 = nullptr;
    hyperlinkButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void monique_ui_Activate::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xff000a12));
    g.fillRoundedRectangle (3.0f, 3.0f, static_cast<float> (getWidth() - 6), static_cast<float> (getHeight() - 6), 1.000f);

    g.setColour (Colours::aqua);
    g.drawRoundedRectangle (3.0f, 3.0f, static_cast<float> (getWidth() - 6), static_cast<float> (getHeight() - 6), 1.000f, 1.000f);

    g.setColour (Colour (0x1bcbcbcb));
    g.fillRect (30, 100 - (2 / 2), 480, 2);

    g.setColour (Colour (0x1bcbcbcb));
    g.fillRect (30, 210 - (2 / 2), 480, 2);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void monique_ui_Activate::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_header->setBounds (30, 10, 480, 60);
    button_buy->setBounds (100, 120, 340, 30);
    button_evaluate->setBounds (100, 160, 340, 30);
    label2->setBounds (30, 60, 480, 25);
    label_header2->setBounds (30, 220, 480, 40);
    label_sub_header2->setBounds (30, 260, 480, 30);
    label_sub_header3->setBounds (30, 280, 480, 30);
    hyperlinkButton->setBounds (30, 280, 480, 30);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void monique_ui_Activate::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_buy)
    {
        //[UserButtonCode_button_buy] -- add your button handler code here..
        URL url("https://sites.fastspring.com/monoplugs/instant/monique");
        url.launchInDefaultBrowser();
        //[/UserButtonCode_button_buy]
    }
    else if (buttonThatWasClicked == button_evaluate)
    {
        //[UserButtonCode_button_evaluate] -- add your button handler code here..
        parent->activation_windows_is_confirmed = true;
        close();
        //[/UserButtonCode_button_evaluate]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void monique_ui_Activate::close() noexcept
{
    parent->activation_window = nullptr;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="monique_ui_Activate" componentName=""
                 parentClasses="public Component, public DropShadower, public FileDragAndDropTarget"
                 constructorParams="Monique_Ui_Mainwindow*parent_" variableInitialisers="DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10)))"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="540" initialHeight="330">
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="3 3 6M 6M" cornerSize="1" fill="solid: ff000a12" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff00ffff"/>
    <RECT pos="30 100c 480 2" fill="solid: 1bcbcbcb" hasStroke="0"/>
    <RECT pos="30 210c 480 2" fill="solid: 1bcbcbcb" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="213ce816bd0c8d7e" memberName="label_header" virtualName=""
         explicitFocusOrder="0" pos="30 10 480 60" textCol="e3ffb500"
         edTextCol="ff000000" edBkgCol="0" labelText="Monique Demo" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="40" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="e594b0617fd9b5ad" memberName="button_buy"
              virtualName="" explicitFocusOrder="0" pos="100 120 340 30" bgColOff="c0ffb500"
              buttonText="Buy Monique Online" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5ac0f6c8ceef7e31" memberName="button_evaluate"
              virtualName="" explicitFocusOrder="0" pos="100 160 340 30" bgColOff="ff240a6a"
              textCol="ffffffff" textColOn="ff00ffff" buttonText="Test Monique (free)"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="8cc8bfeb95ae386b" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="30 60 480 25" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Demo limits: periodical returning noise, no skinning"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="c3bc7766a1daf17c" memberName="label_header2" virtualName=""
         explicitFocusOrder="0" pos="30 220 480 40" textCol="e3ffb500"
         edTextCol="ff000000" edBkgCol="0" labelText="Need Help?" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="25" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="bf2a6a1ce787bee6" memberName="label_sub_header2"
         virtualName="" explicitFocusOrder="0" pos="30 260 480 30" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Check out the Monoplugs Magazine for the latest video tutorials and news."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="19160f6a88a37f0f" memberName="label_sub_header3"
         virtualName="" explicitFocusOrder="0" pos="30 280 480 30" textCol="e000b5bc"
         edTextCol="e900b5bc" edBkgCol="0" labelText="&gt;&gt; http://monoplugs.com/magazine"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <HYPERLINKBUTTON name="" id="9b5f8b1bc06606d4" memberName="hyperlinkButton" virtualName=""
                   explicitFocusOrder="0" pos="30 280 480 30" tooltip="http://monoplugs.com/magazine"
                   buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                   url="http://monoplugs.com/magazine"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
