/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_IIRFILTER_H_INCLUDED
#define JUCE_IIRFILTER_H_INCLUDED

class IIRFilter;

//==============================================================================
/**
    A set of coefficients for use in an IIRFilter object.

    @see IIRFilter
*/
class JUCE_API  IIRCoefficients
{
public:
    //==============================================================================
    /** The raw coefficients.
        You should leave these numbers alone unless you really know what you're doing.
    */
    float coefficients[5];


    //==============================================================================
    /** Creates a null set of coefficients (which will produce silence). */
    IIRCoefficients() noexcept;

    /** Directly constructs an object from the raw coefficients.
        Most people will want to use the static methods instead of this, but
        the constructor is public to allow tinkerers to create their own custom
        filters!
    */
    IIRCoefficients (double c1, double c2, double c3,
                     double c4, double c5, double c6) noexcept;

    /** Creates a copy of another filter. */
    IIRCoefficients (const IIRCoefficients&) noexcept;
    /** Creates a copy of another filter. */
    IIRCoefficients& operator= (const IIRCoefficients&) noexcept;
    /** Destructor. */
    ~IIRCoefficients() noexcept;

    /** Returns the coefficients for a low-pass filter. */
    static IIRCoefficients makeLowPass (double sampleRate,
                                        double frequency) noexcept;

    /** Returns the coefficients for a high-pass filter. */
    static IIRCoefficients makeHighPass (double sampleRate,
                                         double frequency) noexcept;

    //==============================================================================
    /** Returns the coefficients for a low-pass shelf filter with variable Q and gain.

        The gain is a scale factor that the low frequencies are multiplied by, so values
        greater than 1.0 will boost the low frequencies, values less than 1.0 will
        attenuate them.
    */
    static IIRCoefficients makeLowShelf (double sampleRate,
                                         double cutOffFrequency,
                                         double Q,
                                         float gainFactor) noexcept;

    /** Returns the coefficients for a high-pass shelf filter with variable Q and gain.

        The gain is a scale factor that the high frequencies are multiplied by, so values
        greater than 1.0 will boost the high frequencies, values less than 1.0 will
        attenuate them.
    */
    static IIRCoefficients makeHighShelf (double sampleRate,
                                          double cutOffFrequency,
                                          double Q,
                                          float gainFactor) noexcept;

    /** Returns the coefficients for a peak filter centred around a
        given frequency, with a variable Q and gain.

        The gain is a scale factor that the centre frequencies are multiplied by, so
        values greater than 1.0 will boost the centre frequencies, values less than
        1.0 will attenuate them.
    */
    static IIRCoefficients makePeakFilter (double sampleRate,
                                           double centreFrequency,
                                           double Q,
                                           float gainFactor) noexcept;
};

inline IIRCoefficients::IIRCoefficients() noexcept
{
    zeromem (coefficients, sizeof (coefficients));
}

inline IIRCoefficients::~IIRCoefficients() noexcept {}

inline IIRCoefficients::IIRCoefficients (const IIRCoefficients& other) noexcept
{
    memcpy (coefficients, other.coefficients, sizeof (coefficients));
}

inline IIRCoefficients& IIRCoefficients::operator= (const IIRCoefficients& other) noexcept
{
    memcpy (coefficients, other.coefficients, sizeof (coefficients));
    return *this;
}

inline IIRCoefficients::IIRCoefficients (double c1, double c2, double c3,
        double c4, double c5, double c6) noexcept
{
    const double a = 1.0 / c4;

    coefficients[0] = (float) (c1 * a);
    coefficients[1] = (float) (c2 * a);
    coefficients[2] = (float) (c3 * a);
    coefficients[3] = (float) (c5 * a);
    coefficients[4] = (float) (c6 * a);
}

inline IIRCoefficients IIRCoefficients::makeLowPass (const double sampleRate,
        const double frequency) noexcept
{
    jassert (sampleRate > 0);

    const double n = 1.0 / std::tan (double_Pi * frequency / sampleRate);
    const double nSquared = n * n;
    const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);

    return IIRCoefficients (c1,
    c1 * 2.0,
    c1,
    1.0,
    c1 * 2.0 * (1.0 - nSquared),
    c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

inline IIRCoefficients IIRCoefficients::makeHighPass (const double sampleRate,
        const double frequency) noexcept
{
    const double n = std::tan (double_Pi * frequency / sampleRate);
    const double nSquared = n * n;
    const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);

    return IIRCoefficients (c1,
    c1 * -2.0,
    c1,
    1.0,
    c1 * 2.0 * (nSquared - 1.0),
    c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

inline IIRCoefficients IIRCoefficients::makeLowShelf (const double sampleRate,
        const double cutOffFrequency,
        const double Q,
        const float gainFactor) noexcept
{
    jassert (sampleRate > 0);
    jassert (Q > 0);

    const double A = jmax (0.0f, std::sqrt (gainFactor));
    const double aminus1 = A - 1.0;
    const double aplus1 = A + 1.0;
    const double omega = (double_Pi * 2.0 * jmax (cutOffFrequency, 2.0)) / sampleRate;
    const double coso = std::cos (omega);
    const double beta = std::sin (omega) * std::sqrt (A) / Q;
    const double aminus1TimesCoso = aminus1 * coso;

    return IIRCoefficients (A * (aplus1 - aminus1TimesCoso + beta),
    A * 2.0 * (aminus1 - aplus1 * coso),
    A * (aplus1 - aminus1TimesCoso - beta),
    aplus1 + aminus1TimesCoso + beta,
    -2.0 * (aminus1 + aplus1 * coso),
    aplus1 + aminus1TimesCoso - beta);
}

inline IIRCoefficients IIRCoefficients::makeHighShelf (const double sampleRate,
        const double cutOffFrequency,
        const double Q,
        const float gainFactor) noexcept
{
    jassert (sampleRate > 0);
    jassert (Q > 0);

    const double A = jmax (0.0f, std::sqrt (gainFactor));
    const double aminus1 = A - 1.0;
    const double aplus1 = A + 1.0;
    const double omega = (double_Pi * 2.0 * jmax (cutOffFrequency, 2.0)) / sampleRate;
    const double coso = std::cos (omega);
    const double beta = std::sin (omega) * std::sqrt (A) / Q;
    const double aminus1TimesCoso = aminus1 * coso;

    return IIRCoefficients (A * (aplus1 + aminus1TimesCoso + beta),
    A * -2.0 * (aminus1 + aplus1 * coso),
    A * (aplus1 + aminus1TimesCoso - beta),
    aplus1 - aminus1TimesCoso + beta,
    2.0 * (aminus1 - aplus1 * coso),
    aplus1 - aminus1TimesCoso - beta);
}

inline IIRCoefficients IIRCoefficients::makePeakFilter (const double sampleRate,
        const double centreFrequency,
        const double Q,
        const float gainFactor) noexcept
{
    jassert (sampleRate > 0);
    jassert (Q > 0);

    const double A = jmax (0.0f, std::sqrt (gainFactor));
    const double omega = (double_Pi * 2.0 * jmax (centreFrequency, 2.0)) / sampleRate;
    const double alpha = 0.5 * std::sin (omega) / Q;
    const double c2 = -2.0 * std::cos (omega);
    const double alphaTimesA = alpha * A;
    const double alphaOverA = alpha / A;

    return IIRCoefficients (1.0 + alphaTimesA,
    c2,
    1.0 - alphaTimesA,
    1.0 + alphaOverA,
    c2,
    1.0 - alphaOverA);
}

#if JUCE_INTEL
#define JUCE_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8f || n > 1.0e-8f)) n = 0;
#else
#define JUCE_SNAP_TO_ZERO(n)
#endif

//==============================================================================
/**
    An IIR filter that can perform low, high, or band-pass filtering on an
    audio signal.

    @see IIRCoefficient, IIRFilterAudioSource
*/
class JUCE_API  IIRFilter
{
    IIRCoefficients coefficients;
    float v1, v2;
public:
    /** Processes a single sample, without any locking or checking.

    Use this if you need fast processing of a single value, but be aware that
    this isn't thread-safe in the way that processSamples() is.
    */
    float processSingleSampleRaw ( float in) noexcept;

    /** Applies a set of coefficients to this filter. */
    void setCoefficients (const IIRCoefficients& newCoefficients) noexcept;

    //==============================================================================
    /** Creates a filter.

        Initially the filter is inactive, so will have no effect on samples that
        you process with it. Use the setCoefficients() method to turn it into the
        type of filter needed.
    */
    IIRFilter() noexcept;

    /** Creates a copy of another filter. */
    IIRFilter (const IIRFilter&) noexcept;

    /** Destructor. */
    ~IIRFilter() noexcept;

    //==============================================================================
    /** Clears the filter so that any incoming data passes through unchanged. */
    void makeInactive() noexcept;


    /** Returns the coefficients that this filter is using. */
    IIRCoefficients getCoefficients() const noexcept    {
        return coefficients;
    }
    IIRCoefficients& getCoefficients() noexcept { return coefficients; }
    //==============================================================================
    /** Resets the filter's processing pipeline, ready to start a new stream of data.

        Note that this clears the processing state, but the type of filter and
        its coefficients aren't changed. To put a filter into an inactive state, use
        the makeInactive() method.
    */
    void reset() noexcept;

    /** Performs the filter operation on the given set of samples. */
    void processSamples (float* samples, int numSamples) noexcept;
    void processSamples (float* const dest_, const float* const src_, const int numSamples) noexcept;

protected:
    //==============================================================================
    SpinLock processLock;
    bool active;

    IIRFilter& operator= (const IIRFilter&);
    JUCE_LEAK_DETECTOR (IIRFilter)
};

inline void IIRFilter::setCoefficients (const IIRCoefficients& newCoefficients) noexcept
{
    // HACK DISABLED
    //const SpinLock::ScopedLockType sl (processLock);

    coefficients = newCoefficients;
    // active = true;
}

inline float IIRFilter::processSingleSampleRaw ( float in) noexcept
{
    // HACK DISABLED
    /*
    if(!active)
      return in;
    */

    float out = coefficients.coefficients[0] * in + v1;

    JUCE_SNAP_TO_ZERO (out);

    v1 = coefficients.coefficients[1] * in - coefficients.coefficients[3] * out + v2;
    v2 = coefficients.coefficients[2] * in - coefficients.coefficients[4] * out;

    return out;
}

inline void IIRFilter::processSamples (float* const samples, const int numSamples) noexcept
{
    // const SpinLock::ScopedLockType sl (processLock);

    // HACK DISABLED
    //if (active)
    {
        const float c0 = coefficients.coefficients[0];
        const float c1 = coefficients.coefficients[1];
        const float c2 = coefficients.coefficients[2];
        const float c3 = coefficients.coefficients[3];
        const float c4 = coefficients.coefficients[4];
        float lv1 = v1, lv2 = v2;

        for (int i = 0; i < numSamples; ++i)
        {
            const float in = samples[i];
            const float out = c0 * in + lv1;
            samples[i] = out;

            lv1 = c1 * in - c3 * out + lv2;
            lv2 = c2 * in - c4 * out;
        }

        JUCE_SNAP_TO_ZERO (lv1);
        v1 = lv1;
        JUCE_SNAP_TO_ZERO (lv2);
        v2 = lv2;
    }
}
inline void IIRFilter::processSamples (float* const dest_, const float* const src_, const int numSamples) noexcept
{
    // HACK DISABLED
    //if (active)
    {
        const float c0 = coefficients.coefficients[0];
        const float c1 = coefficients.coefficients[1];
        const float c2 = coefficients.coefficients[2];
        const float c3 = coefficients.coefficients[3];
        const float c4 = coefficients.coefficients[4];
        float lv1 = v1, lv2 = v2;

        for (int i = 0; i < numSamples; ++i)
        {
            const float in = src_[i];
            const float out = c0 * in + lv1;
            dest_[i] = out;

            lv1 = c1 * in - c3 * out + lv2;
            lv2 = c2 * in - c4 * out;
        }

        JUCE_SNAP_TO_ZERO (lv1);
        v1 = lv1;
        JUCE_SNAP_TO_ZERO (lv2);
        v2 = lv2;
    }
}

#endif   // JUCE_IIRFILTER_H_INCLUDED
