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

#include "monique_ui_MFOPopup.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_MFOPopup::timerCallback()
{
    if( is_repainting )
        return;

    if( ++callbacks > 9 )
        stopTimer();

    MessageManagerLock mmLock;
    repaint( plotter->getBounds().getX()-1, plotter->getBounds().getY()-1, plotter->getBounds().getWidth()+2, plotter->getBounds().getHeight()+2 );
}
void Monique_Ui_MFOPopup::refresh() noexcept
{
    if( last_wave != mfo_data->wave.get_value()
    or last_speed != mfo_data->speed.get_value()
    or last_offset != mfo_data->phase_shift.get_value() )
    {
        last_wave = mfo_data->wave.get_value();
        last_speed = mfo_data->speed.get_value();
        last_offset = mfo_data->phase_shift.get_value();

        String speed_label(get_lfo_speed_multi_as_text(last_speed, synth_data->runtime_info, synth_data->runtime_notifyer->get_sample_rate() ));
        if( slider_speed->getProperties().set( VAR_INDEX_VALUE_TO_SHOW, speed_label + String("@") + String( is_integer(last_speed) ? "th" : "Hz") ) )
        {
            slider_speed->repaint();
        }
        if( slider_wave->getProperties().set( VAR_INDEX_VALUE_TO_SHOW, String(auto_round( mfo_data->wave.get_value() )) ) )
        {
            slider_wave->repaint();
        }
        if( slider_offset->getProperties().set( VAR_INDEX_VALUE_TO_SHOW, String(auto_round( mfo_data->phase_shift.get_value() )) ) )
        {
            slider_offset->repaint();
        }

        slider_wave->setValue( last_wave, dontSendNotification );
        slider_speed->setValue( last_speed, dontSendNotification );
        slider_offset->setValue( last_offset, dontSendNotification );

        stopTimer();
        callbacks = 0;
        startTimer( synth_data->glide_motor_time.get_value()/10 + 5 );
    }
    {
        SectionTheme& theme = look_and_feel->colours.get_theme( COLOUR_THEMES::POPUP_THEME );
        Colour button_off = theme.button_off_colour;
        auto_close->setColour (TextButton::buttonColourId, synth_data->auto_close_env_popup ? Colours::yellow : button_off );
        keep->setColour (TextButton::buttonColourId, synth_data->auto_switch_env_popup ? Colours::green : button_off );
    }

    past->setEnabled(bool(SHARED::getInstance()->mfo_clipboard));
}

void Monique_Ui_MFOPopup::set_element_to_show( Component*const comp_, Monique_Ui_DualSlider*owner_ )
{
    owner_slider = owner_;
    related_to_comp = comp_;

    int x = get_editor()->getLocalPoint(comp_,Point<int>(0,0)).getX();
    int y = get_editor()->getLocalPoint(comp_,Point<int>(0,0)).getY();

    const float width_scale = 1.0f/original_w*getWidth();
    const int left_move = (width_scale*10);
    setTopLeftPosition( x-left_move, y+comp_->getHeight() );
}
void Monique_Ui_MFOPopup::update_positions(  )
{
    if( related_to_comp )
    {
        set_element_to_show( related_to_comp, owner_slider );
    }
}
void Monique_Ui_MFOPopup::set_clickable_components( Array<Component*>& comps_ ) noexcept
{
    observed_comps = comps_;
    for( int i = 0 ; i != observed_comps.size() ; ++i )
    {
        Component*comp = observed_comps.getUnchecked(i);
        if( dynamic_cast< Monique_Ui_Mainwindow* >( comp ) )
        {
            comp->addMouseListener(this,false);
        }
        else
        {
            comp->addMouseListener(this,true);
        }
    }
}
void Monique_Ui_MFOPopup::mouseDown (const MouseEvent& event)
{
    if(Monique_Ui_Mainwindow* mainwindow = get_editor())
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
                    const bool keeps_open = mainwindow->handle_keep_mfo_open( dual_slider->_config );
                    success = true;
                    if( keeps_open and synth_data->auto_switch_env_popup )
                    {
                        if( owner_slider != dual_slider and owner_slider )
                        {
                            mainwindow->open_mfo_popup(dual_slider);
                            return;
                        }
                    }

                    break;
                }
                comp = comp->getParentComponent();
            }
            if( not success and synth_data->auto_close_env_popup )
            {
                mainwindow->open_mfo_popup(nullptr,nullptr,nullptr,COLOUR_THEMES::DUMMY_THEME);
            }
        }
    }
}
void Monique_Ui_MFOPopup::mouseDrag (const MouseEvent& event)
{
    mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseUp (const MouseEvent& event)
{
    //mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseDoubleClick (const MouseEvent& event)
{
    mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& )
{
    mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseMagnify (const MouseEvent& event, float )
{
    mouseDown(event);
}
void Monique_Ui_MFOPopup::parameter_value_changed( Parameter* param_ ) noexcept
{
    if( param_ == &ui_refresher->synth_data->midi_lfo_wave )
    {
        mfo_data->wave = param_->get_value();
    }
    else if( param_ == &ui_refresher->synth_data->midi_lfo_speed )
    {
        mfo_data->speed = param_->get_value();
    }
    else if( param_ == &ui_refresher->synth_data->midi_lfo_offset )
    {
        mfo_data->phase_shift = param_->get_value();
    }
}
void Monique_Ui_MFOPopup::sliderClicked (Slider*s_)
{
    if(IS_MIDI_LEARN)
    {
        sliderValueChanged( s_ );
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_MFOPopup::Monique_Ui_MFOPopup (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, LFOData*const mfo_data_, COLOUR_THEMES theme_)
    : Monique_Ui_Refreshable(ui_refresher_),
      DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point<int>(10,10))),
      original_w(540), original_h(190),
      parent(parent_),
      mfo_data(mfo_data_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    ui_refresher_->synth_data->midi_lfo_wave.set_value_without_notification(mfo_data_->wave);
    ui_refresher_->synth_data->midi_lfo_speed.set_value_without_notification(mfo_data_->speed.get_value());
    ui_refresher_->synth_data->midi_lfo_offset.set_value_without_notification(mfo_data_->phase_shift);

    last_wave = 999;
    last_speed = 999;
    last_offset = 999;
    owner_slider = nullptr;
    setOwner(this);
    theme = theme_;
    is_repainting = false;
    //[/Constructor_pre]

    addAndMakeVisible (slider_wave = new Slider ("0"));
    slider_wave->setTooltip (TRANS("Define the wave.\n"
                                   "\"(Sine (LEFT), close to Square (RIGHT))\""));
    slider_wave->setRange (0, 1, 0.01);
    slider_wave->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_wave->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_wave->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_wave->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_wave->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_wave->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_wave->addListener (this);

    addAndMakeVisible (label_shape2 = new Label ("new label",
            TRANS("WAVE")));
    label_shape2->setFont (Font (15.00f, Font::plain));
    label_shape2->setJustificationType (Justification::centred);
    label_shape2->setEditable (false, false, false);
    label_shape2->setColour (Label::textColourId, Colours::yellow);
    label_shape2->setColour (TextEditor::textColourId, Colours::black);
    label_shape2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_shape = new Label ("new label",
            TRANS("SPEED")));
    label_shape->setFont (Font (15.00f, Font::plain));
    label_shape->setJustificationType (Justification::centred);
    label_shape->setEditable (false, false, false);
    label_shape->setColour (Label::textColourId, Colours::yellow);
    label_shape->setColour (TextEditor::textColourId, Colours::black);
    label_shape->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_speed = new Slider ("0"));
    slider_speed->setTooltip (TRANS("Define the oscillator speed."));
    slider_speed->setRange (0, 16, 1);
    slider_speed->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_speed->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_speed->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_speed->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_speed->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_speed->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_speed->addListener (this);

    addAndMakeVisible (plotter = new Component());

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
    keep->setButtonText (TRANS("aSW"));
    keep->addListener (this);
    keep->setColour (TextButton::buttonColourId, Colours::green);
    keep->setColour (TextButton::buttonOnColourId, Colours::green);
    keep->setColour (TextButton::textColourOnId, Colours::black);
    keep->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (auto_close = new TextButton (String::empty));
    auto_close->setTooltip (TRANS("OPTION: auto close this popup on any unrelated mouse action.\n"
                                  "(e.g. click the main user interface)"));
    auto_close->setButtonText (TRANS("aCL"));
    auto_close->addListener (this);
    auto_close->setColour (TextButton::buttonColourId, Colours::yellow);
    auto_close->setColour (TextButton::buttonOnColourId, Colours::yellow);
    auto_close->setColour (TextButton::textColourOnId, Colours::black);
    auto_close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (copy = new TextButton (String::empty));
    copy->setTooltip (TRANS("Copy this settings to the clipboard."));
    copy->setButtonText (TRANS("COPY"));
    copy->addListener (this);
    copy->setColour (TextButton::buttonColourId, Colours::cornflowerblue);
    copy->setColour (TextButton::buttonOnColourId, Colours::green);
    copy->setColour (TextButton::textColourOnId, Colours::black);
    copy->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (past = new TextButton (String::empty));
    past->setTooltip (TRANS("Past settings from the clipboard."));
    past->setButtonText (TRANS("PAST"));
    past->addListener (this);
    past->setColour (TextButton::buttonColourId, Colours::blueviolet);
    past->setColour (TextButton::buttonOnColourId, Colours::green);
    past->setColour (TextButton::textColourOnId, Colours::black);
    past->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (label_shape3 = new Label ("new label",
            TRANS("OFFSET")));
    label_shape3->setFont (Font (15.00f, Font::plain));
    label_shape3->setJustificationType (Justification::centred);
    label_shape3->setEditable (false, false, false);
    label_shape3->setColour (Label::textColourId, Colours::yellow);
    label_shape3->setColour (TextEditor::textColourId, Colours::black);
    label_shape3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_offset = new Slider ("0"));
    slider_offset->setTooltip (TRANS("Define the phase offset."));
    slider_offset->setRange (0, 1, 0.01);
    slider_offset->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_offset->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider_offset->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_offset->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_offset->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_offset->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_offset->addListener (this);

    addAndMakeVisible (mfo_minus = new TextButton (String::empty));
    mfo_minus->setTooltip (TRANS("Decrease the speed in steps."));
    mfo_minus->setButtonText (TRANS("-"));
    mfo_minus->addListener (this);
    mfo_minus->setColour (TextButton::buttonColourId, Colours::black);
    mfo_minus->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    mfo_minus->setColour (TextButton::textColourOffId, Colours::yellow);

    addAndMakeVisible (mfo_plus = new TextButton (String::empty));
    mfo_plus->setTooltip (TRANS("Increase the speed in steps."));
    mfo_plus->setButtonText (TRANS("+"));
    mfo_plus->addListener (this);
    mfo_plus->setColour (TextButton::buttonColourId, Colours::black);
    mfo_plus->setColour (TextButton::textColourOnId, Colour (0xffff3b00));
    mfo_plus->setColour (TextButton::textColourOffId, Colours::yellow);


    //[UserPreSize]
    related_to_comp = nullptr;
    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        Component*child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set( VAR_INDEX_COLOUR_THEME, theme_ );
    }
    //this->setRepaintsOnMouseActivity(false);
    plotter->setRepaintsOnMouseActivity(false);
    plotter->setOpaque(false);

    slider_speed->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_offset->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_wave->getProperties().set( VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX );
    slider_speed->setPopupMenuEnabled( true );
    slider_offset->setPopupMenuEnabled( true );
    slider_wave->setPopupMenuEnabled( true );


    close->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    keep->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    auto_close->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    copy->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    past->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    //[/UserPreSize]

    // setSize (540, 190);


    //[Constructor] You can add your own custom stuff here..
    ui_refresher_->synth_data->midi_lfo_wave.register_listener(this);
    ui_refresher_->synth_data->midi_lfo_speed.register_listener(this);
    ui_refresher_->synth_data->midi_lfo_offset.register_listener(this);
    //[/Constructor]
}

Monique_Ui_MFOPopup::~Monique_Ui_MFOPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    ui_refresher->synth_data->midi_lfo_wave.remove_listener(this);
    ui_refresher->synth_data->midi_lfo_speed.remove_listener(this);
    ui_refresher->synth_data->midi_lfo_offset.remove_listener(this);

    parent->show_info_popup(nullptr,nullptr);

    for( int i = 0 ; i != observed_comps.size() ; ++i )
    {
        Component*comp = observed_comps.getUnchecked(i);
        comp->removeMouseListener(this);
    }
    //[/Destructor_pre]

    slider_wave = nullptr;
    label_shape2 = nullptr;
    label_shape = nullptr;
    slider_speed = nullptr;
    plotter = nullptr;
    close = nullptr;
    keep = nullptr;
    auto_close = nullptr;
    copy = nullptr;
    past = nullptr;
    label_shape3 = nullptr;
    slider_offset = nullptr;
    mfo_minus = nullptr;
    mfo_plus = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_MFOPopup::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    is_repainting = true;

    g.setColour(Colours::black.withAlpha(0.8f));
    g.fillRect( getWidth()-10, getHeight()-10, 10,10);

#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.setColour (colours.get_theme( theme ).area_colour);
    g.fillRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f);

    Colour outline_and_track = colours.get_theme( theme ).value_slider_track_colour;
    g.setColour (outline_and_track);
    g.drawRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f, 1.000f);

    g.fillPath (internalPath1);

    /*
    //[/UserPrePaint]

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    */
#include "mono_ui_includeHacks_END.h"
    {
        curve.clearQuick();
        curve.ensureStorageAllocated( 50000 );

        synth_data->get_full_mfo( *mfo_data, curve );

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
        for( int i = 0 ; i != curve_size ; ++i )
        {
            float value = 1.0f-curve[i];
            const int x = scale_w*i+plotter_x;
            const int y = value*plotter_hight+plotter_y;
            if( last_x != x || last_y != y )
            {
                last_y = y;
                g.setColour (outline_and_track);
                g.fillRect (last_x, last_y, 1, 1);

                if( last_x != x )
                {
                    g.setColour (outline_and_track.withAlpha(0.1f));
                    g.fillRect (last_x, last_y, 1, int((1.0f-value)*plotter_hight));
                }
                last_x = x;
            }
        }
    }

    is_repainting = false;
    //[/UserPaint]
}

void Monique_Ui_MFOPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    slider_wave->setBounds (20, 70, 60, 60);
    label_shape2->setBounds (20, 140, 60, 30);
    label_shape->setBounds (90, 140, 60, 30);
    slider_speed->setBounds (90, 70, 60, 60);
    plotter->setBounds (240, 20, 240, 159);
    close->setBounds (490, 20, 40, 20);
    keep->setBounds (490, 70, 40, 20);
    auto_close->setBounds (490, 50, 40, 20);
    copy->setBounds (490, 135, 40, 20);
    past->setBounds (490, 155, 40, 20);
    label_shape3->setBounds (160, 140, 60, 30);
    slider_offset->setBounds (160, 70, 60, 60);
    mfo_minus->setBounds (90, 20, 30, 30);
    mfo_plus->setBounds (120, 20, 30, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f, 0.0f);
    internalPath1.lineTo (50.0f, 10.0f);
    internalPath1.lineTo (30.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_MFOPopup::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_wave)
    {
        //[UserSliderCode_slider_wave] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &ui_refresher->synth_data->midi_lfo_wave,
            sliderThatWasMoved
        )
        else
        {
            mfo_data->wave.set_value( sliderThatWasMoved->getValue() );
            ui_refresher->synth_data->midi_lfo_wave.set_value_without_notification( sliderThatWasMoved->getValue() );
        }
        parent->show_info_popup( sliderThatWasMoved, ui_refresher->synth_data->midi_lfo_wave.midi_control );
        //[/UserSliderCode_slider_wave]
    }
    else if (sliderThatWasMoved == slider_speed)
    {
        //[UserSliderCode_slider_speed] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &ui_refresher->synth_data->midi_lfo_speed,
            sliderThatWasMoved
        )
        else
        {
            mfo_data->speed.set_value( sliderThatWasMoved->getValue() );
            ui_refresher->synth_data->midi_lfo_speed.set_value_without_notification( sliderThatWasMoved->getValue() );
        }
        parent->show_info_popup( sliderThatWasMoved, ui_refresher->synth_data->midi_lfo_speed.midi_control );
        //[/UserSliderCode_slider_speed]
    }
    else if (sliderThatWasMoved == slider_offset)
    {
        //[UserSliderCode_slider_offset] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT
        (
            &ui_refresher->synth_data->midi_lfo_offset,
            sliderThatWasMoved
        )
        else
        {
            mfo_data->phase_shift.set_value( sliderThatWasMoved->getValue() );
            ui_refresher->synth_data->midi_lfo_offset.set_value_without_notification( sliderThatWasMoved->getValue() );
        }
        parent->show_info_popup( sliderThatWasMoved, ui_refresher->synth_data->midi_lfo_offset.midi_control );
        //[/UserSliderCode_slider_offset]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Monique_Ui_MFOPopup::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        if(Monique_Ui_Mainwindow* mainwindow = get_editor())
        {
            mainwindow->open_mfo_popup(nullptr,nullptr,nullptr,COLOUR_THEMES::DUMMY_THEME);
        }
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == keep)
    {
        //[UserButtonCode_keep] -- add your button handler code here..
        synth_data->auto_switch_env_popup ^= true;
        //[/UserButtonCode_keep]
    }
    else if (buttonThatWasClicked == auto_close)
    {
        //[UserButtonCode_auto_close] -- add your button handler code here..
        synth_data->auto_close_env_popup ^= true;
        //[/UserButtonCode_auto_close]
    }
    else if (buttonThatWasClicked == copy)
    {
        //[UserButtonCode_copy] -- add your button handler code here..
        if( not SHARED::getInstance()->mfo_clipboard )
        {
            SHARED::getInstance()->mfo_clipboard = new LFOData( nullptr, 999,"CBFO");
        } ::copy( SHARED::getInstance()->mfo_clipboard, mfo_data );
        //[/UserButtonCode_copy]
    }
    else if (buttonThatWasClicked == past)
    {
        //[UserButtonCode_past] -- add your button handler code here..
        if( SHARED::getInstance()->mfo_clipboard )
        {
            ::copy( mfo_data, SHARED::getInstance()->mfo_clipboard );
        }
        //[/UserButtonCode_past]
    }
    else if (buttonThatWasClicked == mfo_minus)
    {
        //[UserButtonCode_mfo_minus] -- add your button handler code here..
        mfo_data->speed.set_value( int( mfo_data->speed.get_value() - 1 ) );
        //[/UserButtonCode_mfo_minus]
    }
    else if (buttonThatWasClicked == mfo_plus)
    {
        //[UserButtonCode_mfo_plus] -- add your button handler code here..
        mfo_data->speed.set_value( int( mfo_data->speed.get_value() + 1 ) );
        //[/UserButtonCode_mfo_plus]
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_MFOPopup" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable, public DropShadower, public Timer, public ParameterListener"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, LFOData*const mfo_data_, COLOUR_THEMES theme_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10))),&#10;original_w(540), original_h(190),&#10;parent(parent_),&#10;mfo_data(mfo_data_)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="540" initialHeight="190">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="1 10 538 179" cornerSize="10" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 40 0 l 50 10 l 30 10 x</PATH>
  </BACKGROUND>
  <SLIDER name="0" id="1c3ffdc4ff28773b" memberName="slider_wave" virtualName="Slider"
          explicitFocusOrder="0" pos="20 70 60 60" tooltip="Define the wave.&#10;&quot;(Sine (LEFT), close to Square (RIGHT))&quot;"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="1cf28665f6a0e7c6" memberName="label_shape2"
         virtualName="" explicitFocusOrder="0" pos="20 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="WAVE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="ad65d35c7b51c7ea" memberName="label_shape"
         virtualName="" explicitFocusOrder="0" pos="90 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SPEED" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e7a1c7c979888f2f" memberName="slider_speed" virtualName="Slider"
          explicitFocusOrder="0" pos="90 70 60 60" tooltip="Define the oscillator speed."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="16" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="" id="c88b6f548bad2da7" memberName="plotter" virtualName=""
                    explicitFocusOrder="0" pos="240 20 240 159" class="Component"
                    params=""/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="490 20 40 20" tooltip="Close this pop up. &#10;(ESC is your friend)"
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="acbee438c46f4a5d" memberName="keep" virtualName=""
              explicitFocusOrder="0" pos="490 70 40 20" tooltip="OPTION: auto switch this pop up to its siblings on any mouse action at a sibling.&#10;(e.g. from one OSC input to another one of the same filter)"
              bgColOff="ff008000" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="aSW" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="28bf323aea1c44dd" memberName="auto_close" virtualName=""
              explicitFocusOrder="0" pos="490 50 40 20" tooltip="OPTION: auto close this popup on any unrelated mouse action.&#10;(e.g. click the main user interface)"
              bgColOff="ffffff00" bgColOn="ffffff00" textCol="ff000000" textColOn="ff000000"
              buttonText="aCL" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b19da151b3279272" memberName="copy" virtualName=""
              explicitFocusOrder="0" pos="490 135 40 20" tooltip="Copy this settings to the clipboard."
              bgColOff="ff6495ed" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="COPY" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b0118ea77c7b965a" memberName="past" virtualName=""
              explicitFocusOrder="0" pos="490 155 40 20" tooltip="Past settings from the clipboard."
              bgColOff="ff8a2be2" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="PAST" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="f482661fb32d75cc" memberName="label_shape3"
         virtualName="" explicitFocusOrder="0" pos="160 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="OFFSET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65508bcbaa0e7b8b" memberName="slider_offset" virtualName="Slider"
          explicitFocusOrder="0" pos="160 70 60 60" tooltip="Define the phase offset."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.010000000000000000208"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="" id="ff7ac483f2e2c468" memberName="mfo_minus" virtualName=""
              explicitFocusOrder="0" pos="90 20 30 30" tooltip="Decrease the speed in steps."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="-"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b9c09c85829baa4c" memberName="mfo_plus" virtualName=""
              explicitFocusOrder="0" pos="120 20 30 30" tooltip="Increase the speed in steps."
              bgColOff="ff000000" textCol="ffff3b00" textColOn="ffffff00" buttonText="+"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
