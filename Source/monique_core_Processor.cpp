#include "monique_core_Processor.h"

#include "monique_core_Datastructures.h"
#include "monique_core_Synth.h"

#include "monique_ui_MainWindow.h"
#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_Refresher.h"
#include "monique_ui_AmpPainter.h"
#include "monique_ui_LookAndFeel.h"
#include "version.h"

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

//==============================================================================
//==============================================================================
//==============================================================================
static inline void crash_handler(void*)
{
    /*
       A void() function type, used by setApplicationCrashHandler().
       typedef void (*CrashHandlerFunction)();
    */
    File folder = GET_ROOT_FOLDER();
    folder = File(folder.getFullPathName()+PROJECT_FOLDER);
    folder.createDirectory();
    File report( folder.getFullPathName() + String("/crash_log.txt") );
    report.appendText( String("\n\n\n") + String(Time::getMillisecondCounter() ) );
    report.appendText( SystemStats::getStackBacktrace() );
}

struct MoniqueAudioProcessor::standalone_features : public Timer
{
    AudioProcessorPlayer player;
    bool audio_is_successful_initalized = false;

    std::unique_ptr<ClockSmoothBuffer> clock_smoother;
    int64 last_clock_sample = 0;
    int64 last_step_sample = 0;

    bool received_a_clock_in_time = false;
    int connection_missed_counter = 0;

    RuntimeInfo* runtime_info = nullptr;

    CriticalSection block_lock;

    void set_audio_offline() noexcept
    {
        block_lock.enter();
    }

    void set_audio_online() noexcept
    {
        block_lock.exit();
    }

    void timerCallback()
    {
        jassert(runtime_info);

        if( runtime_info->standalone_features_pimpl->is_extern_synced )
        {
            if( not received_a_clock_in_time )
            {
                connection_missed_counter++;
                if( connection_missed_counter > 14 )
                {
                    runtime_info->standalone_features_pimpl->is_extern_synced = false;
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
};

COLD MoniqueAudioProcessor::MoniqueAudioProcessor() noexcept
        : mono_AudioDeviceManager(new RuntimeNotifyer())
        , stored_note(-1)
        , stored_velocity(0)
        , samplePosition(0)
        , lastBlockTime(0)
        , peak_meter(nullptr)
        , restore_time(-1)
        , force_sample_rate_update(true)
        , sampleReader(nullptr)
        , amp_painter(nullptr)
{
    SystemStats::setApplicationCrashHandler(&crash_handler);

    instance_id = SHARED::getInstance()->num_instances;
    SHARED::getInstance()->num_instances++;

    if (is_standalone())
    {
        standalone_features_pimpl = std::make_unique<MoniqueAudioProcessor::standalone_features>();
        standalone_features_pimpl->clock_smoother = std::make_unique<ClockSmoothBuffer>(runtime_notifyer);
    }

    DBG("MONIQUE: init core");
    DBG("MONIQUE: version - " << Monique::Build::FullVersionStr << " built at " << Monique::Build::BuildDate << " " << Monique::Build::BuildTime);

    ui_look_and_feel = new UiLookAndFeel();
    LookAndFeel::setDefaultLookAndFeel(ui_look_and_feel);
    midi_control_handler = new MIDIControlHandler(ui_look_and_feel, this);

    info = new RuntimeInfo();
    if (is_standalone())
    {
        standalone_features_pimpl->runtime_info = info;
    }
    data_buffer = new DataBuffer(1);
    synth_data = new MoniqueSynthData(MASTER, ui_look_and_feel, this, runtime_notifyer, info, data_buffer);
    ui_look_and_feel->set_synth_data(synth_data);
    voice = new MoniqueSynthesiserVoice(this, synth_data, runtime_notifyer, info, data_buffer);
    synth_data->voice = voice;
    synth = new MoniqueSynthesizer(synth_data, voice, new MoniqueSynthesiserSound(), midi_control_handler);

    DBG("MONIQUE: init load last project and settings");

    if (is_standalone())
    {
        synth_data->load_default();
    }

    synth_data->load_settings();
    synth_data->read_midi();

    if (is_standalone())
    {
        synth_data->load(true);
    }
    else
    {
        synth_data->load_default();
    }

    // PARAMETER ORDER
    {
        struct StringPair
        {
            String name;
            String short_name;

            inline StringPair( const String& ident_name_, const String& short_name_ ) noexcept :
            name(ident_name_), short_name(short_name_) {}
            inline StringPair() noexcept {}
        };

        struct StringList
        {
            Array< StringPair > pairs;
            inline void add( String ident_name_, String short_name_ ) noexcept
            {
                pairs.add( StringPair( ident_name_.trim(), short_name_.trim() ) );
            }

        } list;

        //          INDEX AND SAVE NAME				OVERWRITE NAME FOR LIST
        list.add("	SD_2_shift                       	" ,"	CFG SHIFT	");
        list.add("	OSC_0_key-sync                  	" ,"	OSC1 KeySync	");
        list.add("	OSC_0_wave                      	" ,"	OSC1 Wave	");
        list.add("	OSC_0_fm_power                  	" ,"	OSC1 FM Mass	");
        list.add("	OSC_0_master_shift              	" ,"	OSC1 Phase	");
        list.add("	OSC_0_is_lfo_mod                	" ,"	OSC1 PhaseMod	");
        list.add("	OSC_1_sync                      	" ,"	OSC2 Sync	");
        list.add("	OSC_1_wave                      	" ,"	OSC2 Wave	");
        list.add("	OSC_1_fm_power                  	" ,"	OSC2 FM Mass	");
        list.add("	OSC_1_octave                    	" ,"	OSC2 Tune	");
        list.add("	OSC_1_is_lfo_mod                	" ,"	OSC2 TuneMod	");
        list.add("	OSC_2_sync                      	" ,"	OSC3 Sync	");
        list.add("	OSC_2_wave                      	" ,"	OSC3 Wave	");
        list.add("	OSC_2_fm_power                  	" ,"	OSC3 FM Mass	");
        list.add("	OSC_2_octave                    	" ,"	OSC3 Tune	");
        list.add("	OSC_2_is_lfo_mod                	" ,"	OSC3 TuneMod 	");
        list.add("	OSC_0_fm_multi                  	" ,"	FM Tune		");
        list.add("	OSC_0_sync                      	" ,"	FM Sync		");
        list.add("	OSC_0_fm_swing                  	" ,"	FM Swing	");
        list.add("	OSC_0_fm_phase                  	" ,"	FM Shape	");
        list.add("	FLT_0_input_sustain_0           	" ,"	F1 OSC1 Input	");
        list.add("	FLT_0_input_hold_0              	" ,"	F1 ONV1 Active	");
        list.add("	ENV_100_attack                  	" ,"	F1 ONV1 Att	");
        list.add("	ENV_100_decay                   	" ,"	F1 ONV1 Dec	");
        list.add("	ENV_100_sustain                 	" ,"	F1 ONV1 Sus	");
        list.add("	ENV_100_retrigger               	" ,"	F1 ONV1 Retrig	");
        list.add("	ENV_100_release                 	" ,"	F1 ONV1 Rel	");
        list.add("	ENV_100_shape                   	" ,"	F1 ONV1 Slope	");
        list.add("	FLT_0_input_sustain_1           	" ,"	F1 OSC2 Input	");
        list.add("	FLT_0_input_hold_1              	" ,"	F1 ONV2 Active	");
        list.add("	ENV_101_attack                  	" ,"	F1 ONV2 Att	");
        list.add("	ENV_101_decay                   	" ,"	F1 ONV2 Dec	");
        list.add("	ENV_101_sustain                 	" ,"	F1 ONV2 Sus	");
        list.add("	ENV_101_retrigger               	" ,"	F1 ONV2 Retrig	");
        list.add("	ENV_101_release                 	" ,"	F1 ONV2 Rel	");
        list.add("	ENV_101_shape                   	" ,"	F1 ONV2 Slope	");
        list.add("	FLT_0_input_sustain_2           	" ,"	F1 OSC3 Input	");
        list.add("	FLT_0_input_hold_2              	" ,"	F1 ONV3 Active	");
        list.add("	ENV_102_attack                  	" ,"	F1 ONV3 Att	");
        list.add("	ENV_102_decay                   	" ,"	F1 ONV3 Dec	");
        list.add("	ENV_102_sustain                 	" ,"	F1 ONV3 Sus	");
        list.add("	ENV_102_retrigger               	" ,"	F1 ONV3 Retrig	");
        list.add("	ENV_102_release                 	" ,"	F1 ONV3 Rel	");
        list.add("	ENV_102_shape                   	" ,"	F1 ONV3 Slope	");
        list.add("	ENV_0_attack                    	" ,"	F1 ENV Att	");
        list.add("	ENV_0_decay                     	" ,"	F1 ENV Dec	");
        list.add("	ENV_0_sustain                   	" ,"	F1 ENV Sus	");
        list.add("	ENV_0_retrigger                 	" ,"	F1 ENV Retrig	");
        list.add("	ENV_0_release                   	" ,"	F1 ENV Rel	");
        list.add("	ENV_0_shape                     	" ,"	F1 ENV Slope	");
        list.add("	FLT_0_adsr_lfo_mix              	" ,"	F1 ModMix	");
        list.add("	LFO_0_speed                     	" ,"	LFO1 Speed	");
        list.add("	LFO_0_wave                      	" ,"	LFO1 Wave	");
        list.add("	LFO_0_phase                     	" ,"	LFO1 Phase	");
        list.add("	FLT_0_filter_type               	" ,"	F1 FilterType	");
        list.add("	FLT_0_cutoff                    	" ,"	F1 Cutoff	");
        list.add("	FLT_0_modulate_cutoff           	" ,"	F1 CutoffMod	");
        list.add("	FLT_0_resonance                 	" ,"	F1 Reso	");
        list.add("	FLT_0_modulate_resonance        	" ,"	F1 ResoMod	");
        list.add("	FLT_0_distortion                	" ,"	F1 Dist	");
        list.add("	FLT_0_modulate_distortion       	" ,"	F1 DistMod	");
        list.add("	FLT_0_pan                       	" ,"	F1 Pan	");
        list.add("	FLT_0_modulate_pan              	" ,"	F1 PanMod	");
        list.add("	FLT_0_output                    	" ,"	F1 Volume	");
        list.add("	FLT_0_modulate_output           	" ,"	F1 VolumeMod	");
        list.add("	FLT_1_input_sustain_0           	" ,"	F2 OSC1 Input	");
        list.add("	FLT_1_input_hold_0              	" ,"	F2 ONV1 Active	");
        list.add("	ENV_103_attack                  	" ,"	F2 ONV1 Att	");
        list.add("	ENV_103_decay                   	" ,"	F2 ONV1 Dec	");
        list.add("	ENV_103_sustain                 	" ,"	F2 ONV1 Sus	");
        list.add("	ENV_103_retrigger               	" ,"	F2 ONV1 Retrig	");
        list.add("	ENV_103_release                 	" ,"	F2 ONV1 Rel	");
        list.add("	ENV_103_shape                   	" ,"	F2 ONV1 Slope	");
        list.add("	FLT_1_input_sustain_1           	" ,"	F2 OSC2 Input	");
        list.add("	FLT_1_input_hold_1              	" ,"	F2 ONV2 Active	");
        list.add("	ENV_104_attack                  	" ,"	F2 ONV2 Att	");
        list.add("	ENV_104_decay                   	" ,"	F2 ONV2 Dec	");
        list.add("	ENV_104_sustain                 	" ,"	F2 ONV2 Sus	");
        list.add("	ENV_104_retrigger               	" ,"	F2 ONV2 Retrig	");
        list.add("	ENV_104_release                 	" ,"	F2 ONV2 Rel	");
        list.add("	ENV_104_shape                   	" ,"	F2 ONV2 Slope	");
        list.add("	FLT_1_input_sustain_2           	" ,"	F2 OSC3 Input	");
        list.add("	FLT_1_input_hold_2              	" ,"	F2 ONV3 Active	");
        list.add("	ENV_105_attack                  	" ,"	F2 ONV3 Att	");
        list.add("	ENV_105_decay                   	" ,"	F2 ONV3 Dec	");
        list.add("	ENV_105_sustain                 	" ,"	F2 ONV3 Sus	");
        list.add("	ENV_105_retrigger               	" ,"	F2 ONV3 Retrig	");
        list.add("	ENV_105_release                 	" ,"	F2 ONV3 Rel	");
        list.add("	ENV_105_shape                   	" ,"	F2 ONV3 Slope	");
        list.add("	ENV_1_attack                    	" ,"	F2 ENV Att	");
        list.add("	ENV_1_decay                     	" ,"	F2 ENV Dec	");
        list.add("	ENV_1_sustain                   	" ,"	F2 ENV Sus	");
        list.add("	ENV_1_retrigger                 	" ,"	F2 ENV Retrig	");
        list.add("	ENV_1_release                   	" ,"	F2 ENV Rel	");
        list.add("	ENV_1_shape                     	" ,"	F2 ENV Slope	");
        list.add("	FLT_1_adsr_lfo_mix              	" ,"	F2 ModMix	");
        list.add("	LFO_1_speed                     	" ,"	LFO2 Speed	");
        list.add("	LFO_1_wave                      	" ,"	LFO2 Wave	");
        list.add("	LFO_1_phase                     	" ,"	LFO2 Phase	");
        list.add("	FLT_1_filter_type               	" ,"	F2 FilterType	");
        list.add("	FLT_1_cutoff                    	" ,"	F2 Cutoff	");
        list.add("	FLT_1_modulate_cutoff           	" ,"	F2 CutoffMod	");
        list.add("	FLT_1_resonance                 	" ,"	F2 Reso	");
        list.add("	FLT_1_modulate_resonance        	" ,"	F2 ResoMod	");
        list.add("	FLT_1_distortion                	" ,"	F2 Dist	");
        list.add("	FLT_1_modulate_distortion       	" ,"	F2 DistMod	");
        list.add("	FLT_1_pan                       	" ,"	F2 Pan	");
        list.add("	FLT_1_modulate_pan              	" ,"	F2 PanMod	");
        list.add("	FLT_1_output                    	" ,"	F2 Volume	");
        list.add("	FLT_1_modulate_output           	" ,"	F2 VolumeMod	");
        list.add("	FLT_2_input_sustain_0           	" ,"	F3 OSC1 Input	");
        list.add("	FLT_2_input_hold_0              	" ,"	F3 ONV1 Active	");
        list.add("	ENV_106_attack                  	" ,"	F3 ONV1 Att	");
        list.add("	ENV_106_decay                   	" ,"	F3 ONV1 Dec	");
        list.add("	ENV_106_sustain                 	" ,"	F3 ONV1 Sus	");
        list.add("	ENV_106_retrigger               	" ,"	F3 ONV1 Retrig	");
        list.add("	ENV_106_release                 	" ,"	F3 ONV1 Rel	");
        list.add("	ENV_106_shape                   	" ,"	F3 ONV1 Slope	");
        list.add("	FLT_2_input_sustain_1           	" ,"	F3 OSC2 Input	");
        list.add("	FLT_2_input_hold_1              	" ,"	F3 ONV2 Active	");
        list.add("	ENV_107_attack                  	" ,"	F3 ONV2 Att	");
        list.add("	ENV_107_decay                   	" ,"	F3 ONV2 Dec	");
        list.add("	ENV_107_sustain                 	" ,"	F3 ONV2 Sus	");
        list.add("	ENV_107_retrigger               	" ,"	F3 ONV2 Retrig	");
        list.add("	ENV_107_release                 	" ,"	F3 ONV2 Rel	");
        list.add("	ENV_107_shape                   	" ,"	F3 ONV2 Slope	");
        list.add("	FLT_2_input_sustain_2           	" ,"	F3 OSC3 Input	");
        list.add("	FLT_2_input_hold_2              	" ,"	F3 ONV3 Active	");
        list.add("	ENV_108_attack                  	" ,"	F3 ONV3 Att	");
        list.add("	ENV_108_decay                   	" ,"	F3 ONV3 Dec	");
        list.add("	ENV_108_sustain                 	" ,"	F3 ONV3 Sus	");
        list.add("	ENV_108_retrigger               	" ,"	F3 ONV3 Retrig	");
        list.add("	ENV_108_release                 	" ,"	F3 ONV3 Rel	");
        list.add("	ENV_108_shape                   	" ,"	F3 ONV3 Slope	");
        list.add("	ENV_2_attack                    	" ,"	F3 ENV Att	");
        list.add("	ENV_2_decay                     	" ,"	F3 ENV Dec	");
        list.add("	ENV_2_sustain                   	" ,"	F3 ENV Sus	");
        list.add("	ENV_2_retrigger                 	" ,"	F3 ENV Retrig	");
        list.add("	ENV_2_release                   	" ,"	F3 ENV Rel	");
        list.add("	ENV_2_shape                     	" ,"	F3 ENV Slope	");
        list.add("	FLT_2_adsr_lfo_mix              	" ,"	F3 ModMix	");
        list.add("	LFO_2_speed                     	" ,"	LFO3 Speed	");
        list.add("	LFO_2_wave                      	" ,"	LFO3 Wave	");
        list.add("	LFO_2_phase                     	" ,"	LFO3 Phase	");
        list.add("	FLT_2_filter_type               	" ,"	F3 FilterType	");
        list.add("	FLT_2_cutoff                    	" ,"	F3 Cutoff	");
        list.add("	FLT_2_modulate_cutoff           	" ,"	F3 CutoffMod	");
        list.add("	FLT_2_resonance                 	" ,"	F3 Reso	");
        list.add("	FLT_2_modulate_resonance        	" ,"	F3 ResoMod	");
        list.add("	FLT_2_distortion                	" ,"	F3 Dist	");
        list.add("	FLT_2_modulate_distortion       	" ,"	F3 DistMod	");
        list.add("	FLT_2_pan                       	" ,"	F3 Pan	");
        list.add("	FLT_2_modulate_pan              	" ,"	F3 PanMod	");
        list.add("	FLT_2_output                    	" ,"	F3 Volume	");
        list.add("	FLT_2_modulate_output           	" ,"	F3 VolumeMod	");
        list.add("	SD_2_morph_state_0              	" ,"	M_OSC State *meta");
        list.add("	SD_2_morph_switch_state_0       	" ,"	M_OSC Toggle *meta");
        list.add("	MFO_0_speed                     	" ,"	M_OSC Speed	");
        list.add("	MFO_0_wave                      	" ,"	M_OSC Wave	");
        list.add("	MFO_0_phase                     	" ,"	M_OSC Phase	");
        list.add("	SD_2_is_morph_modulated_0       	" ,"	M_OSC Active *meta");
        list.add("	SD_2_morph_state_1              	" ,"	M_FLT State *meta");
        list.add("	SD_2_morph_switch_state_1       	" ,"	M_FLT Toggle *meta");
        list.add("	MFO_1_speed                     	" ,"	M_FLT Speed	");
        list.add("	MFO_1_wave                      	" ,"	M_FLT Wave	");
        list.add("	MFO_1_phase                     	" ,"	M_FLT Phase	");
        list.add("	SD_2_is_morph_modulated_1       	" ,"	M_FLT Active *meta");
        list.add("	SD_2_morph_state_3              	" ,"	M_ARP State *meta");
        list.add("	SD_2_morph_switch_state_3       	" ,"	M_ARP Toggle *meta");
        list.add("	MFO_3_speed                     	" ,"	M_ARP Speed	");
        list.add("	MFO_3_wave                      	" ,"	M_ARP Wave	");
        list.add("	MFO_3_phase                     	" ,"	M_ARP Phase	");
        list.add("	SD_2_is_morph_modulated_3       	" ,"	M_ARP Active *meta");
        list.add("	SD_2_morph_state_2              	" ,"	M_FX State *meta");
        list.add("	SD_2_morph_switch_state_2       	" ,"	M_FX Toggle *meta");
        list.add("	MFO_2_speed                     	" ,"	M_FX Speed	");
        list.add("	MFO_2_wave                      	" ,"	M_FX Wave	");
        list.add("	MFO_2_phase                     	" ,"	M_FX Phase	");
        list.add("	SD_2_is_morph_modulated_2       	" ,"	M_FX Active *meta");
        list.add("	ENV_3_attack                    	" ,"	AMP_ENV Att	");
        list.add("	ENV_3_decay                     	" ,"	AMP_ENV Dec	");
        list.add("	ENV_3_sustain                   	" ,"	AMP_ENV Sus	");
        list.add("	ENV_3_retrigger                 	" ,"	AMP_ENV Retrig	");
        list.add("	ENV_3_release                   	" ,"	AMP_ENV Rel	");
        list.add("	ENV_3_shape                     	" ,"	AMP_ENV Slope	");
        list.add("	EQ_2_velocity_0                 	" ,"	EQ1 Gain	");
        list.add("	EQ_2_hold_0                     	" ,"	EQ1 ENV Active	");
        list.add("	ENV_200_attack                  	" ,"	EQ1 ENV Att	");
        list.add("	ENV_200_decay                   	" ,"	EQ1 ENV Dec	");
        list.add("	ENV_200_sustain                 	" ,"	EQ1 ENV Sus	");
        list.add("	ENV_200_retrigger               	" ,"	EQ1 ENV Retrig	");
        list.add("	ENV_200_release                 	" ,"	EQ1 ENV Rel	");
        list.add("	ENV_200_shape                   	" ,"	EQ1 ENV Slope	");
        list.add("	EQ_2_velocity_1                 	" ,"	EQ2 Gain	");
        list.add("	EQ_2_hold_1                     	" ,"	EQ2 ENV Active	");
        list.add("	ENV_201_attack                  	" ,"	EQ2 ENV Att	");
        list.add("	ENV_201_decay                   	" ,"	EQ2 ENV Dec	");
        list.add("	ENV_201_sustain                 	" ,"	EQ2 ENV Sus	");
        list.add("	ENV_201_retrigger               	" ,"	EQ2 ENV Retrig	");
        list.add("	ENV_201_release                 	" ,"	EQ2 ENV Rel	");
        list.add("	ENV_201_shape                   	" ,"	EQ2 ENV Slope	");
        list.add("	EQ_2_velocity_2                 	" ,"	EQ3 Gain	");
        list.add("	EQ_2_hold_2                     	" ,"	EQ3 ENV Active	");
        list.add("	ENV_202_attack                  	" ,"	EQ3 ENV Att	");
        list.add("	ENV_202_decay                   	" ,"	EQ3 ENV Dec	");
        list.add("	ENV_202_sustain                 	" ,"	EQ3 ENV Sus	");
        list.add("	ENV_202_retrigger               	" ,"	EQ3 ENV Retrig	");
        list.add("	ENV_202_release                 	" ,"	EQ3 ENV Rel	");
        list.add("	ENV_202_shape                   	" ,"	EQ3 ENV Slope	");
        list.add("	EQ_2_velocity_3                 	" ,"	EQ4 Gain	");
        list.add("	EQ_2_hold_3                     	" ,"	EQ4 ENV Active	");
        list.add("	ENV_203_attack                  	" ,"	EQ4 ENV Att	");
        list.add("	ENV_203_decay                   	" ,"	EQ4 ENV Dec	");
        list.add("	ENV_203_sustain                 	" ,"	EQ4 ENV Sus	");
        list.add("	ENV_203_retrigger               	" ,"	EQ4 ENV Retrig	");
        list.add("	ENV_203_release                 	" ,"	EQ4 ENV Rel	");
        list.add("	ENV_203_shape                   	" ,"	EQ4 ENV Slope	");
        list.add("	EQ_2_velocity_4                 	" ,"	EQ5 Gain	");
        list.add("	EQ_2_hold_4                     	" ,"	EQ5 ENV Active	");
        list.add("	ENV_204_attack                  	" ,"	EQ5 ENV Att	");
        list.add("	ENV_204_decay                   	" ,"	EQ5 ENV Dec	");
        list.add("	ENV_204_sustain                 	" ,"	EQ5 ENV Sus	");
        list.add("	ENV_204_retrigger               	" ,"	EQ5 ENV Retrig	");
        list.add("	ENV_204_release                 	" ,"	EQ5 ENV Rel	");
        list.add("	ENV_204_shape                   	" ,"	EQ5 ENV Slope	");
        list.add("	EQ_2_velocity_5                 	" ,"	EQ6 Gain	");
        list.add("	EQ_2_hold_5                     	" ,"	EQ6 ENV Active	");
        list.add("	ENV_205_attack                  	" ,"	EQ6 ENV Att	");
        list.add("	ENV_205_decay                   	" ,"	EQ6 ENV Dec	");
        list.add("	ENV_205_sustain                 	" ,"	EQ6 ENV Sus	");
        list.add("	ENV_205_retrigger               	" ,"	EQ6 ENV Retrig	");
        list.add("	ENV_205_release                 	" ,"	EQ6 ENV Rel	");
        list.add("	ENV_205_shape                   	" ,"	EQ6 ENV Slope	");
        list.add("	EQ_2_velocity_6                 	" ,"	EQ7 Gain	");
        list.add("	EQ_2_hold_6                     	" ,"	EQ7 ENV Active	");
        list.add("	ENV_206_attack                  	" ,"	EQ7 ENV Att	");
        list.add("	ENV_206_decay                   	" ,"	EQ7 ENV Dec	");
        list.add("	ENV_206_sustain                 	" ,"	EQ7 ENV Sus	");
        list.add("	ENV_206_retrigger               	" ,"	EQ7 ENV Retrig	");
        list.add("	ENV_206_release                 	" ,"	EQ7 ENV Rel	");
        list.add("	ENV_206_shape                   	" ,"	EQ7 ENV Slope	");
        list.add("	EQ_2_mix                        	" ,"	EQ All Mix	");
        list.add("	SD_2_shape                      	" ,"	EQ All Reso	");
        list.add("	SD_2_distortion                 	" ,"	FX Dist	");
        list.add("	CHR_2_modulation                	" ,"	FX Chorus	");
        list.add("	CHR_2_pan                       	" ,"	FX ChorusPan	");
        list.add("	SD_2_delay                      	" ,"	FX DelayFBack	");
        list.add("	SD_2_delay_reflexion            	" ,"	FX DelayReflex	");
        list.add("	SD_2_delay_pan                  	" ,"	FX DelayPan	");
        list.add("	SD_2_record                     	" ,"	FX LoopRecord	");
        list.add("	SD_2_record_release             	" ,"	FX LoopRel	");
        list.add("	SD_2_record_size                	" ,"	FX LoopSize	");
        list.add("	VERB_2_room                     	" ,"	FX ReverbRoom	");
        list.add("	VERB_2_width                    	" ,"	FX ReverbWidth	");
        list.add("	VERB_2_dry-wet                  	" ,"	FX ReverbWet	");
        list.add("	VERB_2_pan                      	" ,"	FX ReverbPan	");
        list.add("	SD_2_effect_bypass              	" ,"	FX All Mix	");
        list.add("	SD_2_volume                     	" ,"	MASTER Volume	");
        list.add("	ARP_2_is_on                     	" ,"	ARP Active	");
        list.add("	ARP_2_shuffle                   	" ,"	ARP Shuffle	");
        list.add("	ARP_2_connect                   	" ,"	ARP StepBind	");
        list.add("	ARP_2_step_offset               	" ,"	ARP StepOffset	");
        list.add("	SD_2_glide                      	" ,"	GLOB NoteGlide	");
        list.add("	SD_2_velocity_glide_time        	" ,"	GLOB VeloGlide	");
        list.add("	ARP_2_step_0                    	" ,"	ARP Step1 B1	");
        list.add("	ARP_2_tune_0                    	" ,"	ARP Tune1	");
        list.add("	ARP_2_velocity_0                	" ,"	ARP Velo1	");
        list.add("	ARP_2_step_1                    	" ,"	ARP Step2	");
        list.add("	ARP_2_tune_1                    	" ,"	ARP Tune2	");
        list.add("	ARP_2_velocity_1                	" ,"	ARP Velo2	");
        list.add("	ARP_2_step_2                    	" ,"	ARP Step3	");
        list.add("	ARP_2_tune_2                    	" ,"	ARP Tune3	");
        list.add("	ARP_2_velocity_2                	" ,"	ARP Velo3	");
        list.add("	ARP_2_step_3                    	" ,"	ARP Step4	");
        list.add("	ARP_2_tune_3                    	" ,"	ARP Tune4	");
        list.add("	ARP_2_velocity_3                	" ,"	ARP Velo4	");
        list.add("	ARP_2_step_4                    	" ,"	ARP Step5 B2	");
        list.add("	ARP_2_tune_4                    	" ,"	ARP Tune5	");
        list.add("	ARP_2_velocity_4                	" ,"	ARP Velo5	");
        list.add("	ARP_2_step_5                    	" ,"	ARP Step6	");
        list.add("	ARP_2_tune_5                    	" ,"	ARP Tune6	");
        list.add("	ARP_2_velocity_5                	" ,"	ARP Velo6	");
        list.add("	ARP_2_step_6                    	" ,"	ARP Step7	");
        list.add("	ARP_2_tune_6                    	" ,"	ARP Tune7	");
        list.add("	ARP_2_velocity_6                	" ,"	ARP Velo7	");
        list.add("	ARP_2_step_7                    	" ,"	ARP Step8	");
        list.add("	ARP_2_tune_7                    	" ,"	ARP Tune8	");
        list.add("	ARP_2_velocity_7                	" ,"	ARP Velo8	");
        list.add("	ARP_2_step_8                    	" ,"	ARP Step9 B3	");
        list.add("	ARP_2_tune_8                    	" ,"	ARP Tune9	");
        list.add("	ARP_2_velocity_8                	" ,"	ARP Velo9	");
        list.add("	ARP_2_step_9                    	" ,"	ARP Step10	");
        list.add("	ARP_2_tune_9                    	" ,"	ARP Tune10	");
        list.add("	ARP_2_velocity_9                	" ,"	ARP Velo10	");
        list.add("	ARP_2_step_10                   	" ,"	ARP Step11	");
        list.add("	ARP_2_tune_10                   	" ,"	ARP Tune11	");
        list.add("	ARP_2_velocity_10               	" ,"	ARP Velo11	");
        list.add("	ARP_2_step_11                   	" ,"	ARP Step12	");
        list.add("	ARP_2_tune_11                   	" ,"	ARP Tune12	");
        list.add("	ARP_2_velocity_11               	" ,"	ARP Velo12	");
        list.add("	ARP_2_step_12                   	" ,"	ARP Step13 B4	");
        list.add("	ARP_2_tune_12                   	" ,"	ARP Tune13	");
        list.add("	ARP_2_velocity_12               	" ,"	ARP Velo13	");
        list.add("	ARP_2_step_13                   	" ,"	ARP Step14	");
        list.add("	ARP_2_tune_13                   	" ,"	ARP Tune14	");
        list.add("	ARP_2_velocity_13               	" ,"	ARP Velo14	");
        list.add("	ARP_2_step_14                   	" ,"	ARP Step15	");
        list.add("	ARP_2_tune_14                   	" ,"	ARP Tune15	");
        list.add("	ARP_2_velocity_14               	" ,"	ARP Velo15	");
        list.add("	ARP_2_step_15                   	" ,"	ARP Step16	");
        list.add("	ARP_2_tune_15                   	" ,"	ARP Tune16	");
        list.add("	ARP_2_velocity_15               	" ,"	ARP Velo16	");
        list.add("	ARP_2_fine_offset               	" ,"	ARP FineOffset	");
        list.add("	ARP_2_speed_multi               	" ,"	ARP SpeedMulti	");
        list.add("	SD_2_sync                       	" ,"	GLOB SpeedSync	");
        list.add("	SD_2_speed                      	" ,"	GLOB SpeedBPM	");
        list.add("	SD_2_octave_offset              	" ,"	GLOB OctaveOff	");
        list.add("	SD_2_arp_note_offset            	" ,"	GLOB ProgNote	");
        list.add("	MIDI_0_lfo_popup                	" ,"	RMT LFO POP	");
        list.add("	MIDI_0_lfo_wave                 	" ,"	RMT LFO Wave	");
        list.add("	MIDI_0_lfo_speed                	" ,"	RMT LFO Speed	");
        list.add("	MIDI_0_lfo_offset               	" ,"	RMT LFO Phase	");
        list.add("	MIDI_0_env_popup                	" ,"	RMT ENV POP	");
        list.add("	MIDI_0_env_attack               	" ,"	RMT ENV Att	");
        list.add("	MIDI_0_env_decay                	" ,"	RMT ENV Dec	");
        list.add("	MIDI_0_env_sustain              	" ,"	RMT ENV Sus	");
        list.add("	MIDI_0_env_sustain_time         	" ,"	RMT ENV Retrig	");
        list.add("	MIDI_0_env_release              	" ,"	RMT ENV Rel	");
        list.add("	MIDI_0_env_shape                	" ,"	RMT ENV Slope	");
        list.add("	SD_2_smooth_motor_time          	" ,"	CFG SmoothTime	");
        list.add("	SD_2_morph_motor_time           	" ,"	CFG MorphTime	");
        list.add("	SD_2_midi_pickup_offset         	" ,"	CFG MIDIPickUp	");

        // REORDER THE PARAMS
        {
            Array< Parameter* >& automateable_parameters = synth_data->get_atomateable_parameters();
            {
                Array< Parameter* > reordered_automateable_parameters;
                for( int j = 0 ; j < list.pairs.size() ; ++j )
                {
                    StringPair& pair = list.pairs.getReference(j);
                    const String& name = pair.name;
                    for( int i = 0 ; i != automateable_parameters.size() ; ++i )
                    {
                        Parameter*param = automateable_parameters.getUnchecked(i);
                        if( name == param->get_info().name )
                        {
                            reordered_automateable_parameters.add( automateable_parameters.removeAndReturn(i) );
                            break;
                        }
                    }
                }
               // jassert( ! automateable_parameters.size() );
                automateable_parameters.clearQuick();
                automateable_parameters.addArray( reordered_automateable_parameters );
            }

            // REPLACE NAMES IN THE LIST
            for( int i = 0 ; i != automateable_parameters.size() ; ++i )
            {
                Parameter*param = automateable_parameters.getUnchecked(i);
                const String& name = param->get_info().name;
                for( int j = 0 ; j < list.pairs.size() ; ++j )
                {
                    StringPair& pair = list.pairs.getReference(j);
                    if( name == pair.name )
                    {
                        const_cast< String& >( param->get_info().short_name ) = pair.short_name;
                        list.pairs.remove(j);
                        break;
                    }
                }
            }


            /* Parameter Printout
            for( int i = 0 ; i != synth_data->get_atomateable_parameters().size() ; ++i )
            {
                String name = synth_data->get_atomateable_parameters().getReference(i)->get_info().name;
                String short_name = synth_data->get_atomateable_parameters().getReference(i)->get_info().short_name;

                while( name.length() <= 30 )
                {
                    name.append(" ", 1);
                }

                std::cout
                // << "id:" << i << " "
                // << name
                // << " ,"
                        << short_name
                        << std::endl;
            }
            */
        }
    }

    if (is_plugin())
    {
        init_automatable_parameters();
    }

#ifdef JUCE_IOS
	/*
    if( audio_is_successful_initalized = (initialiseWithDefaultDevices(0,2) == "" ? true : false) )
    {
        AudioDeviceManager::AudioDeviceSetup setup;
        getAudioDeviceSetup(setup);
        //std::cout << setup.bufferSize << " " << setup.sampleRate << std::endl;
        setup.bufferSize = 1024;
        setup.sampleRate = 44100/2;
        setPlayConfigDetails ( 0,2, setup.sampleRate, setup.bufferSize );
        addAudioCallback (&player);
        player.setProcessor (this);

        prepareToPlay( setup.sampleRate, setup.bufferSize );
    }
	*/
#else
    #if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING
        if( audio_is_successful_initalized = (mono_AudioDeviceManager::read() == "") )
        {
            AudioDeviceManager::AudioDeviceSetup setup;
            getAudioDeviceSetup(setup);
            setPlayConfigDetails ( 0, 2, setup.sampleRate, setup.bufferSize );
            addAudioCallback (&player);
            player.setProcessor (this);

            prepareToPlay( setup.sampleRate, setup.bufferSize );
        }
    #endif
#endif

    DBG("init done");
}

COLD MoniqueAudioProcessor::~MoniqueAudioProcessor() noexcept
{
    clear_feedback_and_shutdown();

    if (is_standalone())
    {
        standalone_features_pimpl->clock_smoother.reset();
        standalone_features_pimpl->runtime_info = nullptr;
    }
    else
    {
        for( int i = 0 ; i != automateable_parameters.size() ; ++i )
        {
            Parameter* param( automateable_parameters.getUnchecked(i) );
            if( param )
            {
                param->remove_listener( this );
            }
        }
    }

    synth_data->save_midi();
    synth_data->save_settings();

    SHARED::getInstance()->num_instances--;
    if( SHARED::getInstance()->num_instances == 0 )
    {
        //SHARED::getInstance()->status.save();

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
void MoniqueAudioProcessor::reset_pending_notes()
{
    synth->reset_note_down_store();
}
void MoniqueAudioProcessor::process ( AudioSampleBuffer& buffer_, MidiBuffer& midi_messages_, bool bypassed_ )
{
    if (is_standalone())
    {
        if (not standalone_features_pimpl->block_lock.tryEnter())
        {
            return;
        }
    }

    if( buffer_.getNumChannels() < 1 )
    {
        return;
    }

    if( sample_rate != getSampleRate() || getBlockSize() != block_size )
    {
        prepareToPlay(getSampleRate(),getBlockSize());
    }

    if (synth_data->tuning)
    {
       synth_data->tuning->updateMTSESPStatus();
    }

    const int num_samples = buffer_.getNumSamples();
    buffer_.clear();

    const int64 last_samples_since_start = current_pos_info.timeInSamples;
    const bool was_playing = current_pos_info.isPlaying;

    bool seems_to_record = false;

    if (is_standalone())
    {
        static auto did_render_once = false;
        if (not did_render_once)
        {
            current_pos_info.resetToDefault();
            did_render_once = true;
        }
        current_pos_info.timeSigNumerator = 4;
        current_pos_info.timeSigDenominator = 4;
        current_pos_info.isPlaying = true;
        current_pos_info.isRecording = false;
    }

    if (is_standalone() || getPlayHead())
    {
        if (is_standalone() || getPlayHead()->getCurrentPosition(current_pos_info))
        {
#ifdef JUCE_IOS
			bool iosViaMIDIClock = true;
			// FIX FOR STANDALONE AND PLUGIN TRANSPORT
			if (getPlayHead() != nullptr)
			{
				if (getPlayHead()->getCurrentPosition(current_pos_info))
				{
					iosViaMIDIClock = false;
				}
			}
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
                if (is_standalone())
                {
                    auto& info_standalone_features = *info->standalone_features_pimpl;

                    // +++++ SYNC BLOCK EXTERN MIDI
                    {
                        // CLEAN LAST BLOCK
                        // FOR SECURITy REMOVE INVALID OLD STEPS
                        OwnedArray<Step> &steps_in_block(info_standalone_features.steps_in_block);
                        if (steps_in_block.size())
                        {
                            while (steps_in_block.getFirst()->at_absolute_sample < current_pos_info.timeInSamples)
                            {
                                steps_in_block.remove(0, true);

                                if (not steps_in_block.size())
                                {
                                    break;
                                }
                            }
                        }
                        info_standalone_features.clock_sync_information.clear();

                        // GET THE MESSAGES
                        MidiBuffer sync_messages;
                        //get_sync_input_messages( sync_messages, num_samples );

                        // RUN THE LOOP AND PROCESS TJHE MESSAGES
                        const bool is_synced(synth_data->sync);
                        if (is_synced)// TODOO
                        {
                            MidiBuffer::Iterator message_iter(sync_messages);
                            MidiMessage input_midi_message;
                            int sample_position = 0;

                            const double speed_multiplyer = ArpSequencerData::speed_multi_to_value(synth_data->arp_sequencer_data->speed_multi);
                            static constexpr int clocks_per_step = 6;
                            static constexpr int clocks_per_beat = 24;
                            static constexpr int clocks_per_bar = 96;

                            while (message_iter.getNextEvent(input_midi_message, sample_position))
                            {
                                const int64 abs_event_time_in_samples = current_pos_info.timeInSamples + sample_position;
#ifdef JUCE_IOS
                                if (iosViaMIDIClock)
#endif
                                // CLOCK
                                if (input_midi_message.isMidiClock())
                                {
                                    standalone_features_pimpl->stopTimer();
                                    standalone_features_pimpl->received_a_clock_in_time = true;
                                    info_standalone_features.is_extern_synced = true;
                                    info_standalone_features.clock_sync_information.add_clock(sample_position, abs_event_time_in_samples -  standalone_features_pimpl->last_clock_sample);

                                    const int clock_in_bar = info_standalone_features.clock_counter.clock();
                                    const int clock_absolute = info_standalone_features.clock_counter.clock_absolut();
                                    const int is_step = info_standalone_features.clock_counter.is_step();

                                    if (is_step)
                                    {
                                        const int64 current_samples_per_step = abs_event_time_in_samples - standalone_features_pimpl-> last_step_sample;
                                        standalone_features_pimpl->last_step_sample = abs_event_time_in_samples;

                                        // UPDATE SPEED INFO (should be used for ui)
                                        {
                                            int samples_per_beat = current_samples_per_step * 4;
                                            double ms_per_beat = samplesToMs(samples_per_beat, sample_rate);
                                            if (ms_per_beat < 1)
                                            {
                                                ms_per_beat = 1;
                                            }
                                            current_pos_info.bpm =  standalone_features_pimpl->clock_smoother->add_and_get_average((60.0f * 1000) / ms_per_beat);
                                        }
                                    }

                                    // X 1
                                    bool success = false;
                                    if (speed_multiplyer == 1)
                                    {
                                        if (clock_absolute > 0)
                                        {
                                            if (clock_absolute % clocks_per_step == 0)
                                            {
                                                info_standalone_features.steps_in_block.add(new Step(clock_absolute / clocks_per_step, abs_event_time_in_samples + 1, abs_event_time_in_samples -  standalone_features_pimpl->last_clock_sample));
                                                success = true;
                                            }
                                        }
                                        else
                                        {
                                            info_standalone_features.steps_in_block.add(new Step(0, abs_event_time_in_samples + 1, 0));
                                            success = true;
                                        }
                                    }
                                        //  X > 1 // WIRD LANGSAMER
                                    else if (speed_multiplyer > 1)
                                    {
                                        int sum_generate_sub_clocks_per_clock = speed_multiplyer;
                                        const double current_samples_per_clock = double(abs_event_time_in_samples -  standalone_features_pimpl->last_clock_sample) / speed_multiplyer;
                                        int clock_id = clock_in_bar * speed_multiplyer;
                                        for (int i = 0; i != speed_multiplyer; ++i)
                                        {
                                            if (clock_id > 0)
                                            {
                                                int tmp_clock_id = ((clock_id + i) % 96);
                                                if (tmp_clock_id % clocks_per_step == 0)
                                                {
                                                    info_standalone_features.steps_in_block.add(new Step(tmp_clock_id / clocks_per_step, abs_event_time_in_samples + current_samples_per_clock * i + 1, current_samples_per_clock));

                                                    success = true;
                                                }
                                            }
                                            else
                                            {
                                                info_standalone_features.steps_in_block.add(new Step(0, abs_event_time_in_samples + 1, 0));
                                                success = true;
                                            }
                                        }
                                    }
                                        // X < 1 // WIRD SCHNELLER
                                    else
                                    {
                                        if (clock_absolute > 0)
                                        {
                                            const double speed_multiplyer__ = 1.0 / speed_multiplyer;

                                            const double factor = speed_multiplyer__ * clocks_per_step;
                                            const double faster_clocks_per_bar = speed_multiplyer__ * clocks_per_bar;
                                            const double faster_clocks_semi_absolut = fmod(clock_absolute, faster_clocks_per_bar);
                                            if (fmod(faster_clocks_semi_absolut, factor) == 0)
                                            {
                                                info_standalone_features.steps_in_block.add(new Step(faster_clocks_semi_absolut / factor, abs_event_time_in_samples + 1, (abs_event_time_in_samples -  standalone_features_pimpl->last_clock_sample) * speed_multiplyer__));

                                                success = true;
                                            }
                                        }
                                        else
                                        {
                                            info_standalone_features.steps_in_block.add(new Step(0, abs_event_time_in_samples + 1, 0));
                                        }
                                    }

                                    standalone_features_pimpl->last_clock_sample = abs_event_time_in_samples;

                                    info_standalone_features.clock_counter++;
                                    standalone_features_pimpl->startTimer(100);
                                }
                                    // SYNC START
                                else if (input_midi_message.isMidiStart())
                                {
                                    info_standalone_features.clock_counter.reset();
                                    info_standalone_features.steps_in_block.clearQuick(true);
                                    info_standalone_features.is_running = true;
                                    info_standalone_features.is_extern_synced = true;

                                    standalone_features_pimpl->last_clock_sample =  standalone_features_pimpl->last_clock_sample - abs_event_time_in_samples;
                                    standalone_features_pimpl->last_step_sample =  standalone_features_pimpl->last_step_sample - abs_event_time_in_samples;
                                    current_pos_info.timeInSamples =  standalone_features_pimpl->last_clock_sample;

                                    DBG("START");
                                }
                                    // SYNC STOP
                                else if (input_midi_message.isMidiStop())
                                {
                                    MidiMessage notes_off = MidiMessage::allNotesOff(1);
                                    info_standalone_features.is_running = false;

                                    DBG("STOP");
                                }
                                    // SYNC CONTINUE
                                else if (input_midi_message.isMidiContinue())
                                {
                                    info_standalone_features.is_running = true;
                                    info_standalone_features.is_extern_synced = true;

                                    DBG("START");
                                }
                            }
                        }
                    }
                    // +++++ SYNC BLOCK EXTERN MIDI
                }

                {
                    // RENDER SYNTH

                    if (is_standalone())
                    {
                        //get_cc_input_messages( midi_messages_, num_samples );// TODOO
                        // get_note_input_messages( midi_messages_, num_samples );// TODOO
                        info->standalone_features_pimpl->clock_sync_information.create_a_working_copy();
                    }

                    MidiKeyboardState::processNextMidiBuffer( midi_messages_, 0, num_samples, true );

                    const bool is_playing = current_pos_info.isPlaying;
                    if( was_playing and not is_playing )
                    {
                        if( Monique_Ui_AmpPainter* amp_painter = synth_data->audio_processor->amp_painter )
                        {
                            amp_painter->clear_and_keep_minimum();
                        }
                    }
                    else if( not was_playing and is_playing )
                    {
                        //voice->restart_arp(0);
                    }

                    SHARED::getInstance()->temp_buffer.setSize( 2, getBlockSize() );
                    // NOTE: CP get_working_buffer
                    synth->render_next_block( buffer_, midi_messages_, 0, num_samples );

                    midi_messages_.clear(); // WILL BE FILLED AT THE END
                }

                if (is_standalone())
                {
                    { static bool fix_oss_port_issue = false; jassert(fix_oss_port_issue); fix_oss_port_issue = true; }
                    // FIXME - this is not the right api
                    //send_feedback_messages(midi_messages_, num_samples);
                    //send_thru_messages(num_samples);
                }
                else
                {
                    send_feedback_messages(midi_messages_, num_samples);
                }
            }
        }
    }

    if (is_standalone())
    {
        current_pos_info.timeInSamples += buffer_.getNumSamples();
        standalone_features_pimpl->block_lock.exit();
    }
    else
    {
        if (current_pos_info.isLooping)
        {
            if (current_pos_info.isPlaying)
            {
                current_pos_info.timeInSamples += buffer_.getNumSamples();
            }
            else
            {
                current_pos_info.timeInSamples = 0;
            }
        }
    }
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

bool MoniqueAudioProcessor::isParameterAutomatable ( int i_ ) const
{
    Parameter*param = automateable_parameters.getUnchecked(i_);
    if( ! param )
    {
        return true;
    }

    if( param->get_info().short_name.contains("CFG") or param->get_info().short_name.contains("RMT") )
    {
        return false;
    }

    return true;
}

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
    Parameter*param = automateable_parameters.getUnchecked(i_);
    if( param )
    {
        param->add_ignore_feedback( this );
        {
            set_percent_value( param, percent_ );
        }
        param->remove_ignore_feedback( this );
    }
    else
    {
        param = automateable_parameters.getUnchecked(i_-1);
        param->add_ignore_feedback( this );
        {
            param->set_modulation_amount( (percent_*2)-1 );
        }
        param->remove_ignore_feedback( this );
    }
}

bool MoniqueAudioProcessor::isMetaParameter (int i_) const
{
    Parameter*param = automateable_parameters.getUnchecked(i_);
    if( ! param )
    {
        param = automateable_parameters.getUnchecked(i_-1);
    }

    // if( TYPES_DEF::IS_BOOL != type_of( param ) )
    {
        if( param->get_info().short_name.endsWith("a") )
        {
            if( param->get_info().short_name.contains("*meta") )
            {
                return true;
            }
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

//==============================================================================
//==============================================================================
//==============================================================================
bool MoniqueAudioProcessor::hasEditor() const
{
    return true;
}

const String MoniqueAudioProcessor::getName() const
{
    return JucePlugin_Name;
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
void MoniqueAudioProcessor::getStateInformation ( MemoryBlock& destData )
{
    //xml.getIntAttribute( "BANK", synth_data->current_bank );
    //xml.getIntAttribute( "PROG", synth_data->current_program );
    XmlElement xml("PROJECT-1.0");
    String modded_name = synth_data->alternative_program_name;
    {
        String name = modded_name.fromFirstOccurrenceOf ("0RIGINAL WAS: ",false, false);
        xml.setAttribute( "MODDED_PROGRAM", name == "" ? modded_name : name );
        synth_data->save_to( &xml );
        copyXmlToBinary ( xml, destData );
    }
}

void MoniqueAudioProcessor::setStateInformation ( const void* data, int sizeInBytes )
{
    auto xml = getXmlFromBinary ( data, sizeInBytes );
    if ( xml )
    {
        if ( xml->hasTagName ( "PROJECT-1.0" ) || xml->hasTagName("MONOLisa")  )
        {
            synth_data->read_from( xml.get() );
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
    if(is_plugin())
    {
        if ((Time::getMillisecondCounter() - restore_time) < synth_data->program_restore_block_time)
        {
            return;
        }
    }

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