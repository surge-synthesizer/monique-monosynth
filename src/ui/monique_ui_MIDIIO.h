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

#ifndef __JUCE_HEADER_3437163A265DCA8E__
#define __JUCE_HEADER_3437163A265DCA8E__

//[Headers]     -- You can add your own extra header files here --
#if IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING

#include "App.h"

class mono_AudioDeviceManager;
class Monique_Ui_Mainwindow;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MidiIO : public juce::Component,
                          public Monique_Ui_Refreshable,
                          public juce::ComboBox::Listener,
                          public juce::Button::Listener,
                          public juce::Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MidiIO(Monique_Ui_Refresher *ui_refresher_,
                      mono_AudioDeviceManager *const audio_device_manager_,
                      Monique_Ui_Mainwindow *parent_);
    ~Monique_Ui_MidiIO();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Monique_Ui_Mainwindow *parent;

    int last_state_change;
    void refresh() noexcept override;
    void update_combo_boxed();

    const float original_w;
    const float original_h;

    bool has_grabbed_focus;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    mono_AudioDeviceManager *_audio_device_manager;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> label_7;
    std::unique_ptr<juce::ComboBox> combo_input_main;
    std::unique_ptr<juce::ComboBox> combo_input_main_channel;
    std::unique_ptr<juce::Label> label_3;
    std::unique_ptr<juce::ToggleButton> toggle_input_main_thru;
    std::unique_ptr<juce::Label> label_5;
    std::unique_ptr<juce::ComboBox> combo_output_thru;
    std::unique_ptr<juce::Label> label_6;
    std::unique_ptr<juce::ComboBox> combo_input_cc;
    std::unique_ptr<juce::ToggleButton> toggle_input_main_cc;
    std::unique_ptr<juce::ToggleButton> toggle_input_cc_thru;
    std::unique_ptr<juce::Label> label_9;
    std::unique_ptr<juce::ComboBox> combo_output_cc;
    std::unique_ptr<juce::Slider> slider_midi_pickup;
    std::unique_ptr<juce::Label> label_2;
    std::unique_ptr<juce::Label> label_4;
    std::unique_ptr<juce::TextButton> button_midi_learn;
    std::unique_ptr<juce::TextButton> close;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MidiIO)
};

//[EndFile] You can add extra defines here...
#else
class Monique_Ui_MidiIO
{
};
#endif
//[/EndFile]

#endif // __JUCE_HEADER_3437163A265DCA8E__
