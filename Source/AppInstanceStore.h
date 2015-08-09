#ifndef APP_INSTANCE_STORE_INCLUDED
#define APP_INSTANCE_STORE_INCLUDED

// JUCE
#include "../JuceLibraryCode/JuceHeader.h"


// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
class GstepAudioProcessor;
class UiLookAndFeel;
class UiEditorSynthLite;
class mono_AmpPainter;
struct AppInstanceStore : public DeletedAtShutdown
{
    UiLookAndFeel* init_dummy;
    UiEditorSynthLite* editor;
    GstepAudioProcessor* audio_processor;
    volatile bool try_to_kill_amp_painter;
    CriticalSection amp_painter_lock;
    ScopedPointer<mono_AmpPainter> ampPainter;

    String save_standalone();
    String load_standalone();
    String save_plugin( XmlElement& xml );
    String load_plugin( const XmlElement& xml );

    juce_DeclareSingleton (AppInstanceStore,false)
    
    AppInstanceStore();
    ~AppInstanceStore();
};

#endif // APP_INSTANCE_STORE_INCLUDED


