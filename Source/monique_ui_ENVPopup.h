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

#ifndef __JUCE_HEADER_685271DE805B685A__
#define __JUCE_HEADER_685271DE805B685A__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class Monique_Ui_Mainwindow;
class Monique_Ui_DualSlider;
class ENVData;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_ENVPopup  : public Component,
                             public Monique_Ui_Refreshable,
                             public DropShadower,
                             public Timer,
                             public ParameterListener,
                             public SliderListener,
                             public LabelListener,
                             public ButtonListener
{
public:
    //==============================================================================
    Monique_Ui_ENVPopup (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, ENVData*const env_data_, Parameter*const sustain_, bool left_, bool has_negative_sustain_);
    ~Monique_Ui_ENVPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Component* related_to_comp;
    Monique_Ui_DualSlider*owner_slider;
    void set_element_to_show(Component*const, Monique_Ui_DualSlider*owner_);
    void update_positions();
    void refresh() noexcept override;
    void timerCallback() override;
    int callbacks;

    COLOUR_THEMES theme;

    const ENVData*const is_open_for() const noexcept
    {
        return env_data;
    }
    void set_clickable_components( Array<Component*>& comps_ ) noexcept;

    const float original_w;
    const float original_h;

private:
    bool is_repainting;
    const bool left;
    Array< float > curve;
    Monique_Ui_Mainwindow*const parent;
    ENVData*const env_data;
    Parameter*const sustain;
    Array< Component* > observed_comps;

    float last_attack;
    float last_sustain;
    float last_decay;
    float last_release;
    float last_shape;
    float sustain_time;

    void mouseDown (const MouseEvent& event) override;
    void mouseDrag (const MouseEvent& event) override;
    void mouseUp (const MouseEvent& event) override;
    void mouseDoubleClick (const MouseEvent& event) override;
    void mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& ) override;
    void mouseMagnify (const MouseEvent& event, float ) override;

    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void sliderClicked (Slider*s_) override;
    ScopedPointer<AsyncUpdater> midi_in_runner;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void labelTextChanged (Label* labelThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_attack_bottom;
    ScopedPointer<Slider> slider_attack;
    ScopedPointer<Label> label_decay_bottom;
    ScopedPointer<Slider> slider_decay;
    ScopedPointer<Label> label_release_bottom;
    ScopedPointer<Slider> slider_release;
    ScopedPointer<Slider> slider_sustain_time;
    ScopedPointer<Label> label_sustain_time_bottom;
    ScopedPointer<Label> label_attack;
    ScopedPointer<Label> label_decay;
    ScopedPointer<Label> label_sustain_time;
    ScopedPointer<Label> label_release;
    ScopedPointer<Slider> slider_sustain;
    ScopedPointer<Label> label_sustain_bottom;
    ScopedPointer<Label> label_sustain;
    ScopedPointer<Component> plotter;
    ScopedPointer<Slider> slider_env_shape;
    ScopedPointer<Label> label_shape;
    ScopedPointer<TextButton> close;
    ScopedPointer<TextButton> keep;
    ScopedPointer<TextButton> auto_close;
    ScopedPointer<TextButton> copy;
    ScopedPointer<TextButton> past;
    Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_ENVPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_685271DE805B685A__
