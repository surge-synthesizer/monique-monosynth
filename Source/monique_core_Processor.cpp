#include "monique_core_Processor.h"

#include "monique_core_Datastructures.h"
#include "monique_core_Synth.h"

#include "monique_ui_MainWindow.h"
#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_Refresher.h"
#include "monique_ui_AmpPainter.h"
#include "monique_ui_LookAndFeel.h"

//==============================================================================
//==============================================================================
//==============================================================================
COLD AudioProcessorEditor* MoniqueAudioProcessor::createEditor()
{
    if( not ui_refresher )
    {
        ui_refresher = new Monique_Ui_Refresher( this, ui_look_and_feel, midi_control_handler, synth_data, voice );
    }

    return new Monique_Ui_Mainwindow( ui_refresher );
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoniqueAudioProcessor();
}

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_STANDALONE
#define CLOCKS_TO_SMOOTH (24/6)
class ClockSmoothBuffer : RuntimeListener
{
    int pos;
    float buffer[CLOCKS_TO_SMOOTH];
    float sum;

public:
    //==========================================================================
    inline void add( float val_ ) noexcept
    {
        sum-=buffer[pos];
        buffer[pos] = val_;
        sum+=val_;

        if( ++pos == CLOCKS_TO_SMOOTH )
        {
            pos = 0;
        }
    }
    inline float add_and_get_average( float val_ ) noexcept
    {
        add(val_);
        return get_average();
    }
    inline float get_average() const noexcept
    {
        return sum * ( 1.0 / CLOCKS_TO_SMOOTH );
    }
    inline void reset( float value_ ) noexcept
    {
        sum = 0;
        for( int i = 0 ; i != CLOCKS_TO_SMOOTH ; ++i )
        {
            buffer[i] = value_;
            sum += value_;
        }
    }

    void sample_rate_or_block_changed() noexcept override
    {
        // TODO
    }

public:
    //==========================================================================
    COLD ClockSmoothBuffer( RuntimeNotifyer*const notifyer_ ) : RuntimeListener( notifyer_ ), pos(0), sum(0)
    {
        reset(0);
    }
    COLD ~ClockSmoothBuffer() {}


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClockSmoothBuffer)
};
#endif

//==============================================================================
//==============================================================================
//==============================================================================
class NoteDownStore
{
    MoniqueSynthData*const synth_data;

    int8 root_note;
    int8 second_note;
    int8 third_note;
    bool soft_is_down;
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

    bool are_more_than_one_key_down() const noexcept;

public:
    //==============================================================================
COLD NoteDownStore( MoniqueSynthData*const synth_data_ ) noexcept :
    synth_data(synth_data_),
               root_note(-1),
               second_note(-1),
               third_note(-1),
               soft_is_down(false)
    {}
    COLD ~NoteDownStore() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteDownStore)
};

//==============================================================================
void NoteDownStore::addNote( int8 note_number_, int64 sample_ ) noexcept
{
    NoteTimestampPair pair(note_number_,sample_);
    if( not notes_down.contains( pair ) )
    {
        notes_down.add( pair );
    }

    float master_tune = synth_data->osc_datas[MASTER_OSC]->tune;
    if( root_note == -1 )
    {
        root_note = note_number_;
    }
    else if( second_note == -1 )
    {
        second_note = note_number_;
        synth_data->osc_datas[1]->tune.set_value( float( second_note - root_note ) + master_tune );
    }
    else if( third_note == -1 )
    {
        third_note = note_number_;
        synth_data->osc_datas[2]->tune.set_value( float( third_note - root_note ) + master_tune );
    }
    else
    {
        third_note = note_number_;
        synth_data->osc_datas[2]->tune.set_value( float( third_note - root_note ) + master_tune );
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
        if( input_midi_message.isSoftPedalOn() )
        {
            soft_is_down = true;
        }
        if( input_midi_message.isSoftPedalOff() )
        {
            soft_is_down = false;
        }
        else
        {
            if( input_midi_message.isNoteOn() )
            {
                if( soft_is_down )
                {
                    addNote( input_midi_message.getNoteNumber(), sample_position );
                }
                else
                {
                    // ONLY REMEMBER THe LAST ONE IF THE PEDAL IS NOT DOWN
                    reset();
                    addNote( input_midi_message.getNoteNumber(), sample_position );
                }
            }
            else if( input_midi_message.isNoteOff() )
            {
                removeNote( input_midi_message.getNoteNumber(), sample_position );
            }
        }
    }

    if( not soft_is_down )
    {
        reset();
    }
}
bool NoteDownStore::are_more_than_one_key_down() const noexcept
{
    return notes_down.size() > 1;
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
mono_AudioDeviceManager( new RuntimeNotifyer() ),
#ifdef IS_STANDALONE
                         last_clock_sample(0),
                         last_step_sample(0),
                         try_to_restart_arp( false ),
                         connection_missed_counter(0),
                         received_a_clock_in_time(false),
#endif
                         stored_note(-1),
                         stored_velocity(0),
                         loop_counter(0),

                         peak_meter(nullptr),
#ifdef IS_PLUGIN
                         restore_time(-1),
#endif

                         force_sample_rate_update(true),

                         amp_painter(nullptr)
{
    SHARED::getInstance()->num_instances++;
#ifdef IS_STANDALONE
    clock_smoother = new ClockSmoothBuffer(runtime_notifyer);
#endif
    std::cout << "MONIQUE: init core" << std::endl;
    {
        ui_look_and_feel = new UiLookAndFeel();
        LookAndFeel::setDefaultLookAndFeel( ui_look_and_feel );
        midi_control_handler = new MIDIControlHandler( ui_look_and_feel, this );

        info = new RuntimeInfo();
        data_buffer = new DataBuffer(1);
        synth_data = new MoniqueSynthData( MASTER, ui_look_and_feel, this, runtime_notifyer, info, data_buffer );
        ui_look_and_feel->set_synth_data(synth_data);
        voice = new MoniqueSynthesiserVoice( this, synth_data, runtime_notifyer, info, data_buffer );
        synth_data->voice = voice;
        synth = new MoniqueSynthesizer( synth_data, voice, new MoniqueSynthesiserSound(), midi_control_handler );

        note_down_store = new NoteDownStore( synth_data );
    }

    std::cout << "MONIQUE: init load last project and settings" << std::endl;
    {
#ifdef IS_STANDALONE
        synth_data->load_default();
#endif
        synth_data->load_settings();
        synth_data->read_midi();
#ifdef IS_STANDALONE
        synth_data->load(true);
#else
        synth_data->load_default();
#endif
    }
#ifdef IS_PLUGIN
    init_automatable_parameters();
#endif

#ifdef IS_STANDALONE
    audio_is_successful_initalized = (mono_AudioDeviceManager::read() == "");
    if( audio_is_successful_initalized )
    {
        AudioDeviceManager::AudioDeviceSetup setup;
        getAudioDeviceSetup(setup);
        setPlayConfigDetails ( 0, 2, setup.sampleRate, setup.bufferSize );
        addAudioCallback (&player);
        player.setProcessor (this);

        prepareToPlay( setup.sampleRate, setup.bufferSize );
    }
#endif
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW | _CRTDBG_MODE_WNDW);
    DBG("init done");
}

COLD MoniqueAudioProcessor::~MoniqueAudioProcessor() noexcept
{
    clear_feedback_and_shutdown();

#ifdef IS_STANDALONE
    delete clock_smoother;
    mono_AudioDeviceManager::save();
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

    SHARED::getInstance()->num_instances--;
    if( SHARED::getInstance()->num_instances == 0 )
    {
        if( SHARED::getInstance()->env_clipboard )
        {
            ENVData*env = SHARED::getInstance()->env_clipboard;
            SHARED::getInstance()->env_clipboard = nullptr;
            delete env;
        }
        if( SHARED::getInstance()->mfo_clipboard )
        {
            LFOData*mfo = SHARED::getInstance()->mfo_clipboard;
            SHARED::getInstance()->mfo_clipboard = nullptr;
            delete mfo;
        }
    }

    ui_look_and_feel->clear_synth_data();
    synth->removeVoice(0);
    delete synth;
    delete synth_data;

    ui_refresher = nullptr;
    midi_control_handler = nullptr;
    ui_look_and_feel = nullptr;
    data_buffer = nullptr;
    info = nullptr;
    note_down_store = nullptr;
#ifdef IS_STANDALONE
    set_audio_online();
#endif
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
    if( info->is_extern_synced )
    {
        if( not received_a_clock_in_time )
        {
            connection_missed_counter++;
            if( connection_missed_counter > 14 )
            {
                info->is_extern_synced = false;
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
    voice->bypass_smoother.set_value( true );
    process( buffer_, midi_messages_, false );
}
void MoniqueAudioProcessor::processBlockBypassed( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_ )
{
    voice->bypass_smoother.set_value( false );
    process( buffer_, midi_messages_, true );
}
void MoniqueAudioProcessor::process ( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_, bool bypassed_ )
{
#ifdef IS_STANDALONE
    if( not block_lock.tryEnter() )
    {
        return;
    }
#endif
    if( buffer_.getNumChannels() < 1 )
    {
        return;
    }

    if( sample_rate != getSampleRate() || getBlockSize() != block_size )
    {
        prepareToPlay(getSampleRate(),getBlockSize());
    }

    const int num_samples = buffer_.getNumSamples();
    buffer_.clear();

    const int64 last_samples_since_start = current_pos_info.timeInSamples;
    const bool was_playing = current_pos_info.isPlaying or current_pos_info.isRecording;

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

    {
        {
#else // PLUGIN
    if ( getPlayHead() != nullptr )
    {
        if( getPlayHead()->getCurrentPosition ( current_pos_info ) )
        {
#endif
            if( current_pos_info.isLooping )
            {
                if( last_samples_since_start != 0 ) // DO NOT USE AT THE START
                {
                    current_pos_info.timeInSamples = last_samples_since_start;
                }
            }

            //if( current_pos_info.timeInSamples + num_samples >= 0 ) //&& current_pos_info.isPlaying )
            {
#ifdef IS_STANDALONE
                // +++++ SYNC BLOCK EXTERN MIDI
                {
                    // CLEAN LAST BLOCK
                    // FOR SECURITy REMOVE INVALID OLD STEPS
                    OwnedArray< RuntimeInfo::Step >& steps_in_block( info->steps_in_block );
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
                    info->clock_sync_information.clear();

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

                        const double speed_multiplyer = ArpSequencerData::speed_multi_to_value( synth_data->arp_sequencer_data->speed_multi );
                        static constexpr int clocks_per_step = 6;
                        static constexpr int clocks_per_beat = 24;
                        static constexpr int clocks_per_bar = 96;

                        while( message_iter.getNextEvent( input_midi_message, sample_position ) )
                        {
                            const int64 abs_event_time_in_samples = current_pos_info.timeInSamples+sample_position;

                            // CLOCK
                            if( input_midi_message.isMidiClock() )
                            {
                                Timer::stopTimer();
                                received_a_clock_in_time = true;
                                info->is_extern_synced = true;
                                info->clock_sync_information.add_clock( sample_position, abs_event_time_in_samples-last_clock_sample );

                                const int clock_in_bar = info->clock_counter.clock();
                                const int clock_absolute = info->clock_counter.clock_absolut();
                                const int is_step = info->clock_counter.is_step();

                                if( is_step )
                                {
                                    const int64 current_samples_per_step = abs_event_time_in_samples-last_step_sample;
                                    last_step_sample = abs_event_time_in_samples;

                                    // UPDATE SPEED INFO (should be used for ui)
                                    {
                                        int samples_per_beat = current_samples_per_step*4;
                                        double ms_per_beat = samplesToMs(samples_per_beat,sample_rate);
                                        if( ms_per_beat < 1 )
                                        {
                                            ms_per_beat = 1;
                                        }
                                        current_pos_info.bpm = clock_smoother->add_and_get_average((60.0f * 1000) / ms_per_beat);
                                    }
                                }

                                // X 1
                                bool success = false;
                                if( speed_multiplyer == 1 )
                                {
                                    if( clock_in_bar > 0 )
                                    {
                                        if( clock_in_bar % clocks_per_step == 0 )
                                        {
                                            info->steps_in_block.add( new RuntimeInfo::Step( clock_in_bar/clocks_per_step, abs_event_time_in_samples+1, abs_event_time_in_samples-last_clock_sample ) );
                                            success = true;
                                        }
                                    }
                                    else
                                    {
                                        info->steps_in_block.add( new RuntimeInfo::Step( 0, abs_event_time_in_samples+1, 0 ) );
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
                                        if( clock_id > 0 )
                                        {
                                            int tmp_clock_id = ((clock_id+i)%96);
                                            if( tmp_clock_id % clocks_per_step == 0 )
                                            {
                                                info->steps_in_block.add( new RuntimeInfo::Step( tmp_clock_id/clocks_per_step, abs_event_time_in_samples+current_samples_per_clock*i +1, current_samples_per_clock ) );

                                                success = true;
                                            }
                                        }
                                        else
                                        {
                                            info->steps_in_block.add( new RuntimeInfo::Step( 0, abs_event_time_in_samples+1, 0 ) );
                                            success = true;
                                        }
                                    }
                                }
                                // X < 1 // WIRD SCHNELLER
                                else
                                {
                                    if( clock_absolute > 0 )
                                    {
                                        const double speed_multiplyer__ = 1.0/speed_multiplyer;

                                        const double factor = speed_multiplyer__*clocks_per_step;
                                        const double faster_clocks_per_bar = speed_multiplyer__*clocks_per_bar;
                                        const double faster_clocks_semi_absolut = fmod(clock_absolute, faster_clocks_per_bar);
                                        if( fmod(faster_clocks_semi_absolut, factor) == 0 )
                                        {
                                            info->steps_in_block.add( new RuntimeInfo::Step( faster_clocks_semi_absolut/factor, abs_event_time_in_samples+1, (abs_event_time_in_samples-last_clock_sample)*speed_multiplyer__ ) );

                                            success = true;
                                        }
                                    }
                                    else
                                    {
                                        info->steps_in_block.add( new RuntimeInfo::Step( 0, abs_event_time_in_samples+1, 0 ) );
                                    }
                                }

                                last_clock_sample = abs_event_time_in_samples;

                                info->clock_counter++;
                                Timer::startTimer( 100 );
                            }
                            // SYNC START
                            else if( input_midi_message.isMidiStart() )
                            {
                                info->clock_counter.reset();
                                info->steps_in_block.clearQuick(true);
                                info->is_running = true;
                                info->is_extern_synced = true;

                                last_clock_sample = last_clock_sample-abs_event_time_in_samples;
                                last_step_sample = last_step_sample-abs_event_time_in_samples;
                                current_pos_info.timeInSamples = last_clock_sample;

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
                                info->is_running = false;

                                voice->stop_arp();
                                try_to_restart_arp = true;

                                DBG( "STOP" );
                            }
                            // SYNC CONTINUE
                            else if( input_midi_message.isMidiContinue() )
                            {
                                info->is_running = true;
                                info->is_extern_synced = true;

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
                {
                    // RENDER SYNTH
#ifdef IS_STANDALONE
                    get_cc_input_messages( midi_messages_, num_samples );
                    get_note_input_messages( midi_messages_, num_samples );
                    info->clock_sync_information.create_a_working_copy();
#endif
                    MidiKeyboardState::processNextMidiBuffer( midi_messages_, 0, num_samples, true );
                    note_down_store->handle_midi_messages( midi_messages_ );

                    const bool is_playing = current_pos_info.isPlaying or current_pos_info.isRecording;
                    if( was_playing and not is_playing )
                    {
                        voice->stop_arp();
                        if( Monique_Ui_AmpPainter* amp_painter = synth_data->audio_processor->amp_painter )
                        {
                            amp_painter->clear_and_keep_minimum();
                        }
                    }
                    else if( not was_playing and is_playing )
                    {
                        voice->restart_arp(0);
                    }

                    synth->renderNextBlock ( buffer_, midi_messages_, 0, num_samples );
                    midi_messages_.clear(); // WILL BE FILLED AT THE END
                }
#ifdef IS_STANDALONE
                send_feedback_messages(num_samples);
                send_thru_messages(num_samples);
#else
                send_feedback_messages(midi_messages_, num_samples);
#endif
            }
        }
    }



#ifdef IS_STANDALONE
    current_pos_info.timeInSamples += buffer_.getNumSamples();
    block_lock.exit();
#else
    if( current_pos_info.isLooping )
    {
        if( current_pos_info.isPlaying or current_pos_info.isRecording)
        {
            current_pos_info.timeInSamples += buffer_.getNumSamples();
        }
        else
        {
            current_pos_info.timeInSamples = 0;
        }
    }
#endif
}
//==============================================================================
//==============================================================================
//==============================================================================
COLD void MoniqueAudioProcessor::prepareToPlay ( double sampleRate, int block_size_ )
{
    current_pos_info.timeInSamples = 0;

    // TODO optimize functions without sample rate and block size
    // TODO replace audio sample buffer??
    if( sampleRate > 0 )
    {
        voice->bypass_smoother.reset( sampleRate, 30 );

        synth->setCurrentPlaybackSampleRate (sampleRate);
        runtime_notifyer->set_sample_rate( sampleRate );
    }
    if( block_size_ > 0 )
    {
        runtime_notifyer->set_block_size(block_size_);
        data_buffer->resize_buffer_if_required(block_size_);
    }

    voice->reset_internal();
}
COLD void MoniqueAudioProcessor::sample_rate_or_block_changed() noexcept
{
    const bool sr_changed = runtime_notifyer->get_sample_rate() != getSampleRate();
    const bool block_changed = runtime_notifyer->get_block_size() != getBlockSize();
    if( sr_changed or block_changed or force_sample_rate_update )
    {
        force_sample_rate_update = false;
        prepareToPlay(runtime_notifyer->get_sample_rate(),runtime_notifyer->get_block_size());
    }
    mono_AudioDeviceManager::sample_rate_or_block_changed();
}
COLD void MoniqueAudioProcessor::releaseResources()
{
    current_pos_info.timeInSamples = 0;
    voice->bypass_smoother.set_value( false );

    voice->stop_internal();
}
COLD void MoniqueAudioProcessor::reset()
{
    current_pos_info.timeInSamples = 0;
    voice->bypass_smoother.set_value( false );
}

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::are_more_than_one_key_down() const noexcept
{
    return note_down_store->are_more_than_one_key_down();
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
bool MoniqueAudioProcessor::isMetaParameter (int i_) const
{
    Parameter*param = automateable_parameters.getUnchecked(i_);
    if( ! param )
    {
        param = automateable_parameters.getUnchecked(i_-1);
    }

    if( TYPES_DEF::IS_BOOL != type_of( param ) )
    {
        if( param->get_info().short_name.contains("morph") )
        {
            return true;
        }
    }

    return false;
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
#ifdef IS_PLUGIN
void MoniqueAudioProcessor::getStateInformation ( MemoryBlock& destData )
{
    XmlElement xml("PROJECT-1.0");
    String modded_name = synth_data->alternative_program_name;
    String name = modded_name.fromFirstOccurrenceOf ("0RIGINAL WAS: ",false, false);
    xml.setAttribute( "MODDED_PROGRAM", name == "" ? modded_name : name );
    //xml.getIntAttribute( "BANK", synth_data->current_bank );
    //xml.getIntAttribute( "PROG", synth_data->current_program );
    synth_data->save_to( &xml );
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
            String modded_name = synth_data->alternative_program_name;
            String old_name = xml->getStringAttribute( "MODDED_PROGRAM", "1234567899876543212433442424678" );
            if( old_name != "1234567899876543212433442424678" )
            {
                synth_data->alternative_program_name = String("0RIGINAL WAS: ") + old_name;
                //synth_data->current_bank = xml->getIntAttribute( "BANK", 0 );
                //synth_data->current_program = xml->getIntAttribute( "PROG", -1 );
            }
        }
    }
    else
    {
        synth_data->alternative_program_name = "ERROR: Could not load patch!";
    }

    restore_time = Time::getMillisecondCounter();
}
/*
void getCurrentProgramStateInformation ( MemoryBlock& dest_data_ )
{

}
void setCurrentProgramStateInformation ( const void* data_, int size_in_bytes_ )
{

}
*/
#endif

//==============================================================================
//==============================================================================
//==============================================================================
int MoniqueAudioProcessor::getNumPrograms()
{
    int size = 0;
    for( int bank_id = 0 ; bank_id != 26 ; ++bank_id )
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
#ifdef IS_PLUGIN
    if((Time::getMillisecondCounter()-restore_time) < synth_data->program_restore_block_time)
    {
        return;
    }
#endif

    synth_data->set_current_program_abs(id_);
    synth_data->load(true,true);
}
const String MoniqueAudioProcessor::getProgramName ( int id_ )
{
    return synth_data->get_program_name_abs(id_);
}
void MoniqueAudioProcessor::changeProgramName ( int id_, const String& name_ )
{
    synth_data->set_current_program_abs(id_);
    synth_data->rename(name_);
    if( get_editor() )
    {
        get_editor()->triggerAsyncUpdate();
    }
}





