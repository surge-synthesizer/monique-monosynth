/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

// DEVICE MANAGE TEMPLATE FOR MidiOutput and MidiInput
// Instancing requires the build_new_port_policy_t<AndroidMidiDeviceManager> port policy.
//==============================================================================
enum { IS_INPUT = true, IS_OUTPUT = false };
enum { FORCE_PORT_BUILD = -9876187, INVALID_PORT_ID = -1 };

template<class port_type_t, class build_new_port_policy_t>
class AndroidMidiDeviceManager
{
    struct DeviceInfo
    {
        Array< port_type_t* > ports;
        const String name;
        int index;

        DeviceInfo( port_type_t*const port_, const String& name_, int index_ )
            : name( name_ ),
              index( index_ )
        {
            ports.add( port_ );
        }

        inline bool is_same( const String& name_, int index_ ) const
        {
            if( name_ == name and index_ == index )
                return true;

            return false;
        }
    };

    OwnedArray< DeviceInfo > open_ports;
    Array< const port_type_t* > removed_ports; // ATTENTION ZOMBIES, DO NEVVER USE!!!!!!!

public:
    /// OFTEN CALLED FUNCTIONS
    inline static AndroidMidiDeviceManager& instance()
    {
        static AndroidMidiDeviceManager devMan;
        return devMan;
    }
    inline bool is_still_connected( const port_type_t*const port_ ) const
    {
        return not removed_ports.contains( port_ );
    }
    Array< port_type_t* > empty_ports;
    inline const Array< port_type_t* >& get_ports_for_device( int index ) const
    {
        if( index < open_ports.size() )
            return open_ports.getUnchecked( index )->ports;

        return empty_ports;
    }
    inline int get_deviceindex_for_port( port_type_t*const port_is_dctor_ ) const
    {
        DeviceInfo* dev_info_;
        for( int index = 0 ; index != open_ports.size() ; ++index )
        {
            dev_info_ = open_ports.getUnchecked( index );
            if( dev_info_->ports.contains( port_is_dctor_ ) )
            {
                return dev_info_->index;
            }
        }

        return INVALID_PORT_ID;
    }

    /// INIT FUNCTIONS
    void device_removed (int removed_device_index_ );

    port_type_t* build_add_port( const String& name_, int index_, MidiInputCallback* callback_ = nullptr )
    {
        port_type_t* port = nullptr;

        // check if the devicename or index is invalid!
        if( name_.compare("_ERROR") == 0 )
            return port;
        else if( index_ < 0 )
            return port;

        // ADD TO AN OPEN CONNECTION ?
        DeviceInfo* dev_info_;
        for( int i = 0 ; i != open_ports.size() ; ++i )
        {
            dev_info_ = open_ports.getUnchecked(i);
            if( dev_info_->is_same( name_, index_ ) )
            {
                port = build_new_port_policy_t::force_build_port( callback_ );
                dev_info_->ports.add( port );
                break;
            }
        }

        // ELSE ADD A NEW CONNECTION
        if( not port )
        {
            port = build_new_port_policy_t::force_build_port( callback_ );
            dev_info_ = new DeviceInfo( port, name_, index_ );
            open_ports.add( dev_info_ );
        }

        return port;
    }

    static void get_ports( StringArray& devs, bool true_is_input_else_output_ );
    void remove( port_type_t*const port_is_dctor_ );

private:
    AndroidMidiDeviceManager() {};
};

template<class port_type_t, class build_new_port_policy_t>
void AndroidMidiDeviceManager<port_type_t,build_new_port_policy_t>
::device_removed (int removed_device_index_ )
{
    DeviceInfo* port_to_replace = nullptr;
    for( int index = 0 ; index != open_ports.size() ; ++index )
    {
        port_to_replace = open_ports.getUnchecked( index );
        if( port_to_replace->index == removed_device_index_ )
        {
            // the outport will become a zombi but it does not crash
            Array< port_type_t* >& ports = port_to_replace->ports;
            for( int i = 0 ; i != ports.size() ; ++i )
            {
                removed_ports.add( ports.getUnchecked(i) );
            }

            open_ports.removeObject( port_to_replace, true );
            break;
        }
    }
    // reorder the indexes
    if( port_to_replace )
    {
        DeviceInfo* port_to_reindex;
        for( int index = 0 ; index != open_ports.size() ; ++index )
        {
            port_to_reindex = open_ports.getUnchecked( index );
            if( port_to_reindex->index > removed_device_index_ )
                port_to_reindex->index--;
        }
    }
}

template<class port_type_t, class build_new_port_policy_t>
void AndroidMidiDeviceManager<port_type_t,build_new_port_policy_t>::get_ports( StringArray& devs, bool true_is_input_else_output_ )
{
    int sum_connected_midi_devs = ((jint) android.activity.callIntMethod( JuceAppActivity.getNumDevices,
                                   (jboolean) true_is_input_else_output_ ));
    for( int i = 0 ; i != sum_connected_midi_devs ; ++i )
    {
        const LocalRef<jstring> text ((jstring) android.activity.callObjectMethod( JuceAppActivity.getDeviceName,
                                      (jint) i, (jboolean) true_is_input_else_output_ ));
        String name = juceString (text);
        devs.add( name );
    }
}

template<class port_type_t, class build_new_port_policy_t>
void AndroidMidiDeviceManager<port_type_t,build_new_port_policy_t>::remove( port_type_t*const port_is_dctor_ )
{
    DeviceInfo* dev_info_;
    for( int i = 0 ; i != open_ports.size() ; ++i )
    {
        dev_info_ = open_ports.getUnchecked(i);
        Array< port_type_t* > ports_to_remove;
        for( int j = 0 ; j != dev_info_->ports.size() ; ++j )
        {
            port_type_t* const port = dev_info_->ports.getUnchecked(j);
            if( port_is_dctor_ == port )
            {
                ports_to_remove.add( port );
            }
        }

        for( int j = 0 ; j != ports_to_remove.size() ; ++j )
        {
            dev_info_->ports.removeFirstMatchingValue( ports_to_remove.getUnchecked(j) );
        }
    }

    removed_ports.removeFirstMatchingValue(port_is_dctor_);
}

// Policy to open MidiOutput
//==============================================================================
struct MidiOutputBuildPolicy
{
    static MidiOutput* force_build_port( MidiInputCallback* = nullptr )
    {
        return MidiOutput::openDevice(FORCE_PORT_BUILD);
    }
};

typedef AndroidMidiDeviceManager< MidiOutput, MidiOutputBuildPolicy > AndroidMidiOutputManager;

// CALLS FROM JAVA
//==============================================================================
JUCE_JNI_CALLBACK (JUCE_ANDROID_ACTIVITY_CLASSNAME, onOutputDeviceRemoved, void, (JNIEnv* env, jobject activity,
                   jint device_index))
{
    AndroidMidiOutputManager::instance().device_removed( device_index );
}

// MidiOutput IMPL
//==============================================================================
StringArray MidiOutput::getDevices()
{
    StringArray devs;
    AndroidMidiOutputManager::get_ports( devs, IS_OUTPUT );
    return devs;
}

int MidiOutput::getDefaultDeviceIndex()
{
    return 0;
}

MidiOutput* MidiOutput::openDevice ( int index )
{
    if( index == FORCE_PORT_BUILD ) // jsut a hack to manage it from AndroidMidiDeviceManager (protected method)
        return new MidiOutput();
    else
    {
        StringArray devs = getDevices();
        if( devs.size() > index )
            return AndroidMidiOutputManager::instance().build_add_port( devs[index], index );
    }

    return nullptr;
}

MidiOutput::~MidiOutput()
{
    stopBackgroundThread();

    AndroidMidiOutputManager::instance().remove(this);
}

void MidiOutput::sendMessageNow (const MidiMessage& message_ )
{
    // checks if this device is still online
    // NOTE: if this device exist, it was at least a very short time opend
    if( not AndroidMidiOutputManager::instance().is_still_connected( this ) )
    {
        // TODO send an error message!
        return;
    }

    int device_index = AndroidMidiOutputManager::instance().get_deviceindex_for_port( this );
    if( device_index == INVALID_PORT_ID )
        return;

    enum
    {
        T_sendMidiSingleByte = 0, // CLOCK ETC
        T_sendMidiNoteOn,
        T_sendMidiNoteOff,

        T_sendMidiControlChange,
        T_sendMidiProgramChange,

        T_sendMidiPitchWheel,

        T_sendMidiCableEvents,
        T_sendMidiSystemCommonMessage,
        T_sendMidiSystemExclusive,

        T_sendMidiPolyphonicAftertouch,
        T_sendMidiChannelAftertouch,

        T_sendRPNMessage,
        T_sendNRPNMessage,
    };

    if( message_.isMidiClock() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiMessageOneByte,
                                         device_index,
                                         248
                                        );
    }
    else if( message_.isNoteOn() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiNoteOn,
                                         device_index,
                                         (jint) message_.getChannel()-1,
                                         (jint) message_.getNoteNumber(),
                                         (jint) message_.getVelocity()
                                        );
    }
    else if( message_.isNoteOff() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiNoteOff,
                                         device_index,
                                         (jint) message_.getChannel()-1,
                                         (jint) message_.getNoteNumber(),
                                         (jint) message_.getVelocity()
                                        );
    }
    else if( message_.isController() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiControlChange,
                                         device_index,
                                         (jint) message_.getChannel()-1,
                                         (jint) message_.getNoteNumber(),
                                         (jint) message_.getVelocity()
                                        );
    }
    else if( message_.isProgramChange() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiProgramChange,
                                         device_index,
                                         (jint) message_.getChannel()-1,
                                         (jint) message_.getNoteNumber(),
                                         (jint) message_.getVelocity()
                                        );
    }
    else if( message_.isAllNotesOff() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiMessageOneByte,
                                         device_index,
                                         (jint) message_.getRawData()[0]
                                        );
    }
    else if( message_.isMidiStart() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiMessageOneByte,
                                         device_index,
                                         (jint) message_.getRawData()[0]
                                        );
    }
    else if( message_.isMidiContinue() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiMessageOneByte,
                                         device_index,
                                         (jint) message_.getRawData()[0]
                                        );
    }
    else if( message_.isMidiStop() )
    {
        android.activity.callVoidMethod (JuceAppActivity.sendMidiMessageOneByte,
                                         device_index,
                                         (jint) message_.getRawData()[0]
                                        );
    }

    /*
    isSustainPedalOn
    isSustainPedalOff
    isSoftPedalOn
    isSoftPedalOff
    isSostenutoPedalOn
    isSostenutoPedalOff
    isPitchWheel
    isProgramChange
    isAftertouch
    isSysEx
    isChannelPressure
    isAllSoundOff
    isMetaEvent
    isTrackMetaEvent
    isTrackNameEvent
    isTextMetaEvent
    isTempoMetaEvent
    isTimeSignatureMetaEvent
    isKeySignatureMetaEvent
    isMidiChannelMetaEvent
    isActiveSense

    isSongPositionPointer
    isQuarterFrame
    isFullFrame
    isMidiMachineControlMessage
    */
}

//==============================================================================
struct MidiInputBuildPolicy
{
    static MidiInput* force_build_port( MidiInputCallback* callback )
    {
        return MidiInput::openDevice( FORCE_PORT_BUILD, callback );
    }
};

typedef AndroidMidiDeviceManager< MidiInput, MidiInputBuildPolicy > AndroidMidiInputManager;

// CALLS FROM JAVA
//==============================================================================
JUCE_JNI_CALLBACK (JUCE_ANDROID_ACTIVITY_CLASSNAME, onInputDeviceRemoved, void, (JNIEnv* env, jobject activity,
                   jint device_index))
{
    AndroidMidiInputManager::instance().device_removed( device_index );
}

MidiInput::MidiInput (const String& name_) : name (name_), internal ( nullptr ) {}

MidiInput::~MidiInput()
{
    AndroidMidiInputManager::instance().remove(this);
}

void MidiInput::start() {}

void MidiInput::stop() {}

int MidiInput::getDefaultDeviceIndex()
{
    return 0;
}

StringArray MidiInput::getDevices()
{
    StringArray devs;
    AndroidMidiInputManager::get_ports( devs, IS_INPUT );
    return devs;
}

JUCE_JNI_CALLBACK (JUCE_ANDROID_ACTIVITY_CLASSNAME, midiInputCallback, void, (JNIEnv* env, jobject activity,
                   jint input_device_index, jint byte1, jint byte2, jint byte3))
{
    if( input_device_index < 0 )
        return;

    const Array< MidiInput* >& ports = AndroidMidiInputManager::instance().get_ports_for_device( input_device_index );
    if( ports.size() )
    {
        MidiInput* input;
        MidiInputCallback* callback;
        for( int i = 0 ; i != ports.size() ; ++i )
        {
            input = ports.getUnchecked(i);
            callback = reinterpret_cast< MidiInputCallback* >( input->internal );
	    if( callback )
	      callback->handleIncomingMidiMessage( input, MidiMessage(byte1, byte2, byte3, 0.0) );
        }
    }
}

MidiInput* MidiInput::openDevice (int index, MidiInputCallback* callback)
{
    if( index == FORCE_PORT_BUILD ) // jsut a hack to manage it from AndroidMidiDeviceManager (protected method)
    {
        MidiInput *midi_in = new MidiInput("B-Step MIDI In");
        midi_in->internal = callback;
        return midi_in;
    }
    else
    {
        StringArray devs = getDevices();
        if( devs.size() > index )
            return AndroidMidiInputManager::instance().build_add_port( devs[index], index, callback );
    }

    return nullptr;
}