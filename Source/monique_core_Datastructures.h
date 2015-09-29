#ifndef MONOSYNTH_DATA_H_INCLUDED
#define MONOSYNTH_DATA_H_INCLUDED

#include "App_h_includer.h"

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
enum DATA_TYPES
{
    MORPH = 1,
    MASTER,

    DATA_COPY,

    FILTER_INPUT_ENV_ID_OFFSET = 100,
    EQ_ENV_ID_OFFSET = 200,
    CHORUS_ENV_ID_OFFSET = 300,
};

//==============================================================================
enum WAVE_TYPES
{
    SINE,
    SAW,
    SQUARE,
    NOICE
};

//==============================================================================
enum FILTER_TYPS
{
    LPF_2_PASS = 1,
    HPF,
    BPF,
    HIGH_2_PASS,
    PASS,
    LPF,
    MOOG_AND_LPF, // TODO OBSOLETE BUT USED IN SOME OLD PROJECTS
    UNKNOWN
};

//==============================================================================
enum MONIQUE_SETUP
{
    SUM_OSCS = 3,
    SUM_FILTERS = 3,
    SUM_INPUTS_PER_FILTER = SUM_OSCS,
    SUM_LFOS = SUM_FILTERS,
    SUM_ENVS = SUM_FILTERS + 1,
    MAIN_ENV = 3,

    LEFT = 0,
    RIGHT = 1,

    SUM_ENV_ARP_STEPS = 16,

    OSC_1 = 0,
    MASTER_OSC = OSC_1,
    OSC_2 = 1,
    OSC_3 = 2,

    FILTER_1 = 0,
    FILTER_2 = 1,
    FILTER_3 = 2,

    SUM_MORPHER_GROUPS = 4,
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
class MoniqueAudioProcessor;
class DataBuffer // DEFINITION IN SYNTH.CPP
{
    int size;

public:
    // ==============================================================================
    // WORKERS
    mono_AudioSampleBuffer<9*4+2> tmp_multithread_band_buffer_9_4;
    mono_AudioSampleBuffer<SUM_FILTERS> lfo_amplitudes;
    mono_AudioSampleBuffer<SUM_FILTERS> direct_filter_output_samples;

    mono_AudioSampleBuffer<SUM_OSCS> osc_samples;
    mono_AudioSampleBuffer<SUM_OSCS> osc_switchs;
    mono_AudioSampleBuffer<1> osc_sync_switchs;
    mono_AudioSampleBuffer<1> modulator_samples;

    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_output_samples;
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_amps;

private:
    // ==============================================================================
    friend class MoniqueAudioProcessor;
    COLD void resize_buffer_if_required( int size_ ) noexcept;

public:
    // ==============================================================================
    COLD DataBuffer( int init_buffer_size_ ) noexcept;
    COLD ~DataBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataBuffer)
};

#define GET_DATA(_X_) (*mono_ParameterOwnerStore::getInstance()->_X_)
#define GET_DATA_PTR(_X_) mono_ParameterOwnerStore::getInstance()->_X_

//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer;
class RuntimeListener
{
protected:
    //==========================================================================
    double sample_rate;
    float sample_rate_1ths;
    int block_size;

private:
    //==========================================================================
    friend class RuntimeNotifyer;
    COLD virtual void set_sample_rate( double sr_ ) noexcept;
    COLD virtual void set_block_size( int bs_ ) noexcept;
    COLD virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept;
    COLD virtual void block_size_changed() noexcept;

protected:
    //==========================================================================
    COLD RuntimeListener() noexcept;
    COLD ~RuntimeListener() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeListener)
};

//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer : public DeletedAtShutdown
{
    //==========================================================================
    friend class RuntimeListener;
    Array<RuntimeListener*> listeners;

    //==========================================================================
    double sample_rate;
    float sample_rate_1ths;
    int block_size;

public:
    //==========================================================================
    void set_sample_rate( double sr_ ) noexcept;
    void set_block_size( int bs_ ) noexcept;

    double get_sample_rate() const noexcept;
    int get_block_size() const noexcept;

public:
    //==========================================================================
    juce_DeclareSingleton (RuntimeNotifyer,false)

    COLD RuntimeNotifyer() noexcept;
    COLD ~RuntimeNotifyer() noexcept;
};

//==============================================================================
//==============================================================================
//==============================================================================

struct RuntimeInfo
{
    int64 samples_since_start;
    double bpm;

#ifdef IS_STANDALONE
    bool is_extern_synced;
    bool is_running;

    class ClockCounter
    {
        int clock_counter;
        int clock_counter_absolut;

    public:
        inline void operator++(int) noexcept
        {
            if( ++clock_counter >= 96 )
            {
                clock_counter = 0;
            }
            if( ++clock_counter_absolut >= 96*16 )
            {
                clock_counter_absolut = 0;
            }
        }
        inline int clock() noexcept
        {
            return clock_counter;
        }
        inline int clock_absolut() noexcept
        {
            return clock_counter_absolut;
        }
        inline void reset() noexcept
        {
            clock_counter = 0;
            clock_counter_absolut = clock_counter;
        }

        COLD ClockCounter() : clock_counter(0), clock_counter_absolut(0) {}
    } clock_counter;

    struct Step
    {
        const int step_id;
        const int64 at_absolute_sample;
        const int samples_per_step;

        inline Step( int step_id_, int64 at_absolute_sample_, int64 samples_per_step_ ) noexcept;
        inline ~Step() noexcept;
    };
    OwnedArray<Step> steps_in_block;
#endif

private:
    //==========================================================================
    friend class MoniqueSynthesiserVoice;
    COLD RuntimeInfo() noexcept;
    COLD ~RuntimeInfo() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeInfo)
};

//==============================================================================
#ifdef IS_STANDALONE
inline RuntimeInfo::Step::Step( int step_id_, int64 at_absolute_sample_, int64 samples_per_step_ ) noexcept
:
step_id( step_id_ ),
         at_absolute_sample( at_absolute_sample_ ),
         samples_per_step( samples_per_step_ )
{}
inline RuntimeInfo::Step::~Step() noexcept {}
#endif

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
struct LFOData
{
    Parameter speed;

    //==========================================================================
    COLD LFOData( int id_ ) noexcept;
    COLD ~LFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOData)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct OSCData
{
    const int id;

    Parameter wave;
    ModulatedParameter tune;
    BoolParameter is_lfo_modulated;

    Parameter fm_freq;
    Parameter fm_amount;

    BoolParameter fm_shot;
    BoolParameter sync;
    BoolParameter o_mod;

    IntParameter puls_width;
    Parameter fm_swing;
    IntParameter osc_switch;

    // FOR UI FEEDBACK
    float last_modulation_value;

    //==========================================================================
    COLD OSCData( int id_ ) noexcept;
    COLD ~OSCData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCData)
};

//==============================================================================
//==============================================================================
//==============================================================================
#define MIN_ENV_TIMES 10
struct ENVData
{
    const int id;

    Parameter attack;
    IntParameter max_attack_time;
    Parameter decay;
    IntParameter max_decay_time;
    Parameter sustain;
    Parameter sustain_time;
    Parameter release;
    IntParameter max_release_time;
    
    Parameter shape;

    //==========================================================================
    COLD ENVData( int id_ ) noexcept;
    COLD virtual ~ENVData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVData)
};

//==============================================================================
//==============================================================================
//==============================================================================
// TODO REMOVE
struct ENVPresetDef
{
    Parameter attack_1;
    Parameter decay_1;
    Parameter sustain_time_1;
    Parameter release_1;
    Parameter attack_2;
    Parameter decay_2;
    Parameter sustain_time_2;
    Parameter release_2;
    Parameter attack_3;
    Parameter decay_3;
    Parameter sustain_time_3;
    Parameter release_3;
    Parameter attack_4;
    Parameter decay_4;
    Parameter sustain_time_4;
    Parameter release_4;

    //==========================================================================
    COLD ENVPresetDef( int id_ ) noexcept;
    COLD ~ENVPresetDef() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVPresetDef)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ENVPresetData : public ENVData, ParameterListener
{
    ENVPresetDef*const def;

    Parameter state;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* ) noexcept override;
    void update_adr_values() noexcept;
    void parameter_value_on_load_changed( Parameter* ) noexcept override;

public:
    //==========================================================================
    COLD ENVPresetData( int id_, ENVPresetDef* def_ ) noexcept;
    COLD ~ENVPresetData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVPresetData)

    //==========================================================================
    static float get_attack_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_decay_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_sustain_time_at( const ENVPresetDef& def_, float state_ ) noexcept;
    static float get_release_at( const ENVPresetDef& def_, float state_ ) noexcept;
};

//==============================================================================
//==============================================================================
//==============================================================================
struct FilterData : ParameterListener
{
    IntParameter filter_type;
    Parameter adsr_lfo_mix;

    ModulatedParameter distortion;
    BoolParameter modulate_distortion;

    ModulatedParameter cutoff;
    BoolParameter modulate_cutoff;

    ModulatedParameter resonance;
    BoolParameter modulate_resonance;

    ModulatedParameter gain;
    BoolParameter modulate_gain;


    Parameter compressor;
    ModulatedParameter output;
    Parameter output_clipping;
    BoolParameter modulate_output;

    ArrayOfParameters input_sustains;
    OwnedArray<ENVData> input_envs;
    ArrayOfBoolParameters input_holds;

    ENVData*const env_data;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    COLD FilterData( int id_, ENVPresetDef* env_preset_def_ ) noexcept;
    COLD ~FilterData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( FilterData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ArpSequencerData
{
    BoolParameter is_on;

    ArrayOfBoolParameters step;
    ArrayOfIntParameters tune;
    ArrayOfParameters velocity;

    IntParameter shuffle;
    BoolParameter connect;
    IntParameter speed_multi;

    //==========================================================================
    COLD ArpSequencerData( int id_ ) noexcept;
    COLD ~ArpSequencerData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ArpSequencerData )

    //==========================================================================
    static StringRef speed_multi_to_text( int speed_multi_ ) noexcept;
    static double speed_multi_to_value( int speed_multi_ ) noexcept;

    static StringRef shuffle_to_text( int speed_multi_ ) noexcept;
    static float shuffle_to_value( int speed_multi_ ) noexcept;
};

//==========================================================================
inline double ArpSequencerData::speed_multi_to_value( int speed_multi_ ) noexcept
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
        return 6;
    case -5 :
        return (1.0/6);
    case 6 :
        return 7;
    case -6 :
        return (1.0/7);
    case 7 :
        return 8;
    case -7 :
        return (1.0/8);
    case 8 :
        return 9;
    case -8 :
        return (1.0/9);
    case 9 :
        return 10;
    case -9 :
        return (1.0/10);
    case 10 :
        return 11;
    case -10 :
        return (1.0/11);
    case 11 :
        return 12;
    case -11 :
        return (1.0/12);
    case 12 :
        return 13;
    case -12 :
        return (1.0/13);
    case 13 :
        return 14;
    case -13 :
        return (1.0/14);
    case 14 :
        return 15;
    case -14 :
        return (1.0/15);
    case 15 :
        return 16;
    default : // case -15 :
        return (1.0/16);
    }
}

//==============================================================================
inline StringRef ArpSequencerData::speed_multi_to_text( int speed_multi_ ) noexcept
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
        return "x6";
    case -5 :
        return "/6";
    case 6 :
        return "x7";
    case -6 :
        return "/7";
    case 7 :
        return "x8";
    case -7 :
        return "/8";
    case 8 :
        return "x9";
    case -8 :
        return "/9";
    case 9 :
        return "x10";
    case -9 :
        return "/10";
    case 10 :
        return "x11";
    case -10 :
        return "/11";
    case 11 :
        return "x12";
    case -11 :
        return "/12";
    case 12 :
        return "x16";
    case -12 :
        return "/13";
    case 13 :
        return "x14";
    case -13 :
        return "/14";
    case 14 :
        return "x15";
    case -14 :
        return "/15";
    case 15 :
        return "x16";
    default : // -15 :
        return "/16";
    }
}

//==============================================================================
inline float ArpSequencerData::shuffle_to_value( int suffle_ ) noexcept
{
    switch( suffle_ )
    {
    case 0 :
        return 0;
    case 1 :
        return 1.0f/128;
    case 2 :
        return 1.0f/96;
    case 3 :
        return 1.0f/64;
    case 4 :
        return 1.0f/48;
    case 5 :
        return 1.0f/32;
    case 6 :
        return 1.0f/24;
    case 7 :
        return 1.0f/16;
    case 8 :
        return 1.0f/12;
    case 9 :
        return 1.0f/8;
    case 10 :
        return 2.0f/8;
    case 11 :
        return 3.0f/8;
    case 12 :
        return 4.0f/8;
    case 13 :
        return 5.0f/8;
    case 14 :
        return 6.0f/8;
    case 15 :
        return 7.0f/8;
    case 16 :
        return 1;
    }
}

//==============================================================================
inline StringRef ArpSequencerData::shuffle_to_text( int suffle_ ) noexcept
{
    switch( suffle_ )
    {
    case 0 :
        return "OFF";
    case 1 :
        return "1/128";
    case 2 :
        return "1/96";
    case 3 :
        return "1/64";
    case 4 :
        return "1/32";
    case 5 :
        return "1/32";
    case 6 :
        return "1/24";
    case 7 :
        return "1/16";
    case 8 :
        return "1/12";
    case 9 :
        return "1/8";
    case 10 :
        return "2/8";
    case 11 :
        return "3/8";
    case 12 :
        return "4/8";
    case 13 :
        return "5/8";
    case 14 :
        return "6/8";
    case 15 :
        return "7/8";
    case 16 :
        return "1/1";
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
enum BAND_FREQUENCYS
{
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
//==============================================================================
struct EQData : ParameterListener
{
    ArrayOfParameters velocity;
    ArrayOfBoolParameters hold;

    OwnedArray<ENVData> envs;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    COLD EQData( int id_, ENVPresetDef*const def_ ) noexcept;
    COLD ~EQData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( EQData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ReverbData
{
    Parameter room;
    Parameter dry_wet_mix;
    Parameter width;

    //==========================================================================
    COLD ReverbData( int id_ ) noexcept;
    COLD ~ReverbData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ReverbData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ChorusData : ParameterListener
{
    Parameter modulation;
    BoolParameter hold_modulation;

    ENVPresetData*const modulation_env_data;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    COLD ChorusData( int id_, ENVPresetDef*const def_ ) noexcept;
    COLD ~ChorusData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ChorusData )
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
#ifdef IS_STANDALONE
#define THREAD_LIMIT 4
#else
#define THREAD_LIMIT 0
#endif

class MorphGroup;
struct MoniqueSynthData : ParameterListener
{
    const int id;

    Parameter volume;
    Parameter glide;
    Parameter delay;
    Parameter effect_bypass;
    Parameter final_compression;
    Parameter shape;
    IntParameter octave_offset;
    BoolParameter osc_retune;

    BoolParameter sync;
    Parameter speed;

    IntParameter glide_motor_time;
    IntParameter velocity_glide_time;

    BoolParameter ctrl;

    Parameter midi_pickup_offset;

    // OSCILLOSCOPE SETTINGS
    BoolParameter osci_show_osc_1;
    BoolParameter osci_show_osc_2;
    BoolParameter osci_show_osc_3;
    BoolParameter osci_show_flt_env_1;
    BoolParameter osci_show_flt_env_2;
    BoolParameter osci_show_flt_env_3;
    BoolParameter osci_show_flt_1;
    BoolParameter osci_show_flt_2;
    BoolParameter osci_show_flt_3;
    BoolParameter osci_show_eq;
    BoolParameter osci_show_out;
    BoolParameter osci_show_out_env;
    Parameter osci_show_range;

    // MULTITHREADING
    IntParameter num_extra_threads;

    // SETTINGS
    BoolParameter animate_envs;
    BoolParameter show_tooltips;
    BoolParameter bind_sustain_and_sostenuto_pedal;
    BoolParameter sliders_in_rotary_mode;
    IntParameter sliders_sensitivity;
    Parameter ui_scale_factor;

    ScopedPointer< ENVData > env_data;

    OwnedArray< LFOData > lfo_datas;
    OwnedArray< OSCData > osc_datas;
    ScopedPointer< ENVPresetDef > env_preset_def;
    OwnedArray< FilterData > filter_datas;
    ScopedPointer< EQData > eq_data;
    ScopedPointer< ArpSequencerData > arp_sequencer_data;
    ScopedPointer< ReverbData > reverb_data;
    ScopedPointer< ChorusData > chorus_data;

private:
    // ==============================================================================
    Array< Parameter* > saveable_parameters;
    Array< float > saveable_backups;
    Array< Parameter* > global_parameters;
    Array< Parameter* > all_parameters;
    COLD void colect_saveable_parameters() noexcept;
    COLD void colect_global_parameters() noexcept;

public:
    // TODO
    inline Array< Parameter* >& get_atomateable_parameters() noexcept
    {
        return saveable_parameters;
    }
    inline Array< Parameter* >& get_global_parameters() noexcept
    {
        return global_parameters;
    }
    inline Array< Parameter* >& get_all_parameters() noexcept
    {
        return all_parameters;
    }

    // ==============================================================================
    COLD MoniqueSynthData( DATA_TYPES data_type ) noexcept;
    COLD ~MoniqueSynthData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( MoniqueSynthData )

public:
    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // MORPH
    ArrayOfParameters morhp_states;
    ArrayOfBoolParameters morhp_switch_states;
    Parameter linear_morhp_state;
    IntParameter morph_motor_time;

private:
    ScopedPointer<MorphGroup> morph_group_1, morph_group_2, morph_group_3, morph_group_4;
    OwnedArray< MoniqueSynthData > left_morph_sources;
    OwnedArray< MoniqueSynthData > right_morph_sources;
    StringArray left_morph_source_names;
    StringArray right_morph_source_names;
public:
    const String& get_morph_source_name( int id_abs_ ) const noexcept;
private:

    COLD void init_morph_groups( DATA_TYPES data_type ) noexcept;

    CriticalSection morph_lock;

public:
    inline float get_morph_state( int morpher_id_ ) const noexcept;
    inline bool get_morph_switch_state( int morpher_id_ ) const noexcept;
    inline void morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ = false ) noexcept;
    inline void morph_switch_buttons( int morpher_id_, bool do_switch_ = true ) noexcept;
    inline void run_sync_morph() noexcept;

private:
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_by_automation( Parameter* param_ ) noexcept override;

public:
    // COPY THE CURRENT STATE TO THE SOURCES
    void set_morph_source_data_from_current( int morpher_id_, bool left_or_right_ ) noexcept;
    bool try_to_load_programm_to_left_side( int morpher_id_, int bank_id_, int index_ ) noexcept;
    bool try_to_load_programm_to_right_side( int morpher_id_, int bank_id_, int index_ ) noexcept;

private:
    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // FILE IO
    StringArray banks;
    Array< StringArray > program_names_per_bank;
    String last_program;
    String last_bank;

    int current_program;
    int current_program_abs;
    int current_bank;

public:
    // ==============================================================================
    static void refresh_banks_and_programms() noexcept;
private:
    void calc_current_program_abs() noexcept;

    static void update_banks( StringArray& ) noexcept;
    static void update_bank_programms( int bank_id_, StringArray& program_names_ ) noexcept;

public:
    // ==============================================================================
    const StringArray& get_banks() noexcept;
    const StringArray& get_programms( int bank_id_ ) noexcept;

    // ==============================================================================
    void set_current_bank( int bank_index_ ) noexcept;
    void set_current_program( int programm_index_ ) noexcept;
    void set_current_program_abs( int programm_index_ ) noexcept;

    int get_current_bank() const noexcept;
    int get_current_program() const noexcept;
    const StringArray& get_current_bank_programms() const noexcept;

    const String error_string;
    int get_current_programm_id_abs() const noexcept;
    const String& get_current_program_name_abs() const noexcept;
    const String& get_program_name_abs(int id_) const noexcept;

    // ==============================================================================
    void create_internal_backup() noexcept;
    bool create_new() noexcept;
    bool rename( const String& new_name_ ) noexcept;
    bool replace() noexcept;
    bool remove() noexcept;

    bool load( bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;
    bool load_prev() noexcept;
    bool load_next() noexcept;
private:
    bool load( const String& bank_name_, const String& program_name_, bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;

public:
    // ==============================================================================
    void save_to( XmlElement* xml ) noexcept;
    void read_from( const XmlElement* xml ) noexcept;

private:
    bool write2file( const String& bank_name_, const String& program_name_ ) noexcept;

public:
    void save_settings() const noexcept;
    void ask_and_save_if_changed() noexcept;
    void load_settings() noexcept;

public:
    // ==============================================================================
    void save_midi() const noexcept;
    void read_midi() noexcept;
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
// NOTE: IMPL IN SYNTH.CPP
class ENV;
class MoniqueSynthesiserVoice;
class DataBuffer;
class mono_ParameterOwnerStore : public DeletedAtShutdown
{
public:
    RuntimeInfo* runtime_info;

    DataBuffer* data_buffer;

    ENVPresetDef* env_preset_def;

    Array< LFOData* > lfo_datas;
    Array< OSCData* > osc_datas;
    Array< FilterData* > filter_datas;
    EQData* eq_data;
    ArpSequencerData* arp_data;
    ReverbData* reverb_data;
    ChorusData* chorus_data;
    MoniqueSynthData* synth_data;

    MoniqueSynthesiserVoice* voice;

    // ==============================================================================
    ENVPresetDef* ui_env_preset_def;
    ENVPresetData* ui_env_preset_data;
    ENV* ui_env;

    // ==============================================================================
    static void init_ui_env() noexcept;
    static void get_full_adsr( float state_, Array< float >& curve, int& sustain_start_, int& sustain_end_ ) noexcept;
    static void get_full_adstr( ENVData&env_data_,Array< float >& curve ) noexcept;

    // ==============================================================================
    COLD mono_ParameterOwnerStore() noexcept;
    COLD ~mono_ParameterOwnerStore() noexcept;

    juce_DeclareSingleton (mono_ParameterOwnerStore,false)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ParameterOwnerStore)
};

#endif
