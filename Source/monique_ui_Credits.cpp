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
//[/Headers]

#include "monique_ui_Credits.h"
#include "version.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
monique_ui_Credits::monique_ui_Credits(Monique_Ui_Refresher *ui_refresher_)
    : Monique_Ui_Refreshable(ui_refresher_), original_w(540), original_h(600)
{
    //[Constructor_pre] You can add your own custom stuff here..
    activation_look_and_feel = std::make_unique<LookAndFeel_V3>();
    //[/Constructor_pre]

    label_sub_header9 =
        std::make_unique<Label>(String(), TRANS("Composer and Sounddesigner (www.albaecstasy.ro)"));
    addAndMakeVisible(*label_sub_header9);
    label_sub_header9->setFont(Font(Font::getDefaultSansSerifFontName(), 14.00f, Font::plain));
    label_sub_header9->setJustificationType(Justification::centred);
    label_sub_header9->setEditable(false, false, false);
    label_sub_header9->setColour(Label::textColourId, Colour(0xffcbcbcb));
    label_sub_header9->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header9->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header2 = std::make_unique<Label>("name", TRANS("Adrian Simion "));
    addAndMakeVisible(*label_sub_header2);
    label_sub_header2->setFont(Font(Font::getDefaultSansSerifFontName(), 17.00f, Font::plain));
    label_sub_header2->setJustificationType(Justification::centred);
    label_sub_header2->setEditable(false, false, false);
    label_sub_header2->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header2->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header2->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header3 = std::make_unique<Label>("name", TRANS("Alex Fellows"));
    addAndMakeVisible(*label_sub_header3);
    label_sub_header3->setFont(Font(Font::getDefaultSansSerifFontName(), 17.00f, Font::plain));
    label_sub_header3->setJustificationType(Justification::centred);
    label_sub_header3->setEditable(false, false, false);
    label_sub_header3->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header3->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header3->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header4 = std::make_unique<Label>("name", CharPointer_UTF8("Andreas Gr\xc3\xa4sser"));
    addAndMakeVisible(*label_sub_header4);
    label_sub_header4->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header4->setJustificationType(Justification::centred);
    label_sub_header4->setEditable(false, false, false);
    label_sub_header4->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header4->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header4->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header5 = std::make_unique<Label>("name", TRANS("Anne Dietrich"));
    addAndMakeVisible(*label_sub_header5);
    label_sub_header5->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header5->setJustificationType(Justification::centred);
    label_sub_header5->setEditable(false, false, false);
    label_sub_header5->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header5->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header5->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header6 = std::make_unique<Label>("name", TRANS("Matthias Culmey"));
    addAndMakeVisible(*label_sub_header6);
    label_sub_header6->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header6->setJustificationType(Justification::centred);
    label_sub_header6->setEditable(false, false, false);
    label_sub_header6->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header6->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header6->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header7 = std::make_unique<Label>("name", TRANS("Tim Charlemagne"));
    addAndMakeVisible(*label_sub_header7);
    label_sub_header7->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header7->setJustificationType(Justification::centred);
    label_sub_header7->setEditable(false, false, false);
    label_sub_header7->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header7->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header7->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header10 = std::make_unique<Label>(String(), TRANS("Musician"));
    addAndMakeVisible(*label_sub_header10);
    label_sub_header10->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header10->setJustificationType(Justification::centred);
    label_sub_header10->setEditable(false, false, false);
    label_sub_header10->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header10->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header10->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header11 = std::make_unique<Label>(
        String(), TRANS("Musician and Creator of G-Stomper Studio (www.planet-h.com)"));
    addAndMakeVisible(*label_sub_header11);
    label_sub_header11->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header11->setJustificationType(Justification::centred);
    label_sub_header11->setEditable(false, false, false);
    label_sub_header11->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header11->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header11->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header12 = std::make_unique<Label>(
        String(), TRANS("Musician, Dancer and Choreograph (www.mohiniyattam.de)"));
    addAndMakeVisible(*label_sub_header12);
    label_sub_header12->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header12->setJustificationType(Justification::centred);
    label_sub_header12->setEditable(false, false, false);
    label_sub_header12->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header12->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header12->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header13 =
        std::make_unique<Label>(String(), TRANS("Musician and Audio Engineer (www.waveframe.de)"));
    addAndMakeVisible(*label_sub_header13);
    label_sub_header13->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header13->setJustificationType(Justification::centred);
    label_sub_header13->setEditable(false, false, false);
    label_sub_header13->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header13->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header13->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header8 = std::make_unique<Label>(
        String(), TRANS("Musician & Audio Engineer, UI design (www.soundmanufacture.net)"));
    addAndMakeVisible(*label_sub_header8);
    label_sub_header8->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header8->setJustificationType(Justification::centred);
    label_sub_header8->setEditable(false, false, false);
    label_sub_header8->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header8->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header8->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header16 = std::make_unique<Label>("name", TRANS("Leon van Bokhorst"));
    addAndMakeVisible(*label_sub_header16);
    label_sub_header16->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header16->setJustificationType(Justification::centred);
    label_sub_header16->setEditable(false, false, false);
    label_sub_header16->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header16->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header16->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header17 = std::make_unique<Label>(String(), TRANS("Musician and Sounddesigner"));
    addAndMakeVisible(*label_sub_header17);
    label_sub_header17->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header17->setJustificationType(Justification::centred);
    label_sub_header17->setEditable(false, false, false);
    label_sub_header17->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header17->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header17->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header14 = std::make_unique<Label>(String(), TRANS("Thomas Arndt"));
    addAndMakeVisible(*label_sub_header14);
    label_sub_header14->setFont(Font(Font::getDefaultSansSerifFontName(), 18.00f, Font::plain));
    label_sub_header14->setJustificationType(Justification::centred);
    label_sub_header14->setEditable(false, false, false);
    label_sub_header14->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header14->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header14->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header15 = std::make_unique<Label>("name", TRANS("SPECIAL THANKS TO"));
    addAndMakeVisible(*label_sub_header15);
    label_sub_header15->setFont(Font(Font::getDefaultSansSerifFontName(), 25.40f, Font::bold));
    label_sub_header15->setJustificationType(Justification::centred);
    label_sub_header15->setEditable(false, false, false);
    label_sub_header15->setColour(Label::textColourId, Colour(0xffffb500));
    label_sub_header15->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header15->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header18 = std::make_unique<Label>("name", TRANS("Many many thanks to my Friends"));
    addAndMakeVisible(*label_sub_header18);
    label_sub_header18->setFont(Font(Font::getDefaultSansSerifFontName(), 18.00f, Font::bold));
    label_sub_header18->setJustificationType(Justification::centred);
    label_sub_header18->setEditable(false, false, false);
    label_sub_header18->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header18->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header18->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header19 = std::make_unique<Label>("name", TRANS("Reg Griffiths"));
    addAndMakeVisible(*label_sub_header19);
    label_sub_header19->setFont(Font(Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label_sub_header19->setJustificationType(Justification::centred);
    label_sub_header19->setEditable(false, false, false);
    label_sub_header19->setColour(Label::textColourId, Colour(0xfff47300));
    label_sub_header19->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header19->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label_sub_header20 = std::make_unique<Label>(String(), TRANS("Musician"));
    addAndMakeVisible(*label_sub_header20);
    label_sub_header20->setFont(Font(Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header20->setJustificationType(Justification::centred);
    label_sub_header20->setEditable(false, false, false);
    label_sub_header20->setColour(Label::textColourId, Colours::burlywood);
    label_sub_header20->setColour(TextEditor::textColourId, Colours::black);
    label_sub_header20->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    link_to_monoplugs = std::make_unique<HyperlinkButton>(
        String(), URL("http://monique-synthesizer.monoplugs.com"));
    addAndMakeVisible(*link_to_monoplugs);
    link_to_monoplugs->setTooltip(TRANS("http://monique-synthesizer.monoplugs.com"));

    auto drawable1 = Drawable::createFromImageData(monoplugs_png, monoplugs_pngSize);

    //[UserPreSize]
    // this->setLookAndFeel( activation_look_and_feel );

    Colour yellow = label_sub_header15->findColour(Label::textColourId);
    Colour grey = label_sub_header9->findColour(Label::textColourId);
    Font style_font = label_sub_header15->getFont();
    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        Component *child = getChildComponent(i);
        child->setLookAndFeel(activation_look_and_feel.get());
        child->setInterceptsMouseClicks(false, false);

        if (Label *label = dynamic_cast<Label *>(child))
        {
            label->setLookAndFeel(activation_look_and_feel.get());
            if (label->getName() == "name")
            {
                label->setFont(style_font.withHeight(19));
                label->setColour(Label::textColourId, yellow);
            }
            else
            {
                label->setFont(style_font.withHeight(15));
                label->setColour(Label::textColourId, grey);
            }
        }
    }
    label_sub_header15->setFont(style_font.withHeight(25));
    label_sub_header14->setFont(style_font.withHeight(19));
    label_sub_header14->setColour(Label::textColourId, Colours::white);
    label_sub_header18->setFont(style_font.withHeight(16));
    // label_header->setFont(style_font.withHeight(label_header->getFont().getHeight()));
    //[/UserPreSize]

    setSize(540, 600);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

monique_ui_Credits::~monique_ui_Credits()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_sub_header9 = nullptr;
    label_sub_header2 = nullptr;
    label_sub_header3 = nullptr;
    label_sub_header4 = nullptr;
    label_sub_header5 = nullptr;
    label_sub_header6 = nullptr;
    label_sub_header7 = nullptr;
    label_sub_header10 = nullptr;
    label_sub_header11 = nullptr;
    label_sub_header12 = nullptr;
    label_sub_header13 = nullptr;
    label_sub_header8 = nullptr;
    label_sub_header16 = nullptr;
    label_sub_header17 = nullptr;
    label_sub_header14 = nullptr;
    label_sub_header15 = nullptr;
    label_sub_header18 = nullptr;
    label_sub_header19 = nullptr;
    label_sub_header20 = nullptr;
    link_to_monoplugs = nullptr;
    drawable1 = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void monique_ui_Credits::paint(Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour(Colour(0xff000a12));
    g.fillRoundedRectangle(3.0f, 3.0f, static_cast<float>(getWidth() - 6),
                           static_cast<float>(getHeight() - 6), 1.000f);

    g.setColour(Colours::aqua);
    g.drawRoundedRectangle(3.0f, 3.0f, static_cast<float>(getWidth() - 6),
                           static_cast<float>(getHeight() - 6), 1.000f, 1.000f);

    g.setColour(Colours::black);
    jassert(drawable1 != 0);
    if (drawable1 != 0)
        drawable1->drawWithin(g,
                              Rectangle<float>(-3, proportionOfHeight(0.8933f),
                                               proportionOfWidth(1.0000f),
                                               proportionOfHeight(0.0433f)),
                              RectanglePlacement::centred, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    auto r = getLocalBounds().withTrimmedTop(getLocalBounds().getHeight() - 14).translated(0, -5);
    g.setColour(juce::Colours::lightgrey);
    g.setFont(12);
    g.drawText(std::string("Version: ") + Monique::Build::FullVersionStr +
                   " Built: " + Monique::Build::BuildDate + " " + Monique::Build::BuildTime,
               r, juce::Justification::centred);
    //[/UserPaint]
}

void monique_ui_Credits::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    label_sub_header9->setBounds(0, 80, 540, 30);
    label_sub_header2->setBounds(0, 60, 540, 30);
    label_sub_header3->setBounds(0, 110, 540, 30);
    label_sub_header4->setBounds(0, 160, 540, 30);
    label_sub_header5->setBounds(0, 210, 540, 30);
    label_sub_header6->setBounds(0, 310, 540, 30);
    label_sub_header7->setBounds(0, 410, 540, 30);
    label_sub_header10->setBounds(0, 130, 540, 30);
    label_sub_header11->setBounds(0, 180, 540, 30);
    label_sub_header12->setBounds(0, 230, 540, 30);
    label_sub_header13->setBounds(0, 330, 540, 30);
    label_sub_header8->setBounds(0, 430, 540, 30);
    label_sub_header16->setBounds(0, 260, 540, 30);
    label_sub_header17->setBounds(0, 280, 540, 30);
    label_sub_header14->setBounds(0, 505, 540, 30);
    label_sub_header15->setBounds(0, 20, 540, 30);
    label_sub_header18->setBounds(0, 470, 540, 30);
    label_sub_header19->setBounds(0, 360, 540, 30);
    label_sub_header20->setBounds(0, 380, 540, 30);
    link_to_monoplugs->setBounds(20, 360, 500, 120);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="monique_ui_Credits" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_" variableInitialisers="Monique_Ui_Refreshable(ui_refresher_), original_w(540), original_h(600)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="540" initialHeight="600">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="3 3 6M 6M" cornerSize="1" fill="solid: ff000a12" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff00ffff"/>
    <IMAGE pos="-3 89.333% 100% 4.333%" resource="monoplugs_png" opacity="1"
           mode="1"/>
  </BACKGROUND>
  <LABEL name="" id="1e95c352144fd920" memberName="label_sub_header9"
         virtualName="" explicitFocusOrder="0" pos="0 80 540 30" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Composer and Sounddesigner (www.albaecstasy.ro)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="14" bold="0" italic="0"
         justification="36"/>
  <LABEL name="name" id="6f7c4328b7baa239" memberName="label_sub_header2"
         virtualName="" explicitFocusOrder="0" pos="0 60 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Adrian Simion " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="17" bold="0" italic="0" justification="36"/>
  <LABEL name="name" id="690bde5aa81cd2f4" memberName="label_sub_header3"
         virtualName="" explicitFocusOrder="0" pos="0 110 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Alex Fellows" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="17" bold="0" italic="0" justification="36"/>
  <LABEL name="name" id="dd16070df4e1955a" memberName="label_sub_header4"
         virtualName="" explicitFocusOrder="0" pos="0 160 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Andreas Gr&#228;sser"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="20" bold="0" italic="0"
         justification="36"/>
  <LABEL name="name" id="b59fd8ae1cc006d4" memberName="label_sub_header5"
         virtualName="" explicitFocusOrder="0" pos="0 210 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Anne Dietrich" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="20" bold="0" italic="0" justification="36"/>
  <LABEL name="name" id="29390fe9c449f361" memberName="label_sub_header6"
         virtualName="" explicitFocusOrder="0" pos="0 310 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Matthias Culmey"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="20" bold="0" italic="0"
         justification="36"/>
  <LABEL name="name" id="e4c2d939e59ac295" memberName="label_sub_header7"
         virtualName="" explicitFocusOrder="0" pos="0 410 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Tim Charlemagne"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="20" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="8df510ddb6d6d23f" memberName="label_sub_header10"
         virtualName="" explicitFocusOrder="0" pos="0 130 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="82deafe8566ce562" memberName="label_sub_header11"
         virtualName="" explicitFocusOrder="0" pos="0 180 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician and Creator of G-Stomper Studio (www.planet-h.com)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="5f823a70bd882012" memberName="label_sub_header12"
         virtualName="" explicitFocusOrder="0" pos="0 230 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician, Dancer and Choreograph (www.mohiniyattam.de)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="645a8bfca991c6cc" memberName="label_sub_header13"
         virtualName="" explicitFocusOrder="0" pos="0 330 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician and Audio Engineer (www.waveframe.de)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="5e5a346c46c9c619" memberName="label_sub_header8"
         virtualName="" explicitFocusOrder="0" pos="0 430 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician &amp; Audio Engineer, UI design (www.soundmanufacture.net)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="name" id="3831ac4707e65586" memberName="label_sub_header16"
         virtualName="" explicitFocusOrder="0" pos="0 260 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Leon van Bokhorst"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="20" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="7fd44b4cf645b80d" memberName="label_sub_header17"
         virtualName="" explicitFocusOrder="0" pos="0 280 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician and Sounddesigner"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="3087c77d9d3dd8c8" memberName="label_sub_header14"
         virtualName="" explicitFocusOrder="0" pos="0 505 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Thomas Arndt" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="18" bold="0" italic="0" justification="36"/>
  <LABEL name="name" id="88be6c8701051572" memberName="label_sub_header15"
         virtualName="" explicitFocusOrder="0" pos="0 20 540 30" textCol="ffffb500"
         edTextCol="ff000000" edBkgCol="0" labelText="SPECIAL THANKS TO"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="25.400000000000002132"
         bold="1" italic="0" justification="36"/>
  <LABEL name="name" id="27b96b46598cdd7" memberName="label_sub_header18"
         virtualName="" explicitFocusOrder="0" pos="0 470 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Many many thanks to my Friends"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="18" bold="1" italic="0"
         justification="36"/>
  <LABEL name="name" id="b1682a94c14907f2" memberName="label_sub_header19"
         virtualName="" explicitFocusOrder="0" pos="0 360 540 30" textCol="fff47300"
         edTextCol="ff000000" edBkgCol="0" labelText="Reg Griffiths" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="20" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="719fc612d42156e6" memberName="label_sub_header20"
         virtualName="" explicitFocusOrder="0" pos="0 380 540 30" textCol="ffdeb887"
         edTextCol="ff000000" edBkgCol="0" labelText="Musician" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <HYPERLINKBUTTON name="" id="c0eeed7d74bb8534" memberName="link_to_monoplugs"
                   virtualName="" explicitFocusOrder="0" pos="20 360 500 120" tooltip="http://monique-synthesizer.monoplugs.com"
                   buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                   url="http://monique-synthesizer.monoplugs.com"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: monoplugs_png, 18152, "../Images/Logo_MonoPlugs_1.svg"
static const unsigned char resource_monique_ui_Credits_monoplugs_png[] = {
    60,  63,  120, 109, 108, 32,  118, 101, 114, 115, 105, 111, 110, 61,  34,  49,  46,  48,  34,
    32,  101, 110, 99,  111, 100, 105, 110, 103, 61,  34,  85,  84,  70,  45,  56,  34,  32,  115,
    116, 97,  110, 100, 97,  108, 111, 110, 101, 61,  34,  110, 111, 34,  63,  62,  10,  60,  115,
    118, 103, 10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  100, 99,  61,  34,  104, 116, 116,
    112, 58,  47,  47,  112, 117, 114, 108, 46,  111, 114, 103, 47,  100, 99,  47,  101, 108, 101,
    109, 101, 110, 116, 115, 47,  49,  46,  49,  47,  34,  10,  32,  32,  32,  120, 109, 108, 110,
    115, 58,  99,  99,  61,  34,  104, 116, 116, 112, 58,  47,  47,  99,  114, 101, 97,  116, 105,
    118, 101, 99,  111, 109, 109, 111, 110, 115, 46,  111, 114, 103, 47,  110, 115, 35,  34,  10,
    32,  32,  32,  120, 109, 108, 110, 115, 58,  114, 100, 102, 61,  34,  104, 116, 116, 112, 58,
    47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  49,  57,  57,  57,  47,  48,
    50,  47,  50,  50,  45,  114, 100, 102, 45,  115, 121, 110, 116, 97,  120, 45,  110, 115, 35,
    34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  115, 118, 103, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  115, 111, 100, 105,
    112, 111, 100, 105, 61,  34,  104, 116, 116, 112, 58,  47,  47,  115, 111, 100, 105, 112, 111,
    100, 105, 46,  115, 111, 117, 114, 99,  101, 102, 111, 114, 103, 101, 46,  110, 101, 116, 47,
    68,  84,  68,  47,  115, 111, 100, 105, 112, 111, 100, 105, 45,  48,  46,  100, 116, 100, 34,
    10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  105, 110, 107, 115, 99,  97,  112, 101, 61,
    34,  104, 116, 116, 112, 58,  47,  47,  119, 119, 119, 46,  105, 110, 107, 115, 99,  97,  112,
    101, 46,  111, 114, 103, 47,  110, 97,  109, 101, 115, 112, 97,  99,  101, 115, 47,  105, 110,
    107, 115, 99,  97,  112, 101, 34,  10,  32,  32,  32,  118, 101, 114, 115, 105, 111, 110, 61,
    34,  49,  46,  49,  34,  10,  32,  32,  32,  105, 100, 61,  34,  115, 118, 103, 50,  34,  10,
    32,  32,  32,  118, 105, 101, 119, 66,  111, 120, 61,  34,  48,  32,  48,  32,  55,  52,  52,
    46,  48,  57,  52,  52,  56,  56,  49,  57,  32,  49,  48,  53,  50,  46,  51,  54,  50,  50,
    48,  52,  55,  34,  10,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  50,  57,  55,
    109, 109, 34,  10,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  50,  49,  48,  109, 109,
    34,  10,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  118, 101, 114, 115, 105,
    111, 110, 61,  34,  48,  46,  52,  56,  46,  52,  32,  114, 57,  57,  51,  57,  34,  10,  32,
    32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  100, 111, 99,  110, 97,  109, 101, 61,
    34,  76,  111, 103, 111, 95,  77,  111, 110, 111, 80,  108, 117, 103, 115, 95,  49,  46,  115,
    118, 103, 34,  62,  10,  32,  32,  60,  115, 111, 100, 105, 112, 111, 100, 105, 58,  110, 97,
    109, 101, 100, 118, 105, 101, 119, 10,  32,  32,  32,  32,  32,  112, 97,  103, 101, 99,  111,
    108, 111, 114, 61,  34,  35,  102, 102, 102, 102, 102, 102, 34,  10,  32,  32,  32,  32,  32,
    98,  111, 114, 100, 101, 114, 99,  111, 108, 111, 114, 61,  34,  35,  54,  54,  54,  54,  54,
    54,  34,  10,  32,  32,  32,  32,  32,  98,  111, 114, 100, 101, 114, 111, 112, 97,  99,  105,
    116, 121, 61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  111, 98,  106, 101, 99,  116, 116,
    111, 108, 101, 114, 97,  110, 99,  101, 61,  34,  49,  48,  34,  10,  32,  32,  32,  32,  32,
    103, 114, 105, 100, 116, 111, 108, 101, 114, 97,  110, 99,  101, 61,  34,  49,  48,  34,  10,
    32,  32,  32,  32,  32,  103, 117, 105, 100, 101, 116, 111, 108, 101, 114, 97,  110, 99,  101,
    61,  34,  49,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101,
    58,  112, 97,  103, 101, 111, 112, 97,  99,  105, 116, 121, 61,  34,  48,  34,  10,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  112, 97,  103, 101, 115, 104, 97,
    100, 111, 119, 61,  34,  50,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,
    112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  119, 105, 100, 116, 104, 61,  34,  49,  57,
    50,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119,
    105, 110, 100, 111, 119, 45,  104, 101, 105, 103, 104, 116, 61,  34,  49,  48,  54,  48,  34,
    10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  110, 97,  109, 101, 100, 118, 105, 101, 119,
    52,  57,  34,  10,  32,  32,  32,  32,  32,  115, 104, 111, 119, 103, 114, 105, 100, 61,  34,
    102, 97,  108, 115, 101, 34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112,
    101, 58,  122, 111, 111, 109, 61,  34,  48,  46,  51,  49,  55,  49,  52,  55,  56,  53,  34,
    10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  120, 61,  34,
    45,  54,  53,  54,  46,  55,  52,  54,  53,  56,  34,  10,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  121, 61,  34,  52,  52,  46,  55,  53,  54,  54,  55,
    55,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119, 105,
    110, 100, 111, 119, 45,  120, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  121, 61,  34,  45,  51,  34,
    10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119, 105, 110, 100,
    111, 119, 45,  109, 97,  120, 105, 109, 105, 122, 101, 100, 61,  34,  49,  34,  10,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  117, 114, 114, 101, 110, 116,
    45,  108, 97,  121, 101, 114, 61,  34,  115, 118, 103, 50,  34,  32,  47,  62,  10,  32,  32,
    60,  100, 101, 102, 115, 10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  100, 101, 102, 115,
    52,  34,  32,  47,  62,  10,  32,  32,  60,  109, 101, 116, 97,  100, 97,  116, 97,  10,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  109, 101, 116, 97,  100, 97,  116, 97,  55,  34,  62,
    10,  32,  32,  32,  32,  60,  114, 100, 102, 58,  82,  68,  70,  62,  10,  32,  32,  32,  32,
    32,  32,  60,  99,  99,  58,  87,  111, 114, 107, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  114, 100, 102, 58,  97,  98,  111, 117, 116, 61,  34,  34,  62,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  60,  100, 99,  58,  102, 111, 114, 109, 97,  116, 62,  105, 109, 97,  103,
    101, 47,  115, 118, 103, 43,  120, 109, 108, 60,  47,  100, 99,  58,  102, 111, 114, 109, 97,
    116, 62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  100, 99,  58,  116, 121, 112, 101,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 100, 102, 58,  114, 101, 115,
    111, 117, 114, 99,  101, 61,  34,  104, 116, 116, 112, 58,  47,  47,  112, 117, 114, 108, 46,
    111, 114, 103, 47,  100, 99,  47,  100, 99,  109, 105, 116, 121, 112, 101, 47,  83,  116, 105,
    108, 108, 73,  109, 97,  103, 101, 34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  60,  100, 99,  58,  116, 105, 116, 108, 101, 32,  47,  62,  10,  32,  32,  32,  32,  32,
    32,  60,  47,  99,  99,  58,  87,  111, 114, 107, 62,  10,  32,  32,  32,  32,  60,  47,  114,
    100, 102, 58,  82,  68,  70,  62,  10,  32,  32,  60,  47,  109, 101, 116, 97,  100, 97,  116,
    97,  62,  10,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105,
    108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114,
    117, 108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 58,
    35,  48,  48,  48,  48,  48,  48,  59,  115, 116, 114, 111, 107, 101, 45,  119, 105, 100, 116,
    104, 58,  49,  112, 120, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 99,  97,
    112, 58,  98,  117, 116, 116, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 106,
    111, 105, 110, 58,  109, 105, 116, 101, 114, 59,  115, 116, 114, 111, 107, 101, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114,
    101, 99,  116, 51,  48,  56,  57,  45,  57,  45,  48,  34,  10,  32,  32,  32,  32,  32,  119,
    105, 100, 116, 104, 61,  34,  51,  55,  49,  46,  50,  55,  55,  57,  53,  34,  10,  32,  32,
    32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,  56,  51,  46,  56,  57,  48,  51,
    53,  34,  10,  32,  32,  32,  32,  32,  120, 61,  34,  45,  49,  54,  48,  46,  52,  49,  52,
    49,  53,  34,  10,  32,  32,  32,  32,  32,  121, 61,  34,  52,  52,  52,  46,  50,  48,  55,
    51,  55,  34,  32,  47,  62,  10,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,  48,
    48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105,
    108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114,
    111, 107, 101, 58,  35,  48,  48,  48,  48,  48,  48,  59,  115, 116, 114, 111, 107, 101, 45,
    119, 105, 100, 116, 104, 58,  49,  112, 120, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105,
    110, 101, 99,  97,  112, 58,  98,  117, 116, 116, 59,  115, 116, 114, 111, 107, 101, 45,  108,
    105, 110, 101, 106, 111, 105, 110, 58,  109, 105, 116, 101, 114, 59,  115, 116, 114, 111, 107,
    101, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  114, 101, 99,  116, 51,  48,  56,  57,  45,  57,  45,  56,  34,  10,  32,  32,
    32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  55,  49,  46,  50,  55,  55,  57,  53,
    34,  10,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,  56,  51,  46,
    56,  57,  48,  51,  53,  34,  10,  32,  32,  32,  32,  32,  120, 61,  34,  45,  54,  55,  49,
    46,  50,  49,  54,  57,  50,  34,  10,  32,  32,  32,  32,  32,  121, 61,  34,  52,  52,  49,
    46,  48,  53,  52,  50,  54,  34,  32,  47,  62,  10,  32,  32,  60,  114, 101, 99,  116, 10,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  48,
    48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101,
    110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 58,  35,  48,  48,  48,  48,  48,  48,
    59,  115, 116, 114, 111, 107, 101, 45,  119, 105, 100, 116, 104, 58,  49,  112, 120, 59,  115,
    116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 99,  97,  112, 58,  98,  117, 116, 116, 59,
    115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 106, 111, 105, 110, 58,  109, 105, 116,
    101, 114, 59,  115, 116, 114, 111, 107, 101, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 51,  48,  56,  57,  34,  10,  32,  32,
    32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  55,  49,  46,  50,  55,  55,  57,  53,
    34,  10,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,  56,  51,  46,
    56,  57,  48,  51,  53,  34,  10,  32,  32,  32,  32,  32,  120, 61,  34,  45,  49,  54,  57,
    49,  46,  54,  52,  34,  10,  32,  32,  32,  32,  32,  121, 61,  34,  52,  52,  51,  46,  56,
    49,  51,  50,  34,  10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109,
    61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  45,  52,  46,  57,  50,  50,  57,
    57,  50,  53,  101, 45,  54,  44,  48,  41,  34,  32,  47,  62,  10,  32,  32,  60,  114, 101,
    99,  116, 10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105,
    116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110,
    111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 58,  35,  48,  48,  48,  48,  48,  48,  59,
    115, 116, 114, 111, 107, 101, 45,  119, 105, 100, 116, 104, 58,  49,  112, 120, 59,  115, 116,
    114, 111, 107, 101, 45,  108, 105, 110, 101, 99,  97,  112, 58,  98,  117, 116, 116, 59,  115,
    116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 106, 111, 105, 110, 58,  109, 105, 116, 101,
    114, 59,  115, 116, 114, 111, 107, 101, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,
    10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 51,  48,  56,  57,  45,
    57,  34,  10,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  55,  49,  46,
    50,  55,  55,  57,  53,  34,  10,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,
    34,  51,  56,  51,  46,  56,  57,  48,  51,  53,  34,  10,  32,  32,  32,  32,  32,  120, 61,
    34,  45,  49,  49,  56,  53,  46,  49,  55,  50,  57,  34,  10,  32,  32,  32,  32,  32,  121,
    61,  34,  52,  52,  49,  46,  48,  53,  52,  50,  51,  34,  32,  47,  62,  10,  32,  32,  60,
    103, 10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  52,  55,  52,  34,  10,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  100, 105, 115, 112, 108, 97,  121, 58,
    105, 110, 108, 105, 110, 101, 34,  10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102,
    111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  45,  52,  46,  52,
    53,  57,  49,  54,  49,  57,  44,  54,  50,  46,  52,  50,  56,  50,  54,  54,  41,  34,  62,
    10,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    103, 52,  51,  55,  57,  34,  62,  10,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,
    99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108,
    108, 58,  35,  102, 102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110,
    122, 101, 114, 111, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105,
    115, 105, 98,  105, 108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105,
    115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108,
    111, 119, 58,  118, 105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,
    97,  99,  107, 103, 114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116,
    101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  49,
    51,  49,  46,  55,  54,  53,  49,  49,  44,  51,  54,  49,  46,  52,  56,  48,  52,  55,  32,
    99,  32,  45,  50,  55,  46,  49,  53,  52,  51,  49,  44,  48,  32,  45,  52,  57,  46,  48,
    49,  53,  54,  51,  44,  50,  49,  46,  56,  54,  49,  51,  49,  32,  45,  52,  57,  46,  48,
    49,  53,  54,  51,  44,  52,  57,  46,  48,  49,  53,  54,  50,  32,  108, 32,  48,  44,  51,
    50,  52,  46,  51,  57,  52,  53,  51,  32,  99,  32,  48,  44,  50,  55,  46,  49,  53,  52,
    51,  49,  32,  50,  49,  46,  56,  54,  49,  51,  50,  44,  52,  57,  46,  48,  49,  53,  54,
    51,  32,  52,  57,  46,  48,  49,  53,  54,  51,  44,  52,  57,  46,  48,  49,  53,  54,  51,
    32,  108, 32,  51,  50,  52,  46,  51,  57,  52,  53,  51,  44,  48,  32,  99,  32,  50,  55,
    46,  49,  53,  52,  51,  49,  44,  48,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,
    50,  49,  46,  56,  54,  49,  51,  50,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,
    52,  57,  46,  48,  49,  53,  54,  51,  32,  108, 32,  48,  44,  45,  51,  50,  52,  46,  51,
    57,  52,  53,  51,  32,  99,  32,  48,  44,  45,  50,  55,  46,  49,  53,  52,  51,  49,  32,
    45,  50,  49,  46,  56,  53,  57,  51,  54,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,
    32,  45,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  52,  57,  46,  48,  49,  53,  54,
    50,  32,  108, 32,  45,  51,  50,  52,  46,  51,  57,  52,  53,  51,  44,  48,  32,  122, 32,
    109, 32,  55,  46,  57,  56,  50,  52,  50,  44,  51,  53,  46,  50,  51,  56,  50,  56,  32,
    51,  48,  52,  46,  54,  49,  53,  50,  51,  44,  48,  32,  99,  32,  49,  50,  46,  49,  50,
    52,  51,  49,  44,  48,  32,  50,  49,  46,  56,  56,  52,  55,  55,  44,  49,  49,  46,  50,
    56,  57,  57,  57,  32,  50,  49,  46,  56,  56,  52,  55,  55,  44,  50,  53,  46,  51,  49,
    52,  52,  53,  32,  108, 32,  48,  44,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,
    32,  48,  44,  49,  52,  46,  48,  50,  52,  52,  55,  32,  45,  57,  46,  55,  54,  48,  52,
    54,  44,  50,  53,  46,  51,  49,  52,  52,  54,  32,  45,  50,  49,  46,  56,  56,  52,  55,
    55,  44,  50,  53,  46,  51,  49,  52,  52,  54,  32,  108, 32,  45,  51,  48,  52,  46,  54,
    49,  53,  50,  51,  44,  48,  32,  99,  32,  45,  49,  50,  46,  49,  50,  52,  51,  49,  44,
    48,  32,  45,  50,  49,  46,  56,  56,  54,  55,  50,  44,  45,  49,  49,  46,  50,  56,  57,
    57,  57,  32,  45,  50,  49,  46,  56,  56,  54,  55,  50,  44,  45,  50,  53,  46,  51,  49,
    52,  52,  54,  32,  108, 32,  48,  44,  45,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,
    99,  32,  48,  44,  45,  49,  52,  46,  48,  50,  52,  52,  54,  32,  57,  46,  55,  54,  50,
    52,  49,  44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  50,  49,  46,  56,  56,  54,
    55,  50,  44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  122, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 52,  50,  56,  52,  45,
    54,  45,  55,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,
    99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108,
    108, 58,  35,  102, 102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110,
    122, 101, 114, 111, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105,
    115, 105, 98,  105, 108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105,
    115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108,
    111, 119, 58,  118, 105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,
    97,  99,  107, 103, 114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116,
    101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  54,
    52,  50,  46,  50,  50,  54,  44,  51,  54,  49,  46,  52,  56,  48,  52,  55,  32,  99,  32,
    45,  50,  55,  46,  49,  53,  52,  51,  49,  44,  48,  32,  45,  52,  57,  46,  48,  49,  53,
    54,  51,  44,  50,  49,  46,  56,  54,  49,  51,  49,  32,  45,  52,  57,  46,  48,  49,  53,
    54,  51,  44,  52,  57,  46,  48,  49,  53,  54,  50,  32,  108, 32,  48,  44,  51,  50,  52,
    46,  51,  57,  52,  53,  51,  32,  99,  32,  48,  44,  50,  55,  46,  49,  53,  52,  51,  49,
    32,  50,  49,  46,  56,  54,  49,  51,  50,  44,  52,  57,  46,  48,  49,  53,  54,  51,  32,
    52,  57,  46,  48,  49,  53,  54,  51,  44,  52,  57,  46,  48,  49,  53,  54,  51,  32,  108,
    32,  51,  50,  52,  46,  51,  57,  52,  53,  51,  44,  48,  32,  99,  32,  50,  55,  46,  49,
    53,  52,  51,  49,  44,  48,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  50,  49,
    46,  56,  54,  49,  51,  50,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  52,  57,
    46,  48,  49,  53,  54,  51,  32,  108, 32,  48,  44,  45,  51,  50,  52,  46,  51,  57,  52,
    53,  51,  32,  99,  32,  48,  44,  45,  50,  55,  46,  49,  53,  52,  51,  49,  32,  45,  50,
    49,  46,  56,  53,  57,  51,  54,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,  32,  45,
    52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,  32,
    108, 32,  45,  51,  50,  52,  46,  51,  57,  52,  53,  51,  44,  48,  32,  122, 32,  109, 32,
    55,  46,  57,  56,  50,  52,  50,  44,  51,  53,  46,  50,  51,  56,  50,  56,  32,  51,  48,
    52,  46,  54,  49,  53,  50,  51,  44,  48,  32,  99,  32,  49,  50,  46,  49,  50,  52,  51,
    49,  44,  48,  32,  50,  49,  46,  56,  56,  52,  55,  55,  44,  49,  49,  46,  50,  56,  57,
    57,  57,  32,  50,  49,  46,  56,  56,  52,  55,  55,  44,  50,  53,  46,  51,  49,  52,  52,
    53,  32,  108, 32,  48,  44,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,  32,  48,
    44,  49,  52,  46,  48,  50,  52,  52,  55,  32,  45,  57,  46,  55,  54,  48,  52,  54,  44,
    50,  53,  46,  51,  49,  52,  52,  54,  32,  45,  50,  49,  46,  56,  56,  52,  55,  55,  44,
    50,  53,  46,  51,  49,  52,  52,  54,  32,  108, 32,  45,  51,  48,  52,  46,  54,  49,  53,
    50,  51,  44,  48,  32,  99,  32,  45,  49,  50,  46,  49,  50,  52,  51,  49,  44,  48,  32,
    45,  50,  49,  46,  56,  56,  54,  55,  50,  44,  45,  49,  49,  46,  50,  56,  57,  57,  57,
    32,  45,  50,  49,  46,  56,  56,  54,  55,  50,  44,  45,  50,  53,  46,  51,  49,  52,  52,
    54,  32,  108, 32,  48,  44,  45,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,  32,
    48,  44,  45,  49,  52,  46,  48,  50,  52,  52,  54,  32,  57,  46,  55,  54,  50,  52,  49,
    44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  50,  49,  46,  56,  56,  54,  55,  50,
    44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  122, 34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 52,  50,  56,  52,  45,  54,  45,
    55,  45,  56,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,
    99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108,
    108, 58,  35,  102, 102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110,
    122, 101, 114, 111, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105,
    115, 105, 98,  105, 108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105,
    115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108,
    111, 119, 58,  118, 105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,
    97,  99,  107, 103, 114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116,
    101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  49,
    49,  53,  49,  46,  55,  57,  57,  55,  44,  51,  54,  49,  46,  52,  56,  48,  52,  55,  32,
    99,  32,  45,  50,  55,  46,  49,  53,  52,  51,  44,  48,  32,  45,  52,  57,  46,  48,  49,
    53,  54,  44,  50,  49,  46,  56,  54,  49,  51,  49,  32,  45,  52,  57,  46,  48,  49,  53,
    54,  44,  52,  57,  46,  48,  49,  53,  54,  50,  32,  108, 32,  48,  44,  51,  50,  52,  46,
    51,  57,  52,  53,  51,  32,  99,  32,  48,  44,  50,  55,  46,  49,  53,  52,  51,  49,  32,
    50,  49,  46,  56,  54,  49,  51,  44,  52,  57,  46,  48,  49,  53,  54,  51,  32,  52,  57,
    46,  48,  49,  53,  54,  44,  52,  57,  46,  48,  49,  53,  54,  51,  32,  108, 32,  51,  50,
    52,  46,  51,  57,  52,  53,  52,  44,  48,  32,  99,  32,  50,  55,  46,  49,  53,  52,  51,
    49,  44,  48,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  50,  49,  46,  56,  54,
    49,  51,  50,  32,  52,  57,  46,  48,  49,  51,  54,  55,  44,  45,  52,  57,  46,  48,  49,
    53,  54,  51,  32,  108, 32,  48,  44,  45,  51,  50,  52,  46,  51,  57,  52,  53,  51,  32,
    99,  32,  48,  44,  45,  50,  55,  46,  49,  53,  52,  51,  49,  32,  45,  50,  49,  46,  56,
    53,  57,  51,  54,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,  32,  45,  52,  57,  46,
    48,  49,  51,  54,  55,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,  32,  108, 32,  45,
    51,  50,  52,  46,  51,  57,  52,  53,  52,  44,  48,  32,  122, 32,  109, 32,  55,  46,  57,
    56,  50,  52,  44,  51,  53,  46,  50,  51,  56,  50,  56,  32,  51,  48,  52,  46,  54,  49,
    53,  50,  54,  44,  48,  32,  99,  32,  49,  50,  46,  49,  50,  52,  51,  49,  44,  48,  32,
    50,  49,  46,  56,  56,  52,  55,  55,  44,  49,  49,  46,  50,  56,  57,  57,  57,  32,  50,
    49,  46,  56,  56,  52,  55,  55,  44,  50,  53,  46,  51,  49,  52,  52,  53,  32,  108, 32,
    48,  44,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,  32,  48,  44,  49,  52,  46,
    48,  50,  52,  52,  55,  32,  45,  57,  46,  55,  54,  48,  52,  54,  44,  50,  53,  46,  51,
    49,  52,  52,  54,  32,  45,  50,  49,  46,  56,  56,  52,  55,  55,  44,  50,  53,  46,  51,
    49,  52,  52,  54,  32,  108, 32,  45,  51,  48,  52,  46,  54,  49,  53,  50,  54,  44,  48,
    32,  99,  32,  45,  49,  50,  46,  49,  50,  52,  51,  44,  48,  32,  45,  50,  49,  46,  56,
    56,  54,  55,  44,  45,  49,  49,  46,  50,  56,  57,  57,  57,  32,  45,  50,  49,  46,  56,
    56,  54,  55,  44,  45,  50,  53,  46,  51,  49,  52,  52,  54,  32,  108, 32,  48,  44,  45,
    50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,  32,  48,  44,  45,  49,  52,  46,  48,
    50,  52,  52,  54,  32,  57,  46,  55,  54,  50,  52,  44,  45,  50,  53,  46,  51,  49,  52,
    52,  53,  32,  50,  49,  46,  56,  56,  54,  55,  44,  45,  50,  53,  46,  51,  49,  52,  52,
    53,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114,
    101, 99,  116, 52,  50,  56,  52,  45,  54,  45,  55,  45,  48,  34,  32,  47,  62,  10,  32,
    32,  32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114,
    45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114, 58,  35,
    48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102, 102, 102, 102, 102,
    59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108,
    108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116, 114, 111,
    107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,
    118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108,
    105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105, 98,  108,
    101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117, 110, 100,
    58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  100, 61,  34,  109, 32,  45,  49,  54,  54,  50,  46,  50,  54,  48,  54,  44,
    51,  54,  49,  46,  52,  56,  48,  52,  55,  32,  99,  32,  45,  50,  55,  46,  49,  53,  52,
    51,  44,  48,  32,  45,  52,  57,  46,  48,  49,  53,  54,  44,  50,  49,  46,  56,  54,  49,
    51,  49,  32,  45,  52,  57,  46,  48,  49,  53,  54,  44,  52,  57,  46,  48,  49,  53,  54,
    50,  32,  108, 32,  48,  44,  51,  50,  52,  46,  51,  57,  52,  53,  51,  32,  99,  32,  48,
    44,  50,  55,  46,  49,  53,  52,  51,  49,  32,  50,  49,  46,  56,  54,  49,  51,  44,  52,
    57,  46,  48,  49,  53,  54,  51,  32,  52,  57,  46,  48,  49,  53,  54,  44,  52,  57,  46,
    48,  49,  53,  54,  51,  32,  108, 32,  51,  50,  52,  46,  51,  57,  52,  54,  44,  48,  32,
    99,  32,  50,  55,  46,  49,  53,  52,  51,  44,  48,  32,  52,  57,  46,  48,  49,  51,  54,
    44,  45,  50,  49,  46,  56,  54,  49,  51,  50,  32,  52,  57,  46,  48,  49,  51,  54,  44,
    45,  52,  57,  46,  48,  49,  53,  54,  51,  32,  108, 32,  48,  44,  45,  51,  50,  52,  46,
    51,  57,  52,  53,  51,  32,  99,  32,  48,  44,  45,  50,  55,  46,  49,  53,  52,  51,  49,
    32,  45,  50,  49,  46,  56,  53,  57,  51,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,
    32,  45,  52,  57,  46,  48,  49,  51,  54,  44,  45,  52,  57,  46,  48,  49,  53,  54,  50,
    32,  108, 32,  45,  51,  50,  52,  46,  51,  57,  52,  54,  44,  48,  32,  122, 32,  109, 32,
    55,  46,  57,  56,  50,  52,  44,  51,  53,  46,  50,  51,  56,  50,  56,  32,  51,  48,  52,
    46,  54,  49,  53,  51,  44,  48,  32,  99,  32,  49,  50,  46,  49,  50,  52,  51,  44,  48,
    32,  50,  49,  46,  56,  56,  52,  55,  44,  49,  49,  46,  50,  56,  57,  57,  57,  32,  50,
    49,  46,  56,  56,  52,  55,  44,  50,  53,  46,  51,  49,  52,  52,  53,  32,  108, 32,  48,
    44,  50,  57,  55,  46,  55,  53,  53,  56,  54,  32,  99,  32,  48,  44,  49,  52,  46,  48,
    50,  52,  52,  55,  32,  45,  57,  46,  55,  54,  48,  52,  44,  50,  53,  46,  51,  49,  52,
    52,  54,  32,  45,  50,  49,  46,  56,  56,  52,  55,  44,  50,  53,  46,  51,  49,  52,  52,
    54,  32,  108, 32,  45,  51,  48,  52,  46,  54,  49,  53,  51,  44,  48,  32,  99,  32,  45,
    49,  50,  46,  49,  50,  52,  51,  44,  48,  32,  45,  50,  49,  46,  56,  56,  54,  55,  44,
    45,  49,  49,  46,  50,  56,  57,  57,  57,  32,  45,  50,  49,  46,  56,  56,  54,  55,  44,
    45,  50,  53,  46,  51,  49,  52,  52,  54,  32,  108, 32,  48,  44,  45,  50,  57,  55,  46,
    55,  53,  53,  56,  54,  32,  99,  32,  48,  44,  45,  49,  52,  46,  48,  50,  52,  52,  54,
    32,  57,  46,  55,  54,  50,  52,  44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  50,
    49,  46,  56,  56,  54,  55,  44,  45,  50,  53,  46,  51,  49,  52,  52,  53,  32,  122, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 52,
    50,  56,  52,  45,  54,  45,  55,  45,  56,  45,  49,  34,  32,  47,  62,  10,  32,  32,  32,
    32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  103, 52,  52,  53,  55,  34,  62,  10,  32,  32,  32,  32,  32,  32,
    60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  51,
    55,  53,  34,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  52,  57,  46,  48,
    49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114,
    120, 61,  34,  52,  57,  46,  48,  49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  121, 61,  34,  51,  54,  49,  46,  52,  56,  48,  54,  53,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,  50,  56,  46,
    56,  55,  48,  54,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104,
    101, 105, 103, 104, 116, 61,  34,  52,  50,  50,  46,  52,  50,  52,  55,  49,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  52,  50,
    50,  46,  52,  50,  52,  55,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  114, 101, 99,  116, 52,  50,  56,  52,  45,  54,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108,
    111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102,
    102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,
    115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108,
    105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121,
    58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105,
    115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114,
    111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,  62,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  50,  53,  46,  51,  49,  52,  57,  50,
    52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  50,
    49,  46,  56,  56,  53,  48,  51,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  121, 61,  34,  51,  57,  54,  46,  55,  49,  55,  57,  54,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,  54,  51,  46,  57,  56,  50,  54,
    55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104,
    116, 61,  34,  51,  52,  56,  46,  51,  56,  53,  52,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  52,  56,  46,  51,  56,
    53,  55,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  114, 101, 99,  116, 51,  51,  53,  49,  45,  57,  45,  52,  45,  49,  45,  53,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102,
    105, 108, 108, 58,  35,  102, 52,  55,  51,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101,
    59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  32,  47,  62,
    10,  32,  32,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  32,  32,  60,
    103, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  53,  49,  48,  46,  50,  54,
    48,  54,  51,  44,  50,  46,  56,  54,  56,  57,  52,  48,  56,  101, 45,  54,  41,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  100, 105, 115,
    112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 52,  51,  55,  53,  45,  56,  34,  62,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  114, 121, 61,  34,  52,  57,  46,  48,  49,  52,  57,  57,  57,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  52,  57,  46,  48,
    49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  121,
    61,  34,  51,  54,  49,  46,  52,  56,  48,  54,  53,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  120, 61,  34,  51,  50,  56,  46,  56,  55,  48,  54,  49,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,
    52,  50,  50,  46,  52,  50,  52,  55,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  52,  50,  50,  46,  52,  50,  52,  55,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101,
    99,  116, 52,  50,  56,  52,  45,  54,  45,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114, 58,  35,  48,
    48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102, 102, 102, 102, 102, 59,
    102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108, 108,
    45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116, 114, 111, 107,
    101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,  118,
    105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105,
    110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105, 98,  108, 101,
    59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117, 110, 100, 58,
    97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,  62,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  114, 121, 61,  34,  50,  53,  46,  51,  49,  52,  57,  50,  52,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  50,  49,  46,  56,  56,  53,
    48,  51,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,
    51,  57,  54,  46,  55,  49,  55,  57,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  120, 61,  34,  51,  54,  51,  46,  57,  56,  50,  54,  55,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,  52,
    56,  46,  51,  56,  53,  52,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  119, 105, 100, 116, 104, 61,  34,  51,  52,  56,  46,  51,  56,  53,  55,  55,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116,
    51,  51,  53,  49,  45,  57,  45,  52,  45,  49,  45,  53,  45,  57,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108,
    58,  35,  102, 52,  55,  51,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105,
    116, 121, 58,  49,  59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  100, 105,
    115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  32,  47,  62,  10,  32,  32,
    32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,
    116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  49,  48,  50,  49,  46,  50,  48,  53,  55,
    44,  50,  46,  56,  54,  56,  57,  52,  48,  56,  101, 45,  54,  41,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  100, 105, 115, 112, 108, 97,
    121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  103, 52,  51,  55,  53,  45,  56,  45,  54,  34,  62,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  114, 121, 61,  34,  52,  57,  46,  48,  49,  52,  57,  57,  57,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  52,  57,  46,  48,  49,
    52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,
    34,  51,  54,  49,  46,  52,  56,  48,  54,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  120, 61,  34,  51,  50,  56,  46,  56,  55,  48,  54,  49,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  52,
    50,  50,  46,  52,  50,  52,  55,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  119, 105, 100, 116, 104, 61,  34,  52,  50,  50,  46,  52,  50,  52,  55,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,
    116, 52,  50,  56,  52,  45,  54,  45,  49,  45,  57,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114, 58,  35,
    48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102, 102, 102, 102, 102,
    59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108,
    108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116, 114, 111,
    107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,
    118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108,
    105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105, 98,  108,
    101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117, 110, 100,
    58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,  62,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  114, 121, 61,  34,  50,  53,  46,  51,  49,  52,  57,  50,  52,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  50,  49,  46,  56,  56,
    53,  48,  51,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,
    34,  51,  57,  54,  46,  55,  49,  55,  57,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  120, 61,  34,  51,  54,  51,  46,  57,  56,  50,  54,  55,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,
    52,  56,  46,  51,  56,  53,  52,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  52,  56,  46,  51,  56,  53,  55,  55,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,
    116, 51,  51,  53,  49,  45,  57,  45,  52,  45,  49,  45,  53,  45,  57,  45,  52,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102,
    105, 108, 108, 58,  35,  102, 52,  55,  51,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101,
    59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  32,  47,  62,
    10,  32,  32,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  32,  32,  60,
    103, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  49,  53,  51,  49,  46,  49,
    50,  52,  49,  44,  50,  46,  56,  54,  56,  57,  52,  48,  56,  101, 45,  54,  41,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  100, 105, 115,
    112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 52,  51,  55,  53,  45,  56,  45,  53,  34,  62,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  52,  57,  46,  48,  49,  52,  57,  57,  57,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  52,  57,
    46,  48,  49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  121, 61,  34,  51,  54,  49,  46,  52,  56,  48,  54,  53,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,  50,  56,  46,  56,  55,  48,  54,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104, 116,
    61,  34,  52,  50,  50,  46,  52,  50,  52,  55,  49,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  52,  50,  50,  46,  52,  50,  52,
    55,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    114, 101, 99,  116, 52,  50,  56,  52,  45,  54,  45,  49,  45,  57,  48,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108,
    111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102,
    102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,
    115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108,
    105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121,
    58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105,
    115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114,
    111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,  62,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116, 10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  50,  53,  46,  51,  49,  52,  57,  50,
    52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  50,
    49,  46,  56,  56,  53,  48,  51,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  121, 61,  34,  51,  57,  54,  46,  55,  49,  55,  57,  54,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,  54,  51,  46,  57,  56,  50,  54,
    55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  104, 101, 105, 103, 104,
    116, 61,  34,  51,  52,  56,  46,  51,  56,  53,  52,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  51,  52,  56,  46,  51,  56,
    53,  55,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  114, 101, 99,  116, 51,  51,  53,  49,  45,  57,  45,  52,  45,  49,  45,  53,  45,  57,
    45,  56,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108,
    101, 61,  34,  102, 105, 108, 108, 58,  35,  102, 52,  55,  51,  48,  48,  59,  102, 105, 108,
    108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  115, 116, 114, 111, 107, 101, 58,
    110, 111, 110, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101,
    34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,
    32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  50,  48,
    52,  50,  46,  48,  54,  57,  50,  44,  50,  46,  56,  54,  56,  57,  52,  48,  56,  101, 45,
    54,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  51,  55,  53,  45,  56,  45,
    54,  45,  52,  34,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101, 99,  116,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  52,  57,  46,
    48,  49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    114, 120, 61,  34,  52,  57,  46,  48,  49,  52,  57,  57,  57,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  51,  54,  49,  46,  52,  56,  48,  54,  53,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,  50,  56,
    46,  56,  55,  48,  54,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    104, 101, 105, 103, 104, 116, 61,  34,  52,  50,  50,  46,  52,  50,  52,  55,  49,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,  34,  52,
    50,  50,  46,  52,  50,  52,  55,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 52,  50,  56,  52,  45,  54,  45,  49,  45,
    57,  45,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102,
    105, 108, 108, 58,  35,  102, 102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110,
    111, 110, 122, 101, 114, 111, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,
    118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,
    100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114,
    102, 108, 111, 119, 58,  118, 105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101,
    45,  98,  97,  99,  107, 103, 114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108,
    97,  116, 101, 34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  114, 101,
    99,  116, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 121, 61,  34,  50,
    53,  46,  51,  49,  52,  57,  50,  52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  114, 120, 61,  34,  50,  49,  46,  56,  56,  53,  48,  51,  49,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  121, 61,  34,  51,  57,  54,  46,  55,  49,  55,
    57,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  120, 61,  34,  51,
    54,  51,  46,  57,  56,  50,  54,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  104, 101, 105, 103, 104, 116, 61,  34,  51,  52,  56,  46,  51,  56,  53,  52,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  119, 105, 100, 116, 104, 61,
    34,  51,  52,  56,  46,  51,  56,  53,  55,  55,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  114, 101, 99,  116, 51,  51,  53,  49,  45,  57,  45,
    52,  45,  49,  45,  53,  45,  57,  45,  52,  45,  54,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  102,
    52,  55,  51,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    49,  59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  100, 105, 115, 112, 108,
    97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  32,  47,  62,  10,  32,  32,  32,  32,  32,
    32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  60,  47,
    103, 62,  10,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  103, 52,  54,  56,  52,  34,  10,  32,  32,  32,  32,  32,
    116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116,
    101, 40,  45,  52,  46,  52,  53,  57,  49,  54,  49,  57,  44,  54,  50,  46,  52,  50,  56,
    50,  54,  54,  41,  34,  62,  10,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  103, 52,  55,  50,  49,  34,  62,  10,  32,  32,  32,  32,  32,
    32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114,
    118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,  48,
    48,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110, 111, 100, 100,
    59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  100, 105, 115, 112, 108, 97,
    121, 58,  105, 110, 108, 105, 110, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    100, 61,  34,  109, 32,  57,  54,  56,  46,  49,  55,  54,  53,  57,  44,  52,  55,  48,  46,
    55,  53,  55,  57,  50,  32,  56,  55,  46,  50,  57,  48,  53,  49,  44,  48,  46,  52,  57,
    56,  56,  49,  32,  48,  46,  52,  57,  56,  56,  44,  49,  54,  50,  46,  49,  49,  48,  57,
    55,  32,  56,  52,  46,  55,  57,  54,  53,  44,  48,  32,  48,  44,  51,  55,  46,  52,  49,
    48,  50,  50,  32,  45,  49,  55,  50,  46,  48,  56,  55,  44,  48,  32,  122, 34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  54,  48,
    52,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111,
    110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,
    99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,
    35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116,
    121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101,
    114, 111, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105,
    98,  105, 108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112,
    108, 97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119,
    58,  118, 105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,
    107, 103, 114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  49,  52,  54,  53,
    46,  57,  50,  55,  55,  44,  52,  55,  49,  46,  53,  53,  48,  55,  56,  32,  48,  44,  49,
    50,  50,  46,  48,  51,  55,  49,  49,  32,  99,  32,  48,  44,  50,  57,  46,  52,  50,  57,
    57,  49,  32,  49,  57,  46,  56,  51,  54,  49,  44,  53,  53,  46,  49,  52,  56,  52,  50,
    32,  52,  53,  46,  57,  52,  51,  52,  44,  55,  48,  46,  49,  52,  52,  53,  51,  32,  51,
    48,  46,  50,  52,  56,  56,  44,  49,  55,  46,  51,  55,  53,  48,  56,  32,  54,  55,  46,
    53,  49,  54,  56,  44,  49,  55,  46,  51,  55,  53,  48,  56,  32,  57,  55,  46,  55,  54,
    53,  54,  44,  48,  32,  50,  54,  46,  49,  48,  55,  52,  44,  45,  49,  52,  46,  57,  57,
    54,  49,  52,  32,  52,  55,  46,  56,  52,  49,  56,  44,  45,  52,  48,  46,  54,  53,  55,
    57,  53,  32,  52,  55,  46,  56,  52,  49,  56,  44,  45,  55,  48,  46,  49,  52,  52,  53,
    51,  32,  108, 32,  48,  44,  45,  49,  50,  50,  46,  48,  51,  55,  49,  49,  32,  45,  49,
    56,  46,  53,  49,  51,  55,  44,  48,  32,  45,  50,  48,  46,  53,  49,  53,  54,  44,  48,
    32,  48,  44,  49,  50,  50,  46,  48,  51,  55,  49,  49,  32,  99,  32,  48,  44,  49,  54,
    46,  53,  57,  57,  50,  50,  32,  45,  49,  50,  46,  50,  49,  55,  51,  44,  51,  48,  46,
    50,  54,  57,  57,  51,  32,  45,  50,  55,  46,  48,  49,  55,  54,  44,  51,  56,  46,  55,
    55,  49,  52,  57,  32,  45,  49,  49,  46,  57,  53,  53,  57,  44,  54,  46,  56,  54,  55,
    54,  56,  32,  45,  50,  53,  46,  54,  53,  56,  53,  44,  57,  46,  52,  48,  52,  51,  57,
    32,  45,  51,  56,  46,  57,  48,  56,  50,  44,  55,  46,  54,  50,  49,  48,  57,  32,  108,
    32,  48,  44,  45,  49,  54,  56,  46,  52,  50,  57,  54,  57,  32,  122, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  54,  48,  56,
    34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  53,  54,  51,  34,  62,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116,
    111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  53,
    54,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109,
    32,  45,  53,  55,  52,  46,  54,  51,  56,  55,  51,  44,  54,  55,  49,  46,  49,  48,  50,
    50,  32,  48,  44,  45,  49,  57,  57,  46,  51,  55,  52,  57,  54,  32,  51,  54,  46,  57,
    55,  54,  55,  53,  44,  48,  32,  49,  49,  51,  46,  52,  50,  56,  54,  57,  44,  55,  54,
    46,  52,  53,  49,  57,  52,  32,  48,  44,  45,  55,  54,  46,  52,  53,  49,  57,  52,  32,
    51,  55,  46,  52,  55,  54,  52,  52,  44,  48,  32,  48,  44,  50,  48,  53,  46,  56,  55,
    48,  53,  56,  32,  45,  49,  53,  48,  46,  52,  48,  53,  52,  52,  44,  45,  49,  48,  49,
    46,  52,  51,  54,  50,  51,  32,  48,  44,  57,  52,  46,  57,  52,  48,  51,  49,  32,  122,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  102, 105, 108, 108, 58,  35,  102, 102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,
    114, 117, 108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101,
    58,  35,  48,  48,  48,  48,  48,  48,  59,  115, 116, 114, 111, 107, 101, 45,  119, 105, 100,
    116, 104, 58,  49,  112, 120, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101, 99,
    97,  112, 58,  98,  117, 116, 116, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105, 110, 101,
    106, 111, 105, 110, 58,  109, 105, 116, 101, 114, 59,  115, 116, 114, 111, 107, 101, 45,  111,
    112, 97,  99,  105, 116, 121, 58,  49,  59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110,
    108, 105, 110, 101, 34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  112,
    97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115,
    99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118,
    97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  52,  57,  56,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  49,  54,  50,  53,  46,
    57,  49,  50,  53,  44,  52,  55,  52,  46,  48,  52,  50,  49,  54,  32,  56,  50,  46,  55,
    48,  52,  55,  44,  45,  48,  46,  54,  49,  50,  54,  50,  32,  52,  52,  46,  53,  55,  55,
    53,  44,  56,  48,  46,  56,  54,  54,  55,  56,  32,  52,  52,  46,  56,  54,  54,  44,  45,
    56,  49,  46,  52,  55,  57,  52,  49,  32,  55,  55,  46,  52,  57,  55,  52,  44,  48,  46,
    54,  49,  50,  54,  51,  32,  48,  44,  50,  48,  48,  46,  51,  50,  57,  48,  56,  32,  45,
    56,  55,  46,  57,  49,  50,  44,  48,  32,  48,  44,  45,  49,  48,  54,  46,  50,  57,  48,
    56,  49,  32,  45,  54,  48,  46,  57,  53,  54,  52,  44,  49,  49,  51,  46,  51,  51,  54,
    48,  50,  32,  45,  54,  50,  46,  55,  57,  52,  51,  44,  45,  49,  49,  52,  46,  56,  54,
    55,  53,  57,  32,  48,  44,  49,  48,  55,  46,  56,  50,  50,  51,  56,  32,  45,  51,  56,
    46,  53,  57,  53,  53,  44,  48,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  102, 102, 102,
    102, 102, 102, 59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110, 111,
    100, 100, 59,  115, 116, 114, 111, 107, 101, 58,  35,  48,  48,  48,  48,  48,  48,  59,  115,
    116, 114, 111, 107, 101, 45,  119, 105, 100, 116, 104, 58,  49,  112, 120, 59,  115, 116, 114,
    111, 107, 101, 45,  108, 105, 110, 101, 99,  97,  112, 58,  98,  117, 116, 116, 59,  115, 116,
    114, 111, 107, 101, 45,  108, 105, 110, 101, 106, 111, 105, 110, 58,  109, 105, 116, 101, 114,
    59,  115, 116, 114, 111, 107, 101, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  32,
    47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  53,  51,  53,  34,  62,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  60,  101, 108, 108, 105, 112, 115, 101, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100,
    105, 58,  114, 121, 61,  34,  49,  48,  53,  46,  57,  56,  52,  53,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,
    114, 120, 61,  34,  49,  49,  48,  46,  56,  56,  53,  53,  51,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,
    121, 61,  34,  53,  55,  49,  46,  55,  53,  54,  49,  54,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,  120,
    61,  34,  45,  57,  57,  50,  46,  51,  49,  49,  56,  51,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  56,  56,  49,  46,  52,
    50,  54,  51,  44,  53,  55,  49,  46,  55,  53,  54,  49,  54,  32,  99,  32,  48,  44,  53,
    56,  46,  53,  51,  51,  54,  51,  32,  45,  52,  57,  46,  54,  52,  53,  49,  52,  44,  49,
    48,  53,  46,  57,  56,  52,  53,  32,  45,  49,  49,  48,  46,  56,  56,  53,  53,  51,  44,
    49,  48,  53,  46,  57,  56,  52,  53,  32,  45,  54,  49,  46,  50,  52,  48,  51,  55,  44,
    48,  32,  45,  49,  49,  48,  46,  56,  56,  53,  53,  55,  44,  45,  52,  55,  46,  52,  53,
    48,  56,  55,  32,  45,  49,  49,  48,  46,  56,  56,  53,  53,  55,  44,  45,  49,  48,  53,
    46,  57,  56,  52,  53,  32,  48,  44,  45,  53,  56,  46,  53,  51,  51,  54,  50,  32,  52,
    57,  46,  54,  52,  53,  50,  44,  45,  49,  48,  53,  46,  57,  56,  52,  52,  57,  32,  49,
    49,  48,  46,  56,  56,  53,  53,  55,  44,  45,  49,  48,  53,  46,  57,  56,  52,  52,  57,
    32,  54,  49,  46,  50,  52,  48,  51,  57,  44,  48,  32,  49,  49,  48,  46,  56,  56,  53,
    53,  51,  44,  52,  55,  46,  52,  53,  48,  56,  55,  32,  49,  49,  48,  46,  56,  56,  53,
    53,  51,  44,  49,  48,  53,  46,  57,  56,  52,  52,  57,  32,  122, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111,
    108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102,
    102, 102, 102, 102, 102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111,
    59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105,
    108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,
    121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118,
    105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103,
    114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116,
    104, 52,  53,  48,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  114, 121, 61,  34,  49,  48,  53,  46,  57,  56,  52,  53,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  49,  49,  48,  46,  56,  56,
    53,  53,  51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  99,
    121, 61,  34,  53,  55,  49,  46,  55,  53,  54,  49,  54,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  99,  120, 61,  34,  45,  57,  57,  50,  46,  51,  49,
    49,  56,  51,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  60,
    112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,
    117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114,
    58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,
    48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102,
    105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116,
    114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116,
    121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105,
    110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105,
    98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117,
    110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  57,  57,  50,  46,
    51,  49,  50,  53,  44,  53,  48,  49,  46,  55,  56,  51,  50,  32,  97,  32,  55,  51,  46,
    50,  48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,  48,
    32,  48,  32,  45,  50,  46,  53,  57,  51,  55,  53,  44,  48,  46,  48,  56,  55,  57,  32,
    108, 32,  48,  44,  49,  51,  57,  46,  55,  51,  50,  52,  51,  32,  97,  32,  55,  51,  46,
    50,  48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,  48,
    32,  48,  32,  50,  46,  53,  57,  51,  55,  53,  44,  48,  46,  49,  50,  53,  32,  55,  51,
    46,  50,  48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,
    48,  32,  48,  32,  55,  51,  46,  50,  48,  56,  57,  56,  44,  45,  54,  57,  46,  57,  55,
    50,  54,  54,  32,  55,  51,  46,  50,  48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,
    51,  49,  57,  32,  48,  32,  48,  32,  48,  32,  45,  55,  51,  46,  50,  48,  56,  57,  56,
    44,  45,  54,  57,  46,  57,  55,  50,  54,  54,  32,  122, 34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  53,  48,
    48,  45,  54,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  47,  103,
    62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,
    98,  48,  48,  48,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  103, 52,  53,  51,  53,  45,  57,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110,
    115, 108, 97,  116, 101, 40,  49,  48,  49,  57,  46,  52,  48,  51,  57,  44,  45,  50,  46,
    55,  53,  54,  48,  53,  57,  51,  101, 45,  54,  41,  34,  62,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  60,  101, 108, 108, 105, 112, 115, 101, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  114, 121,
    61,  34,  49,  48,  53,  46,  57,  56,  52,  53,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  114, 120, 61,  34,
    49,  49,  48,  46,  56,  56,  53,  53,  51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,  121, 61,  34,  53,
    55,  49,  46,  55,  53,  54,  49,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  115, 111, 100, 105, 112, 111, 100, 105, 58,  99,  120, 61,  34,  45,  57,
    57,  50,  46,  51,  49,  49,  56,  51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  56,  56,  49,  46,  52,  50,  54,  51,  44,
    53,  55,  49,  46,  55,  53,  54,  49,  54,  32,  99,  32,  48,  44,  53,  56,  46,  53,  51,
    51,  54,  51,  32,  45,  52,  57,  46,  54,  52,  53,  49,  52,  44,  49,  48,  53,  46,  57,
    56,  52,  53,  32,  45,  49,  49,  48,  46,  56,  56,  53,  53,  51,  44,  49,  48,  53,  46,
    57,  56,  52,  53,  32,  45,  54,  49,  46,  50,  52,  48,  51,  55,  44,  48,  32,  45,  49,
    49,  48,  46,  56,  56,  53,  53,  55,  44,  45,  52,  55,  46,  52,  53,  48,  56,  55,  32,
    45,  49,  49,  48,  46,  56,  56,  53,  53,  55,  44,  45,  49,  48,  53,  46,  57,  56,  52,
    53,  32,  48,  44,  45,  53,  56,  46,  53,  51,  51,  54,  50,  32,  52,  57,  46,  54,  52,
    53,  50,  44,  45,  49,  48,  53,  46,  57,  56,  52,  52,  57,  32,  49,  49,  48,  46,  56,
    56,  53,  53,  55,  44,  45,  49,  48,  53,  46,  57,  56,  52,  52,  57,  32,  54,  49,  46,
    50,  52,  48,  51,  57,  44,  48,  32,  49,  49,  48,  46,  56,  56,  53,  53,  51,  44,  52,
    55,  46,  52,  53,  48,  56,  55,  32,  49,  49,  48,  46,  56,  56,  53,  53,  51,  44,  49,
    48,  53,  46,  57,  56,  52,  52,  57,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114, 58,
    35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  102, 102, 102, 102, 102,
    102, 59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105,
    108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116, 114,
    111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121,
    58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110,
    108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105, 98,
    108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117, 110,
    100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  53,  48,
    48,  45,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114,
    121, 61,  34,  49,  48,  53,  46,  57,  56,  52,  53,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  114, 120, 61,  34,  49,  49,  48,  46,  56,  56,  53,  53,
    51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  99,  121, 61,
    34,  53,  55,  49,  46,  55,  53,  54,  49,  54,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  99,  120, 61,  34,  45,  57,  57,  50,  46,  51,  49,  49,  56,
    51,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  60,  112, 97,
    116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114,
    118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114, 58,  35,
    48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,  48,  48,
    59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108,
    108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116, 114, 111,
    107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,
    118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105, 110, 108,
    105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105, 98,  108,
    101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117, 110, 100,
    58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  45,  57,  57,  50,  46,  51,  49,
    50,  53,  44,  53,  48,  49,  46,  55,  56,  51,  50,  32,  97,  32,  55,  51,  46,  50,  48,
    56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,  48,  32,  48,
    32,  45,  50,  46,  53,  57,  51,  55,  53,  44,  48,  46,  48,  56,  55,  57,  32,  108, 32,
    48,  44,  49,  51,  57,  46,  55,  51,  50,  52,  51,  32,  97,  32,  55,  51,  46,  50,  48,
    56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,  48,  32,  48,
    32,  50,  46,  53,  57,  51,  55,  53,  44,  48,  46,  49,  50,  53,  32,  55,  51,  46,  50,
    48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,  57,  32,  48,  32,  48,  32,
    48,  32,  55,  51,  46,  50,  48,  56,  57,  56,  44,  45,  54,  57,  46,  57,  55,  50,  54,
    54,  32,  55,  51,  46,  50,  48,  56,  57,  54,  49,  44,  54,  57,  46,  57,  55,  51,  49,
    57,  32,  48,  32,  48,  32,  48,  32,  45,  55,  51,  46,  50,  48,  56,  57,  56,  44,  45,
    54,  57,  46,  57,  55,  50,  54,  54,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  53,  48,  48,  45,
    54,  45,  57,  34,  32,  47,  62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  47,  103,
    62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  56,
    98,  48,  48,  48,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  103, 52,  53,  51,  53,  45,  57,  45,  54,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,
    116, 114, 105, 120, 40,  48,  46,  53,  52,  49,  53,  51,  53,  51,  57,  44,  48,  44,  48,
    44,  48,  46,  53,  52,  49,  53,  51,  53,  51,  57,  44,  49,  49,  48,  48,  46,  53,  57,
    54,  49,  44,  50,  51,  53,  46,  53,  55,  51,  54,  53,  41,  34,  62,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  60,  103, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  103, 52,  54,  48,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108,
    108, 58,  35,  48,  48,  48,  48,  48,  48,  34,  62,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110,
    110, 101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  112, 97,  116, 104, 52,  53,  48,  48,  45,  48,  45,  48,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  46,  56,  52,  54,  54,  48,  49,
    52,  44,  48,  44,  48,  44,  49,  46,  56,  52,  54,  54,  48,  49,  52,  44,  45,  50,  48,
    51,  50,  46,  51,  54,  50,  51,  44,  45,  52,  51,  53,  46,  48,  49,  48,  54,  51,  41,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,
    34,  109, 32,  53,  54,  51,  46,  50,  50,  52,  54,  49,  44,  52,  55,  50,  46,  54,  53,
    52,  51,  32,  99,  32,  45,  52,  49,  46,  57,  49,  56,  50,  50,  44,  50,  46,  53,  101,
    45,  52,  32,  45,  55,  53,  46,  56,  57,  57,  55,  44,  51,  50,  46,  52,  55,  57,  52,
    53,  32,  45,  55,  53,  46,  57,  48,  48,  51,  57,  44,  55,  50,  46,  53,  52,  52,  57,
    50,  32,  45,  52,  46,  52,  101, 45,  52,  44,  52,  48,  46,  48,  54,  54,  50,  51,  32,
    51,  51,  46,  57,  56,  49,  55,  56,  44,  55,  51,  46,  52,  54,  55,  55,  51,  32,  55,
    53,  46,  57,  48,  48,  55,  57,  44,  55,  51,  46,  52,  54,  55,  57,  56,  32,  52,  49,
    46,  57,  49,  57,  48,  49,  44,  45,  50,  46,  53,  101, 45,  52,  32,  55,  53,  46,  57,
    48,  48,  52,  51,  44,  45,  51,  51,  46,  52,  48,  49,  55,  53,  32,  55,  53,  46,  56,
    57,  57,  57,  57,  44,  45,  55,  51,  46,  52,  54,  55,  57,  56,  32,  45,  54,  46,  57,
    101, 45,  52,  44,  45,  52,  48,  46,  48,  54,  53,  52,  55,  32,  45,  51,  51,  46,  57,
    56,  50,  49,  55,  44,  45,  55,  50,  46,  53,  52,  52,  54,  55,  32,  45,  55,  53,  46,
    57,  48,  48,  51,  57,  44,  45,  55,  50,  46,  53,  52,  52,  57,  50,  32,  122, 32,  109,
    32,  45,  48,  46,  56,  49,  56,  51,  54,  44,  51,  52,  46,  54,  53,  50,  51,  52,  32,
    99,  32,  50,  49,  46,  56,  57,  53,  51,  53,  44,  48,  32,  51,  57,  46,  54,  52,  52,
    51,  54,  44,  49,  54,  46,  57,  54,  52,  57,  56,  32,  51,  57,  46,  54,  52,  52,  53,
    51,  44,  51,  55,  46,  56,  57,  50,  53,  56,  32,  45,  49,  46,  55,  101, 45,  52,  44,
    50,  48,  46,  57,  50,  55,  54,  32,  45,  49,  55,  46,  55,  52,  57,  49,  56,  44,  51,
    55,  46,  56,  57,  50,  53,  56,  32,  45,  51,  57,  46,  54,  52,  52,  53,  51,  44,  51,
    55,  46,  56,  57,  50,  53,  56,  32,  45,  53,  46,  57,  57,  50,  48,  54,  44,  45,  48,
    46,  48,  49,  52,  54,  32,  45,  50,  52,  46,  48,  53,  51,  55,  54,  44,  45,  48,  46,
    48,  51,  53,  57,  32,  45,  50,  52,  46,  53,  50,  49,  52,  56,  44,  45,  48,  46,  48,
    54,  54,  52,  32,  108, 32,  48,  44,  45,  49,  56,  46,  57,  49,  55,  57,  55,  32,  48,
    44,  45,  49,  56,  46,  57,  49,  55,  57,  55,  32,  48,  44,  45,  49,  56,  46,  57,  49,
    55,  57,  55,  32,  48,  44,  45,  49,  56,  46,  57,  49,  54,  48,  49,  32,  99,  32,  48,
    46,  52,  54,  55,  56,  51,  44,  45,  48,  46,  48,  50,  51,  56,  32,  50,  52,  46,  48,
    53,  51,  48,  50,  44,  45,  48,  46,  48,  52,  48,  57,  32,  50,  52,  46,  53,  50,  49,
    52,  56,  44,  45,  48,  46,  48,  52,  56,  56,  32,  122, 34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111,
    108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  48,
    48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111,
    59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105,
    108, 105, 116, 121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,
    121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118,
    105, 115, 105, 98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103,
    114, 111, 117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,
    62,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,  99,  117, 114,
    118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  53,  57,  52,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  100, 61,
    34,  109, 32,  45,  57,  56,  49,  46,  55,  57,  56,  55,  51,  44,  52,  51,  55,  46,  51,
    50,  48,  48,  51,  32,  45,  50,  49,  55,  46,  57,  55,  48,  50,  55,  44,  48,  32,  48,
    44,  51,  54,  53,  46,  54,  55,  54,  50,  50,  32,  49,  54,  48,  46,  53,  53,  44,  48,
    32,  48,  44,  45,  57,  53,  46,  53,  55,  52,  52,  56,  32,  52,  54,  46,  57,  48,  56,
    55,  56,  44,  45,  54,  46,  56,  101, 45,  52,  32,  52,  46,  48,  56,  57,  53,  44,  45,
    52,  56,  46,  55,  51,  48,  57,  54,  32,  45,  54,  57,  46,  56,  56,  54,  52,  56,  44,
    45,  49,  54,  46,  57,  57,  57,  52,  49,  32,  45,  53,  46,  54,  54,  54,  53,  44,  45,
    52,  57,  46,  56,  54,  52,  57,  52,  32,  54,  46,  55,  57,  57,  56,  44,  45,  49,  50,
    50,  46,  51,  57,  53,  55,  53,  32,  54,  57,  46,  53,  48,  56,  54,  57,  44,  45,  49,
    52,  46,  55,  51,  50,  56,  50,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,
    35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116,
    121, 58,  49,  59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110, 111,
    100, 100, 59,  115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 34,  32,  47,  62,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  32,  32,  32,  32,  60,  47,  103, 62,
    10,  32,  32,  32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116,
    111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  99,  111, 108, 111, 114,
    58,  35,  48,  48,  48,  48,  48,  48,  59,  102, 105, 108, 108, 58,  35,  48,  48,  48,  48,
    48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102,
    105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  115, 116,
    114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116,
    121, 58,  118, 105, 115, 105, 98,  108, 101, 59,  100, 105, 115, 112, 108, 97,  121, 58,  105,
    110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,  118, 105, 115, 105,
    98,  108, 101, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111, 117,
    110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  100, 61,  34,  109, 32,  50,  48,  55,  52,  46,  49,  56,  51,  54,
    44,  52,  54,  53,  46,  55,  55,  49,  52,  56,  32,  99,  32,  45,  54,  49,  46,  50,  52,
    48,  49,  44,  51,  46,  57,  101, 45,  52,  32,  45,  49,  49,  48,  46,  56,  56,  52,  55,
    44,  52,  55,  46,  52,  53,  49,  48,  56,  32,  45,  49,  49,  48,  46,  56,  56,  52,  56,
    44,  49,  48,  53,  46,  57,  56,  52,  51,  56,  32,  49,  48,  101, 45,  53,  44,  53,  56,
    46,  53,  51,  51,  51,  32,  52,  57,  46,  54,  52,  52,  56,  44,  49,  48,  53,  46,  57,
    56,  51,  57,  56,  32,  49,  49,  48,  46,  56,  56,  52,  56,  44,  49,  48,  53,  46,  57,
    56,  52,  51,  55,  32,  51,  50,  46,  52,  54,  57,  57,  44,  51,  46,  51,  101, 45,  52,
    32,  54,  53,  46,  48,  52,  52,  57,  44,  45,  57,  46,  55,  53,  55,  53,  49,  32,  56,
    56,  46,  52,  56,  54,  52,  44,  45,  50,  57,  46,  52,  51,  51,  53,  57,  32,  108, 32,
    48,  44,  45,  56,  53,  46,  55,  50,  51,  56,  50,  32,  45,  54,  50,  46,  49,  48,  55,
    44,  45,  48,  46,  49,  52,  52,  51,  52,  32,  48,  44,  51,  54,  46,  54,  52,  54,  55,
    50,  32,  50,  54,  46,  52,  56,  52,  44,  48,  32,  48,  44,  50,  54,  46,  51,  55,  53,
    55,  50,  32,  99,  32,  45,  49,  53,  46,  55,  53,  50,  53,  44,  49,  48,  46,  53,  57,
    55,  49,  54,  32,  45,  51,  52,  46,  48,  49,  55,  55,  44,  49,  53,  46,  53,  49,  53,
    57,  53,  32,  45,  53,  50,  46,  56,  54,  51,  52,  44,  49,  53,  46,  53,  49,  53,  57,
    53,  32,  45,  48,  46,  56,  54,  53,  51,  44,  45,  48,  46,  48,  50,  55,  32,  45,  49,
    46,  55,  51,  44,  45,  48,  46,  48,  54,  56,  55,  32,  45,  50,  46,  53,  57,  51,  56,
    44,  45,  48,  46,  49,  50,  53,  32,  108, 32,  48,  44,  45,  49,  51,  56,  46,  57,  56,
    48,  55,  49,  32,  99,  32,  48,  46,  56,  54,  52,  44,  45,  48,  46,  48,  52,  51,  57,
    32,  49,  46,  55,  50,  56,  55,  44,  45,  48,  46,  48,  55,  51,  50,  32,  50,  46,  53,
    57,  51,  56,  44,  45,  48,  46,  48,  56,  55,  57,  32,  50,  48,  46,  50,  52,  48,  55,
    44,  45,  49,  48,  101, 45,  54,  32,  52,  49,  46,  53,  54,  50,  49,  44,  55,  46,  52,
    54,  52,  55,  50,  32,  53,  55,  46,  56,  49,  48,  53,  44,  50,  48,  46,  48,  53,  56,
    54,  32,  108, 32,  50,  49,  46,  57,  57,  52,  50,  44,  45,  51,  49,  46,  49,  53,  50,
    51,  53,  32,  99,  32,  45,  50,  50,  46,  52,  54,  49,  44,  45,  49,  54,  46,  49,  52,
    55,  57,  55,  32,  45,  53,  49,  46,  49,  55,  51,  44,  45,  50,  52,  46,  57,  49,  56,
    50,  54,  32,  45,  55,  57,  46,  56,  48,  52,  55,  44,  45,  50,  52,  46,  57,  49,  55,
    57,  55,  32,  122, 34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    112, 97,  116, 104, 52,  53,  48,  48,  45,  48,  45,  56,  34,  32,  47,  62,  10,  32,  32,
    32,  32,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110, 101, 99,  116, 111, 114, 45,
    99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 58,  35,  48,  48,
    48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,
    115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  100, 61,  34,  109, 32,  50,  53,  50,  50,  46,  53,  57,  57,  49,  44,  53,
    56,  55,  46,  49,  48,  57,  50,  50,  32,  55,  55,  46,  51,  48,  53,  50,  44,  52,  55,
    46,  54,  55,  48,  48,  56,  32,  99,  32,  48,  44,  48,  32,  45,  49,  57,  46,  54,  55,
    57,  55,  44,  49,  48,  46,  51,  52,  54,  48,  55,  32,  45,  52,  52,  46,  54,  53,  48,
    52,  44,  52,  46,  50,  53,  49,  57,  53,  32,  45,  50,  53,  46,  50,  53,  52,  49,  44,
    45,  54,  46,  49,  54,  51,  50,  55,  32,  45,  52,  49,  46,  48,  49,  49,  52,  44,  45,
    50,  50,  46,  57,  52,  49,  50,  53,  32,  45,  52,  49,  46,  48,  49,  49,  52,  44,  45,
    50,  50,  46,  57,  52,  49,  50,  53,  32,  108, 32,  45,  50,  52,  46,  48,  52,  57,  49,
    44,  50,  56,  46,  52,  54,  56,  53,  57,  32,  99,  32,  53,  52,  46,  55,  51,  57,  51,
    44,  52,  55,  46,  48,  53,  55,  55,  51,  32,  49,  51,  51,  46,  56,  54,  50,  49,  44,
    51,  57,  46,  55,  57,  56,  55,  53,  32,  49,  54,  55,  46,  57,  54,  56,  55,  44,  45,
    57,  46,  55,  55,  57,  50,  57,  32,  49,  55,  46,  56,  52,  53,  56,  44,  45,  51,  48,
    46,  54,  49,  51,  53,  50,  32,  51,  46,  57,  56,  53,  54,  44,  45,  54,  53,  46,  51,
    53,  55,  57,  49,  32,  45,  49,  56,  46,  50,  53,  57,  44,  45,  56,  48,  46,  54,  54,
    55,  57,  56,  32,  108, 32,  45,  55,  55,  46,  53,  55,  57,  55,  44,  45,  52,  55,  46,
    56,  56,  51,  48,  52,  32,  99,  32,  50,  51,  46,  54,  57,  50,  51,  44,  45,  53,  46,
    56,  57,  54,  48,  57,  32,  52,  55,  46,  51,  56,  52,  55,  44,  45,  51,  46,  57,  55,
    53,  55,  55,  32,  55,  49,  46,  48,  55,  55,  44,  49,  48,  46,  52,  52,  49,  54,  52,
    32,  108, 32,  50,  48,  46,  50,  50,  50,  54,  44,  45,  50,  56,  46,  49,  49,  57,  49,
    52,  32,  99,  32,  45,  52,  56,  46,  57,  56,  48,  53,  44,  45,  51,  56,  46,  50,  49,
    53,  49,  55,  32,  45,  49,  51,  54,  46,  55,  51,  48,  52,  44,  45,  50,  53,  46,  49,
    57,  51,  53,  54,  32,  45,  49,  53,  54,  46,  50,  56,  55,  44,  51,  48,  46,  48,  49,
    48,  55,  52,  32,  45,  55,  46,  50,  51,  57,  50,  44,  49,  57,  46,  50,  53,  56,  53,
    53,  32,  49,  46,  51,  51,  53,  52,  44,  53,  51,  46,  55,  57,  50,  55,  53,  32,  50,
    53,  46,  50,  54,  51,  49,  44,  54,  56,  46,  53,  52,  55,  55,  32,  122, 34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 52,  54,  54,
    48,  34,  32,  47,  62,  10,  32,  32,  32,  32,  60,  47,  103, 62,  10,  32,  32,  60,  47,
    103, 62,  10,  32,  32,  60,  112, 97,  116, 104, 10,  32,  32,  32,  32,  32,  100, 61,  34,
    34,  10,  32,  32,  32,  32,  32,  99,  120, 61,  34,  45,  57,  57,  50,  46,  51,  49,  49,
    56,  51,  34,  10,  32,  32,  32,  32,  32,  99,  121, 61,  34,  53,  55,  49,  46,  55,  53,
    54,  49,  54,  34,  10,  32,  32,  32,  32,  32,  114, 120, 61,  34,  49,  49,  48,  46,  56,
    56,  53,  53,  51,  34,  10,  32,  32,  32,  32,  32,  114, 121, 61,  34,  49,  48,  53,  46,
    57,  56,  52,  53,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104,
    52,  53,  48,  48,  45,  52,  34,  10,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  99,  108, 105, 112,
    45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,  100, 105, 115, 112, 108,
    97,  121, 58,  105, 110, 108, 105, 110, 101, 59,  111, 118, 101, 114, 102, 108, 111, 119, 58,
    118, 105, 115, 105, 98,  108, 101, 59,  118, 105, 115, 105, 98,  105, 108, 105, 116, 121, 58,
    118, 105, 115, 105, 98,  108, 101, 59,  111, 112, 97,  99,  105, 116, 121, 58,  48,  46,  54,
    49,  49,  57,  57,  57,  57,  57,  59,  105, 115, 111, 108, 97,  116, 105, 111, 110, 58,  97,
    117, 116, 111, 59,  109, 105, 120, 45,  98,  108, 101, 110, 100, 45,  109, 111, 100, 101, 58,
    110, 111, 114, 109, 97,  108, 59,  99,  111, 108, 111, 114, 45,  105, 110, 116, 101, 114, 112,
    111, 108, 97,  116, 105, 111, 110, 58,  115, 82,  71,  66,  59,  99,  111, 108, 111, 114, 45,
    105, 110, 116, 101, 114, 112, 111, 108, 97,  116, 105, 111, 110, 45,  102, 105, 108, 116, 101,
    114, 115, 58,  108, 105, 110, 101, 97,  114, 82,  71,  66,  59,  115, 111, 108, 105, 100, 45,
    99,  111, 108, 111, 114, 58,  35,  48,  48,  48,  48,  48,  48,  59,  115, 111, 108, 105, 100,
    45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  102, 105, 108, 108, 58,  35,  56,  98,
    48,  48,  48,  48,  59,  102, 105, 108, 108, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,
    59,  102, 105, 108, 108, 45,  114, 117, 108, 101, 58,  110, 111, 110, 122, 101, 114, 111, 59,
    115, 116, 114, 111, 107, 101, 58,  110, 111, 110, 101, 59,  115, 116, 114, 111, 107, 101, 45,
    119, 105, 100, 116, 104, 58,  48,  46,  49,  59,  115, 116, 114, 111, 107, 101, 45,  108, 105,
    110, 101, 99,  97,  112, 58,  98,  117, 116, 116, 59,  115, 116, 114, 111, 107, 101, 45,  108,
    105, 110, 101, 106, 111, 105, 110, 58,  109, 105, 116, 101, 114, 59,  115, 116, 114, 111, 107,
    101, 45,  109, 105, 116, 101, 114, 108, 105, 109, 105, 116, 58,  52,  59,  115, 116, 114, 111,
    107, 101, 45,  100, 97,  115, 104, 97,  114, 114, 97,  121, 58,  110, 111, 110, 101, 59,  115,
    116, 114, 111, 107, 101, 45,  100, 97,  115, 104, 111, 102, 102, 115, 101, 116, 58,  48,  59,
    115, 116, 114, 111, 107, 101, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  59,  99,  111,
    108, 111, 114, 45,  114, 101, 110, 100, 101, 114, 105, 110, 103, 58,  97,  117, 116, 111, 59,
    105, 109, 97,  103, 101, 45,  114, 101, 110, 100, 101, 114, 105, 110, 103, 58,  97,  117, 116,
    111, 59,  115, 104, 97,  112, 101, 45,  114, 101, 110, 100, 101, 114, 105, 110, 103, 58,  97,
    117, 116, 111, 59,  116, 101, 120, 116, 45,  114, 101, 110, 100, 101, 114, 105, 110, 103, 58,
    97,  117, 116, 111, 59,  101, 110, 97,  98,  108, 101, 45,  98,  97,  99,  107, 103, 114, 111,
    117, 110, 100, 58,  97,  99,  99,  117, 109, 117, 108, 97,  116, 101, 34,  32,  47,  62,  10,
    60,  47,  115, 118, 103, 62,  10,  0,   0};

const char *monique_ui_Credits::monoplugs_png =
    (const char *)resource_monique_ui_Credits_monoplugs_png;
const int monique_ui_Credits::monoplugs_pngSize = 18152;

//[EndFile] You can add extra defines here...
//[/EndFile]
