#include "monique_ui_Refresher.h"

//==============================================================================
juce_ImplementSingleton (Monique_Ui_Refresher)
COLD Monique_Ui_Refresher::Monique_Ui_Refresher() noexcept {}
COLD Monique_Ui_Refresher::~Monique_Ui_Refresher() noexcept 
{
    clearSingletonInstance();
}

//==============================================================================
void Monique_Ui_Refresher::timerCallback() 
{
    for( int i = 0 ; i != refreshables.size() ; ++i )
    {
        refreshables.getUnchecked(i)->refresh();
    }
}

COLD void Monique_Ui_Refresher::add(Monique_Ui_Refreshable*const r_) noexcept {
    ScopedLock locked(lock);
    refreshables.add(r_);
}
COLD void Monique_Ui_Refresher::remove(Monique_Ui_Refreshable*const r_) noexcept {
    ScopedLock locked(lock);
    refreshables.removeFirstMatchingValue(r_);
}
COLD void Monique_Ui_Refresher::remove_all() noexcept {
    ScopedLock locked(lock);
    refreshables.clearQuick();
}

//==============================================================================
Monique_Ui_Refreshable::Monique_Ui_Refreshable() noexcept {
    Monique_Ui_Refresher::getInstance()->add(this);
}
Monique_Ui_Refreshable::~Monique_Ui_Refreshable() noexcept {
    Monique_Ui_Refresher::getInstance()->remove( this );
}
