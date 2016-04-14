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
#include "monique_core_Datastructures.h"
#include "monique_ui_MainWindow.h"
//[/Headers]

#include "monique_ui_Activate.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
struct Spinner  : public Component, private Timer
{
    Spinner()
    {
        startTimer (1000 / 50);
    }
    void timerCallback() override
    {
        repaint();
    }

    void paint (Graphics& g) override
    {
        getLookAndFeel().drawSpinningWaitAnimation (g, Colours::darkgrey, 0, 0, getWidth(), getHeight());
    }
};

struct monique_ui_Activate::OverlayComp :
    public Component,
    private Thread,
    private Timer
{
    OverlayComp (monique_ui_Activate& f)  : Thread (String()), form (f)
    {
        email = form.text_mail->getText();
        password = form.text_pw->getText();
        addAndMakeVisible (spinner);

        startThread (4);
    }

    ~OverlayComp()
    {
        stopThread (10000);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white.withAlpha (0.97f));

        g.setColour (Colours::black);
        g.setFont (15.0f);

        g.drawFittedText (TRANS("Contacting XYZ...").replace ("XYZ", SHARED::getInstance()->status.getWebsiteName()),
                          getLocalBounds().reduced (20, 0).removeFromTop (proportionOfHeight (0.6f)),
                          Justification::centred, 5);
    }

    void resized() override
    {
        const int spinnerSize = 40;
        spinner.setBounds ((getWidth() - spinnerSize) / 2, proportionOfHeight (0.6f), spinnerSize, spinnerSize);
    }

    void run() override
    {
        result = SHARED::getInstance()->status.attemptWebserverUnlock (email, password);
        startTimer (100);
    }

    void timerCallback() override
    {
        spinner.setVisible (false);
        stopTimer();

        if (result.errorMessage.isNotEmpty())
        {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              TRANS("Registration Failed"),
                                              result.errorMessage);
        }
        else
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                                             TRANS("Activation Complete!"),
                                             "Thanks again for purchase.\n"
                                             "\n"
                                             "Have a creative and productive time,\n"
                                             "Best Thomas");
        }

        if (result.urlToLaunch.isNotEmpty())
        {
            URL url (result.urlToLaunch);
            url.launchInDefaultBrowser();
        }

        // (local copies because we're about to delete this)
        const bool worked = result.succeeded;
        monique_ui_Activate& f = form;

        delete this;

        if (worked)
            f.close();
    }

    monique_ui_Activate& form;
    Spinner spinner;
    OnlineUnlockStatus::UnlockResult result;
    String email, password;
};

struct monique_ui_Activate::OfflineActivation : public Component
{
    OfflineActivation (monique_ui_Activate& f_) : f(f_)
    {
        // startThread (4);
        f.button_export->setVisible(true);
        f.button_import->setVisible(true);
        f.offline_text->setVisible(true);
        f.label_offline_header->setVisible(true);
        f.button_cancel->setVisible(true);

        f.button_activate_online->setVisible(false);
        f.label_sub_header->setVisible(false);
        f.label5->setVisible(false);
        f.button_activate_offline->setVisible(false);
        f.button_buy->setVisible(false);
        f.button_evaluate->setVisible(false);
        f.label2->setVisible(false);
        f.text_pw->setVisible(false);
        f.text_mail->setVisible(false);
    }

    ~OfflineActivation()
    {
        f.button_export->setVisible(false);
        f.button_import->setVisible(false);
        f.offline_text->setVisible(false);
        f.label_offline_header->setVisible(false);
        f.button_cancel->setVisible(false);

        f.button_activate_online->setVisible(true);
        f.label_sub_header->setVisible(true);
        f.label5->setVisible(true);
        f.button_activate_offline->setVisible(true);
        f.button_buy->setVisible(true);
        f.button_evaluate->setVisible(true);
        f.label2->setVisible(true);
        f.text_pw->setVisible(true);
        f.text_mail->setVisible(true);

        f.repaint();
    }

    monique_ui_Activate& f;
};

static juce_wchar getDefaultPasswordChar() noexcept
{
#if JUCE_LINUX
    return 0x2022;
#else
    return 0x25cf;
#endif
}
//[/MiscUserDefs]

//==============================================================================
monique_ui_Activate::monique_ui_Activate (Monique_Ui_Mainwindow*parent_)
    : DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point<int>(10,10)))
{
    //[Constructor_pre] You can add your own custom stuff here..
    activation_look_and_feel = new LookAndFeel_V3();
    parent = parent_;
    overlayOffline = nullptr;
    //[/Constructor_pre]

    addAndMakeVisible (offline_text = new TextEditor (String()));
    offline_text->setMultiLine (true);
    offline_text->setReturnKeyStartsNewLine (false);
    offline_text->setReadOnly (true);
    offline_text->setScrollbarsShown (false);
    offline_text->setCaretVisible (false);
    offline_text->setPopupMenuEnabled (true);
    offline_text->setColour (TextEditor::textColourId, Colour (0xffcbcbcb));
    offline_text->setColour (TextEditor::backgroundColourId, Colour (0xff000a12));
    offline_text->setText (TRANS("Please login to your Monoplugs web account on http://account.monoplugs.com/ with your order email address and the password you have received via mail after purchase.\n"
    "\n"
    "Go to Orders & Licenses -> Actvate new machine and enter the machine ID below:\n"
    "\n"
    "XYZ\n"
    "\n"
    "Important: you have to execute Monique on the computer on which you like to activate Monique for!\n"
    "\n"
    "After succesful activation you\'ll get a key-file which you can drop here or import it with the file browser."));

    addAndMakeVisible (label_offline_header = new Label ("new label",
                                                         TRANS("Offline Activation")));
    label_offline_header->setFont (Font (15.00f, Font::plain));
    label_offline_header->setJustificationType (Justification::centred);
    label_offline_header->setEditable (false, false, false);
    label_offline_header->setColour (Label::backgroundColourId, Colour (0xff000a12));
    label_offline_header->setColour (Label::textColourId, Colour (0xe3ffb500));
    label_offline_header->setColour (TextEditor::textColourId, Colours::black);
    label_offline_header->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_activate_online = new TextButton ("new button"));
    button_activate_online->setButtonText (TRANS("Activate Online (recommended)"));
    button_activate_online->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_activate_online->addListener (this);
    button_activate_online->setColour (TextButton::buttonColourId, Colour (0xe900bc36));

    addAndMakeVisible (label_header = new Label (String(),
                                                 TRANS("Activate Monique")));
    label_header->setFont (Font (Font::getDefaultSansSerifFontName(), 40.00f, Font::plain));
    label_header->setJustificationType (Justification::centred);
    label_header->setEditable (false, false, false);
    label_header->setColour (Label::textColourId, Colour (0xe3ffb500));
    label_header->setColour (TextEditor::textColourId, Colours::black);
    label_header->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sub_header = new Label (String(),
                                                     TRANS("Please insert your eMail and password to activate Monique on this computer.")));
    label_sub_header->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header->setJustificationType (Justification::centred);
    label_sub_header->setEditable (false, false, false);
    label_sub_header->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label_sub_header->setColour (TextEditor::textColourId, Colours::black);
    label_sub_header->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Don\'t have a Monique license?")));
    label5->setFont (Font (Font::getDefaultSansSerifFontName(), 20.00f, Font::plain));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_activate_offline = new TextButton (String()));
    button_activate_offline->setButtonText (TRANS("Activate Offline"));
    button_activate_offline->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_activate_offline->addListener (this);
    button_activate_offline->setColour (TextButton::buttonColourId, Colour (0xe900b5bc));

    addAndMakeVisible (button_buy = new TextButton ("new button"));
    button_buy->setButtonText (TRANS("Buy Monique"));
    button_buy->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_buy->addListener (this);
    button_buy->setColour (TextButton::buttonColourId, Colour (0xc0ffb500));

    addAndMakeVisible (button_evaluate = new TextButton ("new button"));
    button_evaluate->setButtonText (TRANS("Test Monique (free)"));
    button_evaluate->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_evaluate->addListener (this);
    button_evaluate->setColour (TextButton::buttonColourId, Colour (0xff240a6a));
    button_evaluate->setColour (TextButton::textColourOnId, Colours::white);
    button_evaluate->setColour (TextButton::textColourOffId, Colours::aqua);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Demo limits: project- and audio export is not supported")));
    label2->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (text_pw = new TextEditor (String()));
    text_pw->setMultiLine (false);
    text_pw->setReturnKeyStartsNewLine (false);
    text_pw->setReadOnly (false);
    text_pw->setScrollbarsShown (false);
    text_pw->setCaretVisible (true);
    text_pw->setPopupMenuEnabled (false);
    text_pw->setColour (TextEditor::backgroundColourId, Colour (0xc0cbcbcb));
    text_pw->setText (String());

    addAndMakeVisible (text_mail = new TextEditor (String()));
    text_mail->setMultiLine (false);
    text_mail->setReturnKeyStartsNewLine (false);
    text_mail->setReadOnly (false);
    text_mail->setScrollbarsShown (false);
    text_mail->setCaretVisible (true);
    text_mail->setPopupMenuEnabled (false);
    text_mail->setColour (TextEditor::backgroundColourId, Colour (0xc0cbcbcb));
    text_mail->setText (String());

    addAndMakeVisible (button_export = new TextButton (String()));
    button_export->setButtonText (TRANS("Export Machine ID to File"));
    button_export->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_export->addListener (this);
    button_export->setColour (TextButton::buttonColourId, Colour (0xe900b5bc));

    addAndMakeVisible (button_import = new TextButton (String()));
    button_import->setButtonText (TRANS("Import Key-File"));
    button_import->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_import->addListener (this);
    button_import->setColour (TextButton::buttonColourId, Colour (0xe900bc36));

    addAndMakeVisible (button_cancel = new TextButton (String()));
    button_cancel->setButtonText (TRANS("Cancel"));
    button_cancel->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    button_cancel->addListener (this);
    button_cancel->setColour (TextButton::buttonColourId, Colours::brown);

    addAndMakeVisible (label_header2 = new Label (String(),
                                                  TRANS("Need Help?")));
    label_header2->setFont (Font (Font::getDefaultSansSerifFontName(), 25.00f, Font::plain));
    label_header2->setJustificationType (Justification::centred);
    label_header2->setEditable (false, false, false);
    label_header2->setColour (Label::textColourId, Colour (0xe3ffb500));
    label_header2->setColour (TextEditor::textColourId, Colours::black);
    label_header2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sub_header2 = new Label (String(),
                                                      TRANS("Check out the Monoplugs Magazin for the latest video tutorials and news.")));
    label_sub_header2->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header2->setJustificationType (Justification::centred);
    label_sub_header2->setEditable (false, false, false);
    label_sub_header2->setColour (Label::textColourId, Colour (0xffcbcbcb));
    label_sub_header2->setColour (TextEditor::textColourId, Colours::black);
    label_sub_header2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sub_header3 = new Label (String(),
                                                      TRANS(">> http://Monoplugs.com/Magazin")));
    label_sub_header3->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain));
    label_sub_header3->setJustificationType (Justification::centred);
    label_sub_header3->setEditable (false, false, false);
    label_sub_header3->setColour (Label::textColourId, Colour (0xe000b5bc));
    label_sub_header3->setColour (TextEditor::textColourId, Colour (0xe900b5bc));
    label_sub_header3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (hyperlinkButton = new HyperlinkButton (String(),
                                                              URL ("http://Monoplugs.com/Magazin")));


    //[UserPreSize]
    //setOpaque (true);

    Colour found_colour = label_sub_header->findColour( Label::textColourId );
    Font style_font = label_offline_header->getFont();

    label_offline_header->setLookAndFeel(activation_look_and_feel);
    label_header->setLookAndFeel(activation_look_and_feel);
    label_sub_header->setLookAndFeel(activation_look_and_feel);
    label5->setLookAndFeel(activation_look_and_feel);
    label2->setLookAndFeel(activation_look_and_feel);
    text_pw->setLookAndFeel(activation_look_and_feel);
    text_mail->setLookAndFeel(activation_look_and_feel);
    label_header2->setLookAndFeel(activation_look_and_feel);
    label_sub_header2->setLookAndFeel(activation_look_and_feel);
    label_sub_header3->setLookAndFeel(activation_look_and_feel);
    hyperlinkButton->setTooltip("");

    label_header->setFont(style_font.withHeight(label_header->getFont().getHeight()));
    label_sub_header->setFont(style_font.withHeight(label_sub_header->getFont().getHeight()));
    label5->setFont(style_font.withHeight(label5->getFont().getHeight()));
    label2->setFont(style_font.withHeight(label2->getFont().getHeight()));

    button_activate_online->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_activate_offline->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_buy->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_evaluate->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    label_offline_header->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_import->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_export->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    button_cancel->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );

    //offline_text->setEnabled(false);

    button_import->setVisible(false);
    button_export->setVisible(false);
    offline_text->setVisible(false);
    label_offline_header->setVisible(false);
    button_cancel->setVisible(false);


    text_pw->setEscapeAndReturnKeysConsumed (false);
    text_mail->setEscapeAndReturnKeysConsumed (false);
    text_mail->setTextToShowWhenEmpty ("Email", Colours::grey.darker(0.5));
    text_pw->setTextToShowWhenEmpty ("Password", Colours::grey.darker(0.5));
    text_pw->setPasswordCharacter(getDefaultPasswordChar());

    String user_mail = SHARED::getInstance()->status.getUserEmail();
    if( user_mail != "" )
    {
        text_mail->setText ( user_mail, dontSendNotification);
    }

    button_activate_online->addShortcut (KeyPress (KeyPress::returnKey));
    //[/UserPreSize]

    setSize (540, 520);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

monique_ui_Activate::~monique_ui_Activate()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    offline_text = nullptr;
    label_offline_header = nullptr;
    button_activate_online = nullptr;
    label_header = nullptr;
    label_sub_header = nullptr;
    label5 = nullptr;
    button_activate_offline = nullptr;
    button_buy = nullptr;
    button_evaluate = nullptr;
    label2 = nullptr;
    text_pw = nullptr;
    text_mail = nullptr;
    button_export = nullptr;
    button_import = nullptr;
    button_cancel = nullptr;
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
    g.fillRect (30, 200 - (2 / 2), 480, 2);

    g.setColour (Colour (0x1bcbcbcb));
    g.fillRect (30, 297 - (2 / 2), 480, 2);

    g.setColour (Colour (0x1bcbcbcb));
    g.fillRect (30, 400 - (2 / 2), 480, 2);

    //[UserPaint] Add your own custom painting code here..
    if( overlay )
    {
        overlay->setBounds( getLocalBounds() );
    }
    //[/UserPaint]
}

void monique_ui_Activate::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    offline_text->setBounds (30, 90, 480, 290);
    label_offline_header->setBounds (30, 60, 480, 30);
    button_activate_online->setBounds (170, 150, 200, 30);
    label_header->setBounds (30, 10, 480, 60);
    label_sub_header->setBounds (30, 70, 480, 30);
    label5->setBounds (30, 210, 480, 30);
    button_activate_offline->setBounds (380, 150, 130, 30);
    button_buy->setBounds (100, 250, 340, 30);
    button_evaluate->setBounds (100, 320, 340, 30);
    label2->setBounds (30, 358, 480, 25);
    text_pw->setBounds (280, 110, 230, 25);
    text_mail->setBounds (30, 110, 230, 25);
    button_export->setBounds (240, 340, 200, 30);
    button_import->setBounds (30, 340, 200, 30);
    button_cancel->setBounds (450, 340, 60, 30);
    label_header2->setBounds (30, 410, 480, 40);
    label_sub_header2->setBounds (30, 450, 480, 30);
    label_sub_header3->setBounds (30, 470, 480, 30);
    hyperlinkButton->setBounds (30, 470, 480, 30);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void monique_ui_Activate::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_activate_online)
    {
        //[UserButtonCode_button_activate_online] -- add your button handler code here..
        if ( text_mail->getText().trim().length() < 3)
        {
            showBubbleMessage (TRANS ("Please enter a valid email address!"), *text_mail);
            return;
        }

        if (text_pw->getText().trim().length() < 3)
        {
            showBubbleMessage (TRANS ("Please enter a valid password!"), *text_pw);
            return;
        }
        {
            addAndMakeVisible (overlay = new OverlayComp (*this));
        }
        //[/UserButtonCode_button_activate_online]
    }
    else if (buttonThatWasClicked == button_activate_offline)
    {
        //[UserButtonCode_button_activate_offline] -- add your button handler code here..
        overlayOffline = new OfflineActivation(*this);
	String text = offline_text->getText();
	text = text.replace( "XYZ", get_state() );
	offline_text->setText( text, dontSendNotification);
        //[/UserButtonCode_button_activate_offline]
    }
    else if (buttonThatWasClicked == button_buy)
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
    else if (buttonThatWasClicked == button_export)
    {
        //[UserButtonCode_button_export] -- add your button handler code here..
        FileChooser fc( "Export Machine ID", File( File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory) ), "*.mpexp" );
        if(fc.browseForFileToSave(true))
        {
            File file( fc.getResult() );
            file.replaceWithText( get_state() );
        }
        //[/UserButtonCode_button_export]
    }
    else if (buttonThatWasClicked == button_import)
    {
        //[UserButtonCode_button_import] -- add your button handler code here..
        FileChooser fc( "Import Key File", File( File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory) ), "*.mpkey" );
        if(fc.browseForFileToOpen())
        {
            if( not SHARED::getInstance()->status.applyKeyFile(fc.getResult().loadFileAsString()) )
            {
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  TRANS("Activation Failed"),
                                                  "Something is wrong with the keyfile. If this happens again please contact our support: support@monoplugs.com");
            }
            else
            {
                AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                                  TRANS("Activation Complete!"),
                                                  "Thanks again for purchase.\n"
                                                  "\n"
                                                  "Have a creative and productive time,\n"
                                                  "Best Thomas");

                close();
                return;
            }
        }
        //[/UserButtonCode_button_import]
    }
    else if (buttonThatWasClicked == button_cancel)
    {
        //[UserButtonCode_button_cancel] -- add your button handler code here..
        if( overlayOffline )
        {
            delete overlayOffline;
            overlayOffline = nullptr;
        }
        //[/UserButtonCode_button_cancel]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
String monique_ui_Activate::get_state() noexcept
{
    static String string;
    if( string == "" )
    {
        string += SHARED::getInstance()->status.get_system_code();
        string += SHARED::getInstance()->status.getProductID();
        string += SHARED::getInstance()->status.getLocalMachineIDs()[0];
    }

    /*
    if( string == "" )
    {
        string += OnlineUnlockStatus::MachineIDUtilities::getPlatformPrefix();
        string += "M"; // MONIQUE
        string += SHARED::getInstance()->status.getLocalMachineIDs()[0];
    }
    */

    return string;
}
void monique_ui_Activate::close() noexcept
{
    parent->activation_window = nullptr;
}
void monique_ui_Activate::showBubbleMessage (const String& text, Component& target)
{
    bubble = new BubbleMessageComponent (1500);
    addChildComponent (bubble);

    AttributedString attString;
    attString.append (text, Font (16.0f));

    bubble->showAt (getLocalArea (&target, target.getLocalBounds()),
                    attString, 300,  // numMillisecondsBeforeRemoving
                    true,  // removeWhenMouseClicked
                    false); // deleteSelfAfterUse
}

bool monique_ui_Activate::isInterestedInFileDrag (const StringArray& files)
{
    return files[0].endsWith("mpkey");
}
void monique_ui_Activate::fileDragEnter (const StringArray& files, int x, int y)
{
    if( files[0].endsWith("mpkey") )
    {

    }
}
void monique_ui_Activate::fileDragExit (const StringArray& files )
{
}
void monique_ui_Activate::filesDropped (const StringArray& files, int x, int y)
{
    File file(files[0]);
   // std::cout << files[0] << std::endl;
    if( not SHARED::getInstance()->status.applyKeyFile(file.loadFileAsString()) )
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          TRANS("Activation Failed"),
                                          "Something is wrong with the keyfile. If this happens again please contact our support: support@monoplugs.com");
    }
    else
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          TRANS("Activation Complete!"),
                                          "Thanks again for purchase.\n"
                                          "\n"
                                          "Have a creative and productive time,\n"
                                          "Best Thomas");

        close();
        return;
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="monique_ui_Activate" componentName=""
                 parentClasses="public Component, public DropShadower, public FileDragAndDropTarget"
                 constructorParams="Monique_Ui_Mainwindow*parent_" variableInitialisers="DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10)))"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="540" initialHeight="520">
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="3 3 6M 6M" cornerSize="1" fill="solid: ff000a12" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ff00ffff"/>
    <RECT pos="30 200c 480 2" fill="solid: 1bcbcbcb" hasStroke="0"/>
    <RECT pos="30 297c 480 2" fill="solid: 1bcbcbcb" hasStroke="0"/>
    <RECT pos="30 400c 480 2" fill="solid: 1bcbcbcb" hasStroke="0"/>
  </BACKGROUND>
  <TEXTEDITOR name="" id="2bff6e0b0ad0ded7" memberName="offline_text" virtualName=""
              explicitFocusOrder="0" pos="30 90 480 290" textcol="ffcbcbcb"
              bkgcol="ff000a12" initialText="Please login to your Monoplugs web account on http://account.monoplugs.com/ with your order email address and the password you have received via mail after purchase.&#10;&#10;Go to Orders &amp; Licenses -&gt; Actvate new machine and enter the machine ID below:&#10;&#10;XYZ&#10;&#10;Important: you have to execute Monique on the computer on which you like to activate Monique for!&#10;&#10;After succesful activation you'll get a key-file which you can drop here or import it with the file browser."
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="1"/>
  <LABEL name="new label" id="976adf11950f85fe" memberName="label_offline_header"
         virtualName="" explicitFocusOrder="0" pos="30 60 480 30" bkgCol="ff000a12"
         textCol="e3ffb500" edTextCol="ff000000" edBkgCol="0" labelText="Offline Activation"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="e006bb04d696b756" memberName="button_activate_online"
              virtualName="" explicitFocusOrder="0" pos="170 150 200 30" bgColOff="e900bc36"
              buttonText="Activate Online (recommended)" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="213ce816bd0c8d7e" memberName="label_header" virtualName=""
         explicitFocusOrder="0" pos="30 10 480 60" textCol="e3ffb500"
         edTextCol="ff000000" edBkgCol="0" labelText="Activate Monique"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="40" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="846c760ea14787f7" memberName="label_sub_header" virtualName=""
         explicitFocusOrder="0" pos="30 70 480 30" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Please insert your eMail and password to activate Monique on this computer."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="new label" id="a5ea115723f64cb7" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="30 210 480 30" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Don't have a Monique license?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="20" bold="0" italic="0"
         justification="36"/>
  <TEXTBUTTON name="" id="1143656a3d5b30a5" memberName="button_activate_offline"
              virtualName="" explicitFocusOrder="0" pos="380 150 130 30" bgColOff="e900b5bc"
              buttonText="Activate Offline" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e594b0617fd9b5ad" memberName="button_buy"
              virtualName="" explicitFocusOrder="0" pos="100 250 340 30" bgColOff="c0ffb500"
              buttonText="Buy Monique" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5ac0f6c8ceef7e31" memberName="button_evaluate"
              virtualName="" explicitFocusOrder="0" pos="100 320 340 30" bgColOff="ff240a6a"
              textCol="ffffffff" textColOn="ff00ffff" buttonText="Test Monique (free)"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="8cc8bfeb95ae386b" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="30 358 480 25" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Demo limits: project- and audio export is not supported"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <TEXTEDITOR name="" id="23a5a7ec8974c841" memberName="text_pw" virtualName=""
              explicitFocusOrder="0" pos="280 110 230 25" bkgcol="c0cbcbcb"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="0"/>
  <TEXTEDITOR name="" id="b239fdb662f48410" memberName="text_mail" virtualName=""
              explicitFocusOrder="0" pos="30 110 230 25" bkgcol="c0cbcbcb"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="0"/>
  <TEXTBUTTON name="" id="3d6ca91e42e6c493" memberName="button_export" virtualName=""
              explicitFocusOrder="0" pos="240 340 200 30" bgColOff="e900b5bc"
              buttonText="Export Machine ID to File" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="1c6849e4ff063b3f" memberName="button_import" virtualName=""
              explicitFocusOrder="0" pos="30 340 200 30" bgColOff="e900bc36"
              buttonText="Import Key-File" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="a1675b4c7b84b1ad" memberName="button_cancel" virtualName=""
              explicitFocusOrder="0" pos="450 340 60 30" bgColOff="ffa52a2a"
              buttonText="Cancel" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="c3bc7766a1daf17c" memberName="label_header2" virtualName=""
         explicitFocusOrder="0" pos="30 410 480 40" textCol="e3ffb500"
         edTextCol="ff000000" edBkgCol="0" labelText="Need Help?" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="25" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="bf2a6a1ce787bee6" memberName="label_sub_header2"
         virtualName="" explicitFocusOrder="0" pos="30 450 480 30" textCol="ffcbcbcb"
         edTextCol="ff000000" edBkgCol="0" labelText="Check out the Monoplugs Magazin for the latest video tutorials and news."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <LABEL name="" id="19160f6a88a37f0f" memberName="label_sub_header3"
         virtualName="" explicitFocusOrder="0" pos="30 470 480 30" textCol="e000b5bc"
         edTextCol="e900b5bc" edBkgCol="0" labelText="&gt;&gt; http://Monoplugs.com/Magazin"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="15" bold="0" italic="0"
         justification="36"/>
  <HYPERLINKBUTTON name="" id="9b5f8b1bc06606d4" memberName="hyperlinkButton" virtualName=""
                   explicitFocusOrder="0" pos="30 470 480 30" buttonText="" connectedEdges="0"
                   needsCallback="0" radioGroupId="0" url="http://Monoplugs.com/Magazin"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
