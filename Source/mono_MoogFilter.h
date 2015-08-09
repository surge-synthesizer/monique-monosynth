/*
  ==============================================================================

    mono_MoogFilter.h
    Author:  monotomy

  ==============================================================================
*/

#ifndef MOOG_FILTER_H_INCLUDED
#define MOOG_FILTER_H_INCLUDED

#include "App.h"
#include "SynthData.h"

//==============================================================================
//==============================================================================
//==============================================================================
#define HARD_CLIPPER_STAGE_1_V2 1.1f
#define HARD_CLIPPER_STAGE_2_V2 1.25f
static inline float protection_clipping_v2( float input_and_worker_ ) noexcept
{
    if( input_and_worker_ > HARD_CLIPPER_STAGE_1_V2 )
    {
        input_and_worker_ = HARD_CLIPPER_STAGE_1_V2 + input_and_worker_*0.1f;
        if( input_and_worker_ > HARD_CLIPPER_STAGE_2_V2 )
        {
            input_and_worker_ = HARD_CLIPPER_STAGE_2_V2 + input_and_worker_*0.05f;
            if( input_and_worker_ > HARD_CLIPPER_STAGE_2_V2 )
            {
                input_and_worker_ = HARD_CLIPPER_STAGE_2_V2;
            }
        }
    }
    else if( input_and_worker_ < -HARD_CLIPPER_STAGE_1_V2 )
    {
        input_and_worker_ = -HARD_CLIPPER_STAGE_1_V2 + input_and_worker_*0.1f;
        if( input_and_worker_ < -HARD_CLIPPER_STAGE_2_V2 )
        {
            input_and_worker_ = -HARD_CLIPPER_STAGE_2_V2 + input_and_worker_*0.05f;
            if( input_and_worker_ < -HARD_CLIPPER_STAGE_2_V2 )
            {
                input_and_worker_ = -HARD_CLIPPER_STAGE_2_V2;
            }
        }
    }

    return input_and_worker_;
}
class AnalogFilter : public RuntimeListener
{
    friend class DoubleAnalogFilter;
    float p, k, r, gain;
    float y1,y2,y3,y4;
    float oldx;
    float oldy1,oldy2,oldy3;

    float cutoff, res;

    // NEW
public:
    // OLD OR DEFAULT ONE
    inline void set(float r,float c, float gain_) noexcept;
    inline void update(float resonance_,
                       float cutoff_,
                       float gain_ ) noexcept;
    inline void copy_coefficient_from( const AnalogFilter& other_ ) noexcept;
    inline void copy_state_from( const AnalogFilter& other_ ) noexcept;

    inline float processLow(float input_and_worker_) noexcept;

    inline float processLowResonance(float input_and_worker_) noexcept;
    inline float processHighResonance(float input_and_worker_) noexcept;

    inline void reset() noexcept;

private:
    inline void calc_coefficient() noexcept;
    inline void calc() noexcept;
    
    NOINLINE void sample_rate_changed( double ) noexcept override;

public:
    NOINLINE AnalogFilter();
    NOINLINE ~AnalogFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFilter)
};
//==============================================================================
inline void AnalogFilter::calc_coefficient() noexcept
{
    float f = (cutoff+cutoff) / sample_rate;
    float agressive = 0.48f*gain;
    p=f*((1.5f+agressive)-((0.5f+agressive)*f));
    k=p*2-1;
}
inline void AnalogFilter::calc() noexcept
{
    {
        float f = (cutoff+cutoff) / sample_rate;
        float agressive = 0.48f*gain;
        p=f*((1.5f+agressive)-((0.5f+agressive)*f));
        k=p*2-1;
    }
    {
        float t=(1.0f-p)*1.386249f;
        float t2=12.0f+t*t;
        r = res*(t2+6.0f*t)/(t2-6.0f*t);
    }
}
inline void AnalogFilter::copy_coefficient_from( const AnalogFilter& other_ ) noexcept {
    cutoff = other_.cutoff;
    res = other_.res;

    p = other_.p;
    k = other_.k;
    r = other_.r;
    gain = other_.gain;
}
inline void AnalogFilter::copy_state_from( const AnalogFilter& other_ ) noexcept {
    oldx = other_.oldx;
    oldy1 = other_.oldy1;
    oldy2 = other_.oldy2;
    oldy3 = other_.oldy3;
    y4 = other_.y4;
}
static inline float distortion2( float sample_, float distortion_power_ )
{
    float distortion_add_on = 0.9f*distortion_power_;
    return (1.0f+distortion_add_on)*sample_ - (0.0f+distortion_add_on)*sample_*sample_*sample_;
}
inline float AnalogFilter::processLow(float input_and_worker_) noexcept
{
    input_and_worker_ = protection_clipping_v2(input_and_worker_);

    // process input
    input_and_worker_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= input_and_worker_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4)/6.0f;

    oldx = input_and_worker_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    return (y4 + distortion2(y3*res+y4*(1.0f-res),gain)*gain)/(1.0f+gain);
}
inline void AnalogFilter::reset() noexcept {
    y4 = oldx = oldy1 = oldy2 = oldy3 = 0;
}
//==============================================================================
//==============================================================================
//==============================================================================
class DoubleAnalogFilter
{
    AnalogFilter lpf_1;
    AnalogFilter lpf_2;

    bool was_two_way;

public:
    // ----------------------------------------------------------
    // DEFAULT LP
    inline void setLow(float resonance_,
                       float cutoff_,
                       float gain_) noexcept;

    // ----------------------------------------------------------
    // MONSTER RESONANCE LP
    inline void updateLowResonance(float resonance_,
                                   float cutoff_,
                                   float gain_) noexcept;
    inline float processLowResonance(float input_and_worker_) noexcept;

    // ----------------------------------------------------------
    // 2PASS LP
    inline void updateLow2Pass(float resonance_,
                               float cutoff_,
                               float gain_) noexcept;
    inline float processLow2Pass(float input_and_worker_) noexcept;

    // ----------------------------------------------------------
    // MONSTER RESONANCE HP
    inline void updateHighResonance(float resonance_,
                                    float cutoff_,
                                    float gain_) noexcept;
    inline float processHighResonance(float input_and_worker_) noexcept;

    // ----------------------------------------------------------
    // 2PASS HP
    inline void updateHigh2Pass(float resonance_,
                                float cutoff_,
                                float gain_) noexcept;
    inline float processHigh2Pass(float input_and_worker_) noexcept;

    // ----------------------------------------------------------
    // BAND
    inline void updateBand(float resonance_,
                           float cutoff_,
                           float gain_) noexcept;
    inline float processBand(float input_and_worker_) noexcept;

    //
    inline void reset() noexcept;

    NOINLINE DoubleAnalogFilter();
    NOINLINE ~DoubleAnalogFilter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleAnalogFilter)
};
//==============================================================================
inline void AnalogFilter::set(float r, float c, float gain_) noexcept
{
    if( r != res || c != cutoff || gain != gain_ )
    {
        gain = gain_;
        res = r;
        cutoff = c;
        calc();
    }
}
inline void DoubleAnalogFilter::setLow(float resonance_, float cutoff_, float gain_) noexcept
{
    lpf_1.set( resonance_*0.5f, cutoff_, gain_ );
}

inline void DoubleAnalogFilter::reset() noexcept {
    lpf_1.reset();
    lpf_2.reset();
}
#endif
