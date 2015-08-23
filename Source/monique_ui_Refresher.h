#ifndef MONO_UI_REFRESHER_H_INCLUDED
#define MONO_UI_REFRESHER_H_INCLUDED

#include "App_h_includer.h"

//==============================================================================
class mono_UiRefreshable 
{
public:
    virtual void refresh() noexcept = 0;

protected:
    mono_UiRefreshable() noexcept;
    virtual ~mono_UiRefreshable() noexcept;
};

//==============================================================================
class mono_UiRefresher : public Timer {
    CriticalSection lock;
    Array<mono_UiRefreshable*> refreshables;

    void timerCallback() override;

private:
    friend class mono_UiRefreshable;
    NOINLINE void add(mono_UiRefreshable*const) noexcept;
    NOINLINE void remove(mono_UiRefreshable*const) noexcept;

public:
    NOINLINE void remove_all() noexcept;

private:
    mono_UiRefresher() noexcept;
    ~mono_UiRefresher() noexcept;

public:
    juce_DeclareSingleton (mono_UiRefresher,false)
};

#endif
