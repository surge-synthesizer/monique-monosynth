#include "../../Source/App_h_includer.h"
#include "juce_StandaloneFilterWindow.h"

//==============================================================================
class MoniqueSynthesizerApp  : public JUCEApplication
{


public:
    //==========================================================================
    COLD MoniqueSynthesizerApp() noexcept;
    COLD ~MoniqueSynthesizerApp() noexcept;

    const String getApplicationName() override;
    const String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    //==========================================================================
    COLD void initialise (const String&) override;
    COLD void shutdown() override;

    //==========================================================================
    COLD void suspended() override;
    COLD void resumed() override;

    COLD void systemRequestedQuit() override;
    COLD void anotherInstanceStarted (const String&) override;

private:
    ScopedPointer<StandaloneFilterWindow> standaloneFilterWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesizerApp)
};

//==============================================================================
COLD MoniqueSynthesizerApp::MoniqueSynthesizerApp() noexcept
{
#ifdef USE_COPY_PROTECTION
    if( SHARED::getInstance()->activation_sate == nullptr )
    {
        SHARED::getInstance()->activation_sate = new RealActivationState();
    }
#endif
}
COLD MoniqueSynthesizerApp::~MoniqueSynthesizerApp() noexcept 
{
}

//==============================================================================
const String MoniqueSynthesizerApp::getApplicationName()
{
    return ProjectInfo::projectName;
}
const String MoniqueSynthesizerApp::getApplicationVersion()
{
    return ProjectInfo::versionString;
}
bool MoniqueSynthesizerApp::moreThanOneInstanceAllowed()
{
    return true;
}

//==============================================================================
COLD void MoniqueSynthesizerApp::initialise (const String&)
{
    standaloneFilterWindow = new StandaloneFilterWindow( getApplicationName() + String(" ") + getApplicationVersion() );
#ifndef PROFILE
    {
        // +++++++++ PROFILE
#if JUCE_LINUX || JUCE_WINDOWS
        standaloneFilterWindow->setUsingNativeTitleBar( false );
#else
        standaloneFilterWindow->setUsingNativeTitleBar( false );
#endif
        standaloneFilterWindow->setDropShadowEnabled( true );

#if JUCE_MAC
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::maximiseButton | DocumentWindow::closeButton, true );
#else
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::maximiseButton | DocumentWindow::closeButton, false );
#endif
        standaloneFilterWindow->setResizable(true,true);

        standaloneFilterWindow->addToDesktop();
        standaloneFilterWindow->setVisible(true);
        standaloneFilterWindow->setEnabled(true);
        // +++++++++ PROFILE
    }
#endif

    standaloneFilterWindow->triggerAsyncUpdate();
}
COLD void MoniqueSynthesizerApp::shutdown()
{
    DBG( "USR QUIT");
    MoniqueAudioProcessor*processor = reinterpret_cast< MoniqueAudioProcessor* >( standaloneFilterWindow->getAudioProcessor() );
    processor->set_audio_offline();
    processor->player.setProcessor (nullptr);
    processor->removeAudioCallback (&processor->player);
    processor->closeAudioDevice();

    standaloneFilterWindow = nullptr; // (deletes our window)
}

//==============================================================================
COLD void MoniqueSynthesizerApp::suspended() {}
COLD void MoniqueSynthesizerApp::resumed() {}
COLD void MoniqueSynthesizerApp::systemRequestedQuit()
{
    DBG( "SYS QUIT") ;
    MoniqueAudioProcessor*processor = reinterpret_cast< MoniqueAudioProcessor* >( standaloneFilterWindow->getAudioProcessor() );
    processor->set_audio_offline();
    processor->player.setProcessor (nullptr);
    processor->removeAudioCallback (&processor->player);
    processor->closeAudioDevice();

    quit();
}
COLD void MoniqueSynthesizerApp::anotherInstanceStarted (const String&)
{
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MoniqueSynthesizerApp)



