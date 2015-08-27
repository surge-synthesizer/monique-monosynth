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
#include "monique_ui_LookAndFeel.h"
#include "SynthData.h"
//[/Headers]

#include "monique_ui_DragPad.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class DragPad : public Component
{
    UiDragPad*const parent;
    void mouseDoubleClick (const MouseEvent&) override;
    void mouseDrag(const MouseEvent& event) override;

public:
    NOINLINE DragPad( UiDragPad*const parent_ );
    NOINLINE ~DragPad();
};

//==============================================================================
NOINLINE DragPad::DragPad( UiDragPad*const parent_ ) : parent( parent_ ) {}
NOINLINE DragPad::~DragPad() {}

//==============================================================================
void DragPad::mouseDoubleClick (const MouseEvent&)
{
    const float morph_motor_time = DATA( synth_data ).morph_motor_time;
    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[0], 0, morph_motor_time );
    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[1], 0, morph_motor_time );
    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[2], 0, morph_motor_time );
    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[3], 0, morph_motor_time );

    parent->set_left_to_right_states( 0.5f, 0.5f );
}
void DragPad::mouseDrag(const MouseEvent& event)
{
    Point<int> current_position = event.getPosition();

    float left2right_state = 0;
    {
        if( current_position.getX() >= 0 && current_position.getX() <= getWidth() )
        {
            left2right_state = 1.0f/getWidth()*current_position.getX();
        }
        else if( current_position.getX() < 0 )
        {
            left2right_state = 0;
        }
        else if( current_position.getX() > getWidth() )
        {
            left2right_state = 1;
        }
    }

    float top2bottom_state = 0;
    {
        if( current_position.getY() >= 0 && current_position.getY() <= getHeight() )
        {
            top2bottom_state = 1.0f/getHeight()*current_position.getY();
        }
        else if( current_position.getY() < 0 )
        {
            top2bottom_state = 0;
        }
        else if( current_position.getY() > getHeight() )
        {
            top2bottom_state = 1;
        }
    }

    {
        const float morph_motor_time = DATA( synth_data ).morph_motor_time;

        float morph_top_left = 1.0f-left2right_state-top2bottom_state;
        {
            if( morph_top_left < 0 )
                morph_top_left = 0;
            else if( morph_top_left > 1 )
                morph_top_left = 1;
            ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[0], morph_top_left, morph_motor_time );
        }

        float morph_top_right = left2right_state-top2bottom_state;
        {
            if( morph_top_right < 0 )
                morph_top_right = 0;
            else if( morph_top_right > 1 )
                morph_top_right = 1;
            ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[1], morph_top_right, morph_motor_time );
        }

        float morph_bottom_left = top2bottom_state-left2right_state;
        {
            if( morph_bottom_left < 0 )
                morph_bottom_left = 0;
            else if( morph_bottom_left > 1 )
                morph_bottom_left = 1;
            ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[2], morph_bottom_left, morph_motor_time );
        }

        float morph_bottom_right = top2bottom_state-(1.0f-left2right_state);
        {
            if( morph_bottom_right < 0 )
                morph_bottom_right = 0;
            else if( morph_bottom_right > 1 )
                morph_bottom_right = 1;
            ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[3], morph_bottom_right, morph_motor_time );
        }

        parent->set_left_to_right_states( left2right_state, top2bottom_state );
    }
}
//[/MiscUserDefs]

//==============================================================================
UiDragPad::UiDragPad ()
    : left2right_state(0.5),
      top2bottom_state(0.5),
      current_position(0,0),
      original_w(80), original_h(130)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (track_area = new DragPad (this));


    //[UserPreSize]
    this->setWantsKeyboardFocus(false);
    this->setRepaintsOnMouseActivity(false);
    //this->setOpaque(true);

    for( int i = 0 ; i < getNumChildComponents() ; ++i )
    {
        Component* comp = getChildComponent(i);
        comp->setWantsKeyboardFocus(false);
        comp->setRepaintsOnMouseActivity(false);
        // comp->setOpaque(true);
    }

    /*
    //[/UserPreSize]

    setSize (80, 130);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

UiDragPad::~UiDragPad()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    track_area = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiDragPad::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    ComponentColours colours_ = UiLookAndFeel::getInstance()->colours;

    const float w_h = 15;
    const float x = track_area->getX()+1;
    const float y = track_area->getY()+1;
    const float w = track_area->getWidth()-w_h-2;
    const float h = track_area->getHeight()-w_h-2;

    UiLookAndFeel::getInstance()->drawGlassSphere
    (
        g,
        x+w*left2right_state,
        y+h*top2bottom_state,
        w_h,
        colours_.slider_track_colour, 0.3f
    );
#include "mono_ui_includeHacks_BEGIN.h"
    /*
    //[/UserPrePaint]

    g.setColour (Colour (0xff11ffff));
    g.fillEllipse (45.0f, 97.0f, 20.0f, 20.0f);

    g.setColour (Colour (0xffff3b00));
    g.drawEllipse (45.0f, 97.0f, 20.0f, 20.0f, 2.000f);

    //[UserPaint] Add your own custom painting code here..
    */
    //[/UserPaint]
}

void UiDragPad::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    track_area->setBounds (0, 0, 80, 130);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void UiDragPad::set_left_to_right_states( float left2right_state_, float top2bottom_state_ ) {
    left2right_state = left2right_state_;
    top2bottom_state = top2bottom_state_;

    repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiDragPad" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers="left2right_state(0.5),&#10;top2bottom_state(0.5),&#10;current_position(0,0),&#10;original_w(80), original_h(130)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="80" initialHeight="130">
  <BACKGROUND backgroundColour="50505">
    <ELLIPSE pos="45 97 20 20" fill="solid: ff11ffff" hasStroke="1" stroke="2, mitered, butt"
             strokeColour="solid: ffff3b00"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="c0cbef1abd65e74c" memberName="track_area" virtualName="DragPad"
                    explicitFocusOrder="0" pos="0 0 80 130" class="Component" params="this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
