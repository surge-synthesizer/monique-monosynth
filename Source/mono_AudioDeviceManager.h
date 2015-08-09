/*
  ==============================================================================

    mono_AudioDeviceManager.h
    Created: 21 May 2015 12:20:29pm
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONO_AUDIODEVICEMANAGER_H_INCLUDED
#define MONO_AUDIODEVICEMANAGER_H_INCLUDED

#include "App.h"
#include "SynthData.h"

#define DISABLED_PORT "CLOSED"
#define DAW_INPUT "RECIEVE FROM HOST"
#define DAW_OUTPUT "SEND TO HOST"
#define VIRTUAL_PORT "VIRTUAL PORT"

#define VIRTUAL_PORT_ID -9
#define UNKNOWN_PORT_ID -1

// TODO, update block size from processblock
// TODO dummy, to store old ports

class MidiInputWrapper {
    ScopedPointer<MidiInput> _midi_input;
    const bool is_daw_port;

public:
    inline bool is_same( MidiInput* other_input_ ) const noexcept {
        return !( _midi_input != other_input_ );
    }
    void stop() {
        if( _midi_input )
            _midi_input->stop();
    }

    const String port_ident_name;
    const String name;

    NOINLINE MidiInputWrapper( MidiInput*const midi_output_,
                               const String& port_ident_name_,
                               const String& device_name_ )
        : _midi_input( midi_output_ ),
          is_daw_port( device_name_ == DAW_INPUT ),
          port_ident_name( port_ident_name_ ),
          name( device_name_ )
    {
    }

    ~MidiInputWrapper() {
        stop();
    }

    static MidiInputWrapper* open( int device_index_,
                                   const String& port_ident_name_,
                                   const String& device_name_,
                                   MidiInputCallback* listener_
                                 )
    {
        MidiInput* device = nullptr;
#ifdef IS_PLUGIN
        if( device_name_ == DAW_INPUT )
            ;
        else
#endif
#ifndef JUCE_WINDOWS
            if( device_index_ == VIRTUAL_PORT_ID )
                device = MidiInput::createNewDevice( device_name_, listener_ );
            else
#endif
                device = MidiInput::openDevice( device_index_, listener_ );

        if( device ) {
            device->start();
            return new MidiInputWrapper( device,
                                         port_ident_name_,
                                         device_name_ );
        }
#ifdef IS_PLUGIN
        else if( device_name_ == DAW_INPUT )
        {
            return new MidiInputWrapper( device,
                                         port_ident_name_,
                                         device_name_ );
        }
#endif
        else
            return nullptr;
    };
};

class MidiOutputWrapper : public RuntimeListener
{
    ScopedPointer<MidiOutput> _midi_output;
    const bool is_daw_port;
    int pos_in_buffer;

    CriticalSection daw_lock;
    MidiBuffer _daw_mesages;

public:
    inline void send_message_now( const MidiMessage& message_, int pos_in_buffer_ )
    {
        if( _midi_output )
        {
            _midi_output->sendMessageNow( message_ );
        }
        else if( is_daw_port )
        {
            ScopedLock locked( daw_lock );
            _daw_mesages.addEvent( message_, pos_in_buffer_ );
        }
    }
    /////////////////////////////////////////
    void start() {
        if( _midi_output ) _midi_output->startBackgroundThread();
    }
    void stop() {
        if( _midi_output ) _midi_output->stopBackgroundThread();
    }

    // PLEASE LOCK MANUAL TO USE THIS METHODE - SHOULD BE SAVE, BUT HOW KNOWS
    inline void lock() {
        daw_lock.enter();
    }
    inline void add_message( const MidiMessage& message_, int pos_in_buffer_ )
    {
        if( _midi_output || is_daw_port )
            _daw_mesages.addEvent( message_, pos_in_buffer_ );
    }
    inline void add_message_fifo( const MidiMessage& message_ )
    {
        if( _midi_output || is_daw_port )
        {
            _daw_mesages.addEvent( message_, pos_in_buffer );
            pos_in_buffer++;
        }
    }
    inline bool send_and_clear_block_if_phys_port() {
        if( _midi_output )
        {
            _midi_output->sendBlockOfMessages( _daw_mesages, Time::getMillisecondCounter()+2, sample_rate );
            _daw_mesages.clear();
            pos_in_buffer = 0;
        }

        return true;
    }
    inline void unlock() {
        daw_lock.exit();
    }
    /////////////////////////////////////////

    inline void send_messages_to_daw_and_clear( MidiBuffer& midi_messages_ )
    {
        ScopedLock locked( daw_lock );

        if( is_daw_port )
            midi_messages_.addEvents( _daw_mesages, _daw_mesages.getFirstEventTime(), _daw_mesages.getLastEventTime(), 0 );

        _daw_mesages.clear();
    }

    const String port_ident_name;
    const String name;

    NOINLINE MidiOutputWrapper( MidiOutput*const midi_output_,
                                const String& port_ident_name_,
                                const String& device_name_ )
        : _midi_output( midi_output_ ),
          is_daw_port( device_name_ == DAW_OUTPUT ),
          pos_in_buffer(0),
          port_ident_name( port_ident_name_ ),
          name( device_name_ )
    {
        _daw_mesages.ensureSize( 512 );
    }
    ~MidiOutputWrapper() {
        if( _midi_output )
            _midi_output->stopBackgroundThread();
    }

    static MidiOutputWrapper* open( int device_index_,
                                    const String& port_ident_name_,
                                    const String& device_name_ )
    {
        MidiOutput* device = nullptr;
#ifdef IS_PLUGIN
        if( device_name_ == DAW_OUTPUT )
            ;
        else
#endif
#ifndef JUCE_WINDOWS
            if( device_index_ == VIRTUAL_PORT_ID )
                device = MidiOutput::createNewDevice( device_name_ );
            else
#endif
                device = MidiOutput::openDevice( device_index_ );

        if( device )
            return new MidiOutputWrapper( device,
                                          port_ident_name_,
                                          device_name_ );
#ifdef IS_PLUGIN
        else if( device_name_ == DAW_OUTPUT )
        {
            return new MidiOutputWrapper( device,
                                          port_ident_name_,
                                          device_name_ );
        }
#endif
        else
            return nullptr;
    };

    // TODO send pending messages before close the port!
};

enum SEND_TYPES {
    SEND_CC,
    SEND_NOTE,
    SEND_BYTE
};

enum PORT_IDENT {
    SEND_MIDI_CC_FEEDBACK,
    SEND_MIDI_THRU,
    SEND_MIDI_CLOCK,

    SEND_MIDI_LFO_1,
    SEND_MIDI_LFO_2,
    SEND_MIDI_LFO_3,

    SEND_MIDI_F_ADSR_1,
    SEND_MIDI_F_ADSR_2,
    SEND_MIDI_F_ADSR_3,
    SEND_MIDI_ADSR,

    RECIEVE_MIDI_MAIN,
    RECIEVE_CC
};

class mono_AudioDeviceManager :
    public AudioDeviceManager,
    public MidiInputCallback
{
public:
    bool main_input_thru;
    bool use_main_input_as_cc;

private:
    ScopedPointer<MidiInputWrapper> main_input;

    ScopedPointer<MidiInputWrapper> cc_input;
    bool cc_input_thru;
    ScopedPointer<MidiOutputWrapper> cc_output;

    ScopedPointer<MidiOutputWrapper> thru_output;
    ScopedPointer<MidiOutputWrapper> clock_output;

    OwnedArray<MidiOutputWrapper> lfo_outputs;
    OwnedArray<MidiOutputWrapper> adsr_outputs;

    StringArray output_ident_names;
    StringArray input_ident_names;

    // INPUT
    // call this block at the beginning of your processblock and
    // override the virtual functions to handle it!
    void handle_incoming_midi_messages ( MidiBuffer& midi_messages_ )
    {

    }

    // TODO, send feedback for option
    // TODO, this handles the processor!
    void handleIncomingMidiMessage ( MidiInput* source, const MidiMessage& message ) override
    {
        bool success = false;
        if( main_input )
        {
            if( main_input->is_same( source ) )
            {
#ifdef IS_STANDALONE
                if( message.isMidiClock() )
                    handle_extern_midi_clock(message);
                else if( message.isMidiStart() )
                    handle_extern_midi_start(message);
                else if( message.isMidiStop() )
                    handle_extern_midi_stop(message);
                else if( message.isMidiContinue() )
                    handle_extern_midi_continue(message);


                else
#endif
                    // NOTe ON OFF
                    if( message.isNoteOnOrOff() )
                        handle_extern_note_input( message );

                // CC
                    else if( use_main_input_as_cc && message.isController() )
                    {
                        handle_extern_cc_input( message );
                    }
                // THRU
                if( main_input_thru )
                    if( thru_output )
                        // TODO, dont send clock here.
                        thru_output->send_message_now( message, 0 );

                success = true;
            }
        }
        if( ! success && cc_input )
        {
            if( cc_input->is_same( source ) )
            {
                if( message.isController() || message.isNoteOn() )
                {
                    handle_extern_cc_input( message );

                    success = true;
                }
            }
        }
    }
#ifdef IS_STANDALONE
    virtual void handle_extern_midi_start( const MidiMessage& message ) noexcept = 0;
    virtual void handle_extern_midi_stop( const MidiMessage& message ) noexcept = 0;
    virtual void handle_extern_midi_continue( const MidiMessage& message ) noexcept = 0;
    virtual void handle_extern_midi_clock( const MidiMessage& message ) noexcept = 0;
#endif
    virtual void handle_extern_note_input( const MidiMessage& message ) noexcept = 0;
    virtual void handle_extern_cc_input( const MidiMessage& message ) noexcept = 0;
    virtual void trigger_send_feedback() noexcept = 0;
    virtual void trigger_send_clear_feedback() noexcept = 0;

protected:
    // OUTPUT
    void get_messages_to_send_to_daw( MidiBuffer& midi_messages_ );

public:
    // UNIVERSAL FOR IN AND OUTPUTS
    StringArray get_available_ports( const String& port_ident_name_ );
    void open_port( const String& port_ident_name_, const String& device_name_ );
    String get_selected_device_name( const String& port_ident_name_ ) const;
    bool is_port_open( const String& port_ident_name_ ) const;

public:
    // SEND
    void send_lfo_message( int lfo_id_, const float* lfo_amps_, int num_samples_ );
    void send_adsr_message( int adsr_id_, const float* adsr_amps_, int num_samples_ );
    inline void send_feedback_message( const MidiMessage& message ) noexcept {
        if( cc_output )
            cc_output->send_message_now( message, 0 );
    }

protected:
    mono_AudioDeviceManager();
    void init();

    void stop_midi_devices() {
        if( main_input ) main_input->stop();
        if( cc_input ) cc_input->stop();

        for( int i = 0 ; i != lfo_outputs.size() ; ++i ) {
            if( lfo_outputs[i] )
                lfo_outputs[i]->stop();
        }
        for( int i = 0 ; i != adsr_outputs.size() ; ++i ) {
            if( adsr_outputs[i] )
                adsr_outputs[i]->stop();
        }
    }

public:
    NOINLINE void save() const noexcept;
    NOINLINE void read() noexcept;
};

#endif  // MONO_AUDIODEVICEMANAGER_H_INCLUDED











