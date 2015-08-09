//*****************************************************************************
#ifndef DYNAMIC_FILE_INCLUDED
  #define DYNAMIC_FILE_INCLUDED 1
#else
    PLEASE_INCLUDE_DYNAMIC_SIZE_END_IN_RESIZED
#endif
//*****************************************************************************

// repaint colours
//*****************************************************************************
#define COLOUR_REPLACEMENT_START \
    ComponentColours colours = UiLookAndFeel::getInstance()->colours; \
    Colour orange_replacement = colours.bg_lines; \
    orange_replacement.getARGB(); 
    
COLOUR_REPLACEMENT_START
#define Colour(x) ((x == 0xffff3b00) ? orange_replacement : Colour(x))

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
							      t == 1 ? 0 : t )                               
                                                              
                                                              
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
                                                              float(w)*width_factor, \
                                                              float(h)*height_factor)
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
