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
#include "SynthData.h"
//[/Headers]

#include "UiDragPad.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
class DragPad : public Component {
    UiDragPad*const parent;

    void mouseDrag(const MouseEvent& event) {
        Point<int> current_position = event.getPosition();

        //bool success = false;
	
        float left2right_state = 0;
        if( current_position.getX() >= 0 && current_position.getX() <= getWidth() )
        {
            left2right_state = 1.0f/getWidth()*current_position.getX();
            //success = true;
        }
        else if( current_position.getX() < 0 )
        {
            //success = left2right_state != 0;
            left2right_state = 0;
        }
        else if( current_position.getX() > getWidth() )
        {
            //success = left2right_state != 1;
            left2right_state = 1;
        }

        float top2bottom_state = 0;
        if( current_position.getY() >= 0 && current_position.getY() <= getHeight() )
        {
            top2bottom_state = 1.0f/getHeight()*current_position.getY();
            //success = true;
        }
        else if( current_position.getY() < 0 )
        {
            //success = top2bottom_state != 0;
            top2bottom_state = 0;
        }
        else if( current_position.getY() > getHeight() )
        {
            //success = top2bottom_state != 1;
            top2bottom_state = 1;
        }

        //if( success )
        {
            float morph_top_left = 1.0f-left2right_state-top2bottom_state;
            if( morph_top_left < 0 )
                morph_top_left = 0;
            else if( morph_top_left > 1 )
                morph_top_left = 1;
	    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[0], morph_top_left, DATA( synth_data ).morph_motor_time );

            float morph_top_right = left2right_state-top2bottom_state;
            if( morph_top_right < 0 )
                morph_top_right = 0;
            else if( morph_top_right > 1 )
                morph_top_right = 1;
	    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[1], morph_top_right, DATA( synth_data ).morph_motor_time );

            float morph_bottom_left = top2bottom_state-left2right_state;
            if( morph_bottom_left < 0 )
                morph_bottom_left = 0;
            else if( morph_bottom_left > 1 )
                morph_bottom_left = 1;
	    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[2], morph_bottom_left, DATA( synth_data ).morph_motor_time );

            float morph_bottom_right = top2bottom_state-(1.0f-left2right_state);
            if( morph_bottom_right < 0 )
                morph_bottom_right = 0;
            else if( morph_bottom_right > 1 )
                morph_bottom_right = 1;
	    ChangeParamOverTime::execute( DATA( synth_data ).morhp_states[3], morph_bottom_right, DATA( synth_data ).morph_motor_time );


            parent->set_left_to_right_states( left2right_state, top2bottom_state );
        }

    }

public:
    DragPad( UiDragPad*const parent_ ) : parent( parent_ ) {}
};
//[/MiscUserDefs]

//==============================================================================
UiDragPad::UiDragPad ()
    : left2right_state(0.5),
      top2bottom_state(0.5),
      current_position(0,0)
{
    addAndMakeVisible (track_area = new DragPad (this));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (80, 130);


    //[Constructor] You can add your own custom stuff here..
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
    ComponentColours colours = UiLookAndFeel::getInstance()->colours;
    Colour orange_replacement = colours.bg_lines;
    orange_replacement.getARGB();
#define Colour(x) ((x == 0xffff3b00) ? orange_replacement : Colour(x))

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (static_cast<float> (proportionOfWidth (0.1375f)), static_cast<float> (proportionOfHeight (0.0846f)), static_cast<float> (proportionOfWidth (0.7250f)), static_cast<float> (proportionOfHeight (0.8308f)), 10.000f, 2.000f);


    float e_w( proportionOfWidth (0.2500f) );
    float e_h( proportionOfHeight (0.1538f) );
    g.setColour (colours.slider_track_colour);
    g.fillEllipse (static_cast<float> (proportionOfWidth (left2right_state) )+1, static_cast<float> (proportionOfHeight (top2bottom_state))+1, e_w-2, e_h-2);

    g.setColour (colours.bg_lines);
    g.drawEllipse (static_cast<float> (proportionOfWidth (left2right_state))+1, static_cast<float> (proportionOfHeight (top2bottom_state))+1, e_w-2, e_h-2, 1.5f);

    return;
    //[/UserPrePaint]

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (static_cast<float> (proportionOfWidth (0.1375f)), static_cast<float> (proportionOfHeight (0.0846f)), static_cast<float> (proportionOfWidth (0.7250f)), static_cast<float> (proportionOfHeight (0.8308f)), 10.000f, 2.000f);

    g.setColour (Colours::aqua);
    g.fillEllipse (static_cast<float> (proportionOfWidth (0.5667f)), static_cast<float> (proportionOfHeight (0.7455f)), static_cast<float> (proportionOfWidth (0.2500f)), static_cast<float> (proportionOfHeight (0.1538f)));

    g.setColour (Colour (0xffff3b00));
    g.drawEllipse (static_cast<float> (proportionOfWidth (0.5667f)), static_cast<float> (proportionOfHeight (0.7455f)), static_cast<float> (proportionOfWidth (0.2500f)), static_cast<float> (proportionOfHeight (0.1538f)), 2.000f);

    //[UserPaint] Add your own custom painting code here..
#undef Colour
    //[/UserPaint]
}

void UiDragPad::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    track_area->setBounds (proportionOfWidth (0.1375f), proportionOfHeight (0.0846f), proportionOfWidth (0.7250f), proportionOfHeight (0.8308f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void UiDragPad::set_left_to_right_states( float left2right_state_, float top2bottom_state_ ) {
     left2right_state = left2right_state_*(1.0-0.25f);
     top2bottom_state = top2bottom_state_*(1.0-0.1538f);

     repaint();
}
void UiDragPad::refresh() {

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiDragPad" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams=""
                 variableInitialisers="left2right_state(0.5),&#10;top2bottom_state(0.5),&#10;current_position(0,0)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="80" initialHeight="130">
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="13.75% 8.462% 72.5% 83.077%" cornerSize="10" fill="solid: a5312a"
               hasStroke="1" stroke="2, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ELLIPSE pos="56.667% 74.545% 25% 15.385%" fill="solid: ff00ffff" hasStroke="1"
             stroke="2, mitered, butt" strokeColour="solid: ffff3b00"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="c0cbef1abd65e74c" memberName="track_area" virtualName="DragPad"
                    explicitFocusOrder="0" pos="13.75% 8.462% 72.5% 83.077%" class="Component"
                    params="this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
