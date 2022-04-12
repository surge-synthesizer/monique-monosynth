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

#ifndef __JUCE_HEADER_15EBFFC85DA080CA__
#define __JUCE_HEADER_15EBFFC85DA080CA__

//[Headers]     -- You can add your own extra header files here --
#include "App.h"
#include "core/monique_core_Datastructures.h"

//==============================================================================
//==============================================================================
//==============================================================================
class EndlessBuffer : public RuntimeListener
{
  protected:
    mono_AudioSampleBuffer<1> sample_buffer;

    int current_size;
    int reader_position;

  public:
    inline void write(const float *samples_, int num_samples_) noexcept;
    inline void write(const float *samples_, const float *samples_2_, int num_samples_) noexcept;

  public:
    // TODO you can improve it by coounting the pos one time by a master and not for each painting:
    // also you have it in sync
    inline float get(int pos_) const noexcept;
    inline float get_size() const noexcept { return current_size; }
    void clear() noexcept { sample_buffer.clear(); }
    int get_reader_start() const noexcept { return reader_position; }
    void set_reader_start(int reader_position_) noexcept { reader_position = reader_position_; }

  private:
    COLD virtual void sample_rate_or_block_changed() noexcept override;

  public:
    COLD EndlessBuffer(RuntimeNotifyer *const notifyer_) noexcept;
    COLD virtual ~EndlessBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EndlessBuffer)
};
//==============================================================================
inline void EndlessBuffer::write(const float *samples_, int num_samples_) noexcept
{
    float *const tmp_sample_buffer = sample_buffer.getWritePointer(0);
    int tmp_position = reader_position;

    if (tmp_position + num_samples_ < current_size)
    {
        juce::FloatVectorOperations::copy(&tmp_sample_buffer[tmp_position], samples_, num_samples_);
        tmp_position += num_samples_;
    }
    else
    {
        for (int sid = 0; sid != num_samples_; ++sid)
        {
            if (++tmp_position >= current_size)
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = samples_[sid];
        }
    }

    reader_position = tmp_position;
}
static inline float sample_mix_ui(float s1_, float s2_) noexcept
{
    if ((s1_ > 0) && (s2_ > 0))
    {
        s1_ = s1_ + s2_ - (s1_ * s2_);
    }
    else if ((s1_ < 0) && (s2_ < 0))
    {
        s1_ = s1_ + s2_ + (s1_ * s2_);
    }
    else
    {
        s1_ = s1_ + s2_;
    }

    return s1_;
}
inline void EndlessBuffer::write(const float *samples_, const float *samples_2_,
                                 int num_samples_) noexcept
{
    float *const tmp_sample_buffer = sample_buffer.getWritePointer(0);
    int tmp_position = reader_position;

    {
        for (int sid = 0; sid != num_samples_; ++sid)
        {
            if (++tmp_position >= current_size)
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = sample_mix_ui(samples_[sid], samples_2_[sid]);
        }
    }

    reader_position = tmp_position;
}

//==============================================================================
//==============================================================================
//==============================================================================
class EndlessSwitchBuffer : public EndlessBuffer
{
    mono_AudioSampleBuffer<1> switch_buffer;

  public:
    inline void write(const float *samples_, const float *switchs_, int num_samples_) noexcept;
    inline int get_new_reader_start_position(int samples_to_paint_) const noexcept;
    void clear() noexcept
    {
        juce::FloatVectorOperations::fill(switch_buffer.getWritePointer(), 1,
                                          switch_buffer.get_size());
        EndlessBuffer::clear();
    }

  private:
    COLD void sample_rate_or_block_changed() noexcept override;

  public:
    COLD EndlessSwitchBuffer(RuntimeNotifyer *const notifyer_) noexcept;
    COLD ~EndlessSwitchBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EndlessSwitchBuffer)
};

//==============================================================================
inline void EndlessSwitchBuffer::write(const float *samples_, const float *switchs_,
                                       int num_samples_) noexcept
{
    float *const tmp_sample_buffer = sample_buffer.getWritePointer();
    float *const tmp_switch_buffer = switch_buffer.getWritePointer();
    int tmp_position = reader_position;

    if (tmp_position + num_samples_ < current_size)
    {
        juce::FloatVectorOperations::copy(&tmp_sample_buffer[tmp_position], samples_, num_samples_);
        juce::FloatVectorOperations::copy(&tmp_switch_buffer[tmp_position], switchs_, num_samples_);
        tmp_position += num_samples_;
    }
    else
    {
        for (int sid = 0; sid != num_samples_; ++sid)
        {
            if (++tmp_position >= current_size)
            {
                tmp_position = 0;
            }

            tmp_sample_buffer[tmp_position] = samples_[sid];
            tmp_switch_buffer[tmp_position] = switchs_[sid];
        }
    }

    reader_position = tmp_position;
}
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_AmpPainter : public juce::Timer,
                              public juce::Slider::Listener,
                              public juce::Button::Listener,
                              public juce::Component
{
  public:
    //==============================================================================
    Monique_Ui_AmpPainter(MoniqueSynthData *synth_data_, UiLookAndFeel *look_and_feel_);
    ~Monique_Ui_AmpPainter();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;

  private:
    int current_buffer_start_pos;
    MoniqueSynthData *const synth_data;
    UiLookAndFeel *const look_and_feel;
    juce::OwnedArray<EndlessBuffer> filter_values;
    juce::OwnedArray<EndlessBuffer> filter_env_values;
    std::unique_ptr<EndlessBuffer> eq_values;
    std::unique_ptr<EndlessBuffer> values_env;
    std::unique_ptr<EndlessBuffer> values;
    std::unique_ptr<EndlessSwitchBuffer> master_osc_values;
    juce::OwnedArray<EndlessBuffer> osc_values;

    juce::Array<EndlessBuffer *> buffers;

  public:
    inline void calc_new_cycle() noexcept;
    inline void add_filter(int id_, const float *values_l_, const float *values_r_,
                           int num_samples_) noexcept;
    inline void add_filter_env(int id_, const float *values_, int num_samples_) noexcept;
    inline void add_eq(const float *values_, int num_samples_) noexcept;
    inline void add_out_env(const float *values_, int num_samples_) noexcept;
    inline void add_out(const float *values_l_, const float *values_r_, int num_samples_) noexcept;
    inline void add_master_osc(const float *values_, const float *is_switch_values,
                               int num_samples_) noexcept;
    inline void add_osc(int id_, const float *values_, int num_samples_) noexcept;
    inline void clear_and_keep_minimum() noexcept;

  private:
    void timerCallback() override;

    void refresh_buttons();

    bool is_currently_painting;
    //[/UserMethods]

    void paint(juce::Graphics &g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *sliderThatWasMoved) override;
    void buttonClicked(juce::Button *buttonThatWasClicked) override;

    // Binary resources:
    static const char *bg_editors_svg;
    static const int bg_editors_svgSize;

  private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Component> drawing_area;
    std::unique_ptr<juce::Slider> sl_show_range;
    std::unique_ptr<juce::TextButton> osc_1;
    std::unique_ptr<juce::TextButton> osc_2;
    std::unique_ptr<juce::TextButton> osc_3;
    std::unique_ptr<juce::TextButton> eq;
    std::unique_ptr<juce::TextButton> out;
    std::unique_ptr<juce::TextButton> f_1;
    std::unique_ptr<juce::TextButton> f_2;
    std::unique_ptr<juce::TextButton> f_3;
    std::unique_ptr<juce::TextButton> f_env_1;
    std::unique_ptr<juce::TextButton> f_env_2;
    std::unique_ptr<juce::TextButton> f_env_3;
    std::unique_ptr<juce::TextButton> out_env;
    juce::Path internalPath1;
    juce::Path internalPath2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_AmpPainter)
};

//[EndFile] You can add extra defines here...
inline void Monique_Ui_AmpPainter::calc_new_cycle() noexcept
{
    current_buffer_start_pos = master_osc_values->get_reader_start();
}

inline void Monique_Ui_AmpPainter::add_filter_env(int id_, const float *values_,
                                                  int num_samples_) noexcept
{
    // if( id_ == 0 && synth_data->osci_show_flt_env_1 || id_ == 1 &&
    // synth_data->osci_show_flt_env_2 || id_ == 2 && synth_data->osci_show_flt_env_3 )
    {
        EndlessBuffer *const values = filter_env_values.getUnchecked(id_);
        values->set_reader_start(current_buffer_start_pos);
        values->write(values_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_filter(int id_, const float *values_l_,
                                              const float *values_r_, int num_samples_) noexcept
{
    // if( id_ == 0 && synth_data->osci_show_flt_1 || id_ == 1 && synth_data->osci_show_flt_2 ||
    // id_ == 2 && synth_data->osci_show_flt_3 )
    {
        EndlessBuffer *const values = filter_values.getUnchecked(id_);
        values->set_reader_start(current_buffer_start_pos);
        values->write(values_l_, values_r_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_eq(const float *values_, int num_samples_) noexcept
{
    // if( synth_data->osci_show_eq )
    {
        eq_values->set_reader_start(current_buffer_start_pos);
        eq_values->write(values_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_out_env(const float *values_, int num_samples_) noexcept
{
    // if( synth_data->osci_show_out_env )
    {
        values_env->set_reader_start(current_buffer_start_pos);
        values_env->write(values_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_out(const float *values_l_, const float *values_r_,
                                           int num_samples_) noexcept
{
    // if( synth_data->osci_show_out )
    {
        values->set_reader_start(current_buffer_start_pos);
        values->write(values_l_, values_r_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_master_osc(const float *values_,
                                                  const float *is_switch_values,
                                                  int num_samples_) noexcept
{
    // if( id_ == 0 || id_ == 1 && synth_data->osci_show_osc_2 || id_ == 2 &&
    // synth_data->osci_show_osc_3 )
    {
        master_osc_values->write(values_, is_switch_values, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::add_osc(int id_, const float *values_, int num_samples_) noexcept
{
    // if( id_ == 0 || id_ == 1 && synth_data->osci_show_osc_2 || id_ == 2 &&
    // synth_data->osci_show_osc_3 )
    {
        EndlessBuffer *const osc_values_ = osc_values.getUnchecked(id_ - 1);
        osc_values_->set_reader_start(current_buffer_start_pos);
        osc_values_->write(values_, num_samples_);
    }
}
inline void Monique_Ui_AmpPainter::clear_and_keep_minimum() noexcept
{
    values->clear();
    values_env->clear();
    eq_values->clear();
    filter_values.getUnchecked(0)->clear();
    filter_values.getUnchecked(1)->clear();
    filter_values.getUnchecked(2)->clear();
    filter_env_values.getUnchecked(0)->clear();
    filter_env_values.getUnchecked(1)->clear();
    filter_env_values.getUnchecked(2)->clear();
}
//[/EndFile]

#endif // __JUCE_HEADER_15EBFFC85DA080CA__
