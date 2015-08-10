#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "App_h_includer.h"
#include "mono_AudioDeviceManager.h"

// ********************************************************************************************
// ********************************************************************************************
// ********************************************************************************************
class SynthData;
class SegmentedMeter;
class MONOVoice;
class DATAINProcessor;
template<typename,int>
class CircularBuffer;
#define type_CLOCK_SMOTH_BUFFER CircularBuffer< double, 12 >
class GstepAudioProcessor :
    public AudioProcessor,
    public MidiKeyboardState,
    public MidiKeyboardStateListener,
    public mono_AudioDeviceManager,
    public RuntimeListener
{
    /// MIDI IO
    // TODO not need for plugin
#ifdef IS_STANDALONE
    double last_clock_sample;
    double last_start_sample;
    ScopedPointer<type_CLOCK_SMOTH_BUFFER> clock_smoth_buffer;
#endif
public:
#ifdef IS_STANDALONE
    void handle_extern_midi_start( const MidiMessage& message ) noexcept override;
    void handle_extern_midi_stop( const MidiMessage& message ) noexcept override;
    void handle_extern_midi_continue( const MidiMessage& message ) noexcept override;
    void handle_extern_midi_clock( const MidiMessage& message ) noexcept override;
#endif
    void handle_extern_note_input( const MidiMessage& message ) noexcept override;
    void handle_extern_cc_input( const MidiMessage& message ) noexcept override;
    void trigger_send_feedback() noexcept override;
    void trigger_send_clear_feedback() noexcept override;

    /// PROCESS
    AudioPlayHead::CurrentPositionInfo current_pos_info;
private:
    DATAINProcessor* data_in_processor;
    void processBlock ( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_ ) override;
    void prepareToPlay ( double sampleRate, int samplesPerBlock ) override;
    void releaseResources() override;
    void reset() override;

    /// AUTOMATION PARAMETERS
    int getNumParameters() override;
    bool isParameterAutomatable (int parameterIndex) const override;
    float getParameter ( int index_ ) override;
    void setParameter ( int index_, float value_ ) override;
    const String getParameterName ( int index_ ) override;
    const String getParameterText ( int index_ ) override;
    String getParameterLabel (int index) const override;
    int getParameterNumSteps (int index_ ) override;
    float getParameterDefaultValue (int index_) override;

private:
    /// LOAD AND SAVE
    void getStateInformation ( MemoryBlock& dest_data_ ) override;
    void setStateInformation ( const void* data_, int size_in_bytes_ ) override;

private:
    /// SETUP
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

    /// PROGRAMMS - NOT SUPPORTED YET

    int getNumPrograms()  override;
    int getCurrentProgram()  override;
    void setCurrentProgram ( int index_ )  override;
    const String getProgramName ( int index_ )  override;
    void changeProgramName ( int index_, const String& name_ )  override;

    // --------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------

    AudioProcessorEditor* createEditor()  override;

public:
    GstepAudioProcessor();
#ifdef IS_STANDALONE
    AudioProcessorPlayer player;
    void init_audio();
#endif

    ~GstepAudioProcessor();

private:
    Array< MidiMessage > user_keyboard_messages;
    virtual void handleNoteOn (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber, float velocity) {
        synth.noteOn( midiChannel,midiNoteNumber,velocity );
    }
    virtual void handleNoteOff (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber) {
        synth.noteOff( midiChannel,midiNoteNumber,0, true );
    }

public:
    // TODO privates!!!:
    AudioPlayHead::CurrentPositionInfo pos;

    // SYNTH
    MONOVoice* voice;
    ScopedPointer<SynthData> synth_data;
    Synthesiser synth;

    SegmentedMeter* peak_meter;
    bool repaint_peak_meter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ( GstepAudioProcessor )
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
