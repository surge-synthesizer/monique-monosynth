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

#ifndef __JUCE_HEADER_4BD3D86F1072C2BE__
#define __JUCE_HEADER_4BD3D86F1072C2BE__

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
class Monique_Ui_OptionPopup : public juce::Component,
                               public Monique_Ui_Refreshable,
                               public juce::DropShadower,
                               public juce::Button::Listener
{
  public:
    //==============================================================================
    Monique_Ui_OptionPopup(Monique_Ui_Refresher *ui_refresher_,
                           Monique_Ui_Mainwindow *const parent_, BoolParameter *param_a_,
                           BoolParameter *param_b_);
    ~Monique_Ui_OptionPopup();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    juce::Component *related_to_comp;
    BoolParameter *const param_a;
    BoolParameter *const param_b;
    void set_element_to_show(juce::Component *const);
    void update_positions();
    void refresh() noexcept override;

    void set_infos(juce::StringRef text_a, juce::StringRef text_b, juce::StringRef tool_a,
                   juce::StringRef tool_b);

    const float original_w;
    const float original_h;

  private:
    Monique_Ui_Mainwindow *const parent;
    void mouseExit(const juce::MouseEvent &event) override;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> button_option_a;
    std::unique_ptr<juce::TextButton> button_option_b;
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_OptionPopup)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_4BD3D86F1072C2BE__
