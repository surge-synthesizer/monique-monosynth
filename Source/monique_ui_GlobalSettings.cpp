/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "UiLookAndFeel.h"
#include "SynthData.h"
#include "PluginProcessor.h"
#include "UiEditorSynthLite.h"
//[/Headers]

#include "monique_ui_GlobalSettings.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void UiEditorGlobalSettings::refresh() noexcept
{
    slider_morph_motor_time->setValue( DATA( synth_data ).morph_motor_time, dontSendNotification );
    slider_glide_time->setValue( DATA( synth_data ).glide_motor_time, dontSendNotification );

    toggle_animate_input_env->setToggleState( DATA( synth_data ).animate_input_env ,dontSendNotification );
    toggle_animate_eq_env->setToggleState( DATA( synth_data ).animate_eq_env ,dontSendNotification );
    toggle_output_cc_mute4->setToggleState( DATA( synth_data ).animate_modulations ,dontSendNotification );

    label_cpu_usage->setText
    (
        String
        (
            cpu_usage_smoother.add_and_get_average( AppInstanceStore::getInstance()->audio_processor->getCpuUsage()*100)
        )
        + String("%")
        ,dontSendNotification
    );

    // COLOURS
    if( button_current_editor_color->isVisible() )
    {
        button_current_editor_color->setColour( TextButton::buttonColourId, colour_selector->getCurrentColour() );

        if( current_edited_colour )
            *current_edited_colour = colour_selector->getCurrentColour();

        if( last_repainted_colour != *current_edited_colour )
        {
            last_repainted_colour = *current_edited_colour;
            AppInstanceStore::getInstance()->editor->repaint();
        }
    }
    else
    {
        button_colour_bg->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.bg);
        button_colour_border->setColour (TextButton::buttonColourId, Colour(0xffff3b00) ); //  // UiLookAndFeel::getInstance()->colours.bg
        button_colour_buttons_on->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour);
        button_colour_buttons_off->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour);
        button_colour_slider_1->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour);
        button_colour_slider_2->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour_2);
        button_colour_slider_mod->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour_modulation);
        button_colour_signal_lines->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour.darker());
        button_colour_labels->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.label_text_colour);
    }

    // THREADS
    combo_multicore_cpus->setSelectedId( DATA( synth_data ).num_extra_threads+1, dontSendNotification );
}

void UiEditorGlobalSettings::open_colour_selector(Colour& colour_to_edit_)
{
    last_repainted_colour = colour_to_edit_;

    colour_selector->setCurrentColour( colour_to_edit_ );
    current_edited_colour = &colour_to_edit_;

    colour_selector->setVisible(true);
    button_current_editor_color->setVisible(true);

    label_colour_1->setVisible(false);
    label_colour_2->setVisible(false);
    label_colour_3->setVisible(false);
    label_colour_4->setVisible(false);
    label_colour_5->setVisible(false);
    label_colour_6->setVisible(false);
    label_colour_7->setVisible(false);
    label_colour_8->setVisible(false);
    label_colour_9->setVisible(false);
    button_colour_bg->setVisible(false);
    button_colour_border->setVisible(false);
    button_colour_buttons_on->setVisible(false);
    button_colour_buttons_off->setVisible(false);
    button_colour_slider_1->setVisible(false);
    button_colour_slider_2->setVisible(false);
    button_colour_slider_mod->setVisible(false);
    button_colour_signal_lines->setVisible(false);
    button_colour_labels->setVisible(false);
}

void UiEditorGlobalSettings::update_audio_devices()
{
    combo_audio_device->clear(dontSendNotification);
    combo_sample_rate->clear(dontSendNotification);
    combo_block_size->clear(dontSendNotification);
    combo_audio_driver->clear(dontSendNotification);

    // AUDIO DEVICE MANAGER
    MoniqueAudioProcessor* audio_processor = AppInstanceStore::getInstance()->audio_processor;

    // DRIVERS
    const OwnedArray<AudioIODeviceType>& drivers = audio_processor->getAvailableDeviceTypes();
    const String current_audio_driver = audio_processor->getCurrentAudioDeviceType();
    AudioIODeviceType* active_driver = nullptr;
    for( int i = 0 ; i != drivers.size() ; ++i )
    {
        String driver_name = drivers[i]->getTypeName();
        combo_audio_driver->addItem( driver_name , i+1 );
        if( driver_name == current_audio_driver )
        {
            combo_audio_driver->setSelectedId(i+1,dontSendNotification);
            active_driver = drivers[i];
        }
    }

    // DEVICES (per DRIVER)
    AudioIODevice* active_device = audio_processor->getCurrentAudioDevice();
    if( active_driver || active_device )
    {
        combo_audio_device->setEnabled(true);
        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );
        const String current_audio_device = current_device_setup.outputDeviceName;
        if( active_driver )
        {
            active_driver->scanForDevices();
            StringArray device_names = active_driver->getDeviceNames();
            for( int i = 0 ; i != device_names.size() ; ++i )
            {
                String device_name = device_names[i];
                combo_audio_device->addItem( device_name, i+1 );
                if( current_audio_device == device_name )
                {
                    combo_audio_device->setSelectedId(i+1,dontSendNotification);
                }
            }
        }

        // SAMPLE RATE
        if( active_device )
        {
            combo_sample_rate->setEnabled(true);
            Array<double> sample_rates = active_device->getAvailableSampleRates();
            double current_sample_rate = current_device_setup.sampleRate;
            for( int i = 0 ; i != sample_rates.size() ; ++i )
            {
                double sample_rate = sample_rates[i];
                combo_sample_rate->addItem( String(sample_rate), i +1 );
                if( current_sample_rate == sample_rate )
                {
                    combo_sample_rate->setSelectedId(i+1,dontSendNotification);
                }
            }

            // BLOCK SIZE
            combo_block_size->setEnabled(true);
            Array<int> buffer_sizes = active_device->getAvailableBufferSizes();
            double current_buffer_size = current_device_setup.bufferSize;
            for( int i = 0 ; i != buffer_sizes.size() ; ++i )
            {
                double buffer_Size = buffer_sizes[i];
                combo_block_size->addItem( String(buffer_Size), i +1 );
                if( current_buffer_size == buffer_Size )
                {
                    combo_block_size->setSelectedId(i+1,dontSendNotification);
                }
            }
        }
        else
        {
            combo_sample_rate->addItem( "NO DEVICE", 1 );
            combo_sample_rate->setSelectedId(1,dontSendNotification);
            combo_sample_rate->setEnabled(false);

            combo_block_size->addItem( "NO DEVICE", 1 );
            combo_block_size->setSelectedId(1,dontSendNotification);
            combo_block_size->setEnabled(false);
        }
    }
    else
    {
        combo_audio_device->addItem( "NO DRIVER", 1 );
        combo_audio_device->setSelectedId(1,dontSendNotification);
        combo_audio_device->setEnabled(false);

        combo_sample_rate->addItem( "NO DEVICE", 1 );
        combo_sample_rate->setSelectedId(1,dontSendNotification);
        combo_sample_rate->setEnabled(false);

        combo_block_size->addItem( "NO DEVICE", 1 );
        combo_block_size->setSelectedId(1,dontSendNotification);
        combo_block_size->setEnabled(false);
    }
}
//[/MiscUserDefs]

//==============================================================================
UiEditorGlobalSettings::UiEditorGlobalSettings ()
    : original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label_colour_9 = new Label (String::empty,
                                                   TRANS("MODULATION SLIDER (BACK)")));
    label_colour_9->setFont (Font (30.00f, Font::plain));
    label_colour_9->setJustificationType (Justification::centredLeft);
    label_colour_9->setEditable (false, false, false);
    label_colour_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_current_editor_color = new TextButton (String::empty));
    button_current_editor_color->setButtonText (TRANS("OK"));
    button_current_editor_color->addListener (this);

    addAndMakeVisible (colour_selector = new ColourSelector (ColourSelector::showColourspace
                                                             ,0,7));

    addAndMakeVisible (label_colour_6 = new Label (String::empty,
                                                   TRANS("LABELS")));
    label_colour_6->setFont (Font (30.00f, Font::plain));
    label_colour_6->setJustificationType (Justification::centredLeft);
    label_colour_6->setEditable (false, false, false);
    label_colour_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_4 = new Label (String::empty,
                                                   TRANS("BUTTON ON")));
    label_colour_4->setFont (Font (30.00f, Font::plain));
    label_colour_4->setJustificationType (Justification::centredLeft);
    label_colour_4->setEditable (false, false, false);
    label_colour_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_5 = new Label (String::empty,
                                                   TRANS("BUTTON OFF")));
    label_colour_5->setFont (Font (30.00f, Font::plain));
    label_colour_5->setJustificationType (Justification::centredLeft);
    label_colour_5->setEditable (false, false, false);
    label_colour_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_morph_motor_time = new Slider ("0"));
    slider_morph_motor_time->setRange (1, 20000, 1);
    slider_morph_motor_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_morph_motor_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_morph_motor_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_morph_motor_time->addListener (this);

    addAndMakeVisible (slider_glide_time = new Slider ("0"));
    slider_glide_time->setRange (1, 20000, 1);
    slider_glide_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_glide_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_glide_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_glide_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_glide_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_glide_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_glide_time->addListener (this);

    addAndMakeVisible (label13 = new Label ("new label",
                                            TRANS("MORPH MOTOR")));
    label13->setFont (Font (15.00f, Font::plain));
    label13->setJustificationType (Justification::centred);
    label13->setEditable (false, false, false);
    label13->setColour (Label::textColourId, Colours::yellow);
    label13->setColour (TextEditor::textColourId, Colours::black);
    label13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label7 = new Label ("new label",
                                           TRANS("GLIDE")));
    label7->setFont (Font (15.00f, Font::plain));
    label7->setJustificationType (Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (Label::textColourId, Colours::yellow);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_16 = new Label (String::empty,
                                             TRANS("INPUT ENV\'S")));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_input_env = new ToggleButton (String::empty));
    toggle_animate_input_env->addListener (this);

    addAndMakeVisible (label_18 = new Label (String::empty,
                                             TRANS("EQ ENV")));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredLeft);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_eq_env = new ToggleButton (String::empty));
    toggle_animate_eq_env->addListener (this);

    addAndMakeVisible (label_19 = new Label (String::empty,
                                             TRANS("MODULATIONS")));
    label_19->setFont (Font (30.00f, Font::plain));
    label_19->setJustificationType (Justification::centredLeft);
    label_19->setEditable (false, false, false);
    label_19->setColour (Label::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_output_cc_mute4 = new ToggleButton (String::empty));
    toggle_output_cc_mute4->addListener (this);

    addAndMakeVisible (label_ui_headline_1 = new Label ("DL",
                                                        TRANS("GLOBAL TIMES")));
    label_ui_headline_1->setFont (Font (30.00f, Font::plain));
    label_ui_headline_1->setJustificationType (Justification::centred);
    label_ui_headline_1->setEditable (false, false, false);
    label_ui_headline_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_2 = new Label ("DL",
                                                        TRANS("ANIMATIONS")));
    label_ui_headline_2->setFont (Font (30.00f, Font::plain));
    label_ui_headline_2->setJustificationType (Justification::centred);
    label_ui_headline_2->setEditable (false, false, false);
    label_ui_headline_2->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String::empty,
                                            TRANS("CPU:")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredRight);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_cpu_usage = new Label (String::empty,
                                                    TRANS("20%")));
    label_cpu_usage->setFont (Font (30.00f, Font::plain));
    label_cpu_usage->setJustificationType (Justification::centredLeft);
    label_cpu_usage->setEditable (false, false, false);
    label_cpu_usage->setColour (Label::textColourId, Colour (0xffff3b00));
    label_cpu_usage->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_cpu_usage->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String::empty,
                                            TRANS("THREADS:")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredRight);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_multicore_cpus = new ComboBox (String::empty));
    combo_multicore_cpus->setEditableText (false);
    combo_multicore_cpus->setJustificationType (Justification::centredLeft);
    combo_multicore_cpus->setTextWhenNothingSelected (String::empty);
    combo_multicore_cpus->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_multicore_cpus->addListener (this);

    addAndMakeVisible (label_ui_headline_4 = new Label ("DL",
                                                        TRANS("SLIDERS")));
    label_ui_headline_4->setFont (Font (30.00f, Font::plain));
    label_ui_headline_4->setJustificationType (Justification::centred);
    label_ui_headline_4->setEditable (false, false, false);
    label_ui_headline_4->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_morph_motor_time2 = new Slider ("0"));
    slider_morph_motor_time2->setRange (1, 1000, 1);
    slider_morph_motor_time2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_morph_motor_time2->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_morph_motor_time2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_morph_motor_time2->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_morph_motor_time2->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_morph_motor_time2->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("SENSITIVITY")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::yellow);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_3 = new Label (String::empty,
                                            TRANS("LINEAR DRAG")));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("ROTARY DRAG")));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_6 = new Label (String::empty,
                                            TRANS("WHEEL DRAG")));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_input_env2 = new ToggleButton (String::empty));
    toggle_animate_input_env2->addListener (this);

    addAndMakeVisible (toggle_animate_input_env3 = new ToggleButton (String::empty));
    toggle_animate_input_env3->addListener (this);

    addAndMakeVisible (toggle_animate_input_env4 = new ToggleButton (String::empty));
    toggle_animate_input_env4->addListener (this);

    addAndMakeVisible (label_ui_headline_5 = new Label ("DL",
                                                        TRANS("COLOURS")));
    label_ui_headline_5->setFont (Font (30.00f, Font::plain));
    label_ui_headline_5->setJustificationType (Justification::centred);
    label_ui_headline_5->setEditable (false, false, false);
    label_ui_headline_5->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_1 = new Label (String::empty,
                                                   TRANS("BACKGROUND")));
    label_colour_1->setFont (Font (30.00f, Font::plain));
    label_colour_1->setJustificationType (Justification::centredLeft);
    label_colour_1->setEditable (false, false, false);
    label_colour_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_7 = new Label (String::empty,
                                                   TRANS("VALUE SLIDER (FRONT)")));
    label_colour_7->setFont (Font (30.00f, Font::plain));
    label_colour_7->setJustificationType (Justification::centredLeft);
    label_colour_7->setEditable (false, false, false);
    label_colour_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_8 = new Label (String::empty,
                                                   TRANS("VALUE SLIDER 2 (BACK)")));
    label_colour_8->setFont (Font (30.00f, Font::plain));
    label_colour_8->setJustificationType (Justification::centredLeft);
    label_colour_8->setEditable (false, false, false);
    label_colour_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_2 = new Label (String::empty,
                                                   TRANS("BORDERS")));
    label_colour_2->setFont (Font (30.00f, Font::plain));
    label_colour_2->setJustificationType (Justification::centredLeft);
    label_colour_2->setEditable (false, false, false);
    label_colour_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_colour_bg = new TextButton ("new button"));
    button_colour_bg->setButtonText (String::empty);
    button_colour_bg->addListener (this);

    addAndMakeVisible (button_colour_border = new TextButton ("new button"));
    button_colour_border->setButtonText (String::empty);
    button_colour_border->addListener (this);

    addAndMakeVisible (button_colour_buttons_on = new TextButton ("new button"));
    button_colour_buttons_on->setButtonText (String::empty);
    button_colour_buttons_on->addListener (this);

    addAndMakeVisible (button_colour_slider_1 = new TextButton ("new button"));
    button_colour_slider_1->setButtonText (String::empty);
    button_colour_slider_1->addListener (this);

    addAndMakeVisible (button_colour_slider_2 = new TextButton ("new button"));
    button_colour_slider_2->setButtonText (String::empty);
    button_colour_slider_2->addListener (this);

    addAndMakeVisible (button_colour_slider_mod = new TextButton ("new button"));
    button_colour_slider_mod->setButtonText (String::empty);
    button_colour_slider_mod->addListener (this);

    addAndMakeVisible (label_colour_3 = new Label (String::empty,
                                                   TRANS("SIGNAL LINES")));
    label_colour_3->setFont (Font (30.00f, Font::plain));
    label_colour_3->setJustificationType (Justification::centredLeft);
    label_colour_3->setEditable (false, false, false);
    label_colour_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_colour_signal_lines = new TextButton ("new button"));
    button_colour_signal_lines->setButtonText (String::empty);
    button_colour_signal_lines->addListener (this);

    addAndMakeVisible (button_colour_buttons_off = new TextButton ("new button"));
    button_colour_buttons_off->setButtonText (String::empty);
    button_colour_buttons_off->addListener (this);

    addAndMakeVisible (button_colour_labels = new TextButton ("new button"));
    button_colour_labels->setButtonText (String::empty);
    button_colour_labels->addListener (this);

    addAndMakeVisible (label_ui_headline_6 = new Label ("DL",
                                                        TRANS("AUDIO & CPU")));
    label_ui_headline_6->setFont (Font (30.00f, Font::plain));
    label_ui_headline_6->setJustificationType (Justification::centred);
    label_ui_headline_6->setEditable (false, false, false);
    label_ui_headline_6->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_7 = new Label (String::empty,
                                            TRANS("DEVICE:")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredRight);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_audio_device = new ComboBox (String::empty));
    combo_audio_device->setEditableText (false);
    combo_audio_device->setJustificationType (Justification::centredLeft);
    combo_audio_device->setTextWhenNothingSelected (String::empty);
    combo_audio_device->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_audio_device->addListener (this);

    addAndMakeVisible (label_8 = new Label (String::empty,
                                            TRANS("RATE:")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredRight);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String::empty,
                                            TRANS("BLOCK:")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredRight);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_sample_rate = new ComboBox (String::empty));
    combo_sample_rate->setEditableText (false);
    combo_sample_rate->setJustificationType (Justification::centredLeft);
    combo_sample_rate->setTextWhenNothingSelected (String::empty);
    combo_sample_rate->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_sample_rate->addListener (this);

    addAndMakeVisible (combo_block_size = new ComboBox (String::empty));
    combo_block_size->setEditableText (false);
    combo_block_size->setJustificationType (Justification::centredLeft);
    combo_block_size->setTextWhenNothingSelected (String::empty);
    combo_block_size->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_block_size->addListener (this);

    addAndMakeVisible (label_10 = new Label (String::empty,
                                             TRANS("DRIVER:")));
    label_10->setFont (Font (30.00f, Font::plain));
    label_10->setJustificationType (Justification::centredRight);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_audio_driver = new ComboBox (String::empty));
    combo_audio_driver->setEditableText (false);
    combo_audio_driver->setJustificationType (Justification::centredLeft);
    combo_audio_driver->setTextWhenNothingSelected (String::empty);
    combo_audio_driver->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_audio_driver->addListener (this);


    //[UserPreSize]
    setOpaque(true);
    colour_selector->setVisible(false);
    button_current_editor_color->setVisible(false);

    current_edited_colour = nullptr;

    for( int i = 0 ; i != getNumChildComponents() ; ++i ) {
        getChildComponent(i)->setOpaque(true);

        if( Slider* slider = dynamic_cast<Slider*>(getChildComponent(i)) ) {
            SET_SLIDER_STYLE( slider, VALUE_SLIDER );
        }
    }

    // CPU
    for( int i = 0 ; i != THREAD_LIMIT ; ++i )
        combo_multicore_cpus->addItem(String(i+1),i+1);

    // AUDIO DEVICE
    update_audio_devices();

    /*
    //[/UserPreSize]

    setSize (1465, 180);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

UiEditorGlobalSettings::~UiEditorGlobalSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_colour_9 = nullptr;
    button_current_editor_color = nullptr;
    colour_selector = nullptr;
    label_colour_6 = nullptr;
    label_colour_4 = nullptr;
    label_colour_5 = nullptr;
    slider_morph_motor_time = nullptr;
    slider_glide_time = nullptr;
    label13 = nullptr;
    label7 = nullptr;
    label_16 = nullptr;
    toggle_animate_input_env = nullptr;
    label_18 = nullptr;
    toggle_animate_eq_env = nullptr;
    label_19 = nullptr;
    toggle_output_cc_mute4 = nullptr;
    label_ui_headline_1 = nullptr;
    label_ui_headline_2 = nullptr;
    label_2 = nullptr;
    label_cpu_usage = nullptr;
    label_4 = nullptr;
    combo_multicore_cpus = nullptr;
    label_ui_headline_4 = nullptr;
    slider_morph_motor_time2 = nullptr;
    label2 = nullptr;
    label_3 = nullptr;
    label_5 = nullptr;
    label_6 = nullptr;
    toggle_animate_input_env2 = nullptr;
    toggle_animate_input_env3 = nullptr;
    toggle_animate_input_env4 = nullptr;
    label_ui_headline_5 = nullptr;
    label_colour_1 = nullptr;
    label_colour_7 = nullptr;
    label_colour_8 = nullptr;
    label_colour_2 = nullptr;
    button_colour_bg = nullptr;
    button_colour_border = nullptr;
    button_colour_buttons_on = nullptr;
    button_colour_slider_1 = nullptr;
    button_colour_slider_2 = nullptr;
    button_colour_slider_mod = nullptr;
    label_colour_3 = nullptr;
    button_colour_signal_lines = nullptr;
    button_colour_buttons_off = nullptr;
    button_colour_labels = nullptr;
    label_ui_headline_6 = nullptr;
    label_7 = nullptr;
    combo_audio_device = nullptr;
    label_8 = nullptr;
    label_9 = nullptr;
    combo_sample_rate = nullptr;
    combo_block_size = nullptr;
    label_10 = nullptr;
    combo_audio_driver = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UiEditorGlobalSettings::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (10.0f, 10.0f, 190.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (200.0f, 10.0f, 180.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (710.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (970.0f, 10.0f, 485.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (380.0f, 10.0f, 330.0f, 150.0f, 6.000f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UiEditorGlobalSettings::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label_colour_9->setBounds (1320, 110, 120, 33);
    button_current_editor_color->setBounds (1330, 40, 90, 90);
    colour_selector->setBounds (990, 30, 330, 110);
    label_colour_6->setBounds (1170, 110, 110, 33);
    label_colour_4->setBounds (1170, 30, 110, 33);
    label_colour_5->setBounds (1170, 70, 110, 33);
    slider_morph_motor_time->setBounds (30, 40, 60, 70);
    slider_glide_time->setBounds (120, 40, 60, 70);
    label13->setBounds (12, 110, 96, 33);
    label7->setBounds (120, 110, 60, 33);
    label_16->setBounds (260, 30, 100, 33);
    toggle_animate_input_env->setBounds (220, 30, 33, 33);
    label_18->setBounds (260, 70, 100, 33);
    toggle_animate_eq_env->setBounds (220, 70, 33, 33);
    label_19->setBounds (260, 110, 100, 33);
    toggle_output_cc_mute4->setBounds (220, 110, 33, 33);
    label_ui_headline_1->setBounds (30, -4, 150, 35);
    label_ui_headline_2->setBounds (220, -4, 140, 35);
    label_2->setBounds (540, 70, 60, 33);
    label_cpu_usage->setBounds (610, 70, 80, 33);
    label_4->setBounds (540, 110, 60, 33);
    combo_multicore_cpus->setBounds (610, 110, 80, 30);
    label_ui_headline_4->setBounds (760, -4, 170, 35);
    slider_morph_motor_time2->setBounds (740, 40, 60, 70);
    label2->setBounds (720, 110, 100, 33);
    label_3->setBounds (860, 30, 100, 33);
    label_5->setBounds (860, 70, 100, 33);
    label_6->setBounds (860, 110, 100, 33);
    toggle_animate_input_env2->setBounds (830, 30, 33, 33);
    toggle_animate_input_env3->setBounds (830, 70, 33, 33);
    toggle_animate_input_env4->setBounds (830, 110, 33, 33);
    label_ui_headline_5->setBounds (1120, -4, 182, 35);
    label_colour_1->setBounds (1020, 30, 110, 33);
    label_colour_7->setBounds (1320, 30, 120, 33);
    label_colour_8->setBounds (1320, 70, 120, 33);
    label_colour_2->setBounds (1020, 70, 110, 33);
    button_colour_bg->setBounds (990, 30, 33, 30);
    button_colour_border->setBounds (990, 70, 33, 30);
    button_colour_buttons_on->setBounds (1140, 30, 33, 30);
    button_colour_slider_1->setBounds (1290, 30, 33, 30);
    button_colour_slider_2->setBounds (1290, 70, 33, 30);
    button_colour_slider_mod->setBounds (1290, 110, 33, 30);
    label_colour_3->setBounds (1020, 110, 110, 33);
    button_colour_signal_lines->setBounds (990, 110, 33, 30);
    button_colour_buttons_off->setBounds (1140, 70, 33, 30);
    button_colour_labels->setBounds (1140, 110, 33, 30);
    label_ui_headline_6->setBounds (460, -4, 170, 35);
    label_7->setBounds (390, 70, 60, 33);
    combo_audio_device->setBounds (460, 70, 80, 30);
    label_8->setBounds (390, 110, 60, 33);
    label_9->setBounds (540, 30, 60, 33);
    combo_sample_rate->setBounds (460, 110, 80, 30);
    combo_block_size->setBounds (610, 30, 80, 30);
    label_10->setBounds (390, 30, 60, 33);
    combo_audio_driver->setBounds (460, 30, 80, 30);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void UiEditorGlobalSettings::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_current_editor_color)
    {
        //[UserButtonCode_button_current_editor_color] -- add your button handler code here..
        button_current_editor_color->setVisible(false);
        colour_selector->setVisible(false);

        label_colour_1->setVisible(true);
        label_colour_2->setVisible(true);
        label_colour_3->setVisible(true);
        label_colour_4->setVisible(true);
        label_colour_5->setVisible(true);
        label_colour_6->setVisible(true);
        label_colour_7->setVisible(true);
        label_colour_8->setVisible(true);
        label_colour_9->setVisible(true);
        button_colour_bg->setVisible(true);
        button_colour_border->setVisible(true);
        button_colour_buttons_on->setVisible(true);
        button_colour_buttons_off->setVisible(true);
        button_colour_slider_1->setVisible(true);
        button_colour_slider_2->setVisible(true);
        button_colour_slider_mod->setVisible(true);
        button_colour_signal_lines->setVisible(true);
        button_colour_labels->setVisible(true);

        current_edited_colour = nullptr;
        //[/UserButtonCode_button_current_editor_color]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env)
    {
        //[UserButtonCode_toggle_animate_input_env] -- add your button handler code here..
        DATA( synth_data ).animate_input_env = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_input_env]
    }
    else if (buttonThatWasClicked == toggle_animate_eq_env)
    {
        //[UserButtonCode_toggle_animate_eq_env] -- add your button handler code here..
        DATA( synth_data ).animate_eq_env = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_eq_env]
    }
    else if (buttonThatWasClicked == toggle_output_cc_mute4)
    {
        //[UserButtonCode_toggle_output_cc_mute4] -- add your button handler code here..
        DATA( synth_data ).animate_modulations = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_output_cc_mute4]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env2)
    {
        //[UserButtonCode_toggle_animate_input_env2] -- add your button handler code here..
        //[/UserButtonCode_toggle_animate_input_env2]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env3)
    {
        //[UserButtonCode_toggle_animate_input_env3] -- add your button handler code here..
        //[/UserButtonCode_toggle_animate_input_env3]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env4)
    {
        //[UserButtonCode_toggle_animate_input_env4] -- add your button handler code here..
        //[/UserButtonCode_toggle_animate_input_env4]
    }
    else if (buttonThatWasClicked == button_colour_bg)
    {
        //[UserButtonCode_button_colour_bg] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.bg );
        //[/UserButtonCode_button_colour_bg]
    }
    else if (buttonThatWasClicked == button_colour_border)
    {
        //[UserButtonCode_button_colour_border] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.bg_lines );
        //[/UserButtonCode_button_colour_border]
    }
    else if (buttonThatWasClicked == button_colour_buttons_on)
    {
        //[UserButtonCode_button_colour_buttons_on] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.button_on_colour );
        //[/UserButtonCode_button_colour_buttons_on]
    }
    else if (buttonThatWasClicked == button_colour_slider_1)
    {
        //[UserButtonCode_button_colour_slider_1] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.slider_track_colour );
        //[/UserButtonCode_button_colour_slider_1]
    }
    else if (buttonThatWasClicked == button_colour_slider_2)
    {
        //[UserButtonCode_button_colour_slider_2] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.slider_track_colour_2 );
        //[/UserButtonCode_button_colour_slider_2]
    }
    else if (buttonThatWasClicked == button_colour_slider_mod)
    {
        //[UserButtonCode_button_colour_slider_mod] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.slider_track_colour_modulation );
        //[/UserButtonCode_button_colour_slider_mod]
    }
    else if (buttonThatWasClicked == button_colour_signal_lines)
    {
        //[UserButtonCode_button_colour_signal_lines] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.signal_lines );
        //[/UserButtonCode_button_colour_signal_lines]
    }
    else if (buttonThatWasClicked == button_colour_buttons_off)
    {
        //[UserButtonCode_button_colour_buttons_off] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.button_off_colour );
        //[/UserButtonCode_button_colour_buttons_off]
    }
    else if (buttonThatWasClicked == button_colour_labels)
    {
        //[UserButtonCode_button_colour_labels] -- add your button handler code here..
        open_colour_selector( UiLookAndFeel::getInstance()->colours.label_text_colour );
        //[/UserButtonCode_button_colour_labels]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void UiEditorGlobalSettings::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_morph_motor_time)
    {
        //[UserSliderCode_slider_morph_motor_time] -- add your slider handling code here..
        DATA( synth_data ).morph_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_morph_motor_time]
    }
    else if (sliderThatWasMoved == slider_glide_time)
    {
        //[UserSliderCode_slider_glide_time] -- add your slider handling code here..
        DATA( synth_data ).glide_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_glide_time]
    }
    else if (sliderThatWasMoved == slider_morph_motor_time2)
    {
        //[UserSliderCode_slider_morph_motor_time2] -- add your slider handling code here..
        //[/UserSliderCode_slider_morph_motor_time2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void UiEditorGlobalSettings::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_multicore_cpus)
    {
        //[UserComboBoxCode_combo_multicore_cpus] -- add your combo box handling code here..
        DATA( synth_data ).num_extra_threads = combo_multicore_cpus->getSelectedId()-1;
        //[/UserComboBoxCode_combo_multicore_cpus]
    }
    else if (comboBoxThatHasChanged == combo_audio_device)
    {
        //[UserComboBoxCode_combo_audio_device] -- add your combo box handling code here..
        MoniqueAudioProcessor* audio_processor = AppInstanceStore::getInstance()->audio_processor;

        // GET CURRENT SETTINGS
        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );

        // UPDATE SETTINGS
        current_device_setup.outputDeviceName = combo_audio_device->getText();
        current_device_setup.sampleRate = 0;
        current_device_setup.bufferSize = 0;
        current_device_setup.inputChannels = 0;
        current_device_setup.useDefaultInputChannels = false;
        current_device_setup.outputChannels = 2;
        current_device_setup.useDefaultOutputChannels = false;

        // SET NEW SETTINGS
        String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

        if( error != "" )
        {
            AlertWindow::showNativeDialogBox
            (
                "ERROR: CAN NOT SELECT AUDIO DEVICE!",
                error,
                true
            );
        }
        else if( not audio_processor->audio_is_successful_initalized )
        {
            String error = audio_processor->initialise( 0,2, nullptr, false );
            if( error == "" )
            {
                audio_processor->addAudioCallback (&audio_processor->player);
                audio_processor->player.setProcessor (audio_processor);
                audio_processor->audio_is_successful_initalized = true;
            }
            else
            {
                AlertWindow::showNativeDialogBox
                (
                    "ERROR: CAN NOT SELECT AUDIO DEVICE!",
                    error,
                    true
                );
            }
        }

        update_audio_devices();
        //[/UserComboBoxCode_combo_audio_device]
    }
    else if (comboBoxThatHasChanged == combo_sample_rate)
    {
        //[UserComboBoxCode_combo_sample_rate] -- add your combo box handling code here..
        MoniqueAudioProcessor* audio_processor = AppInstanceStore::getInstance()->audio_processor;
        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );
        current_device_setup.sampleRate = combo_sample_rate->getText().getDoubleValue();
        String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

        if( error != "" )
            AlertWindow::showNativeDialogBox
            (
                "ERROR: CAN NOT SET SAMPLE RATE!",
                error,
                true
            );

        update_audio_devices();
        //[/UserComboBoxCode_combo_sample_rate]
    }
    else if (comboBoxThatHasChanged == combo_block_size)
    {
        //[UserComboBoxCode_combo_block_size] -- add your combo box handling code here..
        MoniqueAudioProcessor* audio_processor = AppInstanceStore::getInstance()->audio_processor;
        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );
        current_device_setup.bufferSize = combo_block_size->getText().getIntValue();
        String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

        if( error != "" )
            AlertWindow::showNativeDialogBox
            (
                "ERROR: CAN NOT SET BLOCK SIZE!",
                error,
                true
            );

        update_audio_devices();
        //[/UserComboBoxCode_combo_block_size]
    }
    else if (comboBoxThatHasChanged == combo_audio_driver)
    {
        //[UserComboBoxCode_combo_audio_driver] -- add your combo box handling code here..
        MoniqueAudioProcessor* audio_processor = AppInstanceStore::getInstance()->audio_processor;
        audio_processor->setCurrentAudioDeviceType(combo_audio_driver->getText(),true);
        if( not audio_processor->audio_is_successful_initalized )
        {
            String error = audio_processor->initialise(0,2, nullptr, false );
            if( error != "" )
            {
                AlertWindow::showNativeDialogBox
                (
                    "ERROR: CAN NOT SELECT AUDIO DEVICE!",
                    error,
                    true
                );
            }
        }

        update_audio_devices();
        //[/UserComboBoxCode_combo_audio_driver]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UiEditorGlobalSettings" componentName=""
                 parentClasses="public Component, public mono_UiRefreshable" constructorParams=""
                 variableInitialisers="original_w(1465), original_h(180)" snapPixels="10"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="1465" initialHeight="180">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="10 10 190 150" cornerSize="6" fill="solid: 50505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="200 10 180 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="710 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="970 10 485 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="380 10 330 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
  </BACKGROUND>
  <LABEL name="" id="f570869fba483c15" memberName="label_colour_9" virtualName=""
         explicitFocusOrder="0" pos="1320 110 120 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MODULATION SLIDER (BACK)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="63428e6e6c1ae9c4" memberName="button_current_editor_color"
              virtualName="" explicitFocusOrder="0" pos="1330 40 90 90" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="f80f96857b452ce6" memberName="colour_selector" virtualName="ColourSelector"
                    explicitFocusOrder="0" pos="990 30 330 110" class="ColourSelector"
                    params="ColourSelector::showColourspace&#10;,0,7"/>
  <LABEL name="" id="62a873940890f9fe" memberName="label_colour_6" virtualName=""
         explicitFocusOrder="0" pos="1170 110 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LABELS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="4164e3b93fd006e0" memberName="label_colour_4" virtualName=""
         explicitFocusOrder="0" pos="1170 30 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BUTTON ON" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d6b2a5702bcb546f" memberName="label_colour_5" virtualName=""
         explicitFocusOrder="0" pos="1170 70 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BUTTON OFF" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="30eb597ef479a74e" memberName="slider_morph_motor_time"
          virtualName="Slider" explicitFocusOrder="0" pos="30 40 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="c244467fb56dffee" memberName="slider_glide_time"
          virtualName="Slider" explicitFocusOrder="0" pos="120 40 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="796db125cec623bd" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="12 110 96 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="MORPH MOTOR" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="2c68301961d63e82" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="120 110 60 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="GLIDE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="dfd8e91824767f78" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="260 30 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="INPUT ENV'S" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="f342323c5495d0a1" memberName="toggle_animate_input_env"
                virtualName="" explicitFocusOrder="0" pos="220 30 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="55ed48882dea6ac8" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="260 70 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="EQ ENV" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="80003915f6558086" memberName="toggle_animate_eq_env"
                virtualName="" explicitFocusOrder="0" pos="220 70 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="" id="48465e518b2a9587" memberName="label_19" virtualName=""
         explicitFocusOrder="0" pos="260 110 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MODULATIONS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="13a54c23c88819cb" memberName="toggle_output_cc_mute4"
                virtualName="" explicitFocusOrder="0" pos="220 110 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline_1"
         virtualName="" explicitFocusOrder="0" pos="30 -4 150 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLOBAL TIMES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="b5bc2cbedd6ff2b1" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="220 -4 140 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ANIMATIONS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="a5e27df00dd3061" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="540 70 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CPU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="6ddda2710d986dce" memberName="label_cpu_usage" virtualName=""
         explicitFocusOrder="0" pos="610 70 80 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="20%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="5a530fedc3a6cb0" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="540 110 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THREADS:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_multicore_cpus"
            virtualName="" explicitFocusOrder="0" pos="610 110 80 30" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="DL" id="dd5b53cdc2ebaa9f" memberName="label_ui_headline_4"
         virtualName="" explicitFocusOrder="0" pos="760 -4 170 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SLIDERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="ada70618221d405" memberName="slider_morph_motor_time2"
          virtualName="Slider" explicitFocusOrder="0" pos="740 40 60 70"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="1000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="e594adc8a1c69523" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="720 110 100 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SENSITIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="308a6ce808eddf2e" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="860 30 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="LINEAR DRAG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="70949c3310a2e558" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="860 70 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ROTARY DRAG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="c89a79c1406a0ae0" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="860 110 100 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="WHEEL DRAG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="992c96a954884e8b" memberName="toggle_animate_input_env2"
                virtualName="" explicitFocusOrder="0" pos="830 30 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="83332647a3954a0b" memberName="toggle_animate_input_env3"
                virtualName="" explicitFocusOrder="0" pos="830 70 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="" id="3a61c0b11e0af4ba" memberName="toggle_animate_input_env4"
                virtualName="" explicitFocusOrder="0" pos="830 110 33 33" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="DL" id="9eb0361b7a0444ac" memberName="label_ui_headline_5"
         virtualName="" explicitFocusOrder="0" pos="1120 -4 182 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="COLOURS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="55a3217ef78a3c61" memberName="label_colour_1" virtualName=""
         explicitFocusOrder="0" pos="1020 30 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BACKGROUND" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="bb43131d9973a62b" memberName="label_colour_7" virtualName=""
         explicitFocusOrder="0" pos="1320 30 120 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="VALUE SLIDER (FRONT)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="414941fefbe1a3cf" memberName="label_colour_8" virtualName=""
         explicitFocusOrder="0" pos="1320 70 120 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="VALUE SLIDER 2 (BACK)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="2efba9c3ce22653b" memberName="label_colour_2" virtualName=""
         explicitFocusOrder="0" pos="1020 70 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BORDERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="914ce4dd638de5f3" memberName="button_colour_bg"
              virtualName="" explicitFocusOrder="0" pos="990 30 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e427e1f8d74ea481" memberName="button_colour_border"
              virtualName="" explicitFocusOrder="0" pos="990 70 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3891f5f1ede2a913" memberName="button_colour_buttons_on"
              virtualName="" explicitFocusOrder="0" pos="1140 30 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2bde73aa342c2457" memberName="button_colour_slider_1"
              virtualName="" explicitFocusOrder="0" pos="1290 30 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8ddcc2620647afb5" memberName="button_colour_slider_2"
              virtualName="" explicitFocusOrder="0" pos="1290 70 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="27311d6a38d6510d" memberName="button_colour_slider_mod"
              virtualName="" explicitFocusOrder="0" pos="1290 110 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="af67a383712e7be7" memberName="label_colour_3" virtualName=""
         explicitFocusOrder="0" pos="1020 110 110 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SIGNAL LINES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="a907651961c48243" memberName="button_colour_signal_lines"
              virtualName="" explicitFocusOrder="0" pos="990 110 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="aa8131218b1a4e24" memberName="button_colour_buttons_off"
              virtualName="" explicitFocusOrder="0" pos="1140 70 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="16cb69a9f61a0c7f" memberName="button_colour_labels"
              virtualName="" explicitFocusOrder="0" pos="1140 110 33 30" buttonText=""
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="DL" id="668d26bcc5860c72" memberName="label_ui_headline_6"
         virtualName="" explicitFocusOrder="0" pos="460 -4 170 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="AUDIO &amp; CPU"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e0756600ee1aa0c2" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="390 70 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DEVICE:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="efb590c00df9d613" memberName="combo_audio_device"
            virtualName="" explicitFocusOrder="0" pos="460 70 80 30" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="49f12ab9e3d54910" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="390 110 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RATE:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="9e7b39300fbdcd67" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="540 30 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BLOCK:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="db95d5d8a64a8ebc" memberName="combo_sample_rate"
            virtualName="" explicitFocusOrder="0" pos="460 110 80 30" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="" id="d76df912445a2ff8" memberName="combo_block_size" virtualName=""
            explicitFocusOrder="0" pos="610 30 80 30" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="2c9d694778c498dc" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="390 30 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DRIVER:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="f91daaa7098deafb" memberName="combo_audio_driver"
            virtualName="" explicitFocusOrder="0" pos="460 30 80 30" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
