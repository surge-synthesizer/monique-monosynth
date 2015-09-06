/*
  ==============================================================================

    mono_AudioDeviceManager.cpp
    Created: 21 May 2015 12:20:29pm
    Author:  monotomy

  ==============================================================================
*/

#include "mono_AudioDeviceManager.h"

mono_AudioDeviceManager::mono_AudioDeviceManager()
    :
    main_input_thru( false ),
    use_main_input_as_cc( false ),
    cc_input_thru( false )
{
    output_ident_names.add("SEND_MIDI_CC_FEEDBACK");
    output_ident_names.add("SEND_MIDI_THRU");
    output_ident_names.add("SEND_MIDI_CLOCK");

    input_ident_names.add("RECIEVE_MIDI_MAIN");
    input_ident_names.add("RECIEVE_CC");
}
void mono_AudioDeviceManager::init()
{
#ifdef IS_PLUGIN
    open_port( "SEND_MIDI_CC_FEEDBACK", DAW_OUTPUT );

    for( int i = 0; i != input_ident_names.size() ; ++i ) {
        open_port( input_ident_names[i], DAW_INPUT );
    }
#endif
}

StringArray mono_AudioDeviceManager::get_available_ports(const String& port_ident_name_) {
    StringArray devices;

    if( output_ident_names.contains( port_ident_name_ ) ) {
        devices.addArray( MidiOutput::getDevices() );
#ifndef JUCE_WINDOWS
        devices.add( VIRTUAL_PORT );
#endif
#ifdef IS_PLUGIN
        devices.add( DAW_OUTPUT );
#endif
        devices.add( DISABLED_PORT );
    }
    else if( input_ident_names.contains( port_ident_name_ ) ) {
        devices.addArray( MidiInput::getDevices() );

#ifndef JUCE_WINDOWS
        devices.add( VIRTUAL_PORT );
#endif
#ifdef IS_PLUGIN
        devices.add( DAW_INPUT );
#endif
        devices.add( DISABLED_PORT );
    }

    return devices;
}

String mono_AudioDeviceManager::get_selected_device_name( const String& port_ident_name_ ) const {
    // OUTPUTS
    if( port_ident_name_ == "SEND_MIDI_CC_FEEDBACK" ) {
        if( cc_output )
            return cc_output->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_THRU" ) {
        if( thru_output )
            return thru_output->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_CLOCK" ) {
        if( clock_output )
            return clock_output->name;
    }

    // INPUTS
    else if( port_ident_name_ == "RECIEVE_MIDI_MAIN" ) {
        if( main_input )
            return main_input->name;
    }
    else if( port_ident_name_ == "RECIEVE_CC" ) {
        if( cc_input )
            return cc_input->name;
    }

    return "CLOSED";
}

bool mono_AudioDeviceManager::is_port_open( const String& port_ident_name_ ) const {
    // OUTPUTS
    if( port_ident_name_ == "SEND_MIDI_CC_FEEDBACK" ) {
        return cc_output;
    }
    else if( port_ident_name_ == "SEND_MIDI_THRU" ) {
        return thru_output;
    }
    else if( port_ident_name_ == "SEND_MIDI_CLOCK" ) {
        return clock_output;
    }

    // INPUTS
    else if( port_ident_name_ == "RECIEVE_MIDI_MAIN" ) {
        return main_input;
    }
    else if( port_ident_name_ == "RECIEVE_CC" ) {
        return cc_input;
    }

    return false;
}

void mono_AudioDeviceManager::get_messages_to_send_to_daw(MidiBuffer& midi_messages_)
{
    if( cc_output )
        cc_output->send_messages_to_daw_and_clear( midi_messages_ );

    if( thru_output )
        thru_output->send_messages_to_daw_and_clear( midi_messages_ );

    // clock_output <- we don't need this in the DAW
}

void mono_AudioDeviceManager::open_port(const String& port_ident_name_, const String& device_name_)
{
    // OUTPUT
    if( output_ident_names.contains( port_ident_name_ ) )
    {
        const StringArray port_names = get_available_ports(port_ident_name_);
        int device_index( device_name_ == VIRTUAL_PORT ? VIRTUAL_PORT_ID : UNKNOWN_PORT_ID );
        if( device_index == UNKNOWN_PORT_ID )
            device_index = port_names.indexOf( device_name_ );

        bool do_open = false;
        if( device_index != UNKNOWN_PORT_ID && device_name_ != DISABLED_PORT )
            do_open = true;

        if( port_ident_name_ == "SEND_MIDI_CC_FEEDBACK" )
        {
            if( cc_output )
                trigger_send_clear_feedback();

            cc_output = nullptr;
            if( do_open ) {
                cc_output = MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ );
                if( cc_output ) {
                    cc_output->start();

                    trigger_send_feedback();
                }
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_THRU" )
        {
            thru_output = nullptr;
            if( do_open )
                thru_output = MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ );
        }
        else if( port_ident_name_ == "SEND_MIDI_CLOCK" )
        {
            clock_output = nullptr;
            if( do_open )
                clock_output = MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ );
        }
    }
    // INPUT
    else if( input_ident_names.contains( port_ident_name_ ) )
    {
        const StringArray port_names = get_available_ports(port_ident_name_);
        int device_index( device_name_ == VIRTUAL_PORT ? VIRTUAL_PORT_ID : UNKNOWN_PORT_ID );
        if( device_index == UNKNOWN_PORT_ID )
            device_index = port_names.indexOf( device_name_ );

        bool do_open = false;
        if( device_index != UNKNOWN_PORT_ID && device_name_ != DISABLED_PORT )
            do_open = true;

        if( port_ident_name_ == "RECIEVE_MIDI_MAIN" )
        {
            main_input = nullptr;
            if( do_open )
                main_input = MidiInputWrapper::open( device_index, port_ident_name_, device_name_, this );
        }
        else if( port_ident_name_ == "RECIEVE_CC" )
        {
            cc_input = nullptr;
            if( do_open )
                cc_input = MidiInputWrapper::open( device_index, port_ident_name_, device_name_, this );;
        }
    }
}
COLD void mono_AudioDeviceManager::save()  const noexcept
{
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    folder = File(folder.getFullPathName()+PROJECT_FOLDER);
    if( folder.createDirectory() )
    {
#ifdef IS_PLUGIN
        File midi_file( File( folder.getFullPathName() + String("/") + "config.pmidi") );
#else
        File midi_file( File( folder.getFullPathName() + String("/") + "config.midi") );
#endif

        XmlElement xml("MIDI-IO-CONFIG-1.0");
        if( main_input )
            xml.setAttribute( "RECIEVE_MIDI_MAIN", main_input->name );
        if( cc_input )
            xml.setAttribute( "RECIEVE_CC", cc_input->name );

        if( cc_output )
            xml.setAttribute( "SEND_MIDI_CC_FEEDBACK", cc_output->name );
        if( thru_output )
            xml.setAttribute( "SEND_MIDI_THRU", thru_output->name );
        if( clock_output )
            xml.setAttribute( "SEND_MIDI_CLOCK", clock_output->name );

        xml.writeToFile(midi_file,"");
    }
}
COLD void mono_AudioDeviceManager::read() noexcept
{
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
#ifdef IS_PLUGIN
    File midi_file = File(folder.getFullPathName()+PROJECT_FOLDER+String("config.pmidi"));
#else
    File midi_file = File(folder.getFullPathName()+PROJECT_FOLDER+String("config.midi"));
#endif
    ScopedPointer<XmlElement> xml = XmlDocument( midi_file ).getDocumentElement();
    if( xml )
    {
        if( xml->hasTagName("MIDI-IO-CONFIG-1.0") )
        {
            open_port( "RECIEVE_MIDI_MAIN", xml->getStringAttribute( "RECIEVE_MIDI_MAIN", DISABLED_PORT ) );
            open_port( "RECIEVE_CC", xml->getStringAttribute( "RECIEVE_CC", DISABLED_PORT ) );

            open_port( "SEND_MIDI_CC_FEEDBACK", xml->getStringAttribute( "SEND_MIDI_CC_FEEDBACK", DISABLED_PORT ) );
            open_port( "SEND_MIDI_THRU", xml->getStringAttribute( "SEND_MIDI_THRU", DISABLED_PORT ) );
            open_port( "SEND_MIDI_CLOCK", xml->getStringAttribute( "SEND_MIDI_CLOCK", DISABLED_PORT ) );
        }
    }
    else
    {
        init();
    }
}

