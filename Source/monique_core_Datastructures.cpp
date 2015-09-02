/*
  ==============================================================================

    SynthData.cpp
    Created: 24 Apr 2015 11:40:30am
    Author:  monotomy

  ==============================================================================
*/

#include "monique_core_Datastructures.h"
#include "Synth.h"

//==============================================================================
//==============================================================================
//==============================================================================
static inline float positive( float x ) noexcept
{
    return x < 0 ? x * -1 : x;
}

//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE RuntimeListener::RuntimeListener() noexcept :
sample_rate(RuntimeNotifyer::getInstance()->sample_rate),
            sample_rate_1ths(RuntimeNotifyer::getInstance()->sample_rate_1ths),
            block_size(RuntimeNotifyer::getInstance()->block_size)
{
    RuntimeNotifyer::getInstance()->listeners.add( this );
    RuntimeNotifyer::getInstance()->listeners.minimiseStorageOverheads();
}

NOINLINE RuntimeListener::~RuntimeListener() noexcept
{
    RuntimeNotifyer::getInstance()->listeners.removeFirstMatchingValue( this );
}

//==============================================================================
NOINLINE void RuntimeListener::set_sample_rate( double sr_ ) noexcept
{
    sample_rate = sr_;
    sample_rate_1ths = 1.0f/sample_rate;
};
NOINLINE void RuntimeListener::set_block_size( int bs_ ) noexcept { block_size = bs_; };
NOINLINE void RuntimeListener::sample_rate_changed( double /* old_sr_ */ ) noexcept {};
NOINLINE void RuntimeListener::block_size_changed() noexcept {};

//==============================================================================
//==============================================================================
//==============================================================================
juce_ImplementSingleton (RuntimeNotifyer)

NOINLINE RuntimeNotifyer::RuntimeNotifyer() noexcept :
sample_rate(44100),
            sample_rate_1ths( 1.0/44100),
            block_size(256)
{
}

NOINLINE RuntimeNotifyer::~RuntimeNotifyer() noexcept
{
    clearSingletonInstance();
}

//==============================================================================
NOINLINE void RuntimeNotifyer::set_sample_rate( double sr_ ) noexcept
{
    double old_sr = sample_rate;
    sample_rate = sr_;
    sample_rate_1ths = 1.0/sample_rate;
    for( int i = 0 ; i != listeners.size() ; ++i )
    {
        listeners[i]->set_sample_rate(sr_);
        listeners[i]->sample_rate_changed(old_sr);
    }
};
NOINLINE void RuntimeNotifyer::set_block_size( int bs_ ) noexcept
{
    block_size = bs_;
    for( int i = 0 ; i != listeners.size() ; ++i ) {
        listeners[i]->set_block_size(bs_);
        listeners[i]->block_size_changed();
    }
};

double RuntimeNotifyer::get_sample_rate() const noexcept
{
    return sample_rate;
}
int RuntimeNotifyer::get_block_size() const noexcept
{
    return block_size;
}

//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE RuntimeInfo::RuntimeInfo() noexcept
:
samples_since_start(0),
                    bpm(120)
#ifdef IS_STANDALONE
                    ,
                    is_extern_synced(false),
                    clock_counter(0)
#endif
{
    mono_ParameterOwnerStore::getInstance()->runtime_info = this;

    std::cout << "MONIQUE: init RTI" << std::endl;
}
NOINLINE RuntimeInfo::~RuntimeInfo() noexcept {}

//==============================================================================
//==============================================================================
//==============================================================================
#define LFO_NAME "LFO"
NOINLINE LFOData::LFOData( int id_ ) noexcept
:
speed
(
    MIN_MAX( 0, 16+127-33 ),
    4,
    16+127-33,

    generate_param_name(LFO_NAME,id_,"speed"),
    generate_short_human_name(LFO_NAME,id_,"speed")
)
{}
NOINLINE LFOData::~LFOData() noexcept {}

//==============================================================================
static inline void copy( LFOData* dest_, const LFOData* src_ ) noexcept
{
    dest_->speed = src_->speed;
}
static inline void collect_saveable_parameters( LFOData* lfo_data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &lfo_data_->speed );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define OSC_NAME "OSC"
NOINLINE OSCData::OSCData( int id_ ) noexcept
:
wave
(
    MIN_MAX( SINE, NOICE ),
    SINE,
    1000,
    generate_param_name(OSC_NAME,id_,"wave"),
    generate_short_human_name(OSC_NAME,id_,"sync")
),
octave
(
    MIN_MAX( -24, 24 ),
    0,
    48*100,
    generate_param_name(OSC_NAME,id_,"octave"),
    generate_short_human_name(OSC_NAME,id_,"octave"),
    0.5 // one octave
),
is_lfo_modulated
(
    false,
    generate_param_name(OSC_NAME,id_,"is_lfo_mod"),
    generate_short_human_name(OSC_NAME,id_,"mod_lfo")
),

// -------------------------------------------------------
fm_multi
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(OSC_NAME,id_,"fm_multi"),
    generate_short_human_name(OSC_NAME,id_,"fm_freq")
),
fm_amount
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(OSC_NAME,id_,"fm_power"),
    generate_short_human_name(OSC_NAME,id_,"fm_power")
),
fm_wave
(
    true,
    generate_param_name(OSC_NAME,id_,"fm_wave"),
    generate_short_human_name(OSC_NAME,id_,"fm_shot")
),
sync
(
    true,
    generate_param_name(OSC_NAME,id_,"sync"),
    generate_short_human_name(OSC_NAME,id_,"fm_sync")
),
mod_off
(
    true,
    generate_param_name(OSC_NAME,id_,"mod_off"),
    generate_short_human_name(OSC_NAME,id_,"mod_off")
),

puls_width
(
    MIN_MAX( -12, 12 ),
    0,
    generate_param_name(OSC_NAME,id_,"puls_width"),
    generate_short_human_name(OSC_NAME,id_,"puls_width")
),
fm_swing
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(OSC_NAME,id_,"fm_swing"),
    generate_short_human_name(OSC_NAME,id_,"fm_swing")
),
osc_switch
(
    MIN_MAX( 0, 16 ),
    0,
    generate_param_name(OSC_NAME,id_,"osc_switch"),
    generate_short_human_name(OSC_NAME,id_,"switch")
),

last_modulation_value( 0 )
{}
NOINLINE OSCData::~OSCData() noexcept {}

//==============================================================================
static inline void copy( OSCData* dest_, const OSCData* src_ ) noexcept
{
    dest_->wave = src_->wave;
    dest_->octave = src_->octave;
    dest_->is_lfo_modulated = src_->is_lfo_modulated;
    dest_->fm_multi = src_->fm_multi;
    dest_->fm_amount = src_->fm_amount;
    dest_->fm_wave = src_->fm_wave;
    dest_->sync = src_->sync;
    dest_->mod_off = src_->mod_off;
    dest_->puls_width = src_->puls_width;
    dest_->fm_swing = src_->fm_swing;
    dest_->osc_switch = src_->osc_switch;
}
static inline void collect_saveable_parameters( OSCData* osc_data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &osc_data_->wave );
    params_.add( &osc_data_->octave );
    params_.add( &osc_data_->is_lfo_modulated );
    params_.add( &osc_data_->fm_multi );
    params_.add( &osc_data_->fm_amount );
    params_.add( &osc_data_->fm_wave );
    params_.add( &osc_data_->sync );
    params_.add( &osc_data_->puls_width );
    params_.add( &osc_data_->fm_swing );
    params_.add( &osc_data_->osc_switch );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define ENV_NAME "ENV"
NOINLINE ENVData::ENVData( int id_ ) noexcept
:
id( id_ ),

attack
(
    MIN_MAX( 0.001, 1 ),
    0.05,
    1000,
    generate_param_name(ENV_NAME,id,"attack"),
    generate_short_human_name(ENV_NAME,id_,"attack")
),
max_attack_time
(
    MIN_MAX( 100, 20000 ),
    2000,
    generate_param_name(ENV_NAME,id,"max_attack_t"),
    generate_short_human_name(ENV_NAME,id_,"max_attack_t")
),

decay
(
    MIN_MAX( 0, 1 ),
    0.02,
    1000,
    generate_param_name(ENV_NAME,id,"decay"),
    generate_short_human_name(ENV_NAME,id_,"decay")
),
max_decay_time
(
    MIN_MAX( 100, 20000 ),
    250,
    generate_param_name(ENV_NAME,id,"max_decay_t"),
    generate_short_human_name(ENV_NAME,id_,"max_decay_t")
),

sustain
(
    MIN_MAX( 0.001, 1 ),
    0.9,
    1000,
    generate_param_name(ENV_NAME,id,"sustain"),
    generate_short_human_name(ENV_NAME,id_,"sustain")
),

sustain_time
(
    MIN_MAX( 0.001, 1 ),
    1,
    1000,
    generate_param_name(ENV_NAME,id,"sustain_time"),
    generate_short_human_name(ENV_NAME,id_,"sus_time")),

release
(
    MIN_MAX( 0.001, 1 ),
    0.2,
    1000,
    generate_param_name(ENV_NAME,id,"release"),
    generate_short_human_name(ENV_NAME,id_,"release")
),
max_release_time
(
    MIN_MAX( 100, 20000 ),
    4000,
    generate_param_name(ENV_NAME,id,"max_release_t"),
    generate_short_human_name(ENV_NAME,id_,"max_release_t")
)
{
}
NOINLINE ENVData::~ENVData() noexcept {}

//==============================================================================
static inline void copy( ENVData* dest_, const ENVData* src_ ) noexcept
{
    dest_->attack = src_->attack;
    dest_->max_attack_time = src_->max_attack_time;
    dest_->decay = src_->decay;
    dest_->max_decay_time = src_->max_decay_time;
    dest_->sustain = src_->sustain;
    dest_->sustain_time = src_->sustain_time;
    dest_->release = src_->release;
    dest_->max_release_time = src_->max_release_time;
}
static inline void collect_saveable_parameters( ENVData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->attack );
    params_.add( &data_->max_attack_time );
    params_.add( &data_->decay );
    params_.add( &data_->max_decay_time );
    params_.add( &data_->sustain );
    params_.add( &data_->sustain_time );
    params_.add( &data_->release );
    params_.add( &data_->max_release_time );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define ENV_PRESET_DEF_NAME "ENVPD"
NOINLINE ENVPresetDef::ENVPresetDef ( int id_ ) noexcept
:
attack_1
(
    MIN_MAX( 0, 1 ),
    0.001,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_1_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_1")
),
decay_1
(
    MIN_MAX( 0, 1 ),
    0.1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_1_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_1")
),
sustain_time_1
(
    MIN_MAX( 0.001, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_1_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_1")
),
release_1
(
    MIN_MAX( 0, 1 ),
    0.001,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_1_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"release_1")
),

// ----
attack_2
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_2_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_2")
),
decay_2
(
    MIN_MAX( 0, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_2_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_2")
),
sustain_time_2
(
    MIN_MAX( 0.001, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_2_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_2")
),
release_2
(
    MIN_MAX( 0, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_2_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"release_2")
),

// ----
attack_3
(
    MIN_MAX( 0, 1 ),
    0.5,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_3_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_3")
),
decay_3
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_3_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_3")
),
sustain_time_3
(
    MIN_MAX( 0.001, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_3_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_3")
),
release_3
(
    MIN_MAX( 0, 1 ),
    0.1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_3_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"release_3")
),

// ----
attack_4
(
    MIN_MAX( 0, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_4_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_4")
),
decay_4
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_4_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_4")
),
sustain_time_4
(
    MIN_MAX( 0.001, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_4_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_4")
),
release_4
(
    MIN_MAX( 0, 1 ),
    1,
    1000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_4_"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"release_4")
),

// ----
max_attack_time
(
    MIN_MAX( 100, 20000 ),
    2000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_attack_t"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"max_attack_t")
),
max_decay_time
(
    MIN_MAX( 100, 20000 ),
    250,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_decay_t"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"max_decay_t")
),
max_release_time
(
    MIN_MAX( 100, 20000 ),
    4000,
    generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_release_t"),
    generate_short_human_name(ENV_PRESET_DEF_NAME,"max_release_t")
)
{}
NOINLINE ENVPresetDef::~ENVPresetDef() noexcept {}

//==============================================================================
static inline void copy( ENVPresetDef* dest_, const ENVPresetDef* src_ ) noexcept
{
    dest_->attack_1 = src_->attack_1;
    dest_->decay_1 = src_->decay_1;
    dest_->sustain_time_1 = src_->sustain_time_1;
    dest_->release_1 = src_->release_1;
    dest_->attack_2 = src_->attack_2;
    dest_->decay_2 = src_->decay_2;
    dest_->sustain_time_2 = src_->sustain_time_2;
    dest_->release_2 = src_->release_2;
    dest_->attack_3 = src_->attack_3;
    dest_->decay_3 = src_->decay_3;
    dest_->sustain_time_3 = src_->sustain_time_3;
    dest_->release_3 = src_->release_3;
    dest_->attack_4 = src_->attack_4;
    dest_->decay_4 = src_->decay_4;
    dest_->sustain_time_4 = src_->sustain_time_4;
    dest_->release_4 = src_->release_4;
    dest_->max_attack_time = src_->max_attack_time;
    dest_->max_decay_time = src_->max_decay_time;
    dest_->max_release_time = src_->max_release_time;
}
static inline void collect_saveable_parameters( ENVPresetDef* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->attack_1 );
    params_.add( &data_->decay_1 );
    params_.add( &data_->sustain_time_1 );
    params_.add( &data_->release_1 );
    params_.add( &data_->attack_2 );
    params_.add( &data_->decay_2 );
    params_.add( &data_->sustain_time_2 );
    params_.add( &data_->release_2 );
    params_.add( &data_->attack_3 );
    params_.add( &data_->decay_3 );
    params_.add( &data_->sustain_time_3 );
    params_.add( &data_->release_3 );
    params_.add( &data_->attack_4 );
    params_.add( &data_->decay_4 );
    params_.add( &data_->sustain_time_4 );
    params_.add( &data_->release_4 );
    params_.add( &data_->max_attack_time );
    params_.add( &data_->max_decay_time );
    params_.add( &data_->max_release_time );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define ENV_PRESET_NAME "ENVP"
NOINLINE ENVPresetData::ENVPresetData( int id_, ENVPresetDef* def_ ) noexcept
:
ENVData( id_ ),
def( def_ ),

state
(
    MIN_MAX( 0, 3 ),
    0.05,
    3000,
    generate_param_name(ENV_PRESET_NAME,id,"state"),
    generate_short_human_name(ENV_PRESET_NAME,id_,"state")
)
{
    state.register_always_listener( this );

    def_->attack_1.register_listener( this );
    def_->attack_2.register_listener( this );
    def_->attack_3.register_listener( this );
    def_->attack_4.register_listener( this );
    def_->sustain_time_1.register_listener( this );
    def_->sustain_time_2.register_listener( this );
    def_->sustain_time_3.register_listener( this );
    def_->sustain_time_4.register_listener( this );
    def_->decay_1.register_listener( this );
    def_->decay_2.register_listener( this );
    def_->decay_3.register_listener( this );
    def_->decay_4.register_listener( this );
    def_->release_1.register_listener( this );
    def_->release_2.register_listener( this );
    def_->release_3.register_listener( this );
    def_->release_4.register_listener( this );
    def_->max_attack_time.register_listener( this );
    def_->max_decay_time.register_listener( this );
    def_->max_release_time.register_listener( this );

    sustain_time = def->sustain_time_1;
    attack = def->attack_1;
    decay = def->decay_1;
    release = def->release_1;
    max_attack_time = def->max_attack_time;
    max_decay_time = def->max_decay_time;
    max_release_time = def->max_release_time;

    update_adr_values(state);
}
NOINLINE ENVPresetData::~ENVPresetData() noexcept
{
    def->attack_1.remove_listener( this );
    def->attack_2.remove_listener( this );
    def->attack_3.remove_listener( this );
    def->attack_4.remove_listener( this );
    def->sustain_time_1.remove_listener( this );
    def->sustain_time_2.remove_listener( this );
    def->sustain_time_3.remove_listener( this );
    def->sustain_time_4.remove_listener( this );
    def->decay_1.remove_listener( this );
    def->decay_2.remove_listener( this );
    def->decay_3.remove_listener( this );
    def->decay_4.remove_listener( this );
    def->release_1.remove_listener( this );
    def->release_2.remove_listener( this );
    def->release_3.remove_listener( this );
    def->release_4.remove_listener( this );
    def->max_attack_time.remove_listener( this );
    def->max_decay_time.remove_listener( this );
    def->max_release_time.remove_listener( this );
}

//==============================================================================
static inline void copy( ENVPresetData* dest_, const ENVPresetData* src_ ) noexcept
{
    dest_->state = src_->state;

    copy( static_cast< ENVData* >(dest_), static_cast< const ENVData* >(src_) );
}
static inline void collect_saveable_parameters( ENVPresetData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->state );
}

//==============================================================================
float ENVPresetData::get_attack_at( const ENVPresetDef& def_, float state_ ) noexcept
{
    bool is_negative = state_ < 0;
    if( is_negative )
        state_*=-1;

    if( state_ <= 1.0f ) {
        float factor = 1.0f/1.0f * state_;
        return def_.attack_1*( 1.0f-factor ) + def_.attack_2 * factor;
    }
    else if( state_ <= 2.0f ) {
        float factor = 1.0f/1.0f * (state_-1);
        return def_.attack_2*( 1.0f-factor ) + def_.attack_3 * factor;
    }
    else {
        float factor = 1.0f/1.0f * (state_-2);
        return def_.attack_3*( 1.0f-factor ) + def_.attack_4 * factor;
    }
}
float ENVPresetData::get_decay_at( const ENVPresetDef& def_, float state_ ) noexcept
{
    bool is_negative = state_ < 0;
    if( is_negative )
        state_*=-1;

    if( state_ <= 1.0f ) {
        float factor = 1.0f/1.0f * state_;
        return def_.decay_1*( 1.0f-factor ) + def_.decay_2 * factor;
    }
    else if( state_ <= 2.0f ) {
        float factor = 1.0f/1.0f * (state_-1);
        return def_.decay_2*( 1.0f-factor ) + def_.decay_3 * factor;
    }
    else {
        float factor = 1.0f/1.0f * (state_-2);
        return def_.decay_3*( 1.0f-factor ) + def_.decay_4 * factor;
    }
}
float ENVPresetData::get_sustain_time_at( const ENVPresetDef& def_, float state_ ) noexcept
{
    bool is_negative = state_ < 0;
    if( is_negative )
        state_*=-1;

    if( state_ <= 1.0f ) {
        float factor = 1.0f/1.0f * state_;
        return def_.sustain_time_1*( 1.0f-factor ) + def_.sustain_time_2 * factor;
    }
    else if( state_ <= 2.0f ) {
        float factor = 1.0f/1.0f * (state_-1);
        return def_.sustain_time_2*( 1.0f-factor ) + def_.sustain_time_3 * factor;
    }
    else {
        float factor = 1.0f/1.0f * (state_-2);
        return def_.sustain_time_3*( 1.0f-factor ) + def_.sustain_time_4 * factor;
    }
}
float ENVPresetData::get_release_at( const ENVPresetDef& def_, float state_ ) noexcept
{
    bool is_negative = state_ < 0;
    if( is_negative )
        state_*=-1;

    if( state_ <= 1.0f ) {
        float factor = 1.0f/1.0f * state_;
        return def_.release_1*( 1.0f-factor ) + def_.release_2 * factor;
    }
    else if( state_ <= 2.0f ) {
        float factor = 1.0f/1.0f * (state_-1);
        return def_.release_2*( 1.0f-factor ) + def_.release_3 * factor;
    }
    else {
        float factor = 1.0f/1.0f * (state_-2);
        return def_.release_3*( 1.0f-factor ) + def_.release_4 * factor;
    }
}
void ENVPresetData::parameter_value_changed( Parameter* param_ ) noexcept
{
    if( param_ == state.ptr() )
    {
        update_adr_values(*param_);
    }
    else if( param_ == def->max_attack_time.ptr() )
    {
        max_attack_time.set_value_without_notification( def->max_attack_time.get_value() );
    }
    else if( param_ == def->max_decay_time.ptr() )
    {
        max_decay_time.set_value_without_notification( def->max_decay_time.get_value() );
    }
    else if( param_ == def->max_release_time.ptr() )
    {
        max_release_time.set_value_without_notification( def->max_release_time.get_value() );
    }
    else
    {
        update_adr_values(state);
    }
}
void ENVPresetData::parameter_value_changed_always_notification( Parameter* param_ ) noexcept
{
    update_adr_values(*param_);
}
void ENVPresetData::update_adr_values( float value_ ) noexcept
{
    if( value_ < 1 )
    {
        float factor = 1.0f/1.0f * value_;

        attack.set_value_without_notification( def->attack_1*( 1.0f-factor ) + def->attack_2 * factor );
        decay.set_value_without_notification( def->decay_1*( 1.0f-factor ) + def->decay_2 * factor );
        sustain_time.set_value_without_notification( def->sustain_time_1*( 1.0f-factor ) + def->sustain_time_2 * factor );
        release.set_value_without_notification( def->release_1*( 1.0f-factor ) + def->release_2 * factor );
    }
    else if( value_ < 2 )
    {
        float factor = 1.0f/1.0f * (value_-1.0f);

        attack.set_value_without_notification( def->attack_2*( 1.0f-factor ) + def->attack_3 * factor );
        decay.set_value_without_notification( def->decay_2*( 1.0f-factor ) + def->decay_3 * factor );
        sustain_time.set_value_without_notification( def->sustain_time_2*( 1.0f-factor ) + def->sustain_time_3 * factor );
        release.set_value_without_notification( def->release_2*( 1.0f-factor ) + def->release_3 * factor );
    }
    else if( value_ <= 3 )
    {
        float factor = 1.0f/1.0f * (value_-2.0f);

        attack.set_value_without_notification( def->attack_3*( 1.0f-factor ) + def->attack_4 * factor );
        decay.set_value_without_notification( def->decay_3*( 1.0f-factor ) + def->decay_4 * factor );
        sustain_time.set_value_without_notification( def->sustain_time_4*( 1.0f-factor ) + def->sustain_time_4 * factor );
        release.set_value_without_notification( def->release_3*( 1.0f-factor ) + def->release_4 * factor );
    }
}
void ENVPresetData::parameter_value_on_load_changed( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define FILTER_NAME "FLT"
NOINLINE FilterData::FilterData( int id_,  Array<ENVData*>& input_env_datas_  ) noexcept
:
// ----
filter_type
(
    MIN_MAX( LPF_2_PASS, MOOG_AND_LPF ),
    LPF_2_PASS,
    generate_param_name(FILTER_NAME,id_,"filter_type"),
    generate_short_human_name(FILTER_NAME,id_,"type")
),

// ----
adsr_lfo_mix
(
    MIN_MAX( -1, 1 ),
    -0.9,
    2000,
    generate_param_name(FILTER_NAME,id_,"adsr_lfo_mix"),
    generate_short_human_name(FILTER_NAME,id_,"lfo_mix")
),

// ----
distortion
(
    MIN_MAX( 0, 1 ),
    0,
    1000,
    generate_param_name(FILTER_NAME,id_,"distortion"),
    generate_short_human_name(FILTER_NAME,id_,"destroy"),
    0.6
),
modulate_distortion
(
    false,
    generate_param_name(FILTER_NAME,id_,"modulate_distortion"),
    generate_short_human_name(FILTER_NAME,id_,"mod_destroy")
),

// ----
cutoff
(
    MIN_MAX( 0.001, 1 ),
    0.2,
    1000,
    generate_param_name(FILTER_NAME,id_,"cutoff"),
    generate_short_human_name(FILTER_NAME,id_,"cutoff"),
    0.7
),
modulate_cutoff
(
    true,
    generate_param_name(FILTER_NAME,id_,"modulate_cutoff"),
    generate_short_human_name(FILTER_NAME,id_,"mod_cutoff")
),

// ----
resonance
(
    MIN_MAX( 0.001, 1 ),
    0.3,
    1000,
    generate_param_name(FILTER_NAME,id_,"resonance"),
    generate_short_human_name(FILTER_NAME,id_,"resonance"),
    0.2
),
modulate_resonance
(
    true,
    generate_param_name(FILTER_NAME,id_,"modulate_resonance"),
    generate_short_human_name(FILTER_NAME,id_,"mod_resonance")
),

// ----
width
(
    MIN_MAX( 0.001, 1 ),
    0.5,
    1000,
    generate_param_name(FILTER_NAME,id_,"width"),
    generate_short_human_name(FILTER_NAME,id_,"width"),
    0.2
),
modulate_width
(
    true,
    generate_param_name(FILTER_NAME,id_,"modulate_width"),
    generate_short_human_name(FILTER_NAME,id_,"mod_width")
),

// ----
gain
(
    MIN_MAX( 0.001, 1 ),
    0.3,
    1000,
    generate_param_name(FILTER_NAME,id_,"gain"),
    generate_short_human_name(FILTER_NAME,id_,"gain"),
    0.8
),
modulate_gain
(
    true,
    generate_param_name(FILTER_NAME,id_,"modulate_gain"),
    generate_short_human_name(FILTER_NAME,id_,"mod_gain")
),

// ----
input_env_datas( input_env_datas_ ),
input_sustains
(
    SUM_INPUTS_PER_FILTER,

    MIN_MAX( -1, 1 ),
    0,
    2000,

    FILTER_NAME,id_,
    "input_sustain","in_sustain",true
),
input_holds
(
    SUM_INPUTS_PER_FILTER,

    true,

    FILTER_NAME,id_,
    "input_hold","in_fix_sus",true
),
// ----
compressor
(
    MIN_MAX( -1, 1 ),
    0,
    2000,
    generate_param_name(FILTER_NAME,id_,"compressor"),
    generate_short_human_name(FILTER_NAME,id_,"peak")
),
output
(
    MIN_MAX( 0, 1 ),
    0.75,
    1000,
    generate_param_name(FILTER_NAME,id_,"output"),
    generate_short_human_name(FILTER_NAME,id_,"volume"),
    0.6
),
output_clipping
(
    MIN_MAX( 0, 1 ),
    1,
    1000,
    generate_param_name(FILTER_NAME,id_,"output_clipping"),
    generate_short_human_name(FILTER_NAME,id_,"clipping")
),
modulate_output
(
    false,
    generate_param_name(FILTER_NAME,id_,"modulate_output"),
    generate_short_human_name(FILTER_NAME,id_,"mod_volume")
)
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        input_sustains[i].register_always_listener(this);
}

NOINLINE FilterData::~FilterData() noexcept
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        input_sustains[i].remove_listener(this);
}

//==============================================================================
static inline void copy( FilterData* dest_, const FilterData* src_ ) noexcept
{
    dest_->filter_type = src_->filter_type;
    dest_->adsr_lfo_mix = src_->adsr_lfo_mix;
    dest_->distortion = src_->distortion;
    dest_->modulate_distortion = src_->modulate_distortion;
    dest_->cutoff = src_->cutoff;
    dest_->modulate_gain = src_->modulate_gain;
    dest_->resonance = src_->resonance;
    dest_->modulate_resonance = src_->modulate_resonance;
    dest_->gain = src_->gain;
    dest_->modulate_gain = src_->modulate_gain;
    dest_->width = src_->width;
    dest_->modulate_width = src_->modulate_width;
    dest_->compressor = src_->compressor;
    dest_->output = src_->output;
    dest_->output_clipping = src_->output_clipping;
    dest_->modulate_output = src_->modulate_output;

    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
    {
        dest_->input_holds[i] = src_->input_holds[i];
        dest_->input_sustains[i] = src_->input_sustains[i];

        copy( dest_->input_env_datas.getUnchecked(i), src_->input_env_datas.getUnchecked(i) );
    }
}
static inline void collect_saveable_parameters( FilterData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->filter_type );
    params_.add( &data_->adsr_lfo_mix );
    params_.add( &data_->distortion );
    params_.add( &data_->modulate_distortion );
    params_.add( &data_->cutoff );
    params_.add( &data_->modulate_cutoff );
    params_.add( &data_->resonance );
    params_.add( &data_->modulate_resonance );
    params_.add( &data_->gain );
    params_.add( &data_->modulate_gain );
    params_.add( &data_->width );
    params_.add( &data_->modulate_width );
    params_.add( &data_->compressor );
    params_.add( &data_->output );
    params_.add( &data_->modulate_output );
    params_.add( &data_->output_clipping );

    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
    {
        params_.add( &data_->input_holds[i] );
        params_.add( &data_->input_sustains[i] );

        collect_saveable_parameters( data_->input_env_datas.getUnchecked(i), params_ );
    }
}

//==============================================================================
void FilterData::parameter_value_changed( Parameter* param_ ) noexcept
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
    {
        if( input_sustains[i].ptr() == param_ )
        {
            input_env_datas[i]->sustain.set_value_without_notification( positive( param_->get_value() ) );
            break;
        }
    }
}
void FilterData::parameter_value_changed_always_notification( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void FilterData::parameter_value_on_load_changed( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define ARP_NAME "ARP"
NOINLINE ArpSequencerData::ArpSequencerData( int id_ ) noexcept
:
is_on
(
    true,
    generate_param_name(ARP_NAME,id_,"is_on"),
    generate_short_human_name(ARP_NAME,"on")
),

// ----
step
(
    SUM_ENV_ARP_STEPS,

    false,

    ARP_NAME,id_,
    "step","step", false
),
tune
(
    SUM_ENV_ARP_STEPS,

    MIN_MAX( -48, 48 ),
    false,
    96*10,

    ARP_NAME,id_,
    "tune","tune", false
),
velocity
(
    SUM_ENV_ARP_STEPS,

    MIN_MAX( 0.001, 1 ),
    0.85,
    1000,

    ARP_NAME,id_,
    "velocity","velocity", false
),

// ----
shuffle
(
    MIN_MAX( 0, 1 ),
    0.333,
    1000,
    generate_param_name(ARP_NAME,id_,"shuffle"),
    generate_short_human_name(ARP_NAME,"shuffle")
),
connect
(
    false,
    generate_param_name(ARP_NAME,id_,"connect"),
    generate_short_human_name(ARP_NAME,"connect")
),

// ----
speed_multi
(
    MIN_MAX( -9, 9 ),
    0,
    generate_param_name(ARP_NAME,id_,"speed_multi"),
    generate_short_human_name(ARP_NAME,"speed_multi")
)
{}

NOINLINE ArpSequencerData::~ArpSequencerData() noexcept {}

//==============================================================================
static inline void copy( ArpSequencerData* dest_, const ArpSequencerData* src_ ) noexcept
{
    dest_->is_on = src_->is_on;

    for( int i = 0 ; i != SUM_ENV_ARP_STEPS ; ++i )
    {

        dest_->step[i] = src_->step[i];
        dest_->tune[i] = src_->tune[i];
        dest_->velocity[i] = src_->velocity[i];
    }

    dest_->shuffle = src_->shuffle;
    dest_->connect = src_->connect;
    dest_->speed_multi = src_->speed_multi;
}
static inline void collect_saveable_parameters( ArpSequencerData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->is_on );

    for( int i = 0 ; i != SUM_ENV_ARP_STEPS ; ++i ) {
        params_.add( &data_->step[i] );
        params_.add( &data_->tune[i] );
        params_.add( &data_->velocity[i] );
    }

    params_.add( &data_->shuffle );
    params_.add( &data_->connect );

    params_.add( &data_->speed_multi );
}

//==============================================================================
StringRef ArpSequencerData::speed_multi_to_text( int speed_multi_ ) noexcept
{
    switch( speed_multi_ )
    {
    case 0 :
        return "x1";
    case 1 :
        return "x2";
    case -1 :
        return "/2";
    case 2 :
        return "x3";
    case -2 :
        return "/3";
    case 3 :
        return "x4";
    case -3 :
        return "/4";
    case 4 :
        return "x5";
    case -4 :
        return "/5";
    case 5 :
        return "x7";
    case -5 :
        return "/7";
    case 6 :
        return "x8";
    case -6 :
        return "/8";
    case 7 :
        return "x9";
    case -7 :
        return "/9";
    case 8 :
        return "x12";
    case -8 :
        return "/12";
    case 9 :
        return "x16";
    default /*-9*/ :
        return "/16";
    }
}
double ArpSequencerData::speed_multi_to_value( int speed_multi_ ) noexcept
{
    switch( speed_multi_ )
    {
    case 0 :
        return 1;
    case 1 :
        return 2;
    case -1 :
        return 0.5;
    case 2 :
        return 3;
    case -2 :
        return (1.0/3);
    case 3 :
        return 4;
    case -3 :
        return (1.0/4);
    case 4 :
        return 5;
    case -4 :
        return (1.0/5);
    case 5 :
        return 7;
    case -5 :
        return (1.0/7);
    case 6 :
        return 8;
    case -6 :
        return (1.0/8);
    case 7 :
        return 9;
    case -7 :
        return (1.0/9);
    case 8 :
        return 12;
    case -8 :
        return (1.0/12);
    case 9 :
        return 16;
    default /*-9*/ :
        return (1.0/16);
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
#define EQ_NAME "EQ"
NOINLINE EQData::EQData( int id_, ENVPresetDef*const def_ ) noexcept
:
velocity
(
    SUM_EQ_BANDS,

    MIN_MAX( -1, 1 ),
    0,
    2000,

    EQ_NAME,id_,
    "velocity","velocity", false
),
hold
(
    SUM_EQ_BANDS,

    true,

    EQ_NAME,id_,
    "hold","velocity", false
)
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        ENVPresetData* data = new ENVPresetData( band_id+EQ_ENV_ID_OFFSET, def_ );
        env_datas.add( data );

        velocity[band_id].register_always_listener(this);
    }
}
NOINLINE EQData::~EQData() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        velocity[band_id].remove_listener(this);
    }
}

//==============================================================================
static inline void copy( EQData* dest_, const EQData* src_ ) noexcept
{
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        dest_->velocity[i] = src_->velocity[i];
        dest_->hold[i] = src_->hold[i];

        copy( dest_->env_datas.getUnchecked( i ), src_->env_datas.getUnchecked( i ) );
    }
}
static inline void collect_saveable_parameters( EQData* data_, Array< Parameter* >& params_ ) noexcept
{
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        collect_saveable_parameters( data_->env_datas.getUnchecked( i ), params_ );

        params_.add( &data_->velocity[i] );
        params_.add( &data_->hold[i] );
    }
}

//==============================================================================
void EQData::parameter_value_changed( Parameter* param_ ) noexcept
{
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        if( velocity[i].ptr() == param_ )
        {
            env_datas[i]->sustain.set_value_without_notification( positive( param_->get_value() ) );
            break;
        }
    }
}
void EQData::parameter_value_on_load_changed( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void EQData::parameter_value_changed_always_notification( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define REVERB_NAME "VERB"
NOINLINE ReverbData::ReverbData( int id_ ) noexcept
:
room
(
    MIN_MAX( 0, 1 ),
    0.333,
    1000,
    generate_param_name(REVERB_NAME,id_,"room"),
    generate_short_human_name(REVERB_NAME,id_,"room")
),
dry_wet_mix
(
    MIN_MAX( 0, 1 ),
    0.75,
    1000,
    generate_param_name(REVERB_NAME,id_,"dry-wet"),
    generate_short_human_name(REVERB_NAME,id_,"dry-wet")
),
width
(
    MIN_MAX( 0, 1 ),
    0.3,
    1000,
    generate_param_name(REVERB_NAME,id_,"width"),
    generate_short_human_name(REVERB_NAME,id_,"width")
)
{}

NOINLINE ReverbData::~ReverbData() noexcept {}

//==============================================================================
static inline void copy( ReverbData* dest_, const ReverbData* src_ ) noexcept
{
    dest_->room = src_->room;
    dest_->dry_wet_mix = src_->dry_wet_mix;
    dest_->width = src_->width;
}
static inline void collect_saveable_parameters( ReverbData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->room );
    params_.add( &data_->dry_wet_mix );
    params_.add( &data_->width );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define CHORUS_NAME "CHR"
NOINLINE ChorusData::ChorusData( int id_, ENVPresetDef*const def_ ) noexcept
:
modulation
(
    MIN_MAX( 0, 1 ),
    0.333,
    1000,
    generate_param_name(CHORUS_NAME,id_,"modulation"),
    generate_short_human_name(CHORUS_NAME,id_,"chorus")
),
hold_modulation
(
    true,
    generate_param_name(CHORUS_NAME,id_,"hold-modulation"),
    generate_short_human_name(CHORUS_NAME,id_,"chorus-fix")
),

// ----
modulation_env_data( new ENVPresetData( CHORUS_ENV_ID_OFFSET, def_ ) ),
shine_env_data( new ENVPresetData( CHORUS_ENV_ID_OFFSET, def_ ) )
{
    modulation.register_always_listener(this);
}
NOINLINE ChorusData::~ChorusData() noexcept
{
    delete shine_env_data;
    delete modulation_env_data;
}

//==============================================================================
static inline void copy( ChorusData* dest_, const ChorusData* src_ ) noexcept
{
    dest_->modulation = src_->modulation;
    dest_->hold_modulation = src_->hold_modulation;
}
static inline void collect_saveable_parameters( ChorusData* data_, Array< Parameter* >& params_ ) noexcept
{
    params_.add( &data_->modulation );
    params_.add( &data_->hold_modulation );
}

//==============================================================================
void ChorusData::parameter_value_changed( Parameter* param_ ) noexcept
{
    modulation_env_data->sustain.set_value( positive( param_->get_value() ) );
}
void ChorusData::parameter_value_on_load_changed( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void ChorusData::parameter_value_changed_always_notification( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define SYNTH_DATA_NAME "SD"
NOINLINE SynthData::SynthData( DATA_TYPES data_type )
    :
    id( data_type ),

    volume
    (
        MIN_MAX( 0, 1 ),
        0.9,
        1000,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"volume"),
        generate_short_human_name("MAIN","volume")
    ),
    glide
    (
        MIN_MAX( 0, 1 ),
        0.05,
        1000,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"glide"),
        generate_short_human_name("MAIN","glide")
    ),
    delay
    (
        MIN_MAX( 0, 1 ),
        0,
        1000,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"delay"),
        generate_short_human_name("MAIN","delay")
    ),
    effect_bypass
    (
        MIN_MAX( 0, 1 ),
        1,
        1000,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"effect_bypass"),
        generate_short_human_name("MAIN","fx_bypass")
    ),
    final_compression
    (
        MIN_MAX( 0, 1 ),
        0.7,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"final_compression"),
        generate_short_human_name("MAIN","clipping")
    ),
    colour
    (
        MIN_MAX( 0, 1 ),
        0.9,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"colour"),
        generate_short_human_name("MAIN","colour")
    ),
    resonance
    (
        MIN_MAX( 0, 1 ),
        0.05,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"shape"),
        generate_short_human_name("MAIN","shape")
    ),
    curve_shape
    (
        MIN_MAX( 0, 1 ),
        0.5,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"env_shape"),
        generate_short_human_name("MAIN","env_shape")
    ),
    octave_offset
    (
        MIN_MAX( -2, 2 ),
        0,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"octave_offset"),
        generate_short_human_name("MAIN","octave")
    ),

    sync
    (
        true,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"sync"),
        generate_short_human_name("MAIN","sync")
    ),
    speed
    (
        MIN_MAX( 20, 1000 ),
        128,
        980*10,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"speed"),
        generate_short_human_name("MAIN","speed")
    ),

    glide_motor_time
    (
        MIN_MAX( 1, 20000 ),
        500,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"glide_motor_time"),
        generate_short_human_name("MAIN","glide_motor_time")
    ),
    velocity_glide_time
    (
        MIN_MAX( 1, 20000 ),
        500,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"velocity_glide_time"),
        generate_short_human_name("MAIN","velocity_glide_time")
    ),

    ctrl
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"ctrl"),
        generate_short_human_name("MAIN","ctrl")
    ),
    midi_pickup_offset
    (
        MIN_MAX( 0, 1 ),
        0.2,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"midi_pickup_offset"),
        generate_short_human_name("MAIN","midi_pick_up")
    ),

    // -------------------------------------------------------------
    osci_show_osc_1
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_osc_1"),
        generate_short_human_name("MAIN","osci_show_osc_1")
    ),
    osci_show_osc_2
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_osc_2"),
        generate_short_human_name("MAIN","osci_show_osc_2")
    ),
    osci_show_osc_3
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_osc_3"),
        generate_short_human_name("MAIN","osci_show_osc_3")
    ),
    osci_show_flt_env_1
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_env_1"),
        generate_short_human_name("MAIN","osci_show_flt_env_1")
    ),
    osci_show_flt_env_2
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_env_2"),
        generate_short_human_name("MAIN","osci_show_flt_env_2")
    ),
    osci_show_flt_env_3
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_env_3"),
        generate_short_human_name("MAIN","osci_show_flt_env_3")
    ),
    osci_show_flt_1
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_1"),
        generate_short_human_name("MAIN","osci_show_flt_1")
    ),
    osci_show_flt_2
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_2"),
        generate_short_human_name("MAIN","osci_show_flt_2")
    ),
    osci_show_flt_3
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_flt_3"),
        generate_short_human_name("MAIN","osci_show_flt_3")
    ),
    osci_show_eq
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_eq"),
        generate_short_human_name("MAIN","osci_show_eq")
    ),
    osci_show_out
    (
        true,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_out"),
        generate_short_human_name("MAIN","osci_show_out")
    ),
    osci_show_out_env
    (
        false,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_out_env"),
        generate_short_human_name("MAIN","osci_show_out_env")
    ),
    osci_show_range
    (
        MIN_MAX( 0, 1 ),
        0.05,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"osci_show_range"),
        generate_short_human_name("MAIN","osci_show_range")
    ),

    // -------------------------------------------------------------
    num_extra_threads
    (
        MIN_MAX( 0, THREAD_LIMIT ),
        0,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"cpus"),
        generate_short_human_name("MAIN","cpus")
    ),

    // -------------------------------------------------------------
    animate_input_env
    (
        true,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_input_env"),
        generate_short_human_name("animate_input_env")
    ),
    animate_eq_env
    (
        true,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_eq_env"),
        generate_short_human_name("animate_eq_env")
    ),
    animate_modulations
    (
        true,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_modulations"),
        generate_short_human_name("animate_modulations")
    ),

    // -------------------------------------------------------------
    morhp_states
    (
        SUM_MORPHER_GROUPS,

        MIN_MAX( 0, 1 ),
        0,
        1000,

        SYNTH_DATA_NAME,MASTER,
        "morph_state","morph",false
    ),
    morhp_switch_states
    (
        SUM_MORPHER_GROUPS,

        LEFT,

        SYNTH_DATA_NAME,MASTER,
        "morph_switch_state","morph_tgl",false
    ),
    linear_morhp_state
    (
        MIN_MAX( 0, 3 ),
        0,
        100,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"linear_morhp_state"),
        generate_short_human_name("morph_line")
    ),
    morph_motor_time
    (
        MIN_MAX( 1, 20000 ),
        1000,
        generate_param_name(SYNTH_DATA_NAME,MASTER,"morph_motor_time"),
        generate_short_human_name("morph_motor")
    ),


    env_preset_def(new ENVPresetDef( MASTER ) ),
    eq_data(new EQData(MASTER, env_preset_def)),
    arp_sequencer_data(new ArpSequencerData( MASTER )),
    reverb_data(new ReverbData( MASTER ) ),
    chorus_data(new ChorusData( MASTER, env_preset_def )),


    current_program(-1),
    current_program_abs(-1),
    current_bank(0)
{
    if( data_type == MASTER )
    {
        mono_ParameterOwnerStore::getInstance()->env_preset_def = env_preset_def;
        mono_ParameterOwnerStore::getInstance()->eq_data = eq_data;
        mono_ParameterOwnerStore::getInstance()->arp_data = arp_sequencer_data;
        mono_ParameterOwnerStore::getInstance()->reverb_data = reverb_data;
        mono_ParameterOwnerStore::getInstance()->chorus_data = chorus_data;
        mono_ParameterOwnerStore::getInstance()->synth_data = this;
    }

    for( int i = 0 ; i != SUM_OSCS ; ++i )
    {
        OSCData* data = new OSCData(i);
        osc_datas.add( data );
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->osc_datas.add( data );
    }

    for( int i = 0 ; i != SUM_LFOS ; ++i )
    {
        LFOData* data = new LFOData(i);
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->lfo_datas.add( data );
        lfo_datas.add( data );
    }

    for( int i = 0 ; i != SUM_FILTERS+1 ; ++i )
    {
        ENVData* data = new ENVData(i);
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->env_datas.add( data );
        env_datas.add( data );
    }

    for( int i = 0 ; i != SUM_FILTERS ; ++i )
    {
        Array< ENVData* > input_envs;
        for( int j = 0 ; j != SUM_INPUTS_PER_FILTER ; ++j )
        {
            ENVPresetData* filter_env_data = new ENVPresetData( j+i*SUM_INPUTS_PER_FILTER+FILTER_INPUT_ENV_ID_OFFSET, env_preset_def );
            if( data_type == MASTER )
                mono_ParameterOwnerStore::getInstance()->filter_input_env_datas.add( filter_env_data );
            filter_input_env_datas.add( filter_env_data );

            input_envs.add( filter_env_data );
        }

        FilterData* filter_data = new FilterData(i,input_envs);
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->filter_datas.add( filter_data );
        filter_datas.add( filter_data );
    }


    init_morph_groups( data_type );
    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
    {
        morhp_states[morpher_id].register_listener(this);
    }
    linear_morhp_state.register_listener(this);

    colect_saveable_parameters();

    if( data_type == MASTER )
        refresh_banks_and_programms();
}
NOINLINE SynthData::~SynthData()
{
    eq_data = nullptr;
    arp_sequencer_data = nullptr;
    reverb_data = nullptr;
    chorus_data = nullptr;

    lfo_datas.clear();
    osc_datas.clear();
    filter_input_env_datas.clear();
    filter_datas.clear();
    env_datas.clear();

    // AS LAST!! unregister listeners
    env_preset_def = nullptr;
}

//==============================================================================
inline const SynthData& SynthData::operator= ( const SynthData& other_ ) noexcept {
    COPY_FROM_OTHER( volume )
    COPY_FROM_OTHER( glide )
    COPY_FROM_OTHER( delay )
    COPY_FROM_OTHER( effect_bypass )
    COPY_FROM_OTHER( speed )
    COPY_FROM_OTHER( glide_motor_time )
    COPY_FROM_OTHER( velocity_glide_time )
    COPY_FROM_OTHER( sync )

    COPY_FROM_OTHER( colour )
    COPY_FROM_OTHER( resonance )
    COPY_FROM_OTHER( curve_shape )
    COPY_FROM_OTHER( octave_offset )

    for( int i = 0 ; i != other_.lfo_datas.size() ; ++i )
    {
        copy( lfo_datas[i], other_.lfo_datas[i] );
    }

    for( int i = 0 ; i != other_.osc_datas.size() ; ++i )
    {
        copy( osc_datas[i], other_.osc_datas[i] );
    }

    for( int i = 0 ; i != other_.env_datas.size() ; ++i )
    {
        copy( env_datas[i], other_.env_datas[i] );
    }

    for( int i = 0 ; i != other_.filter_datas.size() ; ++i )
    {
        copy( filter_datas[i], other_.filter_datas[i] );
    }

    copy( env_preset_def, other_.env_preset_def );
    copy( eq_data, other_.eq_data );
    copy( arp_sequencer_data, other_.arp_sequencer_data );
    copy( reverb_data, other_.reverb_data );
    copy( chorus_data, other_.chorus_data );

    final_compression = other_.final_compression;

    // NO NEED FOR COPY
    // morhp_states

    return *this;
}
NOINLINE void SynthData::get_saveable_params( Array< Parameter* >& params_ ) noexcept
{
    params_.add( &volume );
    params_.add( &glide );
    params_.add( &delay );
    params_.add( &effect_bypass );
    params_.add( &final_compression );
    params_.add( &colour );
    params_.add( &resonance );
    params_.add( &curve_shape );
    params_.add( &octave_offset );
    params_.add( &morph_motor_time );
    params_.add( &morph_motor_time );
    params_.add( &speed );
    params_.add( &glide_motor_time );
    params_.add( &velocity_glide_time );
    params_.add( &midi_pickup_offset );
    params_.add( &ctrl );
    params_.add( &animate_input_env );
    params_.add( &animate_eq_env );
    params_.add( &animate_modulations );
    params_.add( &sync );

    params_.add( &osci_show_osc_1 );
    params_.add( &osci_show_osc_2 );
    params_.add( &osci_show_osc_3 );
    params_.add( &osci_show_flt_env_1 );
    params_.add( &osci_show_flt_env_2 );
    params_.add( &osci_show_flt_env_3 );
    params_.add( &osci_show_flt_1 );
    params_.add( &osci_show_flt_2 );
    params_.add( &osci_show_flt_3 );
    params_.add( &osci_show_eq );
    params_.add( &osci_show_out );
    params_.add( &osci_show_out_env );
    params_.add( &osci_show_range );

    params_.add( &num_extra_threads );

    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
    {
        params_.add( &morhp_states[morpher_id] );
        params_.add( &morhp_switch_states[morpher_id] );
    }
}
NOINLINE void SynthData::colect_saveable_parameters() noexcept {

    // on top to be the first on load and get the right update order (bit hacky, but ok ;--)
    collect_saveable_parameters( env_preset_def, saveable_parameters );

    for( int i = 0 ; i != SUM_LFOS ; ++i )
    {
        collect_saveable_parameters( lfo_datas[i], saveable_parameters );
    }

    for( int i = 0 ; i != SUM_OSCS ;  ++i )
    {
        collect_saveable_parameters( osc_datas[i], saveable_parameters );
    }

    for( int flt_id = 0 ; flt_id != SUM_FILTERS ; ++flt_id )
    {
        collect_saveable_parameters( filter_datas[flt_id], saveable_parameters );
    }

    for( int i = 0 ; i != SUM_ENVS ; ++i )
    {
        collect_saveable_parameters( env_datas[i], saveable_parameters );
    }

    collect_saveable_parameters( eq_data, saveable_parameters );
    collect_saveable_parameters( arp_sequencer_data, saveable_parameters );
    collect_saveable_parameters( reverb_data, saveable_parameters );
    collect_saveable_parameters( chorus_data, saveable_parameters );

    get_saveable_params( saveable_parameters );
}
NOINLINE void SynthData::init_morph_groups( DATA_TYPES data_type ) noexcept
{
    {
        // MAIN
        {
            morph_groups.add( &morph_group_main );
            morph_group_main.set_id( MAIN );

            morph_group_main.register_parameter( volume.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( final_compression.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->attack.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_attack_time.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->decay.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_decay_time.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->sustain.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->sustain_time.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->release.ptr(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_release_time.ptr(), data_type == MASTER  );

            morph_group_main.register_parameter( env_preset_def->attack_1.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_1.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_1.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_2.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_2.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_2.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_3.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_3.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_3.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_4.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_4.ptr(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_4.ptr(), data_type == MASTER );

            //speed_multi
        }

        // OSC'S
        {
            {
                morph_groups.add( &morph_group_osc_1 );
                morph_group_osc_1.set_id( OSC_1 );

                morph_group_osc_1.register_parameter( osc_datas[0]->wave.ptr(), data_type == MASTER );
                morph_group_osc_1.register_parameter( osc_datas[0]->octave.ptr(), data_type == MASTER );
                morph_group_osc_1.register_parameter( osc_datas[0]->fm_amount.ptr(), data_type == MASTER );

                morph_group_osc_1.register_switch_parameter( osc_datas[0]->is_lfo_modulated.bool_ptr(), data_type == MASTER );
                morph_group_osc_1.register_switch_parameter( osc_datas[0]->sync.bool_ptr(), data_type == MASTER );

                morph_group_osc_1.register_switch_parameter( osc_datas[0]->puls_width.int_ptr(), data_type == MASTER );
                morph_group_osc_1.register_switch_parameter( osc_datas[0]->osc_switch.int_ptr(), data_type == MASTER );
            }

            {
                morph_groups.add( &morph_group_osc_2 );
                morph_group_osc_2.set_id( OSC_2 );

                morph_group_osc_2.register_parameter( osc_datas[1]->wave.ptr(), data_type == MASTER );
                morph_group_osc_2.register_parameter( osc_datas[1]->octave.ptr(), data_type == MASTER );
                morph_group_osc_2.register_parameter( osc_datas[1]->fm_amount.ptr(), data_type == MASTER );

                morph_group_osc_2.register_switch_parameter( osc_datas[1]->is_lfo_modulated.bool_ptr(), data_type == MASTER );
                morph_group_osc_2.register_switch_parameter( osc_datas[1]->sync.bool_ptr(), data_type == MASTER );

                morph_group_osc_2.register_switch_parameter( osc_datas[1]->puls_width.int_ptr(), data_type == MASTER );
                morph_group_osc_2.register_switch_parameter( osc_datas[1]->osc_switch.int_ptr(), data_type == MASTER );
            }

            {
                morph_groups.add( &morph_group_osc_3 );
                morph_group_osc_3.set_id( OSC_3 );

                morph_group_osc_3.register_parameter( osc_datas[2]->wave.ptr() , data_type == MASTER  );
                morph_group_osc_3.register_parameter( osc_datas[2]->octave.ptr(), data_type == MASTER  );
                morph_group_osc_3.register_parameter( osc_datas[2]->fm_amount.ptr(), data_type == MASTER  );

                morph_group_osc_3.register_switch_parameter( osc_datas[2]->is_lfo_modulated.bool_ptr(), data_type == MASTER  );
                morph_group_osc_3.register_switch_parameter( osc_datas[2]->sync.bool_ptr(), data_type == MASTER  );

                morph_group_osc_3.register_switch_parameter( osc_datas[2]->puls_width.int_ptr(), data_type == MASTER );
                morph_group_osc_3.register_switch_parameter( osc_datas[2]->osc_switch.int_ptr(), data_type == MASTER );
            }
        }

        // FM
        {
            morph_groups.add( &morph_group_fm );
            morph_group_fm.set_id( FM );

            morph_group_fm.register_parameter( osc_datas[0]->fm_multi.ptr(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[0]->fm_swing.ptr(), data_type == MASTER  );
            // TODO
            //morph_group_fm.register_parameter( osc_datas[0]->osc_swing.ptr(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[1]->fm_multi.ptr(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[2]->fm_multi.ptr(), data_type == MASTER  );

            morph_group_fm.register_switch_parameter( osc_datas[0]->puls_width.int_ptr(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[1]->puls_width.int_ptr(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[2]->puls_width.int_ptr(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[0]->fm_wave.bool_ptr(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[1]->fm_wave.bool_ptr(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[2]->fm_wave.bool_ptr(), data_type == MASTER  );
        }

        // FILTERS
        {
            {
                morph_groups.add( &morph_group_filter_1 );
                morph_group_filter_1.set_id( FILTER_1 );

                // FLT
                morph_group_filter_1.register_parameter( filter_datas[0]->adsr_lfo_mix.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->distortion.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->cutoff.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->resonance.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->width.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->gain.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->output.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->output_clipping.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->compressor.ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_1.register_parameter( filter_input_env_datas[input_id+0*SUM_INPUTS_PER_FILTER]->state.ptr(), data_type == MASTER  );
                    morph_group_filter_1.register_parameter( filter_datas[0]->input_sustains[input_id].ptr(), data_type == MASTER  );
                }

                morph_group_filter_1.register_switch_parameter( filter_datas[0]->filter_type.int_ptr(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_distortion.bool_ptr(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_cutoff.bool_ptr(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_resonance.bool_ptr(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_width.bool_ptr(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_gain.bool_ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_1.register_switch_parameter( reinterpret_cast< BoolParameter* >( filter_datas[0]->input_holds[input_id].ptr() ), data_type == MASTER  );
                }
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_output.bool_ptr(), data_type == MASTER  );

                // LFO
                morph_group_filter_1.register_parameter( lfo_datas[0]->speed.ptr(), data_type == MASTER  );

                // ENV
                morph_group_filter_1.register_parameter( env_datas[0]->attack.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_attack_time.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->decay.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_decay_time.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->sustain.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->sustain_time.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->release.ptr(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_release_time.ptr(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_filter_2 );
                morph_group_filter_2.set_id( FILTER_2 );

                // FLT
                morph_group_filter_2.register_parameter( filter_datas[1]->adsr_lfo_mix.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->distortion.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->cutoff.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->resonance.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->width.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->gain.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->output.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->output_clipping.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->compressor.ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_2.register_parameter( filter_input_env_datas[input_id+1*SUM_INPUTS_PER_FILTER]->state.ptr(), data_type == MASTER  );
                    morph_group_filter_2.register_parameter( filter_datas[1]->input_sustains[input_id].ptr(), data_type == MASTER  );
                }

                morph_group_filter_2.register_switch_parameter( filter_datas[1]->filter_type.int_ptr(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_distortion.bool_ptr(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_cutoff.bool_ptr(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_resonance.bool_ptr(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_width.bool_ptr(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_gain.bool_ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_2.register_switch_parameter( reinterpret_cast< BoolParameter* >( filter_datas[1]->input_holds[input_id].ptr() ), data_type == MASTER  );
                }
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_output.bool_ptr(), data_type == MASTER  );

                // LFO
                morph_group_filter_2.register_parameter( lfo_datas[1]->speed.ptr(), data_type == MASTER  );

                // ENV
                morph_group_filter_2.register_parameter( env_datas[1]->attack.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_attack_time.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->decay.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_decay_time.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->sustain.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->sustain_time.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->release.ptr(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_release_time.ptr(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_filter_3 );
                morph_group_filter_3.set_id( FILTER_3 );

                // FLT
                morph_group_filter_3.register_parameter( filter_datas[2]->adsr_lfo_mix.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->distortion.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->cutoff.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->resonance.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->width.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->gain.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->output.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->output_clipping.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->compressor.ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_3.register_parameter( filter_input_env_datas[input_id+2*SUM_INPUTS_PER_FILTER]->state.ptr(), data_type == MASTER  );
                    morph_group_filter_3.register_parameter( filter_datas[2]->input_sustains[input_id].ptr(), data_type == MASTER  );
                }

                morph_group_filter_3.register_switch_parameter( filter_datas[2]->filter_type.int_ptr(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_distortion.bool_ptr(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_cutoff.bool_ptr(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_resonance.bool_ptr(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_width.bool_ptr(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_gain.bool_ptr(), data_type == MASTER  );
                for( int input_id = 0 ;
                input_id != SUM_INPUTS_PER_FILTER ;
                ++input_id )
                {
                    morph_group_filter_3.register_switch_parameter( reinterpret_cast< BoolParameter* >( filter_datas[2]->input_holds[input_id].ptr() ), data_type == MASTER  );
                }
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_output.bool_ptr(), data_type == MASTER  );

                // LFO
                morph_group_filter_3.register_parameter( lfo_datas[2]->speed.ptr(), data_type == MASTER  );

                // ENV
                morph_group_filter_3.register_parameter( env_datas[2]->attack.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_attack_time.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->decay.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_decay_time.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->sustain.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->sustain_time.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->release.ptr(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_release_time.ptr(), data_type == MASTER  );
            }
        }


        // EQ
        {
            morph_groups.add( &morph_group_eq );
            morph_group_eq.set_id( EQ );

            for( int band_id = 0 ;
            band_id != SUM_EQ_BANDS ;
            ++band_id ) {
                morph_group_eq.register_parameter( eq_data->velocity[band_id].ptr(), data_type == MASTER  );
                morph_group_eq.register_parameter( eq_data->env_datas[band_id]->state.ptr(), data_type == MASTER  );

                morph_group_eq.register_switch_parameter( eq_data->hold[band_id].bool_ptr(), data_type == MASTER  );
            }
        }

        // FX
        {
            morph_groups.add( &morph_group_fx );
            morph_group_fx.set_id( FX );

            // MAIN
            morph_group_fx.register_parameter( colour.ptr(), data_type == MASTER );
            morph_group_fx.register_parameter( resonance.ptr(), data_type == MASTER );
            morph_group_fx.register_parameter( effect_bypass.ptr(), data_type == MASTER  );
            // REVERB
            morph_group_fx.register_parameter( reverb_data->room.ptr(), data_type == MASTER  );
            morph_group_fx.register_parameter( reverb_data->dry_wet_mix.ptr(), data_type == MASTER  );
            morph_group_fx.register_parameter( reverb_data->width.ptr(), data_type == MASTER  );
            // DELAY
            morph_group_fx.register_parameter( delay.ptr(), data_type == MASTER  );
            // CHORUS
            morph_group_fx.register_parameter( chorus_data->modulation.ptr(), data_type == MASTER  );
            morph_group_fx.register_switch_parameter( chorus_data->hold_modulation.bool_ptr(), data_type == MASTER  );
        }

        // ARP
        {
            {
                morph_groups.add( &morph_group_arp_tune );
                morph_group_arp_tune.set_id( ARP_TUNE );

                for( int step_id = 0 ;
                step_id != SUM_ENV_ARP_STEPS ;
                ++step_id ) {
                    morph_group_arp_tune.register_parameter( arp_sequencer_data->tune[step_id].ptr(), data_type == MASTER  );
                }
            }

            {
                morph_groups.add( &morph_group_arp_velocity );
                morph_group_arp_velocity.set_id( ARP_VELOCITY );

                for( int step_id = 0 ;
                step_id != SUM_ENV_ARP_STEPS ;
                ++step_id ) {
                    morph_group_arp_velocity.register_parameter( arp_sequencer_data->velocity[step_id].ptr(), data_type == MASTER  );
                }
            }

            {
                morph_groups.add( &morph_group_arp_glide_shuffle );
                morph_group_arp_glide_shuffle.set_id( ARP_GLIDE_SHUFFLE );

                morph_group_arp_glide_shuffle.register_parameter( arp_sequencer_data->shuffle.ptr(), data_type == MASTER  );
                morph_group_arp_glide_shuffle.register_parameter( glide.ptr(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_arp_switchs );
                morph_group_arp_switchs.set_id( ARP_SWITCHS );

                // is_on
                // speed_multi
                morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data->connect.bool_ptr(), data_type == MASTER  );
                //morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data.connect.ptr(), data_type == MASTER  );
                for( int step_id = 0 ; step_id != SUM_ENV_ARP_STEPS ; ++step_id )
                {
                    morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data->step[step_id].bool_ptr(), data_type == MASTER  );
                }
            }
        }
    }

    // MAKE IT HOT
    // ONLY THE MASTER HAS MORPHE SORCES - OTHERWISE WE BUILD UNLIMITED SOURCES FOR SOURCE
    if( data_type == MASTER )
    {
        for( int morpher_group_id = 0 ; morpher_group_id != SUM_MORPHER_GROUPS ; ++morpher_group_id )
        {
            left_morph_datas.add( new SynthData(static_cast< DATA_TYPES >( MORPH ) ) );
            right_morph_datas.add( new SynthData(static_cast< DATA_TYPES >( MORPH ) ) );
        }
        {
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id ) {
                morpher_selections.add( new MorpherSelection() );
            }
            activate_morph_selection( 0, OSCS );
            activate_morph_selection( 0, FM );
            activate_morph_selection( 1, FILTERS );
            activate_morph_selection( 2, EQ );
            activate_morph_selection( 2, FX );
            activate_morph_selection( 2, MAIN );
            activate_morph_selection( 3, ARP );
        }
    }
}

void SynthData::MorpherSelection::activate(SynthData::MORPH_SELCTIONS_IDS id_, OwnedArray< MorpherSelection >& peers_) {
    // COLLECT IDS TO ENABLE AND DISABLE
    Array< MORPH_SELCTIONS_IDS > ids;
    if( id_ == ALL ) {
        ids.add( ALL );
        ids.add( MAIN );
        ids.add( OSCS );
        ids.add( OSC_1 );
        ids.add( OSC_2 );
        ids.add( OSC_3 );
        ids.add( FILTERS );
        ids.add( FILTER_1 );
        ids.add( FILTER_2 );
        ids.add( FILTER_3 );
        ids.add( FM );
        ids.add( FX );
        ids.add( EQ );
        ids.add( ARP );
        ids.add( ARP_TUNE );
        ids.add( ARP_VELOCITY );
        ids.add( ARP_GLIDE_SHUFFLE );
        ids.add( ARP_SWITCHS );
    }
    else if( id_ == OSCS ) {
        ids.add( OSCS );
        ids.add( OSC_1 );
        ids.add( OSC_2 );
        ids.add( OSC_3 );
    }
    else if( id_ == FILTERS ) {
        ids.add( FILTERS );
        ids.add( FILTER_1 );
        ids.add( FILTER_2 );
        ids.add( FILTER_3 );
    }
    else if( id_ == ARP ) {
        ids.add( ARP );
        ids.add( ARP_TUNE );
        ids.add( ARP_VELOCITY );
        ids.add( ARP_GLIDE_SHUFFLE );
        ids.add( ARP_SWITCHS );
    }
    else
    {
        ids.add( id_ );
    }

    if( ! active_morph_selections.contains( id_ ) )
    {
        // DISABLE AND ENABLE
        for( int i = 0 ; i != ids.size() ; ++i )
        {
            MORPH_SELCTIONS_IDS id = ids.getUnchecked(i);
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
            {
                MorpherSelection* peer = peers_[morpher_id];
                if( peer != this )
                {
                    peer->active_morph_selections.removeFirstMatchingValue( id );
                }
                else if( ! active_morph_selections.contains( id ) )
                {
                    active_morph_selections.add( id );
                }
            }
        }
    }
    else
    {
        for( int i = 0 ; i != ids.size() ; ++i )
        {
            MORPH_SELCTIONS_IDS id = ids.getUnchecked(i);
            active_morph_selections.removeFirstMatchingValue( id );
        }
    }

    // CLEAN HEADERS
    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
    {
        MorpherSelection* peer = peers_[morpher_id];
        peer->clean_header_selections();
    }
}
void SynthData::MorpherSelection::clean_header_selections() {
    if( active_morph_selections.size() != MORPH_SELCTIONS_IDS::SUM )
    {
        active_morph_selections.removeFirstMatchingValue( ALL );
    }
    else if( ! active_morph_selections.contains( ALL ) )
    {
        active_morph_selections.add(ALL);
    }

    if( ! active_morph_selections.contains( OSC_1 )
            || ! active_morph_selections.contains( OSC_2 )
            || ! active_morph_selections.contains( OSC_2 ) )
    {
        active_morph_selections.removeFirstMatchingValue( OSCS );
    }
    else if( ! active_morph_selections.contains( OSCS ) )
    {
        active_morph_selections.add(OSCS);
    }

    if( ! active_morph_selections.contains( FILTER_1 )
            || ! active_morph_selections.contains( FILTER_2 )
            || ! active_morph_selections.contains( FILTER_3 ) )
    {
        active_morph_selections.removeFirstMatchingValue( FILTERS );
    }
    else if( ! active_morph_selections.contains( FILTERS ) )
    {
        active_morph_selections.add(FILTERS);
    }

    if( ! active_morph_selections.contains( ARP_TUNE )
            || ! active_morph_selections.contains( ARP_VELOCITY )
            || ! active_morph_selections.contains( ARP_GLIDE_SHUFFLE )
            || ! active_morph_selections.contains( ARP_SWITCHS ) ) {
        active_morph_selections.removeFirstMatchingValue( ARP );
    }
    else if( ! active_morph_selections.contains( ARP ) )
    {
        active_morph_selections.add(ARP);
    }
}
// MAYBE NOT THE BEST CODE BUT ON A GOOD POSITION (less used function)
void SynthData::activate_morph_selection( int morpher_id_, MORPH_SELCTIONS_IDS id_, bool run_sync_morph_ ) {
    // UPDATE SELECTIONS
    morpher_selections[morpher_id_]->activate( id_, morpher_selections );

    // COLLECT ALL IDS
    Array< int > unselected_selection_ids;
    for( int selection_id = 0 ; selection_id != MORPH_SELCTIONS_IDS::ALL ; ++selection_id )
        unselected_selection_ids.add( selection_id );

    // SET LEFT AND RIGHT SOURCES
    morph_groups_per_morpher.clearQuick();
    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
    {
        morph_groups_per_morpher.add( Array< MorphGroup* >() );

        for( int morph_group_id = 0 ; morph_group_id != ALL ; ++morph_group_id )
        {
            MorpherSelection* morpher_selection = morpher_selections[morpher_id];
            if( morpher_selection->active_morph_selections.contains( morph_group_id ) )
            {
                MorphGroup* morph_group = morph_groups[morph_group_id];

                morph_group->set_sources
                (
                    left_morph_datas.getUnchecked(morpher_id),
                    right_morph_datas.getUnchecked(morpher_id),
                    morhp_states[morpher_id],
                    morhp_switch_states[morpher_id]
                );

                morph_groups_per_morpher.getReference(morpher_id).add(morph_group);

                unselected_selection_ids.removeFirstMatchingValue( morph_group_id );
            }
        }
    }

    // REMOVE/DISABLE UNSELECTED LEFT AND RIGHT SOURCES
    for( int i = 0; i != unselected_selection_ids.size() ; ++i )
    {
        const int id( unselected_selection_ids[i] );
        MorphGroup* morph_group( morph_groups[id] );
        morph_group->set_sources(nullptr,nullptr,0,LEFT);
    }

    // SYNC
    if( run_sync_morph_ )
        run_sync_morph();
}
const Array< int >& SynthData::get_active_morph_selections( int morpher_id_ ) const {
    return morpher_selections[morpher_id_]->active_morph_selections;
}

inline const SynthData::MorphGroup& SynthData::get_morph_group( int id_ ) const noexcept {
    return *morph_groups.getUnchecked( id_ );
}
inline SynthData::MorphGroup& SynthData::get_morph_group( int id_ ) noexcept {
    return *morph_groups.getUnchecked( id_ );
}
inline void SynthData::run_sync_morph() noexcept {
    for( int i = 0 ;
    i != morph_groups.size() ;
    ++i )
    {
        morph_groups[i]->run_sync_morph();
    }
}
bool SynthData::try_to_load_programm_to_left_side( int morpher_id_, int bank_id_, int index_ ) noexcept {
    SynthData* synth_data = left_morph_datas.getUnchecked( morpher_id_ );
    synth_data->set_current_bank( bank_id_ );
    synth_data->set_current_program( index_ );
    bool success = synth_data->load( false );
    if( success )
        run_sync_morph();

    return success;
}
bool SynthData::try_to_load_programm_to_right_side( int morpher_id_, int bank_id_, int index_ ) noexcept {
    SynthData* synth_data = right_morph_datas.getUnchecked( morpher_id_ );
    synth_data->set_current_bank( bank_id_ );
    synth_data->set_current_program( index_ );
    bool success = synth_data->load( false );
    if( success )
        run_sync_morph();

    return success;
}
void SynthData::update_left_morph_source( int morpher_id_ ) noexcept {
    (*left_morph_datas.getUnchecked(morpher_id_)) = *this;
}
void SynthData::update_right_morph_source( int morpher_id_ ) noexcept {
    (*right_morph_datas.getUnchecked(morpher_id_)) = *this;
}
void SynthData::parameter_value_changed( Parameter* param_ ) noexcept
{
    if( param_ == morhp_states[0].ptr() ) {
        morph( 0, *param_ );
    }
    else if( param_ == morhp_states[1].ptr() ) {
        morph( 1, *param_ );
    }
    else if( param_ == morhp_states[2].ptr() ) {
        morph( 2, *param_ );
    }
    else if( param_ == morhp_states[3].ptr() ) {
        morph( 3, *param_ );
    }
    else if( param_ == linear_morhp_state.ptr() )
    {
        float value = *param_;
        if( value <= 1 )
        {
            ChangeParamOverTime::execute( morhp_states[0], 1.0f-value, morph_motor_time );
            ChangeParamOverTime::execute( morhp_states[1], value, morph_motor_time );

            // SMOTH TO ZERO
            float morph_state_2 = get_morph_state(2);
            if( morph_state_2 != 0 )
                ChangeParamOverTime::execute( morhp_states[2], 0, morph_motor_time );
            float morph_state_3 = get_morph_state(3);
            if( morph_state_3 != 0 )
                ChangeParamOverTime::execute( morhp_states[3], 0, morph_motor_time );
        }
        else if( value <= 2 )
        {
            value -= 1;

            ChangeParamOverTime::execute( morhp_states[1], 1.0f-value, morph_motor_time );
            ChangeParamOverTime::execute( morhp_states[2], value, morph_motor_time );

            // SMOTH TO ZERO
            float morph_state_0 = get_morph_state(0);
            if( morph_state_0 != 0 )
                ChangeParamOverTime::execute( morhp_states[0], 0, morph_motor_time );
            float morph_state_3 = get_morph_state(3);
            if( morph_state_3 != 0 )
                ChangeParamOverTime::execute( morhp_states[3], 0, morph_motor_time );
        }
        else
        {
            value -= 2;

            ChangeParamOverTime::execute( morhp_states[2], 1.0f-value, morph_motor_time );
            ChangeParamOverTime::execute( morhp_states[3], value, morph_motor_time );

            // SMOTH TO ZERO
            float morph_state_0 = get_morph_state(0);
            if( morph_state_0 != 0 )
                ChangeParamOverTime::execute( morhp_states[0], 0, morph_motor_time );
            float morph_state_1 = get_morph_state(1);
            if( morph_state_1 != 0 )
                ChangeParamOverTime::execute( morhp_states[1], 0, morph_motor_time );
        }
    }
}
void SynthData::morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ ) noexcept
{
    if( force_ )
    {
        morhp_states[morpher_id_].get_runtime_info().stop_time_change();
        morhp_states[morpher_id_] = morph_amount_left_to_right_;
    }

    Array< MorphGroup* >& morph_groups_for_morpher = morph_groups_per_morpher.getReference(morpher_id_);
    for( int morph_group_id = 0 ; morph_group_id != morph_groups_for_morpher.size() ; ++morph_group_id )
    {
        morph_groups_for_morpher[morph_group_id]->morph( morph_amount_left_to_right_ );
    }
}

float SynthData::get_morph_state( int morpher_id_ ) const noexcept {
    return morhp_states[morpher_id_];
}

void SynthData::morph_switch_buttons( int morpher_id_, bool do_switch_ ) noexcept
{
    if( do_switch_ )
        morhp_switch_states[morpher_id_] ^= true;

    Array< MorphGroup* >& morph_groups_for_morpher = morph_groups_per_morpher.getReference(morpher_id_);
    for( int morph_group_id = 0 ;
    morph_group_id != morph_groups_for_morpher.size() ;
    ++morph_group_id )
        morph_groups_for_morpher[morph_group_id]->morph_switchs( morhp_switch_states[morpher_id_] );
}
bool SynthData::get_morph_switch_state( int morpher_id_ ) const noexcept {
    return morhp_switch_states[morpher_id_];
}

NOINLINE void SynthData::save_to( XmlElement* xml_ ) const noexcept {
    if( xml_ )
    {
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
        {
            write_parameter_to_file( *xml_, saveable_parameters.getUnchecked(i) );
        }

        // MORPH SELECTIONS
        if( id == MASTER )
        {
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
            {
                MorpherSelection& morpher_selection = *morpher_selections[morpher_id];
                for( int morph_group_id = 0; morph_group_id != ALL ; ++morph_group_id )
                    xml_->setAttribute( String("morph_") + String( morpher_id ) + String( morph_group_id ), String(morpher_selection.active_morph_selections.contains( morph_group_id )) );
            }
        }
    }
}

NOINLINE void SynthData::read_from( const XmlElement* xml_ ) noexcept {
    if( xml_ )
    {
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
        {
            read_parameter_from_file( *xml_, saveable_parameters.getUnchecked(i) );
        }

        // FIRST LOAD THE MORPH SOURCES
        if( id == MASTER )
        {
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
            {
                left_morph_datas[morpher_id]->read_from(xml_->getChildByName(String("LeftMorphData_")+String(morpher_id)));
                right_morph_datas[morpher_id]->read_from(xml_->getChildByName(String("RightMorphData_")+String(morpher_id)));
            }
        }

        // MORPH SELECTIONS
        if( id == MASTER )
        {
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
            {
                MorpherSelection& morpher_selection = *morpher_selections[morpher_id];
                for( int morph_group_id = 0; morph_group_id != ALL ; ++morph_group_id )
                {
                    int is_active_stored = xml_->getIntAttribute( String("morph_") + String( morpher_id ) + String( morph_group_id ), 2 );
                    if( morpher_selection.active_morph_selections.contains(morph_group_id) && is_active_stored == 0 )
                        activate_morph_selection( morpher_id, static_cast< MORPH_SELCTIONS_IDS >( morph_group_id ), false );
                    else if( !morpher_selection.active_morph_selections.contains(morph_group_id) && is_active_stored == 1 )
                        activate_morph_selection( morpher_id, static_cast< MORPH_SELCTIONS_IDS >( morph_group_id ), false );
                }
            }
        }
        // AND MORPH IT
        if( id == MASTER )
        {
            for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
            {
                morph( morpher_id, morhp_states[morpher_id] );
                morph_switch_buttons( morpher_id, false );
            }
        }
    }

    //UiLookAndFeel::getInstance()->colours.read_from(xml_);
}

NOINLINE void SynthData::save_midi() const noexcept {
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    folder = File(folder.getFullPathName()+PROJECT_FOLDER);
    if( folder.createDirectory() )
    {
        File midi_file( File( folder.getFullPathName() + String("/") + "patch.midi") );

        XmlElement xml("MIDI-PATCH-1.0");
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
        {
            write_midi_to( xml, saveable_parameters.getUnchecked(i) );
        }

        xml.writeToFile(midi_file,"");
    }
}
NOINLINE void SynthData::read_midi() noexcept {
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    File midi_file = File(folder.getFullPathName()+PROJECT_FOLDER+String("patch.midi"));
    ScopedPointer<XmlElement> xml = XmlDocument( midi_file ).getDocumentElement();
    if( xml )
    {
        if( xml->hasTagName("MIDI-PATCH-1.0") )
        {
            for( int i = 0 ; i != saveable_parameters.size() ; ++i )
                read_midi_from( *xml, saveable_parameters.getUnchecked(i) );
        }
    }
}

//==============================================================================
NOINLINE SynthData::MorphGroup::MorphGroup()
    :
    id(-1),
    left_morph_group( nullptr ), right_morph_group( nullptr ),
    last_power_of_right(0),
    current_switch(LEFT),
    current_callbacks(-1)
{}

NOINLINE void SynthData::MorphGroup::set_id( int id_ ) {
    id = id_;
}

NOINLINE void SynthData::MorphGroup::register_parameter( Parameter* param_, bool is_master_ )
{
    params.add( param_ );

    if( is_master_ )
        param_->register_listener(this);
}
NOINLINE void SynthData::MorphGroup::register_switch_parameter( BoolParameter* param_, bool is_master_ )
{
    /*
    switch_bool_params.add( param_ );

    if( is_master_ )
      param_->register_listener(this);
    */
}
NOINLINE void SynthData::MorphGroup::register_switch_parameter( IntParameter* param_, bool is_master_ )
{
    /*
    switch_int_params.add( param_ );

    if( is_master_ )
        param_->register_listener(this);
    */
}

NOINLINE void SynthData::MorphGroup::set_sources( SynthData* left_source_,
        SynthData* right_source_,
        float current_morph_amount_,
        bool current_switch_state_ ) noexcept
{
    last_power_of_right = current_morph_amount_;
    current_switch = current_switch_state_;

    if( left_source_ && right_source_ )
    {
        left_morph_group = &left_source_->get_morph_group( id );
        right_morph_group = &right_source_->get_morph_group( id );
    }
    else
    {
        left_morph_group = nullptr;
        right_morph_group = nullptr;
    }
}

// TODO update the morphgroups earlyer to morph faster?
#define SYNC_MORPH_STEPS 100
#define SYNC_MORPH_TIME 10
void SynthData::MorphGroup::morph( float power_of_right_ ) noexcept
{
    if( left_morph_group )
    {
        for( int i = 0 ; i != params.size() ; ++i )
        {
            // VALUE
            float new_value = (*left_morph_group->params[i] * (1.0f - power_of_right_ )) + (*right_morph_group->params[i] * power_of_right_);
            params[i]->set_value_without_notification( new_value );

            // MODULATION VALUE
            if( has_modulation( params[i] ) )
            {
                params[i]->set_modulation_amount_without_notification
                (
                    left_morph_group->params[i]->get_modulation_amount() * (1.0f - power_of_right_ )
                    + right_morph_group->params[i]->get_modulation_amount() * power_of_right_
                );
            }
        }
    }
    last_power_of_right = power_of_right_;
}
void SynthData::MorphGroup::run_sync_morph() noexcept {
    stopTimer();
    current_callbacks = 0;

    sync_param_deltas.clearQuick();
    sync_modulation_deltas.clearQuick();
    if( left_morph_group )
    {
        for( int i = 0 ; i != params.size() ; ++i )
        {
            {
                float target_value = (*left_morph_group->params[i] * (1.0f - last_power_of_right )) + (*right_morph_group->params[i] * last_power_of_right);
                float current_value = *params[i];
                sync_param_deltas.add( (target_value-current_value)/SYNC_MORPH_STEPS );
            }
            if( has_modulation( params[i] ) )
            {
                float target_modulation = (left_morph_group->params[i]->get_modulation_amount() * (1.0f - last_power_of_right )) + (right_morph_group->params[i]->get_modulation_amount() * last_power_of_right);
                float current_modulation = params[i]->get_modulation_amount();
                sync_modulation_deltas.add( (target_modulation-current_modulation)/SYNC_MORPH_STEPS );
            }
            else
                sync_modulation_deltas.add( -1 );
        }

        startTimer(SYNC_MORPH_TIME);
    }
}

void SynthData::MorphGroup::timerCallback() {
    for( int i = 0 ; i != params.size() ; ++i )
    {
        {
            const ParameterInfo& info = params[i]->get_info();
            float min = info.min_value;
            float max = info.max_value;
            float new_value = *params[i] + sync_param_deltas[i];
            if( new_value > max )
                new_value = max;
            else if( new_value < min )
                new_value = min;

            params[i]->set_value_without_notification( new_value );
        }
        float modulation_delta = sync_modulation_deltas[i];
        if( modulation_delta != -1 )
        {
            float new_modualtation = params[i]->get_modulation_amount() + modulation_delta;
            if( new_modualtation > 1 )
                new_modualtation = 1;
            else if( new_modualtation < -1 )
                new_modualtation = -1;

            params[i]->set_modulation_amount_without_notification( new_modualtation );
        }
    }

    if( current_callbacks++ == SYNC_MORPH_STEPS )
    {
        stopTimer();
        morph(last_power_of_right);
    }
}

void SynthData::MorphGroup::morph_switchs( bool left_right_ ) noexcept
{
    current_switch = left_right_;
    if( left_morph_group )
    {
        for( int i = 0 ; i != switch_bool_params.size() ; ++i )
        {
            if( current_switch == RIGHT )
                switch_bool_params[i]->set_value_without_notification( right_morph_group->switch_bool_params[i]->get_value() );
            else
                switch_bool_params[i]->set_value_without_notification( left_morph_group->switch_bool_params[i]->get_value() );
        }
        for( int i = 0 ; i != switch_int_params.size() ; ++i )
        {
            if( current_switch == RIGHT )
                switch_int_params[i]->set_value_without_notification( right_morph_group->switch_int_params[i]->get_value() );
            else
                switch_int_params[i]->set_value_without_notification( left_morph_group->switch_int_params[i]->get_value() );
        }
    }
}

void SynthData::MorphGroup::parameter_value_changed( Parameter* param_ ) noexcept
{
    TYPES_DEF type = type_of( param_ );
    if( type == IS_BOOL )
    {
        if( left_morph_group )
        {
            const int param_id = switch_bool_params.indexOf( reinterpret_cast< BoolParameter* >( param_ ) );
            if( param_id != -1 )
            {
                if( current_switch == LEFT )
                    left_morph_group->switch_bool_params[param_id]->set_value_without_notification( param_->get_value() );
                else
                    right_morph_group->switch_bool_params[param_id]->set_value_without_notification( param_->get_value() );
            }
        }
    }
    else if( type == IS_INT )
    {
        if( left_morph_group )
        {
            const int param_id = switch_int_params.indexOf( reinterpret_cast< IntParameter* >( param_ ) );
            if( param_id != -1 )
            {
                if( current_switch == LEFT )
                    left_morph_group->switch_int_params[param_id]->set_value_without_notification( param_->get_value() );
                else
                    right_morph_group->switch_int_params[param_id]->set_value_without_notification( param_->get_value() );
            }
        }
    }
    else
    {
        if( left_morph_group )
        {
            const int param_id = params.indexOf( param_ );
            if( param_id != -1 )
            {
                Parameter& left_source_param = *left_morph_group->params[param_id];
                Parameter& right_source_param = *right_morph_group->params[param_id];

                // x = l*(1-m)+r*m
                // r = (l*(m-1.0f)+x)/m
                // l = (m*r-x) / (m-1)
                float current_value = *param_;
                float right_value = right_source_param;
                bool update_left_or_right = last_power_of_right > 0.5f ? RIGHT : LEFT;
                const ParameterInfo& info = right_source_param.get_info();
                const float max = info.max_value;
                const float min = info.min_value;
                if( update_left_or_right == RIGHT )
                {
                    float left_value = left_source_param;
                    float new_right_value = (left_value*(last_power_of_right-1)+current_value) / last_power_of_right;
                    if( new_right_value > max )
                    {
                        new_right_value = max;
                        update_left_or_right = LEFT;
                    }
                    else if( new_right_value < min )
                    {
                        new_right_value = min;
                        update_left_or_right = LEFT;
                    }

                    right_source_param.set_value_without_notification( new_right_value );
                    right_value = new_right_value;
                }
                if( update_left_or_right == LEFT )
                {
                    float new_left_value = (last_power_of_right*right_value-current_value) / (last_power_of_right-1);
                    if( new_left_value > max )
                    {
                        new_left_value = max;
                    }
                    else if( new_left_value < min )
                    {
                        new_left_value = min;
                    }

                    left_source_param.set_value_without_notification( new_left_value );
                }
            }
        }
    }
}
void SynthData::MorphGroup::parameter_modulation_value_changed( Parameter* param_ ) noexcept {
    if( left_morph_group )
    {
        const int param_id = params.indexOf( param_ );
        if( param_id != -1 )
        {
            Parameter& left_source_param = *left_morph_group->params[param_id];
            Parameter& right_source_param = *right_morph_group->params[param_id];

            float current_modulation = param_->get_modulation_amount();
            float right_modulation = right_source_param.get_modulation_amount();
            bool update_left_or_right = last_power_of_right > 0.5f ? RIGHT : LEFT;
            if( update_left_or_right == RIGHT )
            {
                float left_modulation = left_source_param.get_modulation_amount();
                float new_right_modulation = (left_modulation*(last_power_of_right-1)+current_modulation) / last_power_of_right;

                if( new_right_modulation > 1 )
                {
                    new_right_modulation = 1;
                    update_left_or_right = LEFT;
                }
                else if( new_right_modulation < -1 )
                {
                    new_right_modulation = -1;
                    update_left_or_right = LEFT;
                }

                right_source_param.set_modulation_amount_without_notification( new_right_modulation );
                right_modulation = new_right_modulation;
            }
            if( update_left_or_right == LEFT )
            {
                float new_left_modulation = (last_power_of_right*right_modulation-current_modulation) / (last_power_of_right-1);

                if( new_left_modulation > 1 )
                {
                    new_left_modulation = 1;
                }
                else if( new_left_modulation < -1 )
                {
                    new_left_modulation = -1;
                }

                left_source_param.set_modulation_amount_without_notification( new_left_modulation );
            }
        }
    }
}

static inline File get_bank_folder( const String& bank_name_ ) {
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    folder = File(folder.getFullPathName()+PROJECT_FOLDER+bank_name_);
    folder.createDirectory();

    return folder;
}
static inline File get_program_file( const String& bank_name_, const String& program_name_ ) {
    return File( get_bank_folder( bank_name_ ).getFullPathName()
                 + String("/")
                 + program_name_
                 + ".mlprog");
}

NOINLINE void SynthData::refresh_banks_and_programms()
{
    SynthData& synth_data( GET_DATA( synth_data ) );
    synth_data.banks.clearQuick();
    update_banks( synth_data.banks );

    synth_data.program_names.clearQuick();
    synth_data.program_names.add( StringArray() );
    synth_data.program_names.add( StringArray() );
    synth_data.program_names.add( StringArray() );
    synth_data.program_names.add( StringArray() );

    update_bank_programms( 0, synth_data.program_names.getReference(0) );
    update_bank_programms( 1, synth_data.program_names.getReference(1) );
    update_bank_programms( 2, synth_data.program_names.getReference(2) );
    update_bank_programms( 3, synth_data.program_names.getReference(3) );

    synth_data.calc_current_program_abs();
}
NOINLINE void SynthData::update_banks( StringArray& banks_ ) {
    banks_.add("A");
    banks_.add("B");
    banks_.add("C");
    banks_.add("D");
}

static inline void sort_by_date( Array< File >& file_array_ ) {
    for( int file_to_sort_id = 0; file_to_sort_id != file_array_.size() ; ++file_to_sort_id )
    {
        Time to_move_time = file_array_.getReference(file_to_sort_id).getLastModificationTime();
        for( int compare_id = 0 ; compare_id != file_array_.size() ; ++compare_id )
        {
            if( file_to_sort_id != compare_id )
            {
                Time to_compare_time = file_array_.getReference(compare_id).getLastModificationTime();
                if( to_compare_time < to_move_time )
                {
                    file_array_.swap( file_to_sort_id, compare_id );

                    // RESET FIRST LOOP
                    file_to_sort_id--; // -1 for ++LOOP
                    break;
                }
            }
        }
    }
}
static inline void sort_by_name( Array< File >& file_array_ ) {
    for( int file_to_sort_id = 0; file_to_sort_id != file_array_.size() ; ++file_to_sort_id )
    {
        String to_move_name = file_array_.getReference(file_to_sort_id).getFileName();
        for( int compare_id = 0 ; compare_id != file_array_.size() ; ++compare_id )
        {
            if( file_to_sort_id != compare_id )
            {
                if( file_array_.getReference(compare_id).getFileName().compareNatural(to_move_name) > 0 )
                {
                    file_array_.swap( file_to_sort_id, compare_id );

                    // RESET FIRST LOOP
                    file_to_sort_id--; // -1 for ++LOOP
                    break;
                }
            }
        }
    }
}
NOINLINE void SynthData::update_bank_programms( int bank_id_, StringArray& program_names_ )
{
    File bank_folder = get_bank_folder( GET_DATA( synth_data ).banks[bank_id_] );
    Array< File > program_files;
    bank_folder.findChildFiles( program_files, File::findFiles, false, "*.mlprog" );
    //sort_by_date(program_files);
    sort_by_name(program_files);

    for( int i = 0 ; i != program_files.size() ; ++i )
        program_names_.add( program_files.getReference(i).getFileNameWithoutExtension() );
}

NOINLINE const StringArray& SynthData::get_banks() {
    return GET_DATA( synth_data ).banks;
}
NOINLINE const StringArray& SynthData::get_programms( int bank_id_ ) {
    return GET_DATA( synth_data ).program_names.getReference(bank_id_);
}

int SynthData::get_current_programm_id_abs() const
{
    return current_program_abs;
}
const String& SynthData::get_current_program_name_abs() const noexcept
{
    if( current_program == -1 )
    {
        static String error("ERROR");
        return error;
    }
    return GET_DATA( synth_data ).program_names.getReference(current_bank)[current_program];
}
const String& SynthData::get_program_name_abs(int id_) const noexcept
{
    SynthData& synth_data( GET_DATA( synth_data ) );
    for( int bank_id = 0 ; bank_id != synth_data.banks.size() ; ++bank_id )
    {
        int bank_size = synth_data.program_names.getReference(bank_id).size();
        if( id_ < bank_size )
            return synth_data.program_names.getReference(bank_id)[id_];
        else
            id_ -= bank_size;
    }

    static String error("ERROR");
    return error;
}

void SynthData::calc_current_program_abs() noexcept {
    if( current_program == -1 ) {
        current_program_abs = -1;
        return;
    }

    current_program_abs = 0;
    for( int bank_id = 0 ;
    bank_id != current_bank ;
    ++bank_id )
    {
        int bank_size = GET_DATA( synth_data ).program_names.getReference(bank_id).size();
        if( current_program_abs+current_program < bank_size )
        {
            current_program_abs += current_program;
            break;
        }
        else
            current_program_abs += bank_size;
    }
}
NOINLINE void SynthData::set_current_bank( int bank_index_ ) {
    current_bank = bank_index_;
    current_program = -1; // TODO can be an empty bank
    calc_current_program_abs();
}
NOINLINE void SynthData::set_current_program( int programm_index_ ) {
    current_program = programm_index_;
    calc_current_program_abs();
}
NOINLINE void SynthData::set_current_program_abs( int programm_index_ )
{
    int sum_programms = 0;

    SynthData& synth_data( GET_DATA( synth_data ) );
    for( int bank_id = 0 ; bank_id != synth_data.banks.size() ; ++bank_id ) {
        int bank_size = synth_data.program_names.getReference(bank_id).size();
        if( programm_index_ < bank_size+sum_programms )
        {
            current_bank = bank_id;
            current_program = programm_index_-sum_programms;
            current_program_abs = programm_index_;
            break;
        }
        sum_programms+=bank_size;
    }
}
NOINLINE int SynthData::get_current_bank() const {
    return current_bank;
}
NOINLINE int SynthData::get_current_program() const {
    return current_program;
}
NOINLINE const StringArray& SynthData::get_current_bank_programms()
{
    return GET_DATA( synth_data ).program_names.getReference(current_bank);
}
static inline String& generate_programm_name( const String& bank_,  String& name_ ) {
    bool exist = false;
    int counter = 1;
    String counter_name("");
    do {
        File program = get_program_file( bank_, name_ + counter_name );
        if( program.exists() ) {
            counter_name = String(" - ")+ String(counter);
            counter++;
            exist = true;
        }
        else
        {
            name_ = name_+counter_name;
            exist = false;
        }
    } while( exist );

    return name_;
};
NOINLINE bool SynthData::rename( const String& new_name_ )
{
    if( current_program == -1 )
        return false;

    SynthData& synth_data( GET_DATA( synth_data ) );
    File program = get_program_file( synth_data.banks[current_bank], synth_data.program_names.getReference(current_bank)[current_program] );

    bool success = false;

    String name = new_name_;
    generate_programm_name( synth_data.banks[current_bank], name );
    if( program.existsAsFile() ) {
        success = program.moveFileTo( get_bank_folder(synth_data.banks[current_bank]).getFullPathName()
                                      + String("/")
                                      + name
                                      + ".mlprog" );
    }

    if( success )
    {
        refresh_banks_and_programms();
        current_program = synth_data.program_names[current_bank].indexOf(name);
    }

    return success;
}
NOINLINE bool SynthData::load( bool load_morph_groups ) {
    if( current_program == -1 )
        return false;

    SynthData& synth_data( GET_DATA( synth_data ) );
    return load( synth_data.banks[current_bank], synth_data.program_names.getReference(current_bank)[current_program], load_morph_groups );
}
NOINLINE bool SynthData::load( const String& bank_name_, const String& program_name_, bool load_morph_groups ) {
    bool success = false;

    File program = get_program_file( bank_name_, program_name_ );

    ScopedPointer<XmlElement> xml = XmlDocument( program ).getDocumentElement();
    if( xml )
    {
        if( xml->hasTagName("PROJECT-1.0") || xml->hasTagName("MONOLisa") )
        {
            // LOAD THE OWN DATA
            read_from(xml);


            success = true;
        }
        else
            success = false;
    }

    return success;
}

NOINLINE bool SynthData::load_prev() {
    bool success = false;

    if( current_program-1 >= 0 )
    {
        current_program--;
        success = load();
    }
    else
    {
        int last_index = GET_DATA( synth_data ).program_names.getReference(current_bank).size()-1;
        if( last_index > 0 ) {
            current_program = last_index;
            success = load();
        }
    }

    return success;
}
NOINLINE bool SynthData::load_next() {
    bool success = false;

    SynthData& synth_data( GET_DATA( synth_data ) );
    if( current_program+1 < synth_data.program_names.getReference(current_bank).size() )
    {
        current_program++;
        success = load();
    }
    else
    {
        if( synth_data.program_names.getReference(current_bank).size() )
        {
            current_program = 0;
            success = load();
        }
    }

    return success;
}

NOINLINE bool SynthData::replace() {
    if( current_program == -1 )
        return false;

    SynthData& synth_data( GET_DATA( synth_data ) );
    File program = get_program_file( synth_data.banks[current_bank], synth_data.program_names.getReference(current_bank)[current_program] );
    bool success = AlertWindow::showNativeDialogBox
                   (
                       "REPLACE PROJECT?",
                       String("Overwrite project: ")+synth_data.banks[current_bank]+String(":")+synth_data.program_names.getReference(current_bank)[current_program],
                       true
                   );
    if( success )
        success = write2file( synth_data.banks[current_bank], synth_data.program_names.getReference(current_bank)[current_program] );

    return success;
}
NOINLINE bool SynthData::create_new()
{
    SynthData& synth_data( GET_DATA( synth_data ) );
    String new_program_name = String("New Program");
    generate_programm_name( synth_data.banks[current_bank], new_program_name );

    bool success = write2file( synth_data.banks[current_bank], new_program_name );

    if( success )
    {
        refresh_banks_and_programms();
        current_program = synth_data.program_names[current_bank].indexOf(new_program_name);
    }

    return success;
}

NOINLINE bool SynthData::remove() {
    if( current_program == -1 )
        return false;

    SynthData& synth_data( GET_DATA( synth_data ) );
    File program = get_program_file( synth_data.banks[current_bank], synth_data.program_names.getReference(current_bank)[current_program] );
    bool success = AlertWindow::showNativeDialogBox
                   (
                       "DELETE PROJECT?",
                       String("Delete project: ")+synth_data.banks[current_bank]+String(":")+synth_data.program_names.getReference(current_bank)[current_program],
                       true
                   );
    if( success )
        program.deleteFile();

    refresh_banks_and_programms();

    return success;
}

NOINLINE void SynthData::save_session() {
    write2file( "SESSION", "last" );
}
NOINLINE void SynthData::load_session() {
    load( "SESSION", "last" );
}

NOINLINE bool SynthData::write2file( const String& bank_name_, const String& program_name_ ) {
    File program_file = get_program_file( bank_name_, program_name_ );

    XmlElement xml("PROJECT-1.0");
    save_to( &xml );

    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id )
    {
        left_morph_datas[morpher_id]->save_to(xml.createNewChildElement(String("LeftMorphData_")+String(morpher_id)));
        right_morph_datas[morpher_id]->save_to(xml.createNewChildElement(String("RightMorphData_")+String(morpher_id)));
    }

    return xml.writeToFile(program_file,"");
}

//==============================================================================
NOINLINE mono_ParameterOwnerStore::mono_ParameterOwnerStore() : ui_env(nullptr), ui_env_preset_data(nullptr)
{
}

//==============================================================================
//==============================================================================
//==============================================================================











