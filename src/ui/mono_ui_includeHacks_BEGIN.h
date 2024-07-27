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

//*****************************************************************************
#ifndef DYNAMIC_FILE_INCLUDED
#define DYNAMIC_FILE_INCLUDED 1
#else
PLEASE_INCLUDE_END_IN_RESIZED
#endif
//*****************************************************************************

// repaint
//*****************************************************************************
#define WIDTH_AND_HIGHT_FACTORS                                                                    \
    const float width_factor = 1.0f / original_w * getWidth();                                     \
    const float height_factor = 1.0f / original_h * getHeight();

#define COLOUR_REPLACEMENT_START ComponentColours &colours = look_and_feel->colours;

COLOUR_REPLACEMENT_START
#define Colour(x)                                                                                  \
    ((x == 0xff050505)   ? colours.get_theme(COLOUR_THEMES::BG_THEME).area_colour                  \
     : (x == 0xffff1111) ? colours.get_theme(COLOUR_THEMES::OSC_THEME).area_colour                 \
     : (x == 0xff11ffff) ? colours.get_theme(COLOUR_THEMES::FM_THEME).area_colour                  \
     : (x == 0xff1111ff) ? colours.get_theme(COLOUR_THEMES::FILTER_THEME).area_colour              \
     : (x == 0x301111ff)                                                                           \
         ? colours.get_theme(COLOUR_THEMES::FILTER_THEME).area_colour.withAlpha(0.15f)             \
     : (x == 0x30ffff11) ? colours.get_theme(COLOUR_THEMES::ARP_THEME).area_colour.withAlpha(0.3f) \
     : (x == 0xffffffee) ? colours.get_theme(COLOUR_THEMES::MORPH_THEME).area_colour               \
     : (x == 0xffff11ff) ? colours.get_theme(COLOUR_THEMES::FX_THEME).area_colour                  \
     : (x == 0xffffff11) ? colours.get_theme(COLOUR_THEMES::ARP_THEME).area_colour                 \
     : (x == 0xff11ff11) ? colours.get_theme(COLOUR_THEMES::MASTER_THEME).area_colour              \
                         : juce::Colour(x))

#define fillRoundedRectangle(x, y, w, h, r)                                                        \
    fillRoundedRectangle(                                                                          \
        x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y,                 \
        w > 2 ? float(w) * width_factor : w, h > 2 ? float(h) * height_factor : h, r == 1 ? 0 : r)
#define drawRoundedRectangle(x, y, w, h, r, t)                                                     \
    drawRoundedRectangle(x > 2 ? float(x) * width_factor : x,                                      \
                         y > 2 ? float(y) * height_factor : y,                                     \
                         w > 2 ? float(w) * width_factor : w,                                      \
                         h > 2 ? float(h) * height_factor : h, r == 1 ? 0 : r, t == 1 ? 1 : t)

#define fillRect(x, y, w, h)                                                                       \
    fillRect(x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y,            \
             w > 2 ? float(w) * width_factor : w, h > 2 ? float(h) * height_factor : h)

#define drawRect(x, y, w, h, r)                                                                    \
    drawRect(x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y,            \
             w > 2 ? float(w) * width_factor : w, h > 2 ? float(h) * height_factor : h, float(r))

#define fillEllipse(x, y, w, h)                                                                    \
    fillEllipse(float(x) * width_factor, float(y) * height_factor, float(w) * width_factor,        \
                float(h) * height_factor)

#define drawEllipse(x, y, w, h, b)                                                                 \
    drawEllipse(float(x) * width_factor, float(y) * height_factor, float(w) * width_factor,        \
                float(h) * height_factor, b)
#define ColourGradient(c1, x1, y1, c2, x2, y2, n)                                                  \
    juce::ColourGradient(c1, float(x1) * width_factor, float(y1) * height_factor, c2,              \
                         float(x2) * width_factor, float(y2) * height_factor, n)
#define drawWithin(g, rect, style, n)                                                              \
    drawWithin(g,                                                                                  \
               Rectangle<float>(rect.getX() * width_factor, rect.getY() * height_factor,           \
                                rect.getWidth() * width_factor, rect.getHeight() * height_factor), \
               style, n)

// resized
//*****************************************************************************
#define setBounds(x, y, w, h)                                                                      \
    setBounds(x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y,           \
              w > 2 ? float(w) * width_factor : w, h > 2 ? float(h) * height_factor : h)
#define startNewSubPath(x, y)                                                                      \
    startNewSubPath(x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y)
#define lineTo(x, y)                                                                               \
    lineTo(x > 2 ? float(x) * width_factor : x, y > 2 ? float(y) * height_factor : y)
#define quadraticTo(x, y, x2, y2)                                                                  \
    quadraticTo(x * (1.0f / width_factor, y * height_factor, x2 * width_factor, y2 * height_factor))
