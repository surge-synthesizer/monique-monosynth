#ifndef MONOSYNTH_DATA_H_INCLUDED
#define MONOSYNTH_DATA_H_INCLUDED

#include "App.h"

#define FACTORY_NAME "FACTORY DEFAULT (SCRATCH)"

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
    MORPH_LEFT,
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

    SUM_EQ_BANDS = 7,
#ifdef POLY
    MAX_PLAYBACK_NOTES = 3
#else
	MAX_PLAYBACK_NOTES = 1
#endif
};

//==============================================================================

enum PLAY_MODES
{
    LIFO = 0,
    FIFO,
    HIGH,
    LOW,

    PLAY_MODES_SIZE
};
#ifdef POLY
enum TRACKING_MODES
{
    LOW_FIRST = 0,
    HIGH_FIRST,
    LOW_MIDDLE,
    HIGH_MIDDLE,
    
    TRACKING_MODES_SIZE
};
#endif

#define MIN_CUTOFF 35.0f
#define MAX_CUTOFF 21965.0f

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
class mono_AudioDeviceManager;
class DataBuffer // DEFINITION IN SYNTH.CPP
{
    int size;

public:
    // ==============================================================================
    // WORKERS
    // TODO REDUCE TO NEEDED
    mono_AudioSampleBuffer<SUM_EQ_BANDS> band_env_buffers;
    mono_AudioSampleBuffer<SUM_EQ_BANDS> band_out_buffers;

    mono_AudioSampleBuffer<SUM_FILTERS> lfo_amplitudes;
    mono_AudioSampleBuffer<SUM_MORPHER_GROUPS> mfo_amplitudes;
    mono_AudioSampleBuffer<SUM_FILTERS*2> filter_output_samples_l_r;
    mono_AudioSampleBuffer<2> filter_stereo_output_samples;

    mono_AudioSampleBuffer<SUM_OSCS> osc_samples;
    mono_AudioSampleBuffer<1> osc_switchs;
    mono_AudioSampleBuffer<1> osc_sync_switchs;
    mono_AudioSampleBuffer<1> modulator_samples;

    mono_AudioSampleBuffer<1> final_env;
#ifdef POLY
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_tracking;
#endif
    mono_AudioSampleBuffer<1> chorus_env;

    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_samples;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_env_amps;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_output_samples;
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_amps;

    mono_AudioSampleBuffer<1> tmp_buffer;
    mono_AudioSampleBuffer<1> second_mono_buffer;

    mono_AudioSampleBuffer<1> velocity_buffer;

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

//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer;
class RuntimeListener
{
    RuntimeNotifyer*const notifyer;
protected:
    //==========================================================================
    double sample_rate;
    int block_size;

private:
    //==========================================================================
    friend class RuntimeNotifyer;
    COLD virtual void set_sample_rate( double sr_ ) noexcept;
public:
    inline double get_sample_rate() const noexcept
    {
        return sample_rate;
    }
private:
    COLD virtual void set_block_size( int bs_ ) noexcept;
    COLD virtual void sample_rate_or_block_changed() noexcept = 0;

protected:
    //==========================================================================
    COLD RuntimeListener( RuntimeNotifyer*const notifyer_ ) noexcept;
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
    int block_size;

public:
    //==========================================================================
    void set_sample_rate( double sr_ ) noexcept;
    void set_block_size( int bs_ ) noexcept;

    double get_sample_rate() const noexcept
    {
        return sample_rate;
    }
    int get_block_size() const noexcept
    {
        return block_size;
    }

private:
    //==========================================================================
    friend class MoniqueAudioProcessor;
    friend class mono_AudioDeviceManager;
    friend class ContainerDeletePolicy< RuntimeNotifyer >;
    COLD RuntimeNotifyer() noexcept;
    COLD ~RuntimeNotifyer() noexcept;
};

//==============================================================================
//==============================================================================
//==============================================================================
class Step
{
public:
    const int step_id;
    const int64 at_absolute_sample;
    const int samples_per_step;

    inline Step( int step_id_, int64 at_absolute_sample_, int64 samples_per_step_ ) noexcept
:
    step_id( step_id_ ),
             at_absolute_sample( at_absolute_sample_ ),
             samples_per_step( samples_per_step_ )
    {}
    inline ~Step() noexcept {}
};
struct RuntimeInfo
{
    int64 samples_since_start;
    int64 relative_samples_since_start;
    double bpm;
    double steps_per_sample;

    struct standalone_features
    {
        bool is_extern_synced = false;
        bool is_running = false;

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
            inline int clock() const noexcept
            {
                return clock_counter;
            }
            inline int clock_absolut() const noexcept
            {
                return clock_counter_absolut;
            }
            inline int is_step() const noexcept
            {
                return clock_counter_absolut%(96/16)==0;
            }
            inline void reset() noexcept
            {
                clock_counter = 0;
                clock_counter_absolut = clock_counter;
            }

            ClockCounter() : clock_counter(0), clock_counter_absolut(0) {}
        } clock_counter;

        struct ClockSync
        {
            struct SyncPosPair
            {
                const int pos_in_buffer;
                const int samples_per_clock;

                SyncPosPair(int pos_in_buffer_, int samples_per_clock_) noexcept
                        :
                        pos_in_buffer(pos_in_buffer_),
                        samples_per_clock(samples_per_clock_)
                {}
                ~SyncPosPair() noexcept {}
            };

        private:
            Array< SyncPosPair > clock_informations;
            Array< SyncPosPair > clock_informations_for_current_process_block;

            int last_samples_per_clock;

        public:
            int get_samples_per_clock( int pos_in_buffer_ ) const noexcept
            {
                int samples_per_clock = last_samples_per_clock;
                for( int i = 0 ; i < clock_informations_for_current_process_block.size() ; ++i )
                {
                    const SyncPosPair pair = clock_informations_for_current_process_block.getUnchecked(i);
                    if( pos_in_buffer_ >= pair.pos_in_buffer  )
                    {
                        samples_per_clock = pair.samples_per_clock;
                    }
                    else
                    {
                        break;
                    }
                }
                return samples_per_clock;
            }
            int get_samples_per_clock( int pos_in_buffer_, Array< RuntimeInfo::standalone_features::ClockSync::SyncPosPair > clock_informations_copy_ ) const noexcept
            {
                int samples_per_clock = last_samples_per_clock;
                for( int i = 0 ; i < clock_informations_copy_.size() ; ++i )
                {
                    const SyncPosPair pair = clock_informations_copy_.getUnchecked(i);
                    if( pos_in_buffer_ >= pair.pos_in_buffer  )
                    {
                        samples_per_clock = pair.samples_per_clock;
                    }
                    else
                    {
                        break;
                    }
                }
                return samples_per_clock;
            }
            void create_a_working_copy() noexcept
            {
                clock_informations_for_current_process_block.clearQuick();
                clock_informations_for_current_process_block.addArray( clock_informations );
            }
            Array< RuntimeInfo::standalone_features::ClockSync::SyncPosPair > get_a_working_copy() const noexcept
            {
                return clock_informations;
            }
            bool has_clocks_inside() const noexcept
            {
                return clock_informations.size();
            }
            int get_last_samples_per_clock() const noexcept
            {
                return last_samples_per_clock;
            }
            void add_clock( int pos_in_buffer_, int samples_per_clock_ ) noexcept
            {
                clock_informations.add( SyncPosPair( pos_in_buffer_, samples_per_clock_ ) );
            }
            void clear() noexcept
            {
                const int size = clock_informations.size();
                if( size > 0 )
                {
                    last_samples_per_clock = clock_informations.getReference(size-1).samples_per_clock;
                }
                clock_informations.clearQuick();
            }

            inline ClockSync() noexcept :
                    last_samples_per_clock(500)  {}
            inline ~ClockSync() noexcept {}
        } clock_sync_information;

        OwnedArray<Step> steps_in_block;
    };
    std::unique_ptr<standalone_features> standalone_features_pimpl;

private:
    //==========================================================================
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< RuntimeInfo >;
    COLD RuntimeInfo() noexcept;
    COLD ~RuntimeInfo() noexcept;

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
class MorphGroup;
class SmoothedParameter;
class SmoothManager : public RuntimeListener, DeletedAtShutdown
{
    friend class SmoothedParameter;
    Array< SmoothedParameter* > smoothers;
    RuntimeNotifyer*const notifyer;

    //==========================================================================
    friend class MoniqueSynthData;
    friend class ContainerDeletePolicy< SmoothManager >;
COLD SmoothManager(RuntimeNotifyer*const notifyer_) noexcept :
    RuntimeListener(notifyer_), notifyer(notifyer_) {}
    COLD ~SmoothManager() noexcept {}

    void sample_rate_or_block_changed() noexcept override {};

public:
    void smooth_and_morph( bool force_by_load_, bool do_really_morph_,
                           const float* morph_amount_, int num_samples_,
                           int smooth_motor_time_in_ms_, int morph_motor_time_in_ms_,
                           MorphGroup*morph_group_ ) noexcept;


public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothManager)
};

//==============================================================================
//==============================================================================
//==============================================================================
class ENV;
class SmoothedParameter : RuntimeListener
{
    SmoothManager*const smooth_manager;

    mono_AudioSampleBuffer<1> values;
    mono_AudioSampleBuffer<1> modulation_power;

public:
    Parameter*const param_to_smooth;
    float const max_value;
    float const min_value;

private:
    //==========================================================================
    LinearSmoother simple_smoother;
public:
    void simple_smooth( int smooth_motor_time_in_ms_, int num_samples_ ) noexcept;

private:
    //==========================================================================
    LinearSmoother left_morph_smoother;
    LinearSmoother right_morph_smoother;
    LinearSmoother left_modulation_morph_smoother;
    LinearSmoother right_modulation_morph_smoother;

    LinearSmootherMinMax<0,1> morph_power_smoother;
public:
    void smooth_and_morph( bool force_by_load_, bool is_automated_morph_,
                           int smooth_motor_time_in_ms_, int glide_motor_time_in_ms_,
                           const float* morph_amp_buffer_, float morph_slider_state_,
                           const Parameter*left_source_param_, const Parameter*right_source_param_, int num_samples_ ) noexcept;
private:
    //==========================================================================
    LinearSmootherMinMax<0,1> modulation_power_smoother;
public:
    void process_modulation( const bool is_modulated_, const float*modulator_buffer_, int num_samples_ ) noexcept;

private:
    //==========================================================================
    LinearSmootherMinMax<0,1> amp_power_smoother;
public:
    void process_amp( bool use_env_, int glide_time_in_ms_, ENV*env_, float*amp_buffer_, int num_samples_ ) noexcept;

public:
    //==========================================================================
    inline const float* get_smoothed_value_buffer() const noexcept
    {
        return values.getReadPointer();
    }
    inline void sample_rate_or_block_changed() noexcept override;

    //==========================================================================
    COLD SmoothedParameter( SmoothManager*const smooth_manager_, Parameter*const param_to_smooth_ ) noexcept;
    COLD virtual ~SmoothedParameter() noexcept;
    COLD void set_offline() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothedParameter)
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
    IntParameter speed;

    Parameter wave;
    SmoothedParameter wave_smoother;

    Parameter phase_shift;
    SmoothedParameter phase_shift_smoother;

    //==========================================================================
    COLD LFOData( SmoothManager*smooth_manager_, int id_, const char*name_ ) noexcept;
    COLD ~LFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOData)
};

static inline void copy( LFOData& dest_, const LFOData& src_ ) noexcept
{
    dest_.speed = src_.speed;
    dest_.wave = src_.wave;
    dest_.phase_shift = src_.phase_shift;
}

static inline bool is_integer( float value_ ) noexcept
{
    return value_ == int(value_);
}

static inline float get_lfo_speed_multi( float speed_ ) noexcept
{
    float factor = 1;
    if( speed_ <= 6 )
    {
        if( speed_ <= 0 )
        {
            factor =  16.0f; //return "16/1";
        }
        else if( speed_ <= 1 )
        {
            factor = 12.0f + 4.0f*(1.0f-speed_); //return "12/1";
        }
        else if( speed_ <= 2 )
        {
            factor = 8.0f + 4.0f*(1.0f-(speed_-1));
        }
        else if( speed_ <= 3 )
        {
            factor = 4.0f + 4.0f*(1.0f-(speed_-2));
        }
        else if( speed_ <= 4 )
        {
            factor = 3 + (1.0f-(speed_-3));
        }
        else if( speed_ <= 5 )
        {
            factor = 2 + (1.0f-(speed_-4));
        }
        else if( speed_ <= 6 )
        {
            factor = 1 + (1.0f-(speed_-5));
        }
    }
    else if( speed_ < 17 )
    {
        factor = 0;
        if( speed_ <= 7 )
        {
            factor = 3.0f/4;
            factor += (1.0f-factor)*(1.0f-(speed_-6));
        }
        else if( speed_ <= 8 )
        {
            factor = 1.0f/2;
            factor += (3.0f/4-factor)*(1.0f-(speed_-7));
        }
        else if( speed_ <= 9 )
        {
            factor = 1.0f/3;
            factor += (1.0f/2-factor)*(1.0f-(speed_-8));
        }
        else if( speed_ <= 10 )
        {
            factor = 1.0f/4;
            factor += (1.0f/3-factor)*(1.0f-(speed_-9));
        }
        else if( speed_ <= 11 )
        {
            factor = 1.0f/8;
            factor += (1.0f/4-factor)*(1.0f-(speed_-10));
        }
        else if( speed_ <= 12 )
        {
            factor = 1.0f/12;
            factor += (1.0f/8-factor)*(1.0f-(speed_-11));
        }
        else if( speed_ <= 13 )
        {
            factor = 1.0f/16;
            factor += (1.0f/12-factor)*(1.0f-(speed_-12));
        }
        else if( speed_ <= 14 )
        {
            factor = 1.0f/24;
            factor += (1.0f/16-factor)*(1.0f-(speed_-13));
        }
        else if( speed_ <= 15 )
        {
            factor = 1.0f/32;
            factor += (1.0f/24-factor)*(1.0f-(speed_-14));
        }
        else if( speed_ <= 16 )
        {
            factor = 1.0f/64;
            factor += (1.0f/32-factor)*(1.0f-(speed_-15));
        }
        else
        {
            factor = 1.0f/128;
            factor += (1.0f/64-factor)*(1.0f-(speed_-15));
        }
    }

    return factor;
}
static inline float lfo_speed_in_hertz( float speed_, RuntimeInfo*info_, float sample_rate_ ) noexcept
{
    const float bars_per_sec = info_->bpm/4/60;
    const float cycles_per_sec = bars_per_sec/get_lfo_speed_multi( speed_ );
    return cycles_per_sec;
}
static inline String get_lfo_speed_multi_as_text( float speed_, RuntimeInfo*info_, float sample_rate_ ) noexcept
{
    if( speed_ <= 6 )
    {
        if( speed_ <= 0 )
        {
            return "16/1";
        }
        else if( speed_ == 1 )
        {
            return "12/1";
        }
        else if( speed_ == 2 )
        {
            return "8/1";
        }
        else if( speed_ == 3 )
        {
            return "4/1";
        }
        else if( speed_ == 4 )
        {
            return "3/1";
        }
        else if( speed_ == 5 )
        {
            return "2/1";
        }
        else if( speed_ == 6 )
        {
            return "1/1";
        }

        return String(round001(lfo_speed_in_hertz( speed_, info_, sample_rate_ )));
    }
    else // if( speed_ <= 17 )
    {
        if( speed_ == 7 )
        {
            return "3/4";
        }
        else if( speed_ == 8 )
        {
            return "1/2";
        }
        else if( speed_ == 9 )
        {
            return "1/3";
        }
        else if( speed_ == 10 )
        {
            return "1/4";
        }
        else if( speed_ == 11 )
        {
            return "1/8";
        }
        else if( speed_ == 12 )
        {
            return "1/12";
        }
        else if( speed_ == 13 )
        {
            return "1/16";
        }
        else if( speed_ == 14 )
        {
            return "1/24";
        }
        else if( speed_ == 15 )
        {
            return "1/32";
        }
        else if( speed_ == 16 )
        {
            return "1/64";
        }
        else if( speed_ == 17 )
        {
            return "1/128";
        }

        return String(round001(lfo_speed_in_hertz( speed_, info_, sample_rate_ )));
    }
    /*
    else
    {
        return MidiMessage::getMidiNoteName(frequencyToMidi(midiToFrequency(33+speed_-18)),true,true,0);
    }
    */
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
struct FMOscData
{
    Parameter fm_freq;
    SmoothedParameter fm_freq_smoother;

    BoolParameter sync;
    Parameter fm_swing;
    SmoothedParameter fm_swing_smoother;
    Parameter fm_shape;
    SmoothedParameter fm_shape_smoother;

    ModulatedParameter master_shift;
    SmoothedParameter master_shift_smoother;

    //==========================================================================
    COLD FMOscData( SmoothManager*const smooth_manager_ ) noexcept;
    COLD ~FMOscData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMOscData)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct OSCData
{
    const int id;

    BoolParameter sync;

    Parameter wave;
    SmoothedParameter wave_smoother;
    Parameter fm_amount;
    SmoothedParameter fm_amount_smoother;
    ModulatedParameter tune;
    SmoothedParameter tune_smoother;

    BoolParameter is_lfo_modulated;

    // FOR UI FEEDBACK
    float last_modulation_value;

    //==========================================================================
    COLD OSCData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~OSCData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCData)
};

//==============================================================================
//==============================================================================
//==============================================================================
#define MIN_ENV_TIMES 1.0f // 15
#define MAX_ENV_TIMES 4999.0f // 15
struct ENVData
{
    const int id;

    Parameter attack;
    Parameter decay;
    Parameter sustain;
    SmoothedParameter sustain_smoother;
    Parameter sustain_time;
    Parameter release;

    Parameter shape;
    SmoothedParameter shape_smoother;
    Parameter velosivity;
    SmoothedParameter velosivity_smoother;

    //==========================================================================
    COLD ENVData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ENVData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVData)
};
static inline void copy( ENVData& dest_, const ENVData& src_ ) noexcept
{
    dest_.attack = src_.attack;
    dest_.decay = src_.decay;
    dest_.sustain = src_.sustain;
    dest_.sustain_time = src_.sustain_time;
    dest_.release = src_.release;

    dest_.shape = src_.shape;
    dest_.velosivity = src_.velosivity;
}
// exp(1)-1 1.71828
// exp(2)-1 6.38906
// exp(3)-1 19.0855
// exp(4)-1
static inline float get_env_samples( float time_, double sample_rate_ ) noexcept
{
    const float exp_time_ms = (float(exp(time_*4))-1) / 53.5982f;
    return  jmax(10,msToSamplesFast( exp_time_ms*MAX_ENV_TIMES+MIN_ENV_TIMES, sample_rate_ ));
}
static inline float get_env_ms( float time_ ) noexcept
{
    return ((float(exp(time_*4))-1) / 53.5982f) * MAX_ENV_TIMES + MIN_ENV_TIMES;
}
static inline float reverse_ms_to_slider_value( float time_in_ms_ ) noexcept
{
    float result = time_in_ms_ - MIN_ENV_TIMES;
    if( result < 0 )
    {
        result = 0.000000001;
    }
    result = result / MAX_ENV_TIMES;
    result *= 53.5982f;
    result += 1;
    result = log( result );
    return result / 4;
    // return log(((( ( time_in_ms_-MIN_ENV_TIMES ) /MAX_ENV_TIMES ) * 53.5982f) /4) +1);
}
//==============================================================================
//==============================================================================
//==============================================================================
struct FilterData
{
    IntParameter filter_type;
    Parameter adsr_lfo_mix;
    SmoothedParameter adsr_lfo_mix_smoother;

    ModulatedParameter distortion;
    SmoothedParameter distortion_smoother;
    BoolParameter modulate_distortion;

    ModulatedParameter cutoff;
    SmoothedParameter cutoff_smoother;
    BoolParameter modulate_cutoff;

    ModulatedParameter resonance;
    SmoothedParameter resonance_smoother;
    BoolParameter modulate_resonance;

    ModulatedParameter pan;
    SmoothedParameter pan_smoother;
    BoolParameter modulate_pan;
    ModulatedParameter output;
    SmoothedParameter output_smoother;
    BoolParameter modulate_output;

    ArrayOfParameters input_sustains;
    OwnedArray<SmoothedParameter> input_smoothers;
    ArrayOfBoolParameters input_holds;

    OwnedArray<ENVData> input_envs;

    ENVData*const env_data;

public:
    //==========================================================================
    COLD FilterData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~FilterData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( FilterData )
};
static inline float get_cutoff( float cutoff_slider_value_ ) noexcept
{
    // exp(3)-1 19.0855
    // exp(4)-1
    return ((exp(cutoff_slider_value_*4)-1)/53.5982) * MAX_CUTOFF + MIN_CUTOFF;
}
static inline float reverse_cutoff_to_slider_value( float frequency_ ) noexcept
{
    float result = frequency_ - MIN_CUTOFF;
    if( result < 0 )
    {
        result = 0.000000001;
    }
    result = result / MAX_CUTOFF;
    result *= 53.5982f;
    result += 1;
    result = log( result );
    return result / 4;
    // return log(((( ( time_in_ms_-MIN_ENV_TIMES ) /MAX_ENV_TIMES ) * 53.5982f) /4) +1);
}
//==============================================================================
//==============================================================================
//==============================================================================
struct ArpSequencerData
{
    BoolParameter is_on;
    BoolParameter is_sequencer;

    ArrayOfBoolParameters step;
    ArrayOfIntParameters tune;
    ArrayOfParameters velocity;
    OwnedArray<SmoothedParameter> velocity_smoothers;

    IntParameter shuffle;
    BoolParameter connect;
    IntParameter speed_multi;
    IntParameter step_offset;
    IntParameter fine_offset;

    //==========================================================================
    COLD ArpSequencerData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
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
    default :
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
        return "1/48";
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
        //case 16 :
    default :
        return "1/1";
    }
}

//==============================================================================
struct EQData
{
    ArrayOfParameters velocity;
    OwnedArray<SmoothedParameter> velocity_smoothers;
    ArrayOfBoolParameters hold;

    Parameter bypass;
    SmoothedParameter bypass_smoother;

    OwnedArray<ENVData> envs;

public:
    //==========================================================================
    COLD EQData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~EQData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( EQData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ReverbData
{
    Parameter room;
    SmoothedParameter room_smoother;
    Parameter dry_wet_mix;
    SmoothedParameter dry_wet_mix_smoother;
    Parameter width;
    SmoothedParameter width_smoother;

    Parameter pan;
    SmoothedParameter pan_smoother;

    //==========================================================================
    COLD ReverbData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ReverbData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ReverbData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ChorusData
{
    Parameter modulation;
    SmoothedParameter modulation_smoother;

    Parameter pan;
    SmoothedParameter pan_smoother;

public:
    //==========================================================================
    COLD ChorusData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
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
class MorphGroup : public Timer, ParameterListener
{
    MorphGroup* left_morph_source;
    MorphGroup* right_morph_source;

    friend class MoniqueSynthData;
    friend class SmoothManager;
    Array< Parameter* > params;
    float last_power_of_right;
    Array< BoolParameter* > switch_bool_params;
    bool current_switch;
    Array< IntParameter* > switch_int_params;

public:
    //==========================================================================
    inline int indexOf( const Parameter*param_ ) const noexcept
    {
        return params.indexOf( const_cast<Parameter*>(param_) );
    }
    // NOT FOR HIGH PERFORMANCE
    inline int indexOfBools( const Parameter*param_ ) const noexcept
    {
        if( type_of( param_ ) == IS_BOOL )
        {
            return switch_bool_params.indexOf( reinterpret_cast<BoolParameter*>( const_cast<Parameter*>(param_) ) );
        }

        return -1;
    }
    // NOT FOR HIGH PERFORMANCE
    inline int indexOfInts( const Parameter*param_ ) const noexcept
    {
        if( type_of( param_ ) == IS_INT )
        {
            return switch_int_params.indexOf( reinterpret_cast<IntParameter*>( const_cast<Parameter*>(param_) ) );
        }

        return -1;
    }
    inline const Parameter* get_left_param( int index_ ) const noexcept
    {
        return left_morph_source->params.getUnchecked(index_);
    }
    inline const Parameter* get_right_param( int index_ ) const noexcept
    {
        return right_morph_source->params.getUnchecked(index_);
    }

    inline void morph( float morph_amount_ ) noexcept;
    inline void morph_switchs( bool left_right_ ) noexcept;

private:
    //==========================================================================
    Array< float > sync_param_deltas;
    Array< float > sync_modulation_deltas;
    void run_sync_morph() noexcept;
    int current_callbacks;
    void timerCallback() override;

private:
    //==========================================================================
    // WILL ONLY BE CALLED IN THE MASTER MORPH GROUP, COZ THE SUB GROUBS DOES NOT LISTEN THE PARAMS
    // UPDATES THE LEFT AND RIGHT SOURCES
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_modulation_value_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    // INIT
    COLD MorphGroup() noexcept;
    COLD ~MorphGroup() noexcept;

    COLD void register_parameter( Parameter* param_, bool is_master_ ) noexcept;
    COLD void register_switch_parameter( BoolParameter* param_, bool is_master_ ) noexcept;
    COLD void register_switch_parameter( IntParameter* param_, bool is_master_ ) noexcept;

    COLD void set_sources( MorphGroup* left_source_, MorphGroup* right_source_,
                           float current_morph_amount_, bool current_switch_state_ ) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphGroup)
};

class MTSClient;
struct MoniqueTuningData
{
    ~MoniqueTuningData();

    enum Mode {
        TWELVE_TET,
        SCL_KBM,
        MTS_ESP
    } mode{TWELVE_TET};
    float midiNoteToFrequency(float note)
    {
       switch(mode)
       {
          case TWELVE_TET:
             return 440.0 *  pow ( 2.0f, ((note - 69.0f) * (1.0f/12)) );
             break;
          case SCL_KBM:
             // not implemented yet
             return 440.0 *  pow ( 2.0f, ((note - 69.0f) * (1.0f/12)) );
             break;
          case MTS_ESP:
             return midiNoteFromMTS(note);
             break;
       }
       return 421;
    }

    MTSClient* mts_client{nullptr};
    int mtsChecked{0};
    void updateMTSESPStatus();
    float midiNoteFromMTS(float note);
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
class MoniqueSynthesiserVoice;
struct MoniqueSynthData : ParameterListener
{
    MoniqueSynthData*const master_data;

    UiLookAndFeel*const ui_look_and_feel; // WILL BE NULL FOR MORPH DATA
    MoniqueAudioProcessor*const audio_processor; // WILL BE NULL FOR MORPH DATA

    SmoothManager*const smooth_manager; // TODO is nowhere deleted
    RuntimeNotifyer*const runtime_notifyer;
    RuntimeInfo*const runtime_info;
    DataBuffer*const data_buffer;
    MoniqueSynthesiserVoice*voice; // WILL BE SET BY THE PROCESSOR

    //==============================================================================
    const float*const sine_lookup;
    const float*const cos_lookup;
    const float*const exp_lookup;

    MoniqueTuningData*const tuning;

    const int id;

    BoolParameter is_stereo;
#ifdef POLY
    ArrayOfBoolParameters keytrack_osci;
    ArrayOfIntParameters keytrack_osci_octave_offset;
    ArrayOfBoolParameters keytrack_cutoff;
    ArrayOfIntParameters keytrack_cutoff_octave_offset;
    ArrayOfBoolParameters keytrack_filter_inputs;
    ArrayOfBoolParameters keytrack_filter_env;
    ArrayOfBoolParameters keytrack_filter_volume;
    ArrayOfParameters keytrack_filter_volume_offset;

    IntParameter keytrack_osci_play_mode;
#endif
    Parameter volume;
    SmoothedParameter volume_smoother;
    Parameter glide;
    SmoothedParameter glide_smoother;
    Parameter delay;
    SmoothedParameter delay_smoother;
    Parameter delay_pan;
    SmoothedParameter delay_pan_smoother;
    IntParameter delay_refexion;
    IntParameter delay_record_size;
    Parameter delay_record_release;
    SmoothedParameter delay_record_release_smoother;
    BoolParameter delay_record;
    Parameter effect_bypass;
    SmoothedParameter effect_bypass_smoother;
    Parameter shape;
    SmoothedParameter shape_smoother;
    Parameter distortion;
    SmoothedParameter distortion_smoother;
    IntParameter octave_offset;
    IntParameter note_offset;

    BoolParameter sync;
    Parameter speed;

    IntParameter glide_motor_time;
    IntParameter velocity_glide_time;

    BoolParameter shift;

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

    BoolParameter auto_close_env_popup;
    BoolParameter auto_switch_env_popup;

    BoolParameter is_osci_open;

    BoolParameter keep_arp_always_on;
    BoolParameter keep_arp_always_off;

    // SETTINGS
    BoolParameter animate_envs;
    BoolParameter animate_sliders;
    BoolParameter animate_arp;
    BoolParameter animate_poly;
    BoolParameter show_tooltips;
    BoolParameter sliders_in_rotary_mode;
    IntParameter sliders_sensitivity;
    IntParameter sliders_linear_sensitivity;
    BoolParameter is_rotary_sliders_velocity_mode;
    BoolParameter is_linear_sliders_velocity_mode;
    BoolParameter only_use_rotary_sliders;

    BoolParameter ui_is_large;
    Parameter ui_scale_factor;

    // MIDI HACKS
    Parameter midi_lfo_wave;
    IntParameter midi_lfo_speed;
    Parameter midi_lfo_offset;
    IntParameter midi_lfo_popup;

    Parameter midi_env_attack;
    Parameter midi_env_decay;
    Parameter midi_env_sustain;
    Parameter midi_env_sustain_time;
    Parameter midi_env_release;
    Parameter midi_env_shape;
    IntParameter midi_env_popup;

    ScopedPointer< ENVData > env_data;

    OwnedArray< LFOData > lfo_datas;
    OwnedArray< LFOData > mfo_datas;
    OwnedArray< OSCData > osc_datas;
    ScopedPointer<FMOscData> fm_osc_data;
    OwnedArray< FilterData > filter_datas;
    ScopedPointer< EQData > eq_data;
    ScopedPointer< ArpSequencerData > arp_sequencer_data;
    ScopedPointer< ReverbData > reverb_data;
    ScopedPointer< ChorusData > chorus_data;

private:
    // ==============================================================================
    Array< Parameter* > saveable_parameters;
    Array< Parameter* > automateable_parameters;
    Array< float > saveable_backups;
    Array< Parameter* > global_parameters;
    Array< Parameter* > all_parameters;
    Array< Parameter* > mono_parameters;
    COLD void colect_saveable_parameters() noexcept;
    COLD void colect_global_parameters() noexcept;

public:
    // TODO
    inline Array< Parameter* >& get_atomateable_parameters() noexcept
    {
        return automateable_parameters;
    }
    inline Array< Parameter* >& get_global_parameters() noexcept
    {
        return global_parameters;
    }
    inline Array< Parameter* >& get_all_parameters() noexcept
    {
        return all_parameters;
    }

	bool arp_was_on_before_change = false;
	int changed_programm = -3;

    // ==============================================================================
private:
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< MoniqueSynthData >;
    COLD MoniqueSynthData( DATA_TYPES data_type,
                           UiLookAndFeel*look_and_feel_,
                           MoniqueAudioProcessor*const audio_processor_,
                           RuntimeNotifyer*const runtime_notifyer_,
                           RuntimeInfo*const info_,
                           DataBuffer*data_buffer_,
                           SmoothManager*smooth_manager = nullptr, /* NOTE: the master data owns the manager, but the morph groups will be smoothed*/
                           MoniqueSynthData* master_data = nullptr
                         ) noexcept;
    COLD ~MoniqueSynthData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( MoniqueSynthData )

public:
    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // MORPH
    ArrayOfParameters morhp_states;
    ArrayOfBoolParameters is_morph_modulated;
    ArrayOfParameters morhp_automation_power;
    ArrayOfBoolParameters morhp_switch_states;
    IntParameter morph_motor_time;

public:
    ScopedPointer<MorphGroup> morph_group_1, morph_group_2, morph_group_3, morph_group_4;
private:
    OwnedArray< MoniqueSynthData > left_morph_sources;
    OwnedArray< MoniqueSynthData > right_morph_sources;
    StringArray left_morph_source_names;
    StringArray right_morph_source_names;
public:
    const String& get_morph_source_name( int id_abs_ ) const noexcept;
private:

    COLD void init_morph_groups( DATA_TYPES data_type, MoniqueSynthData*master_data_ ) noexcept;

    CriticalSection morph_lock;

public:
    void set_to_stereo( bool state_ ) noexcept;
    float get_morph_state( int morpher_id_ ) const noexcept;
    bool get_morph_switch_state( int morpher_id_ ) const noexcept;
    void morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ = false ) noexcept;
    void morph_switch_buttons( int morpher_id_, bool do_switch_ = true ) noexcept;
    void run_sync_morph() noexcept;

private:
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_by_automation( Parameter* param_ ) noexcept override;

public:
    // COPY THE CURRENT STATE TO THE SOURCES
    void set_morph_source_data_from_current( int morpher_id_, bool left_or_right_, bool run_sync_morph_ ) noexcept;
    void refresh_morph_programms() noexcept;
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

private:
    String current_theme; // TODO store!
    StringArray colour_themes;
public:
    // ==============================================================================
    const StringArray& get_themes() noexcept;
    const String& get_current_theme() const noexcept;
    bool load_theme( const String& name_ ) noexcept;
    bool replace_theme( const String& name_ ) noexcept;
    bool remove_theme( const String& name_ ) noexcept;
    bool create_new_theme( const String& name_ ) noexcept;

public:
    // ==============================================================================
    static void refresh_banks_and_programms( MoniqueSynthData& synth_data ) noexcept;
private:
    void calc_current_program_abs() noexcept;

    static void update_banks( StringArray& ) noexcept;
    static void update_bank_programms( MoniqueSynthData& synth_data, int bank_id_, StringArray& program_names_ ) noexcept;

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
    String alternative_program_name;

    const String error_string;
    int get_current_programm_id_abs() const noexcept;
    const String& get_current_program_name_abs() const noexcept;
    const String& get_program_name_abs(int id_) const noexcept;

    // ==============================================================================
    void create_internal_backup( const String& programm_name_, const String& bank_name_  ) noexcept;
    static String& generate_programm_name( const String& bank_, String& name_ ) noexcept;
    bool create_new( const String& new_name_ ) noexcept;
    bool rename( const String& new_name_ ) noexcept;
    bool replace() noexcept;
    bool remove() noexcept;

    bool load( bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;
    bool load_prev() noexcept;
    bool load_next() noexcept;
private:
    bool load( const String bank_name_, const String program_name_, bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;

public:
    // ==============================================================================
    ScopedPointer<XmlElement> factory_default;
    void load_default() noexcept;
    void save_to( XmlElement* xml ) noexcept;
    bool force_morph_update__load_flag;
    void read_from( const XmlElement* xml ) noexcept;

private:
    bool write2file( const String& bank_name_, const String& program_name_ ) noexcept;

public:
    int program_restore_block_time;
    void save_settings() const noexcept;
    void ask_and_save_if_changed( bool with_new_option = false ) noexcept;
    void load_settings() noexcept;

public:
    // ==============================================================================
    void save_midi() const noexcept;
    void read_midi() noexcept;

public:
    // ==============================================================================
    void get_full_adstr( ENVData&env_data_,Array< float >& curve ) noexcept;
    void get_full_mfo( LFOData&mfo_data_,Array< float >& curve, MoniqueSynthData*data_ ) noexcept;
    bool is_key_down( int id ) const noexcept;
    float get_tracking_env_state( int id ) const noexcept;
};

//==============================================================================
static inline double delay_multi( int delay_ ) noexcept
{
    switch( delay_ )
    {
    case 0 :
        return 1.0/1024;
    case 1 :
        return 1.0/512;
    case 2 :
        return 1.0/256;
    case 3 :
        return 1.0/128;
    case 4 :
        return 1.0/64;
    case 5 :
        return 1.0/48;
    case 6 :
        return 1.0/32;
    case 7 :
        return 1.0/24;
    case 8 :
        return 1.0/16;
    case 9 :
        return 1.0/12;
    case 10 :
        return 1.0/8;
    case 11 :
        return 1.0/4;
    case 12 :
        return 3.0/8;
    case 13 :
        return 4.0/8;
    case 14 :
        return 5.0/8;
    case 15 :
        return 6.0/8;
    case 16 :
        return 7.0/8;
    case 17 :
        return 1;
    case 18 :
        return 2;
    default :
        return 4;
    }
}
static inline StringRef delay_to_text( int delay_, int sample_rate_ ) noexcept
{
    switch( delay_ )
    {
    case 0 :
        return "/1024";
        break;
    case 1 :
        return  "/512";
        break;
    case 2 :
        return  "/256";
        break;
    case 3 :
        return  "/128";
        break;
    case 4 :
        return  "1/64";
        break;
    case 5 :
        return  "1/48";
        break;
    case 6 :
        return  "1/32";
        break;
    case 7 :
        return  "1/24";
        break;
    case 8 :
        return  "1/16";
        break;
    case 9 :
        return  "1/12";
        break;
    case 10 :
        return  "1/8";
        break;
    case 11 :
        return  "2/8";
        break;
    case 12 :
        return  "3/8";
        break;
    case 13 :
        return  "4/8";
        break;
    case 14 :
        return  "5/8";
        break;
    case 15 :
        return  "6/8";
        break;
    case 16 :
        return  "7/8";
        break;
    case 17 :
        return  "1/1";
    case 18 :
        return "2/1";
    default :
        return "4/1";
    }
}

namespace make_get_shared_singleton_internals
{
    /*
     * Not for public use. Implementation details of make_get_shared_singleton<>()
     *
     * Holds a unique instance of singleton_type, its reference counter and a
     * mutex to lock creation and deletion of this instance.
     */
    template< class singleton_type >
    struct static_data_held_for_singleton_type
    {
        inline static std::unique_ptr< singleton_type > instance;
        inline static std::atomic_bool                  instance_created{ false };
        inline static std::atomic_int                   num_references{ 0 };
        inline static std::mutex                        create_delete_and_client_count_mutex;

    private:
        static_data_held_for_singleton_type() = delete;
    };
}

/*
 * TODO move to own file
 *
 * Singleton meets shared_ptr and dll shared memory.
 *
 * Create xor returns an instance of type shared_singleton_type.
 *
 * lifetime policy:
 * ----------------
 * The lifetime behavior is absolutely identically to a common std::shared ptr.
 * As long as you hold at least one reference (the returned shared_ptr) the instance
 * will be kept alive.
 *
 * creation policy:
 * ----------------
 * The behavior of the function is well known from the singleton get_instance method.
 * If no instance exists a new one will be created and returned.
 * If an instance exists - so somewhere is at least one reference out there - then the
 * existing instance will be returned and no new one will be created.
 *
 * difference to singleton:
 * ------------------------
 * The lifetime is scoped. So if no reference left (at least one returned shared_ptr),
 * then the instance will be deleted and a further call of this function will create
 * and return a new instance.
 *
 * DLL environment:
 * ----------------
 * This function behaves absolutely identically across several instances of the same
 * dll in the same process. It returns the same instance and shares the reference counting.
 *
 * like a singleton:
 * -----------------
 * To use this function like a common singleton you just need to hold a reference of any
 * shared_ptr somewhere, and then you can call this method like the get_instance method
 * of a common singleton.
 *
 * In the JUCE world a good place for the first instance might be the AudioProcessor.
 *
 * Multithreading:
 * ---------------
 * This function is thread save - even across dlls.
 *
 */
template< class shared_singleton_type, class ... construction_arguments >
std::shared_ptr< shared_singleton_type > make_get_shared_singleton( construction_arguments&& ...args )
{
    /*
     * an optimized lock mechanism to avoid unnecessary locks on a mutex by checking a condition
     *
     * see: lock_if
     */
    struct scoped_conditional_lockable
    {
        scoped_conditional_lockable( std::mutex& mutex )
            : mutex{ mutex }
        {}

        /*
         * If the lock_condition is true before and after locking the mutex
         * this method returns true and keeps the mutex locked until this
         * object will be deconstructed
         */
        bool lock_if( std::function< bool() > condition )
        {
            if ( condition() )
            {
                mutex.lock();

                if ( condition() )
                {
                    lock_acquired = true;
                }
                else
                {
                    mutex.unlock();
                }
            }
        }

        /*
         * releases an acquired lock on the mutex
         */
        ~scoped_conditional_lockable()
        {
            if ( lock_acquired )
            {
                mutex.unlock();
            }
        }

    private:
        std::mutex& mutex;
        bool lock_acquired = false;
    };

    using static_data_per_singleton_type = make_get_shared_singleton_internals::static_data_held_for_singleton_type< shared_singleton_type >;

    // this custom deleter kills the singleton when we run out of references
    auto reference_counting_singleton_deleter = [ & ]( shared_singleton_type* instance_to_delete )
    {
        --static_data_per_singleton_type::num_references;
        auto conditional_lockable     = scoped_conditional_lockable{ static_data_per_singleton_type::create_delete_and_client_count_mutex };
        const auto no_references_on_an_existing_instance_left = []()
        {
            return 0 == static_data_per_singleton_type::num_references && static_data_per_singleton_type::instance_created;
        };
        if ( conditional_lockable.lock_if( no_references_on_an_existing_instance_left ) )
        {
            jassert( instance_to_delete == static_data_per_singleton_type::instance.get() );

            DBG( "delete shared singleton instance of: " + String{ typeid( shared_singleton_type ).name() } );

            static_data_per_singleton_type::instance.reset();
            static_data_per_singleton_type::instance_created = false;
        }
    };

    ++static_data_per_singleton_type::num_references;

    auto conditional_lockable          = scoped_conditional_lockable{ static_data_per_singleton_type::create_delete_and_client_count_mutex };
    const auto instance_is_not_created = []()
    {
        return not static_data_per_singleton_type::instance_created;
    };
    if ( conditional_lockable.lock_if( instance_is_not_created ) )
    {
        static_data_per_singleton_type::instance         = std::make_unique< shared_singleton_type >( std::forward< construction_arguments >( args )... );
        static_data_per_singleton_type::instance_created = true;
    }

    return std::shared_ptr< shared_singleton_type >{ static_data_per_singleton_type::instance.get(), reference_counting_singleton_deleter };
}

/*
 * A value held with ID to provide support for mapped singletons via make_get_shared_singleton.
 *
 * To make a picture:
 * make_get_shared_singleton< int >() would always return the same int instance.
 * make_get_shared_singleton< mapped_value< 1, int > >() return the int<1> instance
 * make_get_shared_singleton< mapped_value< 2, int > >() return the int<2> instance
 * ...
 */
template<int id, typename value_type = bool, value_type default_value = false>
struct mapped_value
{
    value_type value = default_value;
};

/*
 * map ids for shared singletons
 */
enum SHARED_VALUE_IDS{
    ENV_CLIPBOARD_HAS_DATA,
    LFO_CLIPBOARD_HAS_DATA
};

/*
 * get xor create a shared ENV clipboard for copy past ENVs between multiple
 * Monique instances across the same process.
 */
inline auto get_shared_ENV_clipboard = []()
{
    return make_get_shared_singleton< ENVData >( nullptr /* without smooth manager */, 999 /* id */ );
};

/*
 * Is set to true if the ENV clipboard contains past-able data
 *
 * see: get_shared_ENV_clipboard
 */
using ENV_clipboard_has_data = mapped_value< ENV_CLIPBOARD_HAS_DATA, bool, false /* clipboard is initially empty */ >;

/*
 * ENV_clipboard_has_data.value is true if the shared ENV clipboard has past-able data
 *
 * see: get_shared_ENV_clipboard
 * see: ENV_clipboard_has_data
 */
inline auto has_ENV_clipboard_data = []()
{
    return make_get_shared_singleton< ENV_clipboard_has_data >(  );
};

/*
 * get xor create a shared LFO clipboard for copy past LFOs between multiple
 * Monique instances across the same process.
 */
inline auto get_shared_LFO_clipboard = []()
{
    return make_get_shared_singleton< LFOData >( nullptr /* without smooth manager */, 999 /* id */, "CBFO" /* name */ );
};

/*
 * Is set to true if the LFO clipboard contains past-able data
 *
 * see: get_shared_LFO_clipboard
 */
using LFO_clipboard_has_data = mapped_value< LFO_CLIPBOARD_HAS_DATA, bool, false /* clipboard is initially empty */ >;

/*
 * LFO_clipboard_has_data.value is true if the shared LFO clipboard has past-able data
 *
 * see: get_shared_LFO_clipboard
 * see: LFO_clipboard_has_data
 */
inline auto has_LFO_clipboard_data = []()
{
    return make_get_shared_singleton< LFO_clipboard_has_data >(  );
};

/*
 * get xor create a shared Status instance for global settings persistence
 */
inline auto get_shared_status = []()
{
    return make_get_shared_singleton< Status >();
};

#endif

