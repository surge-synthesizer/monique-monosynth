#ifndef JUCE_STANDALONEFILTERWINDOW_H_INCLUDED
#define JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

#include "../../Source/monique_core_Processor.h"
#include "../../Source/monique_core_Synth.h"
#include "../../Source/monique_ui_MainWindow.h"

extern AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
//==============================================================================
//==============================================================================
class StandaloneFilterWindow : public DocumentWindow, public AsyncUpdater
{
public:
    COLD StandaloneFilterWindow(const String& title) noexcept;
    COLD ~StandaloneFilterWindow() noexcept;

    //==========================================================================
    COLD void handleAsyncUpdate() override;

    //==========================================================================
    AudioProcessor* getAudioProcessor() const noexcept;
    COLD void createFilter() noexcept;
    COLD void resetFilter() noexcept;

    //==========================================================================
    COLD void closeButtonPressed() override;
    COLD void maximiseButtonPressed() override;

    COLD void resized() override;
    COLD void suspended() noexcept;
    COLD void resumed() noexcept;

    COLD void visibilityChanged() override;
    COLD void minimisationStateChanged( bool isNowMinimised ) override;

    COLD bool keyPressed (const KeyPress& key) override;

private:
    bool keyStateChanged (const bool isKeyDown) override
    {
        return main_window->keyStateChanged( isKeyDown );
    }
    void modifierKeysChanged (const ModifierKeys& modifiers) override
    {
        main_window->modifierKeysChanged( modifiers );
    }

private:
    //==========================================================================
    ScopedPointer<MoniqueAudioProcessor> filter;
    Monique_Ui_Mainwindow* main_window;
    float last_scale_factor;
    float aspect_ratio;

    COLD void deleteFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

//==============================================================================
COLD StandaloneFilterWindow::StandaloneFilterWindow(const String& title) noexcept
:
DocumentWindow(title, Colour(0xff000000), DocumentWindow::minimiseButton | DocumentWindow::closeButton, false ), last_scale_factor(0), aspect_ratio(0.5)
{
    setOpaque(true);
    createFilter();

    if (filter == nullptr)
    {
        JUCEApplicationBase::quit();
    }

    main_window = reinterpret_cast<Monique_Ui_Mainwindow*>(filter->createEditorIfNeeded());
    setContentOwned( main_window, true );
    aspect_ratio = (main_window->original_w+getBorderThickness().getLeft()+getBorderThickness().getRight())/(main_window->original_h+getBorderThickness().getTop()+getBorderThickness().getBottom()+getTitleBarHeight());
    getConstrainer()->setFixedAspectRatio(aspect_ratio);
}
COLD StandaloneFilterWindow::~StandaloneFilterWindow() noexcept
{
    deleteFilter();
}

//==============================================================================
COLD void StandaloneFilterWindow::handleAsyncUpdate()
{
    if( not filter->init_first_time_audio_devices_successfully() )
    {
        AlertWindow::showNativeDialogBox
        (
            "AUDIO DEVICES PROBLEM.",
            "Monique was not able to open an audio device by default.\n"
            "\n"
            "Please choose an audio driver and device by yourself.\n"
            "(Press OK to open the setup (after see bottom right in the setup (DEVICE AND DRIVER)).)",
            false
        );

        main_window->open_setup_editor_if_closed();
    }
    else
    {
        if( not filter->restored_audio_devices_successfully() )
        {
            bool success = AlertWindow::showNativeDialogBox
                           (
                               "AUDIO DEVICES MISSING.",
                               "Monique was not able to open the last audio device.\n"
                               "\n"
                               "Should Monique try to open an alternative device (OK)\n"
                               "or do you like to select an AUDIO device by yourself (cancel)?.",
                               true
                           );
            if( success )
            {
                String error = filter->restore_audio_device(true);

                main_window->open_setup_editor_if_closed();

                if( error == "" )
                {
                    AlertWindow::showNativeDialogBox
                    (
                        "SUCCESS.",
                        "Audio device is open.\n",
                        false
                    );
                }
                else
                {
                    AlertWindow::showNativeDialogBox
                    (
                        "ERROR OPEN AUDIO DEVICE.",
                        error,
                        false
                    );
                }
            }
            else
            {
                main_window->open_setup_editor_if_closed();
            }
        }
        if( not filter->restored_all_devices_successfully() )
        {
            AlertWindow::showNativeDialogBox
            (
                "MIDI DEVICES MISSING.",
                "Monique was not able to reopen all MIDI ports.\n"
                "Please reconnect your MIDI devices or change your settings.",
                false
            );

            main_window->open_midi_editor_if_closed();
        }
    }
}

//==============================================================================
AudioProcessor* StandaloneFilterWindow::getAudioProcessor() const noexcept
{
    return filter;
}
COLD void StandaloneFilterWindow::createFilter() noexcept
{
    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
    filter = reinterpret_cast<MoniqueAudioProcessor*>( createPluginFilter() );
}
COLD void StandaloneFilterWindow::resetFilter() noexcept
{
    deleteFilter();
    createFilter();

    if (filter != nullptr)
    {
        setContentOwned (filter->createEditorIfNeeded(), true);
    }
}

//==============================================================================
COLD void StandaloneFilterWindow::closeButtonPressed()
{
    filter->set_audio_offline();
    {
        filter->closeAudioDevice();
        filter->player.setProcessor (nullptr);
        filter->removeAudioCallback (&filter->player);
        filter->audio_is_successful_initalized = false;
    }

    filter->synth_data->ask_and_save_if_changed();
    JUCEApplicationBase::quit();
}
COLD void StandaloneFilterWindow::maximiseButtonPressed()
{
    if( last_scale_factor == 0 )
    {
        last_scale_factor = main_window->synth_data->ui_scale_factor;
	
        const Desktop::Displays& displays = Desktop::getInstance().getDisplays();
        const Desktop::Displays::Display& display = displays.getDisplayContaining( Point<int>( getBounds().getX(), getBounds().getY() ) );
	
        const Desktop::Displays::Display& main_display( Desktop::getInstance().getDisplays().getDisplayContaining( Point<int>( getBounds().getX(), getBounds().getY() ) ) );
        const int main_display_h = main_display.userArea.getHeight();
        const int main_display_w = main_display.userArea.getWidth();
        int use_height = main_display_h;
        int use_width = main_display_h * aspect_ratio;

        /*
            if( use_width > main_display_w )
            {
                use_height = main_display_w * aspect_ratio;
                float new_scale = 1.0f/original_h*use_height;
                use_width = original_w*new_scale;
            }
            if( main_display_w < use_width )
            {
                use_width = main_display_w * 0.9;
                float new_scale = 1.0f/original_w*use_width;
                use_height = original_h*new_scale;
            }
            */
        setSize(use_width,use_height);
    }
    else
    {
	main_window->synth_data->ui_scale_factor = last_scale_factor;
	last_scale_factor = 0;
	main_window->update_size();
    }
}
COLD void StandaloneFilterWindow::resized()
{
    DocumentWindow::resized();
}

COLD void StandaloneFilterWindow::suspended() noexcept
{
    if( filter->ui_refresher )
    {
        filter->ui_refresher->stopTimer();
    }
}
COLD void StandaloneFilterWindow::resumed() noexcept
{
    if( filter->ui_refresher )
    {
        filter->ui_refresher->startTimer( UI_REFRESH_RATE );
    }
}
COLD void StandaloneFilterWindow::visibilityChanged()
{
    if( isVisible() )
    {
        resumed();
    }
    else
    {
        suspended();
    }
}
COLD void StandaloneFilterWindow::minimisationStateChanged( bool isNowMinimised )
{
    if( isNowMinimised )
    {
        suspended();
    }
    else
    {
        resumed();
    }
}

//==============================================================================
COLD bool StandaloneFilterWindow::keyPressed (const KeyPress& key)
{
    bool success = false;
    if( key.getTextDescription() == "F11"  )
    {
        if( Desktop::getInstance().getKioskModeComponent() )
        {
            Desktop::getInstance().setKioskModeComponent (nullptr);
        }
        else
        {
            Desktop::getInstance().setKioskModeComponent (this);
        }

        success = true;
    }

    if( not success )
    {
        return main_window->keyPressed( key );
    }
    return success;
}

//==============================================================================
COLD void StandaloneFilterWindow::deleteFilter() noexcept
{
    if (filter != nullptr && getContentComponent() != nullptr)
    {
        filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
        clearContentComponent();
    }

    filter = nullptr;
}

#endif   // JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

