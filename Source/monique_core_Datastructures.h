#ifndef MONOSYNTH_DATA_H_INCLUDED
#define MONOSYNTH_DATA_H_INCLUDED

#include "App_h_includer.h"

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

    SUM_EQ_BANDS = 7
};

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
    mono_AudioSampleBuffer<1> chorus_env;

    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_samples;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_env_amps;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_output_samples;
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_amps;

    mono_AudioSampleBuffer<1> tmp_buffer;
    mono_AudioSampleBuffer<1> second_mono_buffer;

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
struct RuntimeInfo
{
    int64 samples_since_start;
    double bpm;
    double steps_per_sample;

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

        COLD ClockCounter() : clock_counter(0), clock_counter_absolut(0) {}
    } clock_counter;

    struct Step
    {
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

    class ClockSync
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

        Array< SyncPosPair > clock_informations;

        int last_samples_per_clock;

    public:
        int get_samples_per_clock( int pos_in_buffer_ ) const noexcept
        {
            int samples_per_clock = last_samples_per_clock;
            for( int i = 0 ; i < clock_informations.size() ; ++i )
            {
                const SyncPosPair& pair = clock_informations.getReference(i);
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
            if( size )
            {
                last_samples_per_clock = clock_informations.getReference(size-1).samples_per_clock;
            }
            clock_informations.clearQuick();
        }

    inline ClockSync() noexcept :
        last_samples_per_clock(0)  {}
        inline ~ClockSync() noexcept {}
    } clock_sync_information;

    OwnedArray<Step> steps_in_block;
#endif

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
    void smooth_and_morph( bool do_really_morph_, const float* morph_amount_, int num_samples_,
                           int smooth_motor_time_in_ms_, int morph_motor_time_in_ms_,
                           MorphGroup*morph_group_ ) noexcept;


public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothManager)
};

//==============================================================================
//==============================================================================
//==============================================================================
class ENV;
class LinearSmoother
{
protected:
    float currentValue, target, step, lastValue;
    int countdown, stepsToTarget;
    bool option;

public:
    //==========================================================================
    inline float tick() noexcept
    {
        if( countdown > 0 )
        {
            if( --countdown < 1 )
            {
                currentValue = target;
                lastValue = currentValue;
            }
            else
            {
                currentValue += step;
                lastValue = currentValue;
            }
        }

        return lastValue;
    }
    inline bool is_up_to_date() const noexcept
    {
        return countdown == 0;
    }
    //==========================================================================
    inline float get_last_value() noexcept
    {
        return lastValue;
    }
    //==========================================================================
    inline void set_value (float newValue) noexcept
    {
        if (target != newValue)
        {
            target = newValue;
            countdown = stepsToTarget;
            step = (target - currentValue) / countdown;
        }
    }

    //==============================================================================
    inline void reset (float sampleRate, float fade_in_ms_) noexcept
    {
        stepsToTarget = int(msToSamplesFast(fade_in_ms_,sampleRate));
        currentValue = target;
        countdown = 0;
    }
    inline void reset_coefficients(float sampleRate, float fade_in_ms_) noexcept
    {
        stepsToTarget = int(msToSamplesFast(fade_in_ms_,sampleRate));
    }

    //==============================================================================
    inline void set_info_flag( bool state_ ) noexcept
    {
        option = state_;
    }
    inline bool get_info_flag() const noexcept
    {
        return option;
    }
public:
    //==============================================================================
    COLD LinearSmoother( float init_state_ = 0 ) noexcept
:
    currentValue(init_state_),
                 target(init_state_),
                 step(0),
                 countdown(-1),
                 stepsToTarget(0),
                 lastValue(init_state_),
                 option(false)
    {}

    COLD ~LinearSmoother() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearSmoother)
};
template<int min = 0, int max = 1>
class LinearSmootherMinMax : public LinearSmoother
{
    int glide_countdown;
public:
    //==============================================================================
    inline float tick() noexcept
    {
        if( countdown > 0 )
        {
            if( --countdown < 1 )
            {
                currentValue = target;
                lastValue = currentValue;
            }
            else
            {
                currentValue += step;
                lastValue = currentValue;
            }

            if( lastValue > max )
            {
                lastValue = max;
            }
            else if( lastValue < min )
            {
                lastValue = min;
            }
        }

        return lastValue;
    }

    inline float glide_tick(float to_value) noexcept
    {
        if(glide_countdown > 0)
        {
            if( stepsToTarget == glide_countdown )
            {
                countdown = glide_countdown;
            }

            target = to_value;
            step = (target - currentValue) / countdown;

            --glide_countdown;
            LinearSmootherMinMax::tick();
        }
        else
        {
            currentValue = to_value;
            lastValue = currentValue;
        }

        return lastValue;
    }

    inline void reset_glide_countdown() noexcept
    {
        glide_countdown = stepsToTarget;
    }

    //==============================================================================
    COLD LinearSmootherMinMax( float init_state_ = 0 ) noexcept
:
    LinearSmoother( init_state_ ),
                    glide_countdown(-1)
    {
    }
    COLD ~LinearSmootherMinMax() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearSmootherMinMax)
};

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
    void smooth_and_morph( bool is_automated_morph_,
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
    Parameter speed;

    Parameter wave;
    SmoothedParameter wave_smoother;

    Parameter phase_shift;
    SmoothedParameter phase_shift_smoother;

    //==========================================================================
    COLD LFOData( SmoothManager*smooth_manager_, int id_, const char*name_ ) noexcept;
    COLD ~LFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOData)
};
static inline void copy( LFOData* dest_, const LFOData* src_ ) noexcept
{
    dest_->speed = src_->speed;
    dest_->wave = src_->wave;
    dest_->phase_shift = src_->phase_shift;
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
#define MIN_ENV_TIMES 1 // 15
#define MAX_ENV_TIMES 5000-1 // 15
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

    //==========================================================================
    COLD ENVData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ENVData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVData)
};
static inline void copy( ENVData* dest_, const ENVData* src_ ) noexcept
{
    dest_->attack = src_->attack;
    dest_->decay = src_->decay;
    dest_->sustain = src_->sustain;
    dest_->sustain_time = src_->sustain_time;
    dest_->release = src_->release;

    dest_->shape = src_->shape;
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float get_cutoff( float cutoff_slider_value_ ) noexcept
{
    // exp(3)-1 19.0855
    // exp(4)-1
    return MAX_CUTOFF * ((exp(cutoff_slider_value_*4)-1)/53.5982) + MIN_CUTOFF;
}
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

//==============================================================================
//==============================================================================
//==============================================================================
struct ArpSequencerData
{
    BoolParameter is_on;

    ArrayOfBoolParameters step;
    ArrayOfIntParameters tune;
    ArrayOfParameters velocity;
    OwnedArray<SmoothedParameter> velocity_smoothers;

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
        //case 16 :
    default :
        return "1/1";
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline int get_low_pass_band_frequency( int band_id_, double sample_rate_ ) noexcept
{
    switch(band_id_)
    {
    case 0 :
        return 80;
    case 1 :
        return 160;
    case 2 :
        return 320;
    case 3 :
        return 640;
    case 4 :
        return 1280;
    case 5 :
        return 2660;
    default :
        return sample_rate_/2;
    }
}
static inline int get_high_pass_band_frequency( int band_id_ ) noexcept
{
    switch(band_id_)
    {
    case 0 :
        return 15;
    case 1 :
        return 80;
    case 2 :
        return 160;
    case 3 :
        return 320;
    case 4 :
        return 640;
    case 5 :
        return 1280;
    default :
        return 2660;
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
#define THREAD_LIMIT 3
#else
#define THREAD_LIMIT 0
#endif

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

    const int id;

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

    BoolParameter auto_close_env_popup;
    BoolParameter auto_switch_env_popup;

    BoolParameter is_osci_open;

    BoolParameter keep_arp_always_on;
    BoolParameter keep_arp_always_off;

    // MULTITHREADING
    IntParameter num_extra_threads;

    // SETTINGS
    BoolParameter animate_envs;
    BoolParameter show_tooltips;
    BoolParameter bind_sustain_and_sostenuto_pedal;
    BoolParameter sliders_in_rotary_mode;
    IntParameter sliders_sensitivity;
    IntParameter sliders_linear_sensitivity;
    BoolParameter is_rotary_sliders_velocity_mode;
    BoolParameter is_linear_sliders_velocity_mode;
    Parameter ui_scale_factor;

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
    void read_from( const XmlElement* xml ) noexcept;

private:
    bool write2file( const String& bank_name_, const String& program_name_ ) noexcept;

public:
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
    void get_full_mfo( LFOData&mfo_data_,Array< float >& curve ) noexcept;
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
    float multi;
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

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class SHARED
#ifdef IS_STANDALONE
    : public DeletedAtShutdown
#endif
{
public:
    int num_instances ;
    ENVData* env_clipboard;
    LFOData* mfo_clipboard;
    juce_DeclareSingleton( SHARED, true );

    SHARED() : num_instances(0), env_clipboard(nullptr),mfo_clipboard(nullptr) {}
};

#endif

