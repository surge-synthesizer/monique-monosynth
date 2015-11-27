#include "../../Source/App_h_includer.h"
#include "juce_StandaloneFilterWindow.h"

#ifdef USE_COPY_PROTECTION

// COPY PROTECTION:
#include <stdio.h>

// Support Unicode compilation and non-Windows compilation
#ifdef _WIN32
#include <tchar.h>
#else
#define _T(x) x
typedef char    TCHAR;
#endif

// To use the static version of TurboActivate then uncomment the next line
#define TURBOACTIVATE_STATIC

// Include the correct library on Windows
#ifdef TURBOACTIVATE_STATIC
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib, "TurboActivate-MDd.lib")
#else
#pragma comment(lib, "TurboActivate-MTd.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib, "TurboActivate-MD.lib")
#else
#pragma comment(lib, "TurboActivate-MT.lib")
#endif
#endif
#else
#pragma comment (lib, "TurboActivate.lib")
#endif

#ifdef JUCE_LINUX
#include "../../CopyProtection/Linux/API/C/TurboActivate.h"
#endif

// TODO: paste your Version GUID here.
#define TA_GUID _T("69c378825652e14a615fa1.63717076")


static inline StringRef get_error_text( int id_ ) noexcept
{
    switch( id_ )
    {
    case 0 :
        return "Ok";
    case 1 :
        return "Fail";
    case 0x00000002L :
        return "Invalid product key.";
    case 0x00000003L :
        return "Monique needs to be activated.";
    case 0x00000004L :
        return "Connection to the server failed.";
    case 0x00000005L :
        return "Monique key has already been activated with the maximum number of computers.";
    case 0x00000006L :
        return "The product key has been revoked.";
    case 0x00000007L :
#ifdef JUCE_DEBUG
        return "The version GUID doesn't match that of the product details file.";
#else
        return "ErrorCode: 1 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000008L :
#ifdef JUCE_DEBUG
        return "The product details file 'TurboActivate.dat' failed to load.";
#else
        return "ErrorCode: 2 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000009L :
#ifdef JUCE_DEBUG
        return "The trial data has been corrupted, using the oldest date possible.";
#else
        return "ErrorCode: 3 (please contact our support support@monoplugs.com).";
#endif
    case 0x0000000CL :
#ifdef JUCE_DEBUG
        return "The trial extension has already been used.";
#else
        return "ErrorCode: 4 (please contact our support support@monoplugs.com).";
#endif
    case 0x0000000DL :
        return "The activation has expired or the system time has been tampered with.\n Ensure your time, timezone, and date settings are correct.";
    case 0x0000000AL :
        return "The product key needs to be reactivated because your hardware has changed or the features data has changed.\n To reactivate simply call Activate again.";
    case 0x0000000BL :
        return
        "The hardware id couldn't be generated due to an error in the COM setup.\n"
        "Re-enable Windows Management Instrumentation (WMI) in your group policy\n"
        "editor or reset the local group policy to the default values. Contact\n"
        "your system admin for more information.";
    case 0x0000000EL :
#ifdef JUCE_DEBUG
        return "The the buffer size was too small. Create a larger buffer and try again.";
#else
        return "ErrorCode: 5 (please contact our support support@monoplugs.com).";
#endif
    case 0x0000000FL :
#ifdef JUCE_DEBUG
        return
        "Insufficient system permission. Either start your process as an\n"
        "admin / elevated user or call the function again with the\n"
        "TA_USER flag instead of the TA_SYSTEM flag.";
#else
        return "ErrorCode: 6 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000010L :
#ifdef JUCE_DEBUG
        return
        "The flags you passed to CheckAndSavePKey(...) or UseTrial(...)\n"
        "were invalid (or missing). Flags like TA_SYSTEM and TA_USER\n"
        "are mutually exclusive -- you can only use one or the other.";
#else
        return "ErrorCode: 7 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000011L :
#ifdef JUCE_DEBUG
        return
        "The function failed because this instance of your program\n"
        "if running inside a virtual machine / hypervisor and you've\n"
        "prevented the function from running inside a VM.";
#else
        return "ErrorCode: 8 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000012L :
#ifdef JUCE_DEBUG
        return
        "The 'extra data' was too long. You're limited to 255 UTF-8 characters.\n"
        "Or, on Windows, a Unicode string that will convert into 255 UTF-8\n"
        "characters or less.";
#else
        return "ErrorCode: 9 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000013L :
#ifdef JUCE_DEBUG
        return "The arguments passed to the function are invalid. Double check your logic.";
#else
        return "ErrorCode: 9 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000014L :
#ifdef JUCE_DEBUG
        return "The product key used is for TurboFloat, not TurboActivate..";
#else
        return "ErrorCode: 10 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000015L :
#ifdef JUCE_DEBUG
        return
        "IsGenuineEx() previously had a TA_E_INET error, and instead\n"
        "of hammering the end-user's network, IsGenuineEx() is waiting\n"
        "5 hours before rechecking on the network.";
#else
        return "ErrorCode: 11 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000016L :
#ifdef JUCE_DEBUG
        return
        "If IsGenuine() or IsGenuineEx() reactivated and the features\n"
        "have changed, then this will be the return code. Treat this\n"
        "as a success.";
#else
        return "ErrorCode: 12 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000017L :
#ifdef JUCE_DEBUG
        return
        "You didn't call the TA_InitAndroid() function before using\n"
        "the TurboActivate functions. This only applies to using TurboActivate\n"
        "on Android -- specifically using TA from native code. You don't\n"
        "need to call this from Java.";
#else
        return "ErrorCode: 13 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000018L :
#ifdef JUCE_DEBUG
        return
        "This product key had a limited number of allowed deactivations.\n"
        "No more deactivations are allowed for the product key. This product\n"
        "is still activated on this computer.";
#else
        return "ErrorCode: 14 (please contact our support support@monoplugs.com).";
#endif
    case 0x00000019L :
#ifdef JUCE_DEBUG
        return "Can't activate because the LimeLM account is cancelled.";
#else
        return "ErrorCode: 15 (please contact our support support@monoplugs.com).";
#endif
    }
}


class RealActivationState : public ActivationState
{
    GENUINE_OPTIONS opts;

    bool is_activated;

    String current_status_header;
    String current_status;
    String current_message_header;
    String current_message;

private:
    //==========================================================================
    bool get_is_activated() const noexcept override
    {
        return is_activated;
    }
    const String& get_status_header() const noexcept override
    {
        return current_status_header;
    }
    const String& get_status() const noexcept override
    {
        return current_status;
    }
    const String& get_message_header() const noexcept override
    {
        return current_message_header;
    }
    const String& get_message() const noexcept override
    {
        return current_message;
    }

    //==========================================================================
    bool check_get_is_activated( HRESULT hr_ ) noexcept
    {
        bool success = false;
        if(hr_ == TA_OK or hr_ == TA_E_FEATURES_CHANGED or hr_ == TA_E_INET or hr_ == TA_E_INET_DELAYED)
        {
            current_status_header = "Monique is activated";
            current_status = "Thanks for using Monique, enjoy!";
            current_message_header = "Activated successfully";
            current_message = "Monique has been activated successfully";

            if(hr_ == TA_E_INET or hr_ == TA_E_INET_DELAYED)
            {
                // TODO: show a warning to your user that this time (or the last time)
                // the IsGenuineEx() failed to connect to the LimeLM servers.
                current_message_header = "Monique is activated, but";
                current_message = "Monique is activated, but it failed to verify the activation with the activation servers.\nYou can still use the app for the duration of the grace period.";
            }

            success = true;
        }

        return success;
    }

public:
#define DUMMY_KEY_STRING "waMXjgpgXCpVsDY57Tbu"
    //==========================================================================
    bool activate( String key_ = DUMMY_KEY_STRING ) noexcept override
    {
        bool success = false;
        HRESULT hr;
        if( key_ == DUMMY_KEY_STRING )
        {
            hr = Activate();

            if (hr == TA_OK)
            {
                current_status_header = "Monique is activated";
                current_status = "Thanks for using Monique, enjoy!";
                current_message_header = "Activated successfully";
                current_message = "Monique has been activated successfully";

                is_activated = true;
            }
            else
            {
                current_status_header = "Activate Monique";
                current_status = "You are running Monique in demo mode!";
                current_message_header = "Activation failed";
                current_message = get_error_text(hr) + "\n\nPlease check the serial key for correctness.";
            }
        }
        else
        {
            hr = CheckAndSavePKey(_T(key_.getCharPointer()), TA_USER);

            if (hr == TA_OK)
            {
                current_message = "Product key saved successfully.";
                //TODO->trigger message

                hr = Activate();

                if (hr == TA_OK)
                {
                    current_status_header = "Monique is activated";
                    current_status = "Thanks for using Monique, enjoy!";
                    current_message_header = "Activated successfully";
                    current_message = "Monique has been activated successfully";

                    is_activated = true;
                }
                else
                {
                    current_status_header = "Activate Monique";
                    current_status = "You are running Monique in demo mode!";
                    current_message_header = "Activation failed";
                    current_message = get_error_text(hr);
                }
            }
            else
            {
                current_status_header = "Activate Monique";
                current_status = "You are running Monique in demo mode!";
                current_message_header = "Product key failed to save";
                current_message = get_error_text(hr);
            }
        }

        //TODO->trigger message

        return is_activated;
    }
    bool deactivate() noexcept override
    {
#define DELETE_KEY 0
#define KEEP_KEY 1
        HRESULT hr = Deactivate(KEEP_KEY);
        current_message = get_error_text(hr);
        return hr == TA_OK;
    }

public:
    //==========================================================================
    RealActivationState() : is_activated(false), opts(), current_status( "You are running Monique in demo mode." ) // product_key( "FM28-3XIP-K9DM-IS7K-GZC4-JTYT-SWTA" ) //
    {
        opts.nLength = sizeof(GENUINE_OPTIONS);

        // How often to verify with the LimeLM servers (90 days)
        opts.nDaysBetweenChecks = 90;

        // The grace period if TurboActivate couldn't connect to the servers.
        // after the grace period is over IsGenuinEx() will return TA_FAIL instead of
        // TA_E_INET or TA_E_INET_DELAYED
        opts.nGraceDaysOnInetErr = 14;

        // In this example we won't show an error if the activation
        // was done offline by passing the TA_SKIP_OFFLINE flag
        opts.flags = TA_SKIP_OFFLINE;

        HRESULT hr = IsGenuineEx(TA_GUID, &opts);
        if( not (is_activated = check_get_is_activated(hr)) )
        {
            uint32_t trialDays = 0;

            // Look in TurboActivate.h for what the error codes mean.
            current_status = String("Not activated: ") + get_error_text(hr);

            // Start or re-validate the trial if it has already started.
            // This need to be called at least once before you can use
            // any other trial functions.
            hr = UseTrial(TA_USER);

            if (hr == TA_OK)
            {
                // Get the number of trial days remaining.
                hr = TrialDaysRemaining(TA_GUID, &trialDays);

                if (hr == TA_OK)
                {
                    current_status_header = "Activate Monique";
                    current_status = String("Trial days remaining: ") + String(trialDays);
                    current_message = "Continue with demo.";
                    // is_trial = true;
                }
                else
                {
                    current_status_header = "Activate Monique";
                    current_status = String("Trial days remaining: ") + String(trialDays);
                    current_message = "Failed to get the trial days remaining: \n" + get_error_text(hr);
                }
            }
            else
            {
                current_status_header = "Monique needs to be activated.";
                current_status = String("Trial time is over.") + String(trialDays);
                current_message = "Failed to UseTrial: \n" + get_error_text(hr);
            }
        }

        std::cout<< current_status << std::endl;
    }
};
#endif // USE_COPY_PROTECTION
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
    DBG( "USR QUIT") ;
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



