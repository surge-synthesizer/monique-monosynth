/*
  ==============================================================================

    parameters.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONO_PARAMETER_H_INCLUDED
#define MONO_PARAMETER_H_INCLUDED

#include "App_h_includer.h"

// ==============================================================================
// ==============================================================================
// ==============================================================================
enum
{
    HAS_NO_MODULATION = -9999
};

// ==============================================================================
enum TYPES_DEF {
    IS_FLOAT = 0,
    IS_BOOL = 1,
    IS_INT = 2,
    UNKNOWN_TYPE = 3,
};

//==============================================================================
//==============================================================================
//==============================================================================
class Parameter;
class ParameterListener
{
    friend class Parameter;
    virtual void parameter_value_changed( Parameter* ) noexcept {}
    virtual void parameter_value_changed_always_notification( Parameter* ) noexcept {}
    virtual void parameter_value_on_load_changed( Parameter* ) noexcept {}
    virtual void parameter_modulation_value_changed( Parameter* ) noexcept {}

protected:
    NOINLINE ParameterListener() noexcept;
    NOINLINE ~ParameterListener() noexcept;
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ChangeParamOverTime;
struct ParameterInfo
{
    const TYPES_DEF type;

    const float min_value;
    const float max_value;
    const float init_value;

    const float init_modulation_amount;

    const int num_steps;

    const String name;
    const String short_name;

private:
    // ==============================================================================
    friend class Parameter;
#define MIN_MAX(min_,max_) min_,max_ /* HELPER MACRO TO MAKE CTOR ARGUMENTS MORE READABLE */
    NOINLINE ParameterInfo( const TYPES_DEF type_,
                            const float min_value_, const float max_value_, const float init_value_,
                            const float init_modulation_amount_,
                            const int num_steps_,
                            const String& name_, const String& short_name_ ) noexcept;
    NOINLINE ~ParameterInfo() noexcept;

private:
    // ==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ParameterInfo )
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ParameterRuntimeInfo
{
public:
    // ==============================================================================
    inline void set_last_modulation_amount( float current_modulation_amount_ ) noexcept
    {
        current_modulation_amount = current_modulation_amount_;
    }
    inline float get_last_modulation_amount() const noexcept
    {
        return current_modulation_amount;
    }
private:
    float current_modulation_amount;

private:
    // ==============================================================================
    friend class Parameter;
    NOINLINE ParameterRuntimeInfo() noexcept;
    NOINLINE ~ParameterRuntimeInfo() noexcept;

private:
    // ==============================================================================
    friend class ChangeParamOverTime;
    ChangeParamOverTime* timeChanger;
public:
    inline void stop_time_change() const noexcept; // see below ChangeParamOverTime

private:
    // ==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ParameterRuntimeInfo )
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
// TODO is it more performant to have the listerner functions not inlined in the param class?
// -> e.g. move to info?
class MIDIControl;
static inline void write_parameter_to_file( XmlElement& xml_, const Parameter* param_ ) noexcept;
static inline void read_parameter_from_file( const XmlElement& xml_, Parameter* param_ ) noexcept;
class Parameter
{
public:
    // ==============================================================================
    // GETTER
    inline operator float() const noexcept
    {
        return value;
    }
    inline float get_value() const noexcept
    {
        return value;
    }
protected:
    // ASSUME THE MEMORY GOES LINEAR FORWARD AND THE GETTER IS THE MOST USED FUNCTION
    float value;

private:
    // IF YOU GET AN COMPILE ERROR YOU SHOULD TAKE A LOOK AT BoolParameter or IntParameter
    //inline operator bool() const noexcept = delete;
    //inline operator int() const noexcept = delete;

public:
    // ==============================================================================
    // HELPER
    inline bool operator== ( float value_ ) const noexcept
    {
        return value == value_;
    }
    inline bool operator== ( int value_ ) const noexcept
    {
        return value == float(value_);
    }
    inline const Parameter* ptr() const noexcept
    {
        return this;
    }
    inline Parameter* ptr() noexcept
    {
        return this;
    }

public:
    // ==============================================================================
    // SETTER
    inline void set_value( float value_ ) noexcept
    {
        if( value != value_ )
        {
            if( value_ > info->max_value )
            {
                value_ = info->max_value;
            }
            else if( value_ < info->min_value )
            {
                value_ = info->min_value;
            }
            value = value_;
            notify_value_listeners();
        }
    }
    inline float operator= ( float value_ ) noexcept
    {
        set_value(value_);
        return value;
    }
    inline float operator= ( const Parameter& other_ ) noexcept
    {
        set_value(other_.value);
        return value;
    }
    inline void set_value_without_notification( float value_ ) noexcept
    {
        if( value != value_ )
        {
            if( value_ > info->max_value )
            {
                value_ = info->max_value;
            }
            else if( value_ < info->min_value )
            {
                value_ = info->min_value;
            }
            value = value_;
        }
    }

public:
    // ==============================================================================
    // MODULATOR (OPTIONAL PARAMETER)
    inline float get_modulation_amount() const noexcept
    {
        return modulation_amount;
    }
protected:
    // MODULATION AMOUNT GOES MIN AND MAX FROM -1 to 1
    float modulation_amount;

public:
    // ==============================================================================
    // MODULATOR (OPTIONAL PARAMETER)
    inline void set_modulation_amount( float modulation_amount_ ) noexcept
    {
        if( modulation_amount != modulation_amount_ )
        {
            if( modulation_amount_ > 1 )
            {
                modulation_amount_ = 1;
            }
            else if( modulation_amount_ < -1 )
            {
                modulation_amount_ = -1;
            }

            modulation_amount = modulation_amount_;
            notify_modulation_value_listeners();
        }
    }
    inline void set_modulation_amount_without_notification( float modulation_amount_ ) noexcept
    {
        if( modulation_amount != modulation_amount_ )
        {
            if( modulation_amount_ > 1 )
            {
                modulation_amount_ = 1;
            }
            else if( modulation_amount_ < -1 )
            {
                modulation_amount_ = -1;
            }

            modulation_amount = modulation_amount_;
        }
    }

public:
    // ==============================================================================
    // INFO
    inline const ParameterInfo& get_info() const noexcept
    {
        return *info;
    }
protected:
    const ParameterInfo*const info;

public:
    // ==============================================================================
    // RUNTIME INFO
    inline const ParameterRuntimeInfo& get_runtime_info() const noexcept
    {
        return *runtime_info;
    }
    inline ParameterRuntimeInfo& get_runtime_info() noexcept
    {
        return *runtime_info;
    }

protected:
    ParameterRuntimeInfo*const runtime_info;

protected:
    // ==============================================================================
    // OBSERVABLE
    Array< ParameterListener* > value_listeners;
    Array< ParameterListener* > always_value_listeners;

public:
    // REGISTRATIONS
    void register_listener( ParameterListener* listener_ ) noexcept;
    void register_always_listener( ParameterListener* listener_ ) noexcept;
    void remove_listener( const ParameterListener* listener_ ) noexcept;

protected:
    // NOT THREAD SAVE IF YOU ADD LISTENERS AT RUNTIME
    // NOTIFICATIONS
    inline void notify_value_listeners() noexcept;
    inline void notify_always_value_listeners() noexcept;
    friend void read_parameter_from_file( const XmlElement&, Parameter* ) noexcept;
    inline void notify_on_load_value_listeners() noexcept;
    inline void notify_modulation_value_listeners() noexcept;

public:
    // ==============================================================================
    // MIFI
    // TODO OWN THREAD NOTIFICATION HANDLER E.G. FOR MIDI
    MIDIControl*const midi_control;

public:
    // ==============================================================================
    // NOTE: the parameter is designed to live for a long time (app start to end and ctors may need some more power as usual)
    NOINLINE Parameter(const float min_value_, const float max_value_, const float init_value_,
                       const int num_steps_,
                       const String& name_, const String& short_name_,
                       const float init_modulation_amount_ = HAS_NO_MODULATION,
                       TYPES_DEF = IS_FLOAT
                      ) noexcept;
    NOINLINE ~Parameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( Parameter )
    MONO_NOT_MOVE_COPY_OPERATOR( Parameter )
};

// ==============================================================================
inline void Parameter::notify_value_listeners() noexcept
{
    for( int i = 0 ; i != value_listeners.size() ; ++i )
    {
        value_listeners.getUnchecked(i)->parameter_value_changed( this );
    }
};
inline void Parameter::notify_always_value_listeners() noexcept
{
    for( int i = 0 ; i != always_value_listeners.size() ; ++i )
    {
        always_value_listeners.getUnchecked(i)->parameter_value_changed_always_notification( this );
    }
};
inline void Parameter::notify_on_load_value_listeners() noexcept
{
    for( int i = 0 ; i != value_listeners.size() ; ++i )
    {
        value_listeners.getUnchecked(i)->parameter_value_on_load_changed( this );
    }
}
inline void Parameter::notify_modulation_value_listeners() noexcept
{
    for( int i = 0 ; i != value_listeners.size() ; ++i )
    {
        value_listeners.getUnchecked(i)->parameter_modulation_value_changed( this );
    }
}

// ==============================================================================
// ==============================================================================
// ==============================================================================
// TODO notifications
class BoolParameter : public Parameter
{
public:
    // ==============================================================================
    // GETTER
    inline bool operator^=( bool ) noexcept
    {
        value = !value;
        return value;
    }
    inline operator bool() const noexcept
    {
        return bool(value);
    }

public:
    // ==============================================================================
    // HELPER
    inline bool operator== ( bool value_ ) const noexcept
    {
        return bool(value) == value_;
    }
    inline const BoolParameter* bool_ptr() const noexcept
    {
        return this;
    }
    inline BoolParameter* bool_ptr() noexcept
    {
        return this;
    }

public:
    // ==============================================================================
    // SETTER
    inline bool operator= ( const bool value_ ) noexcept
    {
        return bool(value = value_);
    }
    inline bool operator= ( const BoolParameter& other_ ) noexcept
    {
        return value = other_.value;
    }

private:
    // IF YOU GET AN COMPILE ERROR, YOU HAVE USED THE WRONG PARAM
    inline operator float() const noexcept = delete;
    inline operator int() const noexcept = delete;

public:
    NOINLINE BoolParameter( const bool init_value_,
                            const String& name_, const String& short_name_ ) noexcept;
    NOINLINE ~BoolParameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( BoolParameter )
    MONO_NOT_MOVE_COPY_OPERATOR( BoolParameter )
};
// ==============================================================================
// ==============================================================================
// ==============================================================================
// TODO notifications
class IntParameter : public Parameter
{
public:
    // ==============================================================================
    // GETTER
    inline operator int() const noexcept
    {
        return int(value);
    }

public:
    // ==============================================================================
    // HELPER
    inline int operator== ( int value_ ) const noexcept
    {
        return int(value) == value_;
    }
    inline const IntParameter* int_ptr() const noexcept
    {
        return this;
    }
    inline IntParameter* int_ptr() noexcept
    {
        return this;
    }

public:
    // ==============================================================================
    // SETTER
    inline int operator= ( int value_ ) noexcept
    {
        if( int(value) != value_ )
        {
            if( value_ > info->max_value )
            {
                value_ = info->max_value;
            }
            else if( value_ < info->min_value )
            {
                value_ = info->min_value;
            }
            value = value_;
            notify_modulation_value_listeners();
        }
        return int(value);
    }
    inline int operator= ( const IntParameter& other_ ) noexcept
    {
        return operator=(int(other_.value));
    }

private:
    // IF YOU GET AN COMPILE ERROR, YOU HAVE USED THE WRONG PARAM
    inline operator float() const noexcept = delete;
    //inline operator bool() const noexcept = delete;

public:
    NOINLINE IntParameter( const int min_value_, const int max_value_, const int init_value_,
                           const String& name_, const String& short_name_ ) noexcept;
    NOINLINE ~IntParameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( IntParameter )
    MONO_NOT_MOVE_COPY_OPERATOR( IntParameter )
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ModulatedParameter : public Parameter
{
public:
    NOINLINE ModulatedParameter(const float min_value_, const float max_value_, const float init_value_,
                                const int num_steps_,
                                const String& name_, const String& short_name_,
                                const float init_modulation_amount_ ) noexcept;
    NOINLINE ~ModulatedParameter() noexcept;
};

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
class ArrayOfParameters
{
    const int size;
    Parameter** parameters;

public:
#if DEBUG
#define DEBUG_CHECK_ARRAY_RANGE( x ) if( x >= size ) { std::cout << "ERROR: ARRAY ACCESS OUT OF RANGE" << std::endl; }
#else
#define DEBUG_CHECK_ARRAY_RANGE( x )
#endif
    inline Parameter& operator[]( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline Parameter& get( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const Parameter& operator[]( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const Parameter& get( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }

public:
    NOINLINE ArrayOfParameters( const int num_parameters_,

                                const float min_value_, const float max_value_, const float init_value_,
                                const int num_steps_,

                                const String& owner_class_name_,
                                const int owner_id_,

                                const String& param_name_,
                                const String& param_name_short_,
                                bool create_human_id_ = true
                              ) noexcept;
    NOINLINE ~ArrayOfParameters() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( ArrayOfParameters )
    MONO_NOT_MOVE_COPY_OPERATOR( ArrayOfParameters )
};

//==============================================================================
//==============================================================================
//==============================================================================
class ArrayOfBoolParameters
{
    const int size;
    BoolParameter** parameters;

public:
    inline BoolParameter& operator[]( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline BoolParameter& get( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const BoolParameter& operator[]( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const BoolParameter& get( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }

public:
    NOINLINE ArrayOfBoolParameters( const int num_parameters_,

                                    const bool init_value_,

                                    const String& owner_class_name_,
                                    const int owner_id_,

                                    const String& param_name_,
                                    const String& param_name_short_,
                                    bool create_human_id_ = true
                                  ) noexcept;
    NOINLINE ~ArrayOfBoolParameters() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( ArrayOfBoolParameters )
    MONO_NOT_MOVE_COPY_OPERATOR( ArrayOfBoolParameters )
};

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
#define PARAM_CHANGE_INTERVAL_IN_MS 20
class ChangeParamOverTime : public Timer // TODO TIME SLICED THREAD ( DONT START HUNDREDS OF TIMERS!)
{
    Parameter& param;

    int sum_callbacks;

    float current_value;
    const float target_value;
    const float value_delta;
    const float min;
    const float max;

    // ==============================================================================
    ChangeParamOverTime( Parameter& param_, float target_value_, int time_in_ms_ ) noexcept;
    ~ChangeParamOverTime() noexcept;

    // ==============================================================================
    void timerCallback() override;
    inline void change() noexcept;

    friend class ParameterRuntimeInfo;
    inline void forceStopAndKill() noexcept;

public:
    // ==============================================================================
    static inline void execute( Parameter& param_, float target_value_, int time_in_ms_ ) noexcept
    {
        new ChangeParamOverTime(param_,target_value_,time_in_ms_);
    }

private:
    // ==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ChangeParamOverTime )
};

inline void ChangeParamOverTime::forceStopAndKill() noexcept
{
    stopTimer();
    if( param.get_runtime_info().timeChanger == this )
        param.get_runtime_info().timeChanger = nullptr;

    delete this;
    return;
}

//==============================================================================
inline void ParameterRuntimeInfo::stop_time_change() const noexcept
{
    if( timeChanger )
        timeChanger->forceStopAndKill();
}

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
// PLUGIN PARAMETER SUPPORT
static inline float get_percent_value( const Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    const float min = info.min_value;
    const float max = info.max_value;
    const float value = param_->get_value();

    return 1.0f/(max-min)*(value-min);
}

//==============================================================================
static inline void set_percent_value( Parameter* param_, float value_in_percent_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    const float min = info.min_value;
    const float max = info.max_value;

    param_->set_value( (max-min)*value_in_percent_ + min );
}

//==============================================================================
static inline float get_percent_default_value( const Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    return 1.0f/(info.max_value-info.min_value)*(info.init_value-info.min_value);
}

//==============================================================================
static inline int get_num_steps( const Parameter* param_ ) noexcept
{
    return param_->get_info().num_steps;
}

//==============================================================================
static inline bool has_modulation( const Parameter* param_ ) noexcept
{
    return param_->get_modulation_amount() != HAS_NO_MODULATION;
}

//==============================================================================
static inline float get_last_modulation_amount( const ModulatedParameter* param_ ) noexcept
{
    return param_->get_runtime_info().get_last_modulation_amount();
}

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
// operator*
static inline float operator* ( const Parameter& param_, const int value_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const Parameter& param_, const float value_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const Parameter& param_, const double value_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const int value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const float value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const double value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()*value_;
}
static inline float operator* ( const Parameter& param_lh_, const Parameter& param_rh_ ) noexcept
{
    return param_lh_.get_value()*param_rh_.get_value();
}

// operator+
static inline float operator+ ( const Parameter& param_, const int value_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const Parameter& param_, const float value_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const Parameter& param_, const double value_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const int value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const float value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const double value_, const Parameter& param_ ) noexcept
{
    return param_.get_value()+value_;
}
static inline float operator+ ( const Parameter& param_lh_, const Parameter& param_rh_ ) noexcept
{
    return param_lh_.get_value()+param_rh_.get_value();
}

// operator!=
static inline bool operator!= ( const bool value_, const BoolParameter& param_rh_ ) noexcept
{
    return value_ != bool(param_rh_.get_value());
}

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
// BUILD NAME HELPERS
NOINLINE static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_ + String("_") + String(param_id_);
}
NOINLINE static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_+1) + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String("_") + param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& param_name_ ) noexcept
{
    return param_name_;
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, const String& param_name_, int param_id_ ) noexcept
{
    return owner_class + String("_") + param_name_ + String("_") + String(param_id_+1);
}
NOINLINE static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_+1) + String("_") + param_name_ + String("_") + String(param_id_+1);
}

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
// FILE IO
static inline void write_parameter_to_file( XmlElement& xml_, const Parameter* param_ ) noexcept
{
    const float value = param_->get_value();
    const ParameterInfo& info = param_->get_info();
    if( value != info.init_value )
    {
        xml_.setAttribute( info.name, value );
    }

    if( has_modulation( param_ ) )
    {
        float modulation_amount = param_->get_modulation_amount();
        if( param_->get_modulation_amount() != info.init_modulation_amount )
        {
            xml_.setAttribute( info.name + String("_mod"), modulation_amount );
        }
    }
}
static inline void read_parameter_from_file( const XmlElement& xml_, Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    {
        const float old_value = param_->get_value();
        float new_value = xml_.getDoubleAttribute( info.name, info.init_value );

        if( new_value != old_value )
        {
            const float max_value = info.max_value;
            if( new_value > max_value )
            {
                new_value = max_value;
            }
            else if( new_value < info.min_value )
            {
                new_value = info.min_value;
            }

            param_->set_value_without_notification( new_value );
            param_->notify_on_load_value_listeners();
        }
    }

    if( has_modulation( param_ ) )
    {
        const float old_modulation_amount = param_->get_modulation_amount();
        const float new_modulation_amount = xml_.getDoubleAttribute( info.name + String("_mod"), info.init_modulation_amount );

        if( old_modulation_amount != new_modulation_amount )
        {
            param_->set_modulation_amount_without_notification( new_modulation_amount );
            param_->notify_on_load_value_listeners();
        }
    }
}

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
// TYPIFICATION
static inline TYPES_DEF type_of( const Parameter* param_ ) noexcept
{
    return param_->get_info().type;
}

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
// TOGGLE / SWITCH / INVERT
// IF YOU HAVE AN FLOT OR INT YOU WILL GET MAX OR MIN - DEPENDING ON THE CURRENT STATE
// IF YOU HAVE AN BOOL::FALSE YOO WILL GET BOOL::TRUE
static inline void toggle ( Parameter* param_ ) noexcept
{
    if( type_of( param_ ) == TYPES_DEF::IS_BOOL )
        param_->set_value( not bool(param_->get_value()) );
}
static inline void min_max_switch( Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    const float min( info.min_value );
    param_->set_value( param_->get_value() != min ? min : info.max_value );
}

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
class MIDIControl : ParameterListener
{
public:
    // TODO use from mono_audiodevicemanager
    enum TYPES
    {
        NOT_SET = -1,
        NOTE = 1,
        CC = 0
    };

private:
    friend class Parameter;
    int listen_type;
    int channel;
    int8 midi_number; // NOTES OR CC
    String is_ctrl_version_of_name;
    bool is_in_ctrl_mode;
    Parameter*const owner;

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
    bool train( const MidiMessage& input_message_, Parameter*const is_ctrl_version_of_ ) noexcept;
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
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;
    void parameter_modulation_value_changed( Parameter* param_ ) noexcept override;


    void generate_feedback_message( MidiMessage& ) const noexcept;
    void generate_modulation_feedback_message( MidiMessage& ) const noexcept;
    void send_standard_feedback() const noexcept;
    void send_modulation_feedback() const noexcept;

public:
    NOINLINE MIDIControl( Parameter*const owner_ );
    NOINLINE ~MIDIControl();

    void clear();
};
class MIDIControlHandler
{
    bool is_activated_and_waiting_for_param;
    Parameter* learning_param;
    Parameter* learning_ctrl_param;

    Array< Component* > learning_comps;

public:
    void toggle_midi_learn() noexcept;
    bool is_waiting_for_param() const noexcept;
    void set_learn_param( Parameter* param_ ) noexcept;
    void set_learn_width_ctrl_param( Parameter* param_, Parameter* ctrl_param_, Component* comp_ ) noexcept;
    void set_learn_param( Parameter* param_, Component* comp_ ) noexcept;
    void set_learn_param( Parameter* param_, Array< Component* > comps_ ) noexcept;
    Parameter* is_learning() const noexcept;
    bool handle_incoming_message( const MidiMessage& input_message_ ) noexcept;
    void clear() noexcept;

    MIDIControlHandler() noexcept;
    ~MIDIControlHandler() noexcept;

    juce_DeclareSingleton (MIDIControlHandler,false)
};

#include "monique_ui_LookAndFeel.h"
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

//==============================================================================
//==============================================================================
//==============================================================================
// FILE IO (MIDI)
static inline void write_midi_to( XmlElement& xml_, const Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    if( param_->midi_control->get_listen_type() != MIDIControl::NOT_SET )
        xml_.setAttribute( info.name + "_MIDI_T", param_->midi_control->get_listen_type() );
    if( param_->midi_control->get_midi_number() != -1 )
        xml_.setAttribute( info.name + "_MIDI_NR", param_->midi_control->get_midi_number() );
    if( param_->midi_control->get_chnanel() != -1 )
        xml_.setAttribute( info.name + "_MIDI_CH", param_->midi_control->get_chnanel() );
    if( param_->midi_control->get_is_ctrl_version_of_name() != "" )
        xml_.setAttribute( info.name + "_MIDI_CTRL", param_->midi_control->get_is_ctrl_version_of_name() );
}
static inline void read_midi_from( const XmlElement& xml_, Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    param_->midi_control->train
    (
        xml_.getIntAttribute( info.name + "_MIDI_T", MIDIControl::NOT_SET ),
        xml_.getIntAttribute( info.name + "_MIDI_NR", -1 ),
        xml_.getIntAttribute( info.name + "_MIDI_CH", -1 ),
        xml_.getStringAttribute( info.name + "_MIDI_CTRL", "" )
    );
}

#endif



