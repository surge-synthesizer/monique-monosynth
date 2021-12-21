/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

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

#include "monique_ui_OptionPopup.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_OptionPopup::refresh() noexcept
{
    SectionTheme &theme = look_and_feel->colours.get_theme(COLOUR_THEMES::POPUP_THEME);
    Colour button_off = theme.button_off_colour;
    Colour button_on = theme.button_on_colour;

    TURN_BUTTON_ON_OR_OFF(button_option_a, param_a->get_value());
    TURN_BUTTON_ON_OR_OFF(button_option_b, param_b->get_value());
}

void Monique_Ui_OptionPopup::set_element_to_show(Component *const comp_)
{
    related_to_comp = comp_;
    int x = get_editor()->getLocalPoint(comp_, Point<int>(0, 0)).getX();
    int y = get_editor()->getLocalPoint(comp_, Point<int>(0, 0)).getY();
    setTopLeftPosition(x + comp_->getWidth(), y - getHeight() / 2 + comp_->getHeight() / 2);
}
void Monique_Ui_OptionPopup::update_positions()
{
    if (related_to_comp)
    {
        set_element_to_show(related_to_comp);
    }
}
void Monique_Ui_OptionPopup::set_infos(StringRef text_a, StringRef text_b, StringRef tool_a,
                                       StringRef tool_b)
{
    button_option_a->setButtonText(text_a.text);
    button_option_b->setButtonText(text_b.text);
    button_option_a->setTooltip(tool_a.text);
    button_option_b->setTooltip(tool_b.text);
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_OptionPopup::Monique_Ui_OptionPopup(Monique_Ui_Refresher *ui_refresher_,
                                               Monique_Ui_Mainwindow *const parent_,
                                               BoolParameter *param_a_, BoolParameter *param_b_)
    : Monique_Ui_Refreshable(ui_refresher_),
      DropShadower(DropShadow(Colours::black.withAlpha(0.8f), 10, Point<int>(10, 10))),
      parent(parent_), param_a(param_a_), param_b(param_b_), original_w(140), original_h(90)
{
    //[Constructor_pre] You can add your own custom stuff here..
    setOwner(this);
    //[/Constructor_pre]

    addAndMakeVisible(button_option_a = new TextButton(String()));
    button_option_a->setButtonText(TRANS("x"));
    button_option_a->addListener(this);
    button_option_a->setColour(TextButton::buttonColourId, Colours::black);
    button_option_a->setColour(TextButton::textColourOnId, Colour(0xffff3b00));
    button_option_a->setColour(TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible(button_option_b = new TextButton(String()));
    button_option_b->setButtonText(TRANS("x"));
    button_option_b->addListener(this);
    button_option_b->setColour(TextButton::buttonColourId, Colours::black);
    button_option_b->setColour(TextButton::textColourOnId, Colour(0xffff3b00));
    button_option_b->setColour(TextButton::textColourOffId, Colours::yellow);

    //[UserPreSize]
    related_to_comp = nullptr;
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME);
    }
    /*
    //[/UserPreSize]

    setSize (140, 90);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_OptionPopup::~Monique_Ui_OptionPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    button_option_a = nullptr;
    button_option_b = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_OptionPopup::paint(Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect(getWidth() - 10, getHeight() - 10, 10, 10);
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.setColour(look_and_feel->colours.get_theme(COLOUR_THEMES::ARP_THEME).area_colour);
    g.fillRoundedRectangle(10.0f, 0.0f, 129.0f, 89.0f, 10.000f);

    g.setColour(
        look_and_feel->colours.get_theme(COLOUR_THEMES::ARP_THEME).value_slider_track_colour);
    g.drawRoundedRectangle(10.0f, 0.0f, 129.0f, 89.0f, 10.000f, 1.000f);

    g.fillPath(internalPath1);

    /*
    //[/UserPrePaint]

    g.setColour (Colour (0xbaffffff));
    g.fillRoundedRectangle (10.0f, 0.0f, 129.0f, 89.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (10.0f, 0.0f, 129.0f, 89.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    */
    //[/UserPaint]
}

void Monique_Ui_OptionPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    button_option_a->setBounds(20, 10, 110, 30);
    button_option_b->setBounds(20, 50, 110, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath(0.0f, 45.0f);
    internalPath1.lineTo(10.0f, 35.0f);
    internalPath1.lineTo(10.0f, 55.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_OptionPopup::buttonClicked(Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_option_a)
    {
        //[UserButtonCode_button_option_a] -- add your button handler code here..
        *param_a ^= true;
        param_b->set_value(false);
        //[/UserButtonCode_button_option_a]
    }
    else if (buttonThatWasClicked == button_option_b)
    {
        //[UserButtonCode_button_option_b] -- add your button handler code here..
        *param_b ^= true;
        param_a->set_value(false);
        //[/UserButtonCode_button_option_b]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Monique_Ui_OptionPopup::mouseExit(const MouseEvent &event)
{
    parent->open_option_popup(nullptr, nullptr, nullptr, nullptr, "", "", "", "");
}
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_OptionPopup" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable, public DropShadower"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, BoolParameter* param_a_, BoolParameter* param_b_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10))),&#10;parent(parent_),&#10;param_a(param_a_),&#10;param_b(param_b_),&#10;original_w(140), original_h(90)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="140" initialHeight="90">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="10 0 129 89" cornerSize="10" fill="solid: baffffff" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 0 45 l 10 35 l 10 55 x</PATH>
  </BACKGROUND>
  <TEXTBUTTON name="" id="4d29473e06fd562f" memberName="button_option_a" virtualName=""
              explicitFocusOrder="0" pos="20 10 110 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="x" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fd107f58e5bf603c" memberName="button_option_b" virtualName=""
              explicitFocusOrder="0" pos="20 50 110 30" bgColOff="ff000000"
              textCol="ffff3b00" textColOn="ffffff00" buttonText="x" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
