//*****************************************************************************
#ifndef DYNAMIC_FILE_INCLUDED
  #define DYNAMIC_FILE_INCLUDED 1
#else
    PLEASE_INCLUDE_END_IN_RESIZED
#endif
//*****************************************************************************

// repaint colours
//*****************************************************************************
#define COLOUR_REPLACEMENT_START \
    ComponentColours& colours = UiLookAndFeel::getInstance()->colours; \
    Colour orange_replacement = colours.bg_lines; \
    Colour blue_replacement = colours.signal_lines; \
    Colour border_replacement = colours.bg_lines; \
    Colour bg_replacement = colours.bg; \
    Colour yellow_replacement = colours.label_text_colour;

COLOUR_REPLACEMENT_START
#define Colour(x) ( \
                       (x == 0xffff3b00) ? orange_replacement : \
                       (x == 0xff11ffff) ? blue_replacement   : \
                       (x == 0xff1111ff) ? border_replacement : \
                       (x == 0xff050505) ? bg_replacement : \
                       (x == 0xffffff11) ? yellow_replacement : \
                                           Colour(x) \
                  )

// repaint
//*****************************************************************************
#define WIDTH_AND_HIGHT_FACTORS \
  const float width_factor = 1.0f/original_w*getWidth(); \
  const float height_factor = 1.0f/original_h*getHeight();

WIDTH_AND_HIGHT_FACTORS
#define fillRoundedRectangle(x,y,w,h,r) fillRoundedRectangle (x > 2 ? float(x)*width_factor : x, \
                                                              y > 2 ? float(y)*height_factor : y, \
                                                              w > 2 ? float(w)*width_factor : w, \
                                                              h > 2 ? float(h)*height_factor : h, \
                                                              r == 1 ? 0 : r )
#define drawRoundedRectangle(x,y,w,h,r,t) drawRoundedRectangle (x > 2 ? float(x)*width_factor : x, \
                                                              y > 2 ? float(y)*height_factor : y, \
                                                              w > 2 ? float(w)*width_factor : w, \
                                                              h > 2 ? float(h)*height_factor : h, \
                                                              r == 1 ? 0 : r, \
							      t == 1 ? 1 : t )                               
                                                              
                                                              
#define fillRect(x,y,w,h)                           fillRect (x > 2 ? float(x)*width_factor : x, \
                                                              y > 2 ? float(y)*height_factor : y, \
                                                              w > 2 ? float(w)*width_factor : w, \
                                                              h > 2 ? float(h)*height_factor : h)
                                                              
#define drawRect(x,y,w,h,r)                         drawRect (x > 2 ? float(x)*width_factor : x, \
                                                              y > 2 ? float(y)*height_factor : y, \
                                                              w > 2 ? float(w)*width_factor : w, \
                                                              h > 2 ? float(h)*height_factor : h, float(r))
                                                              
#define fillEllipse(x,y,w,h)                         fillEllipse(float(x)*width_factor, \
                                                              float(y)*height_factor, \
                                                              float(w), \
                                                              float(h))
                                                              
#define drawEllipse(x,y,w,h,b)                       drawEllipse(float(x)*width_factor, \
                                                              float(y)*height_factor, \
                                                              float(w), \
                                                              float(h), \
                                                              b)
                                                              
// resized
//*****************************************************************************
#define setBounds(x,y,w,h)         setBounds (x > 2 ? float(x)*(1.0f/original_w*getWidth()) : x, \
                                              y > 2 ? float(y)*(1.0f/original_h*getHeight()) : y, \
                                              w > 2 ? float(w)*(1.0f/original_w*getWidth()) : w, \
                                              h > 2 ? float(h)*(1.0f/original_h*getHeight()) : h)
#define startNewSubPath(x,y) startNewSubPath (x > 2 ? float(x)*(1.0f/original_w*getWidth()) : x, \
                                              y > 2 ? float(y)*(1.0f/original_h*getHeight()) : y )
#define lineTo(x,y)                   lineTo (x > 2 ? float(x)*(1.0f/original_w*getWidth()) : x, \
                                              y > 2 ? float(y)*(1.0f/original_h*getHeight()) : y )
#define quadraticTo(x,y,x2,y2)   quadraticTo (x*(1.0f/original_w*getWidth()), y*(1.0f/original_h*getHeight()),x2*(1.0f/original_w*getWidth()), y2*(1.0f/original_h*getHeight()))
