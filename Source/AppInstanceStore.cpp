
#include "AppInstanceStore.h"
#include "PluginProcessor.h"
#include "UiLookAndFeel.h"
#include "mono_AmpPainter.h"

// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************

juce_ImplementSingleton (AppInstanceStore)

UiLookAndFeel* init_lock_and_feel_hack() {
    LookAndFeel::setDefaultLookAndFeel( UiLookAndFeel::getInstance() );
    return nullptr;
}
AppInstanceStore::AppInstanceStore()
    :
    init_dummy( init_lock_and_feel_hack() ),
    editor( nullptr ),
    audio_processor( nullptr ),
    try_to_kill_amp_painter(false)
{
}
AppInstanceStore::~AppInstanceStore()
{
    editor = nullptr;

    clearSingletonInstance();
}
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
String AppInstanceStore::save_standalone() {
    String error;
    return error;
}

String AppInstanceStore::load_standalone() {
    String error;
    return error;
}

String AppInstanceStore::save_plugin( XmlElement& xml_ ) {
    String error;
    return error;
}
String AppInstanceStore::load_plugin( const XmlElement& xml_ ) {
    String error;
    return error;
}
