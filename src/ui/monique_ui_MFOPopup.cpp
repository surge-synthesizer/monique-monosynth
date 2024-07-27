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

#include "monique_ui_MFOPopup.h"

#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "monique_ui_ModulationSlider.h"

#include "core/monique_core_Processor.h"

void Monique_Ui_MFOPopup::timerCallback()
{
    if (is_repainting)
        return;

    if (++callbacks > 9)
        stopTimer();

    juce::MessageManagerLock mmLock;
    repaint(plotter->getBounds().getX() - 1, plotter->getBounds().getY() - 1,
            plotter->getBounds().getWidth() + 2, plotter->getBounds().getHeight() + 2);
}
void Monique_Ui_MFOPopup::refresh() noexcept
{
    if (last_wave != mfo_data->wave.get_value() || last_speed != mfo_data->speed.get_value() ||
        last_offset != mfo_data->phase_shift.get_value())
    {
        last_wave = mfo_data->wave.get_value();
        last_speed = mfo_data->speed.get_value();
        last_offset = mfo_data->phase_shift.get_value();

        juce::String speed_label(get_lfo_speed_multi_as_text(
            last_speed, synth_data->runtime_info, synth_data->runtime_notifyer->get_sample_rate()));
        if (slider_speed->getProperties().set(
                VAR_INDEX_VALUE_TO_SHOW, speed_label + juce::String("@") +
                                             juce::String(is_integer(last_speed) ? "th" : "Hz")))
        {
            slider_speed->repaint();
        }
        if (slider_wave->getProperties().set(VAR_INDEX_VALUE_TO_SHOW,
                                             juce::String(auto_round(mfo_data->wave.get_value()))))
        {
            slider_wave->repaint();
        }
        if (slider_offset->getProperties().set(
                VAR_INDEX_VALUE_TO_SHOW,
                juce::String(auto_round(mfo_data->phase_shift.get_value()))))
        {
            slider_offset->repaint();
        }

        slider_wave->setValue(last_wave, juce::dontSendNotification);
        slider_speed->setValue(last_speed, juce::dontSendNotification);
        slider_offset->setValue(last_offset, juce::dontSendNotification);

        stopTimer();
        callbacks = 0;
        startTimer(synth_data->glide_motor_time.get_value() / 10 + 5);
    }
    {
        SectionTheme &theme = look_and_feel->colours.get_theme(COLOUR_THEMES::POPUP_THEME);
        juce::Colour button_off = theme.button_off_colour;
        auto_close->setColour(juce::TextButton::buttonColourId, synth_data->auto_close_env_popup
                                                                    ? juce::Colours::yellow
                                                                    : button_off);
        keep->setColour(juce::TextButton::buttonColourId,
                        synth_data->auto_switch_env_popup ? juce::Colours::green : button_off);
    }

    past->setEnabled(has_LFO_clipboard_data()->value);
}

void Monique_Ui_MFOPopup::set_element_to_show(juce::Component *const comp_,
                                              Monique_Ui_DualSlider *owner_)
{
    owner_slider = owner_;
    related_to_comp = comp_;

    int x = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getX();
    int y = get_editor()->getLocalPoint(comp_, juce::Point<int>(0, 0)).getY();

    const float width_scale = 1.0f / original_w * getWidth();
    const int left_move = (width_scale * 10);
    setTopLeftPosition(x - left_move, y + comp_->getHeight());
}
void Monique_Ui_MFOPopup::update_positions()
{
    if (related_to_comp)
    {
        set_element_to_show(related_to_comp, owner_slider);
    }
}
void Monique_Ui_MFOPopup::set_clickable_components(juce::Array<juce::Component *> &comps_) noexcept
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
void Monique_Ui_MFOPopup::mouseDown(const juce::MouseEvent &event)
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
                    const bool keeps_open = mainwindow->handle_keep_mfo_open(dual_slider->_config);
                    success = true;
                    if (keeps_open && synth_data->auto_switch_env_popup)
                    {
                        if (owner_slider != dual_slider && owner_slider)
                        {
                            mainwindow->open_mfo_popup(dual_slider);
                            return;
                        }
                    }

                    break;
                }
                comp = comp->getParentComponent();
            }
            if (!success && synth_data->auto_close_env_popup)
            {
                mainwindow->open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
            }
        }
    }
}
void Monique_Ui_MFOPopup::mouseDrag(const juce::MouseEvent &event) { mouseDown(event); }
void Monique_Ui_MFOPopup::mouseUp(const juce::MouseEvent &event)
{
    // mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseDoubleClick(const juce::MouseEvent &event) { mouseDown(event); }
void Monique_Ui_MFOPopup::mouseWheelMove(const juce::MouseEvent &event,
                                         const juce::MouseWheelDetails &)
{
    mouseDown(event);
}
void Monique_Ui_MFOPopup::mouseMagnify(const juce::MouseEvent &event, float) { mouseDown(event); }
void Monique_Ui_MFOPopup::parameter_value_changed(Parameter *param_) noexcept
{
    if (param_ == &ui_refresher->synth_data->midi_lfo_wave)
    {
        mfo_data->wave = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_lfo_speed)
    {
        mfo_data->speed = param_->get_value();
    }
    else if (param_ == &ui_refresher->synth_data->midi_lfo_offset)
    {
        mfo_data->phase_shift = param_->get_value();
    }
}
void Monique_Ui_MFOPopup::sliderClicked(juce::Slider *s_)
{
    if (IS_MIDI_LEARN)
    {
        sliderValueChanged(s_);
    }
}

//==============================================================================
Monique_Ui_MFOPopup::Monique_Ui_MFOPopup(Monique_Ui_Refresher *ui_refresher_,
                                         Monique_Ui_Mainwindow *const parent_,
                                         LFOData *const mfo_data_, COLOUR_THEMES theme_)
    : Monique_Ui_Refreshable(ui_refresher_),
      juce::DropShadower(
          juce::DropShadow(juce::Colours::black.withAlpha(0.8f), 10, juce::Point<int>(10, 10))),
      original_w(540), original_h(190), parent(parent_), mfo_data(mfo_data_)
{
    ui_refresher_->synth_data->midi_lfo_wave.set_value_without_notification(mfo_data_->wave);
    ui_refresher_->synth_data->midi_lfo_speed.set_value_without_notification(
        mfo_data_->speed.get_value());
    ui_refresher_->synth_data->midi_lfo_offset.set_value_without_notification(
        mfo_data_->phase_shift);

    last_wave = 999;
    last_speed = 999;
    last_offset = 999;
    owner_slider = nullptr;
    setOwner(this);
    theme = theme_;
    is_repainting = false;

    slider_wave = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_wave);
    slider_wave->setTooltip(TRANS("Define the wave.\n"
                                  "\"(Sine (LEFT), close to Square (RIGHT))\""));
    slider_wave->setRange(0, 1, 0.01);
    slider_wave->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_wave->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_wave->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_wave->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_wave->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_wave->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_wave->addListener(this);

    label_shape2 = std::make_unique<juce::Label>(juce::String(), TRANS("WAVE"));
    addAndMakeVisible(*label_shape2);
    label_shape2->setFont(juce::Font(15.00f, juce::Font::plain));
    label_shape2->setJustificationType(juce::Justification::centred);
    label_shape2->setEditable(false, false, false);
    label_shape2->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_shape2->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_shape2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label_shape = std::make_unique<juce::Label>(juce::String(), TRANS("SPEED"));
    addAndMakeVisible(*label_shape);
    label_shape->setFont(juce::Font(15.00f, juce::Font::plain));
    label_shape->setJustificationType(juce::Justification::centred);
    label_shape->setEditable(false, false, false);
    label_shape->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_shape->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_shape->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_speed = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_speed);
    slider_speed->setTooltip(TRANS("Define the oscillator speed."));
    slider_speed->setRange(0, 16, 1);
    slider_speed->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_speed->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_speed->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_speed->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_speed->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_speed->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_speed->addListener(this);

    plotter = std::make_unique<juce::Component>();
    addAndMakeVisible(*plotter);

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
    copy->setTooltip(TRANS("Copy this settings to the clipboard."));
    copy->setButtonText(TRANS("COPY"));
    copy->addListener(this);
    copy->setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    copy->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    copy->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    copy->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    past = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*past);
    past->setTooltip(TRANS("Paste settings from the clipboard."));
    past->setButtonText(TRANS("PASTE"));
    past->addListener(this);
    past->setColour(juce::TextButton::buttonColourId, juce::Colours::blueviolet);
    past->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    past->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    past->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    label_shape3 = std::make_unique<juce::Label>(juce::String(), TRANS("PHASE"));
    addAndMakeVisible(*label_shape3);
    label_shape3->setFont(juce::Font(15.00f, juce::Font::plain));
    label_shape3->setJustificationType(juce::Justification::centred);
    label_shape3->setEditable(false, false, false);
    label_shape3->setColour(juce::Label::textColourId, juce::Colours::yellow);
    label_shape3->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label_shape3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    slider_offset = std::make_unique<juce::Slider>("0");
    addAndMakeVisible(*slider_offset);
    slider_offset->setTooltip(TRANS("Define the phase."));
    slider_offset->setRange(0, 1, 0.01);
    slider_offset->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider_offset->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    slider_offset->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    slider_offset->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    slider_offset->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    slider_offset->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff161616));
    slider_offset->addListener(this);

    mfo_minus = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*mfo_minus);
    mfo_minus->setTooltip(TRANS("Decrease the speed in steps."));
    mfo_minus->setButtonText(TRANS("-"));
    mfo_minus->addListener(this);
    mfo_minus->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    mfo_minus->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    mfo_minus->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    mfo_plus = std::make_unique<juce::TextButton>(juce::String());
    addAndMakeVisible(*mfo_plus);
    mfo_plus->setTooltip(TRANS("Increase the speed in steps."));
    mfo_plus->setButtonText(TRANS("+"));
    mfo_plus->addListener(this);
    mfo_plus->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    mfo_plus->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffff3b00));
    mfo_plus->setColour(juce::TextButton::textColourOffId, juce::Colours::yellow);

    related_to_comp = nullptr;
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        getChildComponent(i)->setWantsKeyboardFocus(false);
        juce::Component *child = getChildComponent(i);
        child->setOpaque(true);
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, theme_);
    }
    // this->setRepaintsOnMouseActivity(false);
    plotter->setRepaintsOnMouseActivity(false);
    plotter->setOpaque(false);

    slider_speed->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                      SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_offset->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                       SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_wave->getProperties().set(VAR_INDEX_SLIDER_LABEL_STYLE,
                                     SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX);
    slider_speed->setPopupMenuEnabled(true);
    slider_offset->setPopupMenuEnabled(true);
    slider_wave->setPopupMenuEnabled(true);

    close->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    keep->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    auto_close->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    copy->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);
    past->getProperties().set(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, true);

    /*

    setSize (540, 190);

    */
    ui_refresher_->synth_data->midi_lfo_wave.register_listener(this);
    ui_refresher_->synth_data->midi_lfo_speed.register_listener(this);
    ui_refresher_->synth_data->midi_lfo_offset.register_listener(this);
}

Monique_Ui_MFOPopup::~Monique_Ui_MFOPopup()
{
    ui_refresher->synth_data->midi_lfo_wave.remove_listener(this);
    ui_refresher->synth_data->midi_lfo_speed.remove_listener(this);
    ui_refresher->synth_data->midi_lfo_offset.remove_listener(this);

    parent->show_info_popup(nullptr, nullptr);

    for (int i = 0; i != observed_comps.size(); ++i)
    {
        juce::Component *comp = observed_comps.getUnchecked(i);
        comp->removeMouseListener(this);
    }

    slider_wave = nullptr;
    label_shape2 = nullptr;
    label_shape = nullptr;
    slider_speed = nullptr;
    plotter = nullptr;
    close = nullptr;
    keep = nullptr;
    auto_close = nullptr;
    copy = nullptr;
    past = nullptr;
    label_shape3 = nullptr;
    slider_offset = nullptr;
    mfo_minus = nullptr;
    mfo_plus = nullptr;
}

//==============================================================================
void Monique_Ui_MFOPopup::paint(juce::Graphics &g)
{
    is_repainting = true;

    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRect(getWidth() - 10, getHeight() - 10, 10, 10);

#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    g.setColour(colours.get_theme(theme).area_colour);
    g.fillRoundedRectangle(1.0f, 10.0f, 538.0f, 179.0f, 10.000f);

    juce::Colour outline_and_track = colours.get_theme(theme).value_slider_track_colour;
    g.setColour(outline_and_track);
    g.drawRoundedRectangle(1.0f, 10.0f, 538.0f, 179.0f, 10.000f, 1.000f);

    g.fillPath(internalPath1);

    /*

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f);

    g.setColour (Colours::red);
    g.drawRoundedRectangle (1.0f, 10.0f, 538.0f, 179.0f, 10.000f, 1.000f);

    g.setColour (Colours::red);
    g.fillPath (internalPath1);

    */
#include "mono_ui_includeHacks_END.h"
    {
        curve.clearQuick();
        curve.ensureStorageAllocated(50000);

        synth_data->get_full_mfo(*mfo_data, curve, synth_data);

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
}

void Monique_Ui_MFOPopup::resized()
{
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    slider_wave->setBounds(20, 70, 60, 60);
    label_shape2->setBounds(20, 140, 60, 30);
    label_shape->setBounds(90, 140, 60, 30);
    slider_speed->setBounds(90, 70, 60, 60);
    plotter->setBounds(240, 20, 240, 159);
    close->setBounds(490, 20, 40, 20);
    keep->setBounds(490, 70, 40, 20);
    auto_close->setBounds(490, 50, 40, 20);
    copy->setBounds(490, 135, 40, 20);
    past->setBounds(490, 155, 40, 20);
    label_shape3->setBounds(160, 140, 60, 30);
    slider_offset->setBounds(160, 70, 60, 60);
    mfo_minus->setBounds(90, 20, 30, 30);
    mfo_plus->setBounds(120, 20, 30, 30);
    internalPath1.clear();
    internalPath1.startNewSubPath(40.0f, 0.0f);
    internalPath1.lineTo(50.0f, 10.0f);
    internalPath1.lineTo(30.0f, 10.0f);
    internalPath1.closeSubPath();

#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_MFOPopup::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{

    if (sliderThatWasMoved == slider_wave.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_lfo_wave,
                                                    sliderThatWasMoved)
        else
        {
            mfo_data->wave.set_value(sliderThatWasMoved->getValue());
            ui_refresher->synth_data->midi_lfo_wave.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_lfo_wave.midi_control);
    }
    else if (sliderThatWasMoved == slider_speed.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_lfo_speed,
                                                    sliderThatWasMoved)
        else
        {
            mfo_data->speed.set_value(sliderThatWasMoved->getValue());
            ui_refresher->synth_data->midi_lfo_speed.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_lfo_speed.midi_control);
    }
    else if (sliderThatWasMoved == slider_offset.get())
    {
        IF_MIDI_LEARN__HANDLE__AND_UPDATE_COMPONENT(&ui_refresher->synth_data->midi_lfo_offset,
                                                    sliderThatWasMoved)
        else
        {
            mfo_data->phase_shift.set_value(sliderThatWasMoved->getValue());
            ui_refresher->synth_data->midi_lfo_offset.set_value_without_notification(
                sliderThatWasMoved->getValue());
        }
        parent->show_info_popup(sliderThatWasMoved,
                                ui_refresher->synth_data->midi_lfo_offset.midi_control);
    }
}

void Monique_Ui_MFOPopup::buttonClicked(juce::Button *buttonThatWasClicked)
{

    if (buttonThatWasClicked == close.get())
    {
        if (Monique_Ui_Mainwindow *mainwindow = get_editor())
        {
            mainwindow->open_mfo_popup(nullptr, nullptr, nullptr, COLOUR_THEMES::DUMMY_THEME);
        }
    }
    else if (buttonThatWasClicked == keep.get())
    {
        synth_data->auto_switch_env_popup ^= true;
    }
    else if (buttonThatWasClicked == auto_close.get())
    {
        synth_data->auto_close_env_popup ^= true;
    }
    else if (buttonThatWasClicked == copy.get())
    {
        ::copy(*get_shared_LFO_clipboard(), *mfo_data);
        has_LFO_clipboard_data()->value = true;
    }
    else if (buttonThatWasClicked == past.get())
    {
        jassert(
            has_LFO_clipboard_data()->value); // button should be disabled if there is no content

        ::copy(*mfo_data, *get_shared_LFO_clipboard());
    }
    else if (buttonThatWasClicked == mfo_minus.get())
    {
        mfo_data->speed.set_value(int(mfo_data->speed.get_value() - 1));
    }
    else if (buttonThatWasClicked == mfo_plus.get())
    {
        mfo_data->speed.set_value(int(mfo_data->speed.get_value() + 1));
    }
}
