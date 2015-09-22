#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "App_h_includer.h"
#include "mono_AudioDeviceManager.h"

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
class MoniqueSynthData;
class Monique_Ui_SegmentedMeter;
class MoniqueSynthesiserVoice;
class ClockSmoothBuffer;
class ArpInfo;
class NoteDownStore;
class MoniqueSynthesizer;

class MoniqueAudioProcessor :
    public AudioProcessor,
    public MidiKeyboardState,
    public MidiKeyboardStateListener,
    public mono_AudioDeviceManager
#ifdef IS_PLUGIN
    ,
public ParameterListener
#else
    ,
public Timer
#endif
{
#ifdef IS_STANDALONE
public:
    AudioProcessorPlayer player;
    bool audio_is_successful_initalized;
private:
    ClockSmoothBuffer* clock_smoother;
    int64 last_clock_sample;
    bool try_to_restart_arp;

    bool received_a_clock_in_time;
    bool connection_missed_counter;
    void timerCallback() override;
#endif

    // ==============================================================================
    // DATA & SYNTH PROCESSOR
    MoniqueSynthData* synth_data;
    MoniqueSynthesiserVoice* voice;
    MoniqueSynthesizer* synth;

    // ==============================================================================
    // UI
    Monique_Ui_SegmentedMeter* peak_meter;
    CriticalSection peak_meter_lock;
public:
    void set_peak_meter( Monique_Ui_SegmentedMeter*peak_meter_ ) noexcept;
    void clear_preak_meter() noexcept;

private:
    // ==============================================================================
    // PROCESS
    AudioPlayHead::CurrentPositionInfo current_pos_info;
    void processBlock ( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_ ) override;
    COLD void prepareToPlay ( double sampleRate, int samplesPerBlock ) override;
    COLD void releaseResources() override;
    COLD void reset() override;
public:
    inline const AudioPlayHead::CurrentPositionInfo& get_current_pos_info() const noexcept
    {
        return current_pos_info;
    }

private:
    //==========================================================================
    // MIDI KEYBOARD
    ScopedPointer< NoteDownStore > note_down_store;
public:
    bool are_more_than_one_key_down() const noexcept;
private:
    void handleNoteOn (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber) override;

    // ==============================================================================
    // MIDI
    void trigger_send_feedback() noexcept;
    void trigger_send_clear_feedback() noexcept;

    // ==============================================================================
    /// AUTOMATION PARAMETERS
#ifdef IS_PLUGIN
    Array< Parameter* > automateable_parameters;
    void init_automatable_parameters() noexcept;

    int getNumParameters() override;
    // bool isParameterAutomatable (int parameterIndex) const override;
    float getParameter ( int index_ ) override;
    void setParameter ( int index_, float value_ ) override;
    const String getParameterName ( int index_ ) override;
    const String getParameterText ( int index_ ) override;
    String getParameterLabel (int index) const override;
    int getParameterNumSteps (int index_ ) override;
    float getParameterDefaultValue (int index_) override;

    void parameter_value_changed( Parameter* ) noexcept override;
    void parameter_value_changed_always_notification( Parameter* ) noexcept override;
    void parameter_value_on_load_changed( Parameter* ) noexcept override;
    void parameter_modulation_value_changed( Parameter* ) noexcept override;
#endif

    //==========================================================================
    // LOAD SAVE
#ifdef IS_PLUGIN
    void getStateInformation ( MemoryBlock& dest_data_ ) override;
    void setStateInformation ( const void* data_, int size_in_bytes_ ) override;
#else
    void getStateInformation ( MemoryBlock& dest_data_ ) override {}
    void setStateInformation ( const void* data_, int size_in_bytes_ ) override {}
#endif

    //==========================================================================
    // CONFIG
    bool hasEditor() const  override;
    const String getName() const  override;
    const String getInputChannelName ( int channel_index_ ) const  override;
    const String getOutputChannelName ( int channel_index_ ) const  override;
    bool isInputChannelStereoPair ( int index_ ) const  override;
    bool isOutputChannelStereoPair ( int index_ ) const  override;
    bool acceptsMidi() const  override;
    bool producesMidi() const  override;
    bool silenceInProducesSilenceOut() const  override;
    double getTailLengthSeconds() const  override;

    //==========================================================================
    // PROGRAM HANDLIG (PLUGIN ONLY)
    int getNumPrograms()  override;
    int getCurrentProgram()  override;
    void setCurrentProgram ( int index_ )  override;
    const String getProgramName ( int index_ )  override;
    void changeProgramName ( int index_, const String& name_ )  override;

    //==========================================================================
    // BOOT UI
    COLD AudioProcessorEditor* createEditor()  override;

    //==========================================================================
    friend AudioProcessor* JUCE_CALLTYPE createPluginFilter();
    friend class ContainerDeletePolicy< MoniqueAudioProcessor >;
    COLD MoniqueAudioProcessor() noexcept;
    COLD ~MoniqueAudioProcessor() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ( MoniqueAudioProcessor )
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
