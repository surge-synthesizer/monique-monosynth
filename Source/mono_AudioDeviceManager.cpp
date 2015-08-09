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
    for( int i = 0 ; i != SUM_LFOS ; ++i )
    {
        lfo_outputs.add( nullptr );
    }
    for( int i = 0 ; i != SUM_ENVS ; ++i )
    {
        adsr_outputs.add( nullptr );
    }

    output_ident_names.add("SEND_MIDI_CC_FEEDBACK");
    output_ident_names.add("SEND_MIDI_THRU");
    output_ident_names.add("SEND_MIDI_CLOCK");

    output_ident_names.add("SEND_MIDI_LFO_1");
    output_ident_names.add("SEND_MIDI_LFO_2");
    output_ident_names.add("SEND_MIDI_LFO_3");

    output_ident_names.add("SEND_MIDI_F_ADSR_1");
    output_ident_names.add("SEND_MIDI_F_ADSR_2");
    output_ident_names.add("SEND_MIDI_F_ADSR_3");
    output_ident_names.add("SEND_MIDI_ADSR");

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
    else if( port_ident_name_ == "SEND_MIDI_LFO_1" ) {
        if( lfo_outputs.getUnchecked(0) )
            return lfo_outputs.getUnchecked(0)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_LFO_2" ) {
        if( lfo_outputs.getUnchecked(1) )
            return lfo_outputs.getUnchecked(1)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_LFO_3" ) {
        if( lfo_outputs.getUnchecked(2) )
            return lfo_outputs.getUnchecked(2)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_1" ) {
        if( adsr_outputs.getUnchecked(0) )
            return adsr_outputs.getUnchecked(0)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_2" ) {
        if( adsr_outputs.getUnchecked(1) )
            return adsr_outputs.getUnchecked(1)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_3" ) {
        if( adsr_outputs.getUnchecked(2) )
            return adsr_outputs.getUnchecked(2)->name;
    }
    else if( port_ident_name_ == "SEND_MIDI_ADSR" ) {
        if( adsr_outputs.getUnchecked(3) )
            return adsr_outputs.getUnchecked(3)->name;
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
    else if( port_ident_name_ == "SEND_MIDI_LFO_1" ) {
        return lfo_outputs.getUnchecked(0);
    }
    else if( port_ident_name_ == "SEND_MIDI_LFO_2" ) {
        return lfo_outputs.getUnchecked(1);
    }
    else if( port_ident_name_ == "SEND_MIDI_LFO_3" ) {
        return lfo_outputs.getUnchecked(2);
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_1" ) {
        return adsr_outputs.getUnchecked(0);
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_2" ) {
        return adsr_outputs.getUnchecked(1);
    }
    else if( port_ident_name_ == "SEND_MIDI_F_ADSR_3" ) {
        return adsr_outputs.getUnchecked(2);
    }
    else if( port_ident_name_ == "SEND_MIDI_ADSR" ) {
        return adsr_outputs.getUnchecked(3);
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

    for( int i = 0 ; i != lfo_outputs.size() ; ++i ) {
        MidiOutputWrapper* lfo_output( lfo_outputs.getUnchecked(i) );
        if( lfo_output )
            lfo_output->send_messages_to_daw_and_clear( midi_messages_ );
    }

    for( int i = 0 ; i != adsr_outputs.size() ; ++i ) {
        MidiOutputWrapper* adsr_output( adsr_outputs.getUnchecked(i) );
        if( adsr_output )
            adsr_output->send_messages_to_daw_and_clear( midi_messages_ );
    }
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
        else if( port_ident_name_ == "SEND_MIDI_LFO_1" ) {
            lfo_outputs.set( 0, nullptr );
            if( do_open )
            {
                lfo_outputs.set( 0, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( lfo_outputs[0] )
                    lfo_outputs[0]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_LFO_2" ) {
            lfo_outputs.set( 1, nullptr );
            if( do_open )
            {
                lfo_outputs.set( 1, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( lfo_outputs[1] )
                    lfo_outputs[1]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_LFO_3" ) {
            lfo_outputs.set( 2, nullptr );
            if( do_open )
            {
                lfo_outputs.set( 2, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( lfo_outputs[2] )
                    lfo_outputs[2]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_F_ADSR_1" ) {
            adsr_outputs.set( 0, nullptr );
            if( do_open )
            {
                adsr_outputs.set( 0, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( adsr_outputs[0] )
                    adsr_outputs[0]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_F_ADSR_2" ) {
            adsr_outputs.set( 1, nullptr );
            if( do_open )
            {
                adsr_outputs.set( 1, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( adsr_outputs[1] )
                    adsr_outputs[1]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_F_ADSR_3" ) {
            adsr_outputs.set( 2, nullptr );
            if( do_open )
            {
                adsr_outputs.set( 2, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( adsr_outputs[2] )
                    adsr_outputs[2]->start();
            }
        }
        else if( port_ident_name_ == "SEND_MIDI_ADSR" ) {
            adsr_outputs.set( 3, nullptr );
            if( do_open )
            {
                adsr_outputs.set( 3, MidiOutputWrapper::open( device_index, port_ident_name_, device_name_ ) );
                if( adsr_outputs[3] )
                    adsr_outputs[3]->start();
            }
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
void mono_AudioDeviceManager::send_lfo_message(int lfo_id_, const float* lfo_amps_, int num_samples_)
{
    MidiOutputWrapper*const midi_output_wrapper = lfo_outputs.getUnchecked( lfo_id_ );
    if( midi_output_wrapper )
    {
        midi_output_wrapper->lock();
        {
            uint8 tmp_lfo_value = 255;
            for( int i = 0 ; i != num_samples_ ; ++i ) 
	    {
                uint8 value = 127.0f*lfo_amps_[i];
                if( value != tmp_lfo_value )
                {
                    midi_output_wrapper->add_message( MidiMessage::controllerEvent( 1, 1, value ), i );
                    tmp_lfo_value = value;
                }
            }

            // TODO, we need a slider to adjust the offset +1!
            midi_output_wrapper->send_and_clear_block_if_phys_port();
        }
        midi_output_wrapper->unlock();
    }


    // TODO, output buffer or port
    // TODO, check if the same as last sended, also check port, channel
    // TODO, check interval
}
void mono_AudioDeviceManager::send_adsr_message(int adsr_id_, const float* adsr_amps_, int num_samples_ )
{
    MidiOutputWrapper*const midi_output_wrapper = adsr_outputs.getUnchecked( adsr_id_ );
    if( midi_output_wrapper )
    {
        midi_output_wrapper->lock();
        {
            uint8 tmp_adsr_value = 255;
            for( int i = 0 ; i != num_samples_ ; ++i ) 
	    {
                uint8 value = 127.0f*adsr_amps_[i];
                if( value != tmp_adsr_value )
                {
                    midi_output_wrapper->add_message( MidiMessage::controllerEvent( 1, 1, value ), i );
                    tmp_adsr_value = value;
                }
            }

            // TODO, we need a slider to adjust the offset +1!
            midi_output_wrapper->send_and_clear_block_if_phys_port();
        }
        midi_output_wrapper->unlock();
    }
}
NOINLINE void mono_AudioDeviceManager::save()  const noexcept
{
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
    folder = File(folder.getFullPathName()+"/Monoplugs/Monolisa/");
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
        if( lfo_outputs[0] )
            xml.setAttribute( "SEND_MIDI_LFO_1", lfo_outputs[0]->name );
        if( lfo_outputs[1] )
            xml.setAttribute( "SEND_MIDI_LFO_2", lfo_outputs[1]->name );
        if( lfo_outputs[2] )
            xml.setAttribute( "SEND_MIDI_LFO_3", lfo_outputs[2]->name );

        if( adsr_outputs[0] )
            xml.setAttribute( "SEND_MIDI_F_ADSR_1", adsr_outputs[0]->name );
        if( adsr_outputs[1] )
            xml.setAttribute( "SEND_MIDI_F_ADSR_2", adsr_outputs[1]->name );
        if( adsr_outputs[2] )
            xml.setAttribute( "SEND_MIDI_F_ADSR_3", adsr_outputs[2]->name );
        if( adsr_outputs[3] )
            xml.setAttribute( "SEND_MIDI_ADSR", adsr_outputs[3]->name );

        xml.writeToFile(midi_file,"");
    }
}
NOINLINE void mono_AudioDeviceManager::read() noexcept
{
    File folder = File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
#ifdef IS_PLUGIN
    File midi_file = File(folder.getFullPathName()+"/Monoplugs/Monolisa/config.pmidi");
#else
    File midi_file = File(folder.getFullPathName()+"/Monoplugs/Monolisa/config.midi");
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

            open_port( "SEND_MIDI_LFO_1", xml->getStringAttribute( "SEND_MIDI_LFO_1", DISABLED_PORT ) );
            open_port( "SEND_MIDI_LFO_2", xml->getStringAttribute( "SEND_MIDI_LFO_2", DISABLED_PORT ) );
            open_port( "SEND_MIDI_LFO_3", xml->getStringAttribute( "SEND_MIDI_LFO_3", DISABLED_PORT ) );

            open_port( "SEND_MIDI_F_ADSR_1", xml->getStringAttribute( "SEND_MIDI_F_ADSR_1", DISABLED_PORT ) );
            open_port( "SEND_MIDI_F_ADSR_2", xml->getStringAttribute( "SEND_MIDI_F_ADSR_2", DISABLED_PORT ) );
            open_port( "SEND_MIDI_F_ADSR_3", xml->getStringAttribute( "SEND_MIDI_F_ADSR_3", DISABLED_PORT ) );
            open_port( "SEND_MIDI_ADSR", xml->getStringAttribute( "SEND_MIDI_ADSR", DISABLED_PORT ) );
        }
    }
    else
    {
        init();
    }
}










