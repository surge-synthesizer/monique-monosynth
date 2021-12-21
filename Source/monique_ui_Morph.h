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

#ifndef __JUCE_HEADER_1C60A0F90B7BBE4E__
#define __JUCE_HEADER_1C60A0F90B7BBE4E__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

class Monique_Ui_DragPad;
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_MorphConfig : public Component,
                               public Monique_Ui_Refreshable,
                               public ComboBox::Listener,
                               public Button::Listener,
                               public Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MorphConfig(Monique_Ui_Refresher *const ui_refresher_,
                           UiLookAndFeel *const look_and_feel_);
    ~Monique_Ui_MorphConfig();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;
    int last_program;

  private:
    Array<ComboBox *> morph_combos;
    void refresh() noexcept override;
    void refresh_combos() noexcept;
    //[/UserMethods]

    void paint(Graphics &g) override;
    void resized() override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(Button *buttonThatWasClicked) override;
    void sliderValueChanged(Slider *sliderThatWasMoved) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Array<int> bank_start_indexs;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label_31;
    ScopedPointer<Label> label_32;
    ScopedPointer<Label> label_33;
    ScopedPointer<Label> label_30;
    ScopedPointer<Label> label_34;
    ScopedPointer<ComboBox> combo_morph_group_1;
    ScopedPointer<ComboBox> combo_morph_group_2;
    ScopedPointer<ComboBox> combo_morph_group_3;
    ScopedPointer<ComboBox> combo_morph_group_4;
    ScopedPointer<ComboBox> combo_morph_group_5;
    ScopedPointer<ComboBox> combo_morph_group_6;
    ScopedPointer<ComboBox> combo_morph_group_7;
    ScopedPointer<ComboBox> combo_morph_group_8;
    ScopedPointer<Label> label_19;
    ScopedPointer<TextButton> button_set_1;
    ScopedPointer<Label> label_ui_headline_1;
    ScopedPointer<Label> label_ui_headline_2;
    ScopedPointer<Label> label_ui_headline_3;
    ScopedPointer<Label> label_ui_headline_4;
    ScopedPointer<Label> label_4;
    ScopedPointer<Label> label_ui_headline_5;
    ScopedPointer<Label> label_35;
    ScopedPointer<Label> label_2;
    ScopedPointer<TextButton> button_set_2;
    ScopedPointer<Label> label_3;
    ScopedPointer<Label> label_5;
    ScopedPointer<Label> label_6;
    ScopedPointer<TextButton> button_set_3;
    ScopedPointer<Label> label_7;
    ScopedPointer<Label> label_8;
    ScopedPointer<Label> label_9;
    ScopedPointer<TextButton> button_set_4;
    ScopedPointer<Label> label_10;
    ScopedPointer<Label> label_11;
    ScopedPointer<Label> label_12;
    ScopedPointer<Slider> slider_morph_motor_time;
    ScopedPointer<Label> label_ui_headline_6;
    ScopedPointer<TextButton> button_set_r_1;
    ScopedPointer<TextButton> button_set_l_1;
    ScopedPointer<TextButton> button_set_r_2;
    ScopedPointer<TextButton> button_set_l_2;
    ScopedPointer<TextButton> button_set_r_3;
    ScopedPointer<TextButton> button_set_l_3;
    ScopedPointer<TextButton> button_set_r_4;
    ScopedPointer<TextButton> button_set_l_4;
    ScopedPointer<Monique_Ui_DragPad> drag_pad;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MorphConfig)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_1C60A0F90B7BBE4E__
