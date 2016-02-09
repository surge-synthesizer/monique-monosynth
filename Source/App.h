#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

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

#define PITCHWHEEL_CC -99

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
#if JUCE_LINUX || JUCE_MAC || JUCE_IOS
#define COLD
//__attribute__ ((noinline,cold))
#elif JUCE_WINDOWS
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
    bool doesProductIDMatch (const String& returnedIDFromServer) override
    {
        return getProductID() == returnedIDFromServer;
    }

    /** Returns the URL of the authentication API. */
    URL getServerAuthenticationURL() override
    {
        return URL ("http://keytest.monoplugs.com/info.php");
    }

public:
    /** Returns the name of the web-store website, not for communication, but for
        presenting to the user.
    */
    String getWebsiteName() override
    {
        return "keytest.monoplugs.com";
    }

private:
    /** Subclasses that talk to a particular web-store will implement this method
        to contact their webserver and attempt to unlock the current machine for
        the given username and password. The return value is the XML text from the
        server which contains error information and/or the encrypted keyfile.
    */
    String readReplyFromWebserver (const String& email, const String& password) override
    {
        URL url
        (
            getServerAuthenticationURL()
            .withParameter ("app", getProductID())
            .withParameter ("email", email)
            .withParameter ("pw", password)
            .withParameter ("os", SystemStats::getOperatingSystemName())
            .withParameter ("mach", getLocalMachineIDs()[0])
        );

        // TODO ERROR,
        // TODO url  (call on success)
        // TODO error help url -> auf die seite wird der nutzer verwiesen wenn der fehler bei uns liegt
        //return url.readEntireTextStream();
        String ersatz("Keyfile for Monique User: monoplugs Email: info@monoplugs.com Machine numbers: L83A6A6A69 Created: 6 Feb 2016 8:34:13am #245153c1ef800600456eaaacba93ff8d5c0a6c54839547fea60f9b76e3bcffcc4ac1e6a0c059fd0d868a296a7d34849e55cab41bde8271c34eb2eda5f78b8d7e14221cf20eb2b36ab24d85a6d952c2c9bc46ff46efd91b937913eab56462e7ea13df4cf201f3d0a4770ddc03a14d21faef51e3a19a8f70c9f9979620b1a3373d4db530ffe4bdeb2e51f0ce642b5a5b5f68baf0ab2050aa1ca8c75c519683e795c964637eb60cc084398279c5b7a683484bfb2834241960d44dc0c6dbe7c8207f");
	return String("<?xml encoding=\"UTF-8\"?> <KEYFILE MESSAGE=\"OK\"><KEY>") + ersatz /*url.readEntireTextStream()*/ + String("</KEY></KEYFILE>");
    }

public:
    /** This must return your product's ID, as allocated by the store. */
    String getProductID() override
    {
        return "Monique";
    }
    
private:
    /** This must return the RSA public key for authenticating responses from
        the server for this app. You can get this key from your marketplace
        account page.

        ATTENTION: The key is an 512 bit one
    */
    RSAKey getPublicKey() override
    {
        return RSAKey("5,955d4af1ed484ec480757f4c7aec86bbcec4ba74f8ffd274b52a915aa30af9e07fc7d6c79943c3ac852950c5295d5cd1389058b8cede8a80d64daa5b04631393");
    }

    /** This method must store the given string somewhere in your app's
        persistent properties, so it can be retrieved later by getState().
    */
    void saveState (const String& state_) override
    {
        std::cout << "SAVE   " << state_ << std::endl;
        File project_folder = GET_ROOT_FOLDER();
        project_folder = File(project_folder.getFullPathName()+PROJECT_FOLDER);
        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));
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
        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));
        ScopedPointer<XmlElement> xml = XmlDocument( settings_session_file ).getDocumentElement();
	String state_;
        if( xml )
        {
            if( xml->hasTagName("SETTINGS-1.0") )
            {
                state_ = xml->getStringAttribute( "LAST_SAMPLE", "" );
		return state( state_, true );
            }
        }
        
        return state();
    }

public:
    static String state( String state_ = "", bool write_ = false ) noexcept
    { 
        static String __state;
        if( write_ )
        {
            __state = state_;
        }
        return __state;
    }

    Status() {}
    ~Status() {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Status)
};

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



