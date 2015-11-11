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
    virtual void parameter_value_changed_by_automation( Parameter* ) noexcept {}
    virtual void parameter_value_changed_always_notification( Parameter* ) noexcept {}
    virtual void parameter_value_on_load_changed( Parameter* ) noexcept {}
    virtual void parameter_modulation_value_changed( Parameter* ) noexcept {}

protected:
    COLD ParameterListener() noexcept;
    COLD ~ParameterListener() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ParameterListener )
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ChangeParamOverTime;
class SmoothedParameter;
struct ParameterInfo
{
    const TYPES_DEF type;

    const float min_value;
    const float max_value;
    const float init_value;

    float factory_default_value;
    float program_on_load_value;

    const float init_modulation_amount;
    float factory_default_modulation_amount;
    float program_on_load_modulation_amount;

    const int num_steps;

    const String name;
    const String short_name;
#ifdef IS_PLUGIN
    int parameter_host_id;
#endif

private:
    // ==============================================================================
    friend class Parameter;
#define MIN_MAX(min_,max_) min_,max_ /* HELPER MACRO TO MAKE CTOR ARGUMENTS MORE READABLE */
    COLD ParameterInfo( const TYPES_DEF type_,
                        const float min_value_, const float max_value_, const float init_value_,
                        const float init_modulation_amount_,
                        const int num_steps_,
                        const String& name_, const String& short_name_ ) noexcept;
    COLD ~ParameterInfo() noexcept;

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
    SmoothedParameter*my_smoother;

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
    COLD ParameterRuntimeInfo() noexcept;
    COLD ~ParameterRuntimeInfo() noexcept;

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
#define SNAP_TO_ZERO_AMOUNT 0.00001f
static inline float snap_to_zero( float x_ ) noexcept
{
    if( x_ < SNAP_TO_ZERO_AMOUNT and x_ > -SNAP_TO_ZERO_AMOUNT )
    {
        x_ = 0;
    }

    return x_;
}
static inline float snap_to_max( float x_, float max_ ) noexcept
{
    if( x_ > max_-SNAP_TO_ZERO_AMOUNT )
    {
        x_ = max_;
    }

    return x_;
}
static inline float snap_to_min( float x_, float min_ ) noexcept
{
    if( x_ < min_+SNAP_TO_ZERO_AMOUNT )
    {
        x_ = min_;
    }

    return x_;
}

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
        value_ = snap_to_min(snap_to_max(snap_to_zero(value_),info->max_value),info->min_value);
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
        if (this != &other_)
        {
            set_value(other_.value);
        }
        return value;
    }
    inline void set_value_without_notification( float value_ ) noexcept
    {
        value_ = snap_to_zero(value_);
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
            notify_always_value_listeners();
        }
    }
    inline void set_value_by_automation( float value_ ) noexcept
    {
        value_ = snap_to_zero(value_);
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
            notify_value_listeners_by_automation();
        }
    }
    inline void set_value_on_load( float value_ ) noexcept
    {
        value_ = snap_to_zero(value_);
        //if( value != value_ )
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
        modulation_amount_ = snap_to_zero(modulation_amount_);
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
        modulation_amount_ = snap_to_zero(modulation_amount_);
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
    inline void register_listener( ParameterListener* listener_ ) noexcept;
    inline void register_always_listener( ParameterListener* listener_ ) noexcept;
    inline void remove_listener( const ParameterListener* listener_ ) noexcept;

protected:
    // NOT THREAD SAVE IF YOU ADD LISTENERS AT RUNTIME
    // NOTIFICATIONS
    inline void notify_value_listeners() noexcept;
    inline void notify_value_listeners_by_automation() noexcept;
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
    COLD Parameter(const float min_value_, const float max_value_, const float init_value_,
                   const int num_steps_,
                   const String& name_, const String& short_name_,
                   const float init_modulation_amount_ = HAS_NO_MODULATION,
                   TYPES_DEF = IS_FLOAT
                  ) noexcept;
    COLD ~Parameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( Parameter )
    MONO_NOT_MOVE_COPY_OPERATOR( Parameter )
    JUCE_LEAK_DETECTOR(Parameter)
};

// ==============================================================================
inline void Parameter::register_listener( ParameterListener* listener_ ) noexcept
{
    if( ! value_listeners.contains( listener_ ) )
    {
        value_listeners.add( listener_ );

        value_listeners.minimiseStorageOverheads();
    }
}
inline void Parameter::register_always_listener( ParameterListener* listener_ ) noexcept
{
    if( ! always_value_listeners.contains( listener_ ) )
    {
        always_value_listeners.add( listener_ );
        value_listeners.add( listener_ );

        always_value_listeners.minimiseStorageOverheads();
        value_listeners.minimiseStorageOverheads();
    }
}
inline void Parameter::remove_listener( const ParameterListener* listener_ ) noexcept
{
    always_value_listeners.removeFirstMatchingValue( const_cast< ParameterListener* >( listener_ ) );
    value_listeners.removeFirstMatchingValue( const_cast< ParameterListener* >( listener_ ) );

    always_value_listeners.minimiseStorageOverheads();
    value_listeners.minimiseStorageOverheads();
}

// ==============================================================================
inline void Parameter::notify_value_listeners() noexcept
{
    for( int i = 0 ; i != value_listeners.size() ; ++i )
    {
        value_listeners.getUnchecked(i)->parameter_value_changed( this );
    }
}
inline void Parameter::notify_value_listeners_by_automation() noexcept
{
    for( int i = 0 ; i != value_listeners.size() ; ++i )
    {
        value_listeners.getUnchecked(i)->parameter_value_changed_by_automation( this );
    }
}
inline void Parameter::notify_always_value_listeners() noexcept
{
    for( int i = 0 ; i != always_value_listeners.size() ; ++i )
    {
        always_value_listeners.getUnchecked(i)->parameter_value_changed_always_notification( this );
    }
}
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
        value = value_;
        return bool(value);
    }
    inline bool operator= ( const BoolParameter& other_ ) noexcept
    {
        if (this != &other_)
        {
            value = other_.value;
        }
        return bool(value);
    }

private:
    // IF YOU GET AN COMPILE ERROR, YOU HAVE USED THE WRONG PARAM
    inline operator float() const noexcept = delete;
    inline operator int() const noexcept = delete;

public:
    COLD BoolParameter( const bool init_value_,
                        const String& name_, const String& short_name_ ) noexcept;
    COLD ~BoolParameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( BoolParameter )
    MONO_NOT_MOVE_COPY_OPERATOR( BoolParameter )
    JUCE_LEAK_DETECTOR(BoolParameter)
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
        if (this != &other_)
        {
            operator=(int(other_.value));
        }
        return int(value);
    }

private:
    // IF YOU GET AN COMPILE ERROR, YOU HAVE USED THE WRONG PARAM
    inline operator float() const noexcept = delete;
    //inline operator bool() const noexcept = delete;

public:
    COLD IntParameter( const int min_value_, const int max_value_, const int init_value_,
                       const String& name_, const String& short_name_ ) noexcept;
    COLD ~IntParameter() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( IntParameter )
    MONO_NOT_MOVE_COPY_OPERATOR( IntParameter )
    JUCE_LEAK_DETECTOR(IntParameter)
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ModulatedParameter : public Parameter
{
public:
    COLD ModulatedParameter(const float min_value_, const float max_value_, const float init_value_,
                            const int num_steps_,
                            const String& name_, const String& short_name_,
                            const float init_modulation_amount_ ) noexcept;
    COLD ~ModulatedParameter() noexcept;

    JUCE_LEAK_DETECTOR(ModulatedParameter)
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
    COLD ArrayOfParameters( const int num_parameters_,

                            const float min_value_, const float max_value_, const float init_value_,
                            const int num_steps_,

                            const String& owner_class_name_, const String& short_owner_class_name_,
                            const int owner_id_,

                            const String& param_name_,
                            const String& param_name_short_,
                            bool create_human_id_ = true
                          ) noexcept;
    COLD ~ArrayOfParameters() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( ArrayOfParameters )
    MONO_NOT_MOVE_COPY_OPERATOR( ArrayOfParameters )
    JUCE_LEAK_DETECTOR(ArrayOfParameters)
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
    COLD ArrayOfBoolParameters( const int num_parameters_,

                                const bool init_value_,

                                const String& owner_class_name_, const String& short_owner_class_name_,
                                const int owner_id_,

                                const String& param_name_,
                                const String& param_name_short_,
                                bool create_human_id_ = true
                              ) noexcept;
    COLD ~ArrayOfBoolParameters() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( ArrayOfBoolParameters )
    MONO_NOT_MOVE_COPY_OPERATOR( ArrayOfBoolParameters )
    JUCE_LEAK_DETECTOR(ArrayOfBoolParameters)
};

//==============================================================================
//==============================================================================
//==============================================================================
class ArrayOfIntParameters
{
    const int size;
    IntParameter** parameters;

public:
    inline IntParameter& operator[]( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline IntParameter& get( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const IntParameter& operator[]( int index_ ) const noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }
    inline const IntParameter& get( int index_ ) noexcept
    {
        DEBUG_CHECK_ARRAY_RANGE( index_ );
        return *parameters[ index_ ];
    }

public:
    COLD ArrayOfIntParameters( const int num_parameters_,

                               const int min_value_, const int max_value_, const int init_value_,

                               const String& owner_class_name_, const String& short_owner_class_name_,
                               const int owner_id_,

                               const String& param_name_,
                               const String& param_name_short_,
                               bool create_human_id_ = true
                             ) noexcept;
    COLD ~ArrayOfIntParameters() noexcept;

private:
    // ==============================================================================
    MONO_NOT_CTOR_COPYABLE( ArrayOfIntParameters )
    MONO_NOT_MOVE_COPY_OPERATOR( ArrayOfIntParameters )
    JUCE_LEAK_DETECTOR(ArrayOfIntParameters)
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
    {
        param.get_runtime_info().timeChanger = nullptr;
    }

    delete this;
    return;
}

//==============================================================================
inline void ParameterRuntimeInfo::stop_time_change() const noexcept
{
    if( timeChanger )
    {
        timeChanger->forceStopAndKill();
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
static inline float get_percent_default_modulation_value( const Parameter* param_ ) noexcept
{
    return  param_->get_info().init_modulation_amount;
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
COLD static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_ + String("_") + String(param_id_);
}
COLD static inline String generate_param_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_) + String("_") + param_name_;
}
COLD static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String(owner_id_+1) + String("_") + param_name_;
}
COLD static inline String generate_short_human_name( const String& owner_class, const String& param_name_ ) noexcept
{
    return owner_class + String("_") + String("_") + param_name_;
}
COLD static inline String generate_short_human_name( const String& param_name_ ) noexcept
{
    return param_name_;
}
COLD static inline String generate_short_human_name( const String& owner_class, const String& param_name_, int param_id_ ) noexcept
{
    return owner_class + String("_") + param_name_ + String("_") + String(param_id_+1);
}
COLD static inline String generate_short_human_name( const String& owner_class, int owner_id_, const String& param_name_, int param_id_ ) noexcept
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
    bool success = false;
    const ParameterInfo& info = param_->get_info();
    {
        float new_value = xml_.getDoubleAttribute( info.name, info.init_value );
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

            const_cast<ParameterInfo&>(param_->get_info()).program_on_load_value = new_value;
            param_->set_value_on_load( new_value );
            success = true;
        }
    }

    if( has_modulation( param_ ) )
    {
        float new_modulation_amount = xml_.getDoubleAttribute( info.name + String("_mod"), info.init_modulation_amount );

        const_cast<ParameterInfo&>(param_->get_info()).program_on_load_modulation_amount = new_modulation_amount;
        param_->set_modulation_amount_without_notification( new_modulation_amount );
        success = true;
    }

    if( success )
    {
        param_->notify_on_load_value_listeners();
    }
}
static inline void read_parameter_factory_default_from_file( const XmlElement& xml_, Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    {
        float new_value = xml_.getDoubleAttribute( info.name, info.init_value );
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
            const_cast<ParameterInfo&>(param_->get_info()).factory_default_value = new_value;
        }
    }

    if( has_modulation( param_ ) )
    {
        float new_modulation_amount = xml_.getDoubleAttribute( info.name + String("_mod"), info.init_modulation_amount );
        const_cast<ParameterInfo&>(param_->get_info()).factory_default_modulation_amount = new_modulation_amount;
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
class MoniqueAudioProcessor;
class MIDIControl : ParameterListener
{
public:
    // TODO use from mono_audiodevicemanager
    // TODO only register listener if is trained!
    enum TYPES
    {
        NOT_SET = -1,
        NOTE = 1,
        CC = 0
    };

private:
    friend class MIDIControlHandler;
    friend class Parameter;
    int8 midi_number; // NOTES OR CC
    String is_ctrl_version_of_name;
    bool is_in_ctrl_mode;
    Parameter*const owner;
    MoniqueAudioProcessor*audio_processor;

public:
    int8 get_midi_number() const noexcept
    {
        return midi_number;
    }
    const String& get_is_ctrl_version_of_name() const noexcept
    {
        return is_ctrl_version_of_name;
    }
    bool is_listen_to( int controller_number_ ) const noexcept
    {
        return controller_number_ == midi_number;
    }
    bool read_from_if_you_listen( int controller_number_, int controller_value_, float pickup_offset_ ) noexcept;
    bool train( int controller_number_, Parameter*const is_ctrl_version_of_, MoniqueAudioProcessor*audio_processor ) noexcept;
    bool train( int controller_number_, String is_ctrl_version_of_name_, MoniqueAudioProcessor*audio_processor ) noexcept;
    bool is_valid_trained() const noexcept
    {
        return midi_number != -1 and audio_processor != nullptr;
    }
    void send_feedback_only() const noexcept;
    void send_clear_feedback_only() const noexcept;

    void set_ctrl_mode( bool mode_ ) noexcept;
    bool get_ctrl_mode() const noexcept
    {
        return is_in_ctrl_mode;
    }

private:
    // FEEDBACK
    void start_listen_for_feedback() noexcept;
    void stop_listen_for_feedback() noexcept;
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;
    void parameter_modulation_value_changed( Parameter* param_ ) noexcept override;

    void send_standard_feedback() const noexcept;
    void send_modulation_feedback() const noexcept;

public:
    COLD MIDIControl( Parameter*const owner_ );
    COLD ~MIDIControl();

    void clear();

    JUCE_LEAK_DETECTOR(MIDIControl)
};

class UiLookAndFeel;
class MIDIControlHandler
{
    UiLookAndFeel*const ui_look_and_feel;
    MoniqueAudioProcessor*const audio_processor;

    bool is_activated_and_waiting_for_param;
    Parameter* learning_param;
    Parameter* learning_ctrl_param;

    Array< Component* > learning_comps;
    Array< MIDIControl* > trained_midi_ctrls_;
    void add_trained( MIDIControl*midi_ctrl_ ) noexcept
    {
        trained_midi_ctrls_.add( midi_ctrl_ );
    }
    void remove_trained( MIDIControl*midi_ctrl_ ) noexcept
    {
        trained_midi_ctrls_.removeFirstMatchingValue( midi_ctrl_ );
    }
    friend class MIDIControl;
    MIDIControl* get_trained( String& for_first_name_ ) noexcept
    {
        for( int i = 0 ; i != trained_midi_ctrls_.size() ; ++i )
        {
            MIDIControl*midi_control = trained_midi_ctrls_.getUnchecked(i);
            if( midi_control->is_ctrl_version_of_name == for_first_name_ )
            {
                return midi_control;
            }
        }

        return nullptr;
    }

public:
    void toggle_midi_learn() noexcept;
    bool is_waiting_for_param() const noexcept;
    void set_learn_param( Parameter* param_ ) noexcept;
    void set_learn_width_ctrl_param( Parameter* param_, Parameter* ctrl_param_, Component* comp_ ) noexcept;
    void set_learn_param( Parameter* param_, Component* comp_ ) noexcept;
    void set_learn_param( Parameter* param_, Array< Component* > comps_ ) noexcept;
    Parameter* is_learning() const noexcept;

    bool handle_incoming_message( int controller_number_ ) noexcept;
    void clear() noexcept;

private:
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< MIDIControlHandler >;
    COLD MIDIControlHandler( UiLookAndFeel*look_and_feel_, MoniqueAudioProcessor*const audio_processor_ ) noexcept;
    COLD ~MIDIControlHandler() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIControlHandler)
};

inline bool MIDIControlHandler::is_waiting_for_param() const noexcept
{
    return is_activated_and_waiting_for_param;
}

#include "monique_ui_LookAndFeel.h"
#define IF_MIDI_LEARN__HANDLE( param ) \
        if( midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning() ) \
        { \
            midi_control_handler->set_learn_param( param ); \
        }
#define IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT( param, component_ ) \
        if( midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning() ) \
        { \
            midi_control_handler->set_learn_param( param, component_ ); \
        }
#define IF_MIDI_LEARN__HANDLE_TWO_PARAMS__AND_UPDATE_COMPONENT( param, param_ctrl, component_ ) \
        if( midi_control_handler->is_waiting_for_param() || midi_control_handler->is_learning() ) \
        { \
            midi_control_handler->set_learn_width_ctrl_param( param, param_ctrl, component_ ); \
        }
#define SET_COMPONENT_TO_MIDI_LEARN( comp, ui_look_and_feel_ ) \
        { \
	  ui_look_and_feel_->midi_learn_comp = comp; \
	  comp->repaint(); \
	}
#define UNSET_COMPONENT_MIDI_LEARN( comp, ui_look_and_feel_ ) \
        { \
	  ui_look_and_feel_->midi_learn_comp = nullptr; \
	  comp->repaint(); \
	}

//==============================================================================
//==============================================================================
//==============================================================================
// FILE IO (MIDI)
static inline void write_midi_to( XmlElement& xml_, const Parameter* param_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    if( param_->midi_control->get_midi_number() != -1 )
    {
        xml_.setAttribute( info.name + "_MIDI_NR", param_->midi_control->get_midi_number() );
    }
    if( param_->midi_control->get_is_ctrl_version_of_name() != "" )
    {
        xml_.setAttribute( info.name + "_MIDI_CTRL", param_->midi_control->get_is_ctrl_version_of_name() );
    }
}
static inline void read_midi_from( const XmlElement& xml_, Parameter* param_, MoniqueAudioProcessor*midi_device_manager_ ) noexcept
{
    const ParameterInfo& info = param_->get_info();
    const int number = xml_.getIntAttribute( info.name + "_MIDI_NR", -1 );
    if( number != -1 )
    {
        const String ctrl = xml_.getStringAttribute( info.name + "_MIDI_CTRL", "" );
        param_->midi_control->train
        (
            number,
            ctrl,
            midi_device_manager_
        );
    }
}

#endif




