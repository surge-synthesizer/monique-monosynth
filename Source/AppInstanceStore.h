#ifndef APP_INSTANCE_STORE_INCLUDED
#define APP_INSTANCE_STORE_INCLUDED

#include "App_h_includer.h"

class MoniqueAudioProcessor;
class UiLookAndFeel;
class UiEditorSynthLite;
class mono_AmpPainter;
struct AppInstanceStore : public DeletedAtShutdown
{
    UiLookAndFeel* init_dummy;
    UiEditorSynthLite* editor;
    MoniqueAudioProcessor* audio_processor;
    
    volatile bool try_to_kill_amp_painter;
    CriticalSection amp_painter_lock;
    ScopedPointer< mono_AmpPainter > ampPainter;

    juce_DeclareSingleton (AppInstanceStore,false)
    
private:
    AppInstanceStore() noexcept;
    ~AppInstanceStore() noexcept;
};

#endif