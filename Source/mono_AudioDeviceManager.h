#ifndef MONO_AUDIODEVICEMANAGER_H_INCLUDED
#define MONO_AUDIODEVICEMANAGER_H_INCLUDED

#include "App_h_includer.h"
#include "monique_core_Datastructures.h"

#define CLOSED_PORT "CLOSED"

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class mono_AudioDeviceManager : public AudioDeviceManager, public RuntimeListener
{
public:
    BoolParameter main_input_thru;
    BoolParameter cc_input_thru;
    BoolParameter use_main_input_as_cc;
    IntParameter input_channel;

    //==========================================================================
    enum INPUT_ID
    {
        CC,
        NOTES
    };
    //==========================================================================
    enum OUTPUT_ID
    {
        THRU,
        FEEDBACK
    };
    //==========================================================================
    enum DEVICE_STATE
    {
        OPEN,
        CLOSED,
        REMOVED,
        ERROR
    };

private:
    //==========================================================================
    //==========================================================================
    //==========================================================================
    // INPUT
    class AdvancedMidiInputCallback : public MidiInputCallback
    {
        String device_name;
    protected:
        mono_AudioDeviceManager*const manager;
    public:
        void set_device_name( const String& name_ ) noexcept;
        const String& get_device_name() const noexcept;
    protected:
        COLD AdvancedMidiInputCallback( mono_AudioDeviceManager*manager_ ) noexcept;
        COLD ~AdvancedMidiInputCallback() noexcept;
    };

    //==========================================================================
    class MidiInputCallback_CC : public AdvancedMidiInputCallback
    {
        void handleIncomingMidiMessage (MidiInput*, const MidiMessage& message) override;
    public:
        COLD MidiInputCallback_CC( mono_AudioDeviceManager*manager_ ) noexcept;
        COLD ~MidiInputCallback_CC() noexcept;
    }
    *const cc_input_callback;

    //==========================================================================
    class MidiInputCallback_NOTES : public AdvancedMidiInputCallback
    {
        void handleIncomingMidiMessage (MidiInput*, const MidiMessage& message) override;
    public:
        COLD MidiInputCallback_NOTES( mono_AudioDeviceManager*manager_ ) noexcept;
        COLD ~MidiInputCallback_NOTES() noexcept;
    }
    *const note_input_callback;
    DEVICE_STATE note_input_state,cc_input_state;

    //==========================================================================
    //==========================================================================
    //==========================================================================
    MidiMessageCollector cc_input_collector;
    MidiMessageCollector note_input_collector;
    MidiMessageCollector sync_input_collector;
protected:
    void collect_incoming_midi_messages ( INPUT_ID input_id_, const MidiMessage& midi_message_ ) noexcept;

protected:
    //==========================================================================
    //==========================================================================
    //==========================================================================
    inline void get_cc_input_messages( MidiBuffer& midi_messages_, int num_samples_ ) noexcept
    {
        cc_input_collector.removeNextBlockOfMessages( midi_messages_, num_samples_ );
    }
    inline void get_note_input_messages( MidiBuffer& midi_messages_, int num_samples_ ) noexcept
    {
        note_input_collector.removeNextBlockOfMessages( midi_messages_, num_samples_ );
    }
    inline void get_sync_input_messages( MidiBuffer& midi_messages_, int num_samples_ ) noexcept
    {
        sync_input_collector.removeNextBlockOfMessages( midi_messages_, num_samples_ );
    }

private:
    AdvancedMidiInputCallback* get_input_device_callback( INPUT_ID input_id_ ) const noexcept;
public:
    COLD StringArray get_available_in_ports() const noexcept;
    COLD void open_in_port( INPUT_ID input_id_, const String& device_name_ ) noexcept;
    COLD void close_in_port( INPUT_ID input_id_ ) noexcept;
    COLD String get_selected_in_device( INPUT_ID input_id_ ) const noexcept;
    COLD bool is_selected_in_device_open( INPUT_ID output_id_ ) const noexcept;
    COLD DEVICE_STATE get_selected_in_device_state( INPUT_ID output_id_ ) const noexcept;

    //==========================================================================
    // OUTPUT
private:
    MidiMessageCollector cc_feedback_collector;
    MidiMessageCollector thru_collector;
    MidiOutput *midi_thru_output, *midi_feedback_output;
    DEVICE_STATE midi_thru_output_state, midi_feedback_output_state;
    String midi_thru_name, midi_feedback_name;

public:
    void send_thru_messages( int num_samples_ ) noexcept;
    void send_feedback_messages( int num_samples_ ) noexcept;

private:
    //==========================================================================
    MidiOutput* get_output_device( OUTPUT_ID output_id_ ) const noexcept;

public:
    COLD StringArray get_available_out_ports() const noexcept;
    COLD bool open_out_port( OUTPUT_ID output_id_, const String& device_name_ ) noexcept;
    COLD void close_out_port( OUTPUT_ID output_id_ ) noexcept;
    COLD String get_selected_out_device( OUTPUT_ID output_id_ ) const noexcept;
    COLD bool is_selected_out_device_open( OUTPUT_ID output_id_ ) const noexcept;
    COLD DEVICE_STATE get_selected_out_device_state( OUTPUT_ID output_id_ ) const noexcept;

public:
    // SEND
    inline void send_feedback_message( int cc_number_, int cc_value_ ) noexcept;
    inline void clear_feedback_message( int cc_number_ ) noexcept;

private:
    COLD void sample_rate_changed( double /* old_sr_ */ ) noexcept override;

    class OpenStateChecker : public Timer
    {
        friend class mono_AudioDeviceManager;
        mono_AudioDeviceManager*const manager;
	
	StringArray last_in_devices;
	StringArray last_out_devices;

        volatile bool force_quit;

        bool ignore_note_input;
        bool ignore_cc_input;
        bool ignore_thru_output;
        bool ignore_cc_output;

        bool was_open_note_input;
        bool was_open_cc_input;
        bool was_open_thru_output;
        bool was_open_cc_output;

        bool connection_lost_note_input;
        bool connection_lost_cc_input;
        bool connection_lost_thru_output;
        bool connection_lost_cc_output;

        COLD void timerCallback() override;

        COLD OpenStateChecker( mono_AudioDeviceManager*const manager_ ) noexcept;
        COLD ~OpenStateChecker() noexcept;
    } open_state_checker;
    friend class OpenStateChecker;
    
    int state_change_counter;
    
public:
    int get_state_change_counter() const noexcept
    {
      return state_change_counter;
    }
    
protected:
    COLD mono_AudioDeviceManager() noexcept;
    COLD ~mono_AudioDeviceManager() noexcept;
    void clear_feedback() noexcept;
    void clear_feedback_and_shutdown() noexcept;

    COLD bool save_to( XmlElement* xml ) const noexcept;
    COLD String read_from( const XmlElement* xml ) noexcept;
    COLD String read_defaults() noexcept;
    COLD void save() const noexcept;
    COLD String read() noexcept;
};

inline void mono_AudioDeviceManager::send_feedback_message( int cc_number_, int cc_value_ ) noexcept
{
    if( midi_feedback_output )
    {
        MidiMessage message = MidiMessage::controllerEvent( 1, cc_number_, cc_value_ );
        message.setTimeStamp( Time::getMillisecondCounterHiRes() );
        cc_feedback_collector.addMessageToQueue( message );
    }
}
inline void mono_AudioDeviceManager::clear_feedback_message( int cc_number_ ) noexcept
{
    if( midi_feedback_output )
    {
        midi_feedback_output->sendMessageNow( MidiMessage::controllerEvent( 1, cc_number_, 0 ) );
    }
}
#endif  // MONO_AUDIODEVICEMANAGER_H_INCLUDED
