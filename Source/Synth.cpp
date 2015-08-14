
#include "Synth.h"
#include "mono_AmpPainter.h"


#define THREAD_LIMIT 3
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
class mono_Thread;
class mono_ThreadManager {

    int available_threads;
    CriticalSection cs;

    friend class mono_Thread;
    inline bool can_i_start() noexcept
    {
        ScopedLock lock(cs);
        bool success = false;
        if( available_threads > 0 )
        {
            available_threads--;
            success = true;
        }

        return success;
    }
    inline void im_free() noexcept {
        ScopedLock lock(cs);
        available_threads++;
    }

private:
    mono_ThreadManager();
    ~mono_ThreadManager();

    juce_DeclareSingleton (mono_ThreadManager,false)
};

//==============================================================================
juce_ImplementSingleton (mono_ThreadManager)
mono_ThreadManager::mono_ThreadManager() : available_threads( THREAD_LIMIT ) {}
mono_ThreadManager::~mono_ThreadManager() {
    clearSingletonInstance();
}
//==============================================================================
//==============================================================================
//==============================================================================
class mono_Thread : protected Thread
{
    void run() override
    {
        exec();
        mono_ThreadManager::getInstance()->im_free();
    }

public:
    // HERE YOU HAVE TO WRITE YOUR PARALEL CODE
    virtual void exec() noexcept = 0;

    // IT CHECKS FOR FREE THREADS, OTHERWISE IT RUNS FROM THE CALLER THREAD
    void try_run_paralel() noexcept {
        if( mono_ThreadManager::getInstance()->can_i_start() ) {
            startThread();
        }
        else
        {
            exec();
        }
    }
    inline bool isWorking() const noexcept {
        return isThreadRunning();
    }

protected:
    mono_Thread() : Thread("") {}
    virtual ~mono_Thread() {}
};


juce_ImplementSingleton (mono_ParameterOwnerStore)

// TODO replace the DATA() at the beginning of the loops with constant values

float hidden_function_for_test_in_main(float x) {
    return Random(x).nextFloat();
}
//==============================================================================
//==============================================================================
//==============================================================================
//=========================================================    =====================
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
    mono_ParameterBase<float>*const base;

    float current_value;
    float target_value;
    float delta;
    int counter;

public:
    inline float tick() noexcept;
    inline void update( int glide_time_in_samples_ ) noexcept;

    inline void reset() noexcept;

    NOINLINE ValueSmoother( mono_ParameterBase<float>*const base_ );
    NOINLINE ValueSmoother( const ValueSmoother& other_ );
    NOINLINE ~ValueSmoother();

private:
    //MONO_NOT_CTOR_COPYABLE( ValueSmoother )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmoother )
};

//==============================================================================
NOINLINE ValueSmoother::ValueSmoother( mono_ParameterBase<float>*const base_ )
    :
    base( base_ ),
    current_value( base_->get_scaled_value() ),
    target_value( current_value ),
    delta(0),
    counter(0)
{}
NOINLINE ValueSmoother::ValueSmoother( const ValueSmoother& other_ )
    :
    base( other_.base ),
    current_value( other_.current_value ),
    target_value( other_.target_value ),
    delta(other_.delta),
    counter(other_.counter)
{}

NOINLINE ValueSmoother::~ValueSmoother() {}

//==============================================================================
inline float ValueSmoother::tick() noexcept
{
    if( --counter <= 0 )
        current_value = target_value;
    else
    {
        current_value+=delta;
        //TODO
        /*
            if( current_value > 1 || current_value < 1 )
            {
                current_value = target_value;
                counter = 0;
            }
            */
    }

    return current_value;
}
inline void ValueSmoother::update( int glide_time_in_samples_ ) noexcept {
    float target = base->get_scaled_value();
    if( target_value != target )
    {
        counter = glide_time_in_samples_;
        delta = (target-current_value) / glide_time_in_samples_;
        target_value = target;
    }
}
inline void ValueSmoother::reset() noexcept {
    target_value = base->get_scaled_value();
    current_value = target_value;
    delta = 0;
    counter = 0;
}

//==============================================================================
//==============================================================================
//==============================================================================
class ValueSmootherModulated : public ValueSmoother
{
    float modulation_amount;
    float modulation_range;

    friend class ValueSmootherModulatedUpdater;
    float last_modulation;

public:
    inline float tick( float current_modulation_in_percent_ ) noexcept {
        last_modulation = modulation_range*current_modulation_in_percent_;
        return ValueSmoother::tick() + last_modulation;
    }
    inline float tick( float current_modulation_in_percent_, bool add_modulation_ ) noexcept {
        if( add_modulation_ )
            return ValueSmootherModulated::tick(current_modulation_in_percent_);
        else
            return ValueSmoother::tick();
    }

    inline void update( int glide_time_in_samples_ ) noexcept {
        //TODO MODULATION AMOUNT SMOOTHER
        ValueSmoother::update( glide_time_in_samples_ );
        modulation_amount = base->get_modulation_amount();
        if( modulation_amount >= 0 )
            modulation_range = (base->get_max()-current_value) * modulation_amount;
        else
            modulation_range = (current_value-base->get_min()) * (modulation_amount*-1);
    }

    NOINLINE ValueSmootherModulated( mono_ParameterBase<float>*const base_ );
    NOINLINE ~ValueSmootherModulated();

private:
    MONO_NOT_CTOR_COPYABLE( ValueSmootherModulated )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmootherModulated )
};

//==============================================================================
NOINLINE ValueSmootherModulated::ValueSmootherModulated( mono_ParameterBase<float>*const base_ )
    :
    ValueSmoother( base_ ),
    modulation_amount( base->get_modulation_amount() ),
    modulation_range(0),
    last_modulation(0)
{}
NOINLINE ValueSmootherModulated::~ValueSmootherModulated() {}

//==============================================================================
//==============================================================================
//==============================================================================
class ValueSmootherModulatedTracked : public ValueSmootherModulated
{
    float last_out;
    bool is_changed;

public:
    inline float tick( float current_modulation_in_percent_ ) noexcept {
        float out = ValueSmootherModulated::tick( current_modulation_in_percent_, true );
        is_changed = out != last_out;
        last_out = out;
        return out;
    }
    inline bool is_changed_since_last_tick() const noexcept {
        return is_changed;
    }

    NOINLINE ValueSmootherModulatedTracked( mono_ParameterBase<float>*const base_ );
    NOINLINE ~ValueSmootherModulatedTracked();

private:
    MONO_NOT_CTOR_COPYABLE( ValueSmootherModulatedTracked )
    MONO_NOT_MOVE_COPY_OPERATOR( ValueSmootherModulatedTracked )
};

//==============================================================================
NOINLINE ValueSmootherModulatedTracked::ValueSmootherModulatedTracked( mono_ParameterBase<float>*const base_ )
    :
    ValueSmootherModulated( base_ ),
    is_changed(true),
    last_out(0)
{}
NOINLINE ValueSmootherModulatedTracked::~ValueSmootherModulatedTracked() {}

//==============================================================================
//==============================================================================
//==============================================================================
class ValueSmootherModulatedUpdater {
    ValueSmootherModulated*const smoother;

public:
    inline ValueSmootherModulatedUpdater( ValueSmootherModulated* smoother_, int glide_time_in_samples_ )  noexcept;
    inline ~ValueSmootherModulatedUpdater() noexcept;
};

//==============================================================================
inline ValueSmootherModulatedUpdater::ValueSmootherModulatedUpdater( ValueSmootherModulated* smoother_, int glide_time_in_samples_ ) noexcept
:
smoother( smoother_ )
{
    smoother->update(glide_time_in_samples_);
}
inline ValueSmootherModulatedUpdater::~ValueSmootherModulatedUpdater() noexcept {
    smoother->base->last_modulation_amount = smoother->last_modulation;
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
class AmpSmoothBuffer : RuntimeListener {
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
    NOINLINE AmpSmoothBuffer();
    NOINLINE ~AmpSmoothBuffer();
};

//==============================================================================
NOINLINE AmpSmoothBuffer::AmpSmoothBuffer() : pos(0), sum(0) {
    reset();
}
NOINLINE AmpSmoothBuffer::~AmpSmoothBuffer() {}

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
forcedinline static float lfo2amp( float sample_ ) noexcept {
    return (sample_ + 1.0f)*0.5f;
}

//==============================================================================
//==============================================================================
//==============================================================================
inline static float distortion( float input_and_worker_, float distortion_power_ ) noexcept
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
#define HARD_CLIPPER_STAGE_1 1.1f
#define HARD_CLIPPER_STAGE_2 1.25f

forcedinline static float protection_clipping( float io_ ) noexcept
{
    if( io_ > HARD_CLIPPER_STAGE_1 )
    {
        io_ = HARD_CLIPPER_STAGE_1 + io_*0.1f;
        if( io_ > HARD_CLIPPER_STAGE_2 )
        {
            io_ = HARD_CLIPPER_STAGE_2 + io_*0.05f;
            if( io_ > HARD_CLIPPER_STAGE_2 )
            {
                io_ = HARD_CLIPPER_STAGE_2;
            }
        }
    }
    else if( io_ < -HARD_CLIPPER_STAGE_1 )
    {
        io_ = -HARD_CLIPPER_STAGE_1 + io_*0.1f;
        if( io_ < -HARD_CLIPPER_STAGE_2 )
        {
            io_ = -HARD_CLIPPER_STAGE_2 + io_*0.05f;
            if( io_ < -HARD_CLIPPER_STAGE_2 )
            {
                io_ = -HARD_CLIPPER_STAGE_2;
            }
        }
    }

    return io_;
}

//==============================================================================
//==============================================================================
//==============================================================================
forcedinline static float clipp_to_0_and_1( float input_and_worker_ ) noexcept
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
static float inline taylor_sin(float x) noexcept
{
    float x2 = x*x;
    float x4 = x2*x2;

    // Calculate the terms
    // As long as abs(x) < sqrt(6), which is 2.45, all terms will be positive.
    // Values outside this range should be reduced to [-pi/2, pi/2] anyway for accuracy.
    // Some care has to be given to the factorials.
    // They can be pre-calculated by the compiler,
    // but the value for the higher ones will exceed the storage capacity of int.
    // so force the compiler to use unsigned long longs (if available) or doubles.
    float t1 = x * (1.0f - x2 / (2*3));
    float x5 = x * x4;
    float t2 = x5 * (1.0f - x2 / (6*7)) / (1.0f* 2*3*4*5);
    float x9 = x5 * x4;
    float t3 = x9 * (1.0f - x2 / (10*11)) / (1.0f* 2*3*4*5*6*7*8*9);
    float x13 = x9 * x4;
    float t4 = x13 * (1.0f - x2 / (14*15)) / (1.0f* 2*3*4*5*6*7*8*9*10*11*12*13);
    // add some more if your accuracy requires them.
    // But remember that x is smaller than 2, and the factorial grows very fast
    // so I doubt that 2^17 / 17! will add anything.
    // Even t4 might already be too small to matter when compared with t1.

    return t4 + t3 + t2 + t1;
}
#define TABLESIZE_MULTI 1000
#define SIN_LOOKUP_TABLE_SIZE int(float_Pi*TABLESIZE_MULTI*2)
float* SINE_LOOKUP_TABLE;
struct SIN_LOOKUP
{
    juce_DeclareSingleton (SIN_LOOKUP,false)
    NOINLINE SIN_LOOKUP();
    NOINLINE ~SIN_LOOKUP();
};
juce_ImplementSingleton (SIN_LOOKUP)
SIN_LOOKUP*const sine_lookup_self_init = SIN_LOOKUP::getInstance();

NOINLINE SIN_LOOKUP::SIN_LOOKUP()
{
    SINE_LOOKUP_TABLE = new float[SIN_LOOKUP_TABLE_SIZE+1];
    for(int i = 0; i < SIN_LOOKUP_TABLE_SIZE; i++)
        SINE_LOOKUP_TABLE[i] = std::sin( double(i) / TABLESIZE_MULTI );
}
NOINLINE SIN_LOOKUP::~SIN_LOOKUP()
{
    delete[] SINE_LOOKUP_TABLE;

    clearSingletonInstance();
}

static float inline lookup_sine(float x) noexcept
{
    return SINE_LOOKUP_TABLE[ int(int64(x*TABLESIZE_MULTI) % SIN_LOOKUP_TABLE_SIZE) ];
}

//==============================================================================
NOINLINE DataBuffer::DataBuffer( int init_buffer_size_ )
    :
    current_buffer_size( init_buffer_size_ ),

    tmp_buffer_6( init_buffer_size_ ),
    tmp_multithread_band_buffer_9_4( init_buffer_size_ ),

    lfo_amplitudes( init_buffer_size_ ),
    direct_filter_output_samples( init_buffer_size_ ),

    osc_samples( init_buffer_size_ ),
    osc_switchs( init_buffer_size_ ),
    osc_sync_switchs( init_buffer_size_ ),
    modulator_samples( init_buffer_size_ ),

    filter_output_samples( init_buffer_size_ ),
    filter_env_amps( init_buffer_size_ )
{}

void DataBuffer::resize_buffer_if_required( int min_size_required_ ) noexcept
{
    if( min_size_required_ > current_buffer_size )
    {
        current_buffer_size = min_size_required_;

        tmp_buffer_6.setSize(min_size_required_);
        tmp_multithread_band_buffer_9_4.setSize(min_size_required_);

        lfo_amplitudes.setSize(min_size_required_);
        direct_filter_output_samples.setSize(min_size_required_);

        osc_samples.setSize(min_size_required_);
        osc_switchs.setSize(min_size_required_);
        osc_sync_switchs.setSize(min_size_required_);
        modulator_samples.setSize(min_size_required_);

        filter_output_samples.setSize(min_size_required_);
        filter_env_amps.setSize(min_size_required_);
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
    NOINLINE mono_BlitSaw( float frequency = 220.0f );
    NOINLINE ~mono_BlitSaw();
};

// -----------------------------------------------------------------
NOINLINE mono_BlitSaw::mono_BlitSaw( float frequency )
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
NOINLINE mono_BlitSaw::~mono_BlitSaw() {}

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
        tmp = std::sin( float(m_ * phase_) ) / (p_ * denominator);
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
    NOINLINE mono_BlitSquare( float frequency = 220.0f );
    NOINLINE ~mono_BlitSquare();
};

// -----------------------------------------------------------------
NOINLINE mono_BlitSquare::mono_BlitSquare( float frequency )
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
NOINLINE mono_BlitSquare::~mono_BlitSquare()
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
    NOINLINE void sample_rate_changed( double old_sr_ ) noexcept override;

public:
    NOINLINE mono_SineWave();
    NOINLINE ~mono_SineWave();
};

// -----------------------------------------------------------------
NOINLINE mono_SineWave::mono_SineWave()
    :
    last_tick_value(0),
    delta(0),
    current_angle(0),
    frequency(0),

    _isNewCylce(0)
{
    setFrequency(440);
}

NOINLINE mono_SineWave::~mono_SineWave() {}

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

NOINLINE void mono_SineWave::sample_rate_changed( double /*old_sr_*/ ) noexcept {
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
    NOINLINE mono_Noise();
    NOINLINE ~mono_Noise();
};

// -----------------------------------------------------------------
NOINLINE mono_Noise::mono_Noise() : last_tick_value(0) {}
NOINLINE mono_Noise::~mono_Noise() {}

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
    NOINLINE mono_OnePole( float thePole = 0.9 );
    NOINLINE ~mono_OnePole();

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
    NOINLINE mono_Modulate();
    NOINLINE ~mono_Modulate();
};

// -----------------------------------------------------------------
NOINLINE mono_Modulate::mono_Modulate()
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
NOINLINE mono_Modulate::~mono_Modulate() {}

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
class LFO {
    mono_SineWave sine_generator;

    float last_frequency;

    const int id;
    int last_factor;
    int step_to_wait_for_sync;

public:
    inline void process( int step_number_, int num_samples_ ) noexcept;

private:
    NOINLINE void sync( int step_number_ ) noexcept;

public:
    float get_current_amp() const noexcept;

    NOINLINE LFO( int id_ );
    NOINLINE ~LFO();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFO)
};

//==============================================================================
NOINLINE LFO::LFO( int id_ )
    :
    sine_generator(),

    last_frequency(0),

    id(id_),
    last_factor(0),
    step_to_wait_for_sync(false)
{
}
NOINLINE LFO::~LFO() {}

//==============================================================================
inline void LFO::process( int step_number_, int num_samples_ ) noexcept {
    sync( step_number_ );

    float*const buffer_to_fill =  DATA(data_buffer).lfo_amplitudes.getWritePointer(id);
    for( int sid = 0 ; sid != num_samples_ ; ++sid )
        buffer_to_fill[sid] = (1.0f + sine_generator.tick()) * 0.5f;
}
NOINLINE void LFO::sync( int step_number_ ) noexcept {
    const float speed( DATA( lfo_datas[id] ).speed );
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

            const float whole_notes_per_sec( DATA( runtime_info ).bpm / 4 / 60 ); // = 1 at our slider
            float frequency = whole_notes_per_sec / factor;

            step_to_wait_for_sync--;
            if( last_factor != factor ) {
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

            const float whole_notes_per_sec( DATA( runtime_info ).bpm/4 / 60 ); // = 1 at our slider
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
        const float frequency = midiToFrequency(33+speed-18);
        if( frequency != last_frequency ) {
            last_frequency = frequency;
            sine_generator.setFrequency( frequency );
        }

        step_to_wait_for_sync = 0;
        last_factor = 0;
    }
}

float LFO::get_current_amp() const noexcept {
    return lfo2amp(sine_generator.lastOut());
}

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class OSC : public RuntimeListener {
    const int id;

    float last_frequency;
    int glide_time_in_samples;
    float glide_note_delta;

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

    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;

    mono_Modulate modulator;
    mono_Noise noise;

    ValueSmootherModulatedTracked octave_smoother;
    ValueSmoother fm_amount_smoother;

public:
    inline void process(DataBuffer* data_buffer_, const int num_samples_) noexcept;
    inline void reset( int root_note_ ) noexcept;

public:
    NOINLINE OSC( int id_ );
    NOINLINE ~OSC();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC)
};

NOINLINE OSC::OSC( int id_ )
    :
    id( id_ ),
    last_frequency( 0 ),
    glide_time_in_samples(0),
    glide_note_delta(0),
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

    octave_smoother( &DATA( osc_datas[id] ).octave ),
    fm_amount_smoother( &DATA( osc_datas[id] ).fm_amount )
{
    modulator.setVibratoGain(1);
}
NOINLINE OSC::~OSC() {}

inline void OSC::process(DataBuffer* data_buffer_,
                         const int num_samples_) noexcept
{
    OSCData& osc_data = DATA( osc_datas[id] );
    OSCData& master_osc_data = DATA( osc_datas[MASTER_OSC] );

    // LFO
    const float* lfo_amps( data_buffer_->lfo_amplitudes.getReadPointer(id) );

    // BUFFERS
    float* output_buffer( data_buffer_->osc_samples.getWritePointer(id) );
    float* switch_buffer( data_buffer_->osc_switchs.getWritePointer(id) );

    float* osc_sync_switch_buffer( data_buffer_->osc_sync_switchs.getWritePointer(MASTER_OSC) );
    float* osc_modulator_buffer( data_buffer_->modulator_samples.getWritePointer(MASTER_OSC) );

    // FM SWING
    const float master_fm_swing = master_osc_data.fm_swing;
    const bool master_osc_modulation_is_off = id == MASTER_OSC ? master_osc_data.mod_off : false;
    if( master_fm_swing != 0 ) {
        const bool was_negative = puls_swing_delta < 0;
        puls_swing_delta = sample_rate * master_fm_swing * 0.00000005;
        if( was_negative )
            puls_swing_delta *= -1;
    }
    else
        puls_swing_amp = 1;

    const int glide_motor_time = DATA( synth_data ).glide_motor_time;
    ValueSmootherModulatedUpdater u_octave( &octave_smoother, glide_motor_time );
    fm_amount_smoother.update( glide_motor_time );

    const float wave = osc_data.wave;
    const bool is_lfo_modulated = osc_data.is_lfo_modulated;
    const bool sync = osc_data.sync;
    const float master_fm_multi = master_osc_data.fm_multi;
    const int master_pulse_width = master_osc_data.puls_width;
    const bool master_sync = master_osc_data.sync;
    const bool master_fm_wave = master_osc_data.fm_wave;
    const int master_switch = master_osc_data.osc_switch;
    for( int sid = 0 ; sid < num_samples_ ; ++sid )
    {
        // UPDATE FREQUENCY - TODO after ticks
        {
            const float octave_mod = octave_smoother.tick( is_lfo_modulated ? lfo_amps[sid] : 0 );
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
                    const int note_low = std::floor(floated_note);
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
                else if( master_pulse_width > 0 && puls_with_break_counter < 1 )
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
            if( sync && id != MASTER_OSC )
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
                    if( !waiting_for_modulator_sync && current_modulator_sync_cycle >= used_sync_cycles )
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
            if( puls_with_break_counter > 0 && !master_osc_modulation_is_off ) {
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

inline void OSC::reset( int root_note_ ) noexcept {
    const float glide = DATA( synth_data ).glide;
    if( glide != 0 && (_root_note != root_note_ || glide_time_in_samples > 0 ) ) {
        const float rest_glide = glide_time_in_samples*glide_note_delta;
        const float glide_notes_diverence = _root_note - root_note_;
        glide_time_in_samples = (sample_rate/2) * glide;
        glide_note_delta = (glide_notes_diverence+rest_glide) / glide_time_in_samples;
    }
    else {
        glide_note_delta = 0;
        glide_time_in_samples = 0;
    }

    _root_note = root_note_;
}

//==============================================================================
#define FORCE_ZERO_SAMPLES 50
enum STAGES {
    END_ENV = false,
    ATTACK = true,
    DECAY,
    SUSTAIN,
    RELEASE,
    TRIGGER_START
};
enum OPTIONS {
    WORK_FROM_CURRENT_VALUE = -1
};
class ValueEnvelope : public RuntimeListener {
    int samples_to_target_left;
    float current_value;
    float end_value;
    float delta;
    int current_force_zero_counter;
    float force_zero_offset;

public:
    inline float tick( float shape_, float shape_factor_ ) noexcept;
    inline void update( float end_value_,
                        float time_sample_rate_factor_,
                        float start_value_ = WORK_FROM_CURRENT_VALUE ) noexcept;
    inline bool end_reached() const noexcept;
    inline void replace_current_value( float value_ ) noexcept;
    inline void force_zero_glide() noexcept;

    inline void reset() noexcept;

    // FOR UI PURPOSES
    float get_current_amp() const noexcept;

    NOINLINE ValueEnvelope() noexcept;
    NOINLINE ~ValueEnvelope() noexcept;
};

NOINLINE ValueEnvelope::ValueEnvelope() noexcept
:
samples_to_target_left(0),
                       current_value(0),
                       end_value(0),
                       delta(0),
                       current_force_zero_counter(0),
                       force_zero_offset(0)
{

}
NOINLINE ValueEnvelope::~ValueEnvelope() noexcept {}
// TODO if sustain only call if sustain is endless!
inline float ValueEnvelope::tick( float shape_, float shape_factor_ ) noexcept {
    float value;
    if( samples_to_target_left > 0 )
    {
        --samples_to_target_left;

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

                    current_value -= (((exp(delta_*2)-1.0f)/6.38906f)*shape_factor_release + delta_*(1.0f-shape_factor_release));
                }
            }
            else if( shape_ > 0.75f )
            {
                float delta_ = (end_value-current_value)/samples_to_target_left;
                if( delta_ >= 0 )
                    current_value += (((exp(delta_*2)-1.0f)/6.38906f)*shape_factor_ + delta_*(1.0f-shape_factor_));
                else
                {
                    delta_ *= -1;
                    float shape_factor_release = (shape_-0.75f)*4;
                    current_value -= (((exp(delta_*2)-1.0f)/6.38906f)*shape_factor_release + delta_*(1.0f-shape_factor_release));
                }
            }
            else if( shape_ > 0.5f )
            {
                float delta_ = (end_value-current_value)/samples_to_target_left;
                if( delta_ >= 0 )
                    current_value += (((exp(delta_*2)-1.0f)/6.38906f)*shape_factor_ + delta_*(1.0f-shape_factor_));
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
                current_value+=delta;
            }
        }

        if( current_value > 1 )
            current_value = 1;
        else if( current_value < 0 )
            current_value = 0;
    }

    if( (current_force_zero_counter--) > 0 )
        value = current_value+force_zero_offset*current_force_zero_counter;
    else
        value = current_value;

    return value;
}
inline bool ValueEnvelope::end_reached() const noexcept {
    return samples_to_target_left <= 0;
}
inline void ValueEnvelope::replace_current_value( float value_ ) noexcept {
    current_value = value_;
}
inline void ValueEnvelope::force_zero_glide() noexcept {
    current_force_zero_counter = FORCE_ZERO_SAMPLES;
    force_zero_offset = current_value/FORCE_ZERO_SAMPLES;
    current_value = 0;
}
inline void ValueEnvelope::reset() noexcept {
    samples_to_target_left = 0;
    current_value = 0;
    end_value = 0;
    delta = 0;
    current_force_zero_counter = 0;
    force_zero_offset = 0;
}

inline void ValueEnvelope::update( float end_value_,
                                   float time_sample_rate_factor_,
                                   float start_value_
                                 ) noexcept
{
    // UPDATE INTERNALS
    if( start_value_ != WORK_FROM_CURRENT_VALUE )
        current_value = start_value_;

    end_value = end_value_;

    // CALC
    const float distance = end_value_-current_value;
    samples_to_target_left = msToSamplesFast( time_sample_rate_factor_*1000, sample_rate );
    if( samples_to_target_left > 0 )
    {
        delta = distance / samples_to_target_left;
    }
    else
    {
        delta = 0;
        current_value = end_value;
    }
}

float ValueEnvelope::get_current_amp() const noexcept {
    return current_value;
}

class ENV {
    ValueEnvelope envelop;
public:
    // TODO private
    STAGES current_stage;

private:
    ENVData& data;
    ValueSmoother sustain_smoother;

public:
    inline void process( float* dest_, const int num_samples_ ) noexcept;
private:
    inline void update_stage() noexcept;

public:
    // TODO, add to filter processor
    inline void start_attack( float set_to_zero = true ) noexcept;
    inline void set_to_release() noexcept;
    inline void reset() noexcept;

public:
    float get_amp() const noexcept;

    NOINLINE ENV( ENVData& data_ );
    NOINLINE ~ENV();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENV)
};

NOINLINE ENV::ENV( ENVData& data_ )
    :
    current_stage(END_ENV),
    data( data_ ),
    sustain_smoother( &data_.sustain )
{
}
NOINLINE ENV::~ENV() {}

inline void ENV::process( float* dest_, const int num_samples_ ) noexcept {
    // TODO, if you came from end env, reset sustain!
    sustain_smoother.update( DATA( synth_data ).glide_motor_time );
    const float shape = DATA( synth_data ).curve_shape;
    float shape_factor = 1;
    if( shape < 0.5f ) {
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

inline void ENV::update_stage() noexcept {
    RuntimeInfo& info( DATA( runtime_info ) );

    switch( current_stage )
    {
    case TRIGGER_START :
    {
        if( data.decay )
            envelop.update( 1, data.attack*data.max_attack_time );
        else
        {
            envelop.update( sustain_smoother.tick(), data.attack*data.max_attack_time );
        }

        current_stage = ATTACK;
        break;
    }
    case ATTACK :
    {
        if( data.decay )
        {
            envelop.update( sustain_smoother.tick(), data.decay*data.max_decay_time );
            current_stage = DECAY;
            break;
        }
    }
    case DECAY :
    {
        float sustain_time = data.sustain_time;
        if( sustain_time == 1.0f )
            sustain_time = sustain_time*1000;
        else
            sustain_time = sustain_time*8;

        envelop.update( sustain_smoother.tick(), sustain_time );
        current_stage = SUSTAIN;
    }
    break;
    case SUSTAIN :
    {
        envelop.update( 0, data.release*data.max_release_time );
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

inline void ENV::start_attack( float set_to_zero ) noexcept {
    current_stage = TRIGGER_START;
    if( set_to_zero )
        envelop.force_zero_glide();

    update_stage();
}
inline void ENV::set_to_release() noexcept {
    current_stage = SUSTAIN;
    update_stage();
}

inline void ENV::reset() noexcept {
    current_stage = END_ENV;
    envelop.reset();
}

float ENV::get_amp() const noexcept {
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
class AmpSmoother {
    float current_value;
    float target_value;
    float delta;
    int counter;

private:
    inline void glide_tick() noexcept;

public:
    inline float add_get( float in_ ) noexcept;
    bool is_up_to_date() const noexcept {
        return target_value == current_value;
    }
    inline float add_get_and_keep_current_time( float in_ ) noexcept;
    inline void reset( float value_ = 0 ) noexcept {
        current_value = value_;
        delta = 0;
        counter = 0;
    }
    inline float get_current_value() const noexcept {
        return current_value;
    }

    NOINLINE AmpSmoother( float start_value_ = 0 ) noexcept;
    NOINLINE ~AmpSmoother() noexcept;

private:
    MONO_NOT_CTOR_COPYABLE( AmpSmoother )
    MONO_NOT_MOVE_COPY_OPERATOR( AmpSmoother )
};

//==============================================================================
template< int smooth_samples >
NOINLINE AmpSmoother<smooth_samples>::AmpSmoother( float start_value_ ) noexcept
:
current_value(0),
              target_value(start_value_),
              delta(0),
              counter(0)
{}
template< int smooth_samples >
NOINLINE AmpSmoother<smooth_samples>::~AmpSmoother() noexcept {}

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
    // OLD OR DEFAULT ONE
    inline void set(float r,float c, float gain_) noexcept;
    // RETURNS TRUE ON COFF CHENGED
    inline bool update(float resonance_,
                       float cutoff_,
                       float gain_ ) noexcept;
    inline void copy_coefficient_from( const AnalogFilter& other_ ) noexcept;
    inline void copy_state_from( const AnalogFilter& other_ ) noexcept;

    inline float processLow(float input_and_worker_) noexcept;

    inline float processLowResonance(float input_and_worker_) noexcept;
    inline float processHighResonance(float input_and_worker_) noexcept;

    inline void reset() noexcept;

private:
    inline void calc_coefficient() noexcept;
    inline void calc() noexcept;

    NOINLINE void sample_rate_changed( double ) noexcept override;

public:
    NOINLINE AnalogFilter();
    NOINLINE ~AnalogFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFilter)
};
// -----------------------------------------------------------------
NOINLINE AnalogFilter::AnalogFilter()
    :
    p(0),k(0),r(0),gain(0),
    y1(0),y2(0),y3(0),y4(0),
    oldx(0),oldy1(0),oldy2(0),oldy3(0),

    cutoff(1000), res(0), res4(0)
{
    sample_rate_changed(0);
}
NOINLINE AnalogFilter::~AnalogFilter() {}

// -----------------------------------------------------------------
inline void AnalogFilter::reset() noexcept {
    y4 = oldx = oldy1 = oldy2 = oldy3 = 0;
}

// -----------------------------------------------------------------
inline void AnalogFilter::set(float r, float c, float gain_) noexcept
{
    if( r != res or c != cutoff or gain != gain_ )
    {
        gain = gain_;
        res = r;
        res4 = res*4;
        cutoff = c;
        calc();
    }
}
inline void AnalogFilter::calc() noexcept
{
    {
        const float f = (cutoff+cutoff) * sample_rate_1ths;
        const float agressive = 0.48f*gain;
        p=f*((1.5f+agressive)-((0.5f+agressive)*f));
        k=p*2-1;
    }
    {
        const float t=(1.0f-p)*1.386249f;
        const float t2=12.0f+t*t;
        r = res*(t2+6.0f*t)/(t2-6.0f*t);
    }
}
inline float AnalogFilter::processLow(float input_and_worker_) noexcept
{
    input_and_worker_ = protection_clipping(input_and_worker_);

    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4) * (1.0f/6);

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    return (y4 + distortion(y3*res+y4*(1.0f-res),gain)*gain)/(1.0f+gain);
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
    // MONSTER RESONANCE LP
    inline void updateLowResonance(float resonance_,
                                   float cutoff_,
                                   float gain_) noexcept;
    inline float processLowResonance(float in_) noexcept;

    // ----------------------------------------------------------
    // 2PASS LP
    inline void updateLow2Pass(float resonance_,
                               float cutoff_,
                               float gain_) noexcept;
    inline float processLow2Pass(float in_) noexcept;

    // ----------------------------------------------------------
    // MONSTER RESONANCE HP
    inline void updateHighResonance(float resonance_,
                                    float cutoff_,
                                    float gain_) noexcept;
    inline float processHighResonance(float in_) noexcept;

    // ----------------------------------------------------------
    // 2PASS HP
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

    NOINLINE DoubleAnalogFilter(bool create_smooth_filter = true);
    NOINLINE ~DoubleAnalogFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleAnalogFilter)
};

// -----------------------------------------------------------------
NOINLINE DoubleAnalogFilter::DoubleAnalogFilter(bool create_smooth_filter)
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
NOINLINE DoubleAnalogFilter::~DoubleAnalogFilter() {
    if( smooth_filter )
        delete smooth_filter;
}

inline void DoubleAnalogFilter::reset() noexcept {
    flt_1.reset();
    flt_2.reset();
}

// -----------------------------------------------------------------
forcedinline static float resonance_clipping( float sample_ ) noexcept
{
    return (std::atan(sample_) * (1.0f/float_Pi))*2;
}

// -----------------------------------------------------------------
inline void AnalogFilter::calc_coefficient() noexcept
{
    float f = (cutoff+cutoff) * sample_rate_1ths;
    float agressive = 0.48f*gain;
    p=f*((1.5f+agressive)-((0.5f+agressive)*f));
    k=p*2-1;
}
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
NOINLINE void AnalogFilter::sample_rate_changed(double) noexcept
{
    calc_coefficient();
    calc();
}
// -----------------------------------------------------------------
inline bool AnalogFilter::update(float resonance_, float cutoff_, float gain_) noexcept
{
    bool success = false;
    if( res != resonance_ )
    {
        res = resonance_;
        res4 = res*4;
    }

    if( cutoff != cutoff_ || gain != gain_ )
    {
        gain = gain_;
        cutoff = cutoff_;
        calc_coefficient();
        success = true;
    }
    return success;
}

// 2 PASS LP
// -----------------------------------------------------------------
inline float AnalogFilter::processLowResonance(float input_and_worker_) noexcept
{
    input_and_worker_ = protection_clipping(input_and_worker_);
  
    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4) * (1.0f/6);

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // Add resonance
    return y4 + std::atan( y3 * res4 );
}
inline void DoubleAnalogFilter::updateLowResonance(float resonance_, float cutoff_, float gain_) noexcept
{
    flt_1.update( resonance_, cutoff_+35, gain_ );
}
inline float DoubleAnalogFilter::processLowResonance( float in_ ) noexcept
{
    return process_filter_change
    (
        in_,
        flt_1.processLowResonance( in_ )
    );
}

// LP
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateLow2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    if( flt_2.update( resonance_, cutoff_, gain_ ) )
        flt_1.copy_coefficient_from( flt_2 );
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
inline float AnalogFilter::processHighResonance(float input_and_worker_) noexcept
{
    input_and_worker_ = protection_clipping(input_and_worker_);
  
    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4) * (1.0f/6);

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // RESONANCE
    return (input_and_worker_-y4) + (std::atan( y2 * res4 ));
}
inline void DoubleAnalogFilter::updateHigh2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    if( flt_2.update( resonance_, cutoff_, gain_ ) )
        flt_1.copy_coefficient_from( flt_2 );
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

// HP
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateHighResonance(float resonance_, float cutoff_, float gain_) noexcept
{
    flt_1.update( resonance_, cutoff_, gain_ );
}
inline float DoubleAnalogFilter::processHighResonance(float in_) noexcept
{
    return process_filter_change
    (
        in_,
        flt_1.processHighResonance( in_ )
    );
}

// BAND
// -----------------------------------------------------------------
inline void DoubleAnalogFilter::updateBand(float resonance_, float cutoff_, float gain_ ) noexcept
{
    if( flt_1.update( resonance_, cutoff_+10, gain_ ) )
        flt_2.update( resonance_, cutoff_, gain_ );
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
                flt_2.copy_state_from(flt_1);
                break;
            case LPF_2_PASS :
                break;
            case HPF :
                flt_2.copy_state_from(flt_1);
                break;
            case HIGH_2_PASS :
                break;
            case BPF :
                break;
            case PASS :
                //flt_1.reset();
                //flt_2.reset();
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
    if( glide_time_4_filters != 0 )
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
        io_ = processLowResonance(io_);
        break;
    case LPF_2_PASS :
        io_ = processLow2Pass(io_);
        break;
    case HPF :
        io_ = processHighResonance(io_);
        break;
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

public:
    NOINLINE EnvelopeFollower();
    NOINLINE ~EnvelopeFollower();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeFollower)
};

// -----------------------------------------------------------------
NOINLINE EnvelopeFollower::EnvelopeFollower()
    : envelope (0),
      attack (1),
      release (1)
{}
NOINLINE EnvelopeFollower::~EnvelopeFollower() {}

// -----------------------------------------------------------------
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
class FilterProcessor {
    DoubleAnalogFilter double_filter[SUM_INPUTS_PER_FILTER];
    friend class mono_ParameterOwnerStore;
    OwnedArray< ENV > input_envs;
    EnvelopeFollower env_follower;

    ValueSmootherModulated cutoff_smoother;
    ValueSmootherModulated resonance_smoother;
    ValueSmootherModulated gain_smoother;
    ValueSmootherModulated distortion_smoother;
    ValueSmootherModulated output_smoother;
    ValueSmoother mix_smoother;
    ValueSmoother clipping_smoother;
    ValueSmoother input_sustains[SUM_INPUTS_PER_FILTER];

    const int id;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void process( const int num_samples ) noexcept;

private:
    inline void compress ( float* io_buffer_, float* tmp_buffer_, const float* compressor_signal,
                           float power,
                           int num_samples ) noexcept;

public:
    NOINLINE FilterProcessor( int id_ );
    NOINLINE ~FilterProcessor();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor)
};

// -----------------------------------------------------------------
NOINLINE FilterProcessor::FilterProcessor( int id_ )
    :
    input_envs(),
    env_follower(),

    cutoff_smoother( &DATA( filter_datas[id_] ).cutoff ),
    resonance_smoother( &DATA( filter_datas[id_] ).resonance ),
    gain_smoother( &DATA( filter_datas[id_] ).gain ),
    distortion_smoother( &DATA( filter_datas[id_] ).distortion ),
    output_smoother( &DATA( filter_datas[id_] ).output ),
    mix_smoother( &DATA( filter_datas[id_] ).adsr_lfo_mix ),
    clipping_smoother( &DATA( filter_datas[id_] ).output_clipping ),
    input_sustains { &DATA( filter_datas[id_] ).input_sustains[0], &DATA( filter_datas[id_] ).input_sustains[1], &DATA( filter_datas[id_] ).input_sustains[2] },

               id(id_)
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i )
        input_envs.add( new ENV( DATA( filter_input_env_datas[i + SUM_INPUTS_PER_FILTER * id_] ) ) );
}
NOINLINE FilterProcessor::~FilterProcessor() {}

// -----------------------------------------------------------------
inline void FilterProcessor::start_attack() noexcept
{
    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        input_envs.getUnchecked(input_id)->start_attack();
}
inline void FilterProcessor::start_release() noexcept
{
    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
        input_envs.getUnchecked(input_id)->set_to_release();
}

// -----------------------------------------------------------------
inline void FilterProcessor::process( const int num_samples ) noexcept
{
    DataBuffer& data_buffer( DATA(data_buffer) );
    const int glide_motor_time = DATA( synth_data ).glide_motor_time;

    FilterData& filter_data = DATA( filter_datas[id] );

    // COLLECT / PREPARE
    float* input_ar_amps[SUM_INPUTS_PER_FILTER];
    const float* input_buffers[SUM_INPUTS_PER_FILTER];
    float* out_buffers[SUM_INPUTS_PER_FILTER];
    float* amp_mix = data_buffer.lfo_amplitudes.getWritePointer(id);
    {
#define DIMENSION_INPUT_BUFFER input_id
#define DIMENSION_INPUT_AR_BUFFER SUM_INPUTS_PER_FILTER + input_id
        struct PrepareExecuter : public mono_Thread {
            FilterProcessor*const processor;
            const int num_samples_;
            const int input_id;
            inline void exec() noexcept override
            {
                DataBuffer& data_buffer( DATA(data_buffer) );
                float* tmp_input_buffer = data_buffer.tmp_buffer_6.getWritePointer( DIMENSION_INPUT_BUFFER );
                float* tmp_input_ar_amp = data_buffer.tmp_buffer_6.getWritePointer( DIMENSION_INPUT_AR_BUFFER );

                const int glide_motor_time = DATA( synth_data ).glide_motor_time;

                // CALCULATE INPUTS AND ENVELOPS
                {
                    ValueSmoother& input_sustain_smoother = processor->input_sustains[input_id];
                    input_sustain_smoother.update( glide_motor_time );
                    const float*const osc_input_buffer = data_buffer.osc_samples.getReadPointer(input_id);
                    if( DATA( filter_datas[processor->id] ).input_holds[input_id] )
                    {
                        processor->input_envs.getUnchecked(input_id)->reset();

                        if( processor->id == FILTER_1 )
                        {
                            for( int sid = 0 ; sid != num_samples_ ; ++sid )
                            {
                                tmp_input_ar_amp[sid] = input_sustain_smoother.tick();
                                tmp_input_buffer[sid] = osc_input_buffer[sid];
                            }
                        }
                        else
                        {
                            const float* filter_before_buffer = data_buffer.filter_output_samples.getReadPointer(input_id + SUM_INPUTS_PER_FILTER*(processor->id-1) );
                            for( int sid = 0 ; sid != num_samples_ ; ++sid )
                            {
                                const float sustain = input_sustain_smoother.tick();
                                if( sustain < 0 )
                                {
                                    tmp_input_ar_amp[sid] = sustain*-1;
                                    tmp_input_buffer[sid] = osc_input_buffer[sid];
                                }
                                else
                                {
                                    tmp_input_ar_amp[sid] = sustain;
                                    tmp_input_buffer[sid] = filter_before_buffer[sid];
                                }
                            }

                        }
                    }
                    else // USING ADR
                    {
                        processor->input_envs.getUnchecked(input_id)->process( tmp_input_ar_amp, num_samples_ );

                        if( processor->id == FILTER_1 )
                        {
                            input_sustain_smoother.reset();
                            for( int sid = 0 ; sid != num_samples_ ; ++sid )
                            {
                                tmp_input_buffer[sid] = osc_input_buffer[sid];
                            }
                        }
                        else
                        {
                            const float* filter_before_buffer = data_buffer.filter_output_samples.getReadPointer(input_id + SUM_INPUTS_PER_FILTER*(processor->id-1) );
                            for( int sid = 0 ; sid != num_samples_ ; ++sid )
                            {
                                const float sustain = input_sustain_smoother.tick();
                                if( sustain < 0 )
                                    tmp_input_buffer[sid] = osc_input_buffer[sid];
                                else
                                    tmp_input_buffer[sid] = filter_before_buffer[sid];
                            }
                        }
                    }
                }
            }
            PrepareExecuter(FilterProcessor*const processor_,
            int num_samples__,
            int input_id_)
                : processor(processor_),
                num_samples_(num_samples__),
                input_id(input_id_)
            {}
        };
	/*
	 TRY TO ADD IT TO THE THREAD after 
	*/
	/*
	TRY TO COMPARE WIRTH THE OLD VERSION
	REMOVE ANY ACCESSES TO OUTSIDE FROM THE EXECUTERS!!!  ->>>>>> processor->id etc...
	HOW EXPENSIVE IS A THREAD BUILD? ----> keep the threads builded!
	TRY THE PERFORMANCE IT THE ID IS A FIXED TEMPLATE PARAM!
	*/
        PrepareExecuter prep_1( this, num_samples, 0 );
        prep_1.exec();
        PrepareExecuter prep_2( this, num_samples, 1 );
        prep_2.exec();
        PrepareExecuter prep_3( this, num_samples, 2 );
        prep_3.exec();

        // SINGLE THREAD, WAIT FOR SECOND
        {
            // COLLECT BUFFERS
            for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
            {
                // COLLECT BUFFERS
                input_buffers[input_id] = data_buffer.tmp_buffer_6.getWritePointer( DIMENSION_INPUT_BUFFER );
                input_ar_amps[input_id] = data_buffer.tmp_buffer_6.getWritePointer( DIMENSION_INPUT_AR_BUFFER );
                out_buffers[input_id] = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * id );
            }

            // ADSTR - LFO MIX
            mix_smoother.update(glide_motor_time);
            {
                const float* env_amps = data_buffer.filter_env_amps.getReadPointer(id);
                const float* lfo_amplitudes = data_buffer.lfo_amplitudes.getReadPointer(id);
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    // LFO ADSR MIX - HERE TO SAVE ONE LOOP
                    const float mix = (1.0f+mix_smoother.tick()) * 0.5f;
                    amp_mix[sid] = env_amps[sid]*(1.0f-mix) + lfo_amplitudes[sid]*mix;
                }
            }
        }

        // END OF MULTI THREADED
        while( prep_1.isWorking() ) {}
    }

    // PROCESS FILTER
    {
#define DISTORTION(x)  distortion(x,filter_distortion)

#define MAX_CUTOFF 8000.0f
#define MIN_CUTOFF 40.0f

#define DIMENSION_RESONANCE 0
#define DIMENSION_CUTOFF 1
#define DIMENSION_GAIN 2
#define DIMENSION_DISTORTION 3

        const bool modulate_cutoff = filter_data.modulate_cutoff;
        const bool modulate_resonance = filter_data.modulate_resonance;
        const bool modulate_gain = filter_data.modulate_gain;
        const bool modulate_distortion = filter_data.modulate_distortion;
        ValueSmootherModulatedUpdater u_cutoof( &cutoff_smoother, glide_motor_time );
        ValueSmootherModulatedUpdater u_resonance( &resonance_smoother, glide_motor_time );
        ValueSmootherModulatedUpdater u_gain( &gain_smoother, glide_motor_time );
        ValueSmootherModulatedUpdater u_distortion( &distortion_smoother, glide_motor_time );

        // MULTITHREADED EXECUTERS
        struct FilterExecuterBase : public mono_Thread {};
        FilterExecuterBase* executer_1;
        FilterExecuterBase* executer_2;
        FilterExecuterBase* executer_3;

        switch( filter_data.filter_type )
        {
        case LPF_2_PASS :
        case MOOG_AND_LPF:
        {
            // PREPARE
            {
                float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp, modulate_resonance );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF * cutoff_smoother.tick( amp, modulate_cutoff ) + MIN_CUTOFF) * (1.0f/8);
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct LP2PassExecuter : public FilterExecuterBase {
                DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(LPF_2_PASS);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                    const float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateLow2Pass( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION( filter.processLow2Pass( DISTORTION( input_buffer[sid] ) ) );
                    }
                }
                LP2PassExecuter(FilterProcessor*const processor_,
                                int num_samples__,
                                int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new LP2PassExecuter( this, num_samples, 0 );
            executer_2 = new LP2PassExecuter( this, num_samples, 1 );
            executer_3 = new LP2PassExecuter( this, num_samples, 2 );
        }
        break;
        case LPF:
        {
            // PREPARE
            {
                float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);
                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp, modulate_resonance );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF * cutoff_smoother.tick( amp, modulate_cutoff ) + MIN_CUTOFF) * (1.0f/8);
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct LPExecuter : public FilterExecuterBase {
                DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(LPF);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                    const float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateLowResonance( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid], tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION( filter.processLowResonance( DISTORTION( input_buffer[sid] ) ) );
                    }
                }
                LPExecuter(FilterProcessor*const processor_,
                           int num_samples__,
                           int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new LPExecuter( this, num_samples, 0 );
            executer_2 = new LPExecuter( this, num_samples, 1 );
            executer_3 = new LPExecuter( this, num_samples, 2 );
        }
        break;
        case HIGH_2_PASS :
        {
            // PREPARE
            {
                float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp, modulate_resonance );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF*2.0f) * cutoff_smoother.tick( amp, modulate_cutoff ) + MIN_CUTOFF;
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct HP2PassExecuter : public FilterExecuterBase {
                 DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(HIGH_2_PASS);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                    const float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateHigh2Pass( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION( filter.processHigh2Pass( DISTORTION( input_buffer[sid] ) ) );
                    }
                }
                HP2PassExecuter(FilterProcessor*const processor_,
                                int num_samples__,
                                int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new HP2PassExecuter( this, num_samples, 0 );
            executer_2 = new HP2PassExecuter( this, num_samples, 1 );
            executer_3 = new HP2PassExecuter( this, num_samples, 2 );
        }
        break;
        case HPF :
        {
            // PREPARE
            {
                float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp, modulate_resonance );
                    tmp_cuttof_buffer[sid] = (MAX_CUTOFF*2.0f) * cutoff_smoother.tick( amp, modulate_cutoff ) + MIN_CUTOFF;
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct HPExecuter : public FilterExecuterBase {
                DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(HPF);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                    const float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateHighResonance( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION( filter.processHighResonance( DISTORTION( input_buffer[sid] ) ) );
                    }
                }
                HPExecuter(FilterProcessor*const processor_,
                           int num_samples__,
                           int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new HPExecuter( this, num_samples, 0 );
            executer_2 = new HPExecuter( this, num_samples, 1 );
            executer_3 = new HPExecuter( this, num_samples, 2 );
        }
        break;
        case BPF:
        {
            // PREPARE
            {
                float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    tmp_resonance_buffer[sid] = resonance_smoother.tick( amp, modulate_resonance );
                    tmp_cuttof_buffer[sid] = MAX_CUTOFF * cutoff_smoother.tick( amp, modulate_cutoff ) + MIN_CUTOFF;
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct BandExecuter : public FilterExecuterBase {
                DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(BPF);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_resonance_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_RESONANCE);
                    const float*const tmp_cuttof_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_CUTOFF);
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        filter.updateBand( tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]+35, tmp_gain_buffer[sid] );
                        out_buffer[sid] = DISTORTION( filter.processBand( DISTORTION( input_buffer[sid] ) ) );
                    }
                }
                BandExecuter(FilterProcessor*const processor_,
                             int num_samples__,
                             int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new BandExecuter( this, num_samples, 0 );
            executer_2 = new BandExecuter( this, num_samples, 1 );
            executer_3 = new BandExecuter( this, num_samples, 2 );
        }
        break;
        default:  /* PASS */
        {
            // PREPARE
            {
                float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                for( int sid = 0 ; sid != num_samples ; ++sid )
                {
                    float amp = amp_mix[sid];
                    resonance_smoother.tick( amp, modulate_resonance );
                    cutoff_smoother.tick( amp, modulate_cutoff );
                    tmp_gain_buffer[sid] = gain_smoother.tick( amp, modulate_gain );
                    tmp_distortion_buffer[sid] = distortion_smoother.tick( modulate_distortion ? amp : 0 );
                }
            }

            // PROCESSOR
            struct PassExecuter : public FilterExecuterBase {
                DoubleAnalogFilter& filter;
                const int processor_id;
                const int input_id;
                const int num_samples_;
                inline void exec() noexcept override
                {
                    filter.update_filter_to(PASS);

                    DataBuffer& data_buffer( DATA(data_buffer) );
                    const float*const tmp_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_GAIN);
                    const float*const tmp_distortion_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(DIMENSION_DISTORTION);

                    const float* input_buffer = data_buffer.tmp_buffer_6.getWritePointer( input_id );
                    float* out_buffer = data_buffer.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * processor_id );
                    for( int sid = 0 ; sid != num_samples_ ; ++sid )
                    {
                        const float filter_distortion = tmp_distortion_buffer[sid];
                        out_buffer[sid] = filter.processPass( DISTORTION( DISTORTION( input_buffer[sid] ) ) * 2 *tmp_gain_buffer[sid] );
                    }
                }
                PassExecuter(FilterProcessor*const processor_,
                             int num_samples__,
                             int input_id_)
                    : filter(processor_->double_filter[input_id]),
                      processor_id( input_id ),
                      input_id(input_id_),
                      num_samples_(num_samples__)
                {}
            };
            executer_1 = new PassExecuter( this, num_samples, 0 );
            executer_2 = new PassExecuter( this, num_samples, 1 );
            executer_3 = new PassExecuter( this, num_samples, 2 );
        }
        break;
        }

        // RUN
        executer_1->try_run_paralel();
        executer_2->exec();
        executer_3->exec();
        while( executer_1->isWorking() ) {}

        delete executer_1;
        delete executer_2;
        delete executer_3;
    }

    // COLLECT RESULTS
    clipping_smoother.update( glide_motor_time );
    float*const this_direct_output_buffer = data_buffer.direct_filter_output_samples.getWritePointer(id);
    {
        ValueSmootherModulatedUpdater u_output( &output_smoother, glide_motor_time );
        const bool modulate_output = filter_data.modulate_output;
        for( int sid = 0 ; sid != num_samples ; ++sid )
        {
            // OUTPUT MIX AND DISTORTION
            {
                float amp = output_smoother.tick( modulate_output ? amp_mix[sid] : 0 ) ;

                this_direct_output_buffer[sid] = out_buffers[0][sid] * input_ar_amps[0][sid];
                this_direct_output_buffer[sid] += out_buffers[1][sid] * input_ar_amps[1][sid];
                this_direct_output_buffer[sid] += out_buffers[2][sid] * input_ar_amps[2][sid];
                this_direct_output_buffer[sid] *= amp;

                const float clipping = clipping_smoother.tick();
                this_direct_output_buffer[sid] = ( this_direct_output_buffer[sid]*(1.0f-clipping) + soft_clipping(this_direct_output_buffer[sid])*clipping );
            }
        }
    }

    // OUT FOR USE AS INPUT IN FLT 2 & 3
    if( id != FILTER_3 ) // NO NEED TO STORE THE THIRD BUFFER
    {
        for( int sid = 0 ; sid != num_samples ; ++sid )
        {
            out_buffers[0][sid] = soft_clipping(out_buffers[0][sid]);
            out_buffers[1][sid] = soft_clipping(out_buffers[1][sid]);
            out_buffers[2][sid] = soft_clipping(out_buffers[2][sid]);
        }
    }

    // COMPRESS
    {
        if( id == FILTER_1 )
            compress( this_direct_output_buffer, data_buffer.tmp_buffer_6.getWritePointer(), this_direct_output_buffer,
            filter_data.compressor,
            num_samples );
        else
            compress( this_direct_output_buffer, data_buffer.tmp_buffer_6.getWritePointer(), data_buffer.direct_filter_output_samples.getReadPointer(id-1),
            filter_data.compressor,
            num_samples );
    }

    // VISUALIZE
    if( mono_AmpPainter*const amp_painter = MONOVoice::get_amp_painter() )
    {
        amp_painter->add_filter_env( id, amp_mix, num_samples );
        amp_painter->add_filter( id, this_direct_output_buffer, num_samples );
    }

    // COLLECT THE FINAL OUTPUT
    if( id == FILTER_3 )
    {
        float*const final_filters_output_buffer = data_buffer.direct_filter_output_samples.getWritePointer();
        const float* direct_output_buffer_flt_2 = data_buffer.direct_filter_output_samples.getReadPointer(1);
        for( int sid = 0 ; sid != num_samples ; ++sid )
            final_filters_output_buffer[sid] += (direct_output_buffer_flt_2[sid] + this_direct_output_buffer[sid]);
    }
}

// -----------------------------------------------------------------
inline void FilterProcessor::compress( float* io_buffer_, float* tmp_buffer_, const float* compressor_signal_,
                                       float power,
                                       int num_samples ) noexcept
{
    float use_power = power;
    bool is_negative = false;
    if( use_power < 0 ) {
        use_power*=-1;
        is_negative = true;
    }

    env_follower.setCoefficients( 0.008f * use_power + 0.0001f, 0.005f * use_power + 0.0001f );
    env_follower.processEnvelope( compressor_signal_, tmp_buffer_, num_samples );

    for( int sid = 0 ; sid != num_samples ; ++sid )
    {
        float compression = mono_exp(tmp_buffer_[sid])-1;
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
class EQProcessor : public RuntimeListener {
    float frequency_low_pass[SUM_EQ_BANDS];
    float frequency_high_pass[SUM_EQ_BANDS];

    AmpSmoothBuffer amp_smoother[SUM_EQ_BANDS];
    AnalogFilter filters[SUM_EQ_BANDS];
    IIRFilter low_pass_filters[SUM_EQ_BANDS];
    IIRFilter high_pass_filters[SUM_EQ_BANDS];

    ValueSmoother velocity_smoothers[SUM_EQ_BANDS];

    friend class mono_ParameterOwnerStore;
public:
    OwnedArray< ENV > envs;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void process( int num_samples_ ) noexcept;

    NOINLINE EQProcessor();
    NOINLINE ~EQProcessor();

    NOINLINE void sample_rate_changed( double old_sr_ ) noexcept override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessor)
};

// -----------------------------------------------------------------
NOINLINE EQProcessor::EQProcessor()
    :
    velocity_smoothers
{
    &DATA( eq_data ).velocity[0],
    &DATA( eq_data ).velocity[1],
    &DATA( eq_data ).velocity[2],
    &DATA( eq_data ).velocity[3],
    &DATA( eq_data ).velocity[4],
    &DATA( eq_data ).velocity[5],
    &DATA( eq_data ).velocity[6],
    &DATA( eq_data ).velocity[7],
    &DATA( eq_data ).velocity[8]
}
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        envs.add( new ENV( *DATA( eq_data ).env_datas.getUnchecked( band_id ) ) );

        const float frequency_low_pass_tmp = (62.5f/2) * pow(2,band_id+1);
        frequency_low_pass[band_id] = frequency_low_pass_tmp;
        frequency_high_pass[band_id] = frequency_low_pass_tmp / 2;
    }

    sample_rate_changed(0);
}
NOINLINE EQProcessor::~EQProcessor() {}

// -----------------------------------------------------------------
inline void EQProcessor::start_attack() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        envs.getUnchecked(band_id)->start_attack();
}
inline void EQProcessor::start_release() noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        envs.getUnchecked(band_id)->set_to_release();
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
#define IN_DIMENSION 4*band_id+0
#define OUT_DIMENSION 4*band_id+1
#define GAIN_DIMENSION 4*band_id+2
#define ENV_DIMENSION 4*band_id+3

        struct BandExecuter : public mono_Thread {
            EQProcessor*const processor;
            const int num_samples_;
            const int band_id;
            inline void exec() noexcept override
            {
                DataBuffer& data_buffer( DATA( data_buffer ) );
                const float*const io_buffer = data_buffer.direct_filter_output_samples.getReadPointer();
                const int glide_motor_time = DATA( synth_data ).glide_motor_time;

                const float resonance = 0 ;

                const bool hold_sustain = DATA( eq_data ).hold[band_id];

                // WORKING BUFFERS
                float*const tmp_band_in_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(IN_DIMENSION);
                float*const tmp_band_out_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(OUT_DIMENSION);
                float*const tmp_band_sum_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(GAIN_DIMENSION);
                float*const tmp_env_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(ENV_DIMENSION);

                // PROCESS 100% BAND
                {
                    processor->low_pass_filters[band_id].processSamples( tmp_band_in_buffer, io_buffer, num_samples_ );
                    processor->high_pass_filters[band_id].processSamples( tmp_band_in_buffer, num_samples_ );
                }

                // PROCESS
                {
                    ValueSmoother& velocity_smoother = processor->velocity_smoothers[band_id];
                    velocity_smoother.update( glide_motor_time );
                    {
                        // ENV OR SUSTAIN
                        hold_sustain ? processor->envs[band_id]->reset() : processor->envs[band_id]->process( tmp_env_buffer, num_samples_ );

                        AnalogFilter& filter = processor->filters[band_id];
                        const float filter_frequency = processor->frequency_low_pass[band_id];
                        for( int sid = 0 ; sid != num_samples_ ; ++sid )
                        {
                            const float sustain = velocity_smoother.tick();
                            const float amp( hold_sustain ? processor->amp_smoother[band_id].add_and_get_average( positive(sustain) ) : processor->amp_smoother[band_id].add_and_get_average( tmp_env_buffer[sid] ) );

                            // UPDATE FILTER
                            filter.set( 0.2f*resonance, filter_frequency, amp );

                            // PROCESS
                            {
                                const float gain = sustain < 0 ? 1.0f-amp : 1.0f+5.0f*amp;
                                tmp_band_sum_gain_buffer[sid] = (gain > 1 ? (amp) : 0);
                                const float output = filter.processLow( tmp_band_in_buffer[sid] );

                                // SHAPER
#define FIXED_K 2.0f*0.7f/(1.0f-0.7f)
                                tmp_band_out_buffer[sid] = ( output*(1.0f-resonance) + ( (1.0f+FIXED_K)*output/(1.0f+FIXED_K*std::abs(output)) * (0.5f - 0.1f*resonance))*resonance )*gain;
                            }
                        }
                    }
                }
            }
            BandExecuter(EQProcessor*const processor_,
                         int num_samples__,
                         int band_id_)
                : processor(processor_),
                  num_samples_(num_samples__),
                  band_id(band_id_)
            {}
        };

        BandExecuter* executers[SUM_EQ_BANDS];
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            BandExecuter* executer = new BandExecuter(this, num_samples_, band_id);
            executer->try_run_paralel();
            executers[band_id] = executer;
        }
        while(
            executers[0]->isWorking()
            || executers[1]->isWorking()
            || executers[2]->isWorking()
            || executers[3]->isWorking()
            || executers[4]->isWorking()
            || executers[5]->isWorking()
            || executers[6]->isWorking()
            || executers[7]->isWorking()
            || executers[8]->isWorking()
        ) {}

        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id ) {
            delete executers[band_id];
        }
    }
    // EO MULTITHREADED

    // FINAL MIX - SINGLE THREADED ( NO REALY OPTIMIZED )
    {
        DataBuffer& data_buffer( DATA( data_buffer ) );
        float*const io_buffer = data_buffer.direct_filter_output_samples.getWritePointer();
        float*const tmp_all_band_out_buffer = data_buffer.tmp_buffer_6.getWritePointer(0);
        float*const tmp_all_band_sum_gain_buffer = data_buffer.tmp_buffer_6.getWritePointer(1);
        FloatVectorOperations::fill(tmp_all_band_out_buffer,0,num_samples_);
        FloatVectorOperations::fill(tmp_all_band_sum_gain_buffer,1,num_samples_);
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
        {
            const float*const tmp_band_out_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(OUT_DIMENSION);
            const float*const tmp_band_sum_gain_buffer = data_buffer.tmp_multithread_band_buffer_9_4.getWritePointer(GAIN_DIMENSION);

            for( int sid = 0 ; sid != num_samples_ ; ++sid )
            {
                tmp_all_band_out_buffer[sid] += tmp_band_out_buffer[sid];
                tmp_all_band_sum_gain_buffer[sid] += tmp_band_sum_gain_buffer[sid];
            }
        }
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            io_buffer[sid] = tmp_all_band_out_buffer[sid] / (tmp_all_band_sum_gain_buffer[sid]);
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
    int index[2];
    float last_delay[2];
    float* buffer[2];

public:
    inline void fill(int channel_, float sample_) noexcept;
    inline float tick( int channel_, float delay_ ) noexcept;

private:
    NOINLINE void sample_rate_changed( double ) noexcept override;

public:
    NOINLINE Chorus();
    NOINLINE ~Chorus();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chorus)
};

//==============================================================================
NOINLINE Chorus::Chorus() : buffer_size(0)
{
    index[0] = 0;
    index[1] = 0;
    last_delay[0] = 220;
    last_delay[1] = 200;

    buffer[0] = nullptr;
    buffer[1] = nullptr;

    sample_rate_changed(0);
}
NOINLINE Chorus::~Chorus()
{
    delete [] buffer[0];
    delete [] buffer[1];
}

//==============================================================================
inline void Chorus::fill(int channel_, float sample_) noexcept
{
    index[channel_] = index[channel_] % buffer_size;
    buffer[channel_][index[channel_]] = sample_;
    index[channel_]++;
}
#define DELAY_GLIDE 0.01f
inline float Chorus::tick( int channel_, float delay_ ) noexcept
{
    if( delay_ < last_delay[channel_] - DELAY_GLIDE)
        delay_ = last_delay[channel_] - DELAY_GLIDE;
    else if( delay_ > last_delay[channel_] + DELAY_GLIDE )
        delay_ = last_delay[channel_] + DELAY_GLIDE;

    last_delay[channel_] = delay_;

    float i = float(index[channel_]) - delay_;
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
    return buffer[channel_][ib]*delta + buffer[channel_][ia]*(1.0f-delta);
}
//==============================================================================
NOINLINE void Chorus::sample_rate_changed( double ) noexcept {
    buffer_size = sample_rate/10;
    if( buffer[0] )
        delete[] buffer[0];
    if( buffer[1] )
        delete[] buffer[1];
    buffer[0] = new float[buffer_size];
    buffer[1] = new float[buffer_size];
    for( int i = 0 ; i != buffer_size ; ++i ) {
        buffer[0][i] = 0;
        buffer[1][i] = 0;
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

    NOINLINE void setSize (const int size);
    NOINLINE void clear() noexcept;

    NOINLINE CombFilter();
    NOINLINE ~CombFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CombFilter)
};

//==============================================================================
NOINLINE CombFilter::CombFilter()
    :
    last(0),
    bufferSize (0),
    bufferIndex(0)
{}
NOINLINE CombFilter::~CombFilter() {}

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
NOINLINE void CombFilter::setSize (const int size)
{
    if (size != bufferSize)
    {
        bufferIndex = 0;
        buffer.malloc ((size_t) size);
        bufferSize = size;
    }

    clear();
}
NOINLINE void CombFilter::clear() noexcept
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

    NOINLINE void setSize (const int size);
    NOINLINE void clear() noexcept;

    NOINLINE AllPassFilter();
    NOINLINE ~AllPassFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllPassFilter)
};

//==============================================================================
NOINLINE AllPassFilter::AllPassFilter()
    :
    bufferSize(0),
    bufferIndex(0)
{}
NOINLINE AllPassFilter::~AllPassFilter() {}

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
NOINLINE void AllPassFilter::setSize (const int size)
{
    if (size != bufferSize)
    {
        bufferIndex = 0;
        buffer.malloc ((size_t) size);
        bufferSize = size;
    }

    clear();
}
NOINLINE void AllPassFilter::clear() noexcept
{
    buffer.clear ((size_t) bufferSize);
}

//==============================================================================
//==============================================================================
//==============================================================================
class LinearSmoothedValue
{
    float currentValue, target, step;
    int countdown, stepsToTarget;

public:
    inline float getNextValue() noexcept;
    inline void setValue (float newValue) noexcept;

    NOINLINE void reset (float sampleRate, float fadeLengthSeconds) noexcept;

    NOINLINE LinearSmoothedValue();
    NOINLINE ~LinearSmoothedValue();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinearSmoothedValue)
};

//==============================================================================
NOINLINE LinearSmoothedValue::LinearSmoothedValue()
    :
    currentValue(0),
    target(0),
    step(0),
    countdown(0),
    stepsToTarget(0)
{
}
NOINLINE LinearSmoothedValue::~LinearSmoothedValue() {}

//==============================================================================
inline float LinearSmoothedValue::getNextValue() noexcept
{
    float value;
    if (countdown <= 0)
    {
        value = target;
    }
    else
    {
        --countdown;
        currentValue += step;

        value = currentValue;
    }

    return value;
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
NOINLINE void LinearSmoothedValue::reset (float sampleRate, float fadeLengthSeconds) noexcept
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
    friend class mono_Reverb;
    NOINLINE ReverbParameters();
    NOINLINE ~ReverbParameters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbParameters)
};

//==============================================================================
NOINLINE ReverbParameters::ReverbParameters()
    :
    roomSize   (0.5f),
    wetLevel   (0.33f),
    dryLevel   (0.4f),
    width      (1.0f)
{}
NOINLINE ReverbParameters::~ReverbParameters() {}

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Reverb : RuntimeListener
{
    enum { numCombs = 8, numAllPasses = 4, numChannels = 2 };

    CombFilter comb [numChannels][numCombs];
    AllPassFilter allPass [numChannels][numAllPasses];

    ReverbParameters parameters;
#define REVERB_GAIN 0.013f

    LinearSmoothedValue feedback, dryGain, wetGain1, wetGain2;

public:
    inline void processSingleSampleRawStereo_noDamp ( float& input_l, float& input_r ) noexcept;

    inline ReverbParameters& get_parameters() noexcept;
    inline void update_parameters() noexcept;

    NOINLINE void sample_rate_changed( double /* old_sr_ */ ) noexcept override;
    NOINLINE void reset();

    NOINLINE mono_Reverb();
    NOINLINE ~mono_Reverb();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_Reverb)
};

//==============================================================================
NOINLINE mono_Reverb::mono_Reverb()
{
    update_parameters();
    sample_rate_changed(0);
}
NOINLINE mono_Reverb::~mono_Reverb() {}

//==============================================================================
inline void mono_Reverb::processSingleSampleRawStereo_noDamp ( float& input_l, float& input_r ) noexcept
{
    float outL = 0, outR = 0;
    {
        const float input = (input_l + input_r) * REVERB_GAIN;
        const float feedbck = feedback.getNextValue();
        for (int j = 0; j != numCombs; ++j)  // accumulate the comb filters in parallel
        {
            outL += comb[LEFT][j].process (input, feedbck);
            outR += comb[RIGHT][j].process (input, feedbck);
        }
    }
    for (int j = 0; j != numAllPasses; ++j)  // run the allpass filters in series
    {
        outL = allPass[LEFT][j].process (outL);
        outR = allPass[RIGHT][j].process (outR);
    }

    const float dry  = dryGain.getNextValue();
    const float wet1 = wetGain1.getNextValue();
    const float wet2 = wetGain2.getNextValue();
    input_l  = outL * wet1 + outR * wet2 + input_l  * dry;
    input_r = outR * wet1 + outL * wet2 + input_r * dry;
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
NOINLINE void mono_Reverb::sample_rate_changed (double) noexcept
{
    static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
    static const short allPassTunings[] = { 556, 441, 341, 225 };
    const int stereoSpread = 23;
    const int intSampleRate = (int) sample_rate;

    for (int i = 0; i < numCombs; ++i)
    {
        comb[LEFT][i].setSize ((intSampleRate * combTunings[i]) / 44100);
        comb[RIGHT][i].setSize ((intSampleRate * (combTunings[i] + stereoSpread)) / 44100);
    }

    for (int i = 0; i < numAllPasses; ++i)
    {
        allPass[LEFT][i].setSize ((intSampleRate * allPassTunings[i]) / 44100);
        allPass[RIGHT][i].setSize ((intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
    }

    const float smoothTime = 0.01f;
    //damping .reset (sampleRate, smoothTime);
    feedback.reset (sample_rate, smoothTime);
    dryGain .reset (sample_rate, smoothTime);
    wetGain1.reset (sample_rate, smoothTime);
    wetGain2.reset (sample_rate, smoothTime);
}
NOINLINE void mono_Reverb::reset()
{
    for (int j = 0; j < numChannels; ++j)
    {
        for (int i = 0; i < numCombs; ++i)
            comb[j][i].clear();

        for (int i = 0; i < numAllPasses; ++i)
            allPass[j][i].clear();
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
class FXProcessor
{
    // REVERB
    mono_Reverb reverb;

    // CHORUS
    Chorus chorus;
    AmpSmoothBuffer chorus_smoother;
    ValueSmoother chorus_mod_smoother;
    friend class mono_ParameterOwnerStore;
    ScopedPointer< ENV > chorus_modulation_env;

    // DELAY
    int delayPosition;
    AudioSampleBuffer delayBuffer;
    ValueSmoother delay_smoother;

    // FINAL ENV
    friend class MONOVoice;
    ScopedPointer<ENV> final_env;
    // TODO change to a real glide value
    AmpSmoother<100> velocity_glide;

    ValueSmoother bypass_smoother;
    ValueSmoother volume_smoother;
    ValueSmoother clipping_smoother;

public:
    inline void process( AudioSampleBuffer& output_buffer_, const int start_sample_final_out_, const int num_samples_ ) noexcept;

    void start_attack() noexcept {
        chorus_modulation_env->start_attack();
        final_env->start_attack();
    }
    void start_release() noexcept {
        chorus_modulation_env->set_to_release();
        final_env->set_to_release();
    }

    // TODO RESET
    // delayBuffer.clear();
    // reset chorus

public:
    NOINLINE FXProcessor();
    NOINLINE ~FXProcessor();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FXProcessor)
};

// -----------------------------------------------------------------
NOINLINE FXProcessor::FXProcessor()
    :
    reverb(),

    chorus(),
    chorus_smoother(),
    chorus_mod_smoother( &DATA( chorus_data ).modulation ),
    chorus_modulation_env( new ENV( *(DATA( chorus_data ).modulation_env_data.get() )) ),

    delayPosition( 0 ),
    delayBuffer (2, 12000),
    delay_smoother( &DATA( synth_data ).delay ),

    final_env( new ENV( DATA( env_datas[MAIN_ENV] ) ) ),
    velocity_glide(0),

    bypass_smoother( &DATA( synth_data ).effect_bypass ),
    volume_smoother( &DATA( synth_data ).volume ),
    clipping_smoother( &DATA( synth_data ).final_compression )
{}

NOINLINE FXProcessor::~FXProcessor() {}

// -----------------------------------------------------------------
inline void FXProcessor::process( AudioSampleBuffer& output_buffer_, const int start_sample_final_out_, const int num_samples_ ) noexcept
{
    DataBuffer& data_buffer( DATA(data_buffer) );
    //const int glide_motor_time = msToSamplesFast(DATA( synth_data ).glide_motor_time,44100);
    const int glide_motor_time = DATA( synth_data ).glide_motor_time;

    // COLLECT BUFFERS
    const float* input_buffer = data_buffer.direct_filter_output_samples.getReadPointer();

    // PREPARE REVERB
    ReverbData& reverb_data = DATA( reverb_data );
    const float reverb_room = reverb_data.room;
    const float reverb_dry_wet_mix = reverb_data.dry_wet_mix;
    const float reverb_width = reverb_data.width;
    ReverbParameters& current_params = reverb.get_parameters();
    if(
        current_params.roomSize != reverb_room
        || current_params.dryLevel != reverb_dry_wet_mix
        // current_params.wetLevel != r_params.wetLevel
        || current_params.width != reverb_width
    )
    {
        current_params.roomSize = reverb_room;
        current_params.dryLevel = reverb_dry_wet_mix;
        current_params.wetLevel = 1.0f-reverb_dry_wet_mix;
        current_params.width = reverb_width;

        reverb.update_parameters();
    }

    // PRPARE CHORUS
    float* tmp_chorus_amp_buffer = data_buffer.tmp_buffer_6.getWritePointer(0);
    // TODO only process amp if chorus amp is active
    chorus_modulation_env->process( tmp_chorus_amp_buffer, num_samples_ );
    ChorusData& chorus_data = DATA( chorus_data );
    const bool is_chorus_amp_fix = chorus_data.hold_modulation;
    chorus_mod_smoother.update( glide_motor_time );

    // PREPARE DELAY
    SynthData& synth_data = DATA( synth_data );
    delay_smoother.update( glide_motor_time );
    float* delay_data_l = delayBuffer.getWritePointer (LEFT);
    float* delay_data_r = delayBuffer.getWritePointer (RIGHT);

    // PREPARE FX BYPASS
    bypass_smoother.update( glide_motor_time );

    // PREPARE FINAL ENV
    float* tmp_env_amp = data_buffer.tmp_buffer_6.getWritePointer(1);
    final_env->process( tmp_env_amp, num_samples_ );
    volume_smoother.update( glide_motor_time );
    clipping_smoother.update( glide_motor_time );

    // NOTE depend on the num output channels
    float* final_l_output = output_buffer_.getWritePointer(LEFT);
    float* final_r_output = output_buffer_.getWritePointer(RIGHT);

    // PROCESS IN ONE RUN
    for( int sid = 0 ; sid != num_samples_ ; ++sid )
    {
        float tmp_l;
        float tmp_r;

        // FINAL AMP
        const float in_l_r = input_buffer[sid] * tmp_env_amp[sid]*volume_smoother.tick()*2; // TODO add back velocity glide

        // CHORUS
        {
            const float chorus_modulation = chorus_mod_smoother.tick();
            // TODO do we need the chorus smoother?
            const float modulation_amp = chorus_smoother.add_and_get_average( is_chorus_amp_fix ? chorus_modulation : tmp_chorus_amp_buffer[sid] );

            tmp_l = chorus.tick( LEFT, (modulation_amp * 220) + 0.0015f);
            tmp_r = chorus.tick( RIGHT, (modulation_amp * 200) + 0.002f);

            const float feedback = modulation_amp*0.85f;

            chorus.fill( LEFT, in_l_r + tmp_r * feedback );
            chorus.fill( RIGHT, in_l_r + tmp_l * feedback );
        }

        // REVERB
        {
            reverb.processSingleSampleRawStereo_noDamp
            (
                tmp_l,
                tmp_r
            );
        }

        // DELAY
        {
            const float delay = delay_smoother.tick();

            const float in_l = tmp_l;
            const float in_r = tmp_r;

            const float delay_data_l_in = delay_data_l[delayPosition];
            const float delay_data_r_in = delay_data_r[delayPosition];

            tmp_l += delay_data_l_in;
            tmp_r += delay_data_r_in;

            delay_data_l[delayPosition] = (delay_data_l_in + in_l) * delay;
            delay_data_r[delayPosition] = (delay_data_r_in + in_r) * delay;

            if (++delayPosition >= delayBuffer.getNumSamples())
                delayPosition = 0;
        }

        // BYPASS -> MIX
        {
            const float bypass = bypass_smoother.tick();
            tmp_l = tmp_l*bypass + in_l_r*(1.0f-bypass);
            tmp_r = tmp_r*bypass + in_l_r*(1.0f-bypass);

            const float clipping = clipping_smoother.tick();
            final_l_output[start_sample_final_out_+sid] = ( soft_clipping( tmp_l )*clipping + tmp_l*(1.0f-clipping) );
            final_r_output[start_sample_final_out_+sid] = ( soft_clipping( tmp_r )*clipping + tmp_r*(1.0f-clipping) );
        }
    }

    // VISUALIZE
    if( mono_AmpPainter* amp_painter = MONOVoice::get_amp_painter() )
    {
        amp_painter->add_out( &final_l_output[start_sample_final_out_], num_samples_ );
        amp_painter->add_out_env( tmp_env_amp, num_samples_ );
    }
}



//==============================================================================
bool MonoSynthSound::appliesToNote(int) {
    return true;
}
bool MonoSynthSound::appliesToChannel(int) {
    return true;
}
NOINLINE MonoSynthSound::MonoSynthSound() {}

NOINLINE MonoSynthSound::~MonoSynthSound() {}

//==============================================================================
class ArpSequencer : public RuntimeListener {
    RuntimeInfo& info;
    const ArpSequencerData& data;

    int current_step;
    int next_step_on_hold;
    int step_at_sample_current_buffer;

    int shuffle_to_back_counter;
    bool found_a_step;

public:
    // RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
    inline int process_samples_to_next_step( int start_sample_, int num_samples_ ) noexcept;

    inline int get_current_step() const noexcept;
    inline int get_step_before() const noexcept;
    inline float get_current_tune() const noexcept;
    inline bool is_a_step() const noexcept;
    inline bool should_start() const noexcept;
    inline void reset() noexcept;

    NOINLINE ArpSequencer( RuntimeInfo& info_, const ArpSequencerData& data_ );
    NOINLINE ~ArpSequencer();
};
NOINLINE ArpSequencer::ArpSequencer( RuntimeInfo& info_, const ArpSequencerData& data_ )
    :
    info(info_),
    data(data_),

    current_step(0),
    next_step_on_hold(0),
    step_at_sample_current_buffer(-1),

    shuffle_to_back_counter(0),
    found_a_step(false)
{}
NOINLINE ArpSequencer::~ArpSequencer() {}

// RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
// RETURN NUM SAMPLES IF THERE IS NO STEP IN THE BUFFER
inline int ArpSequencer::process_samples_to_next_step( int start_sample_, int num_samples_ ) noexcept {
    double samples_per_min = sample_rate*60.0;
    double speed_multi;
    switch( data.speed_multi ) {
    case _XNORM :
        speed_multi = 1;
        break;
    case _X2 :
        speed_multi = 2;
        break;
    case _X05 :
        speed_multi = 0.5;
        break;
    case _X4 :
        speed_multi = 4;
        break;
    case _X025 :
        speed_multi = 0.25;
        break;
    default /* _X3 */ :
        speed_multi = 3;
        break;
    }
    double steps_per_min = info.bpm*4.0/1.0 * speed_multi;
    double steps_per_sample = steps_per_min/samples_per_min;
    double samples_per_step = samples_per_min/steps_per_min;
    int64 sync_sample_pos = info.samples_since_start+start_sample_;
    int64 step = next_step_on_hold;
    step_at_sample_current_buffer = -1;
    for( int i = 0 ; i < num_samples_; ++i )
    {
        /*
          static int profile_samples = 44100*10;
          profile_samples--;
          std::cout << profile_samples << std::endl;
          if( profile_samples == 0 )
              exit(0);
        */



#ifdef IS_STANDALONE
        if( info.is_extern_synced )
        {
            if( info.next_step_at_sample.size() )
            {
                if( info.next_step_at_sample.getFirst() == start_sample_+i )
                {
                    step = info.next_step.remove(0);
                    info.next_step_at_sample.remove(0);
                }
            }
        }
        else
#endif
        {
            step = std::floor(steps_per_sample*double(sync_sample_pos))+1; // +1 for future processing
        }

        --shuffle_to_back_counter;

        if ( step != next_step_on_hold )
        {
            next_step_on_hold = step;

            if( current_step % 2 == 0 )
                shuffle_to_back_counter = std::floor(samples_per_step*0.8f * data.shuffle);
            else
                shuffle_to_back_counter = 0;

            found_a_step = true;
        }

        if( found_a_step && shuffle_to_back_counter == 0 )
        {
            found_a_step = false;
            step_at_sample_current_buffer = i;
            current_step = next_step_on_hold;
            return i;
        }

        sync_sample_pos++;
    }

    return num_samples_;
    //return num_samples_-start_sample_;
}
inline int ArpSequencer::get_current_step() const noexcept {
    return current_step % SUM_ENV_ARP_STEPS;
}
inline int ArpSequencer::get_step_before() const noexcept {
    if( current_step > 0 )
        return (current_step-1) % SUM_ENV_ARP_STEPS;
    else
        return 0;
}
inline float ArpSequencer::get_current_tune() const noexcept {
    return data.tune[get_current_step()];
}
inline bool ArpSequencer::is_a_step() const noexcept {
    return step_at_sample_current_buffer != -1;
}
inline bool ArpSequencer::should_start() const noexcept {
    if( is_a_step() )
        if( data.step[get_current_step()] )
            return true;

    return false;
}
inline void ArpSequencer::reset() noexcept {
    current_step = 0;
}
//==============================================================================
NOINLINE MONOVoice::MONOVoice( GstepAudioProcessor*const audio_processor_ )
    :
    audio_processor( audio_processor_ ),
    synth_data( *audio_processor_->synth_data ),

    data_buffer( new DataBuffer(512) ),

    arp_sequencer( new ArpSequencer( info, *synth_data.arp_sequencer_data ) ),

    eq_processor( new EQProcessor() ),
    fx_processor( new FXProcessor() ),

    is_stopped( true ),
    was_arp_started(false),

    current_note(-1),
    current_velocity(0),

    current_step(0)
{
    mono_ParameterOwnerStore::getInstance()->data_buffer = data_buffer;
    mono_ParameterOwnerStore::getInstance()->runtime_info = &info;


    for( int i = 0 ; i != SUM_OSCS ; ++i )
        oscs.add( new OSC( i ) );

    for( int i = 0 ; i != SUM_LFOS ; ++i )
        lfos.add( new LFO( i ) );

    for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
        filter_processors.add( new FilterProcessor( i ));
        filter_envs.add( new ENV( DATA( env_datas[i] ) ) );
    }

    mono_ParameterOwnerStore::getInstance()->voice = this;
}

NOINLINE MONOVoice::~MONOVoice() {
    mono_ParameterOwnerStore::getInstance()->voice = nullptr;
}

void MONOVoice::create_amp_painter() {
    if(!AppInstanceStore::getInstance()->ampPainter)
        AppInstanceStore::getInstance()->ampPainter = new mono_AmpPainter();

    AppInstanceStore::getInstance()->try_to_kill_amp_painter = false;
}
mono_AmpPainter* MONOVoice::get_amp_painter() {
    if( !AppInstanceStore::getInstance()->try_to_kill_amp_painter )
        return AppInstanceStore::getInstance()->ampPainter;
    else
        return nullptr;
}
mono_AmpPainter* MONOVoice::get_lock_amp_painter() {
    if( !AppInstanceStore::getInstance()->try_to_kill_amp_painter )
    {
        AppInstanceStore::getInstance()->amp_painter_lock.enter();
        return AppInstanceStore::getInstance()->ampPainter;
    }
    else
        return nullptr;
}
void MONOVoice::unlock_amp_painter() {
    AppInstanceStore::getInstance()->amp_painter_lock.exit();
}
void MONOVoice::kill_amp_painter() {
    AppInstanceStore::getInstance()->try_to_kill_amp_painter = true;
    ScopedLock locked(AppInstanceStore::getInstance()->amp_painter_lock);

    if(AppInstanceStore::getInstance()->ampPainter)
    {
        AppInstanceStore::getInstance()->ampPainter->stopTimer();
        AppInstanceStore::getInstance()->ampPainter = nullptr;
    }
}

void MONOVoice::startNote( int midi_note_number_, float velocity_, SynthesiserSound* /*sound*/, int pitch_ )
{
    start_internal( midi_note_number_, velocity_ );
    is_stopped = false;
}
void MONOVoice::start_internal( int midi_note_number_, float velocity_ ) noexcept {
    current_note = midi_note_number_;
    current_velocity = velocity_;

    // OSCS
    bool is_arp = synth_data.arp_sequencer_data->is_on;
    float arp_offset = is_arp ? arp_sequencer->get_current_tune() : 0;
    for( int i = 0 ; i != SUM_OSCS ; ++i )
        oscs[i]->reset( current_note+arp_offset );

    // LFOS
    if( !is_arp || !current_note )
    {
        for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id ) {
            filter_envs[voice_id]->start_attack();
            filter_processors[voice_id]->start_attack();
        }
        eq_processor->start_attack();
        fx_processor->start_attack();
    }

}
void MONOVoice::stopNote ( float, bool allowTailOff ) {
    //bool other_notes_active = audio_processor->synth.keys_down.size();
    /*
    if( other_notes_active && allowTailOff )
    {
        // RESTART AN OLDER NOTe
        start_internal( audio_processor->synth.keys_down.getFirst(),
                        audio_processor->synth.velocitys_down.getFirst() );
    }
    // ARPEGGIATE
    else
      */
    if( synth_data.arp_sequencer_data->is_on && current_note && allowTailOff )
        ;
    else // STOP
    {
        if( allowTailOff ) {
            for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id ) {
                filter_envs[voice_id]->set_to_release();
                filter_processors[voice_id]->start_release();
            }
            eq_processor->start_release();
            fx_processor->start_release();
        }
        else
        {
            is_stopped = true;
            clearCurrentNote();
        }
    }
}
int MONOVoice::getCurrentlyPlayingNote() const noexcept {
    return current_note;
}
// TODO must be done earlyer if the env is ended
void MONOVoice::release_if_inactive() noexcept {
    // TODO, reset filters here!
    if( fx_processor->final_env->current_stage == END_ENV ) {
        clearCurrentNote();
        is_stopped = true;
    }
}

void MONOVoice::renderNextBlock ( AudioSampleBuffer& output_buffer_, int start_sample_, int num_samples_ )
{
    if( synth_data.arp_sequencer_data->is_on && current_note != -1 )
        ;
    else if( is_stopped )
    {
        return;
    }

    // GET POSITION INFOS
    info.bpm = synth_data.sync ? audio_processor->pos.bpm : synth_data.speed;
    info.samples_since_start = audio_processor->pos.timeInSamples;

    int count_start_sample = start_sample_;
    int num_samples = num_samples_;
    bool is_a_step = false;
    while( num_samples != 0 )
    {
        int samples_to_next_step_in_buffer = arp_sequencer->process_samples_to_next_step( count_start_sample, num_samples );
        num_samples -= samples_to_next_step_in_buffer;

        render_block( output_buffer_, is_a_step ? arp_sequencer->get_current_step() : -1, count_start_sample, samples_to_next_step_in_buffer );
        count_start_sample += samples_to_next_step_in_buffer;

        const bool connect = synth_data.arp_sequencer_data->connect;
        const bool restart = arp_sequencer->should_start() && synth_data.arp_sequencer_data->is_on;
        is_a_step = arp_sequencer->is_a_step();
        if( was_arp_started && arp_sequencer->is_a_step() ) {
            if( restart && connect )
                ;
            else
            {
                was_arp_started = false;

                // STOP FILTER ENV
                for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
                    filter_envs[i]->set_to_release();
                    filter_processors[i]->start_release();
                }
                eq_processor->start_release();
                fx_processor->start_release();
            }
        }
        if( restart ) {
            if( was_arp_started && connect )
                ;
            else
            {

                was_arp_started = true;

                // TODO add bool for running voices or use was arp started

                // RESTART FILTERS
                for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
                    filter_envs[i]->start_attack();
                    filter_processors[i]->start_attack();
                }
                eq_processor->start_attack();
                fx_processor->start_attack();
            }

            // OSC TUNE
            for( int i = 0 ; i != SUM_OSCS ; ++i ) {
                oscs[i]->reset( current_note+arp_sequencer->get_current_tune() );
            }
        }
    }

    release_if_inactive();
}

float MONOVoice::get_current_frequency() const noexcept {
    return MidiMessage::getMidiNoteInHertz(current_note+arp_sequencer->get_current_tune());
}
void MONOVoice::render_block ( AudioSampleBuffer& output_buffer_, int step_number_, int start_sample_, int num_samples_) {
    mono_AmpPainter* amp_painter = MONOVoice::get_amp_painter();

    const int num_samples = num_samples_;
    if( num_samples == 0 )
        return;

    if( step_number_ != -1 )
        current_step = step_number_;

    // MULTI THREADED FLT_ENV / LFO / OSC
    {
        // MAIN THREAD // NO DEPENCIES
        filter_envs[0]->process( data_buffer->filter_env_amps.getWritePointer(0), num_samples );
        lfos[0]->process( step_number_, num_samples );
        oscs[0]->process( data_buffer, num_samples ); // NEED OSC 0 && LFO 0

        // SUB THREAD
        // DEPENCIES OSC 0
        struct Executer : public mono_Thread {
            MONOVoice*const voice;
            const int num_samples;
            const int step_number;
            const int id;
            void exec() noexcept override {
                voice->filter_envs[id]->process( voice->data_buffer->filter_env_amps.getWritePointer(id), num_samples );
                voice->lfos[id]->process( step_number, num_samples );
                voice->oscs[id]->process( voice->data_buffer, num_samples );
            }
            Executer(int id_,
                     MONOVoice*const voice_,
                     int num_samples_,
                     int step_number_)
                : id(id_),
                  voice(voice_),
                  num_samples(num_samples_),step_number(step_number_) {}
        };
        Executer executer( 1, this, num_samples, step_number_ );
        executer.try_run_paralel();

        filter_envs[2]->process( data_buffer->filter_env_amps.getWritePointer(2), num_samples );
        lfos[2]->process( step_number_, num_samples );
        oscs[2]->process( data_buffer, num_samples ); // NEED OSC 0 && LFO 2

        while( executer.isWorking() ) {}
    }
    /*
    filter_envs[0]->process( data_buffer->filter_env_amps.getWritePointer(0), num_samples );
    lfos[0]->process( step_number_, num_samples );
    oscs[0]->process( data_buffer, num_samples ); // NEED LFO 0

    // THREAD 2
    filter_envs[1]->process( data_buffer->filter_env_amps.getWritePointer(1), num_samples );
    lfos[1]->process( step_number_, num_samples );
    oscs[1]->process( data_buffer, num_samples ); // NEED LFO 1

    // THREAD 3
    filter_envs[2]->process( data_buffer->filter_env_amps.getWritePointer(2), num_samples );
    lfos[2]->process( step_number_, num_samples );
    oscs[2]->process( data_buffer, num_samples ); // NEED LFO 2
    */

    filter_processors[0]->process( num_samples );
    filter_processors[1]->process( num_samples );
    filter_processors[2]->process( num_samples );

    eq_processor->process( num_samples );

    // send curren velocity as arg to the fx_processor?
    /*
     * TODO add back velocity glide
    fx_processor->velocity_glide = current_velocity;
    if( synth_data.arp_sequencer_data->is_on )
        fx_processor->velocity_glide = current_velocity * synth_data.arp_sequencer_data->velocity[current_step];
     */

    fx_processor->process( output_buffer_, start_sample_, num_samples_ );

    // OSCs - THREAD 1 ?

    // VISUALIZE
    if( amp_painter )
        for( int osc_id = 0 ; osc_id != SUM_OSCS ; ++osc_id )
            for( int i = 0 ; i != num_samples ; ++i )
                amp_painter->add_osc( osc_id,
                                      data_buffer->osc_samples.getReadPointer(osc_id)[i],
                                      data_buffer->osc_switchs.getReadPointer(osc_id)[i]
                                    );
    if( amp_painter )
        for( int lfo_id = 0 ; lfo_id != SUM_LFOS ; ++lfo_id )
            for( int i = 0 ; i != num_samples ; ++i )
                amp_painter->add_lfo( lfo_id, data_buffer->lfo_amplitudes.getReadPointer(lfo_id)[i]  );

    // UI INFORMATIONS
    for( int i = 0 ; i != SUM_OSCS ; ++i )
        synth_data.osc_datas[i]->last_modulation_value = data_buffer->lfo_amplitudes.getReadPointer(i)[num_samples-1];

    // CLEAR
    release_if_inactive();
}

float MONOVoice::get_filter_env_amp( int filter_id_ ) const noexcept {
    return filter_envs[filter_id_]->get_amp();
}

float MONOVoice::get_lfo_amp( int lfo_id_ ) const noexcept {
    return lfos[lfo_id_]->get_current_amp();
}
float MONOVoice::get_arp_sequence_amp( int step_ ) const noexcept {
    if( arp_sequencer->get_current_step() == step_ )
        return fx_processor->final_env->get_amp();

    return 0;
}

void MONOVoice::pitchWheelMoved (int) {}
void MONOVoice::controllerMoved (int, int ) {}
//==============================================================================
NOINLINE mono_ParameterOwnerStore::~mono_ParameterOwnerStore()
{
    if( ui_env )
    {
        delete ui_env;
        delete ui_env_preset_data;
    }

    clearSingletonInstance();
}

float mono_ParameterOwnerStore::get_flt_input_env_amp( int flt_id_, int input_id_ ) {
    return mono_ParameterOwnerStore::getInstance()->voice->filter_processors[flt_id_]->input_envs[input_id_]->get_amp();
}

float mono_ParameterOwnerStore::get_band_env_amp( int band_id_ ) {
    return mono_ParameterOwnerStore::getInstance()->voice->eq_processor->envs[band_id_]->get_amp();
}
float mono_ParameterOwnerStore::get_chorus_modulation_env_amp() {
    return mono_ParameterOwnerStore::getInstance()->voice->fx_processor->chorus_modulation_env->get_amp();
}
void mono_ParameterOwnerStore::get_full_adsr( float state_, Array< float >& curve, int& sustain_start_, int& sustain_end_ )
{
    mono_ParameterOwnerStore* store = mono_ParameterOwnerStore::getInstance();
    if( ! store->ui_env ) {
        store->ui_env_preset_data = new ENVPresetData( 999, store->env_preset_def );
        store->ui_env = new ENV( *store->ui_env_preset_data );
    }
    ENV* env = store->ui_env;
    ENVPresetData* data = store->ui_env_preset_data;
    data->state = state_;
    data->sustain = 0.5;
    float* one_sample_buffer = new float;
    bool count_sustain = false;
    env->start_attack();
    const int suatain_samples = RuntimeNotifyer::getInstance()->get_sample_rate() / 10;
    sustain_end_ = -1;
    while( env->current_stage != END_ENV )
    {
        // GET DATA
        env->process( one_sample_buffer, 1 );
        if( env->current_stage != SUSTAIN )
            curve.add(*one_sample_buffer);

        // START COUNT SUSTAIN
        if( env->current_stage == SUSTAIN && ! count_sustain ) {
            count_sustain = true;
            sustain_start_ = curve.size();
        }

        if( sustain_end_ == -1 && count_sustain && data->sustain_time == 1 )
        {
            env->set_to_release();
            sustain_end_ = sustain_start_ + suatain_samples;
        }
        else if( sustain_end_ == -1 && count_sustain && data->sustain_time != 1 ) {
            env->set_to_release();
            sustain_end_ = sustain_start_ + msToSamplesFast(8.0f*data->sustain_time*1000,RuntimeNotifyer::getInstance()->get_sample_rate());
        }
    }

    delete one_sample_buffer;
}