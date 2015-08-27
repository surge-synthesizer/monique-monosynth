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

#include "UiLookAndFeel.h"
#include <iomanip>

juce_ImplementSingleton (UiLookAndFeel)

namespace LookAndFeelHelpers
{
static Colour createBaseColour (Colour buttonColour,
                                bool hasKeyboardFocus,
                                bool isMouseOverButton,
                                bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

    if (isButtonDown)      return baseColour.contrasting (0.2f);
    if (isMouseOverButton) return baseColour.contrasting (0.1f);

    return baseColour;
}

static TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
{
    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = 400;

    AttributedString s;
    s.setJustification (Justification::centred);
    s.append (text, Font (tooltipFontSize, Font::bold), colour);

    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
}
}

//==============================================================================
UiLookAndFeel::UiLookAndFeel()
{
    std::cout << "MONIQUE: init style" << std::endl;

    // initialise the standard set of colours..
    const uint32 textButtonColour      = 0xffbbbbff;
    const uint32 textHighlightColour   = 0x401111ee;
    const uint32 standardOutlineColour = 0xb2808080;

    static const uint32 standardColours[] =
    {
        TextButton::buttonColourId,                 0xff000000,
        TextButton::buttonOnColourId,               0xff4444ff,
        TextButton::textColourOnId,                 0xff000000,
        TextButton::textColourOffId,                0xff000000,

        ToggleButton::textColourId,                 0xff000000,

        TextEditor::backgroundColourId,             0xffffffff,
        TextEditor::textColourId,                   0xffff0000,
        TextEditor::highlightColourId,              0x66ff0000,
        TextEditor::highlightedTextColourId,        0xffffffff,
        TextEditor::outlineColourId,                0x00000000,
        TextEditor::focusedOutlineColourId,         textButtonColour,
        TextEditor::shadowColourId,                 0x38000000,

        CaretComponent::caretColourId,              0xff000000,

        Label::backgroundColourId,                  0x00000000,
        Label::textColourId,                        0xffff3b00,
        Label::outlineColourId,                     0x00000000,

        ScrollBar::backgroundColourId,              0x00000000,
        ScrollBar::thumbColourId,                   0xffffffff,

        TreeView::linesColourId,                    0x4c000000,
        TreeView::backgroundColourId,               0x00000000,
        TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
        TreeView::selectedItemBackgroundColourId,   0x00000000,

        PopupMenu::backgroundColourId,              0xffffffff,
        PopupMenu::textColourId,                    0xff000000,
        PopupMenu::headerTextColourId,              0xff000000,
        PopupMenu::highlightedTextColourId,         0xffffffff,
        PopupMenu::highlightedBackgroundColourId,   0x991111aa,

        ComboBox::buttonColourId,                   0xffbbbbff,
        ComboBox::outlineColourId,                  0xffff0000,
        ComboBox::textColourId,                     0xffff0000,
        ComboBox::backgroundColourId,               0xff000000,
        ComboBox::arrowColourId,                    0x99000000,

        PropertyComponent::backgroundColourId,      0x66ffffff,
        PropertyComponent::labelTextColourId,       0xffff3b00,

        TextPropertyComponent::backgroundColourId,  0xffffffff,
        TextPropertyComponent::textColourId,        0xffffffff,
        TextPropertyComponent::outlineColourId,     standardOutlineColour,

        ListBox::backgroundColourId,                0xffffffff,
        ListBox::outlineColourId,                   standardOutlineColour,
        ListBox::textColourId,                      0xff000000,

        Slider::backgroundColourId,                 0x00000000,
        Slider::thumbColourId,                      textButtonColour,
        Slider::trackColourId,                      0x7fffffff,
        Slider::rotarySliderFillColourId,           0x00000000,
        Slider::rotarySliderOutlineColourId,        0x66000000,
        Slider::textBoxTextColourId,                Colour(Colours::yellowgreen).getARGB(),
        Slider::textBoxBackgroundColourId,          0xff000000,
        Slider::textBoxHighlightColourId,           textHighlightColour,
        Slider::textBoxOutlineColourId,             0xffff3b00,

        ResizableWindow::backgroundColourId,        0xff777777,
        //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)

        AlertWindow::backgroundColourId,            0xff161616,
        AlertWindow::textColourId,                  0xfff0f8ff,
        AlertWindow::outlineColourId,               0xff11ffff,

        ProgressBar::backgroundColourId,            0xffeeeeee,
        ProgressBar::foregroundColourId,            0xffaaaaee,

        TooltipWindow::backgroundColourId,          0xffeeeebb,
        TooltipWindow::textColourId,                0xff000000,
        TooltipWindow::outlineColourId,             0x4c000000,

        TabbedComponent::backgroundColourId,        0x00000000,
        TabbedComponent::outlineColourId,           0xff777777,
        TabbedButtonBar::tabOutlineColourId,        0x80000000,
        TabbedButtonBar::frontOutlineColourId,      0x90000000,

        Toolbar::backgroundColourId,                0xfff6f8f9,
        Toolbar::separatorColourId,                 0x4c000000,
        Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
        Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
        Toolbar::labelTextColourId,                 0xffff3b00,
        Toolbar::editingModeOutlineColourId,        0xffff0000,

        DrawableButton::textColourId,               0xff000000,
        DrawableButton::textColourOnId,             0xff000000,
        DrawableButton::backgroundColourId,         0x00000000,
        DrawableButton::backgroundOnColourId,       0xaabbbbff,

        HyperlinkButton::textColourId,              0xcc1111ee,

        GroupComponent::outlineColourId,            0x66000000,
        GroupComponent::textColourId,               0xff000000,

        BubbleComponent::backgroundColourId,        0xeeeeeebb,
        BubbleComponent::outlineColourId,           0x77000000,

        DirectoryContentsDisplayComponent::highlightColourId,   textHighlightColour,
        DirectoryContentsDisplayComponent::textColourId,        0xff000000,

        0x1000440, /*LassoComponent::lassoFillColourId*/        0x66dddddd,
        0x1000441, /*LassoComponent::lassoOutlineColourId*/     0x99111111,

        0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
        0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
        0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        colours.bg.getARGB(),
        0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     colours.button_on_colour.getARGB(),
        0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          Colours::yellow.getARGB(),
        0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
        0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  colours.bg.getARGB(),
        0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       colours.button_on_colour.getARGB(),
        0x1005008, /*MidiKeyboardComponent::shadowColourId*/                  colours.bg.getARGB(),

        0x1004500, /*CodeEditorComponent::backgroundColourId*/                0xffffffff,
        0x1004502, /*CodeEditorComponent::highlightColourId*/                 textHighlightColour,
        0x1004503, /*CodeEditorComponent::defaultTextColourId*/               0xff000000,
        0x1004504, /*CodeEditorComponent::lineNumberBackgroundId*/            0x44999999,
        0x1004505, /*CodeEditorComponent::lineNumberTextId*/                  0x44000000,

        0x1007000, /*ColourSelector::backgroundColourId*/                     0xff050505,
        0x1007001, /*ColourSelector::labelTextColourId*/                      0xff000000,

        0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          0x00000000,
        0x100ad01, /*KeyMappingEditorComponent::textColourId*/                0xff000000,

        FileSearchPathListComponent::backgroundColourId,        0xffffffff,

        FileChooserDialogBox::titleTextColourId,                0xffff3b00,
    };

    show_values_always = false;
    midi_learn_comp = nullptr;

    for (int i = 0; i < numElementsInArray (standardColours); i += 2)
        setColour ((int) standardColours [i], Colour ((uint32) standardColours [i + 1]));

    defaultFont = Font(Typeface::createSystemTypefaceFor(BinaryData::FjallaOneRegular_otf,BinaryData::FjallaOneRegular_otfSize));
}
UiLookAndFeel::~UiLookAndFeel()
{
    clearSingletonInstance();
}

//==============================================================================
void UiLookAndFeel::drawButtonBackground (Graphics& g,
        Button& button,
        const Colour& backgroundColour,
        bool isMouseOverButton,
        bool isButtonDown)
{
    /*
      g.setImageResamplingQuality( Graphics::highResamplingQuality );

      if( button.isOpaque() )
          g.fillAll (colours.bg);

      int name = button.getName().getIntValue();
      bool is_midi_learn_mode = name >= MIDI_LEARN_NAME_OFFSET;

      // isMouseOverButton
      Colour color_1;
      if( is_midi_learn_mode )
          color_1 =  isButtonDown ? colours.midi_learn.darker (0.4f) : colours.midi_learn.brighter (0.25f);
      else
          color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour).brighter (0.25f);
        //color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour).darker (0.3f);

      const int width = button.getWidth();
      const int height = button.getHeight();

      g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, Colour (backgroundColour).darker (0.3f), 0.0f, height, false));
      //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f, height, false));
      g.fillRoundedRectangle (1, 1, width-2, height-2, 4);

      g.setColour (Colour(0xff333333));
      g.drawRoundedRectangle (1, 1, width-2, height-2, 4, 1.5);
      */
    if( button.isOpaque() )
        g.fillAll (colours.bg);


    const bool is_midi_learn_mode = static_cast< Component* >( &button ) == midi_learn_comp;

    // isMouseOverButton
    Colour color_1;
    if( is_midi_learn_mode )
        color_1 =  isButtonDown ? colours.midi_learn.darker (0.4f) : colours.midi_learn.brighter (0.25f);
    else
        color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour).brighter (0.25f);
    //color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour).darker (0.3f);

    const int width = button.getWidth();
    const int height = button.getHeight();

    Rectangle<int> rect(1, 1, width-2, height-2);
    Rectangle<float> rect_float(1, 1, width-2, height-2);

    if( button.isEnabled() && backgroundColour != colours.button_off_colour )
    {
        Path blur_rect;
        blur_rect.addRoundedRectangle(2, 2, width-4, height-4, 3 );
        DropShadow drop_shadow( color_1, 2, Point<int>(0,0) );
        drop_shadow.drawForPath( g, blur_rect );
    }

    g.setGradientFill (ColourGradient (color_1.darker (0.3f), 0.0f, 0.0f, Colour (backgroundColour), 0.0f, height, false));
    //g.setGradientFill (ColourGradient (color_1, 0.0f, 0.0f, color_1.darker (0.3f), 0.0f, height, false));
    g.fillRoundedRectangle (rect_float, 2);

    {
        if( backgroundColour != colours.button_off_colour )
            g.setColour (color_1.darker (0.6f));
        else
            g.setColour (color_1.darker (4.6f));

        g.drawRoundedRectangle (rect_float, 2, 1);
    }
}

Font UiLookAndFeel::getTextButtonFont (TextButton& button, int buttonHeigh)
{
    return defaultFont; // button.getFont();
}

void UiLookAndFeel::drawButtonText (Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
{
    Font font (getTextButtonFont (button));
    g.setFont (font.withHeight(jmin(int(0.7*button.getHeight()),20)));

    bool is_bg_button = button.findColour( TextButton::buttonColourId ) == colours.button_off_colour;
    Colour color_1;
    if( ! button.isEnabled() )
        color_1 = Colour (0xff414141);
    else if( is_bg_button )
        color_1 = colours.label_text_colour;
    else
        color_1 = colours.button_off_colour;

    const int yIndent = jmin (4, button.proportionOfHeight (0.33f));
    const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt (font.getHeight() * 0.6f);
    const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    /*
        g.setColour(color_1.contrasting(0.1));
        g.drawFittedText (button.getButtonText(),
                          leftIndent,
                          yIndent,
                          button.getWidth() - leftIndent - rightIndent,
                          button.getHeight() - yIndent * 2,
                          Justification::centred, 3, 0.5);
    */
    g.setColour(color_1);
    g.drawFittedText (button.getButtonText(),
                      leftIndent,
                      yIndent,
                      button.getWidth() - leftIndent - rightIndent,
                      button.getHeight() - yIndent * 2,
                      Justification::centred, 3, 0.5);

}

void UiLookAndFeel::drawTickBox (Graphics& g, Component& component,
                                 float x, float y, float w, float h,
                                 const bool ticked,
                                 const bool isEnabled,
                                 const bool isMouseOverButton,
                                 const bool isButtonDown)
{
    if (ticked)
        g.setColour (colours.label_text_colour);
    else
        g.setColour (Colours::black);
    g.fillRect (int(x)-1, int(y)-1, int(w), int(h) );

    g.setColour (Colour (colours.label_text_colour));
    g.drawRect (int(x)-1, int(y)-1, int(w), int(h), 1);
}

void UiLookAndFeel::drawToggleButton (Graphics& g, ToggleButton& button,
                                      bool isMouseOverButton, bool isButtonDown)
{
    if( button.isOpaque() )
        g.fillAll (colours.bg);

    float prop_h = 1.f/25.f*button.getHeight();
    float top = prop_h*4.f;
    float rect_size = button.getHeight()-top*2;
    drawTickBox (g, button,
                 (button.getWidth() - rect_size)/2, top,
                 rect_size, rect_size,
                 button.getToggleState(),
                 button.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);
}

//==============================================================================
AlertWindow* UiLookAndFeel::createAlertWindow (const String& title, const String& message,
        const String& button1, const String& button2, const String& button3,
        AlertWindow::AlertIconType iconType,
        int numButtons, Component* associatedComponent)
{
    AlertWindow* aw = new AlertWindow (title, message, iconType, associatedComponent);

    if (numButtons == 1)
    {
        aw->addButton (button1, 0,
                       KeyPress (KeyPress::escapeKey),
                       KeyPress (KeyPress::returnKey));
    }
    else
    {
        const KeyPress button1ShortCut ((int) CharacterFunctions::toLowerCase (button1[0]), 0, 0);
        KeyPress button2ShortCut ((int) CharacterFunctions::toLowerCase (button2[0]), 0, 0);
        if (button1ShortCut == button2ShortCut)
            button2ShortCut = KeyPress();

        if (numButtons == 2)
        {
            aw->addButton (button1, 1, KeyPress (KeyPress::returnKey), button1ShortCut);
            aw->addButton (button2, 0, KeyPress (KeyPress::escapeKey), button2ShortCut);
        }
        else if (numButtons == 3)
        {
            aw->addButton (button1, 1, button1ShortCut);
            aw->addButton (button2, 2, button2ShortCut);
            aw->addButton (button3, 0, KeyPress (KeyPress::escapeKey));
        }
    }

    return aw;
}

void UiLookAndFeel::drawAlertBox (Graphics& g, AlertWindow& alert,
                                  const Rectangle<int>& textArea, TextLayout& textLayout)
{
    g.fillAll (alert.findColour (AlertWindow::backgroundColourId));

    int iconSpaceUsed = 0;

    const int iconWidth = 50;
    int iconSize = jmin (iconWidth + 50, alert.getHeight() + 20);

    if (alert.containsAnyExtraComponents() || alert.getNumButtons() > 2)
        iconSize = jmin (iconSize, textArea.getHeight() + 50);

    const Rectangle<int> iconRect (iconSize / -10, iconSize / -10,
                                   iconSize, iconSize);

    if (alert.getAlertType() != AlertWindow::NoIcon)
    {
        Path icon;
        uint32 colour;
        char character;

        if (alert.getAlertType() == AlertWindow::WarningIcon)
        {
            colour = 0x55ff5555;
            character = '!';

            icon.addTriangle (iconRect.getX() + iconRect.getWidth() * 0.5f, (float) iconRect.getY(),
                              (float) iconRect.getRight(), (float) iconRect.getBottom(),
                              (float) iconRect.getX(), (float) iconRect.getBottom());

            icon = icon.createPathWithRoundedCorners (5.0f);
        }
        else
        {
            colour    = alert.getAlertType() == AlertWindow::InfoIcon ? (uint32) 0x605555ff : (uint32) 0x40b69900;
            character = alert.getAlertType() == AlertWindow::InfoIcon ? 'i' : '?';

            icon.addEllipse ((float) iconRect.getX(), (float) iconRect.getY(),
                             (float) iconRect.getWidth(), (float) iconRect.getHeight());
        }

        GlyphArrangement ga;
        ga.addFittedText (Font (iconRect.getHeight() * 0.9f, Font::bold),
                          String::charToString ((juce_wchar) (uint8) character),
                          (float) iconRect.getX(), (float) iconRect.getY(),
                          (float) iconRect.getWidth(), (float) iconRect.getHeight(),
                          Justification::centred, false);
        ga.createPath (icon);

        icon.setUsingNonZeroWinding (false);
        g.setColour (Colour (colour));
        g.fillPath (icon);

        iconSpaceUsed = iconWidth;
    }

    g.setColour (alert.findColour (AlertWindow::textColourId));

    textLayout.draw (g, Rectangle<int> (textArea.getX() + iconSpaceUsed,
                                        textArea.getY(),
                                        textArea.getWidth() - iconSpaceUsed,
                                        textArea.getHeight()).toFloat());

    g.setColour (alert.findColour (AlertWindow::outlineColourId));
    g.drawRect (0, 0, alert.getWidth(), alert.getHeight(),2);
}

int UiLookAndFeel::getAlertBoxWindowFlags()
{
    return ComponentPeer::windowAppearsOnTaskbar | ComponentPeer::windowHasDropShadow;
}

int UiLookAndFeel::getAlertWindowButtonHeight()
{
    return int( 1.f/ 900 * Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight() * 30 );
}

Font UiLookAndFeel::getAlertWindowMessageFont()
{
    return defaultFont.withHeight( 1.f/ 900 * Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight() * 20 );
}

Font UiLookAndFeel::getAlertWindowFont()
{
    return defaultFont.withHeight( ( 1.f/ 900 * Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight() * 17 ) );
}

//==============================================================================
Font UiLookAndFeel::getPopupMenuFont()
{
    return defaultFont.withHeight(18.0f);
}

void UiLookAndFeel::getIdealPopupMenuItemSize (const String& text, const bool isSeparator,
        int standardMenuItemHeight, int& idealWidth, int& idealHeight)
{
    if (isSeparator)
    {
        idealWidth = 50;
        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 2 : 10;
    }
    else
    {
        Font font (getPopupMenuFont());

        if (standardMenuItemHeight > 0 && font.getHeight() > standardMenuItemHeight / 1.3f)
            font.setHeight (standardMenuItemHeight / 1.3f);

        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight : roundToInt (font.getHeight() * 1.3f);
        idealWidth = font.getStringWidth (text) + idealHeight * 2;
    }
}

void UiLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    const Colour background (findColour (PopupMenu::backgroundColourId));

    g.fillAll (background);
    g.setColour (background.overlaidWith (Colour (0x2badd8e6)));

    for (int i = 0; i < height; i += 3)
        g.fillRect (0, i, width, 1);

#if ! JUCE_MAC
    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
#endif
}

void UiLookAndFeel::drawPopupMenuUpDownArrow (Graphics& g, int width, int height, bool isScrollUpArrow)
{
    const Colour background (findColour (PopupMenu::backgroundColourId));

    g.setGradientFill (ColourGradient (background, 0.0f, height * 0.5f,
                                       background.withAlpha (0.0f),
                                       0.0f, isScrollUpArrow ? ((float) height) : 0.0f,
                                       false));

    g.fillRect (1, 1, width - 2, height - 2);

    const float hw = width * 0.5f;
    const float arrowW = height * 0.3f;
    const float y1 = height * (isScrollUpArrow ? 0.6f : 0.3f);
    const float y2 = height * (isScrollUpArrow ? 0.3f : 0.6f);

    Path p;
    p.addTriangle (hw - arrowW, y1,
                   hw + arrowW, y1,
                   hw, y2);

    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.5f));
    g.fillPath (p);
}

void UiLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                       const bool isSeparator, const bool isActive,
                                       const bool isHighlighted, const bool isTicked,
                                       const bool hasSubMenu, const String& text,
                                       const String& shortcutKeyText,
                                       const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        Rectangle<int> r (area.reduced (5, 0));
        r.removeFromTop (r.getHeight() / 2 - 1);

        g.setColour (Colour (0x33000000));
        g.fillRect (r.removeFromTop (1));

        g.setColour (Colour (0x66ffffff));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        Colour textColour (findColour (PopupMenu::textColourId));

        if (textColourToUse != nullptr)
            textColour = *textColourToUse;

        Rectangle<int> r (area.reduced (1));

        if (isHighlighted)
        {
            g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
            g.fillRect (r);

            g.setColour (findColour (PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour);
        }

        if (! isActive)
            g.setOpacity (0.3f);

        Font font (getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        Rectangle<float> iconArea (r.removeFromLeft ((r.getHeight() * 5) / 4).reduced (3).toFloat());

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked)
        {
            const Path tick (getTickShape (1.0f));
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea, true));
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float) r.removeFromRight ((int) arrowH).getX();
            const float halfH = (float) r.getCentreY();

            Path p;
            p.addTriangle (x, halfH - arrowH * 0.5f,
                           x, halfH + arrowH * 0.5f,
                           x + arrowH * 0.6f, halfH);

            g.fillPath (p);
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            Font f2 (font);
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

//==============================================================================
void UiLookAndFeel::fillTextEditorBackground (Graphics& g, int /*width*/, int /*height*/, TextEditor& textEditor)
{
    textEditor.setColour (TextEditor::textColourId, colours.bg_lines);
    g.fillAll (textEditor.findColour (TextEditor::backgroundColourId));
}

void UiLookAndFeel::drawTextEditorOutline (Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (textEditor.isEnabled())
    {
        if (textEditor.hasKeyboardFocus (true) && ! textEditor.isReadOnly())
        {
            const int border = 2;

            g.setColour (textEditor.findColour (TextEditor::focusedOutlineColourId));
            g.drawRect (0, 0, width, height, border);

            g.setOpacity (1.0f);
            const Colour shadowColour (textEditor.findColour (TextEditor::shadowColourId).withMultipliedAlpha (0.75f));
            drawBevel (g, 0, 0, width, height + 2, border + 2, shadowColour, shadowColour);
        }
        else
        {
            g.setColour (textEditor.findColour (TextEditor::outlineColourId));
            g.drawRect (0, 0, width, height);

            g.setOpacity (1.0f);
            const Colour shadowColour (textEditor.findColour (TextEditor::shadowColourId));
            drawBevel (g, 0, 0, width, height + 2, 3, shadowColour, shadowColour);
        }
    }
}

CaretComponent* UiLookAndFeel::createCaretComponent (Component* keyFocusOwner)
{
    return new CaretComponent (keyFocusOwner);
}

//==============================================================================
void UiLookAndFeel::drawComboBox (Graphics& g, int width, int height, const bool isButtonDown,
                                  int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{
    if( box.isOpaque() )
        g.fillAll (colours.bg);

    const float outlineThickness =1; // box.isEnabled() ? (isButtonDown ? 1.2f : 0.8f) : 0.3f;

    g.setColour (Colour(0xff000000));
    g.fillRoundedRectangle (1, 1, width-2, height-2, 4);

    drawGlassLozenge (g,
                      buttonX + outlineThickness, buttonY + outlineThickness,
                      buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f,
                      Colour(0xff333333), outlineThickness, -1.0f,
                      true, true, true, true);

    if (box.isEnabled() && box.hasKeyboardFocus (false))
    {
        g.setColour (box.findColour (ComboBox::buttonColourId));
        g.drawRoundedRectangle (1, 1, width-2, height-2, 4, 1.5);
    }
    else
    {
        g.setColour (Colour(0xff333333));
        g.drawRoundedRectangle (1, 1, width-2, height-2, 4, 1.5);
    }

    if (box.isEnabled())
    {
        const float arrowX = 0.3f;
        const float arrowH = 0.2f;

        Path p;
        p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f - arrowH),
                       buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
                       buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);

        p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
                       buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
                       buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);

        g.setColour (Colours::yellow);
        g.fillPath (p);
    }
}

Font UiLookAndFeel::getComboBoxFont (ComboBox& box)
{
    return defaultFont.withHeight (jmin (15.0f, box.getHeight() * 0.85f));
}

Label* UiLookAndFeel::createComboBoxTextBox (ComboBox&)
{
    return new Label (String::empty, String::empty);
}

void UiLookAndFeel::positionComboBoxText (ComboBox& box, Label& label)
{
    label.setBounds (1, 1,
                     box.getWidth() + 3 - box.getHeight(),
                     box.getHeight() - 2);

    label.setFont (getComboBoxFont (box));
}

//==============================================================================
Font UiLookAndFeel::getLabelFont (Label& label)
{
    return defaultFont;
}

void UiLookAndFeel::drawLabel (Graphics& g, Label& label)
{
    if( label.isOpaque() )
        g.fillAll (colours.bg);

    if (! label.isBeingEdited())
    {
        String labelStyle = label.GET_LABEL_STYLE();

        const Font font (getLabelFont (label));

        Colour col = colours.label_text_colour;
        if( labelStyle ==  IS_VALUE_LABEL )
            col = colours.slider_track_colour;
        else if( labelStyle == IS_SECOND_VALUE_LABEL )
        {
            col = colours.slider_track_colour_2;
        }
        else if( labelStyle == IS_DESCRIPTION_LABEL )
        {
            //   col = Colour(0xffff3b00);
        }

        Rectangle<int> textArea (label.getBorderSize().subtractedFrom (label.getLocalBounds()));
        g.setColour (col);
        Path text_path;
        GlyphArrangement glyphs;
        glyphs.addFittedText( font.withHeight(0.6f*textArea.getHeight()),
                              label.getText(),
                              textArea.getX(),textArea.getY(),textArea.getWidth(),textArea.getHeight(),
                              label.getJustificationType(), 1, 0.5f);
        glyphs.createPath(text_path);

        if( labelStyle == IS_VALUE_LABEL ) {
            DropShadow drop_shadow( col, 1, Point<int>(0,0) );
            drop_shadow.drawForPath( g, text_path );
        }

        g.fillPath(text_path);
    }
}

//==============================================================================
void UiLookAndFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle /*style*/, Slider& slider)
{
    const bool is_midi_learn_mode = static_cast< Component* >( &slider ) == midi_learn_comp;

    Colour col = colours.slider_track_colour;
    const bool is_enabled = slider.isEnabled();
    int slider_type = slider.GET_SLIDER_STYLE();
    if( is_midi_learn_mode )
    {
        col = colours.midi_learn;
    }
    else
    {
        if( slider_type == VALUE_SLIDER )
            col = colours.slider_track_colour; //.brighter (0.4f);
        else if( slider_type == VALUE_SLIDER_2 )
            col = colours.slider_track_colour_2; //.brighter (0.4f);
        else
            col = colours.label_text_colour; //.brighter (0.4f);
    }
    if( ! is_enabled )
        col = Colour(0xff777777);

    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    Path indent;
    if (slider.isHorizontal())
    {

        const float iy = y + height * 0.5f - sliderRadius * 0.5f;
        const float ih = sliderRadius;

        g.setColour (Colour (0xff222222) ) ;
        indent.addRoundedRectangle (x - sliderRadius, iy,
                                    width + sliderRadius*2, ih,
                                    5.0f);

        g.fillPath (indent);

        //g.setColour (Colour (colours.bg_lines));
        g.setColour (Colour(0xff777777).darker(7.8));
        g.strokePath (indent, PathStrokeType (1.0f));
    }
    else
    {
        const float ix = x + width * 0.5f - sliderRadius * 0.5f;
        const float iw = sliderRadius;

        {
            g.setColour (col) ;
            indent.addRoundedRectangle (ix, y - sliderRadius + sliderPos,
                                        iw, height + sliderRadius*2 - sliderPos,
                                        5.0f);

            if( is_enabled )
            {
                DropShadow drop_shadow( col, 1, Point<int>(0,0) );
                drop_shadow.drawForPath( g, indent );
            }

            g.fillPath (indent);
            g.setColour (col.darker(6.6f).withAlpha(0.4f));
            g.strokePath (indent, PathStrokeType (1.0f));
        }

        indent.clear();
        if( slider.isOpaque() )
        {
            g.setColour (Colour (0xff222222) ) ;
            indent.addRoundedRectangle (ix, y - sliderRadius,
                                        iw, sliderPos,
                                        5.0f);
            g.fillPath (indent);

            //g.setColour (Colour (colours.bg_lines));
            g.setColour (Colour(0xff777777).darker(7.8));
            g.strokePath (indent, PathStrokeType (1.0f));
        }
    }
}

void UiLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
    float kx, ky;

    if (style == Slider::LinearVertical)
    {
        kx = x + width * 0.5f;
        ky = sliderPos;
    }
    else
    {
        kx = sliderPos;
        ky = y + height * 0.5f;
    }

    const bool is_midi_learn_mode = static_cast< Component* >( &slider ) == midi_learn_comp;
    Colour col = colours.slider_track_colour;
    const bool is_enabled = slider.isEnabled();
    int slider_type = slider.GET_SLIDER_STYLE();
    if( is_midi_learn_mode )
    {
        col = colours.midi_learn;
    }
    else
    {
        if( slider_type == VALUE_SLIDER )
            col = colours.slider_track_colour; //.brighter (0.4f);
        else if( slider_type == VALUE_SLIDER_2 )
            col = colours.slider_track_colour_2; //.brighter (0.4f);
        else
            col = colours.label_text_colour; //.brighter (0.4f);
    }
    if( ! is_enabled )
        col = Colour(0xff777777);

    drawGlassSphere (g,
                     kx - sliderRadius,
                     ky - sliderRadius,
                     sliderRadius * 2.0f,
                     col, outlineThickness);
}

void UiLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                      float sliderPos, float minSliderPos, float maxSliderPos,
                                      const Slider::SliderStyle style, Slider& slider)
{
    if( slider.isOpaque() )
        g.fillAll( colours.bg );

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

        Colour baseColour (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId)
                           .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f),
                           false, isMouseOver,
                           isMouseOver || slider.isMouseButtonDown()));

        drawShinyButtonShape (g,
                              (float) x,
                              style == Slider::LinearBarVertical ? sliderPos
                              : (float) y,
                              style == Slider::LinearBarVertical ? (float) width
                              : (sliderPos - x),
                              style == Slider::LinearBarVertical ? (height - sliderPos)
                              : (float) height, 0.0f,
                              baseColour,
                              slider.isEnabled() ? 0.9f : 0.3f,
                              true, true, true, true);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

        if( ! slider.isOpaque() || slider.GET_SLIDER_STYLE() == VALUE_SLIDER )
            drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

int UiLookAndFeel::getSliderThumbRadius (Slider& slider)
{
    return jmin (5,
                 slider.getHeight() / 2,
                 slider.getWidth() / 2) + 2;
}

void UiLookAndFeel::drawRotarySlider (Graphics& g,
                                      int x, int y, int width, int height,
                                      float sliderPos,
                                      const float rotaryStartAngle, const float rotaryEndAngle,
                                      Slider& slider)
{
    if( slider.isOpaque() )
        g.fillAll (colours.bg);

    float slider_value = slider.getValue();
    float rotaryStartAngle_ = rotaryStartAngle;
    float rotaryEndAngle_ = rotaryEndAngle;

    int slider_type = slider.GET_SLIDER_STYLE();

    const bool is_midi_learn_mode = static_cast< Component* >( &slider ) == midi_learn_comp;

    if( slider.getMinimum() < 0 )
    {
        if( slider_type == MODULATION_SLIDER )
        {
            slider_value = slider_value/100;
            rotaryEndAngle_ = float_Pi*0.8f;
            rotaryStartAngle_ = 0;
            sliderPos = slider_value * rotaryEndAngle_/2.5f;
        }
        else
        {
            rotaryStartAngle_ = 0;
            rotaryEndAngle_ = float_Pi*0.8f;
            sliderPos = (1.0f/slider.getMaximum()*slider_value) * rotaryEndAngle_/2.5f;
        }
    }

    const float radius = jmin (width * 0.5f, height * 0.5f) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    float angle = rotaryStartAngle_ + sliderPos * (rotaryEndAngle_ - rotaryStartAngle_);

    Colour SliderCol;
    if( is_midi_learn_mode )
    {
        SliderCol = colours.midi_learn;
    }
    else
    {
        if( slider_type == VALUE_SLIDER )
            SliderCol = colours.slider_track_colour; //.brighter (0.4f);
        else if( slider_type == VALUE_SLIDER_2 )
            SliderCol = colours.slider_track_colour_2; //.brighter (0.4f);
        else
            SliderCol = colours.slider_track_colour_modulation; //.brighter (0.4f);
    }

#define THICKNESS 0.85f /* (1.f/40*slider.getWidth()); // 0.7f; */

    if ( ! slider.isEnabled())
        SliderCol = Colour(0xff444444);
    {

        Path filledArc;
        if( slider.isOpaque() )
        {
            filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, THICKNESS);
            //g.setColour(Colours::black.withAlpha(0.2f));
            //g.setColour (SliderCol.darker (5.f).withAlpha(0.5f));
            g.setColour (Colour (0xff222222) ) ; //.interpolatedWith(SliderCol.darker(0.8),0.2));
            g.fillPath (filledArc);
            g.setColour(Colour(0xff444444).darker(7.8));
            g.strokePath(filledArc,PathStrokeType(1.f));
        }

        g.setColour (SliderCol);
        filledArc.clear();
        filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle_, angle, THICKNESS);

        //g.fillPath (filledArc);
        //if( slider.isEnabled() )
        {
            {
                DropShadow drop_shadow( SliderCol, 1, Point<int>(0,0) );
                drop_shadow.drawForPath( g, filledArc );
            }
            {
                g.setColour (SliderCol.darker(6.6f).withAlpha(0.4f));
                g.strokePath (filledArc, PathStrokeType (1.0f));
            }
        }


        {
            g.setColour (SliderCol);
            g.fillPath (filledArc);
        }
    }

    // DRAW LABEL, WAVE OR CENTER TRIANGLE
    {
        const int slider_label_style = slider.GET_SLIDER_LABEL_STYLE();
        if( slider_label_style == SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX )
        {
            String value_to_paint = slider.GET_VALUE_TO_PAINT();
            const String suffix = value_to_paint.fromFirstOccurrenceOf("@",false,true);
            value_to_paint = value_to_paint.upToFirstOccurrenceOf("@",false,true);

            const int label_x_ident = width/3.5;
            const int label_y_ident = height/4;
            const int label_h = height/2;
            const int label_w = width-label_x_ident*2;

            if( suffix == "wav" )
            {
                float value_as_float = value_to_paint.getFloatValue();

                Path wave_path;
                if( value_as_float <= 1 )
                {
                    float square_weight = value_as_float;
                    for( int i = 0 ; i != label_w*4 ; ++i )
                    {
                        float value_sin = std::sin( (1.0f/float(label_w*4)*i )*(float_Pi*2) );
                        float value_square;
                        if( i < 1 )
                        {
                            value_square = 0;
                        }
                        else if( i == label_w*4-1 )
                        {
                            value_square = 0;
                        }
                        else
                            value_square = value_sin >= 0 ? 1 : -1;

                        float mix = value_sin*(1.0f-square_weight) + value_square*square_weight;

                        float x = label_x_ident + float(i)/4;
                        float y = label_h - float(label_h)*mix/4;

                        if( i == 0 )
                            wave_path.startNewSubPath( x, y );
                        else
                            wave_path.lineTo( x, y );
                    }
                }
                else if( value_as_float <= 2 )
                {
                    float saw_weight = value_as_float - 1;
                    for( int i = 0 ; i != label_w*4 ; ++i )
                    {
                        float value_sin = std::sin( (1.0f/float(label_w*4)*i )*(float_Pi*2) );
                        float value_square;
                        if( i < 1 )
                        {
                            value_square = 0;
                        }
                        else if( i == label_w*4-1 )
                        {
                            value_square = 0;
                        }
                        else
                            value_square = value_sin >= 0 ? 1 : -1;

                        float value_saw;
                        if( i < 1 )
                        {
                            value_saw = 0;
                        }
                        else if( i == label_w*4-1 )
                        {
                            value_saw = 0;
                        }
                        else
                            value_saw = ((1.0f/(label_w*4)*i)*2 - 1) * -1;

                        float mix = value_square*(1.0f-saw_weight) + value_saw*saw_weight;

                        float x = label_x_ident + float(i)/4;
                        float y = label_h - float(label_h)*mix/4;

                        if( i == 0 )
                            wave_path.startNewSubPath( x, y );
                        else
                            wave_path.lineTo( x, y );
                    }
                }
                else /* RAND */
                {
                    float rand_weight = value_as_float - 2;
                    for( int i = 0 ; i != label_w*4 ; ++i )
                    {
                        float value_saw;
                        if( i < 1 )
                        {
                            value_saw = 0;
                        }
                        else if( i == label_w*4-1 )
                        {
                            value_saw = 0;
                        }
                        else
                            value_saw = ((1.0f/(label_w*4)*i)*2 - 1) * -1;

                        float mix = value_saw*(1.0f-rand_weight) + ((Random::getSystemRandom().nextFloat()-0.5)*2) *rand_weight;

                        float x = label_x_ident + float(i)/4;
                        float y = label_h - float(label_h)*mix/4;

                        if( i == 0 )
                            wave_path.startNewSubPath( x, y );
                        else
                            wave_path.lineTo( x, y );
                    }
                }

                g.setColour( SliderCol );
                g.strokePath(wave_path,PathStrokeType(1.f));
                g.setColour( SliderCol.withAlpha(0.6f) );
                g.strokePath(wave_path,PathStrokeType(3.0f));
            }
            // DRAW VALUE
            else
            {
                Path text_path;
                float font_height_factor = 0.5f;
                if( value_to_paint == "0" )
                    font_height_factor = 0.35f;

                GlyphArrangement glyphs;
                glyphs.addFittedText( defaultFont.withHeight(font_height_factor*height),
                                      value_to_paint,
                                      label_x_ident, label_y_ident, label_w, label_h,
                                      Justification::centred, 1, 0.5f);
                glyphs.createPath(text_path);
                DropShadow drop_shadow( SliderCol, 1, Point<int>(0,0) );
                drop_shadow.drawForPath( g, text_path );
                g.fillPath(text_path);

                // DRAW SUFFIX
                if( suffix != "" && value_to_paint != "0" )
                {
                    // SPECIAL FOR SMALL #
                    float font_height = 0.3;
                    if( suffix == "#" )
                        font_height += 0.1;

                    int left_right_ident = width/2.5;
                    text_path.clear();
                    glyphs.clear();
                    glyphs.addFittedText( defaultFont.withHeight(font_height*height),
                                          suffix,
                                          left_right_ident, height - height/4, width -left_right_ident*2, height/4,
                                          Justification::centred, 1, 0.5f);
                    glyphs.createPath(text_path);
                    DropShadow drop_shadow( SliderCol, 1, Point<int>(0,0) );
                    drop_shadow.drawForPath( g, text_path );
                    g.fillPath(text_path);
                }
            }
        }
        // NO VALUE
        else if( slider_label_style == SLIDER_LABEL_STYLES::JUST_HIDE_CENTER )
        {

        }
        else
        {
            const float innerRadius = radius * 0.2f;
            Path p;
            p.addTriangle (-innerRadius, 0.0f,
                           0.0f, -radius * THICKNESS * 0.7f,
                           innerRadius, 0.0f);

            p.addEllipse (-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
            p.applyTransform( AffineTransform::rotation (angle).translated (centreX, centreY) );
            if( slider.isEnabled() )
            {
                DropShadow drop_shadow( SliderCol.darker(), 1, Point<int>(0,0) );
                drop_shadow.drawForPath( g, p );
            }
            {
                g.setColour (SliderCol);
                g.fillPath (p);
            }
        }
    }
}

Font UiLookAndFeel::getSliderPopupFont (Slider&)
{
    return defaultFont.withHeight (17.0f);
}
int UiLookAndFeel::getSliderPopupPlacement (Slider&)
{
    return BubbleComponent::above | BubbleComponent::below | BubbleComponent::left | BubbleComponent::right;
}

//==============================================================================
void UiLookAndFeel::getTooltipSize (const String& tipText, int& width, int& height)
{
    const TextLayout tl (LookAndFeelHelpers::layoutTooltipText (tipText, Colours::black));

    width  = (int) (tl.getWidth() + 14.0f);
    height = (int) (tl.getHeight() + 6.0f);
}

void UiLookAndFeel::drawTooltip (Graphics& g, const String& text, int width, int height)
{
    g.fillAll (findColour (TooltipWindow::backgroundColourId));

#if ! JUCE_MAC // The mac windows already have a non-optional 1 pix outline, so don't double it here..
    g.setColour (findColour (TooltipWindow::outlineColourId));
    g.drawRect (0, 0, width, height, 1);
#endif

    LookAndFeelHelpers::layoutTooltipText (text, findColour (TooltipWindow::textColourId))
    .draw (g, Rectangle<float> ((float) width, (float) height));
}

//==============================================================================
void UiLookAndFeel::drawCornerResizer (Graphics& g,
                                       int w, int h,
                                       bool /*isMouseOver*/,
                                       bool /*isMouseDragging*/)
{
    const float lineThickness = jmin (w, h) * 0.075f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.setColour (colours.bg_lines);
        g.drawLine (w * i + lineThickness,
                    h + 1.0f,
                    w + 1.0f,
                    h * i + lineThickness,
                    lineThickness);
    }
}

//==============================================================================
void UiLookAndFeel::fillResizableWindowBackground (Graphics& g, int /*w*/, int /*h*/, const BorderSize<int>& /*border*/, ResizableWindow& window) {}

void UiLookAndFeel::drawResizableWindowBorder (Graphics& g, int /*w*/, int /*h*/, const BorderSize<int>& /*border*/, ResizableWindow&)
{
    //g.fillAll(Colours::black);
}

void UiLookAndFeel::drawDocumentWindowTitleBar (DocumentWindow& window, Graphics& g,
        int w, int h, int titleSpaceX, int titleSpaceW,
        const Image* icon, bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();

    g.fillAll(window.getBackgroundColour().contrasting (isActive ? 0.0f : 0.25f));

    Font font = defaultFont.withHeight(h * 0.65f);
    g.setFont (font);

    int textW = font.getStringWidth (window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr)
    {
        iconH = (int) font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = jmin (titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX
                : jmax (titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;

    if (icon != nullptr)
    {
        g.setOpacity (isActive ? 1.0f : 0.6f);
        g.drawImageWithin (*icon, textX, (h - iconH) / 2, iconW, iconH,
                           RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }

    if (window.isColourSpecified (DocumentWindow::textColourId) || isColourSpecified (DocumentWindow::textColourId))
        g.setColour (window.findColour (DocumentWindow::textColourId));
    else
        g.setColour (window.getBackgroundColour().contrasting (isActive ? 0.7f : 0.4f));

    g.drawText (window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}

//==============================================================================
class UiLookAndFeel::GlassWindowButton   : public Button
{
public:
    GlassWindowButton (const String& name, Colour col,
                       const Path& normalShape_,
                       const Path& toggledShape_) noexcept
:
    Button (name),
           colour (col),
           normalShape (normalShape_),
           toggledShape (toggledShape_)
    {
    }

    //==============================================================================
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {
        float alpha = isMouseOverButton ? (isButtonDown ? 1.0f : 0.8f) : 0.55f;

        if (! isEnabled())
            alpha *= 0.5f;

        float x = 0, y = 0, diam;

        if (getWidth() < getHeight())
        {
            diam = (float) getWidth()-4;
            y = (getHeight() - getWidth()) * 0.5f;
        }
        else
        {
            diam = (float) getHeight()-4;
            y = (getWidth() - getHeight()) * 0.5f;
        }

        x += diam * 0.05f +2;
        y += diam * 0.05f +2;
        diam *= 0.9f;

        g.setColour (Colour::greyLevel (0.6f).withAlpha (alpha)); // , 0, y + diam, Colour::greyLevel (0.6f).withAlpha (alpha), 0, y, false
        g.fillEllipse (x, y, diam, diam);

        x += 2.0f;
        y += 2.0f;
        diam -= 4.0f;

        UiLookAndFeel::drawGlassSphere (g, x, y, diam, colour, 1.0f);


        Path& p = getToggleState() ? toggledShape : normalShape;

        const AffineTransform t (p.getTransformToScaleToFit (x + diam * 0.3f, y + diam * 0.3f,
                                 diam * 0.4f, diam * 0.4f, true));

        g.setColour (Colours::black.withAlpha (alpha * 0.6f));
        g.fillPath (p, t);
    }

private:
    Colour colour;
    Path normalShape, toggledShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlassWindowButton)
};

Button* UiLookAndFeel::createDocumentWindowButton (int buttonType)
{
    Path shape;
    const float crossThickness = 0.25f;

    if (buttonType == DocumentWindow::closeButton)
    {
        shape.addLineSegment (Line<float> (0.0f, 0.0f, 1.0f, 1.0f), crossThickness * 1.4f);
        shape.addLineSegment (Line<float> (1.0f, 0.0f, 0.0f, 1.0f), crossThickness * 1.4f);

        return new GlassWindowButton ("close", Colour (0xff991100), shape, shape);
    }

    if (buttonType == DocumentWindow::minimiseButton)
    {
        shape.addLineSegment (Line<float> (0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        return new GlassWindowButton ("minimise", Colour (0xffdfce89), shape, shape);
    }

    if (buttonType == DocumentWindow::maximiseButton)
    {
        shape.addLineSegment (Line<float> (0.5f, 0.0f, 0.5f, 1.0f), crossThickness);
        shape.addLineSegment (Line<float> (0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        Path fullscreenShape;
        fullscreenShape.startNewSubPath (45.0f, 100.0f);
        fullscreenShape.lineTo (0.0f, 100.0f);
        fullscreenShape.lineTo (0.0f, 0.0f);
        fullscreenShape.lineTo (100.0f, 0.0f);
        fullscreenShape.lineTo (100.0f, 45.0f);
        fullscreenShape.addRectangle (45.0f, 45.0f, 100.0f, 100.0f);
        PathStrokeType (30.0f).createStrokedPath (fullscreenShape, fullscreenShape);

        return new GlassWindowButton ("maximise", Colour (0xffdfce89), shape, fullscreenShape);
    }

    jassertfalse;
    return nullptr;
}

void UiLookAndFeel::positionDocumentWindowButtons (DocumentWindow&,
        int titleBarX, int titleBarY,
        int titleBarW, int titleBarH,
        Button* minimiseButton,
        Button* maximiseButton,
        Button* closeButton,
        bool positionTitleBarButtonsOnLeft)
{
    const int buttonW = titleBarH - titleBarH / 8;

    int x = positionTitleBarButtonsOnLeft ? titleBarX + 4
            : titleBarX + titleBarW - buttonW - buttonW / 4;

    if (closeButton != nullptr)
    {
        closeButton->setBounds (x, titleBarY, buttonW, titleBarH);
        x += positionTitleBarButtonsOnLeft ? buttonW : -(buttonW + buttonW / 4);
    }

    if (positionTitleBarButtonsOnLeft)
        std::swap (minimiseButton, maximiseButton);

    if (maximiseButton != nullptr)
    {
        maximiseButton->setBounds (x, titleBarY, buttonW, titleBarH);
        x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
    }

    if (minimiseButton != nullptr)
        minimiseButton->setBounds (x, titleBarY, buttonW, titleBarH);
}

//==============================================================================
void UiLookAndFeel::drawShinyButtonShape (Graphics& g,
        float x, float y, float w, float h,
        float maxCornerSize,
        const Colour& baseColour,
        const float strokeWidth,
        const bool flatOnLeft,
        const bool flatOnRight,
        const bool flatOnTop,
        const bool flatOnBottom) noexcept
{
    if (w <= strokeWidth * 1.1f || h <= strokeWidth * 1.1f)
        return;

    const float cs = jmin (maxCornerSize, w * 0.5f, h * 0.5f);

    Path outline;
    outline.addRoundedRectangle (x, y, w, h, cs, cs,
    ! (flatOnLeft  || flatOnTop),
    ! (flatOnRight || flatOnTop),
    ! (flatOnLeft  || flatOnBottom),
    ! (flatOnRight || flatOnBottom));

    ColourGradient cg (baseColour, 0.0f, y,
    baseColour.overlaidWith (Colour (0x070000ff)), 0.0f, y + h,
    false);

    cg.addColour (0.5,  baseColour.overlaidWith (Colour (0x33ffffff)));
    cg.addColour (0.51, baseColour.overlaidWith (Colour (0x110000ff)));

    g.setGradientFill (cg);
    g.fillPath (outline);

    g.setColour (Colour (0x80000000));
    g.strokePath (outline, PathStrokeType (strokeWidth));
}

//==============================================================================
void UiLookAndFeel::drawGlassSphere (Graphics& g, const float x, const float y,
                                     const float diameter, const Colour& colour,
                                     const float outlineThickness) noexcept
{
    if (diameter <= outlineThickness)
        return;

    Path p;
    p.addEllipse (x, y, diameter, diameter);

    {
        g.setColour (colour);
        g.fillPath (p);
    }

    // g.setGradientFill (ColourGradient (Colours::white, 0, y + diameter * 0.06f,
    // Colours::transparentWhite, 0, y + diameter * 0.3f, false));
    // g.fillEllipse (x + diameter * 0.2f, y + diameter * 0.05f, diameter * 0.6f, diameter * 0.4f);

    ColourGradient cg (Colours::transparentBlack,
    x + diameter * 0.5f, y + diameter * 0.5f,
    Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
    x, y + diameter * 0.5f, true);

    //cg.addColour (0.7, Colours::transparentBlack);
    //cg.addColour (0.8, Colours::black.withAlpha (0.1f * outlineThickness));

    //g.setGradientFill (cg);
    // g.fillPath (p);

    g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.drawEllipse (x, y, diameter, diameter, outlineThickness);
}

//==============================================================================
void UiLookAndFeel::drawGlassPointer (Graphics& g,
                                      const float x, const float y, const float diameter,
                                      const Colour& colour, const float outlineThickness,
                                      const int direction) noexcept
{
    if (diameter <= outlineThickness)
        return;

    Path p;
    p.startNewSubPath (x + diameter * 0.5f, y);
    p.lineTo (x + diameter, y + diameter * 0.6f);
    p.lineTo (x + diameter, y + diameter);
    p.lineTo (x, y + diameter);
    p.lineTo (x, y + diameter * 0.6f);
    p.closeSubPath();

    p.applyTransform (AffineTransform::rotation (direction * (float_Pi * 0.5f), x + diameter * 0.5f, y + diameter * 0.5f));

    {
        ColourGradient cg (Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y,
        Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y + diameter, false);

        cg.addColour (0.4, Colours::white.overlaidWith (colour));

        g.setGradientFill (cg);
        g.fillPath (p);
    }

    ColourGradient cg (Colours::transparentBlack,
    x + diameter * 0.5f, y + diameter * 0.5f,
    Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
    x - diameter * 0.2f, y + diameter * 0.5f, true);

    cg.addColour (0.5, Colours::transparentBlack);
    cg.addColour (0.7, Colours::black.withAlpha (0.07f * outlineThickness));

    g.setGradientFill (cg);
    g.fillPath (p);

    g.setColour (Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.strokePath (p, PathStrokeType (outlineThickness));
}

//==============================================================================
void UiLookAndFeel::drawGlassLozenge (Graphics& g,
                                      const float x, const float y, const float width, const float height,
                                      const Colour& colour, const float outlineThickness, const float cornerSize,
                                      const bool flatOnLeft,
                                      const bool flatOnRight,
                                      const bool flatOnTop,
                                      const bool flatOnBottom) noexcept
{
    Path outline;
    outline.addRoundedRectangle (x, y, width, height, 2 );

    {
        g.setColour (colour);
        g.fillPath (outline);
    }
}
