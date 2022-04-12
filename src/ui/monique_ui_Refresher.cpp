/*
** Monique is Free and Open Source Software
**
** Monique is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html; The authors of the code
** reserve the right to re-license their contributions under the MIT license in the
** future at the discretion of the project maintainers.
**
** Copyright 2016-2022 by various individuals as described by the git transaction log
**
** All source at: https://github.com/surge-synthesizer/monique-monosynth.git
**
** Monique was a commercial product from 2016-2021, with copyright and ownership
** in that period held by Thomas Arndt at Monoplugs. Thomas made Monique
** open source in December 2021.
*/

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
    juce::ScopedLock locked(lock);
    refreshables.add(r_);
}
void Monique_Ui_Refresher::remove(Monique_Ui_Refreshable *const r_) noexcept
{
    if (refreshables.size() > 0)
    {
        juce::ScopedLock locked(lock);
        refreshables.removeFirstMatchingValue(r_);
    }
}
void Monique_Ui_Refresher::remove_all() noexcept
{
    juce::ScopedLock locked(lock);
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
