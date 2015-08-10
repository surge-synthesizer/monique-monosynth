/*
  ==============================================================================

    Synth.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONOSYNTH___H_INCLUDED
#define MONOSYNTH___H_INCLUDED

#include "App_h_includer.h"
#include "SynthData.h"

//==============================================================================
//==============================================================================
//==============================================================================
static inline float soft_clipping( float input_and_worker_ )
{
    //sample_ -= (sample_*sample_*sample_)/3.0f;
    input_and_worker_ = (std::atan(input_and_worker_)/float_Pi)*2;

    // TODO can be done by float vector
    if( input_and_worker_ > 1 )
        input_and_worker_ = 1;
    else if( input_and_worker_ < -1 )
        input_and_worker_ = -1;

    return input_and_worker_;
}
#define HARD_CLIPPER_STAGE_1 1.1f
#define HARD_CLIPPER_STAGE_2 1.25f

static inline float protection_clipping( float input_and_worker_ ) noexcept
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

static inline float wave_mixer_v2( float s_, float s2_ ) noexcept
{
    if ((s_ > 0) && (s2_ > 0))
    {
        s_ += (s2_ - (s_ * s2_));
    }
    else if ((s_ < 0) && (s2_ < 0))
    {
        s_ += (s2_ + (s_ * s2_));
    }
    else
    {
        s_ += s2_;
    }

    return s_;
}

static inline float sample_mixer_v2( float s_, float s2_ ) noexcept
{
    return s_ + s2_;
}

static inline void final_mix( float* dest_buffer,
                              float* s1_buffer,
                              float* s2_buffer,
                              int num_samples_ )
{
    for( int i = 0 ; i != num_samples_ ; ++ i )
        dest_buffer[i] = soft_clipping(s1_buffer[i]+s2_buffer[i]);
}
static inline void final_mix( float* dest_buffer,
                              float* s1_buffer,
                              int num_samples_ )
{
    for( int i = 0 ; i != num_samples_ ; ++ i )
        dest_buffer[i] = soft_clipping(s1_buffer[i]);
}

static inline float sample_mixer_v2( float* target_buffer,
                                     const float* s1_buffer,
                                     const float* s2_buffer,
                                     int num_samples_ ) {
    for( int i = 0 ; i != num_samples_ ; ++ i )
        target_buffer[i] = sample_mixer_v2(s1_buffer[i],s2_buffer[i]);

    return 0;
}

#define RUNTIME_CLASS_ID -1

//==============================================================================
//==============================================================================
//==============================================================================
#define AMP_SMOOTH_SIZE 100 // TODO as option and based to sample rate
class AmpSmoothBuffer : RuntimeListener {
    int pos;
    float buffer[AMP_SMOOTH_SIZE];
    float sum;

public:
    inline void add( float val_ ) noexcept;
    inline float add_and_get_average( float val_ ) noexcept;
    inline float get_average() const noexcept;

private:
    virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept override {
      // TODO resize buffer
    };

public:
    NOINLINE AmpSmoothBuffer();
    NOINLINE ~AmpSmoothBuffer();
};
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
    return sum / AMP_SMOOTH_SIZE;
}

//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================
#include "PluginProcessor.h"
class MonoSynthSound : public SynthesiserSound {
    bool appliesToNote (int /*midiNoteNumber*/) override;
    bool appliesToChannel (int /*midiChannel*/) override;
public:
    NOINLINE MonoSynthSound();
    NOINLINE ~MonoSynthSound();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoSynthSound)
};

//==============================================================================
#define PERFORMANCE_TEST
class LFO;
class OSC;
class ENV;
class FilterProcessor;
class EQProcessor;
class DataBuffer;
class ArpSequencer;
class mono_AmpPainter;
class MONOVoice : public SynthesiserVoice
{
public:
    static void create_amp_painter();
    static mono_AmpPainter* get_amp_painter();
    static mono_AmpPainter* get_lock_amp_painter();
    static void unlock_amp_painter();
    static void kill_amp_painter();
private:
    RuntimeInfo info;

    GstepAudioProcessor*const audio_processor;
// TODO VELOCITY GLIDE
//==============================================================================
    SynthData& synth_data;

    ScopedPointer<DataBuffer> data_buffer;

    OwnedArray< OSC > oscs;
    OwnedArray< LFO > lfos;
    ScopedPointer<ENV> env;
    ScopedPointer<ArpSequencer> arp_sequencer;
    friend class mono_ParameterOwnerStore;
    OwnedArray< FilterProcessor > filter_processors;
    ScopedPointer< EQProcessor > eq_processor;
    OwnedArray< ENV > filter_envs;
    ScopedPointer< ENV > chorus_modulation_env;
    ScopedPointer< ENV > chorus_shine_env;

    bool is_stopped;
    bool was_arp_started;
    int current_note;
    float current_velocity;
public:
    mono_GlideValue<0,1> velocity_glide;
    void process_final_env( int num_samples ) noexcept;
    void process_effect_env( int num_samples ) noexcept;
private:
    int current_step;

    void renderNextBlock (mono_AudioSampleBuffer<4>& , int startSample, int numSamples) override;
    void render_block (mono_AudioSampleBuffer<4>& , int step_number_, int startSample, int numSamples);
    void release_if_inactive() noexcept;
    bool is_active() const noexcept;

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void start_internal( int midiNoteNumber, float velocity ) noexcept;

    void noteOff();
    void stopNote (float, bool allowTailOff) override;

    int getCurrentlyPlayingNote() const noexcept override;

    void pitchWheelMoved (int /*newValue*/) override;
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override;

    //==============================================================================
public:
    // UI INFOS
    float get_filter_env_amp( int filter_id_ ) const noexcept;
    float get_lfo_amp( int lfo_id_ ) const noexcept;
    float get_arp_sequence_amp( int step_ ) const noexcept;
    float get_current_frequency() const noexcept;

    const DataBuffer& get_data_buffer() const {
        return *data_buffer;
    }
    DataBuffer& get_data_buffer() {
        return *data_buffer;
    }

    NOINLINE MONOVoice( GstepAudioProcessor*const audio_processor_ );
    NOINLINE ~MONOVoice();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MONOVoice)
};

#endif
