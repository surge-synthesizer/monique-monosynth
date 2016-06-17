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

#ifndef __JUCE_HEADER_CB44C34F016734A4__
#define __JUCE_HEADER_CB44C34F016734A4__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

// TODO
#define DONT_OVERRIDE_SLIDER_VALUE -99999

class Monique_Ui_DualSlider;
class SnapSlider : public Slider
#ifdef JUCE_IOS
    , public Timer
#endif
{

    friend class Monique_Ui_DualSlider;
    Monique_Ui_DualSlider* owner;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;

#ifdef JUCE_IOS
    // TOOLTOP ON LONG MOUSE DOWN
    //ScopedPointer<TooltipWindow> force_tip;
    Point<int> in_mouse_point;
    void mouseDown (const MouseEvent& event) override
    {
        in_mouse_point = Point<int>(this->getScreenX() + this->getWidth()*0.5, this->getScreenY() + this->getHeight()*0.5);
        Slider::mouseDown( event );
        Timer::startTimer(2000);
    }
    void mouseUp (const MouseEvent& event) override
    {
        Timer::stopTimer();
        Slider::mouseUp( event );
    }

    void timerCallback() override
    {
        const Point<int> new_pos = Desktop::getInstance().getMousePosition();
        if( ( new_pos.getX() > 0.97*in_mouse_point.getX() and new_pos.getX() < 1.03*in_mouse_point.getX() )
                and ( new_pos.getY() > 0.97*in_mouse_point.getY() and new_pos.getY() < 1.03*in_mouse_point.getY() ) )
        {
            showPopupMenu();
	    Timer::stopTimer();
	    return;
        }
        /*
            if( not force_tip )
            {
                force_tip = new TooltipWindow( nullptr, 5 );
            }
            const Point<int> new_pos = Desktop::getInstance().getMousePosition();
            if( ( new_pos.getX() > in_mouse_point.getX()*0.9 or new_pos.getX() < in_mouse_point.getX()*1.1 )
                    and ( new_pos.getY() > in_mouse_point.getY()*0.9 or new_pos.getY() < in_mouse_point.getY()*1.1 ) )
            {
                force_tip->force_for_component( this );
                force_tip->displayTip( in_mouse_point, this->getTooltip() );
            }
            */
    }
#endif

public:
    SnapSlider( const String& name_ ) : Slider( name_ ) { }
};

// THIS SLIDER IS LEFT TO RIGHT IF THE NAME IS "3"
class Left2MiddleSlider : public Slider
{
    Slider*_peer_behind;

    friend class Monique_Ui_DualSlider;
    Monique_Ui_DualSlider* owner;

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

class EventButton : public TextButton
{
    friend class Monique_Ui_DualSlider;
    friend class Monique_Ui_Mainwindow;
    Monique_Ui_DualSlider* owner;
    Monique_Ui_Mainwindow*main_window;

    void mouseDown (const MouseEvent& event) override;
    void mouseUp (const MouseEvent& event) override;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    EventButton( const String& name_ ) : TextButton( name_ ), main_window(nullptr) { }
};
class BottomButton : public TextButton
{
    friend class Monique_Ui_DualSlider;
    Monique_Ui_DualSlider* owner;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    BottomButton( const String& name_ ) : TextButton( name_ ) { }
};
class Labels : public Label
{
    friend class Monique_Ui_DualSlider;
    Monique_Ui_DualSlider* owner;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    Labels( const String& name_, const String& text_ ) : Label( name_, text_ ) { }
};
enum
{
    TOP_BUTTON_IS_ON = -4,
    TOP_BUTTON_IS_OFF = -5,
    NO_TOP_BUTTON_AMP = -3,
    FIXED_TOP_BUTTON_COLOUR = -2,
};

struct ModulationSliderConfigBase
{
    virtual bool is_opaque() const noexcept
    {
        return true;
    }

    //==============================================================================
    // BASIC SLIDER TYPE
    virtual bool get_is_linear() const noexcept
    {
        return false;
    }
    virtual bool action_keep_env_pop_open_for( const ENVData*const env_ ) const noexcept
    {
        return false;
    }
    virtual bool action_keep_env_pop_open_for( const LFOData*const mfo_ ) const noexcept
    {
        return false;
    }
    virtual bool use_click_through_hack() const noexcept
    {
        return false;
    }
    virtual COLOUR_THEMES get_colour_theme() const noexcept
    {
        return COLOUR_THEMES::FILTER_THEME;
    }
    enum SWITCHES
    {
        DONT_SWITCH,
        SWITCH_TO_FX,
        SWITCH_TO_EQ
    };
    virtual SWITCHES get_switch_info() const noexcept
    {
        return SWITCHES::DONT_SWITCH;
    }

    //==============================================================================
    // OPTION POPUP
    virtual BoolParameter* get_top_button_option_param_a() noexcept
    {
        return nullptr;
    }
    virtual BoolParameter* get_top_button_option_param_b() noexcept
    {
        return nullptr;
    }
    virtual StringRef get_top_button_option_param_a_text() const noexcept
    {
        return "";
    }
    virtual StringRef get_top_button_option_param_b_text() const noexcept
    {
        return "";
    }
    virtual StringRef get_top_button_option_param_a_tool_tip() const noexcept
    {
        return "";
    }
    virtual StringRef get_top_button_option_param_b_tool_tip() const noexcept
    {
        return "";
    }

    //==============================================================================
    // FRONT SLIDER
    virtual SLIDER_STYLES get_front_slider_style() const noexcept
    {
        return VALUE_SLIDER;
    }
    virtual Parameter* get_front_parameter_base() const noexcept = 0;
    virtual int get_override_front_min_value() const noexcept
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    virtual int get_override_front_max_value() const noexcept
    {
        return DONT_OVERRIDE_SLIDER_VALUE;
    }
    virtual bool get_override_front_screw_value() const noexcept
    {
        return false;
    }

    //==============================================================================
    // BACK SLIDER
    virtual SLIDER_STYLES get_back_slider_style() const noexcept
    {
        return MODULATION_SLIDER;
    }
    // JUST RETURN THE FRONT PARAM IF YOU LIKT TO SET THE BACK AS MODULATION SLIDER
    virtual Parameter* get_back_parameter_base() const noexcept
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
    virtual BoolParameter* get_top_button_parameter_base() const noexcept
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
    virtual bool has_click_impl() const noexcept
    {
        return false;
    }
    virtual void on_click() noexcept {}

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
        SHOW_OWN_VALUE,
        SHOW_OWN_VALUE_ALWAYS
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
    virtual float get_label_edit_value( float entered_value_ ) const noexcept
    {
        return entered_value_;
    }

    //==============================================================================
    // TOOLTIP
    virtual StringRef get_tootip_front() const noexcept
    {
        return "";
    }
    virtual StringRef get_tootip_top() const noexcept
    {
        return "";
    }
    virtual StringRef get_tootip_bottom() const noexcept
    {
        return "";
    }
    virtual StringRef get_tootip_back() const noexcept
    {
        return "";
    }

protected:
    ModulationSliderConfigBase() {}

public:
    virtual ~ModulationSliderConfigBase() {}
};

class MoniqueSynthData;
class MoniqueAudioProcessor;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_DualSlider  : public Component,
    public Monique_Ui_Refreshable,
    public ButtonListener,
    public SliderListener,
    public LabelListener
{
public:
    //==============================================================================
    Monique_Ui_DualSlider (Monique_Ui_Refresher*ui_refresher_, ModulationSliderConfigBase* config_);
    ~Monique_Ui_DualSlider();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool is_opaque;
    bool is_linear;
    bool is_in_shift_view() const;
    void set_shift_view_mode( bool mode_ );
    void show_view_mode();
    void update_return_values() noexcept;
    bool force_repaint;
    bool force_show_center_value;

    MoniqueAudioProcessor* audio_processor;

    BoolParameter* get_top_parameter() noexcept { return top_parameter; }
    Parameter* get_front_parameter() noexcept { return front_parameter; }
    Parameter* get_back_parameter() noexcept { return back_parameter; }
    Parameter* get_parameter( const Slider*slider_ ) noexcept
    {
        if( slider_ == slider_value )
        {
            return front_parameter;
        }
        else if( slider_ == slider_modulation )
        {
            return back_parameter;
        }

        return nullptr;
    }

    const float original_w;
    const float original_h;

    ModulationSliderConfigBase*const _config;
    SectionTheme*theme;

    EventButton* get_top_button() noexcept
    {
        return button_top;
    }
private:
    Parameter* front_parameter;
    Parameter* modulation_parameter;
    Parameter* back_parameter;
    ModulationSliderConfigBase::TOP_BUTTON_TYPE top_button_type;
    BoolParameter* top_parameter;

    BoolParameter* opt_a_parameter;
    BoolParameter* opt_b_parameter;

    bool runtime_show_value_popup;
    bool last_runtime_show_value_popup;
    ModulationSliderConfigBase::SHOW_TYPES show_value_popup_type;
    float last_value;
    float last_painted_value_slider_val;
    float last_painted_mod_slider_val;

    void refresh() noexcept;

    // HACK
    void sliderClicked (Slider*s_) override;
    // EO HACK
#ifdef IS_PLUGIN
    void sliderDragStarted (Slider*) override;
    void sliderDragEnded (Slider*) override;
#endif

public:
    void sliderValueEnter (Slider*s_);
    void sliderValueExit (Slider*s_);
    void sliderModEnter (Slider*s_);
    void sliderModExit (Slider*s_);
    void topButtonEnter (Component*a_);
    void topButtonExit (Component*b_);

    void mouseDown (const MouseEvent& event) override;
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
public:
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void labelTextChanged (Label* labelThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
public:
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<BottomButton> button_bottom;
    ScopedPointer<Left2MiddleSlider> slider_modulation;
    ScopedPointer<Labels> label;
    ScopedPointer<EventButton> button_top;
    ScopedPointer<SnapSlider> slider_value;
    ScopedPointer<Labels> label_top;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_DualSlider)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_CB44C34F016734A4__
