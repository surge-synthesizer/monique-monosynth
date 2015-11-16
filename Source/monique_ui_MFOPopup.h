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

#ifndef __JUCE_HEADER_B12ED69DCBAC8838__
#define __JUCE_HEADER_B12ED69DCBAC8838__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class Monique_Ui_Mainwindow;
class Monique_Ui_DualSlider;
class LFOData;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MFOPopup  : public Component,
                             public Monique_Ui_Refreshable,
                             public DropShadower,
                             public Timer,
                             public SliderListener,
                             public ButtonListener
{
public:
    //==============================================================================
    Monique_Ui_MFOPopup (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, LFOData*const mfo_data_, COLOUR_THEMES theme_);
    ~Monique_Ui_MFOPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    COLOUR_THEMES theme;
    bool is_repainting;
    
    Component* related_to_comp;
    Monique_Ui_DualSlider*owner_slider;
    void set_element_to_show(Component*const, Monique_Ui_DualSlider*owner_);
    void update_positions();
    void refresh() noexcept override;
    void timerCallback() override;
    int callbacks;

    const LFOData*const is_open_for() const noexcept
    {
      return mfo_data;
    }
    void set_clickable_components( Array<Component*>& comps_ ) noexcept;

    const float original_w;
    const float original_h;

private:
    float last_wave;
    float last_speed;
    float last_offset;

    Array< float > curve;
    Monique_Ui_Mainwindow*const parent;
    LFOData*const mfo_data;
    Array< Component* > observed_comps;

    void mouseDown (const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;
    void mouseUp (const MouseEvent& event) override;
    void mouseDoubleClick (const MouseEvent& event) override;
    void mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& ) override;
    void mouseMagnify (const MouseEvent& event, float ) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> slider_wave;
    ScopedPointer<Label> label_shape2;
    ScopedPointer<Label> label_shape;
    ScopedPointer<Slider> slider_speed;
    ScopedPointer<Component> plotter;
    ScopedPointer<TextButton> close;
    ScopedPointer<TextButton> keep;
    ScopedPointer<TextButton> auto_close;
    ScopedPointer<TextButton> copy;
    ScopedPointer<TextButton> past;
    ScopedPointer<Label> label_shape3;
    ScopedPointer<Slider> slider_offset;
    ScopedPointer<TextButton> mfo_minus;
    ScopedPointer<TextButton> mfo_plus;
    Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_MFOPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B12ED69DCBAC8838__
