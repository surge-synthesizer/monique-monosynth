
#include "monique_core_Synth.h"
#include "monique_core_Datastructures.h"

#include "monique_ui_AmpPainter.h"
#include "monique_core_Processor.h"


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
    inline mono_MultiThreaded() noexcept;
    inline ~mono_MultiThreaded() noexcept;

private:
    //==============================================================================
    Thread* thread;

    friend class mono_ExecuterThread;   // MULTI THREADED EXECUTION
    friend class mono_ThreadManager; 	// CALLER THREAD EXECUTION
    virtual void exec() noexcept = 0;

public:
    //==============================================================================
    inline bool isWorking() const noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_MultiThreaded)
};

//==============================================================================
inline mono_MultiThreaded::mono_MultiThreaded() noexcept {}
inline mono_MultiThreaded::~mono_MultiThreaded() noexcept {}

//==============================================================================
inline bool mono_MultiThreaded::isWorking() const noexcept
{
    bool is_working;
    if( thread )
        is_working = thread->isThreadRunning();
    else
        is_working = false;

    return is_working;
}

//==============================================================================
//==============================================================================
//==============================================================================
class mono_ExecuterThread : protected Thread
{
    friend class mono_ThreadManager;
    mono_MultiThreaded* executeable;

    //==============================================================================
    void run() override
    {
        executeable->exec();
    }

private:
    //==============================================================================
    COLD mono_ExecuterThread() noexcept;

public:
    //==============================================================================
    COLD ~mono_ExecuterThread() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ExecuterThread)
};

//==============================================================================
COLD mono_ExecuterThread::mono_ExecuterThread() noexcept :
Thread("monique_engine_WorkerThread") {}
COLD mono_ExecuterThread::~mono_ExecuterThread() noexcept {}

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
    inline void execute_me( mono_MultiThreaded*const executer_ ) noexcept;

private:
    //==============================================================================
    COLD mono_ThreadManager() noexcept;
    COLD ~mono_ThreadManager() noexcept;

public:
    juce_DeclareSingleton (mono_ThreadManager,false)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_ThreadManager)
};

//==============================================================================
juce_ImplementSingleton (mono_ThreadManager)
COLD mono_ThreadManager::mono_ThreadManager() noexcept
:
synth_data( GET_DATA_PTR( synth_data ) )
{
    for( int i = 0 ; i < THREAD_LIMIT ; ++i )
    {
        mono_ExecuterThread*thread( new mono_ExecuterThread() );
        thread->setPriority(10);
        threads.add( thread );
    }
    threads.minimiseStorageOverheads();
}
COLD mono_ThreadManager::~mono_ThreadManager() noexcept
{
    for( int i = 0 ; i < THREAD_LIMIT ; ++i )
    {
        threads[i]->signalThreadShouldExit();
        threads[i]->waitForThreadToExit(200);
        delete threads[i];
    }
    clearSingletonInstance();
}

//==============================================================================
inline void mono_ThreadManager::execute_me( mono_MultiThreaded*const executer_ ) noexcept
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

//==============================================================================
//==============================================================================
//==============================================================================
struct mono_Thread : public mono_MultiThreaded
{
    //==============================================================================
    inline mono_Thread() noexcept;
    inline ~mono_Thread() noexcept;

    //==============================================================================
    // IT CHECKS FOR FREE THREADS, OTHERWISE IT RUNS FROM THE CALLER THREAD
    inline void try_run_paralel() noexcept
    {
        mono_ThreadManager::getInstanceWithoutCreating()->execute_me(this);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Thread)
};

//==============================================================================
inline mono_Thread::mono_Thread() noexcept {}
inline mono_Thread::~mono_Thread() noexcept {}

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
juce_ImplementSingleton (mono_ParameterOwnerStore)

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
//TODO for 0 to 1 ? or what ever
// TODO rename to param smoother
class ValueSmootherModulatedUpdater;
class ValueSmoother
{
protected:
    friend class ValueSmootherModulatedUpdater;
    Parameter*const base;

    const float min_value;
    const float max_value;

    float current_value;
    float target_value;
    float delta;
    int counter;

public:
    inline float tick() noexcept;
    inline float get_last_tick_value() const noexcept;
    inline void update( int glide_time_in_samples_ ) noexcept;

    inline void reset() noexcept;
    inline void replace_current_value( float value_ ) noexcept;

    COLD ValueSmoother( Parameter*const base_ ) noexcept;
    COLD ValueSmoother( const ValueSmoother& other_ ) noexcept;
    COLD ~ValueSmoother() noexcept;

private:
    //MONO_NOT_CTOR_COPYABLE( ValueSmoother )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmoother )
};

//==============================================================================
COLD ValueSmoother::ValueSmoother( Parameter*const base_ ) noexcept
:
base( base_ ),

      min_value( base_->get_info().min_value ),
      max_value( base_->get_info().max_value ),

      current_value( base_->get_value() ),
      target_value( current_value ),
      delta(0),
      counter(0)
{}

COLD ValueSmoother::ValueSmoother( const ValueSmoother& other_ ) noexcept
:
base( other_.base ),

min_value( other_.base->get_info().min_value ),
max_value( other_.base->get_info().max_value ),

current_value( other_.current_value ),
target_value( other_.target_value ),
delta(other_.delta),
counter(other_.counter)
{}

COLD ValueSmoother::~ValueSmoother() noexcept {}

//==============================================================================
inline float ValueSmoother::tick() noexcept
{
    if( --counter <= 0 )
    {
        current_value = target_value;
    }
    else
    {
        current_value+=delta;
    }

    if( current_value < min_value )
    {
        current_value = min_value;
    }
    else if( current_value > max_value )
    {
        current_value = max_value;
    }

    return current_value;
}
inline float ValueSmoother::get_last_tick_value() const noexcept
{
    return current_value;
}
inline void ValueSmoother::update( int glide_time_in_samples_ ) noexcept
{
    float target = base->get_value();
    if( target_value != target )
    {
        counter = glide_time_in_samples_;
        delta = (target-current_value) / glide_time_in_samples_;
        target_value = target;
    }
}
inline void ValueSmoother::reset() noexcept
{
    target_value = base->get_value();
    current_value = target_value;
    delta = 0;
    counter = 0;
}
inline void ValueSmoother::replace_current_value( float value_ ) noexcept
{
    current_value = value_;
    target_value = value_;
}

//==============================================================================
//==============================================================================
//==============================================================================
class ValueSmootherModulated
{
    ValueSmoother value_smoother;

    Parameter*const base;

    float current_modulation_amount;
    float target_modulation_amount;
    float delta_modulation_amount;
    int modulation_counter;

    const float min_value;
    const float max_value;

    friend class ValueSmootherModulatedUpdater;
    float last_modulation;

public:
    inline float tick( float current_modulation_in_percent_, bool add_modulation_ ) noexcept;
private:
    inline float tick( float current_modulation_in_percent_ ) noexcept;
public:
    inline void update( int glide_time_in_samples_ ) noexcept;
    inline void reset() noexcept;

    COLD ValueSmootherModulated( ModulatedParameter*const base_ ) noexcept;
    COLD ~ValueSmootherModulated() noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( ValueSmootherModulated )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmootherModulated )
};

//==============================================================================
COLD ValueSmootherModulated::ValueSmootherModulated( ModulatedParameter*const base_ ) noexcept
:
value_smoother( base_ ),

                base( base_ ),

                current_modulation_amount( base_->get_modulation_amount() ),
                target_modulation_amount( base_->get_modulation_amount() ),
                delta_modulation_amount( 0 ),
                modulation_counter( 0 ),

                min_value( base_->get_info().min_value ),
                max_value( base_->get_info().max_value ),

                last_modulation(0)
{}
COLD ValueSmootherModulated::~ValueSmootherModulated() noexcept {}

//==============================================================================
inline float ValueSmootherModulated::tick( float current_modulation_in_percent_ ) noexcept
{
    if( --modulation_counter <= 0 )
    {
        current_modulation_amount = target_modulation_amount;
    }
    else
    {
        current_modulation_amount+=delta_modulation_amount;
    }

    if( current_modulation_amount < 1 )
    {
        current_modulation_amount = 1;
    }
    else if( current_modulation_amount > 1 )
    {
        current_modulation_amount = 1;
    }
    last_modulation = current_modulation_in_percent_;

    float smoothed_value = value_smoother.tick();
    if( current_modulation_amount >= 0 )
    {
        smoothed_value += ((max_value-smoothed_value)*current_modulation_amount)*current_modulation_in_percent_;
    }
    else
    {
        smoothed_value += ((smoothed_value-min_value)*current_modulation_amount)*current_modulation_in_percent_;
    }

    return smoothed_value;
}
inline float ValueSmootherModulated::tick( float current_modulation_in_percent_, bool add_modulation_ ) noexcept
{
    float value;
    if( add_modulation_ )
    {
        value = this->tick(current_modulation_in_percent_);
    }
    else
    {
        value = value_smoother.tick();
    }

    if( value < min_value )
    {
        value = min_value;
    }
    else if( value > max_value )
    {
        value = max_value;
    }

    return value;
}
inline void ValueSmootherModulated::update( int glide_time_in_samples_ ) noexcept
{
    value_smoother.update( glide_time_in_samples_ );

    const float target_modulation = base->get_modulation_amount();
    if( target_modulation_amount != target_modulation )
    {
        modulation_counter = glide_time_in_samples_;
        delta_modulation_amount = (target_modulation-current_modulation_amount) / glide_time_in_samples_;
        target_modulation_amount = target_modulation;
    }
}
inline void ValueSmootherModulated::reset() noexcept
{
    value_smoother.reset();

    target_modulation_amount = base->get_modulation_amount();
    current_modulation_amount = target_modulation_amount;
    delta_modulation_amount = 0;
    modulation_counter = 0;
}

//==============================================================================
//==============================================================================
//==============================================================================
class ValueSmootherModulatedTracked : public ValueSmootherModulated
{
    float last_out;
    bool is_changed;

public:
    inline float tick( float current_modulation_in_percent_ ) noexcept;
    inline bool is_changed_since_last_tick() const noexcept;

    COLD ValueSmootherModulatedTracked( ModulatedParameter*const base_ );
    COLD ~ValueSmootherModulatedTracked();

private:
    MONO_NOT_CTOR_COPYABLE( ValueSmootherModulatedTracked )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmootherModulatedTracked )
};

//==============================================================================
COLD ValueSmootherModulatedTracked::ValueSmootherModulatedTracked( ModulatedParameter*const base_ )
    :
    ValueSmootherModulated( base_ ),
    last_out(0),
    is_changed(true)
{}
COLD ValueSmootherModulatedTracked::~ValueSmootherModulatedTracked() {}

//==============================================================================
inline float ValueSmootherModulatedTracked::tick( float current_modulation_in_percent_ ) noexcept
{
    float out = ValueSmootherModulated::tick( current_modulation_in_percent_, true );
    is_changed = out != last_out;
    last_out = out;
    return out;
}
inline bool ValueSmootherModulatedTracked::is_changed_since_last_tick() const noexcept
{
    return is_changed;
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
#define AMP_SMOOTH_SIZE 10 // TODO as option and based to sample rate
class AmpSmoothBuffer : RuntimeListener
{
    int pos;
    float buffer[AMP_SMOOTH_SIZE];
    float sum;

public:
    inline void add( float val_ ) noexcept;
    inline float add_and_get_average( float val_ ) noexcept;
    inline float get_average() const noexcept;
    inline void reset( float value_ = 0 ) noexcept;

private:
    virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept override {
        // TODO resize buffer
    };

public:
    COLD AmpSmoothBuffer();
    COLD ~AmpSmoothBuffer();
};

//==============================================================================
COLD AmpSmoothBuffer::AmpSmoothBuffer() : pos(0), sum(0) {
    reset();
}
COLD AmpSmoothBuffer::~AmpSmoothBuffer() {}

//==============================================================================
inline void AmpSmoothBuffer::add( float val_ ) noexcept {
    sum-=buffer[pos];
    buffer[pos] = val_;
    sum+=val_;

    ++pos;
    if( pos == AMP_SMOOTH_SIZE )
        pos = 0;
}
inline float AmpSmoothBuffer::add_and_get_average( float val_ ) noexcept {
    add(val_);
    return get_average();
}
inline float AmpSmoothBuffer::get_average() const noexcept {
    return sum * ( 1.0f / AMP_SMOOTH_SIZE );
}
inline void AmpSmoothBuffer::reset( float value_ ) noexcept {
    sum = 0;
    for( int i = 0 ; i != AMP_SMOOTH_SIZE ; ++i ) {
        buffer[i] = value_;
        sum += value_;
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
class SwitchSmoother
{
protected:
    float current_value;
    float target_value;
    float delta;
    int counter;
    bool state;

public:
    inline bool reset_counter_on_state_switch( bool maybe_new_state_ ) noexcept;
    inline float tick_to( float current_value_ ) noexcept;
    inline float get_last_tick_value() noexcept;
    inline void reset() noexcept;

    COLD SwitchSmoother() noexcept;
    COLD ~SwitchSmoother() noexcept;

private:
    //MONO_NOT_CTOR_COPYABLE( SwitchSmoother )
    MONO_NOT_MOVE_COPY_OPERATOR( SwitchSmoother )
};

//==============================================================================
COLD SwitchSmoother::SwitchSmoother() noexcept
:
current_value( 0 ),
               target_value( 0 ),
               delta(0),
               counter(0),
               state(true)
{}

COLD SwitchSmoother::~SwitchSmoother() noexcept {}

//==============================================================================
inline bool SwitchSmoother::reset_counter_on_state_switch( bool maybe_new_state_ ) noexcept
{
    if( state != maybe_new_state_ )
    {
        state = maybe_new_state_;
        counter = 500;
        delta = 0;

        return true;
    }

    return false;
}
inline float SwitchSmoother::tick_to( float current_value_ ) noexcept
{
    if( --counter <= 0 )
    {
        current_value = current_value_;
        target_value = current_value_;
        delta = 0;
    }
    else
    {
        //float tmp_old = target_value;
        if( current_value_ != target_value )
        {
            target_value = current_value_;
            delta = (target_value-current_value) / counter;
        }
        //std::cout << counter << " ::: " << "tmp_old:" << tmp_old << " target_value:" << target_value << " delta:" << delta << " delta:" << current_value << std::endl;
    }

    return current_value+=delta;
}
inline float SwitchSmoother::get_last_tick_value() noexcept
{
    return current_value;
}
inline void SwitchSmoother::reset() noexcept
{
    current_value = target_value;
    delta = 0;
    counter = 0;
}

//==============================================================================
//==============================================================================
//==============================================================================
class SwitchAndValueSmootherModulated
{
    friend class ValueSmootherModulatedUpdater;
    SwitchSmoother switch_smoother;
    ValueSmootherModulated value_smoother;

    bool add_modulation;

public:
    //==========================================================================
    inline float tick( float current_modulation_in_percent_ ) noexcept;
    inline void update( int glide_time_in_samples_, bool add_modulation_ ) noexcept;

    inline void reset() noexcept;

    //==========================================================================
    COLD SwitchAndValueSmootherModulated(ModulatedParameter*const base_) noexcept;
    COLD ~SwitchAndValueSmootherModulated() noexcept;
};

//==============================================================================
COLD SwitchAndValueSmootherModulated::SwitchAndValueSmootherModulated(ModulatedParameter*const base_) noexcept
:
value_smoother( base_ ),
                add_modulation(false)
{}
COLD SwitchAndValueSmootherModulated::~SwitchAndValueSmootherModulated() noexcept {}

//==============================================================================
inline float SwitchAndValueSmootherModulated::tick( float current_modulation_in_percent_ ) noexcept
{
    const float value = switch_smoother.tick_to( add_modulation ? current_modulation_in_percent_ : 0 );
    return value_smoother.tick( value, value > 0 );
}
inline void SwitchAndValueSmootherModulated::update( int glide_time_in_samples_, bool add_modulation_ ) noexcept
{
    add_modulation = add_modulation_;
    value_smoother.update( glide_time_in_samples_ );
    switch_smoother.reset_counter_on_state_switch(add_modulation_);
}

inline void SwitchAndValueSmootherModulated::reset() noexcept
{
    switch_smoother.reset();
    value_smoother.reset();
}

//==============================================================================
//==============================================================================
//==============================================================================
// UPDATAES THE MODULATION AMOUNT ON KILL
class ValueSmootherModulatedUpdater
{
    ValueSmootherModulated*const smoother;

public:
    inline ValueSmootherModulatedUpdater( ValueSmootherModulated* smoother_, int glide_time_in_samples_ )  noexcept;
    inline ValueSmootherModulatedUpdater( SwitchAndValueSmootherModulated* smoother_, int glide_time_in_samples_, bool add_modulation_ )  noexcept;
    inline ~ValueSmootherModulatedUpdater() noexcept;
};

//==============================================================================
inline ValueSmootherModulatedUpdater::ValueSmootherModulatedUpdater( ValueSmootherModulated* smoother_, int glide_time_in_samples_ ) noexcept
:
smoother( smoother_ )
{
    smoother_->update(glide_time_in_samples_);
}
inline ValueSmootherModulatedUpdater::ValueSmootherModulatedUpdater( SwitchAndValueSmootherModulated* switch_smoother_, int glide_time_in_samples_, bool add_modulation_ ) noexcept
:
smoother( &switch_smoother_->value_smoother )
{
    switch_smoother_->update(glide_time_in_samples_,add_modulation_);
}
inline ValueSmootherModulatedUpdater::~ValueSmootherModulatedUpdater() noexcept
{
    smoother->base->get_runtime_info().set_last_modulation_amount( smoother->last_modulation );
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
        float distortion_add_on = distortion_power_*0.9;
        input_and_worker_ = (1.0f+distortion_add_on)*input_and_worker_ - (/*0.0f+*/distortion_add_on)*input_and_worker_*input_and_worker_*input_and_worker_;
    }

    return input_and_worker_;
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline float clipp_to_0_and_1( float input_and_worker_ ) noexcept
{
    if( input_and_worker_ > 1 )
        input_and_worker_ = 1;
    else if( input_and_worker_ < 0 )
        input_and_worker_ = 0;

    return input_and_worker_;
}

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
static inline float filter_hard_clipper( float x ) noexcept
{
    return x < -1.2f ? x = -1.2 : x > 1.2 ? x = 1.2 : x;
}

//==============================================================================
//==============================================================================
//==============================================================================
#define TABLESIZE_MULTI 1000
#define SIN_LOOKUP_TABLE_SIZE int(float_Pi*TABLESIZE_MULTI*2)
float* SINE_LOOKUP_TABLE;
class SIN_LOOKUP
{
    COLD SIN_LOOKUP();
    COLD ~SIN_LOOKUP();

public:
    juce_DeclareSingleton (SIN_LOOKUP,false)
};
juce_ImplementSingleton (SIN_LOOKUP)
SIN_LOOKUP*const sine_lookup_self_init = SIN_LOOKUP::getInstance();

COLD SIN_LOOKUP::SIN_LOOKUP()
{
    SINE_LOOKUP_TABLE = new float[SIN_LOOKUP_TABLE_SIZE+1];
    for(int i = 0; i < SIN_LOOKUP_TABLE_SIZE; i++)
        SINE_LOOKUP_TABLE[i] = std::sin( double(i) / TABLESIZE_MULTI );
}
COLD SIN_LOOKUP::~SIN_LOOKUP()
{
    delete[] SINE_LOOKUP_TABLE;

    clearSingletonInstance();
}

static float inline lookup_sine(float x) noexcept
{
    return SINE_LOOKUP_TABLE[ int(int64(x*TABLESIZE_MULTI) % SIN_LOOKUP_TABLE_SIZE) ];
}

//==============================================================================
COLD DataBuffer::DataBuffer( int init_buffer_size_ ) noexcept
:
size( init_buffer_size_ ),

      tmp_multithread_band_buffer_9_4( init_buffer_size_ ),

      lfo_amplitudes( init_buffer_size_ ),
      direct_filter_output_samples( init_buffer_size_ ),

      osc_samples( init_buffer_size_ ),
      osc_switchs( init_buffer_size_ ),
      osc_sync_switchs( init_buffer_size_ ),
      modulator_samples( init_buffer_size_ ),

      filter_output_samples( init_buffer_size_ ),
      filter_env_amps( init_buffer_size_ )
{
    mono_ParameterOwnerStore::getInstance()->data_buffer = this;
}
COLD DataBuffer::~DataBuffer() noexcept {}

//==============================================================================
COLD void DataBuffer::resize_buffer_if_required( int size_ ) noexcept
{
    if( size_ > size )
    {
        size = size_;

        tmp_multithread_band_buffer_9_4.setSize(size_);

        lfo_amplitudes.setSize(size_);
        direct_filter_output_samples.setSize(size_);

        osc_samples.setSize(size_);
        osc_switchs.setSize(size_);
        osc_sync_switchs.setSize(size_);
        modulator_samples.setSize(size_);

        filter_output_samples.setSize(size_);
        filter_env_amps.setSize(size_);
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
class mono_BlitSaw : public RuntimeListener
{
    float last_tick_value;
    float rate_;
    float phase_;
    float p_;
    float C2_;
    float a_;
    float state_;
    unsigned int m_;
    bool _isNewCylce;

public:
    inline float lastOut( void ) const noexcept;
    inline float tick() noexcept;
    inline void reset() noexcept;
    inline void setFrequency( float frequency ) noexcept;
    inline void setPhase( float phase ) noexcept;

    // HACK FOR SYNC
    inline bool isNewCylce() const noexcept;
    inline void clearNewCycleState() noexcept;

private:
    inline void updateHarmonics( void ) noexcept;

public:
    COLD mono_BlitSaw( float frequency = 220.0f );
    COLD ~mono_BlitSaw();
};

// -----------------------------------------------------------------
COLD mono_BlitSaw::mono_BlitSaw( float frequency )
    :
    last_tick_value(0),
    rate_(0),
    phase_(0),
    p_(0),
    C2_(0),
    a_(0),
    state_(0),
    m_(0),
    _isNewCylce(true)
{
    reset();
    setFrequency( frequency );
}
COLD mono_BlitSaw::~mono_BlitSaw() {}

// -----------------------------------------------------------------
inline float mono_BlitSaw::lastOut( void ) const noexcept {
    return last_tick_value;
}
inline float mono_BlitSaw::tick() noexcept
{
    // Avoid a divide by zero, or use of a denormalized divisor
    // at the sinc peak, which has a limiting value of m_ / p_.
    float tmp;
    float denominator = std::sin( phase_ );
    if ( std::fabs(denominator) <= std::numeric_limits<float>::epsilon() )
    {
        tmp = a_;
    }
    else
    {
        tmp = std::sin( phase_*m_ ) / (p_ * denominator);
    }

    tmp +=( state_ - C2_ );
    state_ = tmp * 0.999f;

    phase_ += rate_;
    if ( phase_ >= float_Pi ) {
        phase_ -= float_Pi;
        _isNewCylce = true;
    }
    else
        _isNewCylce = false;

    return last_tick_value = tmp;
}

// -----------------------------------------------------------------
inline void mono_BlitSaw::setPhase( float phase ) noexcept {
    phase_ = float_Pi * phase;
}
inline bool mono_BlitSaw::isNewCylce() const noexcept {
    return _isNewCylce;
}
inline void mono_BlitSaw::clearNewCycleState() noexcept {
    _isNewCylce = false;
}
inline void mono_BlitSaw::reset() noexcept
{
    phase_ = 0;
    state_ = 0;
    last_tick_value = 0;
}
inline void mono_BlitSaw::setFrequency( float frequency ) noexcept
{
    p_ = sample_rate / frequency;
    C2_ = 1.0f / p_;
    rate_ = float_Pi * C2_;
    updateHarmonics();
}
inline void mono_BlitSaw::updateHarmonics( void ) noexcept
{
    m_ = 2 * mono_floor( 0.5f * p_ ) + 1;
    a_ = m_ / p_;
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
class mono_BlitSquare : public RuntimeListener
{
    float last_tick_value;
    float rate_;
    float phase_;
    float p_;
    float a_;
    float lastBlitOutput_;
    float dcbState_;

    unsigned int m_;
    bool _isNewCylce;

public:
    inline float lastOut( void ) const noexcept;
    inline void reset() noexcept;
    inline void setPhase( float phase ) noexcept;

    // HACK FOR SYNC
    inline bool isNewCylce() const noexcept;
    inline void clearNewCycleState() noexcept;
    inline float getPhase() const noexcept;
    inline void setFrequency( float frequency ) noexcept;
    inline float tick( void ) noexcept;

private:
    inline void updateHarmonics( void ) noexcept;

public:
    COLD mono_BlitSquare( float frequency = 220.0f );
    COLD ~mono_BlitSquare();
};

// -----------------------------------------------------------------
COLD mono_BlitSquare::mono_BlitSquare( float frequency )
    :
    last_tick_value(0),
    rate_(0),
    phase_(0),
    p_(0),
    a_(0),
    lastBlitOutput_(0),
    dcbState_(0),
    m_(0),
    _isNewCylce(true)
{
    setFrequency( frequency );
    reset();
}
COLD mono_BlitSquare::~mono_BlitSquare()
{
}

// -----------------------------------------------------------------
inline float mono_BlitSquare::lastOut( void ) const noexcept {
    return last_tick_value;
}
inline float mono_BlitSquare::tick( void ) noexcept
{
    float temp = lastBlitOutput_;

    // Avoid a divide by zero, or use of a denomralized divisor
    // at the sinc peak, which has a limiting value of 1.0.
    float denominator = std::sin( phase_ );
    if ( std::fabs( denominator )  < std::numeric_limits<float>::epsilon() ) {
        // Inexact comparison safely distinguishes betwen *close to zero*, and *close to PI*.
        if ( phase_ < 0.1f || phase_ > (float_Pi*2) - 0.1f )
            lastBlitOutput_ = a_;
        else
            lastBlitOutput_ = -a_;
    }
    else
    {
        lastBlitOutput_ = std::sin( float(m_ * phase_) ) / (p_ * denominator);
    }

    lastBlitOutput_ += temp;

    // Now apply DC blocker.
    last_tick_value = lastBlitOutput_ - dcbState_ + 0.999f * last_tick_value;
    dcbState_ = lastBlitOutput_;

    phase_ += rate_;
    if ( phase_ >= (float_Pi*2) ) {
        phase_ -= (float_Pi*2);
        _isNewCylce = true;
    }
    else
        _isNewCylce = false;

    return last_tick_value;
}

// -----------------------------------------------------------------
inline void mono_BlitSquare::reset() noexcept
{
    phase_ = 0;
    last_tick_value = 0;
    dcbState_ = 0;
    lastBlitOutput_ = 0;
}
inline void mono_BlitSquare::setPhase( float phase ) noexcept {
    phase_ = float_Pi * phase;
}

inline float mono_BlitSquare::getPhase() const noexcept {
    return phase_ * ( 1.0f/float_Pi );
}
inline bool mono_BlitSquare::isNewCylce() const noexcept {
    return _isNewCylce;
}
inline void mono_BlitSquare::clearNewCycleState() noexcept {
    _isNewCylce = false;
}
inline void mono_BlitSquare::setFrequency( float frequency ) noexcept
{
    p_ = 0.5f * sample_rate / frequency;
    rate_ = float_Pi / p_;
    updateHarmonics();
}
inline void mono_BlitSquare::updateHarmonics( void ) noexcept
{
    m_ = (mono_floor( 0.5f * p_ ) + 1) * 2;

    a_ = m_ / p_;
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
class mono_SineWave : public RuntimeListener
{
    float last_tick_value;
    float delta;
    float current_angle;
    float frequency;

    bool _isNewCylce;

public:
    inline float lastOut() const noexcept;
    inline float tick() noexcept;

public:
    inline void setFrequency( float frequency ) noexcept;
    inline void reset() noexcept;

    inline bool isNewCylce() const noexcept;
    inline void clearNewCycleState() noexcept;

private:
    COLD void sample_rate_changed( double old_sr_ ) noexcept override;

public:
    COLD mono_SineWave();
    COLD ~mono_SineWave();
};

// -----------------------------------------------------------------
COLD mono_SineWave::mono_SineWave()
    :
    last_tick_value(0),
    delta(0),
    current_angle(0),
    frequency(0),

    _isNewCylce(0)
{
    setFrequency(440);
}

COLD mono_SineWave::~mono_SineWave() {}

// -----------------------------------------------------------------
inline float mono_SineWave::lastOut() const noexcept {
    return last_tick_value;
}
inline float mono_SineWave::tick() noexcept
{
    // TODO do the lookup unsave!
    float value = lookup_sine( current_angle );
    current_angle += delta;
    if( current_angle > (float_Pi * 2) )
    {
        _isNewCylce = true;
        current_angle -= (float_Pi * 2);
    }
    else
        _isNewCylce = false;

    return last_tick_value = value;
}

// -----------------------------------------------------------------
inline void mono_SineWave::reset( void ) noexcept
{
    current_angle = 0;
    last_tick_value = 0;
}
inline void mono_SineWave::setFrequency( float frequency_ ) noexcept
{
    frequency = frequency_;
    float cyclesPerSample = frequency_ * sample_rate_1ths;
    delta = cyclesPerSample * (float_Pi*2);
}
inline bool mono_SineWave::isNewCylce() const noexcept {
    return _isNewCylce;
}
inline void mono_SineWave::clearNewCycleState() noexcept {
    _isNewCylce = false;
}

COLD void mono_SineWave::sample_rate_changed( double /*old_sr_*/ ) noexcept {
    setFrequency(frequency);
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
class mono_Noise
{
    Random random;
    float last_tick_value;

public:
    inline float lastOut() const noexcept;
    inline float tick() noexcept;

public:
    COLD mono_Noise();
    COLD ~mono_Noise();
};

// -----------------------------------------------------------------
COLD mono_Noise::mono_Noise() : last_tick_value(0) {}
COLD mono_Noise::~mono_Noise() {}

// -----------------------------------------------------------------
inline float mono_Noise::lastOut() const noexcept
{
    return last_tick_value;
}
inline float mono_Noise::tick() noexcept
{
    return last_tick_value = random.nextFloat() * 2 - 1;
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
class mono_OnePole
{
    float last_tick_value;
    float gain;
    float a1, b;

public:
    void setPole( float thePole ) noexcept;
    void setGain( float gain_ ) noexcept;

    float lastOut( void ) const noexcept;
    float tick( float input ) noexcept;

public:
    COLD mono_OnePole( float thePole = 0.9 );
    COLD ~mono_OnePole();

};

// -----------------------------------------------------------------
mono_OnePole::mono_OnePole( float thePole ) :
    last_tick_value(0),
    gain(0),
    a1(0),
    b(0)
{
    this->setPole( thePole );
}

mono_OnePole::~mono_OnePole() {}

// -----------------------------------------------------------------
inline float mono_OnePole::lastOut() const noexcept
{
    return last_tick_value;
}

inline float mono_OnePole::tick( float input ) noexcept
{
    return last_tick_value = b*gain*input - a1*last_tick_value;
}

// -----------------------------------------------------------------
inline void mono_OnePole::setPole( float thePole ) noexcept
{
    if ( thePole > 0 )
        b = 1.0f - thePole;
    else
        b = 1.0f + thePole;

    a1 = -thePole;
}
inline void mono_OnePole::setGain( float gain_ ) noexcept
{
    gain = gain_;
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
class mono_Modulate : public RuntimeListener
{
    mono_SineWave vibrato;
    mono_OnePole filter;
    mono_Noise noise;

    float last_tick_value;
    float vibratoGain;
    unsigned int noiseRate;
    unsigned int noiseCounter;

public:
    inline void reset() noexcept;
    inline void setVibratoRate( float rate ) noexcept;
    inline void setVibratoGain( float gain ) noexcept;
    inline float lastOut() const noexcept;

    inline bool isNewCylce() const noexcept;
    inline void clearNewCycleState() noexcept;

    inline float tick() noexcept;

public:
    COLD mono_Modulate();
    COLD ~mono_Modulate();
};

// -----------------------------------------------------------------
COLD mono_Modulate::mono_Modulate()
    :
    vibrato(),
    filter(),
    noise(),

    last_tick_value(0),
    vibratoGain(0.04),
    noiseRate(0),
    noiseCounter(0)
{
    vibrato.setFrequency( 6.0 );

    noiseRate = (unsigned int) ( 330.0 * sample_rate / 22050.0 );
    noiseCounter = noiseRate;

    filter.setPole( 0.999 );
    filter.setGain( 0.05 );
}
COLD mono_Modulate::~mono_Modulate() {}

// -----------------------------------------------------------------
inline float mono_Modulate::lastOut() const noexcept {
    return last_tick_value;
}
inline float mono_Modulate::tick() noexcept
{
    // Compute periodic and random modulations.
    last_tick_value = vibratoGain * vibrato.tick();
    if ( noiseCounter++ > noiseRate ) {
        noise.tick();
        noiseCounter = 0;
    }
    last_tick_value += filter.tick( noise.lastOut() );

    return last_tick_value;
}

// -----------------------------------------------------------------
inline void mono_Modulate::reset() noexcept {
    last_tick_value = 0;
    vibrato.reset();
}
inline void mono_Modulate::setVibratoRate( float rate ) noexcept {
    vibrato.setFrequency( rate );
}
inline void mono_Modulate::setVibratoGain( float gain ) noexcept {
    vibratoGain = gain;
}
inline bool mono_Modulate::isNewCylce() const noexcept
{
    return vibrato.isNewCylce();
}
inline void mono_Modulate::clearNewCycleState() noexcept
{
    vibrato.clearNewCycleState();
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
class LFO
{
    mono_SineWave sine_generator;

    float last_frequency;

    int last_factor;
    int step_to_wait_for_sync;

    const int id;

    DataBuffer*const data_buffer;
    const LFOData*const lfo_data;
    const RuntimeInfo*const runtime_info;

public:
    //==============================================================================
    inline void process( int step_number_, int num_samples_ ) noexcept;

private:
    inline void sync( int step_number_ ) noexcept;

public:
    //==============================================================================
    float get_current_amp() const noexcept;

private:
    //==============================================================================
    friend class MoniqueSynthesiserVoice;
    COLD LFO( int id_ ) noexcept;
    COLD ~LFO() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFO)
};

//==============================================================================
COLD LFO::LFO( int id_ ) noexcept
:
sine_generator(),

               last_frequency(0),

               last_factor(0),
               step_to_wait_for_sync(false),

               id( id_ ),

               data_buffer( GET_DATA_PTR(data_buffer) ),
               lfo_data( GET_DATA_PTR( lfo_datas[id_] ) ),
               runtime_info( GET_DATA_PTR( runtime_info ) )
{}
COLD LFO::~LFO() noexcept {}

//==============================================================================
inline void LFO::process( int step_number_, int num_samples_ ) noexcept
{
    float* process_buffer( data_buffer->lfo_amplitudes.getWritePointer(id) );
    sync( step_number_ );

    for( int sid = 0 ; sid != num_samples_ ; ++sid )
    {
        process_buffer[sid] = (sine_generator.tick() + 1) * 0.5f;
    }
}
void LFO::sync( int step_number_ ) noexcept
{
    const float speed( lfo_data->speed );
    if( speed <= 6 )
    {
        if( step_number_ == 0 )
        {
            float factor = 1; // 1/1
            if( speed == 0 )
                factor = 16; //return "16/1";
            else if( speed <= 1 )
                factor = 12; //return "12/1";
            else if( speed <= 2 )
                factor = 8;
            else if( speed <= 3 )
                factor = 4;
            else if( speed <= 4 )
                factor = 3;
            else if( speed <= 5 )
                factor = 2;

            const float whole_notes_per_sec( runtime_info->bpm / 4 / 60 ); // = 1 at our slider
            float frequency = whole_notes_per_sec / factor;

            step_to_wait_for_sync--;
            if( last_factor != factor )
            {
                step_to_wait_for_sync = 0;
                last_factor = factor;
            }

            if( step_to_wait_for_sync <= 0 )
            {
                last_frequency = frequency;
                sine_generator.setFrequency( frequency );
                sine_generator.reset();

                step_to_wait_for_sync = last_factor;
            }
        }
    }
    else if( speed <= 17 )
    {
        if( step_number_ == 0 )
        {
            float factor = 0;
            if( speed <= 7 )
                factor = 3.0f/4;
            else if( speed <= 8 )
                factor = 1.0f/2;
            else if( speed <= 9 )
                factor = 1.0f/3;
            else if( speed <= 10 )
                factor = 1.0f/4;
            else if( speed <= 11 )
                factor = 1.0f/8;
            else if( speed <= 12 )
                factor = 1.0f/12;
            else if( speed <= 13 )
                factor = 1.0f/16;
            else if( speed <= 14 )
                factor = 1.0f/24;
            else if( speed <= 15 )
                factor = 1.0f/32;
            else if( speed <= 16 )
                factor = 1.0f/64;
            else
                factor = 1.0f/128;

            const float whole_notes_per_sec( runtime_info->bpm/4 / 60 ); // = 1 at our slider
            double frequency = (whole_notes_per_sec / factor);
            {
                last_frequency = frequency;
                sine_generator.setFrequency( frequency );
                sine_generator.reset();
            }

            step_to_wait_for_sync = 0;
            last_factor = 0;
        }
    }
    else
    {
        float frequency = midiToFrequency(33+speed-18);
        if( frequency != last_frequency )
        {
            last_frequency = frequency;
            sine_generator.setFrequency( frequency );
        }

        step_to_wait_for_sync = 0;
        last_factor = 0;
    }
}

//==============================================================================
float LFO::get_current_amp() const noexcept
{
    return lfo2amp(sine_generator.lastOut());
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
class OSC : public RuntimeListener
{
    const int id;

    float last_frequency;
    int glide_time_in_samples;
    float glide_note_delta;
    SwitchSmoother lfo2fix_octave_smoother;

    bool waiting_for_sync;

    int _root_note;
    int _last_root_note;

    float last_modulator_multi;
    bool waiting_for_modulator_sync;
    int modulator_sync_cycles;
    int current_modulator_sync_cycle;
    int modulator_phase_switch;

    int puls_with_break_counter;
    float current_puls_frequence_offset;
    float puls_swing_amp;
    float puls_swing_delta;
    float puls_swing_multi;
    int puls_swing_switch_counter;
    bool last_puls_was_large;
    bool last_cycle_was_pulse_switch;

    // RAW OSCILATORS
    //==============================================================================
    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;

    mono_Modulate modulator;
    mono_Noise noise;

    ValueSmootherModulatedTracked octave_smoother;
    ValueSmoother fm_amount_smoother;

    // DATA SOURCE
    //==============================================================================
    DataBuffer*const data_buffer;
    const MoniqueSynthData*const synth_data;
    const OSCData* osc_data;
    const OSCData* master_osc_data;

public:
    //==============================================================================
    inline void process(DataBuffer* data_buffer_, const int num_samples_) noexcept;
    inline void reset( int root_note_ ) noexcept;

private:
    //==============================================================================
    friend class MoniqueSynthesiserVoice;
    COLD OSC( const MoniqueSynthData* synth_data_, int id_ ) noexcept;
    COLD ~OSC() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC)
};

//==============================================================================
COLD OSC::OSC( const MoniqueSynthData* synth_data_, int id_ ) noexcept
:
id( id_ ),
    last_frequency( 0 ),
    glide_time_in_samples(0),
    glide_note_delta(0),
    lfo2fix_octave_smoother(),
    waiting_for_sync(false),
    _root_note( 60 ),
    _last_root_note( -1 ),
    last_modulator_multi( 0 ),
    waiting_for_modulator_sync( false ),
    modulator_sync_cycles( 1 ),
    current_modulator_sync_cycle( 0 ),
    modulator_phase_switch( -1 ),
    puls_with_break_counter(0),
    current_puls_frequence_offset(0),
    puls_swing_amp(1),
    puls_swing_delta(0),
    puls_swing_multi(1),
    puls_swing_switch_counter(0),
    last_puls_was_large(false),
    last_cycle_was_pulse_switch(0),

    octave_smoother( &GET_DATA( osc_datas[id_] ).octave ),
    fm_amount_smoother( &GET_DATA( osc_datas[id_] ).fm_amount ),

    data_buffer( GET_DATA_PTR(data_buffer) ),
    synth_data( synth_data_ ),
    osc_data( GET_DATA_PTR( osc_datas[id_] ) ),
    master_osc_data( GET_DATA_PTR( osc_datas[MASTER_OSC] ) )
{
    modulator.setVibratoGain(1);
}
COLD OSC::~OSC() noexcept {}

//==============================================================================
inline void OSC::process(DataBuffer* data_buffer_,
                         const int num_samples_) noexcept
{
    float* const output_buffer( data_buffer->osc_samples.getWritePointer(id) );
    float* const switch_buffer( data_buffer->osc_switchs.getWritePointer(id) );
    float* const osc_sync_switch_buffer( data_buffer->osc_sync_switchs.getWritePointer(MASTER_OSC) );
    float* const osc_modulator_buffer( data_buffer->modulator_samples.getWritePointer(MASTER_OSC) );
    const float* const lfo_amps( ( data_buffer->lfo_amplitudes.getReadPointer(id) ) );

    // FM SWING
    const float master_fm_swing = master_osc_data->fm_swing;
    const bool master_osc_modulation_is_off = id == MASTER_OSC ? master_osc_data->mod_off : false;
    if( master_fm_swing != 0 ) {
        const bool was_negative = puls_swing_delta < 0;
        puls_swing_delta = sample_rate * master_fm_swing * 0.00000005;
        if( was_negative )
            puls_swing_delta *= -1;
    }
    else
        puls_swing_amp = 1;

    const int glide_motor_time = synth_data->glide_motor_time;
    ValueSmootherModulatedUpdater u_octave( &octave_smoother, glide_motor_time );
    fm_amount_smoother.update( glide_motor_time );

    const float wave = osc_data->wave;
    const bool is_lfo_modulated = osc_data->is_lfo_modulated;
    lfo2fix_octave_smoother.reset_counter_on_state_switch( is_lfo_modulated );
    const bool sync = osc_data->sync;
    const float master_fm_multi = master_osc_data->fm_multi;
    const int master_pulse_width = master_osc_data->puls_width;
    const bool master_sync = master_osc_data->sync;
    const bool master_fm_wave = master_osc_data->fm_wave;
    const int master_switch = master_osc_data->osc_switch;
    for( int sid = 0 ; sid < num_samples_ ; ++sid )
    {
        // UPDATE FREQUENCY - TODO after ticks
        {
            const float octave_mod = octave_smoother.tick( lfo2fix_octave_smoother.tick_to( is_lfo_modulated ? lfo_amps[sid] : 0 ) );
            bool change_modulator_frequency = false;

            // GLIDE
            bool is_glide_rest = false;
            if( glide_time_in_samples > 0 ) {
                --glide_time_in_samples;
                is_glide_rest = true;
            }
            if
            (
                _last_root_note != _root_note
                || octave_smoother.is_changed_since_last_tick()
                || is_glide_rest
                || last_cycle_was_pulse_switch
            )
            {
                float new_frequence;
                {
                    float floated_note = ( float(_root_note)
                                           + octave_mod
                                           + glide_note_delta*glide_time_in_samples );
                    const int note_low = mono_floor( floated_note );
                    new_frequence = MidiMessage::getMidiNoteInHertz(note_low);
                    if( note_low < floated_note )
                    {
                        const float frequence_range = MidiMessage::getMidiNoteInHertz(note_low+1) - new_frequence;
                        const float frequence_rest_percent = floated_note-note_low;
                        new_frequence += frequence_range*frequence_rest_percent;
                    }
                    // PULS
                    if( current_puls_frequence_offset != 0 ) {
                        new_frequence*=current_puls_frequence_offset;
                    }

                    if( new_frequence < 1 )
                        new_frequence = 1;
                }
                if( last_frequency != new_frequence ) {
                    saw_generator.setFrequency(new_frequence);
                    square_generator.setFrequency(new_frequence);
                    sine_generator.setFrequency(new_frequence);

                    change_modulator_frequency = true;

                    last_frequency = new_frequence;
                }

                _last_root_note = _root_note;
            }
            else if( master_fm_multi != last_modulator_multi ) {
                change_modulator_frequency = true;
            }

            // MODULATOR FREQUENCY TODO -> only osc 1
            if( change_modulator_frequency ) {
                last_modulator_multi = master_fm_multi;
                const float modulator_frequency = last_frequency* (1.1f + 14.9f*last_modulator_multi);
                modulator.setVibratoRate( modulator_frequency );

                modulator_sync_cycles = mono_floor(modulator_frequency / last_frequency);
            }
        }

        // PROCESS
        float sample = 0;
        if( ! waiting_for_sync )
        {
            saw_generator.tick();
            square_generator.tick();
            sine_generator.tick();

            /* MIX

               0-1 SINE - SQUARE
               1-2 SQARE - SAW
               2-3 SAW - RAND

            */
            // SINE - SQUARE
            if( wave <= 1 )
            {
                const float multi = wave;
                const float sine_wave_powerd = sine_generator.lastOut() * (1.0f-multi);
                const float square_wave_powerd = square_generator.lastOut() * multi;
                sample = sine_wave_powerd+square_wave_powerd;
            }
            // SQUARE - SAW
            else if( wave <= 2 )
            {
                const float multi = wave - 1;
                const float square_wave_powerd = square_generator.lastOut() * (1.0f-multi);
                const float saw_wave_powerd = saw_generator.lastOut() * multi;
                sample = square_wave_powerd+saw_wave_powerd;
            }
            // SAW - RAND
            else /*if( wave <= 3 )*/
            {
                const float multi = wave - 2;
                const float saw_wave_powerd = saw_generator.lastOut() * (1.0f-multi);
                const float noice_powerd = noise.tick()* multi; // noice_generator.lastOut() * multi;
                sample = saw_wave_powerd+noice_powerd;
            }
        }

        // SWITCHS
        bool is_clean_switch;
        {
            is_clean_switch = sine_generator.isNewCylce();
            switch_buffer[sid] = is_clean_switch;
        }

        // SYNC / PULS / SWING MULTI
        {
            // ENTER SYNC
            last_cycle_was_pulse_switch = false;
            bool is_wave_switch;
            if( wave <= 1 )
            {
                is_wave_switch = sine_generator.isNewCylce();
            }
            else if( wave <= 2 )
            {
                is_wave_switch = square_generator.isNewCylce();
            }
            else /*if( wave <= 3 )*/
            {
                is_wave_switch = saw_generator.isNewCylce();
            }
            saw_generator.clearNewCycleState();
            square_generator.clearNewCycleState();
            sine_generator.clearNewCycleState();

            if( is_wave_switch )
            {
                // TODO saw and square offset BLIT - HOW TO SOLVE???
                // PULS |¯|_|¯¯¯|___|¯|_|¯¯¯|_
                if( master_pulse_width < 0 )
                {
                    current_puls_frequence_offset = (1.0f/12.0f*master_pulse_width*-1.0f);
                    if( last_puls_was_large )
                        current_puls_frequence_offset = 1.0f + 1.0f*current_puls_frequence_offset;
                    else
                        current_puls_frequence_offset = 1.0f - 0.5f*current_puls_frequence_offset;

                    last_cycle_was_pulse_switch = true;
                    last_puls_was_large ^= true;
                }
                // PULS WITH _|¯|_  break  _|¯|_
                else if( master_pulse_width > 0 and puls_with_break_counter < 1 )
                {
                    puls_with_break_counter = master_pulse_width;
                    current_puls_frequence_offset = 0;
                }
                else
                {
                    --puls_with_break_counter;
                    current_puls_frequence_offset = 0;
                }

                // SWING
                if( master_switch > 0 )
                {
                    if( puls_swing_switch_counter <= 0 ) {
                        puls_swing_multi *= -1;
                        puls_swing_switch_counter = master_switch;
                    }
                    --puls_swing_switch_counter;
                }

                if( id == MASTER_OSC )
                {
                    osc_sync_switch_buffer[sid] = true;
                }
                else if( sync )
                    waiting_for_sync = true;
            }
            else if( id == MASTER_OSC ) {
                osc_sync_switch_buffer[sid] = false;
            }

            // EXIT SYNC
            if( sync and id != MASTER_OSC )
            {
                if( osc_sync_switch_buffer[sid] )
                {
                    waiting_for_sync = false;
                }
            }
            else
                waiting_for_sync = false;
        }

        // FMlfo_amplitudes
        const float fm_amount = fm_amount_smoother.tick();
        float modulator_sample = 0;
        {
            if( id == MASTER_OSC )
            {
                int used_sync_cycles = 1;
                if( ! master_fm_wave )
                    used_sync_cycles = modulator_sync_cycles;

                // PROCESS MODULATOR
                if( ! waiting_for_modulator_sync )
                    modulator_sample = modulator.tick();

                // FM SYNC
                if( master_sync )
                {
                    if( modulator.isNewCylce() ) {
                        ++current_modulator_sync_cycle;
                    }
                    if( !waiting_for_modulator_sync and current_modulator_sync_cycle >= used_sync_cycles )
                    {
                        waiting_for_modulator_sync = true;
                        modulator_phase_switch *= -1;
                    }

                    if( is_clean_switch ) {
                        waiting_for_modulator_sync = false;
                        current_modulator_sync_cycle = 0;
                    }
                }
                else
                {
                    waiting_for_modulator_sync = false;
                }
                modulator.clearNewCycleState();

                osc_modulator_buffer[sid] = modulator_sample*modulator_phase_switch;
            }
            else
                modulator_sample = osc_modulator_buffer[sid];
        }

        // OUTPUT // PULS FALL DOWN
        {
            // OVERWRITE WITH ZERO PULS?
            if( puls_with_break_counter > 0 and !master_osc_modulation_is_off ) {
                sample = 0;
            }

            // AMP / FM SWING
            {
                puls_swing_amp += puls_swing_delta;
                if( puls_swing_amp < -1 ) {
                    puls_swing_amp = -1;
                    puls_swing_delta *= -1;
                }
                else if( puls_swing_amp > 1 ) {
                    puls_swing_amp = 1;
                    puls_swing_delta *= -1;
                }
                {
                    if( !master_osc_modulation_is_off )
                    {
                        if( master_fm_swing > 0 )
                            modulator_sample *= puls_swing_amp;
                        if( master_switch > 0 )
                            sample *= puls_swing_multi;
                    }
                }
            }

            // OUT
            if( fm_amount )
                sample = sample*(1.0f-fm_amount) + ( (modulator_sample + sample)/2 )*fm_amount;

            output_buffer[sid] = sample;
        }
    }

    _last_root_note = _root_note;
}

inline void OSC::reset( int root_note_ ) noexcept
{
    root_note_ += synth_data->octave_offset *12;
    const float glide = synth_data->glide;
    if( glide != 0 and (_root_note != root_note_ || glide_time_in_samples > 0 ) )
    {
        const float rest_glide = glide_time_in_samples*glide_note_delta;
        const float glide_notes_diverence = _root_note - root_note_;
        glide_time_in_samples = (sample_rate/2) * glide;
        glide_note_delta = (glide_notes_diverence+rest_glide) / glide_time_in_samples;
    }
    else
    {
        glide_note_delta = 0;
        glide_time_in_samples = 0;
    }

    _root_note = root_note_;
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
#define FORCE_ZERO_SAMPLES 50
enum STAGES
{
    END_ENV = false,
    ATTACK = true,
    DECAY,
    SUSTAIN,
    RELEASE,
    TRIGGER_START
};
enum OPTIONS
{
    WORK_FROM_CURRENT_VALUE = -1
};
class ValueEnvelope : public RuntimeListener
{
    int samples_to_target_left;
    float current_value;
    float last_value;
    float end_value;
    int force_zero_target;
    int current_force_zero_counter;

public:
    inline float tick( float shape_, float shape_factor_ ) noexcept;
    inline void update( float end_value_,
                        int time_in_samples,
                        float start_value_ = WORK_FROM_CURRENT_VALUE ) noexcept;
    inline bool end_reached() const noexcept;
    inline void replace_current_value( float value_ ) noexcept;
    inline void force_zero_glide( float set_to_zero_amount_ ) noexcept;

    inline void reset() noexcept;

    //==============================================================================
    // FOR UI PURPOSES
    float get_current_amp() const noexcept;

    //==============================================================================
    COLD ValueEnvelope() noexcept;
    COLD ~ValueEnvelope() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueEnvelope)
};

COLD ValueEnvelope::ValueEnvelope() noexcept
:
samples_to_target_left(0),
                       current_value(0),
                       last_value(0),
                       end_value(0),
                       force_zero_target(FORCE_ZERO_SAMPLES),
                       current_force_zero_counter(FORCE_ZERO_SAMPLES)
{

}
COLD ValueEnvelope::~ValueEnvelope() noexcept {}

//==============================================================================
#define EXP_MULTI 1
#define EXP_DIV 1.71828f
//6.38906f;
// TODO if sustain only call if sustain is endless!
inline float ValueEnvelope::tick( float shape_, float shape_factor_ ) noexcept
{
    //aufräumen und auch wichtig, die lfos müssenm laufen!!!!
    //was wenn wir nur einen kurzes releasse haben, dann stopt der LFO immer
    ++current_force_zero_counter;
    --samples_to_target_left;
    if( current_force_zero_counter < force_zero_target and samples_to_target_left > 50 )
    {
        float delta = float_Pi*( 0.5f + 1.0f*(1.0f/force_zero_target*current_force_zero_counter));
        current_value = last_value * (1.0f + std::sin( delta ) )*0.5f;
    }
    else
    {
        current_force_zero_counter = force_zero_target;
        if( samples_to_target_left > 0 )
        {
            if( samples_to_target_left == 0 )
                current_value = end_value;
            else
            {
                if( shape_ < 0.25f )
                {
                    float delta_ = (end_value-current_value)/samples_to_target_left;
                    if( delta_ >= 0 )
                        current_value += (((log(delta_*5.0f + 1))/1.79176f)*(1.0f-shape_factor_) + delta_*shape_factor_);
                    else
                    {
                        delta_ *= -1;
                        float shape_factor_release = shape_*4;

                        current_value -= (((log(delta_*5.0f + 1))/1.79176f)*(1.0f-shape_factor_release) + delta_*shape_factor_release);
                    }
                }
                else if( shape_ < 0.5f )
                {
                    float delta_ = (end_value-current_value)/samples_to_target_left;
                    if( delta_ >= 0 )
                        current_value += (((log(delta_*5.0f + 1))/1.79176f)*(1.0f-shape_factor_) + delta_*shape_factor_);
                    else
                    {
                        delta_ *= -1;
                        float shape_factor_release = (shape_-0.25f)*8;
                        if( shape_factor_release >= 1.0f )
                            shape_factor_release = 1.0f - (shape_factor_release - 1);

                        current_value -= (((exp(delta_*EXP_MULTI)-1.0f)/EXP_DIV)*shape_factor_release + delta_*(1.0f-shape_factor_release));
                    }
                }
                else if( shape_ > 0.75f )
                {
                    float delta_ = (end_value-current_value)/samples_to_target_left;
                    if( delta_ >= 0 )
                    {
                        current_value += (((exp(delta_*EXP_MULTI)-1.0f)/EXP_DIV)*shape_factor_ + delta_*(1.0f-shape_factor_));
                    }
                    else
                    {
                        delta_ *= -1;
                        float shape_factor_release = (shape_-0.75f)*4;
                        current_value -= (((exp(delta_*EXP_MULTI)-1.0f)/EXP_DIV)*shape_factor_release + delta_*(1.0f-shape_factor_release));
                    }
                }
                else if( shape_ > 0.5f )
                {
                    float delta_ = (end_value-current_value)/samples_to_target_left;
                    if( delta_ >= 0 )
                        current_value += (((exp(delta_*EXP_MULTI)-1.0f)/EXP_DIV)*shape_factor_ + delta_*(1.0f-shape_factor_));
                    else
                    {
                        delta_ *= -1;
                        float shape_factor_release = (shape_-0.5f)*8;
                        if( shape_factor_release >= 1.0f )
                            shape_factor_release = 1.0f - (shape_factor_release - 1);

                        current_value -= (((log(delta_*5.0f + 1))/1.79176f)*shape_factor_release + delta_*(1.0f-shape_factor_release));
                    }
                }
                else
                {
                    float delta_ = (end_value-current_value)/samples_to_target_left;
                    current_value+=delta_;
                }
            }

            if( current_value > 1 )
                current_value = 1;
            else if( current_value < 0 )
                current_value = 0;
        }
    }

    return current_value;
}
inline void ValueEnvelope::update( float end_value_,
                                   int time_in_samples,
                                   float start_value_
                                 ) noexcept
{
    // UPDATE INTERNALS
    if( start_value_ != WORK_FROM_CURRENT_VALUE )
        current_value = start_value_;

    end_value = end_value_;

    // CALC
    samples_to_target_left = msToSamplesFast( time_in_samples + MIN_ENV_TIMES, sample_rate );
    if( samples_to_target_left <= 0 )
        current_value = end_value;
}

//==============================================================================
inline bool ValueEnvelope::end_reached() const noexcept
{
    return samples_to_target_left <= 0;
}
inline void ValueEnvelope::replace_current_value( float value_ ) noexcept
{
    current_value = value_;
}
inline void ValueEnvelope::force_zero_glide( float set_to_zero_amount_ ) noexcept
{
    if( set_to_zero_amount_ > 0 )
    {
        last_value = current_value;
        current_force_zero_counter = 0;
        force_zero_target = msToSamplesFast( 2+40*set_to_zero_amount_, sample_rate );
        if( force_zero_target > samples_to_target_left * 0.8f )
            force_zero_target = samples_to_target_left * 0.8f;
    }
    else
    {
        current_force_zero_counter = 0;
        force_zero_target = 0;
    }
}
inline void ValueEnvelope::reset() noexcept
{
    samples_to_target_left = 0;
    current_value = 0;
    end_value = 0;
    current_force_zero_counter = force_zero_target;
}

//==============================================================================
float ValueEnvelope::get_current_amp() const noexcept
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
class ENV
{
    ValueEnvelope envelop;
    ValueSmoother sustain_smoother;

    STAGES current_stage;

    const MoniqueSynthData*const synth_data;
    const ENVData*const env_data;

public:
    //==============================================================================
    inline void process( float* dest_, const int num_samples_ ) noexcept;
private:
    inline void update_stage() noexcept;

public:
    //==============================================================================
    inline void start_attack( bool force_to_zero = true ) noexcept;
    inline void set_to_release() noexcept;
    inline void reset() noexcept;

public:
    inline STAGES get_current_stage() const noexcept;
    inline void set_current_stage( STAGES current_stage_ ) noexcept;
    inline void overwrite_current_value( float amp_ ) noexcept;
    inline float get_amp() const noexcept;

public:
    //==============================================================================
    COLD ENV( const MoniqueSynthData* synth_data_, ENVData* env_data_ );
    COLD ~ENV();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENV)
};

//==============================================================================
COLD ENV::ENV( const MoniqueSynthData* synth_data_, ENVData* env_data_ )
    :
    envelop(),
    sustain_smoother( &env_data_->sustain ),

    current_stage(END_ENV),

    env_data( env_data_ ),
    synth_data( synth_data_ )
{
}
COLD ENV::~ENV() {}

//==============================================================================
inline void ENV::process( float* dest_, const int num_samples_ ) noexcept
{
    sustain_smoother.update( synth_data->glide_motor_time );
    const float shape = synth_data->curve_shape;
    float shape_factor = 1;
    if( shape < 0.5f )
    {
        shape_factor = shape * 2;
    }
    else if( shape > 0.5f )
    {
        shape_factor = (shape-0.5f) * 2;
    }

    for( int sid = 0 ; sid < num_samples_ ; ++sid )
    {
        if( current_stage == SUSTAIN )
            envelop.replace_current_value( sustain_smoother.tick() );
        else
            sustain_smoother.tick();

        dest_[sid] = envelop.tick( shape, shape_factor );

        if( envelop.end_reached() )
        {
            update_stage();
        }
    }
}

inline void ENV::update_stage() noexcept
{
    switch( current_stage )
    {
    case TRIGGER_START :
    {
        if( env_data->decay > 0 )
            envelop.update( 1, env_data->attack*env_data->max_attack_time );
        else
        {
            envelop.update( sustain_smoother.tick(), env_data->attack*env_data->max_attack_time );
        }

        current_stage = ATTACK;
        break;
    }
    case ATTACK :
    {
        if( env_data->decay > 0 )
        {
            envelop.update( sustain_smoother.tick(), env_data->decay*env_data->max_decay_time );
            current_stage = DECAY;
            break;
        }
    }
    case DECAY :
    {
        float sustain_time = env_data->sustain_time;
        if( sustain_time == 1.0f )
            // TODO unlimited
            sustain_time = sustain_time*100000;
        else
            sustain_time = sustain_time*10000;

        envelop.update( sustain_smoother.tick(), sustain_time );
        current_stage = SUSTAIN;
    }
    break;
    case SUSTAIN :
    {
        envelop.update( 0, env_data->release*env_data->max_release_time );
        current_stage = RELEASE;
    }
    break;
    case RELEASE :
    {
        current_stage = END_ENV;
    }
    break;
    default:
        ;
    }
}

inline void ENV::start_attack( bool force_to_zero ) noexcept
{
    current_stage = TRIGGER_START;
    if( force_to_zero > 0 )
        envelop.force_zero_glide( synth_data->force_envs_to_zero );
    update_stage();
}
inline void ENV::set_to_release() noexcept
{
    current_stage = SUSTAIN;
    update_stage();
}

inline void ENV::reset() noexcept
{
    current_stage = END_ENV;
    envelop.reset();
}

//==============================================================================
inline STAGES ENV::get_current_stage() const noexcept
{
    return current_stage;
}
inline void ENV::set_current_stage( STAGES current_stage_ ) noexcept
{
    current_stage = current_stage_;
}
inline void ENV::overwrite_current_value( float amp_ ) noexcept
{
    envelop.replace_current_value( amp_ );
}

//==============================================================================
inline float ENV::get_amp() const noexcept
{
    return envelop.get_current_amp();
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

    float cutoff, res, res4;

public:
    //==============================================================================
    // RETURNS TRUE ON COFF CHANGED
    inline bool update(float resonance_, float cutoff_, float gain_ ) noexcept;
    inline void update_with_resoance(float resonance_, float cutoff_, float gain_ ) noexcept;
    inline void copy_coefficient_from( const AnalogFilter& other_ ) noexcept;
    inline void copy_state_from( const AnalogFilter& other_ ) noexcept;

    inline float processLow(float input_and_worker_) noexcept;
    inline float processLowResonance(float input_and_worker_) noexcept;
    inline float processHighResonance(float input_and_worker_) noexcept;

    inline void reset() noexcept;

private:
    inline void calc_coefficients( bool with_resonance_ ) noexcept;

    COLD void sample_rate_changed( double ) noexcept override;

public:
    //==============================================================================
    COLD AnalogFilter() noexcept;
    COLD ~AnalogFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFilter)
};

//==============================================================================
COLD AnalogFilter::AnalogFilter() noexcept
:
p(1),k(1),r(1),gain(1),
  y1(1),y2(1),y3(1),y4(1),
  oldx(1),oldy1(1),oldy2(1),oldy3(1),

  cutoff(1000), res(1), res4(1)
{
    sample_rate_changed(0);
}
COLD AnalogFilter::~AnalogFilter() noexcept {}

//==============================================================================
inline bool AnalogFilter::update(float resonance_, float cutoff_, float gain_) noexcept
{
    bool success = false;
    if( cutoff != cutoff_ || gain != gain_ || res != resonance_ )
    {
        gain = gain_;
        cutoff = cutoff_;
        res = resonance_;
        res4 = resonance_*4;
        success = true;
    }
    return success;
}
inline void AnalogFilter::update_with_resoance(float resonance_, float cutoff_, float gain_) noexcept
{
    if( cutoff != cutoff_ || gain != gain_ || res != resonance_ )
    {
        gain = gain_;
        cutoff = cutoff_;
        res = resonance_;
        res4 = resonance_*4;
        calc_coefficients( true );
    }
}

//==============================================================================
inline void AnalogFilter::copy_coefficient_from( const AnalogFilter& other_ ) noexcept
{
    cutoff = other_.cutoff;
    res = other_.res;
    res4 = other_.res4;

    p = other_.p;
    k = other_.k;
    r = other_.r;
    gain = other_.gain;
}
inline void AnalogFilter::copy_state_from( const AnalogFilter& other_ ) noexcept
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

//==============================================================================
inline float AnalogFilter::processLow(float input_and_worker_) noexcept
{
    input_and_worker_ = filter_hard_clipper( input_and_worker_ );

    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    //hard_clipper_1_5( y4 );
    //y4 -= (y4*y4*y4) * (1.0f/6);
    y4 = std::atan(y4);

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    return filter_hard_clipper((y4 + distortion(y3*res+y4*(1.0f-res),gain)*gain)/(1.0f+gain));
}
inline float AnalogFilter::processLowResonance(float input_and_worker_) noexcept
{
    input_and_worker_ = filter_hard_clipper( input_and_worker_ );

    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 = std::atan(y4);
    //y4 -= (y4*y4*y4) * (1.0f/6);
    //hard_clipper_1_5( y4 );

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // Add resonance
    return filter_hard_clipper(y4 + std::atan( y3 * res4 ));
}
inline float AnalogFilter::processHighResonance(float input_and_worker_) noexcept
{
    input_and_worker_ = filter_hard_clipper( input_and_worker_ );

    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    //y4 -= (y4*y4*y4) * (1.0f/6);
    y4 = std::atan(y4);
    //hard_clipper_1_5( y4 );

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // RESONANCE
    return filter_hard_clipper((input_and_worker_-y4) + (std::atan( y2 * res )));
}

//==============================================================================
inline void AnalogFilter::reset() noexcept
{
    cutoff=res=res4=p=k=r=gain=y1=y2=y3=y4=oldx=oldy1=oldy2=oldy3=1;
}

//==============================================================================
inline void AnalogFilter::calc_coefficients( bool with_resonance_ ) noexcept
{
    {
        float f = (cutoff+cutoff) * sample_rate_1ths;
        float agressive = 0.48f*gain;
        p=f*((1.5f+agressive)-((0.5f+agressive)*f));
        k=p*2-1;
    }
    if( with_resonance_ )
    {
        const float t=(1.0f-p)*1.386249f;
        const float t2=12.0f+t*t;
        r = res*(t2+6.0f*t)/(t2-6.0f*t);
    }
    else
    {
        r = 1;
    }
}

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
    // ----------------------------------------------------------
    // LP
    inline void updateLow2Pass(float resonance_,
                               float cutoff_,
                               float gain_) noexcept;
    inline float processLow2Pass(float in_) noexcept;

    // ----------------------------------------------------------
    // HP
    inline void updateHigh2Pass(float resonance_,
                                float cutoff_,
                                float gain_) noexcept;
    inline float processHigh2Pass(float in_) noexcept;

    // ----------------------------------------------------------
    // BAND
    inline void updateBand(float resonance_,
                           float cutoff_,
                           float gain_) noexcept;
    inline float processBand(float in_) noexcept;

    // ----------------------------------------------------------
    // SIMPLE PASS
    inline float processPass(float in_) noexcept;

    // ----------------------------------------------------------
    // FILTER CHANGE
    inline void update_filter_to( FILTER_TYPS ) noexcept;
    inline float process_filter_change( float original_in_, float result_in_ ) noexcept;
    inline float processByType( float in_, FILTER_TYPS ) noexcept;

    //
    inline void reset() noexcept;

    //==============================================================================
    COLD DoubleAnalogFilter(bool create_smooth_filter = true) noexcept;
    COLD ~DoubleAnalogFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleAnalogFilter)
};

//==============================================================================
COLD DoubleAnalogFilter::DoubleAnalogFilter(bool create_smooth_filter) noexcept
:
flt_1(),
      flt_2(),

      smooth_filter( nullptr ),

      last_filter_type(UNKNOWN),
      smooth_filter_type(UNKNOWN),

      glide_time_4_filters(0)
{
    if( create_smooth_filter )
        smooth_filter = new DoubleAnalogFilter(false);
}
COLD DoubleAnalogFilter::~DoubleAnalogFilter() noexcept
{
    if( smooth_filter )
        delete smooth_filter;
}

//==============================================================================
inline void DoubleAnalogFilter::reset() noexcept
{
    last_filter_type = UNKNOWN;
    smooth_filter_type = UNKNOWN;
    glide_time_4_filters = 0;
    if( smooth_filter )
        smooth_filter->reset();
    flt_1.reset();
    flt_2.reset();
}

// -----------------------------------------------------------------
static inline float resonance_clipping( float sample_ ) noexcept
{
    return (std::atan(sample_) * (1.0f/float_Pi))*2;
}

// -----------------------------------------------------------------
COLD void AnalogFilter::sample_rate_changed(double) noexcept
{
    reset();
}

// LP
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateLow2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    if( flt_2.update( resonance_, cutoff_, gain_ ) )
    {
        flt_2.calc_coefficients( false );
        flt_1.copy_coefficient_from( flt_2 );
    }
}
inline float DoubleAnalogFilter::processLow2Pass(float in_) noexcept
{
    const float out = flt_2.processLowResonance( in_ );
    const float gain = flt_1.gain;
    const float low = flt_1.processLowResonance( out*gain );

    return process_filter_change
    (
        in_,
        (out+low)*(1.0f-gain) + resonance_clipping(out+low)*gain
    );
}

// 2 PASS HP
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateHigh2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    if( flt_2.update( resonance_, cutoff_, gain_ ) )
    {
        flt_2.calc_coefficients( false );
        flt_1.copy_coefficient_from( flt_2 );
    }
}
inline float DoubleAnalogFilter::processHigh2Pass(float in_) noexcept
{
    const float out = flt_2.processHighResonance( in_ );
    const float gain = flt_1.gain;
    const float low = flt_1.processHighResonance( out*gain );
    return process_filter_change
    (
        in_,
        (out+low)*(1.0f-gain) + resonance_clipping(out+low)*gain
    );
}

// BAND
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateBand(float resonance_, float cutoff_, float gain_ ) noexcept
{
    if( flt_1.update( resonance_, cutoff_+10, gain_ ) )
    {
        flt_1.calc_coefficients( false );
        flt_2.update( resonance_, cutoff_, gain_ );
        flt_2.calc_coefficients( false );
    }
}
inline float DoubleAnalogFilter::processBand(float in_) noexcept
{
    return process_filter_change
    (
        in_,
        flt_1.processLowResonance(  flt_2.processHighResonance( in_ ) )*0.5f
    );
}

// PASS
// -----------------------------------------------------------------
inline float DoubleAnalogFilter::processPass(float in_) noexcept
{
    return process_filter_change
    (
        in_,
        in_
    );
}

// BY TYPE
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::update_filter_to( FILTER_TYPS type_ ) noexcept {
    if( last_filter_type != type_ )
    {
        if( smooth_filter )
        {
            // SET THE SECOND FILTER TO THE OLD COMPLETE STATE
            smooth_filter->flt_1.copy_coefficient_from(flt_1);
            smooth_filter->flt_1.copy_state_from(flt_1);
            smooth_filter->flt_2.copy_coefficient_from(flt_2);
            smooth_filter->flt_2.copy_state_from(flt_2);

            switch( last_filter_type )
            {
            case LPF :
            case HIGH_2_PASS :
            case LPF_2_PASS :
            case PASS :
            case HPF :
                flt_2.copy_state_from(flt_1);
                //flt_1.reset();
                //flt_2.reset();
                break;
            case BPF :
                break;
            default /*UNKNOWN*/ :
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
inline float DoubleAnalogFilter::process_filter_change( float original_in_, float result_in_ ) noexcept
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
inline float DoubleAnalogFilter::processByType(float io_, FILTER_TYPS type_ ) noexcept {

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
        break;
    }

    return io_;
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
class EnvelopeFollower
{
    float envelope;
    float attack;
    float release;

public:
    inline void processEnvelope (const float* input_buffer_, float* output_buffer_, int num_samples_) noexcept;
    inline void setCoefficients (float attack_, float release_) noexcept;
    inline void reset() noexcept;

public:
    //==============================================================================
    COLD EnvelopeFollower() noexcept;
    COLD ~EnvelopeFollower() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeFollower)
};

//==============================================================================
COLD EnvelopeFollower::EnvelopeFollower() noexcept
:
envelope (0),
         attack (1),
         release (1)
{}
COLD EnvelopeFollower::~EnvelopeFollower() noexcept {}

//==============================================================================
inline void EnvelopeFollower::processEnvelope ( const float* input_buffer_, float* output_buffer_, int num_samples_ ) noexcept
{
    for (int i = 0; i != num_samples_; ++i)
    {
        using namespace std;
        float envIn = fabsf (input_buffer_[i]);

        if (envelope < envIn)
            envelope += attack * (envIn - envelope);
        else if (envelope > envIn)
            envelope -= release * (envelope - envIn);

        output_buffer_[i] = envelope;
    }
}
inline void EnvelopeFollower::setCoefficients (float attack_, float release_) noexcept
{
    attack = attack_;
    release = release_;
}
inline void EnvelopeFollower::reset() noexcept { envelope = 0; }

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
    enum
    {
        DIMENSION_INPUT = 0,
        DIMENSION_INPUT_1 = 0,
        DIMENSION_INPUT_2 = 1,
        DIMENSION_INPUT_3 = 2,
        DIMENSION_INPUT_AMP = 3,
        DIMENSION_INPUT_AMP_1 = 3,
        DIMENSION_INPUT_AMP_2 = 4,
        DIMENSION_INPUT_AMP_3 = 5,
        DIMENSION_RESONANCE,
        DIMENSION_CUTOFF,
        DIMENSION_GAIN,
        DIMENSION_DISTORTION,
        DIMENSION_TMP
    };

    DoubleAnalogFilter double_filter[SUM_INPUTS_PER_FILTER];
    friend class mono_ParameterOwnerStore;

public:
    ScopedPointer< ENV > env;
    OwnedArray< ENV > input_envs;

private:
    EnvelopeFollower env_follower;

    SwitchAndValueSmootherModulated cutoff_smoother;
    SwitchAndValueSmootherModulated resonance_smoother;
    SwitchAndValueSmootherModulated gain_smoother;
    SwitchAndValueSmootherModulated distortion_smoother;
    SwitchAndValueSmootherModulated output_smoother;
    ValueSmoother mix_smoother;
    ValueSmoother clipping_smoother;
    ValueSmoother boost_smoother;
    ValueSmoother input_sustains[SUM_INPUTS_PER_FILTER];
    SwitchSmoother amp2sustain_smoother[SUM_INPUTS_PER_FILTER];

    const int id;

    const MoniqueSynthData*const synth_data;
    const FilterData*const filter_data;
    DataBuffer*const data_buffer;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void reset() noexcept;
    inline void process( const int num_samples ) noexcept;

private:
    inline void pre_process( const int input_id, const int num_samples ) noexcept;

    inline void process_amp_mix( const int num_samples ) noexcept;

private:
    inline void compress ( float* io_buffer_, float* tmp_buffer_, const float* compressor_signal,
                           int glide_motor_time,
                           int num_samples ) noexcept;

public:
    //==============================================================================
    COLD FilterProcessor( const MoniqueSynthData* synth_data_, int id_ ) noexcept;
    COLD ~FilterProcessor() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor)
};

//==============================================================================
COLD FilterProcessor::FilterProcessor( const MoniqueSynthData* synth_data_, int id_ ) noexcept
:
env( new ENV( synth_data_, GET_DATA( filter_datas[id_] ).env_data ) ),
     input_envs(),
     env_follower(),

     cutoff_smoother( &GET_DATA( filter_datas[id_] ).cutoff ),
     resonance_smoother( &GET_DATA( filter_datas[id_] ).resonance ),
     gain_smoother( &GET_DATA( filter_datas[id_] ).gain ),
     distortion_smoother( &GET_DATA( filter_datas[id_] ).distortion ),
     output_smoother( &GET_DATA( filter_datas[id_] ).output ),
     mix_smoother( &GET_DATA( filter_datas[id_] ).adsr_lfo_mix ),
     clipping_smoother( &GET_DATA( filter_datas[id_] ).output_clipping ),
     boost_smoother( &GET_DATA( filter_datas[id_] ).compressor ),
     input_sustains { &GET_DATA( filter_datas[id_] ).input_sustains[0], &GET_DATA( filter_datas[id_] ).input_sustains[1], &GET_DATA( filter_datas[id_] ).input_sustains[2] },
     amp2sustain_smoother(),

     id(id_),

     synth_data( synth_data_ ),
     filter_data( GET_DATA_PTR( filter_datas[id_] ) ),
     data_buffer( GET_DATA_PTR( data_buffer ) )
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
    {
        input_envs.add( new ENV( synth_data_, GET_DATA( filter_datas[id_] ).input_env_datas[i] ) );
    }
}
COLD FilterProcessor::~FilterProcessor() noexcept {}

//==============================================================================
inline void FilterProcessor::start_attack() noexcept
{
    env->start_attack();

    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
    {
        input_envs.getUnchecked(input_id)->start_attack();
    }
}
inline void FilterProcessor::start_release() noexcept
{
    env->set_to_release();

    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
    {
        input_envs.getUnchecked(input_id)->set_to_release();
    }
}
inline void FilterProcessor::reset() noexcept
{
    env_follower.reset();
    env->reset();
    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
    {
        input_envs.getUnchecked(input_id)->reset();
    }

    cutoff_smoother.reset();
    resonance_smoother.reset();
    gain_smoother.reset();
    distortion_smoother.reset();
    output_smoother.reset();
    mix_smoother.reset();
    clipping_smoother.reset();

    input_sustains[0].reset();
    input_sustains[1].reset();
    input_sustains[2].reset();

    double_filter[0].reset();
    double_filter[1].reset();
    double_filter[2].reset();
}

// -----------------------------------------------------------------
inline void FilterProcessor::pre_process( const int input_id, const int num_samples ) noexcept
{
    float* tmp_input_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer( DIMENSION_INPUT + input_id );
    float* tmp_input_ar_amp = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer( DIMENSION_INPUT_AMP + input_id );

    const int glide_motor_time = synth_data->glide_motor_time;

    // CALCULATE INPUTS AND ENVELOPS
    {
        ValueSmoother& input_sustain_smoother = input_sustains[input_id];
        input_sustain_smoother.update( glide_motor_time );
        const float* const osc_input_buffer = data_buffer->osc_samples.getReadPointer(input_id);

        // SWITCH FROM AMP TO INPUT CRACKL REMOVER
        const bool input_hold( filter_data->input_holds[input_id] );
        SwitchSmoother*const amp2s_smoother( &amp2sustain_smoother[input_id] );
        ENV*const input_env( input_envs.getUnchecked(input_id) );
        if( amp2s_smoother->reset_counter_on_state_switch( input_hold ) )
        {
            if( not input_hold )
            {
                input_env->overwrite_current_value( amp2s_smoother->get_last_tick_value() );
                input_env->set_current_stage( ATTACK );
            }
        }

        if( input_hold )
        {
            if( id == FILTER_1 )
            {
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    tmp_input_ar_amp[sid] = amp2s_smoother->tick_to( input_sustain_smoother.tick() );
                    tmp_input_buffer[sid] = osc_input_buffer[sid];
                }
            }
            else
            {
                const float* filter_before_buffer = data_buffer->filter_output_samples.getReadPointer(input_id + SUM_INPUTS_PER_FILTER*(id-1) );
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float sustain = input_sustain_smoother.tick();
                    if( sustain < 0 )
                    {
                        tmp_input_ar_amp[sid] = amp2s_smoother->tick_to( sustain*-1 );
                        tmp_input_buffer[sid] = osc_input_buffer[sid];
                    }
                    else
                    {
                        tmp_input_ar_amp[sid] = amp2s_smoother->tick_to( sustain );
                        tmp_input_buffer[sid] = filter_before_buffer[sid];
                    }
                }

            }
        }
        else // USING ADR
        {
            input_env->process( tmp_input_ar_amp, num_samples );

            if( id == FILTER_1 )
            {
                input_sustain_smoother.reset();
                FloatVectorOperations::copy( tmp_input_buffer, osc_input_buffer, num_samples );

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    tmp_input_ar_amp[sid] = amp2s_smoother->tick_to( tmp_input_ar_amp[sid] );
                }
            }
            else
            {
                const float* filter_before_buffer = data_buffer->filter_output_samples.getReadPointer(input_id + SUM_INPUTS_PER_FILTER*(id-1) );
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float sustain = input_sustain_smoother.tick();
                    if( sustain < 0 )
                        tmp_input_buffer[sid] = osc_input_buffer[sid];
                    else
                        tmp_input_buffer[sid] = filter_before_buffer[sid];

                    tmp_input_ar_amp[sid] = amp2s_smoother->tick_to( tmp_input_ar_amp[sid] );
                }
            }
        }
    }
}
inline void FilterProcessor::process_amp_mix( const int num_samples ) noexcept
{
// ADSTR - LFO MIX
    const int glide_motor_time = synth_data->glide_motor_time;
    float* amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
    mix_smoother.update(glide_motor_time);
    {
        const float* env_amps = data_buffer->filter_env_amps.getReadPointer(id);
        const float* lfo_amplitudes = data_buffer->lfo_amplitudes.getReadPointer(id);
        for( int sid = 0 ; sid != num_samples ; ++sid )
        {
            // LFO ADSR MIX - HERE TO SAVE ONE LOOP
            const float mix = (1.0f+mix_smoother.tick()) * 0.5f;
            amp_mix[sid] = env_amps[sid]*(1.0f-mix) + lfo_amplitudes[sid]*mix;
        }
    }
}
inline void FilterProcessor::process( const int num_samples ) noexcept
{
    const int glide_motor_time = synth_data->glide_motor_time;

    float* amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
    // PROCESS FILTER
    {
#define DISTORTION_IN(x) distortion(x,filter_distortion)*(1.0f/SUM_INPUTS_PER_FILTER)
#define DISTORTION_OUT(x) distortion(x,filter_distortion)

#define MAX_CUTOFF 8000.0f
#define MIN_CUTOFF 40.0f

        ValueSmootherModulatedUpdater u_cutoof( &cutoff_smoother, glide_motor_time, filter_data->modulate_cutoff );
        ValueSmootherModulatedUpdater u_resonance( &resonance_smoother, glide_motor_time, filter_data->modulate_resonance );
        ValueSmootherModulatedUpdater u_gain( &gain_smoother, glide_motor_time, filter_data->modulate_gain );
        ValueSmootherModulatedUpdater u_distortion( &distortion_smoother, glide_motor_time, filter_data->modulate_distortion );
        switch( filter_data->filter_type )
        {
        case LPF :
        case LPF_2_PASS :
        case MOOG_AND_LPF:
        {
            // PREPARE
            {
                process_amp_mix(num_samples);

                float* const tmp_resonance_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float* const tmp_cuttof_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float* const tmp_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float* const tmp_distortion_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF * cutoff_smoother.tick( amp ) + MIN_CUTOFF) * (1.0f/8);
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( amp );
                }
            }

            // PROCESSOR
            struct LP2PassExecuter : public mono_Thread {
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

                    DoubleAnalogFilter& filter(processor->double_filter[input_id]);
                    filter.update_filter_to(LPF_2_PASS);

                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateLow2Pass( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION_OUT( filter.processLow2Pass( DISTORTION_IN( input_buffer[sid] ) ) );
                    }
                }
                LP2PassExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                processor( processor_ ),
                           filter( processor_->double_filter[input_id_] ),
                           input_id( input_id_ ),
                           num_samples_( num_samples__ ),

                           tmp_resonance_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_RESONANCE)),
                           tmp_cuttof_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_CUTOFF)),
                           tmp_gain_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_GAIN)),
                           tmp_distortion_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_DISTORTION)),

                           input_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT + input_id_ )),
                           out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                {}
            };

            // RUN
            {
                // 1
                LP2PassExecuter executer_1( this, num_samples, 0 );
                executer_1.try_run_paralel();
                // 2
                {
                    LP2PassExecuter executer_2( this, num_samples, 1 );
                    executer_2.exec();
                }
                // 3
                {
                    LP2PassExecuter executer_3( this, num_samples, 2 );
                    executer_3.exec();
                }

                while( executer_1.isWorking() ) {}
            }
        }
        break;
        case HPF :
        case HIGH_2_PASS :
        {
            // PREPARE
            {
                process_amp_mix(num_samples);

                float* const tmp_resonance_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float* const tmp_cuttof_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float* const tmp_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float* const tmp_distortion_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF*2.0f) * cutoff_smoother.tick( amp ) + MIN_CUTOFF;
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( amp );
                }
            }

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
                        filter.updateHigh2Pass( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION_OUT( filter.processHigh2Pass( DISTORTION_IN( input_buffer[sid] ) ) );
                    }
                }
                HP2PassExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                processor( processor_ ),
                           filter( processor_->double_filter[input_id_] ),
                           input_id(input_id_),
                           num_samples_(num_samples__),

                           tmp_resonance_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_RESONANCE)),
                           tmp_cuttof_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_CUTOFF)),
                           tmp_gain_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_GAIN)),
                           tmp_distortion_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_DISTORTION)),

                           input_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT + input_id_ )),
                           out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                {}
            };

            // RUN
            {
                // 1
                HP2PassExecuter executer_1( this, num_samples, 0 );
                executer_1.try_run_paralel();

                // 2
                {
                    HP2PassExecuter executer_2( this, num_samples, 1 );
                    executer_2.exec();
                }
                // 3
                {
                    HP2PassExecuter executer_3( this, num_samples, 2 );
                    executer_3.exec();
                }

                while( executer_1.isWorking() ) {}
            }
        }
        break;
        case BPF:
        {
            // PREPARE
            {
                process_amp_mix(num_samples);

                float* const tmp_resonance_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float* const tmp_cuttof_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float* const tmp_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float* const tmp_distortion_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp );
                    tmp_cuttof_buffer[sid] = MAX_CUTOFF * cutoff_smoother.tick( amp ) + MIN_CUTOFF;
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( amp );
                }
            }

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
                        filter.updateBand( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION_OUT( filter.processBand( DISTORTION_IN( input_buffer[sid] ) ) );
                    }
                }
                BandExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                processor( processor_ ),
                           filter( processor_->double_filter[input_id_] ),
                           input_id(input_id_),
                           num_samples_(num_samples__),

                           tmp_resonance_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_RESONANCE)),
                           tmp_cuttof_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_CUTOFF)),
                           tmp_gain_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_GAIN)),
                           tmp_distortion_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_DISTORTION)),

                           input_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT + input_id_ )),
                           out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                {}
            };

            // RUN
            {
                // 1
                BandExecuter executer_1( this, num_samples, 0 );
                executer_1.try_run_paralel();

                // 2
                {
                    BandExecuter executer_2( this, num_samples, 1 );
                    executer_2.exec();
                }
                // 3
                {
                    BandExecuter executer_3( this, num_samples, 2 );
                    executer_3.exec();
                }

                while( executer_1.isWorking() ) {}
            }
        }
        break;
        default : //  PASS
        {
            // PREPARE
            {
                process_amp_mix(num_samples);

                float* const tmp_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float* const tmp_distortion_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float amp = amp_mix[sid];
                    resonance_smoother.tick( amp );
                    cutoff_smoother.tick( amp );
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( amp );
                }
            }

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
                        out_buffer[sid] = filter.processPass( DISTORTION_OUT( DISTORTION_IN( input_buffer[sid] ) ) );
                    }
                }
                PassExecuter(FilterProcessor*const processor_, int num_samples__, int input_id_) noexcept
:
                processor( processor_ ),
                           filter( processor_->double_filter[input_id_] ),
                           input_id(input_id_),
                           num_samples_(num_samples__),

                           tmp_gain_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_GAIN)),
                           tmp_distortion_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_DISTORTION)),

                           input_buffer(processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT + input_id_ )),
                           out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer( input_id_ + SUM_INPUTS_PER_FILTER * processor_->id ))
                {}
            };

            // RUN
            {
                // 1
                PassExecuter executer_1( this, num_samples, 0 );
                executer_1.try_run_paralel();

                // 2
                {
                    PassExecuter executer_2( this, num_samples, 1 );
                    executer_2.exec();
                }
                // 3
                {
                    PassExecuter executer_3( this, num_samples, 2 );
                    executer_3.exec();
                }

                while( executer_1.isWorking() ) {}
            }
        }
        break;
        }
    }

    // COLLECT RESULTS
    float* const this_filter_output_buffer = data_buffer->direct_filter_output_samples.getWritePointer(id);
    {
        float* out_buffer_1( data_buffer->filter_output_samples.getWritePointer( 0 + SUM_INPUTS_PER_FILTER * id ) );
        float* out_buffer_2( data_buffer->filter_output_samples.getWritePointer( 1 + SUM_INPUTS_PER_FILTER * id ) );
        float* out_buffer_3( data_buffer->filter_output_samples.getWritePointer( 2 + SUM_INPUTS_PER_FILTER * id ) );
        const float* input_ar_amp_1( data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT_AMP_1 ) );
        const float* input_ar_amp_2( data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT_AMP_2 ) );
        const float* input_ar_amp_3( data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer( DIMENSION_INPUT_AMP_3 ) );
        clipping_smoother.update( glide_motor_time );
        {
            ValueSmootherModulatedUpdater u_output( &output_smoother, glide_motor_time, filter_data->modulate_output );

            for( int sid = 0 ; sid != num_samples ; ++sid )
            {
                // OUTPUT MIX AND DISTORTION
                {
                    const float amp = output_smoother.tick( amp_mix[sid] );
                    const float clipping = clipping_smoother.tick();
                    const float result_sample = amp * ( out_buffer_1[sid] * input_ar_amp_1[sid] + out_buffer_2[sid] * input_ar_amp_2[sid] + out_buffer_3[sid] * input_ar_amp_3[sid]);
                    this_filter_output_buffer[sid] = ( result_sample*(1.0f-clipping) + soft_clipping(result_sample)*clipping );

                    if( id != FILTER_3 ) // NO NEED TO STORE THE THIRD BUFFER
                    {
                        float sample_1 = out_buffer_1[sid] * SUM_INPUTS_PER_FILTER;
                        float sample_2 = out_buffer_2[sid] * SUM_INPUTS_PER_FILTER;
                        float sample_3 = out_buffer_3[sid] * SUM_INPUTS_PER_FILTER;
                        out_buffer_1[sid] = (sample_1)*(1.0f-clipping) + sample_1*clipping;
                        out_buffer_2[sid] = (sample_2)*(1.0f-clipping) + sample_2*clipping;
                        out_buffer_3[sid] = (sample_3)*(1.0f-clipping) + sample_3*clipping;
                    }
                }
            }
        }
    }

    // COMPRESS
    {
        if( id == FILTER_1 )
        {
            compress( this_filter_output_buffer, data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP), this_filter_output_buffer,
            glide_motor_time,
            num_samples );
        }
        else
        {
            compress( this_filter_output_buffer, data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP), data_buffer->direct_filter_output_samples.getReadPointer(id-1),
            glide_motor_time,
            num_samples );
        }
    }

    // VISUALIZE
    if( Monique_Ui_AmpPainter*const amp_painter = AppInstanceStore::getInstanceWithoutCreating()->get_amp_painter_unsave() )
    {
        amp_painter->add_filter_env( id, amp_mix, num_samples );
        amp_painter->add_filter( id, this_filter_output_buffer, num_samples );
    }

    // COLLECT THE FINAL OUTPUT
    if( id == FILTER_3 )
    {
        float* const final_filters_output_buffer = data_buffer->direct_filter_output_samples.getWritePointer();
        const float* direct_output_buffer_flt_2 = data_buffer->direct_filter_output_samples.getReadPointer(1);
        for( int sid = 0 ; sid != num_samples ; ++sid )
        {
            final_filters_output_buffer[sid] += direct_output_buffer_flt_2[sid] + this_filter_output_buffer[sid];
        }
    }
}

// -----------------------------------------------------------------
inline void FilterProcessor::compress( float* io_buffer_, float* tmp_buffer_, const float* compressor_signal_,
                                       int glide_motor_time_,
                                       int num_samples ) noexcept
{
    boost_smoother.update( glide_motor_time_ );
    float last_value = boost_smoother.get_last_tick_value();
    env_follower.setCoefficients( 0.008f * last_value + 0.0001f, 0.005f * last_value + 0.0001f );
    env_follower.processEnvelope( compressor_signal_, tmp_buffer_, num_samples );

    for( int sid = 0 ; sid != num_samples ; ++sid )
    {
        float use_power = boost_smoother.tick();
        bool is_negative = false;
        if( use_power < 0 )
        {
            use_power*=-1;
            is_negative = true;
        }

        float compression = exp(tmp_buffer_[sid])-1;
        if( is_negative )
            compression = 1.0f-compression;

        if( compression < 0 )
            compression = 0;
        else if( compression > 1 )
            compression = 1;
// io_buffer[sid] = input*(1.0f-use_power) + ( (1.0f+FIXED_K)*input/(1.0f+FIXED_K*std::abs(input)) * (1.f - 0.3f*use_power))*use_power;
        if( is_negative )
            io_buffer_[sid] = (io_buffer_[sid] * (1.0f-use_power)) + ((io_buffer_[sid] * compression)*use_power);
        else
            io_buffer_[sid] = io_buffer_[sid] + ((io_buffer_[sid] * compression)*use_power);
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
class EQProcessor : public RuntimeListener
{
    float frequency_low_pass[SUM_EQ_BANDS];
    float frequency_high_pass[SUM_EQ_BANDS];

    AnalogFilter filters[SUM_EQ_BANDS];
    IIRFilter low_pass_filters[SUM_EQ_BANDS];
    IIRFilter high_pass_filters[SUM_EQ_BANDS];

    ValueSmoother velocity_smoother[SUM_EQ_BANDS];
    ValueSmoother shape_smoother[SUM_EQ_BANDS];
    SwitchSmoother amp2velocity_smoother[SUM_EQ_BANDS];

    friend class mono_ParameterOwnerStore;

    const MoniqueSynthData*const synth_data;
    const EQData*const eq_data;
    DataBuffer*const data_buffer;

public:
    OwnedArray< ENV > envs;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void reset() noexcept;
    inline void process( int num_samples_ ) noexcept;

    //==============================================================================
    COLD EQProcessor( MoniqueSynthData* synth_data_ ) noexcept;
    COLD ~EQProcessor() noexcept;

    COLD void sample_rate_changed( double old_sr_ ) noexcept override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessor)
};

//==============================================================================
COLD EQProcessor::EQProcessor( MoniqueSynthData* synth_data_ ) noexcept
:
velocity_smoother
{
    GET_DATA_PTR( eq_data )->velocity[0].ptr(),
    GET_DATA_PTR( eq_data )->velocity[1].ptr(),
    GET_DATA_PTR( eq_data )->velocity[2].ptr(),
    GET_DATA_PTR( eq_data )->velocity[3].ptr(),
    GET_DATA_PTR( eq_data )->velocity[4].ptr(),
    GET_DATA_PTR( eq_data )->velocity[5].ptr(),
    GET_DATA_PTR( eq_data )->velocity[6].ptr(),
    GET_DATA_PTR( eq_data )->velocity[7].ptr(),
    GET_DATA_PTR( eq_data )->velocity[8].ptr()
},
// TODO only one is needed
shape_smoother
{
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr(),
    synth_data_->resonance.ptr()
},
amp2velocity_smoother(),

                      synth_data( synth_data_ ),
                      eq_data( GET_DATA_PTR( eq_data ) ),
                      data_buffer( GET_DATA_PTR( data_buffer ) )
{
    std::cout << "MONIQUE: init EQ" << std::endl;
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        envs.add( new ENV( synth_data_, eq_data->env_datas.getUnchecked( band_id ) ) );

        const float frequency_low_pass_tmp = (62.5f/2) * pow(2,band_id+1);

        frequency_low_pass[band_id] = frequency_low_pass_tmp;
        frequency_high_pass[band_id] = frequency_low_pass_tmp / 2;
    }

    sample_rate_changed(0);
}
COLD EQProcessor::~EQProcessor() noexcept {}

//==============================================================================
inline void EQProcessor::start_attack() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        envs.getUnchecked(band_id)->start_attack();
    }
}
inline void EQProcessor::start_release() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        envs.getUnchecked(band_id)->set_to_release();
    }
}
inline void EQProcessor::reset() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        filters[band_id].reset();
        low_pass_filters[band_id].reset();
        high_pass_filters[band_id].reset();
        envs.getUnchecked(band_id)->reset();
    }
}

void EQProcessor::sample_rate_changed(double) noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        low_pass_filters[band_id].setCoefficients( IIRCoefficients::makeLowPass( sample_rate, frequency_low_pass[band_id] ) );
        high_pass_filters[band_id].setCoefficients( IIRCoefficients::makeHighPass( sample_rate, frequency_high_pass[band_id] ) );
    }
}

// -----------------------------------------------------------------
inline void EQProcessor::process( int num_samples_ ) noexcept
{
    // MULTITHREADED PER BAND
    {
#define IN_DIMENSION 4*band_id_ +0
#define ENV_DIMENSION 4*band_id_ +1
#define OUT_DIMENSION 4*band_id_ +2
#define GAIN_DIMENSION 4*band_id_ +3

#define GAIN_MULTI 5

        struct BandExecuter : public mono_Thread
        {
            const int num_samples_;
            const int band_id;

            const bool hold_sustain;
            const int glide_motor_time;
            ValueSmoother* const velocity_smoother;
            ValueSmoother* const shape_smoother;
            SwitchSmoother* const amp2velocity_smoother;

            const float filter_frequency;
            IIRFilter& low_pass_filter;
            IIRFilter& high_pass_filter;
            AnalogFilter& filter;
            ENV& env;

            const float* const direct_filter_output_samples;
            float* const tmp_band_in_buffer;
            float* const tmp_band_out_buffer;
            float* const tmp_env_buffer;
            float* const tmp_sum_gain_buffer;

            inline void exec() noexcept override
            {
                // PROCESS 100% BAND
                {
                    low_pass_filter.processSamples( tmp_band_in_buffer, direct_filter_output_samples, num_samples_ );
                    high_pass_filter.processSamples( tmp_band_in_buffer, num_samples_ );
                }

                // PROCESS
                {
                    // CRACKL BLOCKER
                    if( amp2velocity_smoother->reset_counter_on_state_switch( hold_sustain ) )
                    {
                        if( not hold_sustain )
                        {
                            env.overwrite_current_value( amp2velocity_smoother->get_last_tick_value() );
                            env.set_current_stage( ATTACK );
                        }
                    }

                    // PROCESS ENVELOPE
                    if( not hold_sustain )
                        env.process( tmp_env_buffer, num_samples_ );

                    velocity_smoother->update( 250 );
                    shape_smoother->update( 250 );

                    // PROCESS
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float shape = shape_smoother->tick();

                        const float raw_sustain = velocity_smoother->tick(); // from -1 to 1
                        const float normalized_sustain = (1.0f+raw_sustain)*0.5f;
                        const float amp = amp2velocity_smoother->tick_to( hold_sustain ? normalized_sustain : tmp_env_buffer[sid] );

                        tmp_env_buffer[sid ] = amp;

                        // UPDATE FILTER
                        filter.update_with_resoance( 0.2f*shape, filter_frequency, normalized_sustain );

                        // PROCESS
                        {
                            //const float gain = sustain + amp * 4;
                            const float gain = normalized_sustain * amp*GAIN_MULTI;
                            const float input = tmp_band_in_buffer[sid];
                            const float unfiltered_amount = raw_sustain >= 0 ? 1.0f-raw_sustain : 1.0f+raw_sustain;
                            const float fildered_amount = raw_sustain >= 0 ? raw_sustain : 0;
                            float output = filter.processLow( input );
                            output = output*fildered_amount + input*unfiltered_amount;

                            // SHAPER
#define FIXED_K 2.0f*0.7f/(1.0f-0.7f)
                            tmp_band_out_buffer[sid] = ( output*(1.0f-shape) + ( (1.0f+FIXED_K)*output/(1.0f+FIXED_K*std::abs(output)) * (0.5f - 0.1f*shape))*shape ) *gain;
                            tmp_sum_gain_buffer[sid] = 1+fildered_amount;
                        }
                    }
                }
            }
            BandExecuter(EQProcessor*const processor_, int num_samples__, int band_id_) noexcept
:
            num_samples_(num_samples__),
                         band_id(band_id_),

                         hold_sustain( processor_->eq_data->hold[band_id_]),
                         glide_motor_time( processor_->synth_data->glide_motor_time ),
                         shape_smoother( &processor_->shape_smoother[band_id_] ),
                         velocity_smoother( &processor_->velocity_smoother[band_id_] ),
                         amp2velocity_smoother( &processor_->amp2velocity_smoother[band_id_] ),

                         filter_frequency( processor_->frequency_low_pass[band_id_] ),
                         low_pass_filter(processor_->low_pass_filters[band_id_]),
                         high_pass_filter(processor_->high_pass_filters[band_id_]),
                         filter(processor_->filters[band_id_]),

                         env( *processor_->envs[band_id_] ),

                         direct_filter_output_samples( processor_->data_buffer->direct_filter_output_samples.getReadPointer() ),
                         tmp_band_in_buffer( processor_->data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(IN_DIMENSION) ),
                         tmp_band_out_buffer( processor_->data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(OUT_DIMENSION) ),
                         tmp_env_buffer( processor_->data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(ENV_DIMENSION) ),
                         tmp_sum_gain_buffer( processor_->data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(GAIN_DIMENSION) )
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

            BandExecuter* executer = new BandExecuter(this, num_samples_, band_id);
            executer->try_run_paralel();
            if( executer->isWorking() )
            {
                running_threads.add( executer );
            }
            else
                delete executer;
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
#define DIMENSION_TMP 0
#define DIMENSION_TMP_2 1

        float* const out_buffer = data_buffer->direct_filter_output_samples.getWritePointer();
        float* const tmp_all_band_out_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP);
        float* const tmp_all_band_sum_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP_2);
        FloatVectorOperations::clear(tmp_all_band_out_buffer,num_samples_);
        FloatVectorOperations::fill(tmp_all_band_sum_gain_buffer,1,num_samples_);
        for( int band_id_ = 0 ; band_id_ != SUM_EQ_BANDS ; ++band_id_ )
        {
            const float* const tmp_band_out_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(OUT_DIMENSION);
            const float* const tmp_band_gain_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(GAIN_DIMENSION);

            FloatVectorOperations::add( tmp_all_band_out_buffer, tmp_band_out_buffer, num_samples_ );
            FloatVectorOperations::add( tmp_all_band_sum_gain_buffer, tmp_band_gain_buffer, num_samples_ );
        }
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            out_buffer[sid] = (tmp_all_band_out_buffer[sid] / (tmp_all_band_sum_gain_buffer[sid]*0.2));
        }

        if( Monique_Ui_AmpPainter*const amp_painter = AppInstanceStore::getInstanceWithoutCreating()->get_amp_painter_unsave() )
        {
            amp_painter->add_eq( out_buffer, num_samples_ );
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
class Chorus : public RuntimeListener
{
    int buffer_size;
    int index;
    float last_delay;
    float* buffer;

public:
    inline void fill(float sample_) noexcept;
    inline float tick(float delay_) noexcept;
    inline void reset() noexcept;

private:
    COLD void sample_rate_changed( double ) noexcept override;

public:
    //==============================================================================
    COLD Chorus() noexcept;
    COLD ~Chorus() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chorus)
};

//==============================================================================
COLD Chorus::Chorus() noexcept :
buffer_size(0)
{
    index = 0;
    last_delay = 210;

    buffer = nullptr;

    sample_rate_changed(0);
}
COLD Chorus::~Chorus() noexcept
{
    delete [] buffer;
}

//==============================================================================
inline void Chorus::fill(float sample_) noexcept
{
    index = index % buffer_size;
    buffer[index] = sample_;
    index++;
}
#define DELAY_GLIDE 0.01f
inline float Chorus::tick(float delay_ ) noexcept
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
inline void Chorus::reset() noexcept
{
    index = 0;
    last_delay = 210;
    sample_rate_changed(0);
}
//==============================================================================
COLD void Chorus::sample_rate_changed( double ) noexcept {
    buffer_size = sample_rate/10;
    if( buffer )
        delete[] buffer;
    buffer = new float[buffer_size];
    for( int i = 0 ; i != buffer_size ; ++i )
    {
        buffer[i] = 0;
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
class CombFilter
{
    HeapBlock<float> buffer;
    float last;
    int bufferSize, bufferIndex;

public:
    inline float process (const float input, const float feedbackLevel) noexcept;

    COLD void setSize (const int size);
    COLD void clear() noexcept;

    //==============================================================================
    COLD CombFilter() noexcept;
    COLD ~CombFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombFilter)
};

//==============================================================================
COLD CombFilter::CombFilter() noexcept
:
last(0),
     bufferSize (0),
     bufferIndex(0)
{}
COLD CombFilter::~CombFilter() noexcept {}

//==============================================================================
inline float CombFilter::process (const float input, const float feedbackLevel) noexcept
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
COLD void CombFilter::setSize (const int size)
{
    if (size != bufferSize)
    {
        bufferIndex = 0;
        buffer.malloc ((size_t) size);
        bufferSize = size;
    }

    clear();
}
COLD void CombFilter::clear() noexcept
{
    last = 0;
    buffer.clear ((size_t) bufferSize);
}

//==============================================================================
//==============================================================================
//==============================================================================
class AllPassFilter
{
    HeapBlock<float> buffer;
    int bufferSize, bufferIndex;
public:
    inline float process (const float input) noexcept;

    COLD void setSize (const int size);
    COLD void clear() noexcept;

    //==============================================================================
    COLD AllPassFilter() noexcept;
    COLD ~AllPassFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllPassFilter)
};

//==============================================================================
COLD AllPassFilter::AllPassFilter() noexcept
:
bufferSize(0),
           bufferIndex(0)
{}
COLD AllPassFilter::~AllPassFilter() noexcept {}

//==============================================================================
inline float AllPassFilter::process (const float input) noexcept
{
    const float bufferedValue = buffer [bufferIndex];
    float temp = input + (bufferedValue * 0.5f);
    JUCE_UNDENORMALISE (temp);
    buffer[bufferIndex] = temp;
    bufferIndex = (bufferIndex + 1) % bufferSize;

    return bufferedValue - input;
}

//==============================================================================
COLD void AllPassFilter::setSize (const int size)
{
    if (size != bufferSize)
    {
        bufferIndex = 0;
        buffer.malloc ((size_t) size);
        bufferSize = size;
    }

    clear();
}
COLD void AllPassFilter::clear() noexcept
{
    buffer.clear ((size_t) bufferSize);
}

//==============================================================================
//==============================================================================
//==============================================================================
class LinearSmoothedValue
{
    float currentValue, target, step, lastValue;
    int countdown, stepsToTarget;

public:
    inline float getNextValue() noexcept;
    inline float getLastValue() noexcept;
    inline void setValue (float newValue) noexcept;

    COLD void reset (float sampleRate, float fadeLengthSeconds) noexcept;

    //==============================================================================
    COLD LinearSmoothedValue() noexcept;
    COLD ~LinearSmoothedValue() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearSmoothedValue)
};

//==============================================================================
COLD LinearSmoothedValue::LinearSmoothedValue() noexcept
:
currentValue(0),
             target(0),
             step(0),
             countdown(0),
             stepsToTarget(0)
{}

COLD LinearSmoothedValue::~LinearSmoothedValue() noexcept {}

//==============================================================================
inline float LinearSmoothedValue::getNextValue() noexcept
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
//==============================================================================
inline float LinearSmoothedValue::getLastValue() noexcept
{
    return lastValue;
}

inline void LinearSmoothedValue::setValue (float newValue) noexcept
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
COLD void LinearSmoothedValue::reset (float sampleRate, float fadeLengthSeconds) noexcept
{
    stepsToTarget = mono_floor (fadeLengthSeconds * sampleRate);
    currentValue = target;
    countdown = 0;
}

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
    COLD ReverbParameters() noexcept;
    COLD ~ReverbParameters() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbParameters)
};

//==============================================================================
COLD ReverbParameters::ReverbParameters() noexcept
:
roomSize   (0.5f),
           wetLevel   (0.33f),
           dryLevel   (0.4f),
           width      (1.0f)
{}
COLD ReverbParameters::~ReverbParameters() noexcept {}

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
    inline float processSingleSampleRaw ( float in_ ) noexcept;

    inline ReverbParameters& get_parameters() noexcept;
    inline void update_parameters() noexcept;

    COLD void sample_rate_changed( double /* old_sr_ */ ) noexcept override;
    COLD void reset() noexcept;

    //==============================================================================
    COLD mono_Reverb() noexcept;
    COLD ~mono_Reverb() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Reverb)
};

//==============================================================================
COLD mono_Reverb::mono_Reverb() noexcept
{
    update_parameters();
    sample_rate_changed(0);
}
COLD mono_Reverb::~mono_Reverb() noexcept {}

//==============================================================================
inline float mono_Reverb::processSingleSampleRaw ( float in ) noexcept
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

//==============================================================================
inline ReverbParameters& mono_Reverb::get_parameters() noexcept {
    return parameters;
}
inline void mono_Reverb::update_parameters() noexcept
{
#define WET_SCALE_FACTOR 3.0f
#define DRY_SCALE_FACTOR 2.0f
#define ROOM_SCALE_FACTOR 0.28f
#define ROOM_OFFSET 0.7f
    const float wet = parameters.wetLevel * WET_SCALE_FACTOR;
    dryGain.setValue (parameters.dryLevel * DRY_SCALE_FACTOR);
    wetGain1.setValue (0.5f * wet * (1.0f + parameters.width));
    wetGain2.setValue (0.5f * wet * (1.0f - parameters.width));
    feedback.setValue (parameters.roomSize * ROOM_SCALE_FACTOR + ROOM_OFFSET);
}

//==============================================================================
COLD void mono_Reverb::sample_rate_changed (double) noexcept
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
COLD void mono_Reverb::reset() noexcept
{
    for (int i = 0; i < numCombs; ++i)
        comb[i].clear();

    for (int i = 0; i < numAllPasses; ++i)
        allPass[i].clear();
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
class FXProcessor
{
    enum {
        DIMENSION_TMP_L = 0,
        DIMENSION_TMP_R,
        DIMENSION_ENV,
        DIMENSION_CHORUS_MOD_AMP,
        DIMENSION_DELAY,
        DIMENSION_BYPASS,
        DIMENSION_CLIPPING,
        DIMENSION_TMP_CHORUS
    };

    // REVERB
    mono_Reverb reverb_l;
    mono_Reverb reverb_r;

    // CHORUS
#define DELAY_BUFFER_SIZE 12000
    Chorus chorus_l;
    Chorus chorus_r;
    ValueSmoother chorus_mod_smoother;
    SwitchSmoother amp2chorus_smoother;
    friend class mono_ParameterOwnerStore;
    ScopedPointer< ENV > chorus_modulation_env;

    // DELAY
    int delayPosition;
    mono_AudioSampleBuffer<2> delayBuffer;
    ValueSmoother delay_smoother;

    // FINAL ENV
    friend class MoniqueSynthesiserVoice;
    ScopedPointer<ENV> final_env;
    // TODO change to a real glide value
public:
    ValueSmoother velocity_glide;
    AmpSmoothBuffer last_output_smoother;

private:
    ValueSmoother bypass_smoother;
    ValueSmoother volume_smoother;
    ValueSmoother clipping_smoother;

    const MoniqueSynthData*const synth_data;
    DataBuffer*const data_buffer;
    const ReverbData*const reverb_data;
    const ChorusData*const chorus_data;

public:
    inline void process( AudioSampleBuffer& output_buffer_, const int start_sample_final_out_, const int num_samples_ ) noexcept;

    void start_attack() noexcept
    {
        chorus_modulation_env->start_attack();
        final_env->start_attack();
    }
    void start_release() noexcept
    {
        chorus_modulation_env->set_to_release();
        final_env->set_to_release();
    }
    void reset() noexcept
    {
        reverb_l.reset();
        reverb_r.reset();
        chorus_l.reset();
        chorus_r.reset();
        chorus_mod_smoother.reset();
        chorus_modulation_env->reset();
        delayPosition = 0;
        delay_smoother.reset();
        final_env->reset();
        velocity_glide.reset();
        bypass_smoother.reset();
        volume_smoother.reset();
        clipping_smoother.reset();

        last_output_smoother.reset();
    }

public:
    //==============================================================================
    COLD FXProcessor( MoniqueSynthData* synth_data_, Parameter* sequencer_velocity_ ) noexcept;
    COLD ~FXProcessor() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXProcessor)
};

// -----------------------------------------------------------------
COLD FXProcessor::FXProcessor( MoniqueSynthData* synth_data_, Parameter* sequencer_velocity_ ) noexcept
:
reverb_l(),
         reverb_r(),

         chorus_l(),
         chorus_r(),

         chorus_mod_smoother( &GET_DATA( chorus_data ).modulation ),
         amp2chorus_smoother(),
         chorus_modulation_env( new ENV( synth_data_, GET_DATA( chorus_data ).modulation_env_data ) ),

         delayPosition( 0 ),
         delayBuffer ( DELAY_BUFFER_SIZE ),
         delay_smoother( &synth_data_->delay ),

         final_env( new ENV( synth_data_, synth_data_->env_data ) ),
         velocity_glide( sequencer_velocity_ ),

         bypass_smoother( &synth_data_->effect_bypass ),
         volume_smoother( &synth_data_->volume ),
         clipping_smoother( &synth_data_->final_compression ),

         synth_data( synth_data_ ),
         data_buffer( GET_DATA_PTR( data_buffer ) ),
         reverb_data( GET_DATA_PTR( reverb_data ) ),
         chorus_data( GET_DATA_PTR( chorus_data ) )
{
    //delayBuffer.clear();

    std::cout << "MONIQUE: init FX" << std::endl;
}

COLD FXProcessor::~FXProcessor() noexcept {}

// -----------------------------------------------------------------
inline void FXProcessor::process( AudioSampleBuffer& output_buffer_, const int start_sample_final_out_, const int num_samples_ ) noexcept
{
    //const int glide_motor_time = msToSamplesFast(DATA( synth_data ).glide_motor_time,44100);
    const int glide_motor_time = synth_data->glide_motor_time;

    // COLLECT BUFFERS
    float* input_buffer = data_buffer->direct_filter_output_samples.getWritePointer();

    // PREPARE REVERB
    const float reverb_room = reverb_data->room;
    const float reverb_dry_wet_mix = reverb_data->dry_wet_mix;
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

    // PRPARE CHORUS
    // TODO only process amp if chorus amp is active

    // PREPARE
    {
        float* const tmp_env_amp = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_ENV);
        final_env->process( tmp_env_amp, num_samples_ );

        float* const tmp_chorus_mod_amp = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CHORUS_MOD_AMP);
        float* const tmp_delay = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DELAY);
        float* const tmp_bypass = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_BYPASS);
        float* const tmp_clipping = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CLIPPING);

        float* const tmp_chorus_amp_buffer = data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP_CHORUS);
        chorus_modulation_env->process( tmp_chorus_amp_buffer, num_samples_ );

        bypass_smoother.update( glide_motor_time );
        volume_smoother.update( glide_motor_time );
        clipping_smoother.update( glide_motor_time );
        delay_smoother.update( glide_motor_time );
        chorus_mod_smoother.update( glide_motor_time );
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            {
                input_buffer[sid] *= tmp_env_amp[sid]*volume_smoother.tick()*velocity_glide.tick();
            }

            {
                const float chorus_modulation = chorus_mod_smoother.tick();
                const bool is_chorus_amp_fix = chorus_data->hold_modulation;
                amp2chorus_smoother.reset_counter_on_state_switch(chorus_data->hold_modulation);
                tmp_chorus_mod_amp[sid] = amp2chorus_smoother.tick_to( is_chorus_amp_fix ? chorus_modulation : tmp_chorus_amp_buffer[sid] );
            }

            {
                tmp_delay[sid] = delay_smoother.tick();
            }

            {
                tmp_bypass[sid] = bypass_smoother.tick();
            }

            {
                tmp_clipping[sid] = clipping_smoother.tick();
            }
        }
    }
#define VOLUME_GAIN 1.5f
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

            const float* const tmp_chorus_mod_amp;
            const float* const tmp_delay;
            const float* const tmp_bypass;
            const float* const tmp_clipping;

            float* const tmp_samples;

            // LEFT SIDE
            inline void exec() noexcept override
            {
                // CHORUS
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float in_l = input_buffer[sid];
                    const float modulation_amp = tmp_chorus_mod_amp[sid];
                    const float feedback = modulation_amp*0.85f;

                    float tmp_sample  = processor->chorus_l.tick((modulation_amp * 220) + 1.51f);
                    processor->chorus_l.fill( in_l + tmp_sample * feedback );

                    tmp_samples[sid] = tmp_sample;
                }

                // REVERB
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    tmp_samples[sid] = processor->reverb_l.processSingleSampleRaw( tmp_samples[sid] );
                }

                // DELAY
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float in = tmp_samples[sid];
                    const float delay_data_in = delay_data[delay_pos];

                    tmp_samples[sid] += delay_data_in;

                    delay_data[delay_pos] = (delay_data_in + in) * tmp_delay[sid];

                    if ( ++delay_pos >= DELAY_BUFFER_SIZE )
                        delay_pos = 0;
                }

                // BYPASS -> MIX
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float bypass = tmp_bypass[sid];
                    float tmp = tmp_samples[sid];
                    tmp = tmp*bypass + input_buffer[sid]*(1.0f-bypass);
                    tmp *= VOLUME_GAIN;

                    const float clipping = tmp_clipping[sid];
                    final_output[sid] = ( soft_clipping( tmp )*clipping + tmp*(1.0f-clipping) );
                }
            }

            inline void exec_right() noexcept
            {
                // CHORUS
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float in_r = input_buffer[sid];

                    const float modulation_amp = tmp_chorus_mod_amp[sid];
                    const float feedback = modulation_amp*0.85f;

                    float tmp_sample = processor->chorus_r.tick((modulation_amp * 200) + 1.56f);
                    processor->chorus_r.fill( in_r + tmp_sample * feedback );
                    tmp_samples[sid] = tmp_sample;
                }

                // REVERB
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    tmp_samples[sid] = processor->reverb_r.processSingleSampleRaw( tmp_samples[sid] );
                }

                // DELAY
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    const float in = tmp_samples[sid];
                    const float delay_data_in = delay_data[delay_pos];

                    tmp_samples[sid] += delay_data_in;

                    delay_data[delay_pos] = (delay_data_in + in) * tmp_delay[sid];

                    if (++delay_pos >= DELAY_BUFFER_SIZE)
                        delay_pos = 0;
                }
                processor->delayPosition = delay_pos;

                // BYPASS -> MIX
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    {
                        const float bypass = tmp_bypass[sid];
                        float tmp = tmp_samples[sid];
                        tmp = tmp*bypass + input_buffer[sid]*(1.0f-bypass);
                        tmp *= VOLUME_GAIN;
                        const float clipping = tmp_clipping[sid];
                        final_output[sid] = ( soft_clipping( tmp )*clipping + tmp*(1.0f-clipping) );

                        // FOR END ENV - RELEASE IF INACTIVE
                        processor->last_output_smoother.add( tmp );
                    }
                }
            }

            LeftRightExecuter( FXProcessor*const fx_processor_,

                               const int num_samples_,
                               const int delay_pos_,

                               const float* input_buffer_,
                               float* tmp_samples_,

                               float* delay_data_,
                               float* final_output_
                             ) noexcept
:
            processor( fx_processor_ ),

                       num_samples( num_samples_ ),
                       delay_pos( delay_pos_ ),

                       input_buffer(input_buffer_),
                       delay_data(delay_data_),
                       final_output(final_output_),

                       tmp_chorus_mod_amp( fx_processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_CHORUS_MOD_AMP) ),
                       tmp_delay( fx_processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_DELAY) ),
                       tmp_bypass( fx_processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_BYPASS) ),
                       tmp_clipping( fx_processor_->data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_CLIPPING) ),

                       tmp_samples( tmp_samples_ )
            {}
        };

        const int delay_pos = delayPosition;
        LeftRightExecuter left_executer( this,

                                         num_samples_,
                                         delay_pos,

                                         input_buffer,
                                         data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP_L),

                                         delayBuffer.getWritePointer (LEFT),

                                         &output_buffer_.getWritePointer(LEFT)[start_sample_final_out_]
                                       ) ;
        left_executer.try_run_paralel();

        {
            LeftRightExecuter right_executer(
                this,

                num_samples_,
                delay_pos,

                input_buffer,
                data_buffer->tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_TMP_R),

                delayBuffer.getWritePointer (RIGHT),

                &output_buffer_.getWritePointer(RIGHT)[start_sample_final_out_]
            ) ;
            right_executer.exec_right();
        }

        while( left_executer.isWorking() ) {}

        // FINAL MIX
        {
            float* left = output_buffer_.getWritePointer(LEFT);
            float* right = output_buffer_.getWritePointer(RIGHT);
            for( int sid = start_sample_final_out_ ; sid < start_sample_final_out_+num_samples_ ; ++sid )
            {
                float sum = left[sid] + right[sid];
                left[sid] = right[sid] = sum;
            }
        }

        // VISUALIZE
        if( Monique_Ui_AmpPainter* amp_painter = AppInstanceStore::getInstanceWithoutCreating()->get_amp_painter_unsave() )
        {
            amp_painter->add_out( &output_buffer_.getReadPointer(LEFT)[start_sample_final_out_], num_samples_ );
            amp_painter->add_out_env( data_buffer->tmp_multithread_band_buffer_9_4.getReadPointer(DIMENSION_ENV), num_samples_ );
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
class ArpSequencer : public RuntimeListener
{
    RuntimeInfo*const info;
    const ArpSequencerData*const data;

    friend class MoniqueSynthesiserVoice;
    Parameter current_velocity;

    int current_step;
    int next_step_on_hold;
    int step_at_sample_current_buffer;

    int shuffle_to_back_counter;
    bool found_a_step;

public:
    //==============================================================================
    // RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
    inline int process_samples_to_next_step( int start_sample_, int num_samples_ ) noexcept;

    inline int get_current_step() const noexcept;
    inline int get_step_before() const noexcept;
    inline float get_current_tune() const noexcept;
    inline bool found_last_process_a_step() const noexcept;
    inline bool last_found_step_is_enabled() const noexcept;
    inline void reset() noexcept;

    //==============================================================================
    COLD ArpSequencer( RuntimeInfo* info_, const ArpSequencerData* data_ ) noexcept;
    COLD ~ArpSequencer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpSequencer)
};

//==============================================================================
COLD ArpSequencer::ArpSequencer( RuntimeInfo* info_, const ArpSequencerData* data_ ) noexcept
:
info(info_),
     data(data_),

     current_velocity
     (
         MIN_MAX( 0, 1 ),
         0,
         1000,
         generate_param_name("ARP_RT",0,"vc"),
         generate_short_human_name("ARP_RT","vc")
     ),

     current_step(0),
     next_step_on_hold(0),
     step_at_sample_current_buffer(-1),

     shuffle_to_back_counter(0),
     found_a_step(false)
{
    std::cout << "MONIQUE: init SEQUENCER" << std::endl;
}

COLD ArpSequencer::~ArpSequencer() noexcept {}

//==============================================================================
// RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
// RETURN NUM SAMPLES IF THERE IS NO STEP IN THE BUFFER
inline int ArpSequencer::process_samples_to_next_step( int start_sample_, int num_samples_ ) noexcept
{
    double samples_per_min = sample_rate*60;
    double speed_multi = ArpSequencerData::speed_multi_to_value(data->speed_multi);
    double steps_per_min = info->bpm*4/1.0 * speed_multi;
    double steps_per_sample = steps_per_min/samples_per_min;
    double samples_per_step = samples_per_min/steps_per_min;
    int64 sync_sample_pos = info->samples_since_start+start_sample_;
    int64 step = next_step_on_hold;
    step_at_sample_current_buffer = -1;
    for( int i = 0 ; i < num_samples_; ++i )
    {
#ifdef IS_STANDALONE
        if( info->is_extern_synced )
        {
            if( info->next_step_at_sample.size() )
            {
                if( info->next_step_at_sample.getFirst() == start_sample_+i )
                {
                    step = info->next_step.remove(0);
                    info->next_step_at_sample.remove(0);
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
                shuffle_to_back_counter = mono_floor(samples_per_step*0.8f * data->shuffle);
            else
                shuffle_to_back_counter = 0;

            found_a_step = true;
        }

        if( found_a_step and shuffle_to_back_counter == 0 )
        {
            found_a_step = false;
            step_at_sample_current_buffer = i;
            current_step = next_step_on_hold;
            return i;
        }

        sync_sample_pos++;
    }

    return num_samples_;
}

//==============================================================================
inline int ArpSequencer::get_current_step() const noexcept
{
    return current_step % SUM_ENV_ARP_STEPS;
}
inline int ArpSequencer::get_step_before() const noexcept
{
    if( current_step > 0 )
        return (current_step-1) % SUM_ENV_ARP_STEPS;
    else
        return 0;
}
inline float ArpSequencer::get_current_tune() const noexcept
{
    return data->tune[get_current_step()];
}
inline bool ArpSequencer::found_last_process_a_step() const noexcept
{
    return step_at_sample_current_buffer != -1;
}
inline bool ArpSequencer::last_found_step_is_enabled() const noexcept
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
inline void ArpSequencer::reset() noexcept
{
    current_step = 0;
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
COLD MoniqueSynthesiserVoice::MoniqueSynthesiserVoice( MoniqueAudioProcessor*const audio_processor_, MoniqueSynthData*const synth_data_ ) noexcept
:
audio_processor( audio_processor_ ),
synth_data( synth_data_ ),

info( new RuntimeInfo() ),
data_buffer( new DataBuffer(1024) ),

arp_sequencer( new ArpSequencer( info, synth_data_->arp_sequencer_data ) ),
eq_processor( new EQProcessor( synth_data_ ) ),
fx_processor( new FXProcessor( synth_data_, &arp_sequencer->current_velocity ) ),

current_note(-1),
current_velocity(0),
current_step(0),
an_arp_note_is_already_running(false)
{
    mono_ThreadManager::getInstance();
    mono_ParameterOwnerStore::getInstance()->voice = this;

    std::cout << "MONIQUE: init BUFFERS's" << std::endl;

    std::cout << "MONIQUE: init OSC's" << std::endl;
    oscs = new OSC*[SUM_OSCS];
    for( int i = 0 ; i != SUM_OSCS ; ++i )
    {
        oscs[i] = new OSC( synth_data_, i );
    }

    std::cout << "MONIQUE: init LFO's" << std::endl;
    lfos = new LFO*[SUM_LFOS];
    for( int i = 0 ; i != SUM_LFOS ; ++i )
    {
        lfos[i] = new LFO( i );
    }

    std::cout << "MONIQUE: init FILTERS & ENVELOPES" << std::endl;
    filter_processors = new FilterProcessor*[SUM_FILTERS];
    for( int i = 0 ; i != SUM_FILTERS ; ++i )
    {
        filter_processors[i] = new FilterProcessor( synth_data_, i );
    }
}
COLD MoniqueSynthesiserVoice::~MoniqueSynthesiserVoice() noexcept
{
    mono_ParameterOwnerStore::getInstance()->voice = nullptr;

    for( int i = SUM_FILTERS-1 ; i > -1 ; --i )
    {
        delete filter_processors[i];
    }
    for( int i = SUM_LFOS-1 ; i > -1 ; --i )
    {
        delete lfos[i];
    }
    for( int i = SUM_OSCS-1 ; i > -1 ; --i )
    {
        delete oscs[i];
    }

    delete arp_sequencer;
    delete eq_processor;
    delete fx_processor;
    delete data_buffer;
    delete info;

    mono_ThreadManager::deleteInstance();
}

//==============================================================================
void MoniqueSynthesiserVoice::startNote( int midi_note_number_, float velocity_, SynthesiserSound* /*sound*/, int pitch_ )
{
    start_internal( midi_note_number_, velocity_ );
}
void MoniqueSynthesiserVoice::start_internal( int midi_note_number_, float velocity_ ) noexcept
{
    current_note = midi_note_number_;
    current_velocity = velocity_;

    // OSCS
    bool is_arp_on = synth_data->arp_sequencer_data->is_on;
    float arp_offset = is_arp_on ? arp_sequencer->get_current_tune() : 0;
    for( int i = 0 ; i != SUM_OSCS ; ++i )
    {
        oscs[i]->reset( midi_note_number_+arp_offset );
    }

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
void MoniqueSynthesiserVoice::stopNote ( float, bool allowTailOff )
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
void MoniqueSynthesiserVoice::stop_internal() noexcept
{
    for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id )
    {
        filter_processors[voice_id]->start_release();
    }
    eq_processor->start_release();
    fx_processor->start_release();
}

//==============================================================================
void MoniqueSynthesiserVoice::release_if_inactive() noexcept
{
    if( fx_processor->final_env->get_current_stage() == END_ENV )
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
    info->bpm = synth_data->sync ? audio_processor->current_pos_info.bpm : synth_data->speed;
    info->samples_since_start = audio_processor->current_pos_info.timeInSamples;

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
        if( samples_to_next_arp_step_in_this_buffer > 0 )
        {
            render_block( output_buffer_, is_a_step ? arp_sequencer->get_current_step() : -1, count_start_sample, samples_to_next_arp_step_in_this_buffer );
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

        const bool is_a_new_arp_step_to_start = arp_is_on and is_a_step and is_step_enabled;
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
    bool only_process_lfo = current_note == -1;

    Monique_Ui_AmpPainter* amp_painter = AppInstanceStore::getInstanceWithoutCreating()->get_amp_painter_unsave();

    const int num_samples = num_samples_;
    if( num_samples == 0 )
        return;

    if( step_number_ != -1 )
        current_step = step_number_;

    // LFO WILL ALWAYS PEROCESSED
    if( only_process_lfo )
    {
        lfos[0]->process( step_number_, num_samples_ );
        lfos[1]->process( step_number_, num_samples_ );
        lfos[2]->process( step_number_, num_samples_ );

        return;
    }

    // MULTI THREADED FLT_ENV / LFO / OSC
    {
        // MAIN THREAD // NO DEPENCIES
        lfos[0]->process( step_number_, num_samples );
        filter_processors[0]->env->process( data_buffer->filter_env_amps.getWritePointer(0), num_samples );
        oscs[0]->process( data_buffer, num_samples ); // NEED LFO 0

        // SUB THREAD
        // DEPENCIES OSC 0
        struct Executer : public mono_Thread
        {
            MoniqueSynthesiserVoice*const voice;
            const int num_samples;
            const int step_number;
            void exec() noexcept override
            {
                voice->filter_processors[1]->env->process( voice->data_buffer->filter_env_amps.getWritePointer(1), num_samples );
                voice->lfos[1]->process( step_number, num_samples );
                voice->oscs[1]->process( voice->data_buffer, num_samples );
            }
            Executer(MoniqueSynthesiserVoice*const voice_,
            int num_samples_,
            int step_number_)
                : voice(voice_),
                num_samples(num_samples_),step_number(step_number_) {}
        };
        Executer executer( this, num_samples, step_number_ );
        executer.try_run_paralel();

        lfos[2]->process( step_number_, num_samples );
        filter_processors[2]->env->process( data_buffer->filter_env_amps.getWritePointer(2), num_samples );
        oscs[2]->process( data_buffer, num_samples ); // NEED OSC 0 && LFO 2

        while( executer.isWorking() ) {}
    }

    filter_processors[0]->process( num_samples );
    filter_processors[1]->process( num_samples );
    filter_processors[2]->process( num_samples );

    eq_processor->process( num_samples );

    if( synth_data->arp_sequencer_data->is_on )
    {
        arp_sequencer->current_velocity = current_velocity * synth_data->arp_sequencer_data->velocity[current_step];
    }
    else
    {
        arp_sequencer->current_velocity = current_velocity;
    }
    fx_processor->velocity_glide.update( synth_data->velocity_glide_time );

    fx_processor->process( output_buffer_, start_sample_, num_samples_ );
    // OSCs - THREAD 1 ?

    // VISUALIZE
    if( amp_painter )
    {
        amp_painter->add_osc( 0, data_buffer->osc_samples.getReadPointer(0), data_buffer->osc_switchs.getReadPointer(0), num_samples_ );
        amp_painter->add_osc( 1, data_buffer->osc_samples.getReadPointer(1), data_buffer->osc_switchs.getReadPointer(1), num_samples_ );
        amp_painter->add_osc( 2, data_buffer->osc_samples.getReadPointer(2), data_buffer->osc_switchs.getReadPointer(2), num_samples_ );
    }

    // UI INFORMATIONS
    for( int i = 0 ; i != SUM_OSCS ; ++i )
    {
        synth_data->osc_datas[i]->last_modulation_value = data_buffer->lfo_amplitudes.getReadPointer(i)[num_samples-1];
    }
}

int MoniqueSynthesiserVoice::getCurrentlyPlayingNote() const noexcept
{
    return current_note;
}
void MoniqueSynthesiserVoice::pitchWheelMoved (int) {}
void MoniqueSynthesiserVoice::controllerMoved (int, int ) {}

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
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
COLD mono_ParameterOwnerStore::mono_ParameterOwnerStore() noexcept
:
ui_env(nullptr),
ui_env_preset_data(nullptr)
{}

COLD mono_ParameterOwnerStore::~mono_ParameterOwnerStore() noexcept
{
    clearSingletonInstance();
}
//==============================================================================
void mono_ParameterOwnerStore::get_full_adsr( float state_, Array< float >& curve, int& sustain_start_, int& sustain_end_ )
{
    mono_ParameterOwnerStore* store = mono_ParameterOwnerStore::getInstanceWithoutCreating();
    if( ! store->ui_env )
    {
        store->ui_env_preset_data = new ENVPresetData( 999, store->env_preset_def );
        store->ui_env = new ENV( GET_DATA_PTR( synth_data ), store->ui_env_preset_data );
    }
    ENV* env = store->ui_env;
    ENVPresetData* data = store->ui_env_preset_data;
    data->state = state_;
    data->sustain = 0.5;
    float* one_sample_buffer = new float;
    bool count_sustain = false;
    env->start_attack( false );
    env->overwrite_current_value( 0.5 );
    env->set_current_stage( RELEASE );
    env->start_attack( true );
    const int suatain_samples = RuntimeNotifyer::getInstanceWithoutCreating()->get_sample_rate() / 10;
    sustain_end_ = -1;
    bool is_first_attack = true;
    while( true )
    {
        if( ! is_first_attack and ( env->get_current_stage() == SUSTAIN or env->get_current_stage() == DECAY ) )
        {
            break;
        }

        // GET DATA
        env->process( one_sample_buffer, 1 );
        if( env->get_current_stage() != SUSTAIN )
            curve.add(*one_sample_buffer);

        // START COUNT SUSTAIN
        if( env->get_current_stage() == SUSTAIN && ! count_sustain )
        {
            count_sustain = true;
            sustain_start_ = curve.size();
        }

        if( sustain_end_ == -1 && count_sustain && data->sustain_time == 1 )
        {
            env->set_to_release();
            sustain_end_ = sustain_start_ + suatain_samples;
        }
        else if( sustain_end_ == -1 && count_sustain && data->sustain_time != 1 )
        {
            env->set_to_release();
            sustain_end_ = sustain_start_ + msToSamplesFast(8.0f*data->sustain_time*1000,RuntimeNotifyer::getInstanceWithoutCreating()->get_sample_rate());
        }

        if( is_first_attack && env->get_current_stage() == END_ENV )
        {
            is_first_attack = false;
            env->start_attack();
        }
    }

    delete one_sample_buffer;
}



