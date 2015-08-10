/*
  ==============================================================================

    SynthData.cpp
    Created: 24 Apr 2015 11:40:30am
    Author:  monotomy

  ==============================================================================
*/

#include "SynthData.h"
#include "Synth.h"

//==============================================================================
NOINLINE RuntimeInfo::RuntimeInfo()
    :
    samples_since_start(0),
    bpm(120),

    is_extern_synced(false),
    clock_counter(0)
{}
NOINLINE RuntimeInfo::~RuntimeInfo() {}

//==============================================================================
//==============================================================================
//==============================================================================
juce_ImplementSingleton (RuntimeNotifyer)

NOINLINE RuntimeNotifyer::RuntimeNotifyer() :
    sample_rate(44100),
    sample_rate_1ths( 1.0/44100),
    block_size(512)
{
}
NOINLINE RuntimeNotifyer::~RuntimeNotifyer() {
    clearSingletonInstance();
}

//==============================================================================
NOINLINE void RuntimeNotifyer::set_sample_rate( double sr_ ) {
    double old_sr = sample_rate;
    sample_rate = sr_;
    sample_rate_1ths = 1.0/sample_rate;
    for( int i = 0 ; i != listeners.size() ; ++i )
    {
        listeners[i]->set_sample_rate(sr_);
        listeners[i]->sample_rate_changed(old_sr);
    }
};
NOINLINE void RuntimeNotifyer::set_block_size( int bs_ ) {
    block_size = bs_;
    for( int i = 0 ; i != listeners.size() ; ++i ) {
        listeners[i]->set_block_size(bs_);
        listeners[i]->block_size_changed();
    }
};

double RuntimeNotifyer::get_sample_rate() const noexcept {
    return sample_rate;
}
int RuntimeNotifyer::get_block_size() const noexcept {
    return block_size;
}
//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE RuntimeListener::RuntimeListener() :
    sample_rate(RuntimeNotifyer::getInstance()->sample_rate),
    sample_rate_1ths(RuntimeNotifyer::getInstance()->sample_rate_1ths),
    block_size(RuntimeNotifyer::getInstance()->block_size)
{
    RuntimeNotifyer::getInstance()->listeners.add( this );
}
NOINLINE RuntimeListener::~RuntimeListener() {
    RuntimeNotifyer::getInstance()->listeners.removeFirstMatchingValue( this );
}

//==============================================================================
//==============================================================================
//==============================================================================
#define LFO_NAME "LFO"
NOINLINE LFOData::LFOData( int id_ )
    :
    id( id_ ),

    speed(generate_param_name(LFO_NAME,id_,"speed"),generate_short_human_name(LFO_NAME,id_,"speed"))
{
}
NOINLINE LFOData::~LFOData() {}

NOINLINE void LFOData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &speed );
}

//==============================================================================
#define OSC_NAME "OSC"
NOINLINE OSCData::OSCData( int id_ )
    :
    id(id_),

    wave(generate_param_name(OSC_NAME,id,"wave"),generate_short_human_name(OSC_NAME,id_,"sync")),
    octave(generate_param_name(OSC_NAME,id,"octave"),generate_short_human_name(OSC_NAME,id_,"octave")),
    is_lfo_modulated(generate_param_name(OSC_NAME,id,"is_lfo_mod"),generate_short_human_name(OSC_NAME,id_,"mod_lfo")),

    fm_multi(generate_param_name(OSC_NAME,id,"fm_multi"),generate_short_human_name(OSC_NAME,id_,"fm_freq")),
    fm_amount(generate_param_name(OSC_NAME,id,"fm_power"),generate_short_human_name(OSC_NAME,id_,"fm_power")),
    fm_wave(generate_param_name(OSC_NAME,id,"fm_wave"),generate_short_human_name(OSC_NAME,id_,"fm_shot")),
    sync(generate_param_name(OSC_NAME,id,"sync"),generate_short_human_name(OSC_NAME,id_,"fm_sync")),
    mod_off(generate_param_name(OSC_NAME,id,"mod_off"),generate_short_human_name(OSC_NAME,id_,"mod_off")),

    puls_width(generate_param_name(OSC_NAME,id,"puls_width"),generate_short_human_name(OSC_NAME,id_,"puls_width")),
    fm_swing(generate_param_name(OSC_NAME,id,"fm_swing"),generate_short_human_name(OSC_NAME,id_,"fm_swing")),
    osc_switch(generate_param_name(OSC_NAME,id,"osc_switch"),generate_short_human_name(OSC_NAME,id_,"switch")),

    last_modulation_value( 0 )
{}
NOINLINE OSCData::~OSCData() {}

NOINLINE void OSCData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &wave );
    params_.add( &octave );
    params_.add( &is_lfo_modulated );
    params_.add( &fm_multi );
    params_.add( &fm_amount );
    params_.add( &fm_wave );
    params_.add( &sync );
    params_.add( &puls_width );
    params_.add( &fm_swing );
    params_.add( &osc_switch );
}

//==============================================================================
#define ENV_NAME "ENV"
NOINLINE ENVData::ENVData( int id_ )
    :
    id( id_ ),

    attack(generate_param_name(ENV_NAME,id,"attack"),generate_short_human_name(ENV_NAME,id_,"attack")),
    max_attack_time(generate_param_name(ENV_NAME,id,"max_attack_t"),generate_short_human_name(ENV_NAME,id_,"max_attack_t")),
    decay(generate_param_name(ENV_NAME,id,"decay"),generate_short_human_name(ENV_NAME,id_,"decay")),
    max_decay_time(generate_param_name(ENV_NAME,id,"max_decay_t"),generate_short_human_name(ENV_NAME,id_,"max_decay_t")),
    sustain(generate_param_name(ENV_NAME,id,"sustain"),generate_short_human_name(ENV_NAME,id_,"sustain")),
    sustain_time(generate_param_name(ENV_NAME,id,"sustain_time"),generate_short_human_name(ENV_NAME,id_,"sus_time")),
    release(generate_param_name(ENV_NAME,id,"release"),generate_short_human_name(ENV_NAME,id_,"release")),
    max_release_time(generate_param_name(ENV_NAME,id,"max_release_t"),generate_short_human_name(ENV_NAME,id_,"max_release_t"))
{
}
NOINLINE ENVData::~ENVData() {}

NOINLINE void ENVData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &attack );
    params_.add( &max_attack_time );
    params_.add( &decay );
    params_.add( &max_decay_time );
    params_.add( &sustain );
    params_.add( &sustain_time );
    params_.add( &release );
    params_.add( &max_release_time );
}

//==============================================================================
#define ENV_PRESET_DEF_NAME "ENVPD"
NOINLINE ENVPresetDef::ENVPresetDef ( int id_ )
    :
    attack_1(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_1_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_1")),
    decay_1(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_1_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_1")),
    sustain_time_1(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_1_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_1")),
    release_1(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_1_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"release_1")),
    attack_2(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_2_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_2")),
    decay_2(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_2_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_2")),
    sustain_time_2(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_2_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_2")),
    release_2(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_2_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"release_2")),
    attack_3(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_3_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_3")),
    decay_3(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_3_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_3")),
    sustain_time_3(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_3_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_3")),
    release_3(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_3_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"release_3")),
    attack_4(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"attack_4_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"attack_4")),
    decay_4(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"decay_4_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"decay_4")),
    sustain_time_4(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"sustain_time_4_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"sus_time_4")),
    release_4(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"release_4_"),generate_short_human_name(ENV_PRESET_DEF_NAME,"release_4")),
    max_attack_time(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_attack_t"),generate_short_human_name(ENV_PRESET_DEF_NAME,"max_attack_t")),
    max_decay_time(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_decay_t"),generate_short_human_name(ENV_PRESET_DEF_NAME,"max_decay_t")),
    max_release_time(generate_param_name(ENV_PRESET_DEF_NAME,MASTER,"max_release_t"),generate_short_human_name(ENV_PRESET_DEF_NAME,"max_release_t"))
{
}
NOINLINE ENVPresetDef::~ENVPresetDef() {}


NOINLINE void ENVPresetDef::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &attack_1 );
    params_.add( &decay_1 );
    params_.add( &sustain_time_1 );
    params_.add( &release_1 );
    params_.add( &attack_2 );
    params_.add( &decay_2 );
    params_.add( &sustain_time_2 );
    params_.add( &release_2 );
    params_.add( &attack_3 );
    params_.add( &decay_3 );
    params_.add( &sustain_time_3 );
    params_.add( &release_3 );
    params_.add( &attack_4 );
    params_.add( &decay_4 );
    params_.add( &sustain_time_4 );
    params_.add( &release_4 );
    params_.add( &max_attack_time );
    params_.add( &max_decay_time );
    params_.add( &max_release_time );
}
#define ENV_PRESET_NAME "ENVP"
NOINLINE ENVPresetData::ENVPresetData( int id_,
                                       ENVPresetDef* def_ )
    :
    ENVData( id_ ),
    def( def_ ),

    state(generate_param_name(ENV_PRESET_NAME,id,"state"),generate_short_human_name(ENV_PRESET_NAME,id_,"state"))
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

    sustain_time = def->sustain_time_1.get_reference();
    attack = def->attack_1.get_reference();
    decay = def->decay_1.get_reference();
    release = def->release_1.get_reference();
    max_attack_time = def->max_attack_time.get_reference();
    max_decay_time = def->max_decay_time.get_reference();
    max_release_time = def->max_release_time.get_reference();

    update_adr_values(state);
}
NOINLINE ENVPresetData::~ENVPresetData() {

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

NOINLINE void ENVPresetData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &state );
}

float ENVPresetData::get_attack_at( const ENVPresetDef& def_, float state_ ) noexcept {
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
float ENVPresetData::get_decay_at( const ENVPresetDef& def_, float state_ ) noexcept {
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
float ENVPresetData::get_sustain_time_at( const ENVPresetDef& def_, float state_ ) noexcept {
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
float ENVPresetData::get_release_at( const ENVPresetDef& def_, float state_ ) noexcept {
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
void ENVPresetData::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    if( param_ == state.get_base() )
    {
        update_adr_values(*param_);
    }
    else if( param_ == def->max_attack_time.get_base() )
    {
        max_attack_time.set_scaled_without_notification( def->max_attack_time );
    }
    else if( param_ == def->max_decay_time.get_base() )
    {
        max_decay_time.set_scaled_without_notification( def->max_decay_time );
    }
    else if( param_ == def->max_release_time.get_base() )
    {
        max_release_time.set_scaled_without_notification( def->max_release_time );
    }
    else
    {
        update_adr_values(state);
    }
}
void ENVPresetData::parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept {
    update_adr_values(*param_);
}
void ENVPresetData::update_adr_values( float value_ ) noexcept {
    if( value_ < 1 ) {
        float factor = 1.0f/1.0f * value_;

        attack.set_scaled_without_notification( def->attack_1*( 1.0f-factor ) + def->attack_2 * factor );
        decay.set_scaled_without_notification( def->decay_1*( 1.0f-factor ) + def->decay_2 * factor );
        sustain_time.set_scaled_without_notification( def->sustain_time_1*( 1.0f-factor ) + def->sustain_time_2 * factor );
        release.set_scaled_without_notification( def->release_1*( 1.0f-factor ) + def->release_2 * factor );
    }
    else if( value_ < 2 ) {
        float factor = 1.0f/1.0f * (value_-1.0f);

        attack.set_scaled_without_notification( def->attack_2*( 1.0f-factor ) + def->attack_3 * factor );
        decay.set_scaled_without_notification( def->decay_2*( 1.0f-factor ) + def->decay_3 * factor );
        sustain_time.set_scaled_without_notification( def->sustain_time_2*( 1.0f-factor ) + def->sustain_time_3 * factor );
        release.set_scaled_without_notification( def->release_2*( 1.0f-factor ) + def->release_3 * factor );
    }
    else if( value_ <= 3 ) {
        float factor = 1.0f/1.0f * (value_-2.0f);

        attack.set_scaled_without_notification( def->attack_3*( 1.0f-factor ) + def->attack_4 * factor );
        decay.set_scaled_without_notification( def->decay_3*( 1.0f-factor ) + def->decay_4 * factor );
        sustain_time.set_scaled_without_notification( def->sustain_time_4*( 1.0f-factor ) + def->sustain_time_4 * factor );
        release.set_scaled_without_notification( def->release_3*( 1.0f-factor ) + def->release_4 * factor );
    }
}
void ENVPresetData::parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}

//==============================================================================
#define FILTER_NAME "FLT"
NOINLINE FilterData::FilterData( int id_,  Array<ENVData*>& input_env_datas_  )
    :
    id( id_ ),


    filter_type(generate_param_name(FILTER_NAME,id,"filter_type"),generate_short_human_name(FILTER_NAME,id_,"type")),

    adsr_lfo_mix(generate_param_name(FILTER_NAME,id,"adsr_lfo_mix"),generate_short_human_name(FILTER_NAME,id_,"lfo_mix")),

    distortion(generate_param_name(FILTER_NAME,id,"distortion"),generate_short_human_name(FILTER_NAME,id_,"destroy")),
    modulate_distortion(generate_param_name(FILTER_NAME,id,"modulate_distortion"),generate_short_human_name(FILTER_NAME,id_,"mod_destroy")),

    cutoff(generate_param_name(FILTER_NAME,id,"cutoff"),generate_short_human_name(FILTER_NAME,id_,"cutoff")),
    modulate_cutoff(generate_param_name(FILTER_NAME,id,"modulate_cutoff"),generate_short_human_name(FILTER_NAME,id_,"mod_cutoff")),

    resonance(generate_param_name(FILTER_NAME,id,"resonance"),generate_short_human_name(FILTER_NAME,id_,"resonance")),
    modulate_resonance(generate_param_name(FILTER_NAME,id,"modulate_resonance"),generate_short_human_name(FILTER_NAME,id_,"mod_resonance")),

    width(generate_param_name(FILTER_NAME,id,"width"),generate_short_human_name(FILTER_NAME,id_,"width")),
    modulate_width(generate_param_name(FILTER_NAME,id,"modulate_width"),generate_short_human_name(FILTER_NAME,id_,"mod_width")),

    gain(generate_param_name(FILTER_NAME,id,"gain"),generate_short_human_name(FILTER_NAME,id_,"gain")),
    modulate_gain(generate_param_name(FILTER_NAME,id,"modulate_gain"),generate_short_human_name(FILTER_NAME,id_,"mod_gain")),

    input_env_datas( input_env_datas_ ),
    input_sustains( FILTER_NAME,id,"input_sustain","in_sustain" ),
    input_holds( FILTER_NAME,id,"input_hold","in_fix_sus" ),

    compressor( generate_param_name(FILTER_NAME,id,"compressor"),generate_short_human_name(FILTER_NAME,id_,"peak") ),
    output( generate_param_name(FILTER_NAME,id,"output"),generate_short_human_name(FILTER_NAME,id_,"volume") ),
    output_clipping( generate_param_name(FILTER_NAME,id,"output_clipping"),generate_short_human_name(FILTER_NAME,id_,"clipping") ),
    modulate_output( generate_param_name(FILTER_NAME,id,"modulate_output"),generate_short_human_name(FILTER_NAME,id_,"mod_volume") )
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        input_sustains[i].register_always_listener(this);
}
NOINLINE FilterData::~FilterData() {
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        input_sustains[i].remove_listener(this);
}

void FilterData::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i ) {
        if( input_sustains[i].get_base() == param_ )
        {
            // TODO copy sustain!
            input_env_datas[i]->sustain.set_scaled_without_notification( positive( param_->get_reference() ) );
            break;
        }
    }
}
void FilterData::parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}
void FilterData::parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    parameter_value_changed( param_ );
}

NOINLINE void FilterData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &filter_type );
    params_.add( &adsr_lfo_mix );
    params_.add( &distortion );
    params_.add( &modulate_distortion );
    params_.add( &cutoff );
    params_.add( &modulate_cutoff );
    params_.add( &resonance );
    params_.add( &modulate_resonance );
    params_.add( &gain );
    params_.add( &modulate_gain );
    params_.add( &width );
    params_.add( &modulate_width );
    params_.add( &compressor );
    params_.add( &output );
    params_.add( &modulate_output );
    params_.add( &output_clipping );

    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i ) {
        params_.add( &input_holds[i] );
        params_.add( &input_sustains[i] );
    }
}

//==============================================================================
#define ARP_NAME "ARP"
NOINLINE ArpSequencerData::ArpSequencerData( int id_ )
    :
    id( id_ ),

    is_on(generate_param_name(ARP_NAME,id,"is_on"),generate_short_human_name(ARP_NAME,"on")),

    step( ARP_NAME,id,"step","step", false ),
    tune( ARP_NAME,id,"tune","tune", false ),
    velocity( ARP_NAME,id,"velocity","velocity", false ),

    shuffle(generate_param_name(ARP_NAME,id,"shuffle"),generate_short_human_name(ARP_NAME,"shuffle")),
    connect(generate_param_name(ARP_NAME,id,"connect"),generate_short_human_name(ARP_NAME,"connect")),

    speed_multi(generate_param_name(ARP_NAME,id,"speed_multi"),generate_short_human_name(ARP_NAME,"speed_multi"))
{
}
NOINLINE ArpSequencerData::~ArpSequencerData() {}

NOINLINE void ArpSequencerData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &is_on );

    for( int i = 0 ; i != SUM_ENV_ARP_STEPS ; ++i ) {
        params_.add( &step[i] );
        params_.add( &tune[i] );
        params_.add( &velocity[i] );
    }

    params_.add( &shuffle );
    params_.add( &connect );

    params_.add( &speed_multi );
}
//==============================================================================
#define EQ_NAME "EQ"
NOINLINE EQData::EQData( int id_, ENVPresetDef*const def_ )
    :
    id( id_ ),

    velocity( EQ_NAME,id,"velocity","velocity", false ),
    hold( EQ_NAME,id,"hold","velocity", false )
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id ) {
        ENVPresetData* data = new ENVPresetData( band_id+EQ_ENV_ID_OFFSET, def_ );
        env_datas.add( data );

        velocity[band_id].register_always_listener(this);
    }
}
NOINLINE EQData::~EQData() {
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id ) {
        velocity[band_id].remove_listener(this);
    }
}

void EQData::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept {
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        if( velocity[i].get_base() == param_ )
        {
            env_datas[i]->sustain.set_scaled_without_notification( positive( param_->get_reference() ) );
            break;
        }
    }
}
void EQData::parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}
void EQData::parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}
NOINLINE void EQData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        env_datas.getUnchecked( i )->get_saveable_params( params_ );
        params_.add( &velocity[i] );
        params_.add( &hold[i] );
    }
}

//==============================================================================
#define REVERB_NAME "VERB"
NOINLINE ReverbData::ReverbData( int id_ )
    :
    id( id_ ),

    room(generate_param_name(REVERB_NAME,id,"room"),generate_short_human_name(REVERB_NAME,id_,"room")),
    dry_wet_mix(generate_param_name(REVERB_NAME,id,"dry-wet"),generate_short_human_name(REVERB_NAME,id_,"dry-wet")),
    width(generate_param_name(REVERB_NAME,id,"width"),generate_short_human_name(REVERB_NAME,id_,"width"))
{
}
NOINLINE ReverbData::~ReverbData() {}

NOINLINE void ReverbData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &room );
    params_.add( &dry_wet_mix );
    params_.add( &width );
}

//==============================================================================
#define CHORUS_NAME "CHR"
NOINLINE ChorusData::ChorusData( int id_, ENVPresetDef*const def_ )
    :
    id( id_ ),

    modulation(generate_param_name(CHORUS_NAME,id,"modulation"),generate_short_human_name(CHORUS_NAME,id_,"chorus")),
    hold_modulation(generate_param_name(CHORUS_NAME,id,"hold-modulation"),generate_short_human_name(CHORUS_NAME,id_,"chorus-fix")),

    modulation_env_data( new ENVPresetData( CHORUS_ENV_ID_OFFSET, def_ ) ),
    shine_env_data( new ENVPresetData( CHORUS_ENV_ID_OFFSET, def_ ) )
{
    modulation.register_always_listener(this);
}
NOINLINE ChorusData::~ChorusData() {}

NOINLINE void ChorusData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &modulation );
    params_.add( &hold_modulation );
}

void ChorusData::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept {
    modulation_env_data->sustain.set_scaled_without_notification( positive( param_->get_reference() ) );
}
void ChorusData::parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}
void ChorusData::parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept {
    parameter_value_changed( param_ );
}

//==============================================================================
#define SYNTH_DATA_NAME "SD"
NOINLINE SynthData::SynthData( DATA_TYPES data_type )
    :
    id( data_type ),

    volume(generate_param_name(SYNTH_DATA_NAME,MASTER,"volume"),generate_short_human_name("MAIN","volume")),
    glide(generate_param_name(SYNTH_DATA_NAME,MASTER,"glide"),generate_short_human_name("MAIN","glide")),

    delay(generate_param_name(SYNTH_DATA_NAME,MASTER,"delay"),generate_short_human_name("MAIN","delay")),

    effect_bypass(generate_param_name(SYNTH_DATA_NAME,MASTER,"effect_bypass"),generate_short_human_name("MAIN","fx_bypass")),

    final_compression(generate_param_name(SYNTH_DATA_NAME,MASTER,"final_compression"),generate_short_human_name("MAIN","clipping")),
    colour(generate_param_name(SYNTH_DATA_NAME,MASTER,"colour"),generate_short_human_name("MAIN","colour")),
    resonance(generate_param_name(SYNTH_DATA_NAME,MASTER,"shape"),generate_short_human_name("MAIN","shape")),
    curve_shape(generate_param_name(SYNTH_DATA_NAME,MASTER,"env_shape"),generate_short_human_name("MAIN","env_shape")),

    sync(generate_param_name(SYNTH_DATA_NAME,MASTER,"sync"),generate_short_human_name("MAIN","sync")),
    speed(generate_param_name(SYNTH_DATA_NAME,MASTER,"speed"),generate_short_human_name("MAIN","speed")),

    glide_motor_time(generate_param_name(SYNTH_DATA_NAME,MASTER,"glide_motor_time"),generate_short_human_name("MAIN","glide_motor_time")),
    velocity_glide_time(generate_param_name(SYNTH_DATA_NAME,MASTER,"velocity_glide_time"),generate_short_human_name("MAIN","velocity_glide_time")),

    ctrl(generate_param_name(SYNTH_DATA_NAME,MASTER,"ctrl"),generate_short_human_name("MAIN","ctrl")),
    midi_pickup_offset(generate_param_name(SYNTH_DATA_NAME,MASTER,"midi_pickup_offset"),generate_short_human_name("MAIN","midi_pick_up")),

    env_preset_def(new ENVPresetDef( MASTER ) ),
    eq_data(new EQData(MASTER, env_preset_def)),
    arp_sequencer_data(new ArpSequencerData( MASTER )),
    reverb_data(new ReverbData( MASTER ) ),
    chorus_data(new ChorusData( MASTER, env_preset_def )),

    morhp_states( SYNTH_DATA_NAME,MASTER,"morph_state","morph",false ),
    morhp_switch_states( SYNTH_DATA_NAME,MASTER,"morph_switch_state","morph_tgl",false ),
    linear_morhp_state(generate_param_name(SYNTH_DATA_NAME,MASTER,"linear_morhp_state"),generate_short_human_name("morph_line")),
    morph_motor_time(generate_param_name(SYNTH_DATA_NAME,MASTER,"morph_motor_time"),generate_short_human_name("morph_motor")),

    current_program(-1),
    current_program_abs(-1),
    current_bank(0),

    animate_input_env(generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_input_env"),generate_short_human_name("animate_input_env")),
    animate_eq_env(generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_eq_env"),generate_short_human_name("animate_eq_env")),
    animate_modulations(generate_param_name(SYNTH_DATA_NAME,MASTER,"animate_modulations"),generate_short_human_name("animate_modulations"))
{
    if( data_type == MASTER ) {
        mono_ParameterOwnerStore::getInstance()->env_preset_def = env_preset_def;
        mono_ParameterOwnerStore::getInstance()->eq_data = eq_data;
        mono_ParameterOwnerStore::getInstance()->arp_data = arp_sequencer_data;
        mono_ParameterOwnerStore::getInstance()->reverb_data = reverb_data;
        mono_ParameterOwnerStore::getInstance()->chorus_data = chorus_data;
        mono_ParameterOwnerStore::getInstance()->synth_data = this;
    }

    for( int i = 0 ; i != SUM_OSCS ; ++i ) {
        OSCData* data = new OSCData(i);
        osc_datas.add( data );
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->osc_datas.add( data );
    }

    for( int i = 0 ; i != SUM_LFOS ; ++i ) {
        LFOData* data = new LFOData(i);
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->lfo_datas.add( data );
        lfo_datas.add( data );
    }

    for( int i = 0 ; i != SUM_FILTERS+1 ; ++i ) {
        ENVData* data = new ENVData(i);
        if( data_type == MASTER )
            mono_ParameterOwnerStore::getInstance()->env_datas.add( data );
        env_datas.add( data );
    }

    for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
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
    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id ) {
        morhp_states[morpher_id].register_listener(this);
    }
    linear_morhp_state.register_listener(this);

    colect_saveable_parameters();

    if( data_type == MASTER )
        refresh_banks_and_programms();
}
NOINLINE SynthData::~SynthData() {
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

    for( int i = 0 ; i != other_.lfo_datas.size() ; ++i )
        *lfo_datas[i] = *other_.lfo_datas[i];

    for( int i = 0 ; i != other_.osc_datas.size() ; ++i )
        (*osc_datas[i]) = (*other_.osc_datas[i]);

    for( int i = 0 ; i != other_.env_datas.size() ; ++i )
        (*env_datas[i]) = (*other_.env_datas[i]);

    for( int i = 0 ; i != other_.filter_datas.size() ; ++i )
        (*filter_datas[i]) = (*other_.filter_datas[i]);

    for( int i = 0 ; i != other_.filter_input_env_datas.size() ; ++i )
        (*filter_input_env_datas[i]) = (*other_.filter_input_env_datas[i]);

    *env_preset_def = *other_.env_preset_def;
    *eq_data = *other_.eq_data;
    *arp_sequencer_data = *other_.arp_sequencer_data;
    *reverb_data = *other_.reverb_data;
    *chorus_data = *other_.chorus_data;

    final_compression = other_.final_compression;

    // NO NEED FOR COPY
    // morhp_states

    return *this;
}
NOINLINE void SynthData::get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept {
    params_.add( &volume );
    params_.add( &glide );
    params_.add( &delay );
    params_.add( &effect_bypass );
    params_.add( &final_compression );
    params_.add( &colour );
    params_.add( &resonance );
    params_.add( &curve_shape );
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

    for( int morpher_id = 0 ; morpher_id != SUM_MORPHER_GROUPS ; ++morpher_id ) {
        params_.add( &morhp_states[morpher_id] );
        params_.add( &morhp_switch_states[morpher_id] );
    }
}
NOINLINE void SynthData::colect_saveable_parameters() noexcept {

    // on top to be the first on load and get the right update order (bit hacky, but ok ;--)
    env_preset_def->get_saveable_params( saveable_parameters );

    for( int i = 0 ; i != SUM_LFOS ; ++i )
        lfo_datas[i]->get_saveable_params( saveable_parameters );

    for( int i = 0 ; i != SUM_OSCS ; ++i )
        osc_datas[i]->get_saveable_params( saveable_parameters );

    for( int flt_id = 0 ; flt_id != SUM_FILTERS ; ++flt_id ) {
        for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        {
            filter_input_env_datas[input_id+flt_id*SUM_INPUTS_PER_FILTER]->get_saveable_params( saveable_parameters );
        }
        filter_datas[flt_id]->get_saveable_params( saveable_parameters );
    }

    for( int i = 0 ; i != SUM_ENVS ; ++i )
        env_datas[i]->get_saveable_params( saveable_parameters );

    eq_data->get_saveable_params( saveable_parameters );
    arp_sequencer_data->get_saveable_params( saveable_parameters );
    reverb_data->get_saveable_params( saveable_parameters );
    chorus_data->get_saveable_params( saveable_parameters );

    get_saveable_params( saveable_parameters );
}
NOINLINE void SynthData::init_morph_groups( DATA_TYPES data_type ) noexcept {
    {
        // MAIN
        {
            morph_groups.add( &morph_group_main );
            morph_group_main.set_id( MAIN );

            morph_group_main.register_parameter( volume.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( final_compression.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->attack.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_attack_time.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->decay.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_decay_time.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->sustain.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->sustain_time.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->release.get_base(), data_type == MASTER  );
            morph_group_main.register_parameter( env_datas[MAIN_ENV]->max_release_time.get_base(), data_type == MASTER  );

            morph_group_main.register_parameter( env_preset_def->attack_1.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_1.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_1.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_2.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_2.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_2.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_3.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_3.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_3.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->attack_4.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->decay_4.get_base(), data_type == MASTER );
            morph_group_main.register_parameter( env_preset_def->release_4.get_base(), data_type == MASTER );

            //speed_multi
        }

        // OSC'S
        {
            {
                morph_groups.add( &morph_group_osc_1 );
                morph_group_osc_1.set_id( OSC_1 );

                morph_group_osc_1.register_parameter( osc_datas[0]->wave.get_base(), data_type == MASTER );
                morph_group_osc_1.register_parameter( osc_datas[0]->octave.get_base(), data_type == MASTER );
                morph_group_osc_1.register_parameter( osc_datas[0]->fm_amount.get_base(), data_type == MASTER );

                morph_group_osc_1.register_switch_parameter( osc_datas[0]->is_lfo_modulated.get_base(), data_type == MASTER );
                morph_group_osc_1.register_switch_parameter( osc_datas[0]->sync.get_base(), data_type == MASTER );

                morph_group_osc_1.register_switch_parameter( osc_datas[0]->puls_width.get_base(), data_type == MASTER );
                morph_group_osc_1.register_switch_parameter( osc_datas[0]->osc_switch.get_base(), data_type == MASTER );
            }

            {
                morph_groups.add( &morph_group_osc_2 );
                morph_group_osc_2.set_id( OSC_2 );

                morph_group_osc_2.register_parameter( osc_datas[1]->wave.get_base(), data_type == MASTER );
                morph_group_osc_2.register_parameter( osc_datas[1]->octave.get_base(), data_type == MASTER );
                morph_group_osc_2.register_parameter( osc_datas[1]->fm_amount.get_base(), data_type == MASTER );

                morph_group_osc_2.register_switch_parameter( osc_datas[1]->is_lfo_modulated.get_base(), data_type == MASTER );
                morph_group_osc_2.register_switch_parameter( osc_datas[1]->sync.get_base(), data_type == MASTER );

                morph_group_osc_2.register_switch_parameter( osc_datas[1]->puls_width.get_base(), data_type == MASTER );
                morph_group_osc_2.register_switch_parameter( osc_datas[1]->osc_switch.get_base(), data_type == MASTER );
            }

            {
                morph_groups.add( &morph_group_osc_3 );
                morph_group_osc_3.set_id( OSC_3 );

                morph_group_osc_3.register_parameter( osc_datas[2]->wave.get_base() , data_type == MASTER  );
                morph_group_osc_3.register_parameter( osc_datas[2]->octave.get_base(), data_type == MASTER  );
                morph_group_osc_3.register_parameter( osc_datas[2]->fm_amount.get_base(), data_type == MASTER  );

                morph_group_osc_3.register_switch_parameter( osc_datas[2]->is_lfo_modulated.get_base(), data_type == MASTER  );
                morph_group_osc_3.register_switch_parameter( osc_datas[2]->sync.get_base(), data_type == MASTER  );

                morph_group_osc_3.register_switch_parameter( osc_datas[2]->puls_width.get_base(), data_type == MASTER );
                morph_group_osc_3.register_switch_parameter( osc_datas[2]->osc_switch.get_base(), data_type == MASTER );
            }
        }

        // FM
        {
            morph_groups.add( &morph_group_fm );
            morph_group_fm.set_id( FM );

            morph_group_fm.register_parameter( osc_datas[0]->fm_multi.get_base(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[0]->fm_swing.get_base(), data_type == MASTER  );
            // TODO
            //morph_group_fm.register_parameter( osc_datas[0]->osc_swing.get_base(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[1]->fm_multi.get_base(), data_type == MASTER  );
            morph_group_fm.register_parameter( osc_datas[2]->fm_multi.get_base(), data_type == MASTER  );

            morph_group_fm.register_switch_parameter( osc_datas[0]->puls_width.get_base(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[1]->puls_width.get_base(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[2]->puls_width.get_base(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[0]->fm_wave.get_base(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[1]->fm_wave.get_base(), data_type == MASTER  );
            morph_group_fm.register_switch_parameter( osc_datas[2]->fm_wave.get_base(), data_type == MASTER  );
        }

        // FILTERS
        {
            {
                morph_groups.add( &morph_group_filter_1 );
                morph_group_filter_1.set_id( FILTER_1 );

                // FLT
                morph_group_filter_1.register_parameter( filter_datas[0]->adsr_lfo_mix.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->distortion.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->resonance.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->width.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->gain.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->output.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->output_clipping.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( filter_datas[0]->compressor.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_1.register_parameter( filter_input_env_datas[input_id+0*SUM_INPUTS_PER_FILTER]->state.get_base(), data_type == MASTER  );
                    morph_group_filter_1.register_parameter( filter_datas[0]->input_sustains[input_id].get_base(), data_type == MASTER  );
                }

                morph_group_filter_1.register_switch_parameter( filter_datas[0]->filter_type.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_distortion.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_resonance.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_width.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_gain.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_1.register_switch_parameter( filter_datas[0]->input_holds[input_id].get_base(), data_type == MASTER  );
                }
                morph_group_filter_1.register_switch_parameter( filter_datas[0]->modulate_output.get_base(), data_type == MASTER  );

                // LFO
                morph_group_filter_1.register_parameter( lfo_datas[0]->speed.get_base(), data_type == MASTER  );

                // ENV
                morph_group_filter_1.register_parameter( env_datas[0]->attack.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_attack_time.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->decay.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_decay_time.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->sustain.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->sustain_time.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->release.get_base(), data_type == MASTER  );
                morph_group_filter_1.register_parameter( env_datas[0]->max_release_time.get_base(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_filter_2 );
                morph_group_filter_2.set_id( FILTER_2 );

                // FLT
                morph_group_filter_2.register_parameter( filter_datas[1]->adsr_lfo_mix.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->distortion.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->resonance.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->width.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->gain.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->output.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->output_clipping.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( filter_datas[1]->compressor.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_2.register_parameter( filter_input_env_datas[input_id+1*SUM_INPUTS_PER_FILTER]->state.get_base(), data_type == MASTER  );
                    morph_group_filter_2.register_parameter( filter_datas[1]->input_sustains[input_id].get_base(), data_type == MASTER  );
                }

                morph_group_filter_2.register_switch_parameter( filter_datas[1]->filter_type.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_distortion.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_resonance.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_width.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_gain.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_2.register_switch_parameter( filter_datas[1]->input_holds[input_id].get_base(), data_type == MASTER  );
                }
                morph_group_filter_2.register_switch_parameter( filter_datas[1]->modulate_output.get_base(), data_type == MASTER  );

                // LFO
                morph_group_filter_2.register_parameter( lfo_datas[1]->speed.get_base(), data_type == MASTER  );

                // ENV
                morph_group_filter_2.register_parameter( env_datas[1]->attack.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_attack_time.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->decay.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_decay_time.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->sustain.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->sustain_time.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->release.get_base(), data_type == MASTER  );
                morph_group_filter_2.register_parameter( env_datas[1]->max_release_time.get_base(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_filter_3 );
                morph_group_filter_3.set_id( FILTER_3 );

                // FLT
                morph_group_filter_3.register_parameter( filter_datas[2]->adsr_lfo_mix.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->distortion.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->resonance.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->width.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->gain.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->output.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->output_clipping.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( filter_datas[2]->compressor.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_3.register_parameter( filter_input_env_datas[input_id+2*SUM_INPUTS_PER_FILTER]->state.get_base(), data_type == MASTER  );
                    morph_group_filter_3.register_parameter( filter_datas[2]->input_sustains[input_id].get_base(), data_type == MASTER  );
                }

                morph_group_filter_3.register_switch_parameter( filter_datas[2]->filter_type.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_distortion.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_cutoff.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_resonance.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_width.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_gain.get_base(), data_type == MASTER  );
                for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
                {
                    morph_group_filter_3.register_switch_parameter( filter_datas[2]->input_holds[input_id].get_base(), data_type == MASTER  );
                }
                morph_group_filter_3.register_switch_parameter( filter_datas[2]->modulate_output.get_base(), data_type == MASTER  );

                // LFO
                morph_group_filter_3.register_parameter( lfo_datas[2]->speed.get_base(), data_type == MASTER  );

                // ENV
                morph_group_filter_3.register_parameter( env_datas[2]->attack.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_attack_time.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->decay.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_decay_time.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->sustain.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->sustain_time.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->release.get_base(), data_type == MASTER  );
                morph_group_filter_3.register_parameter( env_datas[2]->max_release_time.get_base(), data_type == MASTER  );
            }
        }


        // EQ
        {
            morph_groups.add( &morph_group_eq );
            morph_group_eq.set_id( EQ );

            for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id ) {
                morph_group_eq.register_parameter( eq_data->velocity[band_id].get_base(), data_type == MASTER  );
                morph_group_eq.register_parameter( eq_data->env_datas[band_id]->state.get_base(), data_type == MASTER  );

                morph_group_eq.register_switch_parameter( eq_data->hold[band_id].get_base(), data_type == MASTER  );
            }
        }

        // FX
        {
            morph_groups.add( &morph_group_fx );
            morph_group_fx.set_id( FX );

            // MAIN
            morph_group_fx.register_parameter( colour.get_base(), data_type == MASTER );
            morph_group_fx.register_parameter( resonance.get_base(), data_type == MASTER );
            morph_group_fx.register_parameter( effect_bypass.get_base(), data_type == MASTER  );
            // REVERB
            morph_group_fx.register_parameter( reverb_data->room.get_base(), data_type == MASTER  );
            morph_group_fx.register_parameter( reverb_data->dry_wet_mix.get_base(), data_type == MASTER  );
            morph_group_fx.register_parameter( reverb_data->width.get_base(), data_type == MASTER  );
            // DELAY
            morph_group_fx.register_parameter( delay.get_base(), data_type == MASTER  );
            // CHORUS
            morph_group_fx.register_parameter( chorus_data->modulation.get_base(), data_type == MASTER  );
            morph_group_fx.register_switch_parameter( chorus_data->hold_modulation.get_base(), data_type == MASTER  );
        }

        // ARP
        {
            {
                morph_groups.add( &morph_group_arp_tune );
                morph_group_arp_tune.set_id( ARP_TUNE );

                for( int step_id = 0 ; step_id != SUM_ENV_ARP_STEPS ; ++step_id ) {
                    morph_group_arp_tune.register_parameter( arp_sequencer_data->tune[step_id].get_base(), data_type == MASTER  );
                }
            }

            {
                morph_groups.add( &morph_group_arp_velocity );
                morph_group_arp_velocity.set_id( ARP_VELOCITY );

                for( int step_id = 0 ; step_id != SUM_ENV_ARP_STEPS ; ++step_id ) {
                    morph_group_arp_velocity.register_parameter( arp_sequencer_data->velocity[step_id].get_base(), data_type == MASTER  );
                }
            }

            {
                morph_groups.add( &morph_group_arp_glide_shuffle );
                morph_group_arp_glide_shuffle.set_id( ARP_GLIDE_SHUFFLE );

                morph_group_arp_glide_shuffle.register_parameter( arp_sequencer_data->shuffle.get_base(), data_type == MASTER  );
                morph_group_arp_glide_shuffle.register_parameter( glide.get_base(), data_type == MASTER  );
            }

            {
                morph_groups.add( &morph_group_arp_switchs );
                morph_group_arp_switchs.set_id( ARP_SWITCHS );

                // is_on
                // speed_multi
                morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data->connect.get_base(), data_type == MASTER  );
                //morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data.connect.get_base(), data_type == MASTER  );
                for( int step_id = 0 ; step_id != SUM_ENV_ARP_STEPS ; ++step_id ) {
                    morph_group_arp_switchs.register_switch_parameter( arp_sequencer_data->step[step_id].get_base(), data_type == MASTER  );
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
    for( int i = 0 ; i != morph_groups.size() ; ++i )
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
void SynthData::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept
{
    if( param_ == &morhp_states[0] ) {
        morph( 0, *param_ );
    }
    else if( param_ == &morhp_states[1] ) {
        morph( 1, *param_ );
    }
    else if( param_ == &morhp_states[2] ) {
        morph( 2, *param_ );
    }
    else if( param_ == &morhp_states[3] ) {
        morph( 3, *param_ );
    }
    else if( param_ == &linear_morhp_state )
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
    if( force_ ) {
        morhp_states[morpher_id_].stopTimeChanger();
        morhp_states[morpher_id_] = morph_amount_left_to_right_;
    }

    Array< MorphGroup* >& morph_groups_for_morpher = morph_groups_per_morpher.getReference(morpher_id_);
    for( int morph_group_id = 0 ; morph_group_id != morph_groups_for_morpher.size() ; ++morph_group_id ) {
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
    for( int morph_group_id = 0 ; morph_group_id != morph_groups_for_morpher.size() ; ++morph_group_id )
        morph_groups_for_morpher[morph_group_id]->morph_switchs( morhp_switch_states[morpher_id_] );
}
bool SynthData::get_morph_switch_state( int morpher_id_ ) const noexcept {
    return morhp_switch_states[morpher_id_];
}

NOINLINE void SynthData::save_to( XmlElement* xml_ ) const noexcept {
    if( xml_ )
    {
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
            saveable_parameters.getUnchecked(i)->write_to(*xml_);

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

#include "UiLookAndFeel.h"
NOINLINE void SynthData::read_from( const XmlElement* xml_ ) noexcept {
    if( xml_ )
    {
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
            saveable_parameters.getUnchecked(i)->read_from(*xml_);

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
    folder = File(folder.getFullPathName()+"/Monoplugs/Monolisa/");
    if( folder.createDirectory() )
    {
        File midi_file( File( folder.getFullPathName() + String("/") + "patch.midi") );

        XmlElement xml("MIDI-PATCH-1.0");
        for( int i = 0 ; i != saveable_parameters.size() ; ++i )
            saveable_parameters.getUnchecked(i)->write_midi_to(xml);

        xml.writeToFile(midi_file,"");
    }
}
NOINLINE void SynthData::read_midi() noexcept {
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    File midi_file = File(folder.getFullPathName()+"/Monoplugs/Monolisa/patch.midi");
    ScopedPointer<XmlElement> xml = XmlDocument( midi_file ).getDocumentElement();
    if( xml )
    {
        if( xml->hasTagName("MIDI-PATCH-1.0") )
        {
            for( int i = 0 ; i != saveable_parameters.size() ; ++i )
                saveable_parameters.getUnchecked(i)->read_midi_from(*xml);
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

NOINLINE void SynthData::MorphGroup::register_parameter( mono_ParameterBase< float >*const param_, bool is_master_ ) {
    params.add( param_ );

    if( is_master_ )
        param_->register_listener(this);
}
NOINLINE void SynthData::MorphGroup::register_switch_parameter( mono_ParameterBase< bool >*const param_, bool is_master_ ) {
    switch_bool_params.add( param_ );

    if( is_master_ )
        param_->register_listener(this);
}
NOINLINE void SynthData::MorphGroup::register_switch_parameter( mono_ParameterBase< int >*const param_, bool is_master_ ) {
    switch_int_params.add( param_ );

    if( is_master_ )
        param_->register_listener(this);
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
            params[i]->set_scaled_without_notification( new_value );

            // MODULATION VALUE
            if( params[i]->has_modulation() )
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
            if( params[i]->has_modulation() )
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
            float min = params[i]->get_min();
            float max = params[i]->get_max();
            float new_value = *params[i] + sync_param_deltas[i];
            if( new_value > max )
                new_value = max;
            else if( new_value < min )
                new_value = min;

            params[i]->set_scaled_without_notification( new_value );
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
                switch_bool_params[i]->set_scaled_without_notification( *right_morph_group->switch_bool_params[i] );
            else
                switch_bool_params[i]->set_scaled_without_notification( *left_morph_group->switch_bool_params[i] );
        }
        for( int i = 0 ; i != switch_int_params.size() ; ++i )
        {
            if( current_switch == RIGHT )
                switch_int_params[i]->set_scaled_without_notification( *right_morph_group->switch_int_params[i] );
            else
                switch_int_params[i]->set_scaled_without_notification( *left_morph_group->switch_int_params[i] );
        }
    }
}

void SynthData::MorphGroup::parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept {
    if( left_morph_group )
    {
        const int param_id = params.indexOf( param_ );
        if( param_id != -1 )
        {
            mono_ParameterBase< float >& left_source_param = *left_morph_group->params[param_id];
            mono_ParameterBase< float >& right_source_param = *right_morph_group->params[param_id];

            // x = l*(1-m)+r*m
            // r = (l*(m-1.0f)+x)/m
            // l = (m*r-x) / (m-1)
            float current_value = *param_;
            float right_value = right_source_param;
            bool update_left_or_right = last_power_of_right > 0.5f ? RIGHT : LEFT;
            if( update_left_or_right == RIGHT )
            {
                float left_value = left_source_param;
                float new_right_value = (left_value*(last_power_of_right-1)+current_value) / last_power_of_right;

                float max = right_source_param.get_max();
                float min = right_source_param.get_min();
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

                right_source_param.set_scaled_without_notification( new_right_value );
                right_value = new_right_value;
            }
            if( update_left_or_right == LEFT )
            {
                float new_left_value = (last_power_of_right*right_value-current_value) / (last_power_of_right-1);

                float max = left_source_param.get_max();
                float min = left_source_param.get_min();
                if( new_left_value > max )
                {
                    new_left_value = max;
                }
                else if( new_left_value < min )
                {
                    new_left_value = min;
                }

                left_source_param.set_scaled_without_notification( new_left_value );
            }
        }
    }
}
void SynthData::MorphGroup::parameter_modulation_value_changed( mono_ParameterBase< float >* param_ ) noexcept {
    if( left_morph_group )
    {
        const int param_id = params.indexOf( param_ );
        if( param_id != -1 )
        {
            mono_ParameterBase< float >& left_source_param = *left_morph_group->params[param_id];
            mono_ParameterBase< float >& right_source_param = *right_morph_group->params[param_id];

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
void SynthData::MorphGroup::parameter_value_changed( mono_ParameterBase< bool >* param_ ) noexcept {
    if( left_morph_group )
    {
        const int param_id = switch_bool_params.indexOf( param_ );
        if( param_id != -1 )
        {
            if( current_switch == LEFT )
                left_morph_group->switch_bool_params[param_id]->set_scaled_without_notification( *param_ );
            else
                right_morph_group->switch_bool_params[param_id]->set_scaled_without_notification( *param_ );
        }
    }
}
void SynthData::MorphGroup::parameter_value_changed( mono_ParameterBase< int >* param_ ) noexcept {
    if( left_morph_group )
    {
        const int param_id = switch_int_params.indexOf( param_ );
        if( param_id != -1 )
        {
            if( current_switch == LEFT )
                left_morph_group->switch_int_params[param_id]->set_scaled_without_notification( *param_ );
            else
                right_morph_group->switch_int_params[param_id]->set_scaled_without_notification( *param_ );
        }
    }
}

static inline File get_bank_folder( const String& bank_name_ ) {
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    folder = File(folder.getFullPathName()+"/Monoplugs/Monolisa/"+bank_name_);
    folder.createDirectory();

    return folder;
}
static inline File get_program_file( const String& bank_name_, const String& program_name_ ) {
    return File( get_bank_folder( bank_name_ ).getFullPathName()
                 + String("/")
                 + program_name_
                 + ".mlprog");
}

NOINLINE void SynthData::refresh_banks_and_programms() {
    DATA( synth_data ).banks.clearQuick();
    update_banks( DATA( synth_data ).banks );

    DATA( synth_data ).program_names.clearQuick();
    DATA( synth_data ).program_names.add( StringArray() );
    DATA( synth_data ).program_names.add( StringArray() );
    DATA( synth_data ).program_names.add( StringArray() );
    DATA( synth_data ).program_names.add( StringArray() );

    update_bank_programms( 0, DATA( synth_data ).program_names.getReference(0) );
    update_bank_programms( 1, DATA( synth_data ).program_names.getReference(1) );
    update_bank_programms( 2, DATA( synth_data ).program_names.getReference(2) );
    update_bank_programms( 3, DATA( synth_data ).program_names.getReference(3) );

    DATA( synth_data ).calc_current_program_abs();
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
NOINLINE void SynthData::update_bank_programms( int bank_id_, StringArray& program_names_ ) {
    File bank_folder = get_bank_folder( DATA( synth_data ).banks[bank_id_] );
    Array< File > program_files;
    bank_folder.findChildFiles( program_files, File::findFiles, false, "*.mlprog" );
    //sort_by_date(program_files);
    sort_by_name(program_files);

    for( int i = 0 ; i != program_files.size() ; ++i )
        program_names_.add( program_files.getReference(i).getFileNameWithoutExtension() );
}

NOINLINE const StringArray& SynthData::get_banks() {
    return DATA( synth_data ).banks;
}
NOINLINE const StringArray& SynthData::get_programms( int bank_id_ ) {
    return DATA( synth_data ).program_names.getReference(bank_id_);
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
    return DATA( synth_data ).program_names.getReference(current_bank)[current_program];
}
const String& SynthData::get_program_name_abs(int id_) const noexcept
{
    for( int bank_id = 0 ; bank_id != DATA( synth_data ).banks.size() ; ++bank_id )
    {
        int bank_size = DATA( synth_data ).program_names.getReference(bank_id).size();
        if( id_ < bank_size )
            return DATA( synth_data ).program_names.getReference(bank_id)[id_];
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
    for( int bank_id = 0 ; bank_id != current_bank ; ++bank_id )
    {
        int bank_size = DATA( synth_data ).program_names.getReference(bank_id).size();
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
NOINLINE void SynthData::set_current_program_abs( int programm_index_ ) {
    int sum_programms = 0;
    for( int bank_id = 0 ; bank_id != DATA( synth_data ).banks.size() ; ++bank_id ) {
        int bank_size = DATA( synth_data ).program_names.getReference(bank_id).size();
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
NOINLINE const StringArray& SynthData::get_current_bank_programms() {
    return DATA( synth_data ).program_names.getReference(current_bank);
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
NOINLINE bool SynthData::rename( const String& new_name_ ) {
    if( current_program == -1 )
        return false;

    File program = get_program_file( DATA( synth_data ).banks[current_bank], DATA( synth_data ).program_names.getReference(current_bank)[current_program] );

    bool success = false;

    String name = new_name_;
    generate_programm_name( DATA( synth_data ).banks[current_bank], name );
    if( program.existsAsFile() ) {
        success = program.moveFileTo( get_bank_folder(DATA( synth_data ).banks[current_bank]).getFullPathName()
                                      + String("/")
                                      + name
                                      + ".mlprog" );
    }

    if( success )
    {
        refresh_banks_and_programms();
        current_program = DATA( synth_data ).program_names[current_bank].indexOf(name);
    }

    return success;
}
NOINLINE bool SynthData::load( bool load_morph_groups ) {
    if( current_program == -1 )
        return false;

    return load( DATA( synth_data ).banks[current_bank], DATA( synth_data ).program_names.getReference(current_bank)[current_program], load_morph_groups );
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
        int last_index = DATA( synth_data ).program_names.getReference(current_bank).size()-1;
        if( last_index > 0 ) {
            current_program = last_index;
            success = load();
        }
    }

    return success;
}
NOINLINE bool SynthData::load_next() {
    bool success = false;

    if( current_program+1 < DATA( synth_data ).program_names.getReference(current_bank).size() )
    {
        current_program++;
        success = load();
    }
    else
    {
        if( DATA( synth_data ).program_names.getReference(current_bank).size() )
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

    File program = get_program_file( DATA( synth_data ).banks[current_bank], DATA( synth_data ).program_names.getReference(current_bank)[current_program] );
    bool success = AlertWindow::showNativeDialogBox
                   (
                       "REPLACE PROJECT?",
                       String("Overwrite project: ")+DATA( synth_data ).banks[current_bank]+String(":")+DATA( synth_data ).program_names.getReference(current_bank)[current_program],
                       true
                   );
    if( success )
        success = write2file( DATA( synth_data ).banks[current_bank], DATA( synth_data ).program_names.getReference(current_bank)[current_program] );

    return success;
}
NOINLINE bool SynthData::create_new() {
    String new_program_name = String("New Program");
    generate_programm_name( DATA( synth_data ).banks[current_bank], new_program_name );

    bool success = write2file( DATA( synth_data ).banks[current_bank], new_program_name );

    if( success )
    {
        refresh_banks_and_programms();
        current_program = DATA( synth_data ).program_names[current_bank].indexOf(new_program_name);
    }

    return success;
}

NOINLINE bool SynthData::remove() {
    if( current_program == -1 )
        return false;

    File program = get_program_file( DATA( synth_data ).banks[current_bank], DATA( synth_data ).program_names.getReference(current_bank)[current_program] );
    bool success = AlertWindow::showNativeDialogBox
                   (
                       "DELETE PROJECT?",
                       String("Delete project: ")+DATA( synth_data ).banks[current_bank]+String(":")+DATA( synth_data ).program_names.getReference(current_bank)[current_program],
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









