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
    //==========================================================================
    ScopedPointer<MoniqueAudioProcessor> filter;

    COLD void deleteFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

//==============================================================================
COLD StandaloneFilterWindow::StandaloneFilterWindow(const String& title) noexcept
:
DocumentWindow(title, Colour(0xff000000), DocumentWindow::allButtons, false )
{
    setOpaque(true);
    createFilter();

    if (filter == nullptr)
    {
        JUCEApplicationBase::quit();
    }

#ifndef PROFILE
    setContentOwned(reinterpret_cast<Monique_Ui_Mainwindow*>( filter->createEditorIfNeeded()), true );
#endif
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

        AppInstanceStore::getInstance()->editor->open_setup_editor_if_closed();
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

                AppInstanceStore::getInstance()->editor->open_setup_editor_if_closed();

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
                AppInstanceStore::getInstance()->editor->open_setup_editor_if_closed();
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

            AppInstanceStore::getInstance()->editor->open_midi_editor_if_closed();
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
    GET_DATA( synth_data ).ask_and_save_if_changed();
    JUCEApplicationBase::quit();
}
COLD void StandaloneFilterWindow::maximiseButtonPressed()
{
    if( isResizable() )
    {
        setResizable(false,false);
        DocumentWindow::maximiseButtonPressed();
    }
    else
    {
        setResizable(true,true);
        DocumentWindow::maximiseButtonPressed();
    }
}
COLD void StandaloneFilterWindow::resized()
{
    DocumentWindow::resized();
}

COLD void StandaloneFilterWindow::suspended() noexcept
{
    Monique_Ui_Refresher::getInstance()->stopTimer();
}
COLD void StandaloneFilterWindow::resumed() noexcept
{
    Monique_Ui_Refresher::getInstance()->startTimer( UI_REFRESH_RATE );
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
