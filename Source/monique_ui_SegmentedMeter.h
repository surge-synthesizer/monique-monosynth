#ifndef __Monique_Ui_SegmentedMeter_H__
#define __Monique_Ui_SegmentedMeter_H__

#include "App_h_includer.h"

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

    const uint32 my_red, my_yellow, my_green;

    bool needsRepaint;

public:
    Monique_Ui_SegmentedMeter();
    ~Monique_Ui_SegmentedMeter();

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

        if (++sampleCount == 1024) // 1024 == release/decay
        {
            if (sampleMax > level)
                level = sampleMax;

            sampleMax = 0;
            sampleCount = 0;
        }
    }
}

#endif //__DROWAUDIO_Monique_Ui_SegmentedMeter_H__
