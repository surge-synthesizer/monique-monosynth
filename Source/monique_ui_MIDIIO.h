/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
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
