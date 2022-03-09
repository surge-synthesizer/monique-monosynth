/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#ifndef __DROWAUDIO_AUDIOUTILITY_H__
#define __DROWAUDIO_AUDIOUTILITY_H__

#if JUCE_MSVC
#pragma warning(disable : 4505)
#endif

#include "dRowAudio_Constants.h"
#include <juce_audio_formats/juce_audio_formats.h>

//==============================================================================
/** This file contains some useful utility functions and macros for audio
    applications.
 */
//==============================================================================
/** Converts an absolute value to decibels.
 */
forcedinline static double toDecibels(double absoluteValue) noexcept
{
    return 20.0 * log10(absoluteValue);
}
forcedinline static float toDecibels_fast(float absoluteValue) noexcept
{
    return 20.0f * log10(absoluteValue);
}

/** Converts a value in decibels to an absolute value.
 */
forcedinline static double decibelsToAbsolute(double decibelsValue) noexcept
{
    return pow(10, (decibelsValue * 0.05));
}

/** Converts a time in seconds to minutes.
 */
forcedinline static double secondsToMins(double seconds) noexcept { return seconds * oneOver60; }

/** Converts a time in seconds to a number of samples for a given sample rate.
 */
forcedinline static juce::int64 secondsToSamples(double timeSeconds, double sampleRate) noexcept
{
    return (juce::int64)(timeSeconds * sampleRate);
}

/** Converts a time in milliseconds to a number of samples for a given sample rate.
 */
forcedinline static juce::int64 msToSamples(double timeMs, double sampleRate) noexcept
{
    return (juce::int64)(timeMs * 0.001 * sampleRate);
}
forcedinline static juce::int64 msToSamplesFast64(float timeMs, float sampleRate) noexcept
{
    return (juce::int64)(timeMs * 0.001f * sampleRate);
}
forcedinline static int msToSamplesFast(float timeMs, float sampleRate) noexcept
{
    return timeMs * 0.001f * sampleRate;
}

/** Converts a number of samples to a time in ms for a given sample rate.
 */
forcedinline static double samplesToMs(juce::int64 numSamples, double sampleRate) noexcept
{
    return (1000.0 * (numSamples / sampleRate));
}
forcedinline static float samplesToMsFast(juce::int64 numSamples, float sampleRate) noexcept
{
    return (1000.0f * (float(numSamples) / sampleRate));
}

/** Converts a number of samples to a time in seconds for a given sample rate.
 */
forcedinline static double samplesToSeconds(juce::int64 numSamples, double sampleRate) noexcept
{
    return (numSamples / sampleRate);
}

/** Converts a number of semitones to a given pitch ratio.
 */
static inline double semitonesToPitchRatio(double numSemitones) noexcept
{
    // return pow (10.0, numSemitones * (log10 (2.0) / 12.0));
    return pow(2.0, numSemitones / 12.0);
}

/** Converts pitch ratio to a number of semitones.
 */
static inline double pitchRatioToSemitones(double pitchRatio) noexcept
{
    // return (12.0 / log10 (2.0)) * log10 (pitchRatio);
    return 12.0 * log2(pitchRatio);
}

/** Converts a frequency to MIDI note number.
 */
static inline double frequencyToMidi(double frequency) noexcept
{
    return 69.0 + (12.0 * log2(frequency / 440.0));
}

/** Converts a MIDI note number to a frequency.
 */
static inline double midiToFrequency(double midiNoteNumber) noexcept
{
    return 440.0 * pow(2.0, (midiNoteNumber - 69.0) / 12.0);
}
static inline float midiToFrequencyFast(float midiNoteNumber) noexcept
{
    return 440.0f * pow(2.0f, ((midiNoteNumber - 69.0f) * (1.0f / 12)));
}

/** Converts a time in seconds to a timecode string.
 */
static inline const juce::String timeToTimecodeString(const double seconds) noexcept
{
    const double absSecs = fabs(seconds);
    const juce::String sign((seconds < 0) ? "-" : "");

    const int hours = (int)(absSecs / (60.0 * 60.0));
    const int mins = ((int)(absSecs / 60.0)) % 60;
    const int secs = ((int)absSecs) % 60;

    juce::String t(sign);

    t << juce::String(hours).paddedLeft('0', 2) << ":" << juce::String(mins).paddedLeft('0', 2)
      << ":" << juce::String(secs).paddedLeft('0', 2) << ":"
      << juce::String(juce::roundToInt(absSecs * 1000) % 1000).paddedLeft('0', 2);
    return t;
}

/** Converts a time in seconds to a timecode string displaying mins, secs and 1/10th secs.
 */
static inline const juce::String timeToTimecodeStringLowRes(const double seconds) noexcept
{
    const double absSecs = fabs(seconds);
    const juce::String sign((seconds < 0) ? "-" : "");

    //    const int hours = (int) (absSecs * oneOver60Squared);
    const int mins = ((juce::uint32)(absSecs * oneOver60)) % 60u;
    const int secs = ((juce::uint32)absSecs) % 60u;
    const int tenthSecs = (int)((absSecs - (int)absSecs) * 10);

    juce::String t(sign);

    t << juce::String(mins).paddedLeft('0', 2) << ":" << juce::String(secs).paddedLeft('0', 2)
      << "." << juce::String(tenthSecs).paddedLeft('0', 0);
    return t;
}

/**	Converts a number of seconds to a time string.
        This is useful when displaying times as hrs, mins secs etc.
        as it will only display the units needed.
 */
static inline const juce::String secondsToTimeLength(double numSeconds) noexcept
{
    double decimalTime = numSeconds / 60000.0;

    int hrs = 0;
    int mins = (int)decimalTime;
    int secs = juce::roundToInt((decimalTime - mins) * 60.0);

    juce::String timeString;

    if (mins > 59)
    {
        hrs = mins / 60;
        mins -= hrs * 60;

        timeString << juce::String(hrs) << ":" << juce::String(mins).paddedLeft('0', 2) << ":";
    }
    else
        timeString << juce::String(mins) << ":";

    timeString << juce::String(secs).paddedLeft('0', 2);

    return timeString;
}

/** Formats a CurretPositionInfo to a bars/beats string.
 */
static inline const juce::String ppqToBarsBeatsString(const double ppq, const double /*lastBarPPQ*/,
                                                      const int numerator,
                                                      const int denominator) noexcept
{
    if (numerator == 0 || denominator == 0)
        return "1|1|0";

    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats = (fmod(ppq, ppqPerBar) / ppqPerBar) * numerator;

    const int bar = ((int)ppq) / ppqPerBar + 1;
    const int beat = ((int)beats) + 1;
    const int ticks = ((int)(fmod(beats, 1.0) * 960.0));

    juce::String s;
    s << bar << '|' << beat << '|' << ticks;
    return s;
}

/** Compares a filename extension with a wildcard string.
 */
static inline bool matchesAudioWildcard(const juce::String &extensionToTest,
                                        const juce::String &wildcard,
                                        const bool ignoreCase = true) noexcept
{
    if (ignoreCase ? wildcard.containsIgnoreCase(extensionToTest)
                   : wildcard.contains(extensionToTest))
        return true;
    else
        return false;
}

// Deprecated method, doesn't seem to be used so commented out.

/** Converts a block of audio sample to floating point samples if the reader
    used an integer format.
 */

// static inline void convertToFloat(juce::AudioFormatReader *reader, void *sourceBuffer,
//                                   float *destBuffer, int numSamples) noexcept
// {
//     if (reader != nullptr)
//     {
//         if (!reader->usesFloatingPointData)
//         {
// #if JUCE_BIG_ENDIAN
//             AudioDataConverters::convertInt32BEToFloat((void *)sourceBuffer, destBuffer,
//             numSamples,
//                                                        sizeof(int));
// #else
//             juce::AudioDataConverters::convertInt32LEToFloat((void *)sourceBuffer, destBuffer,
//                                                              numSamples, sizeof(int));
// #endif
//         }
//         else
//         {
//             memcpy(destBuffer, sourceBuffer, sizeof(float) * numSamples);
//         }
//     }
// }

//==============================================================================
/** Returns the number bytes needed to store an AudioSampleBuffer with its
    channel header and sample data.

    This can be used to find out how many bytes to pass to isAudioSampleBuffer().
 */
static inline size_t getNumBytesForAudioSampleBuffer(const juce::AudioSampleBuffer &buffer) noexcept
{
    const size_t channelListSize = (buffer.getNumChannels() + 1) * sizeof(float *);
    const size_t sampleDataSize = buffer.getNumSamples() * buffer.getNumChannels() * sizeof(float);

    return channelListSize + sampleDataSize;
}

/** Parses a block of memory to see if it represents an AudioSampleBuffer.

    This uses the memory layout of an AudioSampleBuffer to make an eductated
    guess at whether the memory represents an AudioSampleBuffer. For speed
    this will only check for buffers up to maxNumChannels channels.

    @param sourceData       The start of the likely AudioSampleBuffer to be
                            tested e.g. AudioSampleBuffer::getArrayOfChannels()
    @param sourceDataSize   The number of bytes of the sourceData
    @param maxNumChannels   An optional maximum number of channels to search
    @returns                true if the sourceData likey represents an AudioSampleBuffer

    @see AudioSampleBufferAudioFormat, getNumBytesForAudioSampleBuffer, AudioSampleBuffer
 */
static inline bool isAudioSampleBuffer(void *sourceData, size_t sourceDataSize,
                                       int maxNumChannels = 128) noexcept
{
    const float **channelList = reinterpret_cast<const float **>(sourceData);

    // get channel list pointers
    juce::Array<const float *> channelPointers;
    for (int i = 0; i < maxNumChannels; i++)
    {
        const float *channelPointer = channelList[i];

        if (channelPointer == nullptr) // found end of channel list
            break;

        channelPointers.add(channelPointer);
    }

    if (channelPointers.size() == 0)
        return false;

    const size_t channelListSize = (channelPointers.size() + 1) * sizeof(float *);
    const size_t expectedNumSamples =
        (sourceDataSize - channelListSize) / (channelPointers.size() * sizeof(float));
    const size_t bytesPerChannel = expectedNumSamples * sizeof(float);

    const float *startOfChannels =
        reinterpret_cast<float *>(juce::addBytesToPointer(sourceData, channelListSize));

    // compare to sample data pointers
    for (int i = 0; i < channelPointers.size(); i++)
    {
        const float *channelPointer =
            juce::addBytesToPointer(startOfChannels, (i * bytesPerChannel));
        if (channelPointer != channelPointers[i])
            return false;
    }

    return true;
}

/** Parses an InputStream to see if it represents an AudioSampleBuffer.

    This uses the memory layout of an AudioSampleBuffer to make an eductated
    guess at whether the stream represents an AudioSampleBuffer. For speed
    this will only check for buffers up to maxNumChannels channels.

    @param inputStream      The stream to check for AudioSampleBuffer contents
    @param maxNumChannels   An optional maximum number of channels to search
    @returns                true if the stream likey represents an AudioSampleBuffer

    @see AudioSampleBufferAudioFormat, getNumBytesForAudioSampleBuffer, AudioSampleBuffer
 */
static inline bool isAudioSampleBuffer(juce::InputStream &inputStream, unsigned int &numChannels,
                                       juce::int64 &numSamples, int maxNumChannels = 128) noexcept
{
    // get start samples
    juce::Array<float> channelStartSamples;
    for (int i = 0; i < maxNumChannels; i++)
    {
        float *channelPointer;
        inputStream.read(&channelPointer, sizeof(float *));

        if (channelPointer == nullptr) // found end of channel list
            break;

        channelStartSamples.add(*channelPointer);
    }

    if (channelStartSamples.size() == 0)
        return false;

    const size_t channelListSize = (channelStartSamples.size() + 1) * sizeof(float *);
    const juce::int64 expectedNumSamples = (inputStream.getTotalLength() - channelListSize) /
                                           (channelStartSamples.size() * sizeof(float));
    const juce::int64 bytesPerChannel = expectedNumSamples * sizeof(float);

    // compare sample values
    for (int i = 0; i < channelStartSamples.size(); i++)
    {
        float sample;
        inputStream.setPosition(channelListSize + (i * bytesPerChannel));
        inputStream.read(&sample, sizeof(float));

        if (sample != channelStartSamples[i])
            return false;
    }

    // slower but possibly more reliable method
    //    for (int i = 0; i < channelStartSamples.size(); i++)
    //    {
    //        float sample;
    //        inputStream.read (&sample, sizeof (float));
    //        inputStream.skipNextBytes (bytesPerChannel);
    //
    //        if (sample != channelStartSamples[i])
    //            return false;
    //    }

    numChannels = channelStartSamples.size();
    numSamples = expectedNumSamples;

    return true;
}

#endif //__DROWAUDIO_AUDIOUTILITY_H__
