#ifndef APP_INSTANCE_STORE_INCLUDED
#define APP_INSTANCE_STORE_INCLUDED

#include "App_h_includer.h"

//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueAudioProcessor;
class UiLookAndFeel;
class Monique_Ui_Mainwindow;
class Monique_Ui_AmpPainter;
struct AppInstanceStore : public DeletedAtShutdown
{
    UiLookAndFeel* init_dummy;
    Monique_Ui_Mainwindow* editor;
    MoniqueAudioProcessor* audio_processor;

    //==============================================================================
private:
    CriticalSection amp_painter_lock;
    ScopedPointer< Monique_Ui_AmpPainter > ampPainter;

public:
    Monique_Ui_AmpPainter* get_create_amp_painter() noexcept;
    // PLEASE LOCK THE AMP PAINTER FOR A BIGGER BLOCK OF WORK AND THEN GET IT
    void lock_amp_painter() noexcept;
    inline Monique_Ui_AmpPainter* get_amp_painter_unsave() noexcept { return ampPainter; }
    void unlock_amp_painter() noexcept;
    void kill_amp_painter() noexcept;

public:
    //==============================================================================
    juce_DeclareSingleton (AppInstanceStore,false)

private:
    AppInstanceStore() noexcept;
    ~AppInstanceStore() noexcept;
};

#endif
