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
    inline StandaloneFilterWindow(const String& title) noexcept;
    inline ~StandaloneFilterWindow() noexcept;

    //==========================================================================
    inline void handleAsyncUpdate() override;

    //==========================================================================
    AudioProcessor* getAudioProcessor() const noexcept;
    inline void createFilter() noexcept;
    inline void resetFilter() noexcept;

    //==========================================================================
    inline void closeButtonPressed() override;
    inline void maximiseButtonPressed() override;

    inline void resized() override;
    inline void suspended() noexcept;
    inline void resumed() noexcept;

    inline void visibilityChanged() override;
    inline void minimisationStateChanged( bool isNowMinimised ) override;

    inline bool keyPressed (const KeyPress& key) override;

#ifdef JUCE_IOS
    inline void swap_size ();
#endif

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
    double last_scale_factor;
    double aspect_ratio;

    inline void deleteFilter() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandaloneFilterWindow)
};

#ifdef JUCE_IOS
static int original_width = 0;
static int original_height = 0;


inline void StandaloneFilterWindow::swap_size()
{
    if( original_width == 0 )
    {
        original_width = getWidth();
        original_height = getHeight();
        //setFullScreen(false);
        //setResizable(false, false);
        setSize(original_width*1.2055, original_height*1.192);
    }
    else
    {
        if( getWidth() == original_width )
        {
            setSize(original_width*1.2055, original_height*1.192);
        }
        else
        {
            setSize(original_width, original_height);
        }
    }
}
#endif
//==============================================================================
inline StandaloneFilterWindow::StandaloneFilterWindow(const String& title) noexcept
:
#ifdef JUCE_IOS
DocumentWindow(title, Colour(0xff000000), 0, false ),
#else
DocumentWindow(title, Colour(0xff000000), DocumentWindow::minimiseButton | DocumentWindow::closeButton, false ),
#endif
               last_scale_factor(0), aspect_ratio(0.5)
{
    setOpaque(false);
    createFilter();

    if (filter == nullptr)
    {
        JUCEApplicationBase::quit();
    }

    this->setLookAndFeel( filter->ui_look_and_feel );
    main_window = reinterpret_cast<Monique_Ui_Mainwindow*>(filter->createEditorIfNeeded());
    setContentOwned( main_window, true );

#ifdef JUCE_IOS
    setOpaque(true);
    setUsingNativeTitleBar (true);
    setResizable(false, false);
    setTitleBarHeight(0);
    setFullScreen(true);
    setDraggable(false);
    //setSize(original_width*1.2055, original_height*1.192);
#else
    aspect_ratio = (double(main_window->original_w)+getBorderThickness().getLeft()+getBorderThickness().getRight())/(main_window->original_h+getBorderThickness().getTop()+getBorderThickness().getBottom()+getTitleBarHeight());
    getConstrainer()->setFixedAspectRatio(aspect_ratio);
#endif
}
inline StandaloneFilterWindow::~StandaloneFilterWindow() noexcept
{
    deleteFilter();
}

//==============================================================================
inline void StandaloneFilterWindow::handleAsyncUpdate()
{
    if( not filter->init_first_time_audio_devices_successfully() )
    {
        bool success = AlertWindow::showNativeDialogBox
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
inline AudioProcessor* StandaloneFilterWindow::getAudioProcessor() const noexcept
{
    return filter;
}
inline void StandaloneFilterWindow::createFilter() noexcept
{
    AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::wrapperType_Standalone);
    filter = reinterpret_cast<MoniqueAudioProcessor*>( createPluginFilter() );
}
inline void StandaloneFilterWindow::resetFilter() noexcept
{
    deleteFilter();
    createFilter();

    if (filter != nullptr)
    {
        setContentOwned (filter->createEditorIfNeeded(), true);
    }
}

//==============================================================================
inline void StandaloneFilterWindow::closeButtonPressed()
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
inline void StandaloneFilterWindow::maximiseButtonPressed()
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
inline void StandaloneFilterWindow::resized()
{
    if( Monique_Ui_Mainwindow* main_window = reinterpret_cast<Monique_Ui_Mainwindow*>(filter->createEditorIfNeeded()) )
    {
        aspect_ratio = (main_window->original_w+getBorderThickness().getLeft()+getBorderThickness().getRight())/(main_window->original_h+getBorderThickness().getTop()+getBorderThickness().getBottom()+getTitleBarHeight());
        if( getConstrainer()  )
            getConstrainer()->setFixedAspectRatio(aspect_ratio);
    }
    DocumentWindow::resized();
}

inline void StandaloneFilterWindow::suspended() noexcept
{
    if( filter->ui_refresher )
    {
        filter->ui_refresher->stopTimer();
    }
}
inline void StandaloneFilterWindow::resumed() noexcept
{
    if( filter->ui_refresher )
    {
        filter->ui_refresher->startTimer( UI_REFRESH_RATE );
    }
}
inline void StandaloneFilterWindow::visibilityChanged()
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
inline void StandaloneFilterWindow::minimisationStateChanged( bool isNowMinimised )
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
inline bool StandaloneFilterWindow::keyPressed (const KeyPress& key)
{
    bool success = false;
    if( key.getTextDescription() == "F11"  )
    {
        maximiseButtonPressed();

        success = true;
    }

    if( not success )
    {
        success = main_window->keyPressed( key );
    }

    return success;
}

//==============================================================================
inline void StandaloneFilterWindow::deleteFilter() noexcept
{
    if (filter != nullptr && getContentComponent() != nullptr)
    {
        filter->editorBeingDeleted (dynamic_cast <AudioProcessorEditor*> (getContentComponent()));
        clearContentComponent();
    }

    filter = nullptr;
}

#endif   // JUCE_STANDALONEFILTERWINDOW_H_INCLUDED

