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

    Colour my_green, my_yellow, my_red, my_bg;

    bool needsRepaint;

public:
    COLD Monique_Ui_SegmentedMeter( Monique_Ui_Refresher*const ui_refresher_ ) noexcept;
    COLD ~Monique_Ui_SegmentedMeter() noexcept;

private:
    void resized() override;
    void paint (Graphics &g) override;
    void moved() override;
    void refresh() noexcept override;

	AffineTransform rotation;

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

/*
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