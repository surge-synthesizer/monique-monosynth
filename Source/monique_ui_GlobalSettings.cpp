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
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"

#include "monique_core_Datastructures.h"
#include "monique_core_Processor.h"
//[/Headers]

#include "monique_ui_GlobalSettings.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_GlobalSettings::refresh() noexcept
{
    MoniqueSynthData& synth_data( GET_DATA( synth_data ) );

    slider_morph_motor_time->setValue( synth_data.morph_motor_time.get_value(), dontSendNotification );
    slider_glide_time->setValue( synth_data.glide_motor_time.get_value(), dontSendNotification );

    toggle_animate_input_env->setToggleState( synth_data.animate_envs ,dontSendNotification );
    toggle_show_tooltips->setToggleState( synth_data.show_tooltips ,dontSendNotification );

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
        button_colour_border->setColour (TextButton::buttonColourId,  UiLookAndFeel::getInstance()->colours.bg_lines );
        button_colour_buttons_on->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_on_colour);
        button_colour_buttons_off->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour);
        button_colour_slider_1->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour);
        button_colour_slider_2->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour_2);
        button_colour_slider_mod->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.slider_track_colour_modulation);
        button_colour_signal_lines->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.signal_lines);
        button_colour_labels->setColour (TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.label_text_colour);
    }

    // THREADS
    combo_multicore_cpus->setSelectedId( synth_data.num_extra_threads+1, dontSendNotification );

    // SLIDERS
    toggle_slider_rotary->setToggleState( synth_data.sliders_in_rotary_mode, dontSendNotification );
    toggle_slider_linear->setToggleState( not synth_data.sliders_in_rotary_mode, dontSendNotification );
    slider_sensitivity->setEnabled( not synth_data.sliders_in_rotary_mode );
}

void Monique_Ui_GlobalSettings::open_colour_selector(Colour& colour_to_edit_)
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

void Monique_Ui_GlobalSettings::update_audio_devices()
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
Monique_Ui_GlobalSettings::Monique_Ui_GlobalSettings ()
    : original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (text_info_monique = new TextEditor ("new text editor"));
    text_info_monique->setMultiLine (true);
    text_info_monique->setReturnKeyStartsNewLine (false);
    text_info_monique->setReadOnly (true);
    text_info_monique->setScrollbarsShown (false);
    text_info_monique->setCaretVisible (false);
    text_info_monique->setPopupMenuEnabled (false);
    text_info_monique->setColour (TextEditor::backgroundColourId, Colour (0xff050505));
    text_info_monique->setText (TRANS("Aus Liebe zur Musik.\n"
    "Thomas Arndt | Monoplugs"));

    addAndMakeVisible (image_vst = new ImageButton ("new button"));
    image_vst->setButtonText (String::empty);

    image_vst->setImages (false, true, true,
                          ImageCache::getFromMemory (vst_logo_100x_png, vst_logo_100x_pngSize), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000));
    addAndMakeVisible (text_info_vst = new TextEditor ("new text editor"));
    text_info_vst->setMultiLine (true);
    text_info_vst->setReturnKeyStartsNewLine (false);
    text_info_vst->setReadOnly (true);
    text_info_vst->setScrollbarsShown (false);
    text_info_vst->setCaretVisible (false);
    text_info_vst->setPopupMenuEnabled (false);
    text_info_vst->setColour (TextEditor::backgroundColourId, Colour (0xff050505));
    text_info_vst->setText (TRANS("VST is a trademark of Steinberg Media Technologies GmbH"));

    addAndMakeVisible (link_to_monoplugs = new HyperlinkButton (String::empty,
                                                                URL ("http://monique-synthesizer.monoplugs.com")));
    link_to_monoplugs->setTooltip (TRANS("http://monique-synthesizer.monoplugs.com"));

    addAndMakeVisible (colour_selector = new ColourSelector (ColourSelector::showColourspace
                                                             ,0,7));

    addAndMakeVisible (label_colour_4 = new Label (String::empty,
                                                   TRANS("BUTTON ON")));
    label_colour_4->setTooltip (TRANS("Click to change this colour."));
    label_colour_4->setFont (Font (30.00f, Font::plain));
    label_colour_4->setJustificationType (Justification::centredLeft);
    label_colour_4->setEditable (false, false, false);
    label_colour_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_current_editor_color = new TextButton (String::empty));
    button_current_editor_color->setButtonText (TRANS("OK"));
    button_current_editor_color->addListener (this);

    addAndMakeVisible (label_colour_7 = new Label (String::empty,
                                                   TRANS("VALUE SLIDER (FRONT)")));
    label_colour_7->setTooltip (TRANS("Click to change this colour."));
    label_colour_7->setFont (Font (30.00f, Font::plain));
    label_colour_7->setJustificationType (Justification::centredLeft);
    label_colour_7->setEditable (false, false, false);
    label_colour_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_9 = new Label (String::empty,
                                                   TRANS("MODULATION SLIDER (BACK)")));
    label_colour_9->setTooltip (TRANS("Click to change this colour."));
    label_colour_9->setFont (Font (30.00f, Font::plain));
    label_colour_9->setJustificationType (Justification::centredLeft);
    label_colour_9->setEditable (false, false, false);
    label_colour_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_6 = new Label (String::empty,
                                                   TRANS("LABELS")));
    label_colour_6->setTooltip (TRANS("Click to change this colour."));
    label_colour_6->setFont (Font (30.00f, Font::plain));
    label_colour_6->setJustificationType (Justification::centredLeft);
    label_colour_6->setEditable (false, false, false);
    label_colour_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_5 = new Label (String::empty,
                                                   TRANS("BUTTON OFF")));
    label_colour_5->setTooltip (TRANS("Click to change this colour."));
    label_colour_5->setFont (Font (30.00f, Font::plain));
    label_colour_5->setJustificationType (Justification::centredLeft);
    label_colour_5->setEditable (false, false, false);
    label_colour_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider_morph_motor_time = new Slider ("0"));
    slider_morph_motor_time->setTooltip (TRANS("Define the MORPH MOTOR TIME.\n"
    "\n"
    "Morph motor time is the speed of the morph sliders if you morph via the morph pad or the horizontal slider below the morph sliders"));
    slider_morph_motor_time->setRange (1, 20000, 1);
    slider_morph_motor_time->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_morph_motor_time->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_morph_motor_time->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_morph_motor_time->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_morph_motor_time->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_morph_motor_time->addListener (this);

    addAndMakeVisible (slider_glide_time = new Slider ("0"));
    slider_glide_time->setTooltip (TRANS("Define the GLIDE MOTOR TIME.\n"
    "\n"
    "Glide motor time is smoothes your inputs at many sliders on the main user interface. \n"
    "\n"
    "Set a large value here and try to change the MASTER VOLUME and do it again with a small one.\n"
    "\n"
    "Affected: OSC sliders, FILTER sliders, EQ sliders, FX sliders, MASTER VOLUME"));
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
                                             TRANS("ANIMATE ENV\'S")));
    label_16->setTooltip (TRANS("Turn amp animations at the FILTER INPUS on or off."));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_input_env = new ToggleButton (String::empty));
    toggle_animate_input_env->setTooltip (TRANS("Turn amp animations at the FILTER INPUS on or off."));
    toggle_animate_input_env->addListener (this);

    addAndMakeVisible (label_18 = new Label (String::empty,
                                             TRANS("SHOW TOOLTIPS")));
    label_18->setTooltip (TRANS("Turn tooltips on or off.\n"
    "\n"
    "Press the \"h\" on your keyboard to show the tooltip if this option is turned off."));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredLeft);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_show_tooltips = new ToggleButton (String::empty));
    toggle_show_tooltips->setTooltip (TRANS("Turn amp animations at the EQ BANK on or off."));
    toggle_show_tooltips->addListener (this);

    addAndMakeVisible (label_ui_headline_1 = new Label ("DL",
                                                        TRANS("GLOBAL TIMES")));
    label_ui_headline_1->setFont (Font (30.00f, Font::plain));
    label_ui_headline_1->setJustificationType (Justification::centred);
    label_ui_headline_1->setEditable (false, false, false);
    label_ui_headline_1->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_2 = new Label ("DL",
                                                        TRANS("MISC")));
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
    combo_multicore_cpus->setTooltip (TRANS("Select the threads you like to spend to process Moniqiue. \n"
    "\n"
    "Note: Its recommended to use NOT more threads as your CPU has cores! \n"
    "Please take a look at the CPU usage and decide how many threads are the best for your CPU."));
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

    addAndMakeVisible (slider_sensitivity = new Slider ("0"));
    slider_sensitivity->setTooltip (TRANS("Define the sensitivity of each rotary slider. You can test the effect with this slider."));
    slider_sensitivity->setRange (100, 2000, 1);
    slider_sensitivity->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider_sensitivity->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider_sensitivity->setColour (Slider::rotarySliderFillColourId, Colours::yellow);
    slider_sensitivity->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff161616));
    slider_sensitivity->setColour (Slider::textBoxTextColourId, Colours::yellow);
    slider_sensitivity->setColour (Slider::textBoxBackgroundColourId, Colour (0xff161616));
    slider_sensitivity->addListener (this);

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
    label_3->setTooltip (TRANS("Set the slider drag to linear (rotary sliders). \n"
    "(You can drag a slider by move the mouse up/down or left/right)"));
    label_3->setFont (Font (30.00f, Font::plain));
    label_3->setJustificationType (Justification::centredLeft);
    label_3->setEditable (false, false, false);
    label_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_5 = new Label (String::empty,
                                            TRANS("ROTARY DRAG")));
    label_5->setTooltip (TRANS("Set the slider drag to rotary (rotary sliders). \n"
    "(You can drag a slider by move the mouse around the slider)"));
    label_5->setFont (Font (30.00f, Font::plain));
    label_5->setJustificationType (Justification::centredLeft);
    label_5->setEditable (false, false, false);
    label_5->setColour (Label::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_slider_linear = new ToggleButton (String::empty));
    toggle_slider_linear->setTooltip (TRANS("Set the slider drag to linear (rotary sliders). \n"
    "(You can drag a slider by move the mouse up/down or left/right)"));
    toggle_slider_linear->addListener (this);

    addAndMakeVisible (toggle_slider_rotary = new ToggleButton (String::empty));
    toggle_slider_rotary->setTooltip (TRANS("Set the slider drag to rotary (rotary sliders). \n"
    "(You can drag a slider by move the mouse around the slider)"));
    toggle_slider_rotary->addListener (this);

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
    label_colour_1->setTooltip (TRANS("Click to change this colour."));
    label_colour_1->setFont (Font (30.00f, Font::plain));
    label_colour_1->setJustificationType (Justification::centredLeft);
    label_colour_1->setEditable (false, false, false);
    label_colour_1->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_1->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_8 = new Label (String::empty,
                                                   TRANS("VALUE SLIDER 2 (BACK)")));
    label_colour_8->setTooltip (TRANS("Click to change this colour."));
    label_colour_8->setFont (Font (30.00f, Font::plain));
    label_colour_8->setJustificationType (Justification::centredLeft);
    label_colour_8->setEditable (false, false, false);
    label_colour_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour_2 = new Label (String::empty,
                                                   TRANS("BORDERS")));
    label_colour_2->setTooltip (TRANS("Click to change this colour."));
    label_colour_2->setFont (Font (30.00f, Font::plain));
    label_colour_2->setJustificationType (Justification::centredLeft);
    label_colour_2->setEditable (false, false, false);
    label_colour_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_colour_bg = new TextButton ("new button"));
    button_colour_bg->setTooltip (TRANS("Click to change this colour."));
    button_colour_bg->setButtonText (String::empty);
    button_colour_bg->addListener (this);

    addAndMakeVisible (button_colour_border = new TextButton ("new button"));
    button_colour_border->setTooltip (TRANS("Click to change this colour."));
    button_colour_border->setButtonText (String::empty);
    button_colour_border->addListener (this);

    addAndMakeVisible (button_colour_buttons_on = new TextButton ("new button"));
    button_colour_buttons_on->setTooltip (TRANS("Click to change this colour."));
    button_colour_buttons_on->setButtonText (String::empty);
    button_colour_buttons_on->addListener (this);

    addAndMakeVisible (button_colour_slider_1 = new TextButton ("new button"));
    button_colour_slider_1->setTooltip (TRANS("Click to change this colour."));
    button_colour_slider_1->setButtonText (String::empty);
    button_colour_slider_1->addListener (this);

    addAndMakeVisible (button_colour_slider_2 = new TextButton ("new button"));
    button_colour_slider_2->setTooltip (TRANS("Click to change this colour."));
    button_colour_slider_2->setButtonText (String::empty);
    button_colour_slider_2->addListener (this);

    addAndMakeVisible (button_colour_slider_mod = new TextButton ("new button"));
    button_colour_slider_mod->setTooltip (TRANS("Click to change this colour."));
    button_colour_slider_mod->setButtonText (String::empty);
    button_colour_slider_mod->addListener (this);

    addAndMakeVisible (label_colour_3 = new Label (String::empty,
                                                   TRANS("SIGNAL LINES")));
    label_colour_3->setTooltip (TRANS("Click to change this colour."));
    label_colour_3->setFont (Font (30.00f, Font::plain));
    label_colour_3->setJustificationType (Justification::centredLeft);
    label_colour_3->setEditable (false, false, false);
    label_colour_3->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_colour_signal_lines = new TextButton ("new button"));
    button_colour_signal_lines->setTooltip (TRANS("Click to change this colour."));
    button_colour_signal_lines->setButtonText (String::empty);
    button_colour_signal_lines->addListener (this);

    addAndMakeVisible (button_colour_buttons_off = new TextButton ("new button"));
    button_colour_buttons_off->setTooltip (TRANS("Click to change this colour."));
    button_colour_buttons_off->setButtonText (String::empty);
    button_colour_buttons_off->addListener (this);

    addAndMakeVisible (button_colour_labels = new TextButton ("new button"));
    button_colour_labels->setTooltip (TRANS("Click to change this colour."));
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
    combo_audio_device->setTooltip (TRANS("Select an audio device you like to use for the audio playback."));
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
    combo_sample_rate->setTooltip (TRANS("Select the sample rate you like to use for the audio playback.\n"
    "\n"
    "Note: the quality of larger sample rates is better, but needs more CPU power."));
    combo_sample_rate->setEditableText (false);
    combo_sample_rate->setJustificationType (Justification::centredLeft);
    combo_sample_rate->setTextWhenNothingSelected (String::empty);
    combo_sample_rate->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_sample_rate->addListener (this);

    addAndMakeVisible (combo_block_size = new ComboBox (String::empty));
    combo_block_size->setTooltip (TRANS("Select the block size you like to use for the audio playback.\n"
    "\n"
    "Note: smaller block sizes are more in time, but needs more CPU power."));
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
    combo_audio_driver->setTooltip (TRANS("Select an audio driver you like to use for the audio playback."));
    combo_audio_driver->setEditableText (false);
    combo_audio_driver->setJustificationType (Justification::centredLeft);
    combo_audio_driver->setTextWhenNothingSelected (String::empty);
    combo_audio_driver->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_audio_driver->addListener (this);

    addAndMakeVisible (button_preset_1 = new TextButton ("new button"));
    button_preset_1->setTooltip (TRANS("Click to load this preset."));
    button_preset_1->setButtonText (TRANS("PRE1"));
    button_preset_1->addListener (this);

    addAndMakeVisible (button_preset_2 = new TextButton ("new button"));
    button_preset_2->setTooltip (TRANS("Click to load this preset."));
    button_preset_2->setButtonText (TRANS("PRE2"));
    button_preset_2->addListener (this);

    addAndMakeVisible (button_preset_3 = new TextButton ("new button"));
    button_preset_3->setTooltip (TRANS("Click to load this preset."));
    button_preset_3->setButtonText (TRANS("PRE3"));
    button_preset_3->addListener (this);


    //[UserPreSize]
#ifdef IS_STANDALONE
    text_info_monique->setVisible(false);
    text_info_vst->setVisible(false);
    image_vst->setVisible(false);
    link_to_monoplugs->setVisible(false);

    label_ui_headline_6->setText("AUDIO & CPU",dontSendNotification);
#else
    label_10->setVisible(false);
    label_7->setVisible(false);
    label_8->setVisible(false);
    combo_sample_rate->setVisible(false);
    combo_audio_device->setVisible(false);
    combo_audio_driver->setVisible(false);
    combo_sample_rate->setVisible(false);
    label_9->setVisible(false);
    label_2->setVisible(false);
    label_4->setVisible(false);
    combo_block_size->setVisible(false);
    label_cpu_usage->setVisible(false);
    combo_multicore_cpus->setVisible(false);

    label_ui_headline_6->setText("MONIQUE 1.0",dontSendNotification);
#endif


    setOpaque(true);
    colour_selector->setVisible(false);
    button_current_editor_color->setVisible(false);

    current_edited_colour = nullptr;

    for( int i = 0 ; i != getNumChildComponents() ; ++i ) {
        getChildComponent(i)->setOpaque(true);

        if( Slider* slider = dynamic_cast<Slider*>(getChildComponent(i)) )
	{
            SET_SLIDER_STYLE( slider, VALUE_SLIDER );
        }
    }
    image_vst->setOpaque(false);
    link_to_monoplugs->setOpaque(false);

    button_preset_1->setColour( TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
    button_preset_2->setColour( TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );
    button_preset_3->setColour( TextButton::buttonColourId, UiLookAndFeel::getInstance()->colours.button_off_colour );

    // CPU
    for( int i = 0 ; i != THREAD_LIMIT ; ++i )
        combo_multicore_cpus->addItem(String(i+1),i+1);

    // AUDIO DEVICE
#ifdef IS_PLUGIN
    //label_10->setVisible(false);
    label_10->setEnabled(false);
    //combo_audio_driver->setVisible(false);
    combo_audio_driver->setEnabled(false);
    //label_7->setVisible(false);
    label_7->setEnabled(false);
    //combo_audio_device->setVisible(false);
    combo_audio_device->setEnabled(false);
    //combo_sample_rate->setVisible(false);
    combo_sample_rate->setEnabled(false);
    //label_8->setVisible(false);
    label_8->setEnabled(false);
    //combo_block_size->setVisible(false);
    combo_block_size->setEnabled(false);
    //label_9->setVisible(false);
    label_9->setEnabled(false);
#else
    update_audio_devices();
#endif

    /*
    //[/UserPreSize]

    setSize (1465, 400);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_GlobalSettings::~Monique_Ui_GlobalSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    text_info_monique = nullptr;
    image_vst = nullptr;
    text_info_vst = nullptr;
    link_to_monoplugs = nullptr;
    colour_selector = nullptr;
    label_colour_4 = nullptr;
    button_current_editor_color = nullptr;
    label_colour_7 = nullptr;
    label_colour_9 = nullptr;
    label_colour_6 = nullptr;
    label_colour_5 = nullptr;
    slider_morph_motor_time = nullptr;
    slider_glide_time = nullptr;
    label13 = nullptr;
    label7 = nullptr;
    label_16 = nullptr;
    toggle_animate_input_env = nullptr;
    label_18 = nullptr;
    toggle_show_tooltips = nullptr;
    label_ui_headline_1 = nullptr;
    label_ui_headline_2 = nullptr;
    label_2 = nullptr;
    label_cpu_usage = nullptr;
    label_4 = nullptr;
    combo_multicore_cpus = nullptr;
    label_ui_headline_4 = nullptr;
    slider_sensitivity = nullptr;
    label2 = nullptr;
    label_3 = nullptr;
    label_5 = nullptr;
    toggle_slider_linear = nullptr;
    toggle_slider_rotary = nullptr;
    label_ui_headline_5 = nullptr;
    label_colour_1 = nullptr;
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
    button_preset_1 = nullptr;
    button_preset_2 = nullptr;
    button_preset_3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_GlobalSettings::paint (Graphics& g)
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
    g.drawRoundedRectangle (380.0f, 10.0f, 260.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (640.0f, 10.0f, 485.0f, 150.0f, 6.000f, 1.000f);

    g.setColour (Colour (0xffff3b00));
    g.drawRoundedRectangle (1125.0f, 10.0f, 330.0f, 150.0f, 6.000f, 1.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_GlobalSettings::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    text_info_monique->setBounds (1150, 30, 290, 50);
    image_vst->setBounds (1340, 80, 90, 60);
    text_info_vst->setBounds (1150, 80, 180, 60);
    link_to_monoplugs->setBounds (1150, 30, 280, 50);
    colour_selector->setBounds (660, 30, 270, 110);
    label_colour_4->setBounds (810, 30, 90, 33);
    button_current_editor_color->setBounds (940, 40, 90, 90);
    label_colour_7->setBounds (930, 30, 120, 33);
    label_colour_9->setBounds (930, 110, 120, 33);
    label_colour_6->setBounds (810, 110, 90, 33);
    label_colour_5->setBounds (810, 70, 90, 33);
    slider_morph_motor_time->setBounds (30, 40, 60, 70);
    slider_glide_time->setBounds (120, 40, 60, 70);
    label13->setBounds (12, 110, 96, 33);
    label7->setBounds (120, 110, 60, 33);
    label_16->setBounds (260, 30, 100, 33);
    toggle_animate_input_env->setBounds (220, 30, 33, 33);
    label_18->setBounds (260, 70, 100, 33);
    toggle_show_tooltips->setBounds (220, 70, 33, 33);
    label_ui_headline_1->setBounds (30, -4, 150, 35);
    label_ui_headline_2->setBounds (220, -4, 140, 35);
    label_2->setBounds (1290, 70, 60, 33);
    label_cpu_usage->setBounds (1360, 70, 80, 33);
    label_4->setBounds (1290, 110, 60, 33);
    combo_multicore_cpus->setBounds (1360, 110, 80, 30);
    label_ui_headline_4->setBounds (430, -4, 170, 35);
    slider_sensitivity->setBounds (410, 40, 60, 70);
    label2->setBounds (390, 110, 100, 33);
    label_3->setBounds (530, 50, 100, 33);
    label_5->setBounds (530, 90, 100, 33);
    toggle_slider_linear->setBounds (500, 50, 33, 33);
    toggle_slider_rotary->setBounds (500, 90, 33, 33);
    label_ui_headline_5->setBounds (790, -4, 182, 35);
    label_colour_1->setBounds (690, 30, 90, 33);
    label_colour_8->setBounds (930, 70, 120, 33);
    label_colour_2->setBounds (690, 70, 90, 33);
    button_colour_bg->setBounds (660, 30, 33, 30);
    button_colour_border->setBounds (660, 70, 33, 30);
    button_colour_buttons_on->setBounds (780, 30, 33, 30);
    button_colour_slider_1->setBounds (900, 30, 33, 30);
    button_colour_slider_2->setBounds (900, 70, 33, 30);
    button_colour_slider_mod->setBounds (900, 110, 33, 30);
    label_colour_3->setBounds (690, 110, 90, 33);
    button_colour_signal_lines->setBounds (660, 110, 33, 30);
    button_colour_buttons_off->setBounds (780, 70, 33, 30);
    button_colour_labels->setBounds (780, 110, 33, 30);
    label_ui_headline_6->setBounds (1210, -4, 170, 35);
    label_7->setBounds (1140, 70, 60, 33);
    combo_audio_device->setBounds (1210, 70, 80, 30);
    label_8->setBounds (1140, 110, 60, 33);
    label_9->setBounds (1290, 30, 60, 33);
    combo_sample_rate->setBounds (1210, 110, 80, 30);
    combo_block_size->setBounds (1360, 30, 80, 30);
    label_10->setBounds (1140, 30, 60, 33);
    combo_audio_driver->setBounds (1210, 30, 80, 30);
    button_preset_1->setBounds (1070, 30, 43, 30);
    button_preset_2->setBounds (1070, 70, 43, 30);
    button_preset_3->setBounds (1070, 110, 43, 30);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_GlobalSettings::buttonClicked (Button* buttonThatWasClicked)
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
        GET_DATA( synth_data ).animate_envs = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_input_env]
    }
    else if (buttonThatWasClicked == toggle_show_tooltips)
    {
        //[UserButtonCode_toggle_show_tooltips] -- add your button handler code here..
        GET_DATA( synth_data ).show_tooltips = buttonThatWasClicked->getToggleState();
	AppInstanceStore::getInstance()->editor->update_tooltip_handling(false);
        //[/UserButtonCode_toggle_show_tooltips]
    }
    else if (buttonThatWasClicked == toggle_slider_linear)
    {
        //[UserButtonCode_toggle_slider_linear] -- add your button handler code here..
        GET_DATA( synth_data ).sliders_in_rotary_mode = false;
        AppInstanceStore::getInstance()->editor->update_slider_handling();
        //[/UserButtonCode_toggle_slider_linear]
    }
    else if (buttonThatWasClicked == toggle_slider_rotary)
    {
        //[UserButtonCode_toggle_slider_rotary] -- add your button handler code here..
        GET_DATA( synth_data ).sliders_in_rotary_mode = true;
        AppInstanceStore::getInstance()->editor->update_slider_handling();
        //[/UserButtonCode_toggle_slider_rotary]
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
    else if (buttonThatWasClicked == button_preset_1)
    {
        //[UserButtonCode_button_preset_1] -- add your button handler code here..
        ComponentColours& colours( UiLookAndFeel::getInstance()->colours );
        colours.slider_track_colour = Colour(4278251775);
        colours.slider_track_colour_2 = Colour(0xffff6600);
        colours.slider_track_colour_modulation = Colour(4294942532);
        colours.button_on_colour = Colour(4294942532);
        colours.button_off_colour = Colour(4279308561);
        colours.label_text_colour = Colour(4294942532);
        colours.midi_learn = Colours::red;
        colours.bg = Colour(0xff050505);
        colours.bg_lines = Colour(0xffff3b00);
        colours.signal_lines = Colour(4278251775);

        AppInstanceStore::getInstance()->editor->repaint();
        //[/UserButtonCode_button_preset_1]
    }
    else if (buttonThatWasClicked == button_preset_2)
    {
        //[UserButtonCode_button_preset_2] -- add your button handler code here..
        ComponentColours& colours( UiLookAndFeel::getInstance()->colours );
        colours.slider_track_colour = Colour(4281270271);
        colours.slider_track_colour_2 = Colour(4294966409);
        colours.slider_track_colour_modulation = Colour(4294901760);
        colours.button_on_colour = Colour(4282580976);
        colours.button_off_colour = Colour(4279308561);
        colours.label_text_colour = Colour(4294966408);
        colours.midi_learn = Colours::red;
        colours.bg = Colour(4278190080);
        colours.bg_lines = Colour(4278245119);
        colours.signal_lines = Colour(4289724416);

        AppInstanceStore::getInstance()->editor->repaint();
        //[/UserButtonCode_button_preset_2]
    }
    else if (buttonThatWasClicked == button_preset_3)
    {
        //[UserButtonCode_button_preset_3] -- add your button handler code here..
        ComponentColours& colours( UiLookAndFeel::getInstance()->colours );
        colours.slider_track_colour = Colour(0xff00e2ff);
        colours.slider_track_colour_2 = Colour(0xffefff00);
        colours.slider_track_colour_modulation = Colour(0xffff8800);
        colours.button_on_colour = Colour(0xffd86300);
        colours.button_off_colour = Colour(0xff111111);
        colours.label_text_colour = Colour(0xffffbe73);
        colours.midi_learn = Colours::red;
        colours.bg = Colour(0xff050505);
        colours.bg_lines = Colour(0xffffc28f);
        colours.signal_lines = Colour(0xffe26700);
	/*
st_col="" st2_col="" stmod_col="" bon_col=""
           boff_col="" ltx_col="" ml_col="" bg_col=""
           bgl_col="" sigl_col=""/>
           */
        AppInstanceStore::getInstance()->editor->repaint();
        //[/UserButtonCode_button_preset_3]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_GlobalSettings::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_morph_motor_time)
    {
        //[UserSliderCode_slider_morph_motor_time] -- add your slider handling code here..
        GET_DATA( synth_data ).morph_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_morph_motor_time]
    }
    else if (sliderThatWasMoved == slider_glide_time)
    {
        //[UserSliderCode_slider_glide_time] -- add your slider handling code here..
        GET_DATA( synth_data ).glide_motor_time = sliderThatWasMoved->getValue();
        //[/UserSliderCode_slider_glide_time]
    }
    else if (sliderThatWasMoved == slider_sensitivity)
    {
        //[UserSliderCode_slider_sensitivity] -- add your slider handling code here..
        GET_DATA( synth_data ).sliders_sensitivity = sliderThatWasMoved->getValue();
        AppInstanceStore::getInstance()->editor->update_slider_handling();
        //[/UserSliderCode_slider_sensitivity]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Monique_Ui_GlobalSettings::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_multicore_cpus)
    {
        //[UserComboBoxCode_combo_multicore_cpus] -- add your combo box handling code here..
        GET_DATA( synth_data ).num_extra_threads = combo_multicore_cpus->getSelectedId()-1;
        //[/UserComboBoxCode_combo_multicore_cpus]
    }
    else if (comboBoxThatHasChanged == combo_audio_device)
    {
        //[UserComboBoxCode_combo_audio_device] -- add your combo box handling code here..
#ifdef IS_STANDALONE
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
#endif
        //[/UserComboBoxCode_combo_audio_device]
    }
    else if (comboBoxThatHasChanged == combo_sample_rate)
    {
        //[UserComboBoxCode_combo_sample_rate] -- add your combo box handling code here..
#ifdef IS_STANDALONE
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
#endif
        //[/UserComboBoxCode_combo_sample_rate]
    }
    else if (comboBoxThatHasChanged == combo_block_size)
    {
        //[UserComboBoxCode_combo_block_size] -- add your combo box handling code here..
#ifdef IS_STANDALONE
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
#endif
        //[/UserComboBoxCode_combo_block_size]
    }
    else if (comboBoxThatHasChanged == combo_audio_driver)
    {
        //[UserComboBoxCode_combo_audio_driver] -- add your combo box handling code here..
#ifdef IS_STANDALONE
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
#endif
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

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_GlobalSettings"
                 componentName="" parentClasses="public Component, public Monique_Ui_Refreshable"
                 constructorParams="" variableInitialisers="original_w(1465), original_h(180)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="400">
  <BACKGROUND backgroundColour="ff050505">
    <ROUNDRECT pos="10 10 190 150" cornerSize="6" fill="solid: 50505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="200 10 180 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="380 10 260 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="640 10 485 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
    <ROUNDRECT pos="1125 10 330 150" cornerSize="6" fill="solid: 0" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff3b00"/>
  </BACKGROUND>
  <TEXTEDITOR name="new text editor" id="ba7b0a21898922b7" memberName="text_info_monique"
              virtualName="" explicitFocusOrder="0" pos="1150 30 290 50" bkgcol="ff050505"
              initialText="Aus Liebe zur Musik.&#10;Thomas Arndt | Monoplugs"
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
  <IMAGEBUTTON name="new button" id="bab1ab6909a963b5" memberName="image_vst"
               virtualName="" explicitFocusOrder="0" pos="1340 80 90 60" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" keepProportions="1"
               resourceNormal="vst_logo_100x_png" opacityNormal="1" colourNormal="0"
               resourceOver="" opacityOver="1" colourOver="0" resourceDown=""
               opacityDown="1" colourDown="0"/>
  <TEXTEDITOR name="new text editor" id="5aad8cd486f0e13c" memberName="text_info_vst"
              virtualName="" explicitFocusOrder="0" pos="1150 80 180 60" bkgcol="ff050505"
              initialText="VST is a trademark of Steinberg Media Technologies GmbH"
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
  <HYPERLINKBUTTON name="" id="c0eeed7d74bb8534" memberName="link_to_monoplugs"
                   virtualName="" explicitFocusOrder="0" pos="1150 30 280 50" tooltip="http://monique-synthesizer.monoplugs.com"
                   buttonText="" connectedEdges="0" needsCallback="0" radioGroupId="0"
                   url="http://monique-synthesizer.monoplugs.com"/>
  <GENERICCOMPONENT name="" id="f80f96857b452ce6" memberName="colour_selector" virtualName="ColourSelector"
                    explicitFocusOrder="0" pos="660 30 270 110" class="ColourSelector"
                    params="ColourSelector::showColourspace&#10;,0,7"/>
  <LABEL name="" id="4164e3b93fd006e0" memberName="label_colour_4" virtualName=""
         explicitFocusOrder="0" pos="810 30 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="BUTTON ON"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="" id="63428e6e6c1ae9c4" memberName="button_current_editor_color"
              virtualName="" explicitFocusOrder="0" pos="940 40 90 90" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="bb43131d9973a62b" memberName="label_colour_7" virtualName=""
         explicitFocusOrder="0" pos="930 30 120 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="VALUE SLIDER (FRONT)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="f570869fba483c15" memberName="label_colour_9" virtualName=""
         explicitFocusOrder="0" pos="930 110 120 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="MODULATION SLIDER (BACK)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="62a873940890f9fe" memberName="label_colour_6" virtualName=""
         explicitFocusOrder="0" pos="810 110 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="LABELS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="d6b2a5702bcb546f" memberName="label_colour_5" virtualName=""
         explicitFocusOrder="0" pos="810 70 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="BUTTON OFF"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <SLIDER name="0" id="30eb597ef479a74e" memberName="slider_morph_motor_time"
          virtualName="Slider" explicitFocusOrder="0" pos="30 40 60 70"
          tooltip="Define the MORPH MOTOR TIME.&#10;&#10;Morph motor time is the speed of the morph sliders if you morph via the morph pad or the horizontal slider below the morph sliders"
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="1" max="20000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="c244467fb56dffee" memberName="slider_glide_time"
          virtualName="Slider" explicitFocusOrder="0" pos="120 40 60 70"
          tooltip="Define the GLIDE MOTOR TIME.&#10;&#10;Glide motor time is smoothes your inputs at many sliders on the main user interface. &#10;&#10;Set a large value here and try to change the MASTER VOLUME and do it again with a small one.&#10;&#10;Affected: OSC sliders, FILTER sliders, EQ sliders, FX sliders, MASTER VOLUME"
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
         explicitFocusOrder="0" pos="260 30 100 33" tooltip="Turn amp animations at the FILTER INPUS on or off."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="ANIMATE ENV'S"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="f342323c5495d0a1" memberName="toggle_animate_input_env"
                virtualName="" explicitFocusOrder="0" pos="220 30 33 33" tooltip="Turn amp animations at the FILTER INPUS on or off."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="" id="55ed48882dea6ac8" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="260 70 100 33" tooltip="Turn tooltips on or off.&#10;&#10;Press the &quot;h&quot; on your keyboard to show the tooltip if this option is turned off."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="SHOW TOOLTIPS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="80003915f6558086" memberName="toggle_show_tooltips"
                virtualName="" explicitFocusOrder="0" pos="220 70 33 33" tooltip="Turn amp animations at the EQ BANK on or off."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="DL" id="b59f286362d58d43" memberName="label_ui_headline_1"
         virtualName="" explicitFocusOrder="0" pos="30 -4 150 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="GLOBAL TIMES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="DL" id="b5bc2cbedd6ff2b1" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="220 -4 140 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MISC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="a5e27df00dd3061" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="1290 70 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CPU:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="6ddda2710d986dce" memberName="label_cpu_usage" virtualName=""
         explicitFocusOrder="0" pos="1360 70 80 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="20%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="5a530fedc3a6cb0" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="1290 110 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THREADS:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_multicore_cpus"
            virtualName="" explicitFocusOrder="0" pos="1360 110 80 30" tooltip="Select the threads you like to spend to process Moniqiue. &#10;&#10;Note: Its recommended to use NOT more threads as your CPU has cores! &#10;Please take a look at the CPU usage and decide how many threads are the best for your CPU."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="DL" id="dd5b53cdc2ebaa9f" memberName="label_ui_headline_4"
         virtualName="" explicitFocusOrder="0" pos="430 -4 170 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SLIDERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="ada70618221d405" memberName="slider_sensitivity"
          virtualName="Slider" explicitFocusOrder="0" pos="410 40 60 70"
          tooltip="Define the sensitivity of each rotary slider. You can test the effect with this slider."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="100" max="2000" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="e594adc8a1c69523" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="390 110 100 33" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SENSITIVITY" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="308a6ce808eddf2e" memberName="label_3" virtualName=""
         explicitFocusOrder="0" pos="530 50 100 33" tooltip="Set the slider drag to linear (rotary sliders). &#10;(You can drag a slider by move the mouse up/down or left/right)"
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="LINEAR DRAG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="70949c3310a2e558" memberName="label_5" virtualName=""
         explicitFocusOrder="0" pos="530 90 100 33" tooltip="Set the slider drag to rotary (rotary sliders). &#10;(You can drag a slider by move the mouse around the slider)"
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="ROTARY DRAG"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="992c96a954884e8b" memberName="toggle_slider_linear"
                virtualName="" explicitFocusOrder="0" pos="500 50 33 33" tooltip="Set the slider drag to linear (rotary sliders). &#10;(You can drag a slider by move the mouse up/down or left/right)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="" id="83332647a3954a0b" memberName="toggle_slider_rotary"
                virtualName="" explicitFocusOrder="0" pos="500 90 33 33" tooltip="Set the slider drag to rotary (rotary sliders). &#10;(You can drag a slider by move the mouse around the slider)"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="DL" id="9eb0361b7a0444ac" memberName="label_ui_headline_5"
         virtualName="" explicitFocusOrder="0" pos="790 -4 182 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="COLOURS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="55a3217ef78a3c61" memberName="label_colour_1" virtualName=""
         explicitFocusOrder="0" pos="690 30 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="BACKGROUND"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="414941fefbe1a3cf" memberName="label_colour_8" virtualName=""
         explicitFocusOrder="0" pos="930 70 120 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="VALUE SLIDER 2 (BACK)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="" id="2efba9c3ce22653b" memberName="label_colour_2" virtualName=""
         explicitFocusOrder="0" pos="690 70 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="BORDERS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="914ce4dd638de5f3" memberName="button_colour_bg"
              virtualName="" explicitFocusOrder="0" pos="660 30 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e427e1f8d74ea481" memberName="button_colour_border"
              virtualName="" explicitFocusOrder="0" pos="660 70 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3891f5f1ede2a913" memberName="button_colour_buttons_on"
              virtualName="" explicitFocusOrder="0" pos="780 30 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2bde73aa342c2457" memberName="button_colour_slider_1"
              virtualName="" explicitFocusOrder="0" pos="900 30 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8ddcc2620647afb5" memberName="button_colour_slider_2"
              virtualName="" explicitFocusOrder="0" pos="900 70 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="27311d6a38d6510d" memberName="button_colour_slider_mod"
              virtualName="" explicitFocusOrder="0" pos="900 110 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="af67a383712e7be7" memberName="label_colour_3" virtualName=""
         explicitFocusOrder="0" pos="690 110 90 33" tooltip="Click to change this colour."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="SIGNAL LINES"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="a907651961c48243" memberName="button_colour_signal_lines"
              virtualName="" explicitFocusOrder="0" pos="660 110 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="aa8131218b1a4e24" memberName="button_colour_buttons_off"
              virtualName="" explicitFocusOrder="0" pos="780 70 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="16cb69a9f61a0c7f" memberName="button_colour_labels"
              virtualName="" explicitFocusOrder="0" pos="780 110 33 30" tooltip="Click to change this colour."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="DL" id="668d26bcc5860c72" memberName="label_ui_headline_6"
         virtualName="" explicitFocusOrder="0" pos="1210 -4 170 35" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="AUDIO &amp; CPU"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e0756600ee1aa0c2" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="1140 70 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DEVICE:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="efb590c00df9d613" memberName="combo_audio_device"
            virtualName="" explicitFocusOrder="0" pos="1210 70 80 30" tooltip="Select an audio device you like to use for the audio playback."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="49f12ab9e3d54910" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="1140 110 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RATE:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="9e7b39300fbdcd67" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="1290 30 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BLOCK:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="db95d5d8a64a8ebc" memberName="combo_sample_rate"
            virtualName="" explicitFocusOrder="0" pos="1210 110 80 30" tooltip="Select the sample rate you like to use for the audio playback.&#10;&#10;Note: the quality of larger sample rates is better, but needs more CPU power."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="" id="d76df912445a2ff8" memberName="combo_block_size" virtualName=""
            explicitFocusOrder="0" pos="1360 30 80 30" tooltip="Select the block size you like to use for the audio playback.&#10;&#10;Note: smaller block sizes are more in time, but needs more CPU power."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="2c9d694778c498dc" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="1140 30 60 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DRIVER:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="f91daaa7098deafb" memberName="combo_audio_driver"
            virtualName="" explicitFocusOrder="0" pos="1210 30 80 30" tooltip="Select an audio driver you like to use for the audio playback."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="bd0d585ced4d3b09" memberName="button_preset_1"
              virtualName="" explicitFocusOrder="0" pos="1070 30 43 30" tooltip="Click to load this preset."
              buttonText="PRE1" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7a187799895dfa50" memberName="button_preset_2"
              virtualName="" explicitFocusOrder="0" pos="1070 70 43 30" tooltip="Click to load this preset."
              buttonText="PRE2" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="202c6d241354941d" memberName="button_preset_3"
              virtualName="" explicitFocusOrder="0" pos="1070 110 43 30" tooltip="Click to load this preset."
              buttonText="PRE3" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: vst_logo_100x_png, 7586, "../Images/vst_logo_100x.png"
static const unsigned char resource_Monique_Ui_GlobalSettings_vst_logo_100x_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,100,0,0,0,58,8,6,0,0,0,70,102,252,211,0,0,0,6,98,75,71,68,0,255,
0,255,0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,221,12,17,8,40,18,239,214,246,240,0,0,29,47,73,68,65,84,120,218,237,124,119,120,92,197,189,246,
59,115,230,148,45,146,44,89,178,44,185,91,198,116,12,152,216,198,190,198,38,166,36,132,123,115,129,16,18,146,56,4,18,46,229,33,95,42,45,16,146,75,62,190,75,117,128,0,54,6,28,122,18,115,109,99,140,59,198,
13,87,108,185,75,178,181,234,171,94,183,158,58,243,253,113,118,87,171,230,142,125,239,243,100,30,157,103,143,118,247,204,153,153,119,126,191,223,251,123,103,246,16,0,184,230,154,111,228,15,206,201,249,
77,126,126,254,183,101,89,206,194,73,21,1,128,156,129,107,78,229,186,179,81,63,1,192,251,173,143,115,30,107,107,111,223,84,93,89,245,220,134,77,235,246,1,0,155,53,235,250,194,9,19,38,172,159,114,229,148,
115,98,177,24,162,145,40,254,87,23,146,24,207,255,5,245,50,153,97,208,160,65,163,43,43,42,111,80,53,237,174,213,107,150,127,204,242,135,12,121,252,202,105,83,207,89,244,209,71,216,184,97,99,151,105,89,
135,0,232,221,136,138,180,214,144,4,222,98,192,249,211,61,183,72,226,85,184,253,32,36,209,161,180,217,71,0,8,49,96,175,251,206,173,244,54,245,30,47,146,120,183,191,250,250,27,205,163,125,54,16,34,253,
89,79,127,117,144,94,223,19,253,214,35,73,18,174,152,120,69,230,173,223,189,117,226,249,23,94,48,183,164,180,180,133,21,20,22,220,210,217,209,129,117,235,62,239,224,156,255,95,33,196,7,156,115,3,160,73,
195,234,117,3,2,2,14,126,76,3,165,9,40,92,240,4,165,238,7,224,112,235,22,199,4,36,189,190,163,1,226,94,149,222,222,100,161,105,247,227,167,8,72,239,241,72,7,164,119,253,36,237,125,244,243,153,219,43,93,
215,81,92,92,60,98,242,149,147,95,203,203,205,189,50,43,43,243,167,76,85,181,204,174,174,46,152,166,117,128,201,108,65,75,115,125,59,254,89,206,88,201,244,103,218,18,149,42,37,38,93,41,132,184,129,118,
251,20,97,56,182,173,255,115,136,206,108,161,52,101,70,16,66,248,105,183,37,17,16,73,34,255,211,59,32,132,56,233,235,250,187,182,188,190,181,207,123,14,231,103,18,146,158,129,62,173,197,192,25,109,200,
177,75,75,71,8,121,217,153,169,1,109,237,8,123,55,23,31,212,86,127,177,75,27,62,52,119,140,79,83,199,128,144,2,195,180,178,108,219,241,130,64,161,148,152,170,172,152,154,34,119,113,33,90,109,199,9,70,
98,122,101,113,105,69,123,94,118,150,37,132,48,1,152,0,44,66,136,24,87,152,139,93,71,106,207,203,207,206,252,57,161,52,171,48,219,255,10,33,100,203,153,235,37,239,17,29,217,87,195,17,79,221,10,222,250,
120,45,242,178,51,33,132,192,214,125,165,231,174,221,186,231,98,8,126,125,36,30,159,110,90,246,185,117,77,45,8,133,163,232,232,10,193,208,13,216,182,13,193,57,64,8,40,165,80,20,5,217,131,50,145,55,56,
27,217,89,25,240,104,154,19,211,245,146,182,174,112,137,44,75,165,66,144,178,61,135,171,131,150,195,149,142,112,244,137,80,220,152,42,203,50,10,179,253,171,0,108,57,227,125,238,6,132,124,165,244,253,68,
203,202,45,197,32,196,109,211,214,253,165,99,23,175,219,250,3,195,48,191,23,141,197,47,232,232,10,131,16,32,174,235,168,12,84,161,174,38,136,166,166,102,132,58,67,136,199,117,112,199,1,8,129,44,203,240,
122,61,200,204,202,196,224,188,193,200,31,58,4,35,71,143,148,134,230,231,93,68,37,233,34,191,207,139,65,153,126,120,53,13,142,0,108,1,200,92,128,18,10,0,206,217,74,159,122,186,172,255,1,229,237,101,235,
241,141,169,151,1,0,22,173,219,250,205,195,149,193,199,116,221,152,26,139,199,97,89,22,36,73,66,125,125,35,214,174,90,135,192,225,10,68,90,219,183,67,136,18,16,210,0,66,66,137,193,84,1,100,1,40,0,196,
57,32,116,130,150,225,87,243,134,230,225,156,115,199,225,170,171,255,5,138,60,20,141,45,237,80,20,5,170,170,66,83,85,151,168,83,114,22,128,32,3,196,144,179,108,33,115,222,89,130,31,223,56,19,0,240,215,
165,235,190,211,222,21,153,99,232,250,112,211,52,97,89,150,235,109,29,7,219,55,111,199,222,109,187,230,66,224,13,168,74,35,128,48,0,3,128,5,64,128,131,130,66,6,160,1,240,65,32,91,143,197,38,213,6,170,
238,11,214,4,39,22,239,220,131,153,215,94,133,111,220,112,29,28,219,134,69,41,40,37,32,137,215,51,53,12,66,8,16,66,82,41,182,232,15,16,89,146,78,251,141,35,186,1,159,170,164,220,80,73,93,11,198,23,230,
66,234,53,27,127,57,251,223,1,0,243,23,173,158,110,217,246,75,134,97,20,152,134,1,211,178,92,194,65,8,84,73,138,93,123,245,180,7,54,175,120,239,109,0,14,172,126,111,233,36,14,29,64,39,128,32,124,69,135,
0,242,14,183,237,235,99,209,216,43,27,63,219,60,170,173,185,13,223,251,225,173,32,212,181,12,74,41,168,68,1,192,43,132,80,19,150,246,85,121,167,48,33,132,167,187,170,62,46,75,0,104,106,10,90,7,42,155,
46,81,25,185,69,162,68,3,33,32,32,160,132,0,4,32,132,192,61,37,137,243,196,255,105,175,52,241,62,165,132,169,178,244,185,87,85,150,165,154,226,43,194,249,195,243,0,0,109,225,216,12,153,177,27,210,39,141,
110,152,90,77,67,211,245,142,195,11,28,199,129,109,219,169,15,163,177,56,186,194,97,231,150,111,206,236,124,226,193,123,157,244,89,118,28,17,147,35,86,206,1,124,218,208,162,191,89,211,208,132,35,129,106,
236,47,57,130,172,236,44,228,228,228,64,146,28,200,182,131,15,150,173,159,95,86,213,56,127,112,238,96,12,202,204,64,118,150,31,170,44,67,86,36,40,178,12,153,73,238,33,75,80,152,4,16,146,162,211,233,109,
17,3,88,69,134,166,64,97,210,249,0,74,211,191,215,199,66,136,203,191,242,126,249,251,57,191,42,24,62,236,199,170,166,193,163,121,224,207,244,129,73,18,36,201,109,72,242,85,102,12,146,68,193,18,13,100,
76,130,17,143,195,113,108,16,2,248,61,10,70,14,205,141,3,232,6,36,26,72,220,108,216,240,121,239,46,250,195,69,151,92,56,147,128,128,201,12,25,126,63,100,69,6,97,26,136,76,32,3,16,150,13,219,118,64,32,
224,165,12,213,193,134,140,234,96,227,125,66,136,35,0,14,38,103,217,49,193,137,5,146,121,241,226,80,36,154,15,16,196,77,11,237,157,33,212,4,27,240,181,137,151,130,49,9,182,109,99,223,129,50,172,221,82,
140,236,156,28,100,101,102,224,138,43,46,69,222,224,28,168,50,131,170,42,80,85,6,143,34,67,211,100,168,50,131,34,203,216,185,115,55,28,33,144,153,145,1,69,149,161,170,42,124,94,47,50,50,51,92,145,36,145,
78,136,132,90,228,85,228,62,180,55,21,67,72,55,34,22,245,141,153,177,102,201,162,127,215,60,26,124,25,25,24,146,63,20,23,79,188,28,231,95,124,49,152,68,65,9,129,148,244,185,128,107,234,9,139,241,123,20,
188,252,236,28,52,6,235,17,141,197,96,234,70,83,76,55,214,247,59,88,94,109,230,31,30,123,106,102,70,134,31,62,191,15,131,115,115,48,105,202,36,220,125,207,143,193,29,14,206,57,170,131,77,40,175,172,67,
56,28,129,202,8,188,170,4,219,177,197,145,234,250,89,170,166,45,202,205,206,122,61,22,55,190,240,104,74,25,33,164,173,199,236,28,16,28,117,180,224,2,67,6,103,99,244,136,66,132,34,81,124,242,233,26,236,
221,119,0,211,166,77,1,149,36,28,62,176,223,222,245,201,146,40,64,76,248,252,254,145,133,143,122,242,6,231,36,124,138,72,252,9,112,238,14,112,150,223,139,63,62,254,167,182,96,89,137,4,40,94,54,104,144,
82,48,172,0,231,157,119,14,166,76,155,140,203,39,78,192,152,49,163,96,26,102,202,14,142,150,220,166,199,16,38,73,98,23,55,237,95,234,102,104,184,222,217,89,208,86,83,51,185,188,180,228,114,253,230,155,
49,249,95,166,117,103,188,92,64,208,228,43,224,8,14,191,166,32,83,161,235,119,28,60,180,1,178,172,3,104,1,23,37,189,77,57,81,182,153,177,216,195,109,225,176,220,38,68,86,77,137,24,121,96,255,161,91,239,
189,255,46,2,97,163,170,166,30,91,119,236,133,105,153,80,152,4,213,175,129,11,10,77,83,137,3,160,178,190,121,156,45,240,140,205,69,131,223,235,221,17,138,25,187,124,170,188,157,82,178,131,16,210,57,32,
56,132,217,137,142,162,112,72,46,150,175,92,135,93,91,118,160,120,27,197,145,146,35,240,120,52,196,195,161,59,33,43,49,16,98,105,62,223,207,1,204,226,66,128,115,14,206,5,184,16,224,194,173,151,11,1,195,
52,241,245,25,211,30,121,183,188,166,21,170,156,101,199,245,220,218,178,242,209,181,135,202,174,88,179,116,197,228,75,38,95,129,31,223,249,3,92,123,221,213,136,199,226,125,84,242,62,44,43,133,149,32,210,
96,45,90,221,24,106,41,75,248,123,21,192,24,43,22,123,100,221,242,229,179,243,242,243,49,254,252,243,18,18,132,59,59,68,26,218,205,157,81,188,242,234,179,133,227,63,121,251,73,66,136,147,30,55,82,174,
202,55,206,157,37,209,64,57,128,167,1,160,232,252,235,72,160,100,181,118,219,15,238,250,87,137,16,79,56,22,71,117,117,13,178,252,10,84,213,235,106,163,137,184,228,209,60,200,244,251,145,149,145,1,66,36,
24,182,40,208,184,248,182,3,124,91,119,68,131,36,16,52,29,126,136,81,178,158,0,27,8,33,21,61,38,131,232,6,105,207,222,131,120,229,165,249,229,225,166,230,87,33,43,215,55,215,212,141,131,16,33,216,117,
239,118,235,76,57,55,91,182,13,193,147,22,225,202,47,46,56,9,128,184,131,119,230,61,189,230,221,215,159,169,66,44,113,161,167,72,129,130,66,64,92,177,111,231,238,103,158,105,108,26,163,40,10,174,186,106,
106,50,214,144,222,177,166,159,160,46,96,243,52,154,21,13,24,0,74,225,29,251,68,164,173,109,244,186,21,43,174,202,203,207,71,78,78,118,66,141,228,16,130,186,128,8,1,203,118,176,191,178,121,252,136,33,
217,111,2,184,195,182,29,48,38,117,131,1,0,209,114,164,3,85,29,41,199,40,66,196,77,63,125,232,154,187,239,190,195,211,21,10,97,239,222,253,176,45,3,30,77,238,49,115,100,198,224,245,104,240,122,53,248,
125,30,100,102,120,49,200,239,135,223,171,129,49,6,2,20,112,160,64,8,76,180,108,126,179,105,218,177,142,112,172,65,83,216,42,77,145,63,33,132,108,6,98,146,16,2,150,101,227,133,57,243,142,132,27,246,220,
160,101,157,87,161,91,214,2,48,73,1,192,209,205,35,100,206,57,149,18,65,155,115,14,39,1,130,224,0,23,194,85,6,220,17,117,27,235,45,114,227,85,60,96,2,168,2,80,5,95,209,142,166,186,134,101,239,253,245,
131,139,47,190,228,66,20,230,14,2,239,103,125,68,244,171,108,245,27,16,43,170,192,216,147,149,251,246,181,238,252,226,11,152,166,153,146,189,82,150,226,154,10,116,211,196,214,146,154,31,9,33,190,195,152,
116,116,33,48,26,192,40,119,162,168,179,103,127,119,158,199,163,225,208,129,82,252,234,255,60,140,61,197,251,64,41,237,209,100,69,113,131,165,71,85,225,245,104,240,123,52,100,248,60,144,93,48,186,253,
187,0,33,160,126,198,216,144,176,97,77,40,171,107,254,237,206,146,138,77,165,213,245,45,243,223,91,112,81,118,150,15,109,237,237,198,150,53,31,222,15,160,92,239,42,229,136,5,58,17,13,52,35,26,104,133,
175,40,157,22,193,180,109,112,206,83,214,33,4,119,221,22,23,112,146,66,121,31,242,144,240,12,46,9,175,129,109,205,46,47,175,16,197,187,246,66,98,236,168,185,14,77,247,101,188,55,62,201,74,163,129,181,
80,148,151,63,251,248,99,167,186,178,50,173,113,34,205,109,9,8,16,52,119,132,233,161,154,230,23,133,16,5,132,16,232,166,221,231,166,35,46,191,62,117,254,210,251,159,188,54,172,96,232,208,72,36,34,254,
242,226,107,7,205,246,174,21,127,127,119,33,86,175,90,135,72,36,234,198,0,74,33,203,50,152,204,92,96,20,5,154,170,186,22,216,71,124,112,139,68,41,60,170,10,85,150,137,97,217,56,20,168,201,37,30,175,252,
238,226,85,168,109,108,146,15,150,87,63,222,218,222,117,71,93,99,203,80,33,132,146,194,32,82,222,167,30,158,136,23,156,195,181,16,193,83,177,212,225,98,192,9,7,0,208,3,128,162,84,180,183,182,191,247,135,
199,254,132,201,83,174,63,114,201,165,179,34,3,177,44,154,206,156,73,111,25,39,26,72,129,66,132,244,60,132,88,253,183,183,22,160,171,163,195,93,146,5,79,129,226,6,58,215,132,143,212,182,20,52,118,132,
95,20,66,72,154,194,250,88,74,237,238,85,0,128,95,60,249,242,191,93,58,225,194,239,72,18,37,11,255,190,168,109,223,150,165,223,149,253,25,119,8,206,23,174,94,248,137,254,218,75,111,96,231,246,98,52,55,
183,192,113,56,52,69,5,99,50,152,68,33,39,193,32,3,167,95,178,68,33,73,52,65,213,25,36,73,130,105,89,216,87,26,160,75,63,251,98,250,146,207,54,47,8,212,212,31,58,88,94,245,68,76,215,39,244,38,32,18,99,
112,82,214,1,112,225,186,44,135,39,44,196,17,189,86,60,251,47,18,99,97,33,196,7,93,109,29,27,90,131,13,171,66,173,173,177,129,38,18,237,95,226,234,5,138,183,8,34,126,36,74,100,249,193,112,115,115,237,
194,247,223,135,109,59,16,130,116,179,174,132,239,18,0,76,219,33,165,181,45,223,138,196,141,159,38,59,153,4,101,246,175,158,4,0,204,250,222,125,133,51,175,154,250,104,134,223,159,177,125,235,14,188,250,
252,107,79,1,56,100,133,74,155,193,201,108,120,180,159,183,4,235,63,251,104,193,7,206,155,115,223,198,71,255,88,130,141,155,182,160,46,88,15,198,100,104,154,2,89,26,0,15,146,242,96,169,5,83,145,150,7,
184,121,21,69,107,71,8,155,118,237,207,222,182,167,228,209,29,251,15,47,171,107,106,251,141,16,194,219,71,32,231,233,44,139,167,24,150,72,48,174,163,22,95,17,156,208,97,129,104,96,37,244,202,153,48,170,
31,128,81,221,222,195,53,246,199,178,4,8,40,142,146,88,249,138,32,34,229,7,224,43,250,117,89,241,158,127,172,252,228,19,220,122,251,247,97,91,38,4,186,153,23,79,80,226,206,176,238,173,104,236,248,181,
16,98,43,33,100,95,56,106,32,115,212,36,188,243,194,227,0,32,125,255,182,155,239,29,50,36,111,82,107,91,43,94,249,203,27,43,97,86,207,73,11,140,58,128,249,240,21,125,10,198,166,182,53,52,221,176,118,233,
202,27,55,125,254,69,222,152,177,163,112,193,5,227,49,125,218,36,204,152,62,5,151,157,63,54,101,248,22,119,45,53,233,161,109,199,73,29,238,76,119,253,127,55,139,34,80,40,67,123,87,24,251,15,87,14,15,199,
140,103,29,46,46,21,66,252,140,16,98,128,32,21,200,83,64,36,172,35,89,151,16,199,88,67,74,39,53,253,190,47,6,22,23,143,75,87,139,6,22,194,87,244,236,23,159,173,251,237,232,177,99,48,121,234,149,112,108,
59,1,72,50,174,8,216,142,131,134,182,208,57,217,126,207,67,66,136,123,8,33,225,100,21,79,205,251,240,202,194,130,252,251,179,6,101,144,57,207,190,20,57,184,247,224,189,189,3,227,189,191,248,61,94,251,
243,127,214,3,248,8,222,162,213,96,120,198,136,197,175,40,221,119,232,214,210,3,165,55,174,90,245,57,29,156,147,141,161,249,121,152,58,109,18,174,157,53,29,51,166,79,130,143,81,8,0,113,75,32,166,155,48,
76,11,166,101,193,178,44,88,166,213,47,209,144,36,10,74,41,154,219,59,177,175,188,250,246,204,12,111,4,192,61,130,11,55,182,10,14,206,105,143,224,206,57,18,116,248,84,181,72,122,52,64,232,209,145,78,210,
58,224,119,122,52,58,99,197,162,37,147,70,142,30,141,161,67,243,221,25,3,10,41,109,35,73,84,183,80,223,22,190,221,239,81,86,10,33,222,39,132,240,7,159,126,61,187,176,96,232,227,35,134,23,102,47,249,239,
101,56,112,160,228,62,196,42,170,170,155,59,10,114,124,218,125,126,175,246,37,128,237,132,144,198,52,144,66,0,66,240,21,29,134,68,63,2,132,55,26,138,204,140,134,35,55,213,84,215,125,107,119,241,190,236,
185,115,255,10,143,215,131,41,147,39,226,134,111,204,194,140,171,167,34,35,35,3,134,105,67,215,45,24,134,11,78,191,67,66,40,40,113,99,77,52,110,144,3,129,218,111,233,145,200,213,222,225,95,51,45,219,114,
7,61,97,121,233,73,162,35,248,105,144,134,197,41,172,135,36,105,93,52,96,193,59,246,246,250,234,234,125,203,23,47,241,254,240,167,63,129,166,40,110,12,73,203,228,9,1,26,218,67,24,156,229,153,175,202,108,
35,180,113,181,94,159,231,214,113,227,198,94,23,172,171,199,250,207,54,46,41,217,241,233,82,192,235,127,232,247,207,252,241,218,235,191,254,51,34,56,46,44,26,185,92,8,113,39,33,164,169,185,171,11,67,178,
178,146,247,229,240,21,233,136,86,232,0,22,37,14,216,190,162,9,182,237,220,172,235,198,77,43,62,93,123,206,138,101,171,25,132,144,198,94,48,158,124,253,235,87,225,178,203,39,64,98,18,76,203,236,27,110,
8,1,149,40,40,147,92,197,151,82,196,13,59,183,190,51,50,157,27,166,37,81,6,33,220,28,196,17,220,181,10,145,100,90,20,142,195,79,11,32,253,152,132,0,112,156,149,251,138,64,40,173,128,16,247,124,185,101,
43,182,109,218,146,162,134,73,214,5,184,231,148,82,28,9,182,169,49,195,122,251,169,57,127,24,127,225,5,231,189,204,152,132,21,203,215,52,175,89,245,217,127,1,232,130,146,123,105,105,89,249,207,52,175,
71,132,99,113,52,180,180,127,179,186,190,105,50,128,110,48,250,243,201,221,180,124,47,162,129,39,16,13,92,2,74,70,65,146,238,2,99,11,43,74,142,84,189,49,103,110,199,253,119,62,32,230,60,247,10,2,129,74,
56,142,211,11,16,215,50,164,4,24,174,20,79,148,104,220,204,130,224,194,118,236,20,181,77,198,143,20,211,74,202,21,167,97,173,240,248,19,195,1,220,151,136,148,11,42,201,139,156,104,244,141,149,31,47,69,
77,85,117,138,217,136,148,222,227,158,59,92,160,188,190,109,198,196,203,47,93,89,80,48,84,217,83,188,143,175,93,189,238,101,196,42,182,39,90,227,72,148,66,162,18,81,20,5,117,77,173,164,185,173,243,81,
33,68,193,81,197,184,116,112,188,69,64,70,17,18,9,222,59,136,6,110,3,149,206,135,166,221,5,73,122,163,108,207,254,210,185,47,204,195,166,13,91,65,210,200,62,75,208,98,150,80,178,37,198,64,168,20,19,16,
53,144,220,85,27,193,5,28,39,65,10,146,217,186,112,51,246,147,221,5,115,116,218,43,144,200,43,78,96,9,211,91,4,30,61,18,37,154,246,116,91,117,117,241,146,191,47,132,97,24,61,181,174,132,197,16,0,157,17,
29,76,243,142,10,133,194,88,190,108,213,230,131,219,150,205,235,209,44,74,18,50,62,131,35,4,14,150,87,79,174,12,54,254,73,8,65,211,105,243,81,221,105,56,208,251,61,29,177,192,98,196,42,238,134,170,222,
2,130,5,203,23,127,138,198,198,102,80,74,33,81,234,230,53,137,28,69,98,12,140,49,24,166,217,241,246,130,15,119,200,154,198,146,121,136,27,208,19,253,226,73,41,69,244,96,109,167,207,101,157,194,14,7,17,
41,47,135,166,253,241,224,206,47,59,151,45,94,10,137,177,180,61,80,137,220,36,177,167,151,115,142,141,159,111,12,45,125,247,221,39,1,180,228,142,155,220,77,186,9,129,36,49,200,140,65,85,85,116,132,34,
216,252,229,254,59,91,218,59,223,232,157,203,156,80,73,186,181,72,249,33,166,40,247,192,118,118,212,215,54,184,107,57,137,236,159,201,44,5,6,165,20,243,94,121,179,249,249,167,30,254,146,49,73,6,72,66,
183,18,238,210,64,202,53,35,21,79,78,255,46,173,212,82,178,56,177,0,159,236,108,172,226,99,72,236,205,21,75,62,118,118,127,185,187,7,40,73,238,46,0,180,181,180,138,151,95,120,229,109,160,125,45,0,180,
166,109,82,35,132,66,86,220,193,145,101,25,154,166,161,173,43,130,69,107,54,255,184,188,58,184,81,8,81,148,204,162,79,8,152,132,91,187,245,39,191,132,221,85,102,130,73,235,199,141,31,11,66,92,64,100,217,
181,16,69,81,224,241,104,120,230,255,205,225,31,47,92,242,98,98,223,22,113,92,126,11,14,145,82,35,146,66,163,211,157,131,144,211,238,178,200,241,135,244,126,165,21,196,43,126,3,206,247,206,255,243,203,
232,232,232,76,204,232,36,5,230,144,101,25,207,60,245,92,77,180,121,255,47,122,40,163,105,73,154,44,203,80,152,2,69,150,19,98,162,2,221,48,233,210,117,91,166,175,221,178,107,87,36,22,127,82,8,145,117,
34,192,36,191,179,112,129,155,119,254,238,79,143,204,202,201,201,118,239,149,0,68,83,53,24,134,137,95,255,252,33,108,88,253,249,155,208,171,222,5,192,252,153,89,80,100,57,33,149,240,148,155,18,232,86,
85,163,113,19,0,166,15,116,223,19,45,61,214,212,233,73,6,120,248,198,185,210,58,33,55,69,35,145,131,175,190,240,103,255,239,158,124,2,0,129,128,0,99,50,150,44,92,132,221,235,215,220,14,128,247,88,35,73,
229,2,46,32,76,97,224,9,129,154,184,11,245,176,76,19,187,15,29,201,58,92,85,247,216,5,227,70,255,170,182,161,249,111,148,210,247,138,15,29,217,87,215,212,106,14,27,50,216,78,204,39,142,52,197,36,18,211,
165,47,15,28,150,182,236,62,168,90,182,61,243,80,160,250,89,195,178,199,200,138,12,85,81,33,203,174,106,190,109,219,78,204,127,245,45,206,168,180,80,196,42,30,62,16,8,94,214,208,210,254,181,207,191,44,
189,194,226,72,37,132,73,23,156,100,89,66,0,49,195,68,89,93,203,107,113,211,202,214,77,123,157,97,59,45,113,211,54,117,203,137,8,33,162,199,181,230,223,127,98,120,10,86,23,45,79,46,68,213,192,87,116,119,
217,129,67,31,44,254,199,127,227,187,183,223,6,69,81,80,81,30,192,187,11,222,127,30,232,218,210,31,24,16,0,165,20,170,170,64,211,52,56,182,131,214,150,6,132,67,97,112,33,32,81,10,89,102,136,234,6,118,
30,40,243,238,62,120,228,206,194,252,220,59,115,179,51,141,152,97,22,55,183,117,28,150,40,109,224,92,116,113,193,173,196,250,132,63,26,55,242,195,209,216,184,80,36,246,181,246,206,144,23,132,128,201,50,
12,195,68,103,71,23,234,106,235,177,110,237,6,81,182,107,79,32,123,196,200,13,207,188,244,220,93,127,249,199,186,219,66,49,29,173,157,97,88,54,135,87,83,82,132,71,128,0,84,2,165,46,35,163,148,66,102,12,
85,77,33,185,53,98,60,59,56,211,11,85,97,0,33,8,199,226,127,230,140,60,152,216,158,116,220,65,157,29,83,92,60,81,247,21,13,124,232,104,163,167,172,249,116,197,207,243,242,243,80,80,48,20,159,46,90,90,
108,196,245,63,14,168,237,16,128,59,28,122,76,135,169,27,216,190,117,39,222,127,231,111,104,106,108,134,230,209,48,104,80,22,242,135,14,65,222,144,92,12,202,206,130,215,235,69,153,166,66,85,21,85,85,213,
41,178,44,79,81,21,5,138,34,131,0,176,28,27,186,97,194,182,236,148,100,98,154,38,98,145,56,90,219,219,209,24,108,64,85,101,13,58,107,131,197,160,100,53,128,133,150,105,204,127,117,238,91,32,32,137,188,
196,29,11,198,36,48,201,61,100,89,134,170,200,174,122,44,75,144,21,217,221,228,33,73,80,85,37,165,66,75,146,4,85,66,185,143,157,120,20,72,219,117,34,122,47,183,156,2,85,144,158,236,106,239,152,240,215,
185,111,204,208,52,15,239,104,109,251,45,98,129,112,191,214,145,232,117,201,129,18,60,250,224,19,176,44,11,77,141,45,159,55,4,170,222,129,204,172,152,97,100,199,58,58,207,169,15,84,157,15,33,198,67,102,
163,84,159,23,154,71,131,170,170,80,20,25,76,150,33,81,154,98,98,174,84,238,192,177,29,88,150,5,67,55,161,235,58,226,225,72,35,76,243,16,64,138,65,233,151,240,122,138,17,171,40,163,25,227,125,145,206,
206,208,238,213,171,74,250,9,66,125,207,19,251,196,64,9,32,72,82,16,75,202,201,46,162,241,240,38,160,213,57,209,160,126,122,183,146,38,6,219,59,216,223,26,107,11,223,21,15,133,175,142,183,119,86,193,182,
55,29,85,249,164,180,178,163,181,237,31,219,214,55,57,0,186,192,249,27,112,130,187,82,203,51,190,34,47,128,76,0,153,224,194,103,68,99,121,70,36,54,60,177,93,52,7,64,70,98,167,162,4,119,103,187,9,32,10,
129,78,0,45,128,168,5,72,19,36,26,134,199,211,5,160,3,209,128,158,92,174,229,220,137,129,144,217,80,213,19,27,143,126,193,74,164,244,138,183,14,230,137,231,33,167,127,43,169,175,8,177,218,189,128,175,
40,0,73,170,132,36,9,216,181,162,55,171,234,85,234,65,233,29,80,148,148,146,142,104,26,19,139,6,98,0,98,0,92,193,113,112,17,160,131,38,0,32,105,71,122,87,68,74,15,18,224,136,5,250,118,175,123,211,133,
0,80,115,218,247,40,14,228,17,142,7,144,147,102,89,3,201,25,238,43,239,119,189,185,239,53,2,64,124,224,13,110,189,74,91,170,238,83,83,246,162,229,248,74,203,49,193,232,171,94,209,94,139,108,255,44,103,
184,244,54,109,250,207,33,57,187,165,55,149,162,233,46,235,76,255,58,162,30,99,79,71,29,110,179,231,78,58,249,230,255,235,208,179,108,35,95,21,203,58,65,48,10,81,145,60,255,133,128,49,75,32,226,28,255,
204,34,52,3,131,143,12,66,224,215,195,134,141,34,193,123,118,8,58,111,202,245,220,138,253,4,86,220,115,76,15,76,64,16,33,187,178,31,59,252,116,199,39,141,6,110,46,4,22,213,159,21,27,57,235,128,244,2,227,
47,128,184,67,251,246,53,62,255,35,255,1,97,218,199,142,102,132,64,18,54,62,154,249,131,48,70,142,94,18,172,170,218,4,0,60,222,62,246,71,23,127,239,223,126,56,238,122,15,163,236,168,115,50,230,24,152,
191,119,209,141,31,223,27,31,135,165,245,15,96,81,125,215,217,177,143,158,187,123,217,217,2,163,30,99,51,0,188,9,224,22,128,80,227,147,205,48,150,111,29,112,0,105,47,140,100,16,4,169,237,247,113,117,124,
20,112,243,28,109,16,127,191,124,181,243,97,96,77,159,77,204,253,249,110,46,4,197,69,249,63,130,164,141,68,48,122,27,22,215,55,97,214,96,224,179,182,51,106,31,226,108,0,18,196,24,12,67,101,18,140,177,
0,230,2,184,54,245,5,46,0,110,247,11,198,167,36,34,94,202,226,33,143,16,238,15,117,40,97,2,160,81,10,225,101,16,105,143,203,145,184,224,56,161,141,32,18,5,38,228,204,0,35,219,240,189,225,223,196,223,234,
74,1,0,215,229,1,171,91,206,176,195,2,24,231,220,97,50,3,161,84,196,227,198,87,246,67,245,97,168,76,90,200,149,0,94,5,112,233,241,94,43,131,24,251,114,50,31,70,69,197,92,0,192,107,147,110,186,98,232,37,
23,103,83,70,242,32,134,129,144,199,133,128,85,220,90,122,101,216,138,42,39,76,79,28,1,92,148,51,26,94,182,11,63,28,117,43,222,171,94,137,213,45,103,228,71,251,132,0,92,112,175,36,36,8,206,9,235,104,107,
223,115,217,196,203,166,157,123,206,120,223,150,173,27,51,251,77,208,78,159,187,250,62,128,231,93,201,227,248,137,157,0,136,228,112,37,21,241,137,115,235,185,89,133,223,247,72,74,247,166,56,193,81,23,
105,68,216,140,158,28,93,180,57,48,38,195,11,31,91,138,255,40,122,8,243,2,115,1,124,229,207,170,58,255,188,243,166,13,202,202,154,90,95,95,143,184,174,135,88,48,24,252,207,250,186,250,85,247,63,112,255,
132,27,110,188,225,77,219,178,155,136,251,227,194,254,237,235,68,59,75,41,172,150,22,92,248,230,98,153,68,113,179,0,188,39,41,45,167,125,95,50,151,84,172,231,132,144,30,155,197,77,110,37,164,242,147,44,
142,0,114,53,9,154,244,95,158,95,95,50,243,22,49,165,233,220,172,49,180,199,179,143,78,229,153,107,105,189,16,66,128,49,166,14,27,54,108,138,16,124,200,166,77,155,161,235,250,114,182,117,251,214,207,52,
143,118,247,240,17,35,94,47,42,42,250,150,170,170,3,175,118,157,76,58,79,41,248,160,44,176,236,76,104,209,8,226,167,73,15,136,218,241,99,114,250,147,118,234,25,140,13,202,205,186,113,180,60,10,5,153,67,
211,151,106,79,94,210,232,231,201,82,66,112,84,84,84,96,195,134,13,8,4,2,173,132,144,183,88,91,91,179,179,117,219,182,5,254,3,7,154,132,16,47,0,40,58,90,91,79,182,203,28,113,108,81,35,120,196,200,128,
115,194,125,34,253,59,223,163,70,201,227,109,109,175,239,17,64,250,178,11,98,85,19,254,174,188,215,231,231,120,167,43,129,118,247,125,8,88,150,5,219,178,182,10,33,126,211,208,80,187,133,13,27,54,10,193,
96,181,13,96,41,128,165,133,133,35,125,132,16,79,234,9,112,164,87,39,123,252,68,76,244,63,80,201,235,56,122,108,63,223,148,169,178,217,45,214,139,185,96,223,149,78,168,91,189,238,99,57,28,186,195,33,146,
55,75,180,75,231,64,150,76,160,72,39,48,108,189,50,229,45,237,60,103,105,116,163,234,243,206,142,198,226,81,2,208,190,63,60,195,0,15,149,235,249,244,189,212,123,61,158,166,215,115,66,113,33,34,13,193,
106,29,0,134,13,27,5,22,12,86,35,249,15,0,4,131,213,209,175,42,152,5,1,68,81,240,67,5,164,197,15,233,62,249,56,29,62,5,80,100,138,140,50,140,207,4,184,252,253,167,91,62,44,107,213,101,75,8,82,79,120,27,
1,40,49,132,29,189,210,119,177,126,203,144,233,66,129,114,98,110,21,128,201,33,175,107,119,114,215,198,151,205,175,40,189,233,198,220,172,51,166,181,22,14,27,133,250,96,53,130,193,234,51,43,95,237,193,
8,92,138,90,124,128,60,101,2,60,191,243,65,250,173,2,226,57,214,117,71,96,10,78,73,211,133,92,105,181,33,168,12,216,30,16,179,19,252,192,112,84,254,36,245,197,191,77,187,29,93,214,171,16,56,254,39,171,
50,2,210,97,66,91,211,110,14,218,105,191,222,208,17,124,32,125,144,206,116,145,206,228,205,230,34,132,221,24,129,235,208,224,16,24,27,39,192,215,73,128,25,50,136,146,190,202,212,251,24,2,70,134,8,201,
111,67,12,33,64,30,7,242,29,144,194,165,114,124,252,167,35,115,183,163,179,211,77,114,174,41,248,26,128,111,224,120,31,139,193,8,72,131,14,239,138,142,112,230,126,231,137,198,182,224,227,73,111,113,54,
192,232,161,246,158,169,114,57,106,177,27,35,240,10,116,126,17,170,255,18,129,51,59,12,39,6,8,200,137,131,165,157,203,16,32,224,32,224,61,62,87,33,208,6,199,235,229,24,222,51,135,20,10,168,112,181,22,
146,124,77,104,47,189,14,90,25,131,127,105,123,75,70,169,115,127,99,75,221,115,73,48,130,103,9,140,179,38,46,94,142,90,236,194,8,76,68,45,46,65,237,162,35,24,17,184,43,35,254,100,136,73,215,16,8,107,0,
234,222,39,20,119,82,9,25,28,122,50,224,177,90,99,189,103,101,251,98,24,60,31,169,199,161,38,226,167,232,67,17,8,179,73,151,22,34,243,26,154,235,86,165,226,220,89,4,3,0,254,63,137,4,77,165,141,226,8,11,
0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* Monique_Ui_GlobalSettings::vst_logo_100x_png = (const char*) resource_Monique_Ui_GlobalSettings_vst_logo_100x_png;
const int Monique_Ui_GlobalSettings::vst_logo_100x_pngSize = 7586;


//[EndFile] You can add extra defines here...
//[/EndFile]
