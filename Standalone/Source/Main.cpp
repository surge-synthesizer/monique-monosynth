#include "../../Source/App_h_includer.h"
#include "juce_StandaloneFilterWindow.h"
/*
#define ARRAY_SIZE 100000
#define TEST_CYCLES 10000
float* work_array_of_floats = new float[ARRAY_SIZE];
float* store_array_of_floats_test_1 = new float[ARRAY_SIZE];
float* store_array_of_floats_test_2 = new float[ARRAY_SIZE];
struct use_stack
{
    void run( int size_, float* target_ ) noexcept
    {
        float work_array_of_floats__[ARRAY_SIZE];
        float output__[ARRAY_SIZE];
        FloatVectorOperations::copy( output__, target_, size_ );
        for( int n = 0; n != TEST_CYCLES ; ++n )
            for( int i = 0; i != size_ ; ++i )
            {
                work_array_of_floats__[i] = output__[i] * output__[i];
                output__[i] = work_array_of_floats__[i]*1.21*work_array_of_floats__[i];
            }

        FloatVectorOperations::copy( target_, output__, size_ );
    }

    use_stack( int size_ ) {}
};
struct use_ptr
{
    static void run( int size_, float* target_, float* src_ ) noexcept
    {
        for( int n = 0; n != TEST_CYCLES ; ++n )
            for( int i = 0; i != size_ ; ++i )
            {
                src_[i] = target_[i] * target_[i];
                target_[i] = src_[i]*1.21*src_[i];
            }
    };
};



void run_test() noexcept
{
    for( int i = 0; i != ARRAY_SIZE ; ++i )
    {
        store_array_of_floats_test_1[i] = 1.23;
        store_array_of_floats_test_2[i] = 1.23;
    }
    use_stack stack__(ARRAY_SIZE);
    int64 start_time = Time::getMillisecondCounter();
    stack__.run( ARRAY_SIZE, store_array_of_floats_test_1 );
    std::cout<< "use_stack: "<< Time::getMillisecondCounter() - start_time << std::endl;

    start_time = Time::getMillisecondCounter();
    use_ptr::run( ARRAY_SIZE, store_array_of_floats_test_2, work_array_of_floats );
    std::cout<< "use_ptr: "<< Time::getMillisecondCounter() - start_time << std::endl;
};
*/
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
COLD MoniqueSynthesizerApp::MoniqueSynthesizerApp() noexcept {}
COLD MoniqueSynthesizerApp::~MoniqueSynthesizerApp() noexcept {}

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
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton | DocumentWindow::maximiseButton, true );
#else
        standaloneFilterWindow->setTitleBarButtonsRequired (DocumentWindow::minimiseButton | DocumentWindow::closeButton | DocumentWindow::maximiseButton, false );
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
    DBG( "USR QUIT") ;
    standaloneFilterWindow = nullptr; // (deletes our window)
}

//==============================================================================
COLD void MoniqueSynthesizerApp::suspended() {}
COLD void MoniqueSynthesizerApp::resumed() {}
COLD void MoniqueSynthesizerApp::systemRequestedQuit()
{
    DBG( "SYS QUIT") ;

    quit();
}
COLD void MoniqueSynthesizerApp::anotherInstanceStarted (const String&)
{
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MoniqueSynthesizerApp)
