
#include "monique_core_Synth.h"
#include "monique_core_Datastructures.h"

#include "monique_ui_AmpPainter.h"
#include "monique_core_Processor.h"

#include "monique_ui_SegmentedMeter.h"

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
static inline float sample_mix( float s1_, float s2_ ) noexcept
{
    if ((s1_ > 0) && (s2_ > 0))
    {
        s1_ = s1_ + s2_ - (s1_ * s2_);
    }
    else if ((s1_ < 0) && (s2_ < 0))
    {
        s1_ = s1_ + s2_ + (s1_ * s2_);
    }
    else
    {
        s1_ = s1_ + s2_;
    }

    return s1_;
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
COLD SmoothedParameter::SmoothedParameter( SmoothManager*const smooth_manager_,
        Parameter*const param_to_smooth_ ) noexcept
:
RuntimeListener( smooth_manager_ ? smooth_manager_->notifyer : nullptr ),
                 smooth_manager(smooth_manager_),

                 values(block_size),
                 values_modulated(block_size),

                 param_to_smooth(param_to_smooth_),

                 max_value( param_to_smooth_->get_info().max_value ),
                 min_value( param_to_smooth_->get_info().min_value ),

                 last_value(param_to_smooth_->get_value()),
                 last_target(-99),
                 difference_per_sample(0),
                 samples_left(0),
                 buffer_is_linear_up_to_date_filled(false),

                 last_modulator(0),
                 was_modulated_last_time(false),

                 amp_switch_samples_left_max(1),
                 amp_switch_samples_left(0),
                 last_amp_automated(0),
                 last_amp_valued(0),
                 was_automated_last_time(false)
{
    if( smooth_manager )
    {
        smooth_manager->smoothers.add(this);
    }
}
COLD SmoothedParameter::~SmoothedParameter() noexcept
{
    if( smooth_manager )
    {
        smooth_manager->smoothers.removeFirstMatchingValue(this);
    }
}

COLD void SmoothedParameter::set_offline() noexcept
{
    if( smooth_manager )
    {
        smooth_manager->smoothers.removeFirstMatchingValue(this);
    }
}

//==============================================================================
COLD void SmoothedParameter::block_size_changed() noexcept
{
    values.setSize( block_size );
    values_modulated.setSize( block_size );
    buffer_is_linear_up_to_date_filled = false;
}

inline void SmoothedParameter::smooth( int glide_motor_time_in_samples, int num_samples_ ) noexcept
{
    {
        const float target = param_to_smooth->get_value();
        if( target != last_target )
        {
            samples_left = glide_motor_time_in_samples;
            difference_per_sample = (target - last_value) / samples_left;
            last_target = target;
        }
    }

    if( samples_left > 0 )
    {
        float* target_buffer = values.getWritePointer();
        if( samples_left >= num_samples_ )
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                target_buffer[i] = last_value += difference_per_sample;
            }
            samples_left -= num_samples_;
        }
        else
        {
            int i = 0;
            for(  ; i != samples_left ; ++i )
            {
                target_buffer[i] = last_value += difference_per_sample;
            }
            samples_left = 0;

            for(  ; i < num_samples_ ; ++i )
            {
                target_buffer[i] = last_target;
            }
            last_value = last_target;
        }

        buffer_is_linear_up_to_date_filled = false;
    }
    else if( not buffer_is_linear_up_to_date_filled )
    {
        float* target_buffer = values.getWritePointer();
        for( int i = 0 ; i != values.get_size() ; ++i )
        {
            target_buffer[i] = last_target;
        }

        last_value = last_target;
        buffer_is_linear_up_to_date_filled = true;
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD SmoothedParameter::ModulatorSignalSmoother::ModulatorSignalSmoother() noexcept :
samples_left_max(1), samples_left(0)
{
    reset(44100);
}
COLD SmoothedParameter::ModulatorSignalSmoother::~ModulatorSignalSmoother() noexcept {}

//==============================================================================
void SmoothedParameter::ModulatorSignalSmoother::reset( float sample_rate_ ) noexcept
{
    samples_left = msToSamplesFast( 20, sample_rate_ );
    samples_left_max = samples_left;
}
float SmoothedParameter::ModulatorSignalSmoother::attack( float current_modulator_signal_ ) noexcept
{
    if( samples_left > 0 )
    {
        samples_left--;
        current_modulator_signal_ *= 1.0f-(1.0f/samples_left_max*samples_left);
    }

    return current_modulator_signal_;
}
float SmoothedParameter::ModulatorSignalSmoother::release_amount() noexcept
{
    if( samples_left > 0 )
    {
        samples_left--;
        return (1.0f/samples_left_max*samples_left);
    }
    else
    {
        return 0;
    }
}
bool SmoothedParameter::ModulatorSignalSmoother::is_released() const noexcept
{
    return samples_left == 0;
}

//==============================================================================
//==============================================================================
//==============================================================================
inline void SmoothedParameter::process_modulation( const bool is_modulated_, const float*modulator_buffer_, int num_samples_ ) noexcept
{
    const float target_modulation = param_to_smooth->get_modulation_amount();
    // TODO SMOOTH MODULATION AMOUNT
    if( is_modulated_ and target_modulation != 0 )
    {
        if( not was_modulated_last_time )
        {
            modulator_smoother.reset(sample_rate);
            was_modulated_last_time = true;
        }

        float* target_buffer = values_modulated.getWritePointer();
        const float* source_buffer = values.getReadPointer();
        if( target_modulation > 0 )
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                float in = source_buffer[i];
                target_buffer[i] = in + modulator_smoother.attack( (max_value-in) * modulator_buffer_[i]*target_modulation );
            }
        }
        else
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                float in = source_buffer[i];
                target_buffer[i] = in + modulator_smoother.attack( (in-min_value) * modulator_buffer_[i]*target_modulation );
            }
        }

        last_modulator = target_buffer[num_samples_-1];
    }
    else
    {
        // ADD THE LAST MODULATOR OFFSET
        if( was_modulated_last_time )
        {
            modulator_smoother.reset(sample_rate);
            was_modulated_last_time = false;
        }

        float* target_buffer = values_modulated.getWritePointer();
        const float* source_buffer = values.getReadPointer();
        if( not modulator_smoother.is_released() )
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                float in = source_buffer[i];
                float release_power = modulator_smoother.release_amount();
                target_buffer[i] = in*(1.0f-release_power) + last_modulator*release_power;
            }
        }
        else
        {
            FloatVectorOperations::copy( target_buffer, source_buffer, num_samples_ );
        }
    }

    param_to_smooth->get_runtime_info().set_last_modulation_amount( modulator_buffer_[num_samples_-1]*target_modulation );
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD SmoothManager::SmoothManager(RuntimeNotifyer*const notifyer_) noexcept :
RuntimeListener(notifyer_), notifyer(notifyer_) {}
COLD SmoothManager::~SmoothManager() noexcept {}

//==============================================================================
inline void SmoothManager::smooth( int num_samples_, int glide_motor_time_in_ms_ ) noexcept
{
    int glide_motor_time_in_samples( msToSamplesFast(glide_motor_time_in_ms_, sample_rate) );
    for( int i = 0 ; i != smoothers.size() ; ++i )
    {
        smoothers.getUnchecked(i)->smooth( glide_motor_time_in_samples, num_samples_ );
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
class mono_ThreadManager;
class mono_ExecuterThread;
class mono_MultiThreaded
{
public:
    //==============================================================================
    inline mono_MultiThreaded() noexcept {}
    inline virtual ~mono_MultiThreaded() noexcept {}

private:
    //==============================================================================
    Thread* thread;

    friend class mono_ExecuterThread;   // MULTI THREADED EXECUTION
    friend class mono_ThreadManager; 	// CALLER THREAD EXECUTION

public:
    virtual void exec() noexcept = 0;

public:
    //==============================================================================
    inline bool isWorking() const noexcept
    {
        bool is_working;
        if( thread )
        {
            is_working = thread->isThreadRunning();
        }
        else
        {
            is_working = false;
        }

        return is_working;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_MultiThreaded)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_ExecuterThread : protected Thread
{
    friend class mono_ThreadManager;
    mono_MultiThreaded* executeable;

    //==============================================================================
    inline void run() override
    {
        executeable->exec();
    }

private:
    //==============================================================================
COLD mono_ExecuterThread() noexcept :
    Thread("monique_engine_WorkerThread") {}

public:
    //==============================================================================
    COLD ~mono_ExecuterThread() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ExecuterThread)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Thread;
class mono_ThreadManager
{
    Array< mono_ExecuterThread* > threads;
    CriticalSection cs;
    MoniqueSynthData*const synth_data;

private:
    //==============================================================================
    friend class mono_Thread;
    inline void execute_me( mono_MultiThreaded*const executer_ ) noexcept
    {
        const int num_threads = synth_data->num_extra_threads;
        if( num_threads > 0 )
        {
            // IF NO THREAD IS AVAILABLE, THE CALLER EXECUTES
            if( cs.tryEnter() )
            {
                for( int i = 0 ; i < num_threads ; ++i )
                {
                    mono_ExecuterThread*thread( threads[i] );
                    if( not thread->isThreadRunning() )
                    {
                        thread->executeable = executer_;
                        executer_->thread = thread;
                        thread->startThread();
                        cs.exit();
                        return;
                    }
                }
                cs.exit();
            }
        }

        executer_->thread = nullptr;
        executer_->exec();
    }

private:
    //==============================================================================
    friend class MoniqueSynthesiserVoice;
    COLD mono_ThreadManager( MoniqueSynthData*const synth_data_ ) noexcept
:
    synth_data( synth_data_ )
    {
        for( int i = 0 ; i < THREAD_LIMIT ; ++i )
        {
            mono_ExecuterThread*thread( new mono_ExecuterThread() );
            thread->setPriority(10);
            threads.add( thread );
        }
        threads.minimiseStorageOverheads();
    }
    COLD ~mono_ThreadManager() noexcept
    {
        for( int i = 0 ; i < THREAD_LIMIT ; ++i )
        {
            threads[i]->signalThreadShouldExit();
            threads[i]->waitForThreadToExit(200);
            delete threads[i];
        }
    }

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ThreadManager)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct mono_Thread : public mono_MultiThreaded
{
    mono_ThreadManager*const thread_manager;

    //==============================================================================
inline mono_Thread( mono_ThreadManager*const thread_manager_ ) noexcept :
    thread_manager(thread_manager_) {}
    inline ~mono_Thread() noexcept {}

    //==============================================================================
    // IT CHECKS FOR FREE THREADS, OTHERWISE IT RUNS FROM THE CALLER THREAD
    inline void try_run_paralel() noexcept
    {
        thread_manager->execute_me(this);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Thread)
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
class Smoother : RuntimeListener
{
    int pos;
    int size_in_ms;
    int size;
    mono_AudioSampleBuffer<1> buffer;
    float* ptr_to_buffer;
    float sum;

public:
    //==========================================================================
    inline void add( float val_ ) noexcept
    {
        sum-=ptr_to_buffer[pos];
        ptr_to_buffer[pos] = val_;
        sum+=val_;

        if( ++pos >= size )
        {
            pos = 0;
        }
    }
    inline float add_and_get_average( float val_ ) noexcept
    {
        add(val_);
        return get_average();
    }
    inline float get_average() const noexcept
    {
        return sum * ( 1.0f / size );
    }
    inline void reset( float value_ ) noexcept
    {
        sum = 0;
        for( int i = 0 ; i != size ; ++i )
        {
            ptr_to_buffer[i] = value_;
            sum += value_;
        }
    }
    void set_size_in_ms( int size_in_ms_, bool force_ = false ) noexcept
    {
        if( size_in_ms != size_in_ms_ or force_ )
        {
            float last_average = get_average();

            size_in_ms = size_in_ms_;
            size = jmax(int64(3),msToSamplesFast(size_in_ms_,sample_rate));
            buffer.setSize( size, true );
            ptr_to_buffer = buffer.getWritePointer();

            if( pos > size )
            {
                pos = 0;
            }

            sum = 0;
            for( int i = 0 ; i != size ; ++i )
            {
                ptr_to_buffer[i] = last_average;
                sum+=last_average;
            }
        }
    }

private:
    virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept override
    {
        set_size_in_ms(size_in_ms, true);
    };

public:
    //==========================================================================
COLD Smoother( RuntimeNotifyer*const notifyer_, int init_size_in_ms_ ) noexcept :
    RuntimeListener(notifyer_),
                    pos(0),
                    sum(0),
                    size_in_ms(init_size_in_ms_),
                    size( jmax(int64(3),msToSamplesFast(init_size_in_ms_,sample_rate)) ),
                    buffer( size ),
                    ptr_to_buffer( buffer.getWritePointer() )
    {
        reset(0);
    }
    COLD ~Smoother() {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Smoother)
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
static inline float soft_clipping( float input_and_worker_ ) noexcept
{
    return (std::atan(input_and_worker_)*(1.0f/float_Pi))*1.5;
}

//==============================================================================
static inline float lfo2amp( float sample_ ) noexcept
{
    return (sample_ + 1.0f)*0.5f;
}

//==============================================================================
static inline float distortion( float input_and_worker_, float distortion_power_ ) noexcept
{
    if( distortion_power_ != 0 )
    {
        const float distortion_add_on = distortion_power_*0.9f;
        input_and_worker_ = ((1.0f+distortion_add_on)*input_and_worker_) - (input_and_worker_*input_and_worker_*input_and_worker_)*distortion_add_on;
    }

    return input_and_worker_;
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float clipp_to_0_and_1( float input_and_worker_ ) noexcept
{
    if( input_and_worker_ > 1 )
    {
        input_and_worker_ = 1;
    }
    else if( input_and_worker_ < 0 )
    {
        input_and_worker_ = 0;
    }

    return input_and_worker_;
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float positive_sustain( float x ) noexcept
{
    return jmax(0.0001f,x < 0 ? x * -1 : x);
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float soft_clipp_greater_1_2( float x ) noexcept
{
    if( x > 1 )
    {
        x = 1.0f + soft_clipping( x - 1.0f );
    }
    else if( x < -1 )
    {
        x = -1.0f + soft_clipping( x + 1.0f );
    }

    if( x > 1.2 )
    {
        x = 1.2;
    }
    else if( x < -1.2 )
    {
        x = -1.2;
    }


    return x;
}
//==============================================================================
//==============================================================================
//==============================================================================
static inline float soft_clipp_greater_0_9( float x ) noexcept
{
    if( x > 0.9f )
    {
        x = 0.9f + soft_clipping( x - 0.9f );
    }
    else if( x < -0.9f )
    {
        x = -0.9f + soft_clipping( x + 0.9f );
    }

    if( x > 1 )
    {
        x = 1;
    }
    else if( x < -1 )
    {
        x = -1;
    }


    return x;
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float hard_clipper_1( float x ) noexcept
{
    if( x > 1 )
    {
        x = 1;
    }
    else if( x < -1 )
    {
        x = -1;
    }

    return x;
}

//==============================================================================
//==============================================================================
//==============================================================================
static float inline lookup(const float*table_, float x) noexcept
{
    return table_[ int(x*TABLESIZE_MULTI) % LOOKUP_TABLE_SIZE ];
}
//==============================================================================
COLD DataBuffer::DataBuffer( int init_buffer_size_ ) noexcept
:
size( init_buffer_size_ ),

      band_env_buffers( init_buffer_size_ ),
      band_out_buffers( init_buffer_size_ ),

      lfo_amplitudes( init_buffer_size_ ),
      filter_stereo_output_samples( init_buffer_size_ ),
      filter_output_samples_l_r( init_buffer_size_ ),

      osc_samples( init_buffer_size_ ),
      osc_switchs( init_buffer_size_ ),
      osc_sync_switchs( init_buffer_size_ ),
      modulator_samples( init_buffer_size_ ),

      final_env( init_buffer_size_ ),
      chorus_env( init_buffer_size_ ),

      filter_input_samples( init_buffer_size_ ),
      filter_input_env_amps( init_buffer_size_ ),
      filter_output_samples( init_buffer_size_ ),
      filter_env_amps( init_buffer_size_ ),

      tmp_buffer( init_buffer_size_ )
{
}
COLD DataBuffer::~DataBuffer() noexcept {}

//==============================================================================
COLD void DataBuffer::resize_buffer_if_required( int size_ ) noexcept
{
    if( size_ > size )
    {
        size = size_;

        band_env_buffers.setSize(size_);
        band_out_buffers.setSize(size_);

        lfo_amplitudes.setSize(size_);
        filter_stereo_output_samples.setSize(size_);
        filter_output_samples_l_r.setSize(size_);

        osc_samples.setSize(size_);
        osc_switchs.setSize(size_);
        osc_sync_switchs.setSize(size_);
        modulator_samples.setSize(size_);

        final_env.setSize(size_);
        chorus_env.setSize(size_);

        filter_input_samples.setSize(size_);
        filter_input_env_amps.setSize(size_);
        filter_output_samples.setSize(size_);
        filter_env_amps.setSize(size_);

        tmp_buffer.setSize(size_);
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
class PerfectCycleCounter : RuntimeListener
{
    //==========================================================================
    double frequency;

    double delta;
    double angle;

    double rate;
    double phase;

    double cylces_per_sec;

    bool new_cycle;

public:
    //==========================================================================
    inline void set_frequency( double frequency_ ) noexcept
    {
        if( frequency != frequency_ )
        {
            frequency = frequency_;
            cylces_per_sec = sample_rate/frequency_;

            delta = frequency_/sample_rate * (double_Pi+double_Pi);
            rate = (1.0 / cylces_per_sec) * double_Pi;
        }
    }
    inline void tick() noexcept
    {
        new_cycle = false;

        angle+=delta;
        phase+=rate;
        if( angle > (double_Pi+double_Pi) )
        {
            angle -= (double_Pi+double_Pi);
            phase -= (double_Pi+double_Pi);
            new_cycle = true;
        }
    }
    inline double get_last_angle() const noexcept
    {
        return angle;
    }
    inline double get_last_phase() const noexcept
    {
        return phase;
    }

    inline double get_cylces_per_sec() const noexcept
    {
        return cylces_per_sec;
    }
    inline bool is_new_cycle() const noexcept
    {
        return new_cycle;
    }

public:
    //==========================================================================
    COLD PerfectCycleCounter( RuntimeNotifyer*const notifyer_ ) noexcept
:
    RuntimeListener(notifyer_),
    frequency(0), delta(0), angle(0), rate(0), phase(0), cylces_per_sec(0), new_cycle(true) {}
    COLD ~PerfectCycleCounter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerfectCycleCounter)
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
class mono_BlitSaw : public RuntimeListener
{
    float phase_offset;
    double last_tick_value;
    double p_;
    double C2_;
    double phase_correction;
    double a_;
    double state_;

    unsigned int m_;

public:
    //==========================================================================
    inline float tick( double phase_ ) noexcept
    {
        double tmp;
        {
            const double working_phase_ = (phase_ + phase_offset) - phase_correction;

            // Avoid a divide by zero, or use of a denormalized divisor
            // at the sinc peak, which has a limiting value of m_ / p_.
            const double denominator = std::sin( working_phase_ );
            if ( std::fabs(denominator) <= std::numeric_limits<double>::epsilon() )
            {
                tmp = a_;
            }
            else
            {
                tmp = sin( working_phase_*m_ ) / (p_ * denominator);
            }
        }

        tmp += ( state_ - C2_ );
        state_ = tmp * 0.995;

        return last_tick_value = tmp;
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_phase_offset( float offset_ ) noexcept
    {
        phase_offset = offset_*float_Pi;
    }
    inline void updateHarmonics( double p ) noexcept
    {
        p_ = p;
        C2_ = 1.0 / p;
        phase_correction = C2_*(double_Pi*0.5) / 0.9;
        m_ = 2 * floor( 0.5 * p ) + 1;
        a_ = p / p;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        state_ = 0;
        last_tick_value = 0;
    }

public:
    //==========================================================================
    COLD mono_BlitSaw( RuntimeNotifyer*const notifyer_ ) noexcept
:
    RuntimeListener(notifyer_),
                    last_tick_value(0),
                    phase_offset(0),
                    p_(0),
                    C2_(0),
                    phase_correction(0),
                    a_(0),
                    state_(0),
                    m_(0)
    {
    }
    COLD ~mono_BlitSaw() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_BlitSaw)
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
//==============================================================================
class mono_BlitSquare : public RuntimeListener
{
    float last_tick_value;
    float phase_offset;
    double a_;
    double p_;
    double phase_correction;
    double lastBlitOutput_;
    double dcbState_;

    unsigned int m_;

public:
    //==========================================================================
    inline float tick( double phase_ ) noexcept
    {
        // OUTPUT PROCESS
        float out;
        {
            const double temp = lastBlitOutput_;
            {
                const double working_phase_ = (phase_ + phase_offset) - phase_correction;

                // Avoid a divide by zero, or use of a denomralized divisor
                // at the sinc peak, which has a limiting value of 1.0.
                {
                    const double denominator = std::sin( working_phase_ );
                    if ( std::fabs( denominator ) < std::numeric_limits<double>::epsilon() )
                    {
                        // Inexact comparison safely distinguishes betwen *close to zero*, and *close to PI*.
                        if( working_phase_ < 0.0001 or working_phase_ > (double_Pi+double_Pi) -0.0001 )
                        {
                            lastBlitOutput_ = a_;
                        }
                        else
                        {
                            lastBlitOutput_ = -a_;
                        }
                    }
                    else
                    {
                        lastBlitOutput_ = std::sin( working_phase_*m_ ) / (p_ * denominator);
                    }
                }
            }

            lastBlitOutput_ += temp;

            // Now apply DC blocker.
            out = lastBlitOutput_ - dcbState_ + 0.999 * last_tick_value;
            dcbState_ = lastBlitOutput_;
        }

        return last_tick_value = out;
    }
    inline float lastOut( void ) const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_phase_offset( float offset_ ) noexcept
    {
        phase_offset = offset_*float_Pi;
    }
    inline void updateHarmonics( double p ) noexcept
    {
        p_ = p * 0.5;
        m_ = (floor( 0.5 * p_ ) + 1) * 2;
        a_ = p / p;

        phase_correction = (1.0 / p) * double_Pi;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        last_tick_value = 0;
        dcbState_ = 0;
        lastBlitOutput_ = 0;
    }

public:
    //==========================================================================
    COLD mono_BlitSquare(RuntimeNotifyer*const notifyer_) noexcept
:
    RuntimeListener(notifyer_),
                    last_tick_value(0),
                    phase_offset(0),
                    a_(0),
                    p_(0),
                    phase_correction(0),
                    lastBlitOutput_(0),
                    dcbState_(0),
                    m_(0)
    {
    }
    COLD ~mono_BlitSquare() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_BlitSquare)
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
class mono_SineWave : public RuntimeListener
{
    const float*const sine_lookup;

    float last_tick_value;
    float phase_offset;

public:
    //==========================================================================
    inline float tick( double angle_ ) noexcept
    {
        return last_tick_value = lookup( sine_lookup, angle_+phase_offset );
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_phase_offset( float offset_ ) noexcept
    {
        phase_offset = offset_ * float_Pi;
    }

public:
    //==========================================================================
COLD mono_SineWave( RuntimeNotifyer*const notifyer_, const float* sine_lookup_ ) noexcept :
    RuntimeListener(notifyer_),
                    sine_lookup(sine_lookup_),
                    last_tick_value(0),
                    phase_offset(0)
    {}
    COLD ~mono_SineWave() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_SineWave)
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
class mono_SineWaveAutonom : public RuntimeListener
{
    const float*const sine_lookup;

    double frequency;

    double delta;
    double angle;

    bool new_cycle;

    float last_tick_value;

public:
    //==========================================================================
    inline float tick() noexcept
    {
        new_cycle = false;

        angle+=delta;
        if( angle > (double_Pi+double_Pi) )
        {
            angle -= (double_Pi+double_Pi);
            new_cycle = true;
        }

        return last_tick_value = lookup( sine_lookup, angle );
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_frequency( double frequency_ ) noexcept
    {
        if( frequency != frequency_ )
        {
            frequency = frequency_;
            delta = frequency_/sample_rate * (double_Pi+double_Pi);
        }
    }
    inline void overwrite_angle( float angle_ ) noexcept
    {
        angle = angle_;
    }

    //==========================================================================
    inline bool is_new_cycle() const noexcept
    {
        return new_cycle;
    }

public:
    //==========================================================================
    COLD mono_SineWaveAutonom( RuntimeNotifyer*const notifyer_, const float*const sine_lookup_ ) noexcept
:
    RuntimeListener( notifyer_ ),

                     sine_lookup(sine_lookup_),
                     frequency(0),

                     delta(0),
                     angle(0),

                     new_cycle(0),

                     last_tick_value(0)
    {}
    COLD ~mono_SineWaveAutonom() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_SineWaveAutonom)
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
class mono_SineWaveAutonomShifted : public RuntimeListener
{
    const float*const sine_lookup;

    double frequency;

    double delta;
    double angle;

    bool new_cycle;

    float last_tick_value;

public:
    //==========================================================================
    inline float tick() noexcept
    {
        new_cycle = false;

        angle+=delta;
        if( angle-(double_Pi*0.5) > (double_Pi+double_Pi) )
        {
            angle -= (double_Pi+double_Pi);
            new_cycle = true;
        }

        return last_tick_value = (lookup( sine_lookup, angle ) * -1 + 1)*0.5;
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_frequency( double frequency_ ) noexcept
    {
        if( frequency != frequency_ )
        {
            frequency = frequency_;
            delta = frequency_/sample_rate * (double_Pi+double_Pi);
        }
    }

    //==========================================================================
    inline bool is_new_cycle() const noexcept
    {
        return new_cycle;
    }

public:
    //==========================================================================
    COLD mono_SineWaveAutonomShifted( RuntimeNotifyer*const notifyer_, const float*const sine_lookup_ ) noexcept
:
    RuntimeListener(notifyer_),

                    sine_lookup(sine_lookup_),
                    frequency(0),

                    delta(0),
                    angle(double_Pi*0.5),

                    new_cycle(0),

                    last_tick_value(0)
    {}
    COLD ~mono_SineWaveAutonomShifted() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_SineWaveAutonomShifted)
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
class mono_Noise
{
    Random random;
    float last_tick_value;

public:
    //==========================================================================
    inline float tick() noexcept
    {
        return last_tick_value = random.nextFloat() * 2 - 1;
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

public:
    //==========================================================================
COLD mono_Noise() noexcept :
    last_tick_value(0) {}
    COLD ~mono_Noise() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Noise)
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
class mono_OnePole
{
    float last_tick_value;
    float gain;
    float a1, b;

public:
    //==========================================================================
    inline float tick( float input ) noexcept
    {
        return last_tick_value = b*gain*input - a1*last_tick_value;
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void setPole( float thePole ) noexcept
    {
        if ( thePole > 0 )
        {
            b = 1.0f - thePole;
        }
        else
        {
            b = 1.0f + thePole;
        }

        a1 = -thePole;
    }
    inline void setGain( float gain_ ) noexcept
    {
        gain = gain_;
    }

public:
    //==========================================================================
COLD mono_OnePole( float thePole = 0.9 ) noexcept:
    last_tick_value(0),
                    gain(0),
                    a1(0),
                    b(0)
    {
        this->setPole( thePole );
    }
    COLD ~mono_OnePole() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_OnePole)
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
class mono_Modulate : public RuntimeListener
{
    const float*const sine_lookup;

    mono_SineWaveAutonomShifted vibrato;
    mono_SineWaveAutonom swing;
    mono_OnePole filter;
    mono_Noise noise;

    float last_tick_value;
    float last_swing_frequency;
    unsigned int noiseRate;
    unsigned int noiseCounter;

public:
    //==========================================================================
    inline float tick() noexcept
    {
        // Compute periodic and random modulations.
        last_tick_value = vibrato.tick();
        if( ++noiseCounter > noiseRate )
        {
            noise.tick();
            noiseCounter = 0;
        }
        last_tick_value += filter.tick( noise.lastOut() );

        return last_tick_value * swing.tick();
    }
    inline float lastOut() const noexcept
    {
        return last_tick_value;
    }

    //==========================================================================
    inline void set_vibrato_frequency( float frequency_ ) noexcept
    {
        vibrato.set_frequency( frequency_ );
    }
    inline void set_swing_frequency( float frequency_ ) noexcept
    {
        if( last_swing_frequency != frequency_ )
        {
            if( frequency_ == 0 )
            {
                swing.overwrite_angle( float_Pi*0.5 );
            }

            last_swing_frequency = frequency_;
            swing.set_frequency( frequency_ );
        }
    }
    inline bool is_next_a_new_cycle() const noexcept
    {
        return vibrato.is_new_cycle();
    }

    //==========================================================================
    inline void reset() noexcept
    {
        last_tick_value = 0;
    }

public:
    //==========================================================================
    COLD mono_Modulate( RuntimeNotifyer*const notifyer_, const float*const sine_lookup_ ) noexcept
:
    RuntimeListener(notifyer_),

                    sine_lookup(sine_lookup_),

                    vibrato(notifyer_,sine_lookup_),
                    swing(notifyer_,sine_lookup_),

                    filter(),
                    noise(),

                    last_tick_value(0),
                    last_swing_frequency(0),
                    noiseRate(0),
                    noiseCounter(0)
    {
        vibrato.set_frequency( 6.0 );
        swing.set_frequency( 0 );
        swing.overwrite_angle( float_Pi*0.5 );

        noiseRate = (unsigned int) ( 330.0 * sample_rate / 22050.0 );
        noiseCounter = noiseRate;

        filter.setPole( 0.999 );
        filter.setGain( 0.05 );
    }
    COLD ~mono_Modulate() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Modulate)
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
class LFO : public RuntimeListener
{
    const float*const sine_lookup;

    mono_SineWave sine_generator;

    float last_speed;
    float samples_per_cylce;
    float last_out;

    float glide_samples;
    int glide_counter;
    float glide_value;

    const int id;

    DataBuffer*const data_buffer;
    const LFOData*const lfo_data;
    const RuntimeInfo*const runtime_info;

public:
    //==============================================================================
    inline void process( int step_number_, int start_pos_in_buffer_, int num_samples_ ) noexcept
    {
        const float speed( lfo_data->speed );
        if( speed != last_speed )
        {
            const float bars_per_sec = runtime_info->bpm/4/60;
            if( speed <= 17 )
            {
                const float cycles_per_sec = bars_per_sec/get_lfo_speed_multi( lfo_data->speed );
                samples_per_cylce = sample_rate / cycles_per_sec;
            }
            else
            {
                samples_per_cylce = sample_rate * (1.0f/midiToFrequency(33+speed-18));
            }

            glide_samples = glide_counter = sample_rate/10;
            glide_value = last_out;
            last_speed = speed;
        }

        int64 sync_sample_pos = runtime_info->samples_since_start+start_pos_in_buffer_;
        float* process_buffer( data_buffer->lfo_amplitudes.getWritePointer(id) );
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            const float current_angle_samples = fmod(sync_sample_pos+sid,samples_per_cylce);
            float amp = lfo2amp( lookup( sine_lookup, (2.0f*float_Pi)*(1.0f/samples_per_cylce*current_angle_samples)) );
            if( --glide_counter > 0 )
            {
                float glide = (1.0f/glide_samples*glide_counter);
                amp = amp*(1.0f-glide) + glide_value*glide;
            }
            process_buffer[sid] = amp;
        }
        last_out = process_buffer[num_samples_-1];
    }
    void sample_rate_changed( double ) noexcept
    {
        last_speed = -1;
    }

    //==========================================================================
    float get_current_amp() const noexcept
    {
        return last_out;
    }

    //==========================================================================
    static float get_lfo_speed_multi( float speed_ ) noexcept
    {
        float factor = 1;
        if( speed_ <= 6 )
        {
            factor = 1;
            if( speed_ == 0 )
                factor = 16; //return "16/1";
            else if( speed_ <= 1 )
                factor = 12; //return "12/1";
            else if( speed_ <= 2 )
                factor = 8;
            else if( speed_ <= 3 )
                factor = 4;
            else if( speed_ <= 4 )
                factor = 3;
            else if( speed_ <= 5 )
                factor = 2;
        }
        else if( speed_ <= 17 )
        {
            factor = 0;
            if( speed_ <= 7 )
                factor = 3.0f/4;
            else if( speed_ <= 8 )
                factor = 1.0f/2;
            else if( speed_ <= 9 )
                factor = 1.0f/3;
            else if( speed_ <= 10 )
                factor = 1.0f/4;
            else if( speed_ <= 11 )
                factor = 1.0f/8;
            else if( speed_ <= 12 )
                factor = 1.0f/12;
            else if( speed_ <= 13 )
                factor = 1.0f/16;
            else if( speed_ <= 14 )
                factor = 1.0f/24;
            else if( speed_ <= 15 )
                factor = 1.0f/32;
            else if( speed_ <= 16 )
                factor = 1.0f/64;
            else
                factor = 1.0f/128;
        }

        return factor;
    }

private:
    //==========================================================================
    friend class MoniqueSynthesiserVoice;
    COLD LFO( RuntimeNotifyer*const notifyer_, MoniqueSynthData*synth_data_, int id_, const float*const sine_lookup_ ) noexcept
:
    RuntimeListener(notifyer_),

                    sine_lookup( sine_lookup_ ),
                    sine_generator( notifyer_, sine_lookup_ ),

                    last_speed(0),
                    samples_per_cylce(100),
                    last_out(0),

                    glide_samples(0),
                    glide_counter(0),
                    glide_value(0),

                    id( id_ ),

                    data_buffer( synth_data_->data_buffer ),
                    lfo_data( synth_data_->lfo_datas[id_] ),
                    runtime_info( synth_data_->runtime_info )
    {}
    COLD ~LFO() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFO)
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
class MasterOSC : public RuntimeListener
{
    // RUNTIME
    //==============================================================================
    int freq_glide_samples_left;
    float freq_glide_delta;

    float root_note;

    float last_root_note;
    float last_frequency;

    float last_modulator_frequency;
    int modulator_sync_cylces;
    int modulator_run_circle;
    bool modulator_waits_for_sync_cycle;

    // RAW OSCILATORS
    //==============================================================================
    PerfectCycleCounter cycle_counter;
    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;
    mono_Noise noise;

    mono_Modulate modulator;

    // DATA SOURCE
    //==============================================================================
    DataBuffer*const data_buffer;
    const MoniqueSynthData*const synth_data;
    FMOscData*const fm_osc_data;
    OSCData*const osc_data;

public:
    //==============================================================================
    inline void process(DataBuffer* data_buffer_, const int num_samples_) noexcept
    {
        float*const output_buffer( data_buffer->osc_samples.getWritePointer(MASTER_OSC) );

        float*const switch_buffer( data_buffer->osc_switchs.getWritePointer() );
        FloatVectorOperations::clear(switch_buffer,num_samples_);
        float*const modulator_buffer( data_buffer->modulator_samples.getWritePointer(MASTER_OSC) );
        const float*const lfo_amps( ( data_buffer->lfo_amplitudes.getReadPointer(MASTER_OSC) ) );

        const bool is_lfo_modulated = osc_data->is_lfo_modulated;
        const bool fm_sync = fm_osc_data->sync;
        const bool fm_shot = fm_osc_data->fm_shot;

        fm_osc_data->master_shift_smoother.process_modulation( is_lfo_modulated, lfo_amps, num_samples_ );
        const float*const smoothed_tune_buffer( osc_data->tune_smoother.get_smoothed_modulated_buffer() );
        const float*const smoothed_fm_amount_buffer( osc_data->fm_amount_smoother.get_smoothed_buffer() );
        const float*const smoothed_wave_buffer( osc_data->wave_smoother.get_smoothed_buffer() );
        const float*const smoothed_fm_freq_buffer( fm_osc_data->fm_freq_smoother.get_smoothed_buffer() );
        const float*const smoothed_fm_swing_buffer( fm_osc_data->fm_swing_smoother.get_smoothed_buffer() );
        const float*const smoothed_phase_offset( fm_osc_data->master_shift_smoother.get_smoothed_modulated_buffer() );

        for( int sid = 0 ; sid < num_samples_ ; ++sid )
        {
            // SETUP TUNE
            {
                // BASE FREQUENCY
                bool base_frequency_changed = false;
                if( freq_glide_samples_left > 0 or last_root_note != root_note )
                {
                    if( freq_glide_samples_left > 0 )
                    {
                        --freq_glide_samples_left;
                    }

                    {
                        last_root_note = root_note;

                        const float new_frequence = jmax( 5.0f, midiToFrequencyFast( root_note + freq_glide_delta*freq_glide_samples_left ) );
                        if( new_frequence != last_frequency )
                        {
                            cycle_counter.set_frequency( new_frequence );
                            square_generator.updateHarmonics( cycle_counter.get_cylces_per_sec() );
                            saw_generator.updateHarmonics( cycle_counter.get_cylces_per_sec() );


                            last_frequency = new_frequence;
                        }
                    }

                    base_frequency_changed = true;
                }

                // MODULATOR FREQUENCY
                {
                    const float modulator_freq = smoothed_fm_freq_buffer[sid];
                    if( base_frequency_changed or modulator_freq != last_modulator_frequency )
                    {
                        last_modulator_frequency = modulator_freq;

                        modulator.set_vibrato_frequency( last_frequency*(modulator_freq*7 +1.1) );

                        modulator_sync_cylces = mono_floor(modulator_freq*7);
                    }
                }

                // ADD PHASE SHIFT
                square_generator.set_phase_offset(smoothed_phase_offset[sid]);
                saw_generator.set_phase_offset(smoothed_phase_offset[sid]);
                sine_generator.set_phase_offset(smoothed_phase_offset[sid]);
            }

            // PROCESS OSC ITSELF
            bool is_last_sample_of_cycle = false;
            float sample;
            {
                // TICK ALL OSCILATTORS
                {
                    cycle_counter.tick();
                    saw_generator.tick( cycle_counter.get_last_phase() );
                    {
                        double angle = cycle_counter.get_last_angle();
                        square_generator.tick( angle );
                        sine_generator.tick( angle );
                    }

                    // FORCE SYNC OF THE SINE AT THE NEXT SID
                    if( cycle_counter.is_new_cycle() )
                    {
                        // SET THE SWITCH
                        is_last_sample_of_cycle = true;
                        switch_buffer[sid] = true;
                    }
                }

                // CALC THE SAMPLE MIX
                {
                    const float wave_form = smoothed_wave_buffer[sid];

                    // SINE - SQUARE
                    if( wave_form <= 1 )
                    {
                        const float multi = wave_form;
                        const float sine_wave_powerd = sine_generator.lastOut() * (1.0f-multi);
                        const float square_wave_powerd = square_generator.lastOut() * multi;
                        sample = sine_wave_powerd+square_wave_powerd;
                    }
                    // SQUARE - SAW
                    else if( wave_form <= 2 )
                    {
                        const float multi = wave_form - 1;
                        const float square_wave_powerd = square_generator.lastOut() * (1.0f-multi);
                        const float saw_wave_powerd = saw_generator.lastOut() * multi;
                        sample = square_wave_powerd+saw_wave_powerd;
                    }
                    // SAW - RAND
                    else /*if( wave_form <= 3 )*/
                    {
                        const float multi = wave_form - 2;
                        const float saw_wave_powerd = saw_generator.lastOut() * (1.0f-multi);
                        const float noice_powerd = noise.tick()* multi;
                        sample = saw_wave_powerd+noice_powerd;
                    }
                }
            }

            // PROCESS MODULATOR
            float modulator_sample = 0;
            {
                // MODULATOR SYNC AND PROCESSING
                if( not modulator_waits_for_sync_cycle )
                {
                    modulator_sample = modulator.tick();
                }

                const bool is_last_sample_of_modulator_cycle = modulator.is_next_a_new_cycle();
                if( fm_shot and is_last_sample_of_modulator_cycle )
                {
                    modulator_waits_for_sync_cycle = not is_last_sample_of_cycle;
                }

                // STOP THE MODULATOR IF IN SYNC // IGNORED IF SHOT IS ENABLED
                if( modulator_run_circle > modulator_sync_cylces and fm_sync and not fm_shot  )
                {
                    modulator_waits_for_sync_cycle = not is_last_sample_of_cycle;
                    modulator_run_circle = 0;
                }
                else if( is_last_sample_of_cycle and not fm_shot )
                {
                    modulator_waits_for_sync_cycle = false;
                    modulator_run_circle = 0;
                }

                // COUNT CYCLES AND CLEAR STATE IF NOT WAITING FOR SYNC
                if( is_last_sample_of_modulator_cycle and not modulator_waits_for_sync_cycle )
                {
                    ++modulator_run_circle;
                }

                // UPDATE SWING
                if( is_last_sample_of_modulator_cycle )
                {
                    modulator.set_swing_frequency( smoothed_fm_swing_buffer[sid]*20 );
                }
            }

            // ADD FM TO THE OUTPUT
            {
                if( const float fm_amount = smoothed_fm_amount_buffer[sid] )
                {
                    sample = sample*(1.0f-fm_amount) + ( (modulator_sample+sample)*0.5 )*fm_amount;
                }

                output_buffer[sid] = sample;
                modulator_buffer[sid] = modulator_sample;
            }
        }
    }

    //==============================================================================
    inline void update( float root_note_ ) noexcept
    {
        root_note_ += synth_data->octave_offset *12;
        const float glide = synth_data->glide;
        if( glide != 0 and (root_note != root_note_ || freq_glide_samples_left > 0 ) )
        {
            root_note_ = jmax( 1.0f, jmin(127.0f,root_note_) );

            const float rest_glide = freq_glide_delta*freq_glide_samples_left;
            freq_glide_samples_left = (sample_rate*0.5) * glide;
            const float glide_notes_diverence = root_note - root_note_;
            freq_glide_delta = (glide_notes_diverence+rest_glide) / freq_glide_samples_left;
        }

        root_note = root_note_;
    }
    inline void reset() noexcept
    {
        saw_generator.reset();
        square_generator.reset();

        modulator.reset();
    }

public:
    //==============================================================================
    COLD MasterOSC( RuntimeNotifyer*const notifyer_, const MoniqueSynthData* synth_data_, const float*const sine_lookup_ ) noexcept
:
    RuntimeListener( notifyer_ ),

                     cycle_counter( notifyer_ ),
                     saw_generator( notifyer_ ),
                     square_generator( notifyer_ ),
                     sine_generator( notifyer_, sine_lookup_ ),
                     noise(),

                     modulator( notifyer_, sine_lookup_ ),

                     freq_glide_samples_left(0),
                     freq_glide_delta(0),

                     root_note(60),

                     last_frequency(0),

                     last_modulator_frequency(0),
                     modulator_sync_cylces(0),
                     modulator_waits_for_sync_cycle(false),


                     data_buffer( synth_data_->data_buffer ),
                     synth_data( synth_data_ ),
                     fm_osc_data( synth_data_->fm_osc_data ),
                     osc_data( synth_data_->osc_datas[MASTER_OSC] )
    {}
    COLD ~MasterOSC() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterOSC)
};

//===================================================================================
//===================================================================================
//===================================================================================
class SecondOSC : public RuntimeListener
{
    const int id;

    // RUNTIME
    //==============================================================================
    int freq_glide_samples_left;
    float freq_glide_delta;

    float root_note;
    float last_root_note;
    float last_tune;
    float last_frequency;

    bool wait_for_new_master_cycle;
    float last_sync_was_to_tune;

    // RAW OSCILATORS
    //==============================================================================
    PerfectCycleCounter cycle_counter;
    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;
    mono_Noise noise;

    // DATA SOURCE
    //==============================================================================
    DataBuffer*const data_buffer;
    const MoniqueSynthData*const synth_data;
    OSCData*const osc_data;
    OSCData*const master_osc_data;

public:
    //==============================================================================
    static inline bool is_syncanble_by_tune( float tune_, float master_tune_ ) noexcept
    {
        bool syncable = false;
        if( tune_ == master_tune_ )
        {
            syncable = true;
        }
        else if( tune_ == master_tune_ +24 )
        {
            syncable = true;
        }
        else if( tune_ == master_tune_ -24 )
        {
            syncable = true;
        }

        return syncable;
    }

    //==============================================================================
    inline void process(DataBuffer* data_buffer_, const int num_samples_) noexcept
    {
        float*const output_buffer( data_buffer->osc_samples.getWritePointer(id) );

        const float*const switch_buffer( data_buffer->osc_switchs.getWritePointer() );
        const float*const modulator_buffer( data_buffer->modulator_samples.getWritePointer(MASTER_OSC) );
        const float*const lfo_amps( ( data_buffer->lfo_amplitudes.getReadPointer(id) ) );

        const bool is_lfo_modulated = osc_data->is_lfo_modulated;

        const bool sync_to_master = osc_data->sync;
        osc_data->tune_smoother.process_modulation( is_lfo_modulated, lfo_amps, num_samples_ );
        const float*const smoothed_tune_buffer( osc_data->tune_smoother.get_smoothed_modulated_buffer() );
        const float*const smoothed_master_tune_buffer( master_osc_data->tune_smoother.get_smoothed_modulated_buffer() );
        const float*const smoothed_fm_amount_buffer( osc_data->fm_amount_smoother.get_smoothed_buffer() );
        const float*const smoothed_wave_buffer( osc_data->wave_smoother.get_smoothed_buffer() );

        for( int sid = 0 ; sid < num_samples_ ; ++sid )
        {
            // SETUP TUNE
            const float tune = smoothed_tune_buffer[sid];
            {
                // FREQUENCY
                if( freq_glide_samples_left > 0 or tune != last_tune or last_root_note != root_note )
                {
                    if( freq_glide_samples_left > 0 )
                    {
                        --freq_glide_samples_left;
                    }

                    {
                        last_tune = tune;
                        last_root_note = root_note;

                        const float new_frequence = jmax( 5.0f, midiToFrequencyFast( root_note + tune + freq_glide_delta*freq_glide_samples_left ) );
                        if( new_frequence != last_frequency )
                        {
                            cycle_counter.set_frequency(new_frequence);
                            square_generator.updateHarmonics( cycle_counter.get_cylces_per_sec() );
                            saw_generator.updateHarmonics( cycle_counter.get_cylces_per_sec() );
                        }
                    }
                }
            }

            // PROCESS OSC AND SYNC IT TO THE MASTER
            const bool is_last_sample_of_master_cycle = switch_buffer[sid];
            float sample = 0;
            {
                // TICK ALL OSCILATTORS
                if( not wait_for_new_master_cycle )
                {
                    cycle_counter.tick();
                    saw_generator.tick( cycle_counter.get_last_phase() );
                    {
                        double angle = cycle_counter.get_last_angle();
                        square_generator.tick( angle );
                        sine_generator.tick( angle );
                    }
                }

                // FORCE SYNC OF THE SINE AT THE NEXT SID
                if( cycle_counter.is_new_cycle() )
                {
                    const bool syncanble_by_tune = is_syncanble_by_tune(tune, smoothed_master_tune_buffer[sid]);
                    if( not syncanble_by_tune )
                    {
                        last_sync_was_to_tune = -25;
                    }
                    if( sync_to_master )
                    {
                        if( not is_lfo_modulated and not syncanble_by_tune and last_sync_was_to_tune != tune )
                        {
                            wait_for_new_master_cycle = true;
                        }
                    }
                    else
                    {
                        if( not is_lfo_modulated and freq_glide_samples_left <= 0 )
                        {
                            if( syncanble_by_tune and last_sync_was_to_tune != tune )
                            {
                                wait_for_new_master_cycle = true;
                                last_sync_was_to_tune = tune;
                            }
                        }
                    }
                }
                if( is_last_sample_of_master_cycle )
                {
                    wait_for_new_master_cycle = false;
                }

                // CALC THE SAMPLE MIX
                if( not wait_for_new_master_cycle )
                {
                    const float wave_form = smoothed_wave_buffer[sid];

                    // SINE - SQUARE
                    if( wave_form <= 1 )
                    {
                        const float multi = wave_form;
                        const float sine_wave_powerd = sine_generator.lastOut() * (1.0f-multi);
                        const float square_wave_powerd = square_generator.lastOut() * multi;
                        sample = sine_wave_powerd+square_wave_powerd;
                    }
                    // SQUARE - SAW
                    else if( wave_form <= 2 )
                    {
                        const float multi = wave_form - 1;
                        const float square_wave_powerd = square_generator.lastOut() * (1.0f-multi);
                        const float saw_wave_powerd = saw_generator.lastOut() * multi;
                        sample = square_wave_powerd+saw_wave_powerd;
                    }
                    // SAW - RAND
                    else /*if( wave_form <= 3 )*/
                    {
                        const float multi = wave_form - 2;
                        const float saw_wave_powerd = saw_generator.lastOut() * (1.0f-multi);
                        const float noice_powerd = noise.tick()* multi;
                        sample = saw_wave_powerd+noice_powerd;
                    }
                }
            }

            // ADD FM TO THE OUTPUT
            {
                float modulator_sample = modulator_buffer[sid];
                if( const float fm_amount = smoothed_fm_amount_buffer[sid] )
                {
                    sample = sample*(1.0f-fm_amount) + ( (modulator_sample+sample)*0.5 )*fm_amount;
                }

                output_buffer[sid] = sample;
            }
        }
    }

    //==============================================================================
    inline void update( float root_note_ ) noexcept
    {
        root_note_ += synth_data->octave_offset *12;
        const float glide = synth_data->glide;
        if( glide != 0 and (root_note != root_note_ || freq_glide_samples_left > 0 ) )
        {
            root_note_ = jmax( 1.0f, jmin(127.0f,root_note_) );

            const float rest_glide = freq_glide_delta*freq_glide_samples_left;
            freq_glide_samples_left = (sample_rate*0.5) * glide;
            const float glide_notes_diverence = root_note - root_note_;
            freq_glide_delta = (glide_notes_diverence+rest_glide) / freq_glide_samples_left;
        }

        root_note = root_note_;
    }
    inline void reset() noexcept
    {
        saw_generator.reset();
        square_generator.reset();
    }

public:
    //==============================================================================
    COLD SecondOSC( RuntimeNotifyer*const notifyer_, const MoniqueSynthData* synth_data_, int id_, const float*const sine_lookup_) noexcept
:
    RuntimeListener( notifyer_ ),

                     id(id_),

                     cycle_counter( notifyer_ ),
                     saw_generator( notifyer_ ),
                     square_generator( notifyer_ ),
                     sine_generator( notifyer_, sine_lookup_ ),
                     noise(),

                     freq_glide_samples_left(0),
                     freq_glide_delta(0),

                     root_note(60),

                     last_tune(-25),
                     last_frequency(0),

                     wait_for_new_master_cycle(false),
                     last_sync_was_to_tune(-25),

                     data_buffer( synth_data_->data_buffer ),
                     synth_data( synth_data_ ),
                     osc_data( synth_data_->osc_datas[id_] ),
                     master_osc_data( synth_data_->osc_datas[MASTER_OSC] )
    {
    }
    COLD ~SecondOSC() noexcept
    {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SecondOSC)
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
class mono_ENVOsccilator : public RuntimeListener
{
    const float*const sine_lookup;
    const float*const cos_lookup;
    const float*const exp_lookup;

    float start_amp;
    float out_amp;
    float target_amp;
    enum TYPE
    {
        ATTACK,
        RELEASE,
        KEEP
    };
    TYPE type;

    float delta;
    float angle;
    float sine_angle_start;

    float shape;
    float time_in_samples;
    int sample_counter;
    bool is_unlimited;

public:
    //==========================================================================
    // SHAPE 0 - sine-, 1 - linear, 2 - sine+
    inline void set_process_values( float start_value_, float end_value_, float shape_, float time_in_ms_ ) noexcept
    {
        start_amp = start_value_;
        target_amp = end_value_;
        shape = shape_;
        time_in_samples = msToSamplesFast( time_in_ms_, sample_rate );
    }
    inline void calculate_attack_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        angle = 0;
        sine_angle_start = float_Pi*1.5f;
        type = TYPE::ATTACK;
        is_unlimited = false;
        delta = float_Pi / sample_counter;
    }
    inline void calculate_release_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        angle = 0;
        sine_angle_start = float_Pi*1.5f;
        type = TYPE::RELEASE;
        is_unlimited = false;
        delta = float_Pi / sample_counter;
    }
    inline void calculate_keep_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        delta = 0;
        angle = 0;
        type = TYPE::KEEP;
        is_unlimited = false;
    }
    inline void calculate_unlimited_coeffecients() noexcept
    {
        sample_counter = 0;
        delta = 0;
        angle = 0;
        type = TYPE::KEEP;
        is_unlimited = true;
    }
    inline void only_calculate_new_target( float target_ ) noexcept
    {
        start_amp = out_amp;
        target_amp = target_;
        sample_counter = time_in_samples;
        angle = 0;
        delta = float_Pi / sample_counter;
    }

public:
    //==========================================================================
    inline float tick() noexcept
    {
        if( --sample_counter > 0 )
        {
            if( delta != 0 )
            {
                angle += delta;
                /*shape*(angle-cos_for_angle)*/
                const float angle_drift = lookup(exp_lookup, angle)*angle*shape + (angle+lookup(cos_lookup, angle+sine_angle_start))*(1.0f-shape);
                if( type == TYPE::ATTACK )
                {
                    out_amp = start_amp + (( lookup(sine_lookup, angle_drift+sine_angle_start) + 1 ) * 0.5f) * (target_amp-start_amp);
                }
                else if( type == TYPE::RELEASE )
                {
                    out_amp = start_amp - (( lookup(sine_lookup, angle_drift+sine_angle_start) + 1 ) * 0.5f) * (start_amp-target_amp);
                }
            }
        }
        return out_amp;
    }

public:
    //==========================================================================
    inline float last_out() const noexcept
    {
        return out_amp;
    }
    inline void overwrite_current_value(float amp_) noexcept
    {
        out_amp = amp_;
    }

    //==========================================================================
    inline bool is_finished() const noexcept
    {
        return is_endless() and sample_counter <= 0;
    }
    inline bool is_endless() const noexcept
    {
        return not is_unlimited;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        sample_counter = 0;
        start_amp = 0;
        out_amp = 0;
        type = KEEP;
        target_amp = 0;
        delta = 0;
        angle = 0;
        time_in_samples = 0;
        sample_counter = 0;
        is_unlimited = false;
    }
public:
    //==========================================================================
    COLD mono_ENVOsccilator( RuntimeNotifyer*const notifyer_,
                             const float*const sine_lookup_,
                             const float*const cos_lookup_,
                             const float*const exp_lookup_ ) noexcept

:
    RuntimeListener( notifyer_ ),
                     sine_lookup( sine_lookup_ ),
                     cos_lookup( cos_lookup_ ),
                     exp_lookup( exp_lookup_ ),

                     start_amp(0),
                     out_amp(0),
                     target_amp(0),
                     type(KEEP),

                     delta(0),
                     angle(0),
                     sine_angle_start(0),

                     shape(0),

                     time_in_samples(0),
                     sample_counter(0),
                     is_unlimited(false)
    {}
    COLD ~mono_ENVOsccilator() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ENVOsccilator)
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
enum STAGES
{
    END_ENV = false,
    ATTACK = true,
    DECAY,
    SUSTAIN,
    RELEASE,
    TRIGGER_START
};
class ENV
{
    mono_ENVOsccilator env_osc;

    STAGES current_stage;

    const MoniqueSynthData*const synth_data;
    float last_sustain;
    bool goes_to_sustain;
    bool is_sustain;

public:
    ENVData*const env_data;

public:
    //==========================================================================
    inline void process( float* dest_, const int num_samples_ ) noexcept
    {
        const float* smoothed_sustain_buffer = env_data->sustain_smoother.get_smoothed_buffer();
        for( int sid = 0 ; sid < num_samples_ ; ++sid )
        {
            if( last_sustain != smoothed_sustain_buffer[sid] )
            {
                last_sustain = smoothed_sustain_buffer[sid];
                if( goes_to_sustain )
                {
                    env_osc.only_calculate_new_target(positive(last_sustain));
                }
                else if( is_sustain )
                {
                    env_osc.overwrite_current_value(positive(last_sustain));
                }
            }
            dest_[sid]  = env_osc.tick();

            if( env_osc.is_finished() )
            {
                update_stage( sid );
            }
        }
    }

private:
    inline void update_stage( int sid_ ) noexcept
    {
        const float* smoothed_sustain_buffer = env_data->sustain_smoother.get_smoothed_buffer();
        switch( current_stage )
        {
        case TRIGGER_START : // --> ATTACK
        {
            {
                if( env_data->decay > 0 )
                {
                    env_osc.set_process_values
                    (
                        env_osc.last_out(), 1,
                        env_data->shape,
                        env_data->attack*env_data->max_attack_time+MIN_ENV_TIMES
                    );
                    goes_to_sustain = false;
                    is_sustain = false;
                }
                else
                {
                    env_osc.set_process_values
                    (
                        env_osc.last_out(), positive_sustain(smoothed_sustain_buffer[sid_]),
                        env_data->shape,
                        env_data->attack*env_data->max_attack_time+MIN_ENV_TIMES
                    );
                    goes_to_sustain = true;
                    is_sustain = false;
                }
                env_osc.calculate_attack_coeffecients();
            }
            current_stage = ATTACK;

            break;
        }
        case ATTACK : // --> DECAY
        {
            if( env_data->decay > 0 )
            {
                env_osc.set_process_values
                (
                    env_osc.last_out(), positive_sustain(smoothed_sustain_buffer[sid_]),
                    env_data->shape,
                    env_data->decay*env_data->max_decay_time
                );
                goes_to_sustain = true;
                is_sustain = false;
                env_osc.calculate_release_coeffecients();

                current_stage = DECAY;

                break;
            }
            // ELSE FALL TO DECAY AN START SUSTAIN LEVEL
        }
        case DECAY : // --> SUSTAIN
        {
            float sustain_time = env_data->sustain_time;
            float sustain_level = positive_sustain(smoothed_sustain_buffer[sid_]);
            env_osc.set_process_values
            (
                env_osc.last_out(), sustain_level,
                env_data->shape,
                sustain_time*10000
            );
            if( sustain_time >= 1.0f )
            {
                env_osc.calculate_unlimited_coeffecients();
            }
            else
            {
                env_osc.calculate_keep_coeffecients();
            }
            goes_to_sustain = false;
            is_sustain = true;
            current_stage = SUSTAIN;

            break;
        }
        case SUSTAIN : // --> RELEASE
        {
            env_osc.set_process_values
            (
                env_osc.last_out(), 0,
                env_data->shape,
                env_data->release*env_data->max_release_time+MIN_ENV_TIMES
            );
            goes_to_sustain = false;
            is_sustain = false;
            env_osc.calculate_release_coeffecients();

            current_stage = RELEASE;

            break;
        }
        case RELEASE : // --> RELEASE
        {
            goes_to_sustain = false;
            is_sustain = false;
            current_stage = END_ENV;

            break;
        }
        default:
        {
            goes_to_sustain = false;
            is_sustain = false;
            current_stage = END_ENV;
        }
        }
    }

public:
    //==========================================================================
    inline void start_attack() noexcept
    {
        current_stage = TRIGGER_START;
        update_stage( 0 );
    }
    inline void set_to_release() noexcept
    {
        current_stage = SUSTAIN;
        update_stage( 0 );
    }
    inline void reset() noexcept
    {
        current_stage = END_ENV;
        env_osc.reset();
    }

    //==============================================================================
    inline STAGES get_current_stage() const noexcept
    {
        return current_stage;
    }
    inline void set_current_stage( STAGES current_stage_ ) noexcept
    {
        current_stage = current_stage_;
    }
    inline void overwrite_current_value( float amp_ ) noexcept
    {
        env_osc.overwrite_current_value( amp_ );
    }
    inline bool is_endless() noexcept
    {
        return env_osc.is_endless();
    }

    //==============================================================================
    inline float get_amp() const noexcept
    {
        return env_osc.last_out();
    }

public:
    //==============================================================================
    inline ENV( RuntimeNotifyer*const notifyer_,
                const MoniqueSynthData* synth_data_,
                ENVData* env_data_,
                const float*const sine_lookup_,
                const float*const cos_lookup_,
                const float*const exp_lookup_ ) noexcept
:
    env_osc( notifyer_, sine_lookup_, cos_lookup_, exp_lookup_ ),

             current_stage(END_ENV),

             synth_data( synth_data_ ),
             env_data( env_data_ ),

             last_sustain(0),
             goes_to_sustain(false),
             is_sustain(false)
    {
    }
    inline ~ENV() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENV)
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
//==============================================================================
//==============================================================================
//==============================================================================
void SmoothedParameter::process_amp( bool use_env_, ENV*env_, float*amp_buffer_, int num_samples_ ) noexcept
{
    if( use_env_ )
    {
        if( not was_automated_last_time )
        {
            int current_stage = env_->get_current_stage();
            if( current_stage == RELEASE or current_stage == END_ENV )
            {
                env_->overwrite_current_value( last_amp_valued );
                env_->set_to_release();
            }
            else if( current_stage == ATTACK )
            {
                env_->overwrite_current_value( last_amp_valued );
                env_->start_attack();
            }
            else if( current_stage == SUSTAIN or current_stage == DECAY )
            {
                env_->overwrite_current_value( last_amp_valued );
            }
            was_automated_last_time = true;
        }
        env_->process( amp_buffer_, num_samples_ );
        last_amp_automated = amp_buffer_[num_samples_-1];
    }
    else
    {
        if( was_automated_last_time )
        {
            was_automated_last_time = false;

            amp_switch_samples_left = msToSamplesFast( 20, sample_rate );
            amp_switch_samples_left_max = amp_switch_samples_left;
        }

        const float* source = values.getReadPointer();
        if( amp_switch_samples_left > 0 )
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                amp_switch_samples_left--;
                if( amp_switch_samples_left > 0 )
                {
                    float l_r_power = (1.0f/amp_switch_samples_left_max*amp_switch_samples_left);
                    amp_buffer_[i] = positive(source[i]*(1.0f-l_r_power)) + last_amp_automated*l_r_power;
                }
                else
                {
                    amp_buffer_[i] = positive(source[i]);
                }
            }
        }
        else
        {
            for( int i = 0 ; i != num_samples_ ; ++i )
            {
                amp_buffer_[i] = positive(source[i]);
            }
        }
        last_amp_valued = amp_buffer_[num_samples_-1];
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
template< int smooth_samples >
class AmpSmoother
{
    float current_value;
    float target_value;
    float delta;
    int counter;

private:
    inline void glide_tick() noexcept;

public:
    inline float add_get( float in_ ) noexcept;
    bool is_up_to_date() const noexcept;
    inline float add_get_and_keep_current_time( float in_ ) noexcept;
    inline void reset( float value_ = 0 ) noexcept;
    inline float get_current_value() const noexcept;

    COLD AmpSmoother( float start_value_ = 0 ) noexcept;
    COLD ~AmpSmoother() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpSmoother)
};

//==============================================================================
template< int smooth_samples >
COLD AmpSmoother<smooth_samples>::AmpSmoother( float start_value_ ) noexcept
:
current_value(0),
              target_value(start_value_),
              delta(0),
              counter(0)
{}
template< int smooth_samples >
COLD AmpSmoother<smooth_samples>::~AmpSmoother() noexcept {}

//==============================================================================
template< int smooth_samples >
inline float AmpSmoother<smooth_samples>::add_get( float in_ ) noexcept {
    if( current_value != in_ || target_value != in_ )
    {
        // ONLY UPDATE IF WE HAVE A NEW VALUE
        if( target_value != in_ )
        {
            counter = smooth_samples;
            delta = (in_-current_value) / smooth_samples;
            target_value = in_;
        }

        glide_tick();
    }

    return current_value;
}
template< int smooth_samples >
bool AmpSmoother<smooth_samples>::is_up_to_date() const noexcept
{
    return target_value == current_value;
}
template< int smooth_samples >
inline void AmpSmoother<smooth_samples>::reset( float value_ ) noexcept
{
    current_value = value_;
    delta = 0;
    counter = 0;
}
template< int smooth_samples >
inline float AmpSmoother<smooth_samples>::add_get_and_keep_current_time( float in_ ) noexcept {
    if( current_value != in_ || target_value != in_ )
    {
        // ONLY UPDATE IF WE HAVE A NEW VALUE
        if( target_value != in_ )
        {
            delta = (in_-current_value) / counter;
            target_value = in_;
        }

        glide_tick();
    }

    return current_value;
}
template< int smooth_samples >
inline void AmpSmoother<smooth_samples>::glide_tick() noexcept
{
    if( --counter <= 0 )
        current_value = target_value;
    else
    {
        current_value+=delta;
        if( current_value > 1 || current_value < 0 )
        {
            current_value = target_value;
            counter = 0;
        }
    }
}
template< int smooth_samples >
inline float AmpSmoother<smooth_samples>::get_current_value() const noexcept
{
    return current_value;
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
class AnalogFilter : public RuntimeListener
{
    friend class DoubleAnalogFilter;
    float p, k, r, gain;
    float y1,y2,y3,y4;
    float oldx;
    float oldy1,oldy2,oldy3;

    float cutoff, res;

    bool force_update;

public:
    //==========================================================================
    // RETURNS TRUE ON COFF CHANGED
    inline bool update(float resonance_, float cutoff_, float gain_) noexcept
    {
        bool success = false;
        resonance_*=0.8;
        if( force_update or ( cutoff != cutoff_ || gain != gain_ || res != resonance_ ) )
        {
            gain = gain_;
            cutoff = cutoff_;
            res = resonance_;
            success = true;

            force_update = false;
        }
        return success;
    }
    inline void update_with_calc(float resonance_, float cutoff_, float gain_) noexcept
    {
        if( force_update or ( cutoff != cutoff_ || gain != gain_ || res != resonance_ ) )
        {
            gain = gain_;
            cutoff = cutoff_;
            res = resonance_;
            calc_coefficients();

            force_update = false;
        }
    }
    //==========================================================================
    inline void copy_coefficient_from( const AnalogFilter& other_ ) noexcept
    {
        cutoff = other_.cutoff;
        res = other_.res;

        p = other_.p;
        k = other_.k;
        r = other_.r;
        gain = other_.gain;
    }
    inline void copy_state_from( const AnalogFilter& other_ ) noexcept
    {
        oldx = other_.oldx;
        oldy1 = other_.oldy1;
        oldy2 = other_.oldy2;
        oldy3 = other_.oldy3;
        y1 = other_.y1;
        y2 = other_.y2;
        y3 = other_.y3;
        y4 = other_.y4;
    }

    //==========================================================================
    inline float processLow(float input_and_worker_) noexcept
    {
        // process input
        input_and_worker_ -= r*y4;

        //Four cascaded onepole filters (bilinear transform)
        y1= input_and_worker_*p + oldx*p - k*y1;
        y2=y1*p + oldy1*p - k*y2;
        y3=y2*p + oldy2*p - k*y3;
        y4=y3*p + oldy3*p - k*y4;

        //Clipper band limited sigmoid
        y4 -= (y4*y4*y4) /6;

        oldx = input_and_worker_;
        oldy1 = y1;
        oldy2 = y2;
        oldy3 = y3;

        return soft_clipp_greater_1_2(y4);
    }
    inline float processLowResonance(float input_and_worker_) noexcept
    {
        // process input
        input_and_worker_ -= r*y4;

        //Four cascaded onepole filters (bilinear transform)
        y1= input_and_worker_*p + oldx*p - k*y1;
        y2=y1*p + oldy1*p - k*y2;
        y3=y2*p + oldy2*p - k*y3;
        y4=y3*p + oldy3*p - k*y4;

        //Clipper band limited sigmoid
        y4 -= (y4*y4*y4) /6;

        oldx = input_and_worker_;
        oldy1 = y1;
        oldy2 = y2;
        oldy3 = y3;

        // Add band resonance
        return soft_clipp_greater_1_2(sample_mix( y4 , y3 * res ));
    }
    inline float processHighResonance(float input_and_worker_) noexcept
    {
        // process input
        input_and_worker_ -= r*y4;

        //Four cascaded onepole filters (bilinear transform)
        y1= input_and_worker_*p + oldx*p - k*y1;
        y2=y1*p + oldy1*p - k*y2;
        y3=y2*p + oldy2*p - k*y3;
        y4=y3*p + oldy3*p - k*y4;

        //Clipper band limited sigmoid
        y4 -= (y4*y4*y4) /6;

        oldx = input_and_worker_;
        oldy1 = y1;
        oldy2 = y2;
        oldy3 = y3;

        // RESONANCE
        return hard_clipper_1(input_and_worker_-y4);
    }

    //==========================================================================
    inline void reset() noexcept
    {
        cutoff=res=p=k=r=gain=y1=y2=y3=y4=oldx=oldy1=oldy2=oldy3=1;
    }

    //==========================================================================
    inline void calc_coefficients() noexcept
    {
        {
            float f = (cutoff+cutoff) * sample_rate_1ths;
            float agressive = 0.48f*gain;
            p=f*((1.5f+agressive)-((0.5f+agressive)*f));
            k=p*2-1;
        }
        {
            const float t=(1.0f-p)*1.386249f;
            const float t2=12.0f+t*t;
            r = res*(t2+6.0f*t)/(t2-6.0f*t);
        }
    }


private:
    //==========================================================================
    COLD void sample_rate_changed(double) noexcept
    {
        reset();
    }

public:
    //==========================================================================
    COLD AnalogFilter( RuntimeNotifyer*const notifyer_ ) noexcept
:
    RuntimeListener( notifyer_ ),

                     p(1),k(1),r(1),gain(1),
                     y1(1),y2(1),y3(1),y4(1),
                     oldx(1),oldy1(1),oldy2(1),oldy3(1),

                     cutoff(1000), res(1),

                     force_update(true)
    {
        sample_rate_changed(0);
    }
    COLD ~AnalogFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFilter)
};

//==============================================================================
//==============================================================================
//==============================================================================
#define FILTER_CHANGE_GLIDE_TIME_MS (msToSamplesFast(200,flt_1.sample_rate)+50)
class DoubleAnalogFilter
{
    AnalogFilter flt_1;
    AnalogFilter flt_2;

    DoubleAnalogFilter* smooth_filter;

    FILTER_TYPS last_filter_type;
    FILTER_TYPS smooth_filter_type;
    int glide_time_4_filters;

public:
    // LP
    //==========================================================================
    inline void updateLow2Pass(float resonance_, float cutoff_, float gain_) noexcept
    {
        if( flt_2.update( resonance_, cutoff_, gain_ ) )
        {
            flt_2.calc_coefficients();
            flt_1.copy_coefficient_from( flt_2 );
            //flt_2.r = 0;
        }
    }
    inline float processLow2Pass(float in_) noexcept
    {
        const float out = flt_2.processLowResonance( in_ );
        const float gain = flt_1.gain;
        const float low = flt_1.processLowResonance( out );

        return process_filter_change
        (
            in_,
            //(out+low)*(1.0f-gain) + resonance_clipping(out+low)*gain
            sample_mix(out*(1.0f-gain), sample_mix(out,low)*gain)
        );
    }

    // 1 PASS HP
    //==========================================================================
    inline void updateHigh2Pass(float resonance_, float cutoff_, float gain_) noexcept
    {
        if( flt_1.update( resonance_, cutoff_, gain_ ) )
        {
            flt_1.calc_coefficients();
        }
    }
    inline float processHigh2Pass(float in_) noexcept
    {
        in_ = soft_clipp_greater_1_2(in_);
        const float out = flt_1.processHighResonance( in_ );
        return process_filter_change
        (
            in_,
            out
        );

    }

    // BAND
    //==========================================================================
    inline void updateBand(float resonance_, float cutoff_, float gain_ ) noexcept
    {
        if( flt_1.update( resonance_, cutoff_+10, gain_ ) )
        {
            flt_1.calc_coefficients();
            flt_2.update( resonance_, cutoff_, gain_ );
            flt_2.calc_coefficients();
        }
    }
    inline float processBand(float in_) noexcept
    {
        return process_filter_change
        (
            in_,
            flt_1.processLowResonance(  flt_2.processHighResonance( in_ ) )*0.5f
        );
    }

    // PASS
    //==========================================================================
    inline float processPass(float in_) noexcept
    {
        return process_filter_change
        (
            in_,
            in_
        );
    }

    // BY TYPE
    //==========================================================================
    inline void update_filter_to( FILTER_TYPS type_ ) noexcept
    {
        if( last_filter_type != type_ )
        {
            if( smooth_filter )
            {
                // SET THE SECOND FILTER TO THE OLD COMPLETE STATE
                if( last_filter_type != PASS )
                {
                    smooth_filter->flt_1.copy_coefficient_from(flt_1);
                    flt_1.force_update = true;
                    smooth_filter->flt_1.copy_state_from(flt_1);
                    smooth_filter->flt_2.copy_coefficient_from(flt_2);
                    flt_2.force_update = true;
                    smooth_filter->flt_2.copy_state_from(flt_2);
                }
                else
                {
                    flt_1.reset();
                    flt_2.reset();
                }

                glide_time_4_filters = FILTER_CHANGE_GLIDE_TIME_MS;

                smooth_filter->last_filter_type = last_filter_type;
                smooth_filter_type = last_filter_type;
            }

            last_filter_type = type_;
        }
    }
    inline float process_filter_change( float original_in_, float result_in_ ) noexcept
    {
        if( glide_time_4_filters > 0 )
        {
            // if( smooth_filter ) IS TRUE IF glide_time_4_filters != 0
            {
                const float smooth_out = smooth_filter->processByType( original_in_, smooth_filter_type );

                float mix = 1.0f/float(FILTER_CHANGE_GLIDE_TIME_MS)*glide_time_4_filters;
                result_in_ = result_in_*(1.0f-mix) + smooth_out*mix;
            }
            --glide_time_4_filters;
        }

        return result_in_;
    }
    inline float processByType(float io_, FILTER_TYPS type_ ) noexcept
    {
        switch( type_ )
        {
        case LPF :
        case LPF_2_PASS :
            io_ = processLow2Pass(io_);
            break;
        case HPF :
        case HIGH_2_PASS :
            io_ = processHigh2Pass(io_);
            break;
        case BPF :
            io_ = processBand(io_);
            break;
        default /* PASS & UNKNOWN */ :
            ; // io_ = filter_hard_clipper(io_);
        }

        return io_;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        last_filter_type = UNKNOWN;
        smooth_filter_type = UNKNOWN;
        glide_time_4_filters = 0;
        if( smooth_filter )
        {
            smooth_filter->reset();
        }
        flt_1.reset();
        flt_2.reset();
    }

public:
    //==============================================================================
    COLD DoubleAnalogFilter( RuntimeNotifyer*const notifyer_, bool create_smooth_filter = true) noexcept
:
    flt_1( notifyer_ ),
           flt_2( notifyer_ ),

           smooth_filter( nullptr ),

           last_filter_type(UNKNOWN),
           smooth_filter_type(UNKNOWN),

           glide_time_4_filters(0)
    {
        if( create_smooth_filter )
        {
            smooth_filter = new DoubleAnalogFilter(notifyer_,false);
        }
    }
    COLD ~DoubleAnalogFilter() noexcept
    {
        if( smooth_filter )
        {
            delete smooth_filter;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleAnalogFilter)
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
//==============================================================================
class FilterProcessor
{
    mono_ThreadManager*const thread_manager;

    OwnedArray<DoubleAnalogFilter> double_filter;
    friend class mono_ParameterOwnerStore;

public:
    ScopedPointer< ENV > env;
    Array< ENVData* > input_env_datas;
    OwnedArray< ENV > input_envs;

private:
    const int id;

    const MoniqueSynthData*const synth_data;
    FilterData*const filter_data;
    DataBuffer*const data_buffer;

public:
    //==========================================================================
    inline void start_attack() noexcept
    {
        env->start_attack();

        for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        {
            input_envs.getUnchecked(input_id)->start_attack();
        }
    }
    inline void start_release() noexcept
    {
        env->set_to_release();

        for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        {
            input_envs.getUnchecked(input_id)->set_to_release();
        }
    }

    //==========================================================================
    inline void reset() noexcept
    {
        env->reset();
        for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        {
            input_envs.getUnchecked(input_id)->reset();
        }

        double_filter[0]->reset();
        double_filter[1]->reset();
        double_filter[2]->reset();
    }

private:
    //==========================================================================
    inline void pre_process( const int input_id, const int num_samples ) noexcept
    {
        // CALCULATE INPUTS AND ENVELOPS
        {
            {
                if( id == FILTER_1 )
                {
                    float* tmp_input_ar_amp = data_buffer->filter_input_env_amps.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * FILTER_1 );
                    ENV*const input_env( input_envs.getUnchecked(input_id) );
                    input_env->env_data->sustain_smoother.process_amp( not filter_data->input_holds[input_id], input_env, tmp_input_ar_amp, num_samples );

                    float* filter_input_buffer = data_buffer->filter_input_samples.getWritePointer( input_id );
                    const float* const osc_input_buffer = data_buffer->osc_samples.getReadPointer(input_id);
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        filter_input_buffer[sid] = osc_input_buffer[sid]*tmp_input_ar_amp[sid];
                    }
                }
                else if( id == FILTER_2 )
                {
                    const float*const smoothed_sustain_buffer( input_env_datas[input_id]->sustain_smoother.get_smoothed_buffer() );
                    float* tmp_input_ar_amp = data_buffer->filter_input_env_amps.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * FILTER_2 );
                    ENV*const input_env( input_envs.getUnchecked(input_id) );
                    input_env->env_data->sustain_smoother.process_amp( not filter_data->input_holds[input_id], input_env, tmp_input_ar_amp, num_samples );

                    float*const filter_input_buffer = data_buffer->filter_input_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER*FILTER_2 );
                    const float*const filter_before_buffer = data_buffer->filter_output_samples.getReadPointer( input_id + SUM_INPUTS_PER_FILTER*FILTER_1 );
                    const float*const osc_input_buffer = data_buffer->osc_samples.getReadPointer(input_id);
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        filter_input_buffer[sid] = smoothed_sustain_buffer[sid] < 0 ? osc_input_buffer[sid]*tmp_input_ar_amp[sid] : filter_before_buffer[sid]*tmp_input_ar_amp[sid];
                    }
                }
                else
                {
                    float* tmp_input_ar_amp_1 = data_buffer->filter_input_env_amps.getWritePointer( 0 + SUM_INPUTS_PER_FILTER * FILTER_3 );
                    float* tmp_input_ar_amp_2 = data_buffer->filter_input_env_amps.getWritePointer( 1 + SUM_INPUTS_PER_FILTER * FILTER_3 );
                    float* tmp_input_ar_amp_3 = data_buffer->filter_input_env_amps.getWritePointer( 2 + SUM_INPUTS_PER_FILTER * FILTER_3 );
                    {
                        ENV*const input_env( input_envs.getUnchecked(0) );
                        input_env->env_data->sustain_smoother.process_amp( not filter_data->input_holds[0], input_env, tmp_input_ar_amp_1, num_samples );
                    }
                    {
                        ENV*const input_env( input_envs.getUnchecked(1) );
                        input_env->env_data->sustain_smoother.process_amp( not filter_data->input_holds[1], input_env, tmp_input_ar_amp_2, num_samples );
                    }
                    {
                        ENV*const input_env( input_envs.getUnchecked(2) );
                        input_env->env_data->sustain_smoother.process_amp( not filter_data->input_holds[2], input_env, tmp_input_ar_amp_3, num_samples );
                    }

                    float*const filter_input_buffer = data_buffer->filter_input_samples.getWritePointer( 0 + SUM_INPUTS_PER_FILTER * FILTER_3 );
                    const float*const smoothed_sustain_buffer_1( input_env_datas[0]->sustain_smoother.get_smoothed_buffer() );
                    const float*const smoothed_sustain_buffer_2( input_env_datas[1]->sustain_smoother.get_smoothed_buffer() );
                    const float*const smoothed_sustain_buffer_3( input_env_datas[2]->sustain_smoother.get_smoothed_buffer() );
                    const float*const filter_before_buffer_1 = data_buffer->filter_output_samples.getReadPointer( 0 + SUM_INPUTS_PER_FILTER*FILTER_2 );
                    const float*const filter_before_buffer_2 = data_buffer->filter_output_samples.getReadPointer( 1 + SUM_INPUTS_PER_FILTER*FILTER_2 );
                    const float*const filter_before_buffer_3 = data_buffer->filter_output_samples.getReadPointer( 2 + SUM_INPUTS_PER_FILTER*FILTER_2 );
                    const float*const osc_input_buffer_1 = data_buffer->osc_samples.getReadPointer(0);
                    const float*const osc_input_buffer_2 = data_buffer->osc_samples.getReadPointer(1);
                    const float*const osc_input_buffer_3 = data_buffer->osc_samples.getReadPointer(2);

                    FloatVectorOperations::clear(filter_input_buffer,num_samples);
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        filter_input_buffer[sid]
                        = sample_mix
                        (
                            sample_mix
                            (
                                smoothed_sustain_buffer_1[sid] < 0 ? osc_input_buffer_1[sid]*tmp_input_ar_amp_1[sid] : filter_before_buffer_1[sid]*tmp_input_ar_amp_1[sid],
                                smoothed_sustain_buffer_2[sid] < 0 ? osc_input_buffer_2[sid]*tmp_input_ar_amp_2[sid] : filter_before_buffer_2[sid]*tmp_input_ar_amp_2[sid]
                            ),
                            smoothed_sustain_buffer_3[sid] < 0 ? osc_input_buffer_3[sid]*tmp_input_ar_amp_3[sid] : filter_before_buffer_3[sid]*tmp_input_ar_amp_3[sid]
                        );
                    }
                }
            }
        }
    }
    inline void process_amp_mix( const int num_samples ) noexcept
    {
        // ADSTR - LFO MIX
        float* amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
        const float* smoothed_mix_buffer( filter_data->adsr_lfo_mix_smoother.get_smoothed_buffer() );
        {
            const float* env_amps = data_buffer->filter_env_amps.getReadPointer(id);
            const float* lfo_amplitudes = data_buffer->lfo_amplitudes.getReadPointer(id);
            for( int sid = 0 ; sid != num_samples ; ++sid )
            {
                // LFO ADSR MIX - HERE TO SAVE ONE LOOP
                const float mix = (1.0f+smoothed_mix_buffer[sid]) * 0.5f;
                amp_mix[sid] = env_amps[sid]*(1.0f-mix) + lfo_amplitudes[sid]*mix;
            }
        }
    }

    static float distortion__( float x_, float distortion_power_ ) noexcept
    {
        if( distortion_power_ != 0 )
        {
            //const float distortion_add_on = distortion_power_*0.9f;
            //x_ = ((1.0f+distortion_add_on)*x_) - (x_*x_*x_)*distortion_add_on;
            // (std::atan(input_and_worker_)*(1.0f/float_Pi))*
            //x_ = x_*(1.0f-distortion_power_) + 0.5f*soft_clipping( x_*10 )*(distortion_power_);
            x_ = x_*(1.0f-distortion_power_) + (std::atan( x_*10 )/10)*distortion_power_;
        }

        return x_;
    }

public:
    //==========================================================================
    inline void process( const int num_samples ) noexcept
    {
        float* amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
        // PROCESS FILTER
        {
#define DISTORTION_IN(x) distortion__(x,filter_distortion)
#define DISTORTION_OUT(x) distortion__(x,filter_distortion)

#define MAX_CUTOFF 8000.0f
#define MIN_CUTOFF 40.0f

            // PREPARE
            {
                process_amp_mix(num_samples);

                filter_data->resonance_smoother.process_modulation( filter_data->modulate_resonance, amp_mix, num_samples );
                filter_data->cutoff_smoother.process_modulation( filter_data->modulate_cutoff, amp_mix, num_samples );
                filter_data->gain_smoother.process_modulation( filter_data->modulate_gain, amp_mix, num_samples );
                filter_data->distortion_smoother.process_modulation( filter_data->modulate_distortion, amp_mix, num_samples );
                filter_data->pan_smoother.process_modulation( filter_data->modulate_pan, amp_mix, num_samples );
            }

            mono_Thread*executer_1 = nullptr;
            mono_Thread*executer_2 = nullptr;
            mono_Thread*executer_3 = nullptr;
            switch( filter_data->filter_type )
            {
            case LPF :
            case LPF_2_PASS :
            case MOOG_AND_LPF:
            {
                // PROCESSOR
                struct LP2PassExecuter : public mono_Thread
                {
                    FilterProcessor*const processor;
                    DoubleAnalogFilter& filter;
                    const int input_id;
                    const int num_samples_;

                    const float* const tmp_resonance_buffer;
                    const float* const tmp_cuttof_buffer;
                    const float* const tmp_gain_buffer;
                    const float* const tmp_distortion_buffer;

                    const float* const input_buffer;
                    float* const out_buffer;

                    inline void exec() noexcept override
                    {
                        processor->pre_process(input_id,num_samples_);

                        filter.update_filter_to(LPF_2_PASS);
                        for( int sid = 0 ; sid != num_samples_ ; ++sid )
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            filter.updateLow2Pass
                            (
                                tmp_resonance_buffer[sid],
                                ( ((MAX_CUTOFF * tmp_cuttof_buffer[sid] + MIN_CUTOFF) * (1.0f/8)) +35 ),
                                tmp_gain_buffer[sid]
                            );
                            out_buffer[sid] = DISTORTION_OUT( filter.processLow2Pass( DISTORTION_IN( input_buffer[sid] ) ) );
                        }
                    }
                    LP2PassExecuter( FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                    mono_Thread( processor_->thread_manager ),
                                 processor( processor_ ),
                                 filter( *processor_->double_filter.getUnchecked(input_id_) ),
                                 input_id( input_id_ ),
                                 num_samples_( num_samples__ ),

                                 tmp_resonance_buffer( processor_->filter_data->resonance_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_cuttof_buffer( processor_->filter_data->cutoff_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_gain_buffer( processor_->filter_data->gain_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_distortion_buffer( processor_->filter_data->distortion_smoother.get_smoothed_modulated_buffer() ),

                                 input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id )),
                                 out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                    {}
                };
                {
                    if( id != FILTER_3 )
                    {
                        executer_1 = new LP2PassExecuter( this, num_samples, 0 );
                        executer_2 = new LP2PassExecuter( this, num_samples, 1 );
                        executer_3 = new LP2PassExecuter( this, num_samples, 2 );
                    }
                    else
                    {
                        // 1, 2 and 3
                        executer_1 = new LP2PassExecuter( this, num_samples, 0 );
                    }
                }
            }
            break;
            case HPF :
            case HIGH_2_PASS :
            {
                // PROCESSOR
                struct HP2PassExecuter : public mono_Thread
                {
                    FilterProcessor*const processor;
                    DoubleAnalogFilter& filter;
                    const int input_id;
                    const int num_samples_;

                    const float* const tmp_resonance_buffer;
                    const float* const tmp_cuttof_buffer;
                    const float* const tmp_gain_buffer;
                    const float* const tmp_distortion_buffer;

                    const float* const input_buffer;
                    float* const out_buffer;

                    inline void exec() noexcept override
                    {
                        processor->pre_process(input_id,num_samples_);

                        filter.update_filter_to(HIGH_2_PASS);
                        for( int sid = 0 ; sid != num_samples_ ; ++sid )
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            filter.updateHigh2Pass
                            (
                                tmp_resonance_buffer[sid],
                                ( ((MAX_CUTOFF * tmp_cuttof_buffer[sid] + MIN_CUTOFF)) +35 ),
                                tmp_gain_buffer[sid]
                            );
                            out_buffer[sid] = DISTORTION_OUT( filter.processHigh2Pass( DISTORTION_IN( input_buffer[sid] ) ) );
                        }
                    }
                    HP2PassExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                    mono_Thread( processor_->thread_manager ),
                                 processor( processor_ ),
                                 filter( *processor_->double_filter.getUnchecked(input_id_) ),
                                 input_id(input_id_),
                                 num_samples_(num_samples__),

                                 tmp_resonance_buffer( processor_->filter_data->resonance_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_cuttof_buffer( processor_->filter_data->cutoff_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_gain_buffer( processor_->filter_data->gain_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_distortion_buffer( processor_->filter_data->distortion_smoother.get_smoothed_modulated_buffer() ),

                                 input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id )),
                                 out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                    {}
                };
                {
                    if( id != FILTER_3 )
                    {
                        executer_1 = new HP2PassExecuter( this, num_samples, 0 );
                        executer_2 = new HP2PassExecuter( this, num_samples, 1 );
                        executer_3 = new HP2PassExecuter( this, num_samples, 2 );
                    }
                    else
                    {
                        // 1, 2 and 3
                        executer_1 = new HP2PassExecuter( this, num_samples, 0 );
                    }
                }
            }
            break;
            case BPF:
            {
                // PROCESSOR
                struct BandExecuter : public mono_Thread
                {
                    FilterProcessor*const processor;
                    DoubleAnalogFilter& filter;
                    const int input_id;
                    const int num_samples_;

                    const float* const tmp_resonance_buffer;
                    const float* const tmp_cuttof_buffer;
                    const float* const tmp_gain_buffer;
                    const float* const tmp_distortion_buffer;

                    const float* const input_buffer;
                    float* const out_buffer;

                    inline void exec() noexcept override
                    {
                        processor->pre_process(input_id,num_samples_);

                        filter.update_filter_to(BPF);

                        for( int sid = 0 ; sid != num_samples_ ; ++sid )
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            filter.updateBand
                            (
                                tmp_resonance_buffer[sid],
                                ( ((MAX_CUTOFF * tmp_cuttof_buffer[sid] + MIN_CUTOFF)) +35 ),
                                tmp_gain_buffer[sid]
                            );
                            out_buffer[sid] = DISTORTION_OUT( filter.processBand( DISTORTION_IN( input_buffer[sid] ) ) );
                        }
                    }
                    BandExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                    mono_Thread( processor_->thread_manager ),
                                 processor( processor_ ),
                                 filter( *processor_->double_filter.getUnchecked(input_id_) ),
                                 input_id(input_id_),
                                 num_samples_(num_samples__),

                                 tmp_resonance_buffer( processor_->filter_data->resonance_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_cuttof_buffer( processor_->filter_data->cutoff_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_gain_buffer( processor_->filter_data->gain_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_distortion_buffer( processor_->filter_data->distortion_smoother.get_smoothed_modulated_buffer() ),

                                 input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id )),
                                 out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                    {}
                };
                {
                    if( id != FILTER_3 )
                    {
                        executer_1 = new BandExecuter( this, num_samples, 0 );
                        executer_2 = new BandExecuter( this, num_samples, 1 );
                        executer_3 = new BandExecuter( this, num_samples, 2 );
                    }
                    else
                    {
                        // 1, 2 and 3
                        executer_1 = new BandExecuter( this, num_samples, 0 );
                    }
                }
            }
            break;
            default : //  PASS
            {
                // PROCESSOR
                struct PassExecuter : public mono_Thread
                {
                    FilterProcessor*const processor;
                    DoubleAnalogFilter& filter;
                    const int input_id;
                    const int num_samples_;

                    const float* const tmp_gain_buffer;
                    const float* const tmp_distortion_buffer;

                    const float* const input_buffer;
                    float* const out_buffer;

                    inline void exec() noexcept override
                    {
                        processor->pre_process(input_id,num_samples_);

                        filter.update_filter_to(PASS);

                        for( int sid = 0 ; sid != num_samples_ ; ++sid )
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            out_buffer[sid] = DISTORTION_OUT( filter.processPass( DISTORTION_IN( input_buffer[sid] ) ) );
                        }
                    }
                    PassExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                    mono_Thread( processor_->thread_manager ),
                                 processor( processor_ ),
                                 filter( *processor_->double_filter.getUnchecked(input_id_) ),
                                 input_id(input_id_),
                                 num_samples_(num_samples__),

                                 tmp_gain_buffer( processor_->filter_data->gain_smoother.get_smoothed_modulated_buffer() ),
                                 tmp_distortion_buffer( processor_->filter_data->distortion_smoother.get_smoothed_modulated_buffer() ),

                                 input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id )),
                                 out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                    {}
                };
                {
                    if( id != FILTER_3 )
                    {
                        executer_1 = new PassExecuter( this, num_samples, 0 );
                        executer_2 = new PassExecuter( this, num_samples, 1 );
                        executer_3 = new PassExecuter( this, num_samples, 2 );
                    }
                    else
                    {
                        // 1, 2 and 3
                        executer_1 = new PassExecuter( this, num_samples, 0 );
                    }
                }
            }
            break;
            }

            // RUN
            if( id != FILTER_3 )
            {
                executer_1->try_run_paralel();
                executer_2->exec();
                delete executer_2;
                executer_3->exec();
                delete executer_3;

                while( executer_1->isWorking() ) {}
                delete executer_1;
            }
            else
            {
                executer_1->exec();
                delete executer_1;
            }
        }

        // COLLECT RESULTS
        float* const this_filter_output_buffer = data_buffer->filter_output_samples_l_r.getWritePointer(id);
        {
            const float*const smoothed_distortion_buffer( filter_data->distortion_smoother.get_smoothed_modulated_buffer() );
            {
                filter_data->output_smoother.process_modulation( filter_data->modulate_output, amp_mix, num_samples );
                const float*const smoothed_output_buffer = filter_data->output_smoother.get_smoothed_modulated_buffer();

                if( id != FILTER_3 )
                {
                    float*const out_buffer_1( data_buffer->filter_output_samples.getWritePointer( 0 + SUM_INPUTS_PER_FILTER * id ) );
                    float*const out_buffer_2( data_buffer->filter_output_samples.getWritePointer( 1 + SUM_INPUTS_PER_FILTER * id ) );
                    float*const out_buffer_3( data_buffer->filter_output_samples.getWritePointer( 2 + SUM_INPUTS_PER_FILTER * id ) );
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        // OUTPUT MIX AND DISTORTION
                        {
                            const float amp = smoothed_output_buffer[sid];
                            float shape_power = smoothed_distortion_buffer[sid];
                            const float result = sample_mix(sample_mix(out_buffer_1[sid], out_buffer_2[sid]), out_buffer_3[sid]) * amp;

                            this_filter_output_buffer[sid] = sample_mix( result*(1.0f-shape_power), soft_clipping( result*5 )*1.5*(shape_power) );
                        }
                    }
                }
                else
                {
                    float*const out_buffer( data_buffer->filter_output_samples.getWritePointer( 0 + SUM_INPUTS_PER_FILTER * FILTER_3 ) );
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        // OUTPUT MIX AND DISTORTION
                        {
                            const float amp = smoothed_output_buffer[sid];
                            float shape_power = smoothed_distortion_buffer[sid];
                            const float result = out_buffer[sid] * amp;

                            this_filter_output_buffer[sid] = sample_mix( result*(1.0f-shape_power), soft_clipping( result*5 )*1.5*(shape_power) );
                        }
                    }
                }
            }
        }

        // PAN & MIX
        {
            const float*const pan_buffer = filter_data->pan_smoother.get_smoothed_modulated_buffer();
            float* const left_and_input_output_buffer = data_buffer->filter_output_samples_l_r.getWritePointer(id);
            float* const right_output_buffer = data_buffer->filter_output_samples_l_r.getWritePointer(SUM_FILTERS+id);
            for( int sid = 0 ; sid != num_samples ; ++sid )
            {
                const float pan = pan_buffer[sid];
                const float output_sample = left_and_input_output_buffer[sid];
                right_output_buffer[sid] = output_sample*jmin(1.0f,pan+1);
                left_and_input_output_buffer[sid] = output_sample*jmin(1.0f,pan*-1+1);
            }

            // VISUALIZE
            if( Monique_Ui_AmpPainter*const amp_painter = synth_data->audio_processor->amp_painter )
            {
                amp_painter->add_filter_env( id, amp_mix, num_samples );
                amp_painter->add_filter( id, right_output_buffer, left_and_input_output_buffer, num_samples );
            }
        }

        // COLLECT THE FINAL OUTPUT
        if( id == FILTER_3 )
        {
            float* const master_left_output_buffer = data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);
            float* const master_right_output_buffer = data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT);
            const float* const left_output_buffer_flt1 = data_buffer->filter_output_samples_l_r.getReadPointer(0);
            const float* const right_output_buffer_flt1 = data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS);
            const float* const left_output_buffer_flt2 = data_buffer->filter_output_samples_l_r.getReadPointer(1);
            const float* const right_output_buffer_flt2 = data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS+1);
            const float* const left_output_buffer_flt3 = data_buffer->filter_output_samples_l_r.getReadPointer(2);
            const float* const right_output_buffer_flt3 = data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS+2);
            const float* const smoothed_distortion = synth_data->distortion_smoother.get_smoothed_buffer();
            for( int sid = 0 ; sid != num_samples ; ++sid )
            {
                const float left = sample_mix(sample_mix(left_output_buffer_flt1[sid], left_output_buffer_flt2[sid]), left_output_buffer_flt3[sid]);
                const float right = sample_mix(sample_mix(right_output_buffer_flt1[sid], right_output_buffer_flt2[sid]), right_output_buffer_flt3[sid]);
                const float left_add = left_output_buffer_flt1[sid] + left_output_buffer_flt2[sid] + left_output_buffer_flt3[sid];
                const float right_add = right_output_buffer_flt1[sid] + right_output_buffer_flt2[sid] + right_output_buffer_flt3[sid];
                const float distortion = smoothed_distortion[sid];

                master_left_output_buffer[sid] = left*(1.0f-distortion) + 1.33f*soft_clipping( left_add*10 )*(distortion);
                master_right_output_buffer[sid] = right*(1.0f-distortion) + 1.33f*soft_clipping( right_add*10 )*(distortion);
            }
        }
    }

public:
    //==============================================================================
    COLD FilterProcessor( RuntimeNotifyer*const notifyer_,
                          const MoniqueSynthData* synth_data_,
                          mono_ThreadManager*const thread_manager,
                          int id_,
                          const float*const sine_lookup_,
                          const float*const cos_lookup_,
                          const float*const exp_lookup_ ) noexcept
:
    thread_manager(thread_manager),

                   env( new ENV( notifyer_, synth_data_, synth_data_->filter_datas[id_]->env_data, sine_lookup_, cos_lookup_, exp_lookup_ ) ),
                   input_envs(),

                   id(id_),

                   synth_data( synth_data_ ),
                   filter_data( synth_data_->filter_datas[id_] ),
                   data_buffer( synth_data_->data_buffer )
    {
        for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        {
            double_filter.add( new DoubleAnalogFilter(notifyer_) );

            ENVData*input_env_data(synth_data_->filter_datas[id_]->input_envs[i]);
            input_env_datas.add( input_env_data );
            input_envs.add( new ENV( notifyer_, synth_data_, input_env_data, sine_lookup_, cos_lookup_, exp_lookup_ ) );
        }
    }
    COLD ~FilterProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor)
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
class EQProcessor : public RuntimeListener
{
    mono_ThreadManager*const thread_manager;

    friend class EQProcessorStereo;

    float frequency_low_pass[SUM_EQ_BANDS];
    float frequency_high_pass[SUM_EQ_BANDS];

    OwnedArray<AnalogFilter> filters;
    IIRFilter high_pass_filters[SUM_EQ_BANDS];

    friend class mono_ParameterOwnerStore;

    const MoniqueSynthData*const synth_data;
    const EQData*const eq_data;
    DataBuffer*const data_buffer;

public:
    //==============================================================================
    inline void reset() noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            filters[band_id]->reset();
            high_pass_filters[band_id].reset();
        }
    }

    void sample_rate_changed(double) noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            high_pass_filters[band_id].setCoefficients( IIRCoefficients::makeHighPass( sample_rate, frequency_high_pass[band_id] ) );
        }
    }

    //==============================================================================
    inline void process( float* io_buffer_, int num_samples_ ) noexcept
    {
        // MULTITHREADED PER BAND
        {
            struct BandExecuter : public mono_Thread
            {
                const int num_samples_;
                const float* const smoothed_shape_buffer;

                const float filter_frequency;
                IIRFilter& high_pass_filter;
                AnalogFilter& filter;

                const float* const filter_in_samples;
                float* const band_out_buffer;
                const float* const env_buffer;

                inline void exec() noexcept override
                {
                    // PROCESS
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float shape = smoothed_shape_buffer[sid];
                        const float amp = env_buffer[sid];
                        const float in = filter_in_samples[sid] * amp;
                        filter.update_with_calc( shape*0.8f, filter_frequency, 0 );
                        float output = high_pass_filter.processSingleSampleRaw ( filter.processLow(in) );
                        band_out_buffer[sid] = output*4;
                    }
                }
                BandExecuter(EQProcessor*const processor_, float*in_buffer_, int num_samples__, int band_id_) noexcept
:
                mono_Thread( processor_->thread_manager ),
                num_samples_(num_samples__),
                smoothed_shape_buffer( processor_->synth_data->shape_smoother.get_smoothed_buffer() ),

                filter_frequency( processor_->frequency_low_pass[band_id_] ),
                high_pass_filter(processor_->high_pass_filters[band_id_]),
                filter(*processor_->filters.getUnchecked(band_id_)),

                filter_in_samples( in_buffer_ ),
                band_out_buffer( processor_->data_buffer->band_out_buffers.getWritePointer(band_id_) ),
                env_buffer( processor_->data_buffer->band_env_buffers.getWritePointer(band_id_) )
                {}
            };

            Array<BandExecuter*> running_threads;
            for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
            {
                // TRY TO FREE SOME MEMORY
                Array<BandExecuter*> copy_of_running_thereads = running_threads;
                for( int i = 0 ; i != copy_of_running_thereads.size() ; ++i )
                {
                    BandExecuter* executer( copy_of_running_thereads[i] );
                    if( not executer->isWorking() )
                    {
                        running_threads.removeFirstMatchingValue(executer);
                        delete executer;
                    }
                }

                BandExecuter* executer = new BandExecuter(this, io_buffer_, num_samples_, band_id);
                executer->try_run_paralel();
                if( executer->isWorking() )
                {
                    running_threads.add( executer );
                }
                else
                {
                    delete executer;
                }
            }

            bool all_done = running_threads.size() == 0;
            while( not all_done )
            {
                Array<BandExecuter*> copy_of_running_thereads = running_threads;
                for( int i = 0 ; i != copy_of_running_thereads.size() ; ++i )
                {
                    BandExecuter* executer( copy_of_running_thereads.getUnchecked(i) );
                    if( not executer->isWorking() )
                    {
                        running_threads.removeFirstMatchingValue(executer);
                        delete executer;
                    }
                }

                all_done = running_threads.size() == 0;
            }
        }
        // EO MULTITHREADED

        // FINAL MIX - SINGLE THREADED ( NO REALY OPTIMIZED )
        {
            const float*const buffer_1( data_buffer->band_out_buffers.getReadPointer(0) );
            const float*const buffer_2( data_buffer->band_out_buffers.getReadPointer(1) );
            const float*const buffer_3( data_buffer->band_out_buffers.getReadPointer(2) );
            const float*const buffer_4( data_buffer->band_out_buffers.getReadPointer(3) );
            const float*const buffer_5( data_buffer->band_out_buffers.getReadPointer(4) );
            const float*const buffer_6( data_buffer->band_out_buffers.getReadPointer(5) );
            const float*const buffer_7( data_buffer->band_out_buffers.getReadPointer(6) );
            //const float* const smoothed_distortion = synth_data->final_clipping_smoother.get_smoothed_buffer() ;
            const float* const smoothed_distortion = synth_data->distortion_smoother.get_smoothed_buffer();
            const float* const smoothed_bypass = eq_data->bypass_smoother.get_smoothed_buffer();
            for( int sid = 0 ; sid != num_samples_ ; ++sid )
            {
                const float distortion =  smoothed_distortion[sid];
                const float bypass = smoothed_bypass[sid];
                if( bypass > 0 )
                {
                    const float sum =
                    sample_mix
                    (
                        sample_mix
                        (
                            sample_mix
                            (
                                sample_mix
                                (
                                    sample_mix
                                    (
                                        sample_mix
                                        (
                                            buffer_7[sid],
                                            buffer_6[sid]
                                        ),
                                        buffer_5[sid]
                                    ),
                                    buffer_4[sid]
                                ),
                                buffer_3[sid]
                            ),
                            buffer_2[sid]
                        ),
                        buffer_1[sid]*-1
                    );

                    float mix = sum*bypass + io_buffer_[sid]*(1.0f-bypass);
                    io_buffer_[sid] = soft_clipp_greater_1_2(mix*(1.0f-distortion) + (std::atan( mix*10 )*0.7f)*distortion);
                    //io_buffer_[sid] = soft_clipp_greater_1_2(sample_mix(mix*(1.0f-distortion), sample_mix( mix, mix )*distortion));
                }
                else
                {
                    io_buffer_[sid] = soft_clipp_greater_1_2(io_buffer_[sid]*(1.0f-distortion) + (std::atan( io_buffer_[sid]*10 )*0.7f)*distortion);
                }
            }
        }
    }

public:
    //==============================================================================
    COLD EQProcessor( RuntimeNotifyer*const notifyer_, MoniqueSynthData* synth_data_, mono_ThreadManager*const thread_manager_ ) noexcept
:
    RuntimeListener(notifyer_),

                    thread_manager(thread_manager_),
                    synth_data( synth_data_ ),
                    eq_data( synth_data_->eq_data ),
                    data_buffer( synth_data_->data_buffer )
    {
        std::cout << "MONIQUE: init EQ L OR R" << std::endl;
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            filters.add( new AnalogFilter(notifyer_) );

            frequency_low_pass[band_id] = get_low_pass_band_frequency( band_id );
            frequency_high_pass[band_id] = get_high_pass_band_frequency( band_id );
        }

        sample_rate_changed(0);
    }
    COLD ~EQProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessor)
};

//==============================================================================
//==============================================================================
//==============================================================================
class EQProcessorStereo
{
    MoniqueSynthData*const synth_data;

    EQProcessor*const left_processor;
    EQProcessor*const right_processor;

    EQData*const eq_data;
    DataBuffer*const data_buffer;

public:
    OwnedArray< ENV > envs;

public:
    //==============================================================================
    inline void start_attack() noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            envs.getUnchecked(band_id)->start_attack();
        }
    }
    inline void start_release() noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            envs.getUnchecked(band_id)->set_to_release();
        }
    }
    inline void reset() noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            envs.getUnchecked(band_id)->reset();
        }
        left_processor->reset();
        right_processor->reset();
    }

    //==============================================================================
    inline void process( int num_samples_ ) noexcept
    {
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            ENV* env( envs.getUnchecked(band_id) );
            ENVData* env_data( env->env_data );

            env_data->sustain_smoother.process_amp( not eq_data->hold[band_id], env, data_buffer->band_env_buffers.getWritePointer(band_id), num_samples_ );
        }

        left_processor->process( data_buffer->filter_stereo_output_samples.getWritePointer(LEFT), num_samples_ );
        right_processor->process( data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT), num_samples_ );

        if( Monique_Ui_AmpPainter*const amp_painter = synth_data->audio_processor->amp_painter )
        {
            amp_painter->add_eq( data_buffer->filter_stereo_output_samples.getWritePointer(LEFT), num_samples_ );
        }
    }

public:
    //==========================================================================
    COLD EQProcessorStereo( RuntimeNotifyer*const notifyer_,
                            MoniqueSynthData* synth_data_,
                            mono_ThreadManager*const thread_manager_,
                            const float*const sine_lookup_,
                            const float*const cos_lookup_,
                            const float*const exp_lookup_ ) noexcept
:
    synth_data( synth_data_ ),
                left_processor( new EQProcessor(notifyer_, synth_data_, thread_manager_) ),
                right_processor( new EQProcessor(notifyer_, synth_data_, thread_manager_) ),
                eq_data( synth_data_->eq_data ),
                data_buffer( synth_data_->data_buffer )
    {
        std::cout << "MONIQUE: init EQ" << std::endl;
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            envs.add( new ENV( notifyer_, synth_data_, synth_data_->eq_data->envs.getUnchecked( band_id ), sine_lookup_, cos_lookup_, exp_lookup_ ) );
        }
    }
    COLD ~EQProcessorStereo() noexcept
    {
        delete right_processor;
        delete left_processor;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessorStereo)
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
class Chorus : public RuntimeListener
{
    int buffer_size;
    int index;
    float last_delay;
    float* buffer;

public:
    //==============================================================================
    inline void fill(float sample_) noexcept
    {
        index = index % buffer_size;
        buffer[index] = sample_;
        index++;
    }
#define DELAY_GLIDE 0.01f
    inline float tick(float delay_ ) noexcept
    {
        if( delay_ < last_delay - DELAY_GLIDE)
            delay_ = last_delay - DELAY_GLIDE;
        else if( delay_ > last_delay + DELAY_GLIDE )
            delay_ = last_delay + DELAY_GLIDE;

        last_delay = delay_;

        float i = float(index) - delay_;
        if(i >= buffer_size)
            i -= buffer_size;
        else if(i < 0)
            i += buffer_size;

        int ia = mono_floor(i);
        if (ia >= buffer_size)
            ia = 0;
        int ib = ia + 1;
        if (ib >= buffer_size)
            ib = 0;

        float delta = i-ia;
        if( delta > 1 )
            delta = 0;
        return buffer[ib]*delta + buffer[ia]*(1.0f-delta);
    }
    inline void reset() noexcept
    {
        index = 0;
        last_delay = 210;
        sample_rate_changed(0);
    }
    //==============================================================================
    COLD void sample_rate_changed( double ) noexcept override
    {
        buffer_size = sample_rate/10;
        if( buffer )
            delete[] buffer;
        buffer = new float[buffer_size];
        for( int i = 0 ; i != buffer_size ; ++i )
        {
            buffer[i] = 0;
        }
    }

public:
    //==============================================================================
    COLD Chorus( RuntimeNotifyer*const notifyer_ ) noexcept
:
    RuntimeListener( notifyer_ ),

                     buffer_size(0)
    {
        index = 0;
        last_delay = 210;

        buffer = nullptr;

        sample_rate_changed(0);
    }
    COLD ~Chorus() noexcept
    {
        delete [] buffer;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chorus)
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
class CombFilter
{
    HeapBlock<float> buffer;
    float last;
    int bufferSize, bufferIndex;

public:
    //==============================================================================
    inline float process (const float input, const float feedbackLevel) noexcept
    {
#define REVERB_DAMP 0.0f
        const float output = buffer[bufferIndex];
        last = (output * (1.0f - REVERB_DAMP)) + (last * REVERB_DAMP);
        JUCE_UNDENORMALISE (last);

        float temp = input + (last * feedbackLevel);
        JUCE_UNDENORMALISE (temp);
        buffer[bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;

        return output;
    }

    //==============================================================================
    COLD void setSize (const int size)
    {
        if (size != bufferSize)
        {
            bufferIndex = 0;
            buffer.malloc ((size_t) size);
            bufferSize = size;
        }

        clear();
    }
    COLD void clear() noexcept
    {
        last = 0;
        buffer.clear ((size_t) bufferSize);
    }

public:
    //==============================================================================
    COLD CombFilter() noexcept
:
    last(0),
         bufferSize (0),
         bufferIndex(0)
    {}
    COLD ~CombFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombFilter)
};

//==============================================================================
//==============================================================================
//==============================================================================
class AllPassFilter
{
    HeapBlock<float> buffer;
    int bufferSize, bufferIndex;
public:
    //==============================================================================
    inline float process (const float input) noexcept
    {
        const float bufferedValue = buffer [bufferIndex];
        float temp = input + (bufferedValue * 0.5f);
        JUCE_UNDENORMALISE (temp);
        buffer[bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;

        return bufferedValue - input;
    }

    //==============================================================================
    COLD void setSize (const int size)
    {
        if (size != bufferSize)
        {
            bufferIndex = 0;
            buffer.malloc ((size_t) size);
            bufferSize = size;
        }

        clear();
    }
    COLD void clear() noexcept
    {
        buffer.clear ((size_t) bufferSize);
    }

public:
    //==============================================================================
    COLD AllPassFilter() noexcept
:
    bufferSize(0),
               bufferIndex(0)
    {}
    COLD ~AllPassFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllPassFilter)
};

//==============================================================================
//==============================================================================
//==============================================================================
class LinearSmoothedValue
{
    float currentValue, target, step, lastValue;
    int countdown, stepsToTarget;

public:
    //==========================================================================
    inline float getNextValue() noexcept
    {
        if (countdown <= 0)
        {
            lastValue = target;
        }
        else
        {
            --countdown;
            currentValue += step;

            lastValue = currentValue;
        }

        return lastValue;
    }
    //==========================================================================
    inline float getLastValue() noexcept
    {
        return lastValue;
    }

    inline void setValue (float newValue) noexcept
    {
        if (target != newValue)
        {
            target = newValue;
            countdown = stepsToTarget;

            if (countdown <= 0)
                currentValue = target;
            else
                step = (target - currentValue) / countdown;
        }
    }

    //==============================================================================
    COLD void reset (float sampleRate, float fadeLengthSeconds) noexcept
    {
        stepsToTarget = mono_floor (fadeLengthSeconds * sampleRate);
        currentValue = target;
        countdown = 0;
    }

public:
    //==============================================================================
    COLD LinearSmoothedValue() noexcept
:
    currentValue(0),
                 target(0),
                 step(0),
                 countdown(0),
                 stepsToTarget(0)
    {}

    COLD ~LinearSmoothedValue() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearSmoothedValue)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Reverb;
struct ReverbParameters
{
    float roomSize;     /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
    float wetLevel;     /**< Wet level, 0 to 1.0 */
    float dryLevel;     /**< Dry level, 0 to 1.0 */
    float width;        /**< mono_Reverb width, 0 to 1.0, where 1.0 is very wide. */

private:
    //==============================================================================
    friend class mono_Reverb;
    COLD ReverbParameters() noexcept
:
    roomSize   (0.5f),
               wetLevel   (0.33f),
               dryLevel   (0.4f),
               width      (1.0f)
    {}
    COLD ~ReverbParameters() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbParameters)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Reverb : RuntimeListener
{
    enum { numCombs = 8, numAllPasses = 4 };

    CombFilter comb[numCombs];
    AllPassFilter allPass[numAllPasses];

    ReverbParameters parameters;
#define REVERB_GAIN 0.013f

    LinearSmoothedValue feedback, dryGain, wetGain1, wetGain2;

public:
    //==========================================================================
    inline float processSingleSampleRaw ( float in ) noexcept
    {
        float out = 0;
        {
            const float input = in * REVERB_GAIN;
            const float feedbck = feedback.getNextValue();
            for (int j = 0; j != numCombs; ++j)  // accumulate the comb filters in parallel
            {
                out += comb[j].process (input, feedbck);
            }
        }
        for (int j = 0; j != numAllPasses; ++j)  // run the allpass filters in series
        {
            out = allPass[j].process (out);
        }

        return out * wetGain1.getNextValue() + out * wetGain2.getNextValue() + in * dryGain.getNextValue();
    }

    //==========================================================================
    inline ReverbParameters& get_parameters() noexcept
    {
        return parameters;
    }
    inline void update_parameters() noexcept
    {
#define WET_SCALE_FACTOR 3.0f
#define DRY_SCALE_FACTOR 1.0f
#define ROOM_SCALE_FACTOR 0.28f
#define ROOM_OFFSET 0.7f
        const float wet = parameters.wetLevel * WET_SCALE_FACTOR;
        dryGain.setValue (parameters.dryLevel * DRY_SCALE_FACTOR);
        wetGain1.setValue (0.5f * wet * (1.0f + parameters.width));
        wetGain2.setValue (0.5f * wet * (1.0f - parameters.width));
        feedback.setValue (parameters.roomSize * ROOM_SCALE_FACTOR + ROOM_OFFSET);
    }

    //==============================================================================
    COLD void sample_rate_changed (double) noexcept
    {
        static const int combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
        static const int allPassTunings[] = { 556, 441, 341, 225 };
        const int stereoSpread = 23;
        const int intSampleRate = (int) sample_rate;

        for (int i = 0; i < numCombs; ++i)
        {
            comb[i].setSize ((intSampleRate * combTunings[i]) / 44100);
            comb[i].setSize ((intSampleRate * (combTunings[i] + stereoSpread)) / 44100);
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[i].setSize ((intSampleRate * allPassTunings[i]) / 44100);
            allPass[i].setSize ((intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
        }

        const float smoothTime = 0.01f;
        feedback.reset (sample_rate, smoothTime);
        dryGain.reset (sample_rate, smoothTime);
        wetGain1.reset (sample_rate, smoothTime);
        wetGain2.reset (sample_rate, smoothTime);
    }
    COLD void reset() noexcept
    {
        for (int i = 0; i < numCombs; ++i)
            comb[i].clear();

        for (int i = 0; i < numAllPasses; ++i)
            allPass[i].clear();
    }

public:
    //==========================================================================
COLD mono_Reverb( RuntimeNotifyer*const notifyer_ ) noexcept :
    RuntimeListener( notifyer_ )
    {
        update_parameters();
        sample_rate_changed(0);
    }
    COLD ~mono_Reverb() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Reverb)
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
class FXProcessor
{
    mono_ThreadManager*const thread_manager;

    // REVERB
    mono_Reverb reverb_l;
    mono_Reverb reverb_r;

    // CHORUS
#define DELAY_BUFFER_SIZE 12000
    Chorus chorus_l;
    Chorus chorus_r;
    friend class mono_ParameterOwnerStore;
    ScopedPointer< ENV > chorus_modulation_env;

    // DELAY
    int delayPosition;
    mono_AudioSampleBuffer<2> delayBuffer;

    // FINAL ENV
    friend class MoniqueSynthesiserVoice;
    ScopedPointer<ENV> final_env;

public:
    Smoother velocity_smoother;
    Smoother last_output_smoother;

private:
    const MoniqueSynthData*const synth_data;
    DataBuffer*const data_buffer;
    const ReverbData*const reverb_data;
    const ChorusData*const chorus_data;

public:
    //==========================================================================
    inline void process( AudioSampleBuffer& output_buffer_, float velocity_, const int start_sample_final_out_, const int num_samples_ ) noexcept
    {
        velocity_smoother.set_size_in_ms( synth_data->velocity_glide_time );

        // COLLECT BUFFERS
        float* left_input_buffer = data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);
        float* right_input_buffer = data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT);

        // PREPARE REVERB
        const float reverb_room = reverb_data->room;
        const float reverb_dry_wet_mix = 1.0f-reverb_data->dry_wet_mix;
        const float reverb_width = reverb_data->width;
        ReverbParameters& rever_params_l = reverb_l.get_parameters();
        if(
            rever_params_l.roomSize != reverb_room
            || rever_params_l.dryLevel != reverb_dry_wet_mix
            // current_params.wetLevel != r_params.wetLevel
            || rever_params_l.width != reverb_width
        )
        {
            rever_params_l.roomSize = reverb_room;
            rever_params_l.dryLevel = reverb_dry_wet_mix;
            rever_params_l.wetLevel = 1.0f-reverb_dry_wet_mix;
            rever_params_l.width = reverb_width;

            ReverbParameters& rever_params_r = reverb_r.get_parameters();
            rever_params_r.roomSize = rever_params_l.roomSize;
            rever_params_r.dryLevel = rever_params_l.dryLevel;
            rever_params_r.wetLevel = rever_params_l.wetLevel;
            rever_params_r.width = rever_params_l.width;

            reverb_l.update_parameters();
            reverb_r.update_parameters();
        }

        // PREPARE
        {
            chorus_modulation_env->env_data->sustain_smoother.process_amp
            (
                not chorus_data->hold_modulation,
                chorus_modulation_env,
                data_buffer->chorus_env.getWritePointer(),
                num_samples_
            );

            float* const final_env_amp = data_buffer->final_env.getWritePointer();
            final_env->process( final_env_amp, num_samples_ );
            for( int sid = 0 ; sid != num_samples_ ; ++sid )
            {
                const float gain = final_env_amp[sid]*velocity_smoother.add_and_get_average(velocity_);
                left_input_buffer[sid] *= gain;
                right_input_buffer[sid] *= gain;
            }
        }

        // STEREO CHORUS
        {
            // CHORUS
            const float* const chorus_env_buffer( data_buffer->chorus_env.getReadPointer() );
            const float* const smoothed_bypass = synth_data->effect_bypass_smoother.get_smoothed_buffer();
            for( int sid = 0 ; sid != num_samples_ ; ++sid )
            {
                const float in_l = left_input_buffer[sid];
                const float in_r = right_input_buffer[sid];

                const float modulation_amp = chorus_env_buffer[sid];
                const float feedback = modulation_amp*0.85f;

                float tmp_sample_l  = chorus_l.tick((modulation_amp * 220) + 1.51f);
                float tmp_sample_r = chorus_r.tick((modulation_amp * 200) + 1.56f);
                chorus_l.fill( sample_mix (in_l, tmp_sample_r * feedback ) );
                chorus_r.fill( sample_mix (in_r, tmp_sample_l * feedback ) );

                const float bypass = smoothed_bypass[sid];
                left_input_buffer[sid] = sample_mix(tmp_sample_l*bypass, in_l*(1.0f-bypass));
                right_input_buffer[sid] = sample_mix(tmp_sample_r*bypass, in_r*(1.0f-bypass));
            }
        }

        // PROCESS
        {
            struct LeftRightExecuter : public mono_Thread
            {
                FXProcessor*const processor;

                const int num_samples;

                int delay_pos;

                const float* const input_buffer;
                float* const delay_data;
                float* const final_output;

                const float* const smoothed_delay_buffer;
                const float* const smoothed_bypass;

                // LEFT SIDE
                inline void exec() noexcept override
                {
                    // DELAY
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        const float in = input_buffer[sid];
                        const float delay_data_in = delay_data[delay_pos];
                        float sample = delay_data_in+in;
                        delay_data[delay_pos] = sample * smoothed_delay_buffer[sid];
                        if ( ++delay_pos >= DELAY_BUFFER_SIZE )
                        {
                            delay_pos = 0;
                        }

                        const float bypass = smoothed_bypass[sid];
                        final_output[sid] = sample_mix(processor->reverb_l.processSingleSampleRaw( sample )*bypass, input_buffer[sid]*(1.0f-bypass));
                    }
                }

                inline void exec_right() noexcept
                {
                    // DELAY / REVERB / MIX
                    for( int sid = 0 ; sid != num_samples ; ++sid )
                    {
                        // DELAY
                        const float in = input_buffer[sid];
                        const float delay_data_in = delay_data[delay_pos];
                        float sample = delay_data_in+in;
                        delay_data[delay_pos] = sample * smoothed_delay_buffer[sid];
                        if (++delay_pos >= DELAY_BUFFER_SIZE)
                        {
                            delay_pos = 0;
                        }

                        // REVERB
                        // MIX
                        const float bypass = smoothed_bypass[sid];
                        sample = sample_mix(processor->reverb_r.processSingleSampleRaw( sample )*bypass, input_buffer[sid]*(1.0f-bypass));
                        final_output[sid] = sample;
                        processor->last_output_smoother.add( sample );
                    }
                    processor->delayPosition = delay_pos;
                }

                LeftRightExecuter( FXProcessor*const fx_processor_,

                                   const int num_samples_,
                                   const int delay_pos_,

                                   const float* input_buffer_,

                                   float* delay_data_,
                                   float* final_output_
                                 ) noexcept
:
                mono_Thread( fx_processor_->thread_manager ),
                             processor( fx_processor_ ),

                             num_samples( num_samples_ ),
                             delay_pos( delay_pos_ ),

                             input_buffer(input_buffer_),
                             delay_data(delay_data_),
                             final_output(final_output_),

                             smoothed_delay_buffer( fx_processor_->synth_data->delay_smoother.get_smoothed_buffer() ),
                             smoothed_bypass( fx_processor_->synth_data->effect_bypass_smoother.get_smoothed_buffer() )
                {}
            };

            const int delay_pos = delayPosition;
            LeftRightExecuter left_executer( this,

                                             num_samples_,
                                             delay_pos,

                                             left_input_buffer,

                                             delayBuffer.getWritePointer (LEFT),

                                             &output_buffer_.getWritePointer(LEFT)[start_sample_final_out_]
                                           ) ;
            left_executer.try_run_paralel();

            {
                LeftRightExecuter right_executer
                (
                    this,

                    num_samples_,
                    delay_pos,

                    right_input_buffer,

                    delayBuffer.getWritePointer (RIGHT),

                    &output_buffer_.getWritePointer(RIGHT)[start_sample_final_out_]
                ) ;
                right_executer.exec_right();
            }

            while( left_executer.isWorking() ) {}
            // FINAL MIX
            {
                const float*const smoothed_volume_buffer = synth_data->volume_smoother.get_smoothed_buffer();
                float*const left_buffer = &output_buffer_.getWritePointer(LEFT)[start_sample_final_out_];
                float*const right_buffer = &output_buffer_.getWritePointer(RIGHT)[start_sample_final_out_];
                for( int sid = 0 ; sid != num_samples_ ; ++sid )
                {
                    const float volume = smoothed_volume_buffer[sid];

                    left_buffer[sid] *= volume*2;
                    right_buffer[sid] *= volume*2;
                }

                // VISUALIZE BEFORE FONAL OUT
                if( Monique_Ui_SegmentedMeter*meter = synth_data->audio_processor->peak_meter )
                {
                    ScopedLock locked(synth_data->audio_processor->peak_meter_lock);
                    meter->process( left_buffer, num_samples_ );
                }

                for( int sid = 0 ; sid != num_samples_ ; ++sid )
                {
                    const float volume = smoothed_volume_buffer[sid];

                    left_buffer[sid] = soft_clipp_greater_0_9( left_buffer[sid] ) ;
                    right_buffer[sid] = soft_clipp_greater_0_9( right_buffer[sid] ) ;
                }
                // VISUALIZE
                if( Monique_Ui_AmpPainter* amp_painter = synth_data->audio_processor->amp_painter )
                {
                    amp_painter->add_out( left_buffer, right_buffer, num_samples_ );
                    amp_painter->add_out_env( data_buffer->final_env.getReadPointer(), num_samples_ );
                }
            }
        }
    }

    //==========================================================================
    void start_attack() noexcept
    {
        chorus_modulation_env->start_attack();
        final_env->start_attack();
    }
    void start_release( bool is_sustain_pedal_down_, bool is_sostenuto_pedal_down_ ) noexcept
    {
        if( not is_sostenuto_pedal_down_ )
        {
            chorus_modulation_env->set_to_release();
        }
        if( not is_sustain_pedal_down_ )
        {
            final_env->set_to_release();
        }
    }
    void reset() noexcept
    {
        reverb_l.reset();
        reverb_r.reset();
        chorus_l.reset();
        chorus_r.reset();
        chorus_modulation_env->reset();
        delayPosition = 0;
        final_env->reset();

        last_output_smoother.reset(50);
    }

public:
    //==============================================================================
    COLD FXProcessor( RuntimeNotifyer*const notifyer_,
                      MoniqueSynthData* synth_data_,
                      mono_ThreadManager*const thread_manager_,
                      const float*const sine_lookup_,
                      const float*const cos_lookup_,
                      const float*const exp_lookup_ ) noexcept
:
    thread_manager(thread_manager_),

                   reverb_l(notifyer_),
                   reverb_r(notifyer_),

                   chorus_l(notifyer_),
                   chorus_r(notifyer_),

                   chorus_modulation_env( new ENV( notifyer_, synth_data_, synth_data_->chorus_data->env_data, sine_lookup_, cos_lookup_, exp_lookup_ ) ),

                   delayPosition( 0 ),
                   delayBuffer ( DELAY_BUFFER_SIZE ),

                   final_env( new ENV( notifyer_, synth_data_, synth_data_->env_data, sine_lookup_, cos_lookup_, exp_lookup_ ) ),

                   velocity_smoother( notifyer_, synth_data_->velocity_glide_time ),
                   last_output_smoother( notifyer_, 50 ),

                   synth_data( synth_data_ ),
                   data_buffer( synth_data_->data_buffer ),
                   reverb_data( synth_data_->reverb_data ),
                   chorus_data( synth_data_->chorus_data )
    {
        std::cout << "MONIQUE: init FX" << std::endl;
    }

    COLD ~FXProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXProcessor)
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
class ArpSequencer : public RuntimeListener
{
    RuntimeInfo*const info;
    const ArpSequencerData*const data;

    friend class MoniqueSynthesiserVoice;

    int current_step;
    int next_step_on_hold;
    int step_at_sample_current_buffer;

    int shuffle_to_back_counter;
    bool found_a_step;

public:
    //==============================================================================
    // RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
    // RETURN NUM SAMPLES IF THERE IS NO STEP IN THE BUFFER
    inline int process_samples_to_next_step( int start_sample_, int num_samples_ ) noexcept
    {
        double samples_per_min = sample_rate*60;
        double speed_multi = ArpSequencerData::speed_multi_to_value(data->speed_multi);
        double steps_per_min = info->bpm*4/1.0 * speed_multi;
        double steps_per_sample = steps_per_min/samples_per_min;
        int64 sync_sample_pos = info->samples_since_start+start_sample_;
        int64 step = next_step_on_hold;
        step_at_sample_current_buffer = -1;

        double samples_per_step = samples_per_min/steps_per_min; // WILL BE OVERRIDDEN IN STANDALONE!
        for( int i = 0 ; i < num_samples_; ++i )
        {
#ifdef IS_STANDALONE
            if( info->is_extern_synced )
            {
                OwnedArray< RuntimeInfo::Step >& steps_in_block( info->steps_in_block );
                if( steps_in_block.size() )
                {
                    RuntimeInfo::Step& step__( *steps_in_block.getFirst() );
                    if( step__.at_absolute_sample == sync_sample_pos )
                    {
                        step = step__.step_id;
                        samples_per_step = step__.samples_per_step;
                        steps_in_block.remove(0,true);
                    }
                }
            }
            else
#endif
            {
                step = mono_floor(steps_per_sample*sync_sample_pos)+1; // +1 for future processing
            }

            --shuffle_to_back_counter;

            if( step != next_step_on_hold )
            {
                next_step_on_hold = step;

                if( current_step % 2 == 0 )
                    shuffle_to_back_counter = mono_floor(samples_per_step * ArpSequencerData::shuffle_to_value( data->shuffle ));
                else
                    shuffle_to_back_counter = 0;

                found_a_step = true;
            }

            if( found_a_step and shuffle_to_back_counter == 0 )
            {
                step_at_sample_current_buffer = i;
                current_step = next_step_on_hold;
                return i;
            }

            sync_sample_pos++;
        }

        return num_samples_;
    }

    //==============================================================================
    inline int get_current_step() const noexcept
    {
        return current_step % SUM_ENV_ARP_STEPS;
    }
    inline int get_step_before() const noexcept
    {
        if( current_step > 0 )
            return (current_step-1) % SUM_ENV_ARP_STEPS;
        else
            return 0;
    }
    inline float get_current_tune() const noexcept
    {
        return data->tune[get_current_step()].get_value();
    }
    inline bool found_last_process_a_step() const noexcept
    {
        return step_at_sample_current_buffer != -1;
    }
    inline bool last_found_step_is_enabled() const noexcept
    {
        bool success = false;
        if( found_last_process_a_step() )
        {
            if( data->step[get_current_step()] )
            {
                success = true;
            }
        }

        return success;
    }
    inline void reset() noexcept
    {
        current_step = 0;
    }

public:
    //==============================================================================
    COLD ArpSequencer( RuntimeNotifyer*const notifyer_, RuntimeInfo* info_, const ArpSequencerData* data_ ) noexcept
:
    RuntimeListener( notifyer_ ),

                     info(info_),
                     data(data_),
                     current_step(0),
                     next_step_on_hold(0),
                     step_at_sample_current_buffer(-1),

                     shuffle_to_back_counter(0),
                     found_a_step(false)
    {
        std::cout << "MONIQUE: init SEQUENCER" << std::endl;
    }

    COLD ~ArpSequencer() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpSequencer)
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
COLD MoniqueSynthesiserVoice::MoniqueSynthesiserVoice( MoniqueAudioProcessor*const audio_processor_,
        MoniqueSynthData*const synth_data_,
        RuntimeNotifyer*const notifyer_,
        RuntimeInfo*const info_,
        DataBuffer*data_buffer_ ) noexcept
:
audio_processor( audio_processor_ ),
                 synth_data( synth_data_ ),
                 thread_manager( new mono_ThreadManager( synth_data_ ) ),

                 info( info_ ),
                 data_buffer( data_buffer_ ),

                 arp_sequencer( new ArpSequencer( notifyer_, info, synth_data_->arp_sequencer_data ) ),
                 eq_processor( new EQProcessorStereo( notifyer_, synth_data_, thread_manager, synth_data_->sine_lookup, synth_data_->cos_lookup, synth_data_->exp_lookup ) ),
                 fx_processor( new FXProcessor( notifyer_, synth_data_, thread_manager, synth_data_->sine_lookup, synth_data_->cos_lookup, synth_data_->exp_lookup ) ),

                 current_note(-1),
                 pitch_offset(0),

                 is_sostenuto_pedal_down(false),
                 stopped_and_sostenuto_pedal_was_down(false),
                 is_soft_pedal_down(false),
                 was_soft_pedal_down_on_note_start(false),
                 is_sustain_pedal_down(false),
                 stopped_and_sustain_pedal_was_down(false),

                 current_velocity(0),
                 current_step(0),
                 an_arp_note_is_already_running(false),
                 sample_position_for_restart_arp(-1)
{
    std::cout << "MONIQUE: init BUFFERS's" << std::endl;

    std::cout << "MONIQUE: init OSC's" << std::endl;
    master_osc = new MasterOSC( notifyer_, synth_data_, synth_data_->sine_lookup  );
    second_osc = new SecondOSC( notifyer_, synth_data_, 1, synth_data_->sine_lookup );
    third_osc = new SecondOSC( notifyer_, synth_data_, 2, synth_data_->sine_lookup );

    std::cout << "MONIQUE: init LFO's" << std::endl;
    lfos = new LFO*[SUM_LFOS];
    for( int i = 0 ; i != SUM_LFOS ; ++i )
    {
        lfos[i] = new LFO( notifyer_, synth_data_, i, synth_data_->sine_lookup );
    }

    std::cout << "MONIQUE: init FILTERS & ENVELOPES" << std::endl;
    filter_processors = new FilterProcessor*[SUM_FILTERS];
    for( int i = 0 ; i != SUM_FILTERS ; ++i )
    {
        filter_processors[i] = new FilterProcessor( notifyer_, synth_data_, thread_manager, i, synth_data_->sine_lookup, synth_data_->cos_lookup, synth_data_->exp_lookup );
    }
}
COLD MoniqueSynthesiserVoice::~MoniqueSynthesiserVoice() noexcept
{
    for( int i = SUM_FILTERS-1 ; i > -1 ; --i )
    {
        delete filter_processors[i];
    }
    for( int i = SUM_LFOS-1 ; i > -1 ; --i )
    {
        delete lfos[i];
    }
    delete third_osc;
    delete second_osc;
    delete master_osc;

    delete arp_sequencer;
    delete eq_processor;
    delete fx_processor;

    delete thread_manager;
}

//==============================================================================
void MoniqueSynthesiserVoice::startNote( int midi_note_number_, float velocity_, SynthesiserSound* /*sound*/, int pitch_ )
{
    start_internal( midi_note_number_, velocity_ );
}
void MoniqueSynthesiserVoice::start_internal( int midi_note_number_, float velocity_ ) noexcept
{
    stopped_and_sostenuto_pedal_was_down = false;
    stopped_and_sustain_pedal_was_down = false;
    was_soft_pedal_down_on_note_start = is_soft_pedal_down;

    current_note = audio_processor->are_more_than_one_key_down() ? current_note : midi_note_number_;
    current_note = midi_note_number_;
    current_velocity = velocity_;

    // OSCS
    bool is_arp_on = synth_data->arp_sequencer_data->is_on;
    float arp_offset = is_arp_on ? arp_sequencer->get_current_tune() : 0;

    float note = current_note+arp_offset+pitch_offset;
    master_osc->update( note );
    second_osc->update( note );
    third_osc->update( note );

    // PROCESSORS
    bool start_up = true;
    const bool arp_connect = synth_data->arp_sequencer_data->connect;
    if( is_arp_on and arp_connect and an_arp_note_is_already_running )
    {
        start_up = false;
    }
    else if( current_note == -1 )
    {
        start_up = false;
    }

    if( start_up )
    {
        for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id )
        {
            filter_processors[voice_id]->start_attack();
        }
        eq_processor->start_attack();
        fx_processor->start_attack();
    }
}
void MoniqueSynthesiserVoice::stopNote( float, bool allowTailOff )
{
    if( not synth_data->arp_sequencer_data->is_on )
    {
        if( allowTailOff )
        {
            stop_internal();
        }
        else
        {
            stop_internal();
            clearCurrentNote();
        }
    }
}
void MoniqueSynthesiserVoice::stop_arp() noexcept
{
    arp_info.current_note = -1;
    sample_position_for_restart_arp = -1;
    if( synth_data->arp_sequencer_data->is_on )
    {
        arp_info.current_note = current_note;
        arp_info.current_velocity = current_velocity;

        stop_internal();
        current_note = -1;
        //clearCurrentNote();
    }
}
void MoniqueSynthesiserVoice::restart_arp( int sample_pos_in_buffer_ ) noexcept
{
    if( arp_info.current_note != -1 )
    {
        current_note = arp_info.current_note;
        current_velocity = arp_info.current_velocity;
        sample_position_for_restart_arp = sample_pos_in_buffer_;
    }
}
void MoniqueSynthesiserVoice::stop_internal() noexcept
{
    stopped_and_sostenuto_pedal_was_down = is_sostenuto_pedal_down;
    stopped_and_sustain_pedal_was_down = is_sustain_pedal_down;

    if( not is_sostenuto_pedal_down )
    {
        eq_processor->start_release();
        for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id )
        {
            filter_processors[voice_id]->start_release();
        }
    }
    fx_processor->start_release( is_sustain_pedal_down, is_sostenuto_pedal_down );
}

//==============================================================================
void MoniqueSynthesiserVoice::release_if_inactive() noexcept
{
    if( not synth_data->arp_sequencer_data->is_on and fx_processor->final_env->get_current_stage() == END_ENV )
    {
        const float last_out_average = fx_processor->last_output_smoother.get_average();
        if( last_out_average < 0.0000001f and last_out_average > -0.0000001f )
        {
            reset();
        }
    }
}

//==============================================================================
void MoniqueSynthesiserVoice::renderNextBlock ( AudioSampleBuffer& output_buffer_, int start_sample_, int num_samples_ )
{
    // GET POSITION INFOS
#ifdef IS_STANDALONE
    if( synth_data->sync )
    {
        if( info->is_extern_synced )
        {
            info->bpm = audio_processor->get_current_pos_info().bpm;
        }
        else
        {
            info->bpm = synth_data->speed;
        }
    }
    else
    {
        info->is_extern_synced = false;
        info->bpm = synth_data->speed;
    }
#else
    if( synth_data->sync )
    {
        info->bpm = audio_processor->get_current_pos_info().bpm;
    }
    else
    {
        info->bpm = synth_data->speed;
    }
#endif
    info->samples_since_start = audio_processor->get_current_pos_info().timeInSamples;

    int count_start_sample = start_sample_;
    int counted_samples = num_samples_;
    bool is_a_step = false;
    const bool connect = synth_data->arp_sequencer_data->connect;
    const bool arp_is_on = synth_data->arp_sequencer_data->is_on;
    while( counted_samples > 0 )
    {
        // SEARCH FOR STEPS (16ths) IN THE CURRENT BUFFER RANGE
        int samples_to_next_arp_step_in_this_buffer = arp_sequencer->process_samples_to_next_step( count_start_sample, counted_samples );
        counted_samples -= samples_to_next_arp_step_in_this_buffer;

        // RENDER THE NEXT BLOCK
        int step_id = arp_sequencer->get_current_step();

        // FORCE AN ARP RESTART
        if( sample_position_for_restart_arp > -1 and step_id == 0 )
        {
            start_internal( current_note, current_velocity );
            an_arp_note_is_already_running = true;
            sample_position_for_restart_arp = -1;
        }
        if( samples_to_next_arp_step_in_this_buffer > 0 )
        {
            render_block( output_buffer_, is_a_step ? step_id : -1, count_start_sample, samples_to_next_arp_step_in_this_buffer );
        }
        count_start_sample += samples_to_next_arp_step_in_this_buffer;

        // HANDLE RETIGGERS
        is_a_step = arp_sequencer->found_last_process_a_step();
        bool is_step_enabled = arp_sequencer->last_found_step_is_enabled();
        if ( is_a_step and an_arp_note_is_already_running and ( not connect or not arp_is_on or not is_step_enabled ) )
        {
            stop_internal();
            an_arp_note_is_already_running = false;
        }

        const bool is_a_new_arp_step_to_start = (arp_is_on and is_a_step and is_step_enabled);
        if( is_a_new_arp_step_to_start )
        {
            start_internal( current_note, current_velocity );
            an_arp_note_is_already_running = true;
        }
    }

    // FREE IT
    release_if_inactive();
}
void MoniqueSynthesiserVoice::render_block ( AudioSampleBuffer& output_buffer_, int step_number_, int start_sample_, int num_samples_) noexcept
{
    if( current_note == -1 )
        return;


    const int num_samples = num_samples_;
    if( num_samples == 0 )
    {
        return;
    }

    if( step_number_ != -1 )
    {
        current_step = step_number_;
    }

    // SMOOTH PARAMETERS
    synth_data->smooth_manager->smooth(num_samples_, synth_data->glide_motor_time );

    // MULTI THREADED FLT_ENV / LFO / OSC
    {
        // MAIN THREAD // NO DEPENCIES
        lfos[0]->process( step_number_, start_sample_, num_samples );
        filter_processors[0]->env->process( data_buffer->filter_env_amps.getWritePointer(0), num_samples );
        master_osc->process( data_buffer, num_samples ); // NEED LFO 0

        // SUB THREAD
        // DEPENCIES OSC 0
        struct Executer : public mono_Thread
        {
            MoniqueSynthesiserVoice*const voice;
            const int num_samples;
            const int start_sample;
            const int step_number;
            void exec() noexcept override
            {
                voice->filter_processors[1]->env->process( voice->data_buffer->filter_env_amps.getWritePointer(1), num_samples );
                voice->lfos[1]->process( step_number, start_sample, num_samples );
                voice->second_osc->process( voice->data_buffer, num_samples );
            }
            Executer(MoniqueSynthesiserVoice*const voice_,
            int num_samples_,
            int start_sample_,
            int step_number_)
                :
                mono_Thread( voice_->thread_manager ),
                voice(voice_),
                num_samples(num_samples_),
                start_sample(start_sample_),
                step_number(step_number_)
            {}
        };
        Executer executer( this, num_samples, start_sample_, step_number_ );
        executer.try_run_paralel();

        lfos[2]->process( step_number_, start_sample_, num_samples );
        filter_processors[2]->env->process( data_buffer->filter_env_amps.getWritePointer(2), num_samples );
        third_osc->process( data_buffer, num_samples ); // NEED OSC 0 && LFO 2

        while( executer.isWorking() ) {}
    }

    filter_processors[0]->process( num_samples );
    filter_processors[1]->process( num_samples );
    filter_processors[2]->process( num_samples );

    eq_processor->process( num_samples );


    float velocity_to_use = current_velocity;
                            if( synth_data->arp_sequencer_data->is_on )
    {
        velocity_to_use *= synth_data->arp_sequencer_data->velocity[current_step];
    }
    fx_processor->process( output_buffer_, velocity_to_use, start_sample_, num_samples_ );
    // OSCs - THREAD 1 ?

    // VISUALIZE
    if( Monique_Ui_AmpPainter* amp_painter = synth_data->audio_processor->amp_painter )
    {
        amp_painter->add_master_osc( data_buffer->osc_samples.getReadPointer(0), data_buffer->osc_switchs.getReadPointer(0), num_samples_ );
        amp_painter->add_osc( 1, data_buffer->osc_samples.getReadPointer(1), num_samples_ );
        amp_painter->add_osc( 2, data_buffer->osc_samples.getReadPointer(2), num_samples_ );
    }

    // UI INFORMATIONS
    for( int i = 0 ; i != SUM_OSCS ; ++i )
    {
        synth_data->osc_datas[i]->last_modulation_value = data_buffer->lfo_amplitudes.getReadPointer(i)[num_samples-1];
    }
}

void MoniqueSynthesiserVoice::pitchWheelMoved (int pitch_ )
{
    pitch_offset = (pitch_ > 0x2000 ? 2.0f/0x2000*(pitch_-0x2000) : -2.0f/0x2000*(0x2000-pitch_));

    bool is_arp_on = synth_data->arp_sequencer_data->is_on;
    float arp_offset = is_arp_on ? arp_sequencer->get_current_tune() : 0;
    master_osc->update( current_note+arp_offset+pitch_offset );
    second_osc->update( current_note+arp_offset+pitch_offset );
    third_osc->update( current_note+arp_offset+pitch_offset );
}

//==============================================================================
void MoniqueSynthesiserVoice::reset() noexcept
{
    if( current_note != -1 )
    {
        current_note = -1;
        reset_internal();
        clearCurrentNote();
    }
}
void MoniqueSynthesiserVoice::reset_internal() noexcept
{
    // TODO reset oscs to zero
    for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id )
    {
        filter_processors[voice_id]->reset();
    }
    eq_processor->reset();
    fx_processor->reset();

    master_osc->reset();
    second_osc->reset();
    third_osc->reset();
}
void MoniqueSynthesiserVoice::handle_sustain_pedal( bool down_ ) noexcept
{
    is_sustain_pedal_down = down_;
    if( not down_ )
    {
        if( stopped_and_sustain_pedal_was_down )
        {
            stop_internal();
        }
    }
}
void MoniqueSynthesiserVoice::handle_sostueno_pedal( bool down_ ) noexcept
{
    is_sostenuto_pedal_down = down_;
    if( not down_ )
    {
        if( stopped_and_sostenuto_pedal_was_down )
        {
            stop_internal();
        }
    }
}
void MoniqueSynthesiserVoice::handle_soft_pedal( bool down_ ) noexcept
{
    is_soft_pedal_down = down_;
}

//==============================================================================
float MoniqueSynthesiserVoice::get_filter_env_amp( int filter_id_ ) const noexcept
{
    return filter_processors[filter_id_]->env->get_amp();
}
float MoniqueSynthesiserVoice::get_lfo_amp( int lfo_id_ ) const noexcept
{
    return lfos[lfo_id_]->get_current_amp();
}
float MoniqueSynthesiserVoice::get_arp_sequence_amp( int step_ ) const noexcept
{
    if( arp_sequencer->get_current_step() == step_ )
        return fx_processor->final_env->get_amp();

    return 0;
}
float MoniqueSynthesiserVoice::get_current_frequency() const noexcept
{
    return MidiMessage::getMidiNoteInHertz(current_note+arp_sequencer->get_current_tune());
}
float MoniqueSynthesiserVoice::get_flt_input_env_amp( int flt_id_, int input_id_ ) const noexcept
{
    return filter_processors[flt_id_]->input_envs[input_id_]->get_amp();
}
float MoniqueSynthesiserVoice::get_band_env_amp( int band_id_ ) const noexcept
{
    return eq_processor->envs[band_id_]->get_amp();
}
float MoniqueSynthesiserVoice::get_chorus_modulation_env_amp() const noexcept
{
    return fx_processor->chorus_modulation_env->get_amp();
}

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueSynthesizer::handleSustainPedal(int, bool isDown)
{
    voice->handle_sustain_pedal( isDown );
}

void MoniqueSynthesizer::handleSostenutoPedal(int, bool isDown)
{
    voice->handle_sostueno_pedal( isDown );
}

void MoniqueSynthesizer::handleSoftPedal(int, bool isDown)
{
    voice->handle_soft_pedal( isDown );
}
void MoniqueSynthesizer::handleBankSelect (int controllerValue) noexcept
{
    synth_data->set_current_bank( jmin(26,controllerValue) );
}
void MoniqueSynthesizer::handleProgramChange (int midiChannel, int programNumber)
{
    if( programNumber != synth_data->get_current_program() )
    {
        synth_data->set_current_program( programNumber );
        if( programNumber == synth_data->get_current_program() )
        {
            synth_data->load(true,true);
        }
    }
}
void MoniqueSynthesizer::handleController (int midiChannel, int cc_number_, int cc_value_)
{
    switch (cc_number_)
    {
    case 0:
        handleBankSelect (cc_value_);
        break;
    case 0x40:
        handleSustainPedal   (midiChannel, cc_value_ >= 64);
        break;
    case 0x42:
        handleSostenutoPedal (midiChannel, cc_value_ >= 64);
        break;
    case 0x43:
        handleSoftPedal      (midiChannel, cc_value_ >= 64);
        break;
    default :
    {
        Parameter*const learing_param = midi_control_handler->is_learning();
        Array< Parameter* >& paramters = synth_data->get_all_parameters();

        // CONTROLL
        if( not learing_param )
        {
            for( int i = 0 ; i != paramters.size() ; ++ i )
            {
                Parameter*const param = paramters.getUnchecked(i);
                if( param->midi_control->read_from_if_you_listen( cc_number_, cc_value_, synth_data->midi_pickup_offset ) )
                {
                    break;
                }
            }
        }
        // TRAIN
        else
        {
            if( midi_control_handler->handle_incoming_message( cc_number_ ) )
            {
                // CLEAR SIBLINGS IF WE HAVE SOMETHING SUCCESSFUL TRAINED
                String learning_param_name = learing_param->get_info().name;
                for( int i = 0 ; i != paramters.size() ; ++ i )
                {
                    Parameter*const param = paramters.getUnchecked(i);
                    if( param != learing_param )
                    {
                        if( param->midi_control->get_is_ctrl_version_of_name() != learning_param_name )
                        {
                            if( param->midi_control->is_listen_to( cc_number_ ) )
                            {
                                param->midi_control->clear();
                            }
                        }
                    }
                }
            }
        }
    }
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD MoniqueSynthesiserSound::MoniqueSynthesiserSound() noexcept {}
COLD MoniqueSynthesiserSound::~MoniqueSynthesiserSound() noexcept {}

//==============================================================================
bool MoniqueSynthesiserSound::appliesToNote(int)
{
    return true;
}
bool MoniqueSynthesiserSound::appliesToChannel(int)
{
    return true;
}

//==============================================================================
void MoniqueSynthData::get_full_adstr( ENVData&env_data_, Array< float >& curve ) noexcept
{
    ENV env( runtime_notifyer, this, &env_data_, sine_lookup, cos_lookup, exp_lookup );
    env.start_attack();
    const float sample_rate = runtime_notifyer->get_sample_rate();
    const float sustain = env_data_.sustain;
    int count_sustain = msToSamplesFast( env_data_.sustain_time*10000, sample_rate );
    while( env.get_current_stage() != END_ENV )
    {
        float sample;
        env.process( &sample, 1 );
        curve.add( sample );

        if( env.get_current_stage() == SUSTAIN )
        {
            //if( not env.is_endless() )
            {
                if( --count_sustain <= 0 )
                {
                    env.set_to_release();
                }
            }
        }
    }
}


