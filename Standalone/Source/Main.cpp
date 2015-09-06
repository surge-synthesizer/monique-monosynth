#include "../../Source/App_h_includer.h"
#include "juce_StandaloneFilterWindow.h"

//==============================================================================
class MoniqueSynthesizerApp  : public JUCEApplication
{
public:
    //==============================================================================
    MoniqueSynthesizerApp() noexcept {}
    ~MoniqueSynthesizerApp() noexcept {}

    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }
    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    //==============================================================================
    void initialise (const String&) override
    {
        standaloneFilterWindow = new StandaloneFilterWindow( getApplicationName() + String(" ") + getApplicationVersion() );
#ifndef PROFILE
// +++++++++ PROFILE
#if JUCE_LINUX || JUCE_WINDOWS
        standaloneFilterWindow->setUsingNativeTitleBar( false );
#else
        standaloneFilterWindow->setUsingNativeTitleBar( false );
#endif
        standaloneFilterWindow->setDropShadowEnabled( true );

#if JUCE_MAC
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton | DocumentWindow::maximiseButton, true );
#else
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton | DocumentWindow::maximiseButton, false );
#endif
        standaloneFilterWindow->setResizable(true,true);

        standaloneFilterWindow->addToDesktop();
        standaloneFilterWindow->setVisible(true);
        standaloneFilterWindow->setEnabled(true);
// +++++++++ PROFILE
#endif
    }

    void shutdown() override
    {
        DBG( "USR QUIT") ;
        standaloneFilterWindow = nullptr; // (deletes our window)
    }

//==============================================================================
    void suspended() override {}
    void resumed() override {}

    void systemRequestedQuit() override
    {
        DBG( "SYS QUIT") ;

        quit();
    }

    void anotherInstanceStarted (const String&) override
    {
    }

private:
    ScopedPointer<StandaloneFilterWindow> standaloneFilterWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoniqueSynthesizerApp)
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MoniqueSynthesizerApp)
