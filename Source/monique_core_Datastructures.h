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
    NOINLINE void resize_buffer_if_required( int size_ ) noexcept;

public:
    // ==============================================================================
    NOINLINE DataBuffer( int init_buffer_size_ ) noexcept;
    NOINLINE ~DataBuffer() noexcept;

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
    NOINLINE virtual void set_sample_rate( double sr_ ) noexcept;
    NOINLINE virtual void set_block_size( int bs_ ) noexcept;
    NOINLINE virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept;
    NOINLINE virtual void block_size_changed() noexcept;

protected:
    //==========================================================================
    NOINLINE RuntimeListener() noexcept;
    NOINLINE ~RuntimeListener() noexcept;

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

    NOINLINE RuntimeNotifyer() noexcept;
    NOINLINE ~RuntimeNotifyer() noexcept;
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
    int clock_counter;

    Array<int64> next_step_at_sample;
    Array<int> next_step;
#endif

private:
    //==========================================================================
    friend class MoniqueSynthesiserVoice;
    NOINLINE RuntimeInfo() noexcept;
    NOINLINE ~RuntimeInfo() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeInfo)
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
struct LFOData
{
    Parameter speed;

    //==========================================================================
    NOINLINE LFOData( int id_ ) noexcept;
    NOINLINE ~LFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOData)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct OSCData
{
    Parameter wave;
    ModulatedParameter octave;
    BoolParameter is_lfo_modulated;

    Parameter fm_multi;
    Parameter fm_amount;

    BoolParameter fm_wave; // TODO RENAME!
    BoolParameter sync;
    BoolParameter mod_off;

    IntParameter puls_width;
    Parameter fm_swing;
    IntParameter osc_switch;

    // FOR UI FEEDBACK
    float last_modulation_value;

    //==========================================================================
    NOINLINE OSCData( int id_ ) noexcept;
    NOINLINE ~OSCData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCData)
};

//==============================================================================
//==============================================================================
//==============================================================================
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

    //==========================================================================
    NOINLINE ENVData( int id_ ) noexcept;
    NOINLINE virtual ~ENVData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVData)
};

//==============================================================================
//==============================================================================
//==============================================================================
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
    NOINLINE ENVPresetDef( int id_ ) noexcept;
    NOINLINE ~ENVPresetDef() noexcept;

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
    NOINLINE ENVPresetData( int id_, ENVPresetDef* def_ ) noexcept;
    NOINLINE ~ENVPresetData() noexcept;

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

    ModulatedParameter width;
    BoolParameter modulate_width;

    ModulatedParameter gain;
    BoolParameter modulate_gain;


    Parameter compressor;
    ModulatedParameter output;
    Parameter output_clipping;
    BoolParameter modulate_output;

    ArrayOfParameters input_sustains;
    ArrayOfBoolParameters input_holds;
    OwnedArray<ENVPresetData> input_env_datas;

    ENVData*const env_data;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    NOINLINE FilterData( int id_, ENVPresetDef* env_preset_def_ ) noexcept;
    NOINLINE ~FilterData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( FilterData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ArpSequencerData
{
    BoolParameter is_on;

    ArrayOfBoolParameters step;
    ArrayOfParameters tune;
    ArrayOfParameters velocity;

    Parameter shuffle;
    BoolParameter connect;
    IntParameter speed_multi;

    //==========================================================================
    NOINLINE ArpSequencerData( int id_ ) noexcept;
    NOINLINE ~ArpSequencerData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ArpSequencerData )

    //==========================================================================
    static StringRef speed_multi_to_text( int speed_multi_ ) noexcept;
    static double speed_multi_to_value( int speed_multi_ ) noexcept;
};

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

    OwnedArray< ENVPresetData > env_datas;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    NOINLINE EQData( int id_, ENVPresetDef*const def_ ) noexcept;
    NOINLINE ~EQData() noexcept;

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
    NOINLINE ReverbData( int id_ ) noexcept;
    NOINLINE ~ReverbData() noexcept;

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
    ENVPresetData*const shine_env_data;

private:
    //==========================================================================
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* param_ ) noexcept override;
    void parameter_value_on_load_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    NOINLINE ChorusData( int id_, ENVPresetDef*const def_ ) noexcept;
    NOINLINE ~ChorusData() noexcept;

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
#define THREAD_LIMIT 4
class MorphGroup;
struct SynthData : ParameterListener
{
    const int id;

    Parameter volume;
    Parameter glide;
    Parameter delay;
    Parameter effect_bypass;
    Parameter final_compression;
    Parameter colour;
    Parameter resonance;
    Parameter curve_shape; 	// TODO RENAME ENV_CURVE_SHAPE
    IntParameter octave_offset;

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
    BoolParameter animate_input_env;
    BoolParameter animate_eq_env;
    BoolParameter animate_modulations;

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
    NOINLINE void colect_saveable_parameters() noexcept;

public:
    // TODO
    inline Array< Parameter* >& get_atomateable_parameters() noexcept
    {
        return saveable_parameters;
    }

    // ==============================================================================
    NOINLINE SynthData( DATA_TYPES data_type ) noexcept;
    NOINLINE ~SynthData() noexcept;

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // MORPH
public:
    ArrayOfParameters morhp_states;
    ArrayOfBoolParameters morhp_switch_states;
    Parameter linear_morhp_state;
    IntParameter morph_motor_time;

private:
    ScopedPointer<MorphGroup> morph_group_1, morph_group_2, morph_group_3, morph_group_4;
    OwnedArray< SynthData > left_morph_sources;
    OwnedArray< SynthData > right_morph_sources;

    NOINLINE void init_morph_groups( DATA_TYPES data_type ) noexcept;

public:
    inline float get_morph_state( int morpher_id_ ) const noexcept;
    inline bool get_morph_switch_state( int morpher_id_ ) const noexcept;
    inline void morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ = false ) noexcept;
    inline void morph_switch_buttons( int morpher_id_, bool do_switch_ = true ) noexcept;
    inline void run_sync_morph() noexcept;

private:
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    
public:
    // COPY THE CURRENT STATE TO THE SOURCES
    void set_morph_source_data_from_current( int morpher_id_, bool left_or_right_ ) noexcept;
    bool try_to_load_programm_to_left_side( int morpher_id_, int bank_id_, int index_ ) noexcept;
    bool try_to_load_programm_to_right_side( int morpher_id_, int bank_id_, int index_ ) noexcept;

    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // FILE IO

    
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












public:
    // ==============================================================================
    NOINLINE void save_to( XmlElement* xml ) const noexcept;
    NOINLINE void read_from( const XmlElement* xml ) noexcept;
    NOINLINE void save_midi() const noexcept;
    NOINLINE void read_midi() noexcept;
};

// ==============================================================================
// ==============================================================================
// ==============================================================================
class ENV;
class MoniqueSynthesiserVoice;
class DataBuffer;
class mono_ParameterOwnerStore : public DeletedAtShutdown {

public:
    RuntimeInfo* runtime_info;

    DataBuffer* data_buffer;

    Array< LFOData* > lfo_datas;
    Array< OSCData* > osc_datas;
    Array< FilterData* > filter_datas;
    ENVPresetDef* env_preset_def;
    EQData* eq_data;
    ArpSequencerData* arp_data;
    ReverbData* reverb_data;
    ChorusData* chorus_data;
    SynthData* synth_data;

    ENVPresetData* ui_env_preset_data;
    ENV* ui_env;

    MoniqueSynthesiserVoice* voice;

    static void get_full_adsr( float state_, Array< float >& curve, int& sustain_start_, int& sustain_end_ );
    static float get_flt_input_env_amp( int flt_id_, int input_id_ );
    static float get_band_env_amp( int band_id_ );
    static float get_chorus_modulation_env_amp();

    NOINLINE mono_ParameterOwnerStore();
    NOINLINE ~mono_ParameterOwnerStore();

    juce_DeclareSingleton (mono_ParameterOwnerStore,false)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ParameterOwnerStore)
};



#endif



