
#include "Synth.h"
#include "mono_AmpPainter.h"

juce_ImplementSingleton (mono_ParameterOwnerStore)

//==============================================================================
//==============================================================================
//==============================================================================
NOINLINE AmpSmoothBuffer::AmpSmoothBuffer() : pos(0), sum(0) {
    for( int i = 0 ; i != AMP_SMOOTH_SIZE ; ++i )
        buffer[i] = 0;
}
NOINLINE AmpSmoothBuffer::~AmpSmoothBuffer() {}

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
#define protection_clipping(x) x
forcedinline static float protection_clipping_DIABLED( float input_and_worker_ ) noexcept
{
    if( input_and_worker_ > HARD_CLIPPER_STAGE_1 )
    {
        input_and_worker_ = HARD_CLIPPER_STAGE_1 + input_and_worker_*0.1f;
        if( input_and_worker_ > HARD_CLIPPER_STAGE_2 )
        {
            input_and_worker_ = HARD_CLIPPER_STAGE_2 + input_and_worker_*0.05f;
            if( input_and_worker_ > HARD_CLIPPER_STAGE_2 )
            {
                input_and_worker_ = HARD_CLIPPER_STAGE_2;
            }
        }
    }
    else if( input_and_worker_ < -HARD_CLIPPER_STAGE_1 )
    {
        input_and_worker_ = -HARD_CLIPPER_STAGE_1 + input_and_worker_*0.1f;
        if( input_and_worker_ < -HARD_CLIPPER_STAGE_2 )
        {
            input_and_worker_ = -HARD_CLIPPER_STAGE_2 + input_and_worker_*0.05f;
            if( input_and_worker_ < -HARD_CLIPPER_STAGE_2 )
            {
                input_and_worker_ = -HARD_CLIPPER_STAGE_2;
            }
        }
    }

    return input_and_worker_;
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
#define TABLESIZE_MULTI 10
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
    current_buffer_size( init_buffer_size_ ) ,
    osc_samples( init_buffer_size_ ),
    osc_switchs( init_buffer_size_ ),
    osc_sync_switchs( init_buffer_size_ ),
    modulator_samples( init_buffer_size_ ),
    working_buffer( init_buffer_size_ ),
    lfo_amplitudes( init_buffer_size_ ),
    filter_input_sustain( init_buffer_size_ ),
    filter_input_env_amps( init_buffer_size_ ),
    filter_output_samples( init_buffer_size_ ),
    filtered_env_amps( init_buffer_size_ ),
    filtered_samples( init_buffer_size_ ),
    env_amp( init_buffer_size_ ),
    chorus_modulation_env_amp( init_buffer_size_ ),
    direct_filter_output_samples( init_buffer_size_ ),
    eq_ar_sustain( init_buffer_size_ ),
    eq_ar_amps( init_buffer_size_ ),
    tmp_samples( init_buffer_size_ )
{}

void DataBuffer::resize_buffer_if_required( int min_size_required_ ) noexcept
{
    if( min_size_required_ > current_buffer_size )
    {
        current_buffer_size = min_size_required_;

        osc_samples.setSize(current_buffer_size);
        osc_switchs.setSize(current_buffer_size);
        osc_sync_switchs.setSize(current_buffer_size);
        modulator_samples.setSize(current_buffer_size);
        working_buffer.setSize(current_buffer_size);
        lfo_amplitudes.setSize(current_buffer_size);
        filter_input_sustain.setSize(current_buffer_size);
        filter_input_env_amps.setSize(current_buffer_size);
        filter_output_samples.setSize(current_buffer_size);
        filtered_env_amps.setSize(current_buffer_size);
        filtered_samples.setSize(current_buffer_size);
        env_amp.setSize(current_buffer_size);
        chorus_modulation_env_amp.setSize(current_buffer_size);
        direct_filter_output_samples.setSize(current_buffer_size);
        eq_ar_sustain.setSize(current_buffer_size);
        eq_ar_amps.setSize(current_buffer_size);
        tmp_samples.setSize(current_buffer_size);
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
    return phase_ / float_Pi;
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
    float last_tick_value;

public:
    inline void setSeed( unsigned int seed = 0 ) noexcept;
    inline float lastOut() const noexcept;
    inline float tick() noexcept;

public:
    NOINLINE mono_Noise( unsigned int seed = 0 );
    NOINLINE ~mono_Noise();
};

// -----------------------------------------------------------------
NOINLINE mono_Noise::mono_Noise( unsigned int seed ) : last_tick_value(0)
{
    this->setSeed( seed );
}
NOINLINE mono_Noise::~mono_Noise() {}

// -----------------------------------------------------------------
inline float mono_Noise::lastOut() const noexcept
{
    return last_tick_value;
}
inline float mono_Noise::tick() noexcept
{
    return last_tick_value = ( 2.0 * rand() / (RAND_MAX + 1.0) - 1.0 );
}
inline void mono_Noise::setSeed( unsigned int seed ) noexcept
{
    if ( seed == 0 )
        srand( (unsigned int) time( NULL ) );
    else
        srand( seed );
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
        b = (float) (1.0 - thePole);
    else
        b = (float) (1.0 + thePole);

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
    inline float lastOut( void ) const noexcept;

    // HACK FOR SYNC
    inline bool isNewCylce() const noexcept;
    inline void clearNewCycleState() noexcept;

    inline float tick( void ) noexcept;

public:
    NOINLINE mono_Modulate( void );
    NOINLINE ~mono_Modulate( void );
};

// -----------------------------------------------------------------
NOINLINE mono_Modulate::mono_Modulate( void )
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
inline float mono_Modulate::lastOut( void ) const noexcept {
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
class LFO {
    mono_SineWave sine_generator;
    
    float last_frequency;

    const int id;
    int last_factor;
    int step_to_wait_for_sync;

public:
    inline void process( DataBuffer& buffer_, int step_number_, int num_samples_ ) noexcept;

private:
    inline void sync( int step_number_ ) noexcept;

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
    last_frequency(1),
    id(id_),
    last_factor(0),
    step_to_wait_for_sync(0)
{
}
NOINLINE LFO::~LFO() {}

inline void LFO::process( DataBuffer& buffer_, int step_number_, int num_samples_ ) noexcept {
    sync( step_number_ );

    float*const output_buffer = buffer_.lfo_amplitudes.getWritePointer(id);
    for( int sid = 0 ; sid < num_samples_ ; ++sid )
        output_buffer[sid] = (1.0f + sine_generator.tick()) / 2;
}
inline void LFO::sync( int step_number_ ) noexcept {
    LFOData& data( DATA( lfo_datas[id] ) );

    if( data.speed <= 6 )
    {
        if( step_number_ == 0 )
        {
            float factor = 1; // 1/1
            if( data.speed == 0 )
                factor = 16; //return "16/1";
            else if( data.speed <= 1 )
                factor = 12; //return "12/1";
            else if( data.speed <= 2 )
                factor = 8;
            else if( data.speed <= 3 )
                factor = 4;
            else if( data.speed <= 4 )
                factor = 3;
            else if( data.speed <= 5 )
                factor = 2;

            const float whole_notes_per_sec( DATA( runtime_info ).bpm/4 / 60 ); // = 1 at our slider
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
    else if( data.speed <= 17 )
    {
        if( step_number_ == 0 )
        {

            float factor = 0;
            if( data.speed <= 7 )
                factor = 3.0f/4;
            else if( data.speed <= 8 )
                factor = 1.0f/2;
            else if( data.speed <= 9 )
                factor = 1.0f/3;
            else if( data.speed <= 10 )
                factor = 1.0f/4;
            else if( data.speed <= 11 )
                factor = 1.0f/8;
            else if( data.speed <= 12 )
                factor = 1.0f/12;
            else if( data.speed <= 13 )
                factor = 1.0f/16;
            else if( data.speed <= 14 )
                factor = 1.0f/24;
            else if( data.speed <= 15 )
                factor = 1.0f/32;
            else if( data.speed <= 16 )
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
        const float frequency = midiToFrequency(33+data.speed-18);
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
static inline float getrand_noise() noexcept {
    return (Random::getSystemRandom().nextFloat() - 0.5f) * 2;
}

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
    last_cycle_was_pulse_switch(0)
{
    modulator.setVibratoGain(1);
}
NOINLINE OSC::~OSC() {}

inline void OSC::process(DataBuffer* data_buffer_,
                         const int num_samples_) noexcept
{
    OSCData::ProcessCopy osc_data;
    DATA( osc_datas[id] ).get_updated_working_copy( osc_data );
    OSCData::ProcessCopy master_osc_data;
    DATA( osc_datas[MASTER_OSC] ).get_working_copy( master_osc_data );

    // LFO
    const float* lfo_amps( data_buffer_->lfo_amplitudes.getReadPointer(id) );

    // BUFFERS
    float* output_buffer( data_buffer_->osc_samples.getWritePointer(id) );
    float* switch_buffer( data_buffer_->osc_switchs.getWritePointer(id) );

    float* osc_sync_switch_buffer( data_buffer_->osc_sync_switchs.getWritePointer(MASTER_OSC) );
    float* osc_modulator_buffer( data_buffer_->modulator_samples.getWritePointer(MASTER_OSC) );

    // FM SWING
    const bool master_osc_modulation_is_off = id == MASTER_OSC ? master_osc_data.mod_off : false;
    if( master_osc_data.fm_swing != 0 ) {
        const bool was_negative = puls_swing_delta < 0;
        puls_swing_delta = sample_rate * master_osc_data.fm_swing * 0.00000005;
        if( was_negative )
            puls_swing_delta *= -1;
    }
    else
        puls_swing_amp = 1;

    for( int sid = 0 ; sid < num_samples_ ; ++sid )
    {
        // UPDATE FREQUENCY - TODO after ticks
        {
            const float octave_mod = osc_data.octave->tick_modulated( osc_data.is_lfo_modulated ? lfo_amps[sid] : 0 );
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
                || osc_data.octave->is_output_changed_since_last_tick()
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
            else if( master_osc_data.fm_multi != last_modulator_multi ) {
                change_modulator_frequency = true;
            }

            // MODULATOR FREQUENCY TODO -> only osc 1
            if( change_modulator_frequency ) {
                last_modulator_multi = master_osc_data.fm_multi;
                const float modulator_frequency = last_frequency* (1.1f + 14.9f*last_modulator_multi);
                modulator.setVibratoRate( modulator_frequency );

                modulator_sync_cycles = std::floor(modulator_frequency / last_frequency);
            }
        }

        // PROCESS
        float sample = 0;
        const float wave = osc_data.wave;
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
                const float noice_powerd = getrand_noise()* multi; // noice_generator.lastOut() * multi;
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
                if( master_osc_data.puls_width < 0 )
                {
                    current_puls_frequence_offset = (1.0f/12.0f*master_osc_data.puls_width*-1.0f);
                    if( last_puls_was_large )
                        current_puls_frequence_offset = 1.0f + 1.0f*current_puls_frequence_offset;
                    else
                        current_puls_frequence_offset = 1.0f - 0.5f*current_puls_frequence_offset;

                    last_cycle_was_pulse_switch = true;
                    last_puls_was_large ^= true;
                }
                // PULS WITH _|¯|_  break  _|¯|_
                else if( master_osc_data.puls_width > 0 && puls_with_break_counter < 1 )
                {
                    puls_with_break_counter = master_osc_data.puls_width;
                    current_puls_frequence_offset = 0;
                }
                else
                {
                    --puls_with_break_counter;
                    current_puls_frequence_offset = 0;
                }

                // SWING
                if( master_osc_data.osc_switch > 0 )
                {
                    if( puls_swing_switch_counter <= 0 ) {
                        puls_swing_multi *= -1;
                        puls_swing_switch_counter = master_osc_data.osc_switch;
                    }
                    --puls_swing_switch_counter;
                }

                if( id == MASTER_OSC )
                {
                    osc_sync_switch_buffer[sid] = true;
                }
                else if( osc_data.sync )
                    waiting_for_sync = true;
            }
            else if( id == MASTER_OSC ) {
                osc_sync_switch_buffer[sid] = false;
            }

            // EXIT SYNC
            if( osc_data.sync && id != MASTER_OSC )
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
        const float fm_amount = osc_data.fm_amount->tick();
        float modulator_sample = 0;
        {
            if( id == MASTER_OSC )
            {
                int used_sync_cycles = 1;
                if( ! master_osc_data.fm_wave )
                    used_sync_cycles = modulator_sync_cycles;

                // PROCESS MODULATOR
                if( ! waiting_for_modulator_sync )
                    modulator_sample = modulator.tick();

                // FM SYNC
                if( master_osc_data.sync )
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
                        if( master_osc_data.fm_swing > 0 )
                            modulator_sample *= puls_swing_amp;
                        if( master_osc_data.osc_switch > 0 )
                            sample *= puls_swing_multi;
                    }
                }
            }

            // OUT
            if( fm_amount )
                output_buffer[sid] = sample*(1.0f-fm_amount) + ( (modulator_sample + sample)/2 )*fm_amount;
            // output_buffer[sid] = sample*(1.0f-fm_amount) + wave_mixer_v2( modulator_sample, sample )*fm_amount;
            //output_buffer[sid] = wave_mixer_v2(sample*(1.0f-fm_amount), wave_mixer_v2( modulator_sample, sample )*fm_amount);
            else
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
    inline void reset( float end_value_,
                       float time_sample_rate_factor_,
                       float start_value_ = WORK_FROM_CURRENT_VALUE ) noexcept;
    inline bool end_reached() const noexcept;
    inline void replace_current_value( float value_ ) noexcept;
    inline void force_zero_glide() noexcept;

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

inline void ValueEnvelope::reset( float end_value_,
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
    int buffer_size;

public:
    inline void process( float* dest_, const int num_samples_ ) noexcept;
private:
    inline void update_stage() noexcept;

public:
    // TODO, add to filter processor
    inline void start_attack( float set_to_zero = true ) noexcept;
    inline void set_to_release() noexcept;

public:
    float get_amp() const noexcept;

    NOINLINE ENV( ENVData& data_ );
    NOINLINE ~ENV();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENV)
};

NOINLINE ENV::ENV( ENVData& data_ )
    :
    current_stage(END_ENV),
    data( data_ )
{
}
NOINLINE ENV::~ENV() {}

inline void ENV::process( float* dest_, const int num_samples_ ) noexcept {
    data.sustain.update( DATA( synth_data ).glide_motor_time );
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
            envelop.replace_current_value( data.sustain.tick() );
        else
            data.sustain.tick();

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
            envelop.reset( 1, data.attack*data.max_attack_time );
        else
        {
            envelop.reset( data.sustain.tick(), data.attack*data.max_attack_time );
        }

        current_stage = ATTACK;
        break;
    }
    case ATTACK :
    {
        if( data.decay )
        {
            envelop.reset( data.sustain.tick(), data.decay*data.max_decay_time );
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

        envelop.reset( data.sustain.tick(), sustain_time );
        current_stage = SUSTAIN;
    }
    break;
    case SUSTAIN :
    {
        envelop.reset( 0, data.release*data.max_release_time );
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

float ENV::get_amp() const noexcept {
    return envelop.get_current_amp();
}

// TODO filter update nur wenn die wertänderung z.b grösser 1 %
// do the calculation

// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------
#include "mono_MoogFilter.h"
#include "dRowAudio_EnvelopeFollower.h"
class FilterProcessor {
    const int id;

    ENV filter_env;
    AmpSmoothBuffer env_smoother;
    AmpSmoothBuffer input_env_smoother[SUM_INPUTS_PER_FILTER];
    OwnedArray< DoubleAnalogFilter > double_filter;
    float last_output[SUM_INPUTS_PER_FILTER];

    friend class mono_ParameterOwnerStore;
    OwnedArray< ENV > input_envs;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void process( DataBuffer& data_buffer_, const int num_samples ) noexcept;

private:
    //  TODO MAKE AN ARRAY WITH ALL FLOATS AND DEFINCE A FIXED INDEX - LESS BY VALUE!!!
    inline void processPass( float** output_, const float input_[SUM_INPUTS_PER_FILTER], const float filter_distortion, const int sid_ ) noexcept;
    inline void process2PassLP( float** output_, const float input_[SUM_INPUTS_PER_FILTER],
                                const float resonance_, const float cutoff_, const float gain_,
                                const float filter_distortion,
                                const int sid_ ) noexcept;
    inline void processLP( float** output_, const float input_[SUM_INPUTS_PER_FILTER],
                           const float resonance_, const float cutoff_, const float gain_,
                           const float filter_distortion,
                           const int sid_ ) noexcept;
    inline void process2PassHP( float** output_, const float input_[SUM_INPUTS_PER_FILTER],
                                const float resonance_, const float cutoff_, const float gain_,
                                const float filter_distortion,
                                const int sid_ ) noexcept;
    inline void processHP( float** output_, const float input_[SUM_INPUTS_PER_FILTER],
                           const float resonance_, const float cutoff_, const float gain_,
                           const float filter_distortion,
                           const int sid_ ) noexcept;
    inline void processBand( float** output_, const float input_[SUM_INPUTS_PER_FILTER],
                             const float resonance_, const float cutoff_, const float gain_,
                             const float filter_distortion,
                             const int sid_ ) noexcept;

    EnvelopeFollower env_follower;
    inline void compress
    (
        float* io_buffer,
        float* working_buffer_,
        const float*  compressor_signal,
        int num_samples,
        float power
    ) noexcept;

public:
    NOINLINE FilterProcessor( int id_ );
    NOINLINE ~FilterProcessor();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor)
};

NOINLINE FilterProcessor::FilterProcessor( int id_ )
    :
    id(id_),

    filter_env( DATA( env_datas[id] ) )
{
    for( int i = 0 ; i != SUM_INPUTS_PER_FILTER ; ++i ) {
        double_filter.add( new DoubleAnalogFilter() );
        input_envs.add( new ENV( DATA( filter_input_env_datas[i + SUM_INPUTS_PER_FILTER * id_] ) ) );
        last_output[i] = 0;
    }
}

NOINLINE FilterProcessor::~FilterProcessor() {}

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

#define FILTER_SAMLE_CLIPPING(x)  distortion(x,filter_distortion)
#define PREDISTORTION(x)  distortion(x,filter_distortion)
inline void FilterProcessor::processPass(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
        output_[io_id][sid_] = FILTER_SAMLE_CLIPPING( PREDISTORTION( input_[io_id] ) );
}

static inline float soft_clipping2( float sample_ )
{
    sample_ = (std::atan(sample_)/float_Pi)*2;
    return sample_;
}
inline void AnalogFilter::update(float resonance_, float cutoff_, float gain_) noexcept
{
    if( cutoff != cutoff_ || gain != gain_ )
    {
        gain = gain_;
        cutoff = cutoff_;
        calc_coefficient();
    }
    res = resonance_;
}

#define MAX_CUTOFF 8000.0f
#define RESONANCE_MULT 80.0f
#define GAIN_MULT 40.0f
#define MIN_GAIN 1.0f
#define MIN_RESONANCE 1.0f
#define MIN_CUTOFF 40.0f

// SHAPER
#define FIXED_K 2.0f*0.7f/(1.0f-0.7f)

// 2 PASS LP
inline float AnalogFilter::processLowResonance(float input_and_worker_) noexcept
{
    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4)/6;

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // Add resonance
    return y4 + std::atan( res * 4.0f * y3 );
}
inline void DoubleAnalogFilter::updateLowResonance(float resonance_, float cutoff_, float gain_) noexcept
{
    //brauchen wir das (+45) oder kann die modulation dies lösen?
    lpf_1.update( resonance_, cutoff_+35, gain_ );
}
inline float DoubleAnalogFilter::processLowResonance(float input_and_worker_) noexcept
{
    was_two_way = false;
    return lpf_1.processLowResonance( input_and_worker_ );
}
inline void FilterProcessor::process2PassLP(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float resonance_, const float cutoff_, const float gain_,
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
    {
        DoubleAnalogFilter* filter = double_filter[io_id];
        const float input = protection_clipping(input_[io_id]);
        if( input != 0 or last_output[io_id] != 0  )
        {
            filter->updateLow2Pass
            (
                resonance_,
                cutoff_+35,
                gain_
            );

            float output = FILTER_SAMLE_CLIPPING( filter->processLow2Pass( PREDISTORTION( input ) ) );
            output_[io_id][sid_] = output;
            last_output[io_id] = output;
        }
        else
        {
            filter->reset();
            output_[io_id][sid_] = 0;
        }
    }
}

// LP
inline void DoubleAnalogFilter::updateLow2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    lpf_1.update( resonance_, cutoff_, gain_ );
    lpf_2.copy_coefficient_from( lpf_1 );
}
inline float DoubleAnalogFilter::processLow2Pass(float input_and_worker_) noexcept
{
    if( not was_two_way )
    {
        lpf_2.copy_state_from( lpf_1 );
        was_two_way = true;
    }
    input_and_worker_ = lpf_2.processLowResonance( input_and_worker_ );
    const float gain = lpf_1.gain;
    float low = lpf_1.processLowResonance( input_and_worker_*gain );

    return (input_and_worker_+low)*(1.0f-gain) + soft_clipping2(input_and_worker_+low)*gain;
}
inline void FilterProcessor::processLP(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float resonance_, const float cutoff_, const float gain_,
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
    {
        DoubleAnalogFilter* filter = double_filter[io_id];
        const float input = protection_clipping(input_[io_id]);
        if( input != 0 or last_output[io_id] != 0 )
        {
            filter->updateLowResonance
            (
                resonance_,
                cutoff_,
                gain_
            );

            float output = FILTER_SAMLE_CLIPPING( filter->processLowResonance( PREDISTORTION( input ) ) );
            output_[io_id][sid_] = output;
            last_output[io_id] = output;
        }
        else
        {
            filter->reset();
            output_[io_id][sid_] = 0;
        }
    }
}

// 2 PASS HP
inline float AnalogFilter::processHighResonance(float input_and_worker_) noexcept
{
    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4)/6;

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    // RESONANCE
    return (input_and_worker_-y4) + (std::atan( res * 4.0f * y2 ));
}
inline void DoubleAnalogFilter::updateHigh2Pass(float resonance_, float cutoff_, float gain_) noexcept
{
    lpf_1.update( resonance_, cutoff_, gain_ );
    lpf_2.copy_coefficient_from( lpf_1 );
}
inline float DoubleAnalogFilter::processHigh2Pass(float input_and_worker_) noexcept
{
    if( not was_two_way )
    {
        lpf_2.copy_state_from( lpf_1 );
        was_two_way = true;
    }
    input_and_worker_ = lpf_2.processHighResonance( input_and_worker_ );
    const float gain = lpf_1.gain;
    float low = lpf_1.processHighResonance( input_and_worker_*gain );
    return (input_and_worker_+low)*(1.0f-gain) + soft_clipping2(input_and_worker_+low)*gain ;
}
inline void FilterProcessor::process2PassHP(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float resonance_, const float cutoff_, const float gain_,
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
    {
        DoubleAnalogFilter* filter = double_filter[io_id];
        const float input = protection_clipping(input_[io_id]);
        if( input != 0 or last_output[io_id] != 0 )
        {
            filter->updateHigh2Pass
            (
                resonance_,
                cutoff_,
                gain_
            );

            float output = FILTER_SAMLE_CLIPPING( filter->processHigh2Pass( PREDISTORTION( input ) ) );
            output_[io_id][sid_] = output;
            last_output[io_id] = output;
        }
        else
        {
            filter->reset();
            output_[io_id][sid_] = 0;
        }
    }
}

// HP
inline void DoubleAnalogFilter::updateHighResonance(float resonance_, float cutoff_, float gain_) noexcept
{
    lpf_1.update( resonance_, cutoff_, gain_ );
}
inline float DoubleAnalogFilter::processHighResonance(float input_and_worker_) noexcept
{
    was_two_way = false;
    return lpf_1.processHighResonance( input_and_worker_ );
}
inline void FilterProcessor::processHP(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float resonance_, const float cutoff_, const float gain_,
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
    {
        DoubleAnalogFilter* filter = double_filter[io_id];
        const float input = protection_clipping(input_[io_id]);
        if( input != 0 or last_output[io_id] != 0 )
        {
            filter->updateHighResonance
            (
                resonance_,
                cutoff_,
                gain_
            );

            float output = FILTER_SAMLE_CLIPPING( filter->processHighResonance( PREDISTORTION( input ) ) );
            output_[io_id][sid_] = output;
            last_output[io_id] = output;
        }
        else
        {
            filter->reset();
            output_[io_id][sid_] = 0;
        }
    }
}

// BAND
inline void DoubleAnalogFilter::updateBand(float resonance_, float cutoff_, float gain_ ) noexcept
{
    lpf_1.update( resonance_, cutoff_+10, gain_ );
    lpf_2.update( resonance_, cutoff_, gain_ );
}
inline float DoubleAnalogFilter::processBand(float input_and_worker_) noexcept
{
    if( not was_two_way )
    {
        lpf_2.copy_state_from( lpf_1 );
        was_two_way = true;
    }
    return lpf_1.processLowResonance( lpf_2.processHighResonance( input_and_worker_ ) );
}
inline void FilterProcessor::processBand(
    float** output_, const float input_[SUM_INPUTS_PER_FILTER],
    const float resonance_, const float cutoff_, const float gain_,
    const float filter_distortion,
    const int sid_ ) noexcept
{
    for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
    {
        DoubleAnalogFilter* filter = double_filter[io_id];
        filter->updateBand
        (
            resonance_,
            cutoff_,
            gain_
        );
        const float input = protection_clipping(input_[io_id]);
        if( input != 0 or last_output[io_id] != 0 )
        {
            float output = FILTER_SAMLE_CLIPPING( filter->processBand( PREDISTORTION( input ) ) );
            output_[io_id][sid_] = output;
            last_output[io_id] = output;
        }
        else
        {
            filter->reset();
            output_[io_id][sid_] = 0;
        }
    }
}
inline void FilterProcessor::process( DataBuffer& data_buffer_, const int num_samples ) noexcept
{
    // GET DATA COPY AND REFERENCE
    FilterData::ProcessCopy filter_data;
    DATA( filter_datas[id] ).get_updated_working_copy( filter_data );

    // COLLECT LFO AMP
    const float* lfo_amplitudes = data_buffer_.lfo_amplitudes.getReadPointer(id);

    // COLLECT INPUT BUFFERS
    const float* left_input_buffers[SUM_INPUTS_PER_FILTER];
    const float* right_input_buffers[SUM_INPUTS_PER_FILTER];
    // COLLECT OUTPUT(in) BUFFERS
    // INPUT ENVELOP
    // ENV / SUSTAIN
    float* out_buffers[SUM_INPUTS_PER_FILTER];
    float* input_ar_amps[SUM_INPUTS_PER_FILTER];
    float* sustain_buffers[SUM_INPUTS_PER_FILTER];
    for( int input_id = 0 ; input_id != SUM_INPUTS_PER_FILTER ; ++input_id )
    {
        // COLLECT BUFFERS
        {
            left_input_buffers[input_id] = data_buffer_.osc_samples.getReadPointer(input_id);
            if( id > 0 )
                right_input_buffers[input_id] = data_buffer_.filter_output_samples.getReadPointer(input_id + SUM_INPUTS_PER_FILTER*(id-1) );
            else
                right_input_buffers[input_id] = left_input_buffers[input_id];

            out_buffers[input_id] = data_buffer_.filter_output_samples.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * id );
            input_ar_amps[input_id] = data_buffer_.filter_input_env_amps.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * id );
            sustain_buffers[input_id] = data_buffer_.filter_input_sustain.getWritePointer( input_id + SUM_INPUTS_PER_FILTER * id );
        }

        // ENV SUSTAIN
        {
            ENV*const env = input_envs.getUnchecked(input_id);
            env->process( input_ar_amps[ input_id ], num_samples );

            // OVERRIDE AMP BY FIXED SUSTAIN VALUE)
            const bool is_input_sustain_fixed = filter_data.input_holds[input_id];
            for( int sid = 0 ; sid != num_samples ; ++sid )
            {
                float sustain = filter_data.input_sustains->get(input_id).tick();
                sustain_buffers[input_id][sid] = sustain;
                if( is_input_sustain_fixed )
                    input_ar_amps[input_id][sid] = positive(sustain);
            }
        }
    }

    // ENVELOP
    const float* env_amps( data_buffer_.filtered_env_amps.getReadPointer(id) );

    // OUTPUT BUFFER
    float* direct_output_buffer( data_buffer_.direct_filter_output_samples.getWritePointer(id) );

    mono_AmpPainter*const amp_painter = MONOVoice::get_amp_painter();
    for( int sid = 0 ; sid != num_samples ; ++sid )
    {
        float amp_mix = (1.0f+filter_data.adsr_lfo_mix->tick()) / 2;
        amp_mix = env_smoother.add_and_get_average( env_amps[sid]*(1.0f-amp_mix) + lfo_amplitudes[sid]*amp_mix );

        float inputs[SUM_INPUTS_PER_FILTER];
        inputs[0] = sustain_buffers[0][sid] < 0 ? left_input_buffers[0][sid] : right_input_buffers[0][sid];
        inputs[1] = sustain_buffers[1][sid] < 0 ? left_input_buffers[1][sid] : right_input_buffers[1][sid];
        inputs[2] = sustain_buffers[2][sid] < 0 ? left_input_buffers[2][sid] : right_input_buffers[2][sid];

        // PROCESS FILTER
        switch( filter_data.filter_type )
        {
        case LPF_2_PASS :
        case MOOG_AND_LPF:
            process2PassLP
            (
                out_buffers,
                inputs,
                filter_data.resonance->tick_modulated( amp_mix, filter_data.modulate_resonance ),
                (MAX_CUTOFF * filter_data.cutoff->tick_modulated( amp_mix, filter_data.modulate_cutoff ) + MIN_CUTOFF) / 8,
                filter_data.gain->tick_modulated( amp_mix, filter_data.modulate_gain ),
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
            break;
        case LPF:
            processLP
            (
                out_buffers,
                inputs,
                filter_data.resonance->tick_modulated( amp_mix, filter_data.modulate_resonance ),
                (MAX_CUTOFF * filter_data.cutoff->tick_modulated( amp_mix, filter_data.modulate_cutoff ) + MIN_CUTOFF) / 8,
                filter_data.gain->tick_modulated( amp_mix, filter_data.modulate_gain ),
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
            break;
        case HIGH_2_PASS :
            process2PassHP
            (
                out_buffers,
                inputs,
                filter_data.resonance->tick_modulated( amp_mix, filter_data.modulate_resonance ),
                MAX_CUTOFF*2.0f * filter_data.cutoff->tick_modulated( amp_mix, filter_data.modulate_cutoff ) + MIN_CUTOFF,
                filter_data.gain->tick_modulated( amp_mix, filter_data.modulate_gain ),
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
            break;
        case HPF :
            processHP
            (
                out_buffers,
                inputs,
                filter_data.resonance->tick_modulated( amp_mix, filter_data.modulate_resonance ),
                MAX_CUTOFF*2.0f * filter_data.cutoff->tick_modulated( amp_mix, filter_data.modulate_cutoff ) + MIN_CUTOFF,
                filter_data.gain->tick_modulated( amp_mix, filter_data.modulate_gain ),
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
            break;
        case BPF:
            processBand
            (
                out_buffers,
                inputs,
                filter_data.resonance->tick_modulated( amp_mix, filter_data.modulate_resonance ),
                MAX_CUTOFF * filter_data.cutoff->tick_modulated( amp_mix, filter_data.modulate_cutoff ) + MIN_CUTOFF,
                filter_data.gain->tick_modulated( amp_mix, filter_data.modulate_gain ),
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
            break;
        default:  /* PASS */
            processPass
            (
                out_buffers,
                inputs,
                filter_data.distortion->tick_modulated( filter_data.modulate_distortion ? amp_mix : 0 ),
                sid
            );
        }

        {
            if( amp_painter )
                amp_painter->add_filter_env( id, amp_mix );
        }

        // OUTPUT MIX AND DISTORTION
        {
            direct_output_buffer[sid] = 0;
            for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
                direct_output_buffer[sid] += out_buffers[io_id][sid]*input_env_smoother[io_id].add_and_get_average(input_ar_amps[io_id][sid]);
        }

        {
            direct_output_buffer[sid] *= filter_data.output->tick_modulated( filter_data.modulate_output ? amp_mix : 0 );
            const float clipping = filter_data.output_clipping->tick();
            direct_output_buffer[sid] = direct_output_buffer[sid]*(1.0f-clipping) + soft_clipping(direct_output_buffer[sid])*clipping;
        }

        {
            for( int io_id = 0 ; io_id != SUM_INPUTS_PER_FILTER ; ++io_id )
                out_buffers[io_id][sid] = soft_clipping(out_buffers[io_id][sid]);
        }

    }

    // COMPRESS
    if( id == 0 )
        compress( direct_output_buffer,
                  data_buffer_.tmp_samples.getWritePointer(0),
                  direct_output_buffer,
                  num_samples,
                  filter_data.compressor );
    else
        compress( direct_output_buffer,
                  data_buffer_.tmp_samples.getWritePointer(0),
                  data_buffer_.direct_filter_output_samples.getReadPointer(id-1),
                  num_samples,
                  filter_data.compressor );
}
inline void FilterProcessor::compress
(
    float* io_buffer,
    float* working_buffer_,
    const float*  compressor_signal,
    int num_samples,
    float power
) noexcept
{
    float use_power = power;
    bool is_negative = false;
    if( use_power < 0 ) {
        use_power*=-1;
        is_negative = true;
    }

    env_follower.setCoefficients( 0.008f * use_power + 0.0001f, 0.005f * use_power + 0.0001f );
    env_follower.processEnvelope( compressor_signal, working_buffer_, num_samples );

    for( int sid = 0 ; sid != num_samples ; ++sid )
    {
        float compression = (exp(working_buffer_[sid])-1);
        if( is_negative )
            compression = 1.0f-compression;

        if( compression < 0 )
            compression = 0;
        else if( compression > 1 )
            compression = 1;
// io_buffer[sid] = input*(1.0f-use_power) + ( (1.0f+FIXED_K)*input/(1.0f+FIXED_K*std::abs(input)) * (1.f - 0.3f*use_power))*use_power;
        if( is_negative )
            io_buffer[sid] = (io_buffer[sid] * (1.0f-use_power)) + ((io_buffer[sid] * compression)*use_power);
        else
            io_buffer[sid] = io_buffer[sid] + ((io_buffer[sid] * compression)*use_power);
    }
}

//==============================================================================
class EQProcessor : public RuntimeListener {
    float frequency_low_pass[SUM_EQ_BANDS];
    float frequency_high_pass[SUM_EQ_BANDS];

    AmpSmoothBuffer amp_smoother[SUM_EQ_BANDS];
    AnalogFilter filter[SUM_EQ_BANDS];
    IIRFilter low_pass_filters[SUM_EQ_BANDS];
    IIRFilter high_pass_filters[SUM_EQ_BANDS];

    friend class mono_ParameterOwnerStore;
public:
    OwnedArray< ENV > envs;

public:
    inline void start_attack() noexcept;
    inline void start_release() noexcept;
    inline void process( DataBuffer& data_buffer_, const int num_samples_ ) noexcept;

    NOINLINE EQProcessor();
    NOINLINE ~EQProcessor();

    NOINLINE void sample_rate_changed( double old_sr_ ) noexcept override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessor)
};

NOINLINE EQProcessor::EQProcessor()
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
void EQProcessor::sample_rate_changed(double) noexcept
{
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        low_pass_filters[band_id].setCoefficients( IIRCoefficients::makeLowPass( sample_rate, frequency_low_pass[band_id] ) );
        high_pass_filters[band_id].setCoefficients( IIRCoefficients::makeHighPass( sample_rate, frequency_high_pass[band_id] ) );
    }
}

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
inline void EQProcessor::process( DataBuffer& data_buffer_, const int num_samples_ ) noexcept
{
    float* io_buffer = data_buffer_.direct_filter_output_samples.getWritePointer(0);
    // AMP BUFFER
    float* ar_buffers[SUM_EQ_BANDS];
    float* sustain_buffers[SUM_EQ_BANDS];
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        ar_buffers[band_id] = data_buffer_.eq_ar_amps.getWritePointer(band_id);
        sustain_buffers[band_id] = data_buffer_.eq_ar_sustain.getWritePointer(band_id);

        DATA( eq_data ).velocity[band_id].update( DATA( synth_data ).glide_motor_time );
        const bool is_input_sustain_fixed = DATA( eq_data ).hold[band_id];
        for( int sid = 0 ; sid != num_samples_ ; ++sid )
        {
            float sustain = DATA( eq_data ).velocity[band_id].tick();
            sustain_buffers[band_id][sid] = sustain;
            if( is_input_sustain_fixed )
                ar_buffers[band_id][sid] = positive(sustain);
        }
    }

    float resonance( DATA( synth_data ).resonance ) ;

    //das klacken muss vor dem band sein

    // PROCESS
    for( int sid = 0 ; sid != num_samples_ ; ++sid )
    {
        float input_sample = io_buffer[sid];
        float sum_output = 0;
        float sum_gains = 1;
        for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id ) // BACKWARTS FOR BASS! <- no more need coz s1+s2
        {
            const float amp = amp_smoother[band_id].add_and_get_average( ar_buffers[band_id][sid] );

            filter[band_id].set( 0.2f*resonance, frequency_low_pass[band_id], amp );
            // PROCESS
            {
                float sustain = sustain_buffers[band_id][sid];
                float gain = sustain < 0 ? 1.0f-amp : 1.0f+5.0f*amp;
                sum_gains += (gain > 1 ? (amp) : 0);
                float output =                     (
                                                       filter[band_id].processLow(
                                                               high_pass_filters[band_id].processSingleSampleRaw(
                                                                       low_pass_filters[band_id].processSingleSampleRaw( input_sample ) ) )
                                                   ) ;

                // SHAPER

                output = output*(1.0f-resonance) + ( (1.0f+FIXED_K)*output/(1.0f+FIXED_K*std::abs(output)) * (0.5f - 0.1f*resonance))*resonance;
                sum_output += output*gain;
            }
        }

        io_buffer[sid] = sum_output / (sum_gains);
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

    data_buffer( new DataBuffer(1024) ),

    arp_sequencer( new ArpSequencer( info, *synth_data.arp_sequencer_data ) ),

    eq_processor( new EQProcessor() ),

    is_stopped( true ),
    was_arp_started(false),

    current_note(-1),
    current_velocity(0),
    velocity_glide(0),

    current_step(0)
{
    mono_ParameterOwnerStore::getInstance()->runtime_info = &info;


    for( int i = 0 ; i != SUM_OSCS ; ++i )
        oscs.add( new OSC( i ) );

    for( int i = 0 ; i != SUM_LFOS ; ++i )
        lfos.add( new LFO( i ) );

    for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
        filter_processors.add( new FilterProcessor( i ));
        filter_envs.add( new ENV( DATA( env_datas[i] ) ) );
    }

    env = new ENV( DATA( env_datas[MAIN_ENV] ) );
    chorus_modulation_env = new ENV( *(DATA(chorus_data).modulation_env_data) );
    chorus_shine_env = new ENV( *(DATA(chorus_data).shine_env_data) );

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
        env->start_attack();

        for( int voice_id = 0 ; voice_id != SUM_FILTERS ; ++voice_id ) {
            filter_envs[voice_id]->start_attack();
            filter_processors[voice_id]->start_attack();
        }
        eq_processor->start_attack();
        chorus_modulation_env->start_attack();
        chorus_shine_env->start_attack();
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

            env->set_to_release();
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
    if( env->current_stage == END_ENV ) {
        clearCurrentNote();
        is_stopped = true;
    }
}

void MONOVoice::renderNextBlock (mono_AudioSampleBuffer<4>& buffer_, int start_sample_, int num_samples_ )
{
    if( synth_data.arp_sequencer_data->is_on && current_note != -1 )
        ;
    else if( is_stopped )
    {
        for( int i = 0 ; i != 4 ; ++i )
            FloatVectorOperations::clear( buffer_.getWritePointer(i), num_samples_ );
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

        render_block( buffer_, is_a_step ? arp_sequencer->get_current_step() : -1, count_start_sample, samples_to_next_step_in_buffer );
        count_start_sample += samples_to_next_step_in_buffer;

        const bool connect = synth_data.arp_sequencer_data->connect;
        const bool restart = arp_sequencer->should_start() && synth_data.arp_sequencer_data->is_on;
        is_a_step = arp_sequencer->is_a_step();
        if( was_arp_started && arp_sequencer->is_a_step() ) {
            if( restart && connect )
                ;
            else
            {
                env->set_to_release();

                was_arp_started = false;

                // STOP FILTER ENV
                for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
                    filter_envs[i]->set_to_release();
                    filter_processors[i]->start_release();
                }
                eq_processor->start_release();
                chorus_modulation_env->set_to_release();
                chorus_shine_env->start_attack();
            }
        }
        if( restart ) {
            if( was_arp_started && connect )
                ;
            else
            {
                env->start_attack();

                was_arp_started = true;

                // TODO add bool for running voices or use was arp started

                // RESTART FILTERS
                for( int i = 0 ; i != SUM_FILTERS ; ++i ) {
                    filter_envs[i]->start_attack();
                    filter_processors[i]->start_attack();
                }
                eq_processor->start_attack();
                chorus_modulation_env->start_attack();
                chorus_shine_env->start_attack();
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
void MONOVoice::render_block (mono_AudioSampleBuffer<4>& buffer_, int step_number_, int start_sample_, int num_samples_) {
    mono_AmpPainter* amp_painter = MONOVoice::get_amp_painter();

    const int num_samples = num_samples_;
    if( num_samples == 0 )
        return;

    if( step_number_ != -1 )
        current_step = step_number_;

    // FILTER ENV - MUST BE FINISHED BEFORE LFOS
    for( int flt_id = 0 ; flt_id != SUM_FILTERS ; ++flt_id )
        filter_envs[flt_id]->process( data_buffer->filtered_env_amps.getWritePointer(flt_id), num_samples );

    // LFOS - THREAD 1 ?
    for( int lfo_id = 0 ; lfo_id < SUM_LFOS ; ++lfo_id ) {
        lfos[lfo_id]->process( *data_buffer, step_number_, num_samples );
    }

    // OSCs - THREAD 1 ?
    for( int osc_id = 0 ; osc_id < SUM_OSCS ; ++osc_id )
        oscs[osc_id]->process( data_buffer, num_samples );


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

    // FILTER PROCESSING
    for( int flt_id = 0 ; flt_id != SUM_FILTERS ; ++flt_id )
        filter_processors[flt_id]->process( *data_buffer, num_samples );

    if( amp_painter )
        for( int filter_id = 0 ; filter_id != SUM_FILTERS ; ++filter_id )
            for( int i = 0 ; i != num_samples ; ++i )
                amp_painter->add_filter( filter_id, data_buffer->direct_filter_output_samples.getReadPointer(filter_id)[i] );

    // COLLECT RESULTS TO CHANNEL 0
    float* output_buffer = data_buffer->direct_filter_output_samples.getWritePointer(0);
    FloatVectorOperations::add( output_buffer, data_buffer->direct_filter_output_samples.getReadPointer(1), num_samples );
    FloatVectorOperations::add( output_buffer, data_buffer->direct_filter_output_samples.getReadPointer(2), num_samples );

    // EQ
    for( int band_id = 0 ; band_id != SUM_EQ_BANDS ; ++band_id )
    {
        ENV*const env = eq_processor->envs.getUnchecked( band_id );
        env->process( data_buffer->eq_ar_amps.getWritePointer(band_id), num_samples_ );
    }
    eq_processor->process( *data_buffer, num_samples );

    velocity_glide = current_velocity;
    if( synth_data.arp_sequencer_data->is_on )
        velocity_glide = current_velocity * synth_data.arp_sequencer_data->velocity[current_step];
    velocity_glide.update( synth_data.velocity_glide_time );

    // FINAL ENV
    const float* samples = data_buffer->direct_filter_output_samples.getReadPointer(0);
    float* final_output_buffer =  buffer_.getWritePointer(0);
    for( int sid = 0 ; sid != num_samples ; ++sid )
        final_output_buffer[start_sample_+sid] = samples[sid];

    // UI INFORMATIONS
    for( int i = 0 ; i != SUM_OSCS ; ++i )
        synth_data.osc_datas[i]->last_modulation_value = data_buffer->lfo_amplitudes.getReadPointer(i)[num_samples-1];

    // CLEAR
    release_if_inactive();
}

void MONOVoice::process_final_env( int num_samples ) noexcept {
    env->process( data_buffer->env_amp.getWritePointer(0), num_samples ); // USED IN THE PROCESS BLOCK
}
void MONOVoice::process_effect_env( int num_samples ) noexcept {
    chorus_modulation_env->process( data_buffer->chorus_modulation_env_amp.getWritePointer(0), num_samples ); // USED IN THE PROCESS BLOCK
}

float MONOVoice::get_filter_env_amp( int filter_id_ ) const noexcept {
    return filter_envs[filter_id_]->get_amp();
}

float MONOVoice::get_lfo_amp( int lfo_id_ ) const noexcept {
    return lfos[lfo_id_]->get_current_amp();
}
float MONOVoice::get_arp_sequence_amp( int step_ ) const noexcept {
    if( arp_sequencer->get_current_step() == step_ )
        return env->get_amp();

    return 0;
}

void MONOVoice::pitchWheelMoved (int) {}
void MONOVoice::controllerMoved (int, int ) {}
//==============================================================================
NOINLINE mono_ParameterOwnerStore::~mono_ParameterOwnerStore()
{
    // this ensures that no dangling pointers are left when the
    // singleton is deleted.
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
    return mono_ParameterOwnerStore::getInstance()->voice->chorus_modulation_env->get_amp();
}
float mono_ParameterOwnerStore::get_chorus_shine_env_amp() {
    return mono_ParameterOwnerStore::getInstance()->voice->chorus_shine_env->get_amp();
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




































































































