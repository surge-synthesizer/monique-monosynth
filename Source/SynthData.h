/*
  ==============================================================================

    SynthData.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONOSYNTH_DATA_H_INCLUDED
#define MONOSYNTH_DATA_H_INCLUDED

#include "App_h_includer.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

#include "mono_Parameters.h"

//==============================================================================
//==============================================================================
//==============================================================================
enum DATA_TYPES {
    MORPH = 1,
    MASTER,

    DATA_COPY
};

//==============================================================================
//==============================================================================
//==============================================================================
enum WAVES {
    SINE,
    SAW,
    SQUARE,
    NOICE
};

enum FILTER_TYPS {
    LPF_2_PASS = 1,
    HPF,
    BPF,
    HIGH_2_PASS,
    PASS,
    LPF,
    MOOG_AND_LPF,
    UNKNOWN
};


enum SYNTHEISIS {
    FM_GAINED = true,
    FM_WAVE_SINE = true,
    FM_ADD = true,
};

//==============================================================================
//==============================================================================
//==============================================================================
enum  {
    SUM_OSCS = 3,
    SUM_AUDIO_CTRL_IN = 4,
    SUM_FILTERS = 3,
    SUM_LFOS = SUM_FILTERS,
    SUM_ENVS = SUM_FILTERS + 1,

    SUM_FILTER_RETRIGGERS = 3,

    SUM_INPUTS_PER_FILTER = 3,
    FILTER_INPUT_ENV_ID_OFFSET = 100,
    EQ_ENV_ID_OFFSET = 200,
    CHORUS_ENV_ID_OFFSET = 300,

    LEFT = 0,
    RIGHT = 1,
    
    FILTER_1 = 0,
    FILTER_2 = 1,
    FILTER_3 = 2,

    MAIN_ENV = SUM_FILTERS,

    SUM_ENV_ARP_STEPS = 16,

    OSC_1 = 0,
    OSC_2,
    OSC_3,

    MASTER_OSC = OSC_1,

    SUM_MORPHER_GROUPS = 4,

    TRIGGER_FILTER_BY_ADSR = 0,
    TRIGGER_FILTER_FROM_EXTERN = 1,
    TRIGGER_FILTER_FROM_RECORD = 2,
};

static inline float positive( float x ) noexcept {
    return x < 0 ? x * -1 : x;
}

#define MAX_MORPH_STEP 0.05f

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer;
class RuntimeListener {
protected:
    double sample_rate;
    float sample_rate_1ths;
    int block_size;

private:
    friend class RuntimeNotifyer;
    NOINLINE virtual void set_sample_rate( double sr_ ) noexcept {
        sample_rate = sr_;
        sample_rate_1ths = 1.0f/sample_rate;
    };
    NOINLINE virtual void set_block_size( int bs_ ) noexcept { block_size = bs_; };
    NOINLINE virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept {};
    NOINLINE virtual void block_size_changed() noexcept {};

protected:
    NOINLINE RuntimeListener();

public:
    NOINLINE virtual ~RuntimeListener();
};
class RuntimeNotifyer : public DeletedAtShutdown
{
    friend class RuntimeListener;
    Array<RuntimeListener*> listeners;

    double sample_rate;
    double sample_rate_1ths;
    int block_size;

public:
    void set_sample_rate( double sr_ );
    void set_block_size( int bs_ );

    double get_sample_rate() const noexcept;
    int get_block_size() const noexcept;

public:
    juce_DeclareSingleton (RuntimeNotifyer,false)

    NOINLINE RuntimeNotifyer();
    NOINLINE ~RuntimeNotifyer();
};

//==============================================================================
struct RuntimeInfo { /* TODO singleton */
    int64 samples_since_start;
    double bpm;

    // TODO standalone only
    bool is_extern_synced;
    bool is_running;
    int clock_counter;
    Array<int64> next_step_at_sample;
    Array<int> next_step;

    NOINLINE RuntimeInfo();
    NOINLINE ~RuntimeInfo();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeInfo)
};

// MACRO EXPECTED other_ as name for the right side
#define COMPARE_NOT_EQUEAL( value ) if( value != other_.value ) return true;
#define COPY_FROM_OTHER( value ) value = other_.value;
//==============================================================================
struct LFOData {
    const int id;

    mono_Parameter<float, 4, 0,16+127-33> speed;

    inline const LFOData& operator=( const LFOData& other_ ) noexcept;

    NOINLINE LFOData( int id_ );
    NOINLINE ~LFOData();

    // TODO can be static
    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    NOINLINE LFOData();
    MONO_NOT_CTOR_COPYABLE( LFOData )
    MONO_NOT_MOVE_COPY_OPERATOR( LFOData )
    JUCE_LEAK_DETECTOR ( LFOData )
};
inline const LFOData& LFOData::operator=( const LFOData& other_ ) noexcept {
    COPY_FROM_OTHER( speed )

    return *this;
}

//==============================================================================
struct OSCData {
    const int id;

    typedef mono_Parameter<float, SINE, SINE,NOICE, 1, 10> wave_t;
    wave_t wave;

    // TODO snap slider
    typedef mono_ParameterGlideModulated<float, 0, -24,24, 1, 100> octave_t;
    octave_t octave;
    typedef mono_Parameter<bool, false> is_lfo_modulated_t;
    is_lfo_modulated_t is_lfo_modulated;

    // TODO snap slider
    typedef mono_Parameter<float, 0, 0,1000, 1000, 100> fm_multi_t;
    fm_multi_t fm_multi;

    typedef mono_Parameter<float, 0, 0,1000, 1000, 100> fm_amount_t;
    fm_amount_t fm_amount;

    typedef mono_Parameter<bool, FM_WAVE_SINE, false,FM_WAVE_SINE> fm_wave_t;
    fm_wave_t fm_wave;

    typedef mono_Parameter<bool, true> sync_t;
    sync_t sync;
    typedef mono_Parameter<bool, true> mod_off_t;
    mod_off_t mod_off;

    typedef mono_Parameter<int, 0, -12,12, 1, 1000 > puls_width_t;
    puls_width_t puls_width;

    typedef mono_Parameter<float, 0, 0,1000, 1000, 100> fm_swing_t;
    fm_swing_t fm_swing;
    typedef mono_Parameter<int, 0, 0,16, 1, 1000> osc_switch_t;
    osc_switch_t osc_switch;

    inline const OSCData& operator=( const OSCData& other_ ) noexcept;

    // FOR UI FEEDBACK
    float last_modulation_value;

    // ONLY ON INIT USED CTORS
    NOINLINE OSCData( int id_ );
    NOINLINE ~OSCData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( OSCData )
    MONO_NOT_MOVE_COPY_OPERATOR( OSCData )
    JUCE_LEAK_DETECTOR( OSCData )
};
inline const OSCData& OSCData::operator=( const OSCData& other_ ) noexcept {
    COPY_FROM_OTHER( wave )
    COPY_FROM_OTHER( octave )
    COPY_FROM_OTHER( is_lfo_modulated )

    COPY_FROM_OTHER( fm_multi )
    COPY_FROM_OTHER( fm_amount )
    COPY_FROM_OTHER( fm_wave )
    COPY_FROM_OTHER( sync )
    COPY_FROM_OTHER( mod_off )

    COPY_FROM_OTHER( puls_width )
    COPY_FROM_OTHER( fm_swing )
    COPY_FROM_OTHER( osc_switch )

    return *this;
}

//==============================================================================
struct ENVData
{
    const int id;

    mono_Parameter< float, 50, 1,1000, 1000, 100 > attack;
    mono_Parameter< float, 2000, 100,20000, 1000, 100 > max_attack_time;
    mono_Parameter< float, 20, 0,1000, 1000, 100 > decay;
    mono_Parameter< float, 250, 100,20000, 1000, 100 > max_decay_time;
    mono_Parameter< float, 900, 1,1000, 1000, 100 > sustain;
    mono_Parameter< float, 1000, 1,1000, 1000, 100 > sustain_time;
    mono_Parameter< float, 200, 1,1000, 1000, 100 > release;
    mono_Parameter< float, 4000, 100,20000, 1000, 100 > max_release_time;

    inline const ENVData& operator=( const ENVData& other_ ) noexcept;

public:
    NOINLINE ENVData( int id_ );
    NOINLINE virtual ~ENVData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ENVData )
    MONO_NOT_MOVE_COPY_OPERATOR( ENVData )
    JUCE_LEAK_DETECTOR( ENVData )
};
inline const ENVData& ENVData::operator=( const ENVData& other_ ) noexcept {
    COPY_FROM_OTHER( attack )
    COPY_FROM_OTHER( max_attack_time )
    COPY_FROM_OTHER( decay )
    COPY_FROM_OTHER( max_decay_time )
    COPY_FROM_OTHER( sustain )
    COPY_FROM_OTHER( sustain_time )
    COPY_FROM_OTHER( release )
    COPY_FROM_OTHER( max_release_time )

    return *this;
}

//==============================================================================
struct ENVPresetDef {
    mono_Parameter< float, 1, 0,1000, 1000, 100 > attack_1;
    mono_Parameter< float, 100, 0,1000, 1000, 100 > decay_1;
    mono_Parameter< float, 1000, 1,1000, 1000, 100 > sustain_time_1;
    mono_Parameter< float, 1, 0,1000, 1000, 100 > release_1;
    mono_Parameter< float, 0, 0,1000, 1000, 100 > attack_2;
    mono_Parameter< float, 1000, 0,1000, 1000, 100 > decay_2;
    mono_Parameter< float, 1000, 1,1000, 1000, 100 > sustain_time_2;
    mono_Parameter< float, 1000, 0,1000, 1000, 100 > release_2;
    mono_Parameter< float, 500, 0,1000, 1000, 100 > attack_3;
    mono_Parameter< float, 0, 0,1000, 1000, 100 > decay_3;
    mono_Parameter< float, 1000, 1,1000, 1000, 100 > sustain_time_3;
    mono_Parameter< float, 100, 0,1000, 1000, 100 > release_3;
    mono_Parameter< float, 1000, 0,1000, 1000, 100 > attack_4;
    mono_Parameter< float, 0, 0,1000, 1000, 100 > decay_4;
    mono_Parameter< float, 1000, 1,1000, 1000, 100 > sustain_time_4;
    mono_Parameter< float, 1000, 0,1000, 1000, 100 > release_4;
    mono_Parameter< float, 2000, 100,20000, 1000, 100 > max_attack_time;
    mono_Parameter< float, 250, 100,20000, 1000, 100 > max_decay_time;
    mono_Parameter< float, 4000, 100,20000, 1000, 100 > max_release_time;

    inline const ENVPresetDef& operator=( const ENVPresetDef& other_ ) noexcept;

    NOINLINE ENVPresetDef( int id_ );
    NOINLINE ~ENVPresetDef();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ENVPresetDef )
    MONO_NOT_MOVE_COPY_OPERATOR( ENVPresetDef )
    JUCE_LEAK_DETECTOR( ENVPresetDef )
};

inline const ENVPresetDef& ENVPresetDef::operator=( const ENVPresetDef& other_ ) noexcept
{
    COPY_FROM_OTHER( attack_1 )
    COPY_FROM_OTHER( decay_1 )
    COPY_FROM_OTHER( sustain_time_1 )
    COPY_FROM_OTHER( release_1 )
    COPY_FROM_OTHER( attack_2 )
    COPY_FROM_OTHER( decay_2 )
    COPY_FROM_OTHER( sustain_time_2 )
    COPY_FROM_OTHER( release_2 )
    COPY_FROM_OTHER( attack_3 )
    COPY_FROM_OTHER( decay_3 )
    COPY_FROM_OTHER( sustain_time_3 )
    COPY_FROM_OTHER( release_3 )
    COPY_FROM_OTHER( attack_4 )
    COPY_FROM_OTHER( decay_4 )
    COPY_FROM_OTHER( sustain_time_4 )
    COPY_FROM_OTHER( release_4 )
    COPY_FROM_OTHER( max_attack_time )
    COPY_FROM_OTHER( max_decay_time )
    COPY_FROM_OTHER( max_release_time )

    return *this;
}

struct ENVPresetData : public ENVData, mono_ParameterListener< float >
{
    ENVPresetDef*const def;

    mono_Parameter< float, 50, 0,3000, 1000, 100 > state;

    static float get_attack_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_decay_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_sustain_time_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_release_at( const ENVPresetDef& def_, float state_ ) noexcept;

private:
    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept override;
    void update_adr_values( float value_ ) noexcept;
    void parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept override;

public:
    inline const ENVPresetData& operator=( const ENVPresetData& other_ ) noexcept;

    NOINLINE ENVPresetData( int id_, ENVPresetDef* def_ );
    NOINLINE ~ENVPresetData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ENVPresetData )
    MONO_NOT_MOVE_COPY_OPERATOR( ENVPresetData )
    JUCE_LEAK_DETECTOR( ENVPresetData )
};
inline const ENVPresetData& ENVPresetData::operator=( const ENVPresetData& other_ ) noexcept {
    COPY_FROM_OTHER( state )

    ENVData::operator=( other_ );

    return *this;
}

//==============================================================================
struct FilterData : mono_ParameterListener< float > {
    const int id;

    typedef mono_Parameter< int, LPF_2_PASS, LPF_2_PASS,MOOG_AND_LPF > filter_type_t;
    filter_type_t filter_type;

    typedef mono_Parameter< float, -900, -1000,1000, 1000, 100 > adsr_lfo_mix_t;
    adsr_lfo_mix_t adsr_lfo_mix;

    typedef mono_ParameterGlideModulated< float, 0, 0,1000, 1000, 1000 > distortion_t;
    distortion_t distortion;
    typedef mono_Parameter< bool, false > modulate_distortion_t;
    modulate_distortion_t modulate_distortion;

    typedef mono_ParameterGlideModulated< float, 200, 1,1000, 1000, 100 > cutoff_t;
    cutoff_t cutoff;
    typedef mono_Parameter< bool, true > modulate_cutoff_t;
    modulate_cutoff_t modulate_cutoff;

    typedef mono_ParameterGlideModulated< float, 300, 1,1000, 1000, 100 > resonance_t;
    resonance_t resonance;
    typedef mono_Parameter< bool, true > modulate_resonance_t;
    modulate_resonance_t modulate_resonance;

    typedef mono_ParameterGlideModulated< float, 500, 1,1000, 1000, 100 > width_t;
    width_t width;
    typedef mono_Parameter< bool, true > modulate_width_t;
    modulate_width_t modulate_width;

    typedef mono_ParameterGlideModulated< float, 300, 1,1000, 1000, 100 > gain_t;
    gain_t gain;
    typedef mono_Parameter< bool, true > modulate_gain_t;
    modulate_gain_t modulate_gain;

    Array<ENVData*> input_env_datas;
    typedef mono_Parameter< float, 0, -1000,1000, 1000, 100 > sustain_replacement_t;
    typedef mono_ParameterArray< sustain_replacement_t, SUM_INPUTS_PER_FILTER > input_sustain_array_t;
    input_sustain_array_t input_sustains;
    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept override;

    typedef mono_Parameter< bool, true > input_hold_t;
    typedef mono_ParameterArray< input_hold_t, SUM_INPUTS_PER_FILTER > input_hold_array_t;
    input_hold_array_t input_holds;

    typedef mono_Parameter< float, 0, -1000,1000, 1000, 100 > compressor_t;
    compressor_t compressor;
    typedef mono_ParameterGlideModulated< float, 750, 0,1000, 1000, 100 > output_t;
    output_t output;
    typedef mono_Parameter< float, 1000, 0,1000, 1000, 100 > output_clipping_t;
    output_clipping_t output_clipping;
    typedef mono_Parameter< bool, false > modulate_output_t;
    modulate_output_t modulate_output;

    inline const FilterData& operator=( const FilterData& other_ ) noexcept;

    // ONLY ON INIT USED CTORS
    NOINLINE FilterData( int id_, Array<ENVData*>& input_env_datas_ );
    NOINLINE ~FilterData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( FilterData )
    MONO_NOT_MOVE_COPY_OPERATOR( FilterData )
    JUCE_LEAK_DETECTOR( FilterData )
};

inline const FilterData& FilterData::operator=( const FilterData& other_ ) noexcept {
    COPY_FROM_OTHER( filter_type )

    COPY_FROM_OTHER( adsr_lfo_mix )

    COPY_FROM_OTHER( distortion )
    COPY_FROM_OTHER( modulate_distortion )

    COPY_FROM_OTHER( cutoff )
    COPY_FROM_OTHER( modulate_gain )

    COPY_FROM_OTHER( resonance )
    COPY_FROM_OTHER( modulate_resonance )

    COPY_FROM_OTHER( gain )
    COPY_FROM_OTHER( modulate_gain )

    COPY_FROM_OTHER( width )
    COPY_FROM_OTHER( modulate_width )

    COPY_FROM_OTHER( compressor )
    COPY_FROM_OTHER( output )
    COPY_FROM_OTHER( output_clipping )
    COPY_FROM_OTHER( modulate_output )

    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i ) {
        COPY_FROM_OTHER( input_sustains[i] )
        COPY_FROM_OTHER( input_holds[i] )
    }

    return *this;
}

//==============================================================================
enum SEQUENCER_SPEED {
    _XNORM,
    _X2,
    _X4,
    _X3,
    _X05,
    _X025
};
struct ArpSequencerData {
    const int id;

    mono_Parameter< bool, true > is_on;

    // SECONDARY VALUE HOLDS THE VELOCITY
    typedef mono_Parameter< bool, false > step_t;
    mono_ParameterArray< step_t, SUM_ENV_ARP_STEPS > step;

    typedef mono_Parameter< float, 0, -48,48 > tune_t;
    mono_ParameterArray< tune_t, SUM_ENV_ARP_STEPS > tune;

    typedef mono_Parameter< float, 850, 1,1000, 1000, 1000 > velocity_t;
    mono_ParameterArray< velocity_t, SUM_ENV_ARP_STEPS > velocity;

    mono_Parameter< float, 333, 0,1000, 1000, 1000 > shuffle;
    mono_Parameter< bool, false > connect;

    typedef mono_Parameter< int, _XNORM, _XNORM,_X025 > speed_multi_t;
    speed_multi_t speed_multi;

    inline const ArpSequencerData& operator=( const ArpSequencerData& other_ ) noexcept;

    NOINLINE ArpSequencerData( int id_ );
    NOINLINE ~ArpSequencerData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ArpSequencerData )
    MONO_NOT_MOVE_COPY_OPERATOR( ArpSequencerData )
    JUCE_LEAK_DETECTOR( ArpSequencerData )
};
inline const ArpSequencerData& ArpSequencerData::operator=( const ArpSequencerData& other_ ) noexcept {
    COPY_FROM_OTHER( is_on )

    for( int i = 0 ; i != SUM_ENV_ARP_STEPS ; ++i ) {
        COPY_FROM_OTHER( step[i] )
        COPY_FROM_OTHER( tune[i] )
        COPY_FROM_OTHER( velocity[i] )
    }

    COPY_FROM_OTHER( shuffle )
    COPY_FROM_OTHER( connect )

    COPY_FROM_OTHER( speed_multi )

    return *this;
}
//==============================================================================
enum EQ {
    BAND_32,
    BAND_625,
    BAND_125,
    BAND_250,
    BAND_500,
    BAND_1000,
    BAND_2000,
    BAND_4000,
    BAND_8000,
    SUM_EQ_BANDS,

};
// TODO G-FORCE
struct EQData : mono_ParameterListener< float >
{
    const int id;

    typedef mono_Parameter< float, 0, -1000,1000, 1000, 100 > sustain_replacement_t;
    mono_ParameterArray< sustain_replacement_t, SUM_EQ_BANDS > velocity;

    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept override;

    typedef mono_Parameter< bool, true > hold_t;
    mono_ParameterArray< hold_t, SUM_EQ_BANDS > hold;

    OwnedArray< ENVPresetData > env_datas;

public:
    inline const EQData& operator=( const EQData& other_ ) noexcept;

    NOINLINE EQData( int id_, ENVPresetDef*const def_ );
    NOINLINE ~EQData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( EQData )
    MONO_NOT_MOVE_COPY_OPERATOR( EQData )
    JUCE_LEAK_DETECTOR( EQData )
};
inline const EQData& EQData::operator=( const EQData& other_ ) noexcept {
    for( int i = 0 ; i != SUM_EQ_BANDS ; ++i )
    {
        COPY_FROM_OTHER( velocity[i] )
        COPY_FROM_OTHER( hold[i] )

        env_datas.getUnchecked( i )->operator=( *other_.env_datas.getUnchecked( i ) );
    }

    return *this;
}

//==============================================================================
struct ReverbData {
    const int id;

    mono_Parameter< float, 333, 0,1000, 1000, 1000 > room;
    mono_Parameter< float, 750, 0,1000, 1000, 1000 > dry_wet_mix;
    mono_Parameter< float, 300, 0,1000, 1000, 1000 > width;

    inline const ReverbData& operator=( const ReverbData& other_ ) noexcept;

    NOINLINE ReverbData( int id_ );
    NOINLINE ~ReverbData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    NOINLINE ReverbData();
    MONO_NOT_CTOR_COPYABLE( ReverbData )
    MONO_NOT_MOVE_COPY_OPERATOR( ReverbData )
    JUCE_LEAK_DETECTOR( ReverbData )
};
inline const ReverbData& ReverbData::operator=( const ReverbData& other_ ) noexcept {
    COPY_FROM_OTHER( room )
    COPY_FROM_OTHER( dry_wet_mix )
    COPY_FROM_OTHER( width )

    return *this;
}
//==============================================================================
struct ChorusData : mono_ParameterListener< float > {
    const int id;

    typedef mono_Parameter< float, 333, 0,1000, 1000, 1000 > sustain_replacement_t;
    sustain_replacement_t modulation;
    mono_Parameter< bool, true > hold_modulation;

    inline const ChorusData& operator=( const ChorusData& other_ ) noexcept;

    ScopedPointer< ENVPresetData > modulation_env_data;
    ScopedPointer< ENVPresetData > shine_env_data;

    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_changed_always_notification( mono_ParameterBase< float >* param_ ) noexcept override;
    void parameter_value_on_load_changed( mono_ParameterBase< float >* param_ ) noexcept override;


    NOINLINE ChorusData( int id_, ENVPresetDef*const def_ );
    NOINLINE ~ChorusData();

    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ChorusData )
    MONO_NOT_MOVE_COPY_OPERATOR( ChorusData )
    JUCE_LEAK_DETECTOR( ChorusData )
};
inline const ChorusData& ChorusData::operator=( const ChorusData& other_ ) noexcept {
    COPY_FROM_OTHER( modulation )
    COPY_FROM_OTHER( hold_modulation )

    return *this;
}

// ==============================================================================
// ==============================================================================
// ==============================================================================
// TODO LFO SNAP & SNAP DURATION
//==============================================================================
struct SynthData : mono_ParameterListener<float>
{
    const int id;

    mono_Parameter< float, 900, 0,1000, 1000, 1000 > volume;
    mono_Parameter< float, 50, 0,1000, 1000, 1000 > glide;
    mono_Parameter< float, 0, 0,1000, 1000, 1000 > delay;
    mono_Parameter< float, 1000, 0,1000, 1000, 1000 > effect_bypass;
    mono_Parameter< float, 700, 0,1000, 1000, 100 > final_compression;
    // TODO GLIDE?
    mono_Parameter< float, 900, 0,1000, 1000, 100 > colour;
    mono_Parameter< float, 50, 0,1000, 1000, 100 > resonance;
    mono_Parameter< float, 500, 0,1000, 1000, 100 > curve_shape;

    mono_Parameter< bool, true > sync;
    typedef mono_Parameter< float, 128,20,1000, 1, 100 > speed_t;
    speed_t speed;

    mono_Parameter< int, 500, 1,20000 > glide_motor_time;
    mono_Parameter< int, 500, 1,20000 > velocity_glide_time;

    mono_Parameter< bool, false > ctrl;
    mono_Parameter< float, 200, 0,1000, 1000, 100 > midi_pickup_offset;

    OwnedArray< LFOData > lfo_datas;
    OwnedArray< OSCData > osc_datas;
    OwnedArray< ENVPresetData > filter_input_env_datas;
    ScopedPointer< ENVPresetDef > env_preset_def;
    OwnedArray< FilterData > filter_datas;
    ScopedPointer< EQData > eq_data;
    OwnedArray< ENVData > env_datas;
    ScopedPointer< ArpSequencerData > arp_sequencer_data;
    ScopedPointer< ReverbData > reverb_data;
    ScopedPointer< ChorusData > chorus_data;

public:
    inline const SynthData& operator= ( const SynthData& other_ ) noexcept;

    // ==============================================================================
private:
    Array< mono_ParameterCompatibilityBase* > saveable_parameters;
    NOINLINE void get_saveable_params( Array< mono_ParameterCompatibilityBase* >& params_ ) noexcept;
    NOINLINE void colect_saveable_parameters() noexcept;
public:
    Array< mono_ParameterCompatibilityBase* >& get_atomateable_parameters() noexcept {
        return saveable_parameters;
    }

public:
    NOINLINE void save_to( XmlElement* xml ) const noexcept;
    NOINLINE void read_from( const XmlElement* xml ) noexcept;
    NOINLINE void save_midi() const noexcept;
    NOINLINE void read_midi() noexcept;

    // ==============================================================================
private:
    // TODO, load DATA
    // TODO, load the mixer states

    // ONLY THE MASTER HAS MORPHE SORCES - OTHERWISE WE BUILD UNLIMITED SOURCES FOR SOURCE
    // BETTER SOLUTION IS TO CREATE A OWN CLASS FOR THE AND INIT IT WITH THE MASTER
    OwnedArray< SynthData > left_morph_datas;
    OwnedArray< SynthData > right_morph_datas;

    // CONTAINS THE IDS OF THE MORPH SOURCES
public:
    enum MORPH_SELCTIONS_IDS {
        MAIN = 0,
        OSC_1,
        OSC_2,
        OSC_3,
        FM,
        FILTER_1,
        FILTER_2,
        FILTER_3,
        EQ,
        FX,
        ARP_TUNE,
        ARP_VELOCITY,
        ARP_GLIDE_SHUFFLE,
        ARP_SWITCHS,

        // TO HAVE AN INDEX FROM 0 to this line
        ALL, // <- MAX INDEX FOR ACTIVE SELECTION IN THE SynthData::MORPH FUNCTION
        OSCS, FILTERS, ARP,

        SUM
    };

private:
    class MorpherSelection {
    public:
        Array< int > active_morph_selections;

        void activate( MORPH_SELCTIONS_IDS id_, OwnedArray< MorpherSelection >& peers_ );

    private:
        void clean_header_selections();
    };
    OwnedArray< MorpherSelection > morpher_selections;

public:
    void activate_morph_selection( int morpher_id_, MORPH_SELCTIONS_IDS id_, bool run_sync_morph_ = true );
    const Array< int >& get_active_morph_selections( int morpher_id_ ) const;

private:
    class MorphGroup
        : public Timer,
          mono_ParameterListener< float >,
          mono_ParameterListener< bool >,
          mono_ParameterListener< int >
    {
        int id;

        MorphGroup* left_morph_group;
        MorphGroup* right_morph_group;

        Array< mono_ParameterBase< float >* > params;
        float last_power_of_right;
        Array< mono_ParameterBase< bool >* > switch_bool_params;
        bool current_switch;
        Array< mono_ParameterBase< int >* > switch_int_params;
    public:
        NOINLINE void set_sources( SynthData* left_source_,
                                   SynthData* right_source_,
                                   float current_morph_amount_,
                                   bool current_switch_state_ ) noexcept;
        inline void morph( float morph_amount_ ) noexcept;
        inline void morph_switchs( bool left_right_ ) noexcept;

        Array< float > sync_param_deltas;
        Array< float > sync_modulation_deltas;
        void run_sync_morph() noexcept;
        int current_callbacks;
        void timerCallback() override;

    public:
        NOINLINE MorphGroup();
        NOINLINE void set_id( int id_ );
        NOINLINE void register_parameter( mono_ParameterBase< float >*const param_, bool is_master_ );
        NOINLINE void register_switch_parameter( mono_ParameterBase< bool >*const param_, bool is_master_ );
        NOINLINE void register_switch_parameter( mono_ParameterBase< int >*const param_, bool is_master_ );

    private:
        void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
        void parameter_modulation_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;
        void parameter_value_changed( mono_ParameterBase< bool >* param_ ) noexcept override;
        void parameter_value_changed( mono_ParameterBase< int >* param_ ) noexcept override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphGroup)
    }
    morph_group_osc_1,
    morph_group_osc_2,
    morph_group_osc_3,
    morph_group_filter_1,
    morph_group_filter_2,
    morph_group_filter_3,
    morph_group_fm,
    morph_group_eq,
    morph_group_fx,
    morph_group_main,
    morph_group_arp_tune,
    morph_group_arp_velocity,
    morph_group_arp_glide_shuffle,
    morph_group_arp_switchs;

    Array< MorphGroup* > morph_groups;
    Array< Array< MorphGroup* > > morph_groups_per_morpher;
    void parameter_value_changed( mono_ParameterBase< float >* param_ ) noexcept override;

    NOINLINE void init_morph_groups( DATA_TYPES data_type ) noexcept;
    inline const MorphGroup& get_morph_group( int id_ ) const noexcept;
    inline MorphGroup& get_morph_group( int id_ ) noexcept;
    inline void run_sync_morph() noexcept;

public:
    // CHANGE THE STATE TO MORPH
    typedef mono_Parameter< float, 0, 0,1000, 1000, 100 > morhp_state_t;
    mono_ParameterArray< morhp_state_t, SUM_MORPHER_GROUPS > morhp_states;
    typedef mono_Parameter< bool, LEFT, LEFT,RIGHT > morhp_switch_state_t;
    mono_ParameterArray< morhp_switch_state_t, SUM_MORPHER_GROUPS > morhp_switch_states;
    mono_Parameter< float, 0, 0,3000, 1000, 100 > linear_morhp_state;
    mono_Parameter< int, 1000, 1,20000 > morph_motor_time;

    bool try_to_load_programm_to_left_side( int morpher_id_, int bank_id_, int index_ ) noexcept;
    bool try_to_load_programm_to_right_side( int morpher_id_, int bank_id_, int index_ ) noexcept;
    void update_left_morph_source( int morpher_id_ ) noexcept;
    void update_right_morph_source( int morpher_id_ ) noexcept;
    void morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ = false ) noexcept;
    float get_morph_state( int morpher_id_ ) const noexcept;
    void morph_switch_buttons( int morpher_id_, bool do_switch_ = true ) noexcept;
    bool get_morph_switch_state( int morpher_id_ ) const noexcept;

    // ==============================================================================

public:
    NOINLINE SynthData( DATA_TYPES data_type );
    NOINLINE ~SynthData();

    mono_Parameter< bool, true > animate_input_env;
    mono_Parameter< bool, true > animate_eq_env;
    mono_Parameter< bool, true > animate_modulations;
private:
    StringArray banks;
    Array< StringArray > program_names;

    int current_program;
    int current_program_abs;
    void calc_current_program_abs() noexcept;
    int current_bank;

public:
    NOINLINE static void refresh_banks_and_programms();
    NOINLINE static void update_banks( StringArray& );
    NOINLINE static void update_bank_programms( int bank_id_, StringArray& program_names_ );

    NOINLINE const StringArray& get_banks();
    NOINLINE const StringArray& get_programms( int bank_id_ );

    int get_current_programm_id_abs() const;
    const String& get_current_program_name_abs() const noexcept;
    const String& get_program_name_abs(int id_) const noexcept;

    NOINLINE void set_current_bank( int bank_index_ );
    NOINLINE void set_current_program( int programm_index_ );
    NOINLINE void set_current_program_abs( int programm_index_ );

    NOINLINE int get_current_bank() const;
    NOINLINE int get_current_program() const;
    NOINLINE const StringArray& get_current_bank_programms();

    NOINLINE bool rename( const String& new_name_ );
    NOINLINE bool load( bool load_morph_groups = true );
    NOINLINE bool load( const String& bank_name_, const String& program_name_, bool load_morph_groups = true );
    NOINLINE bool load_prev();
    NOINLINE bool load_next();
    NOINLINE bool replace();
    NOINLINE bool create_new();
    NOINLINE bool remove();

    NOINLINE void save_session();
    NOINLINE void load_session();

private:
    NOINLINE bool write2file( const String& bank_name_, const String& program_name_ );

private:
    MONO_NOT_CTOR_COPYABLE( SynthData )
    MONO_NOT_MOVE_COPY_OPERATOR( SynthData )
    JUCE_LEAK_DETECTOR( SynthData )
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ENV;
class MONOVoice;
class DataBuffer;
class mono_ParameterOwnerStore : public DeletedAtShutdown {

public:
    RuntimeInfo* runtime_info;
    
    DataBuffer* data_buffer;

    Array< LFOData* > lfo_datas;
    Array< OSCData* > osc_datas;
    Array< ENVPresetData* > filter_input_env_datas;
    Array< FilterData* > filter_datas;
    ENVPresetDef* env_preset_def;
    EQData* eq_data;
    Array< ENVData* > env_datas;
    ArpSequencerData* arp_data;
    ReverbData* reverb_data;
    ChorusData* chorus_data;
    SynthData* synth_data;

    ENVPresetData* ui_env_preset_data;
    ENV* ui_env;

    MONOVoice* voice;

    static void get_full_adsr( float state_, Array< float >& curve, int& sustain_start_, int& sustain_end_ );
    static float get_flt_input_env_amp( int flt_id_, int input_id_ );
    static float get_band_env_amp( int band_id_ );
    static float get_chorus_modulation_env_amp();

    NOINLINE mono_ParameterOwnerStore();
    NOINLINE ~mono_ParameterOwnerStore();

    juce_DeclareSingleton (mono_ParameterOwnerStore,false)
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ParameterOwnerStore)
};

struct DataBuffer { // DEFINITION IN SYNTH.CPP
    int current_buffer_size;
    
    // WORKERS
    mono_AudioSampleBuffer<6> tmp_buffer_6;
    mono_AudioSampleBuffer<9*4> tmp_multithread_band_buffer_9_4;
    
    // OVER MULTIBLE PROCESSORS
    mono_AudioSampleBuffer<SUM_FILTERS> lfo_amplitudes;
    mono_AudioSampleBuffer<SUM_FILTERS> direct_filter_output_samples;

    mono_AudioSampleBuffer<SUM_OSCS> osc_samples;
    // TODO do we need only one?
    mono_AudioSampleBuffer<SUM_OSCS> osc_switchs;
    mono_AudioSampleBuffer<1> osc_sync_switchs;
    mono_AudioSampleBuffer<1> modulator_samples;
    
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_output_samples;
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_amps;
    
    void resize_buffer_if_required( int min_size_required_ ) noexcept;

    NOINLINE DataBuffer( int init_buffer_size_ );

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataBuffer)
};

#define SYNTH_PARAM(_X_) &(mono_ParameterOwnerStore::getInstance()->_X_)
#define DATA(_X_) (*mono_ParameterOwnerStore::getInstance()->_X_)


#pragma GCC diagnostic pop

#endif


