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

#include "dRowAudio_SegmentedMeter.h"

SegmentedMeter::SegmentedMeter()
    : my_red(Colours::red.getARGB()),
      my_yellow(4287387171),
      my_green(4278251775),
      numRedSeg     (2),
      numYellowSeg  (4),
      numGreenSeg   (9),
      totalNumSegs  (numRedSeg + numYellowSeg + numGreenSeg),
      decibelsPerSeg(3.0f),
      numSegs       (0),
      sampleCount   (0),
      samplesToCount(2048),
      sampleMax     (0.0f),
      level         (0.0f),
      needsRepaint  (true)
{
    setOpaque (true);
}

SegmentedMeter::~SegmentedMeter()
{
}

void SegmentedMeter::calculateSegments()
{
    float numDecibels = (float) toDecibels (level.getCurrent());
    // map decibels to numSegs
    numSegs = jmax (0, roundToInt ((numDecibels / decibelsPerSeg) + (totalNumSegs - numRedSeg)));

    // impliment slow decay
    //	level.set((0.5f * level.getCurrent()) + (0.1f * level.getPrevious()));
    level *= 0.8f;

    // only actually need to repaint if the numSegs has changed
    if (! numSegs.areEqual() || needsRepaint)
        repaint();
}

void SegmentedMeter::timerCallback()
{
    calculateSegments();
}

void SegmentedMeter::resized()
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

    const int numSegments = (numRedSeg + numYellowSeg + numGreenSeg);
    const float segmentHeight = (h-1) / numSegments;

    for (int i = 1; i <= numSegments; ++i)
    {
        Colour colour_on;
        Colour colour_off;

        if (i <= numGreenSeg)
        {
            colour_on = Colour(my_green).brighter(0.25);
            colour_off = Colour(my_green).darker(1).darker(0.5);
        }
        else if (i <= (numYellowSeg + numGreenSeg))
        {
            colour_on = Colour(my_yellow).brighter(0.25);
            colour_off = Colour(my_yellow).darker(1).darker(0.5);
        }
        else
        {
            colour_on = Colour(my_red).brighter(0.25);
            colour_off = Colour(my_red).darker(1).darker(0.5);
        }

        float y = h - (i*segmentHeight);
        float y2 = h - ((i+1.0f)*segmentHeight);
	//gOn.fillAll(Colour(0xff161616));
        gOn.setGradientFill (ColourGradient (colour_on.darker(0.1), 0.0f, y, Colour (0xff161616), 0.0f, y2, false));
        gOn.fillRoundedRectangle (1.0f,y, w-2, segmentHeight-2, 0.3);
        gOn.setColour (colour_off);
        gOn.drawRoundedRectangle (1.0f,y, w-2, segmentHeight-2, 0.3, 0.5);
        gOff.setGradientFill (ColourGradient (colour_off.darker(0.3), 0.0f, y, Colour (0xff161616), 0.0f, y2, false));
        gOff.fillRoundedRectangle (1.0f,y, w-2, segmentHeight-2, 0.3);
        gOff.setColour (colour_off.darker (0.6f));
        gOff.drawRoundedRectangle (1.0f,y, w-2, segmentHeight-2, 0.3, 0.5);
    }
/*
    gOn.setColour (Colour(0xff161616).darker (0.4f));
    gOff.drawRoundedRectangle (0, 0, w, h, 3, 1.5);

    gOff.setColour (Colour(0xff161616).darker (0.4f));
    gOff.drawRoundedRectangle (0, 0, w, h, 3, 1.5);
*/
    needsRepaint = true;
}

void SegmentedMeter::paint (Graphics &g)
{
    const int w = getWidth();
    const int h = getHeight();

    g.fillAll(Colour(0xff161616));
    
    if (onImage.isValid())
    {
        const int onHeight = roundToInt ((numSegs.getCurrent() / (float) totalNumSegs) * onImage.getHeight());
        const int offHeight = h - onHeight;

//        g.drawImage (onImage,
//                     0, offHeight, w, onHeight,
//                     0, offHeight, w, onHeight,
//                     false);
//
//        g.drawImage (offImage,
//                     0, 0, w, offHeight,
//                     0, 0, w, offHeight,
//                     false);
        g.drawImage (onImage,
                     0, 0, w, h,
                     0, 0, w, h,
                     false);
        g.drawImage (offImage,
                     0, 0, w, offHeight,
                     0, 0, w, offHeight,
                     false);
    }

    needsRepaint = false;
}

void SegmentedMeter::process()
{
    // calculate current meter level
    if (samples.getData() != nullptr)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = samples[i];
            if( sample > 2 )
                sample = 2;
            if( sample < -2 )
                sample = -2;

            sample = fabsf (sample);

            if (sample > sampleMax)
                sampleMax = sample;

            if (++sampleCount == samplesToCount)
            {
                if (sampleMax > level.getCurrent())
                    level = sampleMax;

                sampleMax = 0.0f;
                sampleCount = 0;
            }
        }
    }
}

