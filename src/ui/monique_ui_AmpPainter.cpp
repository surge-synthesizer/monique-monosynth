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

#include "monique_ui_AmpPainter.h"

#include "monique_ui_LookAndFeel.h"
#include "core/monique_core_Datastructures.h"

#include <memory>

//==============================================================================
Monique_Ui_AmpPainter::Monique_Ui_AmpPainter(MoniqueSynthData *synth_data_,
                                             UiLookAndFeel *look_and_feel_)
    : original_w(1465), original_h(180), synth_data(synth_data_), look_and_feel(look_and_feel_)
{
    is_currently_painting = false;
    current_buffer_start_pos = 0;

    eq_values = std::make_unique<EndlessBuffer>(synth_data_->runtime_notifyer);
    values_env = std::make_unique<EndlessBuffer>(synth_data_->runtime_notifyer);
    values = std::make_unique<EndlessBuffer>(synth_data_->runtime_notifyer);
    master_osc_values = std::make_unique<EndlessSwitchBuffer>(synth_data_->runtime_notifyer);

    drawing_area = std::make_unique<juce::Component>();
    addAndMakeVisible(*drawing_area);

    sl_show_range = std::make_unique<juce::Slider>(juce::String());
    addAndMakeVisible(*sl_show_range);
    sl_show_range->setTooltip(TRANS("Define the drawn time (max = 1 second)."));
    sl_show_range->setRange(0.001, 1, 0.001);
    sl_show_range->setSliderStyle(juce::Slider::LinearHorizontal);
    sl_show_range->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    sl_show_range->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    sl_show_range->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff161616));
    sl_show_range->setColour(juce::Slider::textBoxTextColourId, juce::Colours::yellow);
    sl_show_range->addListener(this);

    osc_1 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*osc_1);
    osc_1->setTooltip(TRANS("Turns visualisation for OSC 1 on or off."));
    osc_1->setButtonText(TRANS("OSC1"));
    osc_1->addListener(this);

    osc_2 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*osc_2);
    osc_2->setTooltip(TRANS("Turns visualisation for OSC 2 on or off."));
    osc_2->setButtonText(TRANS("OSC2"));
    osc_2->addListener(this);

    osc_3 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*osc_3);
    osc_3->setTooltip(TRANS("Turns visualisation for OSC 3 on or off."));
    osc_3->setButtonText(TRANS("OSC3"));
    osc_3->addListener(this);

    eq = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*eq);
    eq->setTooltip(TRANS("Turns visualisation for the EQ BANK output on or off."));
    eq->setButtonText(TRANS("EQ OUT"));
    eq->addListener(this);

    out = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*out);
    out->setTooltip(TRANS("Turns visualisation for the MAIN output on or off."));
    out->setButtonText(TRANS("MAIN OUT"));
    out->addListener(this);

    f_1 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_1);
    f_1->setTooltip(TRANS("Turns visualisation for FILTER 1 OUTPUT on or off."));
    f_1->setButtonText(TRANS("F1 OUT"));
    f_1->addListener(this);

    f_2 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_2);
    f_2->setTooltip(TRANS("Turns visualisation for FILTER 2 OUTPUT on or off."));
    f_2->setButtonText(TRANS("F2 OUT"));
    f_2->addListener(this);

    f_3 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_3);
    f_3->setTooltip(TRANS("Turns visualisation for FILTER 3 OUTPUT on or off."));
    f_3->setButtonText(TRANS("F3 OUT"));
    f_3->addListener(this);

    f_env_1 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_env_1);
    f_env_1->setTooltip(TRANS("Turns visualisation for the FILTER 1 MOD MIX on or off."));
    f_env_1->setButtonText(TRANS("F1 X-MOD"));
    f_env_1->addListener(this);

    f_env_2 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_env_2);
    f_env_2->setTooltip(TRANS("Turns visualisation for the FILTER 2 MOD MIX on or off."));
    f_env_2->setButtonText(TRANS("F2 X-MOD"));
    f_env_2->addListener(this);

    f_env_3 = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*f_env_3);
    f_env_3->setTooltip(TRANS("Turns visualisation for the FILTER 3 MOD MIX on or off."));
    f_env_3->setButtonText(TRANS("F3 X-MOD"));
    f_env_3->addListener(this);

    out_env = std::make_unique<juce::TextButton>("new button");
    addAndMakeVisible(*out_env);
    out_env->setTooltip(TRANS("Turns visualisation for the AMP envelope on or off."));
    out_env->setButtonText(TRANS("AMP ENV"));
    out_env->addListener(this);

    osc_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    osc_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_env_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_env_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));
    filter_env_values.add(new EndlessBuffer(synth_data_->runtime_notifyer));

    buffers.add(master_osc_values.get());
    buffers.add(osc_values[0]);
    buffers.add(osc_values[1]);
    buffers.add(filter_values[0]);
    buffers.add(filter_values[1]);
    buffers.add(filter_values[2]);
    buffers.add(filter_env_values[0]);
    buffers.add(filter_env_values[1]);
    buffers.add(filter_env_values[2]);
    buffers.add(values.get());
    buffers.add(values_env.get());
    buffers.add(eq_values.get());

    for (int i = 0; i != getNumChildComponents(); ++i)
    {
        juce::Component *child = getChildComponent(i);
        if (juce::Button *button = dynamic_cast<juce::Button *>(child))
        {
            button->setOpaque(false);
        }
        else
        {
            child->setOpaque(true);
        }
        child->getProperties().set(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::OSZI_THEME);
    }
    drawing_area->setOpaque(false);
    sl_show_range->setOpaque(false);
    setOpaque(true);

    refresh_buttons();

    /*

    setSize (1465, 180);

    */

    startTimer(UI_REFRESH_RATE);
}

Monique_Ui_AmpPainter::~Monique_Ui_AmpPainter()
{
    drawing_area = nullptr;
    sl_show_range = nullptr;
    osc_1 = nullptr;
    osc_2 = nullptr;
    osc_3 = nullptr;
    eq = nullptr;
    out = nullptr;
    f_1 = nullptr;
    f_2 = nullptr;
    f_3 = nullptr;
    f_env_1 = nullptr;
    f_env_2 = nullptr;
    f_env_3 = nullptr;
    out_env = nullptr;
}

//==============================================================================
void Monique_Ui_AmpPainter::paint(juce::Graphics &g)
{
    // TODO paint all or only values
    if (!is_currently_painting)
    {
        is_currently_painting = true;

        ComponentColours &colours(look_and_feel->colours);

        g.fillAll(colours.get_theme(COLOUR_THEMES::BG_THEME).area_colour);

        // TODO MAKE INTS!
        const int samples_to_paint =
            sl_show_range->getValue() * synth_data->runtime_notifyer->get_sample_rate() * 0.5;
        int width = drawing_area->getWidth();
        float scale = float(width) / samples_to_paint;
        const int paint_start_offset_x = drawing_area->getX();
        const int paint_start_offset_y = drawing_area->getY() + 1;
        const float height = drawing_area->getHeight() - 3;
        const int line_center = paint_start_offset_y + height / 2;

        const int current_size = master_osc_values->get_size();
        const int current_position =
            master_osc_values->get_new_reader_start_position(samples_to_paint);
        {
            /*
                Colour colour = Colour(0xff222222 );
                g.setGradientFill (ColourGradient (colour.darker (0.3f), 0.0f, 0.0f, Colour
               (0xff050505), 0.0f, height, false));
                //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f),
               0.0f, height, false)); g.fillRoundedRectangle (paint_start_offset_x,
               paint_start_offset_y, width, height, 3);

                g.setColour (colour.darker (0.6f));
                g.drawRoundedRectangle (paint_start_offset_x, paint_start_offset_y, width, height,
               3, 1);

                g.setColour (colours.label_text_colour.withAlpha(0.3f));
                g.fillRect (paint_start_offset_x, int(paint_start_offset_y+height/2), width, 1 );
            */
        }
        {
#include "mono_ui_includeHacks_BEGIN.h"
            WIDTH_AND_HIGHT_FACTORS

            g.setColour(Colour(0xffffff11));
            g.fillRoundedRectangle(10.0f, 2.0f, 1440.0f, 198.0f, 10.000f);
#include "mono_ui_includeHacks_END.h"
        }
        {
            g.setColour(colours.get_theme(COLOUR_THEMES::BG_THEME).oszi_1);
#include "mono_ui_includeHacks_BEGIN.h"
            WIDTH_AND_HIGHT_FACTORS
            g.fillRoundedRectangle(110.0f, 20.0f, 1245.0f, 130.0f, 10.000f);
#include "mono_ui_includeHacks_END.h"
        }

        struct Monique_Ui_AmpPainter
        {
            static void exec(juce::Graphics &g,

                             const int buffer_start_pos_, const int buffer_size_,
                             const float scale_,

                             const int x_offset_, const int y_center_, const int height_,

                             const juce::Colour &col_, bool paint_errors_red,

                             EndlessBuffer &source_buffer_, int num_samples_) noexcept
            {
                const juce::Colour col_fill(col_.withAlpha(0.2f));
                const juce::Colour norm_col(
                    col_.withAlpha(0.4f + juce::jmax(0.0f, juce::jmin(0.6f, 0.8f * scale_))));
                const float alpha_norm = 0.4f + juce::jmax(0.0f, juce::jmin(0.6f, 0.8f * scale_));
                const float alpha_fill = 0.2f;

                int last_x = -9999;
                int x_counter = 1;
                int pos_counter = buffer_start_pos_;
                for (int sid = 0; sid < num_samples_; ++sid)
                {

                    if (++pos_counter >= buffer_size_)
                    {
                        pos_counter = 0;
                    }

                    float y = source_buffer_.get(pos_counter);
                    MONO_SNAP_TO_ZERO(y);

                    float x_float = scale_ * sid + x_offset_;
                    MONO_SNAP_TO_ZERO(x_float);
                    const int x = x_float;
                    bool paint_line = false;
                    if (last_x != x)
                    {
                        paint_line = true;
                        last_x = x;
                        x_counter++;
                    }
                    else
                    {
                        x_counter = 1;
                    }

                    float compression_multi = 1;
                    if (sid < num_samples_ * 0.02)
                    {
                        compression_multi = 1.0f / (num_samples_) * (sid / 0.02);
                    }
                    else if (sid > num_samples_ * 0.98)
                    {
                        compression_multi = 1.0f / (num_samples_ * 0.02) * (num_samples_ - sid);
                    }
                    if (y >= 0)
                    {
                        if (y > 1)
                        {
                            y = 1;
                        }

                        const float h_float = y * height_ * 0.5f * compression_multi;
                        int h = h_float;
                        if (paint_line)
                        {
                            g.setColour(col_fill.withAlpha(compression_multi * alpha_fill));
                            g.fillRect(x, y_center_ - h, 1, h);
                        }
                        // DOT
                        {
                            g.setColour(norm_col.withAlpha(compression_multi * alpha_norm));
                            g.fillRect(x_float, float(y_center_) - h_float, 1.0f, 1.0f);
                        }
                    }
                    else
                    {
                        if (y < -1)
                        {
                            y = -1;
                        }

                        const float h_float = y * height_ * -0.5f * compression_multi;
                        int h = h_float;
                        if (paint_line)
                        {
                            g.setColour(col_fill.withAlpha(compression_multi * alpha_fill));
                            g.fillRect(x, y_center_, 1, h);
                        }
                        // DOT
                        {
                            g.setColour(norm_col.withAlpha(compression_multi * alpha_norm));
                            g.fillRect(x_float, float(y_center_) + h_float, 1.0f, 1.0f);
                        }
                    }
                }
            }
        };

        const bool show_osc[SUM_OSCS] = {synth_data->osci_show_osc_1, synth_data->osci_show_osc_2,
                                         synth_data->osci_show_osc_3};
        const bool show_flt[SUM_OSCS] = {synth_data->osci_show_flt_1, synth_data->osci_show_flt_2,
                                         synth_data->osci_show_flt_3};
        const bool show_flt_env[SUM_OSCS] = {synth_data->osci_show_flt_env_1,
                                             synth_data->osci_show_flt_env_2,
                                             synth_data->osci_show_flt_env_3};
        const bool show_eq = synth_data->osci_show_eq;
        const bool show_out = synth_data->osci_show_out;
        const bool show_out_env = synth_data->osci_show_out_env;

        // MASTER OSC'S
        if (show_osc[0])
        {
            juce::Colour col = look_and_feel->colours.get_theme(COLOUR_THEMES::OSC_THEME).oszi_1;
            Monique_Ui_AmpPainter::exec(g,

                                        current_position, current_size, scale,

                                        paint_start_offset_x, line_center, height,

                                        col, false, *master_osc_values,

                                        samples_to_paint);
        }

        // OSC'S
        for (int osc_id = 0; osc_id != 2; ++osc_id)
        {
            EndlessBuffer &values = *osc_values[osc_id];
            if (show_osc[osc_id + 1])
            {
                juce::Colour col;
                if (osc_id == 0)
                    col = look_and_feel->colours.get_theme(COLOUR_THEMES::OSC_THEME).oszi_2;
                else
                    col = look_and_feel->colours.get_theme(COLOUR_THEMES::OSC_THEME).oszi_3;

                Monique_Ui_AmpPainter::exec(g,

                                            current_position, current_size, scale,

                                            paint_start_offset_x, line_center, height,

                                            col, false, values,

                                            samples_to_paint);
            }
        }

        // EQ
        if (show_eq)
        {
            juce::Colour col = look_and_feel->colours.get_theme(COLOUR_THEMES::FX_THEME).oszi_1;

            Monique_Ui_AmpPainter::exec(g,

                                        current_position, current_size,

                                        scale,

                                        paint_start_offset_x, line_center, height,

                                        col, false, *eq_values,

                                        samples_to_paint);
        }

        // FILTERS
        for (int filter_id = 0; filter_id != filter_values.size(); ++filter_id)
        {
            juce::Colour col;
            if (filter_id == 0)
                col = look_and_feel->colours.get_theme(COLOUR_THEMES::FILTER_THEME).oszi_1;
            else if (filter_id == 1)
                col = look_and_feel->colours.get_theme(COLOUR_THEMES::FILTER_THEME).oszi_2;
            else
                col = look_and_feel->colours.get_theme(COLOUR_THEMES::FILTER_THEME).oszi_3;

            EndlessBuffer &values = *filter_values[filter_id];
            if (show_flt[filter_id])
            {
                Monique_Ui_AmpPainter::exec(g,

                                            current_position, current_size,

                                            scale,

                                            paint_start_offset_x, line_center, height,

                                            col, false, values,

                                            samples_to_paint);
            }

            EndlessBuffer &values_env = *filter_env_values[filter_id];
            if (show_flt_env[filter_id])
            {
                Monique_Ui_AmpPainter::exec(g,

                                            current_position, current_size,

                                            scale,

                                            paint_start_offset_x, line_center, height,

                                            col, false, values_env,

                                            samples_to_paint);
            }
        }

        // FINAL OUTPUT
        if (show_out)
        {
            Monique_Ui_AmpPainter::exec(
                g,

                current_position, current_size,

                scale,

                paint_start_offset_x, line_center, height,

                look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_1, true, *values,

                samples_to_paint);
        }

        // MAIN ENV
        if (show_out_env)
        {
            Monique_Ui_AmpPainter::exec(
                g,

                current_position, current_size,

                scale,

                paint_start_offset_x, line_center, height,

                look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_1, false,
                *values_env,

                samples_to_paint);
        }

        {
            g.setColour(look_and_feel->colours.get_theme(COLOUR_THEMES::OSZI_THEME).area_colour);
            g.fillPath(internalPath2);
            g.fillPath(internalPath1);
        }

        is_currently_painting = false;
    }

    /*

    g.fillAll (Colour (0xff050505));

    g.setColour (Colour (0xffffff11));
    g.fillRoundedRectangle (10.0f, 0.0f, 1440.0f, 198.0f, 10.000f);

    g.setColour (Colour (0xff050505));
    g.fillRoundedRectangle (110.0f, 20.0f, 1245.0f, 130.0f, 10.000f);

    g.setColour (Colour (0xffffff11));
    g.fillPath (internalPath1);

    g.setColour (Colours::yellow);
    g.fillPath (internalPath2);

    g.setGradientFill (ColourGradient (Colour (0xff050505),
                                       119.0f, 80.0f,
                                       Colour (0x00ff0000),
                                       140.0f, 80.0f,
                                       false));
    g.fillRoundedRectangle (119.0f, 20.0f, 20.0f, 130.0f, 1.000f);

    g.setGradientFill (ColourGradient (Colour (0xff050505),
                                       1345.0f, 80.0f,
                                       Colour (0x00ff0000),
                                       1325.0f, 80.0f,
                                       false));
    g.fillRoundedRectangle (1325.0f, 20.0f, 20.0f, 128.0f, 1.000f);

    */
}

void Monique_Ui_AmpPainter::resized()
{
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    drawing_area->setBounds(120, 20, 1225, 130);
    sl_show_range->setBounds(110, 150, 1250, 20);
    osc_1->setBounds(30, 20, 60, 20);
    osc_2->setBounds(30, 40, 60, 20);
    osc_3->setBounds(30, 60, 60, 20);
    eq->setBounds(30, 90, 60, 20);
    out->setBounds(30, 110, 60, 20);
    f_1->setBounds(1375, 20, 60, 20);
    f_2->setBounds(1375, 40, 60, 20);
    f_3->setBounds(1375, 60, 60, 20);
    f_env_1->setBounds(1375, 90, 60, 20);
    f_env_2->setBounds(1375, 110, 60, 20);
    f_env_3->setBounds(1375, 130, 60, 20);
    out_env->setBounds(30, 130, 60, 20);
    internalPath1.clear();
    internalPath1.startNewSubPath(110.0f, 75.0f);
    internalPath1.lineTo(120.0f, 85.0f);
    internalPath1.lineTo(110.0f, 95.0f);
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.startNewSubPath(1355.0f, 75.0f);
    internalPath2.lineTo(1345.0f, 85.0f);
    internalPath2.lineTo(1355.0f, 95.0f);
    internalPath2.closeSubPath();

#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_AmpPainter::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == sl_show_range.get())
    {
        synth_data->osci_show_range = sl_show_range->getValue();
    }
}

void Monique_Ui_AmpPainter::buttonClicked(juce::Button *buttonThatWasClicked)
{
    if (buttonThatWasClicked == osc_1.get())
    {
        synth_data->osci_show_osc_1 ^= true;
    }
    else if (buttonThatWasClicked == osc_2.get())
    {
        synth_data->osci_show_osc_2 ^= true;
    }
    else if (buttonThatWasClicked == osc_3.get())
    {
        synth_data->osci_show_osc_3 ^= true;
    }
    else if (buttonThatWasClicked == eq.get())
    {
        synth_data->osci_show_eq ^= true;
    }
    else if (buttonThatWasClicked == out.get())
    {
        synth_data->osci_show_out ^= true;
    }
    else if (buttonThatWasClicked == f_1.get())
    {
        synth_data->osci_show_flt_1 ^= true;
    }
    else if (buttonThatWasClicked == f_2.get())
    {
        synth_data->osci_show_flt_2 ^= true;
    }
    else if (buttonThatWasClicked == f_3.get())
    {
        synth_data->osci_show_flt_3 ^= true;
    }
    else if (buttonThatWasClicked == f_env_1.get())
    {
        synth_data->osci_show_flt_env_1 ^= true;
    }
    else if (buttonThatWasClicked == f_env_2.get())
    {
        synth_data->osci_show_flt_env_2 ^= true;
    }
    else if (buttonThatWasClicked == f_env_3.get())
    {
        synth_data->osci_show_flt_env_3 ^= true;
    }
    else if (buttonThatWasClicked == out_env.get())
    {
        synth_data->osci_show_out_env ^= true;
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
void Monique_Ui_AmpPainter::timerCallback()
{
    repaint(drawing_area->getBounds());
    refresh_buttons();
}

void Monique_Ui_AmpPainter::refresh_buttons()
{
    sl_show_range->setValue(synth_data->osci_show_range, juce::dontSendNotification);

#define SWITCH_OSZI_THEMES(state, button, theme, type)                                             \
    if (state)                                                                                     \
    {                                                                                              \
        if (button->getProperties().set(VAR_INDEX_COLOUR_THEME, theme))                            \
        {                                                                                          \
            button->getProperties().set(VAR_INDEX_BUTTON_AMP, type);                               \
            button->repaint();                                                                     \
        }                                                                                          \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
        if (button->getProperties().set(VAR_INDEX_COLOUR_THEME, OSZI_THEME))                       \
        {                                                                                          \
            button->getProperties().set(VAR_INDEX_BUTTON_AMP, 0);                                  \
            button->repaint();                                                                     \
        }                                                                                          \
    }

    SWITCH_OSZI_THEMES(synth_data->osci_show_osc_1, osc_1, OSC_THEME, OSZI_1)
    SWITCH_OSZI_THEMES(synth_data->osci_show_osc_2, osc_2, OSC_THEME, OSZI_2)
    SWITCH_OSZI_THEMES(synth_data->osci_show_osc_3, osc_3, OSC_THEME, OSZI_3)

    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_1, f_1, FILTER_THEME, OSZI_1)
    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_2, f_2, FILTER_THEME, OSZI_2)
    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_3, f_3, FILTER_THEME, OSZI_3)

    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_env_1, f_env_1, FILTER_THEME, OSZI_1)
    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_env_2, f_env_2, FILTER_THEME, OSZI_2)
    SWITCH_OSZI_THEMES(synth_data->osci_show_flt_env_3, f_env_3, FILTER_THEME, OSZI_3)

    SWITCH_OSZI_THEMES(synth_data->osci_show_eq, eq, FX_THEME, OSZI_1)

    SWITCH_OSZI_THEMES(synth_data->osci_show_out, out, MASTER_THEME, OSZI_1)
    SWITCH_OSZI_THEMES(synth_data->osci_show_out_env, out_env, MASTER_THEME, OSZI_2)
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD EndlessBuffer::EndlessBuffer(RuntimeNotifyer *const notifyer_) noexcept
    : RuntimeListener(notifyer_), sample_buffer(sample_rate * 2 + block_size),
      current_size(sample_rate * 2 + block_size), reader_position(0)
{
}
COLD EndlessBuffer::~EndlessBuffer() noexcept {}

//==============================================================================
COLD void EndlessBuffer::sample_rate_or_block_changed() noexcept
{
    reader_position = 0;
    current_size = sample_rate * 2 + block_size;
    sample_buffer.setSize(current_size);
}

//==============================================================================
inline float EndlessBuffer::get(int pos_) const noexcept
{
    return sample_buffer.getReadPointer()[pos_];
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD EndlessSwitchBuffer::EndlessSwitchBuffer(RuntimeNotifyer *const notifyer_) noexcept
    : EndlessBuffer(notifyer_), switch_buffer(sample_rate * 2 + block_size)
{
    sample_rate_or_block_changed();
}
COLD EndlessSwitchBuffer::~EndlessSwitchBuffer() noexcept {}

//==============================================================================
COLD void EndlessSwitchBuffer::sample_rate_or_block_changed() noexcept
{
    reader_position = 0;
    current_size = sample_rate * 2 + block_size;
    sample_buffer.setSize(current_size);
    switch_buffer.setSize(current_size);
    juce::FloatVectorOperations::fill(switch_buffer.getWritePointer(), true, current_size);
}

//==============================================================================
int EndlessSwitchBuffer::get_new_reader_start_position(int samples_to_paint_) const noexcept
{
    int start_position = reader_position - samples_to_paint_;
    if (start_position < 0)
    {
        start_position = current_size - (start_position * -1);
    }

    start_position = juce::jmin(current_size, juce::jmax(0, start_position));

    const float *const tmp_switch_buffer = switch_buffer.getReadPointer();
    int value = 0;
    for (int i = 0; i != current_size; ++i)
    {
        if (tmp_switch_buffer[start_position] != 0)
        {
            value = start_position;
            break;
        }
        else
        {
            if (start_position-- < 0)
            {
                start_position = current_size - 1;
            }
        }
    }

    return value;
}

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: bg_editors_svg, 7021, "../Files/STYLE/Monique-Hintergrund-1/bg_editors.svg"
static const unsigned char resource_Monique_Ui_AmpPainter_bg_editors_svg[] = {
    60,  63,  120, 109, 108, 32,  118, 101, 114, 115, 105, 111, 110, 61,  34,  49,  46,  48,  34,
    32,  101, 110, 99,  111, 100, 105, 110, 103, 61,  34,  85,  84,  70,  45,  56,  34,  32,  115,
    116, 97,  110, 100, 97,  108, 111, 110, 101, 61,  34,  110, 111, 34,  63,  62,  10,  60,  115,
    118, 103, 10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  100, 99,  61,  34,  104, 116, 116,
    112, 58,  47,  47,  112, 117, 114, 108, 46,  111, 114, 103, 47,  100, 99,  47,  101, 108, 101,
    109, 101, 110, 116, 115, 47,  49,  46,  49,  47,  34,  10,  32,  32,  32,  120, 109, 108, 110,
    115, 58,  99,  99,  61,  34,  104, 116, 116, 112, 58,  47,  47,  99,  114, 101, 97,  116, 105,
    118, 101, 99,  111, 109, 109, 111, 110, 115, 46,  111, 114, 103, 47,  110, 115, 35,  34,  10,
    32,  32,  32,  120, 109, 108, 110, 115, 58,  114, 100, 102, 61,  34,  104, 116, 116, 112, 58,
    47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  49,  57,  57,  57,  47,  48,
    50,  47,  50,  50,  45,  114, 100, 102, 45,  115, 121, 110, 116, 97,  120, 45,  110, 115, 35,
    34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  115, 118, 103, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 61,  34,  104, 116, 116,
    112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114, 103, 47,  50,  48,  48,  48,
    47,  115, 118, 103, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  120, 108, 105, 110,
    107, 61,  34,  104, 116, 116, 112, 58,  47,  47,  119, 119, 119, 46,  119, 51,  46,  111, 114,
    103, 47,  49,  57,  57,  57,  47,  120, 108, 105, 110, 107, 34,  10,  32,  32,  32,  120, 109,
    108, 110, 115, 58,  115, 111, 100, 105, 112, 111, 100, 105, 61,  34,  104, 116, 116, 112, 58,
    47,  47,  115, 111, 100, 105, 112, 111, 100, 105, 46,  115, 111, 117, 114, 99,  101, 102, 111,
    114, 103, 101, 46,  110, 101, 116, 47,  68,  84,  68,  47,  115, 111, 100, 105, 112, 111, 100,
    105, 45,  48,  46,  100, 116, 100, 34,  10,  32,  32,  32,  120, 109, 108, 110, 115, 58,  105,
    110, 107, 115, 99,  97,  112, 101, 61,  34,  104, 116, 116, 112, 58,  47,  47,  119, 119, 119,
    46,  105, 110, 107, 115, 99,  97,  112, 101, 46,  111, 114, 103, 47,  110, 97,  109, 101, 115,
    112, 97,  99,  101, 115, 47,  105, 110, 107, 115, 99,  97,  112, 101, 34,  10,  32,  32,  32,
    119, 105, 100, 116, 104, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  104, 101, 105,
    103, 104, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  118, 105, 101, 119, 66,
    111, 120, 61,  34,  48,  32,  48,  32,  49,  50,  55,  52,  32,  56,  52,  48,  34,  10,  32,
    32,  32,  118, 101, 114, 115, 105, 111, 110, 61,  34,  49,  46,  49,  34,  10,  32,  32,  32,
    120, 109, 108, 58,  115, 112, 97,  99,  101, 61,  34,  112, 114, 101, 115, 101, 114, 118, 101,
    34,  10,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108, 108, 45,  114, 117,
    108, 101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  99,  108, 105, 112, 45,  114, 117, 108,
    101, 58,  101, 118, 101, 110, 111, 100, 100, 59,  115, 116, 114, 111, 107, 101, 45,  108, 105,
    110, 101, 99,  97,  112, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111, 107, 101, 45,
    108, 105, 110, 101, 106, 111, 105, 110, 58,  114, 111, 117, 110, 100, 59,  115, 116, 114, 111,
    107, 101, 45,  109, 105, 116, 101, 114, 108, 105, 109, 105, 116, 58,  49,  46,  52,  49,  52,
    50,  49,  59,  34,  10,  32,  32,  32,  105, 100, 61,  34,  115, 118, 103, 50,  34,  10,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  118, 101, 114, 115, 105, 111, 110, 61,
    34,  48,  46,  52,  56,  46,  52,  32,  114, 57,  57,  51,  57,  34,  10,  32,  32,  32,  115,
    111, 100, 105, 112, 111, 100, 105, 58,  100, 111, 99,  110, 97,  109, 101, 61,  34,  48,  49,
    72,  105, 110, 116, 101, 114, 103, 114, 117, 110, 100, 45,  65,  108, 108, 101, 115, 46,  115,
    118, 103, 34,  62,  60,  109, 101, 116, 97,  100, 97,  116, 97,  10,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  109, 101, 116, 97,  100, 97,  116, 97,  50,  56,  53,  34,  62,  60,  114,
    100, 102, 58,  82,  68,  70,  62,  60,  99,  99,  58,  87,  111, 114, 107, 10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  114, 100, 102, 58,  97,  98,  111, 117, 116, 61,  34,  34,  62,
    60,  100, 99,  58,  102, 111, 114, 109, 97,  116, 62,  105, 109, 97,  103, 101, 47,  115, 118,
    103, 43,  120, 109, 108, 60,  47,  100, 99,  58,  102, 111, 114, 109, 97,  116, 62,  60,  100,
    99,  58,  116, 121, 112, 101, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  114,
    100, 102, 58,  114, 101, 115, 111, 117, 114, 99,  101, 61,  34,  104, 116, 116, 112, 58,  47,
    47,  112, 117, 114, 108, 46,  111, 114, 103, 47,  100, 99,  47,  100, 99,  109, 105, 116, 121,
    112, 101, 47,  83,  116, 105, 108, 108, 73,  109, 97,  103, 101, 34,  32,  47,  62,  60,  100,
    99,  58,  116, 105, 116, 108, 101, 62,  60,  47,  100, 99,  58,  116, 105, 116, 108, 101, 62,
    60,  47,  99,  99,  58,  87,  111, 114, 107, 62,  60,  47,  114, 100, 102, 58,  82,  68,  70,
    62,  60,  47,  109, 101, 116, 97,  100, 97,  116, 97,  62,  60,  115, 111, 100, 105, 112, 111,
    100, 105, 58,  110, 97,  109, 101, 100, 118, 105, 101, 119, 10,  32,  32,  32,  32,  32,  112,
    97,  103, 101, 99,  111, 108, 111, 114, 61,  34,  35,  102, 102, 102, 102, 102, 102, 34,  10,
    32,  32,  32,  32,  32,  98,  111, 114, 100, 101, 114, 99,  111, 108, 111, 114, 61,  34,  35,
    54,  54,  54,  54,  54,  54,  34,  10,  32,  32,  32,  32,  32,  98,  111, 114, 100, 101, 114,
    111, 112, 97,  99,  105, 116, 121, 61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  111, 98,
    106, 101, 99,  116, 116, 111, 108, 101, 114, 97,  110, 99,  101, 61,  34,  49,  48,  34,  10,
    32,  32,  32,  32,  32,  103, 114, 105, 100, 116, 111, 108, 101, 114, 97,  110, 99,  101, 61,
    34,  49,  48,  34,  10,  32,  32,  32,  32,  32,  103, 117, 105, 100, 101, 116, 111, 108, 101,
    114, 97,  110, 99,  101, 61,  34,  49,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107,
    115, 99,  97,  112, 101, 58,  112, 97,  103, 101, 111, 112, 97,  99,  105, 116, 121, 61,  34,
    48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  112, 97,
    103, 101, 115, 104, 97,  100, 111, 119, 61,  34,  50,  34,  10,  32,  32,  32,  32,  32,  105,
    110, 107, 115, 99,  97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  119, 105, 100, 116,
    104, 61,  34,  49,  57,  50,  48,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,
    97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  104, 101, 105, 103, 104, 116, 61,  34,
    49,  48,  54,  48,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  110, 97,  109, 101,
    100, 118, 105, 101, 119, 50,  56,  51,  34,  10,  32,  32,  32,  32,  32,  115, 104, 111, 119,
    103, 114, 105, 100, 61,  34,  102, 97,  108, 115, 101, 34,  10,  32,  32,  32,  32,  32,  105,
    110, 107, 115, 99,  97,  112, 101, 58,  122, 111, 111, 109, 61,  34,  49,  46,  49,  50,  51,
    56,  48,  57,  53,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101,
    58,  99,  120, 61,  34,  54,  50,  56,  46,  49,  48,  55,  56,  49,  34,  10,  32,  32,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  121, 61,  34,  51,  53,  52,  46,
    53,  56,  54,  54,  57,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112,
    101, 58,  119, 105, 110, 100, 111, 119, 45,  120, 61,  34,  45,  51,  34,  10,  32,  32,  32,
    32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  119, 105, 110, 100, 111, 119, 45,  121,
    61,  34,  45,  51,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101,
    58,  119, 105, 110, 100, 111, 119, 45,  109, 97,  120, 105, 109, 105, 122, 101, 100, 61,  34,
    49,  34,  10,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  117,
    114, 114, 101, 110, 116, 45,  108, 97,  121, 101, 114, 61,  34,  115, 118, 103, 50,  34,  32,
    47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111, 114, 109,
    61,  34,  109, 97,  116, 114, 105, 120, 40,  48,  46,  57,  53,  48,  53,  56,  53,  44,  48,
    44,  48,  44,  48,  46,  57,  53,  48,  53,  56,  53,  44,  51,  50,  46,  57,  53,  51,  56,
    44,  48,  46,  54,  54,  57,  41,  34,  10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  103,
    49,  49,  54,  34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  105, 110, 107, 115,
    99,  97,  112, 101, 58,  108, 97,  98,  101, 108, 61,  34,  76,  97,  121, 101, 114, 34,  10,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  108, 97,  121, 101, 114, 49,  34,  10,  32,  32,
    32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  103, 114, 111, 117, 112, 109, 111,
    100, 101, 61,  34,  108, 97,  121, 101, 114, 34,  10,  32,  32,  32,  32,  32,  116, 114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  116, 114, 97,  110, 115, 108, 97,  116, 101, 40,  48,
    44,  49,  49,  41,  34,  32,  47,  62,  60,  103, 10,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  103, 49,  52,  57,  34,  10,  32,  32,  32,  32,  32,  116, 114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  56,  46,  53,  50,  50,  56,  55,  44,
    48,  44,  48,  44,  49,  46,  48,  48,  53,  53,  49,  44,  45,  49,  49,  46,  49,  54,  56,
    54,  44,  54,  54,  49,  46,  57,  52,  57,  41,  34,  62,  60,  112, 97,  116, 104, 10,  32,
    32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 110, 110,
    101, 99,  116, 111, 114, 45,  99,  117, 114, 118, 97,  116, 117, 114, 101, 61,  34,  48,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  112, 97,  116, 104, 49,  53,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  102, 105, 108,
    108, 58,  35,  97,  49,  97,  97,  98,  50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  100,
    61,  34,  109, 32,  49,  52,  55,  46,  50,  48,  54,  44,  52,  51,  46,  51,  48,  49,  32,
    99,  32,  48,  44,  45,  56,  46,  49,  54,  50,  32,  45,  48,  46,  55,  56,  49,  44,  45,
    49,  52,  46,  55,  56,  56,  32,  45,  49,  46,  55,  52,  52,  44,  45,  49,  52,  46,  55,
    56,  56,  32,  108, 32,  45,  49,  52,  48,  46,  55,  49,  55,  44,  48,  32,  67,  32,  51,
    46,  55,  56,  50,  44,  50,  56,  46,  53,  49,  51,  32,  51,  44,  51,  53,  46,  49,  51,
    57,  32,  51,  44,  52,  51,  46,  51,  48,  49,  32,  108, 32,  48,  44,  49,  49,  49,  46,
    52,  53,  55,  32,  99,  32,  48,  44,  56,  46,  49,  54,  50,  32,  48,  46,  55,  56,  50,
    44,  49,  52,  46,  55,  56,  56,  32,  49,  46,  55,  52,  53,  44,  49,  52,  46,  55,  56,
    56,  32,  108, 32,  49,  52,  48,  46,  55,  49,  55,  44,  48,  32,  99,  32,  48,  46,  57,
    54,  51,  44,  48,  32,  49,  46,  55,  52,  52,  44,  45,  54,  46,  54,  50,  54,  32,  49,
    46,  55,  52,  52,  44,  45,  49,  52,  46,  55,  56,  56,  32,  108, 32,  48,  44,  45,  49,
    49,  49,  46,  52,  53,  55,  32,  122, 34,  32,  47,  62,  60,  47,  103, 62,  60,  100, 101,
    102, 115, 10,  32,  32,  32,  32,  32,  105, 100, 61,  34,  100, 101, 102, 115, 50,  50,  55,
    34,  62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,
    61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116,
    115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,
    32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110,
    115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,
    54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,
    49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  62,  60,  115, 116, 111, 112,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,
    37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,
    115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,
    49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105,
    116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    115, 116, 111, 112, 50,  51,  48,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  55,  37,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116,
    111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  55,  48,  44,  54,  57,  44,
    54,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,
    51,  50,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  51,  57,  37,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111,
    108, 111, 114, 58,  114, 103, 98,  40,  57,  56,  44,  57,  56,  44,  57,  56,  41,  59,  115,
    116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  51,  52,  34,  32,  47,
    62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102,
    115, 101, 116, 61,  34,  56,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114,
    103, 98,  40,  57,  53,  44,  57,  53,  44,  57,  53,  41,  59,  115, 116, 111, 112, 45,  111,
    112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  115, 116, 111, 112, 50,  51,  54,  34,  32,  47,  62,  60,  115, 116, 111,
    112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,
    57,  54,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101,
    61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,
    48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,
    99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  115, 116, 111, 112, 50,  51,  56,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,
    48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,
    44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  115, 116, 111, 112, 50,  52,  48,  34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,
    114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114,
    97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,
    76,  105, 110, 101, 97,  114, 50,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,
    34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100,
    105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,
    101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100,
    105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114,
    105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,
    48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,
    41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111,
    102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,
    114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115, 116,
    111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  52,  51,  34,  32,  47,  62,
    60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115,
    101, 116, 61,  34,  49,  55,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115,
    116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103,
    98,  40,  55,  48,  44,  54,  57,  44,  54,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112,
    97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105,
    100, 61,  34,  115, 116, 111, 112, 50,  52,  53,  34,  32,  47,  62,  60,  115, 116, 111, 112,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  51,
    57,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,
    34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  57,  56,  44,
    57,  56,  44,  57,  56,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121,
    58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116,
    111, 112, 50,  52,  55,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  55,  54,  37,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112,
    45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  57,  53,  44,  57,  53,  44,  57,  53,
    41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  52,  57,
    34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    111, 102, 102, 115, 101, 116, 61,  34,  57,  54,  37,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111,
    114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,
    115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  53,  49,  34,  32,
    47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102,
    102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114,
    58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,  41,  59,  115,
    116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  53,  51,  34,  32,  47,
    62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,
    108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,
    32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 51,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,
    117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111,
    114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,
    48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,
    44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111,
    112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,
    44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,
    49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111,
    112, 50,  53,  54,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  55,  37,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,
    99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  55,  48,  44,  54,  57,  44,  54,  53,  41,
    59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  53,  56,  34,
    32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111,
    102, 102, 115, 101, 116, 61,  34,  51,  57,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114,
    58,  114, 103, 98,  40,  57,  56,  44,  57,  56,  44,  57,  56,  41,  59,  115, 116, 111, 112,
    45,  111, 112, 97,  99,  105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  54,  48,  34,  32,  47,  62,  60,  115,
    116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116,
    61,  34,  56,  49,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,
    57,  53,  44,  57,  53,  44,  57,  53,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,
    105, 116, 121, 58,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,
    34,  115, 116, 111, 112, 50,  54,  50,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  57,  54,  37,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115,
    116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,
    55,  55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116,
    121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115,
    116, 111, 112, 50,  54,  52,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116,
    111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,
    55,  44,  49,  56,  55,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121,
    58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116,
    111, 112, 50,  54,  54,  34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114,
    97,  100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105,
    101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  95,  76,  105, 110,
    101, 97,  114, 52,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110,
    116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110,
    85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110,
    116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,
    52,  49,  46,  54,  48,  55,  49,  44,  48,  44,  48,  44,  51,  52,  46,  57,  48,  53,  51,
    44,  53,  55,  46,  49,  52,  56,  54,  44,  54,  55,  51,  46,  49,  54,  54,  41,  34,  62,
    60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115,
    101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116,
    121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,
    40,  49,  49,  54,  44,  49,  49,  57,  44,  49,  50,  50,  41,  59,  115, 116, 111, 112, 45,
    111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  54,  57,  34,  32,  47,  62,  60,  115, 116,
    111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,
    34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  115, 116, 121,
    108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111, 114, 58,  114, 103, 98,  40,
    49,  54,  49,  44,  49,  55,  48,  44,  49,  55,  56,  41,  59,  115, 116, 111, 112, 45,  111,
    112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
    105, 100, 61,  34,  115, 116, 111, 112, 50,  55,  49,  34,  32,  47,  62,  60,  47,  108, 105,
    110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108, 105, 110, 101, 97,
    114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 100,
    61,  34,  95,  76,  105, 110, 101, 97,  114, 53,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103,
    114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,
    112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103,
    114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109,
    97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,  50,
    55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,  48,
    53,  53,  53,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,
    32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108,
    111, 114, 58,  114, 103, 98,  40,  49,  51,  51,  44,  49,  51,  51,  44,  49,  51,  51,  41,
    59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  55,  52,  34,
    32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  111,
    102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111, 108, 111,
    114, 58,  114, 103, 98,  40,  50,  53,  49,  44,  55,  56,  44,  56,  49,  41,  59,  115, 116,
    111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  55,  54,  34,  32,  47,  62,
    60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 62,  60,  108,
    105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,
    32,  32,  105, 100, 61,  34,  95,  76,  105, 110, 101, 97,  114, 54,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121,
    49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117,
    115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,
    32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114,
    109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  49,  48,  51,  49,  46,  48,  51,  44,  48,
    44,  48,  44,  52,  49,  54,  46,  48,  49,  56,  44,  50,  50,  52,  46,  54,  55,  50,  44,
    51,  49,  50,  46,  51,  41,  34,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,
    32,  32,  32,  32,  111, 102, 102, 115, 101, 116, 61,  34,  48,  37,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,
    111, 108, 111, 114, 58,  114, 103, 98,  40,  49,  51,  51,  44,  49,  51,  51,  44,  49,  51,
    51,  41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  55,
    57,  34,  32,  47,  62,  60,  115, 116, 111, 112, 10,  32,  32,  32,  32,  32,  32,  32,  32,
    32,  111, 102, 102, 115, 101, 116, 61,  34,  49,  48,  48,  37,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  32,  32,  115, 116, 121, 108, 101, 61,  34,  115, 116, 111, 112, 45,  99,  111,
    108, 111, 114, 58,  114, 103, 98,  40,  49,  54,  48,  44,  49,  55,  55,  44,  49,  56,  55,
    41,  59,  115, 116, 111, 112, 45,  111, 112, 97,  99,  105, 116, 121, 58,  49,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  115, 116, 111, 112, 50,  56,  49,
    34,  32,  47,  62,  60,  47,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110,
    116, 62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,
    32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,  111, 108, 108,
    101, 99,  116, 61,  34,  97,  108, 119, 97,  121, 115, 34,  10,  32,  32,  32,  32,  32,  32,
    32,  120, 108, 105, 110, 107, 58,  104, 114, 101, 102, 61,  34,  35,  95,  76,  105, 110, 101,
    97,  114, 49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,  108, 105, 110,
    101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 51,  51,  52,  56,  34,  10,  32,  32,
    32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,
    34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,
    32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102,
    111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,
    44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,
    52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,
    120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,
    34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  32,  47,  62,  60,  108, 105, 110, 101, 97,
    114, 71,  114, 97,  100, 105, 101, 110, 116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 110,
    107, 115, 99,  97,  112, 101, 58,  99,  111, 108, 108, 101, 99,  116, 61,  34,  97,  108, 119,
    97,  121, 115, 34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 108, 105, 110, 107, 58,  104,
    114, 101, 102, 61,  34,  35,  95,  76,  105, 110, 101, 97,  114, 50,  34,  10,  32,  32,  32,
    32,  32,  32,  32,  105, 100, 61,  34,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105,
    101, 110, 116, 51,  51,  53,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,
    100, 105, 101, 110, 116, 85,  110, 105, 116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,
    99,  101, 79,  110, 85,  115, 101, 34,  10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,
    100, 105, 101, 110, 116, 84,  114, 97,  110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116,
    114, 105, 120, 40,  54,  49,  46,  56,  48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,
    55,  48,  53,  57,  44,  49,  51,  55,  46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,
    53,  41,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,
    32,  32,  32,  32,  32,  32,  121, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,
    32,  120, 50,  61,  34,  49,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,
    48,  34,  32,  47,  62,  60,  108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110,
    116, 10,  32,  32,  32,  32,  32,  32,  32,  105, 110, 107, 115, 99,  97,  112, 101, 58,  99,
    111, 108, 108, 101, 99,  116, 61,  34,  97,  108, 119, 97,  121, 115, 34,  10,  32,  32,  32,
    32,  32,  32,  32,  120, 108, 105, 110, 107, 58,  104, 114, 101, 102, 61,  34,  35,  95,  76,
    105, 110, 101, 97,  114, 51,  34,  10,  32,  32,  32,  32,  32,  32,  32,  105, 100, 61,  34,
    108, 105, 110, 101, 97,  114, 71,  114, 97,  100, 105, 101, 110, 116, 51,  51,  53,  50,  34,
    10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 85,  110, 105,
    116, 115, 61,  34,  117, 115, 101, 114, 83,  112, 97,  99,  101, 79,  110, 85,  115, 101, 34,
    10,  32,  32,  32,  32,  32,  32,  32,  103, 114, 97,  100, 105, 101, 110, 116, 84,  114, 97,
    110, 115, 102, 111, 114, 109, 61,  34,  109, 97,  116, 114, 105, 120, 40,  54,  49,  46,  56,
    48,  54,  57,  44,  48,  44,  48,  44,  50,  55,  46,  55,  48,  53,  57,  44,  49,  51,  55,
    46,  49,  54,  52,  44,  54,  52,  46,  48,  53,  53,  53,  41,  34,  10,  32,  32,  32,  32,
    32,  32,  32,  120, 49,  61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  121, 49,
    61,  34,  48,  34,  10,  32,  32,  32,  32,  32,  32,  32,  120, 50,  61,  34,  49,  34,  10,
    32,  32,  32,  32,  32,  32,  32,  121, 50,  61,  34,  48,  34,  32,  47,  62,  60,  47,  100,
    101, 102, 115, 62,  60,  47,  115, 118, 103, 62,  0,   0};

const char *Monique_Ui_AmpPainter::bg_editors_svg =
    (const char *)resource_Monique_Ui_AmpPainter_bg_editors_svg;
const int Monique_Ui_AmpPainter::bg_editors_svgSize = 7021;
