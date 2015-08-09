/*
  ==============================================================================

    mono_UiRefresher.cpp
    Created: 24 Apr 2015 12:44:25pm
    Author:  monotomy

  ==============================================================================
*/

#include "mono_UiRefresher.h"

//==============================================================================
juce_ImplementSingleton (mono_UiRefresher)

void mono_UiRefresher::timerCallback() {
    ScopedLock locked(lock);
    for( int i = 0 ; i != refreshables.size() ; ++i )
        refreshables.getUnchecked(i)->refresh();
}

//==============================================================================
mono_UiRefreshable::mono_UiRefreshable() {
    mono_UiRefresher::getInstance()->add(this);
}
mono_UiRefreshable::~mono_UiRefreshable() {
    mono_UiRefresher::getInstance()->remove( this );
}
void mono_UiRefresher::add(mono_UiRefreshable*const r_) {
    ScopedLock locked(lock);
    refreshables.add(r_);
}
void mono_UiRefresher::remove(mono_UiRefreshable*const r_) {
    ScopedLock locked(lock);
    refreshables.removeFirstMatchingValue(r_);
}
void mono_UiRefresher::remove_all() {
    ScopedLock locked(lock);
    refreshables.clearQuick();
}
