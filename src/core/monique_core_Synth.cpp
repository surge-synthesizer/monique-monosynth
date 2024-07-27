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

#include "monique_core_Synth.h"

#include "monique_core_Datastructures.h"
#include "monique_core_Processor.h"
#include "ui/monique_ui_AmpPainter.h"
#include "ui/monique_ui_SegmentedMeter.h"

#include <memory>

//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT WITH SIMDRegister or AudioBuffer and Function
static inline float sample_mix(float s1_, float s2_) noexcept
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
        s1_ += s2_;
    }

    return s1_;
}

//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT WITH TABLE LOCKUP
#define left_pan(pan_, sin_lookup_)                                                                \
    juce::jmax((float)std::sin(((pan_ + 1) * 0.5) * (juce::MathConstants<float>::pi * 0.5f)),      \
               0.00001f)
// TOOPT WITH TABLE LOCKUP
#define right_pan(pan_, cos_lookup_)                                                               \
    juce::jmax((float)std::cos(((pan_ + 1) * 0.5) * (juce::MathConstants<float>::pi * 0.5f)),      \
               0.00001f)

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
COLD SmoothedParameter::SmoothedParameter(SmoothManager *const smooth_manager_,
                                          Parameter *const param_to_smooth_) noexcept
    : RuntimeListener(smooth_manager_ ? smooth_manager_->notifyer : nullptr),
      smooth_manager(smooth_manager_),

      values(block_size), modulation_power(block_size),

      param_to_smooth(param_to_smooth_),

      max_value(param_to_smooth_->get_info().max_value),
      min_value(param_to_smooth_->get_info().min_value),

      simple_smoother(0.001), left_morph_smoother(0.001), right_morph_smoother(0.001),
      left_modulation_morph_smoother(0.001), right_modulation_morph_smoother(0.001),
      morph_power_smoother(0.001), modulation_power_smoother(0.001), amp_power_smoother(0.001)
{
    if (smooth_manager)
    {
        smooth_manager->smoothers.add(this);
        param_to_smooth_->get_runtime_info().my_smoother = this;
    }

    sample_rate_or_block_changed();
}
COLD SmoothedParameter::~SmoothedParameter() noexcept
{
    if (smooth_manager)
    {
        // param_to_smooth->get_runtime_info().my_smoother = nullptr;
        smooth_manager->smoothers.removeFirstMatchingValue(this);
    }
}
COLD void SmoothedParameter::set_offline() noexcept
{
    if (smooth_manager)
    {
        smooth_manager->smoothers.removeFirstMatchingValue(this);
    }
}

//==============================================================================
COLD void SmoothedParameter::sample_rate_or_block_changed() noexcept
{
    values.setSize(block_size);
    modulation_power.setSize(block_size);

    simple_smoother.set_value(morph_power_smoother.get_last_value());
    simple_smoother.reset_coefficients(sample_rate, 0);

    left_morph_smoother.set_value(morph_power_smoother.get_last_value());
    left_morph_smoother.reset_coefficients(sample_rate, 0);

    right_morph_smoother.set_value(morph_power_smoother.get_last_value());
    right_morph_smoother.reset_coefficients(sample_rate, 0);

    left_modulation_morph_smoother.set_value(morph_power_smoother.get_last_value());
    left_modulation_morph_smoother.reset_coefficients(sample_rate, 0);

    right_modulation_morph_smoother.set_value(morph_power_smoother.get_last_value());
    right_modulation_morph_smoother.reset_coefficients(sample_rate, 0);

    morph_power_smoother.set_value(morph_power_smoother.get_last_value());
    morph_power_smoother.reset_coefficients(sample_rate, 0);

    modulation_power_smoother.set_value(morph_power_smoother.get_last_value());
    modulation_power_smoother.reset_coefficients(sample_rate, 0);

    amp_power_smoother.set_value(morph_power_smoother.get_last_value());
    amp_power_smoother.reset_coefficients(sample_rate, 0);
}

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
class Smoother : RuntimeListener
{
    int pos;
    float sum;
    int size_in_ms;
    int size;
    mono_AudioSampleBuffer<1> buffer;
    float *ptr_to_buffer;

  public:
    //==========================================================================
    inline void add(float val_) noexcept
    {
        sum -= ptr_to_buffer[pos];
        ptr_to_buffer[pos] = val_;
        sum += val_;

        if (++pos >= size)
        {
            pos = 0;
        }
    }
    inline float add_and_get_average(float val_) noexcept
    {
        add(val_);
        return get_average();
    }
    inline float get_average() const noexcept { return sum * (1.0f / size); }
    inline void reset(float value_) noexcept
    {
        sum = 0;
        for (int i = 0; i != size; ++i)
        {
            ptr_to_buffer[i] = value_;
            sum += value_;
        }
    }
    void set_size_in_ms(int size_in_ms_, bool force_ = false) noexcept
    {
        if (size_in_ms != size_in_ms_ || force_)
        {
            float last_average = get_average();

            size_in_ms = size_in_ms_;
            size = juce::jmax(3, msToSamplesFast(size_in_ms_, sample_rate));
            buffer.setSize(size, false);
            ptr_to_buffer = buffer.getWritePointer();

            if (pos > size)
            {
                pos = 0;
            }

            sum = 0;
            for (int i = 0; i != size; ++i)
            {
                ptr_to_buffer[i] = last_average;
                sum += last_average;
            }
        }
    }

  private:
    virtual void sample_rate_or_block_changed() noexcept override
    {
        set_size_in_ms(size_in_ms, true);
    };

  public:
    //==========================================================================
    COLD Smoother(RuntimeNotifyer *const notifyer_, int init_size_in_ms_) noexcept
        : RuntimeListener(notifyer_), pos(0), sum(0), size_in_ms(init_size_in_ms_),
          size(juce::jmax(3, msToSamplesFast(init_size_in_ms_, sample_rate))), buffer(size),
          ptr_to_buffer(buffer.getWritePointer())
    {
        reset(0);
    }
    COLD ~Smoother() {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Smoother)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT with AudioBuffer and Function
// atan lookup???
static inline float soft_clipping(float input_and_worker_) noexcept
{
    return (std::atan(input_and_worker_) * (1.0f / juce::MathConstants<float>::pi)) * 1.5;
}

//==============================================================================
// TOOPT with AudioBuffer and Function
static inline float lfo2amp(float sample_) noexcept { return (sample_ + 1.0f) * 0.5f; }

//==============================================================================
static inline float distortion(float input_and_worker_, float distortion_power_) noexcept
{
    if (distortion_power_ != 0)
    {
        const float distortion_add_on = distortion_power_ * 0.9f;
        input_and_worker_ =
            ((1.0f + distortion_add_on) * input_and_worker_) -
            (input_and_worker_ * input_and_worker_ * input_and_worker_) * distortion_add_on;
    }

    return input_and_worker_;
}

//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT with AudioBuffer and Function
static inline float clipp_to_0_and_1(float input_and_worker_) noexcept
{
    if (input_and_worker_ > 1)
    {
        input_and_worker_ = 1;
    }
    else if (input_and_worker_ < 0)
    {
        input_and_worker_ = 0;
    }

    return input_and_worker_;
}

//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT with AudioBuffer and Function
static inline float soft_clipp_greater_1_2(float x) noexcept
{
    if (x > 1)
    {
        x = 1.0f + soft_clipping(x - 1.0f);
    }
    else if (x < -1)
    {
        x = -1.0f + soft_clipping(x + 1.0f);
    }

    if (x > 1.2)
    {
        x = 1.2;
    }
    else if (x < -1.2)
    {
        x = -1.2;
    }

    return x;
}
//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT with AudioBuffer and Function
static inline float soft_clipp_greater_0_9(float x) noexcept
{
    if (x > 0.9f)
    {
        x = 0.9f + soft_clipping(x - 0.9f);
    }
    else if (x < -0.9f)
    {
        x = -0.9f + soft_clipping(x + 0.9f);
    }

    if (x > 1)
    {
        x = 1;
    }
    else if (x < -1)
    {
        x = -1;
    }

    return x;
}

//==============================================================================
//==============================================================================
//==============================================================================
// TOOPT with AudioBuffer and Function
static inline float hard_clipper_1(float x) noexcept
{
    if (x > 1)
    {
        x = 1;
    }
    else if (x < -1)
    {
        x = -1;
    }

    return x;
}

//==============================================================================
//==============================================================================
//==============================================================================
// replace with juce table lookup
static float inline lookup(const float *table_, float x) noexcept
{
    return table_[int(x * TABLESIZE_MULTI) % LOOKUP_TABLE_SIZE];
}
//==============================================================================
COLD DataBuffer::DataBuffer(int init_buffer_size_) noexcept
    : size(init_buffer_size_),

      band_env_buffers(init_buffer_size_), band_out_buffers(init_buffer_size_),

      lfo_amplitudes(init_buffer_size_), mfo_amplitudes(init_buffer_size_),
      filter_output_samples_l_r(init_buffer_size_), filter_stereo_output_samples(init_buffer_size_),

      osc_samples(init_buffer_size_), osc_switchs(init_buffer_size_),
      osc_sync_switchs(init_buffer_size_), modulator_samples(init_buffer_size_),

      final_env(init_buffer_size_),
#ifdef POLY
      filter_env_tracking(init_buffer_size_),
#endif
      chorus_env(init_buffer_size_),

      filter_input_samples(init_buffer_size_), filter_input_env_amps(init_buffer_size_),
      filter_output_samples(init_buffer_size_), filter_env_amps(init_buffer_size_),

      tmp_buffer(init_buffer_size_),

      second_mono_buffer(init_buffer_size_),

      velocity_buffer(init_buffer_size_)
{
}
COLD DataBuffer::~DataBuffer() noexcept {}

//==============================================================================
COLD void DataBuffer::resize_buffer_if_required(int size_) noexcept
{
    if (size_ != size)
    {
        size = size_;

        band_env_buffers.setSize(size_);
        band_out_buffers.setSize(size_);

        lfo_amplitudes.setSize(size_);
        mfo_amplitudes.setSize(size_);
        filter_stereo_output_samples.setSize(size_);
        filter_output_samples_l_r.setSize(size_);

        osc_samples.setSize(size_);
        osc_switchs.setSize(size_);
        osc_sync_switchs.setSize(size_);
        modulator_samples.setSize(size_);

        final_env.setSize(size_);
#ifdef POLY
        filter_env_tracking.setSize(size_);
#endif
        chorus_env.setSize(size_);

        filter_input_samples.setSize(size_);
        filter_input_env_amps.setSize(size_);
        filter_output_samples.setSize(size_);
        filter_env_amps.setSize(size_);

        tmp_buffer.setSize(size_);

        second_mono_buffer.setSize(size_);

        velocity_buffer.setSize(size_);
    }
}

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
class PerfectCycleCounter : RuntimeListener
{
    //==========================================================================
    double frequency;

    double delta;
    double angle;

    double rate;
    double phase;

    double cylces_per_sec;

    bool new_cycle;

  public:
    //==========================================================================
    inline void set_frequency(double frequency_) noexcept
    {
        if (frequency != frequency_)
        {
            frequency = frequency_;
            cylces_per_sec = sample_rate / frequency_;

            delta = frequency_ / sample_rate *
                    (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
            rate = (1.0 / cylces_per_sec) * juce::MathConstants<double>::pi;
        }
    }
    inline void tick() noexcept
    {
        new_cycle = false;

        angle += delta;
        phase += rate;
        if (angle > (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi))
        {
            angle -= (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
            phase -= (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
            new_cycle = true;
        }
    }
    inline double get_last_angle() const noexcept { return angle; }
    inline double get_last_phase() const noexcept { return phase; }

    inline double get_cylces_per_sec() const noexcept { return cylces_per_sec; }
    inline bool is_new_cycle() const noexcept { return new_cycle; }
    inline bool is_clean() const noexcept { return angle == 0 && new_cycle && phase == 0; }

    //==========================================================================
    void reset() noexcept
    {
        angle = 0;
        new_cycle = true;
        phase = 0;
    }

    //==========================================================================
    void sample_rate_or_block_changed() noexcept override
    {
        float frequency_old = frequency;
        frequency = 0;
        set_frequency(frequency_old);
    }

  public:
    //==========================================================================
    COLD PerfectCycleCounter(RuntimeNotifyer *const notifyer_) noexcept
        : RuntimeListener(notifyer_), frequency(0), delta(0), angle(0), rate(0), phase(0),
          cylces_per_sec(0), new_cycle(true)
    {
    }
    COLD ~PerfectCycleCounter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerfectCycleCounter)
};

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
class mono_BlitSaw
{
    float phase_offset;
    double last_tick_value;
    double p_;
    double C2_;
    double phase_correction;
    double a_;
    double state_;

    unsigned int m_;

  public:
    //==========================================================================
    inline float tick(double phase_) noexcept
    {
        double tmp;
        {
            const double working_phase_ = (phase_ + phase_offset) - phase_correction;

            // Avoid a divide by zero, or use of a denormalized divisor
            // at the sinc peak, which has a limiting value of m_ / p_.
            const double denominator = std::sin(working_phase_);
            if (std::fabs(denominator) <= std::numeric_limits<double>::epsilon())
            {
                tmp = a_;
            }
            else
            {
                tmp = sin(working_phase_ * m_) / (p_ * denominator);
            }
        }

        tmp += (state_ - C2_);
        state_ = tmp * 0.995;

        return last_tick_value = tmp;
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_phase_offset(float offset_) noexcept
    {
        phase_offset = offset_ * juce::MathConstants<float>::pi;
    }
    inline void updateHarmonics(double p) noexcept
    {
        p_ = p;
        C2_ = 1.0 / p;
        phase_correction = C2_ * (juce::MathConstants<double>::pi * 0.5) / 0.9;
        m_ = 2 * floor(0.5 * p) + 1;
        a_ = p / p;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        state_ = 0;
        last_tick_value = 0;
    }

  public:
    //==========================================================================
    COLD mono_BlitSaw() noexcept
        : phase_offset(0), last_tick_value(0), p_(0), C2_(0), phase_correction(0), a_(0), state_(0),
          m_(0)
    {
    }
    COLD ~mono_BlitSaw() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_BlitSaw)
};

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
//==============================================================================
class mono_BlitSquare
{
    float last_tick_value;
    float phase_offset;
    double a_;
    double p_;
    double phase_correction;
    double lastBlitOutput_;
    double dcbState_;

    unsigned int m_;

  public:
    //==========================================================================
    inline float tick(double phase_) noexcept
    {
        // OUTPUT PROCESS
        float out;
        {
            const double temp = lastBlitOutput_;
            {
                const double current_phase = (phase_ + phase_offset) - phase_correction;

                // Avoid a divide by zero, or use of a denomralized divisor
                // at the sinc peak, which has a limiting value of 1.0.
                {

                    const double denominator = std::sin(current_phase);
                    if (std::fabs(denominator) < std::numeric_limits<double>::epsilon())
                    {
                        // Inexact comparison safely distinguishes between *close to zero*, and
                        // *close to PI*.
                        if (current_phase < 0.0001 ||
                            current_phase > (juce::MathConstants<double>::pi +
                                             juce::MathConstants<double>::pi) -
                                                0.0001)
                        {
                            lastBlitOutput_ = a_;
                        }
                        else
                        {
                            lastBlitOutput_ = -a_;
                        }
                    }
                    else
                    {
                        lastBlitOutput_ = std::sin(current_phase * m_) / (p_ * denominator);
                    }
                }
            }

            lastBlitOutput_ += temp;

            // Now apply DC blocker.
            out = lastBlitOutput_ - dcbState_ + 0.999f * last_tick_value;
            dcbState_ = lastBlitOutput_;
        }

        return last_tick_value = out;
    }
    inline float lastOut(void) const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_phase_offset(float offset_) noexcept
    {
        phase_offset = offset_ * juce::MathConstants<float>::pi;
    }
    inline void updateHarmonics(double p) noexcept
    {
        p_ = p * 0.5;
        m_ = (floor(0.5 * p_) + 1) * 2;
        a_ = p / p;

        phase_correction = (1.0 / p) * juce::MathConstants<double>::pi;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        last_tick_value = 0;
        dcbState_ = 0;
        lastBlitOutput_ = 0;
    }

  public:
    //==========================================================================
    COLD mono_BlitSquare() noexcept
        : last_tick_value(0), phase_offset(0), a_(0), p_(0), phase_correction(0),
          lastBlitOutput_(0), dcbState_(0), m_(0)
    {
    }
    COLD ~mono_BlitSquare() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_BlitSquare)
};

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
class mono_SineWave
{
    const float *const sine_lookup;

    float last_tick_value;
    float phase_offset;

  public:
    //==========================================================================
    inline float tick(double angle_) noexcept
    {
        return last_tick_value = lookup(sine_lookup, angle_ + phase_offset);
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_phase_offset(float offset_) noexcept
    {
        phase_offset = offset_ * juce::MathConstants<float>::pi;
    }

  public:
    //==========================================================================
    COLD mono_SineWave(const float *sine_lookup_) noexcept
        : sine_lookup(sine_lookup_), last_tick_value(0), phase_offset(0)
    {
    }
    COLD ~mono_SineWave() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_SineWave)
};

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
class mono_SineWaveAutonom : public RuntimeListener
{
    const float *const sine_lookup;

    double frequency;

    double delta;
    double angle;

    bool new_cycle;

    float last_tick_value;

  public:
    //==========================================================================
    inline float tick() noexcept
    {
        new_cycle = false;

        angle += delta;
        if (angle > (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi))
        {
            angle -= (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
            new_cycle = true;
        }

        return last_tick_value = lookup(sine_lookup, angle);
    }
    inline float lastOut_with_phase_offset(float offset_) noexcept
    {
        return lookup(sine_lookup, angle + offset_ * (juce::MathConstants<double>::pi +
                                                      juce::MathConstants<double>::pi));
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_frequency(double frequency_) noexcept
    {
        if (frequency != frequency_)
        {
            frequency = frequency_;
            delta = frequency_ / sample_rate *
                    (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
        }
    }
    inline void overwrite_angle(float angle_) noexcept { angle = angle_; }

    //==========================================================================
    inline bool is_new_cycle() const noexcept { return new_cycle; }

    inline void sample_rate_or_block_changed() noexcept override
    {
        float frequency_old = frequency;
        frequency = 0;
        set_frequency(frequency_old);
    }

  public:
    //==========================================================================
    COLD mono_SineWaveAutonom(RuntimeNotifyer *const notifyer_,
                              const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          sine_lookup(sine_lookup_), frequency(0),

          delta(0), angle(0),

          new_cycle(0),

          last_tick_value(0)
    {
    }
    COLD ~mono_SineWaveAutonom() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_SineWaveAutonom)
};

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
class mono_SineWaveAutonomShifted : public RuntimeListener
{
    const float *const sine_lookup;

    double frequency;

    double delta;
    double angle;

    bool new_cycle;

    float last_tick_value;

  public:
    //==========================================================================
    inline float tick() noexcept
    {
        new_cycle = false;

        angle += delta;
        if (angle - (juce::MathConstants<double>::pi * 0.5) >
            (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi))
        {
            angle -= (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
            new_cycle = true;
        }

        return last_tick_value = (lookup(sine_lookup, angle) * -1 + 1) * 0.5;
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_frequency(double frequency_) noexcept
    {
        if (frequency != frequency_)
        {
            frequency = frequency_;
            delta = frequency_ / sample_rate *
                    (juce::MathConstants<double>::pi + juce::MathConstants<double>::pi);
        }
    }

    //==========================================================================
    inline bool is_new_cycle() const noexcept { return new_cycle; }

    //==========================================================================
    inline void sample_rate_or_block_changed() noexcept override
    {
        float frequency_old = frequency;
        frequency = 0;
        set_frequency(frequency_old);
    }

  public:
    //==========================================================================
    COLD mono_SineWaveAutonomShifted(RuntimeNotifyer *const notifyer_,
                                     const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          sine_lookup(sine_lookup_), frequency(0),

          delta(0), angle(juce::MathConstants<double>::pi * 0.5),

          new_cycle(0),

          last_tick_value(0)
    {
    }
    COLD ~mono_SineWaveAutonomShifted() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_SineWaveAutonomShifted)
};

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
class mono_Noise
{
    juce::Random random;
    float last_tick_value;

  public:
    //==========================================================================
    inline float tick() noexcept { return last_tick_value = random.nextFloat() * 2 - 1; }
    inline float lastOut() const noexcept { return last_tick_value; }

  public:
    //==========================================================================
    COLD mono_Noise() noexcept : last_tick_value(0) {}
    COLD ~mono_Noise() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_Noise)
};

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
class mono_OnePole
{
    float last_tick_value;
    float gain;
    float a1, b;

  public:
    //==========================================================================
    inline float tick(float input) noexcept
    {
        return last_tick_value = b * gain * input - a1 * last_tick_value;
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void setPole(float thePole) noexcept
    {
        if (thePole > 0)
        {
            b = 1.0f - thePole;
        }
        else
        {
            b = 1.0f + thePole;
        }

        a1 = -thePole;
    }
    inline void setGain(float gain_) noexcept { gain = gain_; }

  public:
    //==========================================================================
    COLD mono_OnePole(float thePole = 0.9) noexcept : last_tick_value(0), gain(0), a1(0), b(0)
    {
        this->setPole(thePole);
    }
    COLD ~mono_OnePole() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_OnePole)
};

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
class mono_Modulate : public RuntimeListener
{
    const float *const sine_lookup;

    mono_SineWaveAutonomShifted vibrato;
    mono_SineWaveAutonom swing;
    mono_OnePole filter;
    mono_Noise noise;

    float last_tick_value;
    float last_swing_frequency;
    unsigned int noiseRate;
    unsigned int noiseCounter;

  public:
    //==========================================================================
    inline float tick() noexcept
    {
        // Compute periodic and random modulations.
        last_tick_value = vibrato.tick();
        if (++noiseCounter > noiseRate)
        {
            noise.tick();
            noiseCounter = 0;
        }
        last_tick_value += filter.tick(noise.lastOut());

        return last_tick_value * swing.tick();
    }
    inline float lastOut() const noexcept { return last_tick_value; }

    //==========================================================================
    inline void set_vibrato_frequency(float frequency_) noexcept
    {
        vibrato.set_frequency(frequency_);
    }
    inline void set_swing_frequency(float frequency_) noexcept
    {
        if (last_swing_frequency != frequency_)
        {
            if (frequency_ == 0)
            {
                swing.overwrite_angle(juce::MathConstants<float>::pi * 0.5);
            }

            last_swing_frequency = frequency_;
            swing.set_frequency(frequency_);
        }
    }
    inline bool is_next_a_new_cycle() const noexcept { return vibrato.is_new_cycle(); }

    //==========================================================================
    inline void reset() noexcept { last_tick_value = 0; }

    void sample_rate_or_block_changed() noexcept override
    {
        noiseRate = (unsigned int)(330.0 * sample_rate / 22050.0);
    }

  public:
    //==========================================================================
    COLD mono_Modulate(RuntimeNotifyer *const notifyer_, const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          sine_lookup(sine_lookup_),

          vibrato(notifyer_, sine_lookup_), swing(notifyer_, sine_lookup_),

          filter(), noise(),

          last_tick_value(0), last_swing_frequency(0), noiseRate(0), noiseCounter(0)
    {
        vibrato.set_frequency(6.0);
        swing.set_frequency(0);
        swing.overwrite_angle(juce::MathConstants<float>::pi * 0.5);

        noiseRate = (unsigned int)(330.0 * sample_rate / 22050.0);
        noiseCounter = noiseRate;

        filter.setPole(0.999);
        filter.setGain(0.05);
    }
    COLD ~mono_Modulate() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_Modulate)
};

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
class LFO : public RuntimeListener
{
    const float *const sine_lookup;

    float delta;
    float angle;
    int last_samples_per_clock;

    int last_speed;
    float last_out;

    float glide_samples;
    int glide_counter;
    float glide_value;

    DataBuffer *const data_buffer;
    const LFOData *const lfo_data;
    const RuntimeInfo *const runtime_info;

    //==============================================================================
    inline void calculate_delta(const int samples_per_clock_, const float speed_multi_,
                                const std::int64_t sync_sample_pos_) noexcept
    {
        if (last_samples_per_clock != samples_per_clock_)
        {
            {
                const float samples_per_bar = samples_per_clock_ * 96;
                const float samples_per_cylce = samples_per_bar * speed_multi_;
                delta = (1.0f / samples_per_cylce);
            }

            last_samples_per_clock = samples_per_clock_;
        }
    }

  public:
    inline void process(float *dest_, int step_number_, int absoloute_step_number_,
                        int start_pos_in_buffer_, int num_samples_, bool use_process_sample = true,
                        juce::Array<RuntimeInfo::standalone_features::ClockSync::SyncPosPair>
                            *clock_infos_ = nullptr) noexcept
    {
        // USER SPEED
        const int speed(lfo_data->speed);
        const float speed_multi(get_lfo_speed_multi(speed));
        if (speed != last_speed)
        {
            glide_samples = glide_counter = sample_rate / 1000;
            glide_value = last_out;
            last_speed = speed;
        }

        // TO MIDI CLOCK SYNC
        std::int64_t sync_sample_pos =
            (runtime_info->relative_samples_since_start + start_pos_in_buffer_);
        if (!use_process_sample)
        {
            sync_sample_pos = start_pos_in_buffer_;
        }

        bool same_samples_per_block_for_buffer = true;
        if (is_standalone() && runtime_info->standalone_features_pimpl->is_extern_synced)
        {
            auto &runtime_info_standalone_features = *runtime_info->standalone_features_pimpl;

            juce::Array<RuntimeInfo::standalone_features::ClockSync::SyncPosPair>
                clock_informations =
                    clock_infos_ ? *clock_infos_
                                 : runtime_info_standalone_features.clock_sync_information
                                       .get_a_working_copy();

            if (!runtime_info_standalone_features.clock_sync_information.has_clocks_inside())
            {
                same_samples_per_block_for_buffer = true;
                calculate_delta(runtime_info_standalone_features.clock_sync_information
                                    .get_last_samples_per_clock(),
                                speed_multi, sync_sample_pos);
            }
            else
            {
                same_samples_per_block_for_buffer = false;
            }

            if (sync_sample_pos <= 0)
            {
                angle = 0;
            }

            // FORCE SYNC
            if (step_number_ != -1)
            {
                bool should_smooth = false;
                if (fmod(absoloute_step_number_, speed_multi * 16) == 0)
                {
                    should_smooth = angle != 0;
                    angle = 0;
                }

                if (should_smooth && glide_counter <= 0)
                {
                    glide_samples = glide_counter = juce::jmax(10.0, sample_rate / 1000);
                    glide_value = last_out;
                }
            }

            // PROCESS
            {
                const float *smoothed_wave_buffer(
                    lfo_data->wave_smoother.get_smoothed_value_buffer());
                const float *smoothed_offset_buffer(
                    lfo_data->phase_shift_smoother.get_smoothed_value_buffer());
                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    if (++sync_sample_pos < 0)
                    {
                        continue;
                    }

                    {
                        float amp;
                        {
                            // CURRENT ANGLE
                            {
                                if (!same_samples_per_block_for_buffer)
                                {
                                    calculate_delta(
                                        runtime_info_standalone_features.clock_sync_information
                                            .get_samples_per_clock(start_pos_in_buffer_ + sid,
                                                                   clock_informations),
                                        speed_multi, sync_sample_pos);
                                }
                                angle += delta;
                                angle = angle - floor(angle);
                            }

                            // AMP
                            {
                                const float sine_amp =
                                    lookup(sine_lookup, angle * juce::MathConstants<double>::twoPi +
                                                            smoothed_offset_buffer[sid] *
                                                                juce::MathConstants<double>::twoPi);
                                const float wave = smoothed_wave_buffer[sid];
                                amp = sine_amp * (1.0f - wave) +
                                      (std::atan(sine_amp * 250 * juce::jmax(speed_multi, 1.0f)) *
                                       (1.0f / 1.55)) *
                                          wave;
                                if (amp > 1)
                                {
                                    amp = 1;
                                }
                                else if (amp < -1)
                                {
                                    amp = -1;
                                }
                                amp = lfo2amp(amp);
                            }
                        }

                        if (--glide_counter > 0)
                        {
                            float glide = (1.0f / glide_samples * glide_counter);
                            amp = amp * (1.0f - glide) + glide_value * glide;
                        }

                        dest_[sid] = amp;
                    }
                }
                last_out = dest_[num_samples_ - 1];
            }
        }
        else
        // PERFECT SYNC
        {
            const float bars_per_sec = runtime_info->bpm / 4 / 60;
            const float samples_per_bar = (1.0f / bars_per_sec) * sample_rate;
            const float samples_per_cylce = samples_per_bar * speed_multi;
            const float cycles_per_sample = (1.0 / samples_per_cylce);

            // PROCESS
            {
                const float *smoothed_wave_buffer(
                    lfo_data->wave_smoother.get_smoothed_value_buffer());
                const float *smoothed_offset_buffer(
                    lfo_data->phase_shift_smoother.get_smoothed_value_buffer());
                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    float amp;
                    {
                        angle = cycles_per_sample * sync_sample_pos;
                        angle = angle - floor(angle);

                        // AMP
                        {
                            const float sine_amp =
                                lookup(sine_lookup, angle * juce::MathConstants<double>::twoPi +
                                                        smoothed_offset_buffer[sid] *
                                                            juce::MathConstants<double>::twoPi);
                            const float wave = smoothed_wave_buffer[sid];
                            amp = sine_amp * (1.0f - wave) +
                                  (std::atan(sine_amp * 250 * juce::jmax(speed_multi, 1.0f)) *
                                   (1.0f / 1.55)) *
                                      wave;
                            if (amp > 1)
                            {
                                amp = 1;
                            }
                            else if (amp < -1)
                            {
                                amp = -1;
                            }
                            amp = lfo2amp(amp);
                        }
                    }

                    if (glide_counter > 0 && false)
                    {
                        --glide_counter;
                        float glide = (1.0f / glide_samples * glide_counter);
                        amp = amp * (1.0f - glide) + glide_value * glide;
                    }

                    dest_[sid] = amp;

                    ++sync_sample_pos;
                }
                last_out = dest_[num_samples_ - 1];
            }
        }
    }

    void sample_rate_or_block_changed() noexcept override { last_speed = -1; }

    //==========================================================================
    float get_current_amp() const noexcept { return last_out; }

    //==========================================================================
    void reset() noexcept { last_out = 0; }

  public:
    //==========================================================================
    LFO(RuntimeNotifyer *const notifyer_, MoniqueSynthData *synth_data_, LFOData *lfo_data_,
        const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          sine_lookup(sine_lookup_),

          delta(0), angle(0), last_samples_per_clock(-1),

          last_speed(0), last_out(0),

          glide_samples(0), glide_counter(0), glide_value(0),

          data_buffer(synth_data_->data_buffer), lfo_data(lfo_data_),
          runtime_info(synth_data_->runtime_info)
    {
    }
    ~LFO() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)
};

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
class MasterOSC : public RuntimeListener
{
    // RUNTIME
    //==============================================================================
    int freq_glide_samples_left;
    float freq_glide_delta;

    float sync_glide_samples;
    int sync_glide_samples_left;
    float last_value;
    float last_modulator_value;
    float sync_value;
    float sync_modulator_value;

    float root_note;

    float last_root_note;
    float last_frequency;

    float last_modulator_frequency;
    int modulator_sync_cylces;
    int modulator_run_circle;
    bool modulator_waits_for_sync_cycle;

    // RAW OSCILATORS
    //==============================================================================
    PerfectCycleCounter cycle_counter;
    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;
    mono_Noise noise;

    mono_Modulate modulator;

    // DATA SOURCE
    //==============================================================================
    DataBuffer *const data_buffer;
    const MoniqueSynthData *const synth_data;
    FMOscData *const fm_osc_data;
    OSCData *const osc_data;

  public:
    //==============================================================================
    inline void process(DataBuffer *data_buffer_, const int num_samples_) noexcept
    {
        float *const output_buffer(data_buffer->osc_samples.getWritePointer(MASTER_OSC));

        float *const switch_buffer(data_buffer->osc_switchs.getWritePointer());
        juce::FloatVectorOperations::clear(switch_buffer, num_samples_);
        float *const modulator_buffer(data_buffer->modulator_samples.getWritePointer(MASTER_OSC));
        const float *const lfo_amps((data_buffer->lfo_amplitudes.getReadPointer(MASTER_OSC)));

        const bool is_lfo_modulated = osc_data->is_lfo_modulated;
        const bool fm_sync = fm_osc_data->sync;

        fm_osc_data->master_shift_smoother.process_modulation(is_lfo_modulated, lfo_amps,
                                                              num_samples_);

        // const float*const smoothed_tune_buffer(
        // osc_data->tune_smoother.get_smoothed_modulated_value_buffer() );
        const float *const smoothed_fm_amount_buffer(
            osc_data->fm_amount_smoother.get_smoothed_value_buffer());
        const float *const smoothed_wave_buffer(
            osc_data->wave_smoother.get_smoothed_value_buffer());
        const float *const smoothed_fm_freq_buffer(
            fm_osc_data->fm_freq_smoother.get_smoothed_value_buffer());
        const float *const smoothed_fm_swing_buffer(
            fm_osc_data->fm_swing_smoother.get_smoothed_value_buffer());
        const float *const smoothed_fm_phaser(
            fm_osc_data->fm_shape_smoother.get_smoothed_value_buffer());
        const float *const smoothed_phase_offset(
            fm_osc_data->master_shift_smoother.get_smoothed_value_buffer());

        for (int sid = 0; sid < num_samples_; ++sid)
        {
            // SETUP TUNE
            {
                // BASE FREQUENCY
                bool base_frequency_changed = false;
                if (freq_glide_samples_left > 0 || last_root_note != root_note)
                {
                    if (freq_glide_samples_left > 0)
                    {
                        --freq_glide_samples_left;
                    }

                    {
                        last_root_note = root_note;

                        const float new_frequence = juce::jmax(
                            5.0f, synth_data->tuning->midiNoteToFrequency(
                                      root_note + freq_glide_delta * freq_glide_samples_left));
                        if (new_frequence != last_frequency)
                        {
                            cycle_counter.set_frequency(new_frequence);
                            square_generator.updateHarmonics(cycle_counter.get_cylces_per_sec());
                            saw_generator.updateHarmonics(cycle_counter.get_cylces_per_sec());

                            last_frequency = new_frequence;
                        }
                    }

                    base_frequency_changed = true;
                }

                // MODULATOR FREQUENCY
                {
                    const float modulator_freq = smoothed_fm_freq_buffer[sid];
                    if (base_frequency_changed || modulator_freq != last_modulator_frequency)
                    {
                        modulator.set_vibrato_frequency(
                            last_frequency + last_frequency * (modulator_freq * 6 + 1.01));
                        modulator_sync_cylces = std::floor(modulator_freq * 6 + 1);

                        last_modulator_frequency = modulator_freq;
                    }
                }

                // ADD PHASE SHIFT
                square_generator.set_phase_offset(smoothed_phase_offset[sid]);
                saw_generator.set_phase_offset(smoothed_phase_offset[sid]);
                sine_generator.set_phase_offset(smoothed_phase_offset[sid]);
            }

            // PROCESS OSC ITSELF
            bool is_last_sample_of_cycle = false;
            float sample;
            {
                // TICK ALL OSCILATTORS
                {
                    cycle_counter.tick();
                    saw_generator.tick(cycle_counter.get_last_phase());
                    {
                        double angle = cycle_counter.get_last_angle();
                        square_generator.tick(angle);
                        sine_generator.tick(angle);
                    }

                    // FORCE SYNC OF THE SINE AT THE NEXT SID
                    if (cycle_counter.is_new_cycle())
                    {
                        // SET THE SWITCH
                        is_last_sample_of_cycle = true;
                        switch_buffer[sid] = true;
                    }
                }

                // CALC THE SAMPLE MIX
                {
                    const float wave_form = smoothed_wave_buffer[sid];

                    // SINE - SQUARE
                    if (wave_form <= 1)
                    {
                        const float multi = wave_form;
                        const float sine_wave_powerd = sine_generator.lastOut() * (1.0f - multi);
                        const float square_wave_powerd = square_generator.lastOut() * multi;
                        sample = sine_wave_powerd + square_wave_powerd;
                    }
                    // SQUARE - SAW
                    else if (wave_form <= 2)
                    {
                        const float multi = wave_form - 1;
                        const float square_wave_powerd =
                            square_generator.lastOut() * (1.0f - multi);
                        const float saw_wave_powerd = saw_generator.lastOut() * multi;
                        sample = square_wave_powerd + saw_wave_powerd;
                    }
                    // SAW - RAND
                    else /*if( wave_form <= 3 )*/
                    {
                        const float multi = wave_form - 2;
                        const float saw_wave_powerd = saw_generator.lastOut() * (1.0f - multi);
                        const float noice_powerd = noise.tick() * multi;
                        sample = saw_wave_powerd + noice_powerd;
                    }
                }
            }

            // PROCESS MODULATOR
            float modulator_sample = 0;
            {
                // MODULATOR SYNC AND PROCESSING
                if (!modulator_waits_for_sync_cycle)
                {
                    modulator_sample = modulator.tick();
                }

                const bool is_last_sample_of_modulator_cycle = modulator.is_next_a_new_cycle();

                // STOP THE MODULATOR IF IN SYNC // IGNORED IF SHOT IS ENABLED
                if (modulator_run_circle > modulator_sync_cylces && fm_sync)
                {
                    modulator_waits_for_sync_cycle = !is_last_sample_of_cycle;
                    modulator_run_circle = 0;
                }
                else if (is_last_sample_of_cycle)
                {
                    modulator_waits_for_sync_cycle = false;
                    modulator_run_circle = 0;
                }

                // COUNT CYCLES AND CLEAR STATE IF NOT WAITING FOR SYNC
                if (is_last_sample_of_modulator_cycle && !modulator_waits_for_sync_cycle)
                {
                    ++modulator_run_circle;
                }

                // UPDATE SWING
                if (is_last_sample_of_modulator_cycle)
                {
                    modulator.set_swing_frequency(smoothed_fm_swing_buffer[sid] * 5);
                }
            }

            // ADD FM TO THE OUTPUT
            {
                if (const float fm_amount = smoothed_fm_amount_buffer[sid])
                {
                    const float phase_move =
                        smoothed_fm_phaser[sid]; // * jmin(1.0f,smoothed_fm_swing_buffer[sid]*80);
                    sample = (1.0f - fm_amount) * sample +
                             fm_amount * ((1.0f - phase_move) * sample * modulator_sample +
                                          phase_move * (modulator_sample * modulator_sample *
                                                        modulator_sample * sample));
                }

                if (--sync_glide_samples_left > 0)
                {
                    const float power = 1.0f / sync_glide_samples * sync_glide_samples_left;
                    sample = sample * (1.0f - power) + sync_value * power;
                    modulator_sample =
                        modulator_sample * (1.0f - power) + sync_modulator_value * power;
                }

                output_buffer[sid] = sample;
                modulator_buffer[sid] = modulator_sample;
            }
        }

        last_value = output_buffer[num_samples_ - 1];
        last_modulator_value = modulator_buffer[num_samples_ - 1];
    }

    //==============================================================================
    inline void update(float root_note_, int sample_pos_) noexcept
    {
        root_note_ += synth_data->octave_offset * 12;
        const float glide = synth_data->glide_smoother.get_smoothed_value_buffer()[sample_pos_];
        if (glide != 0 && (root_note != root_note_ || freq_glide_samples_left > 0))
        {
            root_note_ = juce::jmax(1.0f, juce::jmin(127.0f, root_note_));

            const float rest_glide = freq_glide_delta * freq_glide_samples_left;
            freq_glide_samples_left = juce::jmax(10.0f, float(sample_rate * 0.5) * glide);
            const float glide_notes_diverence = root_note - root_note_;
            if (freq_glide_samples_left > 0)
            {
                freq_glide_delta = (glide_notes_diverence + rest_glide) / freq_glide_samples_left;
            }
            else
            {
                freq_glide_delta = 0;
            }
        }

        root_note = root_note_;
    }
    inline void reset() noexcept
    {
        cycle_counter.reset();
        saw_generator.reset();
        square_generator.reset();
        modulator.reset();

        freq_glide_delta = 0;
        freq_glide_samples_left = 0;

        sync_value = last_value;
        sync_modulator_value = last_modulator_value;
        sync_glide_samples = juce::jmax(10, msToSamplesFast(1, sample_rate));
        sync_glide_samples_left = sync_glide_samples;
    }

    void sample_rate_or_block_changed() noexcept override {}

  public:
    //==============================================================================
    COLD MasterOSC(RuntimeNotifyer *const notifyer_, const MoniqueSynthData *synth_data_,
                   const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          freq_glide_samples_left(0), freq_glide_delta(0),

          sync_glide_samples(0), sync_glide_samples_left(0), last_value(0), last_modulator_value(0),
          sync_value(0), sync_modulator_value(0),

          root_note(60),

          last_root_note(0), last_frequency(0),

          last_modulator_frequency(0), modulator_sync_cylces(0), modulator_run_circle(0),
          modulator_waits_for_sync_cycle(false),

          cycle_counter(notifyer_), saw_generator(), square_generator(),
          sine_generator(sine_lookup_), noise(),

          modulator(notifyer_, sine_lookup_),

          data_buffer(synth_data_->data_buffer), synth_data(synth_data_),
          fm_osc_data(synth_data_->fm_osc_data.get()), osc_data(synth_data_->osc_datas[MASTER_OSC])
    {
    }
    COLD ~MasterOSC() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterOSC)
};

//===================================================================================
//===================================================================================
//===================================================================================
class SecondOSC : public RuntimeListener
{
    const int id;

    // RUNTIME
    //==============================================================================
    int freq_glide_samples_left;
    float freq_glide_delta;

    float sync_glide_samples;
    int sync_glide_samples_left;
    float last_value;
    float sync_value;

    float root_note;
    float last_root_note;
    float last_tune;
    float last_frequency;

    bool wait_for_new_master_cycle;
    float last_sync_was_to_tune;

    // RAW OSCILATORS
    //==============================================================================
    PerfectCycleCounter cycle_counter;
    mono_BlitSaw saw_generator;
    mono_BlitSquare square_generator;
    mono_SineWave sine_generator;
    mono_Noise noise;

    // DATA SOURCE
    //==============================================================================
    DataBuffer *const data_buffer;
    const MoniqueSynthData *const synth_data;
    OSCData *const osc_data;
    OSCData *const master_osc_data;
    FMOscData *const fm_osc_data;

  public:
    //==============================================================================
    static inline bool is_syncanble_by_tune(float tune_) noexcept
    {
        bool syncable = false;
        if (tune_ == 0)
        {
            syncable = true;
        }
        else if (tune_ == 0 + 24)
        {
            syncable = true;
        }
        else if (tune_ == 0 - 24)
        {
            syncable = true;
        }

        return syncable;
    }

    //==============================================================================
    inline void process(DataBuffer *data_buffer_, const int num_samples_) noexcept
    {
        float *const output_buffer(data_buffer->osc_samples.getWritePointer(id));

        const float *const switch_buffer(data_buffer->osc_switchs.getWritePointer());
        const float *const modulator_buffer(
            data_buffer->modulator_samples.getWritePointer(MASTER_OSC));
        const float *const lfo_amps((data_buffer->lfo_amplitudes.getReadPointer(id)));

        const bool is_lfo_modulated = osc_data->is_lfo_modulated;

        const bool sync_to_master = osc_data->sync;
        osc_data->tune_smoother.process_modulation(is_lfo_modulated, lfo_amps, num_samples_);
        const float *const smoothed_tune_buffer(
            osc_data->tune_smoother.get_smoothed_value_buffer());
        const float *const smoothed_fm_amount_buffer(
            osc_data->fm_amount_smoother.get_smoothed_value_buffer());
        const float *const smoothed_wave_buffer(
            osc_data->wave_smoother.get_smoothed_value_buffer());
        // const float*const smoothed_fm_swing_buffer(
        // fm_osc_data->fm_swing_smoother.get_smoothed_modulated_value_buffer() );
        const float *const smoothed_fm_phaser(
            fm_osc_data->fm_shape_smoother.get_smoothed_value_buffer());
        for (int sid = 0; sid < num_samples_; ++sid)
        {
            // SETUP TUNE
            const float tune = smoothed_tune_buffer[sid];
            {
                // FREQUENCY
                if (freq_glide_samples_left > 0 || tune != last_tune || last_root_note != root_note)
                {
                    if (freq_glide_samples_left > 0)
                    {
                        --freq_glide_samples_left;
                    }

                    {
                        last_tune = tune;
                        last_root_note = root_note;

                        const float new_frequence =
                            juce::jmax(5.0f, synth_data->tuning->midiNoteToFrequency(
                                                 root_note + tune +
                                                 freq_glide_delta * freq_glide_samples_left));
                        if (new_frequence != last_frequency)
                        {
                            cycle_counter.set_frequency(new_frequence);
                            square_generator.updateHarmonics(cycle_counter.get_cylces_per_sec());
                            saw_generator.updateHarmonics(cycle_counter.get_cylces_per_sec());
                        }
                    }
                }
            }

            // PROCESS OSC AND SYNC IT TO THE MASTER
            const bool is_last_sample_of_master_cycle = switch_buffer[sid];
            float sample = 0;
            {
                // TICK ALL OSCILATTORS
                if (!wait_for_new_master_cycle)
                {
                    cycle_counter.tick();
                    saw_generator.tick(cycle_counter.get_last_phase());
                    {
                        double angle = cycle_counter.get_last_angle();
                        square_generator.tick(angle);
                        sine_generator.tick(angle);
                    }
                }

                // FORCE SYNC OF THE SINE AT THE NEXT SID
                if (cycle_counter.is_new_cycle())
                {
                    const bool syncanble_by_tune = is_syncanble_by_tune(tune);
                    if (!syncanble_by_tune)
                    {
                        last_sync_was_to_tune = -25;
                    }
                    if (sync_to_master)
                    {
                        if (!is_lfo_modulated && !syncanble_by_tune &&
                            last_sync_was_to_tune != tune)
                        {
                            wait_for_new_master_cycle = true;
                        }
                    }
                    else
                    {
                        if (!is_lfo_modulated && freq_glide_samples_left <= 0)
                        {
                            if (syncanble_by_tune && last_sync_was_to_tune != tune)
                            {
                                wait_for_new_master_cycle = true;
                                last_sync_was_to_tune = tune;
                            }
                        }
                    }
                }
                if (is_last_sample_of_master_cycle)
                {
                    wait_for_new_master_cycle = false;
                }

                // CALC THE SAMPLE MIX
                if (!wait_for_new_master_cycle)
                {
                    const float wave_form = smoothed_wave_buffer[sid];

                    // SINE - SQUARE
                    if (wave_form <= 1)
                    {
                        const float multi = wave_form;
                        const float sine_wave_powerd = sine_generator.lastOut() * (1.0f - multi);
                        const float square_wave_powerd = square_generator.lastOut() * multi;
                        sample = sine_wave_powerd + square_wave_powerd;
                    }
                    // SQUARE - SAW
                    else if (wave_form <= 2)
                    {
                        const float multi = wave_form - 1;
                        const float square_wave_powerd =
                            square_generator.lastOut() * (1.0f - multi);
                        const float saw_wave_powerd = saw_generator.lastOut() * multi;
                        sample = square_wave_powerd + saw_wave_powerd;
                    }
                    // SAW - RAND
                    else /*if( wave_form <= 3 )*/
                    {
                        const float multi = wave_form - 2;
                        const float saw_wave_powerd = saw_generator.lastOut() * (1.0f - multi);
                        const float noice_powerd = noise.tick() * multi;
                        sample = saw_wave_powerd + noice_powerd;
                    }
                }
            }

            // ADD FM TO THE OUTPUT
            {
                if (const float fm_amount = smoothed_fm_amount_buffer[sid])
                {
                    float modulator_sample = modulator_buffer[sid];
                    const float phase_move =
                        smoothed_fm_phaser[sid]; // * jmin(1.0f,smoothed_fm_swing_buffer[sid]*80);
                    sample = (1.0f - fm_amount) * sample +
                             fm_amount * ((1.0f - phase_move) * sample * modulator_sample +
                                          phase_move * (modulator_sample * modulator_sample *
                                                        modulator_sample * sample));
                }

                if (--sync_glide_samples_left > 0)
                {
                    const float power = 1.0f / sync_glide_samples * sync_glide_samples_left;
                    sample = sample * (1.0f - power) + sync_value * power;
                }

                output_buffer[sid] = sample;
            }
        }

        last_value = output_buffer[num_samples_ - 1];
    }

    //==============================================================================
    inline void update(float root_note_, int sample_pos_) noexcept
    {
        root_note_ += synth_data->octave_offset * 12;
        const float glide = synth_data->glide_smoother.get_smoothed_value_buffer()[sample_pos_];
        if (glide != 0 && (root_note != root_note_ || freq_glide_samples_left > 0))
        {
            root_note_ = juce::jmax(1.0f, juce::jmin(127.0f, root_note_));

            const float rest_glide = freq_glide_delta * freq_glide_samples_left;
            freq_glide_samples_left = (sample_rate * 0.5) * glide;
            const float glide_notes_diverence = root_note - root_note_;
            if (freq_glide_samples_left > 0)
            {
                freq_glide_delta = (glide_notes_diverence + rest_glide) / freq_glide_samples_left;
            }
            else
            {
                freq_glide_delta = 0;
            }
        }

        root_note = root_note_;
    }
    inline void reset() noexcept
    {
        cycle_counter.reset();
        saw_generator.reset();
        square_generator.reset();

        freq_glide_delta = 0;
        freq_glide_samples_left = 0;

        wait_for_new_master_cycle = false;

        sync_value = last_value;
        sync_glide_samples = juce::jmax(10, msToSamplesFast(1, sample_rate));
        sync_glide_samples_left = sync_glide_samples;
    }
    void sample_rate_or_block_changed() noexcept override {}

  public:
    //==============================================================================
    COLD SecondOSC(RuntimeNotifyer *const notifyer_, const MoniqueSynthData *synth_data_, int id_,
                   const float *const sine_lookup_) noexcept
        : RuntimeListener(notifyer_),

          id(id_),

          freq_glide_samples_left(0), freq_glide_delta(0),

          sync_glide_samples(0), sync_glide_samples_left(0), last_value(0), sync_value(0),

          root_note(60), last_root_note(0), last_tune(-25), last_frequency(0),

          wait_for_new_master_cycle(false), last_sync_was_to_tune(-25),

          cycle_counter(notifyer_), saw_generator(), square_generator(),
          sine_generator(sine_lookup_), noise(),

          data_buffer(synth_data_->data_buffer), synth_data(synth_data_),
          osc_data(synth_data_->osc_datas[id_]),
          master_osc_data(synth_data_->osc_datas[MASTER_OSC]),
          fm_osc_data(synth_data_->fm_osc_data.get())
    {
    }
    COLD ~SecondOSC() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SecondOSC)
};

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
class mono_ENVOsccilator : public RuntimeListener
{
    const float *const sine_lookup;
    const float *const cos_lookup;
    const float *const exp_lookup;

    float start_amp;
    float out_amp;
    float target_amp;
    enum TYPE
    {
        ATTACK,
        RELEASE,
        KEEP
    };
    TYPE type;

    float delta;
    float angle;
    float sine_angle_start;

    float shape;
    float time_in_samples;
    int sample_counter;
    bool is_unlimited;

  public:
    //==========================================================================
    // SHAPE 0 - sine-, 1 - linear, 2 - sine+
    inline void set_process_values(float start_value_, float end_value_, float shape_,
                                   float time_in_samples_) noexcept
    {
        start_amp = start_value_;
        target_amp = end_value_;
        shape = ((shape_ + 1) * 0.5f) * 0.9f + 0.1;
        time_in_samples = time_in_samples_;
    }
    inline void calculate_attack_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        angle = 0;
        sine_angle_start = juce::MathConstants<float>::pi * 1.5f;
        type = TYPE::ATTACK;
        is_unlimited = false;
        delta = juce::MathConstants<float>::pi / sample_counter;
    }
    inline void calculate_release_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        angle = 0;
        sine_angle_start = juce::MathConstants<float>::pi * 1.5f;
        type = TYPE::RELEASE;
        is_unlimited = false;
        delta = juce::MathConstants<float>::pi / sample_counter;
    }
    inline void calculate_keep_coeffecients() noexcept
    {
        sample_counter = time_in_samples;
        delta = 0;
        angle = 0;
        type = TYPE::KEEP;
        is_unlimited = false;
    }
    inline void calculate_unlimited_coeffecients() noexcept
    {
        sample_counter = 0;
        delta = 0;
        angle = 0;
        type = TYPE::KEEP;
        is_unlimited = true;
    }
    inline void only_calculate_new_target(float target_) noexcept
    {
        // start_amp = out_amp;
        target_amp = target_;
        // sample_counter = time_in_samples;
        // delta = (juce::MathConstants<float>::pi) / sample_counter;
        // angle = 0;
    }

    //==========================================================================
    void sample_rate_or_block_changed() noexcept override
    {
        // AUTO RESET ON NEXT CYCLE
    }

  public:
    //==========================================================================
    inline float tick() noexcept
    {
        if (--sample_counter > 0)
        {
            if (delta != 0)
            {
                angle += delta;
                /*shape*(angle-cos_for_angle)*/
                const float angle_drift =
                    lookup(exp_lookup, angle) * angle * shape +
                    (angle + lookup(cos_lookup, angle + sine_angle_start)) * (1.0f - shape);
                if (type == TYPE::ATTACK)
                {
                    out_amp = start_amp +
                              ((lookup(sine_lookup, angle_drift + sine_angle_start) + 1) * 0.5f) *
                                  (target_amp - start_amp);
                }
                else if (type == TYPE::RELEASE)
                {
                    out_amp = start_amp -
                              ((lookup(sine_lookup, angle_drift + sine_angle_start) + 1) * 0.5f) *
                                  (start_amp - target_amp);
                }
            }
        }
        else
        {
            out_amp = target_amp;
        }

        return out_amp;
    }

  public:
    //==========================================================================
    inline float last_out() const noexcept { return out_amp; }
    inline void overwrite_current_value(float amp_) noexcept { out_amp = amp_; }

    //==========================================================================
    inline bool is_finished() const noexcept { return is_endless() && sample_counter <= 0; }
    inline bool is_endless() const noexcept { return !is_unlimited; }

    //==========================================================================
    inline void reset() noexcept
    {
        sample_counter = 0;
        start_amp = 0;
        out_amp = 0;
        type = KEEP;
        target_amp = 0;
        delta = 0;
        angle = 0;
        time_in_samples = 0;
        sample_counter = 0;
        is_unlimited = false;
    }

  public:
    //==========================================================================
    COLD mono_ENVOsccilator(RuntimeNotifyer *const notifyer_, const float *const sine_lookup_,
                            const float *const cos_lookup_, const float *const exp_lookup_) noexcept

        : RuntimeListener(notifyer_), sine_lookup(sine_lookup_), cos_lookup(cos_lookup_),
          exp_lookup(exp_lookup_),

          start_amp(0), out_amp(0), target_amp(0), type(KEEP),

          delta(0), angle(0), sine_angle_start(0),

          shape(0),

          time_in_samples(0), sample_counter(0), is_unlimited(false)
    {
    }
    COLD ~mono_ENVOsccilator() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_ENVOsccilator)
};

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
enum STAGES
{
    END_ENV = false,
    ATTACK = true,
    DECAY,
    SUSTAIN,
    RELEASE,
    TRIGGER_START
};
class ENV : RuntimeListener
{
    mono_ENVOsccilator env_osc;

    STAGES current_stage;

    const MoniqueSynthData *const synth_data;
    float last_sustain;
    bool goes_to_sustain;
    bool is_sustain;

  public:
    ENVData *const env_data;

  public:
    //==========================================================================
    inline void process(float *dest_, const int num_samples_) noexcept
    {
        const float *smoothed_sustain_buffer =
            env_data->sustain_smoother.get_smoothed_value_buffer();
        for (int sid = 0; sid < num_samples_; ++sid)
        {
            const float sustain = smoothed_sustain_buffer[sid];
            if (last_sustain != sustain)
            {
                last_sustain = sustain;
                if (goes_to_sustain)
                {
                    env_osc.only_calculate_new_target(last_sustain);
                }
                else if (is_sustain)
                {
                    env_osc.overwrite_current_value(last_sustain);
                }
            }
            dest_[sid] = env_osc.tick();

            if (env_osc.is_finished())
            {
                update_stage(sid);
            }
        }
    }

  private:
    inline void update_stage(int sid_, bool force_relese_ = false) noexcept
    {
        const float *smoothed_sustain_buffer =
            env_data->sustain_smoother.get_smoothed_value_buffer();
        const float *smoothed_shape_buffer = env_data->shape_smoother.get_smoothed_value_buffer();
        switch (current_stage)
        {
        case TRIGGER_START: // --> ATTACK
        {
            if (env_data->decay > 0)
            {
                env_osc.set_process_values(env_osc.last_out(), 1, smoothed_shape_buffer[sid_],
                                           get_env_samples(env_data->attack, sample_rate));
                goes_to_sustain = false;
                is_sustain = false;
            }
            else
            {
                env_osc.set_process_values(env_osc.last_out(), smoothed_sustain_buffer[sid_],
                                           smoothed_shape_buffer[sid_],
                                           get_env_samples(env_data->attack, sample_rate));
                goes_to_sustain = true;
                is_sustain = false;
            }

            env_osc.calculate_attack_coeffecients();
            current_stage = ATTACK;

            break;
        }
        case ATTACK: // --> DECAY
        {
            const float decay = env_data->decay;
            if (decay > 0)
            {
                env_osc.set_process_values(env_osc.last_out(), smoothed_sustain_buffer[sid_],
                                           smoothed_shape_buffer[sid_],
                                           get_env_samples(decay, sample_rate));
                goes_to_sustain = true;
                is_sustain = false;
                env_osc.calculate_release_coeffecients();

                current_stage = DECAY;

                break;
            }
            // ELSE FALL TO DECAY AND START SUSTAIN LEVEL
            [[fallthrough]];
        }
        case DECAY: // --> SUSTAIN
        {
            const float sustain_time = env_data->sustain_time;
            const float sustain_level = smoothed_sustain_buffer[sid_];
            if (sustain_time >= 1)
            {
                env_osc.set_process_values(env_osc.last_out(), sustain_level,
                                           smoothed_shape_buffer[sid_],
                                           get_env_samples(sustain_time, sample_rate));
                env_osc.calculate_unlimited_coeffecients();
                is_sustain = true;
                goes_to_sustain = false;
            }
            else
            {
                env_osc.set_process_values(
                    env_osc.last_out(), sustain_level, smoothed_shape_buffer[sid_],
                    juce::jmax(20.0f, get_env_samples(sustain_time, sample_rate)));
                env_osc.calculate_release_coeffecients();
                is_sustain = true;
                goes_to_sustain = false;
            }
            current_stage = SUSTAIN;

            break;
        }
        case SUSTAIN: // --> RELEASE or RETRIGGER
        {
            const float sustain_time = env_data->sustain_time;
            if (force_relese_)
            {
                env_osc.set_process_values(env_osc.last_out(), 0, smoothed_shape_buffer[sid_],
                                           get_env_samples(env_data->release, sample_rate));
                goes_to_sustain = false;
                is_sustain = false;
                env_osc.calculate_release_coeffecients();

                current_stage = RELEASE;
            }
            // RETRIGGER
            else
            {
                start_attack();
            }

            break;
        }
        case RELEASE: // --> RELEASE
        {
            goes_to_sustain = false;
            is_sustain = false;
            current_stage = END_ENV;

            break;
        }
        default:
        {
            goes_to_sustain = false;
            is_sustain = false;
            current_stage = END_ENV;
        }
        }
    }

  public:
    //==========================================================================
    inline void start_attack() noexcept
    {
        current_stage = TRIGGER_START;
        update_stage(0);
    }
    inline void set_to_release() noexcept
    {
        current_stage = SUSTAIN;
        update_stage(0, true);
    }
    inline void reset() noexcept
    {
        current_stage = END_ENV;

        last_sustain = 0;
        goes_to_sustain = false;
        is_sustain = false;
        env_osc.reset();
    }

    //==============================================================================
    inline STAGES get_current_stage() const noexcept { return current_stage; }
    inline void set_current_stage(STAGES current_stage_) noexcept
    {
        current_stage = current_stage_;
    }
    inline void overwrite_current_value(float amp_) noexcept
    {
        env_osc.overwrite_current_value(amp_);
        last_sustain = amp_;
    }
    inline bool is_endless() noexcept { return env_osc.is_endless(); }

    //==============================================================================
    inline float get_amp() const noexcept { return env_osc.last_out(); }

    //==============================================================================
    void sample_rate_or_block_changed() noexcept override {}

  public:
    //==============================================================================
    inline ENV(RuntimeNotifyer *const notifyer_, const MoniqueSynthData *synth_data_,
               ENVData *env_data_, const float *const sine_lookup_, const float *const cos_lookup_,
               const float *const exp_lookup_) noexcept
        : RuntimeListener(notifyer_), env_osc(notifyer_, sine_lookup_, cos_lookup_, exp_lookup_),

          current_stage(END_ENV),

          synth_data(synth_data_),

          last_sustain(0), goes_to_sustain(false), is_sustain(false),

          env_data(env_data_)
    {
    }
    inline ~ENV() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ENV)
};

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
//==============================================================================
//==============================================================================
//==============================================================================
template <int smooth_samples> class AmpSmoother
{
    float current_value;
    float target_value;
    float delta;
    int counter;

  private:
    inline void glide_tick() noexcept;

  public:
    inline float add_get(float in_) noexcept;
    bool is_up_to_date() const noexcept;
    inline float add_get_and_keep_current_time(float in_) noexcept;
    inline void reset(float value_ = 0) noexcept;
    inline float get_current_value() const noexcept;

    COLD AmpSmoother(float start_value_ = 0) noexcept;
    COLD ~AmpSmoother() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpSmoother)
};

//==============================================================================
template <int smooth_samples>
COLD AmpSmoother<smooth_samples>::AmpSmoother(float start_value_) noexcept
    : current_value(0), target_value(start_value_), delta(0), counter(0)
{
}
template <int smooth_samples> COLD AmpSmoother<smooth_samples>::~AmpSmoother() noexcept {}

//==============================================================================
template <int smooth_samples> inline float AmpSmoother<smooth_samples>::add_get(float in_) noexcept
{
    if (current_value != in_ || target_value != in_)
    {
        // ONLY UPDATE IF WE HAVE A NEW VALUE
        if (target_value != in_)
        {
            counter = smooth_samples;
            delta = (in_ - current_value) / smooth_samples;
            target_value = in_;
        }

        glide_tick();
    }

    return current_value;
}
template <int smooth_samples> bool AmpSmoother<smooth_samples>::is_up_to_date() const noexcept
{
    return target_value == current_value;
}
template <int smooth_samples> inline void AmpSmoother<smooth_samples>::reset(float value_) noexcept
{
    current_value = value_;
    delta = 0;
    counter = 0;
}
template <int smooth_samples>
inline float AmpSmoother<smooth_samples>::add_get_and_keep_current_time(float in_) noexcept
{
    if (current_value != in_ || target_value != in_)
    {
        // ONLY UPDATE IF WE HAVE A NEW VALUE
        if (target_value != in_)
        {
            delta = (in_ - current_value) / counter;
            target_value = in_;
        }

        glide_tick();
    }

    return current_value;
}
template <int smooth_samples> inline void AmpSmoother<smooth_samples>::glide_tick() noexcept
{
    if (--counter <= 0)
        current_value = target_value;
    else
    {
        current_value += delta;
        if (current_value > 1 || current_value < 0)
        {
            current_value = target_value;
            counter = 0;
        }
    }
}
template <int smooth_samples>
inline float AmpSmoother<smooth_samples>::get_current_value() const noexcept
{
    return current_value;
}

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
class AnalogFilter : public RuntimeListener
{
    friend class DoubleAnalogFilter;
    float p, k, r;
    float y1, y2, y3, y4;
    float oldx;
    float oldy1, oldy2, oldy3;

    float cutoff, res, res_original;

    bool force_update;
    int zero_counter;

  public:
    //==========================================================================
    // RETURNS TRUE ON COFF CHANGED
    inline bool update(float resonance_, float cutoff_) noexcept
    {
        bool success = false;
        if (force_update || (cutoff != cutoff_ || res_original != resonance_))
        {
            cutoff = cutoff_;
            res_original = resonance_;
            res = juce::jmax(0.00001f, resonance_ *= 0.99999);
            success = true;

            force_update = false;
        }
        return success;
    }
    inline void update_with_fixed_cutoff(float resonance_, float cutoff_) noexcept
    {
        if (force_update || (cutoff != cutoff_ || res_original != resonance_))
        {
            cutoff = cutoff_;
            res_original = resonance_;
            res = resonance_;
            calc_coefficients(cutoff);

            force_update = false;
        }
    }
    //==========================================================================
    inline void copy_coefficient_from(const AnalogFilter &other_) noexcept
    {
        cutoff = other_.cutoff;
        res = other_.res;

        p = other_.p;
        k = other_.k;
        r = other_.r;
    }
    inline void copy_state_from(const AnalogFilter &other_) noexcept
    {
        oldx = other_.oldx;
        oldy1 = other_.oldy1;
        oldy2 = other_.oldy2;
        oldy3 = other_.oldy3;
        y1 = other_.y1;
        y2 = other_.y2;
        y3 = other_.y3;
        y4 = other_.y4;
    }
#define MONO_UNDENORMALISE(n)                                                                      \
    if (!(n < -1.0e-8f || n > 1.0e-8f))                                                            \
        n = 0;
    //==========================================================================
    inline float processLow(float input_and_worker_) noexcept
    {
        MONO_UNDENORMALISE(input_and_worker_);
        if (input_and_worker_ != 0)
        {
            zero_counter = 0;
        }
        else if (y4 == 0)
        {
            ++zero_counter;
        }
        else
        {
            zero_counter = 0;
        }

        if (zero_counter < 50)
        {
            input_and_worker_ -= r * y4;
            MONO_UNDENORMALISE(input_and_worker_);

            // Four cascaded onepole filters (bilinear transform)
            y1 = input_and_worker_ * p + oldx * p - k * y1;
            y2 = y1 * p + oldy1 * p - k * y2;
            y3 = y2 * p + oldy2 * p - k * y3;
            y4 = y3 * p + oldy3 * p - k * y4;

            // Clipper band limited sigmoid
            y4 -= (y4 * y4 * y4) / 6;
            MONO_UNDENORMALISE(y4);

            oldx = input_and_worker_;
            oldy1 = y1;
            oldy2 = y2;
            oldy3 = y3;

            input_and_worker_ = soft_clipp_greater_1_2(y4);
            MONO_UNDENORMALISE(input_and_worker_);
        }

        return input_and_worker_;
    }
    inline float processLowResonance(float input_and_worker_) noexcept
    {
        MONO_UNDENORMALISE(input_and_worker_);
        if (input_and_worker_ != 0)
        {
            zero_counter = 0;
        }
        else if (y4 == 0)
        {
            ++zero_counter;
        }
        else
        {
            zero_counter = 0;
        }

        if (zero_counter < 50)
        {
            // process input
            input_and_worker_ -= r * y4;
            MONO_UNDENORMALISE(y1);

            // Four cascaded onepole filters (bilinear transform)
            y1 = input_and_worker_ * p + oldx * p - k * y1;
            y2 = y1 * p + oldy1 * p - k * y2;
            y3 = y2 * p + oldy2 * p - k * y3;
            y4 = y3 * p + oldy3 * p - k * y4;

            // Clipper band limited sigmoid
            y4 -= (y4 * y4 * y4) / 6;
            MONO_UNDENORMALISE(y4);

            oldx = input_and_worker_;
            oldy1 = y1;
            oldy2 = y2;
            oldy3 = y3;

            input_and_worker_ = soft_clipp_greater_1_2(sample_mix(y4, y3 * res));
            MONO_UNDENORMALISE(input_and_worker_);
        }

        return input_and_worker_;
    }
    inline float processHighResonance(float input_and_worker_) noexcept
    {
        MONO_UNDENORMALISE(input_and_worker_);
        if (input_and_worker_ != 0)
        {
            zero_counter = 0;
        }
        else if (y4 == 0)
        {
            ++zero_counter;
        }
        else
        {
            zero_counter = 0;
        }

        if (zero_counter < 50)
        {
            // process input
            input_and_worker_ -= r * y4;
            MONO_UNDENORMALISE(input_and_worker_);

            // Four cascaded onepole filters (bilinear transform)
            y1 = input_and_worker_ * p + oldx * p - k * y1;
            y2 = y1 * p + oldy1 * p - k * y2;
            y3 = y2 * p + oldy2 * p - k * y3;
            y4 = y3 * p + oldy3 * p - k * y4;

            // Clipper band limited sigmoid
            y4 -= (y4 * y4 * y4) / 6;
            MONO_UNDENORMALISE(y4);

            oldx = input_and_worker_;
            oldy1 = y1;
            oldy2 = y2;
            oldy3 = y3;

            input_and_worker_ = hard_clipper_1(input_and_worker_ - y4);
            MONO_UNDENORMALISE(input_and_worker_);
        }

        return input_and_worker_;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        y1 = y2 = y3 = y4 = oldx = oldy1 = oldy2 = oldy3 = 0;
        zero_counter = 0;
    }

    //==========================================================================
    inline void calc_coefficients(float cutoff_) noexcept
    {
        {
            float f = cutoff_ / sample_rate;

            // float agressive = 0.98f *( 1.0f - res );
            // float agressive = 0.98f * jmax(0.0f,( 1.0f/8000.0f*cutoff ));
            // p=f*((1.99f-agressive)-((0.99f-agressive)*f));
            p = f * (1.8f - 0.8f * f);
            k = p * 2 - 1;
        }
        {
            // const float cutoff_power = 1.0f - (0.5f * jmin(1.0f,1.0f/(8000.0f*cutoff_)));
            float t =
                (1.0f - p) * 1.386249f; // + ( 0.2f * (1.0f-1.0f/(1000.0f*jmax(0.00001f,cutoff))) );
            const float t2 = 12.0f + t * t;
            r = res * (t2 + 6.0f * t) / (t2 - 6.0f * t); // * cutoff_power;
        }
    }

  private:
    //==========================================================================
    COLD void sample_rate_or_block_changed() noexcept override
    {
        reset();
        force_update = true;
    }

  public:
    //==========================================================================
    COLD AnalogFilter(RuntimeNotifyer *const notifyer_) noexcept
        : RuntimeListener(notifyer_),

          p(1), k(1), r(1), y1(0), y2(0), y3(0), y4(0), oldx(0), oldy1(0), oldy2(0), oldy3(0),

          cutoff(1000), res(1), res_original(0.99999),

          force_update(true), zero_counter(0)
    {
        sample_rate_or_block_changed();
    }
    COLD ~AnalogFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogFilter)
};

//==============================================================================
//==============================================================================
//==============================================================================
#define FILTER_CHANGE_GLIDE_TIME_MS (msToSamplesFast(200, flt_1.sample_rate) + 50)
class DoubleAnalogFilter
{
    AnalogFilter flt_1;
    AnalogFilter flt_2;

    DoubleAnalogFilter *smooth_filter;

    FILTER_TYPS last_filter_type;
    FILTER_TYPS smooth_filter_type;
    int glide_time_4_filters;

  public:
    // LP
    //==========================================================================
    inline void updateLow2Pass(float resonance_, float cutoff_) noexcept
    {
        if (flt_2.update(resonance_, cutoff_))
        {
            flt_2.calc_coefficients(get_cutoff(cutoff_));
            flt_1.copy_coefficient_from(flt_2);
        }
    }
    inline float processLow2Pass(float in_) noexcept
    {
        const float out = flt_2.processLowResonance(in_);
        const float low = flt_1.processLowResonance(out);

        return process_filter_change(in_,
                                     //(out+low)*(1.0f-gain) + resonance_clipping(out+low)*gain
                                     sample_mix(out, low));
    }

    // 1 PASS HP
    //==========================================================================
    inline void updateHigh2Pass(float resonance_, float cutoff_) noexcept
    {
        if (flt_1.update(resonance_, cutoff_))
        {
            flt_1.calc_coefficients(get_cutoff(cutoff_));
        }
    }
    inline float processHigh2Pass(float in_) noexcept
    {
        in_ = soft_clipp_greater_1_2(in_);
        const float out = flt_1.processHighResonance(in_);
        return process_filter_change(in_, out);
    }

    // BAND
    //==========================================================================
    inline void updateBand(float resonance_, float cutoff_) noexcept
    {
        float cutoff_2 = cutoff_ + cutoff_ * 0.02;
        if (flt_1.update(resonance_, cutoff_2))
        {
            flt_1.calc_coefficients(get_cutoff(cutoff_));
            flt_2.update(resonance_, cutoff_);
            flt_2.calc_coefficients(get_cutoff(cutoff_));
        }
    }
    inline float processBand(float in_) noexcept
    {
        return process_filter_change(
            in_, flt_1.processLowResonance(flt_2.processHighResonance(in_)) * 2);
    }

    // PASS
    //==========================================================================
    inline float processPass(float in_) noexcept { return process_filter_change(in_, in_); }

    // BY TYPE
    //==========================================================================
    inline void update_filter_to(FILTER_TYPS type_) noexcept
    {
        if (last_filter_type != type_)
        {
            if (smooth_filter)
            {
                // SET THE SECOND FILTER TO THE OLD COMPLETE STATE
                if (last_filter_type != PASS)
                {
                    smooth_filter->flt_1.copy_coefficient_from(flt_1);
                    smooth_filter->flt_1.copy_state_from(flt_1);
                    flt_1.force_update = true;

                    smooth_filter->flt_2.copy_coefficient_from(flt_2);
                    smooth_filter->flt_2.copy_state_from(flt_2);
                    flt_2.force_update = true;
                }
                else
                {
                    flt_1.reset();
                    flt_2.reset();
                }
                flt_1.force_update = true;
                flt_2.force_update = true;

                glide_time_4_filters = FILTER_CHANGE_GLIDE_TIME_MS;

                smooth_filter->last_filter_type = last_filter_type;
                smooth_filter_type = last_filter_type;
            }

            last_filter_type = type_;
        }
    }
    inline float process_filter_change(float original_in_, float result_in_) noexcept
    {
        if (glide_time_4_filters > 0)
        {
            // if( smooth_filter ) IS TRUE IF glide_time_4_filters != 0
            {
                const float smooth_out =
                    smooth_filter->processByType(original_in_, smooth_filter_type);

                float mix = 1.0f / float(FILTER_CHANGE_GLIDE_TIME_MS) * glide_time_4_filters;
                result_in_ = result_in_ * (1.0f - mix) + smooth_out * mix;
            }
            --glide_time_4_filters;
        }

        return result_in_;
    }
    inline float processByType(float io_, FILTER_TYPS type_) noexcept
    {
        switch (type_)
        {
        case LPF:
        case LPF_2_PASS:
            io_ = processLow2Pass(io_);
            break;
        case HPF:
        case HIGH_2_PASS:
            io_ = processHigh2Pass(io_);
            break;
        case BPF:
            io_ = processBand(io_);
            break;
        default /* PASS & UNKNOWN */:; // io_ = filter_hard_clipper(io_);
        }

        return io_;
    }

    //==========================================================================
    inline void reset() noexcept
    {
        last_filter_type = UNKNOWN;
        smooth_filter_type = UNKNOWN;
        glide_time_4_filters = 0;
        if (smooth_filter)
        {
            smooth_filter->reset();
        }
        flt_1.reset();
        flt_2.reset();
    }

  public:
    //==============================================================================
    COLD DoubleAnalogFilter(RuntimeNotifyer *const notifyer_,
                            bool create_smooth_filter = true) noexcept
        : flt_1(notifyer_), flt_2(notifyer_),

          smooth_filter(nullptr),

          last_filter_type(UNKNOWN), smooth_filter_type(UNKNOWN),

          glide_time_4_filters(0)
    {
        if (create_smooth_filter)
        {
            smooth_filter = new DoubleAnalogFilter(notifyer_, false);
        }
    }
    COLD ~DoubleAnalogFilter() noexcept
    {
        if (smooth_filter)
        {
            delete smooth_filter;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DoubleAnalogFilter)
};

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
//==============================================================================
class FilterProcessor
{
    juce::OwnedArray<DoubleAnalogFilter> double_filter;
    friend class mono_ParameterOwnerStore;

  public:
    std::unique_ptr<ENV> env;
    juce::Array<ENVData *> input_env_datas;
    juce::OwnedArray<ENV> input_envs;

  private:
    const int id;

    const MoniqueSynthData *const synth_data;
    FilterData *const filter_data;
    DataBuffer *const data_buffer;

    const float *const sin_lookup;
    const float *const cos_lookup;

  public:
    //==========================================================================
    inline void start_attack() noexcept
    {
        start_env();
        start_input_env(0);
        start_input_env(1);
        start_input_env(2);
    }
    inline void start_env() noexcept { env->start_attack(); }
    inline void start_input_env(int id_) noexcept { input_envs.getUnchecked(id_)->start_attack(); }
    inline void start_release() noexcept
    {
        start_env_release();
        start_input_env_release(0);
        start_input_env_release(1);
        start_input_env_release(2);
    }
    inline void start_env_release() noexcept { env->set_to_release(); }
    inline void start_input_env_release(int id_) noexcept
    {
        input_envs.getUnchecked(id_)->set_to_release();
    }

    //==========================================================================
    inline void reset() noexcept
    {
        env->reset();
        for (int input_id = 0; input_id != SUM_INPUTS_PER_FILTER; ++input_id)
        {
            input_envs.getUnchecked(input_id)->reset();
        }

        // double_filter[0]->reset();
        // double_filter[1]->reset();
        // double_filter[2]->reset();
    }

  private:
    //==========================================================================
    inline void pre_process(const int input_id, const int num_samples) noexcept
    {
        // CALCULATE INPUTS AND ENVELOPS
        {
            {
                const int glide_modotr_time = synth_data->glide_motor_time;
                if (id == FILTER_1)
                {
                    float *tmp_input_amp = data_buffer->filter_input_env_amps.getWritePointer(
                        input_id + SUM_INPUTS_PER_FILTER * FILTER_1);
                    ENV *const input_env(input_envs.getUnchecked(input_id));
                    filter_data->input_smoothers[input_id]->process_amp(
                        !filter_data->input_holds[input_id], glide_modotr_time, input_env,
                        tmp_input_amp, num_samples);

                    float *filter_input_buffer =
                        data_buffer->filter_input_samples.getWritePointer(input_id);
                    const float *const osc_input_buffer =
                        data_buffer->osc_samples.getReadPointer(input_id);
                    for (int sid = 0; sid != num_samples; ++sid)
                    {
                        filter_input_buffer[sid] = osc_input_buffer[sid] * tmp_input_amp[sid];
                    }
                }
                else if (id == FILTER_2)
                {
                    float *tmp_input_amp = data_buffer->filter_input_env_amps.getWritePointer(
                        input_id + SUM_INPUTS_PER_FILTER * FILTER_2);
                    ENV *const input_env(input_envs.getUnchecked(input_id));
                    filter_data->input_smoothers[input_id]->process_amp(
                        !filter_data->input_holds[input_id], glide_modotr_time, input_env,
                        tmp_input_amp, num_samples);

                    float *const filter_input_buffer =
                        data_buffer->filter_input_samples.getWritePointer(
                            input_id + SUM_INPUTS_PER_FILTER * FILTER_2);
                    const float *const filter_before_buffer =
                        data_buffer->filter_output_samples.getReadPointer(
                            input_id + SUM_INPUTS_PER_FILTER * FILTER_1);
                    const float *const osc_input_buffer =
                        data_buffer->osc_samples.getReadPointer(input_id);
                    for (int sid = 0; sid != num_samples; ++sid)
                    {
                        filter_input_buffer[sid] =
                            tmp_input_amp[sid] < 0 ? osc_input_buffer[sid] * tmp_input_amp[sid] * -1
                                                   : filter_before_buffer[sid] * tmp_input_amp[sid];
                    }
                }
                else
                {
                    float *tmp_input_amp_1 = data_buffer->filter_input_env_amps.getWritePointer(
                        0 + SUM_INPUTS_PER_FILTER * FILTER_3);
                    float *tmp_input_amp_2 = data_buffer->filter_input_env_amps.getWritePointer(
                        1 + SUM_INPUTS_PER_FILTER * FILTER_3);
                    float *tmp_input_amp_3 = data_buffer->filter_input_env_amps.getWritePointer(
                        2 + SUM_INPUTS_PER_FILTER * FILTER_3);
                    {
                        ENV *const input_env(input_envs.getUnchecked(0));
                        filter_data->input_smoothers[0]->process_amp(!filter_data->input_holds[0],
                                                                     glide_modotr_time, input_env,
                                                                     tmp_input_amp_1, num_samples);
                    }
                    {
                        ENV *const input_env(input_envs.getUnchecked(1));
                        filter_data->input_smoothers[1]->process_amp(!filter_data->input_holds[1],
                                                                     glide_modotr_time, input_env,
                                                                     tmp_input_amp_2, num_samples);
                    }
                    {
                        ENV *const input_env(input_envs.getUnchecked(2));
                        filter_data->input_smoothers[2]->process_amp(!filter_data->input_holds[2],
                                                                     glide_modotr_time, input_env,
                                                                     tmp_input_amp_3, num_samples);
                    }

                    float *const filter_input_buffer =
                        data_buffer->filter_input_samples.getWritePointer(
                            0 + SUM_INPUTS_PER_FILTER * FILTER_3);
                    const float *const filter_before_buffer_1 =
                        data_buffer->filter_output_samples.getReadPointer(
                            0 + SUM_INPUTS_PER_FILTER * FILTER_2);
                    const float *const filter_before_buffer_2 =
                        data_buffer->filter_output_samples.getReadPointer(
                            1 + SUM_INPUTS_PER_FILTER * FILTER_2);
                    const float *const filter_before_buffer_3 =
                        data_buffer->filter_output_samples.getReadPointer(
                            2 + SUM_INPUTS_PER_FILTER * FILTER_2);
                    const float *const osc_input_buffer_1 =
                        data_buffer->osc_samples.getReadPointer(0);
                    const float *const osc_input_buffer_2 =
                        data_buffer->osc_samples.getReadPointer(1);
                    const float *const osc_input_buffer_3 =
                        data_buffer->osc_samples.getReadPointer(2);

                    juce::FloatVectorOperations::clear(filter_input_buffer, num_samples);
                    for (int sid = 0; sid != num_samples; ++sid)
                    {
                        filter_input_buffer[sid] = sample_mix(
                            sample_mix(tmp_input_amp_1[sid] < 0
                                           ? osc_input_buffer_1[sid] * tmp_input_amp_1[sid] * -1
                                           : filter_before_buffer_1[sid] * tmp_input_amp_1[sid],
                                       tmp_input_amp_2[sid] < 0
                                           ? osc_input_buffer_2[sid] * tmp_input_amp_2[sid] * -1
                                           : filter_before_buffer_2[sid] * tmp_input_amp_2[sid]),
                            tmp_input_amp_3[sid] < 0
                                ? osc_input_buffer_3[sid] * tmp_input_amp_3[sid] * -1
                                : filter_before_buffer_3[sid] * tmp_input_amp_3[sid]);
                    }
                }
            }
        }
    }
    inline void process_amp_mix(const int num_samples) noexcept
    {
        // ADSTR - LFO MIX
        float *amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
        const float *smoothed_mix_buffer(
            filter_data->adsr_lfo_mix_smoother.get_smoothed_value_buffer());
        {
            const float *env_amps = data_buffer->filter_env_amps.getReadPointer(id);
            const float *lfo_amplitudes = data_buffer->lfo_amplitudes.getReadPointer(id);
            for (int sid = 0; sid != num_samples; ++sid)
            {
                // LFO ADSR MIX - HERE TO SAVE ONE LOOP
                const float mix = (1.0f + smoothed_mix_buffer[sid]) * 0.5f;
                amp_mix[sid] = env_amps[sid] * (1.0f - mix) + lfo_amplitudes[sid] * mix;
            }
        }
    }

    static float distortion__(float x_, float distortion_power_) noexcept
    {
        if (distortion_power_ != 0)
        {
            // const float distortion_add_on = distortion_power_*0.9f;
            // x_ = ((1.0f+distortion_add_on)*x_) - (x_*x_*x_)*distortion_add_on;
            // (std::atan(input_and_worker_)*(1.0f/juce::MathConstants<float>::pi))*
            // x_ = x_*(1.0f-distortion_power_) + 0.5f*soft_clipping( x_*10 )*(distortion_power_);

            x_ = x_ * (1.0f - distortion_power_) + (std::atan(x_ * 20) / 6.66) * distortion_power_;

            // x_ = x_*(1.0f-distortion_power_) + std::atan( x_*150 )*(1.0f/1.55)*distortion_power_;
        }

        return x_;
    }

  public:
    //==========================================================================
    inline void process(const int num_samples) noexcept
    {
        float *amp_mix = data_buffer->lfo_amplitudes.getWritePointer(id);
        // PROCESS FILTER
        {
#define DISTORTION_IN(x) distortion__(x, filter_distortion)
#define DISTORTION_OUT(x) distortion__(x, filter_distortion)

            // PREPARE
            {
                process_amp_mix(num_samples);

                filter_data->resonance_smoother.process_modulation(filter_data->modulate_resonance,
                                                                   amp_mix, num_samples);
                filter_data->cutoff_smoother.process_modulation(filter_data->modulate_cutoff,
                                                                amp_mix, num_samples);
                filter_data->distortion_smoother.process_modulation(
                    filter_data->modulate_distortion, amp_mix, num_samples);
                if (synth_data->is_stereo)
                    filter_data->pan_smoother.process_modulation(filter_data->modulate_pan, amp_mix,
                                                                 num_samples);
            }

            switch (filter_data->filter_type)
            {
            case LPF:
            case LPF_2_PASS:
            case MOOG_AND_LPF:
            {
                // PROCESSOR
                struct LP2PassExecuter
                {
                    FilterProcessor *const processor;
                    DoubleAnalogFilter &filter;
                    const int input_id;
                    const int num_samples_;

                    const float *const tmp_resonance_buffer;
                    const float *const tmp_cuttof_buffer;
                    const float *const tmp_distortion_buffer;

                    const float *const input_buffer;
                    float *const out_buffer;

                    inline void exec() noexcept
                    {
                        processor->pre_process(input_id, num_samples_);

                        filter.update_filter_to(LPF_2_PASS);
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];

                            /*
                                        filter.updateLow2Pass
                                        (
                                            tmp_resonance_buffer[sid],
                                            ( (10180.0f * tmp_cuttof_buffer[sid]) +20 ),
                                            tmp_gain_buffer[sid]
                                        );
                                        */

                            filter.updateLow2Pass(tmp_resonance_buffer[sid],
                                                  tmp_cuttof_buffer[sid]);
                            out_buffer[sid] = DISTORTION_OUT(
                                filter.processLow2Pass(DISTORTION_IN(input_buffer[sid])));
                        }
                    }
                    LP2PassExecuter(FilterProcessor *const processor_, int num_samples__,
                                    int input_id_) noexcept
                        : processor(processor_),
                          filter(*processor_->double_filter.getUnchecked(input_id_)),
                          input_id(input_id_), num_samples_(num_samples__),

                          tmp_resonance_buffer(processor_->filter_data->resonance_smoother
                                                   .get_smoothed_value_buffer()),
                          tmp_cuttof_buffer(
                              processor_->filter_data->cutoff_smoother.get_smoothed_value_buffer()),
                          tmp_distortion_buffer(processor_->filter_data->distortion_smoother
                                                    .get_smoothed_value_buffer()),

                          input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id)),
                          out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id))
                    {
                    }
                };
                {
                    if (id != FILTER_3)
                    {
                        LP2PassExecuter(this, num_samples, 0).exec();
                        LP2PassExecuter(this, num_samples, 1).exec();
                        LP2PassExecuter(this, num_samples, 2).exec();
                    }
                    else
                    {
                        // 1, 2 and 3
                        LP2PassExecuter(this, num_samples, 0).exec();
                    }
                }
            }
            break;
            case HPF:
            case HIGH_2_PASS:
            {
                // PROCESSOR
                struct HP2PassExecuter
                {
                    FilterProcessor *const processor;
                    DoubleAnalogFilter &filter;
                    const int input_id;
                    const int num_samples_;

                    const float *const tmp_resonance_buffer;
                    const float *const tmp_cuttof_buffer;
                    const float *const tmp_distortion_buffer;

                    const float *const input_buffer;
                    float *const out_buffer;

                    inline void exec() noexcept
                    {
                        processor->pre_process(input_id, num_samples_);

                        filter.update_filter_to(HIGH_2_PASS);
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            filter.updateHigh2Pass(tmp_resonance_buffer[sid],
                                                   tmp_cuttof_buffer[sid]);
                            out_buffer[sid] = DISTORTION_OUT(
                                filter.processHigh2Pass(DISTORTION_IN(input_buffer[sid])));
                        }
                    }
                    HP2PassExecuter(FilterProcessor *const processor_, int num_samples__,
                                    int input_id_) noexcept
                        : processor(processor_),
                          filter(*processor_->double_filter.getUnchecked(input_id_)),
                          input_id(input_id_), num_samples_(num_samples__),

                          tmp_resonance_buffer(processor_->filter_data->resonance_smoother
                                                   .get_smoothed_value_buffer()),
                          tmp_cuttof_buffer(
                              processor_->filter_data->cutoff_smoother.get_smoothed_value_buffer()),
                          tmp_distortion_buffer(processor_->filter_data->distortion_smoother
                                                    .get_smoothed_value_buffer()),

                          input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id)),
                          out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id))
                    {
                    }
                };
                {
                    if (id != FILTER_3)
                    {
                        HP2PassExecuter(this, num_samples, 0).exec();
                        HP2PassExecuter(this, num_samples, 1).exec();
                        HP2PassExecuter(this, num_samples, 2).exec();
                    }
                    else
                    {
                        // 1, 2 and 3
                        HP2PassExecuter(this, num_samples, 0).exec();
                    }
                }
            }
            break;
            case BPF:
            {
                // PROCESSOR
                struct BandExecuter
                {
                    FilterProcessor *const processor;
                    DoubleAnalogFilter &filter;
                    const int input_id;
                    const int num_samples_;

                    const float *const tmp_resonance_buffer;
                    const float *const tmp_cuttof_buffer;
                    const float *const tmp_distortion_buffer;

                    const float *const input_buffer;
                    float *const out_buffer;

                    inline void exec() noexcept
                    {
                        processor->pre_process(input_id, num_samples_);

                        filter.update_filter_to(BPF);

                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            filter.updateBand(tmp_resonance_buffer[sid], tmp_cuttof_buffer[sid]);
                            out_buffer[sid] = DISTORTION_OUT(
                                filter.processBand(DISTORTION_IN(input_buffer[sid])));
                        }
                    }
                    BandExecuter(FilterProcessor *const processor_, int num_samples__,
                                 int input_id_) noexcept
                        : processor(processor_),
                          filter(*processor_->double_filter.getUnchecked(input_id_)),
                          input_id(input_id_), num_samples_(num_samples__),

                          tmp_resonance_buffer(processor_->filter_data->resonance_smoother
                                                   .get_smoothed_value_buffer()),
                          tmp_cuttof_buffer(
                              processor_->filter_data->cutoff_smoother.get_smoothed_value_buffer()),
                          tmp_distortion_buffer(processor_->filter_data->distortion_smoother
                                                    .get_smoothed_value_buffer()),

                          input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id)),
                          out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id))
                    {
                    }
                };
                {
                    if (id != FILTER_3)
                    {
                        BandExecuter(this, num_samples, 0).exec();
                        BandExecuter(this, num_samples, 1).exec();
                        BandExecuter(this, num_samples, 2).exec();
                    }
                    else
                    {
                        // 1, 2 and 3
                        BandExecuter(this, num_samples, 0).exec();
                    }
                }
            }
            break;
            default: //  PASS
            {
                // PROCESSOR
                struct PassExecuter
                {
                    FilterProcessor *const processor;
                    DoubleAnalogFilter &filter;
                    const int input_id;
                    const int num_samples_;

                    const float *const tmp_distortion_buffer;

                    const float *const input_buffer;
                    float *const out_buffer;

                    inline void exec() noexcept
                    {
                        processor->pre_process(input_id, num_samples_);

                        filter.update_filter_to(PASS);

                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            const float filter_distortion = tmp_distortion_buffer[sid];
                            out_buffer[sid] = DISTORTION_OUT(
                                filter.processPass(DISTORTION_IN(input_buffer[sid])));
                        }
                    }
                    PassExecuter(FilterProcessor *const processor_, int num_samples__,
                                 int input_id_) noexcept
                        : processor(processor_),
                          filter(*processor_->double_filter.getUnchecked(input_id_)),
                          input_id(input_id_), num_samples_(num_samples__),

                          tmp_distortion_buffer(processor_->filter_data->distortion_smoother
                                                    .get_smoothed_value_buffer()),

                          input_buffer(processor_->data_buffer->filter_input_samples.getReadPointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id)),
                          out_buffer(processor_->data_buffer->filter_output_samples.getWritePointer(
                              input_id_ + SUM_INPUTS_PER_FILTER * processor_->id))
                    {
                    }
                };
                {
                    if (id != FILTER_3)
                    {
                        PassExecuter(this, num_samples, 0).exec();
                        PassExecuter(this, num_samples, 1).exec();
                        PassExecuter(this, num_samples, 2).exec();
                    }
                    else
                    {
                        // 1, 2 and 3
                        PassExecuter(this, num_samples, 0).exec();
                        ;
                    }
                }
            }
            break;
            }
        }

        // COLLECT RESULTS
        float *const this_filter_output_buffer =
            data_buffer->filter_output_samples_l_r.getWritePointer(id);
        {
            const float *const smoothed_distortion_buffer(
                filter_data->distortion_smoother.get_smoothed_value_buffer());
            {
                filter_data->output_smoother.process_modulation(filter_data->modulate_output,
                                                                amp_mix, num_samples);
                const float *const smoothed_output_buffer =
                    filter_data->output_smoother.get_smoothed_value_buffer();
                if (id != FILTER_3)
                {
                    float *const out_buffer_1(data_buffer->filter_output_samples.getWritePointer(
                        0 + SUM_INPUTS_PER_FILTER * id));
                    float *const out_buffer_2(data_buffer->filter_output_samples.getWritePointer(
                        1 + SUM_INPUTS_PER_FILTER * id));
                    float *const out_buffer_3(data_buffer->filter_output_samples.getWritePointer(
                        2 + SUM_INPUTS_PER_FILTER * id));
                    for (int sid = 0; sid != num_samples; ++sid)
                    {
                        // OUTPUT MIX AND DISTORTION
                        {
                            const float amp = smoothed_output_buffer[sid];
                            float shape_power = smoothed_distortion_buffer[sid];
                            const float result =
                                sample_mix(sample_mix(out_buffer_1[sid], out_buffer_2[sid]),
                                           out_buffer_3[sid]) *
                                amp * 2;

                            this_filter_output_buffer[sid] =
                                sample_mix(result * (1.0f - shape_power),
                                           soft_clipping(result * 5) * 1.5 * (shape_power));
                        }
                    }
                }
                else
                {
                    float *const out_buffer(data_buffer->filter_output_samples.getWritePointer(
                        0 + SUM_INPUTS_PER_FILTER * FILTER_3));
                    for (int sid = 0; sid != num_samples; ++sid)
                    {
                        // OUTPUT MIX AND DISTORTION
                        {
                            const float amp = smoothed_output_buffer[sid];
                            float shape_power = smoothed_distortion_buffer[sid];
                            const float result = out_buffer[sid] * amp * 2;

                            this_filter_output_buffer[sid] =
                                sample_mix(result * (1.0f - shape_power),
                                           soft_clipping(result * 5) * 1.5 * (shape_power));
                        }
                    }
                }
            }
        }

// PAN & MIX
#ifdef POLY
        if (synth_data->is_stereo)
#endif
        {
            const float *const pan_buffer = filter_data->pan_smoother.get_smoothed_value_buffer();
            float *const left_and_input_output_buffer =
                data_buffer->filter_output_samples_l_r.getWritePointer(id);
            float *const right_output_buffer =
                data_buffer->filter_output_samples_l_r.getWritePointer(SUM_FILTERS + id);
#ifdef POLY
            const bool calculate_tracking[SUM_FILTERS] = {synth_data->keytrack_filter_volume[0],
                                                          synth_data->keytrack_filter_volume[1],
                                                          synth_data->keytrack_filter_volume[2]};
            const float *const env_tracking_buffer =
                data_buffer->filter_env_tracking.getReadPointer(id);
#endif
            // const float multiplyer = id == FILTER_3 ? 1.5f : 1;
            for (int sid = 0; sid != num_samples; ++sid)
            {
                const float pan = pan_buffer[sid];
                const float output_sample = left_and_input_output_buffer[sid];
#ifdef POLY
                right_output_buffer[sid] =
                    output_sample * left_pan(pan, sin_lookup) *
                    (calculate_tracking[id]
                         ? env_tracking_buffer[sid] *
                                   (1.0f - synth_data->keytrack_filter_volume_offset[id]) +
                               synth_data->keytrack_filter_volume_offset[id]
                         : 1);
                left_and_input_output_buffer[sid] =
                    output_sample * right_pan(pan, cos_lookup) *
                    (calculate_tracking[id]
                         ? env_tracking_buffer[sid] *
                                   (1.0f - synth_data->keytrack_filter_volume_offset[id]) +
                               synth_data->keytrack_filter_volume_offset[id]
                         : 1);
#else
                right_output_buffer[sid] = output_sample * left_pan(pan, sin_lookup);
                left_and_input_output_buffer[sid] = output_sample * right_pan(pan, cos_lookup);
#endif
            }

            // VISUALIZE
            if (Monique_Ui_AmpPainter *const amp_painter = synth_data->audio_processor->amp_painter)
            {
                amp_painter->add_filter_env(id, amp_mix, num_samples);
                amp_painter->add_filter(id, right_output_buffer, left_and_input_output_buffer,
                                        num_samples);
            }
        }
#ifdef POLY
        else // NOTE just a reduced copy of the function before
        {
            float *const left_and_input_output_buffer =
                data_buffer->filter_output_samples_l_r.getWritePointer(id);
            const bool calculate_tracking[SUM_FILTERS] = {synth_data->keytrack_filter_volume[0],
                                                          synth_data->keytrack_filter_volume[1],
                                                          synth_data->keytrack_filter_volume[2]};
            const float *const env_tracking_buffer =
                data_buffer->filter_env_tracking.getReadPointer(id);
            // const float multiplyer = id == FILTER_3 ? 1.5f : 1;
            for (int sid = 0; sid != num_samples; ++sid)
            {
                const float output_sample = left_and_input_output_buffer[sid];
                left_and_input_output_buffer[sid] =
                    output_sample *
                    (calculate_tracking[id]
                         ? env_tracking_buffer[sid] *
                                   (1.0f - synth_data->keytrack_filter_volume_offset[id]) +
                               synth_data->keytrack_filter_volume_offset[id]
                         : 1);
            }

            // VISUALIZE
            if (Monique_Ui_AmpPainter *const amp_painter = synth_data->audio_processor->amp_painter)
            {
                amp_painter->add_filter_env(id, amp_mix, num_samples);
                amp_painter->add_filter(id, left_and_input_output_buffer,
                                        left_and_input_output_buffer, num_samples);
            }
        }
#endif

        // COLLECT THE FINAL OUTPUT
        if (id == FILTER_3)
        {
            if (synth_data->is_stereo)
            {
                float *const master_left_output_buffer =
                    data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);
                float *const master_right_output_buffer =
                    data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT);
                const float *const left_output_buffer_flt1 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(0);
                const float *const right_output_buffer_flt1 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS);
                const float *const left_output_buffer_flt2 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(1);
                const float *const right_output_buffer_flt2 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS + 1);
                const float *const left_output_buffer_flt3 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(2);
                const float *const right_output_buffer_flt3 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(SUM_FILTERS + 2);
                const float *const smoothed_distortion =
                    synth_data->distortion_smoother.get_smoothed_value_buffer();
                const float *const smoothed_fx_bypass_buffer =
                    synth_data->effect_bypass_smoother.get_smoothed_value_buffer();

                for (int sid = 0; sid != num_samples; ++sid)
                {
                    const float left = sample_mix(
                        sample_mix(left_output_buffer_flt1[sid], left_output_buffer_flt2[sid]),
                        left_output_buffer_flt3[sid]);
                    const float right = sample_mix(
                        sample_mix(right_output_buffer_flt1[sid], right_output_buffer_flt2[sid]),
                        right_output_buffer_flt3[sid]);
                    const float left_add = left_output_buffer_flt1[sid] +
                                           left_output_buffer_flt2[sid] +
                                           left_output_buffer_flt3[sid];
                    const float right_add = right_output_buffer_flt1[sid] +
                                            right_output_buffer_flt2[sid] +
                                            right_output_buffer_flt3[sid];
                    const float distortion =
                        smoothed_distortion[sid] * smoothed_fx_bypass_buffer[sid];

                    master_left_output_buffer[sid] =
                        left * (1.0f - distortion) +
                        1.33f * soft_clipping(left_add * 10) * (distortion);
                    master_right_output_buffer[sid] =
                        right * (1.0f - distortion) +
                        1.33f * soft_clipping(right_add * 10) * (distortion);
                }
            }
            else // NOTE just a reduced copy of the function before
            {
                float *const master_left_output_buffer =
                    data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);
                const float *const left_output_buffer_flt1 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(0);
                const float *const left_output_buffer_flt2 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(1);
                const float *const left_output_buffer_flt3 =
                    data_buffer->filter_output_samples_l_r.getReadPointer(2);
                const float *const smoothed_distortion =
                    synth_data->distortion_smoother.get_smoothed_value_buffer();
                const float *const smoothed_fx_bypass_buffer =
                    synth_data->effect_bypass_smoother.get_smoothed_value_buffer();

                for (int sid = 0; sid != num_samples; ++sid)
                {
                    const float left = sample_mix(
                        sample_mix(left_output_buffer_flt1[sid], left_output_buffer_flt2[sid]),
                        left_output_buffer_flt3[sid]);
                    const float left_add = left_output_buffer_flt1[sid] +
                                           left_output_buffer_flt2[sid] +
                                           left_output_buffer_flt3[sid];
                    const float distortion =
                        smoothed_distortion[sid] * smoothed_fx_bypass_buffer[sid];

                    master_left_output_buffer[sid] =
                        left * (1.0f - distortion) +
                        1.33f * soft_clipping(left_add * 10) * (distortion);
                }
            }
        }
    }

  public:
    //==============================================================================
    COLD FilterProcessor(RuntimeNotifyer *const notifyer_, const MoniqueSynthData *synth_data_,
                         int id_, const float *const sine_lookup_, const float *const cos_lookup_,
                         const float *const exp_lookup_) noexcept
        : env(new ENV(notifyer_, synth_data_, synth_data_->filter_datas[id_]->env_data,
                      sine_lookup_, cos_lookup_, exp_lookup_)),
          input_envs(),

          id(id_),

          synth_data(synth_data_), filter_data(synth_data_->filter_datas[id_]),
          data_buffer(synth_data_->data_buffer),

          sin_lookup(synth_data->sine_lookup), cos_lookup(synth_data->cos_lookup)
    {
        for (int i = 0; i != SUM_INPUTS_PER_FILTER; ++i)
        {
            double_filter.add(new DoubleAnalogFilter(notifyer_));

            ENVData *input_env_data(synth_data_->filter_datas[id_]->input_envs[i]);
            input_env_datas.add(input_env_data);
            input_envs.add(new ENV(notifyer_, synth_data_, input_env_data, sine_lookup_,
                                   cos_lookup_, exp_lookup_));
        }
    }
    COLD ~FilterProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterProcessor)
};

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
//==============================================================================
//==============================================================================
//==============================================================================
static inline float get_low_pass_band_frequency(int band_id_, double sample_rate_) noexcept
{
    switch (band_id_)
    {
    case 0:
        return 82.41;
    case 1:
        return 164.81;
    case 2:
        return 329.63;
    case 3:
        return 659.25;
    case 4:
        return 1318.51;
    case 5:
        return 2637.02;
    default:
        // return 2637.02;
        return sample_rate_ / 2;
    }
}
static inline float get_high_pass_band_frequency(int band_id_) noexcept
{
    switch (band_id_)
    {
    case 0:
        return 20.60;
    case 1:
        return 82.41;
    case 2:
        return 164.81;
    case 3:
        return 329.63;
    case 4:
        return 659.25;
    case 5:
        return 1318.51;
    default:
        return 2637.02;
    }
}
class EQProcessor : public RuntimeListener
{
    friend class EQProcessorStereo;

    float frequency_low_pass[SUM_EQ_BANDS];
    float frequency_high_pass[SUM_EQ_BANDS];

    juce::OwnedArray<AnalogFilter> filters;
    juce::IIRFilter high_pass_filters[SUM_EQ_BANDS];

    friend class mono_ParameterOwnerStore;

    const MoniqueSynthData *const synth_data;
    const EQData *const eq_data;
    DataBuffer *const data_buffer;

  public:
    //==============================================================================
    inline void reset() noexcept
    {
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            filters[band_id]->reset();
            high_pass_filters[band_id].reset();
        }
    }

    void sample_rate_or_block_changed() noexcept override
    {
        reset();
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            frequency_low_pass[band_id] = get_low_pass_band_frequency(band_id, sample_rate);
            high_pass_filters[band_id].setCoefficients(
                juce::IIRCoefficients::makeHighPass(sample_rate, frequency_high_pass[band_id]));
        }
    }

    //==============================================================================
    inline void process(float *io_buffer_, int num_samples_) noexcept
    {
        {
            struct BandExecuter
            {
                const int band_id;

                const int num_samples_;
                const float *const smoothed_shape_buffer;

                const float filter_frequency;
                juce::IIRFilter &high_pass_filter;
                AnalogFilter &filter;

                const float *const filter_in_samples;
                float *const band_out_buffer;
                const float *const env_buffer;

                inline void exec() noexcept { exec_default(); }
                /*
inline void exec_first() noexcept
{
    // PROCESS
    for( int sid = 0 ; sid != num_samples_ ; ++sid )
    {
        const float shape = smoothed_shape_buffer[sid];
        const float amp = env_buffer[sid];
        const float in = filter_in_samples[sid] * amp;
        filter.update_with_fixed_cutoff( shape*0.8f, filter_frequency );
        float output = filter.processLowResonance(in);
        band_out_buffer[sid] = output*4;
    }
}
                */
                inline void exec_default() noexcept
                {
                    // PROCESS
                    for (int sid = 0; sid != num_samples_; ++sid)
                    {
                        const float shape = smoothed_shape_buffer[sid];
                        const float amp = env_buffer[sid];
                        const float in = filter_in_samples[sid] * amp;
                        filter.update_with_fixed_cutoff(shape * 0.8f, filter_frequency);
                        float output =
                            high_pass_filter.processSingleSampleRaw(filter.processLowResonance(in));
                        band_out_buffer[sid] = output * 4;
                    }
                }
                /*
inline void exec_last() noexcept
{
    // PROCESS
    for( int sid = 0 ; sid != num_samples_ ; ++sid )
    {
        const float shape = smoothed_shape_buffer[sid];
        const float amp = env_buffer[sid];
        const float in = filter_in_samples[sid] * amp;
        filter.update_with_fixed_cutoff( shape*0.8f, filter_frequency );
        float output = high_pass_filter.processSingleSampleRaw ( filter.processHighResonance(in) );
        band_out_buffer[sid] = output*1.5;
    }
}
                */
                BandExecuter(EQProcessor *const processor_, float *in_buffer_, int num_samples__,
                             int band_id_) noexcept
                    : band_id(band_id_),

                      num_samples_(num_samples__),
                      smoothed_shape_buffer(
                          processor_->synth_data->shape_smoother.get_smoothed_value_buffer()),

                      filter_frequency(processor_->frequency_low_pass[band_id_]),
                      high_pass_filter(processor_->high_pass_filters[band_id_]),
                      filter(*processor_->filters.getUnchecked(band_id_)),

                      filter_in_samples(in_buffer_),
                      band_out_buffer(
                          processor_->data_buffer->band_out_buffers.getWritePointer(band_id_)),
                      env_buffer(
                          processor_->data_buffer->band_env_buffers.getWritePointer(band_id_))
                {
                }
            };

            for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
            {
                BandExecuter(this, io_buffer_, num_samples_, band_id).exec();
            }
        }
        // EO MULTITHREADED

        // FINAL MIX - SINGLE THREADED ( NOT REALLY OPTIMIZED )
        {
            const float *const buffer_1(data_buffer->band_out_buffers.getReadPointer(0));
            const float *const buffer_2(data_buffer->band_out_buffers.getReadPointer(1));
            const float *const buffer_3(data_buffer->band_out_buffers.getReadPointer(2));
            const float *const buffer_4(data_buffer->band_out_buffers.getReadPointer(3));
            const float *const buffer_5(data_buffer->band_out_buffers.getReadPointer(4));
            const float *const buffer_6(data_buffer->band_out_buffers.getReadPointer(5));
            const float *const buffer_7(data_buffer->band_out_buffers.getReadPointer(6));

            // const float* const smoothed_distortion =
            // synth_data->final_clipping_smoother.get_smoothed_modulated_value_buffer() ;
            const float *const smoothed_distortion =
                synth_data->distortion_smoother.get_smoothed_value_buffer();
            const float *const smoothed_fx_bypass_buffer =
                synth_data->effect_bypass_smoother.get_smoothed_value_buffer();
            const float *const smoothed_bypass =
                eq_data->bypass_smoother.get_smoothed_value_buffer();
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                const float distortion = smoothed_distortion[sid] * smoothed_fx_bypass_buffer[sid];
                const float bypass = smoothed_bypass[sid];
                if (bypass > 0)
                {
                    float sum = sample_mix(
                        sample_mix(sample_mix(sample_mix(sample_mix(sample_mix(buffer_7[sid],
                                                                               buffer_6[sid]),
                                                                    buffer_5[sid]),
                                                         buffer_4[sid]),
                                              buffer_3[sid]),
                                   buffer_2[sid]),
                        buffer_1[sid] * -1);

                    // MONO_SNAP_TO_ZERO(sum)
                    float mix = sum * bypass + io_buffer_[sid] * (1.0f - bypass);
                    io_buffer_[sid] = soft_clipp_greater_1_2(
                        mix * (1.0f - distortion) + (std::atan(mix * 10) * 0.7f) * distortion);
                    // io_buffer_[sid] = soft_clipp_greater_1_2(sample_mix(mix*(1.0f-distortion),
                    // sample_mix( mix, mix )*distortion));
                }
                else
                {
                    io_buffer_[sid] = soft_clipp_greater_1_2(
                        io_buffer_[sid] * (1.0f - distortion) +
                        (std::atan(io_buffer_[sid] * 10) * 0.7f) * distortion);
                }
            }
        }
    }

  public:
    //==============================================================================
    COLD EQProcessor(RuntimeNotifyer *const notifyer_, MoniqueSynthData *synth_data_) noexcept
        : RuntimeListener(notifyer_),

          synth_data(synth_data_), eq_data(synth_data_->eq_data.get()),
          data_buffer(synth_data_->data_buffer)
    {
#ifdef JUCE_DEBUG
        std::cout << "MONIQUE: init EQ L OR R" << std::endl;
#endif

        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            filters.add(new AnalogFilter(notifyer_));

            frequency_low_pass[band_id] = get_low_pass_band_frequency(band_id, sample_rate);
            frequency_high_pass[band_id] = get_high_pass_band_frequency(band_id);
        }
        sample_rate_or_block_changed();
    }
    COLD ~EQProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQProcessor)
};

//==============================================================================
//==============================================================================
//==============================================================================
class EQProcessorStereo
{
    MoniqueSynthData *const synth_data;

    EQProcessor *const left_processor;
    EQProcessor *const right_processor;

    EQData *const eq_data;
    DataBuffer *const data_buffer;

  public:
    juce::OwnedArray<ENV> envs;

  public:
    //==============================================================================
    inline void start_attack() noexcept
    {
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            envs.getUnchecked(band_id)->start_attack();
        }
    }
    inline void start_release() noexcept
    {
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            envs.getUnchecked(band_id)->set_to_release();
        }
    }
    inline void reset() noexcept
    {
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            envs.getUnchecked(band_id)->reset();
        }
        left_processor->reset();
        if (synth_data->is_stereo)
            right_processor->reset();
    }

    //==============================================================================
    inline void process(int num_samples_) noexcept
    {
        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            ENV *env(envs.getUnchecked(band_id));
            eq_data->velocity_smoothers[band_id]->process_amp(
                !eq_data->hold[band_id], synth_data->glide_motor_time, env,
                data_buffer->band_env_buffers.getWritePointer(band_id), num_samples_);
        }

        left_processor->process(data_buffer->filter_stereo_output_samples.getWritePointer(LEFT),
                                num_samples_);
        if (synth_data->is_stereo)
            right_processor->process(
                data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT), num_samples_);

        if (Monique_Ui_AmpPainter *const amp_painter = synth_data->audio_processor->amp_painter)
        {
            amp_painter->add_eq(data_buffer->filter_stereo_output_samples.getWritePointer(LEFT),
                                num_samples_);
        }
    }

  public:
    //==========================================================================
    COLD EQProcessorStereo(RuntimeNotifyer *const notifyer_, MoniqueSynthData *synth_data_,
                           const float *const sine_lookup_, const float *const cos_lookup_,
                           const float *const exp_lookup_) noexcept
        : synth_data(synth_data_), left_processor(new EQProcessor(notifyer_, synth_data_)),
          right_processor(new EQProcessor(notifyer_, synth_data_)),
          eq_data(synth_data_->eq_data.get()), data_buffer(synth_data_->data_buffer)
    {
#ifdef JUCE_DEBUG
        std::cout << "MONIQUE: init EQ" << std::endl;
#endif

        for (int band_id = 0; band_id != SUM_EQ_BANDS; ++band_id)
        {
            envs.add(new ENV(notifyer_, synth_data_,
                             synth_data_->eq_data->envs.getUnchecked(band_id), sine_lookup_,
                             cos_lookup_, exp_lookup_));
        }
    }
    COLD ~EQProcessorStereo() noexcept
    {
        delete right_processor;
        delete left_processor;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQProcessorStereo)
};

//==============================================================================
//==============================================================================
//==============================================================================
class AllPassFilter
{
    juce::HeapBlock<float> buffer;
    int bufferSize, bufferIndex;

  public:
    //==============================================================================
    inline float process(const float input) noexcept
    {
        const float bufferedValue = buffer[bufferIndex];
        float temp = input + (bufferedValue * 0.5f);
        JUCE_UNDENORMALISE(temp);
        buffer[bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;

        return bufferedValue - input;
    }

    //==============================================================================
    COLD void setSize(const int size)
    {
        if (size != bufferSize)
        {
            bufferIndex = 0;
            buffer.malloc((size_t)size);
            bufferSize = size;
        }

        clear();
    }
    COLD void clear() noexcept { buffer.clear((size_t)bufferSize); }

  public:
    //==============================================================================
    COLD AllPassFilter() noexcept : bufferSize(0), bufferIndex(0) {}
    COLD ~AllPassFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AllPassFilter)
};

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
class mono_Chorus : public RuntimeListener
{
    MoniqueSynthData *const synth_data;
    ChorusData *const chorus_data;

    mono_SineWaveAutonom osc_1;
    mono_SineWaveAutonom osc_2;
    mono_SineWaveAutonom osc_3;
    mono_SineWaveAutonom osc_4;
    mono_SineWaveAutonom osc_5;

    int buffer_size;
    int index;
    mono_AudioSampleBuffer<2> data_buffer;
    float *current_left_buffer;
    float *current_right_buffer;

    const float *const sin_lookup;
    const float *const cos_lookup;

  public:
#define SUM_DELAY_LINES 4
    inline void process(float *left_in_, float *right_in_, float *left_out_, float *right_out_,
                        int num_samples_) noexcept
    {
        const float *const chorus_env_buffer(
            chorus_data->modulation_smoother.get_smoothed_value_buffer());
        const float *const smoothed_pan_buffer(
            chorus_data->pan_smoother.get_smoothed_value_buffer());

        const float e_samples = sample_rate / 164.81; // 82.41;
        const float e2_samples = sample_rate / 165.91;
        for (int sid = 0; sid != num_samples_; ++sid)
        {
            // const float power = (exp( (chorus_env_buffer[sid] *0.85f) *2)-1)/6.38906;
            const float power = chorus_env_buffer[sid] * 0.8f;
            const float amps[SUM_DELAY_LINES] = {
                ((osc_1.tick() * 0.7f + osc_2.tick() * 0.3f) + 1) * 0.5f,
                ((osc_1.lastOut_with_phase_offset(0.25f) * 0.3f + osc_3.tick() * 0.6f) + 1) * 0.5f,
                ((osc_1.lastOut_with_phase_offset(0.5f) * 0.4f + osc_4.tick() * 0.5f) + 1) * 0.5f,
                ((osc_1.lastOut_with_phase_offset(0.75f) * 0.6f + osc_5.tick() * 0.4f) + 1) * 0.5f};
            const float pan = smoothed_pan_buffer[sid];

#define CHECK_MAKE_INDEX_VALID(floated_index_, size_)                                              \
    if (floated_index_ >= size_)                                                                   \
    {                                                                                              \
        floated_index_ -= size_;                                                                   \
    }                                                                                              \
    else if (floated_index_ < 0)                                                                   \
    {                                                                                              \
        floated_index_ += size_;                                                                   \
    }
#define CHECK_MAKE_INDEX_VALID_upper_only(floated_index_, size_)                                   \
    if (floated_index_ >= size_)                                                                   \
    {                                                                                              \
        floated_index_ -= size_;                                                                   \
    }
            // L
            const float fade_in = 1.0f - (juce::jmin(1.0f, power * 2));
            const float fade_effect = (juce::jmin(1.0f, power * 2));
            {
                float result_l = 0;
                for (int i = 0; i != SUM_DELAY_LINES; ++i)
                {
                    float delay = (power * 0.9 + 0.1) * e_samples * amps[i];
                    float float_index = float(index) - delay + 1;
                    CHECK_MAKE_INDEX_VALID(float_index, buffer_size)

                    const int index_1 = std::floor(float_index);
                    int index_2 = index_1 + 1;
                    CHECK_MAKE_INDEX_VALID_upper_only(index_2, buffer_size) const float delta =
                        float_index - index_1;

                    result_l += (current_left_buffer[index_1] * (1.0f - delta) +
                                 current_left_buffer[index_2] * delta) /
                                (i + 2);
                }
                {
                    current_left_buffer[index] =
                        sample_mix(left_in_[sid], result_l * power * left_pan(pan, sin_lookup));
                    left_out_[sid] = left_in_[sid] * fade_in + result_l * fade_effect;
                }
            }
            // R
            if (synth_data->is_stereo)
            {
                float result_r = 0;
                for (int i = 0; i != SUM_DELAY_LINES; ++i)
                {
                    float delay = (power * 0.9 + 0.1) * e2_samples * amps[i];
                    float float_index = float(index) - delay + 1;
                    CHECK_MAKE_INDEX_VALID(float_index, buffer_size)

                    const int index_1 = std::floor(float_index);
                    int index_2 = index_1 + 1;
                    CHECK_MAKE_INDEX_VALID_upper_only(index_2, buffer_size) const float delta =
                        float_index - index_1;

                    result_r += (current_right_buffer[index_1] * (1.0f - delta) +
                                 current_right_buffer[index_2] * delta) /
                                (i + 2);
                }
                {
                    current_right_buffer[index] =
                        sample_mix(right_in_[sid], result_r * power * right_pan(pan, cos_lookup));
                    right_out_[sid] = right_in_[sid] * fade_in + result_r * fade_effect;
                }
            }

            index = (index + 1) % buffer_size;
        }
    }

    inline void reset() noexcept { data_buffer.clear(); }
    //==============================================================================
    COLD void sample_rate_or_block_changed() noexcept override
    {
        buffer_size = sample_rate / 82.41;
        data_buffer.setSize(buffer_size, false);
        data_buffer.clear();

        current_left_buffer = data_buffer.getWritePointer(LEFT);
        current_right_buffer = data_buffer.getWritePointer(RIGHT);
    }

  public:
    //==============================================================================
    COLD mono_Chorus(RuntimeNotifyer *const notifyer_, MoniqueSynthData *const synth_data_) noexcept
        : RuntimeListener(notifyer_),

          synth_data(synth_data_),

          chorus_data(synth_data_->chorus_data.get()), osc_1(notifyer_, synth_data_->sine_lookup),
          osc_2(notifyer_, synth_data_->sine_lookup), osc_3(notifyer_, synth_data_->sine_lookup),
          osc_4(notifyer_, synth_data_->sine_lookup),

          osc_5(notifyer_, synth_data_->sine_lookup), buffer_size(1), index(0),

          data_buffer(buffer_size),

          sin_lookup(synth_data_->sine_lookup), cos_lookup(synth_data_->cos_lookup)
    {
        sample_rate_or_block_changed();
        osc_1.set_frequency(0.4);
        osc_2.set_frequency(0.6);
        osc_3.set_frequency(0.55);
        osc_4.set_frequency(0.5);
        osc_5.set_frequency(0.45);
    }
    COLD ~mono_Chorus() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_Chorus)
};

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
class mono_Delay : public RuntimeListener
{
    const MoniqueSynthData *synth_data;
    RuntimeInfo *info;

    double last_bmp_in;

    int reflexion_write_index;
    int last_in_reflexion_size;
    int reflexion;
    int current_reflexion;
    int reflexion_buffer_size;
    int current_reflexion_buffer_size;
    mono_AudioSampleBuffer<2> reflexion_buffer;
    float *active_left_reflexion_buffer;
    float *active_right_reflexion_buffer;

    int record_index;
    int last_in_record_size;
    int record_buffer_size;
    int real_record_buffer_size;
    int num_records_to_write;
    mono_AudioSampleBuffer<2> record_buffer;
    float *active_left_record_buffer;
    float *active_right_record_buffer;
    bool force_clear;

    LinearSmootherMinMax<0, 1> record_switch_smoother;

    const float *const sin_lookup;
    const float *const cos_lookup;

  public:
    //==============================================================================
    inline void set_reflexion_size(int reflexion_in_size_, int record_buffer_size_,
                                   int glide_time_in_ms_, double bpm_) noexcept
    {
        // SETUP THE REFLEXION BUFFER
        if (bpm_ < 20)
        {
            return;
        }

        bool bpm_changed = last_bmp_in != bpm_;
        last_bmp_in = bpm_;

        if (bpm_changed || last_in_reflexion_size != reflexion_in_size_)
        {
            last_in_reflexion_size = reflexion_in_size_;
            update_reflexion_stuff(bpm_);
        }
        if (bpm_changed || last_in_record_size != record_buffer_size_)
        {
            last_in_record_size = record_buffer_size_;
            update_record_stuff(bpm_);
        }

        // TODO ATTENTION FIX, do not clear! -> but if not crash on sample rate changes
        /*
        if( bpm_changed )
        {
            reflexion_write_index = 0;
            current_reflexion = reflexion;
            reflexion_buffer.clear();
            record_buffer.clear();
            record_switch_smoother.set_value(0);
            record_switch_smoother.reset_glide_countdown();
            record_switch_smoother.reset(sample_rate,glide_time_in_ms_);
        }
        */

        record_switch_smoother.reset_coefficients(sample_rate, juce::jmax(200, glide_time_in_ms_));
    }

    //==============================================================================
  private:
    inline void update_reflexion_stuff(double bpm_) noexcept
    {
        // CALCULATE THE NEEDED SIZE
        {
            const double speed_multi = delay_multi(last_in_reflexion_size);
            const double bars_per_sec = bpm_ / 4 / 60;
            const double samples_per_bar = (1.0f / bars_per_sec) * sample_rate;
            reflexion_buffer_size = floor(samples_per_bar);
            reflexion = samples_per_bar * speed_multi;
        }
        if (reflexion_buffer.get_size() < reflexion_buffer_size)
        {
            reflexion_buffer.setSize(reflexion_buffer_size, true);
            active_left_reflexion_buffer = reflexion_buffer.getWritePointer(LEFT);
            active_right_reflexion_buffer = reflexion_buffer.getWritePointer(RIGHT);
        }
    }
    inline void update_record_stuff(double bpm_) noexcept
    {
        // CALCULATE THE NEEDED SIZE
        {
            num_records_to_write = delay_multi(last_in_record_size); // 1, 2 or 4
            const double bars_per_sec = bpm_ / 4 / 60;
            const double samples_per_bar = (1.0f / bars_per_sec) * sample_rate;
            real_record_buffer_size = samples_per_bar * 4;
            record_buffer_size = samples_per_bar;
            // TODO change the record index only on zero
        }
        if (record_buffer.get_size() < real_record_buffer_size)
        {
            record_buffer.setSize(real_record_buffer_size, true);
            active_left_record_buffer = record_buffer.getWritePointer(LEFT);
            active_right_record_buffer = record_buffer.getWritePointer(RIGHT);
        }
    }

  private:
    //==============================================================================
    inline int update_get_reflexion_read_index() noexcept
    {
        if (current_reflexion < reflexion)
        {
            current_reflexion += 2;
            if (current_reflexion > reflexion)
            {
                current_reflexion = reflexion;
            }
        }
        else if (current_reflexion > reflexion)
        {
            --current_reflexion;
        }

        if (current_reflexion_buffer_size < reflexion_buffer_size)
        {
            current_reflexion_buffer_size += 2;
            if (current_reflexion_buffer_size > reflexion_buffer_size)
            {
                current_reflexion_buffer_size = reflexion_buffer_size;
            }
        }
        else if (current_reflexion_buffer_size > reflexion_buffer_size)
        {
            --current_reflexion_buffer_size;
        }

        int read_index = reflexion_write_index - current_reflexion;
        while (read_index < 0)
        {
            read_index += current_reflexion_buffer_size;
        }
        return read_index;
    }

  public:
    //==============================================================================
    inline void process(float *const io_l, float *const io_r, const float *smoothed_power_,
                        const float *smoothed_pan_buffer_, const float *record_release_buffer_,
                        bool record_, const int num_samples_) noexcept
    {
        // PREPARE RECORDING
        record_ = force_clear ? false : record_;
        if (!record_)
        {
            record_switch_smoother.set_value(false);
            record_switch_smoother.reset_glide_countdown();
        }

        // CURRENT INPUT AND REFLEXION OF USER SIZE
        if (synth_data->is_stereo)
        {
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                // REFLEXION AND INPUT
                const int reflexion_read_index = update_get_reflexion_read_index();

                const float left_reflexion_and_input_mix =
                    sample_mix(active_left_reflexion_buffer[reflexion_read_index], io_l[sid]);
                const float right_reflexion_and_input_mix =
                    sample_mix(active_right_reflexion_buffer[reflexion_read_index], io_r[sid]);
                {
                    const float pan = smoothed_pan_buffer_[sid];
                    const float power = smoothed_power_[sid];

                    const float left = left_pan(pan, sin_lookup);
                    const float right = right_pan(pan, cos_lookup);
                    // SWAPPED L AND R HERE - Must be wrong somethere else
                    active_left_reflexion_buffer[reflexion_write_index] =
                        left_reflexion_and_input_mix * power * right;
                    active_right_reflexion_buffer[reflexion_write_index] =
                        right_reflexion_and_input_mix * power * left;
                }

                // RECORD AND MIX BEFORE
                {
                    float record_power = 0;
                    if (record_)
                    {
                        record_power = record_switch_smoother.glide_tick(true);
                    }
                    else if (!record_switch_smoother.is_up_to_date())
                    {
                        record_power = record_switch_smoother.tick();
                    }

                    const float left_record = active_left_record_buffer[record_index];
                    const float right_record = active_right_record_buffer[record_index];
                    if (record_power > 0)
                    {
                        const float record_release = record_release_buffer_[sid];
                        const float left_record_feedback =
                            left_reflexion_and_input_mix * record_power;
                        const float right_record_feedback =
                            right_reflexion_and_input_mix * record_power;
                        if (num_records_to_write > 2)
                        {
                            active_left_record_buffer[record_index] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                            active_right_record_buffer[record_index] =
                                sample_mix(right_record, right_record_feedback) * record_release;
                        }
                        else if (num_records_to_write == 1)
                        {
                            const int record_index_1 = record_index;
                            int record_index_2 = record_index_1 + record_buffer_size;
                            if (record_index_2 >= real_record_buffer_size)
                            {
                                record_index_2 -= real_record_buffer_size;
                            }
                            int record_index_3 = record_index_2 + record_buffer_size;
                            if (record_index_3 >= real_record_buffer_size)
                            {
                                record_index_3 -= real_record_buffer_size;
                            }
                            int record_index_4 = record_index_3 + record_buffer_size;
                            if (record_index_4 >= real_record_buffer_size)
                            {
                                record_index_4 -= real_record_buffer_size;
                            }

                            active_left_record_buffer[record_index_1] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                            active_right_record_buffer[record_index_1] =
                                sample_mix(right_record, right_record_feedback) * record_release;
                            active_left_record_buffer[record_index_2] =
                                sample_mix(active_left_record_buffer[record_index_2],
                                           left_record_feedback) *
                                record_release;
                            active_right_record_buffer[record_index_2] =
                                sample_mix(active_right_record_buffer[record_index_2],
                                           right_record_feedback) *
                                record_release;
                            active_left_record_buffer[record_index_3] =
                                sample_mix(active_left_record_buffer[record_index_3],
                                           left_record_feedback) *
                                record_release;
                            active_right_record_buffer[record_index_3] =
                                sample_mix(active_right_record_buffer[record_index_3],
                                           right_record_feedback) *
                                record_release;
                            active_left_record_buffer[record_index_4] =
                                sample_mix(active_left_record_buffer[record_index_4],
                                           left_record_feedback) *
                                record_release;
                            active_right_record_buffer[record_index_4] =
                                sample_mix(active_right_record_buffer[record_index_4],
                                           right_record_feedback) *
                                record_release;
                        }
                        else // if( num_records_to_write == 2 )
                        {
                            int record_index_2 = record_index + record_buffer_size * 2;
                            if (record_index_2 >= real_record_buffer_size)
                            {
                                record_index_2 -= real_record_buffer_size;
                            }

                            active_left_record_buffer[record_index] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                            active_right_record_buffer[record_index] =
                                sample_mix(right_record, right_record_feedback) * record_release;
                            active_left_record_buffer[record_index_2] =
                                sample_mix(active_left_record_buffer[record_index_2],
                                           left_record_feedback) *
                                record_release;
                            active_right_record_buffer[record_index_2] =
                                sample_mix(active_right_record_buffer[record_index_2],
                                           right_record_feedback) *
                                record_release;
                        }
                    }
                    else if (force_clear)
                    {
                        record_buffer.clear();
                        active_left_record_buffer = record_buffer.getWritePointer(LEFT);
                        active_right_record_buffer = record_buffer.getWritePointer(RIGHT);
                        force_clear = false;
                    }

                    io_l[sid] = sample_mix(left_record, left_reflexion_and_input_mix);
                    io_r[sid] = sample_mix(right_record, right_reflexion_and_input_mix);
                }

                // UPDATE INDEX
                {
                    reflexion_write_index =
                        (reflexion_write_index + 1) % current_reflexion_buffer_size;
                    record_index = (record_index + 1) % real_record_buffer_size;
                }
            }
        }
        else // NOTE just a reduced copy of the function before
        {
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                // REFLEXION AND INPUT
                const int reflexion_read_index = update_get_reflexion_read_index();
                const float left_reflexion_and_input_mix =
                    sample_mix(active_left_reflexion_buffer[reflexion_read_index], io_l[sid]);
                {
                    active_left_reflexion_buffer[reflexion_write_index] =
                        left_reflexion_and_input_mix * smoothed_power_[sid];
                }

                // RECORD AND MIX BEFORE
                {
                    float record_power = 0;
                    if (record_)
                    {
                        record_power = record_switch_smoother.glide_tick(true);
                    }
                    else if (!record_switch_smoother.is_up_to_date())
                    {
                        record_power = record_switch_smoother.tick();
                    }

                    const float left_record = active_left_record_buffer[record_index];
                    if (record_power > 0)
                    {
                        const float record_release = record_release_buffer_[sid];
                        const float left_record_feedback =
                            left_reflexion_and_input_mix * record_power;
                        if (num_records_to_write > 2)
                        {
                            active_left_record_buffer[record_index] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                        }
                        else if (num_records_to_write == 1)
                        {
                            const int record_index_1 = record_index;
                            int record_index_2 = record_index_1 + record_buffer_size;
                            if (record_index_2 >= real_record_buffer_size)
                            {
                                record_index_2 -= real_record_buffer_size;
                            }
                            int record_index_3 = record_index_2 + record_buffer_size;
                            if (record_index_3 >= real_record_buffer_size)
                            {
                                record_index_3 -= real_record_buffer_size;
                            }
                            int record_index_4 = record_index_3 + record_buffer_size;
                            if (record_index_4 >= real_record_buffer_size)
                            {
                                record_index_4 -= real_record_buffer_size;
                            }

                            active_left_record_buffer[record_index_1] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                            active_left_record_buffer[record_index_2] =
                                sample_mix(active_left_record_buffer[record_index_2],
                                           left_record_feedback) *
                                record_release;
                            active_left_record_buffer[record_index_3] =
                                sample_mix(active_left_record_buffer[record_index_3],
                                           left_record_feedback) *
                                record_release;
                            active_left_record_buffer[record_index_4] =
                                sample_mix(active_left_record_buffer[record_index_4],
                                           left_record_feedback) *
                                record_release;
                        }
                        else // if( num_records_to_write == 2 )
                        {
                            int record_index_2 = record_index + record_buffer_size * 2;
                            if (record_index_2 >= real_record_buffer_size)
                            {
                                record_index_2 -= real_record_buffer_size;
                            }

                            active_left_record_buffer[record_index] =
                                sample_mix(left_record, left_record_feedback) * record_release;
                            active_left_record_buffer[record_index_2] =
                                sample_mix(active_left_record_buffer[record_index_2],
                                           left_record_feedback) *
                                record_release;
                        }
                    }
                    else if (force_clear)
                    {
                        record_buffer.clear();
                        active_left_record_buffer = record_buffer.getWritePointer(LEFT);
                        force_clear = false;
                    }

                    io_l[sid] = sample_mix(left_record, left_reflexion_and_input_mix);
                }

                // UPDATE INDEX
                {
                    reflexion_write_index =
                        (reflexion_write_index + 1) % current_reflexion_buffer_size;
                    record_index = (record_index + 1) % real_record_buffer_size;
                }
            }
        }
    }

    //==============================================================================
    inline void reset() noexcept { sample_rate_or_block_changed(); }

    //==============================================================================
    inline void clear_record_buffer() noexcept
    {
        force_clear = true;
        active_left_record_buffer = record_buffer.getWritePointer(LEFT);
        active_right_record_buffer = record_buffer.getWritePointer(RIGHT);
    }

    inline int get_max_duration() const noexcept { return real_record_buffer_size; }

  private:
    //==============================================================================
    COLD void sample_rate_or_block_changed() noexcept override
    {
        update_record_stuff(last_bmp_in);
        update_reflexion_stuff(last_bmp_in);
    }

  public:
    //==============================================================================
    COLD mono_Delay(RuntimeNotifyer *notifyer_, MoniqueSynthData *synth_data_) noexcept
        : RuntimeListener(notifyer_),

          synth_data(synth_data_),

          last_bmp_in(20),

          reflexion_write_index(0), last_in_reflexion_size(0), reflexion(0), current_reflexion(0),
          reflexion_buffer_size(1), current_reflexion_buffer_size(1),
          reflexion_buffer(reflexion_buffer_size),
          active_left_reflexion_buffer(reflexion_buffer.getWritePointer(LEFT)),
          active_right_reflexion_buffer(reflexion_buffer.getWritePointer(RIGHT)),

          record_index(0), last_in_record_size(0), record_buffer_size(1),
          real_record_buffer_size(record_buffer_size), num_records_to_write(1),
          record_buffer(real_record_buffer_size),
          active_left_record_buffer(record_buffer.getWritePointer(LEFT)),
          active_right_record_buffer(record_buffer.getWritePointer(RIGHT)), force_clear(false),

          record_switch_smoother(),

          sin_lookup(synth_data_->sine_lookup), cos_lookup(synth_data_->sine_lookup)
    {
        sample_rate_or_block_changed();
        record_switch_smoother.set_value(0);
    }
    COLD ~mono_Delay() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_Delay)
};

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
class CombFilter
{
    juce::HeapBlock<float> buffer;
    float last;
    int bufferSize, bufferIndex;

  public:
    //==============================================================================
    inline float process(const float input, const float feedbackLevel) noexcept
    {
#define REVERB_DAMP 0
        last = buffer[bufferIndex];
        // last = (output * (1.0f - REVERB_DAMP)) + (last * REVERB_DAMP);
        // JUCE_UNDENORMALISE (last);

        float temp = input + (last * feedbackLevel);
        JUCE_UNDENORMALISE(temp);
        buffer[bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;

        return last;
    }

    //==============================================================================
    COLD void setSize(const int size)
    {
        if (size != bufferSize)
        {
            bufferIndex = 0;
            buffer.malloc((size_t)size);
            bufferSize = size;
        }

        clear();
    }
    COLD void clear() noexcept
    {
        last = 0;
        buffer.clear((size_t)bufferSize);
    }

  public:
    //==============================================================================
    COLD CombFilter() noexcept : last(0), bufferSize(0), bufferIndex(0) {}
    COLD ~CombFilter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CombFilter)
};

//==============================================================================
//==============================================================================
//==============================================================================
class LinearSmoothedValue
{
    float currentValue, target, step, lastValue;
    int countdown, stepsToTarget;

  public:
    //==========================================================================
    inline float getNextValue() noexcept
    {
        if (countdown <= 0)
        {
            lastValue = target;
        }
        else
        {
            --countdown;
            currentValue += step;

            lastValue = currentValue;
        }

        return lastValue;
    }
    //==========================================================================
    inline float getLastValue() noexcept { return lastValue; }

    inline void setValue(float newValue) noexcept
    {
        if (target != newValue)
        {
            target = newValue;
            countdown = stepsToTarget;

            if (countdown <= 0)
            {
                currentValue = target;
            }
            else
            {
                step = (target - currentValue) / countdown;
            }
        }
    }

    //==============================================================================
    COLD void reset(float sampleRate, float fadeLengthSeconds) noexcept
    {
        stepsToTarget = std::floor(fadeLengthSeconds * sampleRate);
        currentValue = target;
        countdown = 0;
    }

  public:
    //==============================================================================
    COLD LinearSmoothedValue() noexcept
        : currentValue(0), target(0), step(0), lastValue(0), countdown(0), stepsToTarget(0)
    {
    }

    COLD ~LinearSmoothedValue() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSmoothedValue)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Reverb;
struct ReverbParameters
{
    float roomSize; /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
    float wetLevel; /**< Wet level, 0 to 1.0 */
    float dryLevel; /**< Dry level, 0 to 1.0 */
    float width;    /**< mono_Reverb width, 0 to 1.0, where 1.0 is very wide. */

  private:
    //==============================================================================
    friend class mono_Reverb;
    COLD ReverbParameters() noexcept : roomSize(0.5f), wetLevel(0.33f), dryLevel(0.4f), width(1.0f)
    {
    }
    COLD ~ReverbParameters() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbParameters)
};

//==============================================================================
//==============================================================================
//==============================================================================
class mono_Reverb : RuntimeListener
{
    const bool left_or_right;

    enum
    {
        numCombs = 8,
        numAllPasses = 4
    };

    CombFilter comb[numCombs];
    AllPassFilter allPass[numAllPasses];

    ReverbParameters parameters;
#define REVERB_GAIN 0.013f

    float wetGain1, wetGain2, feedback; // dryGain,  feedback

  public:
    //==========================================================================
    inline float processSingleSampleRaw(float in) noexcept
    {
        float out = 0;
        {
            const float input = in * REVERB_GAIN;
            for (int j = 0; j != numCombs; ++j) // accumulate the comb filters in parallel
            {
                out += comb[j].process(input, feedback);
            }
        }
        for (int j = 0; j != numAllPasses; ++j) // run the allpass filters in series
        {
            out = allPass[j].process(out);
        }

        return out * wetGain1 + out * wetGain2 + in * parameters.dryLevel;
    }

    //==========================================================================
    inline ReverbParameters &get_parameters() noexcept { return parameters; }
    inline void update_parameters() noexcept
    {
#define ROOM_SCALE 1.0f
#define WET_SCALE_FACTOR 3.0f
#define DRY_SCALE_FACTOR 1.0f
#define ROOM_SCALE_FACTOR (0.28f * ROOM_SCALE)
#define ROOM_OFFSET (1.0f - 0.28f * ROOM_SCALE - 0.02f)
        const float wet = parameters.wetLevel * WET_SCALE_FACTOR;
        // dryGain.setValue (parameters.dryLevel * DRY_SCALE_FACTOR);
        wetGain1 = 0.5f * wet * (1.0f + parameters.width);
        wetGain2 = 0.5f * wet * (1.0f - parameters.width);
        feedback = parameters.roomSize * ROOM_SCALE_FACTOR + ROOM_OFFSET;
    }

    //==============================================================================
    COLD void sample_rate_or_block_changed() noexcept override
    {
        static const int combTunings[] = {1116, 1188, 1277, 1356,
                                          1422, 1491, 1557, 1617}; // (at 44100Hz)
        static const int allPassTunings[] = {556, 441, 341, 225};
        const int stereoSpread = 23;
        const int intSampleRate = (int)sample_rate;

        for (int i = 0; i < numCombs; ++i)
        {
            if (left_or_right == LEFT)
            {
                comb[i].setSize((intSampleRate * combTunings[i] * ROOM_SCALE) / 44100);
            }
            else
            {
                comb[i].setSize(
                    (intSampleRate * (combTunings[i] * ROOM_SCALE + stereoSpread * ROOM_SCALE)) /
                    44100);
            }
        }
        for (int i = 0; i < numAllPasses; ++i)
        {
            if (left_or_right == LEFT)
            {
                allPass[i].setSize((intSampleRate * allPassTunings[i] * ROOM_SCALE) / 44100);
            }
            else
            {
                allPass[i].setSize(
                    (intSampleRate * (allPassTunings[i] * ROOM_SCALE + stereoSpread * ROOM_SCALE)) /
                    44100);
            }
        }
    }
    COLD void reset() noexcept
    {
        for (int i = 0; i < numCombs; ++i)
        {
            comb[i].clear();
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[i].clear();
        }
    }

  public:
    //==========================================================================
    COLD mono_Reverb(RuntimeNotifyer *const notifyer_, bool left_or_right_) noexcept
        : RuntimeListener(notifyer_), left_or_right(left_or_right_), wetGain1(0), wetGain2(0),
          feedback(0)
    {
        update_parameters();
        sample_rate_or_block_changed();
    }
    COLD ~mono_Reverb() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(mono_Reverb)
};

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
class ZeroInCounter
{
    int count_zeros;

  public:
    //==============================================================================
    void add(float value) noexcept
    {
        if (value > -0.0001 && value < 0.0001)
        {
            count_zeros++;
        }
        else
        {
            count_zeros = 0;
        }
    }

    int get_num_zeros() const noexcept { return count_zeros; }

    void reset() noexcept { count_zeros = 0; }

  public:
    //==============================================================================
    COLD ZeroInCounter() noexcept : count_zeros(0) {}
    COLD ~ZeroInCounter() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZeroInCounter)
};
class FXProcessor
{
    // DELAY
    mono_Delay delay;

    // REVERB
    mono_Reverb reverb_l;
    mono_Reverb reverb_r;

    // CHORUS
    mono_Chorus chorus;
    friend class mono_ParameterOwnerStore;

    // FINAL ENV
    friend class MoniqueSynthesiserVoice;
    std::unique_ptr<ENV> final_env;

  public:
    Smoother velocity_smoother;
    int zero_samples_counter;
    LinearSmootherMinMax<false, true> *const bypass_smoother;

  private:
    const MoniqueSynthData *const synth_data;
    DataBuffer *const data_buffer;
    const ReverbData *const reverb_data;
    ChorusData *const chorus_data;

    const float *const sin_lookup;
    const float *const cos_lookup;

  public:
    //==========================================================================
    inline void process(juce::AudioSampleBuffer &output_buffer_, const float *velocity_,
                        const int start_sample_final_out_, const int num_samples_) noexcept
    {
        velocity_smoother.set_size_in_ms(synth_data->velocity_glide_time);

        if (synth_data->is_stereo)
        {
            // COLLECT BUFFERS
            float *left_input_buffer =
                data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);
            float *right_input_buffer =
                data_buffer->filter_stereo_output_samples.getWritePointer(RIGHT);

            float *left_out_buffer = &output_buffer_.getWritePointer(LEFT)[start_sample_final_out_];
            const bool is_stereo = output_buffer_.getNumChannels() > 1;
            float *right_out_buffer =
                is_stereo ? &output_buffer_.getWritePointer(RIGHT)[start_sample_final_out_]
                          : data_buffer->second_mono_buffer.getWritePointer();

            // PREPARE
            {
                float *const final_env_amp = data_buffer->final_env.getWritePointer();
                final_env->process(final_env_amp, num_samples_);

                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    const float gain =
                        final_env_amp[sid] * velocity_smoother.add_and_get_average(velocity_[sid]);
                    left_input_buffer[sid] *= gain;
                    right_input_buffer[sid] *= gain;
                }
            }

            // STEREO CHORUS
            {
                chorus.process(left_input_buffer, right_input_buffer, left_out_buffer,
                               right_out_buffer, num_samples_);
            }

            // DELAY
            {
                delay.set_reflexion_size(synth_data->delay_refexion, synth_data->delay_record_size,
                                         synth_data->glide_motor_time,
                                         is_standalone() ? synth_data->speed
                                                         : synth_data->runtime_info->bpm);

                delay.process(left_out_buffer, right_out_buffer,
                              synth_data->delay_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_pan_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_record_release_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_record, num_samples_);
            }

            // REVERB
            {
                const float *const smoothed_pan_buffer(
                    reverb_data->pan_smoother.get_smoothed_value_buffer());
                const float *const smoothed_bypass_buffer =
                    synth_data->effect_bypass_smoother.get_smoothed_value_buffer();

                const float *const smoothed_room_buffer =
                    reverb_data->room_smoother.get_smoothed_value_buffer();
                const float *const smoothed_with_buffer =
                    reverb_data->width_smoother.get_smoothed_value_buffer();
                const float *const smoothed_dry_wet_mix_buffer =
                    reverb_data->dry_wet_mix_smoother.get_smoothed_value_buffer();
                ReverbParameters &rever_params_l = reverb_l.get_parameters();
                ReverbParameters &rever_params_r = reverb_r.get_parameters();
                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    {
                        const float reverb_room = smoothed_room_buffer[sid];
                        const float reverb_dry_wet_mix = 1.0f - smoothed_dry_wet_mix_buffer[sid];
                        const float reverb_width = smoothed_with_buffer[sid];
                        if (rever_params_l.roomSize != reverb_room ||
                            rever_params_l.dryLevel != reverb_dry_wet_mix
                            //|| rever_params_r.wetLevel != r_params.wetLevel
                            || rever_params_l.width != reverb_width)
                        {
                            rever_params_l.roomSize = reverb_room;
                            rever_params_l.dryLevel = reverb_dry_wet_mix;
                            rever_params_l.wetLevel = 1.0f - reverb_dry_wet_mix;
                            rever_params_l.width = reverb_width;

                            rever_params_r.roomSize = rever_params_l.roomSize;
                            rever_params_r.dryLevel = rever_params_l.dryLevel;
                            rever_params_r.wetLevel = rever_params_l.wetLevel;
                            rever_params_r.width = rever_params_l.width;

                            reverb_l.update_parameters();
                            reverb_r.update_parameters();
                        }

                        const float in_l = left_out_buffer[sid];
                        const float in_r = right_out_buffer[sid];
                        float sample_l = reverb_l.processSingleSampleRaw(in_l);
                        float sample_r = reverb_r.processSingleSampleRaw(in_r);

                        const float pan = smoothed_pan_buffer[sid];
                        const float left = left_pan(pan, sin_lookup);
                        const float right = right_pan(pan, cos_lookup);
                        const float bypass = smoothed_bypass_buffer[sid];
                        left_out_buffer[sid] =
                            sample_mix((sample_l * left + in_l * (1.0f - left)) * bypass,
                                       left_input_buffer[sid] * (1.0f - bypass));
                        right_out_buffer[sid] =
                            sample_mix((sample_r * right + in_r * (1.0f - right)) * bypass,
                                       right_input_buffer[sid] * (1.0f - bypass));
                    }
                }
            }

            // PROCESS
            {
                // FINAL MIX
                {
                    const float *const smoothed_volume_buffer =
                        synth_data->volume_smoother.get_smoothed_value_buffer();
                    if (is_stereo)
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            float left_in = left_out_buffer[sid];
                            float right_in = right_out_buffer[sid];
#define MONO_UNDENORMALISE_OUTPUT(n)                                                               \
    if (!(n < -1.0e-3f || n > 1.0e-3f))                                                            \
        n = 0;
                            MONO_UNDENORMALISE_OUTPUT(left_in);
                            MONO_UNDENORMALISE_OUTPUT(right_in);
                            if (left_in != 0 || right_in != 0)
                            {
                                zero_samples_counter = 0;
                            }
                            else
                            {
                                zero_samples_counter++;
                            }

                            const float volume =
                                smoothed_volume_buffer[sid] * bypass_smoother->tick();
                            left_out_buffer[sid] *= volume * 2;
                            right_out_buffer[sid] *= volume * 2;
                        }
                    }
                    else
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            float left_in = left_out_buffer[sid];
                            float right_in = right_out_buffer[sid];
                            MONO_UNDENORMALISE_OUTPUT(left_in);
                            MONO_UNDENORMALISE_OUTPUT(right_in);
                            if (left_in != 0 || right_in != 0)
                            {
                                zero_samples_counter = 0;
                            }
                            else
                            {
                                zero_samples_counter++;
                            }

                            const float volume =
                                smoothed_volume_buffer[sid] * bypass_smoother->tick();
                            left_out_buffer[sid] *= sample_mix(left_out_buffer[sid] * volume * 2,
                                                               right_out_buffer[sid] * volume * 2);
                        }
                    }

                    // VISUALIZE BEFORE FONAL OUT
                    if (Monique_Ui_SegmentedMeter *meter = synth_data->audio_processor->peak_meter)
                    {
                        juce::ScopedLock locked(synth_data->audio_processor->peak_meter_lock);
                        meter->process(left_out_buffer, num_samples_);
                    }
                    if (is_stereo)
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            left_out_buffer[sid] = soft_clipp_greater_0_9(left_out_buffer[sid]);
                            right_out_buffer[sid] = soft_clipp_greater_0_9(right_out_buffer[sid]);
                        }
                    }
                    else
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            left_out_buffer[sid] = soft_clipp_greater_0_9(left_out_buffer[sid]);
                        }
                    }

                    // VISUALIZE
                    if (Monique_Ui_AmpPainter *amp_painter =
                            synth_data->audio_processor->amp_painter)
                    {
                        amp_painter->add_out(left_out_buffer, right_out_buffer, num_samples_);
                        amp_painter->add_out_env(data_buffer->final_env.getReadPointer(),
                                                 num_samples_);
                    }
                }
            }
        }
        else // NOTE just a reduced copy of the function before
        {
            // COLLECT BUFFERS
            float *left_input_buffer =
                data_buffer->filter_stereo_output_samples.getWritePointer(LEFT);

            float *left_out_buffer = &output_buffer_.getWritePointer(LEFT)[start_sample_final_out_];
            const bool is_stereo = output_buffer_.getNumChannels() > 1;
            float *right_out_buffer =
                is_stereo ? &output_buffer_.getWritePointer(RIGHT)[start_sample_final_out_]
                          : data_buffer->second_mono_buffer.getWritePointer();

            // PREPARE
            {
                float *const final_env_amp = data_buffer->final_env.getWritePointer();
                final_env->process(final_env_amp, num_samples_);

                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    const float gain =
                        final_env_amp[sid] * velocity_smoother.add_and_get_average(velocity_[sid]);
                    left_input_buffer[sid] *= gain;
                }
            }

            // STEREO CHORUS
            {
                chorus.process(left_input_buffer, nullptr, left_out_buffer, nullptr, num_samples_);
            }

            // DELAY
            {
                delay.set_reflexion_size(
                    synth_data->delay_refexion, synth_data->delay_record_size,
                    synth_data->glide_motor_time,
                    is_standalone() ? synth_data->speed // // NOT POSSIBLE TO SYNC <- old comment
                                    : synth_data->runtime_info->bpm);

                delay.process(left_out_buffer, nullptr,
                              synth_data->delay_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_pan_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_record_release_smoother.get_smoothed_value_buffer(),
                              synth_data->delay_record, num_samples_);
            }

            // REVERB
            {
                const float *const smoothed_bypass_buffer =
                    synth_data->effect_bypass_smoother.get_smoothed_value_buffer();

                const float *const smoothed_room_buffer =
                    reverb_data->room_smoother.get_smoothed_value_buffer();
                const float *const smoothed_with_buffer =
                    reverb_data->width_smoother.get_smoothed_value_buffer();
                const float *const smoothed_dry_wet_mix_buffer =
                    reverb_data->dry_wet_mix_smoother.get_smoothed_value_buffer();
                ReverbParameters &rever_params_l = reverb_l.get_parameters();
                for (int sid = 0; sid != num_samples_; ++sid)
                {
                    {
                        const float reverb_room = smoothed_room_buffer[sid];
                        const float reverb_dry_wet_mix = 1.0f - smoothed_dry_wet_mix_buffer[sid];
                        const float reverb_width = smoothed_with_buffer[sid];
                        if (rever_params_l.roomSize != reverb_room ||
                            rever_params_l.dryLevel != reverb_dry_wet_mix
                            //|| rever_params_r.wetLevel != r_params.wetLevel
                            || rever_params_l.width != reverb_width)
                        {
                            rever_params_l.roomSize = reverb_room;
                            rever_params_l.dryLevel = reverb_dry_wet_mix;
                            rever_params_l.wetLevel = 1.0f - reverb_dry_wet_mix;
                            rever_params_l.width = reverb_width;

                            reverb_l.update_parameters();
                        }

                        const float in_l = left_out_buffer[sid];
                        float sample_l = reverb_l.processSingleSampleRaw(in_l);

                        const float bypass = smoothed_bypass_buffer[sid];
                        left_out_buffer[sid] = sample_mix(sample_l + in_l * bypass,
                                                          left_input_buffer[sid] * (1.0f - bypass));
                    }
                }
            }

            // PROCESS
            {
                // FINAL MIX
                {
                    const float *const smoothed_volume_buffer =
                        synth_data->volume_smoother.get_smoothed_value_buffer();
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            float left_in = left_out_buffer[sid];
                            MONO_UNDENORMALISE_OUTPUT(left_in);
                            if (left_in != 0)
                            {
                                zero_samples_counter = 0;
                            }
                            else
                            {
                                zero_samples_counter++;
                            }

                            const float volume =
                                smoothed_volume_buffer[sid] * bypass_smoother->tick();
                            left_out_buffer[sid] = left_out_buffer[sid] * volume * 2;
                        }
                    }

                    // VISUALIZE BEFORE FONAL OUT
                    if (Monique_Ui_SegmentedMeter *meter = synth_data->audio_processor->peak_meter)
                    {
                        juce::ScopedLock locked(synth_data->audio_processor->peak_meter_lock);
                        meter->process(left_out_buffer, num_samples_);
                    }
                    {
                        for (int sid = 0; sid != num_samples_; ++sid)
                        {
                            right_out_buffer[sid] = left_out_buffer[sid] =
                                soft_clipp_greater_0_9(left_out_buffer[sid]);
                        }
                    }

                    // VISUALIZE
                    if (Monique_Ui_AmpPainter *amp_painter =
                            synth_data->audio_processor->amp_painter)
                    {
                        amp_painter->add_out(left_out_buffer, right_out_buffer, num_samples_);
                        amp_painter->add_out_env(data_buffer->final_env.getReadPointer(),
                                                 num_samples_);
                    }
                }
            }
        }
    }

    //==========================================================================
    void start_attack() noexcept
    {
        zero_samples_counter = 0;
        final_env->start_attack();
    }
    void start_release(bool is_sustain_pedal_down_, bool is_sostenuto_pedal_down_) noexcept
    {
        if (!is_sostenuto_pedal_down_)
        {
            // chorus_modulation_env->set_to_release();
        }
        if (!is_sustain_pedal_down_)
        {
            final_env->set_to_release();
        }
    }

    //==========================================================================
    void reset() noexcept
    {
        delay.reset();
        reverb_l.reset();
        reverb_r.reset();
        chorus.reset();

        final_env->reset();

        zero_samples_counter = 0;
        velocity_smoother.reset(0);
    }

  public:
    //==============================================================================
    COLD FXProcessor(RuntimeNotifyer *const notifyer_, MoniqueSynthData *synth_data_,
                     LinearSmootherMinMax<false, true> *bypass_smoother_,
                     const float *const sine_lookup_, const float *const cos_lookup_,
                     const float *const exp_lookup_) noexcept
        :

          delay(notifyer_, synth_data_),

          reverb_l(notifyer_, LEFT), reverb_r(notifyer_, RIGHT),

          chorus(notifyer_, synth_data_),

          final_env(new ENV(notifyer_, synth_data_, synth_data_->env_data.get(), sine_lookup_,
                            cos_lookup_, exp_lookup_)),

          velocity_smoother(notifyer_, synth_data_->velocity_glide_time), zero_samples_counter(0),
          bypass_smoother(bypass_smoother_),

          synth_data(synth_data_), data_buffer(synth_data_->data_buffer),
          reverb_data(synth_data_->reverb_data.get()), chorus_data(synth_data_->chorus_data.get()),

          sin_lookup(synth_data_->sine_lookup), cos_lookup(synth_data_->cos_lookup)
    {
#ifdef JUCE_DEBUG
        std::cout << "MONIQUE: init FX" << std::endl;
#endif
    }

    COLD ~FXProcessor() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXProcessor)
};

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
class ArpSequencer : public RuntimeListener
{
    RuntimeInfo *const info;
    const ArpSequencerData *const data;

    friend class MoniqueSynthesiserVoice;

    int current_step;
    int next_step_on_hold;
    int step_at_sample_current_buffer;

    int shuffle_to_back_counter;
    bool found_a_step;

    std::int64_t user_arp_start_point_in_samples;

    juce::OwnedArray<Step> steps_on_hold;

  public:
    //==============================================================================
    // RETURNS THE NUMBER OF SAMPLES TO THE NEXT STEP
    // RETURN NUM SAMPLES IF THERE IS NO STEP IN THE BUFFER
    inline int process_samples_to_next_step(int start_sample_, int num_samples_) noexcept
    {
        const std::int64_t samples_since_start =
            info->relative_samples_since_start - user_arp_start_point_in_samples;
        double samples_per_min = sample_rate * 60;
        double speed_multi = ArpSequencerData::speed_multi_to_value(data->speed_multi);
        double steps_per_min = info->bpm * 4 / 1.0 * speed_multi;
        double steps_per_sample = steps_per_min / samples_per_min;
        int samples_offset = 0;
        const int fine = data->fine_offset.get_value();
        if (0 != fine)
        {
            float multi = 1;
            switch (fine)
            {
            case -5:
                multi = -1.5;
                break;
            case -4:
                multi = -2;
                break;
            case -3:
                multi = -3;
                break;
            case -2:
                multi = -6;
                break;
            case -1:
                multi = -8;
                break;
            case 1:
                multi = 8;
                break;
            case 2:
                multi = 6;
                break;
            case 3:
                multi = 3;
                break;
            case 4:
                multi = 2;
                break;
            case 5:
                multi = 1.5;
                break;
            }
            const float samples_per_step = 1.0f / steps_per_sample;
            samples_offset = floor(samples_per_step / multi);
        }
        std::int64_t sync_sample_pos = samples_since_start + start_sample_ + samples_offset;
        std::int64_t step = next_step_on_hold;
        step_at_sample_current_buffer = -1;

        double samples_per_step =
            samples_per_min / steps_per_min; // WILL BE OVERRIDDEN IN STANDALONE!

        bool is_extern_synced = false;
        if (is_standalone())
        {
            is_extern_synced = info->standalone_features_pimpl->is_extern_synced;
        }

        for (int i = 0; i < num_samples_; ++i)
        {
            if (++sync_sample_pos < 0)
            {
                continue;
            }

            if (is_standalone() && is_extern_synced)
            {
                juce::OwnedArray<Step> &steps_in_block(
                    info->standalone_features_pimpl->steps_in_block);
                if (steps_in_block.size())
                {
                    Step &step__(*steps_in_block.getFirst());
                    if (step__.at_absolute_sample == sync_sample_pos - samples_offset)
                    {
                        if (samples_offset > 0)
                        {
                            steps_on_hold.add(new Step(step__.step_id,
                                                       step__.at_absolute_sample + samples_offset,
                                                       step__.samples_per_step));
                        }
                        else if (samples_offset < 0)
                        {
                            steps_on_hold.add(new Step(step__.step_id + 1,
                                                       step__.at_absolute_sample +
                                                           (samples_per_step + samples_offset),
                                                       step__.samples_per_step));
                        }
                        else
                        {
                            step = step__.step_id;
                            samples_per_step = step__.samples_per_step;
                            steps_in_block.remove(0, true);
                        }
                    }
                }
                if (steps_on_hold.size())
                {
                    for (int i = 0; i < steps_on_hold.size(); ++i)
                    {
                        Step &step__(*steps_on_hold.getUnchecked(i));
                        if (step__.at_absolute_sample == sync_sample_pos - samples_offset)
                        {
                            step = step__.step_id;
                            samples_per_step = step__.samples_per_step;
                            steps_on_hold.remove(i, true);
                            i--;
                        }
                        // CLEAN
                        else if (step__.at_absolute_sample < sync_sample_pos - samples_offset)
                        {
                            steps_on_hold.remove(i, true);
                            i--;
                        }
                    }
                }
            }
            else
            {
                step = std::floor(steps_per_sample *
                                  sync_sample_pos); //+1; // +1 for future processing
                if (step < 0)
                {
                    step = 0;
                }
            }

            // step += data->step_offset.get_value();

            --shuffle_to_back_counter;

            if (step != next_step_on_hold)
            {
                next_step_on_hold = step;

                if (current_step % 2 == 0)
                {
                    shuffle_to_back_counter = std::floor(
                        samples_per_step * ArpSequencerData::shuffle_to_value(data->shuffle));
                }
                else
                {
                    shuffle_to_back_counter = 0;
                }

                found_a_step = true;
            }

            if (found_a_step && shuffle_to_back_counter == 0)
            {
                step_at_sample_current_buffer = i;
                current_step = next_step_on_hold;
                return i;
            }
        }

        return num_samples_;
    }

    //==============================================================================
    inline int get_current_step() const noexcept
    {
        return int(current_step + data->step_offset.get_value()) % SUM_ENV_ARP_STEPS;
    }
    inline int get_current_absolute_step() const noexcept
    {
        return int(current_step + data->step_offset.get_value());
    }
    inline int get_step_before() const noexcept
    {
        if (int(current_step + data->step_offset.get_value()) > 0)
            return (int(current_step + data->step_offset.get_value()) - 1) % SUM_ENV_ARP_STEPS;
        else
            return 0;
    }
    inline float get_current_tune() const noexcept
    {
        return data->tune[get_current_step()].get_value();
    }
    inline bool found_last_process_a_step() const noexcept
    {
        return step_at_sample_current_buffer != -1;
    }
    inline bool last_found_step_is_enabled() const noexcept
    {
        bool success = false;
        if (found_last_process_a_step())
        {
            if (data->step[get_current_step()])
            {
                success = true;
            }
        }

        return success;
    }
    inline void reset() noexcept
    {
        current_step = 0;
        next_step_on_hold = 0;
        shuffle_to_back_counter = 0;
        steps_on_hold.clear(true);
        step_at_sample_current_buffer = data->step[0] ? 0 : -1;
    }

    void sample_rate_or_block_changed() noexcept override {}

    void set_user_arp_start_point_in_samples(std::int64_t user_arp_start_point_in_samples_) noexcept
    {
        user_arp_start_point_in_samples = user_arp_start_point_in_samples_;
    }

  public:
    //==============================================================================
    COLD ArpSequencer(RuntimeNotifyer *const notifyer_, RuntimeInfo *info_,
                      const ArpSequencerData *data_) noexcept
        : RuntimeListener(notifyer_),

          info(info_), data(data_), current_step(0), next_step_on_hold(0),
          step_at_sample_current_buffer(-1),

          shuffle_to_back_counter(0), found_a_step(false),

          user_arp_start_point_in_samples(0)
    {
#ifdef JUCE_DEBUG
        std::cout << "MONIQUE: init SEQUENCER" << std::endl;
#endif
    }

    COLD ~ArpSequencer() noexcept {}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpSequencer)
};

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
COLD MoniqueSynthesiserVoice::MoniqueSynthesiserVoice(MoniqueAudioProcessor *const audio_processor_,
                                                      MoniqueSynthData *const synth_data_,
                                                      RuntimeNotifyer *const notifyer_,
                                                      RuntimeInfo *const info_,
                                                      DataBuffer *data_buffer_) noexcept
    : audio_processor(audio_processor_), synth_data(synth_data_),

      info(info_), data_buffer(data_buffer_),

      arp_sequencer(new ArpSequencer(notifyer_, info, synth_data_->arp_sequencer_data.get())),
      eq_processor(new EQProcessorStereo(notifyer_, synth_data_, synth_data_->sine_lookup,
                                         synth_data_->cos_lookup, synth_data_->exp_lookup)),
      bypass_smoother(0),
      fx_processor(new FXProcessor(notifyer_, synth_data_, &bypass_smoother,
                                   synth_data_->sine_lookup, synth_data_->cos_lookup,
                                   synth_data_->exp_lookup)),

      current_note(-1), pitch_offset(0),

      is_sostenuto_pedal_down(false), stopped_and_sostenuto_pedal_was_down(false),
      is_soft_pedal_down(false), was_soft_pedal_down_on_note_start(false),
      is_sustain_pedal_down(false), stopped_and_sustain_pedal_was_down(false),

      current_velocity(0), current_step(0), current_running_arp_step(0),
      an_arp_note_is_already_running(false), sample_position_for_restart_arp(-1)
{
#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init BUFFERS's" << std::endl;

    std::cout << "MONIQUE: init OSC's" << std::endl;
#endif

    master_osc = new MasterOSC(notifyer_, synth_data_, synth_data_->sine_lookup);
    second_osc = new SecondOSC(notifyer_, synth_data_, 1, synth_data_->sine_lookup);
    third_osc = new SecondOSC(notifyer_, synth_data_, 2, synth_data_->sine_lookup);

#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init LFO's" << std::endl;
#endif

    lfos = new LFO *[SUM_LFOS];
    for (int i = 0; i != SUM_LFOS; ++i)
    {
        lfos[i] =
            new LFO(notifyer_, synth_data_, synth_data->lfo_datas[i], synth_data_->sine_lookup);
    }

    mfos = new LFO *[SUM_MORPHER_GROUPS];
    for (int i = 0; i != SUM_MORPHER_GROUPS; ++i)
    {
        mfos[i] =
            new LFO(notifyer_, synth_data_, synth_data->mfo_datas[i], synth_data_->sine_lookup);
    }

#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init FILTERS & ENVELOPES" << std::endl;
#endif

    filter_processors = new FilterProcessor *[SUM_FILTERS];
#ifdef POLY
    filter_volume_tracking_envs = new ENV *[SUM_FILTERS];
#endif
    for (int i = 0; i != SUM_FILTERS; ++i)
    {
        filter_processors[i] =
            new FilterProcessor(notifyer_, synth_data_, i, synth_data_->sine_lookup,
                                synth_data_->cos_lookup, synth_data_->exp_lookup);
#ifdef POLY
        filter_volume_tracking_envs[i] =
            new ENV(notifyer_, synth_data_, synth_data_->env_data, synth_data_->sine_lookup,
                    synth_data_->cos_lookup, synth_data_->exp_lookup);
#endif
    }
}
COLD MoniqueSynthesiserVoice::~MoniqueSynthesiserVoice() noexcept
{
#ifdef JUCE_DEBUG
    std::cout << "~MoniqueSynthesiserVoice" << std::endl;
#endif
    for (int i = SUM_FILTERS - 1; i > -1; --i)
    {
#ifdef POLY
        delete filter_volume_tracking_envs[i];
#endif
        delete filter_processors[i];
    }
    delete[] filter_processors;
    for (int i = SUM_LFOS - 1; i > -1; --i)
    {
        delete lfos[i];
    }
    delete[] lfos;
    for (int i = SUM_MORPHER_GROUPS - 1; i > -1; --i)
    {
        delete mfos[i];
    }
    delete[] mfos;

    delete third_osc;
    delete second_osc;
    delete master_osc;

    delete arp_sequencer;
    delete eq_processor;
    delete fx_processor;
}

//==============================================================================
void MoniqueSynthesiserVoice::startNote(int midi_note_number_, float velocity_,
                                        juce::SynthesiserSound * /*sound*/, int pitch_)
{
    start_internal(midi_note_number_, velocity_, 0, true);
}
void MoniqueSynthesiserVoice::start_internal(int midi_note_number_, float velocity_,
                                             int sample_number_, bool is_human_event_,
                                             bool trigger_envelopes_, bool isNoteOff) noexcept
{
    stopped_and_sostenuto_pedal_was_down = false;
    stopped_and_sustain_pedal_was_down = false;
    was_soft_pedal_down_on_note_start = is_soft_pedal_down;

    current_velocity =
        (1.0f - synth_data->env_data->velosivity) * velocity_ + synth_data->env_data->velosivity;

    // OSCS
    MoniqueSynthesizer::NoteDownStore *tmp_note_down_store =
        reinterpret_cast<MoniqueSynthesizer::NoteDownStore *>(note_down_store);
    const int keys_down = tmp_note_down_store->size();
    const bool step_automation_is_on =
        (synth_data->arp_sequencer_data->is_on &&
         (!synth_data->keep_arp_always_off || synth_data->keep_arp_always_on)) ||
        synth_data->keep_arp_always_on;
    bool start_up = (step_automation_is_on && keys_down == 1) ||
                    (step_automation_is_on &&
                     (synth_data->arp_sequencer_data->is_sequencer && current_note != -1) &&
                     !is_human_event_) ||
                    (step_automation_is_on && keys_down > 0 && !is_human_event_) ||
                    (!step_automation_is_on && is_human_event_);
    if (synth_data->arp_sequencer_data->is_sequencer)
    {
        if (is_human_event_)
        {
            arp_sequencer->set_user_arp_start_point_in_samples(0);
        }
    }
#ifdef POLY
    if (an_arp_note_is_already_running && step_automation_is_on
        //|| arp_sequencer->data->connect && keys_down > 1 && step_automation_is_on == false
        //|| step_automation_is_on && is_human_event_
    )
    {
        trigger_envelopes_ = false;
    }
#else
    if ((arp_sequencer->data->connect && an_arp_note_is_already_running && step_automation_is_on) ||
        (arp_sequencer->data->connect && keys_down > 1 && step_automation_is_on == false) ||
        (step_automation_is_on && is_human_event_))
    {
        trigger_envelopes_ = false;
    }
#endif
    // KEYTRACK OR NOTE PLAYBACK
    const float arp_offset = ((step_automation_is_on ||
                               (step_automation_is_on &&
                                synth_data->arp_sequencer_data->is_sequencer && current_note != -1))
                                  ? arp_sequencer->get_current_tune()
                                  : 0);
    {
        // FIRST KEY DOWN
        const bool first_key_down_event = is_human_event_ && keys_down == 1;

        if (!synth_data->arp_sequencer_data->is_sequencer)
        {
            if (keys_down > 0 && is_human_event_ && step_automation_is_on)
            {
                // LEGATO ARP - TURN THIS FIRST BLOCK OF TO HAVE IT NORMAL
#ifndef POLY
                if ((isNoteOff && arp_sequencer->data->connect) ||
                    (keys_down > 1 && arp_sequencer->data->connect))
                    trigger_envelopes_ = false;
                else
#endif
                {
                    arp_sequencer->set_user_arp_start_point_in_samples(info->samples_since_start +
                                                                       sample_number_);
                    arp_sequencer->reset();
                    an_arp_note_is_already_running = synth_data->arp_sequencer_data->step[0];
                    start_up = an_arp_note_is_already_running;
                    trigger_envelopes_ = start_up;
                }
            }
        }
        else
        {
            if (keys_down == 1 && is_human_event_)
            {
                arp_sequencer->set_user_arp_start_point_in_samples(0);
            }
        }

        const juce::MidiMessage *last_message = tmp_note_down_store->get_last();
        int last_note = -1;
        int last_note_id = -1;
        int current_note_id = -2;
        if (last_message)
        {
            last_note = tmp_note_down_store->get_last()->getNoteNumber();
            last_note_id = tmp_note_down_store->get_id(last_note);
        }
        if (is_human_event_)
        {
            current_note_id = tmp_note_down_store->get_id(midi_note_number_);
        }
#ifdef POLY
        int reorder_allowed = synth_data->keytrack_osci[0] + synth_data->keytrack_osci[1] +
                              synth_data->keytrack_osci[2];
#else
        const int reorder_allowed = 0;
#endif
        if (!reorder_allowed)
        {
            current_note = midi_note_number_;
        }
        /*
        else
        {
            if( synth_data->keytrack_osci[0] )
            {
                if( keys_down == 1 )
                {
                    current_note = midi_note_number_;
                }
            }
            else
            {
                current_note = midi_note_number_;
            }
        }
        */

        // INSERT
        if (current_note_id >= 0 && current_note_id <= 2)
        {
            int trigger_again_note_0 = false;
            int trigger_again_note_1 = false;
            int trigger_again_note_2 = false;
            const int incoming_note_value = midi_note_number_;
            const juce::MidiMessage *message_0 = tmp_note_down_store->get_at(0);
            const juce::MidiMessage *message_1 = tmp_note_down_store->get_at(1);
            const juce::MidiMessage *message_2 = tmp_note_down_store->get_at(2);
            bool trigger_again_note_0_was_running = false;
            bool trigger_again_note_1_was_running = false;
            bool trigger_again_note_2_was_running = false;

            if (tmp_note_down_store->size() > 1)
            {
                bool done = false;

                // PLAYBACK MODE
                struct compary
                {
                    virtual bool compare(int left_, int right_) const noexcept = 0;
                    virtual int get_compare_default() const noexcept = 0;
                    virtual ~compary(){};
                };
                struct low_last_compary : public compary
                {
                    bool compare(int left_, int right_) const noexcept override
                    {
                        return left_ < right_;
                    }
                    int get_compare_default() const noexcept override { return -1; }
                };
                struct high_last_compary : public compary
                {
                    bool compare(int left_, int right_) const noexcept override
                    {
                        return left_ > right_;
                    }
                    int get_compare_default() const noexcept override { return 999; }
                };
#ifdef POLY
                std::unique_ptr<compary> comparier;
                std::unique_ptr<compary> comparier2;
                bool first_and_second_swapped = false;
                if (synth_data->keytrack_osci_play_mode == 1)
                {
                    comparier = new high_last_compary();
                }
                if (synth_data->keytrack_osci_play_mode == 0)
                {
                    comparier = new low_last_compary();
                }
                if (synth_data->keytrack_osci_play_mode == 2 || reorder_allowed == false)
                {
                    comparier = nullptr;
                }
                if (synth_data->keytrack_osci_play_mode == 3 || reorder_allowed == false)
                {
                    comparier = new high_last_compary();
                    comparier2 = new low_last_compary();
                }
#else
                auto comparier = std::make_unique<low_last_compary>();
#endif

                int note_0_value = 0;
                int note_1_value = 0;
                int note_2_value = 0;
                if (comparier)
                {
                    note_0_value = note_1_value = note_2_value = comparier->get_compare_default();
                }

                if (message_0)
                {
                    note_0_value = message_0->getNoteNumber();
                }
                if (message_1)
                {
                    note_1_value = message_1->getNoteNumber();
                }
                if (message_2)
                {
                    note_2_value = message_2->getNoteNumber();
                }

                if (comparier)
                {
                    trigger_again_note_0_was_running = tmp_note_down_store->size() > 1;
                    trigger_again_note_1_was_running = tmp_note_down_store->size() > 2;
                    trigger_again_note_2_was_running = tmp_note_down_store->size() > 3;

                    if (comparier->compare(note_0_value, note_1_value))
                    {
                        if (comparier->compare(note_0_value, note_2_value))
                        {
                            tmp_note_down_store->swap(0, 1);
                            tmp_note_down_store->swap(1, 2);

                            trigger_again_note_0 = note_1_value;
                            trigger_again_note_1 = note_2_value;
                            trigger_again_note_2 = note_0_value;

                            if (current_note_id == 0)
                            {
                                current_note_id = 2;
                                trigger_again_note_2 = false;
                            }
                            else if (current_note_id == 1)
                            {
                                current_note_id = 0;
                                trigger_again_note_0 = false;
                            }
                            else if (current_note_id == 2)
                            {
                                current_note_id = 1;
                                trigger_again_note_1 = false;
                            }
                        }
                        else
                        {
                            tmp_note_down_store->swap(0, 1);

                            trigger_again_note_0 = note_1_value;
                            trigger_again_note_1 = note_0_value;
                            trigger_again_note_2 = note_2_value;

                            if (current_note_id == 0)
                            {
                                current_note_id = 1;
                                trigger_again_note_1 = false;
                            }
                            else if (current_note_id == 1)
                            {
                                current_note_id = 0;
                                trigger_again_note_0 = false;
                            }
                            else if (current_note_id == 2)
                            {
                                current_note_id = 2;
                                trigger_again_note_2 = false;
                            }
                        }
                    }
                    else if (comparier->compare(note_0_value, note_2_value))
                    {
                        if (comparier->compare(note_1_value, note_2_value))
                        {
                            tmp_note_down_store->swap(0, 2);
                            tmp_note_down_store->swap(1, 2);

                            trigger_again_note_0 = note_2_value;
                            trigger_again_note_1 = note_0_value;
                            trigger_again_note_2 = note_1_value;

                            if (current_note_id == 0)
                            {
                                current_note_id = 1;
                                trigger_again_note_1 = false;
                            }
                            else if (current_note_id == 1)
                            {
                                current_note_id = 2;
                                trigger_again_note_2 = false;
                            }
                            else if (current_note_id == 2)
                            {
                                current_note_id = 0;
                                trigger_again_note_0 = false;
                            }
                        }
                        else
                        {
                            tmp_note_down_store->swap(0, 2);

                            trigger_again_note_0 = note_2_value;
                            trigger_again_note_1 = note_1_value;
                            trigger_again_note_2 = note_0_value;

                            if (current_note_id == 0)
                            {
                                current_note_id = 2;
                                trigger_again_note_2 = false;
                            }
                            else if (current_note_id == 1)
                            {
                                current_note_id = 1;
                                trigger_again_note_1 = false;
                            }
                            else if (current_note_id == 2)
                            {
                                current_note_id = 0;
                                trigger_again_note_0 = false;
                            }
                        }
                    }
                    else if (comparier->compare(note_1_value, note_2_value))
                    {
                        tmp_note_down_store->swap(1, 2);

                        trigger_again_note_0 = note_0_value;
                        trigger_again_note_1 = note_2_value;
                        trigger_again_note_2 = note_1_value;

                        if (current_note_id == 0)
                        {
                            current_note_id = 0;
                            trigger_again_note_0 = false;
                        }
                        else if (current_note_id == 1)
                        {
                            current_note_id = 2;
                            trigger_again_note_2 = false;
                        }
                        else if (current_note_id == 2)
                        {
                            current_note_id = 1;
                            trigger_again_note_1 = false;
                        }
                    }
                    else
                    {
                        if (current_note_id == 0 ||
                            (note_0_value != comparier->get_compare_default() &&
                             note_0_value != current_note))
                        {
                            trigger_again_note_1 = note_1_value;
                            trigger_again_note_2 = note_2_value;
                        }
                        if (current_note_id == 0)
                            trigger_again_note_0_was_running = false;
                        else if (current_note_id == 1)
                            trigger_again_note_1_was_running = false;
                        else if (current_note_id == 2)
                            trigger_again_note_2_was_running = false;
                    }

                    if (trigger_again_note_0 == comparier->get_compare_default())
                        trigger_again_note_0 = false;
                    if (trigger_again_note_1 == comparier->get_compare_default())
                        trigger_again_note_1 = false;
                    if (trigger_again_note_2 == comparier->get_compare_default())
                        trigger_again_note_2 = false;
                }
            }

            // PROCESSING
            const juce::MidiMessage *message = tmp_note_down_store->get_at(current_note_id);
            if (message)
            {
                int note_number = message->getNoteNumber();
                if (current_note_id == 0 || trigger_again_note_0 != false)
                {
                    const int note_to_use =
                        trigger_again_note_0 != false ? trigger_again_note_0 : note_number;
                    // OSC TRACKING 1 - SWAP ROOT NOTe
                    {
                        current_note = note_to_use;
                    }
#ifdef POLY
                    // CUTOFF TRACKING 1
                    if (synth_data->keytrack_cutoff[0])
                    {
                        // synth_data->filter_datas[0]->cutoff.set_value(
                        // reverse_cutoff_to_slider_value(
                        // synth_data->tuning->midiNoteToFrequency(note_to_use+arp_offset) ) );
                    }
                    if (trigger_envelopes_)
                    {
                        if (!trigger_again_note_0_was_running && !step_automation_is_on)
                        {
                            // ENV TRACKING 1
                            if (synth_data->keytrack_filter_env[0])
                            {
                                filter_processors[0]->start_env();
                            }
                            // INPUT ENV TRACKING 1
                            if (synth_data->keytrack_filter_inputs[0])
                            {
                                filter_processors[0]->start_input_env(0);
                            }
                            if (synth_data->keytrack_filter_inputs[3])
                            {
                                filter_processors[1]->start_input_env(0);
                            }
                            if (synth_data->keytrack_filter_inputs[6])
                            {
                                filter_processors[2]->start_input_env(0);
                            }
                            // VOLUME TRACKING 3
                            if (synth_data->keytrack_filter_volume[0])
                            {
                                filter_volume_tracking_envs[0]->start_attack();
                            }
                        }
                    }
#else
                    if (trigger_envelopes_)
                    {
                        if (!trigger_again_note_0_was_running && !step_automation_is_on)
                        {
                            filter_processors[0]->start_env();
                            filter_processors[1]->start_env();
                            filter_processors[2]->start_env();
                            filter_processors[0]->start_input_env(0);
                            filter_processors[1]->start_input_env(0);
                            filter_processors[2]->start_input_env(0);
                            filter_processors[0]->start_input_env(1);
                            filter_processors[1]->start_input_env(1);
                            filter_processors[2]->start_input_env(1);
                            filter_processors[0]->start_input_env(2);
                            filter_processors[1]->start_input_env(2);
                            filter_processors[2]->start_input_env(2);
                        }
                    }
#endif
                }
                /*else*/ if (current_note_id == 1 || trigger_again_note_1 != false)
                {
#ifdef POLY
                    // OSC TRACKING 2
                    if (synth_data->keytrack_osci[1])
                    {
                        const int note_to_use =
                            trigger_again_note_1 > false ? trigger_again_note_1 : note_number;
                        synth_data->osc_datas[1]->tune.set_value(
                            (note_to_use + synth_data->keytrack_osci_octave_offset[1] * 12) -
                            current_note);
                    }

                    if (trigger_envelopes_)
                    {
                        if ((!trigger_again_note_1_was_running && !step_automation_is_on) ||
                            (!trigger_again_note_2_was_running && step_automation_is_on &&
                             !synth_data->arp_sequencer_data->is_sequencer && is_human_event_))
                        {
                            // ENV TRACKING 2
                            if (synth_data->keytrack_filter_env[1])
                            {
                                filter_processors[1]->start_env();
                            }
                            // VOLUME TRACKING 2
                            if (synth_data->keytrack_filter_volume[1])
                            {
                                filter_volume_tracking_envs[1]->start_attack();
                            }
                            // INPUT ENV TRACKING 2
                            if (synth_data->keytrack_filter_inputs[0 + 1])
                            {
                                filter_processors[0]->start_input_env(0 + 1);
                            }
                            if (synth_data->keytrack_filter_inputs[3 + 1])
                            {
                                filter_processors[1]->start_input_env(0 + 1);
                            }
                            if (synth_data->keytrack_filter_inputs[6 + 1])
                            {
                                filter_processors[2]->start_input_env(0 + 1);
                            }
                        }
                    }
#endif
                }
                /*else*/ if (current_note_id == 2 || trigger_again_note_2 != false)
                {
#ifdef POLY
                    // OSC TRACKING 3
                    if (synth_data->keytrack_osci[2])
                    {
                        const int note_to_use =
                            trigger_again_note_2 > false ? trigger_again_note_2 : note_number;
                        synth_data->osc_datas[2]->tune.set_value(
                            (note_to_use + synth_data->keytrack_osci_octave_offset[2] * 12) -
                            current_note);
                    }

                    if (trigger_envelopes_)
                    {
                        if ((!trigger_again_note_2_was_running && !step_automation_is_on) ||
                            (!trigger_again_note_2_was_running && step_automation_is_on &&
                             !synth_data->arp_sequencer_data->is_sequencer && is_human_event_))
                        {
                            // ENV TRACKING 3
                            if (synth_data->keytrack_filter_env[2])
                            {
                                filter_processors[2]->start_env();
                            }
                            // INPUT ENV TRACKING 3
                            if (synth_data->keytrack_filter_inputs[0 + 2])
                            {
                                filter_processors[0]->start_input_env(0 + 2);
                            }
                            if (synth_data->keytrack_filter_inputs[3 + 2])
                            {
                                filter_processors[1]->start_input_env(0 + 2);
                            }
                            if (synth_data->keytrack_filter_inputs[6 + 2])
                            {
                                filter_processors[2]->start_input_env(0 + 2);
                            }
                            // VOLUME TRACKING 3
                            if (synth_data->keytrack_filter_volume[2])
                            {
                                filter_volume_tracking_envs[2]->start_attack();
                            }
                        }
                    }
#endif
                }
            }
            else
            {
                DBG("key tracking note select bug");
            }
        }
        else if (current_note_id == -2) // AUTOMATED TUNE OFFSETS
        {
            // TODO REPLACE LAST ONE
        }
    }

    // WHEN TO TRIGGER THE PROCESSORS
    if (start_up)
    {
        // ROOTS
        if (trigger_envelopes_)
        {
            eq_processor->start_attack();
            fx_processor->start_attack();
        }

        // NOTES
        {
            master_osc->update(current_note + arp_offset + pitch_offset, sample_number_);
            second_osc->update(current_note + arp_offset + pitch_offset, sample_number_);
            third_osc->update(current_note + arp_offset + pitch_offset, sample_number_);
        }

        // CUTOFF
        {
#ifdef POLY
            if (synth_data->keytrack_cutoff[0])
            {
                const int note_0 =
                    current_note + synth_data->keytrack_cutoff_octave_offset[0] * 12 + arp_offset;
                synth_data->filter_datas[0]->cutoff.set_value(reverse_cutoff_to_slider_value(
                    synth_data->tuning->midiNoteToFrequency(note_0)));
            }
            if (synth_data->keytrack_cutoff[1])
            {
                const int note_1 = current_note + synth_data->osc_datas[1]->tune +
                                   synth_data->keytrack_cutoff_octave_offset[1] * 12 + arp_offset;
                synth_data->filter_datas[1]->cutoff.set_value(reverse_cutoff_to_slider_value(
                    synth_data->tuning->midiNoteToFrequency(note_1)));
            }
            if (synth_data->keytrack_cutoff[2])
            {
                const int note_2 = current_note + synth_data->osc_datas[2]->tune +
                                   synth_data->keytrack_cutoff_octave_offset[2] * 12 + arp_offset;
                synth_data->filter_datas[2]->cutoff.set_value(reverse_cutoff_to_slider_value(
                    synth_data->tuning->midiNoteToFrequency(note_2)));
            }
#endif
        }
        // todo trigger if key down - like you ask is_key_x_down
        // ENVELOPES
#ifdef POLY
        for (int i = 0; i != SUM_FILTERS; ++i)
        {
            const bool trigger =
                trigger_envelopes_ &&
                ((step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer) ||
                 (step_automation_is_on && !synth_data->arp_sequencer_data->is_sequencer &&
                  keys_down > i));

            // FILTER ENV TRACKING
            if (!synth_data->keytrack_filter_env[i] || trigger)
            {
                filter_processors[i]->start_env();
            }
            // FILTER INPUT ENV TRACKING
            if (!synth_data->keytrack_filter_inputs[i * SUM_OSCS + 0] || trigger)
            {
                filter_processors[i]->start_input_env(0);
            }
            if (!synth_data->keytrack_filter_inputs[i * SUM_OSCS + 1] || trigger)
            {
                filter_processors[i]->start_input_env(1);
            }
            if (!synth_data->keytrack_filter_inputs[i * SUM_OSCS + 2] || trigger)
            {
                filter_processors[i]->start_input_env(2);
            }
            // FILTER OUT TRACKING
            if (!synth_data->keytrack_filter_volume[i] || trigger)
            {
                filter_volume_tracking_envs[i]->start_attack();
            }
        }
#else
        const bool trigger =
            trigger_envelopes_ &&
            ((step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer) ||
             (step_automation_is_on && !synth_data->arp_sequencer_data->is_sequencer && keys_down));
        for (int i = 0; i != SUM_FILTERS; ++i)
        {
            // FILTER ENV TRACKING
            if (trigger)
            {
                filter_processors[i]->start_env();
            }
            // FILTER INPUT ENV TRACKING
            if (trigger)
            {
                filter_processors[i]->start_input_env(0);
            }
            if (trigger)
            {
                filter_processors[i]->start_input_env(1);
            }
            if (trigger)
            {
                filter_processors[i]->start_input_env(2);
            }
        }
#endif

        // KEY SYNC
        if (synth_data->osc_datas[MASTER_OSC]->sync && trigger_envelopes_)
        {
            master_osc->reset();
            second_osc->reset();
            third_osc->reset();
        }
    }
}

void MoniqueSynthesiserVoice::stop_arp_controlled(bool force)
{
    MoniqueSynthesizer::NoteDownStore *tmp_note_down_store =
        reinterpret_cast<MoniqueSynthesizer::NoteDownStore *>(note_down_store);
    const bool step_automation_is_on =
        synth_data->arp_sequencer_data->is_on &&
        (!synth_data->keep_arp_always_off || synth_data->keep_arp_always_on);
    const int keys_down = tmp_note_down_store->size();
#ifdef POLY
    for (int i = 0; i != SUM_FILTERS; ++i)
    {
        // FILTER ENV TRACKING
        if (!synth_data->keytrack_filter_env[i] || (step_automation_is_on && keys_down > i) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[i]->start_env_release();
        }
        // FILTER INPUT TRACKING
        if (!synth_data->keytrack_filter_inputs[0 * SUM_OSCS + i] ||
            (step_automation_is_on && keys_down > i) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[0]->start_input_env_release(i);
        }
        if (!synth_data->keytrack_filter_inputs[1 * SUM_OSCS + i] ||
            (step_automation_is_on && keys_down > i) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[1]->start_input_env_release(i);
        }
        if (!synth_data->keytrack_filter_inputs[2 * SUM_OSCS + i] ||
            (step_automation_is_on && keys_down > i) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[2]->start_input_env_release(i);
        }
        // FILTER OUT TRACKING
        if (!synth_data->keytrack_filter_volume[i] || (step_automation_is_on && keys_down > i) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_volume_tracking_envs[i]->set_to_release();
        }
    }
    eq_processor->start_release();
    fx_processor->start_release(false, false);
#else
    for (int i = 0; i != SUM_FILTERS; ++i)
    {
        // FILTER ENV TRACKING
        if (force ||
            (step_automation_is_on && keys_down == 0 && synth_data->arp_sequencer_data->connect) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[i]->start_env_release();
        }
        // FILTER INPUT TRACKING
        if (force ||
            (step_automation_is_on && keys_down == 0 && synth_data->arp_sequencer_data->connect) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[0]->start_input_env_release(i);
        }
        if (force ||
            (step_automation_is_on && keys_down == 0 && synth_data->arp_sequencer_data->connect) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[1]->start_input_env_release(i);
        }
        if (force ||
            (step_automation_is_on && keys_down == 0 && synth_data->arp_sequencer_data->connect) ||
            (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
        {
            filter_processors[2]->start_input_env_release(i);
        }
    }
    if (force ||
        (step_automation_is_on && keys_down == 0 && synth_data->arp_sequencer_data->connect) ||
        (step_automation_is_on && synth_data->arp_sequencer_data->is_sequencer))
    {
        eq_processor->start_release();
        fx_processor->start_release(false, false);
    }
#endif
}
void MoniqueSynthesiserVoice::stop_controlled(const juce::MidiMessage &m_, int sample_pos_)
{
    MoniqueSynthesizer::NoteDownStore *tmp_note_down_store =
        reinterpret_cast<MoniqueSynthesizer::NoteDownStore *>(note_down_store);
    const int note_id = tmp_note_down_store->get_id(m_.getNoteNumber());
    const bool step_automation_is_on =
        synth_data->arp_sequencer_data->is_on &&
        (!synth_data->keep_arp_always_off || synth_data->keep_arp_always_on);
    const int keys_down = tmp_note_down_store->size();

    // PARTIAL STOP
    if (note_id >= 0 && note_id <= 2)
    {
        for (int i = 0; i != 3; ++i)
        {
#ifdef POLY
            if (note_id == i)
            {
                // FILTER ENV TRACKING
                if (synth_data->keytrack_filter_env[i])
                {
                    filter_processors[i]->start_env_release();
                }
                // FILTER INPUT TRACKING
                if (synth_data->keytrack_filter_inputs[0 * SUM_OSCS + i])
                {
                    filter_processors[0]->start_input_env_release(i);
                }
                if (synth_data->keytrack_filter_inputs[1 * SUM_OSCS + i])
                {
                    filter_processors[1]->start_input_env_release(i);
                }
                if (synth_data->keytrack_filter_inputs[2 * SUM_OSCS + i])
                {
                    filter_processors[2]->start_input_env_release(i);
                }
                // FILTER OUT TRACKING
                if (synth_data->keytrack_filter_volume[i])
                {
                    filter_volume_tracking_envs[i]->set_to_release();
                }
            }
#endif
        }
    }
    // REMOVE NOTe AND RESTART IF REQUIRED
    if (tmp_note_down_store->size())
    {
#ifdef POLY
        const int reorder_allowed = synth_data->keytrack_osci[0] + synth_data->keytrack_osci[1] +
                                    synth_data->keytrack_osci[2];
        const int play_mode =
            synth_data->keytrack_osci_play_mode == 2 ? PLAY_MODES::FIFO : PLAY_MODES::LIFO;
#else
        const int reorder_allowed = 0;
        const int play_mode = PLAY_MODES::LIFO;
#endif
        if (const juce::MidiMessage *replacement =
                tmp_note_down_store->remove_note(m_, play_mode, reorder_allowed))
        {
            start_internal(replacement->getNoteNumber(), replacement->getFloatVelocity(),
                           sample_pos_, true, false, true);
        }
        // RESTART LAST
        else if (tmp_note_down_store->get_last() && !reorder_allowed)
        {
            const juce::MidiMessage *message = tmp_note_down_store->get_last();
            start_internal(message->getNoteNumber(), message->getFloatVelocity(), sample_pos_, true,
                           false, true);
        }
        // FULL STOP
        else if (!tmp_note_down_store->size())
        {
            eq_processor->start_release();
            fx_processor->start_release(false, false);

            for (int i = 0; i != 3; ++i)
            {
                filter_processors[i]->start_env_release();
                filter_processors[i]->start_input_env_release(0);
                filter_processors[i]->start_input_env_release(1);
                filter_processors[i]->start_input_env_release(2);
#ifdef POLY
                filter_volume_tracking_envs[i]->set_to_release();
#endif
            }
        }

        if (not(synth_data->arp_sequencer_data->is_sequencer &&
                synth_data->arp_sequencer_data->is_on &&
                (!synth_data->keep_arp_always_off || synth_data->keep_arp_always_on)) &&
            tmp_note_down_store->size() == 0)
        {
            current_note = -1;
        }
    }
}

void MoniqueSynthesiserVoice::stopNote(float note_number_, bool allowTailOff) {}
void MoniqueSynthesiserVoice::stop_internal() noexcept
{
    stopped_and_sostenuto_pedal_was_down = is_sostenuto_pedal_down;
    stopped_and_sustain_pedal_was_down = is_sustain_pedal_down;

    if (!is_sostenuto_pedal_down)
    {
        eq_processor->start_release();
        for (int voice_id = 0; voice_id != SUM_FILTERS; ++voice_id)
        {
            filter_processors[voice_id]->start_release();
        }
    }
    fx_processor->start_release(is_sustain_pedal_down, is_sostenuto_pedal_down);
}

//==============================================================================
void MoniqueSynthesiserVoice::release_if_inactive() noexcept
{
    bool is_arp_on = synth_data->arp_sequencer_data->is_on || synth_data->keep_arp_always_on;
    if (synth_data->keep_arp_always_off)
    {
        is_arp_on = false;
    }

    if (fx_processor->final_env->get_current_stage() == END_ENV &&
        !synth_data->audio_processor->amp_painter)
    {
        bool has_steps_enabled = false;
        for (int i = 0; i != SUM_ENV_ARP_STEPS; ++i)
        {
            if (synth_data->arp_sequencer_data->step[i])
            {
                has_steps_enabled = true;
                break;
            }
        }
        if ((is_arp_on && !has_steps_enabled) || !is_arp_on)
        {
            if (fx_processor->zero_samples_counter > fx_processor->delay.get_max_duration() + 10)
            {
                current_note = -1;
                clearCurrentNote();
            }
        }
    }
    else
    {
        fx_processor->zero_samples_counter = 0;
    }
}

//==============================================================================
void MoniqueSynthesiserVoice::renderNextBlock(juce::AudioSampleBuffer &output_buffer_,
                                              int start_sample_, int num_samples_)
{
    // GET POSITION INFOS
    if (is_standalone())
    {
        if (synth_data->sync)
        {
            if (info->standalone_features_pimpl->is_extern_synced)
            {
                info->bpm = audio_processor->get_current_pos_info().bpm;
            }
            else
            {
                info->bpm = synth_data->speed;
            }
        }
        else
        {
            info->standalone_features_pimpl->is_extern_synced = false;
            info->bpm = synth_data->speed;
        }
    }
    else
    {
        if (synth_data->sync)
        {
            info->bpm = audio_processor->get_current_pos_info().bpm;
        }
        else
        {
            info->bpm = synth_data->speed;
        }
    }

    info->samples_since_start = audio_processor->get_current_pos_info().timeInSamples;
    if (audio_processor->get_current_pos_info().isPlaying)
    {
        info->relative_samples_since_start = info->samples_since_start;
    }

    int count_start_sample = start_sample_;
    int counted_samples = num_samples_;
    bool is_a_step = false;
    const bool connect = synth_data->arp_sequencer_data->connect;
    bool is_arp_on = synth_data->arp_sequencer_data->is_on ||
                     (synth_data->keep_arp_always_on && !synth_data->keep_arp_always_off);
    if (synth_data->keep_arp_always_off)
    {
        is_arp_on = false;
    }
    while (counted_samples > 0)
    {
        // SEARCH FOR STEPS (16ths) IN THE CURRENT BUFFER RANGE
        int samples_to_next_arp_step_in_this_buffer =
            arp_sequencer->process_samples_to_next_step(count_start_sample, counted_samples);
        counted_samples -= samples_to_next_arp_step_in_this_buffer;

        // RENDER THE NEXT BLOCK
        int step_id = arp_sequencer->get_current_step();

        // FORCE AN ARP RESTART
        if (sample_position_for_restart_arp > -1)
        {
            if (step_id == 0)
            {
                start_internal(current_note, current_velocity, counted_samples + start_sample_,
                               false);
                an_arp_note_is_already_running = true;
                sample_position_for_restart_arp = -1;
            }
        }
        else if (is_arp_on && current_note == -1)
        {
            // current_note = arp_info.current_note != -1 ? arp_info.current_note :
            // 60+synth_data->note_offset.get_value()-24; current_velocity =
            // arp_info.current_velocity != 0 ? arp_info.current_velocity : 1;

            // AUTO START
            {
                // current_note = 60+synth_data->note_offset.get_value()-24;
                // current_velocity = 1;
            }
        }

        // HANDLE RETIGGERS
        is_a_step = arp_sequencer->found_last_process_a_step();
        bool is_step_enabled = arp_sequencer->last_found_step_is_enabled();
        if (is_a_step && an_arp_note_is_already_running &&
            (!connect || !is_arp_on || !is_step_enabled))
        {
            stop_arp_controlled();
            an_arp_note_is_already_running = false;
        }

        const bool is_a_new_arp_step_to_start = (is_arp_on && is_a_step && is_step_enabled);
        if (is_a_new_arp_step_to_start)
        {
            current_running_arp_step = step_id;
            start_internal(current_note, current_velocity, counted_samples + start_sample_, false);
            an_arp_note_is_already_running = true;
        }

        if (samples_to_next_arp_step_in_this_buffer > 0)
        {
            render_block(output_buffer_, is_a_step ? step_id : -1,
                         arp_sequencer->get_current_absolute_step(), count_start_sample,
                         samples_to_next_arp_step_in_this_buffer);
        }
        count_start_sample += samples_to_next_arp_step_in_this_buffer;
    }

    // FREE IT
    release_if_inactive();

    if (!audio_processor->get_current_pos_info().isPlaying)
    {
        info->relative_samples_since_start += num_samples_;
    }
}

inline void SmoothManager::smooth_and_morph(bool force_by_load_, bool is_automated_morph_,
                                            const float *morph_power_buffer_, int num_samples_,
                                            int smooth_motor_time_in_ms_,
                                            int morph_motor_time_in_ms_,
                                            MorphGroup *morph_group_) noexcept
{
    // PROCESS THE MORPH
    for (int i = 0; i != smoothers.size(); ++i)
    {
        SmoothedParameter *param = smoothers.getUnchecked(i);
        const int index = morph_group_->indexOf(param->param_to_smooth);
        if (index != -1)
        {
            if (param->param_to_smooth->get_runtime_info().smoothing_is_enabled)
            {
                const Parameter *left_param = morph_group_->get_left_param(index);
                const Parameter *right_param = morph_group_->get_right_param(index);
                param->smooth_and_morph(force_by_load_, is_automated_morph_,
                                        smooth_motor_time_in_ms_, morph_motor_time_in_ms_,
                                        morph_power_buffer_, morph_group_->last_power_of_right,
                                        left_param, right_param, num_samples_);
            }
        }
    }
}

#ifdef JUCE_DEBUG
#define DEBUG_CHECK_MIN_MAX(x)                                                                     \
    if (x > max_value)                                                                             \
    {                                                                                              \
        std::cout << "x to large: " << x << " max: " << max_value << std::endl;                    \
    }                                                                                              \
    else if (x < min_value)                                                                        \
    {                                                                                              \
        std::cout << "x to small: " << x << " max: " << min_value << std::endl;                    \
    }
#else
#define DEBUG_CHECK_MIN_MAX(x)
#endif

// TOOPT
#define FORCE_MIN_MAX(x) juce::jmax(juce::jmin(x, max_value), min_value)

void SmoothedParameter::simple_smooth(int smooth_motor_time_in_ms_, int num_samples_) noexcept
{
    simple_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);
    simple_smoother.set_value(param_to_smooth->get_value());
    float *const target = values.getWritePointer();
    if (!simple_smoother.is_up_to_date())
    {

        for (int sid = 0; sid != num_samples_; ++sid)
        {
            target[sid] = FORCE_MIN_MAX(simple_smoother.tick());
        }

        simple_smoother.set_info_flag(false);
    }
    else
    {
        if (!simple_smoother.get_info_flag())
        {
            juce::FloatVectorOperations::fill(target, simple_smoother.get_last_value(), block_size);

            simple_smoother.set_info_flag(true);
        }
    }

    param_to_smooth->get_runtime_info().set_last_value_state(target[num_samples_ - 1]);
}
// TOOPD FloatVectorOperations
void SmoothedParameter::smooth_and_morph(bool force_by_load_, bool is_automated_morph_,
                                         int smooth_motor_time_in_ms_, int morph_motor_time_in_ms_,
                                         const float *morph_amp_buffer_, float morph_slider_state_,
                                         const Parameter *left_source_param_,
                                         const Parameter *right_source_param_,
                                         int num_samples_) noexcept
{
    left_morph_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);
    right_morph_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);
    morph_power_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);

    left_modulation_morph_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);
    right_modulation_morph_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);

    // LOOKING FORWART TO PROCESS MODUALATION AND AMP MODUALATION
    modulation_power_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);
    amp_power_smoother.reset_coefficients(sample_rate, smooth_motor_time_in_ms_);

    if (force_by_load_)
    {
        morph_power_smoother.set_value(morph_power_smoother.get_last_value());
        // right_morph_smoother.reset( sample_rate, smooth_motor_time_in_ms_ );
        // left_morph_smoother.reset( sample_rate, smooth_motor_time_in_ms_ );
    }

    const bool is_modulateable = has_modulation(param_to_smooth);
    float *const target = values.getWritePointer();
    if (!is_modulateable)
    {
        left_morph_smoother.set_value(left_source_param_->get_value());
        right_morph_smoother.set_value(right_source_param_->get_value());

        // AUTOMATED MORPH
        if (is_automated_morph_)
        {
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                const float power_of_right_ =
                    morph_power_smoother.glide_tick(morph_amp_buffer_[sid]);
                target[sid] = FORCE_MIN_MAX(left_morph_smoother.tick() * (1.0f - power_of_right_) +
                                            right_morph_smoother.tick() * power_of_right_);
            }
        }
        // USER MORPH
        else
        {
            morph_power_smoother.set_value(morph_slider_state_);
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                const float power_of_right = morph_power_smoother.tick();
                target[sid] = FORCE_MIN_MAX(left_morph_smoother.tick() * (1.0f - power_of_right) +
                                            right_morph_smoother.tick() * power_of_right);
            }

            // KEEP UP TO DATE FOR A SWITCH
            morph_power_smoother.reset_glide_countdown();
        }
    }
    // ITS A COPY OF THE PROCESS ABOVE BUT WITH MODULATION
    else
    {
        left_morph_smoother.set_value(left_source_param_->get_value());
        right_morph_smoother.set_value(right_source_param_->get_value());

        left_modulation_morph_smoother.set_value(left_source_param_->get_modulation_amount());
        right_modulation_morph_smoother.set_value(right_source_param_->get_modulation_amount());

        // AUTOMATED MORPH
        float *const target_modulation = modulation_power.getWritePointer();
        if (is_automated_morph_)
        {
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                const float power_of_right_ =
                    morph_power_smoother.glide_tick(morph_amp_buffer_[sid]);

                // VALUE
                target[sid] = FORCE_MIN_MAX(left_morph_smoother.tick() * (1.0f - power_of_right_) +
                                            right_morph_smoother.tick() * power_of_right_);
                // MODULATION
                target_modulation[sid] =
                    left_modulation_morph_smoother.tick() * (1.0f - power_of_right_) +
                    right_modulation_morph_smoother.tick() * power_of_right_;
            }
        }
        // USER MORPH
        else
        {
            morph_power_smoother.set_value(morph_slider_state_);
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                const float power_of_right = morph_power_smoother.tick();
                // VALUE BLOCK
                target[sid] = FORCE_MIN_MAX(left_morph_smoother.tick() * (1.0f - power_of_right) +
                                            right_morph_smoother.tick() * power_of_right);
                // MODULATION BLOCK
                target_modulation[sid] =
                    left_modulation_morph_smoother.tick() * (1.0f - power_of_right) +
                    right_modulation_morph_smoother.tick() * power_of_right;
            }

            // KEEP UP TO DATE FOR A SWITCH
            morph_power_smoother.reset_glide_countdown();
        }
        param_to_smooth->get_runtime_info().set_last_modulation_state(
            target_modulation[num_samples_ - 1]);
    }

    param_to_smooth->get_runtime_info().set_last_value_state(target[num_samples_ - 1]);
}
// TOOPD FloatVectorOperations
inline void SmoothedParameter::process_modulation(const bool is_modulated_,
                                                  const float *modulator_power_buffer_,
                                                  int num_samples_) noexcept
{
    // modulation_power_smoother.reset_coefficients( sample_rate, glide_motor_time_in_ms_ ); <---
    // NOTE: DONE BY SMOOTH_AND_MORPH

    float *const source_and_target = values.getWritePointer();
    const float *const modulation = modulation_power.getReadPointer();
    if (is_modulated_)
    {
        float current_modulation_power;
        for (int sid = 0; sid != num_samples_; ++sid)
        {
            current_modulation_power = modulation[sid] * modulation_power_smoother.glide_tick(
                                                             modulator_power_buffer_[sid]);
            const float in = source_and_target[sid];
            if (current_modulation_power > 0)
            {
                source_and_target[sid] = in + (max_value - in) * current_modulation_power;
                DEBUG_CHECK_MIN_MAX(source_and_target[sid]);
            }
            else
            {
                source_and_target[sid] = in + (in - min_value) * current_modulation_power;
                DEBUG_CHECK_MIN_MAX(source_and_target[sid]);
            }
        }

        param_to_smooth->get_runtime_info().set_last_modulation_amount(current_modulation_power);
    }
    else
    {
        modulation_power_smoother.set_value(0);
        float current_modulation_power = 0;

        if (!modulation_power_smoother.is_up_to_date())
        {
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                current_modulation_power = modulation[sid] * modulation_power_smoother.tick();
                const float in = source_and_target[sid];
                if (current_modulation_power < 0)
                {
                    source_and_target[sid] = in + (in - min_value) * current_modulation_power;
                    DEBUG_CHECK_MIN_MAX(source_and_target[sid]);
                }
                else
                {
                    source_and_target[sid] = in + (max_value - in) * current_modulation_power;
                    DEBUG_CHECK_MIN_MAX(source_and_target[sid]);
                }
            }
        }

        param_to_smooth->get_runtime_info().set_last_modulation_amount(current_modulation_power);

        // KEEP UP TO DATE FOR A SWITCH
        modulation_power_smoother.reset_glide_countdown();
    }
}
// TOOPD FloatVectorOperations
void SmoothedParameter::process_amp(bool use_env_, int glide_time_in_ms_, ENV *env_,
                                    float *amp_buffer_, int num_samples_) noexcept
{
    const float *source = values.getReadPointer();
    if (use_env_)
    {
        env_->process(amp_buffer_, num_samples_);

        for (int sid = 0; sid != num_samples_; ++sid)
        {
            const float current_amp_power = amp_power_smoother.glide_tick(amp_buffer_[sid]);
            amp_buffer_[sid] = source[sid] * current_amp_power;
            DEBUG_CHECK_MIN_MAX(amp_buffer_[sid]);
        }

        param_to_smooth->get_runtime_info().set_last_modulation_amount(
            amp_buffer_[num_samples_ - 1]);

        amp_power_smoother.set_info_flag(false);
    }
    else
    {
        if (!amp_power_smoother.get_info_flag())
        {
            amp_power_smoother.set_value(1);
            amp_power_smoother.set_info_flag(true);
        }
        if (!amp_power_smoother.is_up_to_date())
        {
            env_->process(amp_buffer_, num_samples_);
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                amp_buffer_[sid] = source[sid] * amp_power_smoother.tick();
                DEBUG_CHECK_MIN_MAX(amp_buffer_[sid]);
            }
        }
        else
        {
            juce::FloatVectorOperations::copy(amp_buffer_, source, num_samples_);

            // RESET ENVELOP TO BE UP TO DATE ON A SWITCH
            env_->overwrite_current_value(amp_buffer_[num_samples_ - 1]);
        }

        param_to_smooth->get_runtime_info().set_last_modulation_amount(
            amp_buffer_[num_samples_ - 1]);

        // KEEP UP TO DATE FOR A SWITCH
        amp_power_smoother.reset_glide_countdown();
    }
}
void MoniqueSynthesiserVoice::render_block(juce::AudioSampleBuffer &output_buffer_,
                                           int step_number_, int absolute_step_number_,
                                           int start_sample_, int num_samples_) noexcept
{
    const bool render_anything = current_note != -1 || synth_data->audio_processor->amp_painter;

    const int num_samples = num_samples_;
    if (num_samples == 0)
    {
        return;
    }

    if (step_number_ != -1)
    {
        current_step = step_number_;
    }

    if (Monique_Ui_AmpPainter *amp_painter = synth_data->audio_processor->amp_painter)
    {
        amp_painter->calc_new_cycle();
    }

    // CHECK POSSIBLE BYPASS
    bool must_process = !bypass_smoother.is_up_to_date() || render_anything;
    if (!must_process)
    {
        must_process =
            fx_processor->zero_samples_counter < fx_processor->delay.get_max_duration() + 10;
    }
    if (!must_process)
    {
        must_process = fx_processor->final_env->get_current_stage() != END_ENV;
    }
    if (must_process)
    {
        const int glide_motor_time = synth_data->glide_motor_time;
        const int morph_motor_time = synth_data->morph_motor_time;

        // WORKAROUND TO UPDATE THE MORPH GROUPS
        const bool force_by_load = synth_data->force_morph_update__load_flag;
        synth_data->force_morph_update__load_flag = false;
        /*
        if( synth_data->force_morph_update__load_flag )
               {
                   synth_data->force_morph_update__load_flag = false;
                   synth_data->morhp_states[0].notify_value_listeners();
                   synth_data->morhp_states[1].notify_value_listeners();
                   synth_data->morhp_states[2].notify_value_listeners();
                   synth_data->morhp_states[3].notify_value_listeners();
                   synth_data->morhp_switch_states[0].notify_value_listeners();
                   synth_data->morhp_switch_states[1].notify_value_listeners();
                   synth_data->morhp_switch_states[2].notify_value_listeners();
                   synth_data->morhp_switch_states[3].notify_value_listeners();
               }
               */
        synth_data->delay_record_release_smoother.simple_smooth(glide_motor_time, num_samples);
#ifdef POLY
        if (synth_data->keytrack_filter_volume[0])
            filter_volume_tracking_envs[0]->process(
                synth_data->data_buffer->filter_env_tracking.getWritePointer(0), num_samples);
        else
            filter_volume_tracking_envs[0]->reset();
        if (synth_data->keytrack_filter_volume[1])
            filter_volume_tracking_envs[1]->process(
                synth_data->data_buffer->filter_env_tracking.getWritePointer(1), num_samples);
        else
            filter_volume_tracking_envs[1]->reset();
        if (synth_data->keytrack_filter_volume[2])
            filter_volume_tracking_envs[2]->process(
                synth_data->data_buffer->filter_env_tracking.getWritePointer(2), num_samples);
        else
            filter_volume_tracking_envs[2]->reset();
#endif
        {
            struct SmoothExecuter
            {
                float *const mfo_buffer;
                float *const lfo_buffer;

                LFO *const mfo;
                LFO *const lfo;
                MasterOSC *const master_osc;
                SecondOSC *const second_osc;
                ENV *const filter_env;
                float *const filter_env_buffer;

                MoniqueSynthesiserVoice *const voice;
                SmoothManager *const smooth_manager;
                MorphGroup *const morph_group;
                LFOData *const mfo_data;
                MoniqueSynthData *const synth_data;
                const bool is_modulated;

                const int step_number;
                const int absolute_step_number;
                const int start_sample;
                const int num_samples;

                const int glide_motor_time;
                const int morph_motor_time;

                const bool force_by_load;

                void exec() noexcept
                {
                    mfo_data->wave_smoother.simple_smooth(glide_motor_time, num_samples);
                    mfo_data->phase_shift_smoother.simple_smooth(glide_motor_time, num_samples);

                    mfo->process(mfo_buffer, step_number, absolute_step_number, start_sample,
                                 num_samples);
                    synth_data->smooth_manager->smooth_and_morph(
                        force_by_load, is_modulated, mfo_buffer, num_samples, glide_motor_time,
                        morph_motor_time, morph_group);

                    if (lfo)
                    {
                        lfo->process(lfo_buffer, step_number, absolute_step_number, start_sample,
                                     num_samples);
                    }
                    if (master_osc)
                    {
                        master_osc->process(synth_data->data_buffer, num_samples); // NEED LFO 0
                    }
                    if (second_osc)
                    {
                        second_osc->process(synth_data->data_buffer, num_samples); // NEED LFO 0
                    }
                    if (filter_env)
                    {
                        filter_env->process(filter_env_buffer, num_samples); // NEED LFO 0
                    }
                }

                SmoothExecuter(MoniqueSynthesiserVoice *const voice_, float *const mfo_buffer_,
                               float *const lfo_buffer_,

                               LFO *const mfo_, LFO *const lfo_, MasterOSC *const master_osc_,
                               SecondOSC *const second_osc_, ENV *const filter_env_,
                               float *const filter_env_buffer_,

                               MorphGroup *const morph_group_, LFOData *const mfo_data_,
                               MoniqueSynthData *const synth_data_, const bool is_modulated_,

                               int step_number_, int absolute_step_number_, int start_sample_,
                               int num_samples_,

                               int glide_motor_time_, int morph_motor_time_,

                               bool force_by_load_

                               ) noexcept
                    : mfo_buffer(mfo_buffer_), lfo_buffer(lfo_buffer_),

                      mfo(mfo_), lfo(lfo_), master_osc(master_osc_), second_osc(second_osc_),
                      filter_env(filter_env_), filter_env_buffer(filter_env_buffer_),

                      voice(voice_), smooth_manager(synth_data_->smooth_manager),
                      morph_group(morph_group_), mfo_data(mfo_data_), synth_data(synth_data_),

                      is_modulated(is_modulated_),

                      step_number(step_number_), absolute_step_number(absolute_step_number_),
                      start_sample(start_sample_), num_samples(num_samples_),

                      glide_motor_time(glide_motor_time_), morph_motor_time(morph_motor_time_),

                      force_by_load(force_by_load_)
                {
                }
                ~SmoothExecuter() noexcept {}
            };

            // MASTER THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(0),
                           data_buffer->lfo_amplitudes.getWritePointer(0),

                           mfos[0], lfos[0], master_osc, nullptr,

                           filter_processors[0]->env.get(), // FIRST WILL BE DONE BY THE LAST THREAD
                           data_buffer->filter_env_amps.getWritePointer(0),

                           synth_data->morph_group_1.get(), synth_data->mfo_datas[0], synth_data,

                           synth_data->is_morph_modulated[0],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // OPTIONAL THREAD WITH FILTER 1
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(1),
                           data_buffer->lfo_amplitudes.getWritePointer(1),

                           mfos[1], lfos[1], nullptr,
                           second_osc, // NEED OSC

                           filter_processors[1]->env.get(), // FIRST WILL BE DONE BY THE LAST THREAD
                           data_buffer->filter_env_amps.getWritePointer(1),

                           synth_data->morph_group_2.get(), synth_data->mfo_datas[1], synth_data,

                           synth_data->is_morph_modulated[1],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // OPTIONAL THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(2),
                           data_buffer->lfo_amplitudes.getWritePointer(2),

                           mfos[2], lfos[2], nullptr,
                           third_osc, // NEED OSC 0

                           filter_processors[2]->env.get(),
                           data_buffer->filter_env_amps.getWritePointer(2),

                           synth_data->morph_group_3.get(), synth_data->mfo_datas[2], synth_data,

                           synth_data->is_morph_modulated[2],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // MASTER THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(3), nullptr,

                           mfos[3], nullptr, nullptr, nullptr,

                           nullptr, nullptr,

                           synth_data->morph_group_4.get(), synth_data->mfo_datas[3], synth_data,

                           synth_data->is_morph_modulated[3],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // WITH THREADING INSIDE
            filter_processors[0]->process(num_samples);
            filter_processors[1]->process(num_samples);
            filter_processors[2]->process(num_samples);

            eq_processor->process(num_samples);
        }

        float velocity_to_use = current_velocity;
        bool is_arp_on = synth_data->arp_sequencer_data->is_on || synth_data->keep_arp_always_on;
        if (synth_data->keep_arp_always_off)
        {
            is_arp_on = false;
        }
        float *const velocity_buffer = data_buffer->velocity_buffer.getWritePointer();
        if (is_arp_on)
        {
            const float *const smoothed_arp_step_velocity =
                synth_data->arp_sequencer_data->velocity_smoothers[current_running_arp_step]
                    ->get_smoothed_value_buffer();
            for (int sid = 0; sid != num_samples_; ++sid)
            {
                velocity_buffer[sid] = velocity_to_use * smoothed_arp_step_velocity[sid];
            }
        }
        else
        {
            juce::FloatVectorOperations::fill(velocity_buffer, velocity_to_use, num_samples_);
        }

        fx_processor->process(output_buffer_, velocity_buffer, start_sample_, num_samples_);

        bypass_smoother.set_info_flag(false);
    }
    else
    {
        const int glide_motor_time = synth_data->glide_motor_time;
        const int morph_motor_time = synth_data->morph_motor_time;

        // WORKAROUND TO UPDATE THE MORPH GROUPS
        const bool force_by_load = synth_data->force_morph_update__load_flag;
        synth_data->force_morph_update__load_flag = false;
        /*
        if( synth_data->force_morph_update__load_flag )
               {
                   synth_data->force_morph_update__load_flag = false;
                   synth_data->morhp_states[0].notify_value_listeners();
                   synth_data->morhp_states[1].notify_value_listeners();
                   synth_data->morhp_states[2].notify_value_listeners();
                   synth_data->morhp_states[3].notify_value_listeners();
                   synth_data->morhp_switch_states[0].notify_value_listeners();
                   synth_data->morhp_switch_states[1].notify_value_listeners();
                   synth_data->morhp_switch_states[2].notify_value_listeners();
                   synth_data->morhp_switch_states[3].notify_value_listeners();
               }
               */

        synth_data->delay_record_release_smoother.simple_smooth(glide_motor_time, num_samples);

        {
            struct SmoothExecuter
            {
                float *const mfo_buffer;
                float *const lfo_buffer;

                LFO *const mfo;
                LFO *const lfo;
                MasterOSC *const master_osc;
                SecondOSC *const second_osc;
                ENV *const filter_env;
                float *const filter_env_buffer;

                MoniqueSynthesiserVoice *const voice;
                SmoothManager *const smooth_manager;
                MorphGroup *const morph_group;
                LFOData *const mfo_data;
                MoniqueSynthData *const synth_data;
                const bool is_modulated;

                const int step_number;
                const int absolute_step_number;
                const int start_sample;
                const int num_samples;

                const int glide_motor_time;
                const int morph_motor_time;

                const bool force_by_load;

                void exec() noexcept
                {
                    mfo_data->wave_smoother.simple_smooth(glide_motor_time, num_samples);
                    mfo_data->phase_shift_smoother.simple_smooth(glide_motor_time, num_samples);

                    mfo->process(mfo_buffer, step_number, absolute_step_number, start_sample,
                                 num_samples);
                    synth_data->smooth_manager->smooth_and_morph(
                        force_by_load, is_modulated, mfo_buffer, num_samples, glide_motor_time,
                        morph_motor_time, morph_group);
                    /*
                    if( lfo )
                    {
                        lfo->process( lfo_buffer, step_number, absolute_step_number, start_sample,
                    num_samples );
                    }
                    if( master_osc )
                    {
                        master_osc->process( synth_data->data_buffer, num_samples ); // NEED LFO 0
                    }
                    if( second_osc )
                    {
                        second_osc->process( synth_data->data_buffer, num_samples ); // NEED LFO 0
                    }
                    if( filter_env )
                    {
                        filter_env->process( filter_env_buffer, num_samples ); // NEED LFO 0
                    }
                    */
                }

                SmoothExecuter(MoniqueSynthesiserVoice *const voice_, float *const mfo_buffer_,
                               float *const lfo_buffer_,

                               LFO *const mfo_, LFO *const lfo_, MasterOSC *const master_osc_,
                               SecondOSC *const second_osc_, ENV *const filter_env_,
                               float *const filter_env_buffer_,

                               MorphGroup *const morph_group_, LFOData *const mfo_data_,
                               MoniqueSynthData *const synth_data_, const bool is_modulated_,

                               int step_number_, int absolute_step_number_, int start_sample_,
                               int num_samples_,

                               int glide_motor_time_, int morph_motor_time_,

                               bool force_by_load_

                               ) noexcept
                    : mfo_buffer(mfo_buffer_), lfo_buffer(lfo_buffer_),

                      mfo(mfo_), lfo(lfo_), master_osc(master_osc_), second_osc(second_osc_),
                      filter_env(filter_env_), filter_env_buffer(filter_env_buffer_),

                      voice(voice_), smooth_manager(synth_data_->smooth_manager),
                      morph_group(morph_group_), mfo_data(mfo_data_), synth_data(synth_data_),

                      is_modulated(is_modulated_),

                      step_number(step_number_), absolute_step_number(absolute_step_number_),
                      start_sample(start_sample_), num_samples(num_samples_),

                      glide_motor_time(glide_motor_time_), morph_motor_time(morph_motor_time_),

                      force_by_load(force_by_load_)
                {
                }
                ~SmoothExecuter() noexcept {}
            };

            // MASTER THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(0),
                           data_buffer->lfo_amplitudes.getWritePointer(0),

                           mfos[0], lfos[0], master_osc, nullptr,

                           filter_processors[0]->env.get(), // FIRST WILL BE DONE BY THE LAST THREAD
                           data_buffer->filter_env_amps.getWritePointer(0),

                           synth_data->morph_group_1.get(), synth_data->mfo_datas[0], synth_data,

                           synth_data->is_morph_modulated[0],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // OPTIONAL THREAD WITH FILTER 1
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(1),
                           data_buffer->lfo_amplitudes.getWritePointer(1),

                           mfos[1], lfos[1], nullptr,
                           second_osc, // NEED OSC

                           filter_processors[1]->env.get(), // FIRST WILL BE DONE BY THE LAST THREAD
                           data_buffer->filter_env_amps.getWritePointer(1),

                           synth_data->morph_group_2.get(), synth_data->mfo_datas[1], synth_data,

                           synth_data->is_morph_modulated[1],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // OPTIONAL THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(2),
                           data_buffer->lfo_amplitudes.getWritePointer(2),

                           mfos[2], lfos[2], nullptr,
                           third_osc, // NEED OSC 0

                           filter_processors[2]->env.get(),
                           data_buffer->filter_env_amps.getWritePointer(2),

                           synth_data->morph_group_3.get(), synth_data->mfo_datas[2], synth_data,

                           synth_data->is_morph_modulated[2],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();

            // MASTER THREAD
            SmoothExecuter(this, data_buffer->mfo_amplitudes.getWritePointer(3), nullptr,

                           mfos[3], nullptr, nullptr, nullptr,

                           nullptr, nullptr,

                           synth_data->morph_group_4.get(), synth_data->mfo_datas[3], synth_data,

                           synth_data->is_morph_modulated[3],

                           step_number_, absolute_step_number_, start_sample_, num_samples,

                           glide_motor_time, morph_motor_time,

                           force_by_load)
                .exec();
        }

        if (!bypass_smoother.get_info_flag())
        {
            master_osc->reset();
            second_osc->reset();
            third_osc->reset();
            for (int i = 0; i != SUM_LFOS; ++i)
            {
                lfos[i]->reset();
            }
            for (int i = 0; i != SUM_MORPHER_GROUPS; ++i)
            {
                mfos[i]->reset();
            }
            for (int i = 0; i != SUM_FILTERS; ++i)
            {
                filter_processors[i]->reset();
            }

            bypass_smoother.set_info_flag(true);
        }
    }

    // VISUALIZE
    if (Monique_Ui_AmpPainter *amp_painter = synth_data->audio_processor->amp_painter)
    {
        amp_painter->add_master_osc(data_buffer->osc_samples.getReadPointer(0),
                                    data_buffer->osc_switchs.getReadPointer(0), num_samples_);
        amp_painter->add_osc(1, data_buffer->osc_samples.getReadPointer(1), num_samples_);
        amp_painter->add_osc(2, data_buffer->osc_samples.getReadPointer(2), num_samples_);
    }

    // UI INFORMATION
    for (int i = 0; i != SUM_OSCS; ++i)
    {
        synth_data->osc_datas[i]->last_modulation_value =
            data_buffer->lfo_amplitudes.getReadPointer(i)[num_samples - 1];
    }
}

void MoniqueSynthesiserVoice::pitchWheelMoved(int pitch_)
{
    pitch_offset =
        (pitch_ > 0x2000 ? 2.0f / 0x2000 * (pitch_ - 0x2000) : -2.0f / 0x2000 * (0x2000 - pitch_));

    bool is_arp_on = synth_data->arp_sequencer_data->is_on || synth_data->keep_arp_always_on;
    if (synth_data->keep_arp_always_off)
    {
        is_arp_on = false;
    }
    float arp_offset = is_arp_on ? arp_sequencer->get_current_tune() : 0;
    master_osc->update(current_note + arp_offset + pitch_offset, 0);
    second_osc->update(current_note + arp_offset + pitch_offset, 0);
    third_osc->update(current_note + arp_offset + pitch_offset, 0);
}

//==============================================================================
void MoniqueSynthesiserVoice::reset(bool force) noexcept
{
    if (current_note != -1 || force)
    {
        current_note = -1;
        reset_internal();
        clearCurrentNote();
    }
}
void MoniqueSynthesiserVoice::reset_internal() noexcept
{
    arp_sequencer->reset();
    for (int voice_id = 0; voice_id != SUM_FILTERS; ++voice_id)
    {
        filter_processors[voice_id]->reset();
    }
    eq_processor->reset();
    fx_processor->reset();
    /*
        master_osc->reset();
        second_osc->reset();
        third_osc->reset();
        */
}
void MoniqueSynthesiserVoice::handle_sustain_pedal(bool down_) noexcept
{
    is_sustain_pedal_down = down_;
    if (!down_)
    {
        if (stopped_and_sustain_pedal_was_down)
        {
            stop_internal();
        }
    }
}
void MoniqueSynthesiserVoice::handle_sostueno_pedal(bool down_) noexcept
{
    is_sostenuto_pedal_down = down_;
    if (!down_)
    {
        if (stopped_and_sostenuto_pedal_was_down)
        {
            stop_internal();
        }
    }
}
void MoniqueSynthesiserVoice::handle_soft_pedal(bool down_) noexcept { is_soft_pedal_down = down_; }

//==============================================================================
void MoniqueSynthesiserVoice::clear_record_buffer() noexcept
{
    fx_processor->delay.clear_record_buffer();
}
float MoniqueSynthesiserVoice::get_filter_env_amp(int filter_id_) const noexcept
{
    return filter_processors[filter_id_]->env->get_amp();
}
float MoniqueSynthesiserVoice::get_lfo_amp(int lfo_id_) const noexcept
{
    return lfos[lfo_id_]->get_current_amp();
}
float MoniqueSynthesiserVoice::get_mfo_amp(int lfo_id_) const noexcept
{
    return mfos[lfo_id_]->get_current_amp();
}
float MoniqueSynthesiserVoice::get_arp_sequence_amp(int step_) const noexcept
{
    if (arp_sequencer->get_current_step() == step_)
    {
        return fx_processor->final_env->get_amp() * 0.99f + 0.01f;
    }

    return 0;
}
float MoniqueSynthesiserVoice::get_current_frequency() const noexcept
{
    return juce::MidiMessage::getMidiNoteInHertz(current_note + arp_sequencer->get_current_tune());
}

//==============================================================================
//==============================================================================
//==============================================================================
void MoniqueSynthesizer::handleSustainPedal(int, bool isDown)
{
    voice->handle_sustain_pedal(isDown);
}

void MoniqueSynthesizer::handleSostenutoPedal(int, bool isDown)
{
    voice->handle_sostueno_pedal(isDown);
}

void MoniqueSynthesizer::handleSoftPedal(int, bool isDown) { voice->handle_soft_pedal(isDown); }
void MoniqueSynthesizer::handleBankSelect(int controllerValue) noexcept
{
    synth_data->set_current_bank(juce::jmin(25, controllerValue));
}
void MoniqueSynthesizer::handleProgramChange(int midiChannel, int programNumber)
{
    if (programNumber != synth_data->get_current_program())
    {
        synth_data->set_current_program(programNumber);
        if (programNumber == synth_data->get_current_program())
        {
            synth_data->load(true, true);
        }
    }
}

void MoniqueSynthesizer::handleController(int midiChannel, int cc_number_, int cc_value_)
{
    switch (cc_number_)
    {
    case 0:
        // FIXME on program change from gui we generate a lot of MIDI CC in messages
        //       one of these message is a bank select @ channel 12 (0:12:XXX)
        //       as hot fix bank selects via MIDI CC are disabled for now
        // handleBankSelect (cc_value_);
        break;
    case 0x40:
        handleSustainPedal(midiChannel, cc_value_ >= 64);
        break;
    case 0x42:
        handleSostenutoPedal(midiChannel, cc_value_ >= 64);
        break;
    case 0x43:
        handleSoftPedal(midiChannel, cc_value_ >= 64);
        break;
    default:
    {
        Parameter *const learing_param = midi_control_handler->is_learning();
        juce::Array<Parameter *> &paramters = synth_data->get_all_parameters();

        if (cc_number_ == -99)
        {
            cc_number_ = 0; // WE NOW USE PROGRAM CHANGE
        }

        // CONTROL
        if (!learing_param)
        {
            for (int i = 0; i != paramters.size(); ++i)
            {
                Parameter *const param = paramters.getUnchecked(i);
                if (param->midi_control->read_from_if_you_listen(
                        midiChannel == 2 ? cc_number_ + 128 : cc_number_, cc_value_,
                        synth_data->midi_pickup_offset))
                {
                    break;
                }
            }
        }
        // TRAIN
        else
        {
            if (midi_control_handler->handle_incoming_message(midiChannel == 2 ? cc_number_ + 128
                                                                               : cc_number_))
            {
                // CLEAR SIBLINGS IF WE HAVE SOMETHING SUCCESSFUL TRAINED
                juce::String learning_param_name = learing_param->get_info().name;
                for (int i = 0; i != paramters.size(); ++i)
                {
                    Parameter *const param = paramters.getUnchecked(i);
                    if (param != learing_param)
                    {
                        if (param->midi_control->get_is_ctrl_version_of_name() !=
                            learning_param_name)
                        {
                            if (param->midi_control->is_listen_to(
                                    midiChannel == 2 ? cc_number_ + 128 : cc_number_))
                            {
                                param->midi_control->clear();
                            }
                        }
                    }
                }
            }
        }
    }
    }
}
void MoniqueSynthesizer::handlePitchWheel(int /*midiChannel*/, int wheelValue)
{
    voice->pitchWheelMoved(wheelValue);
}

//==============================================================================
void MoniqueSynthesizer::NoteDownStore::add_note(const juce::MidiMessage &midi_message_,
                                                 int play_mode_) noexcept
{
    if (!notes_down.contains(midi_message_))
    {
        notes_down.add(NoteDownStore::MidiMessageCompareable(midi_message_));
        bool success = false;
        for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
        {
            if (notes_down_order.getUnchecked(i) == nullptr)
            {
                notes_down_order.getReference(i) =
                    new NoteDownStore::MidiMessageCompareable(midi_message_);
                success = true;
                break;
            }
        }
        // REPLACE FIRST
        if (!success)
        {
            juce::Array<MidiMessageCompareable *> messages;
            juce::Array<MidiMessageCompareable *> messagesToKill;
            for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
            {
                if (MidiMessageCompareable *message = notes_down_order.getUnchecked(i))
                {
                    messages.add(message);
                }
            }

            if (messages.size())
            {
                if (play_mode_ == PLAY_MODES::LOW)
                {
                    int higest_note = -1;
                    if (messages.size() == 2)
                    {
                        higest_note =
                            juce::jmax(messages[0]->getNoteNumber(), messages[1]->getNoteNumber());
                    }
                    else if (messages.size() == 3)
                    {
                        higest_note =
                            juce::jmax(messages[0]->getNoteNumber(), messages[1]->getNoteNumber(),
                                       messages[2]->getNoteNumber());
                    }
                    else
                    {
                        higest_note = messages[0]->getNoteNumber();
                    }
                    if (higest_note > midi_message_.getNoteNumber())
                    {
                        for (int i = 0; i != messages.size(); ++i)
                        {
                            if (messages[i]->getNoteNumber() == higest_note)
                            {
                                int working_index = notes_down_order.indexOf(messages[i]);
                                messagesToKill.add(notes_down_order.getUnchecked(working_index));
                                notes_down_order.getReference(working_index) =
                                    new NoteDownStore::MidiMessageCompareable(midi_message_);
                                break;
                            }
                        }
                    }
                }
                else if (play_mode_ == PLAY_MODES::HIGH)
                {
                    int lowest_note = 999;
                    if (messages.size() == 2)
                    {
                        lowest_note =
                            juce::jmax(messages[0]->getNoteNumber(), messages[1]->getNoteNumber());
                    }
                    else if (messages.size() == 3)
                    {
                        lowest_note =
                            juce::jmax(messages[0]->getNoteNumber(), messages[1]->getNoteNumber(),
                                       messages[2]->getNoteNumber());
                    }
                    else
                    {
                        lowest_note = messages[0]->getNoteNumber();
                    }
                    if (lowest_note < midi_message_.getNoteNumber())
                    {
                        for (int i = 0; i != messages.size(); ++i)
                        {
                            if (messages[i]->getNoteNumber() == lowest_note)
                            {
                                int working_index = notes_down_order.indexOf(messages[i]);
                                messagesToKill.add(notes_down_order.getUnchecked(working_index));
                                notes_down_order.getReference(working_index) =
                                    new NoteDownStore::MidiMessageCompareable(midi_message_);
                                break;
                            }
                        }
                    }
                }
                else if (play_mode_ == PLAY_MODES::LIFO)
                {
                    for (int i = notes_down.size() - 1; i >= 0; --i)
                    {
                        for (int k = 0; k != messages.size(); ++k)
                        {
                            if (notes_down.getUnchecked(i).getNoteNumber() ==
                                messages.getUnchecked(k)->getNoteNumber())
                            {
                                int working_index = notes_down_order.indexOf(messages[k]);
                                messagesToKill.add(notes_down_order.getUnchecked(working_index));
                                notes_down_order.getReference(working_index) =
                                    new NoteDownStore::MidiMessageCompareable(midi_message_);
                                i = -1;
                                break;
                            }
                        }
                    }
                }
                else if (play_mode_ == PLAY_MODES::FIFO)
                {
                    for (int i = 0; i < notes_down.size(); ++i)
                    {
                        for (int k = 0; k != messages.size(); ++k)
                        {
                            if (notes_down.getReference(i).getNoteNumber() ==
                                messages.getUnchecked(k)->getNoteNumber())
                            {
                                int working_index = notes_down_order.indexOf(messages[k]);
                                messagesToKill.add(notes_down_order.getUnchecked(working_index));
                                notes_down_order.getReference(working_index) =
                                    new NoteDownStore::MidiMessageCompareable(midi_message_);
                                i = notes_down.size();
                                break;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i != messagesToKill.size(); ++i)
            {
                delete messagesToKill.getUnchecked(i);
            }
        }
    }
}
const juce::MidiMessage *
MoniqueSynthesizer::NoteDownStore::remove_note(const juce::MidiMessage &midi_message_,
                                               int play_mode_, bool reorder_allowed_) noexcept
{
    notes_down.removeFirstMatchingValue(midi_message_);
    for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
    {
        if (juce::MidiMessage *message = notes_down_order.getUnchecked(i))
        {
            if (message->getNoteNumber() == midi_message_.getNoteNumber())
            {
                if (reorder_allowed_)
                {
                    notes_down_order.getReference(i) = get_replacement(*message, play_mode_, i);
                }
                else
                {
                    notes_down_order.getReference(i) = nullptr;
                }
                delete message;
                return notes_down_order.getUnchecked(i);
            }
        }
    }

    return nullptr;
}
MoniqueSynthesizer::NoteDownStore::MidiMessageCompareable *
MoniqueSynthesizer::NoteDownStore::get_replacement(const juce::MidiMessage &message_,
                                                   int play_mode_, int index_) noexcept
{
    juce::Array<MidiMessageCompareable *> messages;
    if (MidiMessageCompareable *message = notes_down_order.getUnchecked(0))
    {
        messages.add(message);
    }
    if (MidiMessageCompareable *message = notes_down_order.getUnchecked(1))
    {
        messages.add(message);
    }
    if (MidiMessageCompareable *message = notes_down_order.getUnchecked(2))
    {
        messages.add(message);
    }

    int index_of_return = -1;
    if (play_mode_ == PLAY_MODES::LOW)
    {
        int lowest_note = 999;
        for (int i = 0; i != notes_down.size(); ++i)
        {
            const MidiMessageCompareable &message = notes_down.getReference(i);
            if (message.getNoteNumber() < lowest_note)
            {
                bool contains = false;
                for (int k = 0; k != messages.size(); ++k)
                {
                    if (messages.getUnchecked(k)->getNoteNumber() == message.getNoteNumber())
                    {
                        contains = true;
                        break;
                    }
                }
                if (!contains)
                {
                    lowest_note = message.getNoteNumber();
                    index_of_return = i;
                }
            }
        }
    }
    else if (play_mode_ == PLAY_MODES::HIGH)
    {
        int higest_note = -999;
        for (int i = 0; i != notes_down.size(); ++i)
        {
            const MidiMessageCompareable &message = notes_down.getReference(i);
            if (message.getNoteNumber() > higest_note)
            {
                bool contains = false;
                for (int k = 0; k != messages.size(); ++k)
                {
                    if (messages.getUnchecked(k)->getNoteNumber() == message.getNoteNumber())
                    {
                        contains = true;
                        break;
                    }
                }
                if (!contains)
                {
                    higest_note = message.getNoteNumber();
                    index_of_return = i;
                }
            }
        }
    }
    else if (play_mode_ == PLAY_MODES::LIFO)
    {
        for (int i = notes_down.size() - 1; i >= 0; --i)
        {
            bool contains = false;
            for (int k = 0; k != messages.size(); ++k)
            {
                if (notes_down.getReference(i).getNoteNumber() ==
                    messages.getUnchecked(k)->getNoteNumber())
                {
                    contains = true;
                    break;
                }
            }
            if (!contains)
            {
                index_of_return = i;
                break;
            }
        }
    }
    else if (play_mode_ == PLAY_MODES::FIFO)
    {
        for (int i = notes_down.size() - 1; i >= 0; --i)
        {
            bool contains = false;
            for (int k = 0; k != messages.size(); ++k)
            {
                if (notes_down.getReference(i).getNoteNumber() ==
                    messages.getUnchecked(k)->getNoteNumber())
                {
                    contains = true;
                    break;
                }
            }
            if (!contains)
            {
                index_of_return = i;
                break;
            }
        }
    }

    if (index_of_return > -1)
    {
        return new NoteDownStore::MidiMessageCompareable(notes_down.getUnchecked(index_of_return));
    }
    else
    {
        return nullptr;
    }
}
void MoniqueSynthesizer::NoteDownStore::reset() noexcept
{
    notes_down.clearQuick();
    for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
    {
        if (juce::MidiMessage *message = notes_down_order.getUnchecked(i))
        {
            delete message;
            notes_down_order.getReference(i) = nullptr;
        }
    }
}
bool MoniqueSynthesizer::NoteDownStore::is_empty() const noexcept { return notes_down.size() == 0; }
const juce::MidiMessage *MoniqueSynthesizer::NoteDownStore::get_last() const noexcept
{
    if (notes_down.size() > 0)
    {
        return &notes_down.getReference(notes_down.size() - 1);
    }
    else
    {
        return nullptr;
    }
}
const int
MoniqueSynthesizer::NoteDownStore::get_id(const juce::MidiMessage &message_) const noexcept
{
    for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
    {
        if (juce::MidiMessage *message = notes_down_order.getUnchecked(i))
        {
            if (message_.getNoteNumber() == message->getNoteNumber())
            {
                return i;
            }
        }
    }

    return -1;
}
const int MoniqueSynthesizer::NoteDownStore::get_id(int note_number_) const noexcept
{
    for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
    {
        if (juce::MidiMessage *message = notes_down_order.getUnchecked(i))
        {
            if (note_number_ == message->getNoteNumber())
            {
                return i;
            }
        }
    }

    return -1;
}
const juce::MidiMessage *MoniqueSynthesizer::NoteDownStore::get_at(int index_) const noexcept
{
    if (index_ < 0 || index_ >= notes_down_order.size())
    {
        return nullptr;
    }
    else
    {
        return notes_down_order.getUnchecked(index_);
    }
}
void MoniqueSynthesizer::NoteDownStore::swap(int index_a_, int index_b_) noexcept
{
    notes_down_order.swap(index_a_, index_b_);
}
MoniqueSynthesizer::NoteDownStore::NoteDownStore(MoniqueSynthData *const synth_data_) noexcept
    : synth_data(synth_data_)
{
    for (int i = 0; i != MAX_PLAYBACK_NOTES; ++i)
    {
        notes_down_order.add(nullptr);
    }
}
MoniqueSynthesizer::NoteDownStore::~NoteDownStore() noexcept {}

void MoniqueSynthesizer::render_next_block(juce::AudioBuffer<float> &outputAudio,
                                           const juce::MidiBuffer &inputMidi, int startSample,
                                           int numSamples) noexcept
{
    process_next_block(outputAudio, inputMidi, startSample, numSamples);
}
void MoniqueSynthesizer::render_next_block(juce::AudioBuffer<double> &outputAudio,
                                           const juce::MidiBuffer &inputMidi, int startSample,
                                           int numSamples) noexcept
{
    // NOT USED
}
void MoniqueSynthesizer::process_next_block(juce::AudioBuffer<float> &outputAudio,
                                            const juce::MidiBuffer &inputMidi, int startSample,
                                            int numSamples)
{
    juce::MidiBuffer::Iterator midiIterator(inputMidi);
    midiIterator.setNextSamplePosition(startSample);

    int midiEventPos;
    juce::MidiMessage m;

    const juce::ScopedLock sl(lock);

    int program_chnage_counter_temp = synth_data->changed_programm;
    if (program_chnage_counter_temp != program_chnage_counter)
    {
        program_chnage_counter = program_chnage_counter_temp;
        if (synth_data->arp_was_on_before_change)
        {
            // voice->reset_internal();

            voice->eq_processor->start_release();
            voice->fx_processor->start_release(false, false);

            for (int i = 0; i != 3; ++i)
            {
                voice->filter_processors[i]->start_env_release();
                voice->filter_processors[i]->start_input_env_release(0);
                voice->filter_processors[i]->start_input_env_release(1);
                voice->filter_processors[i]->start_input_env_release(2);
#ifdef POLY
                voice->filter_volume_tracking_envs[i]->set_to_release();
#endif
            }
        }
        // synth_data->arp_was_on_before_change = false;
        // int note_before_change = synth_data->current_note_before_change;
        // synth_data->current_note_before_change = -1;
        // if (synth_data->keep_arp_always_on == false && arp_was_on_before_change&&
        // synth_data->arp_sequencer_data->is_on == false)
        //{
        //		voice->reset();
        //}
        // else if (arp_was_on_before_change && note_before_change == -1)
        //{
        //	bool is_any_step_on = false;
        //	for (int i = 0; i != 16; ++i)
        //	{
        //		bool is_on = synth_data->arp_sequencer_data->step[i];
        //		if (is_on)
        //		{
        //			is_any_step_on = true;
        //			break;
        //		}
        //	}
        //	if (is_any_step_on )
        //	{
        //		//voice->reset_internal();
        //	}
        //	//voice->reset();
        //}
        // else
        //{
        //	if (note_down_store.size() == 0 )
        //	{
        //		voice->reset();
        //	}
        //	else
        //	{
        //		bool is_any_step_on = false;
        //		for (int i = 0; i != 16; ++i)
        //		{
        //			bool is_on = synth_data->arp_sequencer_data->step[i];
        //			if (is_on)
        //			{
        //				is_any_step_on = true;
        //				break;
        //			}
        //		}
        //		if (is_any_step_on )
        //		{
        //			voice->reset_internal();
        //		}
        //	}
        //}
    }

    while (numSamples > 0)
    {
        if (!midiIterator.getNextEvent(m, midiEventPos))
        {
            renderVoices(outputAudio, startSample, numSamples);
            return;
        }

        const int samplesToNextMidiMessage = midiEventPos - startSample;

        if (samplesToNextMidiMessage >= numSamples)
        {
            renderVoices(outputAudio, startSample, numSamples);
            handle_midi_event(m, midiEventPos);
            break;
        }

        // FIXME - what was the intent here?
        {
            static bool fix_oss_port_issue = false;
            jassert(fix_oss_port_issue);
            fix_oss_port_issue = true;
        }
        // if (samplesToNextMidiMessage < minimumSubBlockSize)
        {
            handle_midi_event(m, midiEventPos);
            continue;
        }

        renderVoices(outputAudio, startSample, samplesToNextMidiMessage);
        handle_midi_event(m, midiEventPos);
        startSample += samplesToNextMidiMessage;
        numSamples -= samplesToNextMidiMessage;
    }

    while (midiIterator.getNextEvent(m, midiEventPos))
    {
        handle_midi_event(m, midiEventPos);
    }
}

void MoniqueSynthesizer::handle_midi_event(const juce::MidiMessage &m, int pos_in_buffer_)
{
    const int channel = m.getChannel();

    if (m.isNoteOn())
    {
#ifdef POLY
        const int play_mode =
            synth_data->keytrack_osci_play_mode == 2 ? PLAY_MODES::FIFO : PLAY_MODES::LIFO;
#else
        const int play_mode = PLAY_MODES::LIFO;
#endif
        note_down_store.add_note(m, play_mode);
        noteOn(channel, m.getNoteNumber(), m.getFloatVelocity());

        /* RETUNE
            if( note_down_store.size() == 1 )
            {
                noteOn (channel, m.getNoteNumber(), m.getFloatVelocity());
            }
            else if( note_down_store.size() == 2 )
            {
                const bool is_arp_on = synth_data->arp_sequencer_data->is_on ||
           synth_data->keep_arp_always_on; const float arp_offset = is_arp_on ?
           voice->arp_sequencer->get_current_tune() : 0;
                synth_data->osc_datas[1]->tune.set_value((voice->current_note-m.getNoteNumber())*-1);
                const float note = voice->current_note + synth_data->osc_datas[1]->tune
           +arp_offset+voice->pitch_offset; voice->second_osc->update( note, pos_in_buffer_ );
            }
            else if( note_down_store.size() == 3 )
            {
                const bool is_arp_on = synth_data->arp_sequencer_data->is_on ||
           synth_data->keep_arp_always_on; const float arp_offset = is_arp_on ?
           voice->arp_sequencer->get_current_tune() : 0;
                synth_data->osc_datas[2]->tune.set_value((voice->current_note-m.getNoteNumber())*-1);
                const float note = voice->current_note + synth_data->osc_datas[2]->tune
           +arp_offset+voice->pitch_offset; voice->third_osc->update( note, pos_in_buffer_ );
            }
            */
    }
    else if (m.isNoteOff())
    {
        voice->stop_controlled(m, pos_in_buffer_);
    }
    else if (m.isAllNotesOff() || m.isAllSoundOff())
    {
        allNotesOff(channel, true);
    }
    else if (m.isPitchWheel())
    {
        const int wheelPos = m.getPitchWheelValue();
        lastPitchWheelValues[channel - 1] = wheelPos;
        handlePitchWheel(channel, wheelPos);
    }
    else if (m.isAftertouch())
    {
        handleAftertouch(channel, m.getNoteNumber(), m.getAfterTouchValue());
    }
    else if (m.isChannelPressure())
    {
        handleChannelPressure(channel, m.getChannelPressureValue());
    }
    else if (m.isController())
    {
        handleController(channel, m.getControllerNumber(), m.getControllerValue());
    }
    else if (m.isProgramChange())
    {
        handleProgramChange(channel, m.getProgramChangeNumber());
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
COLD MoniqueSynthesiserSound::MoniqueSynthesiserSound() noexcept {}
COLD MoniqueSynthesiserSound::~MoniqueSynthesiserSound() noexcept {}

//==============================================================================
bool MoniqueSynthesiserSound::appliesToNote(int) { return true; }
bool MoniqueSynthesiserSound::appliesToChannel(int) { return true; }

//==============================================================================
void MoniqueSynthData::get_full_adstr(ENVData &env_data_, juce::Array<float> &curve) noexcept
{
    ENV env(runtime_notifyer, this, &env_data_, sine_lookup, cos_lookup, exp_lookup);
    env.start_attack();
    const float sample_rate = runtime_notifyer->get_sample_rate();
    int count_sustain = msToSamplesFast(1000, sample_rate);
    // optimize that like below!
    while (env.get_current_stage() != END_ENV)
    {
        float sample;
        env.process(&sample, 1);
        curve.add(sample);

        if (env.get_current_stage() == SUSTAIN)
        {
            // if( !env.is_endless() )
            {
                if (--count_sustain <= 0)
                {
                    env.set_to_release();
                }
            }
        }
    }
}
void MoniqueSynthData::get_full_mfo(LFOData &mfo_data_, juce::Array<float> &curve,
                                    MoniqueSynthData *data_) noexcept
{
    LFO mfo(runtime_notifyer, this, &mfo_data_, sine_lookup);
    int count_time =
        runtime_notifyer->get_sample_rate() * 2; // msToSamplesFast( 44100, sample_rate );
    int i = 0;
    const int blocksize = runtime_notifyer->get_block_size() / 2;
    float *buffer = new float[blocksize];
    curve.ensureStorageAllocated(count_time + blocksize);

    juce::Array<RuntimeInfo::standalone_features::ClockSync::SyncPosPair> clock_sync_information;
    if (is_standalone())
    {
        clock_sync_information =
            runtime_info->standalone_features_pimpl->clock_sync_information.get_a_working_copy();
    }
    while (i * blocksize < count_time)
    {
        mfo.process(buffer, -1, 1, 1 + i * blocksize, blocksize, false, &clock_sync_information);
        if (i > 10)
        {
            for (int sid = 0; sid != blocksize; ++sid)
            {
                curve.add(buffer[sid]);
            }
        }
        i++;
    }
    delete[] buffer;
}
bool MoniqueSynthData::is_key_down(int id) const noexcept
{
    return reinterpret_cast<MoniqueSynthesizer::NoteDownStore *>(voice->note_down_store)
        ->get_at(id);
}
float MoniqueSynthData::get_tracking_env_state(int id) const noexcept
{
#ifdef POLY
    return voice->filter_volume_tracking_envs[id]->get_amp();
#else
    return 0;
#endif
}

//==============================================================================
