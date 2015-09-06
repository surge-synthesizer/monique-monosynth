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
    COLD MoniqueSynthesiserSound() noexcept;
    COLD ~MoniqueSynthesiserSound() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesiserSound)
};

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueAudioProcessor;

class MoniqueSynthData;
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
    MoniqueSynthData*const synth_data;

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
    int current_note;
    float current_velocity;
    int current_step;
    bool an_arp_note_is_already_running;

    //==============================================================================
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void start_internal( int midiNoteNumber, float velocity ) noexcept;
    void stopNote(float, bool allowTailOff) override;
    void stop_internal() noexcept;
    void release_if_inactive() noexcept;
    void reset() noexcept;
    
    void renderNextBlock( AudioSampleBuffer&, int startSample, int numSamples) override;
    void render_block( AudioSampleBuffer&, int step_number_, int startSample, int numSamples) noexcept;
    
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
    float get_flt_input_env_amp( int flt_id_, int input_id_ ) const noexcept;
    float get_band_env_amp( int band_id_ ) const noexcept;
    float get_chorus_modulation_env_amp() const noexcept;

public:
    //==============================================================================
    COLD MoniqueSynthesiserVoice( MoniqueAudioProcessor*const audio_processor_, MoniqueSynthData*const synth_data_ ) noexcept;
    COLD ~MoniqueSynthesiserVoice() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesiserVoice)
};

#endif

