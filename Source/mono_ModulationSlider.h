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

#ifndef __JUCE_HEADER_A80E4E78D5F2A35A__
#define __JUCE_HEADER_A80E4E78D5F2A35A__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"
#include "mono_Parameters.h"

// TODO
#define DONT_OVERRIDE_SLIDER_VALUE -99999

class mono_ModulationSlider;
class SnapSlider : public Slider {
    friend class mono_ModulationSlider;
    mono_ModulationSlider* owner;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    SnapSlider( const String& name_ ) : Slider( name_ ) { }
};

// THIS SLIDER IS LEFT TO RIGHT IF THE NAME IS "3"
class Left2MiddleSlider : public Slider
{
    Slider*_peer_behind;

    friend class mono_ModulationSlider;
    mono_ModulationSlider* owner;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    bool hitTest (int x, int) override
    {
        if( getName() == "3" ) {
            if( x >= getWidth()/2 ) {
                _peer_behind->toFront(true);
                _peer_behind->setEnabled(true);
                this->setEnabled(false);
                return false;

            }
        }

        return true;
    }

    void set_peer_behind( Slider*peer_behind_ ) {
        _peer_behind = peer_behind_;
    }

    Left2MiddleSlider(const String& componentName)
        :
        Slider(componentName),
        _peer_behind( nullptr )
    {}
};

// THIS SLIDER IS ALWAYS MIDDLE TO RIGHT
class Middle2RightSlider : public Slider
{
    Slider*_peer_behind;

public:
    bool hitTest (int x, int) override
    {
        if( x < getWidth()/2 ) {
            _peer_behind->toFront(true);
            _peer_behind->setEnabled(true);
            this->setEnabled(false);
            return false;
        }

        return true;
    }

    void set_peer_behind( Slider*peer_behind_ ) {
        _peer_behind = peer_behind_;
    }

    Middle2RightSlider(const String& componentName)
        :
        Slider(componentName),
        _peer_behind( nullptr )
    {}
};

enum {
    NO_TOP_BUTTON_AMP = -3,
    FIXED_TOP_BUTTON_COLOUR = -2
};

struct ModulationSliderConfigBase
{
    //==============================================================================
    // BASIC SLIDER TYPE
    virtual bool get_is_linear() const noexcept 
    {
        return false;
    }

    //==============================================================================
    // FRONT SLIDER
    virtual SLIDER_STYLES get_front_slider_style() const noexcept
    {
        return UNDEFINED_SLIDER_STYLE;
    }
    virtual mono_ParameterCompatibilityBase* get_front_parameter_base() const noexcept = 0;
    virtual int get_override_front_min_value() const noexcept
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    virtual int get_override_front_max_value() const noexcept
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }

    //==============================================================================
    // BACK SLIDER
    virtual SLIDER_STYLES get_back_slider_style() const noexcept
    {
        return UNDEFINED_SLIDER_STYLE;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    virtual mono_ParameterCompatibilityBase* get_back_parameter_base() const noexcept
    {
        return nullptr;
    }

    //==============================================================================
    // TOP BUTTON
    enum TOP_BUTTON_TYPE
    {
        TOP_BUTTON_IS_MODULATOR,
        TOP_BUTTON_IS_ON_OFF,

        TOP_BUTTON_TYPE_IS_UNKNOWN
    };
    virtual TOP_BUTTON_TYPE get_top_button_type() const noexcept
    {
        return TOP_BUTTON_TYPE_IS_UNKNOWN;
    }
    virtual mono_ParameterCompatibilityBase* get_top_button_parameter_base() const noexcept
    {
        return nullptr;
    }
    virtual StringRef get_top_button_text() const noexcept
    {
        return "";
    }
    virtual float get_top_button_amp() const noexcept
    {
        return NO_TOP_BUTTON_AMP;
    }

    //==============================================================================
    // BOTTOM BUTTON
    virtual StringRef get_bottom_button_text() const noexcept
    {
        return "";
    }
    virtual StringRef get_bottom_button_switch_text() const noexcept
    {
        return "";
    }
    virtual bool get_is_bottom_button_text_dynamic() const noexcept 
    {
        return false;
    }

    //==============================================================================
    // CENTER LABEL
    enum SHOW_TYPES
    {
        DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE,
        SHOW_OWN_VALUE
    };
    virtual SHOW_TYPES show_slider_value_on_top_on_change() const noexcept
    {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    virtual String get_center_value() const noexcept 
    {
        return "";
    }
    virtual StringRef get_center_suffix() const noexcept 
    {
        return "";
    }

protected:
    ModulationSliderConfigBase() {}
    
public:
    virtual ~ModulationSliderConfigBase() {}
};
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class mono_ModulationSlider  : public Component,
    public mono_UiRefreshable,
    public SliderListener,
    public ButtonListener
{
public:
    //==============================================================================
    mono_ModulationSlider (ModulationSliderConfigBase* config_);
    ~mono_ModulationSlider();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool is_in_ctrl_view() const;
    void set_ctrl_view_mode( bool mode_ ) const;
    void show_view_mode();

private:
    ModulationSliderConfigBase*const _config;

    mono_ParameterCompatibilityBase* front_parameter;
    mono_ParameterCompatibilityBase* modulation_parameter;
    mono_ParameterCompatibilityBase* back_parameter;
    ModulationSliderConfigBase::TOP_BUTTON_TYPE top_button_type;
    mono_ParameterCompatibilityBase* top_parameter;

    bool runtime_show_value_popup;
    bool last_runtime_show_value_popup;
    ModulationSliderConfigBase::SHOW_TYPES show_value_popup_type;
    float last_painted_value_slider_val;
    float last_painted_mod_slider_val;

    void refresh() noexcept;
    void sliderClicked (Slider*s_) override;

public:
    void sliderValueEnter (Slider*s_);
    void sliderValueExit (Slider*s_);
    void sliderModEnter (Slider*s_);
    void sliderModExit (Slider*s_);
    
public:
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
    bool keyPressed (const KeyPress& key);
    bool keyStateChanged (const bool isKeyDown);
    void modifierKeysChanged (const ModifierKeys& modifiers);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<SnapSlider> slider_value;
    ScopedPointer<TextButton> button_bottom;
    ScopedPointer<Left2MiddleSlider> slider_modulation;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> button_top;
    ScopedPointer<Label> label_top;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ModulationSlider)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_A80E4E78D5F2A35A__
