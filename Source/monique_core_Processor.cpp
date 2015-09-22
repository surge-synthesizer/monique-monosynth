#include "monique_core_Processor.h"

#include "monique_core_Datastructures.h"
#include "monique_core_Synth.h"

#include "monique_ui_MainWindow.h"
#include "monique_ui_SegmentedMeter.h"

//==============================================================================
//==============================================================================
//==============================================================================
COLD AudioProcessorEditor* MoniqueAudioProcessor::createEditor()
{
    return new Monique_Ui_Mainwindow();
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoniqueAudioProcessor();
}

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_STANDALONE
#define CLOCKS_TO_SMOOTH 24
class ClockSmoothBuffer : RuntimeListener
{
    int pos;
    double buffer[CLOCKS_TO_SMOOTH];
    double sum;

public:
    inline void add( double val_ ) noexcept;
    inline double get_average() const noexcept;
    inline double add_and_get_average( double val_ ) noexcept;
    inline void reset( double value_ = 0 ) noexcept;

public:
    COLD ClockSmoothBuffer();
    COLD ~ClockSmoothBuffer();
};

//==============================================================================
COLD ClockSmoothBuffer::ClockSmoothBuffer() : pos(0), sum(0)
{
    reset();
}
COLD ClockSmoothBuffer::~ClockSmoothBuffer() {}

//==============================================================================
inline void ClockSmoothBuffer::add( double val_ ) noexcept
{
    sum-=buffer[pos];
    buffer[pos] = val_;
    sum+=val_;

    if( ++pos == CLOCKS_TO_SMOOTH )
    {
        pos = 0;
    }
}
inline double ClockSmoothBuffer::add_and_get_average( double val_ ) noexcept
{
    add(val_);
    return get_average();
}
inline double ClockSmoothBuffer::get_average() const noexcept
{
    return sum * ( 1.0 / CLOCKS_TO_SMOOTH );
}
inline void ClockSmoothBuffer::reset( double value_ ) noexcept
{
    sum = 0;
    for( int i = 0 ; i != CLOCKS_TO_SMOOTH ; ++i )
    {
        buffer[i] = value_;
        sum += value_;
    }
}
#endif

//==============================================================================
//==============================================================================
//==============================================================================
class NoteDownStore
{
    int8 root_note;
    int8 second_note;
    int8 third_note;
    struct NoteTimestampPair
    {
        const int8 note_number;
        const int64 sample_pos;

        inline bool operator== ( const NoteTimestampPair pair_ ) const noexcept
        {
            return note_number == pair_.note_number;
        }

        inline NoteTimestampPair( int8 note_number_, int64 sample_pos_ )
noexcept :
        note_number(note_number_), sample_pos(sample_pos_) {}
        inline ~NoteTimestampPair() noexcept {}
    };
    Array< NoteTimestampPair > notes_down;

    //==============================================================================
    void addNote( int8 note_number_, int64 sample_ ) noexcept;
    void removeNote( int8 note_number_, int64 sample_ ) noexcept;

public:
    //==============================================================================
    void handle_midi_messages( const MidiBuffer& messages_ ) noexcept;
    void reset() noexcept;

    bool are_more_than_one_key_down() const noexcept 
    {
        return notes_down.size() > 1;
    }

public:
    //==============================================================================
    COLD NoteDownStore() noexcept;
    COLD ~NoteDownStore() noexcept;
};

//==============================================================================
COLD NoteDownStore::NoteDownStore() noexcept :
root_note(-1),
          second_note(-1),
          third_note(-1)
{}
COLD NoteDownStore::~NoteDownStore() noexcept {}

    bool MoniqueAudioProcessor::are_more_than_one_key_down() const noexcept
    {
      return note_down_store->are_more_than_one_key_down();
    }
//==============================================================================
void NoteDownStore::addNote( int8 note_number_, int64 sample_ ) noexcept
{
    NoteTimestampPair pair(note_number_,sample_);
    if( not notes_down.contains( pair ) )
    {
        notes_down.add( pair );
    }

    float master_tune = GET_DATA( osc_datas[MASTER_OSC] ).tune;
    float used_root_tune = root_note + master_tune;
    if( root_note == -1 )
    {
        root_note = note_number_;
        std::cout <<"1st"<< root_note << std::endl;
    }
    else if( second_note == -1 )
    {
        second_note = note_number_;
        //GET_DATA( osc_datas[1] ).tune.set_value( float( second_note - root_note ) + master_tune );
    }
    else if( third_note == -1 )
    {
        third_note = note_number_;
        //GET_DATA( osc_datas[2] ).tune.set_value( float( third_note - root_note ) + master_tune );
    }
    else
    {
        third_note = note_number_;
        //GET_DATA( osc_datas[2] ).tune.set_value( float( third_note - root_note ) + master_tune );
    }
}
void NoteDownStore::removeNote( int8 note_number_, int64 sample_ ) noexcept
{
    NoteTimestampPair pair(note_number_,sample_);

    notes_down.removeFirstMatchingValue( pair );

    if( root_note == note_number_ )
    {
        root_note = second_note;
        second_note = third_note;
        third_note = -1;
    }
    else if( second_note == note_number_ )
    {
        second_note = third_note;
        third_note = -1;
    }
    else if( third_note == note_number_ )
    {
        third_note = -1;
    }
}
void NoteDownStore::handle_midi_messages( const MidiBuffer& messages_ ) noexcept
{
    MidiBuffer::Iterator message_iter( messages_ );
    MidiMessage input_midi_message;
    int sample_position = 0;
    while( message_iter.getNextEvent( input_midi_message, sample_position ) )
    {
        if( input_midi_message.isNoteOn() )
        {
            addNote( input_midi_message.getNoteNumber(), sample_position );
        }
        else if( input_midi_message.isNoteOff() )
        {
            removeNote( input_midi_message.getNoteNumber(), sample_position );
        }
    }
}

void NoteDownStore::reset() noexcept
{
    notes_down.clearQuick();
    root_note = -1;
    second_note = -1;
    third_note = -1;
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD MoniqueAudioProcessor::MoniqueAudioProcessor() noexcept
:
#ifdef IS_STANDALONE
clock_smoother( new ClockSmoothBuffer() ),
last_clock_sample(0),
try_to_restart_arp( false ),
connection_missed_counter(0),
received_a_clock_in_time(false),
#endif
note_down_store( new NoteDownStore() ),
peak_meter(nullptr)
{
    // CREATE SINGLETONS
    RuntimeNotifyer::getInstance();

    std::cout << "MONIQUE: init core" << std::endl;
    FloatVectorOperations::enableFlushToZeroMode(true);
    {
        AppInstanceStore::getInstance()->audio_processor = this;

        synth = new Synthesiser();
        synth_data = new MoniqueSynthData(MASTER);
        voice = new MoniqueSynthesiserVoice(this,synth_data);
        synth->addVoice (voice);
        synth->addSound (new MoniqueSynthesiserSound());

#ifdef IS_STANDALONE
        audio_is_successful_initalized = (mono_AudioDeviceManager::read() == "");
        if( audio_is_successful_initalized )
        {
            AudioDeviceManager::AudioDeviceSetup setup;
            getAudioDeviceSetup(setup);
            setPlayConfigDetails ( 0, 2, setup.sampleRate, setup.bufferSize );
            addAudioCallback (&player);
            player.setProcessor (this);
        }
#endif
    }

    std::cout << "MONIQUE: init load last project and settings" << std::endl;
    {
        synth_data->load_settings();
        synth_data->read_midi();
#ifdef IS_STANDALONE
        synth_data->load();
#endif
        MidiKeyboardState::addListener(this);
    }
#ifdef IS_PLUGIN
    init_automatable_parameters();
#endif
}

COLD MoniqueAudioProcessor::~MoniqueAudioProcessor() noexcept
{
    trigger_send_clear_feedback();

#ifdef IS_STANDALONE
    delete clock_smoother;
    mono_AudioDeviceManager::save();
    closeAudioDevice();
    removeAudioCallback (&player);
    player.setProcessor(nullptr);
#else
    for( int i = 0 ; i != automateable_parameters.size() ; ++i )
    {
        Parameter* param( automateable_parameters.getUnchecked(i) );
        if( param )
        {
            param->remove_listener( this );
        }
    }
#endif
    synth_data->save_midi();
    synth_data->save_settings();

    AppInstanceStore::getInstance()->audio_processor = nullptr;

    delete synth;
    delete synth_data;
}

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueAudioProcessor::set_peak_meter( Monique_Ui_SegmentedMeter*peak_meter_ ) noexcept
{
    ScopedLock locked(peak_meter_lock);
    peak_meter = peak_meter_;
}
void MoniqueAudioProcessor::clear_preak_meter() noexcept
{
    ScopedLock locked(peak_meter_lock);
    peak_meter = nullptr;
}

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_STANDALONE
void MoniqueAudioProcessor::timerCallback()
{
    if( GET_DATA( runtime_info ).is_extern_synced )
    {
        if( not received_a_clock_in_time )
        {
            connection_missed_counter++;
            if( connection_missed_counter > 14 )
            {
                GET_DATA( runtime_info ).is_extern_synced = false;
            }
        }
        else
        {
            connection_missed_counter = 0;
            received_a_clock_in_time = false;
        }
    }
    else
    {
        Timer::stopTimer();
        return;
    }
}
#endif

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueAudioProcessor::processBlock ( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_ )
{
    if( buffer_.getNumChannels() < 2 )
        return;

    if( sample_rate != getSampleRate() || getBlockSize() != block_size )
    {
        prepareToPlay(getSampleRate(),getBlockSize());
    }

    const int num_samples = buffer_.getNumSamples();
    buffer_.clear();

#ifdef IS_STANDALONE
    static bool is_first_time = true;
    if(is_first_time)
    {
        current_pos_info.resetToDefault();
        is_first_time=false;
    }
    current_pos_info.timeSigNumerator = 4;
    current_pos_info.timeSigDenominator = 4;
    current_pos_info.isPlaying = true;
    current_pos_info.isRecording = false;
    current_pos_info.timeInSamples += buffer_.getNumSamples();
    {
        {
#else // PLUGIN
    if ( getPlayHead() != 0 )
    {
        if( getPlayHead()->getCurrentPosition ( current_pos_info ) )
        {
#endif
            if( current_pos_info.timeInSamples + num_samples >= 0 ) //&& current_pos_info.isPlaying )
            {
#ifdef IS_STANDALONE
                // +++++ SYNC BLOCK EXTERN MIDI
                {
                    // CLEAN LAST BLOCK
                    RuntimeInfo& runtime_info = GET_DATA( runtime_info );
                    // FOR SECURITy REMOVE INVALID OLD STEPS
                    OwnedArray< RuntimeInfo::Step >& steps_in_block( runtime_info.steps_in_block );
                    if( steps_in_block.size() )
                    {
                        while( steps_in_block.getFirst()->at_absolute_sample < current_pos_info.timeInSamples )
                        {
                            steps_in_block.remove(0,true);

                            if( not steps_in_block.size() )
                            {
                                break;
                            }
                        }
                    }

                    // GET THE MESSAGES
                    MidiBuffer sync_messages;
                    get_sync_input_messages( sync_messages, num_samples );

                    // RUN THE LOOP AND PROCESS TJHE MESSAGES
                    const bool is_synced( synth_data->sync );
                    if( is_synced )
                    {
                        MidiBuffer::Iterator message_iter( sync_messages );
                        MidiMessage input_midi_message;
                        int sample_position = 0;

                        const double speed_multiplyer = ArpSequencerData::speed_multi_to_value( GET_DATA( arp_data ).speed_multi );
                        static constexpr int clocks_per_step = 6;
                        static constexpr int clocks_per_beat = 24;
                        static constexpr int clocks_per_bar = 96;
                        while( message_iter.getNextEvent( input_midi_message, sample_position ) )
                        {
                            const int64 abs_event_time_in_samples = current_pos_info.timeInSamples+sample_position;

                            // CLOCK
                            if( input_midi_message.isMidiClock() )
                            {
                                received_a_clock_in_time = true;
                                runtime_info.is_extern_synced = true;
                                if( not Timer::isTimerRunning() )
                                {
                                    Timer::startTimer( 100 );
                                }

                                const int clock_in_bar = runtime_info.clock_counter.clock();
                                const int clock_absolute = runtime_info.clock_counter.clock_absolut();

                                // X 1
                                bool success = false;
                                if( speed_multiplyer == 1 )
                                {
                                    if( clock_in_bar % clocks_per_step == 0 )
                                    {
                                        runtime_info.steps_in_block.add( new RuntimeInfo::Step( clock_in_bar/clocks_per_step, abs_event_time_in_samples, abs_event_time_in_samples-last_clock_sample ) );

                                        success = true;
                                    }
                                }
                                //  X > 1 // WIRD LANGSAMER
                                else if( speed_multiplyer > 1 )
                                {
                                    int sum_generate_sub_clocks_per_clock = speed_multiplyer;
                                    const double current_samples_per_clock = double(abs_event_time_in_samples-last_clock_sample) / speed_multiplyer;
                                    int clock_id = clock_in_bar*speed_multiplyer;
                                    for( int i = 0 ; i != speed_multiplyer ; ++i )
                                    {
                                        int tmp_clock_id = ((clock_id+i)%96);
                                        if( tmp_clock_id % clocks_per_step == 0 )
                                        {
                                            runtime_info.steps_in_block.add( new RuntimeInfo::Step( tmp_clock_id/clocks_per_step, abs_event_time_in_samples+current_samples_per_clock*i, current_samples_per_clock ) );

                                            success = true;
                                        }
                                    }
                                }
                                // X < 1 // WIRD SCHNELLER
                                else
                                {
                                    const double speed_multiplyer__ = 1.0/speed_multiplyer;

                                    const int factor = clocks_per_step*speed_multiplyer__;
                                    const int faster_clocks_per_bar = clocks_per_bar*speed_multiplyer__;
                                    const int faster_clocks_semi_absolut = clock_absolute % faster_clocks_per_bar;
                                    if( faster_clocks_semi_absolut % factor == 0 )
                                    {
                                        runtime_info.steps_in_block.add( new RuntimeInfo::Step( faster_clocks_semi_absolut/factor, abs_event_time_in_samples, (abs_event_time_in_samples-last_clock_sample)*speed_multiplyer__ ) );

                                        success = true;
                                    }
                                }

                                // SYNC SPEED
                                if( is_synced )
                                {
                                    const double current_samples_per_clock = abs_event_time_in_samples-last_clock_sample;
                                    int samples_per_beat = current_samples_per_clock*clocks_per_beat;
                                    double ms_per_beat = samplesToMs(samples_per_beat,sample_rate);
                                    if( not(ms_per_beat > 0 ) )
                                    {
                                        ms_per_beat = 1;
                                    }

                                    current_pos_info.bpm = clock_smoother->add_and_get_average((60.0 * 1000) / ms_per_beat);
                                }

                                last_clock_sample = abs_event_time_in_samples;

                                runtime_info.clock_counter++;
                            }
                            // SYNC START
                            else if( input_midi_message.isMidiStart() )
                            {
                                runtime_info.clock_counter.reset();
                                runtime_info.steps_in_block.clearQuick(true);
                                runtime_info.is_running = true;
                                runtime_info.is_extern_synced = true;

                                if( try_to_restart_arp )
                                {
                                    voice->restart_arp(sample_position);
                                    try_to_restart_arp = false;
                                }

                                DBG( "START" );
                            }
                            // SYNC STOP
                            else if( input_midi_message.isMidiStop() )
                            {
                                MidiMessage notes_off = MidiMessage::allNotesOff(1);
                                runtime_info.is_running = false;

                                voice->stop_arp();
                                try_to_restart_arp = true;

                                DBG( "STOP" );
                            }
                            // SYNC CONTINUE
                            else if( input_midi_message.isMidiContinue() )
                            {
                                runtime_info.is_running = true;
                                runtime_info.is_extern_synced = true;

                                if( try_to_restart_arp )
                                {
                                    voice->restart_arp(sample_position);
                                    try_to_restart_arp = false;
                                }

                                DBG( "START" );
                            }
                        }
                    }
                }
                // +++++ SYNC BLOCK EXTERN MIDI
#endif
#ifdef PROFILE
                {
                    static MidiMessage note_on( MidiMessage::noteOn( 1, 64, 1.0f ) );
                    static bool is_first_block = true;
                    if( is_first_block )
                    {
                        is_first_block = false;
                        midi_messages_.addEvent( note_on, 0 );
                    }

                    if( current_pos_info.timeInSamples > 44100 * 10 )
                        exit(0);
                    else
                        std::cout << "PROCESS NUM SAMPLES:" << current_pos_info.timeInSamples << " LEFT:" << 44100 * 10 - current_pos_info.timeInSamples << std::endl;
                }
#endif
                // PROCESS THE SYNTH
                AppInstanceStore::getInstance()->lock_amp_painter();
                {
                    // RENDER SYNTH
                    get_cc_input_messages( midi_messages_, num_samples );
                    get_note_input_messages( midi_messages_, num_samples );
                    note_down_store->handle_midi_messages( midi_messages_ );
                    synth->renderNextBlock ( buffer_, midi_messages_, 0, num_samples );
                    midi_messages_.clear(); // WILL BE FILLED AT THE END

                    // VISUALIZE
                    if( peak_meter )
                    {
                        ScopedLock locked(peak_meter_lock);
                        peak_meter->process( buffer_.getReadPointer(0), num_samples );
                    }
                }
                AppInstanceStore::getInstance()->unlock_amp_painter();
            }
        }
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD void MoniqueAudioProcessor::prepareToPlay ( double sampleRate, int block_size_ )
{
    // TODO optimize functions without sample rate and block size
    // TODO replace audio sample buffer??
    GET_DATA(data_buffer).resize_buffer_if_required(block_size_);
    synth->setCurrentPlaybackSampleRate (sampleRate);
    RuntimeNotifyer::getInstance()->set_sample_rate( sampleRate );
    RuntimeNotifyer::getInstance()->set_block_size( block_size_ );
    voice->reset_internal();
}
COLD void MoniqueAudioProcessor::releaseResources()
{
    // TODO reset all
}
COLD void MoniqueAudioProcessor::reset()
{
}

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueAudioProcessor::handleNoteOn (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage message( MidiMessage::noteOn( midiChannel, midiNoteNumber, velocity ) );
    message.setTimeStamp( Time::getMillisecondCounterHiRes() );

    collect_incoming_midi_messages
    (
        mono_AudioDeviceManager::INPUT_ID::NOTES,
        message
    );
}
void MoniqueAudioProcessor::handleNoteOff (MidiKeyboardState* /*source*/, int midiChannel, int midiNoteNumber)
{
    MidiMessage message( MidiMessage::noteOff( midiChannel, midiNoteNumber, 0 ) );
    message.setTimeStamp( Time::getMillisecondCounterHiRes() );

    collect_incoming_midi_messages
    (
        mono_AudioDeviceManager::INPUT_ID::NOTES,
        message
    );
}

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueAudioProcessor::trigger_send_feedback() noexcept
{
    Array< Parameter* >& parameters = synth_data->get_atomateable_parameters();
    for( int i = 0 ; i != parameters.size() ; ++ i )
    {
        parameters.getUnchecked(i)->midi_control->send_feedback_only();
    }
}
void MoniqueAudioProcessor::trigger_send_clear_feedback() noexcept
{
    Array< Parameter* >& parameters = synth_data->get_atomateable_parameters();
    for( int i = 0 ; i != parameters.size() ; ++ i )
    {
        parameters.getUnchecked(i)->midi_control->send_clear_feedback_only();
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_PLUGIN
// NOTE THE MODULATION AMOUNT ID OF AN PARAM IS +1
void MoniqueAudioProcessor::init_automatable_parameters() noexcept
{
    Array< Parameter* >&  all_automatable_parameters = synth_data->get_atomateable_parameters();
    for( int i = 0 ; i != all_automatable_parameters.size() ; ++i )
    {
        Parameter* param( all_automatable_parameters.getUnchecked(i) );
        jassert( not automateable_parameters.contains( param ) );
        const_cast< ParameterInfo* >( &param->get_info() )->parameter_host_id = automateable_parameters.size();
        automateable_parameters.add( param );
        param->register_always_listener( this );
        if( has_modulation( param ) )
        {
            automateable_parameters.add( nullptr );
        }
    }
}
int MoniqueAudioProcessor::getNumParameters()
{
    return automateable_parameters.size();
}
/*
bool MoniqueAudioProcessor::isParameterAutomatable ( int ) const
{
    return true;
}
*/
float MoniqueAudioProcessor::getParameter( int i_ )
{
    float value = 0;
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        value = get_percent_value( param );
    }
    else
    {
        value = (1.0f + automateable_parameters.getUnchecked(i_-1)->get_modulation_amount() ) * 0.5;
    }
    return value;
}
const String MoniqueAudioProcessor::getParameterText( int i_ )
{
    String value;
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        value = String(round001(param->get_value()));
    }
    else
    {
        value = String(round01(automateable_parameters.getUnchecked(i_-1)->get_modulation_amount()*100));
    }
    return value;
}
String MoniqueAudioProcessor::getParameterLabel (int i_) const
{
    String value;
    if( not automateable_parameters.getUnchecked(i_) )
    {
        value = "%";
    }
    return value;
}
int MoniqueAudioProcessor::getParameterNumSteps( int i_ )
{
    int value = 0;
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        value = get_num_steps( param );
    }
    else
    {
        value = 1000 * 2;
    }
    return value;
}
float MoniqueAudioProcessor::getParameterDefaultValue( int i_ )
{
    int value = 0;
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        value = get_percent_default_value( param );
    }
    else
    {
        value = (1.0f + get_percent_default_modulation_value( automateable_parameters.getUnchecked(i_-1) ) ) * 0.5;
    }
    return value;
}
const String MoniqueAudioProcessor::getParameterName( int i_ )
{
    String name;
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        name = param->get_info().short_name;
    }
    else
    {
        name = automateable_parameters.getUnchecked(i_-1)->get_info().short_name + String("_MOD");
    }
    return name;
}
void MoniqueAudioProcessor::setParameter( int i_, float percent_ )
{
    if( Parameter*param = automateable_parameters.getUnchecked(i_) )
    {
        set_percent_value( param, percent_ );
    }
    else
    {
        automateable_parameters.getUnchecked(i_-1)->set_modulation_amount( (percent_*2)-1 );
    }
}

//==============================================================================
void MoniqueAudioProcessor::parameter_value_changed( Parameter* param_ ) noexcept
{
    sendParamChangeMessageToListeners( param_->get_info().parameter_host_id, get_percent_value( param_ ) );
}
void MoniqueAudioProcessor::parameter_value_changed_always_notification( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
}
void MoniqueAudioProcessor::parameter_value_on_load_changed( Parameter* param_ ) noexcept
{
    parameter_value_changed( param_ );
    if( has_modulation( param_ ) )
    {
        parameter_modulation_value_changed( param_ );
    }
}
void MoniqueAudioProcessor::parameter_modulation_value_changed( Parameter* param_ ) noexcept
{
    sendParamChangeMessageToListeners( param_->get_info().parameter_host_id+1, (1.0f + param_->get_modulation_amount())*0.5 );
}
#endif

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_PLUGIN
void MoniqueAudioProcessor::getStateInformation ( MemoryBlock& destData )
{
    XmlElement xml("PROJECT-1.0");
    synth_data->save_to( &xml );
    mono_AudioDeviceManager::save_to( &xml );
    copyXmlToBinary ( xml, destData );
}
void MoniqueAudioProcessor::setStateInformation ( const void* data, int sizeInBytes )
{
    ScopedPointer<XmlElement> xml ( getXmlFromBinary ( data, sizeInBytes ) );
    if ( xml )
    {
        if ( xml->hasTagName ( "PROJECT-1.0" ) || xml->hasTagName("MONOLisa")  )
        {
            synth_data->read_from( xml );
            mono_AudioDeviceManager::read_from( xml );
        }
    }
}
#endif

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::hasEditor() const
{
    return true;
}
const String MoniqueAudioProcessor::getName() const
{
#ifdef IS_STANDALONE
    return "";
#else
    return JucePlugin_Name;
#endif
}

//==============================================================================
//==============================================================================
//==============================================================================
const String MoniqueAudioProcessor::getInputChannelName ( int channel_ ) const
{
    return "";
}
const String MoniqueAudioProcessor::getOutputChannelName ( int channel_ ) const
{
    String name;
    switch( channel_ )
    {
    case 0 :
        name = "MONIQUE OUT L";
        break;
    case 1 :
        name = "MONIQUE OUT R";
        break;
    }

    return name;
}

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::isInputChannelStereoPair ( int ) const
{
    return false;
}
bool MoniqueAudioProcessor::isOutputChannelStereoPair ( int id_ ) const
{
    return true;
}

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::acceptsMidi() const
{
    return true;
}
bool MoniqueAudioProcessor::producesMidi() const
{
    return true;
}

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}
double MoniqueAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
//==============================================================================
//==============================================================================
int MoniqueAudioProcessor::getNumPrograms()
{
    int size = 0;
    for( int bank_id = 0 ; bank_id != 4 ; ++bank_id )
    {
        size += synth_data->get_programms( bank_id ).size();
    }

    return size;
}
int MoniqueAudioProcessor::getCurrentProgram()
{
    return synth_data->get_current_programm_id_abs();
}
void MoniqueAudioProcessor::setCurrentProgram ( int id_ )
{
    synth_data->set_current_program_abs(id_);
    synth_data->load();
}
const String MoniqueAudioProcessor::getProgramName ( int id_ )
{
    return synth_data->get_program_name_abs(id_);
}
void MoniqueAudioProcessor::changeProgramName ( int id_, const String& name_ )
{
    synth_data->set_current_program_abs(id_);
    synth_data->rename(name_);
}




