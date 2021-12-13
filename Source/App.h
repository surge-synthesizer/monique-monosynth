#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#define JUCE_DONT_DECLARE_PROJECTINFO 1
namespace ProjectInfo
{
    const char* const  projectName    = "Monique";
    const char* const  versionString  = "1.1";
    const int          versionNumber  = 0x10100;
}

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
#include <JuceHeader.h>
#elif IS_STANDALONE
#include <JuceHeader.h>
#endif
#ifdef JucePlugin_Build_Standalone
#ifdef IS_PLUGIN
#define AUTO_STANDALONE
#endif
#endif

#ifdef JUCE_IOS
#define IS_MOBILE
#endif
#ifdef JUCE_ANDROID
#define IS_MOBILE
#endif


#ifdef JUCE_WINDOWS
//#include "vld.h" // need for debuging, but can be removed without any effects!
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// INLINING //
#if JUCE_LINUX || JUCE_MAC
#define COLD
//__attribute__ ((noinline,cold))
#elif JUCE_WINDOWS || JUCE_IOS || JUCE_ANDROID
#define COLD
//__declspec(noinline)
#else
#define COLD inline
#endif

#include <JuceHeader.h>

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

    using AudioSampleBuffer = juce::AudioBuffer<float>;
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

#define UI_REFRESH_RATE 50 // MS
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
class Status //: public OnlineUnlockStatus
{
    String __state;

    /** This method must store the given string somewhere in your app's
        persistent properties, so it can be retrieved later by getState().
    */
    void saveState (const String& state_)
    {
        File project_folder = GET_ROOT_FOLDER();
        project_folder = File(project_folder.getFullPathName()+PROJECT_FOLDER);

        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));

	ScopedPointer<XmlElement> xml = XmlDocument( settings_session_file ).getDocumentElement().release();
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
    String getState()
    {
        File project_folder = GET_ROOT_FOLDER();
        project_folder = File(project_folder.getFullPathName()+PROJECT_FOLDER);
        File settings_session_file = File(project_folder.getFullPathName()+String("/session.mcfg"));
        ScopedPointer<XmlElement> xml = XmlDocument( settings_session_file ).getDocumentElement().release();
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



