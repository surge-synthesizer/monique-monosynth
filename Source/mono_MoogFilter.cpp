
#include "mono_MoogFilter.h"

NOINLINE AnalogFilter::AnalogFilter()
    :
    p(0),k(0),r(0),gain(0),
    y1(0),y2(0),y3(0),y4(0),
    oldx(0),oldy1(0),oldy2(0),oldy3(0),

    cutoff(100), res(0)
{
  sample_rate_changed(0);
}
NOINLINE AnalogFilter::~AnalogFilter() {}

NOINLINE void AnalogFilter::sample_rate_changed(double) noexcept {
    calc_coefficient();
    calc();
};

NOINLINE DoubleAnalogFilter::DoubleAnalogFilter() : was_two_way(false) {}
NOINLINE DoubleAnalogFilter::~DoubleAnalogFilter() {}

// THE REST OF THE DEF YOU'LL FIND IN SYNTH.CPP

/*
inline float AnalogFilter::processBand(float x_) noexcept
{
    // process input
    x_ -= r*y4;

    //Four cascaded onepole filters (bilinear transform)
    y1= x_*p + oldx*p - k*y1;
    y2=y1*p + oldy1*p - k*y2;
    y3=y2*p + oldy2*p - k*y3;
    y4=y3*p + oldy3*p - k*y4;

    //Clipper band limited sigmoid
    y4 -= (y4*y4*y4)/6.0f;

    oldx = x_;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;
    return  (y3 - y4) * 3.0f;
}
*/