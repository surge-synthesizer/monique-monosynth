/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef MONO_LOOKANDFEEL
#define MONO_LOOKANDFEEL

#include "App_h_includer.h"

//==============================================================================
/**
    @see LookAndFeel_V2
*/
//==============================================================================
//==============================================================================
//==============================================================================
struct ComponentColours 
{
    Colour slider_track_colour;
    Colour slider_track_colour_2;
    Colour slider_track_colour_modulation;
    Colour button_on_colour;
    Colour button_off_colour;
    Colour label_text_colour;
    Colour midi_learn;
    Colour bg;
    Colour bg_lines;
    Colour signal_lines;

    COLD ComponentColours() noexcept;
    COLD ~ComponentColours() noexcept;

    COLD void read_from( XmlElement*xml_ ) noexcept;
    COLD void save_to( XmlElement*xml_ ) noexcept;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ComponentColours )
};

//==============================================================================
// HACK 's
enum MODULATION_SLIDER_STYLE 
{
    MODULATION_SLIDER_IS_FIXED_CENTERED,
    MODULATION_SLIDER_MOVES_WITH_MASTER_FROM_ZERO,
    MODULATION_SLIDER_MOVES_WITH_MASTER,
};

enum SLIDER_STYLES 
{
    VALUE_SLIDER,
    MODULATION_SLIDER,
    FIRST_MODULATION_BUT_HAS_SECOND_MODULATION,
    VALUE_SLIDER_2,

    UNDEFINED_SLIDER_STYLE
};
enum SLIDER_LABEL_STYLES 
{
    DONT_SHOW_TEXT = 0,
    SHOW_MIDDLE_TEXT_BOX = 1,
    JUST_HIDE_CENTER
};

#define IS_VALUE_LABEL "VL"
#define IS_SECOND_VALUE_LABEL "SVL"
#define IS_DESCRIPTION_LABEL "DL"

#define GET_VALUE_TO_PAINT getName
#define SET_VALUE_TO_PAINT setName
#define SET_SLIDER_LABEL_STYLE(s,type) s->setTextBoxStyle(Slider::NoTextBox,true,s->getTextBoxWidth(),type)
#define GET_SLIDER_LABEL_STYLE getTextBoxHeight
#define GET_SLIDER_STYLE getTextBoxWidth
#define SET_SLIDER_STYLE(s,type) s->setTextBoxStyle(Slider::NoTextBox,true,type,s->getTextBoxHeight())

#define GET_LABEL_STYLE getName
#define SET_LABEL_STYLE setName

//==============================================================================
//==============================================================================
//==============================================================================
class UiLookAndFeel  : public LookAndFeel_V2 
{
public:
    ComponentColours colours;
    Component* midi_learn_comp;
    bool show_values_always;

    Font defaultFont;

private:
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< UiLookAndFeel >;
    UiLookAndFeel() noexcept;
    ~UiLookAndFeel() noexcept;

public:
    //==============================================================================
    void drawButtonBackground (Graphics&, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;

    Font getTextButtonFont (TextButton&, int buttonHeight = 0 ) override;

    void drawButtonText (Graphics&, TextButton& button,
                         bool isMouseOverButton, bool isButtonDown) override;

    void drawToggleButton (Graphics&, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override;

    void drawTickBox (Graphics&, Component&,
                      float x, float y, float w, float h,
                      bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override;

    //==============================================================================
    AlertWindow* createAlertWindow (const String& title, const String& message,
                                    const String& button1,
                                    const String& button2,
                                    const String& button3,
                                    AlertWindow::AlertIconType iconType,
                                    int numButtons, Component* associatedComponent) override;

    void drawAlertBox (Graphics&, AlertWindow&, const Rectangle<int>& textArea, TextLayout&) override;
    int getAlertBoxWindowFlags() override;
    int getAlertWindowButtonHeight() override;
    Font getAlertWindowMessageFont() override;
    Font getAlertWindowFont() override;

    //==============================================================================
    void fillTextEditorBackground (Graphics&, int width, int height, TextEditor&) override;
    void drawTextEditorOutline (Graphics&, int width, int height, TextEditor&) override;
    CaretComponent* createCaretComponent (Component* keyFocusOwner) override;

    //==============================================================================
    void drawPopupMenuBackground (Graphics&, int width, int height) override;

    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

    Font getPopupMenuFont() override;

    void drawPopupMenuUpDownArrow (Graphics&, int width, int height, bool isScrollUpArrow) override;

    void getIdealPopupMenuItemSize (const String& text, bool isSeparator, int standardMenuItemHeight,
                                    int& idealWidth, int& idealHeight) override;

    //==============================================================================
    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    Font getComboBoxFont (ComboBox&) override;
    Label* createComboBoxTextBox (ComboBox&) override;
    void positionComboBoxText (ComboBox&, Label&) override;

    //==============================================================================
    void drawLabel (Graphics&, Label&) override;
    Font getLabelFont (Label&) override;

    //==============================================================================
    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&) override;

    void drawLinearSliderBackground (Graphics&, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const Slider::SliderStyle, Slider&) override;

    void drawLinearSliderThumb (Graphics&, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle, Slider&) override;

    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           Slider&) override;

    int getSliderThumbRadius (Slider&) override;
    Font getSliderPopupFont (Slider&) override;
    int getSliderPopupPlacement (Slider&) override;

    //==============================================================================
    void getTooltipSize (const String& tipText, int& width, int& height) override;
    void drawTooltip (Graphics&, const String& text, int width, int height) override;

    //==============================================================================
    void drawCornerResizer (Graphics&, int w, int h, bool isMouseOver, bool isMouseDragging) override;

    //==============================================================================
    void fillResizableWindowBackground (Graphics&, int w, int h, const BorderSize<int>&, ResizableWindow&) override;
    void drawResizableWindowBorder (Graphics&, int w, int h, const BorderSize<int>& border, ResizableWindow&) override;

    //==============================================================================
    void drawDocumentWindowTitleBar (DocumentWindow&, Graphics&, int w, int h,
                                     int titleSpaceX, int titleSpaceW,
                                     const Image* icon, bool drawTitleTextOnLeft) override;

    Button* createDocumentWindowButton (int buttonType) override;

    void positionDocumentWindowButtons (DocumentWindow&,
                                        int titleBarX, int titleBarY, int titleBarW, int titleBarH,
                                        Button* minimiseButton,
                                        Button* maximiseButton,
                                        Button* closeButton,
                                        bool positionTitleBarButtonsOnLeft) override;

    /** Utility function to draw a shiny, glassy circle (for round LED-type buttons). */
    static void drawGlassSphere (Graphics&, float x, float y, float diameter,
                                 const Colour& colour, float outlineThickness) noexcept;

    static void drawGlassPointer (Graphics&, float x, float y, float diameter,
                                  const Colour& colour, float outlineThickness, int direction) noexcept;

    /** Utility function to draw a shiny, glassy oblong (for text buttons). */
    static void drawGlassLozenge (Graphics&,
                                  float x, float y, float width, float height,
                                  const Colour& colour, float outlineThickness, float cornerSize,
                                  bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;


private:
    //==============================================================================
    void drawShinyButtonShape (Graphics&,
                               float x, float y, float w, float h, float maxCornerSize,
                               const Colour& baseColour, float strokeWidth,
                               bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;

    class GlassWindowButton;
    class SliderLabelComp;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiLookAndFeel)
};


#endif   // JUCE_LOOKANDFEEL_V2_H_INCLUDED
