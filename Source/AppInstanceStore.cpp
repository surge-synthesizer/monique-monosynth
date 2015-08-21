
#include "AppInstanceStore.h"
#include "PluginProcessor.h"
#include "UiLookAndFeel.h"
#include "mono_AmpPainter.h"

// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************

juce_ImplementSingleton (AppInstanceStore)

static inline UiLookAndFeel* init_lock_and_feel_hack() noexcept {
    LookAndFeel::setDefaultLookAndFeel( UiLookAndFeel::getInstance() );
    return UiLookAndFeel::getInstance();
}

AppInstanceStore::AppInstanceStore() noexcept
    :
    init_dummy( init_lock_and_feel_hack() ),
    editor( nullptr ),
    audio_processor( nullptr ),
    try_to_kill_amp_painter(false)
{
}

AppInstanceStore::~AppInstanceStore() noexcept
{
    editor = nullptr;

    clearSingletonInstance();
}