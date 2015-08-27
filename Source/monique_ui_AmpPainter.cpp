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
#include "UiLookAndFeel.h"
//[/Headers]

#include "monique_ui_AmpPainter.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
mono_AmpPainter::mono_AmpPainter ()
    : original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (sl_show_range = new Slider (String::empty));
    sl_show_range->setRange (0.001, 1, 0.001);
    sl_show_range->setSliderStyle (Slider::LinearHorizontal);
    sl_show_range->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_show_range->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    sl_show_range->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    sl_show_range->setColour (Slider::textBoxTextColourId, Colours::yellow);
    sl_show_range->addListener (this);

    addAndMakeVisible (osc_1 = new TextButton ("new button"));
    osc_1->setButtonText (TRANS("OSC 1"));
    osc_1->addListener (this);

    addAndMakeVisible (osc_2 = new TextButton ("new button"));
    osc_2->setButtonText (TRANS("OSC 2"));
    osc_2->addListener (this);

    addAndMakeVisible (osc_3 = new TextButton ("new button"));
    osc_3->setButtonText (TRANS("OSC 3"));
    osc_3->addListener (this);

    addAndMakeVisible (eq = new TextButton ("new button"));
    eq->setButtonText (TRANS("EQ"));
    eq->addListener (this);

    addAndMakeVisible (out = new TextButton ("new button"));
    out->setButtonText (TRANS("OUT"));
    out->addListener (this);

    addAndMakeVisible (f_1 = new TextButton ("new button"));
    f_1->setButtonText (TRANS("F 1"));
    f_1->addListener (this);

    addAndMakeVisible (f_2 = new TextButton ("new button"));
    f_2->setButtonText (TRANS("F 2"));
    f_2->addListener (this);

    addAndMakeVisible (f_3 = new TextButton ("new button"));
    f_3->setButtonText (TRANS("F 3"));
    f_3->addListener (this);

    addAndMakeVisible (f_env_1 = new TextButton ("new button"));
    f_env_1->setButtonText (TRANS("F-ADSR 1"));
    f_env_1->addListener (this);

    addAndMakeVisible (f_env_2 = new TextButton ("new button"));
    f_env_2->setButtonText (TRANS("F-ADSR 2"));
    f_env_2->addListener (this);

    addAndMakeVisible (f_env_3 = new TextButton ("new button"));
    f_env_3->setButtonText (TRANS("F-ADSR 3"));
    f_env_3->addListener (this);

    addAndMakeVisible (out_env = new TextButton ("new button"));
    out_env->setButtonText (TRANS("O-ADSR"));
    out_env->addListener (this);

    addAndMakeVisible (drawing_area = new Component());


    //[UserPreSize]
    osc_values.add( new EndlessSwitchBuffer() );
    osc_values.add( new EndlessSwitchBuffer() );
    osc_values.add( new EndlessSwitchBuffer() );
    filter_values.add( new EndlessBuffer() );
    filter_values.add( new EndlessBuffer() );
    filter_values.add( new EndlessBuffer() );
    filter_env_values.add( new EndlessBuffer() );
    filter_env_values.add( new EndlessBuffer() );
    filter_env_values.add( new EndlessBuffer() );

    buffers.add( osc_values[0] );
    buffers.add( osc_values[1] );
    buffers.add( osc_values[2] );
    buffers.add( filter_values[0] );
    buffers.add( filter_values[1] );
    buffers.add( filter_values[2] );
    buffers.add( filter_env_values[0] );
    buffers.add( filter_env_values[1] );
    buffers.add( filter_env_values[2] );
    buffers.add( &values );
    buffers.add( &values_env );

    refresh_buttons();

    for( int i = 0 ; i != getNumChildComponents() ; ++i )
    {
        getChildComponent(i)->setOpaque(true);
    }
    drawing_area->setOpaque(false);
    sl_show_range->setOpaque(false);
    setOpaque(true);


    /*
    //[/UserPreSize]

    setSize (1465, 180);


    //[Constructor] You can add your own custom stuff here..
    */

    startTimer(UI_REFRESH_RATE);
    //[/Constructor]
}

mono_AmpPainter::~mono_AmpPainter()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sl_show_range = nullptr;
    osc_1 = nullptr;
    osc_2 = nullptr;
    osc_3 = nullptr;
    eq = nullptr;
    out = nullptr;
    f_1 = nullptr;
    f_2 = nullptr;
    f_3 = nullptr;
    f_env_1 = nullptr;
    f_env_2 = nullptr;
    f_env_3 = nullptr;
    out_env = nullptr;
    drawing_area = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void mono_AmpPainter::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    // TODO paint all or only values
    {
        lock_for_reading();
	
        g.fillAll (Colour(0xff050505));

        // TODO MAKE INTS!
        const int samples_to_paint = sl_show_range->getValue()*RuntimeNotifyer::getInstance()->get_sample_rate()*0.5;
        int width = drawing_area->getWidth();
        float scale = float(width)/samples_to_paint;
        const int paint_start_offset_x = drawing_area->getX();
        const int paint_start_offset_y = drawing_area->getY();
        const float height = drawing_area->getHeight()-1;
        const int line_center = paint_start_offset_y + height/2;

        const int current_position = osc_values.getUnchecked(0)->get_new_reader_start_position(samples_to_paint);
        {
            Colour colour = Colour(0xff222222 );
            g.setGradientFill (ColourGradient (colour.darker (0.3f), 0.0f, 0.0f, Colour (0xff050505), 0.0f, height, false));
            //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f, height, false));
            g.fillRoundedRectangle (paint_start_offset_x, paint_start_offset_y, width, height, 3);

            g.setColour (colour.darker (0.6f));
            g.drawRoundedRectangle (paint_start_offset_x, paint_start_offset_y, width, height, 3, 1);

            g.setColour (UiLookAndFeel::getInstance()->colours.label_text_colour.withAlpha(0.3f));
            g.fillRect (paint_start_offset_x, int(paint_start_offset_y+height/2), width, 1 );
        }

        struct mono_AmpPainter
        {
            static void exec
            (
                Graphics& g,

                const int buffer_start_pos_,
                const float scale_,

                const int x_offset_,
                const int y_center_,
                const int height_,

                const Colour& col_,

                EndlessBuffer& source_buffer_,
                int num_samples_
            )
            {
                const Colour col_fill(col_.withAlpha(0.1f));
                int last_x = -9999;
                int last_y = -9999;

                int pos_counter = buffer_start_pos_;
                for( int sid = 0 ; sid < num_samples_ ; ++sid )
                {
                    const int x = mono_floor((scale_*sid)+x_offset_);
                    float y = source_buffer_.get_next_and_count(pos_counter);
                    bool paint_line = true;
                    if( last_x == x )
                    {
                        paint_line = false;
                    }
                    {
                        if( y >= 0 )
                        {
                            if( y > 1 )
                                y = 1;

                            int h = mono_floor(y*height_)*0.5f;

                            if( paint_line )
                            {
                                g.setColour(col_fill);
                                g.fillRect(x, y_center_ - h, 1, h);
                            }
                            if( last_y != y || last_x == x )
                            {
                                g.setColour(col_);
                                g.fillRect(x, y_center_ - h, 1, 1);

                                last_y = y;
                            }
                        }
                        else
                        {
                            if( y < -1 )
                                y = -1;

                            int h = mono_floor(y*height_)*-0.5f;
                            if( paint_line )
                            {
                                g.setColour(col_fill);
                                g.fillRect(x, y_center_, 1, h);
                            }
                            if( last_y != y || last_x == x )
                            {
                                g.setColour(col_);
                                g.fillRect(x, y_center_ + h, 1, 1);

                                last_y = y;
                            }
                        }

                        last_x = x;
                    }
                }
            }
        };

        SynthData& synth_data = DATA( synth_data );
        const bool show_osc[SUM_OSCS] = { synth_data.osci_show_osc_1, synth_data.osci_show_osc_2, synth_data.osci_show_osc_3 };
        const bool show_flt[SUM_OSCS] = { synth_data.osci_show_flt_1, synth_data.osci_show_flt_2, synth_data.osci_show_flt_3 };
        const bool show_flt_env[SUM_OSCS] = { synth_data.osci_show_flt_env_1, synth_data.osci_show_flt_env_2, synth_data.osci_show_flt_env_3 };
        const bool show_eq = synth_data.osci_show_eq;
        const bool show_out = synth_data.osci_show_out;
        const bool show_out_env = synth_data.osci_show_out_env;
	
	// OSC'S
        for( int osc_id = 0 ; osc_id != osc_values.size() ; ++osc_id )
        {
            EndlessBuffer& values = *osc_values[osc_id];
            if( show_osc[osc_id] )
            {
                Colour col;
                if( osc_id == 0 )
                    col = Colours::lightblue;
                else if( osc_id == 1 )
                    col = Colours::blueviolet;
                else
                    col = Colours::violet;

                mono_AmpPainter::exec
                (
                    g,

                    current_position,
                    scale,

                    paint_start_offset_x,
                    line_center,
                    height,

                    col,
                    values,

                    samples_to_paint
                );
            }
        }

        // EQ
        if( show_eq )
        {
            Colour col = Colours::green;

            mono_AmpPainter::exec
            (
                g,

                current_position,
                scale,

                paint_start_offset_x,
                line_center,
                height,

                col,
                eq_values,

                samples_to_paint
            );
        }

        // FILTERS
        for( int filter_id = 0 ; filter_id != filter_values.size() ; ++filter_id )
        {
            Colour col;
            if( filter_id == 0 )
                col = Colours::red;
            else if( filter_id == 1 )
                col = Colours::orangered;
            else
                col = Colours::orange;

            EndlessBuffer& values = *filter_values[filter_id];
            if( show_flt[filter_id] )
            {
                mono_AmpPainter::exec
                (
                    g,

                    current_position,
                    scale,

                    paint_start_offset_x,
                    line_center,
                    height,

                    col,

                    values,

                    samples_to_paint
                );
            }

            EndlessBuffer& values_env = *filter_env_values[filter_id];
            if( show_flt_env[filter_id] )
            {
                mono_AmpPainter::exec
                (
                    g,

                    current_position,
                    scale,

                    paint_start_offset_x,
                    line_center,
                    height,

                    col,

                    values_env,

                    samples_to_paint
                );
            }

        }

        // FINAL OUTPUT
        if( show_out )
        {
            mono_AmpPainter::exec
            (
                g,

                current_position,
                scale,

                paint_start_offset_x,
                line_center,
                height,

                UiLookAndFeel::getInstance()->colours.slider_track_colour,

                values,

                samples_to_paint
            );
        }

        // MAIN ENV
        if( show_out_env )
        {
            mono_AmpPainter::exec
            (
                g,

                current_position,
                scale,

                paint_start_offset_x,
                line_center,
                height,

                UiLookAndFeel::getInstance()->colours.slider_track_colour.darker(),

                values_env,

                samples_to_paint
            );
        }

        unlock_for_reading();
    }

    /*
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    //[UserPaint] Add your own custom painting code here..
    */
    //[/UserPaint]
}

void mono_AmpPainter::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPreResize]

    sl_show_range->setBounds (215, 150, 1240, 20);
    osc_1->setBounds (30, 10, 60, 20);
    osc_2->setBounds (30, 35, 60, 20);
    osc_3->setBounds (30, 60, 60, 20);
    eq->setBounds (100, 10, 60, 20);
    out->setBounds (100, 35, 60, 20);
    f_1->setBounds (30, 95, 60, 20);
    f_2->setBounds (30, 120, 60, 20);
    f_3->setBounds (30, 145, 60, 20);
    f_env_1->setBounds (100, 95, 60, 20);
    f_env_2->setBounds (100, 120, 60, 20);
    f_env_3->setBounds (100, 145, 60, 20);
    out_env->setBounds (100, 60, 60, 20);
    drawing_area->setBounds (215, 10, 1240, 135);
    //[UserResized] Add your own custom resize handling here..

#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void mono_AmpPainter::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_show_range)
    {
        //[UserSliderCode_sl_show_range] -- add your slider handling code here..
        DATA( synth_data ).osci_show_range = sl_show_range->getValue();
        //[/UserSliderCode_sl_show_range]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void mono_AmpPainter::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == osc_1)
    {
        //[UserButtonCode_osc_1] -- add your button handler code here..
        DATA( synth_data ).osci_show_osc_1 ^= true;
        //[/UserButtonCode_osc_1]
    }
    else if (buttonThatWasClicked == osc_2)
    {
        //[UserButtonCode_osc_2] -- add your button handler code here..
        DATA( synth_data ).osci_show_osc_2 ^= true;
        //[/UserButtonCode_osc_2]
    }
    else if (buttonThatWasClicked == osc_3)
    {
        //[UserButtonCode_osc_3] -- add your button handler code here..
        DATA( synth_data ).osci_show_osc_3 ^= true;
        //[/UserButtonCode_osc_3]
    }
    else if (buttonThatWasClicked == eq)
    {
        //[UserButtonCode_eq] -- add your button handler code here..
        DATA( synth_data ).osci_show_eq ^= true;
        //[/UserButtonCode_eq]
    }
    else if (buttonThatWasClicked == out)
    {
        //[UserButtonCode_out] -- add your button handler code here..
        DATA( synth_data ).osci_show_out ^= true;
        //[/UserButtonCode_out]
    }
    else if (buttonThatWasClicked == f_1)
    {
        //[UserButtonCode_f_1] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_1 ^= true;
        //[/UserButtonCode_f_1]
    }
    else if (buttonThatWasClicked == f_2)
    {
        //[UserButtonCode_f_2] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_2 ^= true;
        //[/UserButtonCode_f_2]
    }
    else if (buttonThatWasClicked == f_3)
    {
        //[UserButtonCode_f_3] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_3 ^= true;
        //[/UserButtonCode_f_3]
    }
    else if (buttonThatWasClicked == f_env_1)
    {
        //[UserButtonCode_f_env_1] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_env_1 ^= true;
        //[/UserButtonCode_f_env_1]
    }
    else if (buttonThatWasClicked == f_env_2)
    {
        //[UserButtonCode_f_env_2] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_env_2 ^= true;
        //[/UserButtonCode_f_env_2]
    }
    else if (buttonThatWasClicked == f_env_3)
    {
        //[UserButtonCode_f_env_3] -- add your button handler code here..
        DATA( synth_data ).osci_show_flt_env_3 ^= true;
        //[/UserButtonCode_f_env_3]
    }
    else if (buttonThatWasClicked == out_env)
    {
        //[UserButtonCode_out_env] -- add your button handler code here..
        DATA( synth_data ).osci_show_out_env ^= true;
        //[/UserButtonCode_out_env]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//==============================================================================
//==============================================================================
//==============================================================================
void mono_AmpPainter::timerCallback()
{
    repaint( drawing_area->getBounds() );
    refresh_buttons();
}

void mono_AmpPainter::refresh_buttons()
{
    ComponentColours colours = UiLookAndFeel::getInstance()->colours;
    Colour button_on = colours.button_on_colour;
    Colour button_off = colours.button_off_colour;

    SynthData& synth_data = DATA( synth_data );

    sl_show_range->setValue(synth_data.osci_show_range, dontSendNotification );

    osc_1->setColour( TextButton::buttonColourId, synth_data.osci_show_osc_1 ? Colours::lightblue : button_off );
    osc_2->setColour( TextButton::buttonColourId, synth_data.osci_show_osc_2 ? Colours::blueviolet : button_off );
    osc_3->setColour( TextButton::buttonColourId, synth_data.osci_show_osc_3 ? Colours::violet : button_off );

    eq->setColour( TextButton::buttonColourId, synth_data.osci_show_eq ? Colours::green : button_off );

    f_1->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_1 ? Colours::red : button_off );
    f_2->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_2 ? Colours::orangered : button_off );
    f_3->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_3 ? Colours::orange : button_off );

    f_env_1->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_env_1 ? Colours::red : button_off );
    f_env_2->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_env_2 ? Colours::orangered : button_off );
    f_env_3->setColour( TextButton::buttonColourId, synth_data.osci_show_flt_env_3 ? Colours::orange : button_off );

    out->setColour( TextButton::buttonColourId, synth_data.osci_show_out ? UiLookAndFeel::getInstance()->colours.slider_track_colour : button_off );
    out_env->setColour( TextButton::buttonColourId, synth_data.osci_show_out_env ? UiLookAndFeel::getInstance()->colours.slider_track_colour.darker() : button_off );
}

//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE EndlessBuffer::EndlessBuffer() : current_size(sample_rate * 2 + block_size), sample_buffer( sample_rate * 2 + block_size ), reader_position(0) {}
NOINLINE EndlessBuffer::~EndlessBuffer() {}

//==============================================================================
NOINLINE void EndlessBuffer::sample_rate_changed( double /* old_sr_ */ ) noexcept
{
    ScopedLock locked(writer_lock);
    ScopedLock locked2(reader_lock);
    reader_position = 0;
    current_size = sample_rate * 2 + block_size;
    sample_buffer.setSize( current_size );
}
NOINLINE void EndlessBuffer::block_size_changed() noexcept
{
    sample_rate_changed(0);
}

//==============================================================================
inline void mono_AmpPainter::lock_for_reading() noexcept {
    for( int i = 0 ; i != buffers.size() ; ++i )
        buffers.getUnchecked(i)->read_lock();
}
inline void EndlessBuffer::read_lock() noexcept
{
    reader_lock.enter();
}
inline float EndlessBuffer::get_next_and_count( int& pos_ ) const noexcept
{
    pos_++;
    if( pos_ >= current_size )
        pos_ = 0;

    return sample_buffer.getReadPointer()[pos_];
}
inline void mono_AmpPainter::unlock_for_reading() noexcept {
    for( int i = 0 ; i != buffers.size() ; ++i )
        buffers.getUnchecked(i)->read_unlock();
}
inline void EndlessBuffer::read_unlock() noexcept
{
    reader_lock.enter();
}

//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE EndlessSwitchBuffer::EndlessSwitchBuffer() : switch_buffer( sample_rate * 2 + block_size ) {}
NOINLINE EndlessSwitchBuffer::~EndlessSwitchBuffer() {}

//==============================================================================
NOINLINE void EndlessSwitchBuffer::sample_rate_changed( double /* old_sr_ */ ) noexcept
{
    ScopedLock locked(writer_lock);
    ScopedLock locked2(reader_lock);
    reader_position = 0;
    current_size = sample_rate * 2 + block_size;
    sample_buffer.setSize( current_size );
    switch_buffer.setSize( current_size );
}
NOINLINE void EndlessSwitchBuffer::block_size_changed() noexcept
{
    sample_rate_changed(0);
}

//==============================================================================
int EndlessSwitchBuffer::get_new_reader_start_position( int samples_to_paint_ ) const noexcept
{
    int start_position = reader_position - samples_to_paint_;
    if( start_position < 0 )
        start_position = current_size - (start_position*-1);

    const float*const tmp_switch_buffer = switch_buffer.getReadPointer();
    // GO TO PAST AN FIND THE LAST SWICH
    for( int i = 0 ; i != current_size ; ++i )
    {
        if( tmp_switch_buffer[start_position] == 1 )
        {
            return start_position;
        }
        else
        {
            start_position--;
            if( start_position < 0 )
            {
                start_position = current_size-1;
            }
        }
    }

    return 0;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="mono_AmpPainter" componentName=""
                 parentClasses="public Component, public Timer" constructorParams=""
                 variableInitialisers="original_w(1465), original_h(180)" snapPixels="5"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505"/>
  <SLIDER name="" id="6770eaa357af0c63" memberName="sl_show_range" virtualName=""
          explicitFocusOrder="0" pos="215 150 1240 20" rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" min="0.0010000000000000000208"
          max="1" int="0.0010000000000000000208" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="new button" id="f50c5e2947daf2d9" memberName="osc_1" virtualName=""
              explicitFocusOrder="0" pos="30 10 60 20" buttonText="OSC 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="430b47338d775167" memberName="osc_2" virtualName=""
              explicitFocusOrder="0" pos="30 35 60 20" buttonText="OSC 2" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2c8665efd6c0c37d" memberName="osc_3" virtualName=""
              explicitFocusOrder="0" pos="30 60 60 20" buttonText="OSC 3" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="80760cb7f2a9d968" memberName="eq" virtualName=""
              explicitFocusOrder="0" pos="100 10 60 20" buttonText="EQ" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="13f5cd2a936d7f93" memberName="out" virtualName=""
              explicitFocusOrder="0" pos="100 35 60 20" buttonText="OUT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b51569f32393a334" memberName="f_1" virtualName=""
              explicitFocusOrder="0" pos="30 95 60 20" buttonText="F 1" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="773e5360237ed15c" memberName="f_2" virtualName=""
              explicitFocusOrder="0" pos="30 120 60 20" buttonText="F 2" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="60cf3f432ebdbf40" memberName="f_3" virtualName=""
              explicitFocusOrder="0" pos="30 145 60 20" buttonText="F 3" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3aa1b921ef4aea49" memberName="f_env_1"
              virtualName="" explicitFocusOrder="0" pos="100 95 60 20" buttonText="F-ADSR 1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="68fc0bbd2cf58e73" memberName="f_env_2"
              virtualName="" explicitFocusOrder="0" pos="100 120 60 20" buttonText="F-ADSR 2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1d48bab8b4aaf7c9" memberName="f_env_3"
              virtualName="" explicitFocusOrder="0" pos="100 145 60 20" buttonText="F-ADSR 3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="733cb649c95fb68" memberName="out_env" virtualName=""
              explicitFocusOrder="0" pos="100 60 60 20" buttonText="O-ADSR"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="87835d83e09366f2" memberName="drawing_area" virtualName=""
                    explicitFocusOrder="0" pos="215 10 1240 135" class="Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
