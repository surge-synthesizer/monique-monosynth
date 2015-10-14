#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_LookAndFeel.h"

#define NUM_RED_SEG 2
#define NUM_YELLOW_SEG 3
#define NUM_GREEN_SEG 4
#define TOTAL_NUM_SEG (NUM_RED_SEG+NUM_YELLOW_SEG+NUM_GREEN_SEG)

#define DB_PER_SEC 3.0f
#define SAMPLES_TO_COUNT 2048

COLD Monique_Ui_SegmentedMeter::Monique_Ui_SegmentedMeter( Monique_Ui_Refresher*const ui_refresher_ ) noexcept
:
Monique_Ui_Refreshable(ui_refresher_),
                       sampleMax     (0.0f),
                       level         (0.0f),

                       numSegs       (0),
                       last_numSeg   (-1),
                       sampleCount   (0),

                       my_red( Colours::red.getARGB() ),
                       my_yellow( look_and_feel->colours.button_on_colour.getARGB()),
                       my_green( look_and_feel->colours.slider_track_colour.getARGB() ),

                       needsRepaint  (true)
{
    setOpaque (true);
}

COLD Monique_Ui_SegmentedMeter::~Monique_Ui_SegmentedMeter() noexcept {}

void Monique_Ui_SegmentedMeter::refresh() noexcept
{
    // map decibels to numSegs
    numSegs = jmax (0, roundToInt ((toDecibels_fast(level) / DB_PER_SEC) + (TOTAL_NUM_SEG - NUM_RED_SEG)));
    level *= 0.8f;
    if( numSegs != last_numSeg || needsRepaint)
    {
        last_numSeg = numSegs;
        repaint();
    }
}

void Monique_Ui_SegmentedMeter::resized()
{
    const float w = getWidth();
    const float h = getHeight();

    onImage = Image (Image::ARGB,
                     w, h,
                     true);
    offImage = Image (Image::ARGB,
                      w, h,
                      true);

    Graphics gOn (onImage);
    Graphics gOff (offImage);



    const float segmentWidth = (w-1) / TOTAL_NUM_SEG;
    for (int i = 1; i <= TOTAL_NUM_SEG; ++i)
    {
        Colour colour_on;
        Colour colour_off;

        if (i <= NUM_GREEN_SEG)
        {
            colour_on = Colour(my_green).brighter(0.25);
            colour_off = Colour(my_green).darker(1).darker(0.5);
        }
        else if (i <= (NUM_YELLOW_SEG + NUM_GREEN_SEG))
        {
            colour_on = Colour(my_yellow).brighter(0.25);
            colour_off = Colour(my_yellow).darker(1).darker(0.5);
        }
        else
        {
            colour_on = Colour(my_red).brighter(0.25);
            colour_off = Colour(my_red).darker(1).darker(0.5);
        }

        float x = w - (i*segmentWidth);
        float x2 = w - ((i+1.0f)*segmentWidth);

        gOn.setGradientFill (ColourGradient (colour_on, x, 0, Colour (0xff333333), x2, 0, false));
        gOn.fillRoundedRectangle (x,1.0f, segmentWidth-2, h-2, 0);

        gOff.setGradientFill (ColourGradient (colour_off.darker(0.3), x, 0, Colour (0xff161616), x2, 0, false));
        gOff.fillRoundedRectangle (x,1.0f, segmentWidth-2, h-2, 0);
        gOff.setColour (colour_off.darker (0.6f));
        gOff.drawRoundedRectangle (x,1.0f, segmentWidth-2, h-2, 0, 1);

    }

    needsRepaint = true;
}

void Monique_Ui_SegmentedMeter::moved()
{
    needsRepaint = true;
}

void Monique_Ui_SegmentedMeter::paint (Graphics &g)
{
    needsRepaint = false;

    const int w = getWidth();
    const int h = getHeight();

    g.fillAll(Colour(0xff050505));

    g.addTransform( AffineTransform::rotation
                    (
                        (float) (180.0f / (180.0 / double_Pi)),
                        0.5*w,
                        0.5*h
                    )
                  );

    if (onImage.isValid())
    {
        g.drawImage (onImage,
                     0, 0, w, h,
                     0, 0, w, h,
                     false);

        const int offWidth = w - roundToInt((float(numSegs) / TOTAL_NUM_SEG) * onImage.getWidth());
        g.drawImage (offImage,
                     0, 0, offWidth, h,
                     0, 0, offWidth, h,
                     false);
    }
}
