#ifndef __SEGMENTEDMETER_H__
#define __SEGMENTEDMETER_H__

#include "App_h_includer.h"

// SIMPLYFYED DROW AUDIO SEGMENT METER
class SegmentedMeter
    :
public Component,
public mono_UiRefreshable
{
    Image onImage, offImage;

    float sampleMax;
    float level;

    int numSegs, last_numSeg;
    int sampleCount;

    const uint32 my_red, my_yellow, my_green;

    bool needsRepaint;

public:
    SegmentedMeter();
    ~SegmentedMeter();

private:
    void resized() override;
    void paint (Graphics &g) override;
    void moved() override;
    void refresh() override;

public:
    inline void process( const float* values, int numSamples ) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SegmentedMeter);
};

inline void SegmentedMeter::process( const float* values, int num_samples_ ) noexcept
{
    for (int sid = 0; sid < num_samples_; ++sid)
    {
        using namespace std;
        float sample = fabsf (values[sid]);

        if (sample > sampleMax)
            sampleMax = sample;

        if (++sampleCount == 1024) // 1024 == release/decay
        {
            if (sampleMax > level)
                level = sampleMax;

            sampleMax = 0;
            sampleCount = 0;
        }
    }
}

#endif //__DROWAUDIO_SEGMENTEDMETER_H__
