#include "monique_ui_Refresher.h"

//==============================================================================

Monique_Ui_Refresher::~Monique_Ui_Refresher() noexcept {}

//==============================================================================
void Monique_Ui_Refresher::timerCallback()
{
    // MessageManagerLock mmLock;
    for (int i = 0; i != refreshables.size(); ++i)
    {
        refreshables.getUnchecked(i)->refresh();
    }
}

void Monique_Ui_Refresher::add(Monique_Ui_Refreshable *const r_) noexcept
{
    ScopedLock locked(lock);
    refreshables.add(r_);
}
void Monique_Ui_Refresher::remove(Monique_Ui_Refreshable *const r_) noexcept
{
    if (refreshables.size() > 0)
    {
        ScopedLock locked(lock);
        refreshables.removeFirstMatchingValue(r_);
    }
}
void Monique_Ui_Refresher::remove_all() noexcept
{
    ScopedLock locked(lock);
    refreshables.clearQuick();
}

//==============================================================================
Monique_Ui_Refreshable::Monique_Ui_Refreshable(Monique_Ui_Refresher *ui_refresher_) noexcept
    : ui_refresher(ui_refresher_), look_and_feel(ui_refresher_->look_and_feel),
      midi_control_handler(ui_refresher_->midi_control_handler),
      synth_data(ui_refresher_->synth_data), voice(ui_refresher_->voice)
{
    ui_refresher_->add(this);
}
Monique_Ui_Refreshable::~Monique_Ui_Refreshable() noexcept { ui_refresher->remove(this); }

Monique_Ui_Mainwindow *Monique_Ui_Refreshable::get_editor() noexcept
{
    return ui_refresher->editor;
}
