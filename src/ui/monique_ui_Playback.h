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

#ifndef __JUCE_HEADER_65FB963CF91AD20E__
#define __JUCE_HEADER_65FB963CF91AD20E__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Playback : public juce::Component,
                            public Monique_Ui_Refreshable,
                            public juce::Button::Listener,
                            public juce::Slider::Listener
{
  public:
    //==============================================================================
    Monique_Ui_Playback(Monique_Ui_Refresher *ui_refresher_, Monique_Ui_Mainwindow *parent_);
    ~Monique_Ui_Playback();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    Monique_Ui_Mainwindow *parent;

    void refresh() noexcept override;

    const float original_w;
    const float original_h;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> button_preset_agro;
    std::unique_ptr<juce::TextButton> button_preset_down;
    std::unique_ptr<juce::TextButton> button_preset_rising;
    std::unique_ptr<juce::TextButton> button_preset_soft;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_3;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_2;
    std::unique_ptr<juce::TextButton> button_flt_out_triggering_1;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_3;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_2;
    std::unique_ptr<juce::TextButton> button_flt_env_triggering_1;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_3;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_2;
    std::unique_ptr<juce::TextButton> button_flt_input_triggering_1;
    std::unique_ptr<juce::TextButton> filter_type_bg_button_1;
    std::unique_ptr<juce::TextButton> button_tracking_mode_hm;
    std::unique_ptr<juce::TextButton> button_tracking_mode_keep;
    std::unique_ptr<juce::TextButton> button_tracking_mode_lf;
    std::unique_ptr<juce::TextButton> button_tracking_mode_hf;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_3;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_2;
    std::unique_ptr<juce::Slider> slider_cutoff_tracking_oct_1;
    std::unique_ptr<juce::Slider> slider_osc_tracking_oct_3;
    std::unique_ptr<juce::Slider> slider_osc_tracking_oct_2;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_3;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_2;
    std::unique_ptr<juce::TextButton> button_cutoff_tracking_1;
    std::unique_ptr<juce::TextButton> button_osc_tracking_3;
    std::unique_ptr<juce::TextButton> button_osc_tracking_2;
    std::unique_ptr<juce::TextButton> button_osc_tracking_1;
    std::unique_ptr<juce::Label> label_2;
    std::unique_ptr<juce::Label> label_4;
    std::unique_ptr<juce::Label> label_5;
    std::unique_ptr<juce::Label> label_6;
    std::unique_ptr<juce::Label> label_7;
    std::unique_ptr<juce::Label> label_8;
    std::unique_ptr<juce::Label> label_9;
    std::unique_ptr<juce::Label> label_10;
    std::unique_ptr<juce::TextButton> button_osc_key_tracking_highest2;
    std::unique_ptr<juce::Label> label_12;
    std::unique_ptr<juce::Label> label_13;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_1;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_2;
    std::unique_ptr<juce::Slider> slider_flt_out_sesitivity_3;
    std::unique_ptr<juce::Label> label_21;
    std::unique_ptr<juce::Label> label_22;
    std::unique_ptr<juce::Label> label_23;
    std::unique_ptr<juce::Label> label_24;
    std::unique_ptr<juce::TextButton> close;
    std::unique_ptr<juce::Label> label_3;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Playback)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif // __JUCE_HEADER_65FB963CF91AD20E__
