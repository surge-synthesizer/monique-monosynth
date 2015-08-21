/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_A80E4E78D5F2A35A__
#define __JUCE_HEADER_A80E4E78D5F2A35A__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"
#include "mono_UiUtilities.h"

typedef Array< float > snap_map_t;

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

// TODO make a class as argument which hold all the init data, so you dont have to change the ui file again and again
enum {
    NO_TOP_BUTTON_AMP = -3,
    FIXED_TOP_BUTTON_COLOUR = -2
};
struct ModulationSliderConfigBase {
    virtual StringRef get_bottom_button_text() const {
        return "";
    }
    virtual bool get_is_bottom_button_text_dynamic() const {
        return false;
    }
    virtual mono_ParameterCompatibilityBase* get_parameter_compatibility_base() const {
        return nullptr;
    }
    virtual mono_ParameterCompatibilityBase* get_modulation_parameter_compatibility_base() const {
        return get_parameter_compatibility_base();
    }

    virtual StringRef get_botton_button_switch_text() const {
        return "";
    }

    virtual StringRef get_top_button_text() const {
        return "";
    }
    virtual mono_ParameterCompatibilityBase* get_button_parameter_compatibility_base() const {
        return nullptr;
    }
    virtual float get_top_button_amp() const {
        return NO_TOP_BUTTON_AMP;
    }
    virtual StringArray get_modulation_source_list() const {
        return StringArray();
    }

    virtual bool is_modulation_slider_centered() const {
        return false;
    }

    virtual const snap_map_t* get_snap_map() const {
        return nullptr;
    }

    virtual float get_override_min_value() const {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    virtual float get_override_max_value() const {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    virtual bool get_is_linear() const {
        return false;
    }
    enum SHOW_TYPES {
        DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE,
        SHOW_OWN_VALUE
    };
    virtual SHOW_TYPES show_slider_value_on_top_on_change() const {
        return DEFAULT_SHOW_SLIDER_VAL_ON_CHANGE;
    }
    virtual String get_top_value() const {
        return "";
    }
    virtual StringRef get_top_suffix() const {
        return "";
    }
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

    ParameterReference _parameter;
    ParameterReference _modulator_parameter;
    bool _modulator_is_own_parameter;
    ParameterReference _button_parameter;

    bool show_value_popup;
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
    ScopedPointer<TextButton> button_switch;
    ScopedPointer<Left2MiddleSlider> slider_modulation;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> button_modulator;
    ScopedPointer<Label> label_top;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ModulationSlider)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_A80E4E78D5F2A35A__
