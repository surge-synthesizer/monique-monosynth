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
#include "monique_ui_ModulationSlider.h"

#include "monique_core_Processor.h"
//[/Headers]

#include "monique_ui_ENVPopup.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_ENVPopup::refresh() noexcept
{
    if( last_attack != env_data->attack.get_value()
    or last_sustain != env_data->sustain.get_value()
    or last_decay != env_data->decay.get_value()
    or sustain_time != env_data->sustain_time.get_value()
    or last_release != env_data->release.get_value()
    or last_shape != env_data->shape.get_value()
      )
    {
        last_attack = env_data->attack.get_value();
        last_sustain = env_data->sustain.get_value();
        last_decay = env_data->decay.get_value();
        sustain_time = env_data->sustain_time.get_value();
        last_shape = env_data->shape.get_value();
        last_release = env_data->release.get_value();

        slider_attack->setValue( env_data->attack.get_value(), dontSendNotification );
        label_attack->setText(String( MIN_ENV_TIMES + slider_attack->getValue()*20000 )+String("ms"), dontSendNotification);

        slider_decay->setValue( env_data->decay.get_value(), dontSendNotification );
        label_decay->setText(String( slider_decay->getValue()*20000 )+String("ms"), dontSendNotification);

        slider_sustain->setValue( sustain->get_value(), dontSendNotification );
        label_sustain->setText(String( slider_sustain->getValue()*100 ), dontSendNotification);

        slider_sustain_time->setValue( env_data->sustain_time.get_value()*10000, dontSendNotification );
        label_sustain_time->setText(String( round0(slider_sustain_time->getValue()) )+String("ms"), dontSendNotification);

        slider_release->setValue( env_data->release.get_value(), dontSendNotification );
        label_release->setText(String( MIN_ENV_TIMES + slider_release->getValue() *20000)+String("ms"), dontSendNotification);

        slider_env_shape->setValue( env_data->shape.get_value(), dontSendNotification );

        repaint();
    }

    {
        ComponentColours& colours = UiLookAndFeel::getInstance()->colours;
        Colour button_off = colours.button_off_colour;
        auto_close->setColour (TextButton::buttonColourId, GET_DATA( synth_data ).auto_close_env_popup ? Colours::yellow : button_off );
        keep->setColour (TextButton::buttonColourId, GET_DATA( synth_data ).auto_switch_env_popup ? Colours::green : button_off );
    }
}

void Monique_Ui_ENVPopup::set_element_to_show( Component*const comp_, Monique_Ui_DualSlider*owner_ )
{
    owner_slider = owner_;
    related_to_comp = comp_;
    Component* parent = comp_->getParentComponent();
    int x = comp_->getX();
    int y = comp_->getY();

    while( parent )
    {
        if( parent->getParentComponent() ) // IGNORES THE MAIN WINDOW
        {
            x += parent->getX();
            y += parent->getY();
        }
        parent = parent->getParentComponent();
    }
    const float width_scale = 1.0f/original_w*getWidth();
    const float height_scale = 1.0f/original_h*getHeight();
    const int left_move = not left ? getWidth() - (width_scale*80) + (width_scale*10) : (width_scale*10);
    setBounds( x-left_move, y, getWidth(), getHeight() );
}
void Monique_Ui_ENVPopup::update_positions(  )
{
    if( related_to_comp )
    {
        set_element_to_show( related_to_comp, owner_slider );
    }
}
void Monique_Ui_ENVPopup::set_clickable_components( Array<Component*>& comps_ ) noexcept
{
    observed_comps = comps_;
    for( int i = 0 ; i != observed_comps.size() ; ++i )
    {
        Component*comp = observed_comps.getUnchecked(i);
        if( Monique_Ui_Mainwindow*mainwin = dynamic_cast< Monique_Ui_Mainwindow* >( comp ) )
        {
            comp->addMouseListener(this,false);
        }
        else
        {
            comp->addMouseListener(this,true);
        }
    }
}
void Monique_Ui_ENVPopup::mouseDown (const MouseEvent& event)
{
    if(Monique_Ui_Mainwindow* mainwindow = AppInstanceStore::getInstance()->editor)
    {
        bool success = false;
        Component*const event_comp = event.eventComponent;
        Component* comp = event_comp;
        if( comp != this )
        {
            while( comp )
            {
                if( Monique_Ui_DualSlider*dual_slider = dynamic_cast< Monique_Ui_DualSlider* >( comp ) )
                {
                    const bool keeps_open = mainwindow->handle_keep_env_open( dual_slider->_config );
                    success = true;
                    if( keeps_open and GET_DATA( synth_data ).auto_switch_env_popup )
                    {
                        if( owner_slider != dual_slider and owner_slider )
                        {
                            mainwindow->open_env_popup(dual_slider);
                            return;
                        }
                    }

                    break;
                }
                comp = comp->getParentComponent();
            }
            if( not success and GET_DATA( synth_data ).auto_close_env_popup )
            {
                mainwindow->open_env_popup(nullptr,nullptr,nullptr,nullptr,false);
            }
        }
    }
}
void Monique_Ui_ENVPopup::mouseDrag (const MouseEvent& event)
{
    mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseUp (const MouseEvent& event)
{
    //mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseDoubleClick (const MouseEvent& event)
{
    mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& )
{
    mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseMagnify (const MouseEvent& event, float )
{
    mouseDown(event);
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_ENVPopup::Monique_Ui_ENVPopup (Monique_Ui_Mainwindow*const parent_, ENVData*const env_data_, Parameter*const sustain_, bool left_, bool has_negative_sustain_)
    : parent(parent_),env_data(env_data_),sustain(sustain_),original_w(710), original_h(190),left(left_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    last_attack = 0;
    last_sustain= 0;
    last_decay= 0;
    last_release= 0;
    last_shape= 0;
    sustain_time= 0;
    owner_slider = nullptr;
    //[/Constructor_pre]

    addAndMakeVisible (label_attack_bottom = new Label (String::empty,
                                                        TRANS("ATTACK")));
    label_attack_bottom->setFont (Font (15.00f, Font::plain));
    label_attack_bottom->setJustificationType (Justification::centred);
    label_attack_bottom->setEditable (false, false, false);
    label_attack_bottom->setColour (Label::textColourId, Colours::yellow);
    label_attack_bottom->setColour (TextEditor::textColourId, Colours::black);
    label_attack_bottom->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_attack = new Slider ("0"));
    slider_attack->setRange (0, 1, 5e-05);
    slider_attack->setSliderStyle (Slider::LinearVertical);
    slider_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_attack->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_attack->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_attack->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_attack->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_attack->addListener (this);

    addAndMakeVisible (label_decay_bottom = new Label (String::empty,
                                                       TRANS("DECAY")));
    label_decay_bottom->setFont (Font (15.00f, Font::plain));
    label_decay_bottom->setJustificationType (Justification::centred);
    label_decay_bottom->setEditable (false, false, false);
    label_decay_bottom->setColour (Label::textColourId, Colours::yellow);
    label_decay_bottom->setColour (TextEditor::textColourId, Colours::black);
    label_decay_bottom->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_decay = new Slider ("0"));
    slider_decay->setRange (0, 1, 5e-05);
    slider_decay->setSliderStyle (Slider::LinearVertical);
    slider_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_decay->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_decay->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_decay->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_decay->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_decay->addListener (this);

    addAndMakeVisible (label_release_bottom = new Label (String::empty,
                                                         TRANS("RELEASE")));
    label_release_bottom->setFont (Font (15.00f, Font::plain));
    label_release_bottom->setJustificationType (Justification::centred);
    label_release_bottom->setEditable (false, false, false);
    label_release_bottom->setColour (Label::textColourId, Colours::yellow);
    label_release_bottom->setColour (TextEditor::textColourId, Colours::black);
    label_release_bottom->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_release = new Slider ("0"));
    slider_release->setRange (0, 1, 5e-05);
    slider_release->setSliderStyle (Slider::LinearVertical);
    slider_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_release->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_release->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_release->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_release->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_release->addListener (this);

    addAndMakeVisible (slider_sustain_time = new Slider ("0"));
    slider_sustain_time->setRange (0, 10000, 1);
    slider_sustain_time->setSliderStyle (Slider::LinearVertical);
    slider_sustain_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain_time->addListener (this);

    addAndMakeVisible (label_sustain_time_bottom = new Label (String::empty,
                                                              TRANS("SUS TIME")));
    label_sustain_time_bottom->setFont (Font (15.00f, Font::plain));
    label_sustain_time_bottom->setJustificationType (Justification::centred);
    label_sustain_time_bottom->setEditable (false, false, false);
    label_sustain_time_bottom->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time_bottom->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time_bottom->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_attack = new Label ("VL",
                                                 TRANS("x\n")));
    label_attack->setFont (Font (15.00f, Font::plain));
    label_attack->setJustificationType (Justification::centred);
    label_attack->setEditable (false, false, false);
    label_attack->setColour (Label::textColourId, Colours::yellow);
    label_attack->setColour (TextEditor::textColourId, Colours::black);
    label_attack->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_decay = new Label ("VL",
                                                TRANS("x\n")));
    label_decay->setFont (Font (15.00f, Font::plain));
    label_decay->setJustificationType (Justification::centred);
    label_decay->setEditable (false, false, false);
    label_decay->setColour (Label::textColourId, Colours::yellow);
    label_decay->setColour (TextEditor::textColourId, Colours::black);
    label_decay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain_time = new Label ("VL",
                                                       TRANS("x\n")));
    label_sustain_time->setFont (Font (15.00f, Font::plain));
    label_sustain_time->setJustificationType (Justification::centred);
    label_sustain_time->setEditable (false, false, false);
    label_sustain_time->setColour (Label::textColourId, Colours::yellow);
    label_sustain_time->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_time->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_release = new Label ("VL",
                                                  TRANS("x\n")));
    label_release->setFont (Font (15.00f, Font::plain));
    label_release->setJustificationType (Justification::centred);
    label_release->setEditable (false, false, false);
    label_release->setColour (Label::textColourId, Colours::yellow);
    label_release->setColour (TextEditor::textColourId, Colours::black);
    label_release->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_sustain = new Slider ("0"));
    slider_sustain->setRange (0, 1, 0.001);
    slider_sustain->setSliderStyle (Slider::LinearVertical);
    slider_sustain->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sustain->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sustain->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sustain->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sustain->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sustain->addListener (this);

    addAndMakeVisible (label_sustain_bottom = new Label (String::empty,
                                                         TRANS("(SUSTAIN)")));
    label_sustain_bottom->setFont (Font (15.00f, Font::plain));
    label_sustain_bottom->setJustificationType (Justification::centred);
    label_sustain_bottom->setEditable (false, false, false);
    label_sustain_bottom->setColour (Label::textColourId, Colours::yellow);
    label_sustain_bottom->setColour (TextEditor::textColourId, Colours::black);
    label_sustain_bottom->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_sustain = new Label ("VL",
                                                  TRANS("x\n")));
    label_sustain->setFont (Font (15.00f, Font::plain));
    label_sustain->setJustificationType (Justification::centred);
    label_sustain->setEditable (false, false, false);
    label_sustain->setColour (Label::textColourId, Colours::yellow);
    label_sustain->setColour (TextEditor::textColourId, Colours::black);
    label_sustain->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (plotter = new Component());

    addAndMakeVisible (slider_env_shape = new Slider ("0"));
    slider_env_shape->setTooltip (TRANS("Define the ADSR curve type. This is a global paraneter and will be used for all envelopes (includes FILTER ADSR\'s and MAIN ADSR)."));
    slider_env_shape->setRange (0, 1, 0.01);
    slider_env_shape->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_env_shape->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_env_shape->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_env_shape->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_env_shape->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_env_shape->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_env_shape->addListener (this);

    addAndMakeVisible (label_shape = new Label ("new label",
                                                TRANS("SHAPE")));
    label_shape->setFont (Font (15.00f, Font::plain));
    label_shape->setJustificationType (Justification::centred);
    label_shape->setEditable (false, false, false);
    label_shape->setColour (Label::textColourId, Colours::yellow);
    label_shape->setColour (TextEditor::textColourId, Colours::black);
    label_shape->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (close = new TextButton (String::empty));
    close->setTooltip (TRANS("Close this pop up. \n"
    "(ESC is your friend)"));
    close->setButtonText (TRANS("X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (keep = new TextButton (String::empty));
    keep->setTooltip (TRANS("OPTION: auto switch this pop up to its siblings on any mouse action at a sibling.\n"
    "(e.g. from one OSC input to another one of the same filter)"));
    keep->setButtonText (TRANS("S"));
    keep->addListener (this);
    keep->setColour (TextButton::buttonColourId, Colours::green);
    keep->setColour (TextButton::buttonOnColourId, Colours::green);
    keep->setColour (TextButton::textColourOnId, Colours::black);
    keep->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (auto_close = new TextButton (String::empty));
    auto_close->setTooltip (TRANS("OPTION: auto close this popup on any unrelated mouse action.\n"
    "(e.g. click the main user interface)"));
    auto_close->setButtonText (TRANS("C"));
    auto_close->addListener (this);
    auto_close->setColour (TextButton::buttonColourId, Colours::yellow);
    auto_close->setColour (TextButton::buttonOnColourId, Colours::yellow);
    auto_close->setColour (TextButton::textColourOnId, Colours::black);
    auto_close->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]
    related_to_comp = nullptr;
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setRepaintsOnMouseActivity(false);
        getChildComponent(i)->setWantsKeyboardFocus(false);

        if( Slider* slider = dynamic_cast<Slider*>(getChildComponent(i)) )
        {
            SET_SLIDER_STYLE( slider, VALUE_SLIDER );
            slider->setOpaque(true);
        }
    }

    if( has_negative_sustain_ )
    {
        slider_sustain->setRange (-1, 1, 0.0005);
    }

    //[/UserPreSize]

    setSize (710, 190);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Monique_Ui_ENVPopup::~Monique_Ui_ENVPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    for( int i = 0 ; i != observed_comps.size() ; ++i )
    {
        Component*comp = observed_comps.getUnchecked(i);
        comp->removeMouseListener(this);
    }
    //[/Destructor_pre]

    label_attack_bottom = nullptr;
    slider_attack = nullptr;
    label_decay_bottom = nullptr;
    slider_decay = nullptr;
    label_release_bottom = nullptr;
    slider_release = nullptr;
    slider_sustain_time = nullptr;
    label_sustain_time_bottom = nullptr;
    label_attack = nullptr;
    label_decay = nullptr;
    label_sustain_time = nullptr;
    label_release = nullptr;
    slider_sustain = nullptr;
    label_sustain_bottom = nullptr;
    label_sustain = nullptr;
    plotter = nullptr;
    slider_env_shape = nullptr;
    label_shape = nullptr;
    close = nullptr;
    keep = nullptr;
    auto_close = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_ENVPopup::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1.0f, 10.0f, 708.0f, 179.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 708.0f, 179.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_END.h"
    {
        curve.clearQuick();
        curve.ensureStorageAllocated( 50000 );

        mono_ParameterOwnerStore::get_full_adstr( *env_data, curve );

        int plotter_x = plotter->getX();
        int plotter_y = plotter->getY();
        int plotter_width = plotter->getWidth();
        int plotter_hight = plotter->getHeight();

        {
            Colour colour = Colour(0xff222222 );
            g.setGradientFill (ColourGradient (colour.darker (0.3f), 0.0f, 0.0f, Colour (0xff050505), 0.0f, plotter_hight, false));
            //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f, height, false));
            g.fillRoundedRectangle (plotter_x, plotter_y, plotter_width, plotter_hight, 3);

            g.setColour (colour.darker (0.6f));
            g.drawRoundedRectangle (plotter_x, plotter_y, plotter_width, plotter_hight, 3, 1);
        }

        const int curve_size = curve.size();
        float scale_w = float(plotter_width)/(curve.size());

        int last_x = -1;
        int last_y = -1;
        Colour col( UiLookAndFeel::getInstance()->colours.slider_track_colour );
        for( int i = 0 ; i != curve_size ; ++i )
        {
            float value = 1.0f-curve[i];
            const int x = scale_w*i+plotter_x;
            const int y = value*plotter_hight+plotter_y;
            if( last_x != x || last_y != y )
            {
                last_y = y;
                g.setColour (col);
                g.fillRect (last_x, last_y, 1, 1);

                if( last_x != x )
                {
                    g.setColour (col.withAlpha(0.1f));
                    g.fillRect (last_x, last_y, 1, int((1.0f-value)*plotter_hight));
                }
                last_x = x;
            }
        }
    }
    //[/UserPaint]
}

void Monique_Ui_ENVPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"

    const int left_move = not left ? original_w - 50-30 : 0;

    label_attack_bottom->setBounds (20, 140, 60, 33);
    slider_attack->setBounds (20, 60, 60, 80);
    label_decay_bottom->setBounds (80, 140, 60, 33);
    slider_decay->setBounds (80, 60, 60, 80);
    label_release_bottom->setBounds (260, 140, 60, 33);
    slider_release->setBounds (260, 60, 60, 80);
    slider_sustain_time->setBounds (200, 60, 60, 80);
    label_sustain_time_bottom->setBounds (200, 140, 60, 33);
    label_attack->setBounds (20, 20, 60, 33);
    label_decay->setBounds (80, 20, 60, 33);
    label_sustain_time->setBounds (200, 20, 60, 33);
    label_release->setBounds (260, 20, 60, 33);
    slider_sustain->setBounds (140, 60, 60, 80);
    label_sustain_bottom->setBounds (140, 140, 60, 33);
    label_sustain->setBounds (140, 20, 60, 33);
    plotter->setBounds (410, 20, 260, 159);
    slider_env_shape->setBounds (340, 60, 60, 80);
    label_shape->setBounds (340, 140, 60, 33);
    close->setBounds (680, 20, 20, 20);
    keep->setBounds (680, 160, 20, 20);
    auto_close->setBounds (680, 130, 20, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f+left_move, 0.0f);
    internalPath1.lineTo (50.0f+left_move, 10.0f);
    internalPath1.lineTo (30.0f+left_move, 10.0f);
    internalPath1.closeSubPath();
    /*
    //[/UserPreResize]

    label_attack_bottom->setBounds (20, 140, 60, 33);
    slider_attack->setBounds (20, 60, 60, 80);
    label_decay_bottom->setBounds (80, 140, 60, 33);
    slider_decay->setBounds (80, 60, 60, 80);
    label_release_bottom->setBounds (260, 140, 60, 33);
    slider_release->setBounds (260, 60, 60, 80);
    slider_sustain_time->setBounds (200, 60, 60, 80);
    label_sustain_time_bottom->setBounds (200, 140, 60, 33);
    label_attack->setBounds (20, 20, 60, 33);
    label_decay->setBounds (80, 20, 60, 33);
    label_sustain_time->setBounds (200, 20, 60, 33);
    label_release->setBounds (260, 20, 60, 33);
    slider_sustain->setBounds (140, 60, 60, 80);
    label_sustain_bottom->setBounds (140, 140, 60, 33);
    label_sustain->setBounds (140, 20, 60, 33);
    plotter->setBounds (410, 20, 260, 159);
    slider_env_shape->setBounds (340, 60, 60, 80);
    label_shape->setBounds (340, 140, 60, 33);
    close->setBounds (680, 20, 20, 20);
    keep->setBounds (680, 160, 20, 20);
    auto_close->setBounds (680, 130, 20, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f, 0.0f);
    internalPath1.lineTo (50.0f, 10.0f);
    internalPath1.lineTo (30.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
      */
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_ENVPopup::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_attack)
    {
        //[UserSliderCode_slider_attack] -- add your slider handling code here..
        env_data->attack = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_attack]
    }
    else if (sliderThatWasMoved == slider_decay)
    {
        //[UserSliderCode_slider_decay] -- add your slider handling code here..
        env_data->decay = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_decay]
    }
    else if (sliderThatWasMoved == slider_release)
    {
        //[UserSliderCode_slider_release] -- add your slider handling code here..
        env_data->release = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_release]
    }
    else if (sliderThatWasMoved == slider_sustain_time)
    {
        //[UserSliderCode_slider_sustain_time] -- add your slider handling code here..
        env_data->sustain_time = sliderThatWasMoved->getValue()/10000;
        //[/UserSliderCode_slider_sustain_time]
    }
    else if (sliderThatWasMoved == slider_sustain)
    {
        //[UserSliderCode_slider_sustain] -- add your slider handling code here..
        sustain->set_value(sliderThatWasMoved->getValue());
        //[/UserSliderCode_slider_sustain]
    }
    else if (sliderThatWasMoved == slider_env_shape)
    {
        //[UserSliderCode_slider_env_shape] -- add your slider handling code here..
        env_data->shape = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_env_shape]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Monique_Ui_ENVPopup::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        if(Monique_Ui_Mainwindow* mainwindow = AppInstanceStore::getInstance()->editor)
        {
            mainwindow->open_env_popup(nullptr,nullptr,nullptr,nullptr,false);
        }
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == keep)
    {
        //[UserButtonCode_keep] -- add your button handler code here..
        GET_DATA( synth_data ).auto_switch_env_popup ^= true;
        //[/UserButtonCode_keep]
    }
    else if (buttonThatWasClicked == auto_close)
    {
        //[UserButtonCode_auto_close] -- add your button handler code here..
        GET_DATA( synth_data ).auto_close_env_popup ^= true;
        //[/UserButtonCode_auto_close]
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_ENVPopup" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="Monique_Ui_Mainwindow*const parent_, ENVData*const env_data_, Parameter*const sustain_, bool left_, bool has_negative_sustain_"
                 variableInitialisers="parent(parent_),env_data(env_data_),sustain(sustain_),original_w(710), original_h(190),left(left_)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="710" initialHeight="190">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="1 10 708 179" cornerSize="10" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 40 0 l 50 10 l 30 10 x</PATH>
  </BACKGROUND>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label_attack_bottom"
         virtualName="" explicitFocusOrder="0" pos="20 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATTACK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_attack" virtualName="Slider"
          explicitFocusOrder="0" pos="20 60 60 80" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1" int="5.0000000000000002396e-05" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="5269c763f2d5a37b" memberName="label_decay_bottom"
         virtualName="" explicitFocusOrder="0" pos="80 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DECAY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b62502f225e4fe3a" memberName="slider_decay" virtualName="Slider"
          explicitFocusOrder="0" pos="80 60 60 80" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1" int="5.0000000000000002396e-05" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="d001c80859e5b7cb" memberName="label_release_bottom"
         virtualName="" explicitFocusOrder="0" pos="260 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RELEASE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e8c91a0aabc505e" memberName="slider_release" virtualName="Slider"
          explicitFocusOrder="0" pos="260 60 60 80" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1" int="5.0000000000000002396e-05" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="76a391a494643c63" memberName="slider_sustain_time"
          virtualName="Slider" explicitFocusOrder="0" pos="200 60 60 80"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="10000" int="1" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ffcf23120599c6e5" memberName="label_sustain_time_bottom"
         virtualName="" explicitFocusOrder="0" pos="200 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS TIME" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a09cec04c5ae6b58" memberName="label_attack" virtualName=""
         explicitFocusOrder="0" pos="20 20 60 33" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1b295ca55294b0a2" memberName="label_decay" virtualName=""
         explicitFocusOrder="0" pos="80 20 60 33" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e1944df446a5aea6" memberName="label_sustain_time"
         virtualName="" explicitFocusOrder="0" pos="200 20 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="72cc727ebebbec15" memberName="label_release" virtualName=""
         explicitFocusOrder="0" pos="260 20 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b7e5d3f5d3dbf47a" memberName="slider_sustain" virtualName="Slider"
          explicitFocusOrder="0" pos="140 60 60 80" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1" int="0.0010000000000000000208" style="LinearVertical"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ee00adc332943fc6" memberName="label_sustain_bottom"
         virtualName="" explicitFocusOrder="0" pos="140 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="(SUSTAIN)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="8b7051eff652e1d6" memberName="label_sustain" virtualName=""
         explicitFocusOrder="0" pos="140 20 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c88b6f548bad2da7" memberName="plotter" virtualName=""
                    explicitFocusOrder="0" pos="410 20 260 159" class="Component"
                    params=""/>
  <SLIDER name="0" id="e7a1c7c979888f2f" memberName="slider_env_shape"
          virtualName="Slider" explicitFocusOrder="0" pos="340 60 60 80"
          tooltip="Define the ADSR curve type. This is a global paraneter and will be used for all envelopes (includes FILTER ADSR's and MAIN ADSR)."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ad65d35c7b51c7ea" memberName="label_shape"
         virtualName="" explicitFocusOrder="0" pos="340 140 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SHAPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="680 20 20 20" tooltip="Close this pop up. &#10;(ESC is your friend)"
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="acbee438c46f4a5d" memberName="keep" virtualName=""
              explicitFocusOrder="0" pos="680 160 20 20" tooltip="OPTION: auto switch this pop up to its siblings on any mouse action at a sibling.&#10;(e.g. from one OSC input to another one of the same filter)"
              bgColOff="ff008000" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="S" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="28bf323aea1c44dd" memberName="auto_close" virtualName=""
              explicitFocusOrder="0" pos="680 130 20 20" tooltip="OPTION: auto close this popup on any unrelated mouse action.&#10;(e.g. click the main user interface)"
              bgColOff="ffffff00" bgColOn="ffffff00" textCol="ff000000" textColOn="ff000000"
              buttonText="C" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
