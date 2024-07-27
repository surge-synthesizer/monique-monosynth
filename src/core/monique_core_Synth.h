/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
*/

#ifndef MONOSYNTH___H_INCLUDED
#define MONOSYNTH___H_INCLUDED

#include "App.h"

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueSynthesiserSound : public juce::SynthesiserSound
{
    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;

  public:
    COLD MoniqueSynthesiserSound() noexcept;
    COLD ~MoniqueSynthesiserSound() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MoniqueSynthesiserSound)
};

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueAudioProcessor;

struct MoniqueSynthData;
struct RuntimeInfo;
class LFO;
class MFO;
class SecondOSC;
class MasterOSC;
class ENV;
class FilterProcessor;
class EQProcessorStereo;
class FXProcessor;
class DataBuffer;
class ArpSequencer;
class SmoothManager;
class RuntimeNotifyer;
class MoniqueSynthesizer;

#define TABLESIZE_MULTI 1000
// #define LOOKUP_TABLE_SIZE int(float_Pi*TABLESIZE_MULTI*2)
static const int LOOKUP_TABLE_SIZE = int(juce::MathConstants<float>::twoPi * TABLESIZE_MULTI);

class MoniqueSynthesiserVoice : public juce::SynthesiserVoice
{
    //==============================================================================
    MoniqueAudioProcessor *const audio_processor;
    MoniqueSynthData *const synth_data;

    //==============================================================================
    RuntimeInfo *const info;
    DataBuffer *const data_buffer;

    friend class mono_ParameterOwnerStore;

  public:
    ArpSequencer *const arp_sequencer;
    EQProcessorStereo *const eq_processor;

  public:
    int samples_must_be_zero_before_bypass;
    LinearSmootherMinMax<false, true> bypass_smoother;

  private:
    FXProcessor *const fx_processor;

    //==============================================================================
    MasterOSC *master_osc;
    SecondOSC *second_osc;
    SecondOSC *third_osc;
    LFO **lfos;
    LFO **mfos;
    FilterProcessor **filter_processors;
#ifdef POLY
    ENV **filter_volume_tracking_envs;
#endif

    //==============================================================================
    friend MoniqueSynthesizer;
    int current_note;
    float pitch_offset;
    bool is_sostenuto_pedal_down;
    bool stopped_and_sostenuto_pedal_was_down;
    bool is_soft_pedal_down;
    bool was_soft_pedal_down_on_note_start;
    bool is_sustain_pedal_down;
    bool stopped_and_sustain_pedal_was_down;
    float current_velocity;
    int current_step;
    int current_running_arp_step;
    bool an_arp_note_is_already_running;
    int sample_position_for_restart_arp;

    //==============================================================================
    bool canPlaySound(juce::SynthesiserSound *) override { return true; }

  public:
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *,
                   int /*currentPitchWheelPosition*/) override;

  private:
    void start_internal(int midiNoteNumber, float velocity, int sample_number_,
                        bool is_human_event_, bool trigger_envelopes_ = true,
                        bool isNoteOff = false) noexcept;

  public:
    void stop_controlled(const juce::MidiMessage &message_, int sample_pos_);
    void stop_arp_controlled(bool force = false);
    void stopNote(float, bool allowTailOff) override;

  public:
    void stop_internal() noexcept;

  private:
    void release_if_inactive() noexcept;

  private:
    friend struct MoniqueSynthData;
    void *note_down_store;
    void set_note_down_store(void *note_down_store_) noexcept
    {
        note_down_store = note_down_store_;
    }

  public:
  private:
    void renderNextBlock(juce::AudioSampleBuffer &, int startSample, int numSamples) override;
    void render_block(juce::AudioSampleBuffer &, int step_number_, int absolute_step_number_,
                      int startSample, int numSamples) noexcept;

    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override {
    } // see synthesizer
  public:
    //==============================================================================
    int get_current_note() const noexcept;
    float get_current_velocity() const noexcept;
    void reset(bool force = false) noexcept;
    void reset_internal() noexcept;
    void handle_sustain_pedal(bool down_) noexcept;
    void handle_sostueno_pedal(bool down_) noexcept;
    void handle_soft_pedal(bool down_) noexcept;
    void clear_record_buffer() noexcept;

  public:
    //==============================================================================
    // UI INFOS
    float get_filter_env_amp(int filter_id_) const noexcept;
    float get_lfo_amp(int lfo_id_) const noexcept;
    float get_mfo_amp(int mfo_id_) const noexcept;
    float get_arp_sequence_amp(int step_) const noexcept;
    float get_current_frequency() const noexcept;

  public:
    //==============================================================================
    COLD MoniqueSynthesiserVoice(MoniqueAudioProcessor *const audio_processor_,
                                 MoniqueSynthData *const synth_data_,
                                 RuntimeNotifyer *const notifyer_, RuntimeInfo *const info_,
                                 DataBuffer *data_buffer_) noexcept;
    COLD ~MoniqueSynthesiserVoice() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MoniqueSynthesiserVoice)
};

//==================================================================================
inline int MoniqueSynthesiserVoice::get_current_note() const noexcept { return current_note; }
inline float MoniqueSynthesiserVoice::get_current_velocity() const noexcept
{
    return current_velocity;
}

//==============================================================================
//==============================================================================
//==============================================================================
class MIDIControlHandler;
struct MoniqueSynthData;
class MoniqueSynthesizer : public juce::Synthesiser
{
    MIDIControlHandler *const midi_control_handler;

    MoniqueSynthData *const synth_data;
    MoniqueSynthesiserVoice *const voice;

    int program_chnage_counter = -3;

    void handleSustainPedal(int midiChannel, bool isDown) override;
    void handleSostenutoPedal(int midiChannel, bool isDown) override;
    void handleSoftPedal(int midiChannel, bool isDown) override;

    void handleBankSelect(int controllerValue) noexcept;
    void handleProgramChange(int midiChannel, int programNumber) override;
    void handleController(int midiChannel, int controllerNumber, int controllerValue) override;
    void handlePitchWheel(int midiChannel, int wheelValue) override;

  public:
    struct NoteDownStore
    {
        MoniqueSynthData *const synth_data;

        struct MidiMessageCompareable : public juce::MidiMessage
        {
            bool operator==(const juce::MidiMessage &other_) const noexcept
            {
                if (other_.isNoteOn())
                {
                    if (other_.getNoteNumber() == getNoteNumber())
                    {
                        return true;
                    }
                }

                return false;
            }

            MidiMessageCompareable(const juce::MidiMessage &message_) noexcept
                : juce::MidiMessage(message_)
            {
            }
            MidiMessageCompareable() noexcept {}
        };

        juce::Array<MidiMessageCompareable> notes_down; // 0 will be no note or off
        juce::Array<MidiMessageCompareable *> notes_down_order;

        //==============================================================================
        void add_note(const juce::MidiMessage &midi_message_, int play_mode_) noexcept;
        // Returns a replacement, if exist
        const juce::MidiMessage *remove_note(const juce::MidiMessage &midi_message_, int play_mode_,
                                             bool reorder_allowed_) noexcept;
        MidiMessageCompareable *get_replacement(const juce::MidiMessage &message_, int play_mode_,
                                                int index_) noexcept;
        int size() const noexcept { return notes_down.size(); }
        bool is_empty() const noexcept;
        const juce::MidiMessage *get_last() const noexcept;
        const int get_id(const juce::MidiMessage &message_) const noexcept;
        const int get_id(int note_number_) const noexcept;
        // can be nullptr if nothing is on
        const juce::MidiMessage *get_at(int index_) const noexcept;
        void swap(int index_a_, int index_b_) noexcept;

        //==============================================================================
        void reset() noexcept;

        //==============================================================================
        COLD NoteDownStore(MoniqueSynthData *const synth_data_) noexcept;
        COLD ~NoteDownStore() noexcept;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteDownStore)
    };

  public:
    void render_next_block(juce::AudioBuffer<float> &outputAudio, const juce::MidiBuffer &inputMidi,
                           int startSample, int numSamples) noexcept;
    void render_next_block(juce::AudioBuffer<double> &outputAudio,
                           const juce::MidiBuffer &inputMidi, int startSample,
                           int numSamples) noexcept;

    void reset_note_down_store() { note_down_store.reset(); }

  private:
    NoteDownStore note_down_store;

  private:
    void process_next_block(juce::AudioBuffer<float> &outputAudio,
                            const juce::MidiBuffer &inputMidi, int startSample, int numSamples);
    void handle_midi_event(const juce::MidiMessage &m, int pos_in_buffer_);

  public:
    COLD juce::SynthesiserVoice *addVoice(juce::SynthesiserVoice *newVoice) noexcept;
    COLD juce::SynthesiserSound *addSound(const juce::SynthesiserSound::Ptr &sound_) noexcept;

  public:
    COLD MoniqueSynthesizer(MoniqueSynthData *const synth_data_, MoniqueSynthesiserVoice *voice_,
                            const juce::SynthesiserSound::Ptr &sound_,
                            MIDIControlHandler *const midi_control_handler_) noexcept
        : midi_control_handler(midi_control_handler_), synth_data(synth_data_), voice(voice_),
          note_down_store(synth_data_)
    {
        juce::Synthesiser::addVoice(voice_);
        juce::Synthesiser::addSound(sound_);
        voice_->set_note_down_store(&note_down_store);
    }
    COLD ~MoniqueSynthesizer() noexcept {}
};

#endif
