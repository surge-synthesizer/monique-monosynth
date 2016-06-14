/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

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
    if( not combo_theme->hasKeyboardFocus (false) and combo_theme->isTextEditable() )
    {
        combo_theme->setEditableText(false);
    }

    toggle_animate_input_env->setToggleState( synth_data->animate_envs ,dontSendNotification );
    toggle_show_tooltips->setToggleState( synth_data->show_tooltips ,dontSendNotification );
    toggle_animate_sliders->setToggleState( synth_data->animate_sliders, dontSendNotification );

#ifdef IS_STANDALONE
    label_cpu_usage->setText
    (
        String
        (
            cpu_usage_smoother.add_and_get_average( synth_data->audio_processor->getCpuUsage()*100)
        )
        + String("%")
        ,dontSendNotification
    );
#endif

    // COLOURS
    if( not block_colour_update )
    {
        if( not label->isBeingEdited() )
        {
            label->setText( colour_selector->getCurrentColour().toString(), dontSendNotification );
        }

        bool repaint_buttons = force_repaint;
        force_repaint = false;
        // if( button_current_editor_color->isVisible() )
        {

            if( current_edited_colour )
            {
                *current_edited_colour = colour_selector->getCurrentColour();
            }

            if( last_repainted_colour != *current_edited_colour )
            {
                colour_selector->setColour( ColourSelector::backgroundColourId, editable_theme->area_colour );
                last_repainted_colour = *current_edited_colour;
                if( toggle_for_all->getToggleState() ) // and editable_theme != &look_and_feel->colours.get_theme( COLOUR_THEMES::BG_THEME ) )
                {
                    for( int i = 0 ; i != COLOUR_THEMES::SUM_OF ; ++i )
                    {
                        if( i == BG_THEME )
                        {
                            continue;
                        }

                        SectionTheme& theme = look_and_feel->colours.get_theme( static_cast< COLOUR_THEMES >( i ) );
                        Colour& colour = theme.get_color( current_colour );

                        colour = last_repainted_colour;
                    }
                }

                repaint_buttons = true;
                get_editor()->repaint();
                colour_selector->repaint();
            }
        }
        if( repaint_buttons )
        {
#define COLOUR_BUTTON_REFRESH( button, color_code_ ) \
        button->getProperties().set( VAR_INDEX_COLOUR_THEME, editable_theme->id ); \
        button->getProperties().set( VAR_INDEX_BUTTON_AMP, color_code_ ); \
	button->repaint();

            COLOUR_BUTTON_REFRESH( button_colour_background, COLOUR_CODES::AREA_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_labels, COLOUR_CODES::AREA_FONT_COLOUR );

            COLOUR_BUTTON_REFRESH( button_colour_slider_disabled, COLOUR_CODES::DISABLED_SLIDER_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_slider_bg, COLOUR_CODES::SLIDER_BACKGROUND_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_slider_1, COLOUR_CODES::VALUE_SLIDER_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_slider_2, COLOUR_CODES::VALUE_SLIDER_2_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_slider_mod, COLOUR_CODES::MOD_SLIDER_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_buttons_on, COLOUR_CODES::BUTTON_ON_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_buttons_font_on, COLOUR_CODES::BUTTON_ON_FONT_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_buttons_off, COLOUR_CODES::BUTTON_OFF_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_buttons_font_off, COLOUR_CODES::BUTTON_OFF_FONT_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_buttons_on, COLOUR_CODES::BUTTON_ON_COLOUR );
            COLOUR_BUTTON_REFRESH( button_colour_oszi_1, COLOUR_CODES::OSZI_1 );
            COLOUR_BUTTON_REFRESH( button_colour_oszi_2, COLOUR_CODES::OSZI_2 );
            COLOUR_BUTTON_REFRESH( button_colour_oszi_3, COLOUR_CODES::OSZI_3 );

            button_colour_bg->repaint();
            button_colour_bg_svg_1->repaint();
            button_colour_bg_svg_2->repaint();
            button_colour_bg_svg_3->repaint();
            button_colour_bg_svg_4->repaint();
            button_colour_bg_svg_5->repaint();
            button_colour_bg_svg_6->repaint();
            button_colour_bg_svg_7->repaint();

            label_section__->getProperties().set( VAR_INDEX_COLOUR_THEME, editable_theme->id );
            label_section__->repaint();
            label_slider__->getProperties().set( VAR_INDEX_COLOUR_THEME, editable_theme->id );
            label_slider__->repaint();
            label_buttons__->getProperties().set( VAR_INDEX_COLOUR_THEME, editable_theme->id );
            label_buttons__->repaint();
        }
    }

    // THREADS
    if( combo_multicore_cpus != getCurrentlyFocusedComponent() )
    {
        combo_multicore_cpus->setSelectedId( synth_data->num_extra_threads+1, dontSendNotification );
    }
}

void Monique_Ui_GlobalSettings::handleAsyncUpdate()
{
    update_colour_presets();
    update_audio_devices();
}
void Monique_Ui_GlobalSettings::open_colour_selector( COLOUR_CODES code_ )
{
    current_colour = code_;
    last_repainted_colour = editable_theme->get_color(current_colour);

    colour_selector->setCurrentColour( last_repainted_colour );
    colour_selector->setColour( ColourSelector::backgroundColourId, editable_theme->area_colour );
    current_edited_colour = &editable_theme->get_color(current_colour);

#define RESIZE_SELECTOR( button ) selected_element_marker->setBounds( 1.0f+button->getX(), 1.0f+button->getY(), 0.4f*button_colour_background->getHeight(), 0.4f*button_colour_background->getHeight() );
    switch( code_ )
    {
    case COLOUR_CODES::AREA_COLOUR :
        RESIZE_SELECTOR( button_colour_background ) break;
    case COLOUR_CODES::AREA_FONT_COLOUR :
        RESIZE_SELECTOR( button_colour_labels ) break;
    case COLOUR_CODES::VALUE_SLIDER_COLOUR :
        RESIZE_SELECTOR( button_colour_slider_1 ) break;
    case COLOUR_CODES::VALUE_SLIDER_2_COLOUR :
        RESIZE_SELECTOR( button_colour_slider_2 ) break;
    case COLOUR_CODES::MOD_SLIDER_COLOUR :
        RESIZE_SELECTOR( button_colour_slider_mod ) break;
    case COLOUR_CODES::DISABLED_SLIDER_COLOUR :
        RESIZE_SELECTOR( button_colour_slider_disabled ) break;
    case COLOUR_CODES::SLIDER_BACKGROUND_COLOUR :
        RESIZE_SELECTOR( button_colour_slider_bg ) break;
    case COLOUR_CODES::BUTTON_ON_COLOUR :
        RESIZE_SELECTOR( button_colour_buttons_on ) break;
    case COLOUR_CODES::BUTTON_ON_FONT_COLOUR :
        RESIZE_SELECTOR( button_colour_buttons_font_on ) break;
    case COLOUR_CODES::BUTTON_OFF_COLOUR :
        RESIZE_SELECTOR( button_colour_buttons_off ) break;
    case COLOUR_CODES::BUTTON_OFF_FONT_COLOUR :
        RESIZE_SELECTOR( button_colour_buttons_font_off ) break;
    case COLOUR_CODES::OSZI_1 :
        RESIZE_SELECTOR( button_colour_oszi_1 ) break;
    case COLOUR_CODES::OSZI_2 :
        RESIZE_SELECTOR( button_colour_oszi_2 ) break;
    case COLOUR_CODES::OSZI_3 :
        RESIZE_SELECTOR( button_colour_oszi_3 ) break;
    }
#undef RESIZE_SELECTOR
#define RESIZE_SELECTOR( button ) selected_section_marker->setBounds( 1.0f+button->getX(), 1.0f+button->getY(), 0.4f*button_colour_background->getHeight(), 0.4f*button_colour_background->getHeight() );
    switch( editable_theme->id )
    {
    case COLOUR_THEMES::BG_THEME :
        RESIZE_SELECTOR( button_colour_bg ) break;
    case COLOUR_THEMES::OSC_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_7 ) break;
    case COLOUR_THEMES::FM_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_2 ) break;
    case COLOUR_THEMES::FILTER_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_1 ) break;
    case COLOUR_THEMES::MORPH_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_5 ) break;
    case COLOUR_THEMES::FX_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_3 ) break;
    case COLOUR_THEMES::MASTER_THEME :
        RESIZE_SELECTOR( button_colour_bg_svg_6 ) break;
        // case COLOUR_THEMES::ARP_THEME :
    default :
        RESIZE_SELECTOR( button_colour_bg_svg_4 ) break;
    }
#undef RESIZE_SELECTOR

    colour_selector->repaint();
}

void Monique_Ui_GlobalSettings::update_audio_devices()
{
#ifdef IS_STANDALONE
    combo_audio_device->clear(dontSendNotification);
    combo_sample_rate->clear(dontSendNotification);
    combo_block_size->clear(dontSendNotification);
    combo_audio_driver->clear(dontSendNotification);

    // AUDIO DEVICE MANAGER
    MoniqueAudioProcessor* audio_processor = synth_data->audio_processor;

    // DRIVERS
    const OwnedArray<AudioIODeviceType>& drivers = audio_processor->getAvailableDeviceTypes();
    const String current_audio_driver = audio_processor->getCurrentAudioDeviceType();

    AudioIODeviceType* active_driver = nullptr;
    for( int i = 0 ; i != drivers.size() ; ++i )
    {
        String driver_name = drivers[i]->getTypeName();
        combo_audio_driver->addItem( driver_name , i+1 );
#ifndef JUCE_IOS
        if( driver_name == current_audio_driver )
#endif
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
    }

    if( not active_driver )
    {
        combo_audio_driver->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));

        combo_audio_device->setText( "NO DRIVER", dontSendNotification );
        combo_audio_device->setEnabled(false);
        combo_audio_device->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));

        combo_sample_rate->setText( "NO DRIVER", dontSendNotification );
        combo_sample_rate->setEnabled(false);
        combo_sample_rate->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));

        combo_block_size->setText( "NO DRIVER", dontSendNotification );
        combo_block_size->setEnabled(false);
        combo_block_size->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
    }
    else
    {
        combo_audio_driver->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));

        combo_audio_device->setEnabled(true);
        combo_audio_device->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.5f));
        combo_sample_rate->setEnabled(true);
        combo_sample_rate->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.2f));
        combo_block_size->setEnabled(true);
        combo_block_size->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.2f));
    }

    if( not active_device )
    {
        combo_audio_device->setColour(ComboBox::backgroundColourId, Colours::red.withAlpha(0.5f));

        combo_sample_rate->setText( "NO DEVICE", dontSendNotification );
        combo_sample_rate->setEnabled(false);
        combo_sample_rate->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));

        combo_block_size->setText( "NO DEVICE", dontSendNotification );
        combo_block_size->setEnabled(false);
        combo_block_size->setColour(ComboBox::backgroundColourId, Colours::orange.withAlpha(0.2f));
    }
    else
    {
        combo_audio_device->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.3f));

        combo_sample_rate->setEnabled(true);
        combo_sample_rate->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.2f));
        combo_block_size->setEnabled(true);
        combo_block_size->setColour(ComboBox::backgroundColourId, Colours::green.withAlpha(0.2f));
    }

    if( not active_driver )
    {
        combo_audio_driver->showPopup();
    }
    else if( not active_device )
    {
        combo_audio_device->showPopup();
    }
#endif
}

void Monique_Ui_GlobalSettings::update_colour_presets()
{
    const StringArray& themes = synth_data->get_themes();
    StringArray allowes_themes;

    // FILTER THEMES OUT
    if( not SHARED::getInstance()->status.isUnlocked() )
    {
        for( int i = 0 ; i != themes.size() ; ++i )
        {
            String name = themes[i];
            if( name == "D-BLUC" or name == "DARK" or name == "LIGHT1" or name == "RED" )
            {
                allowes_themes.add(name);
            }
        }
    }
    else
    {
      for( int i = 0 ; i != themes.size() ; ++i )
        {
                allowes_themes.add(themes[i]);
        }
    }

    {
        combo_theme->clear(dontSendNotification);
        combo_theme->addItemList(allowes_themes,1);
        String stored_theme = synth_data->get_current_theme();
        const int index = allowes_themes.indexOf(stored_theme);
        if( index != -1 )
        {
            combo_theme->setSelectedId(index+1,dontSendNotification);
        }
        else
        {
            combo_theme->setText(String("AWAY: ") + stored_theme,dontSendNotification);
        }
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_GlobalSettings::Monique_Ui_GlobalSettings (Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*parent_ )
    : Monique_Ui_Refreshable(ui_refresher_),
      original_w(1465), original_h(180)
{
    //[Constructor_pre] You can add your own custom stuff here..
    editable_theme = &ui_refresher_->look_and_feel->colours.get_theme( COLOUR_THEMES::BG_THEME );
    current_colour = COLOUR_CODES::AREA_COLOUR;
    force_repaint = true;
    colour_clipboard = 0xffffffff;
    block_colour_update = false;
    parent = parent_;
    //[/Constructor_pre]

    addAndMakeVisible (label_ui_headline_9 = new Label ("DL",
                                                        TRANS("VST is a trademark of Steinberg Media Technologies GmbH")));
    label_ui_headline_9->setFont (Font (30.00f, Font::plain));
    label_ui_headline_9->setJustificationType (Justification::centredLeft);
    label_ui_headline_9->setEditable (false, false, false);
    label_ui_headline_9->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_3 = new Label ("DL",
                                                        TRANS("Aus Liebe zur Musik.")));
    label_ui_headline_3->setFont (Font (30.00f, Font::plain));
    label_ui_headline_3->setJustificationType (Justification::centredLeft);
    label_ui_headline_3->setEditable (false, false, false);
    label_ui_headline_3->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_3->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_ui_headline_7 = new Label ("DL",
                                                        TRANS("Thomas Arndt | Monoplugs")));
    label_ui_headline_7->setFont (Font (30.00f, Font::plain));
    label_ui_headline_7->setJustificationType (Justification::centredLeft);
    label_ui_headline_7->setEditable (false, false, false);
    label_ui_headline_7->setColour (Label::textColourId, Colour (0xff1111ff));
    label_ui_headline_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (image_vst = new ImageButton ("new button"));
    image_vst->setButtonText (String());

    image_vst->setImages (false, true, true,
                          ImageCache::getFromMemory (vst_logo_100x_png, vst_logo_100x_pngSize), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000));
    addAndMakeVisible (label_8 = new Label (String(),
                                            TRANS("RATE")));
    label_8->setFont (Font (30.00f, Font::plain));
    label_8->setJustificationType (Justification::centredRight);
    label_8->setEditable (false, false, false);
    label_8->setColour (Label::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_audio_device = new ComboBox (String()));
    combo_audio_device->setTooltip (TRANS("Select an audio device you like to use for the audio playback."));
    combo_audio_device->setEditableText (false);
    combo_audio_device->setJustificationType (Justification::centredLeft);
    combo_audio_device->setTextWhenNothingSelected (String());
    combo_audio_device->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_audio_device->addListener (this);

    addAndMakeVisible (label_7 = new Label (String(),
                                            TRANS("DEVICE")));
    label_7->setFont (Font (30.00f, Font::plain));
    label_7->setJustificationType (Justification::centredRight);
    label_7->setEditable (false, false, false);
    label_7->setColour (Label::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_audio_driver = new ComboBox (String()));
    combo_audio_driver->setTooltip (TRANS("Select an audio driver you like to use for the audio playback."));
    combo_audio_driver->setEditableText (false);
    combo_audio_driver->setJustificationType (Justification::centredLeft);
    combo_audio_driver->setTextWhenNothingSelected (String());
    combo_audio_driver->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_audio_driver->addListener (this);

    addAndMakeVisible (button_colour_bg = new TextButton ("new button"));
    button_colour_bg->setTooltip (TRANS("Click to edit the colours of the background section."));
    button_colour_bg->setButtonText (String());
    button_colour_bg->addListener (this);

    addAndMakeVisible (button_colour_background = new TextButton ("new button"));
    button_colour_background->setTooltip (TRANS("Section BACKGROUND"));
    button_colour_background->setButtonText (TRANS("B"));
    button_colour_background->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_background->addListener (this);

    addAndMakeVisible (label_buttons__ = new Label (String(),
                                                    TRANS("Buttons\n")));
    label_buttons__->setTooltip (TRANS("Click a button on the right to change the colour for this element."));
    label_buttons__->setFont (Font (30.00f, Font::plain));
    label_buttons__->setJustificationType (Justification::centredRight);
    label_buttons__->setEditable (false, false, false);
    label_buttons__->setColour (Label::textColourId, Colour (0xffff3b00));
    label_buttons__->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_buttons__->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_slider__ = new Label (String(),
                                                   TRANS("Sliders")));
    label_slider__->setTooltip (TRANS("Click a button on the right to change the colour for this element."));
    label_slider__->setFont (Font (30.00f, Font::plain));
    label_slider__->setJustificationType (Justification::centredRight);
    label_slider__->setEditable (false, false, false);
    label_slider__->setColour (Label::textColourId, Colour (0xffff3b00));
    label_slider__->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_slider__->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_section__ = new Label (String(),
                                                    TRANS("Section")));
    label_section__->setTooltip (TRANS("Click a button on the right to change the colour for this element."));
    label_section__->setFont (Font (30.00f, Font::plain));
    label_section__->setJustificationType (Justification::centredRight);
    label_section__->setEditable (false, false, false);
    label_section__->setColour (Label::textColourId, Colour (0xffff3b00));
    label_section__->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_section__->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_9 = new Label (String(),
                                            TRANS("BLOCK")));
    label_9->setFont (Font (30.00f, Font::plain));
    label_9->setJustificationType (Justification::centredRight);
    label_9->setEditable (false, false, false);
    label_9->setColour (Label::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_2 = new Label (String(),
                                            TRANS("CPU")));
    label_2->setFont (Font (30.00f, Font::plain));
    label_2->setJustificationType (Justification::centredRight);
    label_2->setEditable (false, false, false);
    label_2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_4 = new Label (String(),
                                            TRANS("THREADS")));
    label_4->setFont (Font (30.00f, Font::plain));
    label_4->setJustificationType (Justification::centredRight);
    label_4->setEditable (false, false, false);
    label_4->setColour (Label::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_multicore_cpus = new ComboBox (String()));
    combo_multicore_cpus->setTooltip (TRANS("Select the threads you like to spend to process Moniqiue. \n"
    "\n"
    "Note: Its recommended to use NOT more threads as your CPU has cores! \n"
    "Please take a look at the CPU usage and decide how many threads are the best for your CPU."));
    combo_multicore_cpus->setEditableText (false);
    combo_multicore_cpus->setJustificationType (Justification::centredLeft);
    combo_multicore_cpus->setTextWhenNothingSelected (String());
    combo_multicore_cpus->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_multicore_cpus->addListener (this);

    addAndMakeVisible (label_cpu_usage = new Label (String(),
                                                    TRANS("20%")));
    label_cpu_usage->setFont (Font (30.00f, Font::plain));
    label_cpu_usage->setJustificationType (Justification::centredLeft);
    label_cpu_usage->setEditable (false, false, false);
    label_cpu_usage->setColour (Label::textColourId, Colour (0xffff3b00));
    label_cpu_usage->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_cpu_usage->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_block_size = new ComboBox (String()));
    combo_block_size->setTooltip (TRANS("Select the block size you like to use for the audio playback.\n"
    "\n"
    "Note: smaller block sizes are more in time, but needs more CPU power."));
    combo_block_size->setEditableText (false);
    combo_block_size->setJustificationType (Justification::centredLeft);
    combo_block_size->setTextWhenNothingSelected (String());
    combo_block_size->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_block_size->addListener (this);

    addAndMakeVisible (label_10 = new Label (String(),
                                             TRANS("DRIVER")));
    label_10->setFont (Font (30.00f, Font::plain));
    label_10->setJustificationType (Justification::centredRight);
    label_10->setEditable (false, false, false);
    label_10->setColour (Label::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_sample_rate = new ComboBox (String()));
    combo_sample_rate->setTooltip (TRANS("Select the sample rate you like to use for the audio playback.\n"
    "\n"
    "Note: the quality of larger sample rates is better, but needs more CPU power."));
    combo_sample_rate->setEditableText (false);
    combo_sample_rate->setJustificationType (Justification::centredLeft);
    combo_sample_rate->setTextWhenNothingSelected (String());
    combo_sample_rate->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    combo_sample_rate->addListener (this);

    addAndMakeVisible (label_16 = new Label (String(),
                                             TRANS("ANI-ENV\'S")));
    label_16->setTooltip (TRANS("Turn amp animations on buttons on or off."));
    label_16->setFont (Font (30.00f, Font::plain));
    label_16->setJustificationType (Justification::centredLeft);
    label_16->setEditable (false, false, false);
    label_16->setColour (Label::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_input_env = new ToggleButton (String()));
    toggle_animate_input_env->setTooltip (TRANS("Turn amp animations on buttons on or off."));
    toggle_animate_input_env->addListener (this);

    addAndMakeVisible (label_18 = new Label (String(),
                                             TRANS("TOOLTIPS")));
    label_18->setTooltip (TRANS("Turn tooltips on or off.\n"
    "\n"
    "Press the \"CTRL\" + \"h\" on your keyboard to show the tooltip if this option is turned off."));
    label_18->setFont (Font (30.00f, Font::plain));
    label_18->setJustificationType (Justification::centredLeft);
    label_18->setEditable (false, false, false);
    label_18->setColour (Label::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_show_tooltips = new ToggleButton (String()));
    toggle_show_tooltips->setTooltip (TRANS("Turn tooltips on or off.\n"
    "\n"
    "Press the \"CTRL\" + \"h\" on your keyboard to show the tooltip if this option is turned off."));
    toggle_show_tooltips->addListener (this);

    addAndMakeVisible (label_ui_headline_2 = new Label (String(),
                                                        TRANS("MISC")));
    label_ui_headline_2->setFont (Font (30.00f, Font::plain));
    label_ui_headline_2->setJustificationType (Justification::centred);
    label_ui_headline_2->setEditable (false, false, false);
    label_ui_headline_2->setColour (Label::textColourId, Colour (0xff050505));
    label_ui_headline_2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_colour_buttons_on = new TextButton ("new button"));
    button_colour_buttons_on->setTooltip (TRANS("Button ON"));
    button_colour_buttons_on->setButtonText (TRANS("O"));
    button_colour_buttons_on->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_buttons_on->addListener (this);

    addAndMakeVisible (button_colour_slider_1 = new TextButton ("new button"));
    button_colour_slider_1->setTooltip (TRANS("FRONT slider"));
    button_colour_slider_1->setButtonText (TRANS("1"));
    button_colour_slider_1->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_slider_1->addListener (this);

    addAndMakeVisible (button_colour_slider_2 = new TextButton ("new button"));
    button_colour_slider_2->setTooltip (TRANS("BACK slider"));
    button_colour_slider_2->setButtonText (TRANS("2"));
    button_colour_slider_2->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_slider_2->addListener (this);

    addAndMakeVisible (button_colour_slider_mod = new TextButton ("new button"));
    button_colour_slider_mod->setTooltip (TRANS("MODULATION slider (back)"));
    button_colour_slider_mod->setButtonText (TRANS("M"));
    button_colour_slider_mod->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_slider_mod->addListener (this);

    addAndMakeVisible (button_colour_buttons_font_on = new TextButton ("new button"));
    button_colour_buttons_font_on->setTooltip (TRANS("Button font ON "));
    button_colour_buttons_font_on->setButtonText (TRANS("F"));
    button_colour_buttons_font_on->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_buttons_font_on->addListener (this);

    addAndMakeVisible (button_colour_labels = new TextButton ("new button"));
    button_colour_labels->setTooltip (TRANS("Section FONT"));
    button_colour_labels->setButtonText (TRANS("F"));
    button_colour_labels->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_labels->addListener (this);

    addAndMakeVisible (label_ui_headline_6 = new Label (String(),
                                                        TRANS("AUDIO & CPU")));
    label_ui_headline_6->setFont (Font (30.00f, Font::plain));
    label_ui_headline_6->setJustificationType (Justification::centred);
    label_ui_headline_6->setEditable (false, false, false);
    label_ui_headline_6->setColour (Label::textColourId, Colour (0xff050505));
    label_ui_headline_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_ui_headline_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (button_replace_preset = new TextButton ("new button"));
    button_replace_preset->setTooltip (TRANS("Replace this theme."));
    button_replace_preset->setButtonText (TRANS("SAVE"));
    button_replace_preset->addListener (this);

    addAndMakeVisible (button_save_as_preset = new TextButton ("new button"));
    button_save_as_preset->setTooltip (TRANS("Save theme under new name."));
    button_save_as_preset->setButtonText (TRANS("SAVE AS"));
    button_save_as_preset->addListener (this);

    addAndMakeVisible (label_6 = new Label (String(),
                                            TRANS("ANI-MORPH")));
    label_6->setTooltip (TRANS("Turn morph animations on sliders on or off."));
    label_6->setFont (Font (30.00f, Font::plain));
    label_6->setJustificationType (Justification::centredLeft);
    label_6->setEditable (false, false, false);
    label_6->setColour (Label::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_animate_sliders = new ToggleButton (String()));
    toggle_animate_sliders->setTooltip (TRANS("Turn morph animations on sliders on or off."));
    toggle_animate_sliders->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_1 = new TextButton ("new button"));
    button_colour_bg_svg_1->setTooltip (TRANS("Click to edit the colours of the FILTER section."));
    button_colour_bg_svg_1->setButtonText (TRANS("FLT"));
    button_colour_bg_svg_1->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    button_colour_bg_svg_1->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_2 = new TextButton ("new button"));
    button_colour_bg_svg_2->setTooltip (TRANS("Click to edit the colours of the FM section."));
    button_colour_bg_svg_2->setButtonText (TRANS("FM"));
    button_colour_bg_svg_2->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_bg_svg_2->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_3 = new TextButton ("new button"));
    button_colour_bg_svg_3->setTooltip (TRANS("Click to edit the colours of the FX/EQ section."));
    button_colour_bg_svg_3->setButtonText (TRANS("FX/EQ"));
    button_colour_bg_svg_3->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_bg_svg_3->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_4 = new TextButton ("new button"));
    button_colour_bg_svg_4->setTooltip (TRANS("Click to edit the colours of the ARP section."));
    button_colour_bg_svg_4->setButtonText (TRANS("ARP"));
    button_colour_bg_svg_4->setConnectedEdges (Button::ConnectedOnTop);
    button_colour_bg_svg_4->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_5 = new TextButton ("new button"));
    button_colour_bg_svg_5->setTooltip (TRANS("Click to edit the colours of the MORPH section."));
    button_colour_bg_svg_5->setButtonText (TRANS("MO"));
    button_colour_bg_svg_5->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_bg_svg_5->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_6 = new TextButton ("new button"));
    button_colour_bg_svg_6->setTooltip (TRANS("Click to edit the colours of the MASTER OUT section."));
    button_colour_bg_svg_6->setButtonText (TRANS("V"));
    button_colour_bg_svg_6->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_bg_svg_6->addListener (this);

    addAndMakeVisible (button_colour_bg_svg_7 = new TextButton ("new button"));
    button_colour_bg_svg_7->setTooltip (TRANS("Click to edit the colours of the OSC section."));
    button_colour_bg_svg_7->setButtonText (TRANS("O"));
    button_colour_bg_svg_7->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    button_colour_bg_svg_7->addListener (this);

    addAndMakeVisible (copy = new TextButton (String()));
    copy->setTooltip (TRANS("Copy current colour to clipboard."));
    copy->setButtonText (TRANS("COPY"));
    copy->addListener (this);
    copy->setColour (TextButton::buttonColourId, Colours::cornflowerblue);
    copy->setColour (TextButton::buttonOnColourId, Colours::green);
    copy->setColour (TextButton::textColourOnId, Colours::black);
    copy->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (past = new TextButton (String()));
    past->setTooltip (TRANS("Past colour from clipboard."));
    past->setButtonText (TRANS("PAST"));
    past->addListener (this);
    past->setColour (TextButton::buttonColourId, Colours::blueviolet);
    past->setColour (TextButton::buttonOnColourId, Colours::green);
    past->setColour (TextButton::textColourOnId, Colours::black);
    past->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (button_colour_buttons_off = new TextButton ("new button"));
    button_colour_buttons_off->setTooltip (TRANS("Button OFF or BACKGROUND"));
    button_colour_buttons_off->setButtonText (TRANS("B"));
    button_colour_buttons_off->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_buttons_off->addListener (this);

    addAndMakeVisible (button_colour_buttons_font_off = new TextButton ("new button"));
    button_colour_buttons_font_off->setTooltip (TRANS("Button font OFF"));
    button_colour_buttons_font_off->setButtonText (TRANS("F"));
    button_colour_buttons_font_off->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_buttons_font_off->addListener (this);

    addAndMakeVisible (button_colour_slider_bg = new TextButton ("new button"));
    button_colour_slider_bg->setTooltip (TRANS("Slider BACKGROUND"));
    button_colour_slider_bg->setButtonText (TRANS("B"));
    button_colour_slider_bg->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_slider_bg->addListener (this);

    addAndMakeVisible (button_remove_preset = new TextButton ("new button"));
    button_remove_preset->setTooltip (TRANS("Delete selected theme."));
    button_remove_preset->setButtonText (TRANS("DELETE"));
    button_remove_preset->addListener (this);

    addAndMakeVisible (label_11 = new Label (String(),
                                             TRANS("FOR ALL")));
    label_11->setTooltip (TRANS("Enable this option to change colours for an element over all sections."));
    label_11->setFont (Font (30.00f, Font::plain));
    label_11->setJustificationType (Justification::centredLeft);
    label_11->setEditable (false, false, false);
    label_11->setColour (Label::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggle_for_all = new ToggleButton (String()));
    toggle_for_all->setTooltip (TRANS("Enable this option to change colours for an element over all sections."));
    toggle_for_all->addListener (this);

    addAndMakeVisible (copy2 = new TextButton (String()));
    copy2->setTooltip (TRANS("Copy this section colours to clipboard."));
    copy2->setButtonText (TRANS("COPY"));
    copy2->addListener (this);
    copy2->setColour (TextButton::buttonColourId, Colours::cornflowerblue);
    copy2->setColour (TextButton::buttonOnColourId, Colours::green);
    copy2->setColour (TextButton::textColourOnId, Colours::black);
    copy2->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (past2 = new TextButton (String()));
    past2->setTooltip (TRANS("Past section colours from clipboard."));
    past2->setButtonText (TRANS("PAST"));
    past2->addListener (this);
    past2->setColour (TextButton::buttonColourId, Colours::blueviolet);
    past2->setColour (TextButton::buttonOnColourId, Colours::green);
    past2->setColour (TextButton::textColourOnId, Colours::black);
    past2->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (button_colour_slider_disabled = new TextButton ("new button"));
    button_colour_slider_disabled->setTooltip (TRANS("Slider DISABLED (currently in background)"));
    button_colour_slider_disabled->setButtonText (TRANS("D"));
    button_colour_slider_disabled->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_slider_disabled->addListener (this);

    addAndMakeVisible (label_section = new Label (String(),
                                                  TRANS("SECTION: Background")));
    label_section->setFont (Font (30.00f, Font::plain));
    label_section->setJustificationType (Justification::centred);
    label_section->setEditable (false, false, false);
    label_section->setColour (Label::textColourId, Colour (0xffff3b00));
    label_section->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_section->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_colour = new Label (String(),
                                                 TRANS("ELEMENT: Section Background")));
    label_colour->setFont (Font (30.00f, Font::plain));
    label_colour->setJustificationType (Justification::centred);
    label_colour->setEditable (false, false, false);
    label_colour->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (colour_selector = new ColourSelector (ColourSelector::showColourspace
                                                             ,2,4));

    addAndMakeVisible (label_colour2 = new Label (String(),
                                                  TRANS("COLOUR SELECTOR")));
    label_colour2->setFont (Font (30.00f, Font::plain));
    label_colour2->setJustificationType (Justification::centred);
    label_colour2->setEditable (false, false, false);
    label_colour2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_colour2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_colour2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label_section2 = new Label (String(),
                                                   TRANS("THEMES")));
    label_section2->setFont (Font (30.00f, Font::plain));
    label_section2->setJustificationType (Justification::centred);
    label_section2->setEditable (false, false, false);
    label_section2->setColour (Label::textColourId, Colour (0xffff3b00));
    label_section2->setColour (TextEditor::textColourId, Colour (0xffff3b00));
    label_section2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (combo_theme = new ComboBox (String()));
    combo_theme->setTooltip (TRANS("Select and load a colour preset."));
    combo_theme->setEditableText (true);
    combo_theme->setJustificationType (Justification::centredLeft);
    combo_theme->setTextWhenNothingSelected (TRANS("DEFAULT"));
    combo_theme->setTextWhenNoChoicesAvailable (TRANS("EMPTY BANK"));
    combo_theme->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("FFFFFFFF\n")));
    label->setTooltip (TRANS("Past your RGB colour code here."));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (true, true, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    label->addListener (this);

    addAndMakeVisible (button_colour_oszi_1 = new TextButton (String()));
    button_colour_oszi_1->setTooltip (TRANS("Oscilloscope 1 (Background of the Oszi  (BG-Section))"));
    button_colour_oszi_1->setButtonText (TRANS("O1"));
    button_colour_oszi_1->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_oszi_1->addListener (this);

    addAndMakeVisible (button_colour_oszi_2 = new TextButton (String()));
    button_colour_oszi_2->setTooltip (TRANS("Oscilloscope 2 (only: Filter 2, OSC 2, Amp ENV)"));
    button_colour_oszi_2->setButtonText (TRANS("O2"));
    button_colour_oszi_2->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_oszi_2->addListener (this);

    addAndMakeVisible (button_colour_oszi_3 = new TextButton (String()));
    button_colour_oszi_3->setTooltip (TRANS("Oscilloscope 3 (only: Filter 3, OSC 3)"));
    button_colour_oszi_3->setButtonText (TRANS("O3"));
    button_colour_oszi_3->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    button_colour_oszi_3->addListener (this);

    addAndMakeVisible (selected_section_marker = new TextButton ("new button"));
    selected_section_marker->setButtonText (String());
    selected_section_marker->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    selected_section_marker->addListener (this);
    selected_section_marker->setColour (TextButton::buttonColourId, Colours::red);

    addAndMakeVisible (selected_element_marker = new TextButton ("new button"));
    selected_element_marker->setButtonText (String());
    selected_element_marker->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    selected_element_marker->addListener (this);
    selected_element_marker->setColour (TextButton::buttonColourId, Colours::red);

    addAndMakeVisible (credits_poper = new CreditsPoper (parent_,true));

    addAndMakeVisible (close = new TextButton (String()));
    close->setTooltip (TRANS("Close setup."));
    close->setButtonText (TRANS("X"));
    close->addListener (this);
    close->setColour (TextButton::buttonColourId, Colours::red);
    close->setColour (TextButton::buttonOnColourId, Colours::red);
    close->setColour (TextButton::textColourOnId, Colours::black);
    close->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (no_colour_in_demo_button = new TextButton ("new button"));
    no_colour_in_demo_button->setButtonText (TRANS("Pro Version Only"));
    no_colour_in_demo_button->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    no_colour_in_demo_button->addListener (this);
    no_colour_in_demo_button->setColour (TextButton::buttonColourId, Colour (0x6a929297));
    no_colour_in_demo_button->setColour (TextButton::textColourOnId, Colours::black);
    no_colour_in_demo_button->setColour (TextButton::textColourOffId, Colours::black);


    //[UserPreSize]

#ifdef IS_STANDALONE
    label_ui_headline_3->setVisible(false);
    label_ui_headline_7->setVisible(false);
    label_ui_headline_9->setVisible(false);
    credits_poper->setVisible(false);
    image_vst->setVisible(false);

    label_ui_headline_6->setText("AUDIO & CPU",dontSendNotification);
#else
#ifdef IS_VST
    // DEFAULT SETUP IN THE EDITOR
#elif IS_AU
    image_vst->setImages (false, true, true,
                          ImageCache::getFromMemory (au_logo_100x_png, au_logo_100x_pngSize), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000));
    label_ui_headline_9->setText("AudioUnits is a trademark of Apple Computer, Inc.",dontSendNotification);
#elif IS_AAX
    image_vst->setImages (false, true, true,
                          ImageCache::getFromMemory (aax_logo_100x_png, aax_logo_100x_pngSize), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000),
                          Image(), 1.000f, Colour (0x00000000));
    label_ui_headline_9->setText("AAX is a trademark of Avid Technology, Inc.",dontSendNotification);
#endif
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

    label_ui_headline_6->setText(String("MONIQUE ")+String(ProjectInfo::versionString),dontSendNotification);
#endif

    /// COLOURS
    for( int i = 0 ; i != getNumChildComponents() ; ++i )
    {
        Component*child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::SETUP_THEME );
        if( Slider* slider = dynamic_cast<Slider*>(child) )
        {
            slider->getProperties().set( VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER );
        }
    }
    setOpaque(true);
    selected_section_marker->setOpaque(false);
    selected_section_marker->setOpaque(false);
    label_colour2->setOpaque(false);
    label_colour->setOpaque(false);
    label_section2->setOpaque(false);
    label_section->setOpaque(false);
    {
        current_edited_colour = nullptr;

        button_colour_bg->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::BG_THEME );
        button_colour_bg->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_1->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME );
        button_colour_bg_svg_1->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_2->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FM_THEME );
        button_colour_bg_svg_2->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_3->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FX_THEME );
        button_colour_bg_svg_3->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_4->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME );
        button_colour_bg_svg_4->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_5->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MORPH_THEME );
        button_colour_bg_svg_5->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_6->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::MASTER_THEME );
        button_colour_bg_svg_6->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );
        button_colour_bg_svg_7->getProperties().set( VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::OSC_THEME );
        button_colour_bg_svg_7->getProperties().set( VAR_INDEX_BUTTON_AMP, COLOUR_CODES::AREA_COLOUR );


        selected_section_marker->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        selected_element_marker->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        selected_section_marker->setInterceptsMouseClicks( false,false );
        selected_element_marker->setInterceptsMouseClicks( false,false );
        copy2->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        past2->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        copy->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        past->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
    }

    if( not SHARED::getInstance()->status.isUnlocked() )
    {
        no_colour_in_demo_button->getProperties().set( VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true );
        no_colour_in_demo_button->setOpaque(false);
    }
    else
    {
        no_colour_in_demo_button->setVisible(false);
    }

    image_vst->setOpaque(false);
    label_ui_headline_2->setOpaque(false);
    label_ui_headline_6->setOpaque(false);
    label_11->setOpaque(false);
    label_section__->setOpaque(false);
    label_slider__->setOpaque(false);
    label_buttons__->setOpaque(false);
    credits_poper->setOpaque(false);

    // CPU
    {
        for( int i = 0 ; i != THREAD_LIMIT ; ++i )
        {
            combo_multicore_cpus->addItem(String(i+1),i+1);
        }
    }

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

    update_colour_presets();
#else
    triggerAsyncUpdate();
#endif

    /*
    //[/UserPreSize]

    setSize (1465, 380);


    //[Constructor] You can add your own custom stuff here..
    */
    //[/Constructor]
}

Monique_Ui_GlobalSettings::~Monique_Ui_GlobalSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label_ui_headline_9 = nullptr;
    label_ui_headline_3 = nullptr;
    label_ui_headline_7 = nullptr;
    image_vst = nullptr;
    label_8 = nullptr;
    combo_audio_device = nullptr;
    label_7 = nullptr;
    combo_audio_driver = nullptr;
    button_colour_bg = nullptr;
    button_colour_background = nullptr;
    label_buttons__ = nullptr;
    label_slider__ = nullptr;
    label_section__ = nullptr;
    label_9 = nullptr;
    label_2 = nullptr;
    label_4 = nullptr;
    combo_multicore_cpus = nullptr;
    label_cpu_usage = nullptr;
    combo_block_size = nullptr;
    label_10 = nullptr;
    combo_sample_rate = nullptr;
    label_16 = nullptr;
    toggle_animate_input_env = nullptr;
    label_18 = nullptr;
    toggle_show_tooltips = nullptr;
    label_ui_headline_2 = nullptr;
    button_colour_buttons_on = nullptr;
    button_colour_slider_1 = nullptr;
    button_colour_slider_2 = nullptr;
    button_colour_slider_mod = nullptr;
    button_colour_buttons_font_on = nullptr;
    button_colour_labels = nullptr;
    label_ui_headline_6 = nullptr;
    button_replace_preset = nullptr;
    button_save_as_preset = nullptr;
    label_6 = nullptr;
    toggle_animate_sliders = nullptr;
    button_colour_bg_svg_1 = nullptr;
    button_colour_bg_svg_2 = nullptr;
    button_colour_bg_svg_3 = nullptr;
    button_colour_bg_svg_4 = nullptr;
    button_colour_bg_svg_5 = nullptr;
    button_colour_bg_svg_6 = nullptr;
    button_colour_bg_svg_7 = nullptr;
    copy = nullptr;
    past = nullptr;
    button_colour_buttons_off = nullptr;
    button_colour_buttons_font_off = nullptr;
    button_colour_slider_bg = nullptr;
    button_remove_preset = nullptr;
    label_11 = nullptr;
    toggle_for_all = nullptr;
    copy2 = nullptr;
    past2 = nullptr;
    button_colour_slider_disabled = nullptr;
    label_section = nullptr;
    label_colour = nullptr;
    colour_selector = nullptr;
    label_colour2 = nullptr;
    label_section2 = nullptr;
    combo_theme = nullptr;
    label = nullptr;
    button_colour_oszi_1 = nullptr;
    button_colour_oszi_2 = nullptr;
    button_colour_oszi_3 = nullptr;
    selected_section_marker = nullptr;
    selected_element_marker = nullptr;
    credits_poper = nullptr;
    close = nullptr;
    no_colour_in_demo_button = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_GlobalSettings::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPrePaint]

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffffff11));
    g.fillRect (420, 10, 30, 30);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (181.0f, 0.0f, 248.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, 0.0f, 169.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (1126.0f, 0.0f, 324.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (441.0f, 0.0f, 288.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRect (720, 10, 30, 30);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (741.0f, 0.0f, 248.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (1001.0f, 0.0f, 123.0f, 250.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillRect (980, 10, 30, 30);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Monique_Ui_GlobalSettings::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    WIDTH_AND_HIGHT_FACTORS
    //[/UserPreResize]

    label_ui_headline_9->setBounds (1150, 130, 290, 30);
    label_ui_headline_3->setBounds (1150, 50, 190, 30);
    label_ui_headline_7->setBounds (1150, 80, 190, 30);
    image_vst->setBounds (1340, 60, 90, 60);
    label_8->setBounds (1140, 130, 60, 30);
    combo_audio_device->setBounds (1200, 90, 80, 30);
    label_7->setBounds (1140, 90, 60, 30);
    combo_audio_driver->setBounds (1200, 50, 80, 30);
    button_colour_bg->setBounds (200, 40, 210, 120);
    button_colour_background->setBounds (540, 40, 30, 30);
    label_buttons__->setBounds (450, 100, 80, 30);
    label_slider__->setBounds (450, 70, 80, 30);
    label_section__->setBounds (450, 40, 80, 30);
    label_9->setBounds (1295, 50, 60, 30);
    label_2->setBounds (1295, 90, 60, 30);
    label_4->setBounds (1295, 130, 60, 30);
    combo_multicore_cpus->setBounds (1355, 130, 80, 30);
    label_cpu_usage->setBounds (1355, 90, 80, 33);
    combo_block_size->setBounds (1355, 50, 80, 30);
    label_10->setBounds (1140, 50, 60, 30);
    combo_sample_rate->setBounds (1200, 130, 80, 30);
    label_16->setBounds (60, 50, 100, 30);
    toggle_animate_input_env->setBounds (30, 50, 33, 30);
    label_18->setBounds (60, 130, 100, 30);
    toggle_show_tooltips->setBounds (30, 130, 33, 33);
    label_ui_headline_2->setBounds (10, 0, 170, 30);
    button_colour_buttons_on->setBounds (540, 100, 30, 30);
    button_colour_slider_1->setBounds (540, 70, 30, 30);
    button_colour_slider_2->setBounds (580, 70, 30, 30);
    button_colour_slider_mod->setBounds (610, 70, 30, 30);
    button_colour_buttons_font_on->setBounds (570, 100, 30, 30);
    button_colour_labels->setBounds (570, 40, 30, 30);
    label_ui_headline_6->setBounds (1130, 0, 315, 30);
    button_replace_preset->setBounds (1020, 75, 85, 30);
    button_save_as_preset->setBounds (1020, 105, 85, 30);
    label_6->setBounds (60, 90, 100, 30);
    toggle_animate_sliders->setBounds (30, 90, 33, 33);
    button_colour_bg_svg_1->setBounds (250, 50, 150, 30);
    button_colour_bg_svg_2->setBounds (210, 80, 40, 30);
    button_colour_bg_svg_3->setBounds (290, 80, 80, 30);
    button_colour_bg_svg_4->setBounds (210, 110, 190, 30);
    button_colour_bg_svg_5->setBounds (250, 80, 40, 30);
    button_colour_bg_svg_6->setBounds (370, 80, 30, 30);
    button_colour_bg_svg_7->setBounds (210, 50, 40, 30);
    copy->setBounds (870, 135, 50, 30);
    past->setBounds (920, 135, 50, 30);
    button_colour_buttons_off->setBounds (610, 100, 30, 30);
    button_colour_buttons_font_off->setBounds (640, 100, 30, 30);
    button_colour_slider_bg->setBounds (680, 70, 30, 30);
    button_remove_preset->setBounds (1020, 135, 85, 30);
    label_11->setBounds (450, 135, 80, 30);
    toggle_for_all->setBounds (540, 135, 33, 33);
    copy2->setBounds (610, 135, 50, 30);
    past2->setBounds (660, 135, 50, 30);
    button_colour_slider_disabled->setBounds (650, 70, 30, 30);
    label_section->setBounds (180, 0, 250, 30);
    label_colour->setBounds (440, 0, 290, 30);
    colour_selector->setBounds (760, 40, 210, 90);
    label_colour2->setBounds (740, 0, 250, 30);
    label_section2->setBounds (1000, 0, 125, 30);
    combo_theme->setBounds (1020, 40, 85, 30);
    label->setBounds (760, 135, 110, 30);
    button_colour_oszi_1->setBounds (620, 40, 30, 30);
    button_colour_oszi_2->setBounds (650, 40, 30, 30);
    button_colour_oszi_3->setBounds (680, 40, 30, 30);
    selected_section_marker->setBounds (470, 220, 6, 6);
    selected_element_marker->setBounds (210, 230, 6, 6);
    credits_poper->setBounds (1130, 50, 320, 160);
    close->setBounds (1420, 5, 25, 25);
    no_colour_in_demo_button->setBounds (190, 30, 790, 145);
    //[UserResized] Add your own custom resize handling here..
#include "mono_ui_includeHacks_END.h"

    open_colour_selector(current_colour);

    //[/UserResized]
}

void Monique_Ui_GlobalSettings::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == combo_audio_device)
    {
        //[UserComboBoxCode_combo_audio_device] -- add your combo box handling code here..
#ifdef IS_STANDALONE
        MoniqueAudioProcessor* audio_processor = synth_data->audio_processor;
        audio_processor->set_audio_offline();
        {
            // GET CURRENT SETTINGS
            AudioDeviceManager::AudioDeviceSetup current_device_setup;
            audio_processor->getAudioDeviceSetup( current_device_setup );

            // UPDATE SETTINGS
            current_device_setup.outputDeviceName = combo_audio_device->getText();
            current_device_setup.sampleRate = 0;
            current_device_setup.bufferSize = 0;
            current_device_setup.useDefaultInputChannels = true;
            current_device_setup.useDefaultOutputChannels = true;

            // SET NEW SETTINGS
            audio_processor->player.setProcessor (nullptr);
            audio_processor->removeAudioCallback (&audio_processor->player);
            String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

            if( error != "" )
            {
                AlertWindow::showMessageBoxAsync
                (
                    AlertWindow::AlertIconType::WarningIcon,
                    "ERROR: OPEN AUDIO DEVICE!",
                    error
                );
                audio_processor->audio_is_successful_initalized = false;
            }
            else if( not audio_processor->audio_is_successful_initalized )
            {

#ifdef JUCE_IOS
                String error = audio_processor->initialise(1,1, nullptr, false );
#else
                String error = audio_processor->initialise(2,2, nullptr, false );
#endif
                if( error == "" )
                {
                    audio_processor->addAudioCallback (&audio_processor->player);
                    audio_processor->player.setProcessor (audio_processor);
                    audio_processor->audio_is_successful_initalized = true;
                }
                else
                {
                    audio_processor->audio_is_successful_initalized = false;
                    AlertWindow::showMessageBoxAsync
                    (
                        AlertWindow::AlertIconType::WarningIcon,
                        "ERROR: OPEN AUDIO DEVICE!",
                        error
                    );
                }
            }
            else
            {
                //String error = audio_processor->initialise( 2,2, nullptr, false );
                audio_processor->addAudioCallback (&audio_processor->player);
                audio_processor->player.setProcessor (audio_processor);
                audio_processor->audio_is_successful_initalized = true;
                /*
                        if( error != "" )
                        {
                            AlertWindow::showMessageBoxAsync
                            (
                                AlertWindow::AlertIconType::WarningIcon,
                                "ERROR: OPEN AUDIO DEVICE!",
                                error
                            );
                        }
                        */
            }
        }

        update_audio_devices();
        audio_processor->set_audio_online();
#endif
        //[/UserComboBoxCode_combo_audio_device]
    }
    else if (comboBoxThatHasChanged == combo_audio_driver)
    {
        //[UserComboBoxCode_combo_audio_driver] -- add your combo box handling code here..
#ifdef IS_STANDALONE
        MoniqueAudioProcessor* audio_processor = synth_data->audio_processor;
        audio_processor->set_audio_offline();
        {
            audio_processor->setCurrentAudioDeviceType(combo_audio_driver->getText(),true);
            if( not audio_processor->audio_is_successful_initalized )
            {
#ifdef JUCE_IOS
                String error = audio_processor->initialise(1,1, nullptr, false );
#else
                String error = audio_processor->initialise(2,2, nullptr, false );
#endif
                if( error != "" )
                {
                    AlertWindow::showMessageBoxAsync
                    (
                        AlertWindow::AlertIconType::WarningIcon,
                        "ERROR: OPEN AUDIO DEVICE!",
                        error
                    );
                    audio_processor->audio_is_successful_initalized = false;
                }
                else
                {
                    audio_processor->audio_is_successful_initalized = true;
                }
            }
        }

        update_audio_devices();
        audio_processor->set_audio_online();
#endif
        //[/UserComboBoxCode_combo_audio_driver]
    }
    else if (comboBoxThatHasChanged == combo_multicore_cpus)
    {
        //[UserComboBoxCode_combo_multicore_cpus] -- add your combo box handling code here..
        synth_data->num_extra_threads = combo_multicore_cpus->getSelectedId()-1;
        //[/UserComboBoxCode_combo_multicore_cpus]
    }
    else if (comboBoxThatHasChanged == combo_block_size)
    {
        //[UserComboBoxCode_combo_block_size] -- add your combo box handling code here..
#ifdef IS_STANDALONE
        MoniqueAudioProcessor* audio_processor = synth_data->audio_processor;
        audio_processor->set_audio_offline();
        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );
        current_device_setup.bufferSize = combo_block_size->getText().getIntValue();
        {
            String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

            if( error != "" )
            {
                AlertWindow::showMessageBoxAsync
                (
                    AlertWindow::AlertIconType::WarningIcon,
                    "ERROR: SET BLOCK SIZE!",
                    error
                );
            }
        }

        update_audio_devices();
        audio_processor->set_audio_online();
#endif
        //[/UserComboBoxCode_combo_block_size]
    }
    else if (comboBoxThatHasChanged == combo_sample_rate)
    {
        //[UserComboBoxCode_combo_sample_rate] -- add your combo box handling code here..
#ifdef IS_STANDALONE
        MoniqueAudioProcessor* audio_processor = synth_data->audio_processor;
        audio_processor->set_audio_offline();

        AudioDeviceManager::AudioDeviceSetup current_device_setup;
        audio_processor->getAudioDeviceSetup( current_device_setup );
        current_device_setup.sampleRate = combo_sample_rate->getText().getDoubleValue();
        {
            String error = audio_processor->setAudioDeviceSetup( current_device_setup, true );

            if( error != "" )
            {
                AlertWindow::showMessageBoxAsync
                (
                    AlertWindow::AlertIconType::WarningIcon,
                    "ERROR: SET SAMPLE RATE!",
                    error
                );
            }
        }

        update_audio_devices();
        audio_processor->set_audio_online();
#endif
        //[/UserComboBoxCode_combo_sample_rate]
    }
    else if (comboBoxThatHasChanged == combo_theme)
    {
        //[UserComboBoxCode_combo_theme] -- add your combo box handling code here..
        String new_name = combo_theme->getText();
        String old_name = combo_theme->getItemText(combo_theme->getSelectedItemIndex());
        if( old_name != new_name && combo_theme->getSelectedItemIndex() == -1 )
        {
            synth_data->create_new_theme(new_name);
            //show_programs_and_select(true);
        }
        else
        {
#ifdef ASK_FOR_SAVE
            synth_data->ask_and_save_if_changed();
#endif
            block_colour_update = true;
            if( synth_data->load_theme(combo_theme->getText()) )
            {
                colour_selector->setCurrentColour( *current_edited_colour );
            }
            get_editor()->repaint();
            block_colour_update = false;
        }
        update_colour_presets();


        //[/UserComboBoxCode_combo_theme]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Monique_Ui_GlobalSettings::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == button_colour_bg)
    {
        //[UserButtonCode_button_colour_bg] -- add your button handler code here..
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( BG_THEME );
        label_section->setText("ELEMENT: Background",dontSendNotification);
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg]
    }
    else if (buttonThatWasClicked == button_colour_background)
    {
        //[UserButtonCode_button_colour_background] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::AREA_COLOUR );
        label_colour->setText("ELEMENT: Section Background",dontSendNotification);
        //[/UserButtonCode_button_colour_background]
    }
    else if (buttonThatWasClicked == toggle_animate_input_env)
    {
        //[UserButtonCode_toggle_animate_input_env] -- add your button handler code here..
        synth_data->animate_envs = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_input_env]
    }
    else if (buttonThatWasClicked == toggle_show_tooltips)
    {
        //[UserButtonCode_toggle_show_tooltips] -- add your button handler code here..
        synth_data->show_tooltips = buttonThatWasClicked->getToggleState();
        get_editor()->update_tooltip_handling(false);
        //[/UserButtonCode_toggle_show_tooltips]
    }
    else if (buttonThatWasClicked == button_colour_buttons_on)
    {
        //[UserButtonCode_button_colour_buttons_on] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::BUTTON_ON_COLOUR );
        label_colour->setText("ELEMENT: Button On",dontSendNotification);
        //[/UserButtonCode_button_colour_buttons_on]
    }
    else if (buttonThatWasClicked == button_colour_slider_1)
    {
        //[UserButtonCode_button_colour_slider_1] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::VALUE_SLIDER_COLOUR );
        label_colour->setText("ELEMENT: Front Slider",dontSendNotification);
        //[/UserButtonCode_button_colour_slider_1]
    }
    else if (buttonThatWasClicked == button_colour_slider_2)
    {
        //[UserButtonCode_button_colour_slider_2] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::VALUE_SLIDER_2_COLOUR );
        label_colour->setText("ELEMENT: Back Slider",dontSendNotification);
        //[/UserButtonCode_button_colour_slider_2]
    }
    else if (buttonThatWasClicked == button_colour_slider_mod)
    {
        //[UserButtonCode_button_colour_slider_mod] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::MOD_SLIDER_COLOUR );
        label_colour->setText("ELEMENT: Modulation Slider (back)",dontSendNotification);
        //[/UserButtonCode_button_colour_slider_mod]
    }
    else if (buttonThatWasClicked == button_colour_buttons_font_on)
    {
        //[UserButtonCode_button_colour_buttons_font_on] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::BUTTON_ON_FONT_COLOUR );
        label_colour->setText("ELEMENT: Button Font ON",dontSendNotification);
        //[/UserButtonCode_button_colour_buttons_font_on]
    }
    else if (buttonThatWasClicked == button_colour_labels)
    {
        //[UserButtonCode_button_colour_labels] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::AREA_FONT_COLOUR );
        label_colour->setText("ELEMENT: Section Labels",dontSendNotification);
        //[/UserButtonCode_button_colour_labels]
    }
    else if (buttonThatWasClicked == button_replace_preset)
    {
        //[UserButtonCode_button_replace_preset] -- add your button handler code here..
        synth_data->replace_theme( combo_theme->getText() );
        //[/UserButtonCode_button_replace_preset]
    }
    else if (buttonThatWasClicked == button_save_as_preset)
    {
        //[UserButtonCode_button_save_as_preset] -- add your button handler code here..
        combo_theme->setEditableText(true);
        combo_theme->showEditor();
        //[/UserButtonCode_button_save_as_preset]
    }
    else if (buttonThatWasClicked == toggle_animate_sliders)
    {
        //[UserButtonCode_toggle_animate_sliders] -- add your button handler code here..
        synth_data->animate_sliders = buttonThatWasClicked->getToggleState();
        //[/UserButtonCode_toggle_animate_sliders]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_1)
    {
        //[UserButtonCode_button_colour_bg_svg_1] -- add your button handler code here..
        label_section->setText("SECTION: Filters",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( FILTER_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_1]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_2)
    {
        //[UserButtonCode_button_colour_bg_svg_2] -- add your button handler code here..
        label_section->setText("SECTION: FM",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( FM_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_2]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_3)
    {
        //[UserButtonCode_button_colour_bg_svg_3] -- add your button handler code here..
        label_section->setText("SECTION: FX/EQ / AMP",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( FX_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_3]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_4)
    {
        //[UserButtonCode_button_colour_bg_svg_4] -- add your button handler code here..
        label_section->setText("SECTION: Arpeggiator",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( ARP_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_4]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_5)
    {
        //[UserButtonCode_button_colour_bg_svg_5] -- add your button handler code here..
        label_section->setText("SECTION: Morph",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( MORPH_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_5]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_6)
    {
        //[UserButtonCode_button_colour_bg_svg_6] -- add your button handler code here..
        label_section->setText("SECTION:: MAster Out",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( MASTER_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_6]
    }
    else if (buttonThatWasClicked == button_colour_bg_svg_7)
    {
        //[UserButtonCode_button_colour_bg_svg_7] -- add your button handler code here..
        label_section->setText("SECTION: Oscillators",dontSendNotification);
        editable_theme = &ui_refresher->look_and_feel->colours.get_theme( OSC_THEME );
        open_colour_selector( current_colour );
        force_repaint = true;
        //[/UserButtonCode_button_colour_bg_svg_7]
    }
    else if (buttonThatWasClicked == copy)
    {
        //[UserButtonCode_copy] -- add your button handler code here..
        colour_clipboard = last_repainted_colour.getARGB();
        //[/UserButtonCode_copy]
    }
    else if (buttonThatWasClicked == past)
    {
        //[UserButtonCode_past] -- add your button handler code here..
        colour_selector->setCurrentColour(Colour(colour_clipboard));
        //[/UserButtonCode_past]
    }
    else if (buttonThatWasClicked == button_colour_buttons_off)
    {
        //[UserButtonCode_button_colour_buttons_off] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::BUTTON_OFF_COLOUR );
        label_colour->setText("ELEMENT: Button OFF",dontSendNotification);
        //[/UserButtonCode_button_colour_buttons_off]
    }
    else if (buttonThatWasClicked == button_colour_buttons_font_off)
    {
        //[UserButtonCode_button_colour_buttons_font_off] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::BUTTON_OFF_FONT_COLOUR );
        label_colour->setText("ELEMENT: Button Font OFF",dontSendNotification);
        //[/UserButtonCode_button_colour_buttons_font_off]
    }
    else if (buttonThatWasClicked == button_colour_slider_bg)
    {
        //[UserButtonCode_button_colour_slider_bg] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::SLIDER_BACKGROUND_COLOUR );
        label_colour->setText("ELEMENT: Slider BG",dontSendNotification);
        //[/UserButtonCode_button_colour_slider_bg]
    }
    else if (buttonThatWasClicked == button_remove_preset)
    {
        //[UserButtonCode_button_remove_preset] -- add your button handler code here..
        if( synth_data->remove_theme( combo_theme->getText() ) )
        {
            update_colour_presets();
        }
        //[/UserButtonCode_button_remove_preset]
    }
    else if (buttonThatWasClicked == toggle_for_all)
    {
        //[UserButtonCode_toggle_for_all] -- add your button handler code here..
        //[/UserButtonCode_toggle_for_all]
    }
    else if (buttonThatWasClicked == copy2)
    {
        //[UserButtonCode_copy2] -- add your button handler code here..
        if( not look_and_feel->colours.theme_clipboard )
        {
            look_and_feel->colours.theme_clipboard = new SectionTheme( CLIPBOARD_THEME );
        }
        if( editable_theme )
        {
            look_and_feel->colours.theme_clipboard->copy( *editable_theme );
        }
        //[/UserButtonCode_copy2]
    }
    else if (buttonThatWasClicked == past2)
    {
        //[UserButtonCode_past2] -- add your button handler code here..
        if( look_and_feel->colours.theme_clipboard )
        {
            block_colour_update = true;
            editable_theme->copy( *look_and_feel->colours.theme_clipboard );
            colour_selector->setCurrentColour( *current_edited_colour );
            get_editor()->repaint();
            block_colour_update = false;
        }
        //[/UserButtonCode_past2]
    }
    else if (buttonThatWasClicked == button_colour_slider_disabled)
    {
        //[UserButtonCode_button_colour_slider_disabled] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::DISABLED_SLIDER_COLOUR );
        label_colour->setText("ELEMENT: Slider Disabled",dontSendNotification);
        //[/UserButtonCode_button_colour_slider_disabled]
    }
    else if (buttonThatWasClicked == button_colour_oszi_1)
    {
        //[UserButtonCode_button_colour_oszi_1] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::OSZI_1 );
        label_colour->setText("ELEMENT: Oszi 1",dontSendNotification);
        //[/UserButtonCode_button_colour_oszi_1]
    }
    else if (buttonThatWasClicked == button_colour_oszi_2)
    {
        //[UserButtonCode_button_colour_oszi_2] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::OSZI_2 );
        label_colour->setText("ELEMENT: Oszi 2",dontSendNotification);
        //[/UserButtonCode_button_colour_oszi_2]
    }
    else if (buttonThatWasClicked == button_colour_oszi_3)
    {
        //[UserButtonCode_button_colour_oszi_3] -- add your button handler code here..
        open_colour_selector( COLOUR_CODES::OSZI_3 );
        label_colour->setText("ELEMENT: Oszi 3",dontSendNotification);
        //[/UserButtonCode_button_colour_oszi_3]
    }
    else if (buttonThatWasClicked == selected_section_marker)
    {
        //[UserButtonCode_selected_section_marker] -- add your button handler code here..
        //[/UserButtonCode_selected_section_marker]
    }
    else if (buttonThatWasClicked == selected_element_marker)
    {
        //[UserButtonCode_selected_element_marker] -- add your button handler code here..
        //[/UserButtonCode_selected_element_marker]
    }
    else if (buttonThatWasClicked == close)
    {
        //[UserButtonCode_close] -- add your button handler code here..
        parent->editor_global_settings = nullptr;
        return;
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == no_colour_in_demo_button)
    {
        //[UserButtonCode_no_colour_in_demo_button] -- add your button handler code here..
        Monique_Ui_Mainwindow* editor = get_editor();
        if( editor )
        {
            editor->show_activation_screen();
            editor->resized();
        }
        //[/UserButtonCode_no_colour_in_demo_button]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Monique_Ui_GlobalSettings::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == label)
    {
        //[UserLabelCode_label] -- add your label text handling code here..
        colour_selector->setCurrentColour( Colour::fromString( label->getText() ) );
        //[/UserLabelCode_label]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
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
                 componentName="" parentClasses="public Component, public Monique_Ui_Refreshable, public AsyncUpdater"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*parent_ "
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;original_w(1465), original_h(180)"
                 snapPixels="5" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1465" initialHeight="380">
  <BACKGROUND backgroundColour="ff050505">
    <RECT pos="420 10 30 30" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="181 0 248 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="10 0 169 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1126 0 324 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="441 0 288 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <RECT pos="720 10 30 30" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="741 0 248 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <ROUNDRECT pos="1001 0 123 250" cornerSize="10" fill="solid: ffffff11" hasStroke="0"/>
    <RECT pos="980 10 30 30" fill="solid: ffffff11" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="DL" id="2d28c9ae6faef00d" memberName="label_ui_headline_9"
         virtualName="" explicitFocusOrder="0" pos="1150 130 290 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="VST is a trademark of Steinberg Media Technologies GmbH"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="DL" id="7e5a2ac3e4741d59" memberName="label_ui_headline_3"
         virtualName="" explicitFocusOrder="0" pos="1150 50 190 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="Aus Liebe zur Musik."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <LABEL name="DL" id="191728a9e80d3f82" memberName="label_ui_headline_7"
         virtualName="" explicitFocusOrder="0" pos="1150 80 190 30" textCol="ff1111ff"
         edTextCol="ffff3b00" edBkgCol="0" labelText="Thomas Arndt | Monoplugs"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <IMAGEBUTTON name="new button" id="bab1ab6909a963b5" memberName="image_vst"
               virtualName="" explicitFocusOrder="0" pos="1340 60 90 60" buttonText=""
               connectedEdges="0" needsCallback="0" radioGroupId="0" keepProportions="1"
               resourceNormal="vst_logo_100x_png" opacityNormal="1" colourNormal="0"
               resourceOver="" opacityOver="1" colourOver="0" resourceDown=""
               opacityDown="1" colourDown="0"/>
  <LABEL name="" id="49f12ab9e3d54910" memberName="label_8" virtualName=""
         explicitFocusOrder="0" pos="1140 130 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="RATE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="efb590c00df9d613" memberName="combo_audio_device"
            virtualName="" explicitFocusOrder="0" pos="1200 90 80 30" tooltip="Select an audio device you like to use for the audio playback."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="e0756600ee1aa0c2" memberName="label_7" virtualName=""
         explicitFocusOrder="0" pos="1140 90 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DEVICE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="f91daaa7098deafb" memberName="combo_audio_driver"
            virtualName="" explicitFocusOrder="0" pos="1200 50 80 30" tooltip="Select an audio driver you like to use for the audio playback."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="914ce4dd638de5f3" memberName="button_colour_bg"
              virtualName="" explicitFocusOrder="0" pos="200 40 210 120" tooltip="Click to edit the colours of the background section."
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3ec96e4b24b77ca8" memberName="button_colour_background"
              virtualName="" explicitFocusOrder="0" pos="540 40 30 30" tooltip="Section BACKGROUND"
              buttonText="B" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="4164e3b93fd006e0" memberName="label_buttons__" virtualName=""
         explicitFocusOrder="0" pos="450 100 80 30" tooltip="Click a button on the right to change the colour for this element."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="Buttons&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="bb43131d9973a62b" memberName="label_slider__" virtualName=""
         explicitFocusOrder="0" pos="450 70 80 30" tooltip="Click a button on the right to change the colour for this element."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="Sliders"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="62a873940890f9fe" memberName="label_section__" virtualName=""
         explicitFocusOrder="0" pos="450 40 80 30" tooltip="Click a button on the right to change the colour for this element."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="Section"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="9e7b39300fbdcd67" memberName="label_9" virtualName=""
         explicitFocusOrder="0" pos="1295 50 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="BLOCK" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="a5e27df00dd3061" memberName="label_2" virtualName=""
         explicitFocusOrder="0" pos="1295 90 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="CPU" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <LABEL name="" id="5a530fedc3a6cb0" memberName="label_4" virtualName=""
         explicitFocusOrder="0" pos="1295 130 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THREADS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="78586adbf5ab9e5a" memberName="combo_multicore_cpus"
            virtualName="" explicitFocusOrder="0" pos="1355 130 80 30" tooltip="Select the threads you like to spend to process Moniqiue. &#10;&#10;Note: Its recommended to use NOT more threads as your CPU has cores! &#10;Please take a look at the CPU usage and decide how many threads are the best for your CPU."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="6ddda2710d986dce" memberName="label_cpu_usage" virtualName=""
         explicitFocusOrder="0" pos="1355 90 80 33" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="20%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="" id="d76df912445a2ff8" memberName="combo_block_size" virtualName=""
            explicitFocusOrder="0" pos="1355 50 80 30" tooltip="Select the block size you like to use for the audio playback.&#10;&#10;Note: smaller block sizes are more in time, but needs more CPU power."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="2c9d694778c498dc" memberName="label_10" virtualName=""
         explicitFocusOrder="0" pos="1140 50 60 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="DRIVER" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="" id="db95d5d8a64a8ebc" memberName="combo_sample_rate"
            virtualName="" explicitFocusOrder="0" pos="1200 130 80 30" tooltip="Select the sample rate you like to use for the audio playback.&#10;&#10;Note: the quality of larger sample rates is better, but needs more CPU power."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="" id="dfd8e91824767f78" memberName="label_16" virtualName=""
         explicitFocusOrder="0" pos="60 50 100 30" tooltip="Turn amp animations on buttons on or off."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="ANI-ENV'S"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="f342323c5495d0a1" memberName="toggle_animate_input_env"
                virtualName="" explicitFocusOrder="0" pos="30 50 33 30" tooltip="Turn amp animations on buttons on or off."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="" id="55ed48882dea6ac8" memberName="label_18" virtualName=""
         explicitFocusOrder="0" pos="60 130 100 30" tooltip="Turn tooltips on or off.&#10;&#10;Press the &quot;CTRL&quot; + &quot;h&quot; on your keyboard to show the tooltip if this option is turned off."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="TOOLTIPS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="80003915f6558086" memberName="toggle_show_tooltips"
                virtualName="" explicitFocusOrder="0" pos="30 130 33 33" tooltip="Turn tooltips on or off.&#10;&#10;Press the &quot;CTRL&quot; + &quot;h&quot; on your keyboard to show the tooltip if this option is turned off."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="" id="b5bc2cbedd6ff2b1" memberName="label_ui_headline_2"
         virtualName="" explicitFocusOrder="0" pos="10 0 170 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="MISC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="3891f5f1ede2a913" memberName="button_colour_buttons_on"
              virtualName="" explicitFocusOrder="0" pos="540 100 30 30" tooltip="Button ON"
              buttonText="O" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="2bde73aa342c2457" memberName="button_colour_slider_1"
              virtualName="" explicitFocusOrder="0" pos="540 70 30 30" tooltip="FRONT slider"
              buttonText="1" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8ddcc2620647afb5" memberName="button_colour_slider_2"
              virtualName="" explicitFocusOrder="0" pos="580 70 30 30" tooltip="BACK slider"
              buttonText="2" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="27311d6a38d6510d" memberName="button_colour_slider_mod"
              virtualName="" explicitFocusOrder="0" pos="610 70 30 30" tooltip="MODULATION slider (back)"
              buttonText="M" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="aa8131218b1a4e24" memberName="button_colour_buttons_font_on"
              virtualName="" explicitFocusOrder="0" pos="570 100 30 30" tooltip="Button font ON "
              buttonText="F" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="16cb69a9f61a0c7f" memberName="button_colour_labels"
              virtualName="" explicitFocusOrder="0" pos="570 40 30 30" tooltip="Section FONT"
              buttonText="F" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="668d26bcc5860c72" memberName="label_ui_headline_6"
         virtualName="" explicitFocusOrder="0" pos="1130 0 315 30" textCol="ff050505"
         edTextCol="ffff3b00" edBkgCol="0" labelText="AUDIO &amp; CPU"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="7a187799895dfa50" memberName="button_replace_preset"
              virtualName="" explicitFocusOrder="0" pos="1020 75 85 30" tooltip="Replace this theme."
              buttonText="SAVE" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="202c6d241354941d" memberName="button_save_as_preset"
              virtualName="" explicitFocusOrder="0" pos="1020 105 85 30" tooltip="Save theme under new name."
              buttonText="SAVE AS" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="caaf1edf55d35c5f" memberName="label_6" virtualName=""
         explicitFocusOrder="0" pos="60 90 100 30" tooltip="Turn morph animations on sliders on or off."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="ANI-MORPH"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="f2eadca2a245268" memberName="toggle_animate_sliders"
                virtualName="" explicitFocusOrder="0" pos="30 90 33 33" tooltip="Turn morph animations on sliders on or off."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="new button" id="fd4b76365c4c2a5e" memberName="button_colour_bg_svg_1"
              virtualName="" explicitFocusOrder="0" pos="250 50 150 30" tooltip="Click to edit the colours of the FILTER section."
              buttonText="FLT" connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e3c6c1baaac01d56" memberName="button_colour_bg_svg_2"
              virtualName="" explicitFocusOrder="0" pos="210 80 40 30" tooltip="Click to edit the colours of the FM section."
              buttonText="FM" connectedEdges="14" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="fe1f29d9b8f708b3" memberName="button_colour_bg_svg_3"
              virtualName="" explicitFocusOrder="0" pos="290 80 80 30" tooltip="Click to edit the colours of the FX/EQ section."
              buttonText="FX/EQ" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="80f61ac5e64e6a34" memberName="button_colour_bg_svg_4"
              virtualName="" explicitFocusOrder="0" pos="210 110 190 30" tooltip="Click to edit the colours of the ARP section."
              buttonText="ARP" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="44f7c17040ebf13c" memberName="button_colour_bg_svg_5"
              virtualName="" explicitFocusOrder="0" pos="250 80 40 30" tooltip="Click to edit the colours of the MORPH section."
              buttonText="MO" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6727f30ac42caa0d" memberName="button_colour_bg_svg_6"
              virtualName="" explicitFocusOrder="0" pos="370 80 30 30" tooltip="Click to edit the colours of the MASTER OUT section."
              buttonText="V" connectedEdges="13" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c717d250fbd1db93" memberName="button_colour_bg_svg_7"
              virtualName="" explicitFocusOrder="0" pos="210 50 40 30" tooltip="Click to edit the colours of the OSC section."
              buttonText="O" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b19da151b3279272" memberName="copy" virtualName=""
              explicitFocusOrder="0" pos="870 135 50 30" tooltip="Copy current colour to clipboard."
              bgColOff="ff6495ed" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="COPY" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b0118ea77c7b965a" memberName="past" virtualName=""
              explicitFocusOrder="0" pos="920 135 50 30" tooltip="Past colour from clipboard."
              bgColOff="ff8a2be2" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="PAST" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7d731c5ccb2764a7" memberName="button_colour_buttons_off"
              virtualName="" explicitFocusOrder="0" pos="610 100 30 30" tooltip="Button OFF or BACKGROUND"
              buttonText="B" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3957685351d64b5" memberName="button_colour_buttons_font_off"
              virtualName="" explicitFocusOrder="0" pos="640 100 30 30" tooltip="Button font OFF"
              buttonText="F" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3bc5199278f4fec2" memberName="button_colour_slider_bg"
              virtualName="" explicitFocusOrder="0" pos="680 70 30 30" tooltip="Slider BACKGROUND"
              buttonText="B" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="dc03718552ee7376" memberName="button_remove_preset"
              virtualName="" explicitFocusOrder="0" pos="1020 135 85 30" tooltip="Delete selected theme."
              buttonText="DELETE" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="469942994b9d3ab6" memberName="label_11" virtualName=""
         explicitFocusOrder="0" pos="450 135 80 30" tooltip="Enable this option to change colours for an element over all sections."
         textCol="ffff3b00" edTextCol="ffff3b00" edBkgCol="0" labelText="FOR ALL"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="" id="6ce0794e47e964c1" memberName="toggle_for_all" virtualName=""
                explicitFocusOrder="0" pos="540 135 33 33" tooltip="Enable this option to change colours for an element over all sections."
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="" id="fc890749322b9a94" memberName="copy2" virtualName=""
              explicitFocusOrder="0" pos="610 135 50 30" tooltip="Copy this section colours to clipboard."
              bgColOff="ff6495ed" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="COPY" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="9b18f825a99d66b3" memberName="past2" virtualName=""
              explicitFocusOrder="0" pos="660 135 50 30" tooltip="Past section colours from clipboard."
              bgColOff="ff8a2be2" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="PAST" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="cb7727cac6ed885c" memberName="button_colour_slider_disabled"
              virtualName="" explicitFocusOrder="0" pos="650 70 30 30" tooltip="Slider DISABLED (currently in background)"
              buttonText="D" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="43f371944c5349a0" memberName="label_section" virtualName=""
         explicitFocusOrder="0" pos="180 0 250 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="SECTION: Background"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="3df2195659f98455" memberName="label_colour" virtualName=""
         explicitFocusOrder="0" pos="440 0 290 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="ELEMENT: Section Background"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="f80f96857b452ce6" memberName="colour_selector" virtualName="ColourSelector"
                    explicitFocusOrder="0" pos="760 40 210 90" class="ColourSelector"
                    params="ColourSelector::showColourspace&#10;,2,4"/>
  <LABEL name="" id="b88c1ab0963c74ff" memberName="label_colour2" virtualName=""
         explicitFocusOrder="0" pos="740 0 250 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="COLOUR SELECTOR"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="30" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="f08b39275a2e5ee1" memberName="label_section2" virtualName=""
         explicitFocusOrder="0" pos="1000 0 125 30" textCol="ffff3b00"
         edTextCol="ffff3b00" edBkgCol="0" labelText="THEMES" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="30" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="" id="7c9b1844748d88e" memberName="combo_theme" virtualName=""
            explicitFocusOrder="0" pos="1020 40 85 30" tooltip="Select and load a colour preset."
            editable="1" layout="33" items="" textWhenNonSelected="DEFAULT"
            textWhenNoItems="EMPTY BANK"/>
  <LABEL name="new label" id="420f5ac5a731bf77" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="760 135 110 30" tooltip="Past your RGB colour code here."
         edTextCol="ff000000" edBkgCol="0" labelText="FFFFFFFF&#10;" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="bffbca814f2563fa" memberName="button_colour_oszi_1"
              virtualName="" explicitFocusOrder="0" pos="620 40 30 30" tooltip="Oscilloscope 1 (Background of the Oszi  (BG-Section))"
              buttonText="O1" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="317270c2d409712f" memberName="button_colour_oszi_2"
              virtualName="" explicitFocusOrder="0" pos="650 40 30 30" tooltip="Oscilloscope 2 (only: Filter 2, OSC 2, Amp ENV)"
              buttonText="O2" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="fe0eb706386817fb" memberName="button_colour_oszi_3"
              virtualName="" explicitFocusOrder="0" pos="680 40 30 30" tooltip="Oscilloscope 3 (only: Filter 3, OSC 3)"
              buttonText="O3" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="9576828202c258dd" memberName="selected_section_marker"
              virtualName="" explicitFocusOrder="0" pos="470 220 6 6" bgColOff="ffff0000"
              buttonText="" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6d481b284cf86dd0" memberName="selected_element_marker"
              virtualName="" explicitFocusOrder="0" pos="210 230 6 6" bgColOff="ffff0000"
              buttonText="" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="108c0bad0a3c052f" memberName="credits_poper" virtualName="CreditsPoper"
                    explicitFocusOrder="0" pos="1130 50 320 160" class="Component"
                    params="parent_,true"/>
  <TEXTBUTTON name="" id="b6a23ff465364b08" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="1420 5 25 25" tooltip="Close setup."
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="fd31a701fd535e79" memberName="no_colour_in_demo_button"
              virtualName="" explicitFocusOrder="0" pos="190 30 790 145" bgColOff="6a929297"
              textCol="ff000000" textColOn="ff000000" buttonText="Pro Version Only"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
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

// JUCER_RESOURCE: au_logo_100x_png, 4848, "../Images/au_logo_100x.png"
static const unsigned char resource_Monique_Ui_GlobalSettings_au_logo_100x_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,121,0,0,0,120,8,3,0,0,0,225,120,173,222,0,0,2,238,80,76,84,69,255,
255,255,254,254,254,255,254,253,253,254,254,252,252,252,255,252,250,255,251,248,255,248,243,249,250,249,255,247,234,254,245,241,245,246,246,255,244,227,249,243,242,244,244,244,254,243,236,255,242,216,
242,243,243,241,241,242,252,238,236,240,240,240,255,239,207,239,239,239,254,236,226,245,238,230,237,237,238,254,235,219,247,234,234,254,235,195,235,235,235,253,231,226,253,229,209,253,228,219,255,231,
183,231,232,233,245,227,228,254,232,142,230,230,231,227,229,229,228,228,228,253,222,211,254,226,169,254,230,131,223,228,235,227,227,227,226,226,226,253,222,194,254,227,118,223,223,223,252,217,202,222,
224,224,255,224,107,254,220,151,252,216,183,255,224,97,219,219,219,250,211,208,252,211,195,255,221,89,255,220,82,254,215,136,255,219,99,215,215,215,214,216,217,252,208,168,220,214,207,211,215,218,255,
217,76,255,215,90,254,210,123,255,215,71,237,209,170,251,204,184,211,211,211,255,211,82,255,211,68,254,207,113,255,210,76,250,199,196,251,203,157,204,208,214,207,207,207,255,206,66,253,202,99,250,194,
171,255,203,73,232,201,144,237,195,195,204,203,203,199,203,209,251,195,140,255,201,62,252,198,84,201,200,200,249,188,182,240,199,93,196,200,204,208,199,181,255,196,59,199,198,198,232,195,132,250,186,161,
250,189,132,253,194,75,247,192,90,254,193,67,196,195,195,254,192,56,222,193,134,188,195,207,254,189,54,228,192,105,250,184,121,253,189,63,253,188,57,193,192,192,231,181,181,253,186,51,249,177,147,253,
184,53,253,185,47,188,188,188,214,187,122,253,183,41,250,177,109,181,188,199,246,171,165,252,178,61,253,181,36,197,184,152,253,180,33,252,178,51,246,168,153,189,183,165,248,168,136,183,182,182,249,171,
99,229,167,168,251,171,48,249,165,87,178,177,177,248,159,122,250,165,47,244,157,137,226,159,161,173,172,172,242,153,153,248,159,76,249,160,46,246,153,117,248,155,69,170,168,169,248,156,44,247,150,106,
248,153,64,167,165,166,247,153,43,243,143,142,248,150,60,247,148,86,247,148,55,163,162,163,247,147,51,246,149,42,246,141,97,246,146,42,243,139,109,247,143,46,247,144,48,246,142,67,159,157,158,245,143,
41,246,141,53,244,141,42,247,140,41,245,138,47,245,133,87,247,137,35,244,136,45,153,152,152,245,137,40,218,134,137,245,133,77,235,129,130,245,133,41,246,134,29,244,133,49,243,130,60,244,128,40,146,145,
145,244,123,72,242,119,84,142,140,140,243,120,41,244,116,62,237,109,106,243,111,40,244,109,53,134,131,132,242,103,76,239,101,94,243,104,45,211,106,110,243,100,40,243,97,36,241,93,62,243,94,33,123,121,
121,242,90,26,240,89,45,234,85,88,212,89,94,115,113,113,240,83,40,112,110,110,220,78,84,110,108,108,239,73,55,239,72,39,205,78,84,107,104,105,239,68,69,103,101,101,101,98,99,238,61,39,98,95,96,95,92,93,
196,60,67,238,49,31,89,86,87,236,45,41,208,47,55,80,77,78,232,30,36,73,70,71,234,26,29,213,30,38,68,64,65,187,30,39,64,61,62,62,58,59,55,52,53,196,12,23,51,47,48,46,43,43,41,37,38,35,31,32,29,25,26,24,
20,21,14,10,11,22,235,138,14,0,0,0,1,98,75,71,68,0,136,5,29,72,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,0,7,116,73,77,69,7,222,4,6,12,49,21,150,145,52,222,0,0,15,136,73,68,65,84,104,
222,237,154,127,76,83,247,22,192,129,75,3,22,165,8,17,94,157,10,178,17,96,11,88,51,28,115,111,50,222,30,230,169,241,199,26,220,166,4,77,22,3,4,217,80,146,133,64,102,36,75,124,192,164,93,130,48,160,16,
160,54,52,197,255,52,36,132,191,88,165,133,169,55,147,100,79,140,113,27,129,4,180,105,150,20,104,122,111,238,127,239,156,243,189,183,220,210,162,110,35,239,229,189,112,16,40,237,189,223,207,247,156,239,
57,231,123,190,231,26,197,253,183,36,106,147,188,73,222,36,111,146,255,207,200,71,234,55,24,18,87,127,240,149,200,25,227,110,115,202,70,130,211,44,252,248,158,87,32,235,134,220,110,247,96,206,198,129,
115,70,121,158,183,188,2,185,158,7,242,67,139,118,163,192,186,161,135,48,160,187,254,165,228,131,147,120,221,248,6,234,204,70,156,216,255,18,114,10,155,225,105,120,25,19,183,1,190,133,63,42,220,145,172,
184,150,124,154,192,102,124,153,127,57,253,207,130,243,47,231,227,47,244,28,183,251,200,11,201,25,19,184,202,227,232,218,233,109,119,58,242,255,28,248,64,215,157,214,173,56,42,218,155,31,79,123,1,89,211,
252,80,153,157,230,178,221,106,239,45,214,252,113,174,166,204,106,179,218,43,87,237,93,241,2,50,77,206,61,132,184,226,225,97,171,213,222,166,251,227,228,173,29,118,43,176,15,192,75,237,32,57,89,218,186,
100,77,51,218,154,71,191,78,239,176,193,109,93,235,155,91,19,187,45,229,181,148,148,109,177,209,235,175,114,175,21,38,223,138,188,131,52,112,69,204,122,228,52,50,74,51,168,172,169,188,3,119,89,79,225,
187,91,194,45,190,237,245,79,251,93,63,255,6,242,243,131,254,79,95,143,15,159,215,22,252,89,6,179,183,222,193,65,180,102,92,198,137,148,245,200,245,228,131,25,56,221,1,188,169,21,153,73,229,185,107,208,
175,125,249,224,183,149,149,223,158,147,192,171,159,31,124,250,218,154,96,42,40,69,207,138,107,67,195,245,98,136,228,184,149,96,141,68,214,78,160,202,102,136,65,237,21,59,170,140,183,196,29,175,173,125,
127,139,154,251,205,191,86,86,158,255,228,234,191,118,237,218,55,215,174,245,187,126,122,190,178,226,250,114,155,234,146,164,195,13,13,239,147,189,113,148,59,149,48,115,29,41,61,26,19,153,124,4,167,197,
99,174,201,71,176,189,12,117,205,173,173,169,105,56,158,20,188,232,164,11,184,174,27,231,79,158,103,114,242,228,213,27,174,231,43,191,61,120,55,120,201,142,210,218,186,186,186,189,20,32,54,69,131,35,20,
53,57,145,201,22,74,53,58,229,134,94,116,141,164,42,28,165,90,33,107,251,159,175,44,184,110,156,190,170,150,243,87,111,220,123,182,242,243,55,138,175,237,170,1,169,45,199,123,210,7,72,5,80,53,109,144,
87,82,84,24,57,109,28,201,184,20,59,7,200,51,208,211,222,111,0,112,93,174,2,126,240,108,97,174,255,218,213,102,148,122,252,2,129,151,87,175,186,230,86,158,245,43,254,80,128,247,212,22,196,160,167,218,
21,29,234,81,233,241,184,72,228,211,147,152,104,114,100,159,180,117,237,68,195,225,24,117,199,229,33,181,174,133,133,57,87,179,153,4,128,102,250,7,127,195,47,215,220,194,179,7,242,50,106,74,235,20,67,
165,247,0,217,134,49,189,31,189,104,242,72,36,178,133,71,255,194,173,162,3,85,46,35,149,209,214,117,178,173,183,33,248,251,239,64,44,12,222,106,86,4,222,251,97,97,225,89,191,140,222,193,148,134,17,226,
72,233,54,156,182,5,149,110,142,64,214,141,226,66,84,200,25,192,102,197,36,146,84,131,35,228,202,227,245,47,44,44,252,120,123,8,164,205,210,166,22,139,197,50,244,221,237,167,160,245,151,178,210,52,227,
106,140,172,124,114,153,157,242,198,207,143,106,194,201,7,39,148,125,185,18,189,226,10,120,90,76,1,146,171,182,178,11,206,131,198,79,239,223,30,29,28,28,236,82,164,3,190,219,224,27,222,187,125,31,208,
207,79,202,113,85,167,76,121,103,171,93,78,73,171,128,53,100,76,35,15,41,101,99,252,223,41,198,236,85,5,247,127,37,171,156,246,43,128,103,238,143,223,186,117,171,171,107,0,254,245,118,245,210,11,250,190,
117,107,220,61,3,232,123,178,210,5,232,153,229,232,79,101,144,13,109,173,104,84,244,238,201,211,225,100,12,117,30,151,57,189,203,6,137,7,141,189,23,195,67,137,168,126,4,243,147,19,183,110,89,173,3,161,
114,107,192,10,228,73,30,209,215,228,149,38,7,217,33,167,67,91,7,122,55,46,52,223,28,70,214,98,49,66,51,42,238,197,60,143,155,212,97,184,185,225,48,11,132,140,95,23,158,62,229,239,79,142,57,48,55,88,7,
66,190,80,198,238,242,128,94,184,199,118,183,45,199,17,93,128,33,138,38,36,69,200,170,150,48,242,30,116,176,9,90,102,52,54,110,171,49,104,236,134,189,236,243,27,160,50,207,187,239,202,228,48,113,140,221,
157,116,243,51,115,11,87,217,245,185,95,161,185,241,213,229,144,133,30,221,179,150,204,214,31,141,210,106,31,24,176,225,50,39,85,163,127,49,99,167,221,91,152,153,1,240,93,135,195,97,99,98,181,118,118,
14,116,118,210,203,225,97,7,160,193,152,51,11,46,205,106,96,209,74,149,217,209,97,89,33,15,228,140,181,100,74,218,163,232,8,109,193,101,6,114,237,113,182,91,156,68,239,2,141,199,28,142,225,97,2,119,170,
101,24,208,99,160,245,125,126,238,87,150,192,183,98,242,174,222,133,53,17,120,130,29,183,61,221,168,58,151,4,201,148,220,44,114,77,96,235,192,36,255,62,122,118,1,251,248,218,2,104,60,9,224,59,164,115,
231,90,1,242,29,212,250,254,204,220,121,230,221,135,107,229,180,155,143,3,182,97,104,142,170,11,193,80,50,58,94,126,151,109,128,57,216,113,76,252,44,101,235,92,115,51,247,39,209,212,160,221,48,177,190,
149,133,254,232,233,177,219,237,104,112,158,159,187,193,162,176,0,23,250,176,236,98,54,42,38,105,75,170,95,75,166,253,19,223,45,38,227,224,221,229,24,83,187,216,50,187,158,146,119,57,236,61,61,61,106,
174,2,239,65,113,56,192,222,79,93,172,242,216,91,43,187,152,14,28,135,213,85,205,145,200,26,36,211,26,20,91,101,135,216,130,228,170,29,44,166,126,156,193,128,114,144,157,25,174,41,40,65,56,121,217,140,
139,21,122,187,170,225,246,82,28,154,178,216,129,87,32,163,206,72,222,90,5,183,150,51,215,126,87,86,217,70,220,235,215,155,214,8,177,123,236,168,52,255,189,138,92,142,254,73,5,78,177,18,208,230,152,151,
147,203,85,228,57,126,82,1,175,114,235,224,75,141,182,217,112,165,127,96,17,155,68,38,219,170,38,227,62,252,208,172,121,37,114,109,105,80,103,36,51,48,145,235,130,18,100,3,26,148,14,146,49,172,202,67,
200,71,126,7,89,209,57,103,230,254,93,135,173,7,201,161,88,21,28,208,176,212,99,110,197,218,180,88,33,228,138,63,64,206,248,193,125,215,97,149,77,45,227,170,101,81,163,193,220,252,80,138,106,157,99,212,
100,242,176,230,136,30,86,161,38,107,212,81,117,219,61,6,171,76,26,203,216,143,101,81,177,65,105,219,152,251,59,153,92,23,140,42,155,245,5,81,165,202,36,189,112,14,107,197,13,170,20,55,73,182,97,232,190,
227,199,172,214,32,88,193,158,80,224,12,141,74,143,241,245,204,156,185,184,67,31,71,219,5,51,137,121,125,178,57,152,236,112,173,14,3,88,201,158,21,64,38,149,21,238,137,160,16,91,70,119,90,199,220,114,
31,136,106,131,130,96,58,198,122,104,80,125,206,8,146,79,195,22,200,15,134,36,187,92,32,55,28,102,26,236,119,143,89,7,86,193,39,78,252,93,37,178,218,140,44,111,70,108,131,206,149,141,104,107,213,194,242,
225,70,28,190,99,160,199,187,71,177,204,199,101,233,61,64,62,2,123,149,28,86,105,150,187,54,70,174,102,250,254,85,17,153,77,228,166,206,225,49,51,235,74,36,85,41,245,204,41,27,86,26,216,9,25,87,247,75,
130,228,156,113,94,174,207,112,39,183,151,225,180,209,134,181,187,100,115,223,181,118,94,191,14,111,16,24,136,239,129,188,3,223,132,63,65,90,55,93,239,181,142,21,179,235,247,162,177,171,208,181,177,238,
181,85,42,21,247,104,250,90,50,157,62,38,208,20,167,40,113,163,145,177,96,111,40,96,230,206,24,116,244,94,39,149,101,133,223,145,5,103,128,90,51,178,163,45,69,174,123,27,228,173,74,135,105,187,87,9,103,
126,72,27,86,1,90,148,205,42,191,11,182,29,218,160,113,161,89,50,64,23,116,12,92,111,170,38,91,171,184,50,156,236,93,141,100,217,129,200,216,20,24,249,88,80,118,165,43,174,109,14,175,61,177,69,66,245,
153,150,218,5,184,208,91,107,84,230,222,217,102,237,104,98,42,191,7,180,183,65,246,193,23,136,140,70,178,173,85,163,50,118,29,238,23,197,88,245,182,225,25,107,136,87,119,228,86,201,232,98,60,213,103,87,
172,114,9,168,193,218,160,166,84,62,61,31,176,126,139,58,19,24,168,251,222,68,217,247,230,62,134,62,1,62,214,212,213,43,183,55,104,135,173,193,184,136,163,2,240,50,121,82,72,67,46,74,221,104,85,229,79,
86,15,237,106,80,41,205,85,14,16,185,164,4,193,64,125,227,205,55,64,128,14,232,146,19,168,115,167,236,94,49,84,22,212,238,8,118,31,112,66,167,241,116,62,170,139,112,162,67,99,80,86,77,195,206,138,124,
200,32,165,229,163,103,220,229,142,234,186,42,34,239,35,40,19,66,151,28,251,184,170,250,219,50,121,239,165,195,73,29,221,119,10,27,46,29,26,229,40,97,137,116,150,196,125,155,31,194,57,93,198,147,39,149,
98,123,191,82,31,159,211,174,52,85,87,29,43,41,121,251,237,55,85,228,55,128,124,168,228,88,85,85,83,165,114,58,166,243,88,3,218,42,141,156,166,146,108,202,203,59,67,24,153,194,141,74,67,106,111,216,243,
149,218,85,169,185,97,156,202,166,170,242,146,146,67,97,58,35,185,233,148,210,78,217,65,137,128,234,229,3,184,202,182,116,74,146,74,65,31,70,142,25,87,54,177,184,54,10,105,173,82,199,49,187,225,165,127,
57,133,74,31,10,81,90,81,185,56,65,30,142,182,215,154,154,29,114,241,110,181,93,9,182,104,134,34,247,73,42,168,59,153,161,204,148,148,102,101,115,21,6,102,116,108,66,98,162,254,31,165,10,90,246,47,25,
92,154,159,154,152,16,79,227,229,226,206,76,205,10,174,216,166,12,148,51,185,166,45,165,38,167,77,42,141,18,29,174,142,181,67,203,90,52,181,4,142,226,226,19,147,245,89,111,253,237,147,210,99,199,14,29,
66,247,6,7,223,7,65,5,224,210,79,12,111,237,78,221,158,192,58,130,185,112,15,53,196,116,189,65,227,85,80,43,78,187,78,39,206,66,39,28,92,138,3,232,145,54,106,1,238,173,41,165,202,23,84,222,158,186,59,
219,80,248,225,103,159,95,42,133,197,6,248,219,135,0,91,82,122,233,147,207,62,48,228,101,233,147,101,165,185,93,229,117,20,138,149,65,149,247,80,19,185,121,189,30,224,126,94,81,122,43,213,200,20,134,220,
94,230,95,104,236,228,221,89,121,134,162,179,103,27,191,254,252,210,165,82,146,75,159,127,254,245,197,179,31,126,96,200,206,4,157,149,62,100,18,21,20,249,212,99,66,149,99,104,251,119,103,172,71,142,163,
22,56,117,183,243,241,172,104,107,83,247,225,9,173,207,204,54,124,112,244,236,23,95,180,52,254,243,107,144,127,54,54,126,113,246,163,34,208,120,55,129,67,86,143,53,31,243,149,133,228,205,154,117,187,204,
164,52,95,31,108,100,217,42,85,215,70,69,199,198,3,59,117,119,86,150,161,176,168,232,35,38,31,22,129,161,179,51,245,122,224,198,114,234,198,175,230,10,217,26,247,219,24,51,175,180,23,215,33,235,176,51,
229,158,220,163,180,153,89,61,166,98,115,177,9,9,9,219,183,167,166,238,6,126,54,74,102,102,166,62,57,57,17,252,58,54,58,116,48,106,22,176,179,97,6,245,53,205,218,23,60,77,160,62,52,29,102,185,3,80,196,
92,14,107,172,35,60,54,62,30,248,16,98,36,9,9,241,241,177,177,240,73,216,99,160,180,43,114,205,169,101,15,50,114,94,244,28,35,142,226,157,53,112,202,122,79,69,126,150,16,133,178,206,95,161,93,238,202,
222,83,171,15,19,234,53,47,124,118,179,127,18,181,158,64,39,140,203,255,19,15,49,100,52,13,145,67,105,121,60,227,37,79,202,42,120,68,15,233,184,13,147,20,234,242,242,167,95,246,140,142,186,83,170,182,
213,159,22,45,243,107,139,230,165,207,37,247,99,182,121,88,191,145,100,62,210,227,198,8,190,81,129,46,166,217,56,107,199,85,132,63,160,91,231,201,183,121,60,39,102,35,159,63,199,28,156,104,126,181,103,
238,58,29,183,193,178,71,179,249,63,28,54,201,155,228,77,242,38,249,63,76,222,94,88,4,197,116,116,252,239,27,62,254,21,201,241,166,190,246,172,117,46,188,224,241,25,184,212,233,217,139,225,31,37,155,186,
207,208,139,115,125,166,132,144,79,186,231,71,94,141,108,8,4,196,190,117,46,108,17,164,66,174,69,146,158,132,91,201,176,20,152,162,23,211,130,55,59,228,19,191,180,108,164,67,194,203,200,35,162,32,61,74,
136,76,110,244,3,217,224,151,34,40,97,88,22,24,249,145,232,11,37,63,17,23,147,185,196,246,249,190,151,144,163,60,126,175,232,59,26,177,206,101,228,168,228,212,248,224,59,42,157,133,48,157,217,199,9,250,
68,142,51,250,196,145,240,155,66,200,133,1,223,136,79,234,198,5,191,104,50,192,47,125,203,197,221,184,142,141,78,231,81,19,146,19,90,90,10,233,202,190,169,145,70,125,100,114,161,201,152,104,28,113,182,
0,147,51,182,156,227,178,156,203,194,163,150,11,241,92,236,69,167,179,175,48,34,249,166,224,201,252,69,122,148,204,113,89,62,178,234,69,113,249,2,199,29,245,136,146,40,46,45,3,57,91,146,156,184,42,1,73,
16,37,207,209,136,100,167,228,121,34,73,162,244,120,59,199,205,75,30,248,59,16,144,164,165,212,196,71,34,44,166,55,34,121,81,120,194,221,148,252,48,173,76,47,145,47,136,190,115,92,252,162,24,152,30,153,
15,8,98,33,151,37,34,25,220,225,81,159,115,73,242,102,69,34,247,1,96,118,26,102,11,198,155,21,61,177,23,60,126,193,243,196,201,117,139,254,169,155,211,206,72,100,112,81,19,119,110,89,106,95,37,11,64,238,
14,224,50,109,127,44,74,50,57,111,73,156,199,136,9,122,91,40,185,197,47,78,115,92,145,79,122,156,140,100,61,103,244,138,55,241,195,224,76,195,200,35,129,229,188,132,188,121,244,110,53,121,90,90,2,187,
113,38,65,50,48,242,77,127,160,17,222,200,242,136,243,81,17,200,228,16,92,252,148,52,191,155,145,207,49,15,27,145,132,233,11,145,201,139,66,96,126,113,113,73,88,206,84,145,207,112,179,210,98,130,18,85,
68,30,17,252,20,121,191,72,243,44,78,11,85,228,60,36,231,193,184,78,105,86,31,66,206,94,132,213,126,156,23,129,92,228,21,4,73,146,132,128,216,18,66,158,15,35,7,2,100,182,89,50,58,138,79,152,150,195,215,
19,133,228,108,34,207,135,146,57,125,159,87,20,231,147,195,201,35,66,224,130,94,159,8,171,242,8,201,168,69,31,179,246,50,94,221,46,40,228,110,127,160,5,222,200,243,136,191,200,185,201,3,142,4,191,18,189,
226,44,39,235,204,77,5,117,62,227,19,152,67,68,37,58,5,191,41,156,252,68,244,82,150,120,44,46,39,234,23,37,79,17,151,229,21,124,23,184,155,1,193,153,28,101,88,12,122,88,145,79,92,132,88,118,138,162,105,
117,210,35,209,177,177,206,64,160,155,11,215,25,28,98,42,33,158,75,76,141,226,140,130,191,47,140,92,4,106,70,179,168,22,219,193,29,68,239,244,146,31,227,57,121,81,20,61,211,94,49,32,53,202,241,60,18,16,
61,221,211,96,185,68,249,222,84,48,163,103,202,35,162,202,232,138,160,115,244,20,146,33,158,245,92,204,162,36,204,142,112,211,75,35,55,23,37,111,97,24,185,197,187,116,142,121,234,188,111,138,203,154,245,
11,130,183,207,183,8,9,255,220,188,63,32,120,157,191,44,195,250,47,249,201,77,189,2,164,150,233,96,18,227,140,139,203,130,32,248,30,165,162,67,120,151,193,13,162,70,2,211,169,64,155,133,183,76,62,63,88,
105,42,32,138,1,143,41,220,195,242,206,24,99,217,59,133,198,34,204,152,237,237,133,156,177,8,3,42,181,209,100,42,226,12,70,61,23,107,52,162,115,69,21,181,155,186,207,133,236,175,141,237,166,118,35,217,
44,245,168,17,87,45,235,12,228,95,131,145,84,60,211,222,125,49,90,15,151,152,178,54,171,161,77,242,38,121,147,188,73,222,36,255,239,144,255,13,146,159,133,80,37,28,6,136,0,0,0,0,73,69,78,68,174,66,96,
130,0,0};

const char* Monique_Ui_GlobalSettings::au_logo_100x_png = (const char*) resource_Monique_Ui_GlobalSettings_au_logo_100x_png;
const int Monique_Ui_GlobalSettings::au_logo_100x_pngSize = 4848;

// JUCER_RESOURCE: aax_logo_100x_png, 12573, "../Images/aax_logo_100x.png"
static const unsigned char resource_Monique_Ui_GlobalSettings_aax_logo_100x_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,100,0,0,0,100,8,6,0,0,0,112,226,149,84,0,0,0,6,98,75,71,68,0,255,
0,255,0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,46,35,0,0,46,35,1,120,165,63,118,0,0,0,7,116,73,77,69,7,224,1,24,17,55,1,136,185,190,7,0,0,32,0,73,68,65,84,120,218,237,157,121,128,28,85,185,246,
127,103,169,234,101,214,132,132,16,1,65,32,18,68,100,17,217,84,20,36,17,3,136,162,65,65,68,192,245,162,8,110,184,11,42,226,85,80,84,80,132,15,174,168,232,5,17,55,4,65,63,69,133,171,92,65,4,9,59,33,251,
158,204,218,51,189,84,87,157,239,143,58,117,106,233,158,73,4,68,255,248,58,233,233,174,234,238,154,158,243,212,243,238,239,91,162,119,96,128,218,232,40,64,105,222,252,249,175,25,27,25,249,192,228,196,
196,75,35,99,20,255,255,246,79,187,9,8,43,213,234,93,189,253,253,223,92,246,196,19,63,3,234,189,3,3,8,251,186,55,127,239,189,31,154,216,188,121,143,209,145,17,130,86,11,99,204,191,219,31,144,123,38,186,
188,18,223,76,230,103,183,237,127,159,155,239,251,84,122,123,233,221,110,187,229,75,31,127,252,37,192,102,1,148,230,239,189,247,146,198,208,208,30,107,215,173,67,254,91,129,32,237,163,112,123,68,87,16,
166,186,153,12,16,17,32,48,68,29,175,252,43,111,6,120,206,115,158,3,149,202,234,101,75,151,238,41,230,205,159,255,186,198,240,240,79,55,108,216,240,47,5,35,93,106,97,127,218,109,33,50,172,16,93,62,177,
117,40,210,231,134,152,248,198,237,139,31,205,191,20,32,3,244,244,246,82,153,57,243,157,98,206,14,59,252,161,62,60,124,120,171,217,252,23,2,33,220,63,16,14,4,209,133,21,162,43,56,221,229,91,22,136,206,
159,9,56,150,49,98,42,96,4,162,203,175,18,217,157,162,112,170,8,65,20,69,132,97,72,100,204,54,241,89,123,30,94,95,223,18,209,215,215,215,110,215,106,234,217,214,25,2,217,1,130,200,50,131,132,25,201,130,
8,251,95,34,186,104,146,110,42,37,62,134,193,136,236,107,166,243,185,36,214,153,34,89,208,100,191,176,15,246,251,9,1,66,34,164,112,251,164,148,96,31,165,125,20,66,16,134,33,97,59,164,94,159,100,124,124,
124,155,214,68,245,244,24,29,25,243,172,130,17,47,121,6,12,33,187,2,209,177,79,8,7,164,44,136,174,220,83,187,152,201,231,13,6,33,227,115,63,57,158,17,241,162,11,4,70,218,253,50,11,136,72,206,5,132,187,
11,132,82,110,193,29,0,74,33,132,64,41,229,182,149,189,99,32,8,90,76,76,76,176,101,243,102,234,245,250,244,162,203,24,161,159,125,70,200,248,15,44,50,164,155,232,74,196,129,20,133,163,76,161,237,36,57,
48,28,59,68,172,206,133,101,89,204,12,1,210,238,23,241,123,5,2,35,76,140,189,16,49,83,44,56,82,166,223,73,100,24,35,68,158,45,201,93,8,129,210,138,82,185,196,192,224,32,253,3,3,172,93,189,154,145,145,
145,233,69,215,179,202,8,145,62,79,151,45,221,150,153,211,82,218,37,146,82,216,229,203,255,235,16,87,146,14,77,99,28,177,140,91,72,147,48,72,88,246,8,17,179,202,62,10,11,132,99,137,76,183,133,4,33,83,
16,178,98,42,190,43,247,60,97,137,178,207,183,219,110,22,213,158,30,30,121,232,33,198,199,199,167,212,43,162,167,183,215,132,181,218,63,77,89,75,100,252,40,100,102,59,207,144,24,44,217,105,234,202,44,
87,18,248,100,167,184,146,5,169,69,42,114,18,81,149,130,97,210,231,130,148,29,25,96,146,231,49,0,246,61,210,126,59,21,179,71,42,153,138,42,41,81,82,33,149,180,219,10,173,227,253,90,107,148,210,40,37,241,
60,143,225,225,97,238,187,247,94,194,48,236,36,120,181,250,207,97,72,158,21,50,179,148,2,114,64,72,107,161,136,14,241,149,136,169,20,216,244,103,214,242,73,197,89,34,136,12,2,233,20,185,116,0,36,107,154,
2,18,235,152,120,209,227,253,210,62,90,162,202,68,92,37,162,51,101,136,16,64,194,134,2,24,42,7,134,66,169,248,185,84,138,237,231,204,161,127,96,128,225,161,161,103,71,100,117,50,66,117,40,242,148,17,69,
198,216,69,23,169,34,239,6,136,232,194,140,100,51,97,150,131,72,164,38,170,17,6,41,68,10,156,16,8,105,48,34,3,176,72,23,223,125,86,36,64,196,224,196,159,147,8,73,44,162,148,204,136,42,137,18,18,157,128,
163,84,12,140,214,104,173,209,74,49,115,230,76,134,134,134,186,138,45,253,204,131,33,145,66,102,196,75,202,12,41,68,102,59,21,103,29,230,175,20,57,179,87,22,88,151,174,92,222,206,18,206,84,21,206,219,
144,194,26,183,66,32,139,150,85,70,199,64,6,4,105,21,126,242,122,98,230,42,11,76,114,23,177,142,19,89,189,33,101,204,16,165,209,58,177,184,98,166,104,165,240,60,143,129,193,193,127,190,82,119,76,16,210,
46,181,156,82,116,77,201,24,129,123,31,157,66,44,21,89,110,33,243,231,152,148,169,22,1,99,173,168,248,253,70,24,36,2,35,99,49,134,101,133,145,246,19,93,152,97,221,142,24,144,4,36,251,92,42,225,158,11,
41,144,186,160,204,149,180,224,104,148,142,193,208,158,135,86,10,223,243,18,59,239,159,1,136,112,139,47,45,24,201,189,155,66,239,230,135,200,130,79,146,10,48,210,119,139,84,255,32,221,57,157,126,11,153,
234,155,196,247,72,62,111,36,40,19,139,166,68,11,25,103,202,154,152,85,194,56,177,148,138,169,4,140,132,25,88,81,101,247,41,133,148,196,250,67,74,132,138,77,221,14,49,165,53,158,5,67,106,77,43,8,166,180,
178,244,51,165,188,165,72,116,133,202,168,112,129,16,42,35,190,138,86,87,162,65,164,19,83,221,188,119,233,120,34,83,107,39,251,61,100,114,182,201,188,9,236,78,125,227,20,180,147,116,50,19,22,201,128,97,
165,107,44,94,165,5,45,97,66,34,178,132,180,12,145,40,37,172,14,177,143,74,162,180,180,34,42,214,25,169,114,143,51,26,211,133,169,244,211,131,35,11,70,34,166,82,17,149,103,68,214,49,20,57,177,22,155,183,
89,95,69,116,209,26,210,90,84,41,115,12,206,111,115,239,41,138,32,18,141,146,60,23,89,101,93,244,67,210,48,9,137,201,107,153,33,51,74,92,168,172,238,16,40,25,123,232,82,91,51,88,197,122,68,105,229,24,
162,148,114,223,161,86,171,61,243,12,145,5,102,200,194,130,23,173,43,105,255,194,14,177,37,243,32,209,225,50,138,28,104,89,201,43,147,69,204,43,145,140,162,150,49,24,9,27,164,113,172,112,64,56,230,20,
116,134,178,192,41,171,188,19,157,161,172,35,104,25,34,149,213,31,202,90,88,90,218,240,137,116,98,75,217,240,138,13,143,208,152,38,132,162,159,142,2,239,100,134,204,49,35,191,95,228,116,75,12,134,204,
4,21,101,206,248,205,1,34,19,19,88,230,34,176,177,111,97,25,36,101,70,132,37,62,70,30,4,156,207,129,85,232,34,198,67,166,214,148,112,138,61,6,37,49,111,133,83,228,169,152,18,74,160,172,152,146,58,15,136,
246,36,202,203,131,1,16,69,17,19,19,19,207,28,32,121,5,222,141,25,201,254,172,213,165,50,103,187,85,246,178,232,189,139,156,53,21,175,143,76,173,31,107,66,39,241,173,132,25,194,45,182,213,70,34,9,160,
75,100,2,76,162,99,100,106,157,165,32,88,17,39,51,254,134,85,218,88,179,54,214,31,22,132,196,247,144,201,182,5,194,222,165,82,40,47,182,170,148,148,78,135,37,183,86,171,245,204,1,146,248,21,194,89,83,
106,10,102,164,12,34,39,180,236,18,202,60,99,114,64,100,20,185,11,224,101,60,252,4,12,167,105,68,86,49,196,79,165,72,77,223,228,108,119,17,252,132,9,89,197,174,172,251,162,172,217,45,201,232,9,114,250,
34,81,224,194,90,86,82,199,34,75,103,116,135,214,10,169,85,204,66,34,7,135,148,146,13,235,215,19,68,17,234,233,2,226,204,209,156,105,43,156,34,238,6,70,209,234,138,153,145,58,142,25,190,185,227,96,249,
132,13,224,229,34,193,66,186,197,21,78,23,224,60,184,68,193,155,228,61,50,147,202,74,130,132,50,49,139,179,226,201,90,83,9,67,172,168,114,128,56,241,36,156,152,18,82,88,7,48,35,166,180,245,59,44,147,12,
134,36,134,147,4,50,159,120,252,241,105,51,179,250,31,49,111,243,98,202,50,66,200,20,132,12,24,177,112,201,239,143,153,33,115,113,45,217,213,105,180,158,177,243,70,100,10,134,72,205,222,52,142,37,29,24,
9,18,194,5,15,165,11,20,186,144,76,226,133,219,5,79,124,13,103,222,38,214,149,51,103,19,145,149,154,182,29,58,67,75,148,167,28,120,177,152,138,64,72,140,137,179,146,99,99,99,108,220,184,113,218,12,162,
254,71,60,142,156,19,56,213,163,101,70,17,12,41,149,91,120,153,243,222,243,108,147,9,112,100,25,34,211,40,172,19,103,121,23,61,117,210,51,193,63,145,70,106,69,198,146,202,133,71,100,186,223,121,221,82,
58,31,196,49,194,90,87,66,146,49,109,165,13,34,42,103,230,38,204,48,153,160,142,20,130,200,24,150,63,249,228,214,83,185,219,230,109,164,76,192,46,32,25,167,47,171,71,200,137,169,20,12,41,242,62,73,222,
123,207,68,132,165,180,150,104,38,50,236,236,209,172,143,49,61,24,34,227,79,36,198,129,144,210,154,186,169,227,151,196,163,80,164,44,177,34,73,170,244,46,156,210,22,40,45,237,182,114,78,96,28,225,21,22,
138,40,19,93,139,19,0,24,88,189,114,245,86,243,235,219,0,136,44,68,147,68,206,202,202,199,166,84,102,171,27,24,170,75,136,37,181,178,148,76,45,53,199,202,84,25,116,5,67,100,194,87,91,3,35,62,84,156,106,
45,138,43,23,48,76,252,13,157,58,129,9,16,66,9,164,86,104,37,17,78,103,88,81,229,192,72,129,176,81,51,164,132,117,107,215,50,58,60,234,210,202,79,9,16,209,37,70,69,134,9,121,93,146,183,166,138,96,196,
236,42,58,146,169,143,34,101,122,180,52,185,157,130,193,54,50,195,85,173,136,12,24,34,1,35,201,240,165,17,218,68,161,39,86,21,206,249,43,234,13,145,58,125,9,16,58,54,113,211,175,24,97,50,114,197,16,33,
17,132,237,54,15,254,125,137,229,138,196,16,62,85,64,10,114,219,41,219,226,89,44,11,198,173,180,103,100,202,140,60,24,170,67,124,137,98,174,92,228,67,236,114,27,152,129,77,249,38,96,164,74,61,181,154,
164,141,69,37,166,119,162,164,179,58,35,231,141,43,137,180,122,67,37,202,92,203,152,41,25,49,101,136,92,89,69,18,224,148,8,34,12,107,86,173,101,243,166,45,40,161,136,76,52,45,75,244,214,3,135,50,245,63,
156,92,239,98,109,101,116,11,206,154,234,12,62,230,44,50,97,99,94,162,80,73,82,144,77,233,83,43,134,178,204,96,10,48,10,226,42,1,3,17,203,127,153,128,145,20,37,88,15,220,41,111,141,139,224,166,166,109,
226,0,38,186,67,58,29,17,217,37,78,152,145,60,70,161,225,177,135,30,141,193,32,178,169,123,51,37,75,244,180,226,74,228,11,211,10,134,105,33,34,155,53,131,45,64,70,80,174,84,216,117,222,46,246,29,138,225,
77,35,108,94,187,5,161,99,163,64,10,81,40,235,233,4,35,27,161,149,174,114,4,162,48,226,121,123,237,130,239,251,8,1,97,24,178,226,177,85,177,130,46,48,35,49,12,146,194,132,216,143,208,204,217,105,54,82,
42,70,54,143,96,140,201,177,34,49,109,99,231,79,57,7,80,121,150,85,2,182,159,59,11,169,4,245,70,147,141,235,215,199,32,91,128,164,144,44,95,186,156,161,205,195,78,164,135,133,18,165,109,2,36,171,168,41,
36,153,242,81,222,188,114,38,147,237,147,86,135,92,241,243,203,216,255,208,253,72,234,208,148,167,88,184,219,241,12,111,28,233,14,134,156,2,12,145,73,221,10,48,145,224,181,167,190,134,115,47,57,135,176,
29,58,79,248,154,175,92,203,181,95,191,14,116,98,106,167,89,72,233,114,25,18,237,105,190,125,235,55,232,31,236,5,96,211,250,45,124,104,241,199,208,94,201,154,181,197,8,174,205,117,88,144,194,48,228,232,
197,11,57,242,152,35,220,119,186,244,139,151,177,244,209,39,220,26,140,141,142,241,224,253,15,186,92,78,44,198,36,225,52,186,68,78,45,174,138,255,138,73,166,196,177,203,155,172,137,226,7,201,220,157,231,
178,239,65,47,194,47,249,148,202,62,126,217,71,41,197,105,31,122,11,33,237,206,51,68,230,107,18,179,96,184,240,135,140,127,148,202,154,197,239,126,61,74,41,252,146,143,95,242,209,158,102,225,226,87,33,
203,130,200,68,169,194,46,128,33,164,164,119,176,143,129,25,125,148,42,37,74,149,18,59,61,239,57,124,248,226,179,105,181,227,92,133,82,73,14,220,67,41,133,167,61,60,173,241,180,198,24,216,115,159,231,
179,240,117,11,208,158,142,239,90,51,54,54,70,189,81,39,104,7,68,81,196,35,75,30,33,104,5,25,29,153,68,59,242,133,128,211,2,210,89,81,152,158,253,69,69,158,51,136,165,200,122,45,132,173,144,87,191,97,
33,229,106,185,227,151,30,186,240,96,40,71,4,81,147,40,177,217,139,96,200,60,24,50,19,123,146,18,6,182,27,96,215,231,63,183,227,216,207,221,125,103,170,179,74,52,162,58,17,161,181,224,84,166,144,45,214,
9,181,145,113,62,121,218,103,115,159,61,240,21,7,240,130,67,246,162,25,212,65,96,51,127,113,30,60,1,71,8,73,255,96,31,111,57,243,164,220,103,191,116,254,151,184,243,142,59,24,26,25,166,213,108,177,114,
217,42,86,175,88,93,40,4,148,133,71,177,45,12,233,94,73,216,77,151,80,8,22,202,204,47,140,136,120,239,39,223,211,85,63,237,49,127,55,182,223,101,59,154,81,157,200,132,25,45,189,45,96,8,90,205,22,199,159,
122,44,218,235,174,2,79,59,251,173,108,25,223,76,104,2,231,165,75,165,44,24,202,137,172,21,79,172,226,150,235,111,203,21,73,159,253,153,51,9,100,139,32,108,33,165,140,153,225,249,120,218,67,72,73,208,
12,57,238,164,99,232,237,239,117,159,251,211,31,255,196,141,55,252,132,241,137,49,38,235,19,108,222,188,133,71,254,254,48,38,204,158,224,133,252,206,20,44,145,93,69,149,40,166,82,167,42,88,176,207,133,
200,59,144,6,142,93,188,136,158,190,170,59,246,7,223,245,225,220,47,62,231,51,103,49,218,26,37,20,109,140,233,230,118,116,7,3,192,68,112,250,185,167,184,99,221,126,219,239,249,203,159,238,118,219,111,
60,229,13,248,21,69,43,106,197,114,219,49,195,130,98,75,119,180,167,185,250,226,107,24,27,25,115,159,29,152,49,192,231,190,121,62,99,147,99,68,68,113,94,92,41,132,84,16,193,225,71,31,198,254,135,237,151,
203,111,156,123,206,185,180,130,38,136,184,70,107,197,19,43,104,212,155,185,74,205,124,9,211,212,21,152,83,139,172,92,181,84,174,99,35,19,208,19,157,96,16,155,133,167,188,55,165,244,208,150,97,126,252,
195,27,185,231,174,123,220,190,99,78,88,196,156,185,179,9,76,155,72,68,133,202,158,130,121,155,5,195,192,9,103,28,151,251,206,215,92,241,93,174,250,214,213,185,125,103,156,125,6,205,118,131,144,208,134,
65,82,7,52,73,48,105,165,40,251,21,62,116,202,199,104,52,210,60,247,94,251,236,201,33,71,190,132,201,230,164,213,39,177,197,57,56,115,144,69,39,29,237,222,23,134,17,111,63,233,237,140,141,142,161,61,143,
190,106,31,195,235,71,24,218,48,156,137,197,81,168,86,206,174,99,167,22,145,211,245,106,136,108,113,65,33,31,158,45,54,38,91,143,40,4,51,102,205,96,207,125,247,116,199,125,248,129,135,169,77,76,240,173,
175,124,59,47,90,222,247,54,90,81,147,200,132,46,58,155,130,33,156,102,119,127,152,16,120,158,230,232,55,47,200,128,61,196,146,251,151,240,183,187,239,99,116,100,212,237,95,120,236,81,8,31,66,19,196,117,
88,66,166,206,93,166,126,170,84,42,209,156,8,248,241,213,63,201,125,183,143,124,254,131,212,91,53,26,65,131,48,50,4,141,128,147,207,58,17,207,247,220,123,126,122,253,79,184,255,190,191,227,121,30,253,
213,62,130,137,128,117,203,215,59,179,191,184,118,197,42,77,33,58,11,199,101,103,115,138,200,71,120,69,103,163,76,81,6,102,129,11,131,144,151,47,124,41,253,131,125,238,168,151,126,249,50,148,84,252,253,
47,75,24,31,75,123,37,22,28,115,36,194,131,144,0,35,162,169,193,200,196,167,122,6,122,152,191,223,243,221,49,150,220,247,32,235,214,173,103,211,186,205,60,241,200,82,183,255,5,251,236,69,255,204,126,130,
40,136,197,150,74,152,97,107,166,180,173,193,85,154,74,169,194,175,174,255,13,247,222,117,95,110,53,62,119,217,121,212,26,99,52,155,13,142,126,243,2,118,222,125,71,247,218,3,247,45,225,11,231,125,17,99,
12,213,74,15,50,212,172,95,190,17,105,100,161,226,140,108,153,70,129,25,157,205,71,114,107,221,76,197,228,106,28,180,19,133,131,167,2,173,17,53,249,224,133,103,187,99,142,14,143,242,243,95,221,132,86,
154,13,43,55,242,216,3,79,184,215,246,222,119,111,250,183,235,37,48,1,145,49,41,24,142,40,121,48,130,118,192,130,215,31,145,179,220,46,253,242,101,72,37,41,169,18,215,126,243,135,153,136,188,228,180,51,
79,101,108,98,212,50,144,212,51,207,166,94,117,108,210,14,14,204,224,202,11,255,139,225,45,195,238,24,243,246,218,131,189,15,156,79,121,176,196,203,23,29,230,246,143,141,142,241,209,247,127,20,33,161,
92,42,227,203,18,235,158,92,79,80,111,91,95,77,22,78,232,76,80,69,76,175,71,100,62,170,75,145,7,152,108,161,166,16,57,7,77,20,50,122,32,120,213,130,87,50,107,206,118,238,23,124,249,252,139,169,82,65,161,
169,120,61,92,122,222,229,185,179,240,211,95,254,36,195,147,195,132,132,105,234,89,58,183,60,211,185,4,38,52,188,235,83,167,187,207,174,94,185,134,223,253,250,118,60,229,83,246,43,220,121,235,255,50,62,
146,50,240,212,119,158,130,223,227,17,68,45,144,6,41,172,200,178,21,134,82,37,121,12,77,169,228,83,242,202,92,251,205,235,115,223,239,204,115,223,195,59,63,114,90,206,162,187,248,243,95,101,195,250,245,
120,218,163,172,42,108,89,53,76,107,34,200,100,66,233,168,190,164,139,165,234,150,117,74,134,8,65,182,202,41,111,170,145,235,253,43,246,116,36,74,236,148,179,78,118,199,171,79,214,249,237,173,191,141,
77,71,169,41,121,101,254,247,183,247,178,113,237,70,247,158,227,79,60,142,89,219,207,164,77,11,35,66,7,134,44,128,33,132,224,85,175,127,5,149,158,138,251,236,21,95,187,130,114,181,140,175,52,101,175,74,
127,111,31,63,190,250,103,185,5,125,195,91,94,79,43,108,18,154,40,14,169,39,32,100,196,86,82,105,88,41,87,120,244,222,199,248,213,143,83,83,184,92,41,179,203,238,187,184,237,235,191,251,35,110,250,233,
77,8,169,41,233,50,227,27,39,152,216,50,233,98,116,89,177,47,10,82,132,130,193,36,186,52,231,201,78,113,149,23,68,142,102,50,227,45,23,59,101,45,24,189,61,61,236,123,240,139,220,151,95,181,98,53,43,150,
175,68,162,208,162,132,22,30,213,74,15,223,255,122,254,44,124,243,25,39,18,68,173,152,37,66,184,26,221,44,24,82,73,142,126,211,194,28,216,127,188,253,78,235,65,151,41,235,18,229,114,133,191,220,126,15,
173,102,43,85,238,199,29,69,155,32,246,73,132,200,36,148,180,3,66,123,202,122,225,30,3,3,131,220,118,195,239,88,177,116,101,135,127,243,248,195,143,115,217,37,223,68,251,10,95,122,76,110,110,48,182,105,
34,118,24,187,118,132,229,217,144,21,243,20,88,147,171,189,20,93,28,67,10,212,51,130,60,24,34,127,200,176,29,178,223,97,251,177,221,246,51,221,145,174,186,244,106,90,173,0,37,53,90,248,72,20,158,150,220,
125,251,61,52,27,233,162,29,125,252,66,34,21,210,54,161,171,68,207,41,115,41,168,246,84,216,231,160,189,221,103,214,173,89,199,210,199,158,64,41,77,89,149,240,189,50,74,106,214,175,220,200,240,230,180,
109,108,191,3,247,195,171,104,154,237,38,145,136,144,34,91,16,157,176,37,238,221,16,50,78,146,245,247,246,81,242,75,157,77,153,158,34,104,183,144,70,210,170,181,25,91,95,195,19,94,166,235,49,191,240,69,
54,152,174,90,55,95,24,171,59,243,230,197,168,171,149,115,38,107,89,145,43,243,148,8,198,195,9,62,121,201,185,185,143,255,237,158,191,241,226,131,14,160,36,171,84,85,47,101,89,66,10,73,100,34,38,198,106,
148,202,49,120,47,62,228,0,122,6,171,4,35,77,34,74,40,161,114,250,35,106,71,28,116,196,129,12,204,236,119,199,190,245,23,183,178,207,254,251,80,45,247,50,80,30,164,175,220,143,167,125,140,49,252,245,206,
251,88,100,217,36,165,228,148,119,189,133,171,47,185,134,25,125,51,226,212,107,194,14,203,12,173,149,235,41,105,76,54,57,229,156,55,177,195,206,115,58,0,217,109,143,221,120,215,123,223,197,5,31,249,34,
186,85,194,87,37,187,208,81,177,41,2,1,46,67,34,210,174,70,76,71,195,106,94,100,137,158,222,94,19,213,38,144,40,91,45,162,172,60,140,189,78,33,20,202,110,75,251,92,162,80,66,217,208,98,252,185,23,190,
248,133,92,255,63,63,120,202,149,194,63,250,222,13,124,228,140,79,176,195,192,92,74,186,146,118,187,10,65,125,178,193,205,143,223,200,140,89,131,79,249,248,135,237,249,114,118,154,253,92,102,14,204,166,
236,151,241,252,180,230,86,170,244,12,125,241,43,246,231,248,211,22,185,237,191,255,237,239,204,223,123,62,190,239,187,125,111,59,246,237,220,117,235,61,248,210,183,37,62,33,145,137,136,8,237,61,34,50,
161,219,54,68,132,38,196,16,18,218,247,38,207,13,161,123,77,84,203,197,20,93,65,193,139,60,233,100,23,9,153,88,94,139,223,241,198,167,213,212,112,226,169,139,25,152,213,79,219,196,62,73,154,23,23,28,116,
228,129,79,11,12,128,69,39,188,134,70,187,129,33,138,43,11,187,128,81,233,173,178,232,164,212,233,108,53,91,124,230,67,231,243,145,179,62,154,59,214,37,215,92,76,185,207,183,9,91,83,56,203,69,71,84,208,
100,178,236,34,147,207,41,14,12,113,34,107,234,65,46,25,45,34,200,217,85,38,83,93,88,42,149,56,228,149,47,73,123,253,34,195,240,240,8,221,71,190,164,237,102,213,106,149,114,37,245,41,142,63,233,56,126,
246,221,155,40,19,101,218,200,4,71,47,62,42,119,132,177,177,49,218,65,88,108,164,202,125,127,173,21,125,253,125,25,39,244,40,254,231,55,127,138,131,134,66,116,128,97,140,225,204,243,207,192,43,165,158,
248,55,190,116,25,171,86,173,98,213,202,149,252,233,15,127,230,176,87,28,10,192,204,89,51,184,244,134,75,56,117,193,219,233,241,122,210,69,54,121,147,39,234,208,206,121,112,232,18,203,210,157,127,142,
40,64,36,114,5,14,73,3,69,82,184,22,134,134,249,47,218,147,157,158,183,147,251,204,99,143,62,193,162,195,143,141,115,206,217,240,128,136,3,143,73,119,236,235,22,31,207,133,95,187,32,99,17,45,224,199,215,
252,148,208,180,65,248,241,194,105,197,129,175,60,32,103,93,157,116,220,91,88,189,106,117,218,142,44,84,161,178,68,178,227,206,115,185,246,39,223,119,128,31,120,232,139,9,76,155,86,216,116,149,139,217,
219,241,167,29,195,192,118,3,110,251,230,159,220,194,117,223,191,142,176,29,81,31,109,114,250,107,222,205,189,27,254,76,79,95,15,0,135,31,245,50,62,116,193,251,185,244,188,203,209,210,235,60,233,50,37,
189,157,186,163,112,242,187,247,138,124,119,119,183,122,172,78,139,33,143,89,51,104,240,177,139,63,146,251,236,69,159,187,152,225,45,195,140,110,25,101,108,104,220,222,107,212,134,198,25,27,182,207,71,
38,248,253,109,127,164,153,105,94,121,249,145,47,197,75,162,180,38,34,138,34,246,61,116,31,102,237,144,58,154,43,151,175,226,222,187,239,101,108,116,140,137,177,73,38,199,27,76,142,79,82,175,53,168,215,
234,212,39,26,212,39,234,60,242,247,71,89,179,114,77,106,33,41,197,155,79,95,204,232,216,8,237,40,112,227,167,162,48,98,151,231,63,151,3,95,185,191,123,239,240,208,8,223,188,228,114,130,32,160,54,52,65,
123,204,160,3,159,119,44,122,47,38,74,89,255,31,231,190,139,157,158,191,35,237,48,200,157,243,102,10,169,48,157,36,18,249,150,163,41,177,235,28,197,82,96,217,94,123,207,231,69,7,237,227,182,199,199,198,
249,209,117,215,227,107,143,146,170,208,35,251,233,211,51,232,215,131,244,233,65,250,61,123,215,131,108,94,54,204,253,119,63,144,59,254,7,62,115,14,227,19,163,132,132,52,234,13,206,254,194,153,185,215,
47,252,244,23,241,60,159,138,87,101,176,58,147,217,189,219,51,103,96,46,219,15,204,101,206,224,92,230,206,120,14,115,103,236,200,206,115,118,229,170,75,174,201,125,246,244,247,156,70,96,90,76,54,39,9,
195,54,96,216,110,238,118,156,246,145,147,115,239,59,247,204,143,178,124,217,114,134,55,142,18,78,64,69,86,41,201,50,15,221,245,40,183,252,40,117,26,181,231,241,179,63,221,128,241,35,66,147,150,57,76,
5,131,217,202,90,79,9,200,52,117,65,29,71,120,253,219,94,151,219,190,242,27,87,162,133,70,75,77,69,86,232,209,253,244,234,62,250,188,126,122,189,126,250,188,65,250,188,1,250,188,65,102,246,108,199,229,
159,187,170,176,104,167,82,238,47,211,38,96,239,151,236,197,115,118,157,235,94,219,178,121,11,55,255,226,22,74,126,137,106,169,151,25,61,51,153,213,191,61,179,250,102,51,123,96,123,102,15,236,192,236,
193,29,152,51,99,46,59,206,218,137,149,15,174,97,100,40,141,0,43,173,56,236,136,67,153,108,212,104,133,45,12,112,194,59,142,67,233,180,22,253,59,151,127,151,59,126,119,7,163,155,199,160,37,41,203,42,30,
165,216,135,146,62,31,60,233,227,172,91,181,222,189,191,127,176,159,75,255,251,18,2,90,54,251,249,244,230,198,252,195,128,100,127,157,210,138,35,22,29,158,235,125,184,229,23,191,138,187,77,133,143,39,
227,192,155,47,61,180,244,240,132,143,39,61,119,47,121,101,30,249,235,99,44,127,108,69,238,215,44,124,237,81,4,97,139,35,79,120,69,71,222,163,167,183,138,167,125,170,165,42,61,229,94,42,165,10,149,74,
53,222,46,85,233,41,245,208,83,238,165,90,233,101,214,172,217,252,254,166,59,114,199,120,197,81,135,83,171,215,24,29,25,101,209,201,11,217,101,222,206,238,181,141,235,55,114,201,133,95,103,100,120,12,
209,22,148,69,5,95,196,126,81,226,81,84,189,30,206,89,124,46,65,43,72,29,219,215,189,154,5,39,188,138,201,246,100,92,234,147,251,141,211,3,100,186,3,98,166,124,83,146,162,48,50,255,158,40,138,216,233,
121,59,50,111,239,121,110,223,230,77,91,120,224,254,37,40,161,209,210,199,151,126,220,180,146,84,166,216,188,185,43,56,16,146,146,95,226,150,235,110,203,29,251,152,19,94,67,96,90,188,244,168,67,50,201,
160,48,14,36,106,143,178,87,166,90,234,193,183,169,85,37,117,234,117,251,113,40,68,219,128,225,210,37,79,18,133,145,59,206,33,135,31,76,219,4,60,127,255,221,57,116,193,65,57,203,112,241,194,147,88,183,
118,29,194,72,74,162,66,73,84,208,120,121,179,84,104,150,252,239,195,220,80,200,159,92,113,253,183,120,206,206,115,98,63,195,77,173,219,54,80,178,239,144,83,191,108,91,239,51,35,143,242,135,142,152,61,
103,86,94,246,190,239,163,180,219,109,20,154,146,244,209,66,167,181,182,73,131,172,155,65,37,93,33,193,175,111,252,93,238,140,27,152,57,64,51,106,210,55,152,122,230,247,252,249,30,30,184,255,65,60,229,
81,45,85,169,248,21,60,207,203,23,175,41,141,74,130,133,182,18,100,227,154,45,60,120,239,195,238,56,165,82,41,214,79,173,70,65,111,124,156,135,30,124,24,37,52,190,40,83,18,21,60,225,23,102,171,216,128,
163,95,230,130,179,46,226,166,235,110,206,152,205,48,52,62,76,96,130,184,73,199,76,55,245,209,116,65,195,206,97,73,61,117,109,43,8,83,79,92,73,207,214,220,42,132,243,206,21,74,72,164,212,152,208,48,56,
99,6,149,254,50,43,215,175,98,100,124,152,146,87,162,170,123,232,211,131,84,84,15,74,120,174,253,79,38,77,153,153,41,8,216,169,60,181,250,56,179,118,25,164,22,140,179,116,233,19,244,84,250,24,172,206,
100,215,93,118,97,172,49,198,146,7,255,78,165,183,66,111,185,151,217,253,115,152,217,55,155,74,169,146,233,11,87,182,47,60,9,24,198,13,251,66,192,240,200,40,166,20,48,52,190,153,229,43,151,17,181,13,51,
7,102,177,251,206,187,211,108,180,120,240,225,135,120,120,233,67,248,202,195,163,68,143,232,161,34,123,81,5,118,164,235,23,209,22,109,134,155,91,144,37,67,75,54,216,84,223,68,191,232,197,151,101,124,226,
248,86,92,96,106,61,114,147,120,230,214,163,55,69,175,190,141,168,150,167,14,157,72,17,255,113,130,124,184,68,9,27,78,17,218,13,121,105,71,109,154,166,65,68,136,22,154,170,236,165,199,235,199,151,101,
43,178,186,129,145,20,58,199,249,234,136,144,137,118,141,90,48,78,16,5,120,82,211,91,238,195,247,202,180,194,6,245,246,36,82,72,122,43,125,108,215,55,139,254,234,0,158,239,197,140,240,116,166,89,95,225,
89,118,36,190,70,59,12,24,25,31,97,227,240,122,70,106,35,8,1,21,93,37,156,128,85,79,174,166,214,24,35,16,45,4,146,178,168,80,166,103,74,118,36,128,132,132,4,166,73,211,76,82,55,147,180,76,211,138,52,21,
127,214,72,176,239,75,66,39,46,140,146,3,40,9,187,196,128,104,92,195,85,145,67,17,6,149,204,96,75,117,137,13,48,26,19,119,37,73,33,240,164,70,81,137,69,16,62,190,42,163,133,206,139,167,14,102,144,25,252,
21,187,158,61,126,31,158,246,8,9,81,82,81,242,203,120,202,163,66,133,94,211,135,84,146,170,95,161,167,210,131,246,58,193,208,218,138,42,165,115,142,159,146,138,222,106,31,8,232,171,246,51,49,62,193,230,
53,67,12,173,25,69,24,73,69,246,226,19,34,145,104,188,105,193,112,171,101,76,108,121,137,18,6,131,70,187,32,163,116,171,214,185,186,201,103,243,251,82,157,163,139,191,200,20,186,28,220,20,158,36,107,151,
76,241,180,35,141,226,142,39,141,16,190,19,107,90,106,219,73,4,202,213,216,102,192,72,170,247,92,53,122,60,62,175,36,52,190,240,211,114,79,59,180,37,105,194,84,82,198,53,82,118,76,69,10,134,202,128,161,
108,75,89,214,119,146,148,60,31,165,6,153,220,82,103,227,19,195,140,13,213,208,120,32,12,26,223,85,175,211,49,147,171,11,24,25,143,67,34,241,68,9,133,178,12,48,86,44,69,233,123,77,94,47,23,143,150,117,
39,53,46,232,21,101,116,124,118,158,103,50,42,207,216,22,20,108,141,170,73,71,240,137,56,124,33,69,60,154,40,59,83,42,169,139,18,153,150,180,236,52,157,100,212,81,110,50,155,206,140,59,210,233,60,170,
52,219,23,119,185,38,10,60,155,112,18,170,115,49,133,20,52,106,45,86,47,93,195,154,101,235,152,24,159,140,67,252,91,247,145,167,132,132,76,96,49,94,19,59,164,32,211,117,155,174,109,122,162,231,159,165,
199,73,0,214,197,128,74,81,120,9,203,6,153,249,176,155,45,34,226,170,112,66,147,27,252,149,54,110,24,202,61,101,154,19,77,148,150,174,239,59,49,127,123,250,170,52,234,45,215,64,3,16,70,17,166,13,70,217,
140,66,40,0,155,218,141,4,50,20,72,79,231,230,135,104,95,199,45,202,170,251,162,110,90,179,137,13,171,54,177,242,137,213,241,232,214,140,25,220,225,152,217,170,148,233,224,48,57,89,98,58,198,207,146,121,
45,17,72,211,205,219,206,254,212,89,57,102,76,220,189,106,178,102,175,52,54,5,99,226,55,219,239,218,142,34,14,126,233,126,252,224,182,239,115,202,130,183,241,183,63,223,143,244,109,237,145,76,227,68,23,
253,224,2,190,247,181,31,242,215,59,238,179,141,244,18,99,96,223,67,246,230,226,235,47,228,139,103,127,133,63,252,242,14,252,146,207,139,14,217,135,19,223,125,66,58,26,67,8,246,121,201,222,60,116,223,
35,68,237,200,5,16,127,254,221,155,105,214,155,188,102,241,2,110,252,175,95,240,217,171,62,193,229,159,189,154,77,107,55,57,182,9,33,105,214,155,108,90,187,153,101,15,175,224,211,87,158,203,119,47,250,
33,175,61,109,17,243,246,221,157,201,241,122,126,84,172,77,57,252,236,59,55,243,171,255,254,141,235,232,202,135,62,76,106,227,102,225,48,133,249,192,134,28,23,146,245,235,4,171,83,4,234,41,253,70,17,159,
245,57,135,69,100,3,249,134,114,185,196,239,111,253,3,223,185,229,42,142,152,191,128,218,80,205,182,174,41,55,178,181,217,104,18,69,97,28,193,181,38,115,216,142,56,235,243,239,225,199,87,253,140,119,127,
242,52,126,249,163,91,24,40,245,179,228,175,15,241,200,127,60,106,13,195,0,161,4,63,253,203,245,92,240,129,47,210,168,55,172,190,136,171,209,119,222,101,39,130,48,160,86,171,241,199,95,221,201,171,223,
244,42,174,188,240,106,60,223,199,215,37,106,163,99,108,88,181,145,39,31,94,78,59,8,121,254,190,123,240,208,223,30,225,196,247,157,192,167,78,255,60,203,30,93,14,170,83,34,24,19,247,147,75,163,98,195,
36,215,226,111,50,75,107,186,240,197,56,62,100,133,126,52,5,0,221,166,106,203,169,169,72,62,84,150,180,251,58,71,49,126,215,3,247,62,192,39,223,247,41,62,127,217,121,12,215,108,57,143,136,226,121,35,214,
16,8,137,8,76,139,208,180,49,194,240,188,249,187,50,99,246,12,46,59,255,219,68,6,14,94,112,0,141,118,195,213,79,9,105,136,76,100,65,132,70,80,167,214,172,81,111,213,109,5,73,219,102,222,12,65,216,228,
39,215,254,156,157,119,219,17,74,48,54,62,198,234,229,107,120,248,175,143,242,248,3,75,105,212,27,188,255,194,119,115,199,45,127,166,17,52,49,68,4,180,104,209,160,25,53,8,76,147,182,9,220,61,164,237,76,
213,162,117,20,79,195,46,156,233,166,115,221,34,183,126,177,131,40,178,175,155,130,238,40,204,76,238,16,150,198,228,41,148,5,3,97,210,20,139,101,115,169,226,243,147,235,126,138,246,53,103,125,234,189,
52,90,147,132,54,193,100,146,192,140,136,112,255,140,225,144,163,14,226,218,111,252,55,33,33,223,252,194,183,121,195,233,39,16,210,38,162,141,84,18,79,251,84,74,21,122,43,113,130,169,228,149,208,74,227,
251,62,189,149,62,250,123,7,232,173,244,34,133,160,163,90,213,75,0,0,16,158,73,68,65,84,109,218,212,198,199,185,255,47,15,176,221,246,51,121,226,209,165,60,112,207,131,108,217,56,132,16,130,106,181,194,
201,103,47,230,135,151,253,40,46,43,37,138,173,32,219,204,175,240,208,248,120,148,50,119,207,249,89,57,145,238,22,61,177,161,162,78,102,184,245,139,114,156,42,26,4,249,19,63,234,14,72,158,134,86,21,137,
76,27,188,72,39,182,101,199,236,69,38,226,204,183,158,197,233,103,158,198,252,253,246,36,36,176,65,182,204,25,38,34,144,16,180,2,94,189,248,40,254,112,243,157,148,170,62,183,254,236,215,60,111,247,93,
241,61,47,254,226,194,224,251,62,61,229,30,7,72,181,210,67,185,84,161,167,220,203,64,223,32,131,125,51,232,169,246,198,250,194,218,123,55,253,240,22,78,120,235,241,60,242,216,99,180,130,166,91,132,3,95,
121,0,247,222,113,31,15,63,242,8,145,45,51,170,143,55,168,69,19,76,180,39,105,4,77,130,32,160,21,180,241,101,41,14,128,10,31,109,189,109,183,168,81,156,159,153,74,92,209,101,79,12,122,94,111,152,140,57,
28,255,159,150,33,5,217,38,83,177,149,204,8,137,197,81,154,245,19,118,210,103,125,178,206,41,139,222,202,55,190,247,53,38,235,19,177,120,74,2,146,54,150,101,162,136,151,45,60,148,137,218,4,99,181,113,
34,34,42,229,10,119,221,254,23,94,243,134,163,105,181,227,6,30,41,36,74,123,104,29,103,226,180,242,208,42,30,147,231,123,62,190,231,217,40,130,96,162,54,201,166,213,27,184,253,231,119,176,195,142,59,176,
223,62,47,138,139,6,76,155,118,208,102,209,91,22,114,241,135,46,165,164,124,12,80,159,168,115,253,159,191,199,221,203,238,224,174,101,127,224,142,101,191,230,119,203,111,230,151,15,92,79,171,217,234,44,
237,148,130,114,213,231,121,123,237,194,140,237,7,80,158,180,252,136,172,248,138,114,140,240,203,158,45,200,142,58,172,177,200,152,46,30,159,153,58,133,155,244,147,98,210,169,158,206,67,39,114,105,219,
120,141,99,51,55,249,29,74,73,86,45,95,205,237,183,254,158,43,111,252,54,103,189,229,3,8,157,204,193,77,135,219,191,231,211,239,224,222,59,238,103,255,195,246,37,162,141,167,61,198,71,106,188,243,67,167,
115,203,13,183,198,139,105,131,142,73,158,34,153,41,146,76,104,147,90,209,110,181,169,141,214,88,186,228,73,106,181,73,100,32,185,246,27,215,241,190,243,222,195,57,111,58,151,80,132,148,75,62,123,189,
120,79,150,63,177,34,62,57,34,168,84,42,124,226,109,231,243,228,147,203,48,34,114,138,219,68,6,163,35,218,166,133,20,26,133,162,111,176,143,11,190,247,41,14,123,245,193,110,141,214,175,222,192,101,159,
191,156,27,191,243,211,156,2,111,181,3,222,243,225,119,242,137,139,62,206,218,85,107,57,120,247,131,49,97,148,3,134,46,98,110,43,12,33,199,16,236,72,213,172,136,50,214,250,194,14,39,78,199,113,131,150,
138,11,62,252,5,230,204,157,195,17,139,94,65,16,54,227,179,194,122,229,187,206,219,153,190,129,94,102,63,103,59,142,61,249,104,94,123,242,49,28,251,230,69,236,185,207,60,148,214,188,108,225,161,177,156,
23,33,82,9,180,157,81,168,116,58,233,211,96,24,90,63,196,170,101,107,25,222,52,194,232,240,56,96,208,158,199,21,95,248,14,7,28,182,47,253,51,250,104,5,45,222,117,222,25,252,207,109,119,209,108,54,48,24,
148,237,137,223,184,110,51,107,86,172,97,245,202,85,172,92,177,138,149,43,87,178,122,245,106,218,34,160,77,155,136,144,201,214,36,87,254,246,235,28,178,224,37,156,116,216,105,236,38,246,226,5,149,125,
89,177,116,57,103,124,240,109,232,138,164,109,69,115,59,106,115,192,193,251,241,137,139,62,206,77,63,186,137,190,129,62,107,32,196,193,120,211,193,137,162,213,53,77,23,110,140,154,21,79,210,228,186,97,
99,15,29,103,210,74,50,163,242,172,7,62,56,48,200,251,78,126,63,215,253,246,7,156,124,255,91,137,162,216,234,10,163,144,195,143,126,25,63,248,214,245,92,119,213,143,9,76,11,100,72,197,175,34,81,28,245,
250,87,177,240,245,11,184,251,142,123,8,77,232,218,150,99,64,226,56,85,179,222,100,221,150,245,172,89,186,158,129,25,125,28,118,244,193,133,63,70,243,235,27,110,231,133,47,126,1,247,220,241,55,78,62,107,
49,199,204,123,35,145,181,10,165,141,222,166,85,2,73,4,91,103,252,245,184,157,251,136,99,14,103,143,23,238,198,217,111,248,48,127,249,243,221,180,9,24,109,212,57,113,193,201,204,218,97,54,195,181,33,180,
208,40,161,216,101,143,231,242,227,59,111,224,63,63,249,159,132,81,200,171,142,57,146,150,9,48,38,114,189,45,198,68,29,202,63,171,204,217,214,124,136,19,89,198,14,176,207,76,190,136,68,94,172,37,179,9,
71,135,199,248,234,249,95,227,234,159,93,137,87,246,9,77,72,43,104,114,194,41,175,227,143,183,221,73,169,199,163,84,214,84,42,85,252,178,79,185,183,196,237,55,223,206,30,123,238,142,242,20,237,40,142,
9,37,145,98,1,140,108,30,101,229,210,181,44,185,251,17,198,134,198,186,126,107,207,247,248,229,247,110,229,221,31,123,7,243,95,52,143,63,220,124,7,171,214,173,142,139,208,144,54,92,18,207,6,20,8,148,80,
120,198,167,36,74,248,162,68,73,196,74,93,24,201,129,175,216,159,86,179,197,210,199,151,177,195,174,179,121,245,9,71,113,244,241,11,217,126,246,246,108,94,187,41,174,184,177,69,110,87,220,120,57,247,223,
115,31,231,95,248,89,151,85,77,139,228,12,152,40,175,155,141,153,242,154,62,122,186,240,0,194,184,161,245,241,223,17,101,70,86,100,135,135,101,138,31,164,192,19,138,219,126,250,27,230,189,96,30,167,189,
247,84,174,248,234,149,28,119,226,49,12,109,26,102,104,120,8,68,132,239,151,168,148,42,148,188,114,28,16,236,145,220,247,231,251,57,238,196,99,185,245,198,95,199,29,180,58,70,126,205,178,245,44,123,116,
37,97,35,194,23,229,142,74,215,236,109,233,146,39,121,222,188,93,248,200,151,62,192,121,103,94,128,175,149,21,87,202,197,97,103,206,153,193,156,137,57,8,41,240,41,197,65,70,11,253,228,196,4,163,195,163,
244,205,136,155,58,191,250,131,255,36,52,33,15,252,245,1,230,239,51,159,23,238,191,55,223,190,232,74,190,244,169,139,16,24,190,119,203,53,244,246,247,112,236,97,199,82,161,146,118,21,187,136,111,148,121,
44,234,14,179,237,128,228,83,184,38,55,82,53,153,85,216,108,54,216,184,126,67,90,25,47,141,155,59,213,211,83,225,218,203,175,229,176,87,30,202,166,141,27,121,219,153,111,229,155,95,254,22,109,19,160,164,
140,205,216,106,31,101,175,28,71,117,181,228,174,255,251,87,78,61,231,77,220,124,195,109,52,155,13,150,63,186,130,181,43,214,179,226,241,149,180,26,45,20,158,251,67,131,86,155,77,107,55,231,135,41,35,
8,131,144,203,62,125,5,199,191,253,24,86,45,95,21,135,242,81,40,155,128,123,228,190,39,248,202,117,23,90,17,220,153,170,254,227,45,127,226,99,167,125,154,118,216,198,47,249,92,125,217,53,92,126,229,229,
84,233,33,34,226,170,235,174,224,140,179,79,227,255,124,227,42,14,126,249,65,28,190,240,229,188,227,13,239,164,127,70,63,94,175,166,191,191,31,41,4,59,239,188,35,227,181,26,163,67,67,25,109,17,77,105,
93,145,173,237,237,118,185,10,173,117,220,170,37,226,180,168,180,73,41,41,108,148,85,166,244,143,132,65,11,77,217,43,83,210,37,151,74,21,82,210,106,53,9,162,128,182,105,83,242,42,113,234,85,123,244,86,
250,232,41,247,226,123,165,76,14,67,50,89,175,177,101,104,11,195,27,71,152,28,106,18,52,66,34,217,166,77,128,34,14,207,39,225,140,86,216,34,148,113,203,154,71,236,63,0,113,227,190,106,211,50,13,34,12,
190,240,241,69,25,133,166,221,14,105,70,77,90,76,210,38,136,51,165,214,99,74,66,240,74,73,206,252,196,187,121,223,103,222,195,81,251,30,205,163,143,62,26,251,36,17,188,236,85,47,229,250,223,252,144,3,
118,57,144,215,190,241,88,206,255,202,249,83,6,34,127,253,171,95,179,120,209,9,72,129,51,149,227,236,96,212,93,127,84,171,91,137,57,103,198,230,137,196,83,207,48,68,217,235,100,248,82,187,169,106,168,
116,238,148,82,138,74,165,66,79,165,151,254,106,63,61,229,42,37,191,20,183,129,249,190,157,200,22,47,0,2,26,19,13,54,175,25,98,253,210,141,12,175,27,37,12,66,180,138,101,191,135,135,182,99,105,221,73,
163,226,51,95,161,92,18,44,57,153,180,176,149,47,194,183,169,88,233,74,76,203,126,137,170,215,67,213,235,193,247,188,184,214,215,147,104,29,207,53,49,68,252,234,198,91,209,90,115,228,49,71,16,132,173,
56,66,108,224,168,215,28,73,208,10,104,5,77,190,246,213,175,83,17,85,74,162,228,146,109,31,63,231,99,76,212,106,8,33,120,245,162,87,19,137,200,90,90,89,221,241,148,231,101,37,195,234,99,39,81,136,76,53,
41,64,210,221,42,108,142,194,205,43,76,7,130,41,165,241,132,112,125,25,90,121,46,201,164,148,66,249,138,250,100,157,218,232,36,27,86,110,164,54,62,65,20,26,60,233,231,70,161,201,194,181,170,146,194,75,
45,180,205,211,200,142,170,75,95,148,220,251,178,175,75,36,90,120,182,214,54,36,178,209,217,72,88,135,79,24,30,127,104,41,95,253,244,215,249,204,151,63,201,65,47,61,144,187,239,188,135,125,95,188,47,175,
125,243,177,124,255,219,223,103,243,198,205,104,169,241,241,1,67,219,180,105,152,58,149,106,21,33,37,125,170,135,118,24,179,65,58,53,16,77,43,174,166,7,68,8,59,104,56,14,40,74,235,119,228,198,112,39,19,
159,69,58,88,216,141,198,179,185,143,248,42,1,113,69,72,172,43,52,158,242,80,90,17,70,33,67,107,54,48,182,101,140,205,235,134,49,6,219,2,209,173,119,94,209,189,167,94,77,113,50,197,213,1,83,101,252,98,
96,84,156,114,176,11,21,23,72,75,32,164,236,149,184,252,11,87,176,101,203,22,222,246,190,83,120,193,190,47,160,62,81,231,188,115,62,203,215,190,254,53,74,182,21,33,169,78,84,66,225,27,143,199,30,121,148,
191,254,229,30,155,61,48,153,100,174,217,42,24,211,234,16,207,243,232,233,233,141,207,114,161,144,50,214,11,90,104,132,84,104,169,226,122,168,164,38,74,36,215,201,136,171,22,29,0,50,14,121,40,105,175,
18,160,227,121,33,227,67,53,198,71,106,108,89,191,133,48,48,93,7,221,76,125,41,189,127,236,86,12,115,167,97,143,188,213,227,30,77,228,42,70,154,237,6,109,21,160,74,146,48,104,211,10,90,8,41,92,65,92,2,
122,162,23,218,81,128,17,198,186,132,97,154,246,51,97,87,207,188,168,67,166,159,185,152,180,63,103,204,92,35,141,155,250,140,27,62,140,155,196,230,134,16,219,49,121,82,167,172,1,67,109,188,198,200,166,
17,70,135,198,153,28,175,199,105,215,142,203,88,216,11,174,24,105,91,233,100,122,9,188,142,254,110,209,213,104,207,103,64,243,201,164,108,212,182,152,203,72,2,130,201,2,107,173,48,198,16,54,90,118,42,
68,154,176,117,151,225,203,0,46,165,36,180,173,121,142,29,38,98,91,175,38,170,167,175,51,181,67,240,165,40,92,211,15,183,240,2,156,34,39,115,97,197,220,64,73,37,168,141,215,168,79,52,216,178,126,51,205,
122,0,38,126,111,100,172,140,21,41,16,89,96,98,39,206,228,27,83,5,219,192,20,251,231,155,124,44,54,251,24,21,227,75,38,178,251,34,87,170,144,172,131,177,57,248,164,218,42,201,99,184,112,126,198,138,202,
54,31,68,198,228,142,245,148,1,201,13,253,178,227,183,115,58,36,203,12,33,50,195,234,113,51,11,133,132,32,8,216,178,113,11,181,145,26,99,67,227,110,164,159,176,146,91,32,227,71,147,12,98,23,233,149,213,
114,124,73,47,159,103,76,177,239,72,76,225,69,117,3,131,14,7,45,21,95,81,231,163,61,187,147,211,36,114,211,170,83,22,165,209,223,108,228,215,216,237,238,38,238,83,99,136,148,233,148,184,164,28,52,211,
140,233,134,16,103,45,43,59,84,178,213,106,48,81,171,51,62,60,206,248,104,45,62,211,237,116,159,208,216,154,44,210,171,170,185,165,55,194,21,224,21,47,95,177,181,139,20,119,22,188,154,66,8,175,152,185,
203,231,126,34,199,156,8,140,113,105,40,67,152,62,55,38,207,8,236,162,155,168,99,191,193,100,202,129,120,250,12,233,184,198,171,45,146,118,34,204,14,31,206,14,29,110,183,3,106,163,227,140,141,142,51,49,
58,65,59,136,139,222,146,126,93,227,168,108,247,8,225,70,223,153,44,39,76,230,194,143,25,118,100,162,103,153,237,169,35,13,29,87,134,54,102,138,202,17,147,99,69,42,102,50,192,152,48,179,200,41,24,81,55,
48,76,244,15,131,49,45,32,198,50,68,186,75,196,73,87,181,238,174,24,144,92,168,87,73,218,97,200,232,240,24,245,137,58,99,35,99,196,19,246,164,109,131,14,237,4,161,172,201,105,219,137,77,76,187,248,162,
93,50,179,240,201,236,173,226,149,163,69,62,214,49,101,131,81,49,35,87,176,178,10,113,38,99,232,42,178,28,40,118,209,77,238,204,159,130,25,38,154,210,27,127,90,12,145,174,136,77,22,152,98,21,182,18,180,
195,128,137,218,4,181,177,26,147,19,117,130,102,96,235,127,165,75,248,59,167,204,8,66,145,142,131,4,73,72,20,195,96,36,145,48,5,157,33,50,157,172,5,72,140,216,106,177,191,233,82,233,65,55,102,152,124,
181,72,148,1,44,203,152,84,129,155,105,193,48,79,163,113,71,79,219,44,37,82,7,47,91,89,168,164,36,138,66,198,199,198,104,54,90,241,8,109,147,94,206,34,34,66,154,228,82,117,202,217,236,201,180,185,40,51,
25,34,46,135,147,78,76,81,152,106,103,58,38,34,48,77,219,216,84,229,104,197,199,200,21,116,152,92,25,66,106,101,101,245,72,78,108,37,112,88,107,170,27,24,102,27,77,220,127,216,83,23,78,111,36,99,141,20,
97,59,164,209,170,51,54,62,70,125,178,78,16,180,80,54,193,35,132,116,5,169,33,32,141,32,18,184,210,252,236,99,156,96,137,92,9,102,250,211,234,12,147,159,56,36,59,244,7,174,0,182,200,11,119,165,104,151,
150,166,163,206,54,234,82,124,16,229,242,21,81,129,47,89,211,184,200,152,103,6,140,173,50,36,123,57,234,40,138,24,31,31,39,104,181,24,29,29,205,213,19,69,182,114,220,216,107,108,69,118,17,34,4,113,42,
219,88,176,50,102,174,205,204,37,70,174,17,210,118,118,231,199,19,196,243,126,5,97,183,161,95,83,184,34,166,80,205,17,117,45,243,36,199,12,211,5,8,67,65,175,152,60,8,142,33,38,221,54,79,179,199,112,90,
134,72,41,105,183,219,180,154,77,38,39,39,105,54,26,4,237,118,87,223,56,17,75,145,193,165,45,227,233,164,169,168,74,175,67,219,233,4,26,99,156,197,53,149,255,17,101,140,223,40,211,240,216,77,169,139,46,
162,138,46,181,80,145,137,166,112,24,51,154,36,227,189,211,161,67,204,83,86,224,255,16,32,97,24,50,54,58,74,208,106,81,203,196,186,196,180,10,52,74,77,90,27,178,151,238,154,0,214,255,32,185,226,102,98,
230,166,192,132,38,35,156,68,247,81,71,166,139,39,98,166,240,68,204,116,254,136,233,52,121,201,120,212,197,184,150,233,208,33,145,211,53,79,151,21,219,4,72,59,8,24,29,25,33,140,162,109,14,231,37,78,84,
42,190,132,99,72,238,209,8,66,59,27,189,200,7,227,226,88,69,83,55,29,162,102,166,232,3,47,122,33,105,96,143,204,204,158,180,140,35,234,90,240,150,247,220,201,249,35,166,139,213,245,204,129,49,45,32,81,
20,177,45,17,163,174,159,117,226,43,93,68,209,69,84,69,198,130,32,68,238,242,221,38,99,234,10,167,166,45,16,38,235,153,139,105,123,90,163,206,42,129,156,102,232,172,214,76,24,65,23,48,162,46,226,235,153,
191,253,83,46,112,159,79,238,27,164,177,214,149,16,214,198,202,48,1,137,52,153,109,145,92,119,35,63,70,48,34,59,212,37,111,0,23,175,107,211,221,91,239,226,181,231,42,10,187,121,239,83,41,123,243,207,90,
182,127,30,32,217,243,212,89,87,86,183,132,144,211,33,81,150,59,198,90,84,25,3,32,44,12,228,20,185,170,225,109,237,119,42,40,118,99,58,189,245,142,234,144,172,14,49,207,136,21,245,47,5,164,168,91,98,107,
203,216,69,78,25,211,57,247,89,88,147,56,171,196,69,46,209,63,253,72,169,140,131,104,204,86,189,118,186,234,144,206,218,220,103,227,166,237,9,171,158,117,96,18,198,88,171,43,171,93,178,122,163,24,225,
21,214,220,53,91,213,112,221,152,193,54,130,241,236,49,162,16,174,50,186,82,173,222,213,108,181,94,218,106,181,158,181,95,156,5,38,203,6,50,215,188,234,156,127,91,180,171,182,49,65,85,0,161,195,65,52,
100,44,170,188,186,127,54,111,90,107,116,169,244,176,154,51,119,110,157,48,124,67,163,94,127,74,22,213,51,165,105,76,33,143,145,239,77,49,211,182,145,49,229,254,172,247,156,77,30,21,147,73,81,14,148,127,
197,10,148,43,21,42,253,253,159,22,64,101,247,121,243,30,138,106,181,93,215,174,91,247,47,2,165,179,154,132,46,37,63,29,41,169,233,173,222,92,21,127,119,75,43,250,151,255,173,6,24,232,239,167,50,115,230,
134,21,43,87,238,145,252,73,179,118,155,55,239,158,218,166,77,187,76,214,106,180,219,109,254,93,110,91,19,85,162,235,48,36,179,85,91,235,223,225,166,181,198,43,149,232,159,61,123,195,138,229,203,247,6,
182,136,222,129,1,106,163,163,0,213,29,159,251,220,147,39,107,181,247,183,154,205,23,26,99,4,255,134,55,49,205,214,20,105,170,127,203,155,16,194,120,165,210,195,149,106,245,27,235,214,174,253,1,81,84,
235,29,24,224,255,1,162,18,14,168,30,204,180,44,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* Monique_Ui_GlobalSettings::aax_logo_100x_png = (const char*) resource_Monique_Ui_GlobalSettings_aax_logo_100x_png;
const int Monique_Ui_GlobalSettings::aax_logo_100x_pngSize = 12573;


//[EndFile] You can add extra defines here...
//[/EndFile]
