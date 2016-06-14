#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#define JUCE_DONT_DECLARE_PROJECTINFO 1
namespace ProjectInfo
{
    const char* const  projectName    = "Monique";
    const char* const  versionString  = "1.0.9.2";
    const int          versionNumber  = 0x1000902;
}

//#define TRACKTION

/* USER RETURN NOTES FOR SLIDERS
   SET_USER_RETURN_VALUE
*/
#define GLOBAL_RETURN_MODE_USER "UGRM"
#define GLOBAL_RETURN_MODE_FACTORY "FGRM"
#define GLOBAL_RETURN_MODE_PROGRAM "PGRM"
#define GLOBAL_RETURN_MODE_UNDO "UGRM"

#define RETURN_VALUE_USER "URV"
#define RETURN_VALUE_FACTORY "FRV"
#define RETURN_VALUE_PROGRAM "PRV"
#define RETURN_VALUE_UNDO "URV"

#define PITCHWHEEL_CC 0

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
#define JUCE_JACK_CLIENT_NAME "MoniqueJACK"

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
#define MONO_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8f || n > 1.0e-8f)) n = 0;

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#define MONO_NOT_CTOR_COPYABLE( class_name ) 	\
  class_name( const class_name& );		\
  class_name( class_name&& );

#define MONO_EMPTY_COPY_OPERATOR( class_name ) 				\
  const class_name& operator=( const class_name& ) noexcept { return *this; }	\
  class_name& operator=( class_name&& ) noexcept { return *this; }

#define MONO_NOT_MOVE_COPY_OPERATOR( class_name ) 				\
  class_name& operator=( class_name&& ) noexcept;

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// JUCE
#ifdef IS_PLUGIN // MOST OF THE TIME WE DEVEL IN STANDALONE MODE
#include "../Plugin/JuceLibraryCode/JuceHeader.h"
#elif IS_STANDALONE
#include "../Standalone/JuceLibraryCode/JuceHeader.h"
#endif

#ifdef JUCE_WINDOWS
#include "vld.h" // need for debuging, but can be removed without any effects!
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// INLINING //
#if JUCE_LINUX || JUCE_MAC
#define COLD
//__attribute__ ((noinline,cold))
#elif JUCE_WINDOWS || JUCE_IOS
#define COLD
//__declspec(noinline)
#else
#define COLD inline
#endif

// HACK
//==============================================================================
//==============================================================================
//==============================================================================
template<int num_channels>
class mono_AudioSampleBuffer
{
#ifdef JUCE_DEBUG
#define DEBUG_BUFFER_SIDE_OFFSET 1
#else
#define DEBUG_BUFFER_SIDE_OFFSET 0
#endif

    AudioSampleBuffer buffer;
    int size;

public:

    inline const float* getReadPointer (int channelNumber = 0) const noexcept
    {
#ifdef JUCE_DEBUG
        if( buffer.getReadPointer( channelNumber )[size] != 0 )
        {
            if( true )
            {
                std::cout<< "buffer size overwriten getReadPointer " << buffer.getReadPointer( channelNumber )[size] << " size:" << size << std::endl;
            }
        }
#endif
        return buffer.getReadPointer( channelNumber );
    }
    inline float* getWritePointer (int channelNumber = 0) noexcept
    {
#ifdef JUCE_DEBUG
        if( buffer.getReadPointer( channelNumber )[size] != 0 )
        {
            std::cout<< "buffer size overwriten getWritePointer " << buffer.getReadPointer( channelNumber )[size] << " size:" << size << std::endl;
        }
#endif
        return buffer.getWritePointer( channelNumber );

    }
    inline void setSize (int newNumSamples, bool keep_existing_content_ = false ) noexcept
    {
        buffer.setSize( num_channels, newNumSamples+DEBUG_BUFFER_SIDE_OFFSET, keep_existing_content_, true, false );
        size = newNumSamples;
    }

    inline int get_size() const noexcept
    {
        return size;
    }
    inline void clear() noexcept
    {
        buffer.clear();
    }

    //==========================================================================
    COLD mono_AudioSampleBuffer(int numSamples) noexcept
:
    buffer( AudioSampleBuffer( num_channels, numSamples+DEBUG_BUFFER_SIDE_OFFSET ) ), size( numSamples )
    {
        buffer.clear();
    }
    COLD ~mono_AudioSampleBuffer() noexcept
    {
#ifdef JUCE_DEBUG
        if( buffer.getReadPointer( 0 )[size] != 0 )
        {
            std::cout<< "buffer size overwriten" << std::endl;
            //jassert( false );
        }
#endif
        // delete buffer;
    }
};

// MSVC REPLACEMENTS
#define CONSTEXPR_SUPPORT

#if JUCE_WINDOWS
#define constexpr const
#undef CONSTEXPR_SUPPORT
#define or ||
#define and &&
#define not !
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#include "dRowAudio_AudioUtility.h"

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#define UI_REFRESH_RATE 34 // MS
#include "monique_core_Parameters.h"
#include "monique_ui_Refresher.h"

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#define PROJECT_FOLDER String("/Monoplugs/Monique/")
#define THEMES_FOLDER String("/Monoplugs/Monique/Themes/")
#define MIDI_FOLDER String("/Monoplugs/Monique/MIDI/")
#if JUCE_MAC
#	define ROOT_FOLDER userApplicationDataDirectory
#elif JUCE_LINUX || RASPBERRY || JUCE_ANDROID
#	define ROOT_FOLDER userApplicationDataDirectory
#elif JUCE_WINDOWS
#	define ROOT_FOLDER userApplicationDataDirectory
#elif JUCE_IOS
#	define ROOT_FOLDER userApplicationDataDirectory
#endif


#if JUCE_MAC || JUCE_IOS
static inline File GET_ROOT_FOLDER() noexcept
{
    return File(File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER).getFullPathName() + String("/Application Support/"));
}
#else
static inline File GET_ROOT_FOLDER() noexcept
{
    return File::getSpecialLocation(File::SpecialLocationType::ROOT_FOLDER);
}
#endif


// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
class Status : public OnlineUnlockStatus
{
    String __state;

    bool doesProductIDMatch (const String& returnedIDFromServer) override
    {
        return getProductID() == returnedIDFromServer;
    }

    /** Returns the URL of the authentication API. */
#ifdef TRACKTION
    URL getServerAuthenticationURL()
    {
        return URL ("https://www.tracktion.com/marketplace/authenticate.php");
    }
#else
    URL getServerAuthenticationURL() override
    {
        return URL ("http://monoplugsweb.humanbeinc.de/Account/ActivateProductOnline");
    }
#endif

public:
    /** Returns the name of the web-store website, not for communication, but for
        presenting to the user.
    */
#ifdef TRACKTION
    String getWebsiteName()
    {
        return "tracktion.com";
    }
#else
    String getWebsiteName() override
    {
        return "Monoplugs Product Activation ";
    }
#endif

    static String get_system_code() noexcept
    {
        String system = "U00";
        switch( SystemStats::getOperatingSystemType() )
        {
        case SystemStats::MacOSX_10_4 :
            system = "M04";
            break;
        case SystemStats::MacOSX_10_5 :
            system = "M05";
            break;
        case SystemStats::MacOSX_10_6 :
            system = "M06";
            break;
        case SystemStats::MacOSX_10_7 :
            system = "M07";
            break;
        case SystemStats::MacOSX_10_8 :
            system = "M08";
            break;
        case SystemStats::MacOSX_10_9 :
            system = "M09";
            break;
        case SystemStats::MacOSX_10_10 :
            system = "M10";
            break;
        case SystemStats::MacOSX_10_11 :
            system = "M11";
            break;

        case SystemStats::Win2000 :
            system = "W02";
            break;
        case SystemStats::WinXP  :
            system = "W05";
            break;
        case SystemStats::WinVista :
            system = "W06";
            break;
        case SystemStats::Windows7 :
            system = "W07";
            break;
        case SystemStats::Windows8_0 :
            system = "W08";
            break;
        case SystemStats::Windows8_1 :
            system = "W08";
            break;
        case SystemStats::Windows10 :
            system = "W10";
            break;

        case SystemStats::UnknownOS :
            system = "U00";
            break;

        case SystemStats::MacOSX :
            system = "M00";
            break;
        case SystemStats::Windows :
            system = "W00";
            break;

        case SystemStats::Linux :
            system = "L00";
            break;
        case SystemStats::Android :
            system = "A00";
            break;
        case SystemStats::iOS :
            system = "I00";
            break;
        }

        return system;
    }

private:
    /** Subclasses that talk to a particular web-store will implement this method
        to contact their webserver and attempt to unlock the current machine for
        the given username and password. The return value is the XML text from the
        server which contains error information and/or the encrypted keyfile.
    */
#ifdef TRACKTION
    String readReplyFromWebserver (const String& email, const String& password)
    {
        URL url (getServerAuthenticationURL()
                 .withParameter ("product", getProductID())
                 .withParameter ("email", email)
                 .withParameter ("pw", password)
                 .withParameter ("os", SystemStats::getOperatingSystemName())
                 .withParameter ("mach", getLocalMachineIDs()[0]));

        DBG ("Trying to unlock via URL: " << url.toString (true));

        return url.readEntireTextStream();
    }
#else
    String readReplyFromWebserver (const String& email, const String& password) override
    {
        URL url
        (
            getServerAuthenticationURL()
            .withParameter ("app", getProductID())
            .withParameter ("email", email)
            .withParameter ("pw", password)
            .withParameter ("os", get_system_code())
            .withParameter ("mach", getLocalMachineIDs()[0])
        );
        //std::cout << url.toString(true) << std::endl;

        String feedback(url.readEntireTextStream());

        File folder = GET_ROOT_FOLDER();
        folder = File(folder.getFullPathName()+PROJECT_FOLDER);
        File feedback_file = File(folder.getFullPathName()+String("/com.log"));
        feedback_file.appendText( String( getProductID() ) + String(" ") + email + String(" ") + get_system_code() + String(" ") + getLocalMachineIDs()[0] + String("\n") );
        feedback_file.appendText( feedback+ String("\n")+ String("\n") );

        return String("<?xml encoding=\"UTF-8\"?> <KEYFILE MESSAGE=\"OK\"><KEY>") + feedback.fromFirstOccurrenceOf("#",true,false) + String("</KEY></KEYFILE>");
    }
#endif

public:
    /** This must return your product's ID, as allocated by the store. */
    String getProductID() override
    {
        return "MONI";
    }

private:
    /** This must return the RSA public key for authenticating responses from
        the server for this app. You can get this key from your marketplace
        account page.

        ATTENTION: The key is an 512 bit one
    */
    RSAKey getPublicKey() override
    {
        //return RSAKey("5,955d4af1ed484ec480757f4c7aec86bbcec4ba74f8ffd274b52a915aa30af9e07fc7d6c79943c3ac852950c5295d5cd1389058b8cede8a80d64daa5b04631393");
#ifdef TRACKTION
	return RSAKey("5,7ec7f6995c2f2d26144c16edfb5e74dfec3ea62fba618b8eef9a82b86db9ff4eadad2d840e36e64aa84566fb4039da19b9d170aa0d591437e3b3deabfc40b10d");
#else
        return RSAKey("3,c8d959202b69fcc103dfac088f550c20af52897b70f268159c961f66df628d2debff017e99a0b6ab1678de4e0bd07a2e30eaf107c7d92e3ca569bc17c178ea5f");
#endif
    }

    /** This method must store the given string somewhere in your app's
        persistent properties, so it can be retrieved later by getState().
    */
    void saveState (const String& state_) override
    {
        File project_folder = GET_ROOT_FOLDER();
        project_folder = File(project_folder.getFullPathName()+PROJECT_FOLDER);
#ifdef TRACKTION
        File settings_session_file = File(project_folder.getFullPathName()+String("/tracktion.mcfg"));
#else
        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));
#endif
	ScopedPointer<XmlElement> xml = XmlDocument( settings_session_file ).getDocumentElement();
        if( xml )
        {
            if( xml->hasTagName("SETTINGS-1.0") )
            {
                xml->setAttribute( "LAST_SAMPLE", state_ );
                xml->writeToFile(settings_session_file,"");
            }
        }
        else if( project_folder.createDirectory() )
        {
            XmlElement xml("SETTINGS-1.0");

            xml.setAttribute( "LAST_SAMPLE", state_ );
            xml.writeToFile(settings_session_file,"");
        }

        state( state_, true );
    }

    /** This method must retrieve the last state that was provided by the
        saveState method.

        On first-run, it should just return an empty string.
    */
    String getState() override
    {
        File project_folder = GET_ROOT_FOLDER();
        project_folder = File(project_folder.getFullPathName()+PROJECT_FOLDER);
#ifdef TRACKTION
        File settings_session_file = File(project_folder.getFullPathName()+String("/tracktion.mcfg"));
#else
        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));
#endif
        ScopedPointer<XmlElement> xml = XmlDocument( settings_session_file ).getDocumentElement();
        String state_;
        if( xml )
        {
            if( xml->hasTagName("SETTINGS-1.0") )
            {
                state_ = xml->getStringAttribute( "LAST_SAMPLE", "" );
                return state( state_, state_.length() > 30 ); // PREVENT FROM INVALID KEYS
            }
        }

        return state();
    }

public:
    String state( String state_ = "", bool write_ = false ) noexcept
    {
        if( write_ )
        {
            __state = state_;
        }
        return __state;
    }

#ifdef JUCE_IOS
    bool isUnlocked() const noexcept { return true; }
#endif 
    
    Status() {}
    ~Status() {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Status)
};
#ifdef TRACKTION
class Monique_Ui_Mainwindow;
struct OnlineUnlockForm2 : public OnlineUnlockForm
{
    Monique_Ui_Mainwindow*parent;
    void dismiss() override; // SEE MAINWINDOW.CPP
    OnlineUnlockForm2( OnlineUnlockStatus& status_, const String& userInstructions_, bool hasCancelButton_, Monique_Ui_Mainwindow*parent_ ) noexcept
:
    OnlineUnlockForm( status_, userInstructions_, hasCancelButton_ ), parent( parent_ ) {}
};
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

static inline int round0( float value ) noexcept
{
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value);
}
static inline float round01( float value ) noexcept
{
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value*10)/10;
}
static inline float round001( float value ) noexcept
{
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value*100)/100;
}
static inline float round0001( float value ) noexcept
{
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value*1000)/1000;
}
static inline float auto_round( float value ) noexcept
{
    /*
      if( value < 1 and value > -1 )
      {
          value = round0001( value );
      }
      else
        */
    if( value < 10 and value > -10 )
    {
        value = round001( value );
    }
    else if( value < 100 and value > -100 )
    {
        value = round01( value );
    }
    else
    {
        value = round0( value );
    }

    return value;
}

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#ifdef DEBUG
#include <iomanip>
static inline void debug_sample_print( float in_, int samples_to_print = 1024, const String& info_ = "" )
{
    static int count_samples = 0;
    if( count_samples < samples_to_print )
    {
        //std::cout << count_samples << " notes:"<<  info_ <<  " >>"<<  std::fixed << std::setprecision(10) << std::abs( in_ ) << std::endl;
        count_samples ++;
    }
}

#define PRINT_TIME( instruction ) \
  static double time_sum = 0; \
  static int time_counter = 0; \
  time_counter++; \
  double time = Time::getMillisecondCounterHiRes(); \
  \
  instruction \
  \
  time_sum+= (Time::getMillisecondCounterHiRes()-time); \
  std::cout << time_sum/time_counter << std::endl;
#endif

#endif  // APP_H_INCLUDED



