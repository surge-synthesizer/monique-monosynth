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

#ifndef __Monique_Ui_SegmentedMeter_H__
#define __Monique_Ui_SegmentedMeter_H__

#include "App.h"

// SIMPLYFYED DROW AUDIO SEGMENT METER
class Monique_Ui_SegmentedMeter : public juce::Component, public Monique_Ui_Refreshable
{
    juce::Image onImage, offImage;

    float sampleMax;
    float level;

    int numSegs, last_numSeg;
    int sampleCount;

    juce::Colour my_green, my_yellow, my_red, my_bg;

    bool needsRepaint;

  public:
    COLD Monique_Ui_SegmentedMeter(Monique_Ui_Refresher *const ui_refresher_) noexcept;
    COLD ~Monique_Ui_SegmentedMeter() noexcept;

  private:
    void resized() override;
    void paint(juce::Graphics &g) override;
    void moved() override;
    void refresh() noexcept override;

  public:
    inline void process(const float *values, int numSamples) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_SegmentedMeter)
};

inline void Monique_Ui_SegmentedMeter::process(const float *values, int num_samples_) noexcept
{
    for (int sid = 0; sid < num_samples_; ++sid)
    {
        using namespace std;
        float sample = fabsf(values[sid]);

        if (sample > sampleMax)
            sampleMax = sample;

        if (++sampleCount == 2048) // 1024 == release/decay
        {
            if (sampleMax > level)
                level = sampleMax;

            sampleMax = 0;
            sampleCount = 0;
        }
    }
}

#endif //__DROWAUDIO_Monique_Ui_SegmentedMeter_H__

/*
#ifndef __Monique_Ui_SegmentedMeter_H__
#define __Monique_Ui_SegmentedMeter_H__

#include "App.h"

// SIMPLYFYED DROW AUDIO SEGMENT METER
class Monique_Ui_SegmentedMeter
    :
public Component,
public Monique_Ui_Refreshable
{
    Image onImage, offImage;

    float sampleMax;
    float level;

    int numSegs, last_numSeg;
    int sampleCount;

    const Colour& my_yellow_ref, my_green_ref, my_bg_ref;
    Colour my_red, my_yellow, my_green, my_bg;

    bool needsRepaint;

public:
    COLD Monique_Ui_SegmentedMeter( Monique_Ui_Refresher*const ui_refresher_ ) noexcept;
    COLD ~Monique_Ui_SegmentedMeter() noexcept;

private:
    void resized() override;
    void paint (Graphics &g) override;
    void moved() override;
    void refresh() noexcept override;

public:
    inline void process( const float* values, int numSamples ) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_SegmentedMeter);
};

inline void Monique_Ui_SegmentedMeter::process( const float* values, int num_samples_ ) noexcept
{
    for (int sid = 0; sid < num_samples_; ++sid)
    {
        using namespace std;
        float sample = fabsf (values[sid]);

        if (sample > sampleMax)
            sampleMax = sample;

        if (++sampleCount == 2048) // 1024 == release/decay
        {
            if (sampleMax > level)
                level = sampleMax;

            sampleMax = 0;
            sampleCount = 0;
        }
    }
}

#endif //__DROWAUDIO_Monique_Ui_SegmentedMeter_H__

*/