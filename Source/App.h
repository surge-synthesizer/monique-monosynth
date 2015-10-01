#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

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

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// INLINING //
#if JUCE_LINUX || JUCE_MAC
#define COLD __attribute__ ((noinline,cold))
#elif JUCE_WINDOWS
#define COLD __declspec(noinline)
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
    AudioSampleBuffer*const buffer;
    int size;

public:
    inline const float* getReadPointer (int channelNumber = 0) const noexcept;
    inline float* getWritePointer (int channelNumber = 0) noexcept;

    //==========================================================================
    COLD void setSize (int newNumSamples ) noexcept;
    inline int get_size() const noexcept { return size; }

    //==========================================================================
    COLD mono_AudioSampleBuffer(int numSamples) noexcept;
    COLD ~mono_AudioSampleBuffer() noexcept;
};

//==============================================================================
template<int num_channels>
COLD mono_AudioSampleBuffer<num_channels>::mono_AudioSampleBuffer(int numSamples) noexcept
:
buffer( new AudioSampleBuffer( num_channels, numSamples ) ), size( numSamples )
{
    buffer->clear();
}
template<int num_channels>
COLD mono_AudioSampleBuffer<num_channels>::~mono_AudioSampleBuffer() noexcept
{
    delete buffer;
}

//==============================================================================
template<int num_channels>
inline const float* mono_AudioSampleBuffer<num_channels>::getReadPointer (int channelNumber) const noexcept
{
    return buffer->getReadPointer( channelNumber );
}
template<int num_channels>
inline float* mono_AudioSampleBuffer<num_channels>::getWritePointer (int channelNumber) noexcept
{
    return buffer->getWritePointer( channelNumber );
}
template<int num_channels>
COLD void mono_AudioSampleBuffer<num_channels>::setSize (int newNumSamples ) noexcept
{
    buffer->setSize( num_channels, newNumSamples );
    size = newNumSamples;
}

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
#include "monique_AppInstanceStore.h"

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
#if JUCE_MAC
#	define ROOT_FOLDER userMusicDirectory
#elif JUCE_LINUX || RASPBERRY || JUCE_ANDROID
#	define ROOT_FOLDER userDocumentsDirectory
#elif JUCE_WINDOWS
#	define ROOT_FOLDER userDocumentsDirectory
#elif JUCE_IOS
#	define ROOT_FOLDER userDocumentsDirectory
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
        std::cout << count_samples << " notes:"<<  info_ <<  " >>"<<  std::fixed << std::setprecision(10) << std::abs( in_ ) << std::endl;
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

// TWEAKED ALGORYTHIMS
//==============================================================================
//==============================================================================
//==============================================================================
inline static int mono_floor(float x) noexcept
{
    const int i = int(x);
    return i - ( i > x );
}

#endif  // APP_H_INCLUDED



