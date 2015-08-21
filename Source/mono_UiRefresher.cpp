#include "mono_UiRefresher.h"

//==============================================================================
mono_UiRefresher::mono_UiRefresher() noexcept {}
mono_UiRefresher::~mono_UiRefresher() noexcept {
    clearSingletonInstance();
}
//==============================================================================
juce_ImplementSingleton (mono_UiRefresher)

void mono_UiRefresher::timerCallback() 
{
    Thread::setCurrentThreadPriority(1);
    for( int i = 0 ; i != refreshables.size() ; ++i )
    {
        refreshables.getUnchecked(i)->refresh();
    }
}

NOINLINE void mono_UiRefresher::add(mono_UiRefreshable*const r_) noexcept {
    ScopedLock locked(lock);
    refreshables.add(r_);
}
NOINLINE void mono_UiRefresher::remove(mono_UiRefreshable*const r_) noexcept {
    ScopedLock locked(lock);
    refreshables.removeFirstMatchingValue(r_);
}
NOINLINE void mono_UiRefresher::remove_all() noexcept {
    ScopedLock locked(lock);
    refreshables.clearQuick();
}

//==============================================================================
mono_UiRefreshable::mono_UiRefreshable() noexcept {
    mono_UiRefresher::getInstance()->add(this);
}
mono_UiRefreshable::~mono_UiRefreshable() noexcept {
    mono_UiRefresher::getInstance()->remove( this );
}
