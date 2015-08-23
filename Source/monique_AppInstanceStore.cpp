
#include "monique_AppInstanceStore.h"

#include "UiLookAndFeel.h"
#include "monique_ui_AmpPainter.h"

//==============================================================================
static inline UiLookAndFeel* init_lock_and_feel_hack() noexcept 
{
    LookAndFeel::setDefaultLookAndFeel( UiLookAndFeel::getInstance() );
    return UiLookAndFeel::getInstance();
}

//==============================================================================
juce_ImplementSingleton (AppInstanceStore)

//==============================================================================
AppInstanceStore::AppInstanceStore() noexcept
:
init_dummy( init_lock_and_feel_hack() ),
            editor( nullptr ),
            audio_processor( nullptr )
{
}

AppInstanceStore::~AppInstanceStore() noexcept
{
    editor = nullptr;
    clearSingletonInstance();
}

//==============================================================================
void AppInstanceStore::lock_amp_painter() noexcept
{
    amp_painter_lock.enter();
    if( ampPainter )
      ampPainter->lock_for_writing();
}

mono_AmpPainter* AppInstanceStore::get_create_amp_painter() noexcept
{
    if( ! ampPainter )
    {
        ScopedLock locked(amp_painter_lock);
        ampPainter = new mono_AmpPainter();
    }
    
    return ampPainter;
}

void AppInstanceStore::unlock_amp_painter() noexcept
{
    amp_painter_lock.exit();
    if( ampPainter )
      ampPainter->unlock_for_writing();
}
void AppInstanceStore::kill_amp_painter() noexcept
{
    ScopedLock locked(amp_painter_lock);
    if(ampPainter)
    {
        ampPainter = nullptr;
    }
}

