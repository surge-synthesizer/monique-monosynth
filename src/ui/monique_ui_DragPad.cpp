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

#include "monique_ui_DragPad.h"

#include "monique_ui_LookAndFeel.h"
#include "core/monique_core_Datastructures.h"

#include <memory>

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class DragPad : public juce::Component
{
    MoniqueSynthData *const synth_data;

    Monique_Ui_DragPad *const parent;
    void mouseDoubleClick(const juce::MouseEvent &) override;
    void mouseDrag(const juce::MouseEvent &event) override;

  public:
    COLD DragPad(MoniqueSynthData *const synth_data_, Monique_Ui_DragPad *const parent_);
    COLD ~DragPad();
};

//==============================================================================
COLD DragPad::DragPad(MoniqueSynthData *const synth_data_, Monique_Ui_DragPad *const parent_)
    : synth_data(synth_data_), parent(parent_)
{
}
COLD DragPad::~DragPad() {}

//==============================================================================
void DragPad::mouseDoubleClick(const juce::MouseEvent &)
{
    const int morph_motor_time(synth_data->morph_motor_time);
    ChangeParamOverTime::execute(synth_data->morhp_states[0], 0, morph_motor_time);
    ChangeParamOverTime::execute(synth_data->morhp_states[1], 0, morph_motor_time);
    ChangeParamOverTime::execute(synth_data->morhp_states[2], 0, morph_motor_time);
    ChangeParamOverTime::execute(synth_data->morhp_states[3], 0, morph_motor_time);

    parent->set_left_to_right_states(0.5f, 0.5f);
}
void DragPad::mouseDrag(const juce::MouseEvent &event)
{
    juce::Point<int> current_position = event.getPosition();

    float left2right_state = 0;
    {
        if (current_position.getX() >= 0 && current_position.getX() <= getWidth())
        {
            left2right_state = 1.0f / getWidth() * current_position.getX();
        }
        else if (current_position.getX() < 0)
        {
            left2right_state = 0;
        }
        else if (current_position.getX() > getWidth())
        {
            left2right_state = 1;
        }
    }

    float top2bottom_state = 0;
    {
        if (current_position.getY() >= 0 && current_position.getY() <= getHeight())
        {
            top2bottom_state = 1.0f / getHeight() * current_position.getY();
        }
        else if (current_position.getY() < 0)
        {
            top2bottom_state = 0;
        }
        else if (current_position.getY() > getHeight())
        {
            top2bottom_state = 1;
        }
    }

    {
        const int morph_motor_time(synth_data->morph_motor_time);

        float morph_top_left = 1.0f - left2right_state - top2bottom_state;
        {
            if (morph_top_left < 0)
                morph_top_left = 0;
            else if (morph_top_left > 1)
                morph_top_left = 1;
            ChangeParamOverTime::execute(synth_data->morhp_states[0], morph_top_left,
                                         morph_motor_time);
        }

        float morph_top_right = left2right_state - top2bottom_state;
        {
            if (morph_top_right < 0)
                morph_top_right = 0;
            else if (morph_top_right > 1)
                morph_top_right = 1;
            ChangeParamOverTime::execute(synth_data->morhp_states[1], morph_top_right,
                                         morph_motor_time);
        }

        float morph_bottom_left = top2bottom_state - left2right_state;
        {
            if (morph_bottom_left < 0)
                morph_bottom_left = 0;
            else if (morph_bottom_left > 1)
                morph_bottom_left = 1;
            ChangeParamOverTime::execute(synth_data->morhp_states[3], morph_bottom_left,
                                         morph_motor_time);
        }

        float morph_bottom_right = top2bottom_state - (1.0f - left2right_state);
        {
            if (morph_bottom_right < 0)
                morph_bottom_right = 0;
            else if (morph_bottom_right > 1)
                morph_bottom_right = 1;
            ChangeParamOverTime::execute(synth_data->morhp_states[2], morph_bottom_right,
                                         morph_motor_time);
        }

        parent->set_left_to_right_states(left2right_state, top2bottom_state);
    }
}

//==============================================================================
Monique_Ui_DragPad::Monique_Ui_DragPad(Monique_Ui_Refresher *ui_refresher_)
    : original_w(80), original_h(130), ui_refresher(ui_refresher_), left2right_state(0.5),
      top2bottom_state(0.5), current_position(0, 0)
{
    look_and_feel = ui_refresher->look_and_feel;

    track_area = std::make_unique<DragPad>(ui_refresher_->synth_data, this);
    addAndMakeVisible(*track_area);

    this->setWantsKeyboardFocus(false);
    // this->setOpaque(true);

    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        juce::Component *comp = getChildComponent(i);
        comp->setWantsKeyboardFocus(false);
        // comp->setOpaque(true);
    }

    /*

    setSize (80, 130);

    */
}

Monique_Ui_DragPad::~Monique_Ui_DragPad() { track_area = nullptr; }

//==============================================================================
void Monique_Ui_DragPad::paint(juce::Graphics &g)
{
    ComponentColours &colours_ = look_and_feel->colours;

    const float w_h = 15;
    const float x = track_area->getX() + 1;
    const float y = track_area->getY() + 1;
    const float w = track_area->getWidth() - w_h - 2;
    const float h = track_area->getHeight() - w_h - 2;

    look_and_feel->drawGlassSphere(
        g, x + w * left2right_state, y + h * top2bottom_state, w_h,
        colours_.get_theme(COLOUR_THEMES::MORPH_THEME).value_slider_track_colour, 0.3f);
    /*

    g.setColour (Colour (0xff11ffff));
    g.fillEllipse (45.0f, 97.0f, 20.0f, 20.0f);

    g.setColour (Colour (0xffff3b00));
    g.drawEllipse (45.0f, 97.0f, 20.0f, 20.0f, 2.000f);

    */
}

void Monique_Ui_DragPad::resized()
{
#include "mono_ui_includeHacks_BEGIN.h"
    WIDTH_AND_HIGHT_FACTORS

    track_area->setBounds(0, 0, 80, 130);
#include "mono_ui_includeHacks_END.h"
}

void Monique_Ui_DragPad::set_left_to_right_states(float left2right_state_, float top2bottom_state_)
{
    left2right_state = left2right_state_;
    top2bottom_state = top2bottom_state_;

    repaint();
}
