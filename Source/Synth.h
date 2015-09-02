#ifndef MONOSYNTH___H_INCLUDED
#define MONOSYNTH___H_INCLUDED

#include "App_h_includer.h"

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueSynthesiserSound : public SynthesiserSound
{
    bool appliesToNote (int) override;
    bool appliesToChannel (int) override;
    
public:
    NOINLINE MoniqueSynthesiserSound() noexcept;
    NOINLINE ~MoniqueSynthesiserSound() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesiserSound)
};

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueAudioProcessor;

class SynthData;
class RuntimeInfo;
class LFO;
class OSC;
class ENV;
class FilterProcessor;
class EQProcessor;
class FXProcessor;
class DataBuffer;
class ArpSequencer;

class MoniqueSynthesiserVoice : public SynthesiserVoice
{
    //==============================================================================
    MoniqueAudioProcessor*const audio_processor;
    SynthData*const synth_data;
    
    //==============================================================================
    RuntimeInfo*const info;
    DataBuffer*const data_buffer;

    friend class mono_ParameterOwnerStore;
    ArpSequencer*const arp_sequencer;
    EQProcessor*const eq_processor;
    FXProcessor*const fx_processor;
    
    //==============================================================================
    OSC** oscs;
    LFO** lfos;
    FilterProcessor** filter_processors;

    //==============================================================================
    bool is_stopped;
    bool was_arp_started;
    int current_note;
    float current_velocity;
    int current_step;

    //==============================================================================
    void renderNextBlock( AudioSampleBuffer&, int startSample, int numSamples) override;
    void render_block( AudioSampleBuffer&, int step_number_, int startSample, int numSamples) noexcept;
    void release_if_inactive() noexcept;

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void start_internal( int midiNoteNumber, float velocity ) noexcept;
    void stopNote(float, bool allowTailOff) override;
    int getCurrentlyPlayingNote() const noexcept override;
    void pitchWheelMoved (int /*newValue*/) override;
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override;

public:
    //==============================================================================
    // UI INFOS
    float get_filter_env_amp( int filter_id_ ) const noexcept;
    float get_lfo_amp( int lfo_id_ ) const noexcept;
    float get_arp_sequence_amp( int step_ ) const noexcept;
    float get_current_frequency() const noexcept;

public:
    //==============================================================================
    NOINLINE MoniqueSynthesiserVoice( MoniqueAudioProcessor*const audio_processor_, SynthData*const synth_data_ ) noexcept;
    NOINLINE ~MoniqueSynthesiserVoice() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesiserVoice)
};

#endif
