/*
  ==============================================================================

    FractionalDelayBuffer.h
    Created: 21 Jan 2014 12:46:20am
    Author:  Marc Wilhite

  ==============================================================================
*/

#ifndef CHORUS_H_INCLUDED
#define CHORUS_H_INCLUDED

#include "App_h_includer.h"
#include "SynthData.h"
#define DELAY_GLIDE 0.01f

class Chorus : public RuntimeListener
{
    int buffer_size;
    int index[2];
    float last_delay[2];
    float* buffer[2];

public:
    inline void fill(int channel_, float sample_) noexcept;
    inline float tick( int channel_, float delay_ ) noexcept;

private:
    void sample_rate_changed( double ) noexcept override;

public:
    NOINLINE Chorus();
    NOINLINE ~Chorus();
};
inline void Chorus::fill(int channel_, float sample_) noexcept
{
    index[channel_] = index[channel_] % buffer_size;
    buffer[channel_][index[channel_]] = sample_;
    index[channel_]++;
}
inline float Chorus::tick( int channel_, float delay_ ) noexcept
{
    if( delay_ < last_delay[channel_] - DELAY_GLIDE)
        delay_ = last_delay[channel_] - DELAY_GLIDE;
    else if( delay_ > last_delay[channel_] + DELAY_GLIDE )
        delay_ = last_delay[channel_] + DELAY_GLIDE;

    last_delay[channel_] = delay_;

    float i = float(index[channel_]) - delay_;
    if(i >= buffer_size)
        i -= buffer_size;
    else if(i < 0)
        i += buffer_size;

    int ia = std::floor(i);
    if (ia >= buffer_size)
        ia = 0;
    int ib = ia + 1;
    if (ib >= buffer_size)
        ib = 0;

    float delta = i-ia;
    if( delta > 1 )
        delta = 0;
    return buffer[channel_][ib]*delta + buffer[channel_][ia]*(1.0f-delta);
}

#endif  // FRACTIONALDELAYBUFFER_H_INCLUDED





