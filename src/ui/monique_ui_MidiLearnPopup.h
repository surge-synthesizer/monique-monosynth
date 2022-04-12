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

#ifndef __JUCE_HEADER_4AEB729D0E305906__
#define __JUCE_HEADER_4AEB729D0E305906__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class Monique_Ui_Mainwindow;
class MIDIControl;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MainwindowPopup : public juce::Component,
                                   public Monique_Ui_Refreshable,
                                   public juce::DropShadower,
                                   public juce::ComboBox::Listener,
                                   public juce::Button::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MainwindowPopup(Monique_Ui_Refresher *ui_refresher_,
                               Monique_Ui_Mainwindow *const parent_, MIDIControl *midi_control_);
    ~Monique_Ui_MainwindowPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const juce::Component *related_to_comp;
    void set_element_to_show(const juce::Component *const);
    void update_positions();
    void refresh() noexcept override;

    const float original_w;
    const float original_h;

  private:
    Monique_Ui_Mainwindow *const parent;
    MIDIControl *_midi_control;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> combo_midi_number;
    std::unique_ptr<juce::TextButton> close;
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MainwindowPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_4AEB729D0E305906__
