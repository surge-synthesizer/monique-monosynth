/*
  ==============================================================================

    Synth.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MONO_UI_REFRESHER_H_INCLUDED
#define MONO_UI_REFRESHER_H_INCLUDED

#include "App.h"

//==============================================================================
class mono_UiRefreshable {
public:
    virtual void refresh() = 0;

protected:
    mono_UiRefreshable();
    virtual ~mono_UiRefreshable();
};

//==============================================================================
class mono_UiRefresher : public Timer {
    CriticalSection lock;

    Array<mono_UiRefreshable*> refreshables;

    void timerCallback() override;

public:
    void add(mono_UiRefreshable*const);
    void remove(mono_UiRefreshable*const);
    void remove_all();
    
    mono_UiRefresher() {}
    ~mono_UiRefresher() { clearSingletonInstance(); }

    juce_DeclareSingleton (mono_UiRefresher,false)
};


#endif
