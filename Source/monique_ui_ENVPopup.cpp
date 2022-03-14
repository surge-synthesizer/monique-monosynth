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

//[Headers] You can add your own extra header files here...
#include "monique_ui_MainWindow.h"
#include "monique_ui_LookAndFeel.h"
#include "monique_ui_ModulationSlider.h"

#include "monique_core_Processor.h"
//[/Headers]

#include "monique_ui_ENVPopup.h"
#include <memory>

//[MiscUserDefs] You can add your own user definitions and misc code here...
void Monique_Ui_ENVPopup::timerCallback()
{
    if (is_repainting)
        return;

    if (++callbacks > 9)
        stopTimer();

    juce::MessageManagerLock mmLock;
    repaint(plotter->getBounds().getX() - 1, plotter->getBounds().getY() - 1,
            plotter->getBounds().getWidth() + 2, plotter->getBounds().getHeight() + 2);
}
void Monique_Ui_ENVPopup::refresh() noexcept
{
    if (is_repainting)
    {
        return;
    }

    const float attack__ = env_data->attack.get_value();
    const float sustain__ = env_data->sustain.get_value();
    const float decay__ = env_data->decay.get_value();
    const float release__ = env_data->release.get_value();
    const float shape__ = env_data->shape.get_value();
    const float sustain_time__ = env_data->sustain_time.get_value();

    if (last_attack != attack__ || last_sustain != sustain__ or last_decay != decay__ ||
        sustain_time != sustain_time__ || last_release != release__ || last_shape != shape__)
    {
        last_attack = attack__;
        last_sustain = sustain__;
        last_decay = decay__;
        sustain_time = sustain_time__;
        last_shape = shape__;
        last_release = release__;

        const juce::Component *const comp_under_mouse = getCurrentlyFocusedComponent();
        if (getCurrentlyFocusedComponent() != slider_attack.get() &&
            comp_under_mouse != slider_attack.get())
        {
            slider_attack->setValue(last_attack, juce::dontSendNotification);
        }
        label_attack->setText(juce::String(auto_round(get_env_ms(last_attack))) +
                                  juce::String("ms"),
                              juce::dontSendNotification);

        if (last_decay > 0)
        {
            if (getCurrentlyFocusedComponent() != slider_decay.get() &&
                comp_under_mouse != slider_decay.get())
            {
                slider_decay->setValue(last_decay, juce::dontSendNotification);
            }
            label_decay->setText(juce::String(auto_round(get_env_ms(last_decay))) +
                                     juce::String("ms"),
                                 juce::dontSendNotification);
        }
        else
        {
            if (getCurrentlyFocusedComponent() != slider_decay.get() &&
                comp_under_mouse != slider_decay.get())
            {
                slider_decay->setValue(0, juce::dontSendNotification);
            }
            label_decay->setText(juce::String("OFF"), juce::dontSendNotification);
        }

        if (getCurrentlyFocusedComponent() != slider_sustain.get() &&
            comp_under_mouse != slider_sustain.get())
        {
            slider_sustain->setValue(last_sustain, juce::dontSendNotification);
        }
        slider_sustain->setValue(last_sustain, juce::dontSendNotification);
        label_sustain->setText(juce::String(slider_sustain->getValue() * 100),
                               juce::dontSendNotification);

        if (getCurrentlyFocusedComponent() != slider_sustain_time.get() &&
            comp_under_mouse != slider_sustain_time.get())
        {
            slider_sustain_time->setValue(sustain_time, juce::dontSendNotification);
        }
        if (slider_sustain_time->getValue() < 1)
        {
            label_sustain_time->setText(juce::String(auto_round(get_env_ms(sustain_time))) +
                                            juce::String("ms"),
                                        juce::dontSendNotification);
        }
        else
        {
            label_sustain_time->setText(juce::String("UNLTD"), juce::dontSendNotification);
        }

        if (getCurrentlyFocusedComponent() != slider_release.get() &&
            comp_under_mouse != slider_release.get())
        {
            slider_release->setValue(last_release, juce::dontSendNotification);
        }
        label_release->setText(juce::String(auto_round(get_env_ms(last_release))) +
                                   juce::String("ms"),
                               juce::dontSendNotification);

        if (getCurrentlyFocusedComponent() != slider_env_shape.get() &&
            comp_under_mouse != slider_env_shape.get())
        {
            slider_env_shape->setValue(last_shape, juce::dontSendNotification);
        }

        stopTimer();
        callbacks = 0;
        startTimer(synth_data->glide_motor_time.get_value() / 10 + 5);
    }

    {
        ComponentColours &colours = look_and_feel->colours;
        juce::Colour button_off = colours.get_theme(theme).area_colour;
        auto_close->setColour(juce::TextButton::buttonColourId, synth_data->auto_close_env_popup
                                                                    ? juce::Colours::yellow
                                                                    : button_off);
        keep->setColour(juce::TextButton::buttonColourId,
                        synth_data->auto_switch_env_popup ? juce::Colours::green : button_off);
    }

    past->setEnabled(has_ENV_clipboard_data()->value);
}

void Monique_Ui_ENVPopup::set_element_to_show(juce::Component *const comp_,
                                              Monique_Ui_DualSlider *owner_)
{
    theme = owner_->_config->get_colour_theme();
    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        juce::Component *child = getChildComponent(i);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, theme);
    }

    owner_slider = owner_;
    related_to_comp = comp_;

    int x = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getX();
    int y = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getY();

    const float width_scale = 1.0f / original_w * getWidth();
    const int left_move =
        !left ? getWidth() - (width_scale * 80) + (width_scale * 10) : (width_scale * 10);
    setTopLeftPosition(x - left_move, y + comp_->getHeight());
}
void Monique_Ui_ENVPopup::update_positions()
{
    if (related_to_comp)
    {
        set_element_to_show(related_to_comp, owner_slider);
    }
}
void Monique_Ui_ENVPopup::set_clickable_components(juce::Array<juce::Component *> &comps_) noexcept
{
    observed_comps = comps_;
    for (int i = 0; i != observed_comps.size(); ++i)
    {
        juce::Component *comp = observed_comps.getUnchecked(i);
        if (dynamic_cast<Monique_Ui_Mainwindow *>(comp))
        {
            comp->addMouseListener(this, false);
        }
        else
        {
            comp->addMouseListener(this, true);
        }
    }
}
void Monique_Ui_ENVPopup::mouseDown(const juce::MouseEvent &event)
{
    if (Monique_Ui_Mainwindow *mainwindow = get_editor())
    {
        bool success = false;
        juce::Component *const event_comp = event.eventComponent;
        juce::Component *comp = event_comp;
        if (comp != this)
        {
            while (comp)
            {
                if (Monique_Ui_DualSlider *dual_slider =
                        dynamic_cast<Monique_Ui_DualSlider *>(comp))
                {
                    const bool keeps_open = mainwindow->handle_keep_env_open(dual_slider->_config);
                    success = true;
                    if (keeps_open && synth_data->auto_switch_env_popup)
                    {
                        if (owner_slider != dual_slider && owner_slider)
                        {
                            mainwindow->open_env_popup(dual_slider);
                            return;
                        }
                    }

                    break;
                }
                comp = comp->getParentComponent();
            }
            if (!success && synth_data->auto_close_env_popup)
            {
                mainwindow->open_env_popup(nullptr, nullptr, nullptr, nullptr, false);
            }
        }
    }
}
void Monique_Ui_ENVPopup::mouseDrag(const juce::MouseEvent &event) { mouseDown(event); }
void Monique_Ui_ENVPopup::mouseUp(const juce::MouseEvent &event)
{
    // mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseDoubleClick(const juce::MouseEvent &event) { mouseDown(event); }
void Monique_Ui_ENVPopup::mouseWheelMove(const juce::MouseEvent &event,
                                         const juce::MouseWheelDetails &)
{
    mouseDown(event);
}
void Monique_Ui_ENVPopup::mouseMagnify(const juce::MouseEvent &event, float) { mouseDown(event); }
void Monique_Ui_ENVPopup::parameter_value_changed(Parameter *param_) noexcept
{
    if (param_ == &ui_refresher->synth_data->midi_env_attack)
    {
        env_data->attack = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_env_decay)
    {
        env_data->decay = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_env_sustain)
    {
        env_data->sustain = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_env_sustain_time)
    {
        env_data->sustain_time = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_env_release)
    {
        env_data->release = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_env_shape)
    {
        env_data->shape = param_->get_value();
    }
}
void Monique_Ui_ENVPopup::sliderClicked(juce::Slider *s_)
{
    if (IS_MIDI_LEARN)
    {
        sliderValueChanged(s_);
    }
}
//[/MiscUserDefs]

//==============================================================================
Monique_Ui_ENVPopup::Monique_Ui_ENVPopup(Monique_Ui_Refresher *ui_refresher_,
                                         Monique_Ui_Mainwindow *const parent_,
                                         ENVData *const env_data_, Parameter *const sustain_,
                                         bool left_, bool has_negative_sustain_)
    : Monique_Ui_Refreshable(ui_refresher_), juce::DropShadower(juce::DropShadow(
                                                 juce::Colours::black.withAlpha(0.8f), 10,
                                                 juce::Point<int>(10, 10))),
      original_w(710), original_h(190), left(left_), parent(parent_), env_data(env_data_),
      sustain(sustain_)
{
    //[Constructor_pre] You can add your own custom stuff here..

    last_attack = 0;
    last_sustain = 0;
    last_decay = 0;
    last_release = 0;
    last_shape = 0;
    sustain_time = 0;
    owner_slider = nullptr;
    setOwner(this);
    is_repainting = false;
    theme = COLOUR_THEMES::FILTER_THEME;
    //[/Constructor_pre]

    label_attack_bottom = std::make_unique<juce::Label>(juce::String(), TRANS("ATT"));
    addAndMakeVisible(*label_attack_bottom);
    label_attack_bottom->setFont(juce::Font(15.00f, juce::Font::plain));
    label_attack_bottom->setJustificationType(juce::Justification::centred);
    label_attack_bottom->setEditable(false, false, false);
    label_attack_bottom->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_attack_bottom->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_attack_bottom->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_attack = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_attack);
    slider_attack->setTooltip(TRANS("Define the curve attack in ms."));
    slider_attack->setRange(0, 1, 0.0002);
    slider_attack->setSliderStyle(juce::Slider::LinearVertical);
    slider_attack->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_attack->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_attack->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_attack->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_attack->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_attack->addListener(this);

    label_decay_bottom = std::make_unique<juce::Label>(juce::String(), TRANS("DEC"));
    addAndMakeVisible(*label_decay_bottom);
    label_decay_bottom->setFont(juce::Font(15.00f, juce::Font::plain));
    label_decay_bottom->setJustificationType(juce::Justification::centred);
    label_decay_bottom->setEditable(false, false, false);
    label_decay_bottom->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_decay_bottom->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_decay_bottom->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_decay = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_decay);
    slider_decay->setTooltip(TRANS("Define the curve decay in ms."));
    slider_decay->setRange(0, 1, 0.0002);
    slider_decay->setSliderStyle(juce::Slider::LinearVertical);
    slider_decay->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_decay->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_decay->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_decay->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_decay->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_decay->addListener(this);

    label_release_bottom = std::make_unique<juce::Label>(juce::String(), TRANS("REL"));
    addAndMakeVisible(*label_release_bottom);
    label_release_bottom->setFont(juce::Font(15.00f, juce::Font::plain));
    label_release_bottom->setJustificationType(juce::Justification::centred);
    label_release_bottom->setEditable(false, false, false);
    label_release_bottom->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_release_bottom->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_release_bottom->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_release = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_release);
    slider_release->setTooltip(TRANS("Define the curve release in ms."));
    slider_release->setRange(0, 1, 0.0002);
    slider_release->setSliderStyle(juce::Slider::LinearVertical);
    slider_release->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_release->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_release->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_release->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_release->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_release->addListener(this);

    slider_sustain_time = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_sustain_time);
    slider_sustain_time->setTooltip(TRANS("Define the sustain time in ms."));
    slider_sustain_time->setRange(0, 1, 0.0002);
    slider_sustain_time->setSliderStyle(juce::Slider::LinearVertical);
    slider_sustain_time->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_sustain_time->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_sustain_time->setColour(juce::Slider::rotarySliderOutlineColourId,
                                   juce::Colour(0xff161616));
    slider_sustain_time->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_sustain_time->setColour(juce::Slider::textBoxBackgroundColourId,
                                   juce::Colour(0xff161616));
    slider_sustain_time->addListener(this);

    label_sustain_time_bottom = std::make_unique<juce::Label>(juce::String(), TRANS("RETRIG"));
    addAndMakeVisible(*label_sustain_time_bottom);
    label_sustain_time_bottom->setFont(juce::Font(15.00f, juce::Font::plain));
    label_sustain_time_bottom->setJustificationType(juce::Justification::centred);
    label_sustain_time_bottom->setEditable(false, false, false);
    label_sustain_time_bottom->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_sustain_time_bottom->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_sustain_time_bottom->setColour(juce::TextEditor::backgroundColourId,
                                         juce::Colour(0x00000000));

    label_attack = std::make_unique<juce::Label>("VL", TRANS("x\n"));
    addAndMakeVisible(*label_attack);
    label_attack->setFont(juce::Font(15.00f, juce::Font::plain));
    label_attack->setJustificationType(juce::Justification::centred);
    label_attack->setEditable(true, true, false);
    label_attack->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_attack->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_attack->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_attack->addListener(this);

    label_decay = std::make_unique<juce::Label>("VL", TRANS("x\n"));
    addAndMakeVisible(*label_decay);
    label_decay->setFont(juce::Font(15.00f, juce::Font::plain));
    label_decay->setJustificationType(juce::Justification::centred);
    label_decay->setEditable(true, true, false);
    label_decay->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_decay->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_decay->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_decay->addListener(this);

    label_sustain_time = std::make_unique<juce::Label>("VL", TRANS("x\n"));
    addAndMakeVisible(*label_sustain_time);
    label_sustain_time->setFont(juce::Font(15.00f, juce::Font::plain));
    label_sustain_time->setJustificationType(juce::Justification::centred);
    label_sustain_time->setEditable(true, true, false);
    label_sustain_time->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_sustain_time->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_sustain_time->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_sustain_time->addListener(this);

    label_release = std::make_unique<juce::Label>("VL", TRANS("x\n"));
    addAndMakeVisible(*label_release);
    label_release->setFont(juce::Font(15.00f, juce::Font::plain));
    label_release->setJustificationType(juce::Justification::centred);
    label_release->setEditable(true, true, false);
    label_release->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_release->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_release->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_release->addListener(this);

    slider_sustain = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_sustain);
    slider_sustain->setTooltip(TRANS("Define the curve sustain in ms."));
    slider_sustain->setRange(0, 1, 0.001);
    slider_sustain->setSliderStyle(juce::Slider::LinearVertical);
    slider_sustain->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_sustain->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_sustain->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_sustain->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_sustain->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_sustain->addListener(this);

    label_sustain_bottom = std::make_unique<juce::Label>(juce::String(), TRANS("SUS"));
    addAndMakeVisible(*label_sustain_bottom);
    label_sustain_bottom->setFont(juce::Font(15.00f, juce::Font::plain));
    label_sustain_bottom->setJustificationType(juce::Justification::centred);
    label_sustain_bottom->setEditable(false, false, false);
    label_sustain_bottom->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_sustain_bottom->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_sustain_bottom->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_sustain = std::make_unique<juce::Label>("VL", TRANS("x\n"));
    addAndMakeVisible(*label_sustain);
    label_sustain->setFont(juce::Font(15.00f, juce::Font::plain));
    label_sustain->setJustificationType(juce::Justification::centred);
    label_sustain->setEditable(true, true, false);
    label_sustain->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_sustain->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_sustain->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    label_sustain->addListener(this);

    plotter = std::make_unique<juce::Component>();
    addAndMakeVisible(*plotter);

    slider_env_shape = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_env_shape);
    slider_env_shape->setTooltip(TRANS("Define the curve shape type."));
    slider_env_shape->setRange(-1, 1, 0.01);
    slider_env_shape->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_env_shape->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_env_shape->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_env_shape->setColour(juce::Slider::rotarySliderOutlineColourId,
                                juce::Colour(0xff161616));
    slider_env_shape->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_env_shape->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_env_shape->addListener(this);

    label_shape = std::make_unique<juce::Label>("new label", TRANS("SLOPE"));
    addAndMakeVisible(*label_shape);
    label_shape->setFont(juce::Font(15.00f, juce::Font::plain));
    label_shape->setJustificationType(juce::Justification::centred);
    label_shape->setEditable(false, false, false);
    label_shape->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_shape->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_shape->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    close = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*close);
    close->setTooltip(TRANS("Close this pop up. \n"
                            "(ESC is your friend)"));
    close->setButtonText(TRANS("X"));
    close->addListener(this);
    close->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    close->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    close->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    close->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    keep = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*keep);
    keep->setTooltip(
        TRANS("OPTION: auto switch this pop up to its siblings on any mouse action at a sibling.\n"
              "(e.g. from one OSC input to another one of the same filter)"));
    keep->setButtonText(TRANS("aSW"));
    keep->addListener(this);
    keep->setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    keep->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    keep->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    keep->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    auto_close = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*auto_close);
    auto_close->setTooltip(TRANS("OPTION: auto close this popup on any unrelated mouse action.\n"
                                 "(e.g. click the main user interface)"));
    auto_close->setButtonText(TRANS("aCL"));
    auto_close->addListener(this);
    auto_close->setColour(juce::TextButton::buttonColourId, juce::Colours::yellow);
    auto_close->setColour(juce::TextButton::buttonOnColourId, juce::Colours::yellow);
    auto_close->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    auto_close->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    copy = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*copy);
    copy->setTooltip(TRANS("Copy this envelop settings to the clipboard."));
    copy->setButtonText(TRANS("COPY"));
    copy->addListener(this);
    copy->setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    copy->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    copy->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    copy->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    past = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*past);
    past->setTooltip(TRANS("Paste envelop settings from the clipboard."));
    past->setButtonText(TRANS("PASTE"));
    past->addListener(this);
    past->setColour(juce::TextButton::buttonColourId, juce::Colours::blueviolet);
    past->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    past->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    past->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    //[UserPreSize]
    if (synth_data->only_use_rotary_sliders)
    {
        slider_attack->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider_decay->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider_release->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider_sustain_time->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider_sustain->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    }
    else
    {
        slider_attack->setSliderStyle(juce::Slider::LinearVertical);
        slider_decay->setSliderStyle(juce::Slider::LinearVertical);
        slider_release->setSliderStyle(juce::Slider::LinearVertical);
        slider_sustain_time->setSliderStyle(juce::Slider::LinearVertical);
        slider_sustain->setSliderStyle(juce::Slider::LinearVertical);
    }

    related_to_comp = nullptr;
    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        juce::Component *child = getChildComponent(i);
        child->setWantsKeyboardFocus(false);
        child->setOpaque(true);
    }
    plotter->setRepaintsOnMouseActivity(false);
    this->setRepaintsOnMouseActivity(false);
    plotter->setOpaque(false);

    close->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    keep->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    auto_close->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    copy->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    past->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);

    slider_attack->setPopupMenuEnabled(true);
    slider_decay->setPopupMenuEnabled(true);
    slider_sustain->setPopupMenuEnabled(true);
    slider_sustain_time->setPopupMenuEnabled(true);
    slider_release->setPopupMenuEnabled(true);
    slider_env_shape->setPopupMenuEnabled(true);

    // setOpaque(true);
    /*
    //[/UserPreSize]

    setSize (710, 190);


    //[Constructor] You can add your own custom stuff here..
    */
    ui_refresher_->synth_data->midi_env_attack.set_value_without_notification(env_data_->attack);
    ui_refresher_->synth_data->midi_env_decay.set_value_without_notification(env_data_->decay);
    ui_refresher_->synth_data->midi_env_sustain.set_value_without_notification(env_data_->sustain);
    ui_refresher_->synth_data->midi_env_sustain_time.set_value_without_notification(
        env_data_->sustain_time);
    ui_refresher_->synth_data->midi_env_release.set_value_without_notification(env_data_->release);
    ui_refresher_->synth_data->midi_env_shape.set_value_without_notification(env_data_->release);
    ui_refresher_->synth_data->midi_env_attack.register_listener(this);
    ui_refresher_->synth_data->midi_env_decay.register_listener(this);
    ui_refresher_->synth_data->midi_env_sustain.register_listener(this);
    ui_refresher_->synth_data->midi_env_sustain_time.register_listener(this);
    ui_refresher_->synth_data->midi_env_release.register_listener(this);
    ui_refresher_->synth_data->midi_env_shape.register_listener(this);
    //[/Constructor]
}

Monique_Ui_ENVPopup::~Monique_Ui_ENVPopup()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    ui_refresher->synth_data->midi_env_attack.remove_listener(this);
    ui_refresher->synth_data->midi_env_decay.remove_listener(this);
    ui_refresher->synth_data->midi_env_sustain.remove_listener(this);
    ui_refresher->synth_data->midi_env_sustain_time.remove_listener(this);
    ui_refresher->synth_data->midi_env_release.remove_listener(this);
    ui_refresher->synth_data->midi_env_shape.remove_listener(this);

    parent->show_info_popup(nullptr, nullptr);

    for (int i = 0; i != observed_comps.size(); ++i)
    {
        juce::Component *comp = observed_comps.getUnchecked(i);
        comp->removeMouseListener(this);
    }
    //[/Destructor_pre]

    label_attack_bottom = nullptr;
    slider_attack = nullptr;
    label_decay_bottom = nullptr;
    slider_decay = nullptr;
    label_release_bottom = nullptr;
    slider_release = nullptr;
    slider_sustain_time = nullptr;
    label_sustain_time_bottom = nullptr;
    label_attack = nullptr;
    label_decay = nullptr;
    label_sustain_time = nullptr;
    label_release = nullptr;
    slider_sustain = nullptr;
    label_sustain_bottom = nullptr;
    label_sustain = nullptr;
    plotter = nullptr;
    slider_env_shape = nullptr;
    label_shape = nullptr;
    close = nullptr;
    keep = nullptr;
    auto_close = nullptr;
    copy = nullptr;
    past = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Monique_Ui_ENVPopup::paint(juce::Graphics &g)
{
    //[UserPrePaint] Add your own custom painting code here..
    is_repainting = true;

    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRect(getWidth() - 10, getHeight() - 10, 10, 10);

#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.setColour(colours.get_theme(theme).area_colour);
    g.fillRoundedRectangle(1.0f, 10.0f, 708.0f, 179.0f, 10.000f);

    juce::Colour outline_and_track = colours.get_theme(theme).value_slider_track_colour;
    g.setColour(outline_and_track);
    g.drawRoundedRectangle(1.0f, 10.0f, 708.0f, 179.0f, 10.000f, 1.000f);

    g.fillPath(internalPath1);

    /*
    //[/UserPrePaint]

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1.0f, 10.0f, 708.0f, 179.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 708.0f, 179.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    */
#include "mono_ui_includeHacks_END.h"
    {
        curve.clearQuick();
        curve.ensureStorageAllocated(50000);

        synth_data->get_full_adstr(*env_data, curve);

        int plotter_x = plotter->getX();
        int plotter_y = plotter->getY();
        int plotter_width = plotter->getWidth();
        int plotter_hight = plotter->getHeight();

        {
            juce::Colour colour = juce::Colour(0xff222222);
            g.setGradientFill(juce::ColourGradient(colour.darker(0.3f), 0.0f, 0.0f,
                                                   juce::Colour(0xff050505), 0.0f, plotter_hight,
                                                   false));
            // g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f,
            // height, false));
            g.fillRoundedRectangle(plotter_x, plotter_y, plotter_width, plotter_hight, 3);

            g.setColour(colour.darker(0.6f));
            g.drawRoundedRectangle(plotter_x, plotter_y, plotter_width, plotter_hight, 3, 1);
        }

        const int curve_size = curve.size();
        float scale_w = float(plotter_width) / (curve.size());

        int last_x = -1;
        int last_y = -1;
        for (int i = 0; i != curve_size; ++i)
        {
            float value = 1.0f - curve[i];
            const int x = scale_w * i + plotter_x;
            const int y = value * plotter_hight + plotter_y;
            if (last_x != x || last_y != y)
            {
                last_y = y;
                g.setColour(outline_and_track);
                g.fillRect(last_x, last_y, 1, 1);

                if (last_x != x)
                {
                    g.setColour(outline_and_track.withAlpha(0.1f));
                    g.fillRect(last_x, last_y, 1, int((1.0f - value) * plotter_hight));
                }
                last_x = x;
            }
        }
    }

    is_repainting = false;
    //[/UserPaint]
}

void Monique_Ui_ENVPopup::resized()
{
    //[UserPreResize] Add your own custom resize code here..
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    const int left_move = !left ? original_w - 50 - 30 : 0;

    label_attack_bottom->setBounds(20, 140, 60, 33);
    slider_attack->setBounds(20, 60, 60, 80);
    label_decay_bottom->setBounds(80, 140, 60, 33);
    slider_decay->setBounds(80, 60, 60, 80);
    label_release_bottom->setBounds(260, 140, 60, 33);
    slider_release->setBounds(260, 60, 60, 80);
    slider_sustain_time->setBounds(200, 60, 60, 80);
    label_sustain_time_bottom->setBounds(200, 140, 60, 33);
    label_attack->setBounds(20, 20, 60, 33);
    label_decay->setBounds(80, 20, 60, 33);
    label_sustain_time->setBounds(200, 20, 60, 33);
    label_release->setBounds(260, 20, 60, 33);
    slider_sustain->setBounds(140, 60, 60, 80);
    label_sustain_bottom->setBounds(140, 140, 60, 33);
    label_sustain->setBounds(140, 20, 60, 33);
    plotter->setBounds(410, 20, 240, 159);
    slider_env_shape->setBounds(340, 60, 60, 80);
    label_shape->setBounds(340, 140, 60, 33);
    close->setBounds(660, 20, 40, 20);
    keep->setBounds(660, 70, 40, 20);
    auto_close->setBounds(660, 50, 40, 20);
    copy->setBounds(660, 135, 40, 20);
    past->setBounds(660, 155, 40, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath(40.0f + left_move, 0.0f);
    internalPath1.lineTo(50.0f + left_move, 10.0f);
    internalPath1.lineTo(30.0f + left_move, 10.0f);
    internalPath1.closeSubPath();
    /*
    //[/UserPreResize]

    label_attack_bottom->setBounds (20, 140, 60, 30);
    slider_attack->setBounds (20, 60, 60, 80);
    label_decay_bottom->setBounds (80, 140, 60, 30);
    slider_decay->setBounds (80, 60, 60, 80);
    label_release_bottom->setBounds (260, 140, 60, 30);
    slider_release->setBounds (260, 60, 60, 80);
    slider_sustain_time->setBounds (200, 60, 60, 80);
    label_sustain_time_bottom->setBounds (200, 140, 60, 30);
    label_attack->setBounds (20, 20, 60, 30);
    label_decay->setBounds (80, 20, 60, 30);
    label_sustain_time->setBounds (200, 20, 60, 30);
    label_release->setBounds (260, 20, 60, 30);
    slider_sustain->setBounds (140, 60, 60, 80);
    label_sustain_bottom->setBounds (140, 140, 60, 30);
    label_sustain->setBounds (140, 20, 60, 30);
    plotter->setBounds (410, 20, 240, 159);
    slider_env_shape->setBounds (340, 60, 60, 80);
    label_shape->setBounds (340, 140, 60, 30);
    close->setBounds (660, 20, 40, 20);
    keep->setBounds (660, 70, 40, 20);
    auto_close->setBounds (660, 50, 40, 20);
    copy->setBounds (660, 135, 40, 20);
    past->setBounds (660, 155, 40, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath (40.0f, 0.0f);
    internalPath1.lineTo (50.0f, 10.0f);
    internalPath1.lineTo (30.0f, 10.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
      */
#include "mono_ui_includeHacks_END.h"
    //[/UserResized]
}

void Monique_Ui_ENVPopup::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider_attack.get())
    {
        //[UserSliderCode_slider_attack] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_env_attack,
                                                    sliderThatWasMoved)
        else
        {
            env_data->attack = sliderThatWasMoved->getValue();
            ui_refresher->synth_data->midi_env_attack.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_attack.midi_control);
        //[/UserSliderCode_slider_attack]
    }
    else if (sliderThatWasMoved == slider_decay.get())
    {
        //[UserSliderCode_slider_decay] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_env_decay,
                                                    sliderThatWasMoved)
        else
        {
            env_data->decay = sliderThatWasMoved->getValue();
            ui_refresher->synth_data->midi_env_decay.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_decay.midi_control);
        //[/UserSliderCode_slider_decay]
    }
    else if (sliderThatWasMoved == slider_release.get())
    {
        //[UserSliderCode_slider_release] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_env_release,
                                                    sliderThatWasMoved)
        else
        {
            env_data->release = sliderThatWasMoved->getValue();
            ui_refresher->synth_data->midi_env_release.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_release.midi_control);
        //[/UserSliderCode_slider_release]
    }
    else if (sliderThatWasMoved == slider_sustain_time.get())
    {
        //[UserSliderCode_slider_sustain_time] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(
            &ui_refresher->synth_data->midi_env_sustain_time, sliderThatWasMoved)
        else
        {
            env_data->sustain_time = sliderThatWasMoved->getValue();
            ui_refresher->synth_data->midi_env_sustain_time.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_sustain_time.midi_control);
        //[/UserSliderCode_slider_sustain_time]
    }
    else if (sliderThatWasMoved == slider_sustain.get())
    {
        //[UserSliderCode_slider_sustain] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_env_sustain,
                                                    sliderThatWasMoved)
        else
        {
            sustain->set_value(sliderThatWasMoved->getValue());
            ui_refresher->synth_data->midi_env_sustain.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_sustain.midi_control);
        //[/UserSliderCode_slider_sustain]
    }
    else if (sliderThatWasMoved == slider_env_shape.get())
    {
        //[UserSliderCode_slider_env_shape] -- add your slider handling code here..
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_env_shape,
                                                    sliderThatWasMoved)
        else
        {
            env_data->shape = sliderThatWasMoved->getValue();
            ui_refresher->synth_data->midi_env_shape.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_env_shape.midi_control);
        //[/UserSliderCode_slider_env_shape]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Monique_Ui_ENVPopup::labelTextChanged(juce::Label *labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == label_attack.get())
    {
        //[UserLabelCode_label_attack] -- add your label text handling code here..
        float value_percent = labelThatHasChanged->getText().getFloatValue();
        slider_attack->setValue(reverse_ms_to_slider_value(value_percent), juce::sendNotification);
        //[/UserLabelCode_label_attack]
    }
    else if (labelThatHasChanged == label_decay.get())
    {
        //[UserLabelCode_label_decay] -- add your label text handling code here..
        float value_percent = labelThatHasChanged->getText().getFloatValue();
        slider_decay->setValue(reverse_ms_to_slider_value(value_percent), juce::sendNotification);
        //[/UserLabelCode_label_decay]
    }
    else if (labelThatHasChanged == label_sustain_time.get())
    {
        //[UserLabelCode_label_sustain_time] -- add your label text handling code here..
        float value_percent = labelThatHasChanged->getText().getFloatValue();
        slider_sustain_time->setValue(reverse_ms_to_slider_value(value_percent),
                                      juce::sendNotification);
        //[/UserLabelCode_label_sustain_time]
    }
    else if (labelThatHasChanged == label_release.get())
    {
        //[UserLabelCode_label_release] -- add your label text handling code here..
        float value_percent = labelThatHasChanged->getText().getFloatValue();
        slider_release->setValue(reverse_ms_to_slider_value(value_percent), juce::sendNotification);
        //[/UserLabelCode_label_release]
    }
    else if (labelThatHasChanged == label_sustain.get())
    {
        //[UserLabelCode_label_sustain] -- add your label text handling code here..
        slider_sustain->setValue((labelThatHasChanged->getText().getFloatValue() * 10) /
                                     env_data->sustain.get_info().num_steps,
                                 juce::sendNotification);
        //[/UserLabelCode_label_sustain]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}

void Monique_Ui_ENVPopup::buttonClicked(juce::Button *buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == close.get())
    {
        //[UserButtonCode_close] -- add your button handler code here..
        if (Monique_Ui_Mainwindow *mainwindow = get_editor())
        {
            mainwindow->open_env_popup(nullptr, nullptr, nullptr, nullptr, false);
        }
        //[/UserButtonCode_close]
    }
    else if (buttonThatWasClicked == keep.get())
    {
        //[UserButtonCode_keep] -- add your button handler code here..
        synth_data->auto_switch_env_popup ^= true;
        //[/UserButtonCode_keep]
    }
    else if (buttonThatWasClicked == auto_close.get())
    {
        //[UserButtonCode_auto_close] -- add your button handler code here..
        synth_data->auto_close_env_popup ^= true;
        //[/UserButtonCode_auto_close]
    }
    else if (buttonThatWasClicked == copy.get())
    {
        //[UserButtonCode_copy] -- add your button handler code here..
        ::copy(*get_shared_ENV_clipboard(), *env_data);
        has_ENV_clipboard_data()->value = true;
        //[/UserButtonCode_copy]
    }
    else if (buttonThatWasClicked == past.get())
    {
        //[UserButtonCode_past] -- add your button handler code here..
        jassert(
            has_ENV_clipboard_data()->value); // button should be disabled if there is no content

        ::copy(*env_data, *get_shared_ENV_clipboard());
        //[/UserButtonCode_past]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Monique_Ui_ENVPopup" componentName=""
                 parentClasses="public Component, public Monique_Ui_Refreshable, public DropShadower, public Timer, public ParameterListener"
                 constructorParams="Monique_Ui_Refresher*ui_refresher_, Monique_Ui_Mainwindow*const parent_, ENVData*const env_data_, Parameter*const sustain_, bool left_, bool has_negative_sustain_"
                 variableInitialisers="Monique_Ui_Refreshable(ui_refresher_),&#10;DropShadower(DropShadow(Colours::black.withAlpha(0.8f),10,Point&lt;int&gt;(10,10))),&#10;parent(parent_),&#10;env_data(env_data_),&#10;sustain(sustain_),&#10;original_w(710), original_h(190),&#10;left(left_)"
                 snapPixels="10" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="710" initialHeight="190">
  <BACKGROUND backgroundColour="ffffff">
    <ROUNDRECT pos="1 10 708 179" cornerSize="10" fill="solid: ff050505" hasStroke="1"
               stroke="1, mitered, butt" strokeColour="solid: ffff0000"/>
    <PATH pos="0 0 100 100" fill="solid: ffff0000" hasStroke="0" nonZeroWinding="1">s 40 0 l 50 10 l 30 10 x</PATH>
  </BACKGROUND>
  <LABEL name="" id="c4d4f0ae59fb458b" memberName="label_attack_bottom"
         virtualName="" explicitFocusOrder="0" pos="20 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="ATT" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="65a4c85262fddcd2" memberName="slider_attack" virtualName="Slider"
          explicitFocusOrder="0" pos="20 60 60 80" tooltip="Define the curve attack in ms."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.00020000000000000000958"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="5269c763f2d5a37b" memberName="label_decay_bottom"
         virtualName="" explicitFocusOrder="0" pos="80 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="DEC" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b62502f225e4fe3a" memberName="slider_decay" virtualName="Slider"
          explicitFocusOrder="0" pos="80 60 60 80" tooltip="Define the curve decay in ms."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.00020000000000000000958"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="d001c80859e5b7cb" memberName="label_release_bottom"
         virtualName="" explicitFocusOrder="0" pos="260 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="REL" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="e8c91a0aabc505e" memberName="slider_release" virtualName="Slider"
          explicitFocusOrder="0" pos="260 60 60 80" tooltip="Define the curve release in ms."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.00020000000000000000958"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="0" id="76a391a494643c63" memberName="slider_sustain_time"
          virtualName="Slider" explicitFocusOrder="0" pos="200 60 60 80"
          tooltip="Define the sustain time in ms." rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="0" max="1" int="0.00020000000000000000958" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ffcf23120599c6e5" memberName="label_sustain_time_bottom"
         virtualName="" explicitFocusOrder="0" pos="200 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="RETRIG" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="a09cec04c5ae6b58" memberName="label_attack" virtualName=""
         explicitFocusOrder="0" pos="20 20 60 30" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="1" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="1b295ca55294b0a2" memberName="label_decay" virtualName=""
         explicitFocusOrder="0" pos="80 20 60 30" textCol="ffffff00" edTextCol="ff000000"
         edBkgCol="0" labelText="x&#10;" editableSingleClick="1" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="e1944df446a5aea6" memberName="label_sustain_time"
         virtualName="" explicitFocusOrder="0" pos="200 20 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="72cc727ebebbec15" memberName="label_release" virtualName=""
         explicitFocusOrder="0" pos="260 20 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="0" id="b7e5d3f5d3dbf47a" memberName="slider_sustain" virtualName="Slider"
          explicitFocusOrder="0" pos="140 60 60 80" tooltip="Define the curve sustain in ms."
          rotarysliderfill="ffffff00" rotaryslideroutline="ff161616" textboxtext="ffffff00"
          textboxbkgd="ff161616" min="0" max="1" int="0.0010000000000000000208"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="" id="ee00adc332943fc6" memberName="label_sustain_bottom"
         virtualName="" explicitFocusOrder="0" pos="140 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SUS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="VL" id="8b7051eff652e1d6" memberName="label_sustain" virtualName=""
         explicitFocusOrder="0" pos="140 20 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="x&#10;" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="" id="c88b6f548bad2da7" memberName="plotter" virtualName=""
                    explicitFocusOrder="0" pos="410 20 240 159" class="Component"
                    params=""/>
  <SLIDER name="0" id="e7a1c7c979888f2f" memberName="slider_env_shape"
          virtualName="Slider" explicitFocusOrder="0" pos="340 60 60 80"
          tooltip="Define the curve shape type." rotarysliderfill="ffffff00"
          rotaryslideroutline="ff161616" textboxtext="ffffff00" textboxbkgd="ff161616"
          min="-1" max="1" int="0.010000000000000000208" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ad65d35c7b51c7ea" memberName="label_shape"
         virtualName="" explicitFocusOrder="0" pos="340 140 60 30" textCol="ffffff00"
         edTextCol="ff000000" edBkgCol="0" labelText="SLOPE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="337cd4804743bec8" memberName="close" virtualName=""
              explicitFocusOrder="0" pos="660 20 40 20" tooltip="Close this pop up. &#10;(ESC is your friend)"
              bgColOff="ffff0000" bgColOn="ffff0000" textCol="ff000000" textColOn="ff000000"
              buttonText="X" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="acbee438c46f4a5d" memberName="keep" virtualName=""
              explicitFocusOrder="0" pos="660 70 40 20" tooltip="OPTION: auto switch this pop up to its siblings on any mouse action at a sibling.&#10;(e.g. from one OSC input to another one of the same filter)"
              bgColOff="ff008000" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="aSW" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="28bf323aea1c44dd" memberName="auto_close" virtualName=""
              explicitFocusOrder="0" pos="660 50 40 20" tooltip="OPTION: auto close this popup on any unrelated mouse action.&#10;(e.g. click the main user interface)"
              bgColOff="ffffff00" bgColOn="ffffff00" textCol="ff000000" textColOn="ff000000"
              buttonText="aCL" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b19da151b3279272" memberName="copy" virtualName=""
              explicitFocusOrder="0" pos="660 135 40 20" tooltip="Copy this envelop settings to the clipboard."
              bgColOff="ff6495ed" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="COPY" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="b0118ea77c7b965a" memberName="past" virtualName=""
              explicitFocusOrder="0" pos="660 155 40 20" tooltip="Paste envelop settings from the clipboard."
              bgColOff="ff8a2be2" bgColOn="ff008000" textCol="ff000000" textColOn="ff000000"
              buttonText="PASTE" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
