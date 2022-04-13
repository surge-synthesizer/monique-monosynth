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

#ifndef __JUCE_HEADER_1C60A0F90B7BBE4E__
#define __JUCE_HEADER_1C60A0F90B7BBE4E__

#include "App.h"

class Monique_Ui_DragPad;

//==============================================================================

class Monique_Ui_MorphConfig : public juce::Component,
                               public Monique_Ui_Refreshable,
                               public juce::ComboBox::Listener,
                               public juce::Button::Listener,
                               public juce::Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_MorphConfig(Monique_Ui_Refresher *const ui_refresher_,
                           UiLookAndFeel *const look_and_feel_);
    ~Monique_Ui_MorphConfig();

    //==============================================================================
    const float original_w;
    const float original_h;
    int last_program;

  private:
    juce::Array<juce::ComboBox *> morph_combos;
    void refresh() noexcept override;
    void refresh_combos() noexcept;

    void paint(juce::Graphics &g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;

  private:
    juce::Array<int> bank_start_indexs;

    //==============================================================================
    std::unique_ptr<juce::Label> label_31;
    std::unique_ptr<juce::Label> label_32;
    std::unique_ptr<juce::Label> label_33;
    std::unique_ptr<juce::Label> label_30;
    std::unique_ptr<juce::Label> label_34;
    std::unique_ptr<juce::ComboBox> combo_morph_group_1;
    std::unique_ptr<juce::ComboBox> combo_morph_group_2;
    std::unique_ptr<juce::ComboBox> combo_morph_group_3;
    std::unique_ptr<juce::ComboBox> combo_morph_group_4;
    std::unique_ptr<juce::ComboBox> combo_morph_group_5;
    std::unique_ptr<juce::ComboBox> combo_morph_group_6;
    std::unique_ptr<juce::ComboBox> combo_morph_group_7;
    std::unique_ptr<juce::ComboBox> combo_morph_group_8;
    std::unique_ptr<juce::Label> label_19;
    std::unique_ptr<juce::TextButton> button_set_1;
    std::unique_ptr<juce::Label> label_ui_headline_1;
    std::unique_ptr<juce::Label> label_ui_headline_2;
    std::unique_ptr<juce::Label> label_ui_headline_3;
    std::unique_ptr<juce::Label> label_ui_headline_4;
    std::unique_ptr<juce::Label> label_4;
    std::unique_ptr<juce::Label> label_ui_headline_5;
    std::unique_ptr<juce::Label> label_35;
    std::unique_ptr<juce::Label> label_2;
    std::unique_ptr<juce::TextButton> button_set_2;
    std::unique_ptr<juce::Label> label_3;
    std::unique_ptr<juce::Label> label_5;
    std::unique_ptr<juce::Label> label_6;
    std::unique_ptr<juce::TextButton> button_set_3;
    std::unique_ptr<juce::Label> label_7;
    std::unique_ptr<juce::Label> label_8;
    std::unique_ptr<juce::Label> label_9;
    std::unique_ptr<juce::TextButton> button_set_4;
    std::unique_ptr<juce::Label> label_10;
    std::unique_ptr<juce::Label> label_11;
    std::unique_ptr<juce::Label> label_12;
    std::unique_ptr<juce::Slider> slider_morph_motor_time;
    std::unique_ptr<juce::Label> label_ui_headline_6;
    std::unique_ptr<juce::TextButton> button_set_r_1;
    std::unique_ptr<juce::TextButton> button_set_l_1;
    std::unique_ptr<juce::TextButton> button_set_r_2;
    std::unique_ptr<juce::TextButton> button_set_l_2;
    std::unique_ptr<juce::TextButton> button_set_r_3;
    std::unique_ptr<juce::TextButton> button_set_l_3;
    std::unique_ptr<juce::TextButton> button_set_r_4;
    std::unique_ptr<juce::TextButton> button_set_l_4;
    std::unique_ptr<Monique_Ui_DragPad> drag_pad;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_MorphConfig)
};

#endif // __JUCE_HEADER_1C60A0F90B7BBE4E__
