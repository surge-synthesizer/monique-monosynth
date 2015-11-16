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

#define VAR_INDEX_COLOUR_THEME "0"
#define VAR_INDEX_SLIDER_TYPE "1"
#define VAR_INDEX_OVERRIDE_BUTTON_COLOUR "2"
#define VAR_INDEX_COLOUR_THEME_INVERTED "3"
#define VAR_INDEX_SLIDER_LABEL_STYLE "4"
#define VAR_INDEX_BUTTON_AMP "5"
#define VAR_INDEX_STATE_VAR "6"
#define VAR_INDEX_VALUE_TO_SHOW "7"
#define VAR_INDEX_BUTTON_TEXT "8"

#define TURN_BUTTON_OFF( button ) \
if( button->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_OFF ) ) { \
        button->repaint(); \
    }

#define TURN_BUTTON_ON( button ) \
if( button->getProperties().set( VAR_INDEX_BUTTON_AMP, TURN_ON ) ) { \
        button->repaint(); \
    }

#define IS_BUTTON_ON( button ) ( button->getProperties().getWithDefault( VAR_INDEX_BUTTON_AMP, false ) )
    
#define TURN_BUTTON_ON_OR_OFF( button, state ) \
 if(state) { TURN_BUTTON_ON(button); } else { TURN_BUTTON_OFF(button); }

enum COLOR_REPLACEMENTS
{
    TURN_OFF = 0,
    TURN_ON = 1,
    FORCE_BIT_RED = 2,
    FORCE_RED,
    USE_AREA_COLOUR,
    USE_AREA_TRANSCULENT
};

enum COLOUR_THEMES
{
    OSC_THEME,
    FM_THEME,
    FILTER_THEME,
    FX_THEME,
    MASTER_THEME,
    ARP_THEME,
    MORPH_THEME,
    BG_THEME,
    DUMMY_THEME,
    //OSZI_THEME,
    //SETUP_THEME,
    //MIDI_THEME,
    //TOP_THEME,

    SUM_OF,
    CLIPBOARD_THEME
};
enum COLOUR_CODES
{
    AREA_COLOUR = USE_AREA_TRANSCULENT + 1,
    AREA_FONT_COLOUR,
    VALUE_SLIDER_COLOUR,
    VALUE_SLIDER_2_COLOUR,
    MOD_SLIDER_COLOUR,
    DISABLED_SLIDER_COLOUR,
    SLIDER_BACKGROUND_COLOUR,
    BUTTON_ON_COLOUR,
    BUTTON_ON_FONT_COLOUR,
    BUTTON_OFF_COLOUR,
    BUTTON_OFF_FONT_COLOUR,
};
#define TOP_THEME ARP_THEME
#define OSZI_THEME ARP_THEME
#define SETUP_THEME ARP_THEME
#define MIDI_THEME ARP_THEME
#define POPUP_THEME ARP_THEME
#define KEYBOARD_THEME ARP_THEME
#define MORPH_EDITOR_THEME MORPH_THEME

enum SLIDER_STYLES
{
    VALUE_SLIDER,
    VALUE_SLIDER_WITHOUT_BACK_SLIDER,
    VALUE_SLIDER_2,
    MODULATION_SLIDER
};
enum SLIDER_LABEL_STYLES
{
    DONT_SHOW_TEXT,
    SHOW_MIDDLE_TEXT_BOX,
    JUST_HIDE_CENTER
};

struct SectionTheme
{
    const COLOUR_THEMES id;

    Colour area_colour;
    Colour area_font_colour;

    Colour value_slider_track_colour;
    Colour value_2_slider_track_colour;
    Colour mod_slider_track_colour;
    Colour disabled_track_colour;
    Colour slider_bg_colour;

    Colour button_on_font_colour;
    Colour button_on_colour;
    Colour button_off_font_colour;
    Colour button_off_colour;

    void copy( const SectionTheme& other_ ) noexcept
    {
        area_colour = other_.area_colour;
        area_font_colour = other_.area_font_colour;
        value_slider_track_colour = other_.value_slider_track_colour;
        value_2_slider_track_colour = other_.value_2_slider_track_colour;
        mod_slider_track_colour = other_.mod_slider_track_colour;
        disabled_track_colour = other_.disabled_track_colour;
        slider_bg_colour = other_.slider_bg_colour;
        button_on_font_colour = other_.button_on_font_colour;
        button_on_colour = other_.button_on_colour;
        button_off_font_colour = other_.button_off_font_colour;
        button_off_colour = other_.button_off_colour;
    }

    Colour& get_color( COLOUR_CODES code_ ) noexcept
    {
        switch( code_ )
        {
        case COLOUR_CODES::AREA_COLOUR :
            return area_colour;
        case COLOUR_CODES::AREA_FONT_COLOUR :
            return area_font_colour;
        case COLOUR_CODES::VALUE_SLIDER_COLOUR :
            return value_slider_track_colour;
        case COLOUR_CODES::VALUE_SLIDER_2_COLOUR :
            return value_2_slider_track_colour;
        case COLOUR_CODES::MOD_SLIDER_COLOUR :
            return mod_slider_track_colour;
        case COLOUR_CODES::DISABLED_SLIDER_COLOUR :
            return disabled_track_colour;
        case COLOUR_CODES::SLIDER_BACKGROUND_COLOUR :
            return slider_bg_colour;
        case COLOUR_CODES::BUTTON_ON_COLOUR :
            return button_on_colour;
        case COLOUR_CODES::BUTTON_ON_FONT_COLOUR :
            return button_on_font_colour;
        case COLOUR_CODES::BUTTON_OFF_COLOUR :
            return button_off_colour;
            //case COLOUR_CODES::BUTTON_OFF_FONT_COLOUR :
        default :
            return button_off_font_colour;
        }
    }
    /*
    const Colour& get_color( COLOUR_CODES code_ ) const noexcept
    {
        switch( code_ )
        {
        case COLOUR_CODES::AREA_COLOUR :
            return area_colour;
        case COLOUR_CODES::AREA_FONT_COLOUR :
            return area_font_colour;
        case COLOUR_CODES::VALUE_SLIDER_COLOUR :
            return value_slider_track_colour;
        case COLOUR_CODES::VALUE_SLIDER_2_COLOUR :
            return value_2_slider_track_colour;
        case COLOUR_CODES::MOD_SLIDER_COLOUR :
            return mod_slider_track_colour;
        case COLOUR_CODES::DISABLED_SLIDER_COLOUR :
            return disabled_track_colour;
        case COLOUR_CODES::SLIDER_BACKGROUND_COLOUR :
            return slider_bg_colour;
        case COLOUR_CODES::BUTTON_ON_COLOUR :
            return button_on_font_colour;
        case COLOUR_CODES::BUTTON_ON_FONT_COLOUR :
            return button_on_colour;
        case COLOUR_CODES::BUTTON_OFF_COLOUR :
            return button_off_font_colour;
        case COLOUR_CODES::BUTTON_OFF_FONT_COLOUR :
            return button_off_colour;
        }
    }
    */

    SectionTheme(COLOUR_THEMES id_)
        : id( id_ ),

          area_colour( 0xff333333 ),
          area_font_colour( 0xffffffff ),

          value_slider_track_colour( 0xffffffff ),
          value_2_slider_track_colour( 0xffdddddd ),
          mod_slider_track_colour( 0xffaaaaaa ),
          disabled_track_colour( 0xff555555 ),
          slider_bg_colour( 0xff050505 ),

          button_on_font_colour( 0xff050505 ),
          button_on_colour( 0xffffffff ),
          button_off_font_colour( 0xffffffff ),
          button_off_colour( 0xff050505 )
    {
    }
};

struct ComponentColours
{
    ScopedPointer<SectionTheme> theme_clipboard;
    OwnedArray<SectionTheme> themes;

    void init_themes() noexcept
    {
        //themes.add( new SectionTheme( TOP_THEME ) );
        themes.add( new SectionTheme( OSC_THEME ) );
        themes.add( new SectionTheme( FM_THEME ) );
        themes.add( new SectionTheme( FILTER_THEME ) );
        themes.add( new SectionTheme( FX_THEME ) );
        themes.add( new SectionTheme( MASTER_THEME ) );
        themes.add( new SectionTheme( ARP_THEME ) );
        //themes.add( new SectionTheme( OSZI_THEME ) );
        //themes.add( new SectionTheme( SETUP_THEME ) );
        //themes.add( new SectionTheme( MIDI_THEME ) );
        themes.add( new SectionTheme( MORPH_THEME ) );
        themes.add( new SectionTheme( BG_THEME ) );

        SectionTheme*theme = new SectionTheme(DUMMY_THEME);
        theme->area_colour = Colours::magenta;
        theme->area_font_colour = Colours::magenta;
        theme->value_slider_track_colour = Colours::magenta;
        theme->value_2_slider_track_colour = Colours::magenta;
        theme->mod_slider_track_colour = Colours::magenta;
        theme->disabled_track_colour = Colours::magenta;
        theme->slider_bg_colour = Colours::magenta;
        theme->button_on_font_colour = Colours::magenta;
        theme->button_on_colour = Colours::magenta;
        theme->button_off_font_colour = Colours::magenta;
        theme->button_off_colour = Colours::magenta;
        themes.add( theme );
    }
    const SectionTheme& get_theme(COLOUR_THEMES id_) const noexcept
    {
        return *themes.getUnchecked( static_cast<int>(id_) );
    }
    SectionTheme& get_theme(COLOUR_THEMES id_) noexcept
    {
        return *themes.getUnchecked( static_cast<int>(id_) );
    }

    Colour midi_learn;

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

//==============================================================================
//==============================================================================
//==============================================================================
class Monique_Ui_Mainwindow;
class MoniqueSynthData;
class UiLookAndFeel  : public LookAndFeel_V2, public SliderListener
{
    friend class Monique_Ui_Mainwindow;
    friend class MoniqueSynthData;
    Monique_Ui_Mainwindow*mainwindow;

    BoolParameter is_global_user_return;// = true;
    BoolParameter is_global_factory_return;// = false;
    BoolParameter is_global_program_return;// = false;
    BoolParameter is_global_undo_return;// = false;

    MoniqueSynthData*synth_data;

public:
    void set_synth_data( MoniqueSynthData*synth_data_ ) {
        synth_data = synth_data_;
    }
    void clear_synth_data() {
        synth_data = nullptr;
    }

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
    void drawPopupMenuSectionHeader (Graphics& g, const Rectangle<int>& area, const String& sectionName) override;
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
    //==============================================================================
    //==============================================================================
    ScopedPointer<Slider> popup_smooth_Slider;
    ScopedPointer<Slider> popup_linear_sensi_slider;
    ScopedPointer<Slider> popup_rotary_sensi_slider;
    ScopedPointer<Slider> popup_midi_snap_slider;
    PopupMenu* getCustomPopupMenu (Slider*) /*override*/;
    bool sliderMenuCallback (const int result, Slider* slider) /*override*/;
    bool sliderDoubleClicked ( Slider* slider) /*override*/;
    void sliderValueChanged (Slider* sliderThatWasMoved) /*override*/;

    //==============================================================================
    Rectangle<int> getTooltipBounds (const String& tipText, Point<int> screenPos, Rectangle<int> parentArea) override;
    
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
