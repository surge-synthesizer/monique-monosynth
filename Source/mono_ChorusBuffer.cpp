/*
  ==============================================================================

    FractionalDelayBuffer.cpp
    Created: 21 Jan 2014 12:46:20am
    Author:  Marc Wilhite

  ==============================================================================
*/

#include "mono_ChorusBuffer.h"

Chorus::Chorus()
{
    index[0] = 0;
    index[1] = 0;
    last_delay[0] = 220;
    last_delay[1] = 200;

    buffer[0] = nullptr;
    buffer[1] = nullptr;

    sample_rate_changed(0);
}

Chorus::~Chorus()
{
    delete [] buffer[0];
    delete [] buffer[1];
}

void Chorus::sample_rate_changed( double ) noexcept {
    buffer_size = sample_rate/10;
    if( buffer[0] )
        delete[] buffer[0];
    if( buffer[1] )
        delete[] buffer[1];
    buffer[0] = new float[buffer_size];
    buffer[1] = new float[buffer_size];
    for( int i = 0 ; i != buffer_size ; ++i ) {
        buffer[0][i] = 0;
        buffer[1][i] = 0;
    }
};

