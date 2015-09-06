#ifndef MONO_UI_REFRESHER_H_INCLUDED
#define MONO_UI_REFRESHER_H_INCLUDED

#include "App_h_includer.h"

//==============================================================================
class Monique_Ui_Refreshable
{
public:
    virtual void refresh() noexcept = 0;

protected:
    COLD Monique_Ui_Refreshable() noexcept;
    COLD ~Monique_Ui_Refreshable() noexcept;
};

//==============================================================================
class Monique_Ui_Refresher : public Timer 
{
    CriticalSection lock;
    Array<Monique_Ui_Refreshable*> refreshables;

    void timerCallback() override;

private:
    friend class Monique_Ui_Refreshable;
    COLD void add(Monique_Ui_Refreshable*const) noexcept;
    COLD void remove(Monique_Ui_Refreshable*const) noexcept;

public:
    COLD void remove_all() noexcept;

private:
    COLD Monique_Ui_Refresher() noexcept;
    COLD ~Monique_Ui_Refresher() noexcept;

public:
    juce_DeclareSingleton_SingleThreaded_Minimal (Monique_Ui_Refresher)
};

#endif
