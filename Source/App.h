/*
  ==============================================================================

    App.h
    Created: 28 May 2014 10:30:02am
    Author:  monotomy

  ==============================================================================
*/

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#define ff161616 ff101010

#ifdef MONIQUE_STANDALONE
#	define IS_STANDALONE
#else
#	define IS_PLUGIN
#endif

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
#ifdef MONIQUE_PLUGIN // MOST OF THE TIME WE DEVEL IN STANDALONE MODE
#include "../Plugin/JuceLibraryCode/JuceHeader.h"
#include "../Plugin/JuceLibraryCode/BinaryData.h"
#else
#include "../Standalone/JuceLibraryCode/JuceHeader.h"
#include "../Standalone/JuceLibraryCode/BinaryData.h"
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// INLINING
#if JUCE_LINUX || JUCE_MAC
#define NOINLINE __attribute__ ((noinline))
#elif JUCE_WINDOWS
#define NOINLINE __declspec(noinline)
#else 
#define NOINLINE todo_UNKNOWN_PLATTFORM__________
#endif
  
// MSVC REPLACEMENTS
#define CONSTEXPR_SUPPORT
  
#if JUCE_WINDOWS
#define constexpr const
#undef CONSTEXPR_SUPPORT
#define or ||
#define and &&
#define not !
#endif
  
  // MOVE SUPPORT
#if JUCE_MAC
  #define MOVE(x) x
#elif JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
  #define MOVE(x) std::move(x)
#else
  #define MOVE(x) x
#endif

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#include "dRowAudio_AudioUtility.h"
#include "dRowAudio_MathsUtilities.h"
#include "AppInstanceStore.h"

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

#define UI_REFRESH_RATE 34 // MS
#include "mono_Parameters.h"
#include "mono_UiRefresher.h"

// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
#if JUCE_MAC
#	define ROOT_FOLDER userMusicDirectory
#	define MONOLISA_ROOT_FOLDER "/MONOLisa"
#elif JUCE_LINUX || RASPBERRY || JUCE_ANDROID
#	define ROOT_FOLDER userDocumentsDirectory
#	define MONOLISA_ROOT_FOLDER "/MONOLisa"
#elif JUCE_WINDOWS
#	define ROOT_FOLDER userDocumentsDirectory
#	define MONOLISA_ROOT_FOLDER "/MONOLisa"
#elif JUCE_IOS
#	define ROOT_FOLDER userDocumentsDirectory
#	define MONOLISA_ROOT_FOLDER "/MONOLisa"
#endif
  
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
  
static inline int round0( float value ) {
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value);
}
static inline float round01( float value ) {
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value*10)/10;
}
static inline float round001( float value ) {
    using namespace std; // MAC COMPILER PROBLEMS
    return roundf(value*100)/100;
}
  
#endif  // APP_H_INCLUDED
