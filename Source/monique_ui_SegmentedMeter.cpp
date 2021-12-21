#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_LookAndFeel.h"

#define NUM_RED_SEG 2
#define NUM_YELLOW_SEG 3
#define NUM_GREEN_SEG 4
#define TOTAL_NUM_SEG (NUM_RED_SEG + NUM_YELLOW_SEG + NUM_GREEN_SEG)

#define DB_PER_SEC 3.0f
#define SAMPLES_TO_COUNT 2048

COLD Monique_Ui_SegmentedMeter::Monique_Ui_SegmentedMeter(
    Monique_Ui_Refresher *const ui_refresher_) noexcept
    : Monique_Ui_Refreshable(ui_refresher_), sampleMax(0.0f), level(0.0f),

      numSegs(0), last_numSeg(-1), sampleCount(0),

      my_green(look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_1),
      my_yellow(look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_2),
      my_red(look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_3),
      my_bg(look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).area_colour),

      needsRepaint(true)
{
    setOpaque(true);
}

COLD Monique_Ui_SegmentedMeter::~Monique_Ui_SegmentedMeter() noexcept {}

void Monique_Ui_SegmentedMeter::refresh() noexcept
{
    // map decibels to numSegs
    numSegs =
        jmax(0, roundToInt((toDecibels_fast(level) / DB_PER_SEC) + (TOTAL_NUM_SEG - NUM_RED_SEG)));
    level *= 0.8f;
    if (numSegs != last_numSeg or needsRepaint)
    {
        last_numSeg = numSegs;
        repaint();
    }
}

void Monique_Ui_SegmentedMeter::resized()
{
    const float w = getWidth();
    const float h = getHeight();

    onImage = Image(Image::ARGB, w, h, true);
    offImage = Image(Image::ARGB, w, h, true);

    Graphics gOn(onImage);
    Graphics gOff(offImage);

    my_green = look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_1;
    my_yellow = look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_2;
    my_red = look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).oszi_3;
    my_bg = look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME).button_off_colour;

    const float segmentWidth = (w - 1) / TOTAL_NUM_SEG;
    for (int i = 1; i <= TOTAL_NUM_SEG; ++i)
    {
        Colour colour_on;
        Colour colour_off;

        if (i <= NUM_GREEN_SEG)
        {
            colour_on = my_green.brighter(0.25);
            colour_off = my_green.darker(1).darker(0.5);
        }
        else if (i <= (NUM_YELLOW_SEG + NUM_GREEN_SEG))
        {
            colour_on = my_yellow.brighter(0.25);
            colour_off = my_yellow.darker(1).darker(0.5);
        }
        else
        {
            colour_on = my_red.brighter(0.25);
            colour_off = my_red.darker(1).darker(0.5);
        }

        const float x = w - (segmentWidth * i);

        gOn.setColour(colour_on);
        gOn.fillRoundedRectangle(x, 1.0f, segmentWidth - 2, h - 2, 0);

        gOff.setColour(colour_off.darker(0.3));
        gOff.fillRoundedRectangle(x, 1.0f, segmentWidth - 2, h - 2, 0);
        gOff.setColour(colour_off.darker(0.6f));
        gOff.drawRoundedRectangle(x, 1.0f, segmentWidth - 2, h - 2, 0, 1);
    }

    needsRepaint = true;
}

void Monique_Ui_SegmentedMeter::moved() { needsRepaint = true; }

void Monique_Ui_SegmentedMeter::paint(Graphics &g)
{
    {
        const SectionTheme &theme = look_and_feel->colours.get_theme(COLOUR_THEMES::MASTER_THEME);
        if (my_red != theme.oszi_3 or my_yellow != theme.oszi_2 or my_green != theme.oszi_1 or
            my_bg != theme.button_off_colour)
        {
            resized();
            return;
        }
    }

    needsRepaint = false;

    const int w = getWidth();
    const int h = getHeight();

    g.fillAll(Colour(my_bg));

    g.addTransform(
        AffineTransform::rotation((float)(180.0f / (180.0 / double_Pi)), 0.5f * w, 0.5f * h));

    if (onImage.isValid())
    {
        g.drawImage(onImage, 0, 0, w, h, 0, 0, w, h, false);

        const int offWidth =
            w - jmin(w, jmax(0, roundToInt((float(numSegs) / TOTAL_NUM_SEG) * onImage.getWidth())));
        g.drawImage(offImage, 0, 0, offWidth, h, 0, 0, offWidth, h, false);
    }
}

/*
#include "monique_ui_SegmentedMeter.h"
#include "monique_ui_LookAndFeel.h"

#define NUM_RED_SEG 2
#define NUM_YELLOW_SEG 3
#define NUM_GREEN_SEG 4
#define TOTAL_NUM_SEG (NUM_RED_SEG+NUM_YELLOW_SEG+NUM_GREEN_SEG)

#define DB_PER_SEC 3.0f
#define SAMPLES_TO_COUNT 2048

COLD Monique_Ui_SegmentedMeter::Monique_Ui_SegmentedMeter( Monique_Ui_Refresher*const ui_refresher_
) noexcept
:
Monique_Ui_Refreshable(ui_refresher_),
                       sampleMax     (0.0f),
                       level         (0.0f),

                       numSegs       (0),
                       last_numSeg   (-1),
                       sampleCount   (0),

                       my_yellow_ref( look_and_feel->colours.get_theme( COLOUR_THEMES::MASTER_THEME
).button_on_colour ), my_green_ref( look_and_feel->colours.get_theme( COLOUR_THEMES::MASTER_THEME
).button_off_colour ), my_bg_ref( look_and_feel->colours.get_theme( COLOUR_THEMES::MASTER_THEME
).area_colour ),

                       my_red( Colours::red ),
                       my_yellow( my_yellow_ref ),
                       my_green( my_green_ref ),
                       my_bg( my_bg_ref ),

                       needsRepaint  (true)
{
    setOpaque (true);
}

COLD Monique_Ui_SegmentedMeter::~Monique_Ui_SegmentedMeter() noexcept {}

void Monique_Ui_SegmentedMeter::refresh() noexcept
{
    // map decibels to numSegs
    numSegs = jmax (0, roundToInt ((toDecibels_fast(level) / DB_PER_SEC) + (TOTAL_NUM_SEG -
NUM_RED_SEG))); level *= 0.8f; if( numSegs != last_numSeg || needsRepaint)
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
            colour_on = my_green_ref.brighter(0.25);
            colour_off = my_green_ref.darker(1).darker(0.5);
        }
        else if (i <= (NUM_YELLOW_SEG + NUM_GREEN_SEG))
        {
            colour_on = my_yellow_ref.brighter(0.25);
            colour_off = my_yellow_ref.darker(1).darker(0.5);
        }
        else
        {
            colour_on = my_red.brighter(0.25);
            colour_off = my_red.darker(1).darker(0.5);
        }

        const float x = w - (segmentWidth*i);

        gOn.setColour (colour_on);
        gOn.fillRoundedRectangle (x,1.0f, segmentWidth-2, h-2, 0);

        gOff.setColour (colour_off.darker(0.3));
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
    if( my_yellow != my_yellow_ref or my_green != my_green_ref or my_bg != my_bg_ref )
    {
        my_yellow = my_yellow_ref;
        my_green = my_green_ref;
        my_bg = my_bg_ref;

        offImage.clear(offImage.getBounds());
        onImage.clear(onImage.getBounds());

        needsRepaint = true;

        resized();
        refresh();
        return;
    }

    needsRepaint = false;

    const int w = getWidth();
    const int h = getHeight();

    g.fillAll(Colour(0xff050505));

    g.addTransform( AffineTransform::rotation
                    (
                        (float) (180.0f / (180.0 / double_Pi)),
                        0.5f*w,
                        0.5f*h
                    )
                  );

    if (onImage.isValid())
    {
        g.drawImage (onImage,
                     0, 0, w, h,
                     0, 0, w, h,
                     false);

        const int offWidth = w - jmin(w,jmax(0,roundToInt((float(numSegs) / TOTAL_NUM_SEG) *
onImage.getWidth()))); g.drawImage (offImage, 0, 0, offWidth, h, 0, 0, offWidth, h, false);
    }
}
*/