/*
  ==============================================================================

    parameters.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONO_PARAMETER_H_INCLUDED
#define MONO_PARAMETER_H_INCLUDED

#include "App_h_includer.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

#define DEFAULT_MODULATION 0.2

// ==============================================================================
// ==============================================================================
// ==============================================================================
template<typename T>
class mono_ParameterBase;

// TODO listener for modulators
template<typename T>
class mono_ParameterListener {
    friend class mono_ParameterBase< T >;
    virtual void parameter_value_changed( mono_ParameterBase< T >* param_ ) noexcept = 0;
    virtual void parameter_value_changed_always_notification( mono_ParameterBase< T >* param_ ) noexcept {};
    virtual void parameter_modulation_value_changed( mono_ParameterBase< T >* param_ ) noexcept {};
    virtual void parameter_value_on_load_changed( mono_ParameterBase< T >* param_ ) noexcept {};

protected:
    NOINLINE mono_ParameterListener();
    NOINLINE ~mono_ParameterListener();
};

template<typename T>
NOINLINE mono_ParameterListener<T>::mono_ParameterListener() {};
template<typename T>
NOINLINE mono_ParameterListener<T>::~mono_ParameterListener() {};

// ==============================================================================
// ==============================================================================
// ==============================================================================
enum
{
    HAS_NO_MODULATION = -9999,

    MODULATION_AMOUNT_MIN = -100,
    MODULATION_AMOUNT_MAX = 100
};

class mono_ParameterCompatibilityBase;
class MIDIControl : mono_ParameterListener< float >, mono_ParameterListener< int >, mono_ParameterListener< bool >
{
public:
    // TODO use from mono_audiodevicemanager
    enum TYPES {
        NOT_SET = -1,
        NOTE = 1,
        CC = 0
    };

private:
    friend class mono_ParameterCompatibilityBase;
    int listen_type;
    int channel;
    int8 midi_number; // NOTES OR CC
    String is_ctrl_version_of_name;
    bool is_in_ctrl_mode;
    mono_ParameterCompatibilityBase*const owner;

public:
    int get_listen_type() const noexcept {
        return listen_type;
    }
    int8 get_midi_number() const noexcept {
        return midi_number;
    }
    int get_chnanel() const noexcept {
        return channel;
    }
    const String& get_is_ctrl_version_of_name() const noexcept {
        return is_ctrl_version_of_name;
    }

    bool is_listen_to( MidiMessage& input_message_ ) const noexcept;
    bool read_from_if_you_listen( const MidiMessage& input_message_ ) noexcept;
    bool train( const MidiMessage& input_message_, mono_ParameterCompatibilityBase*const is_ctrl_version_of_ ) noexcept;
    bool train( int listen_type_, int8 midi_number, int channel_, String is_ctrl_version_of_name_ ) noexcept;
    bool is_valid_trained() const noexcept;
    void send_feedback_only() const noexcept;
    void send_clear_feedback_only() const noexcept;

    void set_ctrl_mode( bool mode_ ) noexcept;
    inline bool get_ctrl_mode() const noexcept {
        return is_in_ctrl_mode;
    }

private:
    // FEEDBACK
    void start_listen_for_feedback() noexcept;
    void stop_listen_for_feedback() noexcept;
    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_modulation_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_changed( mono_ParameterBase< bool >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< bool >* param_ ) noexcept override;
    void parameter_value_changed( mono_ParameterBase< int >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< int >* param_ ) noexcept override;
    void generate_feedback_message( MidiMessage& ) const noexcept;
    void generate_modulation_feedback_message( MidiMessage& ) const noexcept;
    void send_standard_feedback() const noexcept;
    void send_modulation_feedback() const noexcept;

public:
    NOINLINE MIDIControl( mono_ParameterCompatibilityBase*const owner_ );
    NOINLINE ~MIDIControl();

    void clear();
};
class MIDIControlHandler {
    bool is_activated_and_waiting_for_param;
    mono_ParameterCompatibilityBase* learning_param;
    mono_ParameterCompatibilityBase* learning_ctrl_param;

    Array< Component* > learning_comps;

public:
    void toggle_midi_learn();
    bool is_waiting_for_param() const;
    void set_learn_param( mono_ParameterCompatibilityBase* param_ );
    void set_learn_width_ctrl_param( mono_ParameterCompatibilityBase* param_, mono_ParameterCompatibilityBase* ctrl_param_, Component* comp_ );
    void set_learn_param( mono_ParameterCompatibilityBase* param_, Component* comp_ );
    void set_learn_param( mono_ParameterCompatibilityBase* param_, Array< Component* > comps_ );
    mono_ParameterCompatibilityBase* is_learning() const;
    bool handle_incoming_message( const MidiMessage& input_message_ );
    void clear();

    MIDIControlHandler();

    juce_DeclareSingleton (MIDIControlHandler,false)
};
#define IF_MIDI_LEARN__HANDLE( param ) \
        if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() ) \
        { \
            MIDIControlHandler::getInstance()->set_learn_param( param ); \
        }
#define IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT( param, component_ ) \
        if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() ) \
        { \
            MIDIControlHandler::getInstance()->set_learn_param( param, component_ ); \
        }
#define IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT( param, param_ctrl, component_ ) \
        if( MIDIControlHandler::getInstance()->is_waiting_for_param() || MIDIControlHandler::getInstance()->is_learning() ) \
        { \
            MIDIControlHandler::getInstance()->set_learn_width_ctrl_param( param, param_ctrl, component_ ); \
        }
#define SET_COMPONENT_TO_MIDI_LEARN( comp ) \
        { \
	  UiLookAndFeel::getInstance()->midi_learn_comp = comp; \
	  comp->repaint(); \
	}
#define UNSET_COMPONENT_MIDI_LEARN( comp ) \
        { \
	  UiLookAndFeel::getInstance()->midi_learn_comp = nullptr; \
	  comp->repaint(); \
	}
// ==============================================================================
enum TYPES_DEF {
    IS_FLOAT = 0,
    IS_BOOL = 1,
    IS_INT = 2,
    UNKNOWN_TYPE = 3,
};
template<class T>
struct simple_type_info {
    inline int type() const {
        return UNKNOWN_TYPE;
    }
};
template<>
struct simple_type_info<bool> {
    inline int type() const {
        return IS_BOOL;
    }
};
template<>
struct simple_type_info<int> {
    inline int type() const {
        return IS_INT;
    }
};
template<>
struct simple_type_info<float> {
    inline int type() const {
        return IS_FLOAT;
    }
};
// ==============================================================================
// ==============================================================================
// ==============================================================================
template<typename T>
class mono_ParameterListener;

struct mono_ParameterCompatibilityBase {
    virtual float get_scaled_value() = 0;
    virtual void set_scaled_value( float ) = 0;
    virtual void toggle_value() = 0;

    // NEEDED FOR SET BY USER
    NOINLINE virtual float slider_interval() const noexcept = 0;
    NOINLINE virtual int min_unscaled() const noexcept = 0;
    NOINLINE virtual int max_unscaled() const noexcept = 0;
    NOINLINE virtual int reset_unscaled() const noexcept = 0;
    NOINLINE virtual int scale() const noexcept = 0;

    // RETURNS HAS_NO_MODULATION (-1) if the function isn't overwritten
    NOINLINE virtual float get_modulation_amount() const noexcept;
    virtual void set_modulation_amount(float) noexcept {};
    virtual void set_modulation_amount_without_notification(float) noexcept {}
    NOINLINE virtual float get_last_modulation_amount() const noexcept;
    virtual bool has_modulation() const noexcept {
        return false;
    }

    // SAVE AND RESTORE
    NOINLINE virtual void write_to( XmlElement& xml_ ) const noexcept {};
    NOINLINE virtual void read_from( const XmlElement& xml_ ) noexcept {};
    NOINLINE virtual void write_midi_to( XmlElement& xml_ ) const noexcept {};
    NOINLINE virtual void read_midi_from( const XmlElement& xml_ ) noexcept {};

    // PLUGINPROCESSOR AUTOMATIONS
    NOINLINE virtual const char* get_name() const noexcept = 0;
    NOINLINE virtual const char* get_short_name() const noexcept = 0;
    virtual float get_float_percent_value() const noexcept = 0;
    virtual void set_float_percent_value( float percent_ ) noexcept = 0;
    NOINLINE virtual float get_float_percent_dafault_value() const noexcept = 0;
    NOINLINE virtual int get_num_steps() const noexcept = 0;

    // MIDI CONTROLL
    ScopedPointer<MIDIControl> midi_control;
    virtual int get_type() const noexcept = 0;

protected:
    NOINLINE mono_ParameterCompatibilityBase() noexcept;
    NOINLINE ~mono_ParameterCompatibilityBase() noexcept;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( mono_ParameterCompatibilityBase )
};

// ==============================================================================
#define PARAM_CHANGE_INTERVAL_IN_MS 20
class ChangeParamOverTime : public Timer {
    mono_ParameterBase< float >& _param;
    int _sum_callbacks;
    const float _target_value;
    const float _value_delta;

    void timerCallback();
    void change();
    ChangeParamOverTime( mono_ParameterBase< float >& param_, float target_value_, int time_in_ms_ );

public:
    void forceStopAndKill();

    static void execute( mono_ParameterBase< float >& param_, float target_value_, int time_in_ms_ ) {
        new ChangeParamOverTime(param_,target_value_,time_in_ms_);
    }
};
template<typename T>
class mono_ParameterBase : public mono_ParameterCompatibilityBase, public simple_type_info<T> {
protected:
    T value;

public:
    inline operator T() const noexcept {
        return value;
    }
    // TODO, we dont need a return here.
    inline T operator^=( bool ) noexcept {
        value = !value;
        notify_value_listeners();

        return value;
    }
    inline const T& get_reference() const noexcept {
        return value;
    }
    // TODO, we dont need a return here.
    inline T set_scaled( T value_ ) noexcept {
        value = value_;
        notify_value_listeners();

        return value_;
    }
    virtual T get_min() noexcept { return 0; }
    virtual T get_max() noexcept { return 0; }

    inline T set_scaled_without_notification( T value_ ) noexcept {
        value = value_;
        notify_always_value_listeners();

        return value_;
    }

    int get_type() const noexcept override {
        return simple_type_info< T >::type();
    }

    // ---------------------------------------
private:
    Array< mono_ParameterListener< T >* > listeners;
    Array< mono_ParameterListener< T >* > always_listeners;
    inline void notify_value_listeners() {
        for( int i = 0 ; i != listeners.size() ; ++i ) {
            listeners.getUnchecked(i)->parameter_value_changed( this );
        }
    }
protected:
    inline void notify_modulation_value_listeners() {
        for( int i = 0 ; i != listeners.size() ; ++i ) {
            listeners.getUnchecked(i)->parameter_modulation_value_changed( this );
        }
    }
    inline void notify_on_load_value_listeners() {
        for( int i = 0 ; i != listeners.size() ; ++i ) {
            listeners.getUnchecked(i)->parameter_value_on_load_changed( this );
        }
    }
    inline void notify_always_value_listeners() {
        for( int i = 0 ; i != always_listeners.size() ; ++i ) {
            always_listeners.getUnchecked(i)->parameter_value_changed_always_notification( this );
        }
    }
public:
    NOINLINE void register_listener( mono_ParameterListener< T >*const listener_ );
    NOINLINE void register_always_listener( mono_ParameterListener< T >*const listener_ );
    NOINLINE void remove_listener( mono_ParameterListener< T >*const listener_ );

protected:
    NOINLINE explicit mono_ParameterBase( T init_value_ );
    NOINLINE ~mono_ParameterBase();

private:
    NOINLINE mono_ParameterBase();

    friend class ChangeParamOverTime;
    ChangeParamOverTime* timeChanger;
public:
    void stopTimeChanger();
};

template<typename T>
NOINLINE mono_ParameterBase<T>::mono_ParameterBase( T init_value_ ) : value( init_value_ ), timeChanger( nullptr ) {}
template<typename T>
NOINLINE mono_ParameterBase<T>::~mono_ParameterBase() {
    stopTimeChanger();
}

template<typename T>
NOINLINE void mono_ParameterBase<T>::register_listener( mono_ParameterListener< T >*const listener_ ) {
    if( ! listeners.contains( listener_ ) ) {
        listeners.add( listener_ );
    }
}
template<typename T>
NOINLINE void mono_ParameterBase<T>::register_always_listener( mono_ParameterListener< T >*const listener_ ) {
    if( ! always_listeners.contains( listener_ ) ) {
        always_listeners.add( listener_ );
        listeners.add( listener_ );
    }
}
template<typename T>
NOINLINE void mono_ParameterBase<T>::remove_listener( mono_ParameterListener< T >*const listener_ ) {
    listeners.removeFirstMatchingValue( listener_ );
    always_listeners.removeFirstMatchingValue( listener_ );
}

template<typename T>
void mono_ParameterBase<T>::stopTimeChanger() {
    if( timeChanger )
        timeChanger->forceStopAndKill();
}

// ==============================================================================
// ==============================================================================
// ==============================================================================
#define MONO_PARAMETER_TEMPLATE_DECLARATION_INIT \
typename T,					\
         int init_value_ = false,		\
         int min_ = false,			\
         int max_ = true,			\
         int scale_factor = 1,			\
         int interval = 1000 /* *0.001 */

#define MONO_PARAMETER_TEMPLATE_DECLARATION 	\
typename T, 					\
int init_value_, 				\
int min_, int max_, 				\
int scale_factor, 				\
int interval /* /1000 */

#define MONO_PARAMETER_TEMPLATE_DEFINITION	\
T, 						\
init_value_, 					\
min_, 						\
max_, 						\
scale_factor, 					\
interval

// ==============================================================================
template<MONO_PARAMETER_TEMPLATE_DECLARATION_INIT>
class mono_Parameter : public mono_ParameterBase< T > {
public:
    typedef mono_ParameterBase< T > parameter_base_t;
    typedef T type;

    static constexpr int MIN_UNSCALED = min_;
    static constexpr int MAX_UNSCALED = max_;
    static constexpr int SCALE = scale_factor;
#ifdef CONSTEXPR_SUPPORT
    static constexpr T MIN_SCALED = T(MIN_UNSCALED)/SCALE;
    static constexpr T MAX_SCALED = T(MAX_UNSCALED)/SCALE;
    static constexpr T INIT_SCALED = T(init_value_)/SCALE;
    static constexpr T NUM_STEPS = T(T(MAX_UNSCALED)-T(MIN_UNSCALED))*SCALE;
#else
    static const T MIN_SCALED;
    static const T MAX_SCALED;
    static const T INIT_SCALED;
    static const T NUM_STEPS;
#endif

    using parameter_base_t::operator T;
    using parameter_base_t::operator=;
    using parameter_base_t::operator^=;
    using parameter_base_t::get_reference;

    inline T operator=( T value_ ) noexcept {
        if( value_ > MAX_SCALED ) {
            DBG( "ERROR: set to big value: " << get_short_name() << "->" << value_ << " max:"<<  MAX_SCALED );
            value_ = MAX_SCALED;
        }
        else if( value_ < MIN_SCALED ) {
            DBG( "ERROR: set to small value: " << get_short_name() << "->" << value_ << " min:"<<  MIN_SCALED  );
            value_ = MIN_SCALED;
        }

        parameter_base_t::set_scaled( value_ );

        return value_;
    }
    inline const mono_Parameter& operator=( const mono_Parameter& other_ ) noexcept {
        parameter_base_t::set_scaled( other_.value );

        return *this;
    }

    parameter_base_t* get_base() noexcept {
        return static_cast< parameter_base_t* >( this );
    }

    T get_min() noexcept override {
        return MIN_SCALED;
    }
    T get_max() noexcept override {
        return MAX_SCALED;
    }

    void toggle_value() override {
        if( parameter_base_t::value == MIN_SCALED )
            parameter_base_t::set_scaled( MAX_SCALED );
        else
            parameter_base_t::set_scaled( MIN_SCALED );
    }

    // PLUGINPROCESSOR AUTOMATIONS
    float get_float_percent_value() const noexcept override;
    void set_float_percent_value( float percent_ ) noexcept override;
    NOINLINE float get_float_percent_dafault_value() const noexcept override;
    NOINLINE int get_num_steps() const noexcept override;
    NOINLINE const char* get_name() const noexcept override;
    NOINLINE const char* get_short_name() const noexcept override;

    // TODO SET FROM NORMALIZED

private:
    NOINLINE float slider_interval() const noexcept override;
    NOINLINE int min_unscaled() const noexcept override;
    NOINLINE int max_unscaled() const noexcept override;
    NOINLINE int reset_unscaled() const noexcept override;
    NOINLINE int scale() const noexcept override;

    NOINLINE float get_scaled_value() noexcept override;
    NOINLINE void set_scaled_value( float v_ ) noexcept override;
public:
    const String name;
    const String short_name;

    NOINLINE mono_Parameter( const String& name_, const String& short_name_ ) noexcept;
    NOINLINE ~mono_Parameter() noexcept;

    NOINLINE void write_to( XmlElement& xml_ ) const noexcept override;
    NOINLINE void read_from( const XmlElement& xml_ ) noexcept override;
    NOINLINE void write_midi_to( XmlElement& xml_ ) const noexcept override;
    NOINLINE void read_midi_from( const XmlElement& xml_ ) noexcept override;

private:
    MONO_NOT_CTOR_COPYABLE( mono_Parameter )
    MONO_NOT_MOVE_COPY_OPERATOR( mono_Parameter )
    NOINLINE mono_Parameter();
};
#ifndef CONSTEXPR_SUPPORT
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
const T mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::MIN_SCALED = T(MIN_UNSCALED)/SCALE;

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
const T mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::MAX_SCALED = T(MAX_UNSCALED)/SCALE;

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
const T mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::INIT_SCALED = T(init_value_)/SCALE;

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
const T mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::NUM_STEPS = T(T(MAX_UNSCALED)-T(MIN_UNSCALED))*SCALE;
#endif
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE int mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::min_unscaled() const noexcept {
    return min_;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE int mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::max_unscaled() const noexcept {
    return max_;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE int mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::reset_unscaled() const noexcept {
    return init_value_;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE int mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::scale() const noexcept {
    return scale_factor;
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE float mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::slider_interval() const noexcept {
    return float(interval)/1000;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE float mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_scaled_value() noexcept {
    return parameter_base_t::value;
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::set_scaled_value( float v_ ) noexcept {
    operator=( v_ );
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
float mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_float_percent_value() const noexcept {
    return 1.0f/(MAX_SCALED-MIN_SCALED)*(parameter_base_t::value-MIN_SCALED);
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::set_float_percent_value( float percent_ ) noexcept {
    operator=( (MAX_SCALED-MIN_SCALED)*percent_ + MIN_SCALED );
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE float mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_float_percent_dafault_value() const noexcept {
    return INIT_SCALED;
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE int mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_num_steps() const noexcept {
    return NUM_STEPS;
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE const char* mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_name() const noexcept {
    return name.toRawUTF8();
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE const char* mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_short_name() const noexcept {
    return short_name.toRawUTF8();
}


template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::mono_Parameter( const String& name_, const String& short_name_ ) noexcept
:
parameter_base_t( T(init_value_)/scale_factor ), name( name_ ), short_name(short_name_)
{}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::~mono_Parameter() noexcept {}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::write_to( XmlElement& xml_ ) const noexcept {
    if( parameter_base_t::value != INIT_SCALED )
        xml_.setAttribute( name, parameter_base_t::value );
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::read_from( const XmlElement& xml_ ) noexcept {
    T new_value = xml_.getDoubleAttribute( name, T(init_value_)/scale_factor );
    if( new_value > MAX_SCALED ) {
        new_value = MAX_SCALED;
    }
    else if( new_value < MIN_SCALED ) {
        new_value = MIN_SCALED;
    }
    parameter_base_t::set_scaled_without_notification ( new_value );
    parameter_base_t::notify_on_load_value_listeners();
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::write_midi_to( XmlElement& xml_ ) const noexcept {
    if( parameter_base_t::midi_control->get_listen_type() != MIDIControl::NOT_SET )
        xml_.setAttribute( name + "_MIDI_T", parameter_base_t::midi_control->get_listen_type() );
    if( parameter_base_t::midi_control->get_midi_number() != -1 )
        xml_.setAttribute( name + "_MIDI_NR", parameter_base_t::midi_control->get_midi_number() );
    if( parameter_base_t::midi_control->get_chnanel() != -1 )
        xml_.setAttribute( name + "_MIDI_CH", parameter_base_t::midi_control->get_chnanel() );
    if( parameter_base_t::midi_control->get_is_ctrl_version_of_name() != "" )
        xml_.setAttribute( name + "_MIDI_CTRL", parameter_base_t::midi_control->get_is_ctrl_version_of_name() );
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>::read_midi_from( const XmlElement& xml_ ) noexcept {
    parameter_base_t::midi_control->train
    (
        xml_.getIntAttribute( name + "_MIDI_T", MIDIControl::NOT_SET ),
        xml_.getIntAttribute( name + "_MIDI_NR", -1 ),
        xml_.getIntAttribute( name + "_MIDI_CH", -1 ),
        xml_.getStringAttribute( name + "_MIDI_CTRL", "" )
    );
}

// ==============================================================================
// ==============================================================================
// ==============================================================================
NOINLINE static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept {
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_ + String("_") + String(param_id_);
}
NOINLINE static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept {
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept {
    return owner_class + String("_") + String(owner_id_+1) + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, const String& param_name_ ) noexcept {
    return owner_class + String("_") + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& param_name_ ) noexcept {
    return param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, const String& param_name_, int param_id_ ) noexcept {
    return owner_class + String("_") + param_name_ + String("_") + String(param_id_+1);
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept {
    return owner_class + String("_") + String(owner_id_+1) + String("_") + param_name_ + String("_") + String(param_id_+1);
}
// ==============================================================================
// ==============================================================================
// ==============================================================================
template<typename mono_Parameter_t>
class mono_GlideParameter {
    int counter;
    float current_value;
    const float& source_value;
    float delta;

public:
    inline float glide_tick() noexcept {
        if( counter )
        {
            --counter;

            if( counter == 0 )
                current_value = source_value;
            else
            {
                current_value+=delta;
                if( current_value > mono_Parameter_t::MAX_SCALED || current_value < mono_Parameter_t::MIN_SCALED )
                {
                    current_value = source_value;
                    counter = 0;
                }
            }
        }

        return current_value;
    }

    inline void update( int n_ ) noexcept {
        if( source_value != current_value )
        {
            counter = n_;
            delta = (source_value-current_value) / counter;
        }
        else
            counter = 0;
    }

    inline float get_current_value() const noexcept {
        return current_value;
    }

    inline void reset() noexcept {
        current_value = source_value;
        delta = 0;
        counter = 0;
    }

    MONO_EMPTY_COPY_OPERATOR( mono_GlideParameter )

    NOINLINE mono_GlideParameter( const mono_Parameter_t& source_ ) noexcept;
    NOINLINE ~mono_GlideParameter() noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( mono_GlideParameter )
    NOINLINE mono_GlideParameter();
};

template<typename mono_Parameter_t>
NOINLINE mono_GlideParameter<mono_Parameter_t>
::mono_GlideParameter( const mono_Parameter_t& source_ ) noexcept :
counter(0),
        current_value(source_),
        source_value(source_.get_reference()),
        delta(0)
{}
template<typename mono_Parameter_t>
NOINLINE mono_GlideParameter<mono_Parameter_t>::~mono_GlideParameter() noexcept {}

// ==============================================================================
// ==============================================================================
// ==============================================================================
class mono_ParameterGlideBase {
    inline virtual void update( int n_ ) noexcept = 0;
    inline virtual float tick() noexcept = 0;
    inline virtual float get_last_tick() const noexcept = 0;
};

template<MONO_PARAMETER_TEMPLATE_DECLARATION_INIT>
class mono_ParameterGlide :
    public mono_ParameterGlideBase,
    public mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION>
{
protected:
    typedef mono_Parameter<MONO_PARAMETER_TEMPLATE_DEFINITION> parameter_t;
    typedef typename parameter_t::parameter_base_t parameter_base_t;
    typedef mono_GlideParameter< parameter_t > glide_parameter_t;

    glide_parameter_t parameter_glide;
    float last_tick_value;
    bool is_changed_since_last_tick;

public:
    inline virtual void update( int n_ ) noexcept override {
        //  hier müssen wir ein snap value einführen, sonst wird ewig geklitten wenn wir aller 512 samples ein update machen,
        //  dazu den letzten wert fürs update merken und schauen ob wir das update für diese änderung bereits durchgeführt haben
        parameter_glide.update(n_);
    }
    inline virtual float tick() noexcept override {
        return update_last_value( parameter_glide.glide_tick() );
    }
    inline void reset() noexcept {
        parameter_glide.reset();
        last_tick_value = parameter_glide.get_current_value();
        is_changed_since_last_tick = false;

    }
    inline virtual float get_last_tick() const noexcept override {
        return last_tick_value;
    }
protected:
    inline float update_last_value( float new_value_ ) noexcept {
        if( new_value_ != last_tick_value )
            is_changed_since_last_tick = true;
        else
            is_changed_since_last_tick = false;

        return last_tick_value = new_value_;
    }

public:
    inline bool is_output_changed_since_last_tick( ) const {
        return is_changed_since_last_tick;
    }

    inline const mono_ParameterGlide& operator=( const mono_ParameterGlide& other_ ) noexcept {
        parameter_t::operator=( other_.parameter_base_t::value );

        return *this;
    }
    inline const mono_ParameterGlide& operator=( const T v_ ) noexcept {
        parameter_t::operator=( v_ );

        return *this;
    }
    using parameter_t::operator T;
    using parameter_t::operator ^=;
    using parameter_t::get_reference;
    using parameter_t::get_base;

public:
    NOINLINE mono_ParameterGlide( const String& name_, const String& short_name_  ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( mono_ParameterGlide )
    MONO_NOT_MOVE_COPY_OPERATOR( mono_ParameterGlide )
    mono_ParameterGlide();
};
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE mono_ParameterGlide<MONO_PARAMETER_TEMPLATE_DEFINITION>::mono_ParameterGlide( const String& name_, const String& short_name_  ) noexcept
:
parameter_t( name_, short_name_ ),
             parameter_glide( *static_cast< parameter_t* >(this) ),
             last_tick_value(0),
             is_changed_since_last_tick( true )
{}

// ==============================================================================
// ==============================================================================
// ==============================================================================
template<int min_, int max_>
class mono_GlideValue {
    int counter;
    float current_value;
    float value;
    float delta;

public:
    inline float glide_tick() noexcept {
        if( counter )
        {
            --counter;

            if( !counter )
                current_value = value;
            else
            {
                current_value+=delta;
                if( current_value > max_ || current_value < min_ )
                {
                    current_value = value;
                    counter = 0;
                }
            }
        }

        return current_value;
    }

    inline void update( int n_ ) noexcept {
        if( value != current_value )
        {
            counter = n_;
            delta = (value-current_value) / n_;
        }
        else
            counter = 0;
    }
    inline bool is_up_to_date() const {
        return counter == 0;
    }

    inline void override_current_value( float current_value_ ) noexcept {
        current_value = current_value_;
    }
    inline float get_last_tick() noexcept {
        return current_value;
    }

    inline const mono_GlideValue& operator=( const mono_GlideValue& other_ ) noexcept {
        value = other_.value;

        return *this;
    }
    inline float operator=( float amount_ ) noexcept {
        value = amount_;

        return amount_;
    }

    inline operator float() const noexcept {
        return value;
    }

    NOINLINE mono_GlideValue(float init_value_=0) noexcept;
    NOINLINE ~mono_GlideValue() noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( mono_GlideValue )
    MONO_NOT_MOVE_COPY_OPERATOR( mono_GlideValue )
};

template<int min_, int max_>
NOINLINE mono_GlideValue<min_,max_>
::mono_GlideValue(float init_value_) noexcept
:
counter(0),
        current_value(init_value_),
        value(init_value_),
        delta(0)
{}
template<int min_, int max_>
NOINLINE mono_GlideValue<min_,max_>::~mono_GlideValue() noexcept {}

// ==============================================================================
class mono_ParameterGlideModulatedBase {
    inline virtual float tick_modulated( float modulator_signal_, bool add_modulation_ = true ) noexcept = 0;
};
template<MONO_PARAMETER_TEMPLATE_DECLARATION_INIT>
class mono_ParameterGlideModulated :
    public mono_ParameterGlide< MONO_PARAMETER_TEMPLATE_DEFINITION >,
    public mono_ParameterGlideModulatedBase
{
    typedef mono_ParameterGlide< MONO_PARAMETER_TEMPLATE_DEFINITION > parameter_glide_t;
    typedef typename parameter_glide_t::parameter_t parameter_t;
    typedef typename parameter_glide_t::parameter_base_t parameter_base_t;
    typedef mono_GlideValue<-1,1> modulation_t;

    modulation_t modulation;
    float last_modulator;

public:
    virtual inline float get_modulation_amount() const noexcept override {
        return modulation;
    }

private:
    virtual void set_modulation_amount( float modulation_amount_ ) noexcept override;
    virtual void set_modulation_amount_without_notification( float modulation_amount_ ) noexcept override;

    NOINLINE virtual float get_last_modulation_amount() const noexcept override;
    bool has_modulation() const noexcept override {
        return true;
    }

public:
    inline void update( int n_ ) noexcept override {
        parameter_glide_t::update(n_);
        modulation.update(n_);
    }
    inline float tick_modulated( float modulator_signal_, bool add_modulation_ = true ) noexcept override {
        float modulator_amount = modulation.glide_tick();
        if( add_modulation_ ) {
            if( modulator_amount >= 0 )
                return tick_to_up( modulator_signal_, modulator_amount );
            else
                return tick_to_down( modulator_signal_, modulator_amount*-1 );
        }
        else
            return parameter_glide_t::tick();
    }

private:
    // IT'S private? YOU SHOULD USE THE GLIDE PARAMETER NOT THE MODULATED ONE
    virtual float tick() noexcept override {
        return parameter_glide_t::tick();
    }

    // TODO is this correct???
    // PLEASE TEST
    inline float tick_to_up( float modulator_signal_, float modulator_amount_ ) noexcept {
        last_modulator = modulator_signal_ * modulator_amount_;
        float glide_value = parameter_glide_t::parameter_glide.glide_tick();
        float modulation_range = (parameter_t::MAX_SCALED-glide_value) * modulator_amount_;
        return parameter_glide_t::update_last_value( glide_value + modulation_range*modulator_signal_ );
    }
    inline float tick_to_down( float modulator_signal_, float modulator_amount_ ) noexcept {
        last_modulator = modulator_signal_*modulator_amount_;
        float glide_value = parameter_glide_t::parameter_glide.glide_tick();
        float modulation_range = (glide_value-parameter_t::MIN_SCALED) * modulator_amount_;
        return parameter_glide_t::update_last_value( glide_value - modulation_range*modulator_signal_ );
    }

public:
    using parameter_glide_t::is_output_changed_since_last_tick;

    inline const mono_ParameterGlideModulated& operator=( const mono_ParameterGlideModulated& other_ ) noexcept {
        parameter_base_t::set_scaled( other_.parameter_base_t::value );
        modulation = other_.modulation;

        return *this;
    }
    inline const mono_ParameterGlideModulated& operator=( const T v_ ) noexcept {
        parameter_t::operator=( v_ );

        return *this;
    }
    using parameter_glide_t::operator T;
    using parameter_glide_t::operator ^=;
    using parameter_glide_t::get_reference;
    using parameter_glide_t::get_base;

public:
    NOINLINE mono_ParameterGlideModulated( const String& name_, const String& short_name_ ) noexcept;
    NOINLINE ~mono_ParameterGlideModulated() noexcept;

private:
    NOINLINE void write_to( XmlElement& xml_ ) const noexcept override;
    NOINLINE void read_from( const XmlElement& xml_ ) noexcept override;
    NOINLINE void write_midi_to( XmlElement& xml_ ) const noexcept override;
    NOINLINE void read_midi_from( const XmlElement& xml_ ) noexcept override;

    MONO_NOT_CTOR_COPYABLE( mono_ParameterGlideModulated )
    MONO_NOT_MOVE_COPY_OPERATOR( mono_ParameterGlideModulated )
    mono_ParameterGlideModulated();
};
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::mono_ParameterGlideModulated( const String& name_, const String& short_name_ ) noexcept
:
parameter_glide_t( name_, short_name_ ),
                   modulation( DEFAULT_MODULATION ),
                   last_modulator(0)
{}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::~mono_ParameterGlideModulated() noexcept {}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::set_modulation_amount( float modulation_amount_ ) noexcept {
    modulation = modulation_amount_;
    parameter_base_t::notify_modulation_value_listeners();
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::set_modulation_amount_without_notification( float modulation_amount_ ) noexcept {
    modulation = modulation_amount_;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE float mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::get_last_modulation_amount() const noexcept {
    return last_modulator;
}

template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::write_to( XmlElement& xml_ ) const noexcept {
    if( parameter_base_t::value != parameter_t::INIT_SCALED )
        xml_.setAttribute( parameter_t::name, parameter_base_t::value );
    if( modulation != DEFAULT_MODULATION )
        xml_.setAttribute( parameter_t::name + String("_mod"), modulation );
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::read_from( const XmlElement& xml_ ) noexcept {
    T new_value = xml_.getDoubleAttribute( parameter_t::name, T(init_value_)/scale_factor );
    if( new_value > parameter_t::MAX_SCALED ) {
        new_value = parameter_t::MAX_SCALED;
    }
    else if( new_value < parameter_t::MIN_SCALED ) {
        new_value = parameter_t::MIN_SCALED;
    }
    parameter_base_t::set_scaled_without_notification ( new_value );

    T new_modulation = xml_.getDoubleAttribute( parameter_t::name + String("_mod"), DEFAULT_MODULATION );
    if( new_modulation > 1 ) {
        new_modulation = 1;
    }
    else if( new_modulation < -1 ) {
        new_modulation = -1;
    }
    modulation = new_modulation;

    update(500);

    parameter_base_t::notify_on_load_value_listeners();
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::write_midi_to( XmlElement& xml_ ) const noexcept {
    if( parameter_base_t::midi_control->get_listen_type() != MIDIControl::NOT_SET )
        xml_.setAttribute( parameter_t::name + "_MIDI_T", parameter_base_t::midi_control->get_listen_type() );
    if( parameter_base_t::midi_control->get_midi_number() != -1 )
        xml_.setAttribute( parameter_t::name + "_MIDI_NR", parameter_base_t::midi_control->get_midi_number() );
    if( parameter_base_t::midi_control->get_chnanel() != -1 )
        xml_.setAttribute( parameter_t::name + "_MIDI_CH", parameter_base_t::midi_control->get_chnanel() );
    if( parameter_base_t::midi_control->get_is_ctrl_version_of_name() != "" )
        xml_.setAttribute( parameter_t::name + "_MIDI_CTRL", parameter_base_t::midi_control->get_is_ctrl_version_of_name() );
}
template<MONO_PARAMETER_TEMPLATE_DECLARATION>
NOINLINE void mono_ParameterGlideModulated<MONO_PARAMETER_TEMPLATE_DEFINITION>::read_midi_from( const XmlElement& xml_ ) noexcept {
    parameter_base_t::midi_control->train
    (
        xml_.getIntAttribute( parameter_t::name + "_MIDI_T", MIDIControl::NOT_SET ),
        xml_.getIntAttribute( parameter_t::name + "_MIDI_NR", -1 ),
        xml_.getIntAttribute( parameter_t::name + "_MIDI_CH", -1 ),
        xml_.getStringAttribute( parameter_t::name + "_MIDI_CTRL", "" )
    );
}
// ==============================================================================
// ==============================================================================
// ==============================================================================
template< class mono_Parameter_t, int size_ >
class mono_ParameterArray {
    OwnedArray< mono_Parameter_t > array;

public:
    typedef mono_Parameter_t mono_Parameter_type;
    typedef typename mono_Parameter_t::type type;
    typedef typename mono_Parameter_t::type array_type[size_];
    enum { SIZE = size_ };

    inline mono_Parameter_t& operator[](int i) noexcept {
        return *array.getUnchecked(i);
    }
    inline const mono_Parameter_t& operator[](int i) const noexcept {
        return *array.getUnchecked(i);
    }
    inline mono_Parameter_t& get(int i) noexcept {
        return *array.getUnchecked(i);
    }
    inline const mono_Parameter_t& get(int i) const noexcept {
        return *array.getUnchecked(i);
    }

    NOINLINE mono_ParameterArray( const String& owner_class_name_,
                                  int owner_id_,
                                  const String& param_name_,
                                  const String& param_name_short_,
                                  bool create_human_id_ = true
                                ) noexcept;
    NOINLINE ~mono_ParameterArray();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ParameterArray)
    NOINLINE mono_ParameterArray();
};

template< class mono_Parameter_t, int size_ >
mono_ParameterArray< mono_Parameter_t,size_ >::mono_ParameterArray(
    const String& owner_class_name_,
    int owner_id_,
    const String& param_name_,
    const String& param_name_short_,
    bool create_human_id_ ) noexcept
{
    for( int i = 0 ; i != SIZE ; ++ i )
        array.add( new mono_Parameter_t(generate_param_name(owner_class_name_,owner_id_,param_name_,i),
        create_human_id_ ? generate_short_human_name(owner_class_name_,owner_id_,param_name_short_,i) : generate_short_human_name(owner_class_name_,param_name_short_,i) ) );

    array.minimiseStorageOverheads();
}

template< class mono_Parameter_t, int size_ >
mono_ParameterArray< mono_Parameter_t,size_ >::~mono_ParameterArray() {}

// ==============================================================================
// ==============================================================================
// ==============================================================================
template< class mono_ParameterArray_t >
class mono_ParameterReferenceArray {
    Array< typename mono_ParameterArray_t::mono_Parameter_type* > array;

public:
    typedef typename mono_ParameterArray_t::mono_Parameter_type mono_Parameter_type;
    typedef typename mono_ParameterArray_t::type type;
    enum { SIZE = mono_ParameterArray_t::SIZE };

    inline mono_Parameter_type& operator[](int i) noexcept {
        return *array.getUnchecked(i);
    }
    inline const mono_Parameter_type& operator[](int i) const noexcept {
        return *array.getUnchecked(i);
    }

    void init( mono_ParameterArray_t& source_array_ ) noexcept {
        for( int i = 0 ; i != SIZE ; ++i ) {
            array.add( &source_array_[i] );
        }
    }

    NOINLINE mono_ParameterReferenceArray() {}
    NOINLINE ~mono_ParameterReferenceArray() {}

private:
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ParameterReferenceArray)

};

#pragma GCC diagnostic pop

#endif



